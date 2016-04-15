#include "interface/ROOT.h"
#include <string>
#include <vector>
#include <sstream>
#include <cstdarg>
#include <utility>

using namespace std;


vector<string> explode(string del, string str)
{
int slen = str.length();
int dlen = del.length();
int found_a;
string before;
vector<string> final;
vector<string> fail (0);
if(dlen == 0) return fail;
if(slen == 0) return fail;
for(int counter = 0; counter < slen; counter ++)
{
    found_a = str.find(del);
    if(found_a != string::npos)
{
before = str.substr(0, found_a + dlen);
final.push_back(before);
str.replace(0, found_a + dlen, "");
}
else
{
if(str.length() > 0)
{
    final.push_back(str);
    str.replace(0, str.length(), "");
}
else
{
    break;
}
    }
}
return final;
}


void MergeRateHistograms(string folder, vector<string> files){
  
  TFile * total = new TFile((folder+"total.root").c_str(),"RECREATE");
  
  string aFile;
  
  for (int i = 0 ; i < files.size() ; i++){
    string rfile = folder+files.at(i);
    TFile * rootFile = new TFile(rfile.c_str(), "READ","in");
    TH1F * h1;
    TIter nextkey( rootFile->GetListOfKeys() );
    TKey *key;
    TObject * obj;
    TDirectory * dir = gDirectory;
    map <string, string> saved;
    
    while (key = (TKey*)nextkey()) {
	
	obj = (TH1F*)key->ReadObj();
	// populate the rolls here
	string histoCurrentName = obj->GetName();
	if ( (histoCurrentName.substr(0,1) == "W" || histoCurrentName.substr(0,2) == "RE") && saved.find(histoCurrentName) == saved.end()) {
	  //h1 = (TH1F*)rootFile->Get((histoCurrentName).c_str());
	  h1 = dynamic_cast<TH1F*>(obj);
	  cout << h1->GetName() << endl;
	  total->cd();
	  h1->Write();
	  saved[histoCurrentName] = histoCurrentName;
	}
    }
    rootFile->Close("R");
  }
  //total->Write();
  total->Close("R");
  
}

int main(int argc,char * argv[]){
  
  vector<string> files;
  for (int i = 2 ; i < argc ; i++ ){
    files.push_back(argv[i]);
    //cout << i+1 << endl;
  }
  //cout << argc << endl;
  MergeRateHistograms(argv[1],files);
  
  return 0;
}
