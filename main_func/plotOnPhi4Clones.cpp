#include "Extensions/NoiseAnalyzer.h"
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

using namespace std;

int main(int argc, char** argv){

  if (argc < 5) {
	cout << "Usage: " << argv[0] << "SubDet RunNumber Wheel RID PartitionName [OutFile]" << endl;
	return 0;
  }
  
  string RollID = string(argv[4]);
  int WheelDiskNo = atoi(argv[3]);
  int runNumber = atoi(argv[2]);
  string subDet = argv[1];
  int partitionNumber = atoi(argv[5]);
  cout << "You're asking partition " << partitionNumber << endl;
  string ContainerName ="ContainerRun2011.root";
  //Load area map
  DataObject areas;
  areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);

  map<string, double> areaMap;

  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }

  TFile* contFile = TFile::Open(ContainerName.c_str());
  Acquisition Run2011(contFile);

  
  
  //  map<string, TGraph*> RollIDs;
  map<string, TGraph*> RollIDs;
  
  TFile* f;
  
  if (argc >= 7)
    f = new TFile(argv[6], "RECREATE");
  else
    f = new TFile("Outputs/OutputPlotOnPhiVR.root", "RECREATE");
  
  //  TGraph* plot = new TGraph();
  TGraph* plot = new TGraph();
  plot->SetName("RateVsPhi");
  
  //Load Run
  Run* currentRun = Run2011.getRunById(runNumber) ;
  
  contFile = TFile::Open(ContainerName.c_str());
  currentRun->FillFromContainer(contFile, areaMap);
  contFile->Close();
  currentRun->PrintStatistics();
  
  map<string,ExRoll* > Rolls; 
  vector<TGraph*> vPlot;
  int minSector = 1;
  int nSector;
  int emptyStrip = 0;
  int runIndex = -1;  

  
  cout << "Loading Sectors..." << endl;
  
  //Filling Plots
  if (subDet == "Barrel"){
    
    RollIDs["RB1in"] = new TGraph();
    RollIDs["RB2in"] = new TGraph();
    RollIDs["RB1out"] = new TGraph();
    RollIDs["RB2out"] = new TGraph();
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
  
  //Filling                                                                                                         
  contFile = TFile::Open( ContainerName.c_str() );
  
  cout << "Begin Loop" << endl;  
  for (map<string, TGraph*>::const_iterator itRoll = RollIDs.begin(); itRoll != RollIDs.end(); itRoll++){
    int nPoint = 0;
  bool setSize = false;    
    string name;
    
    if (subDet == "Barrel")
      name = "Barrel_" + itRoll->first;
    else if (subDet == "EndCapPlus" || subDet == "EndCapMinus")
      name = "EndCap_" + itRoll->first;
    
    itRoll->second->SetName(name.c_str());


 
  for (int iSector = minSector; iSector <= nSector; iSector++){
    
    double TotRate = 0;
    double TotArea = 0;
    
    
    for (map<string,ExRoll* >::const_iterator it = Rolls.begin(); it != Rolls.end(); it++){

      string th1path = it->second->getTH1Path();
      
      if (subDet == "Barrel"){
	string opt1 = "F";
	string opt2 = "+F";
	string opt3 = "-F";

	if (it->second->getSector() != iSector) continue;     
	
	if (it->second->getRollID() != itRoll->first) continue;      
	

	if (WheelDiskNo != -9)
	  if (it->second->getWheel_Disk_Number() != WheelDiskNo) continue;

	if (it->second->getOptionalRollID().find(opt1) == string::npos && it->second->getOptionalRollID().find(opt2) == string::npos) {
		 continue;
	}	
	if (it->second->getOptionalRollID().find(opt3) != string::npos){
		continue;
	}	
	
	
      }
      
      else if (subDet == "EndCapPlus" || subDet == "EndCapMinus"){
	
	if (atoi(it->second->getRollID().c_str()) != iSector) continue;
        
	if (it->second->getSector() != atoi(itRoll->first.c_str())) continue;
	
	if (WheelDiskNo != -9)
	  if (it->second->getWheel_Disk_Number() != WheelDiskNo) continue;
      }
      int nClones = it->second->getClones();
      emptyStrip += it->second->getEmptyStrip();

      TH1F* histo = (TH1F*)(contFile->Get(th1path.c_str()));

      int cNumber = partitionNumber;
      if (partitionNumber > nClones-1) continue;
      cout << "This roll has " << nClones << endl;
      double TotRate = 0;
      double TotArea = 0;
      int cEffectiveNumber =  cNumber + 1;
      
      NoiseAnalyzer nAnalyzer;
      TH1F* cHisto = it->second->getRateHistoForClone(histo, cEffectiveNumber);
      TotRate = nAnalyzer.HealClone(it->second, cHisto, cEffectiveNumber);
      double cloneArea = it->second->getCloneArea(cNumber);
      TotArea += cloneArea;
     
      double NormalizedRate = (TotRate / TotArea);
      itRoll->second->SetPoint(nPoint, (iSector-1)*6.28/(double)nSector, NormalizedRate/2);
      cout << "\t\tPOINT: " << nPoint << " " << (iSector-1)*6.28/(double)nSector << " " << NormalizedRate/2 << endl;
      nPoint++;
    }
  }  
  }


  for (map<string, TGraph*>::const_iterator it = RollIDs.begin(); it != RollIDs.end(); it++)
    f->WriteObject( it->second, it->second->GetName() );
  
  f->Close();
  return 0;
}

