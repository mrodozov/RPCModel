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
  string rootFolder = argv[1];
  DataObject areaFile(argv[2]),
  coefficientFile(argv[3]);
  string AllRolls=argv[4];
  DataObject LumiFile(argv[5]);
  string outputContainer=argv[6];
  bool saveHisto=atoi(argv[7]);
  double coefficientThreshold=atof(argv[8]);
  bool use_signle_roll=atoi(argv[9]);
  string roll_passed=argv[10];
  
  
  calculateLumiForRunWithFitCoefficientRefference(rootFolder,areaFile,coefficientFile,AllRolls,LumiFile,outputContainer,saveHisto,coefficientThreshold,use_signle_roll,roll_passed);

  return 0;
}
