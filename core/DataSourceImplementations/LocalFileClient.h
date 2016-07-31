#ifndef LOCAL_FILE_CLIENT_H
#define LOCAL_FILE_CLIENT_H

#include "DataSourceInterface.h"
#include "RateData.h"
#include "CurrentData.h"
//#include "../../ROOT/ROOT.h"

class LocalFileClient : public DataService {
  
protected:
  
  
  
protected:
  
  void getRateDataFromJSONfile(const string & fileName);
  void getRateDataFromROOTfile(const string & fileName);
  
  
public:
  void setRateData(RateData * rd);
  
  RateData * getRateData();
  virtual RateData * getDataForRequest(const string & request); // 
  //virtual CurrentData * getDataForRequest(const string & request); //
  
  LocalFileClient();
  virtual ~LocalFileClient();
  
};

#endif