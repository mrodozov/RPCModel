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
//#include "TH1F.h"
//#include "TFile.h"
//#include "TGraph.h"

using namespace std;

int main( int argc ,char *argv[] ){
  
  //Create a new Acquisition Registry
  TFile* contFile = TFile::Open("Container20062011.root");
  Acquisition Run2011(contFile);
  
//  ifstream json("./jsonFilesMu/Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt");
    ifstream json("./jsonFilesMu/Cert_160404-167913_7TeV_PromptReco_Collisions11_JSON.txt");
      Run2011.LoadJSON(json);

//  DataObject filterRun;
//  filterRun.getDataFromFile("filters/collisions2011_167913_runNames.txt",1);  
//  Run2011.LoadSelectionByDO(&filterRun, 1);
  //Run2011.PrintValidRuns();
  
  while (Run2011.hasNext()){
    
    Run* currentRun = Run2011.next();
    
    //run selection
//    if (currentRun->getID() > 163869) break;
    if (currentRun->getLumiSection() < 100) continue;
    if (!Run2011.isValid(currentRun, false)) continue;
    
    //Filling
      if (currentRun->isFilled() == true){
      currentRun->PrintStatistics();
    }
  }  
  return 0;
  }
  
