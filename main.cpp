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
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include <boost/property_tree/ptree.hpp>
#include <streambuf>

using namespace std;
using namespace boost::property_tree::json_parser;


int main( int argc ,char *argv[] ){
  
  
  //string inputFile = argv[1];
  //int exit_code = IsCorruptedFile(inputFile);

//   string inputFile = argv[1];
//   int exit_code = IsCorruptedFile(inputFile);
//   return exit_code;

  //exit(exit_code);
  
  /*
  string inputFile = argv[1];
  string run_date_map = argv[2];
  int labelModulo = atoi(argv[3]);
  string xAxisTitle = argv[4];
  string datafolder = argv[5];
  DeadMaskedInactivePlot(inputFile,run_date_map,labelModulo,xAxisTitle,datafolder);
  */
  
  //getInstLum(argv[1]);
  //return 0;
  
  
//   string runsuffix = argv[2], runlist = argv[1], area = argv [3], result_folder = argv[4];
//   getCorrectedChambersRateDistribution(runlist,runsuffix,area,result_folder);
  
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
  string root_file=argv[1],out_put_rolls_db_file=argv[2],out_put_strips_db_file=argv[3],error_file=argv[10],Roll_list=argv[11];                                                                                
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
 //string inputRolls=argv[1], raw=argv[2], area=argv[3], tower=argv[4], chips=argv[5], output=argv[6];
 //WriteIDsMap(inputRolls,raw,area,tower,chips,output);
 
 
 
  /*
  string rateFile = argv[1];
  bool subtractIntrinsic = atoi(argv[2]);
  string fileWithIntrinsic = argv[3];
  DataObject area = DataObject(argv[4]);
  double cutValueSingleStrip = atof(argv[5]);
  string SGmapFile = argv[6];
  DataObject LumiFile = DataObject(argv[7]);
  string theDisk = argv[8];
  QueryObject * obj = new QueryObject;
  
  obj->setCanvasTitle(argv[9]);
  //obj->setRVSPHItext(("Disk +"+theDisk+"/-"+theDisk).c_str());
  obj->setRVSPHItext(argv[10]);
  obj->insertRecordForEcapRoll(("-"+theDisk).c_str(),"2",kRed+3,22,("RE-"+theDisk+"/2").c_str(),1.5);
  obj->insertRecordForEcapRoll(("+"+theDisk).c_str(),"2",kBlue+1,29,("RE+"+theDisk+"/2").c_str(),1.6);  
  obj->insertRecordForEcapRoll(("-"+theDisk).c_str(),"3",kMagenta,21,("RE-"+theDisk+"/3").c_str(),1);
  obj->insertRecordForEcapRoll(("+"+theDisk).c_str(),"3",kGreen-1,20,("RE+"+theDisk+"/3").c_str(),1);
  
  //obj->setRVSPHItext("#sqrt{s} = 8 TeV, Instantaneous Luminosity 17.08 10^{32}cm^{-2} s^{-1}");
  //obj->insertRecordForEcapRoll(("+"+theDisk).c_str(),"1",kGreen-1,20,("RE+"+theDisk+"/1").c_str(),1); // RE_1_1
  
  plotEcap_RateVsPhi(rateFile,
			  subtractIntrinsic,
			  //to be zero,
			  fileWithIntrinsic,
			  area,
			  cutValueSingleStrip ,
			  SGmapFile, // in localResources,
			  LumiFile,
			  obj
			 );	 
 
 */
  
  /*
  string RateFilesContainer = argv[1], LumiFile = argv[2], areaFile=argv[3], AllRollNames=argv[4];
  int percentsOfCut=atoi(argv[5]);
  bool SaveHistograms = atoi(argv[6]);
  string outputFolder = argv[7];
  
  getCorrelationCoefficients( RateFilesContainer,  LumiFile,  areaFile, AllRollNames, percentsOfCut, SaveHistograms , outputFolder);
  */
  
  
//    string rootFile = argv[1], outputFile = argv[4],  area = argv[5];
//    int HVbarrel = atoi(argv[2]), HVendcap = atoi(argv[3]);
//    
//    WriteNoiseScanResults(rootFile,HVbarrel,HVendcap,outputFile,area);

  //PrintDBIDsvsNewIDs(argv[1],argv[2]);
  /*
  RooRealVar x("x","x",-10,10) ;
  RooRealVar mean("mean","Mean of Gaussian",0,-10,10) ;
  RooRealVar sigma("sigma","Width of Gaussian",3,-10,10) ; 
  RooGaussian gauss("gauss","gauss(x,mean,sigma)",x,mean,sigma) ; 
  RooPlot * aplot = x.frame();
  
  gauss.plotOn(aplot);
  aplot->SaveAs("frame.root");
  */
  
  
//    string LumiFile = argv[1],
//    rootFilesFolder = argv[2], area = argv[3], outputFile = argv[4], suffix = argv[5];  
//    WriteRateVsLumiPerRollFile(LumiFile,rootFilesFolder,area,outputFile,suffix);
   
//    string fiftheen = argv[1],
//    sixteen = argv[2];
//    Ratios(fiftheen, sixteen);
  
//    string ids_file = argv[1],
//    input = argv[2], output = argv[3], twoDmapFile = argv[4] , ShortB =argv[5] ,ShortE=argv[6];
//    SlopeRatiosComparisonForPairsOfIDs(ids_file,input,output,twoDmapFile,  ShortB,  ShortE);
   
   // printTimeBins(argv[1]);
//    string fname = argv[1];
//    GetLumiHistogramPerLS(fname);
    
    // string rateFolders = argv[1];string lumiFiles = argv[2];string lbToChMap = argv[3];string area = argv[4];
    // testLumiRateCorrelation(rateFolders,lumiFiles,lbToChMap,area);
    
    sortHistogramsByMeanAndRMS(argv[1]);
        
    //testLumiRateCorrelation_SummariesByRuns_StripsRolls(argv[1]);
    
    //testLumiRateCorrelation_MergeStripsHistos(argv[1]);
    
   //   string jsonDatafile = argv[1] , jsonMapFile = argv[2], rootOutput=argv[3];
   //  yaxisttitle = argv[5],fileSuffix = argv[6];
   //  int barrelmax = atoi(argv[3]), endcapmax = atoi(argv[4]);
   
//     get2DplotsForJSONFileUsingAndJSONmap(jsonDatafile,jsonMapFile,barrelmax,endcapmax,yaxisttitle,fileSuffix);
//   DrawVvsPhiUsingJSONdataAndJSONconfig(jsonDatafile,jsonMapFile); 
//    plot_X_vs_Y_values_using_JSON_data_and_JSON_config(jsonDatafile,jsonMapFile,rootOutput);
   
    //string rootFile=argv[1],jsonFile=argv[2],areaFile=argv[3];
    //int bmax = atoi(argv[4]),emax = atoi(argv[5]);
    
   
   //get2DplotsOnRateFromROOTfile(rootFile,jsonFile,areaFile,bmax,emax);
  
  // drawLumiPerChamberUsingJSONfile(jsonFile,rootFile);
   
   //compareHistos();
   
  /*  
  
  
  string datafolder = argv[1], rollName = argv[2];
  string area_file = argv[3];
  string LumiFile = argv[4];
  DataObject Lumi(LumiFile);
  DataObject rollNames(rollName);
  DataObject area(area_file);  
  
  
  for (int i = 0 ; i < Lumi.getLenght() ; i++){
    
    string run = Lumi.getElement(i+1,1);
    string jfile = datafolder+"run"+run+"/output_strips.json";
    cout << jfile << endl;
    
    ifstream ss(jfile);
    ptree JSONfile;
    
    boost::property_tree::read_json(ss, JSONfile); // loads really slow
    ptree rate = JSONfile.get_child("rates");
    
    
    
    for (int j = 0 ; j < rollNames.getLenght() ; j++){
      if (j == 1) break;
    
      string r_name = rollNames.getElement(j+1 , 1);
    
      ExRoll * aroll = new ExRoll(r_name);
      aroll->allocStrips();
      aroll->initStrips();
      
      aroll->setStripsAreaFromSource_cmsswResource(area);
      aroll->setStripsRateFromJSONsource(rate);
      
      cout << run << " " << aroll->getFullOnlineRollID() << " " << aroll->getAvgRatePSCWithoutCorrections() << endl;

      
      
      for(int ii = 0 ; ii < aroll->getClones() ; ii++){
	for(int j = aroll->getFirstStripNumberOfClone(ii+1) ; j <= aroll->getLastStripNumberOfClone(ii+1) ; j++){
	  cout << j << " " << aroll->getStrip(j)->getRate() << " " << aroll->getStrip(j)->getRate()/aroll->getStripsAreaFromClone(ii+1) << endl;
	}
      }
      
      
      delete aroll;
    
    }
    
    
    ss.clear();
    ss.close();
    
  }
  
  */
 
  return 0;
}
