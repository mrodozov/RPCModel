//#include "../DataSourceInterface.h"
#include "RPCDSRateData.h"

using namespace std;

RPCDSRateData::RPCDSRateData(){
}

vector< double > RPCDSRateData::getRateForRollChannels(const string& RollName){
  vector<double> v;
  return v;
}




ptree * RPCDSRateData::getDataAsJSON(){
  ptree * t;
  return t;
}


vector< vector< double > > RPCDSRateData::getFullRateRateforRoll(const string& RollName){
}

ptree * RPCDSRateData::getJSONdataForKeyInDataAsJSON(const string& key){
  ptree * t;
  return t;
}

void RPCDSRateData::setRateFromSource(ptree * jsonData){
  this->json = jsonData;
}


RPCDSRateData::~RPCDSRateData(){
}

