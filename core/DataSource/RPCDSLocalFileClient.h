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
  string * parseRequest();
  
public:
  
  RPCDSRateData * getRateDataObjectFrom(RPCDSDataType * data); //
  virtual void getDataForRequest(const string & request); // 
  
    RPCDSLocalFileClient();
    virtual ~RPCDSLocalFileClient();
  
};

#endif