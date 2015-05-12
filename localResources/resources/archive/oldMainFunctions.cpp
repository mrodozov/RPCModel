/* // comparing the old and the oficial area resources
  DataObject * objectWithNames = new DataObject;
  objectWithNames->getDataFromFile(argv[1],1);
  DataObject oldAreaResource;
  oldAreaResource.getDataFromFile(argv[2],2);
  DataObject newAreaResource;
  newAreaResource.getDataFromFile(argv[3],2);
  string currentName;
  ofstream OFS;
  OFS.open(argv[4]);
  for (int i = 0 ; i < objectWithNames->getLenght();i++){
    currentName = objectWithNames->getElementFromPropertyContainer(i+1,1);
    ExRoll * test1 = new ExRoll(currentName);
    ExRoll * test2 = new ExRoll(currentName);
    test1->setStripsAreaFromSource(oldAreaResource);
    test2->setStripsAreaFromSource_cmsswResource(newAreaResource);
    for (int j=0;j < test1->getClones();j++){
      OFS << test1->getStripsAreaFromClone(j+1) <<" "<< test2->getStripsAreaFromClone(j+1)<< " " <<test1->getRollIDofClone(j+1)<<" "<<test2->getRollIDofClone(j+1)<<"\n";
      OFS.clear();
    }
    delete test1,test2;
  }
  OFS.close();
*/


/*
 DataObject areaResource;
  areaResource.getDataFromFile(argv[9],2);
  
  string WR=argv[1],ID=argv[4],OID=argv[5],rootFname = argv[6];
  int WRN=atoi(argv[2]),Sector=atoi(argv[3]);
  
  ExRoll * aRoll = new ExRoll(WR,WRN,Sector,ID,OID);
  aRoll->setStripsRateFromRootFileSource(rootFname);
  aRoll->setStripsAreaFromSource(areaResource);
  //cout << aRoll->getFitValueFromCuttedRateHistoForClone(atoi(argv[7]),atoi(argv[8])) << endl;
  cout << aRoll->getAvgRatePSCusingCutValueAndSingleFit(atoi(argv[8])) << endl;
  //aRoll->getAverageRatePSCforAllClonesUsingRefitedHistos();
  delete aRoll;
*/


/*
  system("date | awk {'print $4'}");
  
  string trying;
  ifstream IFS;
  IFS.open(argv[2]);
  string LINE;
  while (getline(IFS,LINE)){i.e. in a close range)
  ofstream OS;
  string resultPath = argv[4];
  resultPath += "/"+LINE;
  OS.open(resultPath.c_str());
  // while -> read all possible rolls and pass them on the loop
  loopOnRunList(argv[1],LINE,atof(argv[3]),OS);
  OS.clear();
  OS.close();
  }
  //TH1F * h1 = new TH1F();
  // runlist , rolllist , cutvalue , outputdir - arguments
  
  system("date | awk {'print $4'}");
  */

/*
  // 1 -> runlist 2 -> rolllist 3 -> resultPath 4 -> areas
  DataObject areas;
  areas.getDataFromFile(argv[4],2);
  string roll_name;
  cout << "main" << endl;
  ifstream IFS;
  ofstream OFS;
  IFS.open(argv[2]);
  
  while(getline(IFS,roll_name)){
  OFS.open((argv[3]+roll_name).c_str());
  calculateRPCSforOneRollForRunlist(argv[1],roll_name,OFS,areas);
  OFS.close();
  IFS.clear();
  }
  IFS.close(); /** abnormal memory leak , still wonder from where it comes */ /
*/

/*
  areas.getDataFromFile(argv[7],2);
  string WR=argv[1],ID=argv[4],OID=argv[5],rootFname = argv[6];
  int WRN=atoi(argv[2]),Sector=atoi(argv[3]);
  // use a run list
  TFile nonPtr(argv[6],"READ","in");
  // so open the file here , and loop on this logic 
  
  ExRoll * aroll = new ExRoll(WR,WRN,Sector,ID,OID);
  aroll->setStripsRateFromRootFileSource(nonPtr);
  aroll->setStripsAreaFromSource(areas);
  cout << aroll->getAverageRatePSCforAllClonesUsingRefitedHistos() << endl;
  nonPtr.Close();
  delete aroll; */


/*
  system("date | awk {'print $4'}");
  
  string trying;
  ifstream IFS;
  IFS.open(argv[2]);
  string LINE;
  while (getline(IFS,LINE)){i.e. in a close range)
  ofstream OS;
  string resultPath = argv[4];
  resultPath += "/"+LINE;
  OS.open(resultPath.c_str());
  // while -> read all possible rolls and pass them on the loop
  loopOnRunList(argv[1],LINE,atof(argv[3]),OS);
  OS.clear();
  OS.close();
  }
  //TH1F * h1 = new TH1F();
  // runlist , rolllist , cutvalue , outputdir - arguments
  
  system("date | awk {'print $4'}");
  */


/*
  DataObject areaObject;
  areaObject.getDataFromFile(argv[5],2);
  string resultsContainer = argv[2];
  resultsContainer += "/";
  resultsContainer+= argv[7];
  ofstream OFS;
  OFS.open((resultsContainer).c_str());
  getRPCSforOneRollForRunlistWithCutValue(argv[1],argv[7],OFS,argv[3],argv[4],areaObject,atoi(argv[6]));
  */


/*
  TH1F * h1,* h2;
  TCanvas * can1 = new TCanvas("c1","can1",1200,700);
  TCanvas * can2 = new TCanvas("c2","can2",1200,700);
  can1->cd();
  can1->SetLogy();
  h1 = aroll->getRefitedRateHistoForClone(atoi(argv[7]));
  TF1 * linearFunc = new TF1("f","pol0",0,h1->GetNbinsX());
  h1->Fit(linearFunc);
  //cout << "parameter refited histo " << linearFunc->GetParameter(0) << endl;
  
  
  
  /*
  h1->Draw();
  can1->SaveAs(("example_refited_"+aroll->getRollIDofCloneWithNewIdentifiers(atoi(argv[7]))+".root").c_str());
  can1->SaveAs(("example_refited_"+aroll->getRollIDofCloneWithNewIdentifiers(atoi(argv[7]))+".png").c_str());
  delete h1;
  /*
  can2->SetLogy();
  can2->cd();
  h1 = aroll->getRateHistoForClone(atoi(argv[7]));
  h1->Draw();
  can2->SaveAs("example.root");
  can2->SaveAs("ex.png");
  */
  //delete aroll,can1,h1,can2,linearFunc;
  
  /*
  system("date | awk {'print $4'}");
  
  string trying;
  ifstream IFS;
  IFS.open(argv[2]);
  string LINE;
  while (getline(IFS,LINE)){
  ofstream OS;
  string resultPath = argv[4];
  resultPath += "/"+LINE;
  OS.open(resultPath.c_str());
  // while -> read all possible rolls and pass them on the loop
  loopOnRunList(argv[1],LINE,atof(argv[3]),OS);
  OS.clear();
  OS.close();
  }
  //TH1F * h1 = new TH1F();
  // runlist , rolllist , cutvalue , outputdir - arguments
  
  system("date | awk {'print $4'}");
  */