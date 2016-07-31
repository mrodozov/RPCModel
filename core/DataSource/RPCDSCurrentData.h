#ifndef CURRENT_DATA_H
#define CURRENT_DATA_H

#include <string>
#include "RPCDSDataSourceInterface.h"

using namespace std;

class RPCDSCurrentData : public RPCDSDataType {
  
  // one run current, roll-current
  map<string, vector<double> >  averageCurrentPerRoll;
  // runlist
  map<string, vector<vector <double> > > fullCurrentPerRoll;
  
  
public:
  
  virtual ptree * getDataAsJSON();
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key);
  
  
  RPCDSCurrentData();
  virtual ~RPCDSCurrentData();
    
};


#endif