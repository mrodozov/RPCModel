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
RPCDSDataService::~RPCDSDataService(){}

RPCDSDataSourceBase::RPCDSDataSourceBase(){}
RPCDSDataSourceBase::~RPCDSDataSourceBase(){}

// abstract classes need constr and destructors (at least need virtual destructor that doesn't call the previous)

RPCDSDataSource::RPCDSDataSource(){
}

RPCDSDataSource::~RPCDSDataSource(){  
}