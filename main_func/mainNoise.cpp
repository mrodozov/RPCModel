
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <cmath>
#include <stdlib.h>
/** non-standart headers , the model , the wrappers and ROOT */
#include "../core/DataObject.h"
#include "../core/ExtendedStrip.h"
#include "../core/ExtendedRoll.h"
#include "../ROOT/ROOT.h"
#include "../Extensions/Run.h"
#include "../Extensions/Acquisition.h"
#include "../Extensions/NoiseAnalyzer.h"
#include "../ROOT/ROOT.h"

using namespace std;

int main(int argc, char** argv){

  gROOT->ProcessLine(".x ~/setTDRStyle.C");
  TApplication app("myApp", 0, 0);
  TCanvas* c1 = new TCanvas();

  TFile* contFile = TFile::Open("ContainerRun2011.root");
  Acquisition Run2011(contFile);

  map<string, double> areamap;
  contFile->Close();

  double NormalizedRate = 0;

  Run* currentRun = Run2011.getRunById(167830);

  //Filling
  contFile = TFile::Open("ContainerRun2011.root");

  currentRun->FillFromContainer(contFile, areamap);
  contFile->Close();

  map<string,ExRoll* > barrelMap = currentRun->GetAllTheRolls();

  //string rollName0 = "W-1_RB3_2F";

  //  string rollName0 = "W-2_RB1out_9B";
//      string rollName0 = "W0_RB1in_2B";
//  	string rollName0 = "W-2_RB2in_10B";
  // string rollName0 = "W+1_RB2in_11F";
//   string rollName0 = "RE+1_2_36";
  /* string rollName2 = "RE+1_2_01";
     string rollName3 = "RE+1_2_02";*/
  // string rollName0 = "RE-2_2_21";
    string rollName0 = argv[1];
  NoiseAnalyzer ana0;

/*  TFile* refFile = TFile::Open(barrelMap[rollName0.c_str()]->getRootFileName().c_str());

  TH1F* rollReference0 = (TH1F*)( refFile->Get( barrelMap[rollName0.c_str()]->getTH1Path().c_str() ) );
  TH1F* dist0  = (TH1F*)( rollReference0->Clone() );
  TH1F* factor = ana0.Associate(barrelMap[rollName0.c_str()], rollReference0, dist0, true);*/

TFile* refFile = TFile::Open(barrelMap[rollName0.c_str()]->getRootFileName().c_str());
ana0.HealCloneByClone(barrelMap[rollName0.c_str()]);
TH1F* rollReference0 = (TH1F*)( refFile->Get( barrelMap[rollName0.c_str()]->getTH1Path().c_str() ) );
TH1F* dist0 = barrelMap[rollName0.c_str()]->buildHistoFromRoll();
TH1F* factor = ana0.FrequencyDistribution(rollReference0);


cout << "DONE" << endl;
cout << factor->GetName() << endl;
int firstBin = factor->FindFirstBinAbove(0);
int lastBin = factor->FindLastBinAbove(0);
double fcv = factor->GetBinCenter(firstBin);
double lcv = factor->GetBinCenter(lastBin);


  cout << "Luminosity: " << currentRun->getLumi() << endl;
  cout << "Integral by Histo (corr): " << dist0->Integral() << endl;
  cout << "Integral by Histo (ref): " <<  rollReference0->Integral() << endl;

  double integral;

  for (int k=1; k<=96;k++){
  integral += barrelMap[rollName0.c_str()]->getStrip(k)->getRate(); 
//	cout << k << ": " << barrelMap[rollName0.c_str()]->getStrip(k)->getRate() << endl; 
  }

  cout << "Empty Strip: " << barrelMap[rollName0.c_str()]->getEmptyStrip() << endl;
  cout << "Integral by strips:" << integral << endl;


  dist0->SetTitle(string("Corrected Rate (" + rollName0 + ")" ).c_str());
  dist0->GetXaxis()->SetTitle("Strip No.");
  dist0->GetYaxis()->SetTitle("Rate");

  rollReference0->SetTitle(string("Uncorrected rate (" + rollName0 +")" ).c_str());
  rollReference0->GetXaxis()->SetTitle("Strip No.");
  rollReference0->GetYaxis()->SetTitle("Rate (Hz)");
  rollReference0->GetYaxis()->SetTitleOffset(1.6);

  dist0->GetXaxis()->CenterTitle();
  dist0->GetYaxis()->CenterTitle();

  rollReference0->GetXaxis()->CenterTitle();
  rollReference0->GetYaxis()->CenterTitle();

  c1->Divide(3,1);

//  c1->cd(2);
  TCanvas* d0 = new TCanvas("d0", "d0", 600, 600);
  string filename0 = rollName0 + "_corr.pdf";
  dist0->Draw();
d0->Print(filename0.c_str());
//  c1->cd(1);
  TCanvas* d1 = new TCanvas("d1", "d1", 600, 600);
  rollReference0->SetTitleOffset(1.5);
  rollReference0->Draw();
  cout << rollReference0->Integral(33, 64) << endl;;
  string filename1 = rollName0 + "_uncorr.pdf";
  rollReference0->Draw();
  d1->Print(filename1.c_str());

//  c1->cd(3);
  TCanvas* d2 = new TCanvas("d2", "d2", 600, 600);
  factor->Rebin(10);
  factor->GetXaxis()->SetTitle("Rate (Hz)");
  factor->GetYaxis()->SetTitle("Entries");
  factor->GetXaxis()->CenterTitle();
  factor->GetXaxis()->SetRangeUser(0, 400);
  factor->GetYaxis()->CenterTitle();
  string filename2 = rollName0 + "_rdist.pdf";
  factor->Draw();
  d2->Print(filename2.c_str());

  c1->Draw();
  //c1->Print( filename.c_str() );

  app.Run();

  refFile->Close();

  return 0;

}
