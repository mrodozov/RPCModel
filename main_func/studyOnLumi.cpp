#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <cmath>
#include <stdlib.h>
/** non-standart headers , the model , the wrappers and ROOT */
#include "core/DataObject.h"
#include "ExtendedStrip.h"
#include "ExtendedRoll.h"
#include "ParserFunction.h"
#include "ROOT/plots.h"
#include "Wrappers/WrapperFunctions.h"
#include "ROOT/ROOT.h"
//#include <curl/curl.h>
#include "Extensions/Run.h"
#include "Extensions/Acquisition.h"
#include "TH1F.h"
#include "TFile.h"
#include "TGraph.h"
#include "../Extensions/Run.h"
using namespace std;

int main( int argc ,char *argv[] ){
  
  DataObject areas;
  areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);
  
  map<string, double> areaMap;
  
  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }

  //Syntax:
  //EndCap RE(WDN)_SECTOR_RID
  //Barrel W(WDN)_RID_SECTOR

  if (argc < 5) {
    
    cout << "Usage: " << argv[0] << " Zone Wheel Sector RollID [OutFile]" << endl;
    cout << endl;
    cout << "  Conventions:"<< endl;
    cout << "    Barrel: W(WDN)_RID_SECTOR" << endl;
    cout << "    EndCap: RE(WDN)_SECTOR_RID" << endl;
    cout << endl;
    return 0;
  }

  /*  string zone = "EndCapPlus";
  string RollID = "10";
  int Sector = 2;
  int WheelDiskNo = 2;
  */

  string zone = argv[1];                                                                                                                                                  
  string RollID = argv[4]; 
  int Sector = atoi(argv[3]);                                                                                                                                          
  int WheelDiskNo = atoi(argv[2]);                                                                                                                                                            

  //Create a new Acquisition Registry
  TFile* contFile = TFile::Open("Container20062011.root");
  Acquisition Run2011(contFile);

//  ifstream json("./jsonFilesMu/Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt");
    ifstream json("./jsonFilesMu/Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON.txt");
     Run2011.LoadJSON(json);
/*
  DataObject filterRun;
  filterRun.getDataFromFile("filters/collisions2011_167913_runNames.txt",1);  
  Run2011.LoadSelectionByDO(&filterRun, 1);*/

      TFile* f;
      if (argc >= 6)
	f = new TFile(argv[5], "RECREATE");
      else
	f = new TFile("Outputs/OutputFile.root", "RECREATE");

  TGraph* plot = new TGraph();
  plot->SetName("RateVsLumi");
  
  plot->SetMarkerStyle(21);
  contFile->Close();
    int k=0;    
  
  while (Run2011.hasNext()){
    
    double NormalizedRate = 0;
    
    Run* currentRun = Run2011.next();

    //run selection
//    if ( currentRun->getID() > 163289 && currentRun->getID() < 163340 ) continue;
//    if (currentRun->getID() > 163869) break;
    if (currentRun->getLumiSection() < 100) continue;
    if (!Run2011.isValid(currentRun, false)) continue;
    

    //Filling
    contFile = TFile::Open("Container20062011.root");
    currentRun->FillFromContainer(contFile, areaMap, true);
    contFile->Close();
        
    double TotRate = 0;
    double TotArea = 0;
    int totAmount = 0;

    if (currentRun->isFilled() == true){
      
      
      map<string,ExRoll* > barrelMap;

      if (zone == "Barrel")
	barrelMap = currentRun->GetBarrel();

      else if (zone == "EndCapPlus")
	barrelMap = currentRun->GetEndCapPlus();

      else if (zone == "EndCapMinus")
	barrelMap = currentRun->GetEndCapMinus();

      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){

	if (RollID != "-9")
	  if (it->second->getRollID().c_str() != RollID) continue;

	if (Sector != -9)
	  if (it->second->getSector() != Sector) continue;

	if (WheelDiskNo != -9)
	  if (it->second->getWheel_Disk_Number() != WheelDiskNo) continue;


//	if ( it->second->getOptionalRollID().compare("B") ) continue;
//	cout << "SEC " << it->second->getSector()  << endl;
/*	cout << "OPT: " << it->second-> getOptionalRollID() << endl;
	cout << "RAW: " << it->second->getRawIDofClone(1) << endl;*/
//	cout << "PASS: " << it->second->getFullOnlineRollID() << endl;


	TotRate += it->second->getRollRate();
        double rollArea = it->second->getRollArea();
        TotArea += rollArea;	
      }

      //Free RAM
      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){
	delete it->second;
      }

      double NormalizedRate = TotRate / TotArea;
	      
        if (TotRate != 0 && TotArea != 0 && currentRun->getDelivered() != 0 && currentRun->isFilled()){
	cout << endl;
	currentRun->PrintStatistics();	
	cout << currentRun->getID() 
	     //<< ": Lumi: " << currentRun->getLumi() << " "
	     <<  " * Rate: " << TotRate << " * area: "
	     << TotArea << " * norm: " 
	     << NormalizedRate << endl;
	cout << endl;
// 	plot->SetBinContent(plot->FindBin(currentRun->getLumi()), NormalizedRate);
 	plot->SetPoint(k , currentRun->getLumi(), NormalizedRate / 2);
//	cout << k << endl;
	k++;
      }
    }
  }
  
  plot->SetDrawOption("P");
  f->WriteObject(plot, plot->GetName());

  
  return 0;
}
