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

using namespace std;

int main( int argc ,char *argv[] ){
  
  /**
   * 
   * 
   * 
   */
  
  if (argc != 15){
    std::cout << " 14 arguments needed ! " << endl;
    exit(-1);
  }
  
  DataObject lumiObj(argv[1]);
  string filesCont = argv[2];
  string outCont = argv[3];
  string IntrinsicFile = argv[4];
  bool WriteNewIntrinsicFile = atoi(argv[5]);
  bool intr = atoi(argv[6]);
  string histogram_title = argv[7];
  string root_file_title = argv[8]; 
  string good_runs_map = argv[9];
  string disk_ = argv[10];
  string orientation = argv[11];
  bool avEnt = atoi(argv[12]);
  int sect=atoi(argv[13]);
  int maxY = atoi(argv[14]);
  //int tmpSectorStart=atoi(argv[12]);
  //int tmpSectorEnd=atoi(argv[13]);
  //string tmpPart = argv[14];
  QueryObject * obj = new QueryObject;
  
  //obj->insertNewRecordWithOptions(("RE+1/2 C"),histogram_title.c_str(),0,10500,0,10,kRed+3,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R3",orientation,0,36,"A");
  /*
  obj->insertNewRecordWithOptions(("75 ns"),histogram_title.c_str(),0,3500,0,10,kRed+3,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(160000,161474,1);
  
  obj->insertNewRecordWithOptions(("50 ns increase k"),histogram_title.c_str(),0,3500,0,10,kGreen,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(161475,167913,2);
  
  obj->insertNewRecordWithOptions(("Reduce #epsilon,inc. N"),histogram_title.c_str(),0,3500,0,10,kBlack,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(167914,173664,3);
  
  obj->insertNewRecordWithOptions(("#beta*1 m"),histogram_title.c_str(),0,3500,0,10,kGray,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(173665,176841,4);
  
  obj->insertNewRecordWithOptions(("Increase N"),histogram_title.c_str(),0,3500,0,10,kCyan,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(176842,179558,5);
  
  obj->insertNewRecordWithOptions(("All the rest"),histogram_title.c_str(),0,3500,0,10,kBlue,20,"Instantaneous Luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,0,10,"C");
  obj->insertRunRangeForRecord(179558,180252,6);
  */
  
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R2 C").c_str(),histogram_title.c_str(),0,3500,0,maxY,kRed+3,20,"Instantaneous luminosity (10^{30} cm^{-2} s^{-1})","Rate (Hz/cm^{2})",disk_,"R2",orientation,sect,sect,"C");
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R2 B").c_str(),"sec",0,3500,0,10,kRed,21,"sha","LQ",disk_,"R2",orientation,sect,sect,"B");
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R2 A").c_str(),"third",0,3500,0,10,kBlue,22,"onche","bonche",disk_,"R2",orientation,sect,sect,"A");
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R3 C").c_str(),"fourth",0,3500,0,10,kCyan,23,"doesnt","matter",disk_,"R3",orientation,sect,sect,"C");
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R3 B").c_str(),"fifth",0,3500,0,10,kBlack,24,"doesnt","matter",disk_,"R3",orientation,sect,sect,"B");
  obj->insertNewRecordWithOptions(("RE"+orientation+disk_+" R3 A").c_str(),"sixt",0,3500,0,10,kSpring-7,25,"doesnt","matter",disk_,"R3",orientation,sect,sect,"A");
  
  // standard plots
  obj->setCanvasTitle(root_file_title.c_str());
  
  plotRateVsLumi_using_the_database_files_rollLevel_Ecap_Offline(lumiObj,filesCont,outCont,IntrinsicFile,WriteNewIntrinsicFile,intr,avEnt,good_runs_map,obj);
  
}
