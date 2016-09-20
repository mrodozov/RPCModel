/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef EXTENDED_STRIP_H
#define EXTENDED_STRIP_H

#include "Strip.h"
#include "vector"
//#include "TH1F.h"

using namespace std;

class ExStrip : public Strip{
  
  double rate;
  bool isToMask;
  bool isToUnmask;
  double timeWhileNoisy;
  double maxRate;
  //TH1F * channelRateProfile;
  
public:
  
  ExStrip();
  ExStrip(const std::string & W_D,const int & W_D_N,const int & sec,const std::string & R_ID,const std::string & R_ID_N,const int & num);
  
  ~ExStrip();
  
  virtual void initWithValues(const  std::string & W_D,const int & W_D_N,const int & sec,const std::string & R_ID,const std::string & R_ID_N,const int & num);
  
  void setRate (const double &);
  const double & getRate();
  void setToBeMasked();
  void unsetToBeMasked();
  const bool & isAboutToBeMasked();
  void setToBeUnmasked();
  void unsetToBeUnmasked();
  const bool & isAboutToBeUnmasked();
  void setTimeAsNoisy(const double &);
  const double & getTimeAsNoisy();
  void setmaxRate(const double &);
  const double & getMaxRate();
  //void setRateProfileHisto( TH1F * rateProfile){ this->channelRateProfile = rateProfile; }
  //TH1F * getRateProfileHisto(){return this->channelRateProfile;}
  
  virtual void ClearConditions(); 
  

};

#endif