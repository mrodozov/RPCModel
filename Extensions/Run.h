/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef RUN_H
#define RUN_H
#include "../core/DataObject.h"
#include "../core/ExtendedRoll.h"

#include <string>
#include <map>
#include "../ROOT/ROOT.h"


//#include "../core/Roll.h"
//#include "../core/Strips.h"
using namespace std;

class Run{

 public:
  Run(){setFilled(false); ID = -1;};
  Run(int);
  
  Run(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection);
  Run(int RunID, double Pressure, double Temperature, double Delivered, double LumiSection, string BeginTime, string EndTime);

  //set mandatory parameters
  inline void setPressure(double p) { Pressure = p;};
  inline void setTemperature(double t) {Temperature = t; };
  void setID(int ID_);
  inline void setDelivered(double Deli) { Delivered = Deli;};
  inline void setLumiSection(double LumiS) { LumiSection = LumiS;};
  inline void setEndTime(string endT) { endTime = endT;};
  inline void setBeginTime(string begT) { beginTime = begT;};

  void setResourceFile(string f) { ResourceFile = f;};
  void setLumi(double Delivered, double LumiSection);

  //put in the Specified Run in a rootFile
  void addToRootFile(TDirectory* f);

  //getters methods for mandatory parameters
  inline double getLumi() { return Lumi;};
  inline double getDelivered() {return Delivered;};
  inline double getLumiSection() {return LumiSection;};
  inline string getBeginTime(){ return beginTime;};
  inline string getEndTime(){ return endTime; };
  inline double getPressure() {return Pressure;};
  inline double getTemperature() {return Temperature;};
  inline int getID() {return ID;};
  inline string getResourceFile(){return ResourceFile;};
  void PrintStatistics(); 
  
  //Set other optional parameters
  void setParameter(string paramName, double paramValue);
  double getParameter(string paramName);
  void setParameter(map<string, double> ParamMap){Parameter = ParamMap;};
  //Fill from root files in the default path
  bool Fill(map<string, double>& areaMap);

  /*
    Fill from a single root File, specified in the argument; each 
    root file must provide a folder which contains all the plots about recorded rate
   */
  bool FillFromContainer(TFile* f, map<string, double>& areaMap, string Selection = "None");
  
  //  vector<Roll*> getRolls(int RunID);
  double RateIntegral(int RunID);

  const map<string,ExRoll* >& GetAllTheRolls() {return AllTheRolls;};
  const map<string,ExRoll* >& GetEndCapPlus() {return EndCapPlus;};
  const map<string,ExRoll* >& GetEndCapMinus() {return EndCapMinus;};
  const map<string,ExRoll* >& GetBarrel() {return Barrel;};
  map<string, double> GetParamMap() {return Parameter;};

bool isFilled(){return Filled;};
void setFilled(bool filled = true){Filled = filled;};

 private:

  string ResourceFile;

  double Temperature;
  double Pressure;
  string Directory;
  double Delivered;
  double LumiSection;
  double Lumi;

  string beginTime;
  string endTime;

  int ID;
  bool Filled;
  string RunStr;
  map<string,ExRoll* > AllTheRolls;
  map<string,ExRoll* > EndCapPlus;
  map<string,ExRoll* > EndCapMinus;
  map<string,ExRoll* > Barrel;
  map<string, double> Parameter;
  vector<double> CurrentValues;
};
#endif

