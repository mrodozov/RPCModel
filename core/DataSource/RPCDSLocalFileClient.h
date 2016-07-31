#ifndef LOCAL_FILE_CLIENT_H
#define LOCAL_FILE_CLIENT_H

#include "RPCDSDataSourceInterface.h"
#include "RPCDSRateData.h"
#include "RPCDSCurrentData.h"
//#include "../../ROOT/ROOT.h"

class RPCDSLocalFileClient : public RPCDSDataService {
  
protected:  
  
protected:
  
  void getRateDataFromJSONfile(const string & fileName);
  void getRateDataFromROOTfile(const string & fileName);
  
  
public:
  ptree * jsonfile;
  void setRateData(RPCDSRateData * rd);
  
  RPCDSRateData * getRateDataObjectFrom(RPCDSDataType * data); //
  virtual RPCDSDataType * getDataForRequest(const string & request); // 
  
    RPCDSLocalFileClient();
    virtual ~RPCDSLocalFileClient();
  
};

#endif