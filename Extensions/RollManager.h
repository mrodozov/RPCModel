/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef ROLLMANAGER_H
#define ROLLMANAGER_H
#include <map>
#include "../core/ExtendedStrip.h"
#include "../core/Roll.h"
#include <vector>

using namespace std;

class RollManager{

 public:

  RollManager();

  vector<Roll*>* getRolls(int RunID);  
  double RateIntegral(int RunID);

 private:

  map<int, vector<Roll*> > AllTheRolls;
  vector<Roll*> Barrel;
  vector<Roll*> EndCap;

};


#endif
