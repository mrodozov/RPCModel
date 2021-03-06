#ifndef DATA_SOURCE_INTERFACE_H
#define DATA_SOURCE_INTERFACE_H

#include <string>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <queue>

using namespace boost::property_tree;
using namespace std;

class RPCDSDataType {
  
private:
  
  //std::string type;
  
public:
  
  //virtual std::string getType()=0;
  virtual ptree * getDataAsJSON()=0;
  virtual ptree * getJSONdataForKeyInDataAsJSON(const string & key)=0;
  //virtual RPCDSDataType * getRPCDSDataForKey(const string & key)=0;
  RPCDSDataType();
  virtual ~RPCDSDataType();
  
};

class RPCDSDataService {
  
private:
  queue<RPCDSDataType*> resultsQueue;
  // counter of all requests 
  
protected:
  
  virtual void put(RPCDSDataType * data){this->resultsQueue.push(data);}
  virtual void requestStarted(const string & request) = 0; // initial setup, specific for each case
  virtual void handleRequest(const string & request) = 0; //  
  virtual void requestFinished(const string & request) = 0; // call back to notify the processing is over ? 
  
public:
  
  virtual RPCDSDataType * get(){RPCDSDataType * r = this->resultsQueue.front(); this->resultsQueue.pop();return r;} // it might cause a problem if pop erases the element
  virtual void getDataForRequest(const string & request){
    this->requestStarted(request);
    this->handleRequest(request); 
    this->requestFinished(request);
  }; // process request, put request result on the queue. define in the three protected functions 
  int getQueueSize() const { return this->resultsQueue.size();}
  
  // virtual RPCDSDataService * getNext ()=0; //
  
  virtual ~RPCDSDataService();
  RPCDSDataService();
  
};


/**
 * DataSource is the black box to have simple public interface for requesting data
 * and wait for the result on the result queue from the client side.
 * The data processing after the request is sent are handled by the DataServices
 */


class RPCDSDataSourceBase {
  
  map<string, RPCDSDataService*> servicesMap;
  
protected:
  
  virtual void insertNewServiceForKey(const string & key,RPCDSDataService * service){this->servicesMap[key] = service;}
  virtual void removeServiceForKey(const string & key){ delete this->servicesMap.at(key) ; servicesMap.at(key) = nullptr; }
  
public:  
  
  virtual RPCDSDataService * getServiceForKey(const string & key) { return this->servicesMap.at(key);}
  //virtual RPCDSDataType * getDataFromServiceForKey(const string & serviceKey)=0;
  RPCDSDataType * getResultFromQueue(const string & serviceKey) {this->getServiceForKey(serviceKey)->get();} // get 
  
  //virtual void setPointerToQueueResultsForService(const string & serviceKey){ this->servicesMap. }
  // setup the consumers pointers to DataService without DataSource
  
  // why not only create service for request 
  
  
  virtual ~RPCDSDataSourceBase();
  RPCDSDataSourceBase();
  
};





#endif