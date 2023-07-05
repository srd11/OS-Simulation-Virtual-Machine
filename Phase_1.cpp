#include<iostream>
#include<vector>
#include<fstream>
#include <string>
using namespace std;
#define ROW 100
#define COL 4

vector<vector<string>> Memory(ROW, vector<string>(COL,"-"));
int c;
vector<string> R(4);
vector<string> IR(4);
// IR [1, 2]: Bytes 1, 2 of IR/Operation Code
// IR [3, 4]: Bytes 3, 4 of IR/Operand Address
int IC {0};
bool C {false};
int SI=1;
string line;
int IC1;
int programEnded=0;


void READ(int);
void WRITE(int);
void LOAD();
void TERMINATE();
void printM();
void STARTEXCEUTION();
void MASTERMODE();
void INIT();

void INIT(){
    for(int i=0;i<ROW;i++){
    for(int j=0;j<COL;j++){
        Memory[i][j]="-";
    }
    }

    IC=0;
    C = false;
    R.clear();
    IR.clear();
    IC1=0;
    SI=1;
    programEnded=0;
}


void MASTERMODE(){
    

    if(SI==1){
        READ(IC1);
    }
    else if(SI==2)
    {
        WRITE(IC1);
    }
    else{
        TERMINATE();
    }

return;
}

void TERMINATE(){

     ofstream fout("output.txt",ios::app);
            while (fout) {
                      fout<<"Program ended";
                      fout <<endl;
                      fout <<endl;
                      fout <<endl;
                      break;
                 }
            fout.close();
    programEnded=1;
    return;
}


void READ(int IC1){
            string s = IR[2];
            string s1 = IR[3];
            s=s+s1;
            int n = stoi(s);
            // int wordLen = words[3].length();
            int wordLen = line.length();
            int c=0;
            bool flag = false;
            
            for(int j=n;j<n+10;j++){
                for(int k=0;k<COL;k++){
                     if(c < wordLen)
                     {
                        Memory[j][k]=line[c++];
                        
                     }
                    else
                     {
                        flag = true;
                        break;
                     }  
                }
              if(flag==true)
              {
                break;
              }
            }

    return;
}



void WRITE(int IC1){
            string s = IR[2];
            string s1 = IR[3];
            s=s+s1;
            int n = stoi(s);
            int c=0;

            bool flag = false;
            string word="";
            vector <string> buffer;

     for(int j=n;j<n+10;j++){
                for(int k=0;k<COL;k++){
                     if(Memory[j][k]!="-")
                     {
                         word+=Memory.at(j).at(k);
                        
                     }
                    else if(Memory[j][k]=="-"){
                        continue;
                    }
                    else
                     {
                        flag = true;
                        break;
                     }  
                }
              if(flag==true)
              {
                break;
              }
            }
            buffer.push_back(word);

    ofstream fout("output.txt",ios::app);
    string line1;

    
    int i=0;
    
    while (fout && i<buffer.size()) {
        fout << buffer[i]<< endl;
        i++;
    }
    fout.close();


return;
}





void STARTEXCEUTION(){
           
int cnt=0;
while(IC1!=IC){
            
            
            if(programEnded==1){
                return;
            }
            int n;
            for(int i=0;i<4;i++){
                IR[i]=Memory[IC1][i];
            }
            if(IR[0]!="H"){
                string s = IR[2];
                string s1 = IR[3];
                s=s+s1;
                n = stoi(s);
            }
            
            if(IR[0]=="G"){
                SI=1;
                cnt++;
                if(cnt>1){
                    return;
                }
                MASTERMODE();
                // return;
            }
            else if(IR[0]=="P"){
                  SI=2;
                  MASTERMODE();
            }
                
            else if(IR[0]=="H"){
                SI=3;
                MASTERMODE();

            }
           else if(IR[0]=="L")
            {
                for(int j=0;j<4;j++){
                    R[j]=Memory[n][j];
                }
           }
            else if (IR[0]=="S")
            {
                 for(int j=0;j<4;j++){
                    Memory[n][j]=R[j];
                 }

            }
            else if(IR[0]=="C")
            {
                for(int j=0;j<4;j++){
                     if(R[j]==Memory[n][j])
                          C=true;
                     else{
                          C=false;
                          break;
                     }

                 }
           
            }
            else if(IR[0]=="B"){
                if(C==true){
                    IC1=n;
                 }
             }
           
   
    if(C==true && IR[0]=="B")
            continue;
    else
        ++IC1;
   
 }   
 return;
 
}



void LOAD(){
    ifstream fin;
    fin.open("input.txt");
    int temp=0;
    string s;
    bool flag = false;
    int count=0;
    int program_card_counter=0;


     while(fin){
         getline(fin, line); 
        int c=0;
 
         if(line[0]=='$'){
            count++;
         }
         else if(count==1){
         int wordLen = line.length();
         c=0;
         flag=false;
            for(int j=program_card_counter*10;j<program_card_counter*10+10;j++){
                for(int k=0;k<COL;k++){
                

                    if(c < wordLen){
                         Memory[j][k]=line[c++];
                    }
                    else
                    {
                    flag = true;
                    break;
                    }  
                }
                IC++;
             if(flag==true)
             {
               
                break;
             }
          }
          program_card_counter++;
         }
         else if(count==2){
            
            STARTEXCEUTION();
         } 
      if(count==3){
             printM();
             INIT();
             count=0;
             program_card_counter=0;
         }
     }
     fin.close();
    
}   

void printM(){
    int cnt = 0;
    int block = 0;
    for(int i=0; i<ROW; i++)
    {   
        if(cnt==10 || cnt == 0)
        {
            cout<<"\nBlock "<< block <<endl;
            block++;
            cnt=0;
        }
        cout<<i<<" =>  ";
        for(int j=0; j<COL; j++)
        {
            cout<<Memory.at(i).at(j)<<"  ";
        }
        cnt++;
        
        cout<<endl;
    }
     cout<<"Pgogram Ended"<<endl<<endl; 
}

int main(){
    ofstream fout;   
    fout.open("output.txt");
    fout<<"";
    fout.close();
    LOAD();
}