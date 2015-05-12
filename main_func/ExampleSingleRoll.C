#include "GetPlotRateVsLumiSingleRoll.C"

ExampleSingleRoll(){
   TGraph* mg2 = GetSinglePlotRateVsLumiSingleRoll(
    corr = false,
    Area_ = "EndCap", 
    Wheel_ = "+3", 
    Sector_ = "2", 
    Number_ = "22", 
    Letter_ = "F", 
    Year_ = "2011", 
    Corr_ = "UnCorrected", 
    PlotName_ = "RateVsLumi_", 
    RunNumber_ = 180252, 
    PartNo_ = "1",
	kBlue, 20
  );
  mg2->Draw("AP");
  gStyle->SetOptFit(0000);  
  string RollName=  GetRollName("RE+3_2_22", "1");
  TPaveText* text = new TPaveText(0.2, 0.7, 0.4, 0.8, "NDC");
  text->SetFillColor(0);
  text->AddText(RollName.c_str());
  text->Draw("same");
}
