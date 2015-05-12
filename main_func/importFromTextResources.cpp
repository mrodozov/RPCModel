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
#include <iomanip>

using namespace std;

int main( int argc ,char *argv[] ){

   DataObject areas;
   areas.getDataFromFile("resources/static_resources/area_noise_CMSSW_IDS.txt",2);

   
   string Suffix = string(argv[1]);
   string Year = string(argv[2]);

   DataObject lumiDeli;
   int nColoumnsLumiDeli = 5;

   string FileName = "ContainerRun" + Year + "_lumiCalc2v4" + Suffix + ".root";
   cout << "The new Resource will be: " << FileName << endl;

   string resFile = string("./resources/Extensions/" + Year + "/JSONLumiCalc2v4_" + Year + "_" + Suffix );
   //lumiDeli.getFastDataFromFile(resFile, nColoumnsLumiDeli);
   lumiDeli.getDataFromFile(resFile,nColoumnsLumiDeli); // same as above, uses your algorithm 
   cout << "Resource file: " << resFile << endl;

   map<string, double> areaMap;
   Acquisition RunContainer;

  //Load Areas from DataObject
  for (int i=1; i<= areas.getLenght(); i++){
    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
  }

  //Load luminosity infos
  for (int j=1; j<= lumiDeli.getLenght(); j++){

    int RunID = atof(lumiDeli.getElementFromPropertyContainer(j,1).c_str());
    double Lumi = atof(lumiDeli.getElementFromPropertyContainer(j,2).c_str());
    double Deli = atof(lumiDeli.getElementFromPropertyContainer(j,3).c_str());

    string EndTime;
    string BeginTime;

   //load time informations (if available)
    string aBeginDate = string(lumiDeli.getElementFromPropertyContainer(j,4).c_str());
    string aBeginTime = string(lumiDeli.getElementFromPropertyContainer(j,5).c_str());
    BeginTime = aBeginDate + " " + aBeginTime;

    /*
    string aEndDate = string(lumiDeli.getElementFromPropertyContainer(j,4).c_str());
    string aEndTime = string(lumiDeli.getElementFromPropertyContainer(j,5).c_str());
    EndTime = aEndDate + " " + aEndTime;
    */

    RunContainer.AddRun(RunID, Year);
    Run* currentRun =  RunContainer.getRunById(RunID);
    currentRun->setLumi(Deli, Lumi);
    currentRun->setBeginTime(BeginTime);

  }

  TFile* contFile;
  contFile = TFile::Open(FileName.c_str(), "UPDATE");
  bool isZombie = false;
  map<int, bool> ExistingRuns;

  //If the file already exist update just all the not existing runs...
  cout << "Check if container already esists..." << endl;
  if (!contFile || contFile->IsZombie()){
    cout << "A previous ContainerRun" << Year <<  ".root does not exists or it is a zombie..." << endl;
    contFile = new TFile(FileName.c_str(), "RECREATE");
    isZombie = true;
  }
  else{
    TKey *key;
    TIter nextkey(contFile->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
      const char *classname = key->GetClassName();
      TClass *cl = gROOT->GetClass(classname);
      if (!cl) continue;
      if (cl->InheritsFrom(TDirectory::Class())) {
	int RunNumber = atoi(key->GetName());
	ExistingRuns[RunNumber] = true;
	cout << "existing run: " << RunNumber << endl;
      }
    }
  }

  RunContainer.begin();
  cout << "Filling Runs..." << endl;
  cout << RunContainer.numberOfRuns() << endl;
  int counter = 0;

  while (RunContainer.hasNext()){

    double NormalizedRate = 0;
    Run* currentRun = RunContainer.next();

    //skip working on this run if it already exist
    if (isZombie == false){
 	if (ExistingRuns[currentRun->getID()]){
		cout << "Run " << currentRun->getID() << " will be skipped, beacause already exist." << endl;
		continue;
	}
    }

    if (currentRun)
	currentRun->Fill(areaMap);
  }
  cout << "Writing on file..." << endl;

  //This method accept a pointer to a map with the existing runs not to be written.
  RunContainer.WriteAcquisition(contFile, &ExistingRuns);

  contFile->Close();
  return 0;
  
}
