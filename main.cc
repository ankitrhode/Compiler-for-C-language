#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;
extern void  print_info();
extern bool err;
extern vector<string> printstr;
extern string curr_code_func;
extern map <string, GlobalSymbolTableEntry *> globalSymbolTable;
int main (int argc, char** argv)
{
  Parser parser;
  parser.parse();
  // print_info();

  if(!err){

    cout<<"\n";
    printf(".data\n");
    for(int i=0;i<printstr.size();i++)
      {printf("s%d:\t.asciiz %s\n",i,printstr[i].c_str());}
    printf(".text\n");
    printf("mtc1 $0 $f2\n");

    int curr = 0;

    for(int i = 0 ; i < globalSymbolTable.size() ; i++) {
      map <string, GlobalSymbolTableEntry *> :: iterator it = globalSymbolTable.begin();
      for(; it!= globalSymbolTable.end() ; it++) {
        if(it -> second -> cnt == curr) {
          if(it->second->ast==NULL){}
            else{
            //start generating code now
            
            curr_code_func = it->first;
            cout<< it->first + ":\n";

            map < string, LocalSymbolTableEntry * > :: iterator it1 = (it->second->symtab).begin();
            int local_var_size=0;

            for(;it1!=(it->second->symtab).end();it1++)
              {
                if( !it1->second->isParameter)
                  local_var_size+= it1->second->sz;
              }

            printf("addi $sp $sp -4\nsw $ra 0($sp)\naddi $sp $sp -4\nsw $fp 0($sp)\nmove $fp $sp\naddi $sp $sp %d\n\n",-(local_var_size));
            
            (it->second->ast)->generate_code();
            
            printf("addi $sp $sp %d\nlw $fp 0($sp)\naddi $sp $sp 4\nlw $ra 0($sp)\naddi $sp $sp 4\njr $ra\n",(local_var_size));
          
          }
          curr++;
          break;
        }
      }
      cout<<endl;      
    }
  }
}



