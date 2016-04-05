/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include "Crawler.h"
#include "../core/DataObject.h"
#include <cmath>
#include <stdlib.h>
#include <dirent.h>
#include "../ROOT/ROOT.h"

using namespace std;

Crawler::Crawler(string fileName){
	f = new TFile(fileName.c_str(), "RECREATE");
}

/*
Call ScanDirectory method specifying a new BaseDirectory
which will be kept in memory
 */
void Crawler::ScanDirectory(string s){
  SetBaseDirectory(s);
  ScanDirectory();
}

/*
The method looks for all the runXXXXXX directory into a 
BaseDirectory previously specified and stores all the directory
names into a specified vector.
 */
void Crawler::ScanDirectory(){

DIR* d;
string path;
 d = opendir(baseDirectory.c_str());
 if(d) {
   dirent* fDirent;
   while((fDirent = readdir(d))){
     string nomefile = fDirent->d_name;        
     if (nomefile.size() == 9){
       string extension = nomefile.substr(0, 3);
       if (extension == "run"){
	 cout << "Adding " << nomefile << ";" << endl;
	 vFile.push_back(nomefile);
       }
     }
   }
 }
 
}

/*
For each runXXXXXX directory previously detected, creates a new directory 
in a rootfile. In each rootfile a new Ntuple is created
 */
void Crawler::Crawl(string fileName, int fields){
    Double_t RawID = 0;
    Int_t IsDisconnected = 0;
    Int_t IsDead = 0;
    Int_t IsMasked = 0;
    Int_t Channel = 0;
    Int_t PreviousRawID = -1;
    Double_t CurrentRate = 0;
  
    for (int k=0; k<vFile.size();k++){
      
      TDirectory* dir = f->mkdir(vFile[k].c_str(), vFile[k].c_str());
      
      string fName =  baseDirectory + vFile[k] + "/" + fileName;
      DataObject cObj;
      cObj.getDataFromFile(fName,fields);
      
      string RawIDstr;
      
      
      TTree* myTree = new TTree("MyTree", "MyTree");
      
      //myTree->Branch("RawID", &RawID, "RawID/L");
      //       myTree->Branch("Channel", &Channel, "Channel/I");
      //myTree->Branch("IsDisconnected", &RawID, "IsDisconnected/I");
      //myTree->Branch("IsDead", &IsDead, "IsDead/I");
      //myTree->Branch("IsMasked", &IsMasked, "IsMasked/I");
      myTree->Branch("Rate", &CurrentRate, "CurrentRate/D");
      
    for (int i=1; i<= cObj.getLenght(); i++){

      RawIDstr = cObj.getElementFromPropertyContainer(i,position["RawID"]);
      Channel = atoi(cObj.getElementFromPropertyContainer(i,position["Channel"]).c_str());
      IsDisconnected = atoi(cObj.getElementFromPropertyContainer(i,position["IsDisconnected"]).c_str());
      IsDead = atoi(cObj.getElementFromPropertyContainer(i,position["IsDead"]).c_str());
      IsMasked = atoi(cObj.getElementFromPropertyContainer(i,position["IsMasked"]).c_str());
      CurrentRate = atof(cObj.getElementFromPropertyContainer(i,position["Rate"]).c_str());

      cout << CurrentRate << endl;

	myTree->Fill();

      RawID = atoi(RawIDstr.c_str());
      if (RawID == 0) continue;

      /*      if (RawID != PreviousRawID){	
	//if is not the first cycle
	
	if (myTree){
	  dir->WriteObject(myTree, myTree->GetName());
	  }*/

	PreviousRawID = atoi(RawIDstr.c_str());
	//	delete myTree;
        if (!Dictionary[RawIDstr].empty())
	  RawIDstr = Dictionary[RawIDstr];
	
	string name = RawIDstr + "_histo";

	//	t = new TNtuple(RawIDstr.c_str(), RawIDstr.c_str(),"Channel:IsDisconnected:IsDead:IsMasked") ;	


      }
      //      t->Fill(RawID, Channel, IsDisconnected, IsDead, IsMasked);
      //      cout << RawIDstr << " " << Channel << " " << IsDisconnected << " " << IsDead << " " << IsMasked<<endl;

    dir->WriteObject(myTree, myTree->GetName());
    delete myTree;
    }




    
    
}

/*
Given a regular expression as representation of database fields,
create a map which associate the name of the field with the 
coloumn number
 */
void Crawler::SetRegularExpression(string regular){
  
  bool endOfString = false;
  int fromWhere = -1;
  int coloumn = 1;

  while (!endOfString){

    int pos = regular.find(",", fromWhere+1);
    string term = regular.substr(fromWhere + 1,  pos - fromWhere -1 );
    position[term] = coloumn;
    coloumn++;
    fromWhere += (pos - fromWhere);
    if (pos == string::npos) {
      endOfString = true;
        continue;
    }

  }
 
}

void Crawler::PrintTableRegExp(){

cout << "****************" << endl;
 cout << "Term" << " " << "Coloumn" << endl;
cout << "****************" << endl;
  for (map<string, int>::const_iterator it = position.begin();
       it != position.end(); it++){
    cout << it->first << "|" << it->second << "|" << endl; 
  }

}

/*
Close the opened rootfile
 */
void Crawler::WriteRootFile(){

  //	for (int k=0; k<DataSet.size(); k++)
  //		f->WriteObject(DataSet[k], DataSet[k]->GetName());
	f->Close();
}

/*
Create a map which associates the number of the roll (RAW)
to the name of the roll
 */
void Crawler::CreateDictionary(string fileName, int nFields){

  DataObject dicObj;
  dicObj.getDataFromFile(fileName.c_str(), nFields);

  for (int i=1; i<= dicObj.getLenght(); i++){
    string raw = dicObj.getElementFromPropertyContainer(i,1);
    string rollName = dicObj.getElementFromPropertyContainer(i,2);
    Dictionary[raw] = rollName;
  }


}

/*
Just print the association table between RAW roll code and the roll name
 */
void Crawler::PrintDictionary(){

for (map<string, string>::const_iterator it = Dictionary.begin(); it != Dictionary.end(); it++){
	cout << it->first << " | " << it->second << endl;
}

}
