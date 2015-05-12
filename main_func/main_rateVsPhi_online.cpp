/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 17.05.2013
 * Info added at SVN revision 80
 * 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
//#include <mysql/mysql.h>
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../core/Chip.h"
#include "../Applications.h"
#include "../ROOT/ROOT.h"

int main (int argc,char * argv[]){
  
  /** 
   * Example CL usage - exe with arguments
   * ./
   */
  
  string rateFile = argv[1];
  bool subtractIntrinsic = atoi(argv[2]);
  string fileWithIntrinsic = argv[3];
  DataObject area = DataObject(argv[4]);
  double cutValueSingleStrip = atof(argv[5]);
  string SGmapFile = argv[6];
  DataObject LumiFile = DataObject(argv[7]);
  string theDisk = argv[8];
  QueryObject * obj = new QueryObject;
  
  obj->setCanvasTitle(argv[9]);
  //obj->setRVSPHItext(("Disk +"+theDisk+"/-"+theDisk).c_str());
  obj->setRVSPHItext(argv[10]);
  obj->insertRecordForEcapRoll(("-"+theDisk).c_str(),"2",kRed+3,22,("RE-"+theDisk+"/2").c_str(),1.5);
  obj->insertRecordForEcapRoll(("+"+theDisk).c_str(),"2",kBlue+1,29,("RE+"+theDisk+"/2").c_str(),1.6);  
  obj->insertRecordForEcapRoll(("-"+theDisk).c_str(),"3",kMagenta,21,("RE-"+theDisk+"/3").c_str(),1);
  obj->insertRecordForEcapRoll(("+"+theDisk).c_str(),"3",kCyan,20,("RE+"+theDisk+"/3").c_str(),1);
  //obj->setRVSPHItext("#sqrt{s} = 8 TeV, Instantaneous Luminosity 17.08 10^{32}cm^{-2} s^{-1}");
  
  plotEcap_RateVsPhi(rateFile,
			  subtractIntrinsic /*to be zero*/,
			  fileWithIntrinsic,
			  area,
			  cutValueSingleStrip /* */,
			  SGmapFile /* in localResources*/,
			  LumiFile,
			  obj
			 );
  return 0;
}
