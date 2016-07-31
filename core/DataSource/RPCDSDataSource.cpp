#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <map>
#include "RPCDSDataSource.h"
#include "RPCDSRateData.h"
#include "RPCDSCurrentData.h"

//#include "../ROOT/ROOT.h"

using namespace std;
using namespace boost::property_tree;


RPCDSDataType::RPCDSDataType(){}
RPCDSDataType::~RPCDSDataType(){}

RPCDSDataService::RPCDSDataService(){}
RPCDSDataService::~RPCDSDataService(){this->erase();}

RPCDSDataSourceBase::RPCDSDataSourceBase(){}
RPCDSDataSourceBase::~RPCDSDataSourceBase(){}

// abstract classes need constr and destructors (at least need virtual destructor that doesn't call the previous)

RPCDSDataSource::RPCDSDataSource(){
}

RPCDSDataType * RPCDSDataSource::getRateDataForServiceKey( const string & serviceKey){
  return dynamic_cast< RPCDSRateData* >( this->getDataFromServiceForKey( serviceKey ));
}

RPCDSDataType* RPCDSDataSource::getCurrentForRunlist(const vector< string >& runlist){
  return dynamic_cast<RPCDSCurrentData*>(this->getDataFromServiceForKey(runlist.at(0)));
}

RPCDSDataType* RPCDSDataSource::getCurrentForServiceKey(const string& serviceKey){
  return dynamic_cast<RPCDSCurrentData*>(this->getDataFromServiceForKey(serviceKey));
}

RPCDSDataType* RPCDSDataSource::getLumiDataForKey(const string& serviceKey){
  return dynamic_cast<RPCDSRateData*>(this->getDataFromServiceForKey(serviceKey));
}

RPCDSDataType* RPCDSDataSource::getRateForRunlist(const vector< string >& runlist){
  return dynamic_cast<RPCDSRateData*>(this->getDataFromServiceForKey(runlist.at(0)));
}


RPCDSDataSource::~RPCDSDataSource(){  
}