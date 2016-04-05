#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
/** non-standart headers , the model , the wrappers and ROOT */
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../ROOT/ROOT.h"
#include "../Extensions/Run.h"
#include "../Extensions/Acquisition.h"

using namespace std;

/*
Just fill a ROOT file container starting from infos kept 
in resource file (Areas, and Params)
*/
int main( int argc ,char *argv[] ){
  
  DataObject DataRun;
  int numberOfFields = 7;
  
  //Scan data file which contains all the runs features
  DataRun.getDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/tmpFiles/tmpOut",numberOfFields);

  DataObject areas;
  areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);

  map<string, double> areaMap;
  
  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }
  
  //Create a new Acquisition Registry
  Acquisition Run2011(&DataRun);
  TFile* contFile = new TFile("Container.root", "UPDATE");

  //SetHV
  Run2011.setParameters(&DataRun, "HVRB", 2);
  double HVRB_Max = 9300;

  cout << "Filling Runs..." << endl;

  while (Run2011.hasNext()){

    double NormalizedRate = 0;
    Run* currentRun = Run2011.next();
    currentRun->Fill(areaMap);
  }

	Run2011.WriteAcquisition(contFile);
  return 0;
  
}
