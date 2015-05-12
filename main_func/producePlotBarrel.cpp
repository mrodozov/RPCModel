#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
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

  map<string, double> areaMap;
  
  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }

  //Create a new Acquisition Registry
  TFile* contFile = TFile::Open("Container.root");
  Acquisition Run2011(contFile);
  TFile* f = new TFile("OutputFile.root", "RECREATE");
  TH2F* plot = new TH2F("RateVsLumi", "Rate Vs Lumi", 2000, 0, 2000, 100, 0, 100);
  contFile->Close();
  while (Run2011.hasNext()){

    double NormalizedRate = 0;

    Run* currentRun = Run2011.next();
    //otherwise no directories
    contFile = TFile::Open("Container.root");
    currentRun->FillFromContainer(contFile, areaMap);
    contFile->Close();

    double TotRate = 0;
    double TotArea = 0;
    int totAmount = 0;

    if (currentRun->isFilled() == true){

      const map<string,ExRoll* > barrelMap = currentRun->GetBarrel();

      for (map<string,ExRoll* >::const_iterator it = barrelMap.begin(); it != barrelMap.end(); it++){

       //Roll Rate
        for (int k=1; k<=96; k++){
          ExStrip * strip = it->second->getStrip(k);
          TotRate = TotRate + strip->getRate();
       	}

      	//Roll Area
        double rollArea = it->second->getRollArea();
        TotArea = TotArea + rollArea;
      }
      double NormalizedRate = TotRate / TotArea;

      if (TotRate != 0 && currentRun->getDelivered() != 0 ){
        cout << currentRun->getID() << " " << currentRun->getLumi() << " " << TotRate << " " << TotArea << " " << NormalizedRate <<endl;
 	plot->Fill(currentRun->getLumi(), NormalizedRate);
	cout  << currentRun->getLumi() << " " << NormalizedRate << endl; 
  }
    }
  }
  f->WriteObject(plot, "Plot");
  return 0;
}
