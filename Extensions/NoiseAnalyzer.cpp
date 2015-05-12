/** Author: Michele Gabusi  mgabusi@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#include "NoiseAnalyzer.h"
#include "../core/ExtendedStrip.h"

#include "../ROOT/ROOT.h"

#include <iostream>
#include <cmath>
using namespace std;

NoiseAnalyzer::NoiseAnalyzer(){

}


void NoiseAnalyzer::analyzeNoisyStrip(){

}

double NoiseAnalyzer::Heal(ExRoll* exRoll, bool ActivateCorrection){

	TFile* refFile = TFile::Open( exRoll->getRootFileName().c_str() );

	TH1F* refHisto = (TH1F*)( refFile->Get( exRoll->getTH1Path().c_str() ) );
	TH1F* clHisto  = (TH1F*)( refHisto->Clone() );
	TH1F* dist0 = Associate(exRoll, refHisto, clHisto, ActivateCorrection, 0);

	refFile->Close();
	double integral = 0;

	for (int k=1; k<=96;k++)
	  integral += exRoll->getStrip(k)->getRate(); 

        delete refHisto;
        delete clHisto;
        delete dist0;

	refFile->Close();
	refFile->Delete(); 
	return integral;
}

double NoiseAnalyzer::HealCloneByClone(ExRoll* exRoll, bool ActivateCorrection){

	TFile* refFile = TFile::Open( exRoll->getRootFileName().c_str() );
        TH1F* refHisto = (TH1F*)( refFile->Get( exRoll->getTH1Path().c_str() ) );
	double integral;

	for(int i = 0;i < exRoll->getClones() ; i++){
		TH1F* cHisto = exRoll->getRateHistoForClone(refHisto, i+1);
		integral += HealClone(exRoll,cHisto, i+1, ActivateCorrection);
	}

	refFile->Close();
	refFile->Delete();
	return integral;
}

double NoiseAnalyzer::HealClone(ExRoll* exRoll, TH1F* refHisto, int cloneNumber, bool ActivateCorrection){

	TH1F* clHisto = (TH1F*)(refHisto->Clone());

	int startingStrip = (cloneNumber-1)*(96/exRoll->getClones());
	TH1F* dist0 = Associate(exRoll, refHisto, clHisto, ActivateCorrection, startingStrip);

	double cloneIntegral = clHisto->Integral();

	delete refHisto;
	delete clHisto;
	delete dist0;

	return cloneIntegral;
}

TH1F* NoiseAnalyzer::FrequencyDistribution(TH1F* ref){

  TH1F* rateDist = new TH1F("RateDistribution", "Rate Distribution", 10000, 0, 10000);

  for (int k = 1; k<= ref->GetNbinsX(); k++){
    double value = ref->GetBinContent(k);
    if (value > 0.1){
      rateDist->Fill(value);
    }
  }
  rateDist->Scale(1/rateDist->Integral());

  return rateDist;
}


TH1F* NoiseAnalyzer::Associate(ExRoll* exRoll, TH1F* ref, TH1F* corr, bool correctionEnabled, int startingStrip){

  TH1F* corrected = corr;
  corrected->Reset();
  corrected->SetName("Corr");
  TH1F* rateDist = new TH1F("RateDistribution", "Rate Distribution", 10000, 0, 10000);
  double mostProbBin = 0;

  for (int k = 1; k<= ref->GetNbinsX(); k++){

    double value = ref->GetBinContent(k);
    if (value > 0.1){
      rateDist->Fill(value);
    }
  }
  rateDist->Scale(1/rateDist->Integral());

  if (correctionEnabled != true) {

    for (int k=1; k<= ref->GetNbinsX(); k++){

      double cont = ref->GetBinContent(k);
      corrected->SetBinContent(k, cont);
      exRoll->getStrip(startingStrip+k)->setRate(cont);
      exRoll->AddToRate(cont);
      //      nFills++;
    }

    return rateDist;
  }
  mostProbBin = rateDist->GetMaximumBin();

//  int notZero = rateDist->FindFirstBinAbove(1);
  int SeventyWidthBins = ref->GetNbinsX();

  double Cut;
  Cut = 0.80;
  double RateCut = 1e100;

  for (int j=0; j<rateDist->GetNbinsX(); j++){
  //  double integralPeak = rateDist->Integral(notZero, j);
      double integralPeak = rateDist->Integral(mostProbBin -j, mostProbBin +j);

    /*  cout << "Integral (" << mostProbBin -j << ", " << mostProbBin +j << ") / ("<< rateDist->GetBinCenter(mostProbBin -j)
  << ", " << rateDist->GetBinCenter(mostProbBin + j) << ") : "<< integralPeak<< " (Cut at " << Cut << ")" << endl;*/
    if (rateDist->GetBinCenter(mostProbBin + j) > RateCut){
      SeventyWidthBins = j;
      break;
    }
    
    if (integralPeak >= Cut){
      SeventyWidthBins = j;
      break;
    }
  }
  int minRateBin = mostProbBin - fabs(SeventyWidthBins + 1);
  //  if (minRateBin < 1) minRateBin = 1;
  
  int maxRateBin = mostProbBin + fabs(SeventyWidthBins + 1);
  //  if (maxRateBin > ref->GetNbinsX()) maxRateBin = ref->GetNbinsX();
  
  int nFills = 0;
  double sIntegral = 0;  
  double maxValueOfRate =rateDist->GetBinCenter(maxRateBin);
  
  for (int k=1; k< ref->GetNbinsX(); k++){
    
    double cont = ref->GetBinContent(k);
    
    if ( cont > maxValueOfRate ){
      corrected->SetBinContent(k, 0);
      sIntegral += cont;
      ExStrip* strip = exRoll->getStrip(startingStrip+k);
      strip->setRate(0);
      strip->Mask();
      nFills++;
    }
    
    else{
      corrected->SetBinContent(k, cont);
//      cout << startingStrip+k << " " << cont << endl;
      exRoll->getStrip(startingStrip+k)->setRate(cont);
      exRoll->AddToRate(cont);
      nFills++;
    }
  }
  double scaleFactor = (ref->Integral() - ref->Integral (minRateBin, maxRateBin )) / corrected->Integral() ;
  double intRemoved = rateDist->Integral(minRateBin, maxRateBin);
  
  /*    cout << "Statistics: " << endl;
	cout << " => Scale Factor: " << scaleFactor << endl;
	cout << " (*) Cut Fraction: " << Cut << endl;
	cout << " (*) Max Value Of Rate: " << maxValueOfRate << endl;
	cout << " (*) Cycles: " << nFills << endl;
	cout << " (*) Corrected Integral: " << corrected->Integral() << endl;
	cout << " (*) Distrib Integral: " << rateDist->Integral() << endl;
	cout << " (*) Subtracted Areat: " << sIntegral <<endl;
	cout << " (*) Sum: " << sIntegral + corrected->Integral() << endl;
	cout << " (*) Uncorrected Integral: " << ref->Integral() << endl;*/

  corrected->SetTitle("Corrected");
  ref->SetTitle("Uncorrected");
  return rateDist;
  
}

TH1F* NoiseAnalyzer::OldAssociate(ExRoll* exRoll, TH1F* ref, TH1F* corr, bool correctionEnabled){

  TH1F* corrected = corr;
  corrected->Reset();

  if (correctionEnabled != true) {

    for (int k=1; k<= ref->GetNbinsX(); k++){

      double cont = ref->GetBinContent(k);
      corrected->SetBinContent(k, cont);
      exRoll->getStrip(k)->setRate(cont);
      exRoll->AddToRate(cont);
      //      nFills++;
    }

    return corrected;
  }

  corrected->SetName("Corr");
  TH1F* rateDist = new TH1F("RateDistribution", "Rate Distribution", 10000, 0, 10000);
  double mostProbBin = 0;
  
  for (int k = 1; k<= ref->GetNbinsX(); k++){
    
    double value = ref->GetBinContent(k);
    if (value > 0.1){
      rateDist->Fill(value);
    }
  }
  
  rateDist->Scale(1/rateDist->Integral());

//  double distrib = rateDist->Integral();
  mostProbBin = rateDist->GetMaximumBin();
  int SeventyWidthBins = ref->GetNbinsX();

  double Cut = 0.40;  

  for (int j=0; j<rateDist->GetNbinsX(); j++){
  double integralPeak = rateDist->Integral(mostProbBin -j, mostProbBin +j);

/*  cout << "Integral (" << mostProbBin -j << ", " << mostProbBin +j << ") / ("<< rateDist->GetBinCenter(mostProbBin -j)
  << ", " << rateDist->GetBinCenter(mostProbBin + j) << ") : "<< integralPeak<< " (Cut at " << Cut << ")" << endl;*/

  if (integralPeak >= Cut){
      SeventyWidthBins = j;
      break;
    }
  }

  int minRateBin = mostProbBin - fabs(SeventyWidthBins + 1);
//  if (minRateBin < 1) minRateBin = 1;

  int maxRateBin = mostProbBin + fabs(SeventyWidthBins + 1);
//  if (maxRateBin > ref->GetNbinsX()) maxRateBin = ref->GetNbinsX();

  int nFills = 0;
  double sIntegral = 0;  
  double maxValueOfRate =rateDist->GetBinCenter(maxRateBin);

  for (int k=1; k< ref->GetNbinsX(); k++){

    double cont = ref->GetBinContent(k);
    
    if ( cont > maxValueOfRate ){
      corrected->SetBinContent(k, 0);
      sIntegral += cont;
      ExStrip* strip = exRoll->getStrip(k);
      strip->setRate(0);
      strip->Mask();
      nFills++;
    }
    
    else{
      corrected->SetBinContent(k, cont);
      exRoll->getStrip(k)->setRate(cont);
      exRoll->AddToRate(cont);
      nFills++;
    }
  }
  
  double scaleFactor = (ref->Integral() - ref->Integral (minRateBin, maxRateBin )) / corrected->Integral() ;
  double intRemoved = rateDist->Integral(minRateBin, maxRateBin);

/*    cout << "Statistics: " << endl;
    cout << " => Scale Factor: " << scaleFactor << endl;
    cout << " (*) Cut Fraction: " << Cut << endl;
    cout << " (*) Max Value Of Rate: " << maxValueOfRate << endl;
    cout << " (*) Cycles: " << nFills << endl;
    cout << " (*) Corrected Integral: " << corrected->Integral() << endl;
    cout << " (*) Distrib Integral: " << rateDist->Integral() << endl;
    cout << " (*) Subtracted Areat: " << sIntegral <<endl;
    cout << " (*) Sum: " << sIntegral + corrected->Integral() << endl;
    cout << " (*) Uncorrected Integral: " << ref->Integral() << endl;*/

  corrected->SetTitle("Corrected");
  ref->SetTitle("Uncorrected");
  
  return rateDist;
  
}



TH1F* NoiseAnalyzer::makeRateDistribution(){

	
	TH1F* corrected = (TH1F*)ref->Clone();
	corrected->SetName("Corr");
	TH1F* rateDist = new TH1F("RateDistribution", "Rate Distribution", 1000, 0, 1000);

	double mostProbBin = 0;

        for (int k = 1; k<= ref->GetNbinsX(); k++){

                double value = ref->GetBinContent(k);
                if (value > 0.1){
                rateDist->Fill(value);
                }
        }

        rateDist->Scale(1/rateDist->Integral());
        mostProbBin = rateDist->GetMaximumBin();
        int SeventyWidthBins = 0;
	cout << "continue" << endl;
        for (int j=1; j<=ref->GetNbinsX() / 2; j++){
                double integralPeak = rateDist->Integral(mostProbBin -j, mostProbBin +j);

                if (integralPeak > 0.50){
                        SeventyWidthBins = j;
                        break;
                }
        }
	cout << "endOfcycle" << endl;
        int minRateBin = mostProbBin - fabs(mostProbBin - SeventyWidthBins);
        int maxRateBin = mostProbBin + fabs(mostProbBin - SeventyWidthBins);
        int nFills = 0;

cout << "FIN QUI" << endl;
        for (int k=1; k<= ref->GetNbinsX(); k++){

                double cont = ref->GetBinContent(k);
                double xCoord =rateDist->GetBinCenter(rateDist->FindBin(maxRateBin));

                if ( cont > xCoord ){
                        corrected->SetBinContent(k, 0);
			cout << "Yes " << k << endl;
                        ExStrip* strip = exRoll->getStrip(k);
			cout << "no" << endl;
                        strip->Mask();
                        nFills++;
                }

                else{
                        corrected->SetBinContent(k, cont);
                        nFills++;
                }

        }
cout << "ANCORA" << endl;
        double scaleFactor = (ref->Integral() - ref->Integral (minRateBin, maxRateBin )) / corrected->Integral() ;
        double intRemoved = ref->Integral(minRateBin, maxRateBin);

        corrected->Scale( scaleFactor );

/*        cout << "Statistics: " << endl;

        cout << " (*) Cycles: " << nFills << endl;
        cout << " (*) Corrected Integral: " << corrected->Integral() << endl;
        cout << " (*) Subtracted Area: " << intRemoved << endl;
        cout << " (*) Sum: " << intRemoved + corrected->Integral() << endl;
        cout << " (*) Uncorrected Integral: " << ref->Integral() << endl;*/

        corrected->SetTitle("Corrected");
        ref->SetTitle("Uncorrected");

        delete rateDist;
        return corrected;

}
