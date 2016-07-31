#ifndef DATA_SOURCE_INTERFACE_H
#define DATA_SOURCE_INTERFACE_H

#include <string>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

class RPCDSDataType {
  
private:
  
  //std::string type;
  
public:
  
  //virtual std::string getType()=0;
  virtual ptree * getDataAsJSON()=0;
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key)=0;
  //virtual RPCDSDataType * getRPCDSDataForKey(const string & key)=0;
  RPCDSDataType();
  virtual ~RPCDSDataType();
  
};


class RPCDSDataService {
  
private:
  
  RPCDSDataType * data;
  
protected:
  // clear data
  virtual void erase(){delete this->data;}
public:
  
  //virtual RPCDSDataType * getRPCDSDataForConfigDetails(InputConfig * config){ };
  virtual RPCDSDataType * getDataForRequest(const string & request)=0; //
  virtual void setData(RPCDSDataType * d){ this->data = d;}
  virtual RPCDSDataType * getData(){return this->data;}
  
  // virtual RPCDSDataService * getNext ()=0; //
  
  virtual ~RPCDSDataService();
  RPCDSDataService();
  
};


class RPCDSDataSourceBase {
  
  map<string, RPCDSDataService*> servicesMap;
  
protected:
  
  virtual void insertNewServiceForKey(const string & key,RPCDSDataService * service){this->servicesMap[key] = service;}
  virtual void removeServiceForKey(const string & key){ delete this->servicesMap.at(key) ; servicesMap.at(key) = nullptr; }
  
public:  
  
  virtual RPCDSDataService * getServiceForKey(const string & key) { return this->servicesMap.at(key);}
  virtual RPCDSDataType * getDataFromServiceForKey(const string & serviceKey)=0;
  
  virtual ~RPCDSDataSourceBase();
  RPCDSDataSourceBase();
  
};





#endif