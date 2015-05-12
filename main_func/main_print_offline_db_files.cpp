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
   *  print two 1 text file for 1 total_runnum.root file. Each file contains two columns - the RollName and the AverageRate for this roll
   *  where the average rate is calculated with or without correction, and if with - the correction is given with  integer number
   *  
   *  
   * 
   */
  
  if (argc != 8){
    cout << "It needs 7 arguments !" << endl;
    exit(-1);
  }
  
  string rootContainer,outputContainer,RollList,LumiFileName,AreaFileName;
  bool corrections;
  double correctionInPercents;
  rootContainer = argv[1];
  outputContainer = argv[2];
  LumiFileName = argv[3];
  AreaFileName = argv[4];
  RollList = argv[5];
  corrections = atoi(argv[6]);
  correctionInPercents = atoi(argv[7]);
  DataObject lumiFile(LumiFileName);
  DataObject area(AreaFileName);
  
  print_offline_db_files(rootContainer,outputContainer , lumiFile, area,RollList,corrections,correctionInPercents);
  
  return 0;
}
