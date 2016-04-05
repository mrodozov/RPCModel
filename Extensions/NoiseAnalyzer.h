/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef NOISE_ANALYZER
#define NOISE_ANALYZER

#include "../core/ExtendedRoll.h"
#include "../core/ExtendedStrip.h"
#include "../ROOT/ROOT.h"

class NoiseAnalyzer{

public:

	NoiseAnalyzer();
        NoiseAnalyzer(TH1F* cRef) { ref = cRef; };
        NoiseAnalyzer(ExRoll* ex) { exRoll = ex; };

	TH1F* getReference() { return ref; };
 	void setReference(TH1F* cRef) {ref = cRef; };
	TH1F* Associate(ExRoll* exRoll, TH1F* ref, TH1F* clone, bool correctionEnabled, int startingStrip = 1);
	TH1F* OldAssociate(ExRoll* exRoll, TH1F* ref, TH1F* clone, bool correctionEnabled);
	void analyzeNoisyStrip();
	TH1F* makeRateDistribution();
	TH1F* makeRateDistributionFromRoll();

	TH1F* GetCurrentReference(){ return cReference; };
	TH1F* GetCurrentCorrected(){ return cCorrected; };
	TH1F* GetCurrentDistribution(){ return cDistribution; };

	double Heal(ExRoll* exRoll, bool ActivateCorrection = false);
	double HealCloneByClone(ExRoll*, bool ActivateCorrection = false);
	double HealClone(ExRoll* exRoll, TH1F* refHisto,int cloneNumber, bool ActivateCorrection = false);
	TH1F* FrequencyDistribution(TH1F* ref);
private:
	TH1F* ref;
	TH1F* cReference;
	TH1F* cCorrected;
	TH1F* cDistribution;
	ExRoll* exRoll;
};

#endif
