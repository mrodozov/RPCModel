/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef CRAW
#define CRAW

#include <map>
#include "../ROOT/ROOT.h"


//#include "Run.h"

using namespace std;

class Crawler{

 public:

  Crawler(string s);
  
  void SetBaseDirectory(string directory) {baseDirectory = directory; };

  /*
    It's something like
    Coloumn1,Coloumn2,Column3,...
    that is
    RawID,OfflineName,IsDisconnected,IsDead,IsMasked 
  */

  void SetRegularExpression(string regular) ;
  void ScanDirectory(std::string);
  void ScanDirectory();
  void Crawl(string fileName, int nFields);
//  TNtuple* GetDataSet() { return DataSet; };
  void WriteRootFile();
  void PrintTableRegExp();
  void CreateDictionary(string fileName, int nFields);
  void PrintDictionary();

 private:
  string regExp;
  string baseDirectory;
  vector<TNtuple*> DataSet;
  map<string, int> position;
  map<string, string> Dictionary; 
  vector<string> vFile;
  TFile* f;
};

#endif
