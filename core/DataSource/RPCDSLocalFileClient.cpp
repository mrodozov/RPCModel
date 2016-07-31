#include "RPCDSLocalFileClient.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::property_tree;

RPCDSLocalFileClient::RPCDSLocalFileClient(){
  //this->setData(new DataType);
  this->setRateData(new RPCDSRateData);
}

RPCDSDataType* RPCDSLocalFileClient::getDataForRequest(const string& request){
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
  this->jsonfile = jsonobj;
}

void RPCDSLocalFileClient::setRateData(RPCDSRateData* rd){
}

RPCDSRateData* RPCDSLocalFileClient::getRateDataObjectFrom(RPCDSDataType* data){
  return dynamic_cast<RPCDSRateData*>(data); // get a concrete type 
}



void RPCDSLocalFileClient::getRateDataFromROOTfile(const string& fileName){
  
}

RPCDSLocalFileClient::~RPCDSLocalFileClient(){
}
