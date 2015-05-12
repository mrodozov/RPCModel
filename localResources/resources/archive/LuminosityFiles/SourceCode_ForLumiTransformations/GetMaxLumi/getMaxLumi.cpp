#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <map>

using namespace std;

int main (int argc,char * argv[]){
  
  ifstream IFS;
  IFS.open(argv[1]);
  string LINE;
  istringstream iss;
  string run,runWithThisLumi;
  double time=23.357;
  double LS;
  double DEL_LUMI,result;
  double current = 0 ;
  
  map <double,string> theMap;
  map <double,string>::iterator theIter;
  map <string,string> GOOD_RUNS;

  ifstream IFS2;
  IFS2.open(argv[2]);
  
  while(getline(IFS2,LINE)){
    GOOD_RUNS[LINE] = LINE;
    IFS2.clear();
  }
  

  IFS2.close();
  
  while (getline(IFS,LINE)){
    iss.str(LINE);
    iss >> run >> LS >> DEL_LUMI;
    result = (DEL_LUMI/LS)/time;
    
    theMap[result]=run;
              
    if(result > current && GOOD_RUNS.find(run) != GOOD_RUNS.end()){
      current = result;
    }
    
    
    iss.clear();
  }
  
  cout <<  theMap.find(current)->second <<" " <<current<< endl;
  
  return 0;
}
