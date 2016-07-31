#include "LocalFileClient.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::property_tree;

LocalFileClient::LocalFileClient(){
  //this->setData(new DataType);
  this->setRateData(new RateData);
}

RateData* LocalFileClient::getDataForRequest(const string& request){
  if(request.find(".json") != std::string::npos) this->getRateDataFromJSONfile(request);
  return  this->getRateData();
}


// to remove completely InputConfig

void LocalFileClient::getRateDataFromJSONfile(const string& fileName){
  ifstream ifs(fileName.c_str());
  ptree * jsonobj = new ptree;
  boost::property_tree::read_json(ifs,*jsonobj);
  ifs.clear(); ifs.close();
  cout << "json read" << endl;
  cout << "blabla" << endl;
}

void LocalFileClient::setRateData(RateData* rd)
{

}

RateData* LocalFileClient::getRateData()
{

}


void LocalFileClient::getRateDataFromROOTfile(const string& fileName){
  
}

LocalFileClient::~LocalFileClient(){
}
