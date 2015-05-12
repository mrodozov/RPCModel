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
#include "Extensions/NoiseAnalyzer.h"
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
  /*  
      string zone = "EndCapPlus";
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
  ifstream json(jsonFile.c_str());
  Run2011.LoadJSON(json);
  
  /*
    DataObject filterRun;
    filterRun.getDataFromFile("filters/collisions2011_167913_runNames.txt",1);  
    Run2011.LoadSelectionByDO(&filterRun, 1);*/

  TFile* f;
  if (argc >= 7)
    f = new TFile(argv[6], "RECREATE");
  else
    f = new TFile("Outputs/OutputFile.root", "RECREATE");

  
  
  vector<TGraph*> vPlot;
  vector<TGraph*> vPlotRuns;
  
    

  contFile->Close();
  int k=0;    
  int emptyStrip = 0;
  
  cout << "Analysis of: " << zone << " - Wheel/Disk No." << WheelDiskNo << " ; Sector: " << Sector << " ; ID:" << RollID << endl;
  cout << "Correction activated: " << corrected<< endl;
  cout << "Dataset: " << ContainerName << endl;
  cout << "JSON File: " << jsonFile << endl;
  
  
  bool setSize = false;
  int runIndex = -1;
  
  while (Run2011.hasNext()){
    
    
    double NormalizedRate = 0;
    Run* currentRun = Run2011.next();
    
    //run selection
    if (currentRun->getLumiSection() < 100) continue;
    if (!Run2011.isValid(currentRun, true)) continue;
    
    //Filling
    contFile = TFile::Open( ContainerName.c_str() );
    currentRun->FillFromContainer(contFile, areaMap);
    
    int totAmount = 0;
    
    if (currentRun->isFilled() == true){
      runIndex++;
      
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
	  if ( it->second->getOptionalRollID().compare(optionalID) ) continue;
        }

	int nClones = it->second->getClones();	
	if (setSize == false){
	  vPlot.resize(nClones);
	  vPlotRuns.resize(nClones);
	  for (int l=0; l<vPlot.size(); l++){
		vPlot[l] = new TGraph();
		vPlotRuns[l] = new TGraph();
	  }
	  setSize = true;
	}
	
        emptyStrip += it->second->getEmptyStrip();
	
	string th1path = it->second->getTH1Path();
	TH1F* histo = (TH1F*)(contFile->Get(th1path.c_str()));

	
	for(int cNumber = 0; cNumber < nClones; cNumber++){

	double TotRate = 0;
	double TotArea = 0;
	int cEffectiveNumber =  cNumber + 1;
	  NoiseAnalyzer nAnalyzer;
	  TH1F* cHisto = it->second->getRateHistoForClone(histo, cEffectiveNumber);
	  TotRate = nAnalyzer.HealClone(it->second, cHisto, cEffectiveNumber);
//	  delete cHisto;
          cout << "PASS: " << it->second->getFullOnlineRollID() << endl;
	  double cloneArea = it->second->getCloneArea(cNumber);

	  TotArea = cloneArea;
	  NormalizedRate = TotRate / TotArea;

	  if (TotRate != 0 && TotArea != 0 && currentRun->getDelivered() != 0 && currentRun->isFilled()) {
	    //	currentRun->PrintStatistics();	
	    
	    cout <<cNumber << " " << runIndex << " "  << currentRun->getID() <<
	      "\t" << currentRun->getLumiSection() << "\t" << currentRun->getDelivered() 
		 << "\t" << currentRun->getLumi() <<
	      "\t" << TotRate << "\t" << TotArea << "\t" << NormalizedRate << endl;
	  
	    vPlot.at(cNumber)->SetPoint(runIndex , currentRun->getLumi(), NormalizedRate);
	    vPlotRuns.at(cNumber)->SetPoint(runIndex, currentRun->getID(), NormalizedRate);
	    k++;
	    
	  }
	  
	}//end loop on clones
      } //end loop on rolls

      //Free RAM
      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){
	delete it->second;
      }
      
      delete currentRun;
      contFile->Close();  
      
    }//end of filled runs
  }//end on run loop
  
  for (int k=0; k<vPlot.size();k++){
    stringstream ss;
    ss << k+1;
    string lumiName = "RateVsLumi_" +  ss.str();
    string runName = "RateVsRun_" +  ss.str();

    vPlot.at(k)->SetName( lumiName.c_str() );
    vPlotRuns.at(k)->SetName( runName.c_str() );

    f->WriteObject(vPlot.at(k), vPlot.at(k)->GetName() );
    f->WriteObject(vPlotRuns.at(k), vPlotRuns.at(k)->GetName() );
  }
  
  return 0;
}
