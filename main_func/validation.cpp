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
using namespace std;

int main( int argc ,char *argv[] ){
  
  DataObject areas;
  areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);
  
  vector<double> rates;
  vector<double> valuesSilvia;
  map<string, double> areaMap;
  
  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }
  
  //Create a new Acquisition Registry
  TFile* contFile = TFile::Open("Container20062011.root");
  Acquisition Run2011(contFile);
  
  /*  ifstream json("./jsonFilesMu/Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt");
      Run2011.LoadJSON(json);*/

    
  DataObject runFilter;
  int nColoumnsFilter = 6;
  int filterCol = 1;
  runFilter.getFastDataFromFile("./resources/Extensions/2011/replotmuondpg/tot_RE+3_2_10_2011.txt",nColoumnsFilter);
  for (int i=1; i<= runFilter.getLenght(); i++){
    valuesSilvia.push_back(atof(runFilter.getElementFromPropertyContainer(i,6).c_str()));
  }
  
  
  Run2011.LoadSelectionByDO(&runFilter, filterCol);
  
  //Run2011.PrintValidRuns();
  
  TFile* f = new TFile("Outputs/OutputFile.root", "RECREATE");
  TH2F* plot = new TH2F("RateVsLumi", "Rate Vs Lumi", 2000, 0, 2000, 100, 0, 10);
  
  plot->SetMarkerStyle(21);
  contFile->Close();
  
  int n=0; 
  int nSkip = 100;

  
  while (Run2011.hasNext()){
    
    n++;
    double NormalizedRate = 0;
    
    Run* currentRun = Run2011.next();
    //check on Runs
    //    if (n <= nSkip) continue;
    
    //otherwise no directories
    contFile = TFile::Open("Container20062011.root");
    currentRun->FillFromContainer(contFile, areaMap);
    contFile->Close();
    
    if (currentRun->getID() > 163869) break; //163869
    if (!Run2011.isValid(currentRun)) continue;
    
    //    cout << "continue on: " << currentRun->getID() << endl;
    double TotRate = 0;
    double TotArea = 0;
    int totAmount = 0;
    int k=0;    
    if (currentRun->isFilled() == true){
      
      const map<string,ExRoll* > barrelMap = currentRun->GetBarrel();
      
      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){
	
	/*
	//Typical selection for Endcap
	//like RE+3_2_10
       	if (atoi(it->second->getRollID().c_str()) != 10) continue;
	if (it->second->getSector() != 2) continue;
	if (it->second->getWheel_Disk_Number() != 3) continue;
	*/

	
	//Typical selection for Barrel
	//like tot_W+1_RB4_04
	if (it->second->getWheel_Disk_Number() != 2) continue;
	if (it->second->getRollID() != "RB2in") continue;
	if (it->second->getSector() != 4) continue;
//	if ( it->second->getOptionalRollID().compare("F") ) continue;
//	cout << "SEC " << it->second->getSector()  << endl;
	cout << "OPT: " << it->second-> getOptionalRollID() << endl;
	cout << "PASS: " << it->second->getFullOnlineRollID() << endl;

	//Roll Rate
        for (int k=1; k<97; k++){
          ExStrip * strip = it->second->getStrip(k);
          TotRate+= strip->getRate();
	  if (TotRate == 0)
	    delete strip;
       	}
	
      	//Roll Area
        double rollArea = it->second->getRollArea();
        TotArea += rollArea;
	cout << rollArea << endl;
      }
      double NormalizedRate = TotRate / TotArea;
      
        if (TotRate != 0 && TotArea != 0 && currentRun->getDelivered() != 0 && currentRun->isFilled()){
		
	cout << currentRun->getID() << " "
	     << currentRun->getLumi() << " "
	     <<  "; rate: " << TotRate << "; area: "
	     << TotArea << "; norm: " 
	     << NormalizedRate << endl;

 	plot->Fill(currentRun->getLumi(), NormalizedRate);
	k++;
      }
    }
  }
  
    
  f->WriteObject(plot, "Plot");
  
  return 0;
}
