#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include "RPCDSDataSourceInterface.h"
#include <boost/property_tree/ptree.hpp>
#include <string.h>

using namespace std;

class RPCDSDataSource : public RPCDSDataSourceBase {

private:

protected:  
  
  
public:
  
  // define methods that for concrete key return concrete type of DataService, using the getServiceForKey method 
  
  virtual RPCDSDataType * getDataFromServiceForKey(const string & serviceKey) { return this->getServiceForKey(serviceKey)->getData();  }  
  //
  virtual RPCDSDataType * getRateDataForServiceKey( const string & serviceKey);
  
  virtual RPCDSDataType * getCurrentForServiceKey( const string & serviceKey);
  
  virtual RPCDSDataType * getLumiDataForKey(const string & serviceKey);
  
  virtual RPCDSDataType * getRateForRunlist(const vector<string> & runlist);
  
  virtual RPCDSDataType * getCurrentForRunlist(const vector<string> & runlist);
  
  void createServiceForKey(const string & key, RPCDSDataService * s) { this->insertNewServiceForKey(key,s);}
  void deleteServiceForKey(const string & key) { this->removeServiceForKey(key);}
  
  
  // now for each possible service
  
    RPCDSDataSource();
    virtual ~RPCDSDataSource();
  
};



#endif