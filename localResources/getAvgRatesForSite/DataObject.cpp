#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "DataObject.h"
#include <cstdlib>
#include <stdlib.h>

using namespace std;

DataObject::DataObject(std::string fileName, int columns){
  this->getDataFromFile(fileName,columns);
}

DataObject::DataObject(std::string fileName){
  ifstream IFS;
  int columns=0;
  stringstream buff;
  string LINE,token;
  IFS.open(fileName.c_str());
  getline(IFS,LINE);
  buff.str(LINE);
  while(buff >> token){
    columns++;
  }
  IFS.clear();
  IFS.close();
  this->getDataFromFile(fileName,columns);
}

DataObject::DataObject(){
  
}

void DataObject::getDataFromFile(string fileName,int columns){
  
  
  ifstream IFS;
  string key,token;
  stringstream buffer;
  IFS.open(fileName.c_str());
  string LINE;
  int argumentCounter = 0;
  int matrixIndex = 1;
  
  while(getline(IFS,LINE)){
    string * aRow;
    buffer.str(LINE);
    if(columns == 1){
      aRow = new string;
    }
    else{
      aRow = new string[columns-1];
    }
    
    argumentCounter = 0;
    while(buffer >> token && argumentCounter < columns ){
      if(columns == 1){
	key = token;
	*aRow = token;
      }
      else{
	if(argumentCounter == 0){
	  key = token;
	}
	else {
	  aRow[argumentCounter-1] = token;
	}
	argumentCounter++;
      }
    }
    
    map<string,string*> aBuffer ;
    this->KeysValuesMap[key] = aRow;
    aBuffer[key] = aRow;
    this->ElementsAsMatrixMap[matrixIndex] = aBuffer;
    matrixIndex++;
    buffer.clear();
  }
  IFS.clear();
  IFS.close();
  
}


void DataObject::getDataFromDB(){
  
}


string DataObject::getElementFromPropertyContainer(int rowIndex,int columnIndex){
  string returnValue;
  
  if(columnIndex == 1){
    returnValue = this->ElementsAsMatrixMap.find(rowIndex)->second.begin()->first;
  }
  else{
    returnValue = this->ElementsAsMatrixMap.find(rowIndex)->second.begin()->second[columnIndex-2];
  }
  
  return returnValue;
  
}

string DataObject::getElementByKey(string Key, int Element){
  return this->KeysValuesMap.find(Key)->second[Element-1];
}

int DataObject::getElementByKeyAsInt(string Key, int Element){
  return atoi(this->KeysValuesMap.find(Key)->second[Element-1].c_str());
}

double DataObject::getElementByKeyAsDouble(string Key, int Element){
  return atof(this->KeysValuesMap.find(Key)->second[Element-1].c_str());
}

string DataObject::getElement(int Row,int Column){
  return this->getElementFromPropertyContainer(Row,Column);
}

int DataObject::getElementAsInt(int Row,int Column){
  return atoi(getElementFromPropertyContainer(Row,Column).c_str());
}

double DataObject::getElementAsDouble(int Row,int Column){
  return atof(getElementFromPropertyContainer(Row,Column).c_str());
}

int DataObject::getLenght(){
  return this->ElementsAsMatrixMap.size();
}



DataObject::~DataObject(){
  this->KeysValuesMap.clear();
  this->ElementsAsMatrixMap.clear();
}

