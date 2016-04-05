/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifdef __MAKECINT__
#pragma link c++ class map<string, double>;
#pragma link c++ class map<string, double>::iterator;
#pragma link c++ class pair<string, double>
#endif
#include "Acquisition.h"
#include "../ROOT/ROOT.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace std;

Acquisition::Acquisition():useJsonFile(false){
  gROOT->ProcessLine("#include <map>");
  gROOT->ProcessLine("#include <string>");
  Directory = "resources/rootContainer/";
}

Acquisition::Acquisition(DataObject* dataobj, string Year):useJsonFile(false){
  gROOT->ProcessLine("#include <map>");
  gROOT->ProcessLine("#include <string>");
  Directory = "resources/rootContainer/";
  setFromObject(dataobj, Year);
}

Acquisition::Acquisition(TFile* file, string Year):useJsonFile(false){
  gROOT->ProcessLine("#include <map>");
  gROOT->ProcessLine("#include <string>");
  Directory = "resources/rootContainer/";
  setFromRootFile(file, Year);
}

void Acquisition::MarkRunIDAsGood(int RunID){
    useJsonFile = true;
    goodRuns[RunID] = true; 
}

void Acquisition::MarkRunIDAsBad(int RunID){

    useJsonFile = true;
    goodRuns[RunID] = false; 
}

void Acquisition::PrintValidRuns(){
	cout << "Good Runs: " << endl;

	int nValid = 0;
	for(map<int, bool>::const_iterator it = goodRuns.begin(); it != goodRuns.end(); it++){
		if (it->second == true){
			nValid++;
			cout << it->first << endl;
		}
	}
	cout << nValid << " valid runs found." << endl;

}


void Acquisition::LoadSelectionByDO(DataObject* DataObj, int filterCol){

  useJsonFile = true;


  for (int i=1; i<= DataObj->getLenght(); i++){
    int runNumber = atof(DataObj->getElementFromPropertyContainer(i,filterCol).c_str());
    //cout << "marking as good: " << runNumber << endl;
    MarkRunIDAsGood(runNumber);   
    }
}

void Acquisition::MarkBadRunsByD0(DataObject* DataObj, int filterCol){


  for (int i=1; i<= DataObj->getLenght(); i++){
    int runNumber = atof(DataObj->getElementFromPropertyContainer(i,filterCol).c_str());
    //cout << "marking as good: " << runNumber << endl;
    MarkRunIDAsBad(runNumber);   
  }


}


void Acquisition::LoadJSON(istream& f){
cout << "Loading run from JSON file..." << endl;

useJsonFile = true;
Json::Value root;
Json::Reader reader; 
bool parsingSuccesful = reader.parse (f, root, true); 
if (!parsingSuccesful){
	cout << "Error while parsing JSON file" << endl;
	return;
}

Json::Value::Members runs = root.getMemberNames();

for (int k=0; k<runs.size(); k++){
	MarkRunIDAsGood(atoi(runs[k].c_str()));
}
}

Run* Acquisition::AddRun(int RunID, string Year){
   //   cout << "Using JSON File:" << useJsonFile << endl;  
   if (useJsonFile == true){
	if (goodRuns[RunID] != true){
//		cout << "Run " << RunID << " is not a good Run. Skipped." <<endl;
		return new Run();
	}
	}
   RunContainer[RunID] = new Run(RunID);

               ostringstream out;    
               out << RunID;
               string Path = Directory + Year;
               string fileName = Path + "/total_" + out.str() + ".root";

  RunContainer[RunID]->setResourceFile(fileName);
  itRun = RunContainer.begin();
  return RunContainer[RunID];
}

Run* Acquisition::AddRun(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection, string Year){

   if (useJsonFile == true){
        if (goodRuns[RunID] != true){
            //    cout << "Run " << RunID << " is not a good Run. Skipped." <<endl;
                return new Run();
        }
     }

   RunContainer[RunID] = new Run(RunID, Pressure, Temperature, Delivered, LumiSection);

               ostringstream out;    
               out << RunID;
               string Path = Directory + Year;
               string fileName = Path + "/total_" + out.str() + ".root";

  RunContainer[RunID]->setResourceFile(fileName);
  itRun = RunContainer.begin();
  return RunContainer[RunID];

}

Run* Acquisition::AddRun(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection, string Year, string BeginTime, string EndTime){

   if (useJsonFile == true){
        if (goodRuns[RunID] != true){
            //    cout << "Run " << RunID << " is not a good Run. Skipped." <<endl;
                return new Run();
        }
     }

   RunContainer[RunID] = new Run(RunID, Pressure, Temperature, Delivered, LumiSection, BeginTime, EndTime);

               ostringstream out;    
               out << RunID;
               string Path = Directory + Year;
               string fileName = Path + "/total_" + out.str() + ".root";

  RunContainer[RunID]->setResourceFile(fileName);
  itRun = RunContainer.begin();
  return RunContainer[RunID];

}


bool Acquisition::WriteAcquisition(TFile* f, map<int, bool>* ExistingRuns_, int nMax){

  int n = 0;
  map<string, double> paramMap;

  string beginTime;
  string endTime;
  
  for (map<int, Run*>::const_iterator it = RunContainer.begin(); it != RunContainer.end(); it++){
    
    if ((*ExistingRuns_)[it->first]) continue;

    if (n >= nMax) break;
    stringstream out;

    if (!(it->second)) continue;
    out << it->second->getID();
    n++;
    
    TDirectory* currentDir = f->mkdir(out.str().c_str());
    TNtuple* Params = new TNtuple("EnvParams", "Environmental Parameters", "pressure:temperature:delivered:lumi");
    TTree* OtherParams = new TTree("FreeParams", "Free Parameters");
    OtherParams->Branch("paramMap", &paramMap);
    OtherParams->Branch("BeginTime", &beginTime);
    OtherParams->Branch("EndTime", &endTime);

    paramMap = it->second->GetParamMap();
    beginTime = it->second->getBeginTime();
    endTime =   it->second->getEndTime();

    Params->Fill( it->second->getPressure(), it->second->getTemperature(),  it->second->getDelivered(), it->second->getLumiSection() );
    OtherParams->Fill();
    
    it->second->addToRootFile(currentDir);
    currentDir->WriteObject(Params, "EnvParams");
    currentDir->WriteObject(OtherParams, "FreeParams");
    
    delete Params;

  }
    
  f->Close();
  f->Delete();
  return true;
}

bool Acquisition::WriteAcquisition(TFile* f, int nMax){
/*
//check on existence of file
FileStat_t buf;
if (gSystem->GetPathInfo(f->GetName(), buf) == 0){
cout << "The file " << f->GetName()<< " already Exists!" << endl;
cout << "Quitting..." << endl;
return false;
}
*/


  int n = 0;
  for (map<int, Run*>::const_iterator it = RunContainer.begin();
       it != RunContainer.end(); it++){

    if (n >= nMax) break;
    stringstream out;

    if (!(it->second)) continue;
    out << it->second->getID();
    n++;
    
    TDirectory* currentDir = f->mkdir(out.str().c_str());
    TNtuple* Params = new TNtuple("EnvParams", "Environmental Parameters", "pressure:temperature:delivered:lumi");
    TTree* OtherParams = new TTree("FreeParams", "Free Parameters");
    
    map<string, double> paramMap;
    OtherParams->Branch("paramMap", &paramMap);

    paramMap = it->second->GetParamMap();
    OtherParams->Fill();
    Params->Fill( it->second->getPressure(), it->second->getTemperature(),  it->second->getDelivered(), it->second->getLumiSection() );
    
    it->second->addToRootFile(currentDir);
    currentDir->WriteObject(Params, "EnvParams");
    currentDir->WriteObject(OtherParams, "FreeParams");
    
    delete Params;

  }

  f->Close();
  f->Delete();
  return true;
}


void Acquisition::setFromRootFile(TFile* file, string Year){

    TIter nextDir( file->GetListOfKeys() );
    TKey* currentDir;
 

    //loop on all runs and all strip contained

    while(currentDir = (TKey*)nextDir()){

      TClass* c2 = gROOT->GetClass( currentDir->GetClassName() );

      if (!c2->InheritsFrom("TDirectory")) continue;

	 TDirectory* dir = (TDirectory*)(currentDir->ReadObj());
	 
	 TIter nextObject (dir->GetListOfKeys());
	 TKey* currentObject;
	 while (currentObject = (TKey*)nextObject() ){
	   
	   TClass* cObject = gROOT->GetClass( currentObject->GetClassName() );
	   int RunID;
	   if (cObject->InheritsFrom("TH1")) {
	   }
	   else if (cObject->InheritsFrom("TNtuple")){
	     
	     TNtuple* ntuple = (TNtuple*)(currentObject->ReadObj()); 
	     
	     Float_t Temp; 
	     Float_t Pressure;
	     Float_t Deliv; 
	     Float_t LumiSection; 

	     RunID = atof(dir->GetName());
	     ntuple->SetBranchAddress("temperature",&Temp);
	     ntuple->SetBranchAddress("pressure",&Pressure);
	     ntuple->SetBranchAddress("delivered",&Deliv);
	     ntuple->SetBranchAddress("lumi",&LumiSection);

	     for (int k=0; k<ntuple->GetEntries(); k++){

	       ntuple->GetEntry(k);

//	       cout << RunID << " " << Temp << " " << Pressure << " " << Deliv << " " << LumiSection << endl;
	       Run* currentRun = AddRun(RunID, Pressure, Temp, Deliv, LumiSection);
	       if (currentRun->getID() != -1)
			currentRun->setFilled();
		else
			currentRun->setFilled(false);

	     }
	   }
	   else if (cObject->InheritsFrom("TTree")){
	     TTree* FreeParam = (TTree*)(currentObject->ReadObj());
	     //	     if (FreeParam->GetName() != "FreeParams") continue;
	     //  cout << FreeParam->GetName() << endl;
	     TBranchElement *branch; 

	     int nEntries = FreeParam->GetEntries();
	     map<string, double>* params = 0;

	     string* beginTime = new string();
	     string* endTime = new string();

	     FreeParam->SetBranchAddress("paramMap", &params);
             FreeParam->SetBranchAddress("BeginTime", &beginTime);
             FreeParam->SetBranchAddress("EndTime", &endTime);

	     for (int k=0; k<1; k++){
	       FreeParam->GetEntry(k);
	       RunContainer[RunID]->setParameter(*params);
     	       RunContainer[RunID]->setBeginTime(*beginTime);
	       RunContainer[RunID]->setEndTime(*endTime);

	       //MarkRunIDAsBad(RunID);
	       /*	       for (map<string, double>::const_iterator it = params->begin(); it != params->end(); it++){
                 cout << it->first << " " << it->second << endl;
		 }*/
		 }
	     
	   }
	 }	 
    }
    



itRun = RunContainer.begin();
    
}


/*
File format:
RUN, HV1, HV2, PRESSURE, TEMP, LUMI, DELI
*/
void Acquisition::setFromObject(DataObject* dataobj, string Year){

  int PRESSURE = 4;
  int TEMPERATURE = 5;
  int RUNID = 1;
  int DELI = 7;
  int LUMISECTION = 6;
  
  string Path = Directory + Year;

  //fill the Runs

  for (int i=1; i <= dataobj->getLenght(); i++){

    int RunID = atoi(dataobj->getElementFromPropertyContainer(i ,RUNID).c_str());
    double Temp = atof(dataobj->getElementFromPropertyContainer(i ,TEMPERATURE).c_str());
    double Pressure = atof(dataobj->getElementFromPropertyContainer(i ,PRESSURE).c_str());
    double Deliv = atof(dataobj->getElementFromPropertyContainer(i ,DELI).c_str());
    double LumiSection = atof(dataobj->getElementFromPropertyContainer(i ,LUMISECTION).c_str());    

    Run* currentRun = AddRun(RunID, Pressure, Temp, Deliv, LumiSection);

    currentRun->setFilled();

  }
  
  itRun = RunContainer.begin();

}

void Acquisition::setParameters(DataObject* dataobj, string paramName, int fieldNumber){

  int RUNID = 1;

  for (int i=1; i <= dataobj->getLenght(); i++){

    int RunID =  atoi(dataobj->getElementFromPropertyContainer(i ,RUNID).c_str());
    double paramValue = atof(dataobj->getElementFromPropertyContainer(i ,fieldNumber).c_str());

    if (!RunContainer[RunID]) continue;
    RunContainer[RunID]->setParameter(paramName, paramValue);

  }

}

Run* Acquisition::getRunById(int numberOfRun){
  return RunContainer[numberOfRun];
}

Run* Acquisition::getFirstRun(){
  return RunContainer.begin()->second;
}

Run* Acquisition::getLastRun(){
  return RunContainer.end()->second;
}

Run* Acquisition::begin(){
	itRun = RunContainer.begin();
	return itRun->second;
}

Run* Acquisition::next(){

  if (itRun != RunContainer.end())
    itRun++;

  return itRun->second;
  
}

Run* Acquisition::previous(){

  if (itRun != RunContainer.begin())
    itRun--;

  return itRun->second;

}


bool Acquisition::hasNext(){

  if (itRun != --RunContainer.end())
    return true;
  else
    return false;

}


bool Acquisition::isValid(Run* run, bool verbose){
 
  if (useJsonFile == true){
//	cout << goodRuns[run->getID()] << " " << run->getID() << endl;

        if (goodRuns[run->getID()] == 0){
		if (verbose == true){
			cout << "Warning: Run " << run->getID() << " is not a good Run. Skipped." << endl;
		}
                return false;
        }

     }
return true;
}
