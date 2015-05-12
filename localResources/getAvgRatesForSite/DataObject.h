#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <string>
#include <map>

using namespace std;

class DataObject{
  
  map<string,string*> KeysValuesMap;
  map<int,map<string,string*> > ElementsAsMatrixMap;
  
  
 public:
  
  std::string getElementFromPropertyContainer(int Row,int Column);
  std::string getElement(int Row,int Column);
  int getElementAsInt(int Row,int Column);
  double getElementAsDouble(int Row,int Column);
  string getElementByKey(string Key,int Element);
  int getElementByKeyAsInt(string Key,int Element);
  double getElementByKeyAsDouble(string Key,int Element);
  void getDataFromFile(std::string,int Columns);
  int getLenght();
  
  virtual void getDataFromDB();
  
  DataObject(string fileName,int columns); // this constructor will be used both because of history issues and because it could restrict the number of columns from file to be used
  DataObject(string fileName); // this constructor have to find out how many rows the file has
  DataObject();
  ~DataObject();
  
};

#endif