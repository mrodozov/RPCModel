
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <cmath>
#include <stdlib.h>
/** non-standart headers , the model , the wrappers and ROOT */
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../Extensions/Run.h"
#include "../Extensions/Acquisition.h"
#include "../ROOT/ROOT.h"

using namespace std;

int main(int argc, char** argv){

  if (argc < 5) {
	cout << "Usage: " << argv[0] << "SubDet RunNumber Wheel RID [OutFile]" << endl;
	return 0;
  }
  
  string RollID = string(argv[4]);
  int WheelDiskNo = atoi(argv[3]);
  int runNumber = atoi(argv[2]);
  string subDet = argv[1];

  //Load area map
  DataObject areas;
  areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);

  map<string, double> areaMap;

  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }

  TFile* contFile = TFile::Open("Container_160954-176309.root");
  Acquisition Run2011(contFile);
  contFile->Close();


//  map<string, TGraph*> RollIDs;
  map<string, TGraph*> RollIDs;

  TFile* f;

  if (argc >= 6)
    f = new TFile(argv[5], "RECREATE");
  else
    f = new TFile("Outputs/OutputPlotOnPhiVR.root", "RECREATE");
  
//  TGraph* plot = new TGraph();
  TGraph* plot = new TGraph();
  plot->SetName("RateVsPhi");
  
  //Load Run


  Run* currentRun = Run2011.getRunById(runNumber) ;

  contFile = TFile::Open("Container_160954-176309.root");
  currentRun->FillFromContainer(contFile, areaMap);
  contFile->Close();
  
  map<string,ExRoll* > Rolls; 

  int minSector = 1;
  int nSector;

  cout << "Loading Sectors..." << endl;

  //Filling Plots
  if (subDet == "Barrel"){

	RollIDs["RB1in"] = new TGraph();
	RollIDs["RB2in"] = new TGraph();
	RollIDs["RB3"] = new TGraph();
	RollIDs["RB4"] = new TGraph();
        nSector = 12;
	Rolls = currentRun->GetBarrel();
  }

  else if (subDet == "EndCapPlus"){

	RollIDs["2"] = new TGraph();
	RollIDs["3"] = new TGraph();
        nSector = 36;

	Rolls = currentRun->GetEndCapPlus();
  }

  else if (subDet == "EndCapMinus"){

    RollIDs["2"] = new TGraph();
    RollIDs["3"] = new TGraph();
    nSector = 36;

    Rolls = currentRun->GetEndCapMinus();
  }

  for (map<string, TGraph*>::const_iterator itRoll = RollIDs.begin(); itRoll != RollIDs.end(); itRoll++){
    
    int nPoint = 0;
    string name;
    
    if (subDet == "Barrel")
      name = "Barrel_" + itRoll->first;
    else if (subDet == "EndCapPlus")
      name = "EndCapPlus_" + itRoll->first;
    else if (subDet == "EndCapMinus")
      name = "EndCapPlus_" + itRoll->first;

    itRoll->second->SetName(name.c_str());

    double TotRate = 0;
    double TotArea = 0;
    
    for (int iSector = minSector; iSector < nSector; iSector++){
      
      for (map<string,ExRoll* >::const_iterator it = Rolls.begin(); it != Rolls.end(); it++){
	
	if (subDet == "Barrel"){
	  
	  if (it->second->getSector() != iSector) continue;     
	  
	  if (it->second->getRollID() != itRoll->first) continue;      
	  
	  
	  if (WheelDiskNo != -9)
	    if (it->second->getWheel_Disk_Number() != WheelDiskNo) continue;
	}
	
	else if (subDet == "EndCap"){
	
	  if (atoi(it->second->getRollID().c_str()) != iSector) continue;
	  
	  if (it->second->getSector() != atoi(itRoll->first.c_str())) continue;
	
	  if (WheelDiskNo != -9)
	    if (it->second->getWheel_Disk_Number() != WheelDiskNo) continue;
	
	}
	cout << "PASS: " << it->second->getFullOnlineRollID() << endl;
	
	//      TotRate += it->second->getRollRate();
        TotRate += it->second->CalculateRateFromTH1(true);
	
	cout<< TotRate << endl;      
	
	double rollArea = it->second->getRollArea();
	
	TotArea += rollArea;
      }
    
    double NormalizedRate = (TotRate / TotArea);
    itRoll->second->SetPoint(nPoint, iSector*6.28/(double)nSector, NormalizedRate/2);
    cout << "\t\tPOINT: " << nPoint << " " << iSector*6.28/(double)nSector << " " << NormalizedRate/2 << endl;
    nPoint++;
  }

  }

  for (map<string, TGraph*>::const_iterator it = RollIDs.begin(); it != RollIDs.end(); it++)
	  f->WriteObject( it->second, it->second->GetName() );
  f->Close();
  return 0;
}
