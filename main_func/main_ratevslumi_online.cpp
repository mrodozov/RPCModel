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
  
  /** executable : RateVsLumi_Online.app
  * example use :
  *./RateVsLumi_Online.app ../../mirchoBranch/resources/online_100/online_database_ 2011_LBfileTime.lumi INTRINSIC_FULL_online area_noise_cmssw 2011_LBTIME_plots - 24 2012excllist
  * Other hints :
  * 1. online_100/online_database_ is with the suffix of all the files. They have to contain online-like preprocessed records - "RollName avgRate"
  */
  
  QueryObject * aQuery = new QueryObject;
  string DataFolder = argv[1]; // folder with text files
  DataObject Lumi(argv[2],3); // 3 rows, run lumi and ls  
  string IntrinsicOnlineFile = argv[3]; // 
  DataObject area(argv[4],2); //
  aQuery->setCanvasTitle(argv[5]);
  string orientation = argv[6],ring = argv[7],sector = argv[8];
  DataObject exluded(argv[9]);
  int cutThreshold =atoi(argv[10]);
  aQuery->setHistoXtitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
  aQuery->setHistoYtitle(" Rate (Hz/cm^{2})");
  aQuery->setMainTitle("RE"+orientation+ring+sector);
  string emtpyString = "";
  bool useLumiAsRateDivider = atoi(argv[11]);
  bool debugOUTPUT = atoi(argv[12]);
  
  if(useLumiAsRateDivider){
    aQuery->setHistoYtitle(" Rate/Lumi  (cm^{-2}s^{-1}/10^{27}cm^{-2}s^{-1})");
  } else {
    aQuery->setHistoYtitle(" Rate (Hz/cm^{2})");
  }
  
  // run ranges for different HV 
  /*
  aQuery->insertNewOnlineRecord(("RE"+orientation+ring+sector).c_str(),(" 190645 195633 "+emtpyString).c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE"+orientation+ring+sector).c_str(),(" 195634 198022 "+emtpyString).c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("RE"+orientation+ring+sector).c_str(),(" 198023 199973 "+emtpyString).c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE"+orientation+ring+sector).c_str(),(" 200041 208686 "+emtpyString).c_str(),kGreen,kOpenTriangleDown);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,1);
  aQuery->insertRunRangeForOnlineRecord(195634,198022,2);
  aQuery->insertRunRangeForOnlineRecord(198023,199073,3);
  aQuery->insertRunRangeForOnlineRecord(199074,208686,4);
  */  
  
  /*
  aQuery->setHistoXtitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
  aQuery->setHistoYtitle(" Rate (Hz/cm^{2})");
  aQuery->insertNewOnlineRecord(""+orientation,"",kOrange,kOpenCircle);
  aQuery->insertNewOnlineRecord(""+orientation,""+orientation,kOrange,kFullCircle);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,1);
  aQuery->insertRunRangeForOnlineRecord(195634,203002,2);
  */
  
  /*
  aQuery->insertNewOnlineRecord("W+2_"+orientation,"",kOrange,kOpenCircle);
  aQuery->insertNewOnlineRecord("W+2_"+orientation,"W+2_"+orientation,kOrange,kFullCircle);
  aQuery->insertNewOnlineRecord("W-2_"+orientation,"",kGreen,kOpenSquare);
  aQuery->insertNewOnlineRecord("W-2_"+orientation,"W-2_"+orientation,kGreen,kFullSquare);
  aQuery->insertNewOnlineRecord("W+1_"+orientation,"",kViolet,kOpenTriangleUp);
  aQuery->insertNewOnlineRecord("W+1_"+orientation,"W+1_"+orientation,kViolet,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W-1_"+orientation,"",kRed+1,kOpenTriangleDown);
  aQuery->insertNewOnlineRecord("W-1_"+orientation,"W-1_"+orientation,kRed+1,kFullTriangleDown);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,1);
  aQuery->insertRunRangeForOnlineRecord(195634,203002,2);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,3);
  aQuery->insertRunRangeForOnlineRecord(195634,203002,4);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,5);
  aQuery->insertRunRangeForOnlineRecord(195634,203002,6);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,7);
  aQuery->insertRunRangeForOnlineRecord(195634,203002,8);
  */
  
  /** 2011 plots , per layer */
    // per layer drawing - Barrel
  /*
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB4").c_str(),("W "+orientation+" RB4").c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB1in").c_str(),("W "+orientation+" RB1").c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB2in").c_str(),("W "+orientation+" RB2").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("W"+orientation+"_RB3").c_str(),("W "+orientation+" RB3").c_str(),kSpring-7,25);
  */
   // ecap
   //
   /*
  aQuery->insertNewOnlineRecord(("RE"+orientation+"2_2_"+sector).c_str(),("RE D"+orientation+"2 R2 ").c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE"+orientation+"3_2_"+sector).c_str(),("RE D"+orientation+"3 R2 ").c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("RE"+orientation+"3_3_"+sector).c_str(),("RE D"+orientation+"3 R3 ").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE"+orientation+"2_3_"+sector).c_str(),("RE D"+orientation+"2 R3 ").c_str(),kSpring-7,25);
  */
  // by single disk
  
  /*
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"_2_"+sector).c_str(),("RE D+"+orientation+" R2 - 2015").c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"_2_"+sector).c_str(),("RE D-"+orientation+" R2 ").c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"_3_"+sector).c_str(),("RE D+"+orientation+" R3 ").c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"_3_"+sector).c_str(),("RE D-"+orientation+" R3 ").c_str(),kSpring-7,25);
  */
  // system
  
  /*
  aQuery->insertNewOnlineRecord("RE","Endcap",kBlack,21);
  aQuery->insertNewOnlineRecord("R","System average",kGreen+3,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W","Barrel",kRed,kFullCircle);
  */
  
  // system , HV ranges 2012
  /*
  aQuery->insertNewOnlineRecord((orientation).c_str(),(" < 195633"+emtpyString).c_str(),kRed,22);
  aQuery->insertNewOnlineRecord((orientation).c_str(),(" [195634 198022] "+emtpyString).c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord((orientation).c_str(),(" 198023 199973 "+emtpyString).c_str(),kBlack,21);
  aQuery->insertNewOnlineRecord((orientation).c_str(),(">  200041"+emtpyString).c_str(),kGreen,kOpenTriangleDown);
  aQuery->insertRunRangeForOnlineRecord(190645,195633,1);
  aQuery->insertRunRangeForOnlineRecord(195634,198022,2);
  aQuery->insertRunRangeForOnlineRecord(19802/Plots/barrel_W0.png3,199073,3);
  aQuery->insertRunRangeForOnlineRecord(199074,208686,4);
  */
  
  // by wheels barrel
  //aQuery->insertNewOnlineRecord(("W"+orientation).c_str(),("W "+orientation).c_str(),kRed,22);
  
  
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"_2"+sector).c_str(),("RE -"+orientation+" 2 ").c_str(),kBlue,22);
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"_2"+sector).c_str(),("RE +"+orientation+" 2").c_str(),kRed,22);
  aQuery->insertNewOnlineRecord(("RE-"+orientation+"_3"+sector).c_str(),("RE -"+orientation+" 3 ").c_str(),kSpring-7,25);
  aQuery->insertNewOnlineRecord(("RE+"+orientation+"_3"+sector).c_str(),("RE +"+orientation+" 3 ").c_str(),kBlack,21);
  
  //aQuery->insertNewOnlineRecord("RE+4_2",("RE "+orientation+"+1 1 4 ").c_str(),kSpring-7,27);
  
  // by plus/minus disk
  
  //aQuery->insertNewOnlineRecord(("RE"+orientation+"+"+sector).c_str(),("RE "+orientation+"+ "+sector).c_str(),kBlack,21);
  //aQuery->insertNewOnlineRecord(("RE"+orientation+"-"+sector).c_str(),("RE "+orientation+"- "+sector).c_str(),kRed,25);
  
  //aQuery->insertNewOnlineRecord("RB1","RB1",kBlack,21);
  //aQuery->insertNewOnlineRecord("RB2","RB2",kRed,25);
  
  
  //aQuery->insertNewOnlineRecord(orientation,orientation,kBlue,kOpenTriangleDown);
  /*
  aQuery->insertNewOnlineRecord("W-2","W-2",kViolet-7,kOpenCircle);
  aQuery->insertNewOnlineRecord("W+1","W+1",kRed,kFullSquare);
  aQuery->insertNewOnlineRecord("W-1","W-1",kSpring-7,kFullCircle);
  aQuery->insertNewOnlineRecord("W0","W0",kBlack,kFullTriangleUp);
  aQuery->insertNewOnlineRecord("W+2","W+2",kGreen-7,kOpenDiamond);
  */
  
  
  plotRateVsLumi_using_the_database_rollLevel_online(DataFolder,Lumi,IntrinsicOnlineFile,area,0,cutThreshold,aQuery,exluded,useLumiAsRateDivider,debugOUTPUT);
  
}
