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

int main(int argc,char * argv[]){
  
  
  /**
   * @brief
   * @param 
   * 
   */
  
  string RootContainer = argv[1],
  LumiAsRunList = argv[2] ,
  CoefficientRefference2011 = argv[3],
  areaFile = argv[4],
  AllRollNames = argv[5],
  outputContainer = argv[6];
  
  recoverLumiForEachRunWithCoeffAndRateFiles(RootContainer,LumiAsRunList,CoefficientRefference2011,areaFile,AllRollNames,outputContainer);

  return 0;
}
