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
  
  
  void createServiceForKey(const string & key, RPCDSDataService * s) { this->insertNewServiceForKey(key,s);}
  void deleteServiceForKey(const string & key) { this->removeServiceForKey(key);}
  
  // now for each possible service

  RPCDSDataSource();
  virtual ~RPCDSDataSource();
  
};



#endif