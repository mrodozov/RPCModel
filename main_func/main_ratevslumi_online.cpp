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

using namespace std;

int main( int argc ,char *argv[] ){
  
  /** executable : RateVsLumi_Online.app
  * example use :
  *./RateVsLumi_Online.app ../../mirchoBranch/resources/online_100/online_database_ 2011_LBfileTime.lumi INTRINSIC_FULL_online area_noise_cmssw 2011_LBTIME_plots - 24 2012excllist
  * Other hints :
  * 1. online_100/online_database_ is with the suffix of all the files. They have to contain online-like preprocessed records - "RollName avgRate"
  */  
  
  /*
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB4").c_str(),("W "+orientation+" RB4").c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB1in").c_str(),("W "+orientation+" RB1").c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB2in").c_str(),("W "+orientation+" RB2").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB3").c_str(),("W "+orientation+" RB3").c_str(),kSpring-7,25);
  */
  
  
  QueryObject * aQuery = new QueryObject;
  string DataFolder = argv[1]; // folder with text files
  DataObject Lumi(argv[2],3); // 3 rows, run lumi and ls  
  DataObject area(argv[3],2); //
  aQuery->setCanvasTitle(argv[4]);
  string orientation = argv[5],ring = argv[6],sector = argv[7];
  int cutThreshold =atoi(argv[8]);
  aQuery->setHistoXtitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
  aQuery->setHistoYtitle(" Rate (Hz/cm^{2})");
  aQuery->setMainTitle("RE"+orientation+ring+sector);
  string emtpyString = "";
  bool isOffline = atoi(argv[9]);
  bool debugOUTPUT = atoi(argv[10]);
  string outputJSON = argv[11];
    
  isOffline = 0;
  auto dataMap =  prepareDataSourceWithRatesAndLumi(DataFolder,Lumi);
  
  sector = "";
  outputJSON = "RED1R1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->setCanvasTitle("RE_1_1_Online");
  aQuery->insertNewOnlineRecord(("RE+1_1_01"+sector).c_str(),("RE +1 1 S 1"+sector).c_str(),kBlue,22);  
  aQuery->insertNewOnlineRecord(("RE+1_1_02"+sector).c_str(),("RE +1 1 S 2"+sector).c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE+1_1_03"+sector).c_str(),("RE +1 1 S 3"+sector).c_str(),kSpring-7,25);
  aQuery->insertNewOnlineRecord(("RE+1_1_04"+sector).c_str(),("RE +1 1 S 4"+sector).c_str(),kBlack,21);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,outputJSON);
  
  vector<string> disks;
  disks.push_back("1");
  disks.push_back("2");
  disks.push_back("3");
  disks.push_back("4");
  
  for ( auto & i : disks){
    
    orientation = i;
    aQuery->clearAllOnlineRollRecords();
    aQuery->setCanvasTitle("RE"+orientation+"_average");  sector = "";   outputJSON = "RE"+orientation+"_average.json";
    
    aQuery->insertNewOnlineRecord(("RE+"+orientation+"_2"+sector).c_str(),("RE+"+orientation+" 2 "+sector).c_str(),kBlue,22);  
    aQuery->insertNewOnlineRecord(("RE-"+orientation+"_2"+sector).c_str(),("RE-"+orientation+" 2 "+sector).c_str(),kRed,22);
    aQuery->insertNewOnlineRecord(("RE+"+orientation+"_3"+sector).c_str(),("RE+"+orientation+" 3 "+sector).c_str(),kSpring-7,25);
    aQuery->insertNewOnlineRecord(("RE-"+orientation+"_3"+sector).c_str(),("RE-"+orientation+" 3 "+sector).c_str(),kBlack,21);
    //isOffline = 0;
    plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,outputJSON);
    
  }
  
  aQuery->clearAllOnlineRollRecords();
  aQuery->setCanvasTitle("SystemBarrelEndcap");
  outputJSON = "SystemRateLumi.json";
  
  aQuery->insertNewOnlineRecord("RE","Endcap",kBlack,21);
  aQuery->insertNewOnlineRecord("R","System average",kGreen+3,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W","Barrel",kRed,kFullCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,outputJSON);
  
  aQuery->setCanvasTitle("BarrelWheels");
  outputJSON = "BarrelWheels.json";    
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W-2","W-2",kViolet-7,kOpenCircle);
  aQuery->insertNewOnlineRecord("W+1","W+1",kRed,kFullSquare);
  aQuery->insertNewOnlineRecord("W-1","W-1",kSpring-7,kFullCircle);
  aQuery->insertNewOnlineRecord("W0","W0",kBlack,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W+2","W+2",kGreen-7,kOpenDiamond);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,outputJSON);
  
  isOffline = 1;
  //auto rollLevelData = prepareDataSourceWithRatesAndLumi(DataFolder,Lumi,isOffline);
  sector = "10_C";
  outputJSON = "HotRE43sectors_Eta_C.json";
  aQuery->clearAllOnlineRollRecords();
  orientation = "";
  aQuery->setCanvasTitle("RE_43_HotZones_Offline_C");
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"4_R2_CH"+sector).c_str(),("RE -4"+orientation+" 2 S"+sector).c_str(),kBlue,22);  
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"4_R2_CH"+sector).c_str(),("RE +4"+orientation+" 2 S"+sector).c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"3_R2_CH"+sector).c_str(),("RE -3"+orientation+" 2 S"+sector).c_str(),kSpring-7,25);
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"3_R2_CH"+sector).c_str(),("RE +3"+orientation+" 2 S"+sector).c_str(),kBlack,21);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,outputJSON);    
  
}
