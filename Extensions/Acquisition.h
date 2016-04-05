/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef ACQUISITION_H
#define ACQUISITION_H
#include "Run.h"
#include "../core/DataObject.h"
#include <map>
#include <string>
#include "../ServiceClasses/JSON/json/json.h"

using namespace std;

/*

This class manages the main informations associated to each single Run. All the data can be
alternatively loaded by DataObject and RootFiles.
Two ways of use are allowed:

1) If a RootFile Container with all parameters classified by RunID is available, you can import saved settings
using setFromRootFile method. Luminosity and environmental parameter will be loaded in single Run objects.
You can then fill your run object calling FillFromContainer, passing the file in which your data are stored. 
The parameter "Year" which identifies the period of acquisition, is mandatory, although not necessary to access
to rootContainer file.

2) If just textfiles are provided, you can load your params with setFromObject. Luminosity and environmental 
parameter can be loaded in single Run objects. You can then fill your run object with FillFromContainer, or 
just calling Fill, if a root file container has not been produced yet.

If can also load just main parameters invoking one of the two constructor of Acquisition. The Run object will
not be filled.

In the latter case, you may be interested in creating a new rootContainer file, in which both rate information and 
environmental parameters are stored. You can then save your entire acquisition, just calling WriteAcquisition 
(passing the name of the file you want to save in).

All the parameters specified through setParameters will not be saved.

*/
class Acquisition{

 public:

  Acquisition();
  Acquisition(DataObject* dataobj, string Year = "2011"); 
  Acquisition(TFile*, string Year = "2011");

  //Read Run Parameters from a DataObject and creates runs
  void setFromObject(DataObject* dataobj, string Year = "2011");
  void LoadJSON(std::istream&);

  void LoadSelectionByDO(DataObject* DataObj, int filterCol);
  void MarkBadRunsByD0(DataObject* DataObj, int filterCol);

  void MarkRunIDAsGood(int Run);
  void MarkRunIDAsBad(int Run);

  void RemoveRunSelection(){ useJsonFile = false;};

  //Read Run Parameters from a root file and creates runs
  void setFromRootFile(TFile* file, string Year = "2011");

  //set directory where serach single rootfile
  void setDirectory(string Dir_) { Directory = Dir_;};
  
  //set a New parameter from DataObject with a name equal to paramName, from the coloumn specified
  void setParameters(DataObject* dataobj, string paramName, int fieldNumber);

  //Get run passing an int ID as argument
  Run* getRunById(int numberOfRun);
  void PrintValidRuns();
  //Fill a container with all the runs of the current acquisition
  bool WriteAcquisition(TFile* f, map<int, bool>* ExistingRuns_, int nMax = 9999999);
  // this is returned from the dead for consistency purpose, remove it when you notice
  bool WriteAcquisition(TFile* f, int nMax = 9999999);
  Run* AddRun(int RunID, string Year = "2011");
  Run* AddRun(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection, string Year = "2011");                                                    
  Run* AddRun(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection, string Year, string BeginTime, string EndTime);
                        
  Run* begin();  
  Run* getFirstRun();
  Run* getLastRun();
  Run* next();
  Run* previous();
  bool hasNext();
  int  numberOfRuns(){return RunContainer.size();};
  bool isValid(Run* run, bool verbose = true);
 private:
  bool useJsonFile;
  map<int, bool> goodRuns;
  DataObject* dataobj_;
  map<int, Run*> RunContainer;
  map<int, Run*>::const_iterator itRun;
  string Directory;
  
};

#endif
