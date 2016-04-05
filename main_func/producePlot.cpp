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
#include "../ROOT/ROOT.h"
#include "../Extensions/Run.h"
#include "../Extensions/Acquisition.h"
#include "../Extensions/NoiseAnalyzer.h"

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
  bool corrected = true;
  /*  string zone = "EndCapPlus";
  string RollID = "10";
  int Sector = 2;
  int WheelDiskNo = 2;
  */

  string zone = argv[1];                                                                                                                                                  
  string RollID = argv[4]; 
  int Sector = atoi(argv[3]);                                                                                                                                          
  int WheelDiskNo = atoi(argv[2]);                                                                                                                                                            
  string optionalID = argv[5];

  string ContainerName = "ContainerRun2011.root";
  string jsonFile = "./jsonFilesMu/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt";
  //Create a new Acquisition Registry

  TFile* contFile = TFile::Open( ContainerName.c_str() );
  
	Acquisition Run2011(contFile);
/*	ifstream json(jsonFile.c_str());
	Run2011.LoadJSON(json);*/


  DataObject filterRun;
  filterRun.getDataFromFile("filters/moreThan100.csv",1);  
  Run2011.LoadSelectionByDO(&filterRun, 1);

      TFile* f;
      if (argc >= 6){
	f = new TFile(argv[6], "RECREATE");
          cout << "Writing results on: " << argv[6] << endl;
}
      else{
	f = new TFile("Outputs/OutputFile.root", "RECREATE");
	cout << "Writing results on: Outputs/OutputFile.root"  << endl;
}

  TGraph* plot = new TGraph();
  TGraph* plotRun = new TGraph();

  plot->SetName("RateVsLumi");
  plotRun->SetName("RateVsRun");

  plot->SetMarkerStyle(21);
  contFile->Close();
    int k=0;    
    int emptyStrip = 0;

	cout << "Analysis of: " << zone << " - Wheel/Disk No." << WheelDiskNo << " ; Sector: " << Sector << " ; ID:" << RollID << endl;
	cout << "Correction activated: " << corrected<< endl;
	cout << "Dataset: " << ContainerName << endl;
	cout << "JSON File: " << jsonFile << endl;

// Run2011.PrintValidRuns();
  cout << "Number of runs: " << Run2011.numberOfRuns() << endl;
  while (Run2011.hasNext()){
    
    double NormalizedRate = 0;
    
    Run* currentRun = Run2011.next();

    //run selection
//    if ( currentRun->getID() > 163289 && currentRun->getID() < 163340 ) continue;
//    if (currentRun->getID() < 170000) continue;
//    if (currentRun->getLumiSection() < 100) continue;
//    if (currentRun->getID() < 178420) continue;
    if (!Run2011.isValid(currentRun, false)) continue;
    
    //Filling
      contFile = TFile::Open( ContainerName.c_str() );

    currentRun->FillFromContainer(contFile, areaMap);

        
    double TotRate = 0;
    double TotArea = 0;
    int totAmount = 0;
    if (currentRun->isFilled() == false){
	cout << "Not filled: " << currentRun->getID() << endl;
    }
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

	//When we will decide to plot partition by partition
	if (optionalID != "-9"){
//	cout << optionalID << endl;
//	cout << it->second->getOptionalRollID() << endl;
                if ( it->second->getOptionalRollID().compare(optionalID) ) continue;
	}

      
        emptyStrip += it->second->getEmptyStrip();
//	if ( it->second->getOptionalRollID().compare("B") ) continue;
//	cout << "SEC " << it->second->getSector()  << endl;
/*	cout << "OPT: " << it->second-> getOptionalRollID() << endl;
	cout << "RAW: " << it->second->getRawIDofClone(1) << endl;*/
//	cout << "PASS: " << it->second->getFullOnlineRollID() << endl;
/*	TotRate += it->second->CalculateRateFromTH1(corrected);
	string th1path = it->second->getTH1Path();
	TH1F* histo = (TH1F*)(contFile->Get(th1path.c_str()));*/


        NoiseAnalyzer nAnalyzer;
        TotRate += nAnalyzer.HealCloneByClone(it->second);
//	cout << "VALUE: " << nAnalyzer.HealCloneByClone(it->second) << endl;
        double rollArea = it->second->getRollArea();
        TotArea += rollArea;	
      }

      //Free RAM
      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){
	delete it->second;
      }

      double NormalizedRate = TotRate / TotArea;
//       cout << NormalizedRate << endl;	      
  string zone = argv[1];                                                                                                                                                  
  string RollID = argv[4]; 
  int Sector = atoi(argv[3]);                                                                                                                                          
  int WheelDiskNo = atoi(argv[2]);     


        if (TotRate != 0 && TotArea != 0 && currentRun->getDelivered() != 0 && currentRun->isFilled()){
//	currentRun->PrintStatistics();	
             
             cout << currentRun->getID() <<
	     "\t" << currentRun->getLumiSection() << "\t" << currentRun->getDelivered() 
	     << "\t" << currentRun->getLumi() <<
	     "\t" << TotRate << "\t" << TotArea << "\t" << NormalizedRate/2 << endl;
		
// 	plot->SetBinContent(plot->FindBin(currentRun->getLumi()), NormalizedRate);
	cout << "POINT: " << k << " "  << currentRun->getLumi()<< " " << NormalizedRate / 2 << endl;; 
	plot->SetPoint(k , currentRun->getLumi(), NormalizedRate / 2);
	plotRun->SetPoint(k, currentRun->getID(), NormalizedRate / 2);
	k++;
	}
    }
    delete currentRun;
    contFile->Close();  
    contFile->Delete();
  }

  plot->SetDrawOption("P");
  f->WriteObject(plotRun, plotRun->GetName());
  f->WriteObject(plot, plot->GetName());

  
  return 0;
}
