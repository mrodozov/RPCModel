#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
/** non standart */
#include "DataObject.h"
#include "get_avg.h"


void get_average_for_list_of_parts(string listOfParts,string inputDBfile, string outputFile){
  
  DataObject partsList(listOfParts);
  DataObject inputFile(inputDBfile);
  ofstream OFS;
  OFS.open(outputFile.c_str());
  double currentRate;
  int counterOfRolls;
  string currentPartName;
  
  for(int i = 1 ; i <= partsList.getLenght() ; i ++){
    
    currentRate = 0 ; counterOfRolls = 0;
    currentPartName = partsList.getElement(i,1);
    
    for (int j = 1 ; j <= inputFile.getLenght();j++){
      if(inputFile.getElement(j,1).find(currentPartName) != std::string::npos && inputFile.getElementAsDouble(j,7) >= 0) {
	currentRate += inputFile.getElementAsDouble(j,7);
	counterOfRolls ++;
      }
    }
    
    OFS << currentPartName << " " << currentRate/counterOfRolls  << " Hz/cm^2 <br>"  << endl;
    
    //cout << currentPartName << " " << currentRate/counterOfRolls << " " << currentRate << " " << counterOfRolls << '\n'; // keep for debug
    OFS.clear();
  }
  
  OFS.close();
}
