#ifndef RATE_DATA_H
#define RATE_DATA_H

#include <string>
#include "DataSourceInterface.h"

using namespace std;

class RateData : public DataType {
  
  // one run rate
  map<string, vector<double> >  averageRatePerRoll;
  map<string, vector<vector <double> > > fullRatePerRoll;
  
protected:
  
  
public:
  
  map<string, vector<double> > getAverageRateAllRolls() const { return this->averageRatePerRoll;}
  map<string, vector<vector< double > > > getFullAverageRateAllRolls() const { return this->fullRatePerRoll;}
  vector<double> getRateForRollChannels(const string & RollName);
  vector<vector<double>> getFullRateRateforRoll(const string & RollName);
  
  virtual ptree * getDataAsJSON();
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key);
  
  
  RateData();
  virtual ~RateData();
    
};


#endif