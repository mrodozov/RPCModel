#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
/** non-standard headers , the model , the wrappers and ROOT */
#include "core/DataObject.h"
#include "core/ExtendedStrip.h"
#include "core/ExtendedRoll.h"
#include "Applications.h"
#include "ROOT/ROOT.h"

using namespace std;

int main( int argc ,char *argv[] ){
  
  string inputFile = argv[1];
  //int exit_code = IsCorruptedFile(inputFile);
  //exit(exit_code);
  DeadMaskedInactivePlot(inputFile);
  
  return 0;
  
  //string chamberSuffix = argv[1], fileInput = argv[2], area = argv [3];
  //getCorrectedChambersRateDistribution(chamberSuffix,fileInput,area);
  
  //string Output = argv[1]; 
  //DataObject Masked(argv[2]), Dead(argv[3]), Areas(argv[4]), RawIDFile(argv[5]);
  //string rollNames = argv[6],runNumber = argv[7];
  //WriteRollInactiveStripsRatios( Output, Masked, Dead, Areas,RawIDFile, rollNames , runNumber);
  
  /*
  string inputRollNames=argv[1],OutputFolder=argv[2],RateFile = argv[3];
  DataObject Masked(argv[4],3),Dead(argv[5],3),ToMask(argv[6],4),ToUnmask(argv[7],3),RollTowerMap(argv[8],2);
  map<string,string> towerMap;
  // fill the map from the dataobject                                                                                                                                                                           
  for (int i=0;i < RollTowerMap.getLenght() ; i ++){
    towerMap[RollTowerMap.getElement(i+1,1)] = RollTowerMap.getElement(i+1,2);
  }
  WriteHTMLforNoisePageStripConditions(inputRollNames,OutputFolder,RateFile,Masked,Dead,ToMask,ToUnmask,towerMap);
  */

  /*
  string newLBsList=argv[1],oldLBsList=argv[2],areaFile=argv[3],resourceDir=argv[4],runNum=argv[5],resultPic=argv[6],textOutput=argv[7];
  NewOldLBsHistos(newLBsList,oldLBsList,areaFile,resourceDir,runNum,resultPic,textOutput);
  */
  
  /*
  string root_file=argv[1],out_put_rolls_db_file=argv[2],out_put_strips_db_file=argv[3],error_file=argv[12],Roll_list=argv[13];                                                                                
  DataObject Mskd;                                                                                                                                                                                             
  DataObject Dead;                                                                                                                                                                                             
  DataObject ToMask;                                                                                                                                                                                           
  DataObject ToUnmask;                                                                                                                                                                                         
  DataObject Areas;                                                                                                                                                                                            
  DataObject RawIDObj;                                                                                                                                                                                         
  Mskd.getDataFromFile(argv[4],2);                                                                                                                                                                             
  Dead.getDataFromFile(argv[5],2);                                                                                                                                                                             
  ToMask.getDataFromFile(argv[6],2);                                                                                                                                                                           
  ToUnmask.getDataFromFile(argv[7],2);                                                                                                                                                                         
  Areas.getDataFromFile(argv[8],2);                                                                                                                                                                            
  RawIDObj.getDataFromFile(argv[9],2);                                                                                                                                                                         
  cout << "Translating roll and strip files for DB please wait ..." << endl;                                                                                                                                   
  WriteRollsAndStripsFilesForDB_usingRootFile(root_file,out_put_rolls_db_file,out_put_strips_db_file,error_file,Mskd,Dead,ToMask,ToUnmask,Areas,RawIDObj,Roll_list);                                          
  */
  //cout << "Translating rolls with dead and masked strips only for efficiency studies ..." << endl;                                                                                                           
  //string efficiency_input = argv[10],efficiency_output = argv[11];                                                                                                                                           
  //WriteResultsUsingIOtextFilesAndDataSources(efficiency_input,efficiency_output,Mskd,Dead,RawIDObj);
  
  //exit(exit_code);
  
  
  
}
