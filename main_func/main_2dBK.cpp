#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../core/Chip.h"
#include "../Applications.h"
#include "../ROOT/ROOT.h"

int main( int argc ,char *argv[] ){
  
 string folder=argv[1];
 string areaFile = argv[2];
 string shortBarrelIDS = argv[3];
 string shortEcapIDS = argv[4];
 string runList = argv[5];
 string ListOfRolls = argv[6];
 bool correctionToBeApplied = atoi(argv[7]);
 int correctionInPercents = atoi(argv[8]);
 double barrelMax= atof(argv[9]);
 double ecapMax = atof(argv[10]);	
 string CenterOfMassE = argv[11]; 
 //cout << "blabla" << endl;
 
 draw2DbackgroundPlot( folder, areaFile, shortBarrelIDS, shortEcapIDS, runList, ListOfRolls, correctionToBeApplied, correctionInPercents, barrelMax, ecapMax,CenterOfMassE);
 
  
}