#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>

#include "../../../core/DataObject.h"

using namespace std;

int main (int argc,char * argv[]){
  
  ofstream OFS;
  DataObject input(argv[1],4);
  
  
  OFS.open(argv[2]);
  
  for (int i=1 ; i <= input.getLenght() ; i++){
    OFS << input.getElement(i,1) << " " << input.getElement(i,2)<<" ";
    if(input.getElement(i,4) == "pb"){
      OFS << input.getElementAsDouble(i,3)*1000000 <<"\n";
    }
    if(input.getElement(i,4) == "nb"){
      OFS << input.getElementAsDouble(i,3)*1000 <<"\n";
    }
    
  }
  
  
  
  return 0;
  
}