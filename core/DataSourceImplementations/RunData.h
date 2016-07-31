#ifndef RUN_DATA_H
#define RUN_DATA_H

#include "DataSourceInterface.h"
#include <boost/property_tree/ptree.hpp>

class RunData : public DataType {

  
  
private:
  
  virtual ptree* getJSONdataForKeyInDataAsJSON(const string & key);
  
};


#endif