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

  DataObject listOfRuns; 
  int nColoumnsListOfRuns = 1;
  listOfRuns.getDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/Extensions/2011/listOfRuns",nColoumnsListOfRuns);

  DataObject environParams;
  int nColoumnsParams = 6;
  //environParams.getFastDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/Extensions/2011/environParams",nColoumnsParams);
  environParams.getDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/Extensions/2011/environParams",nColoumnsParams);

  DataObject lumiDeli; 
  int nColoumnsLumiDeli = 3;
  //lumiDeli.getFastDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/Extensions/2011/2011Lumi.txt", nColoumnsLumiDeli);
  environParams.getDataFromFile("/home/fynu/mgabusi/scratch/SWFBCK/resources/Extensions/2011/environParams",nColoumnsParams);
  
  map<string, double> areaMap;

  Acquisition Run2011;

  //Load listOfRuns from DataObject                                                      
  for (int i=1; i<= listOfRuns.getLenght(); i++){

    int ID = atoi(listOfRuns.getElementFromPropertyContainer(i,1).c_str());    
    Run2011.AddRun(ID);

  }

  //Load Areas from DataObject
  for (int i=1; i<= areas.getLenght(); i++){

    double area = atof(areas.getElementFromPropertyContainer(i,2).c_str());
    string roll = areas.getElementFromPropertyContainer(i,1);
    areaMap[roll] = area;
    
  }

  //Load Params from DataObject
  for (int i=1; i<= environParams.getLenght(); i++){

    int RunID = atoi(environParams.getElementFromPropertyContainer(i,1).c_str());
    int HV_Barrel = atof(environParams.getElementFromPropertyContainer(i,2).c_str());
    int HV_EndCap = atof(environParams.getElementFromPropertyContainer(i,3).c_str());
    int Temp = atof(environParams.getElementFromPropertyContainer(i,5).c_str());
    int Pressure = atof(environParams.getElementFromPropertyContainer(i,4).c_str());

    Run* currentRun =  Run2011.getRunById(RunID);
    if (currentRun){
      
      currentRun->setTemperature(Temp);
      currentRun->setPressure(Pressure);
    }
  }
  
  //Load LumiDeli from DataObject
  for (int i=1; i<= lumiDeli.getLenght(); i++){
    
    int RunID = atof(lumiDeli.getElementFromPropertyContainer(i,1).c_str());
    int Lumi = atof(lumiDeli.getElementFromPropertyContainer(i,2).c_str());
    int Deli = atof(lumiDeli.getElementFromPropertyContainer(i,3).c_str());
    
    Run* currentRun = Run2011.getRunById(RunID);
    if (currentRun){
      //currentRun->setLumiSection(Lumi);
      // currentRun->setDelivered(Deli);
      currentRun->setLumi(Deli, Lumi);
    }
  }
  
  
  cout << "Filling Runs..." << endl;
  cout << "Summary Table" << endl;  
int k=0;
  while (Run2011.hasNext()){
    double NormalizedRate = 0;

    Run* currentRun = Run2011.next();
	string separator = "\t";
    if (currentRun->getDelivered() != 0){ 
k++;
      cout << setw(11)<< currentRun->getID() 
	 << separator << currentRun->getTemperature()  << setw(11) 
	 << separator << currentRun->getPressure() << setw(11) 
	 << separator << currentRun->getDelivered()  << setw(11)
	 << separator << currentRun->getLumiSection()  << setw(11)
	 << separator << currentRun->getLumi()  << setw(11)
	 << endl;  
  }
  }
  cout << "Total Runs: " << Run2011.numberOfRuns() << " Displayed: " << k <<endl;
  return 0;
  
}
