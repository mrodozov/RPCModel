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
#include "DataSource.h"
#include "RateData.h"
#include "CurrentData.h"

//#include "../ROOT/ROOT.h"

using namespace std;
using namespace boost::property_tree;


DataType::DataType(){}
DataType::~DataType(){}

DataService::DataService(){}
DataService::~DataService(){this->erase();}

DataSourceBase::DataSourceBase(){}
DataSourceBase::~DataSourceBase(){}

// abstract classes need constr and destructors (at least need virtual destructor that doesn't call the previous)

DataSource::DataSource(){
}

DataType * DataSource::getRateDataForServiceKey( const string & serviceKey){
  return dynamic_cast< RateData* >( this->getDataFromServiceForKey( serviceKey ));
}

DataType* DataSource::getCurrentForRunlist(const vector< string >& runlist){
  return dynamic_cast<CurrentData*>(this->getDataFromServiceForKey(runlist.at(0)));
}

DataType* DataSource::getCurrentForServiceKey(const string& serviceKey){
  return dynamic_cast<CurrentData*>(this->getDataFromServiceForKey(serviceKey));
}

DataType* DataSource::getLumiDataForKey(const string& serviceKey){
  return dynamic_cast<RateData*>(this->getDataFromServiceForKey(serviceKey));
}

DataType* DataSource::getRateForRunlist(const vector< string >& runlist){
  return dynamic_cast<RateData*>(this->getDataFromServiceForKey(runlist.at(0)));
}


DataSource::~DataSource(){  
}