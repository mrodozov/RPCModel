/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 17.05.2013
 * Info added at SVN revision 80
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
//#include <mysql/mysql.h>
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../core/Chip.h"
#include "../Applications.h"
#include "../ROOT/ROOT.h"
#include "../ServiceClasses/Service.h"
#include "../ServiceClasses/JSON/json/json.h"
#include "../ServiceClasses/DBconnector.h"

int main (int argc,char * argv[]){
  
  /** 
   * 
   * 
   */
  
  string 
  runLowerRateFile = argv[1],
  runHigherRateFile = argv[2],
  area = argv[3],
  allRollsList = argv[4],
  SGrollsMap = argv[5],
  cutValue = argv[6];
  
  getRatioOfChambersForTwoRuns(runLowerRateFile,runHigherRateFile,area,allRollsList,SGrollsMap,atoi(cutValue.c_str()));
  
  return 0;
  
}
