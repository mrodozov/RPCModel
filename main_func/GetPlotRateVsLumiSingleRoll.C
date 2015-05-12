
TMultiGraph* GetPlotRateVsLumiSingleRoll(

   bool RemovePedestal_ = true,
   string Area_ = "EndCap", 
   string Wheel_ = "+1", 
   string Sector_ = "3", 
   string Number_ = "03", 
   string Letter_ = "F", 
   string Year_ = "2012", 
   string Corr_ = "UnCorrected", 
   string PlotName_ = "RateVsLumi_", 
   int RunNumber_ = 180252, 
   string PartNo_ = "1",
   int StartingColor = kOrange,
   int StartingMarker = 20
   ){
  
   gROOT->ProcessLine(".x ~/setTDRStyle.C");
  
   string Area = Area_;
   string Wheel = Wheel_;
   string Sector = Sector_; 
   string Number = Number_; 
   string Letter = Letter_;
   string Year = Year_; 
   string Corr = Corr_;
   string PlotName = PlotName_; 
   int RunNumber = RunNumber_;
   string partNo = PartNo_;
   
   string prefix = "/home/michele/prg/swfbck/Outputs/AllTheClones/" + Year + "/" + Corr + "/";
   
   string fName;
  
  TMultiGraph* mg = new TMultiGraph();
  
   if (Area == "Barrel")
      fName = prefix + "W" + Wheel + "_" + Sector + "_" + Number + Letter + ".root";
   if (Area == "EndCap")
      fName = prefix + "RE" + Wheel + "_" + Sector + "_" + Number + ".root";
  
   cout << "Loading " << fName << endl;
  
   TFile *f1 = TFile::Open(fName.c_str());
   TIter next(f1->GetListOfKeys());
   TKey *key;
   int marker = StartingMarker;
   int color = StartingColor;
   
   while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      TGraph *g = (TGraph*)key->ReadObj();

      if (PlotName.find("Lumi") != string::npos)
	      if (string(g->GetName()).find("Run") != string::npos) continue;
      if (PlotName.find("Run") != string::npos)
	      if (string(g->GetName()).find("Lumi") != string::npos) continue;


      if (partNo != "-1"){
	cout << g->GetName() << " " << PlotName + partNo << endl;
	if (string(g->GetName()) != PlotName + partNo)
	continue;	
      }			
     if (RemovePedestal_) RemovePedestal(g);
      mg->Add(g);
      g->SetMarkerStyle(++marker);
      g->SetMarkerColor(++color);
   }
  
  return mg;

}


TGraph*  GetSinglePlotRateVsLumiSingleRoll(

  bool RemovePedestal_ = true,
   string Area_ = "EndCap", 
   string Wheel_ = "+1", 
   string Sector_ = "3", 
   string Number_ = "03", 
   string Letter_ = "F", 
   string Year_ = "2012", 
   string Corr_ = "UnCorrected", 
   string PlotName_ = "RateVsLumi_", 
   int RunNumber_ = 180252, 
   string PartNo_ = "1",
   int StartingColor = kOrange,
   int StartingMarker = 20

   ){
  
   gROOT->ProcessLine(".x ~/setTDRStyle.C");
  
   string Area = Area_;
   string Wheel = Wheel_;
   string Sector = Sector_; 
   string Number = Number_; 
   string Letter = Letter_;
   string Year = Year_; 
   string Corr = Corr_;
   string PlotName = PlotName_; 
   int RunNumber = RunNumber_;
   string partNo = PartNo_;
   
   string prefix = "/home/michele/prg/swfbck/Outputs/AllTheClones/" + Year + "/" + Corr + "/";
   
   string fName;
  
  TGraph* mg = new TGraph();
  
   if (Area == "Barrel")
      fName = prefix + "W" + Wheel + "_" + Sector + "_" + Number + Letter + ".root";
   if (Area == "EndCap")
      fName = prefix + "RE" + Wheel + "_" + Sector + "_" + Number + ".root";
  
   cout << "Loading " << fName << endl;
  
   TFile *f1 = TFile::Open(fName.c_str());
   TIter next(f1->GetListOfKeys());
   TKey *key;
   int marker = StartingMarker;
   int color = StartingColor;
    
   while ((key = (TKey*)next())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      TGraph *g = (TGraph*)key->ReadObj();
      if (PlotName.find("Lumi") != string::npos)
	      if (string(g->GetName()).find("Run") != string::npos) continue;
      if (PlotName.find("Run") != string::npos)
	      if (string(g->GetName()).find("Lumi") != string::npos) continue;
     
      if (string(g->GetName()) != PlotName + partNo)
      continue;		
      mg = (TGraph*)g->Clone();
      if (RemovePedestal_) RemovePedestal(mg);
      mg->SetMarkerStyle(++marker);
      mg->SetMarkerColor(++color);
   }

  mg->GetXaxis()->SetTitle("Instantaneous Luminosity (10^{30} cm^{-2}s^{-1})");
  mg->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");

  return mg;
}

void RemovePedestal(TGraph* g){

    TF1* line = new TF1("line", "[0]+x*[1]", 800, 3000);
    line->SetParameter(0, 0.01);
    line->SetParameter(1, 0.001);

    TFitResultPtr resptr = g->Fit(line, "RS0");
    TFitResult* res = resptr.Get();
    if (!res){
         cout << "Fit failed: " << FileNames.at(k).c_str() << endl;
         continue;
    }

    for (int k=0; k<g->GetN(); k++){
	double x, y;
	g->GetPoint(k, x, y);
	g->SetPoint( k, x, y - line->GetParameter(0) );
    }

}

string GetRollName(string name, string clone){

string command = "dec " + name + " " + clone + " > test.txt"; 
gSystem->Exec(command.c_str());
ifstream in("test.txt");
string name;
in >> name;
in.close();
return name;
}
