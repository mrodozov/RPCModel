//#include "../DataSourceInterface.h"
#include "RateData.h"

using namespace std;

RateData::RateData(){
}

vector< double > RateData::getRateForRollChannels(const string& RollName){
  vector<double> v;
  return v;
}




ptree * RateData::getDataAsJSON(){
  ptree * t;
  return t;
}


vector< vector< double > > RateData::getFullRateRateforRoll(const string& RollName){
}

ptree * RateData::getJSONdataForKeyInDataAsJSON(const string& key){
  ptree * t;
  return t;
}

RateData::~RateData(){
}
