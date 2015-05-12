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
#include "../Extensions/NoiseAnalyzer.h"
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

  TFile* contFile = TFile::Open("ContainerRun2011.root");
  Acquisition Run2011(contFile);
  contFile->Close();

//  map<string, TGraph*> WDNo;
  map<string, TGraph*> WDNo;

  TFile* f;

  if (argc >= 6)
    f = new TFile(argv[5], "RECREATE");
  else
    f = new TFile("Outputs/OutputPlotOnPhiVZ.root", "RECREATE");
  
//  TGraph* plot = new TGraph();
  TGraph* plot = new TGraph();
  plot->SetName("RateVsPhi");
  
  //Load Run
  Run* currentRun = Run2011.getRunById(runNumber) ;

  contFile = TFile::Open("ContainerRun2011.root");
  currentRun->FillFromContainer(contFile, areaMap);
  contFile->Close();
  currentRun->PrintStatistics();
  
  map<string,ExRoll* > Rolls; 

  int minSector = 1;
  int nSector;

  cout << "Loading Sectors..." << endl;

  //Filling Plots
  if (subDet == "Barrel"){

	WDNo["-2"] = new TGraph();
	WDNo["-1"] = new TGraph();
	WDNo["+0"] = new TGraph();
	WDNo["+1"] = new TGraph();
	WDNo["+2"] = new TGraph();
	Rolls = currentRun->GetBarrel();
	nSector = 12;
  }

  else if (subDet == "EndCapPlus"){

	WDNo["1"] = new TGraph();
	WDNo["2"] = new TGraph();
	WDNo["3"] = new TGraph();
	nSector = 36;

	Rolls = currentRun->GetEndCapPlus();
  }

  else if (subDet == "EndCapMinus"){

        WDNo["-1"] = new TGraph();
        WDNo["-2"] = new TGraph();
        WDNo["-3"] = new TGraph();
        nSector = 36;

        Rolls = currentRun->GetEndCapMinus();
  }

  for (map<string, TGraph*>::const_iterator itRoll = WDNo.begin(); itRoll != WDNo.end(); itRoll++){
    int nPoint =0 ;

  cout << "******* " << itRoll->first.c_str() << endl;
	string name;

	if (subDet == "Barrel")
		name = "Barrel" + itRoll->first;
	else if (subDet == "EndCapPlus" || subDet == "EndCapMinus" )
		 name = "EndCap" + itRoll->first;

	itRoll->second->SetName(name.c_str());

  for (int iSector = minSector; iSector <= nSector; iSector++){

    double TotRate = 0;
    double TotArea = 0;
    for (map<string,ExRoll* >::const_iterator it = Rolls.begin(); it != Rolls.end(); it++){
 
      if (subDet == "Barrel"){
	string opt1 = "F";
	string opt2 = "+F";
	string opt3 = "-F";

      if (it->second->getSector() != iSector) continue;     

      if (it->second->getRollID() != RollID) continue;      

      if (it->second->getWheel_Disk_Number() !=  atoi(itRoll->first.c_str())) continue;

       if (it->second->getOptionalRollID().find(opt1) == string::npos && it->second->getOptionalRollID().find(opt2) == string::npos) {
        	  continue;
	        }
       if (it->second->getOptionalRollID().find(opt3) != string::npos) continue;


      }

      else if (subDet == "EndCapPlus" || subDet == "EndCapMinus"){

 
          if (atoi(it->second->getRollID().c_str()) != iSector) continue;
        
	  if (it->second->getSector() != atoi(RollID.c_str())) continue;

        cout << "\t\tPASS: " << it->second->getFullOnlineRollID() << endl;

	  if (it->second->getWheel_Disk_Number() != atoi(itRoll->first.c_str())) continue;

      }

      cout << "\t\tPASS: " << it->second->getFullOnlineRollID() << endl;

     // TotRate += it->second->getRollRate();
/*
	TotRate += it->second->CalculateRateFromTH1(true);
      double rollArea = it->second->getRollArea();
      TotArea += rollArea;*/

        NoiseAnalyzer nAnalyzer;
        TotRate += nAnalyzer.HealCloneByClone(it->second);
//      cout << "VALUE: " << nAnalyzer.HealCloneByClone(it->second) << endl;
        double rollArea = it->second->getRollArea();
        TotArea += rollArea;  
      
    }
    
    double NormalizedRate = TotRate / TotArea;
    itRoll->second->SetPoint(nPoint, (iSector-1)*6.28/(double)nSector, NormalizedRate/2);
    cout << "POINT: " << nPoint<<" " << (iSector-1) *6.28/(double)nSector<< " " << NormalizedRate/2 << endl;
    nPoint++;
  }

  }

  for (map<string, TGraph*>::const_iterator it = WDNo.begin(); it != WDNo.end(); it++)
	  f->WriteObject( it->second, it->second->GetName() );
  f->Close();
  return 0;
}
