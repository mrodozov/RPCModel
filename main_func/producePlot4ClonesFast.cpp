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
  
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " RollName [OutFile]" << endl;
    return 0;
  }
  
  string rName = argv[1];
  bool ActivateCorrection = true;
  string CorrDir;

  if (ActivateCorrection) 
	CorrDir = "Corrected/";
  else
	CorrDir = "UnCorrected/";

 //Settings 2011
  string ContainerName = "ContainerRun2011.root";
  string jsonFile = "./jsonFilesMu/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt";
  string dirOut = "Outputs/AllTheClones/2011/" + CorrDir;

  
  /*
  //Settings 2012  
  string ContainerName = "ContainerRun2012.root";
  string jsonFile = "./jsonFilesMu/Cert_190456-193336_8TeV_PromptReco_Collisions12_JSON.txt";
  string dirOut = "Outputs/AllTheClones/2012/" + CorrDir;
*/
  //Create a new Acquisition Registry
  
  TFile* contFile = TFile::Open( ContainerName.c_str() );
  cout << contFile->GetName() << endl;  
  Acquisition RunContainer(contFile);
  ifstream json(jsonFile.c_str());
  RunContainer.LoadJSON(json);
  
  TFile* f;
  if (argc >= 3)
    f = new TFile(argv[2], "RECREATE");
  else{
    f = new TFile(string(dirOut + rName + ".root").c_str(), "RECREATE");
  }
  
  vector<TGraph*> vPlot;
  vector<TGraph*> vPlotRuns;
  
  contFile->Close();
  int k=0;    
  int emptyStrip = 0;
  
  cout << "Analysis of          : " << rName              << endl;
  cout << "Correction activated : " << ActivateCorrection << endl;
  cout << "Dataset              : " << ContainerName      << endl;
  cout << "JSON File            : " << jsonFile           << endl;
  
  bool setSize = false;
  int runIndex = -1;
  
  while (RunContainer.hasNext()){
    
    
    double NormalizedRate = 0;
    Run* currentRun = RunContainer.next();
    
    //run selection
    if (currentRun->getLumiSection() < 100) continue;
    if (!RunContainer.isValid(currentRun, true)) continue;
    
    //Filling
    contFile = TFile::Open( ContainerName.c_str() );
    currentRun->FillFromContainer(contFile, areaMap, rName);
    
    int totAmount = 0;
    if (currentRun->isFilled() == true){
      runIndex++;
      
      map<string,ExRoll* > barrelMap;
      barrelMap = currentRun->GetAllTheRolls();
      
      ExRoll* cRoll = barrelMap[rName];
      if (!cRoll) continue;
      int nClones = cRoll->getClones();	
      if (setSize == false){
	vPlot.resize(nClones);
	vPlotRuns.resize(nClones);
	for (int l=0; l<vPlot.size(); l++){
	  vPlot[l] = new TGraph();
	  vPlotRuns[l] = new TGraph();
	}
	setSize = true;
      }
      
      emptyStrip += cRoll->getEmptyStrip();
      
      string th1path = cRoll->getTH1Path();
      TH1F* histo = (TH1F*)(contFile->Get(th1path.c_str()));
      
      
      for(int cNumber = 0; cNumber < nClones; cNumber++){
	
	double TotRate = 0;
	double TotArea = 0;
	int cEffectiveNumber =  cNumber + 1;
	NoiseAnalyzer nAnalyzer;
	TH1F* cHisto = cRoll->getRateHistoForClone(histo, cEffectiveNumber);
	TotRate = nAnalyzer.HealClone(cRoll, cHisto, cEffectiveNumber, ActivateCorrection);
	//	  delete cHisto;
	cout << "PASS: " << cRoll->getFullOnlineRollID() << endl;
	double cloneArea = cRoll->getCloneArea(cNumber);
	
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
      
      
      //Free RAM
      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){
	delete cRoll;
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
  f->Close();
  return 0;
}
