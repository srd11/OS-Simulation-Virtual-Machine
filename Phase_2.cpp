#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define ROW 300
#define COL 4
vector<vector<string>> Memory(ROW, vector<string>(COL, ""));
vector<string> R(4), IR(4), words{}, PCB(12);
char value = ' ';
int IC{00};
bool C{false};
int  SI = 3, TI = 0, PI = 0, PTE, PTR, PID, TTC = 0, LLC = 0, TTL = 0,
    TLL = 0, RA, program_card_counter, programEnded = 0, frame_no, IC_cnt;
string line;
vector<int> randomno;
int IC1{00};
void READ(), WRITE(), LOAD(), TERMINATE(int), STARTEXECUTION(), KERNALMODE(),
    PRINT_MEMORY(), PCB_Initilization(), get_RA(), WRITE_TERMINATE(int),
    PROGRAM_PAGE_NO(), INIT();
int ALLOCATE(), ADDRESS_MAP(int), GET_VALID_PAGE_NO();
void PRINT() {
  // IC : 11
  // IR : PD50
  // TTC : 10
  // LLC :
  IC_cnt = IC1 + 1;
  if (TTC > TTL) {
    IC_cnt--;
  }
  ofstream fout("output.txt", ios::app);
  fout << endl;
  fout << "Job ID :" << PID << endl;
  fout << "IC : " << IC_cnt << endl;
  fout << "IR : ";
  for (int i = 0; i < 4; i++) {
    fout << IR[i];
  }
  fout << endl;
  fout << "TTC : " << TTC << endl;
  fout << "LLC : " << LLC << endl;
  fout.close();
}
void INIT() {
  R.clear();
  IR.clear();
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      Memory[i][j] = "";
    }
  }
  char value = ' ';
  for(int i=0;i<4;i++){
    IR[i]={""};
  }
//   IR = {""};
  R={" "};
  IC = 0, SI = 3, TI = 0, PI = 0;
  C = false;
  PTE = 0, PTR = 0;
  TTC = 0, LLC = 0, TTL = 0, TLL = 0;
  program_card_counter = 0;
  programEnded = 0;
  words.clear();
  line = "";
  PCB.clear();
  randomno.clear();
  IC1 = 0, IC_cnt = 0;
  fill(R.begin(), R.end(), value);
}

int getUniqueNo() {
  int uniqueNo = 0;
  int flag = 0;
  do {
    uniqueNo = ALLOCATE();
    flag = 0;
    for (int i = 0; i < randomno.size(); i++) {
      if (uniqueNo == randomno[i]) {
        flag = 1;
        break;
      }
    }
  } while (flag == 1);
  return uniqueNo;
}
void PROGRAM_PAGE_NO() {
  frame_no = getUniqueNo();
  randomno.push_back(frame_no);
  int num = frame_no;
  if (num > 9) {
    int n1 = num % 10;
    num = num / 10;
    string s = to_string(n1);
    Memory[PTR + program_card_counter][3] = s;
    n1 = num % 10;
    s = to_string(n1);
    Memory[PTR + program_card_counter][2] = s;
  } else {
    Memory[PTR + program_card_counter][2] = to_string(0);
    Memory[PTR + program_card_counter][3] = to_string(num);
  }
  frame_no = frame_no * 10;
}
void WRITE_TERMINATE(int n) {
  ofstream fout("output.txt", ios::app);
  string buffer;
  int l;
  switch (n) {
  case 0:
    buffer = "No error";
    l = 0;
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    PRINT_MEMORY();
    break;
  case 1:
    buffer = "Out of Data";
    l = 0;
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  case 2:
    buffer = "Line Limit Exceeded";
    l = 0;
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  case 3:
    l = 0;
    buffer = "Time Limit Exceeded";
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  case 4:
    buffer = "Operation Code Error";
    l = 0;
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  case 5:
    buffer = "Operand Error";
    l = 0;
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  case 6:
    l = 0;
    buffer = "Invalid Page Fault";
    while (fout && l < buffer.size()) {
      fout << buffer[l++];
    }
    fout.close();
    PRINT();
    break;
  }
  programEnded = 1;
  return;
}
void PCB_Initilization() {
  string _pid, _ttl, _tll;
  for (int i = 0; i < 4; i++) {
    _pid = _pid + line[i + 4];  // 1 * 4
    _ttl = _ttl + line[i + 8];  // 2 * 4
    _tll = _tll + line[i + 12]; // 3 * 8
  }
  TTL = stoi(_ttl);
  TLL = stoi(_tll);
  PID = stoi(_pid);
}
void get_RA() {
  string s = IR[2] + IR[3];
  if (!isdigit(s[0]) || !isdigit(s[1])) {
    PI = 2, TI = 0;
    KERNALMODE();
  }
  RA = ADDRESS_MAP(stoi(s));
}
int GET_VALID_PAGE_NO() {
  int FN = getUniqueNo(), num = FN;
  randomno.push_back(FN);
  if (FN > 9) {
    int n1 = FN % 10;
    FN = FN / 10;
    string s = to_string(n1);
    Memory[PTE][3] = s;
    n1 = FN % 10;
    s = to_string(n1);
    Memory[PTE][2] = s;
  } else {
    Memory[PTE][2] = to_string(0);
    Memory[PTE][3] = to_string(FN);
  }
  return num;
}
int ALLOCATE()
 { 
  return rand() % 30; //to generate random number
 }
int ADDRESS_MAP(int VA) {
  if (VA < 0 && VA > 99) {
    PI = 2;
    TERMINATE(5);
  }
  int num = 0;
  PTE = PTR + (VA / 10);
  string s = Memory[PTE][2] + Memory[PTE][3];
  if (s == "**") {
    num = GET_VALID_PAGE_NO();
    RA = num * 10 + (VA % 10);
    return RA;
  }
  if (!isdigit(s[0])) {
    PI = 2;
    KERNALMODE();
  }
  return stoi(s) * 10 + (VA % 10);
}
void KERNALMODE() {
  if (TTC > TTL) {
    TI = 2, SI = 1;
  }

  if (programEnded == 1)
    return;
  if (TI == 0) {
    if (SI == 1)
      READ();
    else if (SI == 2)
      WRITE();
    else if (SI == 3)
      TERMINATE(0);
  }
  if (TI == 2) {
    if (SI == 1)
      TERMINATE(3);
    else if (SI == 2) {
      WRITE();
      TERMINATE(3);
    } else if (SI == 3)
      TERMINATE(0);
  }
  if (TI == 2) {
    if (PI == 1) {
      TERMINATE(4);
    } else if (PI == 2) {
      TERMINATE(5);
    } else if (PI == 3)
      TERMINATE(3);
  }

  if (TI == 0) {
    if (PI == 1)
      TERMINATE(4);
    else if (PI == 2)
      TERMINATE(5);
    else if (PI == 3)
      TERMINATE(6);
  }
  if (programEnded == 1)
    return;
}
void TERMINATE(int n) {
  switch (n) {
  case 0:
    cout << "No Error";
    WRITE_TERMINATE(0);
    break;
  case 1:
    cout << "Out of Data" << endl;
    WRITE_TERMINATE(1);
    break;
  case 2:
    cout << "Line limit Exceeded" << endl;
    WRITE_TERMINATE(2);
    break;
  case 3:
    cout << "Time Limit Exceeded" << endl;
    WRITE_TERMINATE(3);
    break;
  case 4:
    cout << "Operation Code Error" << endl;
    WRITE_TERMINATE(4);
    break;
  case 5:
    cout << "Operand Error" << endl;
    WRITE_TERMINATE(5);
    break;
  case 6:
    cout << "Invalid Page Fault" << endl;
    WRITE_TERMINATE(6);
    break;
  }
}
void READ() {
  int wordLen = line.length();
  if (line[0] == '$' && line[1] == 'E') {
    TERMINATE(1);
  }
  int c = 0;
  for (int j = RA; j < RA + 10; j++) {
    for (int k = 0; k < COL; k++) {
      if (c < wordLen) {
        Memory[j][k] = line[c++];
      } else {
        return;
      }
    }
  }
}
void WRITE() {
  if (LLC >= TLL)
    TERMINATE(2);
  else {
    string word = "";
    vector<string> buffer;
    for (int j = RA; j < RA + 10; j++) {
      for (int k = 0; k < COL; k++) {
        word += Memory[j][k];
      }
    }
    buffer.push_back(word);
    ofstream fout("output.txt", ios::app);
    for (int i = 0; fout && i < buffer.size(); i++) {
      fout << buffer[i] << endl;
      i++;
    }
    fout.close();
    LLC++;
  }

  return;
}
void STARTEXECUTION() {
  int cnt = 0;
  while (IC1 != IC) {
    if (TTC > TTL && programEnded == 0) {
      TI = 2;
      SI = 1;
      KERNALMODE();
    }
    if (programEnded == 1) {
      return;
    }

    RA = ADDRESS_MAP(IC1);
    for (int i = 0; i < 4; i++) {
      IR[i] = Memory[RA][i];
    }

    if (IR[0] == "G" && IR[1] == "D") {
      get_RA();
      SI = 1;
      cnt++;
      if (cnt > 1) {
        return;
      }
      TTC = TTC + 2;
      KERNALMODE();
    } else if (IR[0] == "P" && IR[1] == "D") {
      SI = 2;
      get_RA();
      TTC++;
      KERNALMODE();
    } else if (IR[0] == "H") {
      TTC++;
      SI = 3;
      KERNALMODE();
    } else if (IR[0] == "L" && IR[1] == "R") {
      get_RA();
      if (Memory[RA][0] == "" && Memory[RA][1] == "" && Memory[RA][2] == "" &&
          Memory[RA][3] == "") {
        PI = 3;
        TERMINATE(6);
      }
      for (int j = 0; j < 4; j++) {
        R[j] = Memory[RA][j];
      }
      TTC++;
    } else if (IR[0] == "S" && IR[1] == "R") {
      SI = 1;
      get_RA();
      if (R[0] == " " && R[1] == " " && R[2] == " " && R[3] == " ") {
        PI = 3;
        TERMINATE(6);
      } else {
        for (int j = 0; j < 4; j++) {
          Memory[RA][j] = R[j];
        }
        TTC = TTC + 2;
      }
    } else if (IR[0] == "C" && IR[1] == "R") {
      get_RA();
      for (int j = 0; j < 4; j++) {
        if (R[j] == Memory[RA][j])
          C = true;
        else {
          C = false;
          break;
        }
      }
      TTC++;
    } else if (IR[0] == "B" && IR[1] == "T") {
      string s = Memory[RA][2];
      string s1 = Memory[RA][3];
      s = s + s1;
      int n = stoi(s);
      if (C == true) {
        IC1 = n;
      }
    } else {
      PI = 1;
      SI = 0;
      TTC++;
      KERNALMODE();
    }

    if (C == true && IR[0] == "B")
      continue;
    else
      ++IC1;
  }
  return;
}
void LOAD() {
  ifstream fin;
  fin.open("input.txt");
  string s;
  bool flag = false;
  int count = 0;
  while (fin) {
    getline(fin, line);
    int c = 0;
    if (line[0] == '$' && line[1] == 'A') {
      PCB_Initilization();
      PTE = ALLOCATE();
      randomno.push_back(PTE);
      PTR = PTE * 10;
      for (int j = PTR; j < PTR + 10; j++) {
        for (int k = 0; k < COL; k++) {
          Memory[j][k] = '*';
        }
      }
      count++;
    } else if (line[0] == '$' && line[1] == 'E') {
    
      if(IR[0]=="" || IR[0]=="G" && IR[1]=="D" && programEnded==0){
                TERMINATE(1);
      }
      cout << "Program Ended" << endl;
      ofstream fout("output.txt", ios::app);
      while (fout) {
        fout << endl;
        fout << endl;
        break;
      }
      fout.close();
      INIT();
      count = 0;
    } else if (line[0] == '$' && line[1] == 'D') {
      count++;
    } else if (count == 2) {
      STARTEXECUTION();
    } else if (count == 1) {
      int wordLen = line.length();
      c = 0;
      flag = false;
      PROGRAM_PAGE_NO();
      for (int j = frame_no; j < ROW; j++) {
        for (int k = 0; k < COL; k++) {
          if (c < wordLen) {
            Memory[j][k] = line[c++];
          } else {
            flag = true;
            break;
          }
        }
        IC++;
        if (flag == true) {
          break;
        }
      }
      program_card_counter++;
    }
  }
  fin.close();
}
void PRINT_MEMORY() {
  int count = 0, block = 0;
  for (int i = 0; i < ROW; i++) {
    if (count == 10 || count == 0) {
      cout << "\nBlock " << block << endl;
      block++;
      count = 0;
    }
    cout << i << " => ";
    for (int j = 0; j < COL; j++) {
      cout << Memory.at(i).at(j) << " ";
    }
    count++;
    cout << endl;
  }
}
int main() {
  fill(R.begin(), R.end(), value);
  ofstream fout;
  fout.open("output.txt");
  fout << "";
  fout.close();
  LOAD();
}