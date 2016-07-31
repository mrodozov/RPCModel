#ifndef DATA_SOURCE_INTERFACE_H
#define DATA_SOURCE_INTERFACE_H

#include <string>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

class DataType {
  
private:
  
  //std::string type;
  
public:
  
  //virtual std::string getType()=0;
  virtual ptree * getDataAsJSON()=0;
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key)=0;
  //virtual DataType * getDataForKey(const string & key)=0;
  DataType();
  virtual ~DataType();
  
};


class DataService {
  
private:
  
  DataType * data;
  
protected:
  // clear data
  virtual void erase(){delete this->data;}
public:
  
  //virtual DataType * getDataForConfigDetails(InputConfig * config){ };
  virtual DataType * getDataForRequest(const string & request)=0; //
  virtual void setData(DataType * d){ this->data = d;}
  virtual DataType * getData(){return this->data;}
  
  // virtual DataService * getNext ()=0; //
  
  virtual ~DataService();
  DataService();
  
};


class DataSourceBase {
  
  map<string, DataService*> servicesMap;
  
protected:
  
  virtual void insertNewServiceForKey(const string & key,DataService * service){this->servicesMap[key] = service;}
  virtual void removeServiceForKey(const string & key){ delete this->servicesMap.at(key) ; servicesMap.at(key) = nullptr; }
  
public:  
  
  virtual DataService * getServiceForKey(const string & key) { return this->servicesMap.at(key);}
  virtual DataType * getDataFromServiceForKey(const string & serviceKey)=0;
  
  virtual ~DataSourceBase();
  DataSourceBase();
  
};





#endif