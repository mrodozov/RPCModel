#ifndef RPC_SUBPART_INTERFACE_H
#define RPC_SUBPART_INTERFACE_H


/**
 * Abstract base for basic functionality of a class that only has the 
 * purpose to group chambers by different criteria
 */

#include "../RollInterface.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class RPCSubPartInterface {
  
  map<string, RollInterface*> chambers; // reference to 
  vector<string> listOfChambers; // list of rolls the partition represents
  RPCSubPartInterface * listOfPartitions; // if it can be subdivided, not clear how to make it generic
  
protected:
  // 
  void setChambersList(const vector<string> & list){this->listOfChambers = list;}
  void setChamberForName(const string & ch_name, RollInterface* chamber) {this->chambers[ch_name] = chamber;} // set 
  RollInterface * getRollForName (const string & name) const { return this->chambers.at(name) ;}
  
private:
  // here goes only pointers setup to rolls and sorting methods (order by methods)
  vector<string> getChambersList() const { return this->listOfChambers; } 
  vector<RollInterface*> getListOfChambersOrderedBySector() const = 0;
  vector<RollInterface*> getListOfChambersOrderedByStation() const = 0;
  // and so on, each subpart to decide. 
  
  RPCSubPartInterface();
  virtual ~RPCSubPartInterface();
  
};

RPCSubPartInterface::~RPCSubPartInterface()
{
  this->listOfChambers.clear(); // don't delete the chamber objects, just clear the reference to them
}

RPCSubPartInterface::RPCSubPartInterface()
{

}


#endif