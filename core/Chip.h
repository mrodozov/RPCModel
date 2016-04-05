/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef RPC_CHIP_H
#define RPC_CHIP_H

#include <string>
#include <fstream>
#include <sstream>

#include "ChipInterface.h"
#include "ExtendedStrip.h"
#include "RollInterface.h"
#include "../ROOT/ROOT.h"

using namespace std;

class Chip : public ChipBaseClass{
  
  int chipNumber;
  int chipID;
  
protected:
  
  virtual ExStrip * getPointerToStripPointers(); // to be overridden 
  virtual void setStripsPointersToPointToObjects(StripInterface * Allocated_ExStrip_Objects); // to be protected
  
public:
  
  virtual ExStrip * getChipStrip(const int & stripNumber); // tested
  virtual void assignStripsFromRoll(RollInterface * aRoll); // tested
  void setChipNumber(const int & ChipNumber); // tested
  void setChipID(int ID); // tested
  
  const int & getChipNumber(); // tested
  const int & getChipID(); // tested
  
//  RollChip();//decide what should be the constructor if not empty
  
  double getMeanValueOfStrips(); // tested
  double getAverageValueOfStrips(); // tested
  TH1F * getHistoOfChipStrips(const string & runString); // tested
  TH1F * getDistributionOfChipsStripRateValues(const string & runString); // not tested 
  double getMaxRateValueOfChipStrips();  // tested
  
  Chip();
  
  ~Chip();
  
};

#endif