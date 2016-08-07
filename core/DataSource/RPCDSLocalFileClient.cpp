#include "RPCDSLocalFileClient.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::property_tree;

RPCDSLocalFileClient::RPCDSLocalFileClient(){
}

void RPCDSLocalFileClient::getDataForRequest(const string& request){
  if(request.find(".json") != std::string::npos) this->getRateDataFromJSONfile(request);
  
  //return  this->getRateData();
}


// to remove completely InputConfig

void RPCDSLocalFileClient::getRateDataFromJSONfile(const string& fileName){
  ifstream ifs(fileName.c_str());
  ptree * jsonobj = new ptree;
  boost::property_tree::read_json(ifs,*jsonobj);
  ifs.clear(); ifs.close();
  cout << "json read" << endl;
  cout << "blabla" << endl;
  RPCDSRateData * rdata = new RPCDSRateData;
  rdata->setRateFromSource(jsonobj);
  this->put(rdata);
}


RPCDSRateData* RPCDSLocalFileClient::getRateDataObjectFrom(RPCDSDataType* data){
  return dynamic_cast<RPCDSRateData*>(data); // get a concrete type 
}



void RPCDSLocalFileClient::getRateDataFromROOTfile(const string& fileName){
  
}

RPCDSLocalFileClient::~RPCDSLocalFileClient(){
}
