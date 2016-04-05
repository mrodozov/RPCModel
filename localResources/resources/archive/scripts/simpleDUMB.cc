#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>

/** non-standart headers , the model , the wrappers and ROOT */

#include "/home/rodozov/Projects/CERN/TranslationModel/ROOT/ROOT.h"

using namespace std;

int main (int argc,char * argv[]){
  
  ifstream IFS;
  IFS.open(argv[1]);
  string LINE;
  
  while(getline(IFS,LINE)){
    TFile * f = new TFile(LINE.c_str(),"READ");
    TH1F * h1;
    TIter iter(f->GetListOfKeys());
    TKey *key;
    TObject * obj;
    while(key = (TKey*)iter()){
      TCanvas * can;
      obj= key->ReadObj();
      can = (TCanvas*)obj;
      //cout << obj->IsA()->Class_Name() << endl;
      obj->SaveAs((LINE.substr(0,(LINE.size() - 5))+".eps").c_str());
      obj->SaveAs((LINE.substr(0,(LINE.size() - 5))+".gif").c_str());
      can->SaveAs((LINE.substr(0,(LINE.size() - 5))+".png").c_str());
//       can->Draw();
    }
    IFS.clear();
    
  }
  IFS.close();
  
//   while (1){
//     sleep(200);
//   }
  
  return 0;
}
