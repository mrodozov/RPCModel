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
  string DataFolder = argv[1]; // folder with root files
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
  string conditionsFolder = argv[11];
  string outputJSON = argv[12];
  
  //cout << conditionsFolder << endl;
  
  isOffline = 1;
  auto dataMap =  prepareDataSourceWithRatesAndLumi(DataFolder,Lumi);
  auto conditionsMap = getConditionsMapFromFolderForRunlist(argv[2],conditionsFolder);
  
  
  sector = "";
  /*
  outputJSON = "RED1R1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->setCanvasTitle("RE_1_1_Online");
  
  aQuery->insertNewOnlineRecord(("RE+1_R1_CH04"+sector).c_str(),("RE +1 1 S 4"+sector).c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE+1_R1_CH02"+sector).c_str(),("RE +1 1 S 2"+sector).c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE+1_R1_CH03"+sector).c_str(),("RE +1 1 S 3"+sector).c_str(),kSpring-7,25);
  aQuery->insertNewOnlineRecord(("RE+1_R1_CH01"+sector).c_str(),("RE +1 1 S 1"+sector).c_str(),kBlue,23);  
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);
  
  vector<string> disks;
  disks.push_back("4");
  disks.push_back("3");
  disks.push_back("2");
  disks.push_back("1");
  
  
  for ( auto & i : disks){
    
    orientation = i;
    aQuery->clearAllOnlineRollRecords();
    aQuery->setCanvasTitle("RE"+orientation+"_average");  sector = "";   outputJSON = "RE"+orientation+"_average.json";
    
    aQuery->insertNewOnlineRecord(("RE-"+orientation+"_R2_CH"+sector).c_str(),("RE-"+orientation+" 2 "+sector).c_str(),kRed,22);
    aQuery->insertNewOnlineRecord(("RE+"+orientation+"_R2_CH"+sector).c_str(),("RE+"+orientation+" 2 "+sector).c_str(),kBlue,23);      
    aQuery->insertNewOnlineRecord(("RE-"+orientation+"_R3_CH"+sector).c_str(),("RE-"+orientation+" 3 "+sector).c_str(),kBlack,21);
    aQuery->insertNewOnlineRecord(("RE+"+orientation+"_R3_CH"+sector).c_str(),("RE+"+orientation+" 3 "+sector).c_str(),kSpring-7,25);
    //isOffline = 0;
    plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);
    
  }
  
  aQuery->clearAllOnlineRollRecords();
  aQuery->setCanvasTitle("SystemBarrelEndcap");
  outputJSON = "SystemRateLumi.json";
  
  aQuery->insertNewOnlineRecord("RE","Endcap",kBlack,21);
  aQuery->insertNewOnlineRecord("R","System average",kGreen+3,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W","Barrel",kRed,kFullCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);
  
  
  
  aQuery->setCanvasTitle("Barrel Wheels");
  outputJSON = "BarrelWheels.json";    
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W+2","W+2",kRed-3,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W-2","W-2",kRed+1,kOpenTriangleUp);
  aQuery->insertNewOnlineRecord("W-1","W-1",kBlue-3,kOpenSquare);
  aQuery->insertNewOnlineRecord("W+1","W+1",kBlue+2,kFullSquare);
  aQuery->insertNewOnlineRecord("W+0","W0",kBlack,kFullCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,0);  
  
  aQuery->setCanvasTitle("Endcap Stations");
  outputJSON = "allEcapDisks.json";	
  aQuery->clearAllOnlineRollRecords();
  
  //isOffline = 0;
  
  aQuery->insertNewOnlineRecord("RE-4_R","RE-4",kRed,kOpenCircle);
  aQuery->insertNewOnlineRecord("RE+4_R","RE+4",kRed,kFullCircle);      
  aQuery->insertNewOnlineRecord("RE-3_R","RE-3",kBlack,kOpenTriangleDown);
  aQuery->insertNewOnlineRecord("RE+3_R","RE+3",kBlack,kFullTriangleDown);
  aQuery->insertNewOnlineRecord("RE-2_R","RE-2",kGreen,kOpenTriangleUp);      
  aQuery->insertNewOnlineRecord("RE+2_R","RE+2",kGreen,kFullTriangleUp);    
  aQuery->insertNewOnlineRecord("RE+1_R","RE+1",kBlue,kOpenSquare);
  aQuery->insertNewOnlineRecord("RE-1_R","RE-1",kBlue,kFullSquare);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,0);
  */
  
  //isOffline = 1;
  /*
  aQuery->setCanvasTitle("Barrel W+2");
  outputJSON = "W+2.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W+2","W+2 ",kRed-3,kFullTriangleUp);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Barrel W+1");
  outputJSON = "W+1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W+1","W+1 ",kBlue+2,kFullSquare);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Barrel W-1");
  outputJSON = "W-1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W-1","W-1 ",kBlue-3,kOpenSquare);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Barrel W-2");
  outputJSON = "W-2.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W-2","W-2 ",kRed+1,kOpenTriangleUp);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Barrel W0");
  outputJSON = "W0.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("W+0","W0 ",kBlack,kFullCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  
  aQuery->setCanvasTitle("Endcap RE-4");
  outputJSON = "RE-4.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE-4_R","RE-4 ",kRed,kOpenCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE+4");
  outputJSON = "RE+4.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE+4_R","RE+4 ",kRed,kFullCircle);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE-3");
  outputJSON = "RE-3.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE-3_R","RE-3 ",kBlack,kOpenTriangleDown);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE+3");
  outputJSON = "RE+3.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE+3_R","RE+3 ",kBlack,kFullTriangleDown);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE-2");
  outputJSON = "RE-2.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE-2_R","RE-2 ",kGreen,kOpenTriangleUp);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE+2");
  outputJSON = "RE+2.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE+2_R","RE+2 ",kGreen,kFullTriangleUp);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE-1");
  outputJSON = "RE-1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE-1_R","RE-1 ",kBlue,kFullSquare);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  aQuery->setCanvasTitle("Endcap RE+1");
  outputJSON = "RE+1.json";
  aQuery->clearAllOnlineRollRecords();
  aQuery->insertNewOnlineRecord("RE+1_R","RE+1 ",kBlue,kOpenSquare);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON,1);
  
  */
  
  
  //auto rollLevelData = prepareDataSourceWithRatesAndLumi(DataFolder,Lumi,isOffline);
  
  sector = "10_C";
  outputJSON = "RE_43_HotZones_Offline_10_C.json";
  aQuery->clearAllOnlineRollRecords();
  orientation = "";
  aQuery->setCanvasTitle("RE4 R2 Sector 10");
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"4_R2_CH"+sector).c_str(),("RE -4"+orientation+" 2 S 10 C").c_str(),kBlue,23);  
//   aQuery->insertNewOnlineRecord(("RE+"+orientation+"4_R2_CH"+sector).c_str(),("RE +4"+orientation+" 2 S 10 C").c_str(),kRed,22);  
//   aQuery->insertNewOnlineRecord(("RE+"+orientation+"3_R2_CH"+sector).c_str(),("RE +3"+orientation+" 2 S 10 C").c_str(),kBlack,21);
//   aQuery->insertNewOnlineRecord(("RE-"+orientation+"3_R2_CH"+sector).c_str(),("RE -3"+orientation+" 2 S 10 C").c_str(),kSpring-7,25);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);    
  
  /*
  
  sector = "27_C";
  outputJSON = "RE_43_HotZones_Offline_27_C.json";
  aQuery->clearAllOnlineRollRecords();
  orientation = "";
  aQuery->setCanvasTitle("RE4 R2 Sector 27");
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"4_R2_CH"+sector).c_str(),("RE -4"+orientation+" 2 S 27 C").c_str(),kBlue,23);  
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"4_R2_CH"+sector).c_str(),("RE +4"+orientation+" 2 S 27 C").c_str(),kRed,22);  
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"3_R2_CH"+sector).c_str(),("RE +3"+orientation+" 2 S 27 C").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"3_R2_CH"+sector).c_str(),("RE -3"+orientation+" 2 S 27 C").c_str(),kSpring-7,25);
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);    
  
  
  sector = "";
  outputJSON = "Wp2_RB4ppS4F.json";
  aQuery->clearAllOnlineRollRecords();
  orientation = "Wp2_RB4ppS4F";
  aQuery->setCanvasTitle("Wp2_RB4ppS4F");
  aQuery->insertNewOnlineRecord("W+2_RB4++_S04_Forward","W+2 RB4++ S4 Forward",kGreen-7,kOpenDiamond);
  
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"4_R2_CH"+sector).c_str(),("RE -4"+orientation+" 2 S 27 C").c_str(),kBlue,23);  
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"4_R2_CH"+sector).c_str(),("RE +4"+orientation+" 2 S 27 C").c_str(),kRed,22);  
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"3_R2_CH"+sector).c_str(),("RE +3"+orientation+" 2 S 27 C").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"3_R2_CH"+sector).c_str(),("RE -3"+orientation+" 2 S 27 C").c_str(),kSpring-7,25);
  
  plotRateVsLumi_using_root_and_JSON(dataMap,Lumi,cutThreshold,aQuery,isOffline,debugOUTPUT,conditionsMap,outputJSON);  
  */
}
