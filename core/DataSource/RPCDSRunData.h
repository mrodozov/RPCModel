#ifndef RUN_DATA_H
#define RUN_DATA_H

#include "RPCDSDataSourceInterface.h"
#include <boost/property_tree/ptree.hpp>

class RPCDSRunData : public RPCDSDataType {
  
private:
  
  virtual ptree* getJSONdataForKeyInDataAsJSON(const string & key);
  
};


#endif