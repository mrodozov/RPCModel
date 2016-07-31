#ifndef CURRENT_DATA_H
#define CURRENT_DATA_H

#include <string>
#include "DataSourceInterface.h"

using namespace std;

class CurrentData : public DataType {
  
  // one run current, roll-current
  map<string, vector<double> >  averageCurrentPerRoll;
  // runlist
  map<string, vector<vector <double> > > fullCurrentPerRoll;
  
  
public:
  
  virtual ptree * getDataAsJSON();
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key);
  
  
  CurrentData();
  virtual ~CurrentData();
    
};


#endif