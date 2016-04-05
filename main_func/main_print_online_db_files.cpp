/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 17.05.2013
 * Info added at SVN revision 80
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <new>
#include <stdlib.h>
#include <unistd.h>
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../Extensions/NoiseAnalyzer.h"
#include "../Applications.h"

using namespace std;

int main (int argc, char * argv[]){

  DataObject area(argv[4],2);
  DataObject Lumi(argv[3],3);
  print_online_dbfiles(argv[1],argv[2],Lumi,area,argv[5],atoi(argv[6]),atof(argv[7]));
  
  return 0;
  
}