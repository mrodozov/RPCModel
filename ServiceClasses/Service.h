/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/

#ifndef SERVICE_CLASSES_H
#define SERVICE_CLASSES_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include "../ROOT/ROOT.h"
#include <map>
//#include <json/json.h>

using namespace std;

// this enumeration could be nice for Objective-C fans :D (added by rodozov for convenience)

enum booleans { NO=0,YES=1 };

class QueryObject{
  protected:
  
  struct onlineRollStruct{
    string regex;
    int Color;
    int Marker;
    string histoName;
    int runStart;
    int runEnd;
    bool cutByRunRange;
  };
  
  struct OnlineEcapRecord{
    string histoName;
    string EcapDisk;
    string EcapRing;
    int Color;
    int Marker;
    double MarkerSize;
  };
  
  struct optionsStruct{
    string histoName;
    string histoTitle;
    int histoColor;
    int histoMarkerStyle;
    double histoMinX;
    double histoMaxX;
    double histoMinY;
    double histoMaxY;
    string histoXtitle;
    string histoYtitle;
    string disk,ring,orientation,partOfChamber;
    int chamberStart,chamberEnd;
    int runStart,runEnd;
    bool cutByRunRange;
  };
  
private:
  int histoCounter;
  int onlineEcapCounter;
  int onlineRollCounter;
  int onlineRollsXaxis;
  int onlineRollYaxis;
  string histoXtitle;
  string histoYtitle;
  string mainHistoTitle;
  map<int,OnlineEcapRecord> ecapRecordsMap;
  map<int,optionsStruct> optionsMap;
  map<int,onlineRollStruct> online_Roll_Map;
  string canvasTitle;
  string Ecap_rateVsPhi_text;
  
public:
  
  int getHistoCounter();
  int getOnlineEcapCounter();
  int getOnlineRollCounter();
  
  OnlineEcapRecord getEcapMapForRecord(int);
  optionsStruct getOptMapForRecord(int);
  onlineRollStruct getOnlineRollMapForRecord(int);
  
  
  void insertNewRecordWithOptions(string,string,double,double,double,double,int,int,string,string,string,string,string,int,int,string);
  void insertRunRangeForRecord(int,int,int);
  void insertRunRangeForOnlineRecord( int runStart, int runEnd,int record);
  void insertRecordForEcapRoll(string,string,int,int,string,int);
  void insertNewOnlineRecord(string,string,int,int);
  string getStringForEcapForRecord(int);
  string getNameForRecord(int);
  string getTitleForRecord(int);
  void setCanvasTitle(string);
  string getCanvasTitle();
  void setRVSPHItext(string);
  string getRateVsPhiText();
  void setXaxis(int);
  void setYaxis(int);
  int getXaxis();
  int getYaxis();
  void setHistoXtitle(string);
  void setHistoYtitle(string);
  string getHistoXtitle();
  string getHistoYtitle();
  void setMainTitle(string);
  string getMainTitle();
  
  /**
   * after the tests , rewrite this set methods to be setHistoTitle,setHistoName,
   * and for the fiter options use enum with names like ecapplusonly ecapminusonly and so on 
   */
  
  QueryObject();
  ~QueryObject();
  
};

/** @brief returns vector of strings with the runnumbers certified by the certification group as GOOD
 * @par string JSONfile (the official JSON with the lumisections)
 */
//vector<string> getVectorListOfGoodRunsFromJSONfile(string theJSON);






#endif
