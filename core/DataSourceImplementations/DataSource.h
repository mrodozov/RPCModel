#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include "DataSourceInterface.h"
#include <boost/property_tree/ptree.hpp>
#include <string.h>

using namespace std;

class DataSource : public DataSourceBase {

private:

protected:  
  
  
public:
  
  // define methods that for concrete key return concrete type of DataService, using the getServiceForKey method 
  
  virtual DataType * getDataFromServiceForKey(const string & serviceKey) { return this->getServiceForKey(serviceKey)->getData();  }  
  //
  virtual DataType * getRateDataForServiceKey( const string & serviceKey);
  
  virtual DataType * getCurrentForServiceKey( const string & serviceKey);
  
  virtual DataType * getLumiDataForKey(const string & serviceKey);
  
  virtual DataType * getRateForRunlist(const vector<string> & runlist);
  
  virtual DataType * getCurrentForRunlist(const vector<string> & runlist);
  
  void createServiceForKey(const string & key, DataService * s) { this->insertNewServiceForKey(key,s);}
  void deleteServiceForKey(const string & key) { this->removeServiceForKey(key);}
  
  
  // now for each possible service
  
    DataSource();
    virtual ~DataSource();
  
};



#endif