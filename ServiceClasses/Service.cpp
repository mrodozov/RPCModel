/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#include "Service.h"
#include "JSON/json/json.h"

using namespace std;

/** @brief QueryObject methods
 * @brief the query object purpose is to define the hisograms to be drawed . 
 */

QueryObject::QueryObject(){
  this->histoCounter = 0;
  this->onlineEcapCounter = 0;
  this->onlineRollCounter =0;
}

int QueryObject::getHistoCounter(){
  return this->histoCounter;
}

QueryObject::optionsStruct QueryObject::getOptMapForRecord(int recordNumber){
  return optionsMap.find(recordNumber)->second;
}

QueryObject::OnlineEcapRecord QueryObject::getEcapMapForRecord(int recNumber){
  return ecapRecordsMap.find(recNumber)->second;
}


void QueryObject::insertNewRecordWithOptions(string hName, string hTitle, double minX, double maxX, double mixY, double maxY, int hColor, int hMarker, string hXtitle, string hYtitle,string DiskFilter,string RingFileter,string OrientationFilter,int chamberStart,int chamberEnd,string part){
  this->histoCounter ++;
  optionsMap[getHistoCounter()].histoName = hName;
  optionsMap[getHistoCounter()].histoTitle = hTitle;
  optionsMap[getHistoCounter()].histoMinX = minX;
  optionsMap[getHistoCounter()].histoMaxX = maxX;
  optionsMap[getHistoCounter()].histoMinY = mixY;
  optionsMap[getHistoCounter()].histoMaxY = maxY;
  optionsMap[getHistoCounter()].histoColor = hColor;
  optionsMap[getHistoCounter()].histoMarkerStyle = hMarker;
  optionsMap[getHistoCounter()].histoXtitle = hXtitle;
  optionsMap[getHistoCounter()].histoYtitle = hYtitle;
  // roll filter options
  optionsMap[getHistoCounter()].disk = DiskFilter;
  optionsMap[getHistoCounter()].ring = RingFileter;
  optionsMap[getHistoCounter()].orientation = OrientationFilter; 
  optionsMap[getHistoCounter()].chamberStart = chamberStart;
  optionsMap[getHistoCounter()].chamberEnd = chamberEnd;
  optionsMap[getHistoCounter()].partOfChamber = part;
  optionsMap[getHistoCounter()].cutByRunRange = false;
  optionsMap[getHistoCounter()].runStart = 0 ;
  optionsMap[getHistoCounter()].runEnd = 0;
}

void QueryObject::insertRunRangeForRecord( int runStart, int runEnd,int record){
  optionsMap[record].cutByRunRange = true;
  optionsMap[record].runStart = runStart;
  optionsMap[record].runEnd = runEnd;
}

void QueryObject::insertRunRangeForOnlineRecord( int runStart, int runEnd,int record){
  online_Roll_Map[record].cutByRunRange = true;
  online_Roll_Map[record].runStart = runStart;
  online_Roll_Map[record].runEnd = runEnd;
}

string QueryObject::getNameForRecord(int elementNumber){
    return getOptMapForRecord(elementNumber).histoName ;
}

string QueryObject::getTitleForRecord(int elementNumber){
  return getOptMapForRecord(elementNumber).histoTitle ;
}

void QueryObject::setCanvasTitle(string title){
  this->canvasTitle = title;
}

string QueryObject::getCanvasTitle(){
  return this->canvasTitle;
}

int QueryObject::getOnlineEcapCounter(){
  return this->onlineEcapCounter;
}

string QueryObject::getStringForEcapForRecord(int recordNumber){
  return "RE"+ecapRecordsMap.find(recordNumber)->second.EcapDisk+"_"+ecapRecordsMap.find(recordNumber)->second.EcapRing+"_";
}

void QueryObject::insertRecordForEcapRoll(string Disk, string Ring,int Color,int Marker,string histoName,int RelativeMarkerSize){
  this->onlineEcapCounter ++;
  ecapRecordsMap[this->onlineEcapCounter].EcapDisk = Disk;
  ecapRecordsMap[this->onlineEcapCounter].EcapRing = Ring;
  ecapRecordsMap[this->onlineEcapCounter].Color = Color;
  ecapRecordsMap[this->onlineEcapCounter].Marker = Marker;
  ecapRecordsMap[this->onlineEcapCounter].histoName = histoName;
  ecapRecordsMap[this->onlineEcapCounter].MarkerSize = RelativeMarkerSize;
}

void QueryObject::setRVSPHItext(string theText){
  this->Ecap_rateVsPhi_text = theText;
}

string QueryObject::getRateVsPhiText(){
  return this->Ecap_rateVsPhi_text;
}

void QueryObject::setXaxis(int axis){
  this->onlineRollsXaxis = axis;
}

void QueryObject::setYaxis(int axis){
  this->onlineRollYaxis = axis;
}

int QueryObject::getXaxis(){
  return this->onlineRollsXaxis;
}

int QueryObject::getYaxis(){
  return this->onlineRollYaxis;
}

//


int QueryObject::getOnlineRollCounter(){
  return this->onlineRollCounter;
}

QueryObject::onlineRollStruct QueryObject::getOnlineRollMapForRecord(int record){
  return this->online_Roll_Map.find(record)->second;
}

void QueryObject::insertNewOnlineRecord(string regex, string title, int Color, int Marker){
  
  this->onlineRollCounter ++;
  online_Roll_Map[this->onlineRollCounter].Color=Color;
  online_Roll_Map[this->onlineRollCounter].Marker = Marker;
  online_Roll_Map[this->onlineRollCounter].histoName = title;
  online_Roll_Map[this->onlineRollCounter].regex = regex;
  online_Roll_Map[this->onlineRollCounter].cutByRunRange =false;
  online_Roll_Map[this->onlineRollCounter].runStart = 0;
  online_Roll_Map[this->onlineRollCounter].runEnd = 0;
  
}

void QueryObject::setHistoXtitle(string xtitle){
  this->histoXtitle = xtitle;
}

void QueryObject::setHistoYtitle(string ytitle){
  this->histoYtitle=ytitle;
}

string QueryObject::getHistoXtitle(){
  return this->histoXtitle;
}

string QueryObject::getHistoYtitle(){
  return this->histoYtitle;
}

void QueryObject::setMainTitle(string title){
  this->mainHistoTitle = title;
}

std::string QueryObject::getMainTitle(){
  return this->mainHistoTitle;
}

void QueryObject::clearAllOnlineRollRecords() {
  this->online_Roll_Map.clear();
  map<int,QueryObject::onlineRollStruct> nextMap;
  this->online_Roll_Map = nextMap; //because regardless the map is clear it clearly something remained leading to debugging and shit
}


/** 
 * 
 */

/*
vector< string > getVectorListOfGoodRunsFromJSONfile(string theJSON){
  ifstream IFS;
  IFS.open(theJSON.c_str());
  string JSONstring;
  getline(IFS,JSONstring);
  IFS.clear();
  IFS.close();
  
  Json::Value json_string;
  Json::Reader aReader;
  if(!aReader.parse(JSONstring,json_string))
  {
    cout << "Failed to parse" << endl;
    aReader.getFormatedErrorMessages();
  }
  
  vector<string> Members = json_string.getMemberNames();
  
  return Members;
  
}
*/

