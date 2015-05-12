/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#include "Run.h"
#include "../ROOT/ROOT.h"
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include "../core/ExtendedStrip.h"
#include <iomanip>
#include "NoiseAnalyzer.h"
using namespace std;
Run::Run(int Run){

  Temperature = 0;
  Pressure = 0;
  setLumi (0, 0);
  beginTime = "";
  endTime = "";
  ID =Run;  

}

Run::Run(int RunID_, double Pressure_, double Temperature_, double Delivered_, double LumiSection_){
  
  Temperature = Temperature_;
  Pressure = Pressure_;
  setLumi (Delivered_, LumiSection_);
  ID = RunID_;	
  beginTime = "";
  endTime = "";
  stringstream ss;
  ss << ID;
  RunStr = "run" + ss.str();

}

Run::Run(int RunID_, double Pressure_, double Temperature_, double Delivered_, double LumiSection_, string begTime, string endTime){
  
  Temperature = Temperature_;
  Pressure = Pressure_;
  setLumi (Delivered_, LumiSection_);
  ID = RunID_;	
  beginTime = begTime;
  endTime = endTime;
  stringstream ss;
  ss << ID;
  RunStr = "run" + ss.str();

}
void Run::setLumi(double Deliv, double LumiS){
  
  Delivered = Deliv;
  LumiSection = LumiS;
  Lumi = ( Delivered / LumiSection ) / 23.31;

}


void Run::addToRootFile(TDirectory* cdtof){

  using namespace std;
TFile* copyFrom;
FileStat_t buf;
if (gSystem->GetPathInfo(ResourceFile.c_str(), buf) < 1){
   copyFrom =TFile::Open(ResourceFile.c_str());
}
else{
cout << "Resource file " << ResourceFile.c_str() << " does not exist..." <<endl;
Filled = false;
 return;
}
  cout << "Adding Run" << ID << endl;
  TIter next(copyFrom->GetListOfKeys());

  stringstream out;
  out << ID;
  TKey *key;

  while( (key = (TKey*)next() )){
    TClass *c1 = gROOT->GetClass(key->GetClassName());
    if (c1->InheritsFrom("TH1")){
      cdtof->WriteObject((TH1F*)copyFrom->Get(key->GetName()), key->GetName());
    }
  }
  copyFrom->Close();
  


}

void Run::setID(int ID_){
  stringstream ss;
  ss << ID_;
  RunStr = "run" + ss.str();
}

void Run::setParameter(string paramName, double paramValue){
  Parameter[paramName] = paramValue;
}

double Run::getParameter(string paramName){
 return Parameter[paramName];
}




bool Run::FillFromContainer(TFile* f, map<string, double>& areaMap, string selection){
  
  stringstream dirName;
  dirName << ID;

  TDirectory* dir = (TDirectory*)(f->Get( dirName.str().c_str() ));
  if (!dir) return false;
  f->cd( dirName.str().c_str() );

  TIter nextHisto( dir->GetListOfKeys() );

  TKey* currentHisto;

  //loop on all histograms contained in a loop
  while(currentHisto = (TKey*)nextHisto()){

    TClass* c2 = gROOT->GetClass( currentHisto->GetClassName() );

    if (!c2->InheritsFrom("TH1")) continue;
    string rollName = string(currentHisto->GetName());
    if (selection != "None" && rollName != selection) continue;

    if (rollName.size() <= 1 || rollName.size() <= 2) continue;

    if (rollName.substr(0, 1) == "W" || rollName.substr(0, 2) == "RE") {
      //      cout << "Name of Roll:" << rollName << endl;
      AllTheRolls[rollName] = new ExRoll(rollName) ;

      vector<string> CloneName;
      int nClones = AllTheRolls[rollName]->getClones();

      for (int j=0; j<nClones; j++){
	string nameOfClone = AllTheRolls[rollName]->getRollIDofCloneWithNewIdentifiers(j+1);
	CloneName.push_back( nameOfClone );
      }

      TH1F* histo = (TH1F*)currentHisto->ReadObj();

      AllTheRolls[rollName]->setRootFileName( f->GetName() );
      AllTheRolls[rollName]->setTH1Path( dirName.str() + "/" + histo->GetName() );
      AllTheRolls[rollName]->setStripsAreaFromMap(areaMap);
//	cout << dirName.str() + "/" + histo->GetName() << endl;
      if ( AllTheRolls[rollName]->isInBarrel() ) 
        Barrel[rollName] = AllTheRolls[rollName];

      if (AllTheRolls[rollName]->isInEndcap() &&  AllTheRolls[rollName]->getWheel_Disk_Number() > 0)
        EndCapPlus[rollName] = AllTheRolls[rollName];

      if (AllTheRolls[rollName]->isInEndcap() && AllTheRolls[rollName]->getWheel_Disk_Number() < 0)
        EndCapMinus[rollName] = AllTheRolls[rollName];
    }
     setFilled();
    }
}

bool Run::Fill(map<string, double>& areaMap){

    //if (ResourceFile)
    //cout << "name: " << ResourceFile <<endl;
  if (Filled == false) return false;


  const char* name = ResourceFile.c_str();

FileStat_t buf;
if (gSystem->GetPathInfo(ResourceFile.c_str(), buf) < 1){
   //copyFrom =TFile::Open(ResourceFile.c_str());
}
else{
cout << "Resource file " << ResourceFile.c_str() << " does not exist..." <<endl;
Filled = false;
 return false;
	}


	TFile f1(name);
    f1.Open(ResourceFile.c_str());
    TIter next(f1.GetListOfKeys());
    TKey* key;

    while( (key = (TKey*)next() )){

      TClass* c1 = gROOT->GetClass( key->GetClassName() );

      if (!c1->InheritsFrom("TH1")) continue;
      
      string rollName = string(key->GetName());

      if (rollName.size() <= 1 || rollName.size() <= 2) continue;

      if (rollName.substr(0, 1) == "W" || rollName.substr(0, 2) == "RE") {
	//	cout << "Name of Roll:" << rollName << endl;

	AllTheRolls[rollName] = new ExRoll(rollName) ; 
	TH1F* histo = (TH1F*) ( f1.Get( rollName.c_str() ));

      AllTheRolls[rollName]->setStripsRateFromTH1Source( histo );
	cout << "The histogram has the current integral: " << histo->Integral() << endl;
      AllTheRolls[rollName]->setStripsAreaFromMap(areaMap);

      if ( AllTheRolls[rollName]->isInBarrel() )
	Barrel[rollName] = AllTheRolls[rollName];
      
      if (AllTheRolls[rollName]->getWheel_Disk_Number() > 0)
	EndCapPlus[rollName] = AllTheRolls[rollName];	
      
      if (AllTheRolls[rollName]->getWheel_Disk_Number() < 0) 
	EndCapMinus[rollName] = AllTheRolls[rollName];	

      delete histo;
      }
    }
    f1.Close();
    f1.Delete();
   delete key;
//   delete f1;
	return true;
//  aroll->setStripsRateFromRootFileSource(ResourceFile);       
}
		
void Run::PrintStatistics(){

	string splitter =  " | ";

	cout << splitter << "ID: ";
	cout << setw (12) << getID() << splitter << setw (12)<<
	"P: " << getPressure() << splitter << setw (12) <<
	"T: " << getTemperature() << splitter << setw(12) << 
	 "LumiSection: " << getLumiSection() << splitter << setw (12)<<
	"Delivered: " << getDelivered() << splitter << setw (12)<<
	"Lumi: " << getLumi() << splitter << setw (12)<<
	"Begin time: " << getBeginTime() << splitter << setw (12)<<
	"End time: " << getEndTime() << splitter << setw (12)<<
	endl;

/*        cout << getID() << splitter << 
        getLumiSection() << splitter << 
        getDelivered() << splitter << 
        getLumi() << splitter << endl;*/

}
