/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <boost/concept_check.hpp>
#include "core/DataObject.h"
#include "core/ExtendedStrip.h"
#include "core/ExtendedRoll.h"
#include "core/Chip.h"
#include "Applications.h"
#include "ROOT/tdrStyle.h"
//#include "ServiceClasses/Service.h"


using namespace std;


void PrintGeometryTable(string InputWithRollNames, string Ouput, DataObject& Areas, DataObject& RawIDFile) {
    ifstream IFS; // intput file stream
    ofstream OFS; // output file stream
    IFS.open(InputWithRollNames.c_str());
    OFS.open(Ouput.c_str());
    string LineOfInput;//,WheelRing,ID,Opt_ID;
    //int WRN,Sector;
    while (getline(IFS,LineOfInput)) {
//     ParseOnlineRollTypeString(LineOfInput,WheelRing,WRN,Sector,ID,Opt_ID);
//     ExRoll * aRoll = new ExRoll(WheelRing,WRN,Sector,ID,Opt_ID);
        ExRoll * aRoll = new ExRoll(LineOfInput);
        aRoll->setRollRawIDfromSource(RawIDFile);
        aRoll->setStripsAreaFromSource_cmsswResource(Areas);
        aRoll->WriteGeomTable(OFS);
	
        delete aRoll;
        IFS.clear();
    }
    //cout << "its before the closure" << endl; // debug output
    IFS.close();
    OFS.close();
}



TH2F * getHVvsRatePlotForOneRollForRunList(string & rollName,const string & resourcePath,const string & suffix,DataObject & RunHVTempDataObject,DataObject & areas) {
    ExRoll * aroll ;
    string * bufferRunNumber = new string;
    double * buffHV = new double;
    double * refPress = new double;
    double * refTemp = new double;
    *buffHV = 0;
    *bufferRunNumber = "" ,*refPress = 1010, *refTemp=293;
    // check the range of the rate and the HV ... fo the HV is easy , for the rate owful ...
    
    TH2F * histo = new TH2F("h1","HV vs Rate",100,9500,10000,100,0,0.3);
    
    for (int i = 0 ; i < RunHVTempDataObject.getLenght() ; i ++) {
        aroll = new ExRoll(rollName);
        aroll->setStripsAreaFromSource_cmsswResource(areas);
        *bufferRunNumber=RunHVTempDataObject.getElementFromPropertyContainer(i+1,1); // not to use the long name++++++
        aroll->setStripsRateFromRootFileSource(resourcePath+suffix+*bufferRunNumber+".root");
        if (aroll->isInBarrel()) {
            *buffHV = atof(RunHVTempDataObject.getElementFromPropertyContainer(i+1,2).c_str());
        }
        else {
            *buffHV = atof(RunHVTempDataObject.getElementFromPropertyContainer(i+1,2).c_str());
        }
        
        *buffHV = *buffHV*((*refPress/atof(RunHVTempDataObject.getElementFromPropertyContainer(i+1,4).c_str()))*(atof(RunHVTempDataObject.getElementFromPropertyContainer(i+1,5).c_str())/(*refTemp)));
	
        if (aroll->getAvgRatePSCWithoutCorrections() > 0) {
            histo->Fill(*buffHV,aroll->getAvgRatePSCWithoutCorrections(),3);
        }
        delete aroll;
    }
    
    delete bufferRunNumber,buffHV,refPress,refTemp;
    return histo;
}



void GetSingleDoubleGapRatios(string MapOfSingleGapRolls, string regex, string DBfilesContainer, string runlist,string LumiFile,string histoTitle,int histoBins,int histocolor,int histoLineColor) {
    // result -> root files with the distribution
    // new histogram added 30.12.11 -> Single/Double gap ratio Vs luminosity

    // function done , most of the unnecessary sortings has been removed , works really fast and efficient
    int divider = 1;

    ifstream IFS,IFS2;
    string tmpW,tmpID,tmpOID,tmp_r_name
    ,nextIfEcap,previousIfEcap,
    LINE,RUN,theRoll,
    bufferMainRollName,bufferPrevRollName,bufferNextRollName;
    int tmpWORD,tempSector,nextIfBarrel,previousIfBarrel,FilteredRollsNumber;
    IFS.open(MapOfSingleGapRolls.c_str());
    istringstream ISS;
    stringstream SS; // SS , change this name , you nazi :D
    double theRate,tempVal,ratiosSumBuffer=0;
    map<string,string> MapOFSGrolls;
    map <string,map<string,double> > RateByRuns;
    map <string,map<string,double> >::iterator RateByRuns_iter;
    map<string,double*> RunNum_LS_Lumi;
    map<string, string *> RollContainer;

    TCanvas * can1 = new TCanvas ("can1","the title",1200,700);
    TCanvas * can2 = new TCanvas ("can2","RatiosAverage",1200,700);
    TCanvas * can3 = new TCanvas ("can3","RatiosMean",1200,700);
    TH1F * h1 = new TH1F("h1","DG/SG ratio",histoBins,0,10);
    TH2F * RatioAverageVsLumi = new TH2F("h2"," Ratios average Vs luminosity",100,0,3600,100,0,10);
    TH2F * RatiosDistrubutionMeanVsLumi = new TH2F("h3"," Ratios mean Vs luminosity",100,0,3600,100,0,10);

    // load in map all SG roll names

    ratiosSumBuffer = 0;
    while (getline(IFS,LINE)) {
        MapOFSGrolls[LINE] = LINE;
        IFS.clear();
    }
    IFS.close();

    IFS.open(runlist.c_str());

    // SG names loaded

    cout << "SG roll names loaded " << endl;

    // load the rate for each run

    while (getline(IFS,RUN)) {
        IFS2.open((DBfilesContainer+"online_database_"+RUN+".txt").c_str());
        map <string,double> roll_rate_map;
        if (IFS2.is_open()) {
            while (getline(IFS2,LINE)) {
                ISS.str(LINE);
                ISS >> theRoll >> theRate ;
                roll_rate_map[theRoll] = theRate;
                IFS2.clear();
                ISS.clear();
            }
            IFS2.close();
            RateByRuns[RUN] = roll_rate_map;
        }
    }
    
    // rate loaded
    
    cout << "Rate loaded" << endl;
    
    // load the luminosity as map for each run
    
    DataObject TheLumiDO(LumiFile,3);
    for (int i=1;i <= TheLumiDO.getLenght();i++) {
        double * pairs = new double[2];
        pairs[0] = TheLumiDO.getElementAsInt(i,2);
        pairs[1] = TheLumiDO.getElementAsDouble(i,3)/TheLumiDO.getElementAsDouble(i,2);
        pairs[1] = pairs[1]/23.31;
        RunNum_LS_Lumi[TheLumiDO.getElement(i,1)] = pairs;
    }

    // luminosity loaded

    cout << "Lumi loaded" << endl;

    // load a map with all possible roll names where each key is the SG and the two values are the neighbours . Do it once - use the map of the names to search for a rate entries

    for (map<string,string>::iterator SingleGapIter = MapOFSGrolls.begin();SingleGapIter != MapOFSGrolls.end();SingleGapIter++) { // loop the rolls
        if (SingleGapIter->first.find(regex) != string::npos) {
            tmp_r_name = SingleGapIter->first;
            // filter whats not in the current substring

            ExRoll * aRoll = new ExRoll(tmp_r_name);
	    
            string * name_pairs = new string[2];
            bufferMainRollName = aRoll->getFullOnlineRollID();
            bufferNextRollName = aRoll->getNextNeighbourName();
            bufferPrevRollName = aRoll->getPreviousNeighbourName();
            name_pairs[0] = bufferPrevRollName;
            name_pairs[1] = bufferNextRollName;
            RollContainer[bufferMainRollName] = name_pairs;

            // debug check , what would happen if the rolls are deleted , does this will lead to destruction of the map values

            delete aRoll;
            //cout  << bufferMainRollName << " " << bufferPrevRollName << " " << bufferNextRollName << endl;

        } // end of the filter if conditions

    }
    
    // end of loading map with SG and their neighbours
    
    cout << "Map of SG rolls and their neighbours loaded" << endl;
    FilteredRollsNumber = RollContainer.size();
    cout << "SG rolls map contains " << FilteredRollsNumber << " elements"<< endl;

    TH1F * SingleRunRatiosDistributionMean;

    for (RateByRuns_iter = RateByRuns.begin() ; RateByRuns_iter != RateByRuns.end() ; RateByRuns_iter++) {
        // loop the runs
        ratiosSumBuffer = 0;
        SingleRunRatiosDistributionMean = new TH1F(RateByRuns_iter->first.c_str(),"",100,0,100); // this histogram is to take the mean for the SG ratio values for each run

        for (map<string,string*>::iterator SGMapIter = RollContainer.begin();SGMapIter != RollContainer.end();SGMapIter++) { // loop the rolls

            if (RateByRuns_iter->second.find(SGMapIter->first)->second == 0) {
                //cout << " The roll"<< aRoll->getFullOnlineRollID() << " for run " << RateByRuns_iter->first << " has rate value 0 , not possible to calculate ratio ..." << endl;
            }

            tempVal = (RateByRuns_iter->second.find(SGMapIter->second[0])->second+RateByRuns_iter->second.find(SGMapIter->second[1])->second)/ divider; // the average of the neighbours
            tempVal = tempVal / RateByRuns_iter->second.find(SGMapIter->first)->second ;
            h1->Fill(tempVal);
            ratiosSumBuffer+=tempVal;
            SingleRunRatiosDistributionMean->Fill(tempVal);

        }

        // try to find the run number
        if (RunNum_LS_Lumi.find(RateByRuns_iter->first) != RunNum_LS_Lumi.end()) {

            RatioAverageVsLumi->Fill(RunNum_LS_Lumi.find(RateByRuns_iter->first)->second[1], (ratiosSumBuffer/FilteredRollsNumber) , 2 );
            RatiosDistrubutionMeanVsLumi->Fill(RunNum_LS_Lumi.find(RateByRuns_iter->first)->second[1], SingleRunRatiosDistributionMean->GetMean(), 2);

        }

        delete SingleRunRatiosDistributionMean;

    }

    setTDRStyle();
    can1->SetFillColor(0);
    can1->cd();
    //h1->SetFillColor(histocolor);
    h1->GetYaxis()->SetTitle("Number of entries");
    h1->GetXaxis()->SetTitle("DG/SG average rate ratio values");
    h1->SetLineColor(histoLineColor);
    h1->SetTitle(histoTitle.c_str());
    h1->Draw();
    can1->SaveAs((histoTitle+".root").c_str());
    can1->SaveAs((histoTitle+".png").c_str());
    h1->SaveAs((histoTitle+"_histo.root").c_str());
    delete h1;
    can2->cd();
    // put some style for this histos
    RatioAverageVsLumi->GetYaxis()->SetTitle("Average Ratio Value");
    RatioAverageVsLumi->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");

    RatioAverageVsLumi->SetMarkerStyle(kCircle);
    RatioAverageVsLumi->SetMarkerColor(kRed);
    RatioAverageVsLumi->SetTitle((histoTitle+" Average of ratio values vs luminosity").c_str());
    RatioAverageVsLumi->SetStats(NO);
    RatioAverageVsLumi->Draw();
    can2->SaveAs((histoTitle+"_scatterAverage.root").c_str());
    can2->SaveAs((histoTitle+"_scatterAverage.png").c_str());
    delete RatioAverageVsLumi;
    
    can3->cd();
    RatiosDistrubutionMeanVsLumi->GetYaxis()->SetTitle("Mean value of SG/DG values distribution");
    RatiosDistrubutionMeanVsLumi->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
    RatiosDistrubutionMeanVsLumi->SetMarkerStyle(kCircle);
    RatiosDistrubutionMeanVsLumi->SetMarkerColor(kBlue);
    RatiosDistrubutionMeanVsLumi->SetTitle((histoTitle+" Mean value of each run SG/DG ratio values distribution Vs luminosity").c_str());
    RatiosDistrubutionMeanVsLumi->SetStats(NO);
    RatiosDistrubutionMeanVsLumi->Draw();
    can3->SaveAs((histoTitle+"_scatterMeans.root").c_str());
    can3->SaveAs((histoTitle+"_scatterMeans.png").c_str());
    delete RatiosDistrubutionMeanVsLumi;
    // voila ;)
    
}



void extractAverageRatesFromRootFilesForListOfFiles(const std::string& runlist, const std::string& pathToRateResources,const string & suffix ,const string & pathTo_TM_Resources,const std::string& areaFileString ,const std::string& outputPath,const int & singleStripCutValue) {
    DataObject area;
    area.getDataFromFile(areaFileString,2);
    // but this one need new method with which the roll could assign its strip rate values from a TH1F object !!! - ok then -> Its done
    std::ofstream OFS;
    ifstream IFS;
    string LINE_IN_RUNLIST,rootFileName,histoCurrentName;
    IFS.open(runlist.c_str());
    int currentNumberOfNotCountedStrips=0,currentNumberOfCountedStrips;
    TFile * file;
    while (getline(IFS,LINE_IN_RUNLIST)) {
        // open the root file with that name in that directory , loop on the names inside and construct the rolls according to the name
        rootFileName = pathToRateResources+suffix+LINE_IN_RUNLIST+".root";
        DataObject ToMaskObj;
        ToMaskObj.getDataFromFile(pathTo_TM_Resources+LINE_IN_RUNLIST+".txt",2);
        OFS.open((outputPath+LINE_IN_RUNLIST).c_str());
        TH1F * h1;
        file = new TFile(rootFileName.c_str(),"READ","in");
        TIter nextkey( file->GetListOfKeys() );
        TKey *key;
        TObject *obj;
        while (key = (TKey*)nextkey()) {
            obj = key->ReadObj();
            h1 = (TH1F*)obj;
            histoCurrentName = h1->GetName();
            // populate the rolls here
            ExRoll * singleRoll;
            if (histoCurrentName.substr(0,1) == "W" || histoCurrentName.substr(0,2) == "RE") {
                singleRoll = new ExRoll(histoCurrentName);
                singleRoll->setStripsAreaFromSource_cmsswResource(area);
                singleRoll->setStripsRatesFromTH1FObject(h1);
                // add here logic , if the strip should be masked its rate must go zero
                //singleRoll->setStripsToBeMaskedFromSource(ToMaskObj);
                currentNumberOfNotCountedStrips = 0;
                currentNumberOfCountedStrips = 0;
                for (int i=1;i<97;i++) {
                    //if(singleRoll->getStrip(i)->isAboutToBeMasked()){
                    if (singleRoll->getStrip(i)->getRate() > singleStripCutValue) {
                        singleRoll->getStrip(i)->setRate(0);
                        currentNumberOfNotCountedStrips++;
                    }
                    if (singleRoll->getStrip(i)->getRate() > 0) {
                        currentNumberOfCountedStrips++;
                    }
                }
                // ok print here
                OFS << singleRoll->getFullOnlineRollID() <<" "<< singleRoll->getAvgRatePSCWithoutCorrections() <<" "<<currentNumberOfNotCountedStrips <<" "<<currentNumberOfCountedStrips+currentNumberOfNotCountedStrips <<"\n";
                delete singleRoll;
                OFS.clear();
            }
            OFS.clear();
            delete h1,obj;
	    
        }
        //file->Close();
        delete file;
        OFS.close();
    }
}

void getMeanValuesFromStripsDistributionsForRunlist(const std::string& runlist, const std::string& resourcePath, const std::string& sufix, const std::string& Out) {
    ofstream OFS;
    OFS.open(Out.c_str());
    string LINE,currentRootFile,histoCurrentName;
    ifstream IFS;
    IFS.open(runlist.c_str());
    TFile * rootFile;
    while (getline(IFS,LINE)) {
        currentRootFile = resourcePath+sufix+LINE+".root";
        rootFile = new TFile(currentRootFile.c_str(),"READ","in");
        TIter nextkey( rootFile->GetListOfKeys() );
        TH1F *h1;
        TKey *key;
        TObject *obj;
        while (key = (TKey*)nextkey()) {
            obj = key->ReadObj();
            h1 = (TH1F*)obj;
            histoCurrentName = h1->GetName();
            if (histoCurrentName.find("Strips - WIN") != std::string::npos) {

                OFS << LINE <<" "<< h1->GetMean()<<"\n";
            }
            OFS.clear();
            delete h1,obj;
        }
        rootFile->Delete();
        rootFile->Close("R");
        delete rootFile;
        IFS.clear();
    }
    IFS.close();
    OFS.close();
}

void WriteRollInactiveStripsRatios( string Output, DataObject& Masked, DataObject& Dead, DataObject& Areas, DataObject& RawIDFile, string rollNames,string runNumber){
  
  ofstream ResultsOFS;
  ResultsOFS.open(Output.c_str());  
  
  string onlineRollName;
  ifstream RollNamesIFstream;
  RollNamesIFstream.open(rollNames.c_str());
  
  while (getline(RollNamesIFstream,onlineRollName)){
      if (onlineRollName.find("RE") != string::npos) continue; // get barrel only
      ExRoll * aRoll = new ExRoll(onlineRollName);
      aRoll->allocStrips();
      aRoll->initStrips();
      
      aRoll->setMaskedStripsFromSource(Masked);
      aRoll->setDeadStripsFromSource(Dead);
      aRoll->setRollRawIDfromSource(RawIDFile);
      
      int clones = 0;
      clones = aRoll->getClones();
      
      if (aRoll->getMasked() > 0 || aRoll->getDead() > 0 ){
	
	for (int clone = 0 ; clone < clones ; clone++ ){
	  
	  if (aRoll->getClones() == 6 && clone < 2) continue;
	  double totalStripInClone = (96/clones) - aRoll->getUnplugedFromClone(clone+1);
	  double maskedInClone = aRoll->getMaskedFromClone(clone+1);
	  double deadInClone = aRoll->getDeadFromClone(clone+1);
	  double inactiveInClone = maskedInClone + deadInClone;
	  
	  ResultsOFS << runNumber << " " << aRoll->getRollIDofCloneWithNewIdentifiers(clone+1) << " ";
	  ResultsOFS << aRoll->getRawIDofClone(clone+1) << " ";
	  ResultsOFS << double(maskedInClone/totalStripInClone) << " " << double(deadInClone/totalStripInClone) << " " << double(inactiveInClone/totalStripInClone) << " ";
	  ResultsOFS << maskedInClone << " " << deadInClone << " " << inactiveInClone << " " << totalStripInClone << "\n";
	  ResultsOFS.clear();
	}
      }      
      delete aRoll;
  }
  
  ResultsOFS.close();
  RollNamesIFstream.close();
  
}

void WriteRollsAndStripsFilesForDB_usingRootFile(string rootFile, string OutputRolls,string OutputStrips,string ErrorFile, DataObject& Masked, DataObject& Dead, DataObject& ToMask, DataObject& ToUnmask, DataObject& Areas, DataObject& RawIDFile,string rollNames) {
    int exit_code = 0;
    ofstream StripsOFS;
    ofstream RollsOFS,ErrorOFS;
    StripsOFS.open(OutputStrips.c_str());
    RollsOFS.open(OutputRolls.c_str());
    ErrorOFS.open(ErrorFile.c_str());
    ifstream check;
    // fill the map with all online names
    map <string,bool> onlineNameMap;
    map <string,bool>::iterator ONM_iterator;
    string onlineRollName;
    check.open(rollNames.c_str());
    while (getline(check,onlineRollName)) {
        onlineNameMap[onlineRollName] = false;
	//cout << onlineRollName << endl;
        check.clear();
    }
    check.close();
    // end of fill
    TFile * rFile = new TFile(rootFile.c_str(),"READ","in");
    
    TIter nextkey(rFile->GetListOfKeys());
    TH1F *h1;
    TKey *key;
    TObject *obj;
    
    while (key = (TKey*)nextkey()) {
        h1 = (TH1F*)(key->ReadObj());
        string rollName = h1->GetName();
	//cout << rollName << endl;
        if (onlineNameMap.find(rollName) != onlineNameMap.end()) {
// 	    cout << "--" << rollName << endl;
            ExRoll * aRoll = new ExRoll(rollName);
	    aRoll->allocStrips();
	    aRoll->initStrips();
	    
            aRoll->setMaskedStripsFromSource(Masked);
            aRoll->setDeadStripsFromSource(Dead);
            aRoll->setStripsToBeMaskedFromSource(ToMask);
            aRoll->setStripsToBeUnmaskedFromSource(ToUnmask);
            aRoll->setStripsAreaFromSource_cmsswResource(Areas);
	    for (int cl=1 ; cl <= aRoll->getClones() ; cl++){
	      
	      //cout << aRoll->getRollIDofClone(cl) << endl;
	      //cout <<  aRoll->getRollOfflineIDofClone(cl) << endl;
	      //cout <<  aRoll->getRollIDofClone_withEtaPartSeparated(cl) << endl;
// 	      cout <<  aRoll->getRollIDofCloneWithNewIdentifiers(cl) << endl;
// 	      cout << "---"  << endl;
	    }
            aRoll->setRollRawIDfromSource(RawIDFile);
            aRoll->setStripsRatesFromTH1FObject(h1);
            aRoll->WriteResultForDB(RollsOFS);
            aRoll->WriteDetailedResultsForDB(StripsOFS,ErrorOFS);
            onlineNameMap[rollName] = true;
            delete aRoll;
	    
        }
        
        delete h1;

    }
    
    rFile->Delete();
    rFile->Close("R");
    
    // check the missing names and put the records for them in the ofstreams
    
    for (ONM_iterator = onlineNameMap.begin();ONM_iterator != onlineNameMap.end();ONM_iterator++) {
        
        if (!ONM_iterator->second) {
	    
            string r_name;
            r_name=ONM_iterator->first;
            ExRoll * chamber = new ExRoll(r_name);
	    chamber->allocStrips();
	    chamber->initStrips();
	    
            chamber->setRollRawIDfromSource(RawIDFile);
            for (int k=0;k < chamber->getClones() ; k++) {
                RollsOFS << chamber->getRawIDofClone(k+1) <<" "<<chamber->getRollIDofClone(k+1)<<" "<<" -99 -99 -99 -99 -99 -99\n";
                RollsOFS.clear();
                for (int j=k*(96/chamber->getClones());j < (k+1)*(96/chamber->getClones());j++) {
                    StripsOFS << chamber->getRawIDofClone(k+1) <<" "<< chamber->getStrip(j+1)->getOnlineNumber() <<" "<< chamber->getStrip(j+1)->getOfflineNumber() <<" -99 -99 -99\n";
                    StripsOFS.clear();
                }
            }
            delete chamber;
        }
    }
    
    
    RollsOFS.close();
    StripsOFS.close();
    ErrorOFS.close();
}

void WriteResultsUsingIOtextFilesAndDataSources(std::string inputWithRollNames, std::string outputFileName, DataObject & objectWithMasked, DataObject & objectWithDead, DataObject & objectWithRAW) {
    
    DataObject * objWithRollNames = new DataObject;
    Roll * aRoll ;
    ifstream File;
    ofstream OutFile;
    string LINE;
    File.open(inputWithRollNames.c_str());
    OutFile.open(outputFileName.c_str());
    objWithRollNames->getDataFromFile(inputWithRollNames,1);
    //std::cout << objWithRollNames->getLenght() << std::endl;
    for (int i=0;i < objWithRollNames->getLenght() ; i++) {
        std::getline(File,LINE);
        aRoll = new Roll(LINE);
        aRoll->setMaskedStripsFromSource(objectWithMasked);
        aRoll->setDeadStripsFromSource(objectWithDead);
        aRoll->setRollRawIDfromSource(objectWithRAW);
        aRoll->WriteResults(OutFile);
        delete aRoll;
	
    }
    File.clear();
    File.close();
    OutFile.clear();
    OutFile.close();
    
    delete objWithRollNames;
    
}


void writeIntrinsicForCMSSW_Rolls(DataObject & lumiResource,DataObject & area,string rootCont, string outCont,string IntrinsicFile,bool PlotOffline,bool PlotOnline) {//,double (*getRateCallBackFunc)(int)){


    int count = 0;
    ofstream OFS;
    ifstream IFS;
    string LINE;
    // write 'run instLumi' file subroutine
    OFS.open("fileCHE");
    for (int i = 0; i < lumiResource.getLenght();i++) {
        OFS << lumiResource.getElement(i+1,1) << " " << double((atof(lumiResource.getElement(i+1,3).c_str()))/atof(lumiResource.getElement(i+1,2).c_str()))/23.31 <<"\n";
        OFS.clear();
    }
    OFS.close();
    DataObject newLumi;
    newLumi.getDataFromFile("fileCHE",3);
    system("rm fileCHE");
    // end of subroutine
    
    map<int,map<string,double* > > runnumberMap; // the int is the runnumber
    map<int,map<string,double* > >::iterator runIterator;
    map<string,double* >::iterator rollIterator; // the string is the CMSSW id for the roll
    map<int,map<string,double* > > runnumberMap_online;
    map<string,double* >::iterator rollIterator_online;
    map<int,map<string,double* > >::iterator runIterator_online;
    
    for (int i = 1 ; i < newLumi.getLenght()+1 ; i++) { // loop on all runs in newLumi object
        ExRoll * roll;
        TH1F * h1;
        TFile * file = new TFile((rootCont+"total_"+newLumi.getElement(i,1)+".root").c_str(),"READ","in");
        cout << "reading file " <<newLumi.getElement(i,1) << endl;
        TIter nextkey(file->GetListOfKeys());
        TKey * key;
        TObject * obj1;
        map <string,double*> rollPairs;
        map <string,double*> rollPairs_online;

        while (key = (TKey*)nextkey()) { // for each file open , loop on the histograms inside
//      obj1 = key->ReadObj();
            h1 = (TH1F*)key->ReadObj();
            string * rollName = new string;
            *rollName = h1->GetName();
            if (rollName->substr(0,1) == "W" || rollName->substr(0,2) == "RE") { // TODO rewrite this condition
                roll = new ExRoll(*rollName);
                roll->setStripsRatesFromTH1FObject(h1);
                roll->setStripsAreaFromSource_cmsswResource(area);
                //cout << " reading file "<<newLumi.getElement(i,1) ;
                int * j = new int;
                *j = 0;
                double ** ptr;
                //roll->getRollIDofClone()
                ptr=new double * [roll->getClones()]; // should mean two new objects of type "pointer to double"
                //cout << "enter while" << endl; // debug
                for (*j=0;*j < roll->getClones() ; *j=*j+1) {
                    ptr[*j]=new double[2];
                    ptr[*j][0] = 0 ;
                    ptr[*j][1] = 0 ;
                    ptr[*j][0] = newLumi.getElementAsDouble(i,2);
                    ptr[*j][1] = roll->getRollRatePSCFromClone(*j+1); // change this with appropriate callback :D
                    rollPairs[roll->getRollIDofClone(*j+1)] = ptr[*j];
                }
                /** the ptr object hold the values to which the rollPairs points to. if we delete it , we delete the values as well*/
                //for( *j=0;*j <roll->getClones();*j=*j+1)
                //  delete [] ptr[*j];
                //delete [] ptr;

                double * a_ptr = new double[2];
                a_ptr[0] = newLumi.getElementAsDouble(i,2);
                a_ptr[1] = roll->getAvgRatePSCWithoutCorrections();
                rollPairs_online[roll->getFullOnlineRollID()] = a_ptr;

                delete roll,j;

            }
            //h1->Delete();
            delete h1;
            delete rollName;
        }

        //
        runnumberMap[newLumi.getElementAsInt(i,1)] = rollPairs;
        runnumberMap_online[newLumi.getElementAsInt(i,1)] = rollPairs_online;
        file->Delete();
        file->Close("R");
        delete file;
    }

    /** the data is stored in two maps , loop on it to find each roll rate*/

    int iter=0;
    map<string,double*> roll_with_values;
    map<int,map<string,double*> >::iterator runIterator2;
    map<string,double> rollName_intrinsicRate_map;
    if (PlotOffline) {

        IntrinsicFile = outCont+IntrinsicFile;
        OFS.open(IntrinsicFile.c_str());

        for (runIterator = runnumberMap.begin() ; (runIterator != runnumberMap.end() && iter == 0); runIterator++) {
            //now second loop to get the rolls one by one and to fill histograms

            for (rollIterator=runIterator->second.begin();rollIterator!=runIterator->second.end();rollIterator++) {
                // for each roll search for it in the runmap by name
                //cout <<rollIterator->first ;//<<" ";// << endl;
                double * d_ptr;
                TCanvas * can = new TCanvas(rollIterator->first.c_str(),rollIterator->first.c_str(),1000,700);
                TH2F *h = new TH2F(rollIterator->first.c_str(),"",1000,0,3500,1000,0,10);
                TF1 * f1 = new TF1("f1","[0]*pol1",0,3500);

                for (runIterator2 = runnumberMap.begin();runIterator2 != runnumberMap.end();runIterator2++) {
                    roll_with_values = runIterator2->second;
                    d_ptr = roll_with_values.find(rollIterator->first)->second;//lumi
                    //cout <<" " << runIterator2->first <<" "<<d_ptr[0] <<" "<<d_ptr[1];
                    h->Fill(d_ptr[0],d_ptr[1]);
                }
                can->cd();
                h->Fit(f1);
                h->Draw();
                OFS << rollIterator->first << " " << f1->GetMinimum() <<"\n";
                can->SaveAs((outCont+rollIterator->first+".root").c_str());
                //h->Delete();can->Delete(); f1->Delete();// delete cause segfault
                delete h,can,f1;
                cout << endl;
                OFS.clear();
            }
            OFS.close();
            iter++;
        }
    }

    iter = 0;

    if (PlotOnline) {
        //if plotoffline flag is passed as true
        IntrinsicFile = outCont+IntrinsicFile+"_online";
        OFS.open(IntrinsicFile.c_str());

        for (runIterator_online = runnumberMap_online.begin();runIterator_online != runnumberMap_online.end() ;runIterator_online++) {
            if (iter == 0) {
                int itr=0;
                for (rollIterator_online = runIterator_online->second.begin();rollIterator_online != runIterator_online->second.end() && itr < 1232;rollIterator_online++) {
                    TCanvas * can2 = new TCanvas(rollIterator_online->first.c_str(),rollIterator_online->first.c_str(),1000,700);
                    TH2F * h2 = new TH2F(rollIterator_online->first.c_str(),rollIterator_online->first.c_str(),1000,0,3500,1000,0,10);
                    TF1 * f2 = new TF1("f1","[0]*pol1",0,3500);
                    double * ptr_to_values;
                    for (runIterator_online = runnumberMap_online.begin();runIterator_online != runnumberMap_online.end() ;runIterator_online++) {
                        ptr_to_values = runIterator_online->second.find(rollIterator_online->first)->second;
// 	  ptr_to_values = runIterator_online->value.find(rollIterator_online->key)->value;
                        h2->Fill(ptr_to_values[0],ptr_to_values[1]);
                    }
                    itr++;
                    can2->cd();
                    h2->Fit(f2);
                    h2->SetMarkerStyle(kFullCircle);
                    h2->Draw();
                    OFS << rollIterator_online->first << " " << f2->GetMinimum() << "\n";
                    can2->SaveAs((outCont+rollIterator_online->first+".root").c_str());
                    delete h2,f2,can2;
                    OFS.clear();
                    //cout << "inner loop " << rollIterator_online->first << endl;
                }
                OFS.close();
                //cout << " outer loop " << endl;
                iter=1;
            }
            
        }
        
        //cout << "out of if" << endl;
    }
    // segmentation fault in the end of the execution , seems rollIterator_online contains more then 1232 elements , use complete map of online rolls for that
}


void plotRateVsLumi_using_the_database_files_rollLevel_Ecap_Offline(DataObject& LumiFile, string filesCont, string outCont, string IntrinsicFile, bool WriteNewIntrinsic,bool subtractIntrinsic,bool averageEntries,string GoodRunsMap,QueryObject * query) {
    
    // TODO add also the Rate vs Runnum as another histogram , it should use the same filters from the QueryObject
    // JSON object as a query ? :P
    // TODO
    //gStyle->SetOptStat(1111110);
    
    int divider = 1;//2; // in case we want the rate to be divided
    string gapInTitle = "gap/"; // in case we divide on two ,
    map <string,double> run_lumi_map,intrinsic_map;
    map <string,double>::iterator run_lumi_map_iter,intrinsic_map_iter,roll_rate_map_iter;
    map <string,map<string,double> > run_rollsMap_map;
    map <string,map<string,double> >::iterator run_rollsMap_map_iter;
    int min_lumi_sections = 0; // minimum lumisections
    istringstream iss;
    string a_line;
    ifstream i_f;
    map <string,string> good_runs;
    i_f.open(GoodRunsMap.c_str());
    while (getline(i_f,a_line)) {
        good_runs[a_line] = a_line;
        i_f.clear();
    }
    i_f.close();
    
    for (int i = 0 ; i < LumiFile.getLenght() ; i++) {
        if (LumiFile.getElementAsInt(i+1,2) > min_lumi_sections && good_runs.find(LumiFile.getElement(i+1,1)) != good_runs.end()) {// more than 100 lumisections
            run_lumi_map[LumiFile.getElement(i+1,1)] = (LumiFile.getElementAsDouble(i+1,3)/LumiFile.getElementAsDouble(i+1,2))/23.31;
        }
    }
    
    string buffStr;
    ifstream IFS;
    string roll,RAW_ID;
    double rate,rate_pcsq;
    int dead,masked,toU,toM;
    for (run_lumi_map_iter = run_lumi_map.begin();run_lumi_map_iter != run_lumi_map.end();run_lumi_map_iter++) {
        //cout << run_lumi_map_iter->first <<" "<<run_lumi_map_iter->second << endl;

        buffStr = filesCont+"offline_database_"+run_lumi_map_iter->first+".txt";
        IFS.open(buffStr.c_str());

        if (IFS.is_open()) {
            map<string,double> roll_rate_map;
            //cout << run_lumi_map_iter->first << endl;
            // do for each file
            int countLines = 0;
            while (getline(IFS,buffStr)) {
                if (countLines > 0) {
                    iss.str(buffStr);
                    rate_pcsq = 0 ;
                    //iss >> RAW_ID >> roll >> dead >> masked >>toU >> toM >> rate >> rate_pcsq;
		    iss >> roll >> rate_pcsq;
                    // temp if condition
                    if (roll.find("RE") != string::npos && roll.find("CH") != string::npos) {
                        roll_rate_map[roll] = rate_pcsq;
                        //cout << roll << " " << rate_pcsq << endl;
                    }

                }
                countLines++;
                iss.clear();
                IFS.clear();
            }
            run_rollsMap_map[run_lumi_map_iter->first] = roll_rate_map;

        }
        IFS.close();

    }
    
    //
    if (WriteNewIntrinsic) {
        // fill the histos for the intrinsic here , also create a file to store the results
        for (run_rollsMap_map_iter = run_rollsMap_map.begin() ; run_rollsMap_map_iter != run_rollsMap_map.end();run_rollsMap_map_iter++) {

        }
    }
    //if(!WriteNewIntrinsic){
    // if the flag is 0 , open the existing intrinsic file
    else {
        DataObject intrinsicObj(IntrinsicFile,2);
        for (int i=0;i< intrinsicObj.getLenght();i++) {
            intrinsic_map[intrinsicObj.getElement(i+1,1)] = intrinsicObj.getElementAsDouble(i+1,2);
        }
    }

    double currentIntrinsicValue=0,currentLumi=0;
    double rate_sum = 0;
    int count_recs =0;
    setTDRStyle(); // hell yeah , it works
    
    
    TLegend * leg;
    leg = new TLegend(0.218063,0.606759,0.404552,0.93226);
    TCanvas * a_can = new TCanvas("Can_vas","Title",1200,700);
    TCanvas * function_canvas = new TCanvas("FC","FC",1200,700);
    a_can->cd();
    
    for (int i=0 ; i < query->getHistoCounter() ; i++) {
        TH2F * tmpHist = new TH2F(query->getNameForRecord(i+1).c_str(),""/*query->getTitleForRecord(i+1).c_str()*/,1000,query->getOptMapForRecord(i+1).histoMinX,
                                  query->getOptMapForRecord(i+1).histoMaxX,1000,query->getOptMapForRecord(i+1).histoMinY,query->getOptMapForRecord(i+1).histoMaxY
                                 );
	
        double minLumi=50000,maxLumi=0;
	
        // the other loops goes here
        for (run_rollsMap_map_iter = run_rollsMap_map.begin() ; run_rollsMap_map_iter != run_rollsMap_map.end();run_rollsMap_map_iter++) {
            currentLumi = run_lumi_map.find(run_rollsMap_map_iter->first)->second;
            for (roll_rate_map_iter = run_rollsMap_map_iter->second.begin();roll_rate_map_iter != run_rollsMap_map_iter->second.end(); roll_rate_map_iter++) {
                currentIntrinsicValue = intrinsic_map.find(roll_rate_map_iter->first)->second;
                if (!subtractIntrinsic) {
                    currentIntrinsicValue = 0;
                }
                // here all the check for the name will goes
                //if (roll_rate_map_iter->first.find(query->getOptMapForRecord(i+1).histoRegexOption) != string::npos){
                if (rollFilter(roll_rate_map_iter->first,query->getOptMapForRecord(i+1).disk,query->getOptMapForRecord(i+1).ring,query->getOptMapForRecord(i+1).orientation,query->getOptMapForRecord(i+1).chamberStart,query->getOptMapForRecord(i+1).chamberEnd,query->getOptMapForRecord(i+1).partOfChamber) ) {
                    //cout << roll_rate_map_iter->first << endl;
                    if (!averageEntries) {
                        if (query->getOptMapForRecord(i+1).cutByRunRange
                                &&  atoi(run_rollsMap_map_iter->first.c_str()) >= query->getOptMapForRecord(i+1).runStart
                                &&  atoi(run_rollsMap_map_iter->first.c_str()) <= query->getOptMapForRecord(i+1).runEnd
                           )

                        {
                            /** @brief last point */
                            tmpHist->Fill(currentLumi,(rate_sum/count_recs)/divider,3);
                        }
                        if (query->getOptMapForRecord(i+1).cutByRunRange == false) {
                            // if there is no cutByRunRange - just fill the dots
                            //cout << "normal execution " << endl;
                            tmpHist->Fill(currentLumi,(rate_sum/count_recs)/divider,3);
                        }
                    }


                    rate_sum += roll_rate_map_iter->second - currentIntrinsicValue;
                    count_recs++;
                }
            }
            
            if (averageEntries) {
                if (query->getOptMapForRecord(i+1).cutByRunRange
                        &&  atoi(run_rollsMap_map_iter->first.c_str()) >= query->getOptMapForRecord(i+1).runStart
                        &&  atoi(run_rollsMap_map_iter->first.c_str()) <= query->getOptMapForRecord(i+1).runEnd
                   )
                {
                    tmpHist->Fill(currentLumi,(rate_sum/count_recs)/divider,3);

                }
                
                if (query->getOptMapForRecord(i+1).cutByRunRange == false) {
		  tmpHist->Fill(currentLumi,(rate_sum/count_recs)/divider,3);  
                }
            }
            
            if (query->getOptMapForRecord(i+1).cutByRunRange
                    &&  atoi(run_rollsMap_map_iter->first.c_str()) >= query->getOptMapForRecord(i+1).runStart
                    &&  atoi(run_rollsMap_map_iter->first.c_str()) <= query->getOptMapForRecord(i+1).runEnd
               )
            {
                if (minLumi > currentLumi) {
                    minLumi = currentLumi;
                }
                if (currentLumi > maxLumi) {
                    maxLumi = currentLumi;
                }
            }
            
            rate_sum = 0;
            count_recs = 0;
            /** @brief find the luminosity range */
	    
        }
        
        tmpHist->SetMarkerStyle(query->getOptMapForRecord(i+1).histoMarkerStyle);
        tmpHist->SetMarkerColor(query->getOptMapForRecord(i+1).histoColor);
        tmpHist->SetTitleSize(0.05);
        tmpHist->GetXaxis()->SetTitle(query->getOptMapForRecord(i+1).histoXtitle.c_str());
        tmpHist->GetYaxis()->SetTitle(query->getOptMapForRecord(i+1).histoYtitle.c_str());
        tmpHist->SetStats(false);
	tmpHist->GetCorrelationFactor();
        TF1 * func = new TF1(tmpHist->GetName(),"[0]+x*[1]",minLumi,maxLumi);
        double par1,par2;
        func->SetParName(0,"par0");
        func->SetParName(1,"par1");
        //func->SetRange(minLumi,maxLumi);
        // private case , remove when not needed
        //func->SetRange(0,10000); // in case of extrapolation
        func->SetLineColor(tmpHist->GetMarkerColor());
        func->SetLineWidth(3);
        //cout << func->GetLineWidth() << endl;
        //tmpHist->Fit(func,"R");
	//tmpHist->RebinX();
        //func->GetMaximumX()
        //cout << func->GetMaximum() << endl;
	
//	tmpHist->GetYaxis()->SetLabelOffset((tmpHist->GetYaxis()->GetLabelOffset())*0.2);
	
	tmpHist->GetYaxis()->SetTitleOffset(tmpHist->GetXaxis()->GetTitleOffset()*0.8);
	tmpHist->GetXaxis()->SetTitleOffset((tmpHist->GetXaxis()->GetTitleOffset())*0.9);
	tmpHist->GetXaxis()->SetTitleSize(tmpHist->GetYaxis()->GetTitleSize());
	tmpHist->GetXaxis()->SetLabelSize(0.04);
	tmpHist->GetYaxis()->SetLabelSize(tmpHist->GetXaxis()->GetLabelSize());
	
        leg->AddEntry(tmpHist,tmpHist->GetName(),"p");
        if (i+1==1) {
            a_can->cd();
            tmpHist->Draw();
            function_canvas->cd() ;
            func->Draw();
        }
        else {
            a_can->cd() ;
            tmpHist->Draw("same");
            function_canvas->cd() ;
            func->Draw("same");
        }
        
        
        //delete func;
        //delete tmpHist;
    }
    
//     a_can->
    a_can->cd();
    leg->SetFillColor(0);
    leg->SetTextSize(0.06);
    leg->SetBorderSize(0);
    leg->Draw();
    function_canvas->cd();
    leg->Draw();
    a_can->SaveAs((query->getCanvasTitle()+".root").c_str());
    a_can->SaveAs((query->getCanvasTitle()+".png").c_str());
    
    
}

void plotEcap_RateVsPhi(string rateFile, bool subtractIntrinsic, string fileWithIntrinsic,DataObject & area ,double cutValueSingleStrip,string SGmapFile,DataObject & LumiFile,QueryObject* query) {
    
    int divider = 1;
    TCanvas *c1 = new TCanvas("c1","multigraph",10,10,1200,700);
    c1->SetFillColor(0);
    TH1F *hr = c1->DrawFrame(0,0,6.282,48);
    //TH1F *hr = c1->DrawFrame(0,0,6.282,1.2);
    hr->SetXTitle("  #phi (rad) ");
    hr->GetXaxis()->SetTitleSize(hr->GetXaxis()->GetTitleSize()*1.5);
    hr->GetYaxis()->SetTitleSize(hr->GetYaxis()->GetTitleSize()*1.5);
    hr->GetXaxis()->SetTitleOffset(hr->GetXaxis()->GetTitleOffset()*0.8);
    hr->GetYaxis()->SetTitleOffset(hr->GetYaxis()->GetTitleOffset()*0.8);
    hr->GetXaxis()->SetLabelFont(42);
    hr->GetYaxis()->SetLabelFont(42);
    hr->SetYTitle("Rate (Hz/cm^{2})");
    hr->SetFillColor(0);
    
    TLegend * leg;
    //leg->SetTextSize(leg->GetTextSize()*1.2);
    leg = new TLegend(0.127019,0.573407,0.312775,0.896122);
    TPaveText * pt,* pt2,* secondText,* rateText;
    pt = new TPaveText(0.0994983,0.924332,0.253344,0.989614,"NDC"); // NDC sets coords
    pt2 = new TPaveText(0.492475,0.81454,0.899666,0.897626,"NDC");
    pt->SetTextFont(42);
    pt2->SetTextFont(42);
    secondText = new TPaveText(0.704112,0.904432,0.906021,0.975069,"NDC");
    secondText->SetFillColor(0);
    secondText->SetBorderSize(0);
    secondText->SetTextSize(0.08);
    secondText->AddText("CMS Preliminary");
    secondText->SetTextFont(42);
    
    string substring_runnum = rateFile.substr(rateFile.find("total_")+6,6);
    cout << " BLABLABLA is " << substring_runnum << endl;    
    double lum = LumiFile.getElementByKeyAsDouble(substring_runnum,2) / LumiFile.getElementByKeyAsDouble(substring_runnum,1);
    lum = lum / 23.31;
    cout << lum << endl;
    
    stringstream ss;
    ss << lum;
    ss.clear();
    //int run = 190705;
    
    //secondText->InsertText("#sqrt{s} = 8 TeV, (17.59 pb^{-1})");
    
    // relative to pad dimensions
    pt->SetFillColor(0); // text is black on white
    pt->SetBorderSize(0); //no shade
    pt->SetTextSize(0.04);
    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);
    
    // add here the Single Gap map
    map<string,string> SG_MAP;
    string LINE_OF_MAP;
    ifstream ifs_map;
    ifs_map.open(SGmapFile.c_str());
    while (getline(ifs_map,LINE_OF_MAP)) {
        SG_MAP[LINE_OF_MAP] = LINE_OF_MAP;
        ifs_map.clear();
    }
    ifs_map.close();
    
    // intrinsic map reading (definately from a file created earlier)
    
    TH1F * a_strange_histo;
    map<string,double> intrinsic_map;
    ifstream IFS;
    IFS.open(fileWithIntrinsic.c_str());
    istringstream ISS;
    string LINE,roll_name;
    double rateOfRoll;

    while (getline(IFS,LINE)) {
        ISS.str(LINE);
        ISS >> roll_name >> rateOfRoll;
        intrinsic_map[roll_name] = rateOfRoll;
        ISS.clear();
        IFS.clear();
    }
    IFS.close();

    // map is filled

    TFile * file = new TFile(rateFile.c_str(),"READ");
    for (int i=0;i<query->getOnlineEcapCounter();i++) {
        TIter nextkey(file->GetListOfKeys());
        TKey * key;
        TObject * obj1;
        TH1F * h1;
        string channel;
        double arrayOfRates[37],endcapPhi[37],ex[37],ey[37],sg_array[37];
        for (int ii = 0 ; ii < 37 ; ii++) {
            endcapPhi[ii] = (ii*6.28/36);
            //cout << endcapPhi[i] << " " << count<< endl;
            //cout  << ii << " " << arrayOfRates[ii] << endl;
	    arrayOfRates[ii] = 0;
            ex[ii]=0;
            ey[ii]=0;
            sg_array[ii] = -5;
        }
        
        while (key = (TKey*)nextkey()) {
            obj1 = key->ReadObj();
            h1 = (TH1F*)obj1;
            stringstream SS;
            string * rollName = new string;
            *rollName = h1->GetName();
            // check each name for the roll name and the ch number
            for (int j = 1;j < 37 ; j++) {
                if (j<10) {
                    SS << j;
                    string _j;
                    SS >> _j;
                    channel = "0"+_j;//atoi(j);
                    SS.clear();
                }
                else {
                    SS << j;
                    SS >> channel;
                    SS.clear();
                    //channel = atoi(j);
                }
                
                if (*rollName == query->getStringForEcapForRecord(i+1)+channel) {
                    ExRoll * aRoll = new ExRoll(*rollName);
		    aRoll->allocStrips();
		    aRoll->initStrips();
                    aRoll->setStripsAreaFromSource_cmsswResource(area);
                    aRoll->setStripsRatesFromTH1FObject(h1);
                    double intrinsicValue = intrinsic_map.find(aRoll->getFullOnlineRollID())->second;
                    if (!subtractIntrinsic) {
                        intrinsicValue = 0;
                    }
                    
                    /** @brief This value here should change according to the run conditions , it is */
                    /** @brief CONDITION TO BE REMOVED ! Use more smart method to fight the noise strips */
		    
                    double xbins = 1000;
                    int hist_range = 100000;
                    a_strange_histo = new TH1F(aRoll->getFullOnlineRollID().c_str(),aRoll->getFullOnlineRollID().c_str(),1000,0,10000);
		    
                    for (int f = 1; f < 97 ;f++) {
                        a_strange_histo->Fill(aRoll->getStrip(f)->getRate());
			
                    }
                    //a_strange_histo->SaveAs((aRoll->getFullOnlineRollID()+".root").c_str());
                    
                    //TODO - get rid of this implementation and use the common method removeNoisyStripsForCloneWithPercentValue
                    
                    for (int k=1;k < 97;k++) {
                        if (aRoll->getStrip(k)->getRate() > (a_strange_histo->GetMean() + (a_strange_histo->GetMean()*0.85) ) ) {
                            aRoll->getStrip(k)->setRate(0); /**  OR    aRoll->getStrip(k)->setRate(check_->GetMean()); */
                        }
                        /*if(aRoll->getStrip(k)->getRate() > cutValueSingleStrip ){
                           aRoll->getStrip(k)->setRate(0);
                         }*/
                    }
                    
                    /** @brief end of the special condition */

                    arrayOfRates[j-1] = (aRoll->getAvgRatePSCWithoutCorrections()-intrinsicValue)/divider;

                    if (SG_MAP.find(aRoll->getFullOnlineRollID()) != SG_MAP.end() ) {
                        sg_array[j-1] = arrayOfRates[j-1];
                    }

                    //cout << aRoll->getFullOnlineRollID() << endl;
                    delete a_strange_histo;
                    delete aRoll;
                }
            }
            delete rollName,h1;
        }
        
        arrayOfRates[36] = arrayOfRates[0];
        //cout << arrayOfRates[36] << endl;
        TGraphErrors * graph = new TGraphErrors(37,endcapPhi,arrayOfRates,ex,ey);
        TGraphErrors * graphSG = new TGraphErrors(37,endcapPhi,sg_array,ex,ey);
	
	
	for (int iii = 0 ; iii < 37 ; iii++){
	  cout << query->getEcapMapForRecord(i+1).histoName << " " << iii+1 << " " << arrayOfRates[iii] << endl;
	}
	
	
        graph->SetMarkerColor(query->getEcapMapForRecord(i+1).Color); // () black  2 red 4 blue 6 fucsia 8 green 9 violet
        graph->SetMarkerStyle(query->getEcapMapForRecord(i+1).Marker); // 20 circle 21 square 29 star 22/23 triangle
        graph->SetMarkerSize(query->getEcapMapForRecord(i+1).MarkerSize);
	graph->SetLineColor(query->getEcapMapForRecord(i+1).Color);
        graph->SetLineWidth(0);
	
        graphSG->SetMarkerColor(kRed);
        graphSG->SetMarkerStyle(graph->GetMarkerStyle());
        graphSG->SetMarkerSize(1.6);
        graphSG->SetLineColor(0);
        graphSG->SetLineStyle(0);
        graph->Draw("LP");
//         graphSG->Draw("LP");
        leg->AddEntry(graph,query->getEcapMapForRecord(i+1).histoName.c_str(),"p");
    }
//   leg->AddEntry();
//     TGraphErrors * emptyGraph = new TGraphErrors();
//     emptyGraph->SetMarkerColor(kRed);
//     emptyGraph->SetMarkerStyle(20);
//     emptyGraph->SetMarkerSize(1.6);
//     leg->AddEntry(emptyGraph,"SG mode","lp");
    
    setTDRStyle();
    c1->SetFillColor(0);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->Draw();
    pt->AddText("#sqrt{s} = 13 TeV");
    pt->Draw();
    pt->SetTextFont(leg->GetTextFont());
    pt->SetTextSize(leg->GetTextSize());
    //pt2->SetTextFont(leg->GetTextFont());
    pt->SetTextSize(leg->GetTextSize());
    pt2->SetTextSize(leg->GetTextSize());
    pt2->SetBorderSize(0);
    pt2->SetFillColor(0);
    pt2->AddText(query->getRateVsPhiText().c_str());
    pt2->Draw();
    //secondText->SetTextSize(secondText->GetTextSize()*1.25);
    //secondText->Draw();
    //rateText->Draw();
    c1->SaveAs((query->getCanvasTitle()+".root").c_str());
    //c1->SaveAs(("/home/rodozov/Desktop/RumiTeX/pictures/DISK2PHIASSYM.png").c_str());
}

void plotRateVsLumi_using_root_and_JSON(string data_folder ,DataObject & lumiFile , string intrinsicFile, DataObject& area, bool intrinsicShouldBeSubtracted,double cutThreshold, QueryObject* query,DataObject & exlusionFile,bool isOffline,bool divideRateOnLumi,bool debugOUTPUT,string & outputJSON) {
  
  // TODO - if it contains .json substring as last 5 symbols, open file with the string, else - try to parse the string as json.
  
  ptree query_JSON;
  
  
  int divider = 1;//2;
  map<string,double> run_lumi_map;
  map<string,map<string,double> > run_rollRate_map;
    map<string,double>::iterator run_lumi_map_iter,run_iterator;
    map<string,map<string,double> >::iterator run_rollRate_map_iter;
    map<string,double> intrinsic_map;
    int min_lumi_sections = 100;
    ifstream IFS;
    istringstream iss;
    string LINE,rollName;
    double rateOfRoll=0;
    double biggestOn_X=0;
    double biggestOn_Y=0;
    double currentLumi =0;
//     map<string,string> subparts;
//     for (int i = 0 ; query->getOnlineRollCounter() ; i++){
//       subparts[query->getOnlineRollMapForRecord(i+1).regex] = query->getOnlineRollMapForRecord(i+1).regex;
//     }
    ptree run_rateVlumi_JSON;
    DataObject areaDO("localResources/area_noise_cmssw_withRE4");
    
    /* to remove that shit !*/
    
    TPaveText * pt,* secondText;
    pt = new TPaveText(0.092511,0.930556,0.234949,0.997222,"NDC"); // NDC sets coords
    secondText = new TPaveText(0.653846,0.925595,0.950669,0.997024,"NDC");
    secondText->SetFillColor(0);
    secondText->SetBorderSize(0);
    secondText->SetTextSize(0.05);
    
    secondText->AddText("CMS Preliminary");
    pt->SetFillColor(0); // text is black on white
    pt->SetBorderSize(0); //no shade
    pt->SetTextSize(0.05);
    pt->AddText("#sqrt{s} = 13 TeV");
    /*   */
    // fill the intrinsic map
    
    DataObject DO_INTR_FILE(intrinsicFile);
    for (int i=1;i <= DO_INTR_FILE.getLenght();i++){
      intrinsic_map[DO_INTR_FILE.getElement(i,1)]= DO_INTR_FILE.getElementAsDouble(i,2);
    }
    
    //fill exclusion map
    map<string,string> exclusionsList;
    
    for(int i = 1; i <= exlusionFile.getLenght();i++){
      exclusionsList[exlusionFile.getElement(i,1)] = exlusionFile.getElement(i,1);
    }
    
    for (int i = 0 ; i < lumiFile.getLenght() ; i++) {
        if (lumiFile.getElementAsInt(i+1,2) > min_lumi_sections && (exclusionsList.find(lumiFile.getElement(i+1,1)) == exclusionsList.end())) {// more than 100 lumisections
	    currentLumi = (lumiFile.getElementAsDouble(i+1,3)/lumiFile.getElementAsDouble(i+1,2))/23.31;
	    //cout << lumiFile.getElement(i+1,1) << " " << currentLumi << endl;
	    
            run_lumi_map[lumiFile.getElement(i+1,1)] = currentLumi;
	    if(currentLumi > biggestOn_X) biggestOn_X = currentLumi;
	    
	    map<string,double> currentRates;
	    
	    TFile * file = new TFile((data_folder+"total_"+lumiFile.getElement(i+1,1)+".root").c_str(),"READ");
	    cout << "reading file " << data_folder+"total_"+lumiFile.getElement(i+1,1)+".root" << endl;
	    TIter nextkey(file->GetListOfKeys());
	    TKey * key;
	    TH1F * h1;
	    TObject * obj1;    
	    
	    if (! file->IsOpen()) { cout << "File " << data_folder+"total_"+lumiFile.getElement(i+1,1)+".root" << " missing !" << endl; continue ; }
            
            while (key = (TKey*)nextkey()) {
	      obj1 = key->ReadObj();
	      string nameOfRoll = obj1->GetName();
// 	      if (subparts.find(nameOfRoll) == subparts.end()) continue;
	      if (nameOfRoll.substr(0,1) == "W" || nameOfRoll.substr(0,2) == "RE") {
		
		h1 = dynamic_cast<TH1F*>(obj1);
		ExRoll * aroll = new ExRoll(nameOfRoll);
		aroll->setStripsAreaFromSource_cmsswResource(areaDO);
		aroll->setStripsRatesFromTH1FObject(h1);
		aroll->removeNoisyStripsForAllClonesWithPercentValue(100);
		if (isOffline) {
		  for (int r_id = 1 ; r_id <= aroll->getClones() ; r_id++){
		    currentRates[aroll->getRollIDofCloneWithNewIdentifiers(r_id)] = aroll->getAvgRatePSCWithoutCorrectionsForClone(r_id);
		  }
		}
		else {
		  currentRates[nameOfRoll] = aroll->getAvgRatePSCWithoutCorrections();
		}
		delete aroll;
	      }
	    }
	    
	    run_rollRate_map[lumiFile.getElement(i+1,1)] = currentRates;
	    
	    file->Close("R");
	    file->Delete();
	   
        }
    }
    
    biggestOn_X += 500;
    
    TLegend * leg;
    leg = new TLegend(0.135095,0.58678,0.352423,0.886329);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(6);
    leg->SetTextSize(30);
    
    TCanvas * can = new TCanvas("can",query->getCanvasTitle().c_str(),1200,700);
    can->SetFillColor(0);
    can->cd();
    
    /** ONLY TO FIND biggests -> first find the biggest value on Y axis to assign the Y */
    
    for (int i=0;i < query->getOnlineRollCounter() ; i++) {
        int counter=0;
        double current_rate=0;
        // first find the biggest value on Y axis to assign the Y 
	
	for (run_rollRate_map_iter = run_rollRate_map.begin();run_rollRate_map_iter != run_rollRate_map.end();run_rollRate_map_iter++) {
            counter = 0;
            current_rate = 0;
            for (run_iterator = run_rollRate_map_iter->second.begin();run_iterator != run_rollRate_map_iter->second.end();run_iterator++) {
                if (run_iterator->first.find(query->getOnlineRollMapForRecord(i+1).regex) != string::npos && run_iterator->second < cutThreshold) {
		  
                  current_rate +=run_iterator->second;
                  counter ++;
                }
            }
            if (current_rate/counter > biggestOn_Y) biggestOn_Y = (current_rate/counter);  
	    
	}
    }    
    
    biggestOn_Y += 0.5;
    int multiplier = 1;
    
    for (int i=0;i < query->getOnlineRollCounter() ; i++) {
        int counter;
        double current_rate,current_luminosity_;
        
	// first find the biggest value on Y axis to assign the Y 
	TH2F * hist = new TH2F(query->getOnlineRollMapForRecord(i+1).histoName.c_str(),"",1000,0,biggestOn_X,10000,0,biggestOn_Y);
	
	run_rateVlumi_JSON.add_child(query->getOnlineRollMapForRecord(i+1).regex,ptree());
	run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex).add_child("values",ptree());
	run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex).add<int>("marker",query->getOnlineRollMapForRecord(i+1).Marker);
	run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex).add<int>("color",query->getOnlineRollMapForRecord(i+1).Color);
	run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex).add<string>("title",query->getOnlineRollMapForRecord(i+1).histoName);
	//run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex).add<int>("title",query->getOnlineRollMapForRecord(i+1).);
	//run_rateVlumi_JSON.add<>();
	
	

	for (run_rollRate_map_iter = run_rollRate_map.begin();run_rollRate_map_iter != run_rollRate_map.end();run_rollRate_map_iter++) {
            counter = 0;
            current_rate = 0;
            for (run_iterator = run_rollRate_map_iter->second.begin();run_iterator != run_rollRate_map_iter->second.end();run_iterator++) {
              //if (run_iterator->first.find(query->getOnlineRollMapForRecord(i+1).regex) != string::npos && run_iterator->first.find("RE+1_1") == string::npos && run_iterator->first.find("RE-4_3_13") == string::npos && run_iterator->second < cutThreshold) {
	      if (run_iterator->second > cutThreshold) continue;
	      if ( run_iterator->first.find(query->getOnlineRollMapForRecord(i+1).regex) == string::npos ) continue;
	      if ( run_iterator->first.find("RE+1_1") != string::npos && ( ( query->getOnlineRollMapForRecord(i+1).regex.find("RE+1_1") == string::npos )) ) continue;
	      
	      current_rate +=run_iterator->second;
	      counter ++;
	      cout << run_iterator->first << " " << run_iterator->second << endl;
            }            
            current_luminosity_ = run_lumi_map.find(run_rollRate_map_iter->first)->second;            

	    if(query->getOnlineRollMapForRecord(i+1).cutByRunRange 
	     &&  atoi(run_rollRate_map_iter->first.c_str()) <=  query->getOnlineRollMapForRecord(i+1).runStart
	     &&  atoi(run_rollRate_map_iter->first.c_str()) >= query->getOnlineRollMapForRecord(i+1).runEnd
	  ) continue;
	  
	  if(divideRateOnLumi) { divider = current_luminosity_; multiplier = 1000;}
	  double res_r = current_rate/counter;
	  hist->Fill(current_luminosity_,res_r,3);
	  int cl_int = 0;
	  if (current_luminosity_ > 0 ) {cl_int = current_luminosity_ ; current_luminosity_ = cl_int;}
	  else { cl_int = current_luminosity_ * 1000 ; current_luminosity_ = double(cl_int) / 1000 ;}
	  ptree & array = run_rateVlumi_JSON.get_child(query->getOnlineRollMapForRecord(i+1).regex+".values");//.get_child("values");
	  string apair = boost::lexical_cast<string>(current_luminosity_)+","+boost::lexical_cast<string>(res_r);
	  ptree p;
	  string cl = boost::lexical_cast<string>(current_luminosity_);
	  p.put_value(cl);
	  p.put_value<double>(current_luminosity_);
	  array.push_back(std::make_pair("",p));
// 	  string rr = boost::lexical_cast<string>(res_r);
// 	  cl += "\"";
// 	  rr = "\""+rr;
	  p.put_value<double>(res_r);
  	  array.push_back(std::make_pair("",p));
	  
        }
        

        hist->SetMarkerStyle(query->getOnlineRollMapForRecord(i+1).Marker);
        hist->SetMarkerColor(query->getOnlineRollMapForRecord(i+1).Color);
        hist->SetStats(false);
	// to be removed after fit
	TF1 * func = new TF1((query->getOnlineRollMapForRecord(i+1).histoName+"_function").c_str(),"[0]+x*[1]",0,50000);
	
	
        if (i+1 == 1) {
            hist->GetXaxis()->SetTitle(query->getHistoXtitle().c_str());
            hist->GetYaxis()->SetTitle(query->getHistoYtitle().c_str());
	    hist->GetXaxis()->SetTitleSize(hist->GetXaxis()->GetTitleSize()*1.5);
	    hist->GetYaxis()->SetTitleSize(hist->GetYaxis()->GetTitleSize()*1.5);
	    hist->GetXaxis()->SetTitleOffset(hist->GetXaxis()->GetTitleOffset()*0.8);
	    hist->GetYaxis()->SetTitleOffset(hist->GetYaxis()->GetTitleOffset()*0.8);
            hist->Draw();
// 	    hist->SetName("twelve");
// 	    hist->SaveAs("2012.root");
        }
        else {
            hist->Draw("same");
// 	    hist->SetName("twelve");
// 	    hist->SaveAs("2012.root");
        }
        cout << "Correlation factor " << hist->GetCorrelationFactor() << endl;
        // hist->Fit(func);
	cout << hist->GetName() << " " << func->GetMaximum() << endl;
	if(query->getOnlineRollMapForRecord(i+1).histoName != ""){
        leg->AddEntry(hist,query->getOnlineRollMapForRecord(i+1).histoName.c_str(),"p");
	}
// 	delete func;
	
    }
    
    setTDRStyle();
    pt->Draw();
    secondText->Draw();
    leg->Draw();
    can->SaveAs((query->getCanvasTitle()+".png").c_str());
    can->SaveAs((query->getCanvasTitle()+".root").c_str());
    
    ofstream OFS(outputJSON.c_str());
    boost::property_tree::json_parser::write_json(OFS,run_rateVlumi_JSON);
    OFS.close();
}



/** //@brief  Pure technical function , just to write the database files - online , recalculated and stored
 *
 *
 *
 */

void print_online_dbfiles(string rootContainer,string outputContainer ,DataObject & lumiFile,DataObject& area,string RollList,bool corrections, double correctionInPercents) {

  map<string,double> run_lumi_map;
  map<string,double>::iterator run_lumi_map_iter;
  map<string,ExRoll*> mapOfRolls;
  //int min_lumi_sections = 100; // no rule for the lumisections here
  
  
  for (int i = 0 ; i < lumiFile.getLenght() ; i++) {
    //if (lumiFile.getElementAsInt(i+1,2) > min_lumi_sections ){// more than 100 lumisections
    run_lumi_map[lumiFile.getElement(i+1,1)] = (lumiFile.getElementAsDouble(i+1,3)/lumiFile.getElementAsDouble(i+1,2))/23.31;
    //}
  }
  
  DataObject RList(RollList,1);
    string singleRollName;
    for (int i=1;i <= RList.getLenght();i++) {
        singleRollName = RList.getElement(i,1);
        ExRoll * aroll = new ExRoll(singleRollName);
	aroll->allocStrips();
	aroll->initStrips();
        aroll->setStripsAreaFromSource_cmsswResource(area);
        mapOfRolls[RList.getElement(i,1)] = aroll;
    }
    
    


    for (run_lumi_map_iter = run_lumi_map.begin();run_lumi_map_iter != run_lumi_map.end();run_lumi_map_iter++) {
        TFile * file = new TFile((rootContainer+"total_"+run_lumi_map_iter->first+".root").c_str(),"READ");
        TIter nextkey(file->GetListOfKeys());
        TKey * key;
        TH1F * h1;
        TObject * obj1;
        double rollMaxValue=0;
        ExRoll * aRoll;
        //map <string,double> temp_roll_rate_map ;
        if (file->IsOpen()) {
            ofstream OFS;
            OFS.open((outputContainer+"online_database_"+run_lumi_map_iter->first+".txt").c_str());
            while (key = (TKey*)nextkey()) {
                h1 = (TH1F*)key->ReadObj();
                string * nameOfRoll = new string;
                *nameOfRoll = h1->GetName();
                if (nameOfRoll->substr(0,1) == "W" || nameOfRoll->substr(0,2) == "RE") {
                    aRoll = mapOfRolls.find(*nameOfRoll)->second;//(*nameOfRoll);
                    aRoll->setStripsRatesFromTH1FObject(h1);
                    //aRoll->setStripsAreaFromSource_cmsswResource(area);
                    //temp_roll_rate_map[aRoll->getFullOnlineRollID()] = aRoll->getAvgRatePSCWithoutCorrections();

                    // add the two cases - with or without correction
                    if (corrections) {
                        rollMaxValue = aRoll->getMaxRateValue();
                        for (int i=1;i <= aRoll->getClones() ; i++) {
                            TH1F * histCorrection = new TH1F(aRoll->getRollIDofClone(i).c_str(),aRoll->getRollIDofClone(i).c_str(),rollMaxValue+5,0,rollMaxValue+5);
                            for (int j=(i-1)*(96/aRoll->getClones()); j < (i)*(96/aRoll->getClones());j++) {
                                histCorrection->Fill(aRoll->getStrip(j+1)->getRate());

                            }
                            for (int j=(i-1)*(96/aRoll->getClones()); j < (i)*(96/aRoll->getClones());j++) {
                                if (aRoll->getStrip(j+1)->getRate() > (histCorrection->GetMean() + (correctionInPercents/100)*histCorrection->GetMean())) {
                                    aRoll->getStrip(j+1)->setRate(0);
                                }
                            }
                            delete histCorrection;
                        }


                    }

                    OFS << aRoll->getFullOnlineRollID() <<" "<<aRoll->getAvgRatePSCWithoutCorrections() <<"\n";


                    //delete aRoll;
                }
                delete nameOfRoll,h1;
                OFS.clear();
            }
            OFS.close();

        }
        //run_rollRate_map[run_lumi_map_iter->first] = temp_roll_rate_map;
        // when ready with this file
        cout << "reading run" << run_lumi_map_iter->first << "..." << endl;
        file->Close("R");
        file->Delete();
    }

}


void print_offline_db_files(string rootContainer,string outputContainer ,DataObject & lumiFile,DataObject& area,string RollList,bool corrections, double correctionInPercents) {

  map<string,double> run_lumi_map;
  map<string,double>::iterator run_lumi_map_iter;
  map<string,ExRoll*> mapOfRolls;
  //int min_lumi_sections = 100; // no rule for the lumisections here
  string nameOfRoll="";
  for (int i = 0 ; i < lumiFile.getLenght() ; i++) {
    //if (lumiFile.getElementAsInt(i+1,2) > min_lumi_sections ){// more than 100 lumisections
    run_lumi_map[lumiFile.getElement(i+1,1)] = (lumiFile.getElementAsDouble(i+1,3)/lumiFile.getElementAsDouble(i+1,2))/23.31;
    //}
  }
  
  DataObject RList(RollList,1);
    string singleRollName;
    for (int i=1;i <= RList.getLenght();i++) {
        singleRollName = RList.getElement(i,1);
        ExRoll * aroll = new ExRoll(singleRollName);
	aroll->allocStrips();aroll->initStrips();
        aroll->setStripsAreaFromSource_cmsswResource(area);
        mapOfRolls[RList.getElement(i,1)] = aroll;
    }
    cout << "Rolls are set ... " << endl;
    for (run_lumi_map_iter = run_lumi_map.begin();run_lumi_map_iter != run_lumi_map.end();run_lumi_map_iter++) {
        TFile * file = new TFile((rootContainer+"total_"+run_lumi_map_iter->first+".root").c_str(),"READ");
        TIter nextkey(file->GetListOfKeys());
        TKey * key;
        TH1F * h1;
        TObject * obj1;
        double rollMaxValue=0;
        ExRoll * aRoll;
	ofstream OFS;
        //map <string,double> temp_roll_rate_map ;
	
        if (file->IsOpen()) {
            
            OFS.open((outputContainer+"offline_database_"+run_lumi_map_iter->first+".txt").c_str());
	    
            while (key = (TKey*)nextkey()) {
                h1 = (TH1F*)key->ReadObj();                
                nameOfRoll = h1->GetName();
		
                if (nameOfRoll.substr(0,1) == "W" || nameOfRoll.substr(0,2) == "RE") {
                    aRoll = mapOfRolls.find(nameOfRoll)->second;
                    aRoll->setStripsRatesFromTH1FObject(h1);

                    // add the two cases - with or without correction
                    if (corrections) {
		      aRoll->removeNoisyStripsForAllClonesWithPercentValue(correctionInPercents);
                    }
                    
                    for(int clns = 1 ; clns <= aRoll->getClones(); clns++){
		      
		      OFS << aRoll->getRollIDofCloneWithNewIdentifiers(clns) << " " << aRoll->getAvgRatePSCWithoutCorrectionsForClone(clns) << "\n";
		      
		    }
                    
                }
                
                delete h1;
                OFS.clear();
            }
            
            OFS.close();

        }
        //run_rollRate_map[run_lumi_map_iter->first] = temp_roll_rate_map;
        // when ready with this file
        cout << "reading run" << run_lumi_map_iter->first << "..." << endl;
        file->Close("R");
        file->Delete();
    }

}

bool rollFilter(string rollName,string Disk, string Ring, string Orientation, int chamberStart,int chamberEnd,string part) {

    // TODO -> make the filter to be able to take also Barrel parts :)
    string chamberFromName;

    if (
        (
            (
                (rollName.substr(3,1) == "1" && Disk == "1") ||
                (rollName.substr(3,1) == "2" && Disk == "2") ||
                (rollName.substr(3,1) == "3" && Disk == "3")
            )
            ||
            (
                ((rollName.substr(3,1) == "1" || rollName.substr(3,1) == "2")  && Disk == "12") ||
                ((rollName.substr(3,1) == "2" || rollName.substr(3,1) == "3")  && Disk == "23") ||
                ((rollName.substr(3,1) == "1" || rollName.substr(3,1) == "3")  && Disk == "13")
            )
            ||
            (
                (rollName.substr(3,1) == "1" || rollName.substr(3,1) == "2" || rollName.substr(3,1) == "3")  && Disk == "123"
            )

        )

        &&

        (
            (rollName.find("-") != string::npos && Orientation == "-") ||
            (rollName.find("+")!= string::npos && Orientation == "+" ) ||
            ((rollName.find("+")!= string::npos || rollName.find("-"))!= string::npos && (Orientation == "+-" || Orientation == "-+" ))
        )
        /** @brief first conditions , search for the + - conditions : + , - or both */
        &&
        (
            (rollName.find("R2")!= string::npos && Ring == "R2") ||
            (rollName.find("R3")!= string::npos && Ring == "R3") ||
            ( ( rollName.find("R2")!= string::npos || rollName.find("R3")!= string::npos) && Ring == "R2,3" )
        )
        /** @brief second , it search for the ring condition ring 2 , ring 3 or ring 2 and 3 */
        &&
        (
            (
                (rollName.find("_A")!= string::npos && part == "A" ) ||
                ( rollName.find("_B")!= string::npos && part == "B") ||
                ( rollName.substr(12,2)== "_C" && part == "C")
            )

            ||

            (
                ((rollName.find("_A")!= string::npos || rollName.find("_B")!= string::npos) && part == "AB"  ) ||
                ((rollName.find("_B")!= string::npos || rollName.substr(12,2)== "_C") && part == "BC"  ) ||
                ((rollName.find("_A")!= string::npos || rollName.substr(12,2)== "_C") && part == "AC"  )
            )
            ||

            ((rollName.find("_A")!= string::npos || rollName.find("_B")!= string::npos || rollName.find("_C")) && part == "ABC"  )

        )

        /** @brief third , it search for which part or for what kind of combination */

    ) {

        if (rollName.find("CH0") != string::npos) {
            chamberFromName = rollName.substr(11,1);
//       chamberFromName = chamberFromName.substr(2,2);
        }
        else {
            chamberFromName = rollName.substr(10,2);
//       chamberFromName = chamberFromName.substr(3,2);
        }

        /*
        cout << atoi(chamberFromName.c_str()) << endl;
        cout << chamberStart << " start" << endl;
        cout << chamberEnd << " end" << endl;
        */

        bool var;

        if (atoi(chamberFromName.c_str()) >= chamberStart && atoi(chamberFromName.c_str()) <= chamberEnd) {
            var = true;
        }
        else {
            var = false;
        }
        return var;
        /** @brief forth , find does the chamber number goes between the start and end of the filter values */

    }

    else {
        return false;
    }
}

bool universalRollFilter(string barrel_or_ecap, int Wheel_or_Disk_number_start, int Wheel_or_Disk_number_end, int EcapRing_or_BarrelLayer_start, int EcapRing_or_BarrelLayer_end, int Sector_start, int Sector_end, string optionallBarrelID, int partitionStart, int partitionEnd, bool usedForOffline) {

}



void WriteHTMLforNoisePageStripConditions(string inputWithRollNames, string outputFolder,string RateFile, DataObject& Masked, DataObject& Dead, DataObject& ToMask, DataObject& ToUnmask,map<string,string> towerMap) {
    DataObject rollList(inputWithRollNames,1);
    ExRoll * aRoll;
    string currentRollName;
    ofstream OFS_masked,OFS_dead,OFS_TM,OFS_TU;
    // be careful here its used i for iteration , not i+1
    OFS_TM.open((outputFolder+"ToMask_AllTowers_offline.txt").c_str());
    OFS_TU.open((outputFolder+"ToUnmask_AllTowers_offline.txt").c_str());
    OFS_dead.open((outputFolder+"Dead_AllTowers_offline.txt").c_str());
    OFS_masked.open((outputFolder+"Masked_AllTowers_offline").c_str());

    for (int i=1;i < rollList.getLenght() + 1 ; i++) {
        // one element is one roll names - linkboard name
        currentRollName = rollList.getElement(i,1);
        aRoll = new ExRoll(currentRollName);
	aRoll->allocStrips();
	aRoll->initStrips();
        aRoll->setStripsRateFromRootFileSource(RateFile);
        aRoll->setStripsToBeMasked_withTheTime_andMaxRate_FromSource(ToMask);
        aRoll->setStripsToBeUnmasked_withMaxRate_FromSource(ToUnmask);
        aRoll->setMaskedStripsFromSource(Masked);
        aRoll->setDeadStripsWithMaxRate(Dead);
//    aRoll->setDeadStripsFromSource(Dead);  // dont remove , its the old method used where only the dead strips was printed
//     aRoll->setStripsToBeMaskedFromSource(ToMask); // these methods are assigning only the numbers , but not the rest of the details
//     aRoll->setStripsToBeUnmaskedFromSource(ToUnmask); //
        aRoll->setTowerNameFromMap(towerMap);

        //OFS_masked.open((outputFolder+"ToMask_AllTowers_offline.txt").c_str());

        for (int j=0;j < aRoll->getClones() ; j++) {
            // if the tower file is not open , open it , if it is - write inside
            // check each of the clones , we need only files for those that have something to be written

            if (aRoll->getMaskedFromClone(j+1) > 0) {
                OFS_masked << "\n"<< "Strips masked in roll " << aRoll->getRollIDofClone_withEtaPartSeparated(j+1) << "("  <<  aRoll->getFullOnlineRollID()<< ") | " <<" "<< aRoll->getTowerName() << "\n"
                << "| LB channel | Strip Number | "<< aRoll->getTowerName() << "\n" ;
                //<< "OR LB name(so called online) "<< aRoll->getFullOnlineRollID()  << " " << aRoll->getTowerName()<< "\n" << "\n";
            }
            if (aRoll->getDeadFromClone(j+1) > 0) {
                OFS_dead << "\n" << "Strips considered 'dead' in roll " << aRoll->getRollIDofClone_withEtaPartSeparated(j+1) << "("  <<  aRoll->getFullOnlineRollID()<< ") | "<< " " << aRoll->getTowerName() << "\n"
                << "| LB channel | Strip Number | Max Rate [ Hz ] | "<< aRoll->getTowerName() << "\n" ;
                //<< "\n" <<"OR LB name(so called online) "<< aRoll->getFullOnlineRollID() << " " << aRoll->getTowerName()<< "\n" << "\n";
            }
            if (aRoll->getStripsToBeMaskedFromClone(j+1) > 0) {
                OFS_TM << "\n"<< "Strips to be masked in roll " << aRoll->getRollIDofClone_withEtaPartSeparated(j+1) << " ("  <<  aRoll->getFullOnlineRollID()<< ") | " << " " << aRoll->getTowerName() << "\n"
                << "| LB channel | Strip Number | minutes     | Noise Rate [kHz] | " << aRoll->getTowerName()<< "\n" ;

                // 	<< "\n" <<"OR LB name(so called online) "<< aRoll->getFullOnlineRollID()<< " " << aRoll->getTowerName()<< "\n" << "\n";
            }
            if (aRoll->getStripsToBeUnmaskedFromClone(j+1) > 0) {
                OFS_TU << "Strips to be unmasked in roll  " << aRoll->getRollIDofClone_withEtaPartSeparated(j+1) << "("  <<  aRoll->getFullOnlineRollID()<< ") | " << " " << aRoll->getTowerName() << "\n" ;
                OFS_TU << "| LB channel | Strip Number | Max Rate [ Hz] | " << aRoll->getTowerName()<< "\n" ;


// 	"OR LB name(so called online)"<< aRoll->getFullOnlineRollID() << " " << aRoll-> getTowerName() << "\n" <<"\n";
            }

            for (int k=j*(96/aRoll->getClones()); k < (j+1)*(96/aRoll->getClones()); k++) {
                // write inside the file IF THE FILE .is_open() , otherwise - don't
                if (OFS_dead.is_open()) {
                    if (aRoll->getStrip(k+1)->getIsDead()) {
                        OFS_dead << "|   " << aRoll->getStrip(k+1)->getOnlineNumber() <<"   |   " <<  aRoll->getStrip(k+1)->getOfflineNumber() << "   |   " << aRoll->getStrip(k+1)->getMaxRate() << " Hz   |   " << aRoll->getTowerName() << "\n" ;
                    }
                }

                if (OFS_masked.is_open()) {
                    if (aRoll->getStrip(k+1)->getIsMasked()) {
                        OFS_masked << "|   " << aRoll->getStrip(k+1)->getOnlineNumber() <<"       |     " <<  aRoll->getStrip(k+1)->getOfflineNumber() << " "<< aRoll->getTowerName() << "\n" ;

                    }
                }

                if (OFS_TM.is_open()) {
                    if (aRoll->getStrip(k+1)->isAboutToBeMasked()) {
                        OFS_TM << "|   " << aRoll->getStrip(k+1)->getOnlineNumber() <<"       |   " <<  aRoll->getStrip(k+1)->getOfflineNumber() << "         |   " << aRoll->getStrip(k+1)->getTimeAsNoisy() << "   |   " << aRoll->getStrip(k+1)->getMaxRate() << " kHz    | "<< aRoll->getTowerName()<< "\n" ;
                    }
                }

                if (OFS_TU.is_open()) {
                    if (aRoll->getStrip(k+1)->isAboutToBeUnmasked()) {
                        OFS_TU << "|   " << aRoll->getStrip(k+1)->getOnlineNumber() <<"   |   " <<  aRoll->getStrip(k+1)->getOfflineNumber() << "   |   " << aRoll->getStrip(k+1)->getMaxRate() << " Hz |"<< aRoll->getTowerName() << "\n" ;

                    }
                }

                OFS_dead.clear();
                OFS_masked.clear();
                OFS_TM.clear();
                OFS_TU.clear();

            }
        }


        delete aRoll;
    }
    OFS_dead.close();
    OFS_TM.close();
    OFS_TU.close();
    OFS_masked.close();
}

void getRollNamesFromSingleFile(string rootFile, string towerName, string output) { // implement also for CMSSW and Construction DB identifiers
    TFile * file = new TFile(rootFile.c_str(),"READ");
    TIter nextkey(file->GetListOfKeys());
    TKey * key;
    TH1F * h1;
    TObject * obj1;
    //map <string,double> temp_roll_rate_map ;
    if (file->IsOpen()) {
        ofstream OFS;
        OFS.open(output.c_str());
        while (key = (TKey*)nextkey()) {
            h1 = (TH1F*)key->ReadObj();
            string * nameOfRoll = new string;
            *nameOfRoll = h1->GetName();
            if (nameOfRoll->substr(0,1) == "W" || nameOfRoll->substr(0,2) == "RE") {
                ExRoll * aRoll = new ExRoll(*nameOfRoll);
                //temp_roll_rate_map[aRoll->getFullOnlineRollID()] = aRoll->getAvgRatePSCWithoutCorrections();
                OFS << aRoll->getFullOnlineRollID() <<" "<< towerName <<"\n";
                delete aRoll;
            }
            delete nameOfRoll,h1;
            OFS.clear();
        }
        OFS.close();

    }
}

map< string, std::map< int, int > > getRollChipsMapFromDataObject(DataObject& lbFebMapFile) {
    map <string,map<int,int> > Roll_ChipMap;
    map <int,int> bufferMap;
    string fullstring;
    for (int i = 1;i <= lbFebMapFile.getLenght() ; i++) {
        fullstring = lbFebMapFile.getElement(i,1)+" "+lbFebMapFile.getElement(i,2);
        bufferMap[lbFebMapFile.getElementAsInt(i,3)] = lbFebMapFile.getElementAsInt(i,4);
        if (i == lbFebMapFile.getLenght() || lbFebMapFile.getElement(i+1,1)+" "+lbFebMapFile.getElement(i+1,2) != fullstring) {
            Roll_ChipMap[fullstring] = bufferMap;

            bufferMap.erase(bufferMap.begin(),bufferMap.end());
            bufferMap.clear();
        }
        //}
    }
    return Roll_ChipMap;
}

map< string, string > getRollNameLBnameMapFromDataObject(DataObject& LBnameRollNameMapFile) {
    map<string,string> LBnameRollNameMap;
    for (int i = 1 ; i <= LBnameRollNameMapFile.getLenght();i++) {
        LBnameRollNameMap[LBnameRollNameMapFile.getElement(i,1)+" "+LBnameRollNameMapFile.getElement(i,2)]
        = LBnameRollNameMapFile.getElement(i,3);
    }
    return LBnameRollNameMap;
}

void writeChipHistosForThresholdScan ( string fileListPar, string pathToFileList, string chipsMapFile, string LBMapFile, string run_threshold_value_file, string outputDestination, string ChipsIDSonlyFile,
				       string disabledChipsFile, string rollNamesFile, string thresholdDumpFile,string fileWithDefaultThresholds,string fileWithMinThresholds, string SaveDetailedHistosValue, string FitFunctionDefinition,
				       string RateMinimumRuleValue, string histogramMaximum,bool UseDefaultTHRValuesFromFile ) {
  
    gStyle->Reset();
    gStyle->SetOptStat(111111);
    gStyle->UseCurrentStyle();
    // clean this mess here, most of this maps are probably not used
    map <string,map<string,TH1F*> > run_RootFiles_map;
    map <string,map<string,TH1F*> >::iterator iter;
    map <string,TH1F*> name_object_map;
    map <int,map<string,double> > Chip_Runnumber_rate_map;
    map <string,bool> ChipsLoopMap;
    map <string,map<int,int> > Roll_ChipMap;
    map <string,string> LBnamesMap;
    map<int,TH2F*> chip_histo_map;
    map<string,int> run_threshold_map;
    map<string,int> run_MIN_TH_DeviationMap;
    map<string,string> disabled_chips;
    map<string,int> ThresholdDumpMap;
    map<string,TH1F*> run_ProblemChipsRateDistribution;
    int BinNum =0;
    //TH1F * ProblemChipsRatesByRun;
    
    
    string lineOfList,histoCurrentName;
    
    ifstream IFS;
    
    map<string,TH1F*>::iterator inner_iter;
    string name,chip_id;
    stringstream SS;
    
    DataObject chipsMap(chipsMapFile,4);
    DataObject LBmap(LBMapFile,3);
    DataObject minFEBthresholds(fileWithMinThresholds);
    DataObject defaultFEBthresholds(fileWithDefaultThresholds);
    
    ofstream OFS,OFS_differences;
    
    Roll_ChipMap = getRollChipsMapFromDataObject(chipsMap);
    LBnamesMap = getRollNameLBnameMapFromDataObject(LBmap);
    
    DataObject run_threshold(run_threshold_value_file);
    
    
    for (int i=1 ; i <= run_threshold.getLenght();i++) {
        run_threshold_map[run_threshold.getElement(i,1)] = run_threshold.getElementAsInt(i,2);
	run_MIN_TH_DeviationMap[run_threshold.getElement(i,1)] = run_threshold.getElementAsInt(i,3);
	run_ProblemChipsRateDistribution[run_threshold.getElement(i,1)] = new TH1F((run_threshold.getElement(i,1)).c_str(),(run_threshold.getElement(i,1)).c_str(),1000,0,1000);
    }
    
    string ChipsThrValsFile = outputDestination+"_OutFileChipThresholds.txt";
    
    // load the chip loop map with all false
    DataObject ChipLoopMapDO(ChipsIDSonlyFile,1);
    for (int i = 1 ; i <= ChipLoopMapDO.getLenght() ; i++) {
        // init all the chips in this map to be with true values , and if appropriate value found - assign to false in the main loop
        ChipsLoopMap[ChipLoopMapDO.getElement(i,1)] = true;
    }

    DataObject disabled_chipsDO(disabledChipsFile,1);
    for (int i = 1 ; i <= disabled_chipsDO.getLenght();i++) {
        disabled_chips[disabled_chipsDO.getElement(i,1)] = disabled_chipsDO.getElement(i,1);
    }
    
    DataObject RollNames(rollNamesFile,1);
    DataObject ThresholdDumpMapDO(thresholdDumpFile,2);
    for (int i=1 ; i <= ThresholdDumpMapDO.getLenght();i++) {
        ThresholdDumpMap[ThresholdDumpMapDO.getElement(i,1)] = ThresholdDumpMapDO.getElementAsInt(i,2);
    }

    // for debug , just print it once
    cout << " Chips map has this size of elements -> " << ChipsLoopMap.size() << endl;

    bool SaveDetailedHistos=atoi(SaveDetailedHistosValue.c_str());
    string definition = FitFunctionDefinition;
    double rateMinimumRule = atof(RateMinimumRuleValue.c_str());
    string histo_max_point = histogramMaximum;

    // end of definitions


    IFS.open(fileListPar.c_str());
    while (getline(IFS,lineOfList)) {
        //cout << lineOfList << endl;


        TFile * aFile = new TFile((pathToFileList+lineOfList+"/total.root").c_str(),"READ");
        TIter iterator(aFile->GetListOfKeys());
        TH1F * theHistoPointer;
        TKey * aKey;
        while (aKey = dynamic_cast<TKey*>(iterator())) {
            theHistoPointer = (TH1F*)aKey->ReadObj();
            histoCurrentName=theHistoPointer->GetName();
            if (histoCurrentName.substr(0,1) == "W" || histoCurrentName.substr(0,2) == "RE" ) {
                name_object_map[histoCurrentName] = dynamic_cast<TH1F*>(theHistoPointer->Clone());
                //cout << histoCurrentName << endl;
            }

        }
        run_RootFiles_map[lineOfList] = name_object_map;
        name_object_map.clear();
        //aFile->Delete();
        //aFile->Close("R");
    }
    
    cout << "Data files closed !" << endl;
    
    // open the output file
    OFS.open(ChipsThrValsFile.c_str());
    OFS_differences.open((outputDestination+"Differences_between_dump_andNewValues.txt").c_str());

    cout << "Break point for seg fault ! (check if the files are closed, does the map is still filled with values) " << endl;
    cout << "Analysis and histograms writing" << endl;

    //cout << "The map is " << run_RootFiles_map.size() << " elements" << endl;

//   /** @brief Cumulative histograms writing - all chips average for single run
    int deviationValue = 0;
    int lessthanH = 0 , morethanH = 0;
    double chip_rate = 0;
    
    for (iter = run_RootFiles_map.begin();iter != run_RootFiles_map.end() ; iter++) {
        TH1F * cumulativeHisto = new TH1F ((iter->first+"_hist").c_str(),iter->first.c_str(),1000,0,atoi(histo_max_point.c_str()));
	TH1F * chipsValuesOne = new TH1F((iter->first+"_chipsOne").c_str(),iter->first.c_str(),100,200,300);
	TH1F * chipsValuesTwo = new TH1F((iter->first+"_chipsTwo").c_str(),iter->first.c_str(),42,199,241);
	
        TCanvas * cumul_canvas = new TCanvas ((iter->first+"_can").c_str(),iter->first.c_str(),1200,700);
	TCanvas * can_chipsOne = new TCanvas ((iter->first+"_can_one").c_str(),iter->first.c_str(),1200,700);
        TCanvas * can_chipsTwo = new TCanvas ((iter->first+"_can_two").c_str(),iter->first.c_str(),1200,700);
	
	deviationValue = run_MIN_TH_DeviationMap.find(iter->first)->second;
	stringstream str_buff;
	string chipsID_as_string;
        for (map<string,TH1F*>::iterator itr = iter->second.begin() ; itr != iter->second.end() ; itr++) {
            string name = itr->first;
            ExRoll * a_Roll = new ExRoll(name);
	    a_Roll->allocStrips();
	    a_Roll->initStrips();
            a_Roll->setStripsRatesFromTH1FObject(itr->second);
            a_Roll->assignChipsFromSource(Roll_ChipMap);
            for (int i=0;i < a_Roll->getNumberOfChipsAssigned();i++) {
	      chip_rate = a_Roll->getChipByNumber(i)->getAverageValueOfStrips();
              cumulativeHisto->Fill(chip_rate);
	      if(chip_rate <= 100){
		lessthanH++;
	      }
	      else{
		morethanH++;
	      }
	      str_buff << a_Roll->getChipByNumber(i)->getChipID();
	      str_buff >> chipsID_as_string;
	      chipsValuesOne->Fill(minFEBthresholds.getElementByKeyAsInt(chipsID_as_string,1)+deviationValue);
	      chipsValuesTwo->Fill(minFEBthresholds.getElementByKeyAsInt(chipsID_as_string,1)+deviationValue);
	      str_buff.clear();
            }
            delete a_Roll;

        }
        
        cout << iter->first << " Less than 100 Hz " << lessthanH << " More than 100 Hz " << morethanH << endl;
	lessthanH = 0 ; morethanH = 0;


        cumulativeHisto->GetYaxis()->SetTitle("Number of entries");
        cumulativeHisto->GetXaxis()->SetTitle("Average rate per chip");
        cumulativeHisto->SetLineColor(kBlue);
        TPaveText * pt = new TPaveText(-2,10000,0,14000);
        cumul_canvas->cd();
        cumulativeHisto->Draw();
        cumul_canvas->SetLogy();
        cumul_canvas->SetLogx();
        cumul_canvas->SaveAs((outputDestination+"Cumulative_histos/"+iter->first+"_"+histo_max_point+"_can.root").c_str());
        cumul_canvas->SaveAs((outputDestination+"Cumulative_histos/"+iter->first+"_"+histo_max_point+"_can.png").c_str());
	// the febs values
	can_chipsOne->cd();
	chipsValuesOne->SetLineColor(kBlue);
	chipsValuesOne->SetFillColor(kBlue);
	chipsValuesOne->GetXaxis()->SetTitle("Threshold values in mV");
	chipsValuesOne->GetYaxis()->SetTitle("Number of entries");
	chipsValuesOne->Draw();
	can_chipsOne->SetLogy();
	can_chipsOne->SaveAs((outputDestination+"Cumulative_histos/"+iter->first+"_chipsOne.root").c_str());
	can_chipsTwo->cd();
	chipsValuesTwo->SetLineColor(kBlue);
	chipsValuesTwo->SetFillColor(kBlue);
	chipsValuesTwo->GetXaxis()->SetTitle("Threshold values in mV");
	chipsValuesTwo->GetYaxis()->SetTitle("Number of entries");
	chipsValuesTwo->Draw();
	can_chipsTwo->SetLogy();
	can_chipsTwo->SaveAs((outputDestination+"Cumulative_histos/"+iter->first+"_chipsTwo.root").c_str());

        delete cumulativeHisto,cumul_canvas;
    }
    
    // writing the chips thresholds for the default values
    
    TH1F * chipsDefOne = new TH1F("","Default threshold values",100,200,300);
    TH1F * chipsDefTwo = new TH1F("","Default threshold values",42,199,241);
    TCanvas * canDef_chipsOne = new TCanvas ("thone","thone",1200,700);
    TCanvas * canDef_chipsTwo = new TCanvas ("thtwo","thtwo",1200,700);
    
    for(int i=1; i <= defaultFEBthresholds.getLenght();i++){
      chipsDefOne->Fill(defaultFEBthresholds.getElementAsInt(i,2));
      chipsDefTwo->Fill(defaultFEBthresholds.getElementAsInt(i,2));
    }
    
    canDef_chipsOne->cd();
    chipsDefOne->SetLineColor(kBlue);
    chipsDefOne->SetFillColor(kBlue);
    chipsDefOne->GetXaxis()->SetTitle("Threshold values in mV");
    chipsDefOne->GetYaxis()->SetTitle("Number of entries");
    chipsDefOne->Draw();
    canDef_chipsOne->SetLogy();
    canDef_chipsOne->SaveAs((outputDestination+"Cumulative_histos/Defaults_chipsOne.root").c_str());
    canDef_chipsTwo->cd();
    chipsDefTwo->SetLineColor(kBlue);
    chipsDefTwo->SetFillColor(kBlue);
    chipsDefTwo->GetXaxis()->SetTitle("Threshold values in mV");
    chipsDefTwo->GetYaxis()->SetTitle("Number of entries");
    chipsDefTwo->Draw();
    canDef_chipsTwo->SetLogy();
    canDef_chipsTwo->SaveAs((outputDestination+"Cumulative_histos/Defaults_chipsTwo.root").c_str());
    
    
    // end of Cumulative histos writing
    
    // terminate here , because overwrite of the histos is not desired
    cout << "terminated point , cumulative histos needed only !" << endl;
    exit(0);
   /** @brief end of cumulative histograms */
   


    TH1F * Differences_cumulative_histo = new TH1F("Diff_histo","Diff_cumul_histo",5,-2,2);
    TH1F * Differences_cumulative_histo_all = new TH1F("Diff_histo_all","Diff_cumul_histo_all",210,-100,110);
    TCanvas * diff_cumul_hist_can = new TCanvas("__","__",1200,700);
    TCanvas * diff_cumul_hist_can_all = new TCanvas("___","___",1200,700);
    int plus=0,minus=0,zero=0;
    
   /** @brief The REAL mess starts here :) ENJOY :D */
   
   
    TPaveText * pt = new TPaveText(-2,10000,0,14000);    
    TFile * Cumulative_Root_File = new TFile((outputDestination+"ChipsRootHistos.root").c_str(),"NEW");
    if(Cumulative_Root_File->IsOpen()){cout << "the result file for histos has been open!" << endl;};
    
    int counter_chips = 1;
    
    
    for (int i=1;i <= RollNames.getLenght();i++) {
        name = RollNames.getElement(i,1);
        ExRoll * aRoll = new ExRoll(name);
	aRoll->allocStrips();
	aRoll->initStrips();
        aRoll->assignChipsFromSource(Roll_ChipMap);
        aRoll->setLBnameFromMap(LBnamesMap);
        Chip * aChip;
	
        //for(iter = run_RootFiles_map.begin();iter != run_RootFiles_map.end() ; iter++){
        //aRoll->setStripsRatesFromTH1FObject(iter->second.find(aRoll->getFullOnlineRollID())->second);

        for (int i=0;i < aRoll->getNumberOfChipsAssigned();i++) {
            aChip = aRoll->getChipByNumber(i);
            SS << aChip->getChipID();
            SS >> chip_id;
            SS.clear();
            double appropriateThrValue = 4000 ;

            double max_chip_average_value_in_runs=0;
            double max_chip_strips_mean=0;
            for (iter = run_RootFiles_map.begin();iter != run_RootFiles_map.end() ; iter++) {
                aRoll->setStripsRatesFromTH1FObject(iter->second.find(aRoll->getFullOnlineRollID())->second);
                if (aChip->getAverageValueOfStrips() > max_chip_average_value_in_runs) {
                    max_chip_average_value_in_runs = aChip->getAverageValueOfStrips();
                }
                if (aChip->getMeanValueOfStrips() > max_chip_strips_mean) {
                    max_chip_strips_mean = aChip->getMeanValueOfStrips();
                }
                //hist->Fill(run_threshold_map.find(iter->first)->second,aChip->getAverageValueOfStrips(),1);
            }
            
            //TH2F * hist = new TH2F((chip_id+"_average").c_str(),chip_id.c_str(),100,190,310,100,0,max_chip_average_value_in_runs+10);
            //TH2F * hist2 = new TH2F((chip_id+"_mean").c_str(),chip_id.c_str(),100,190,310,100,0,max_chip_strips_mean+10);
            
            TH1F * distribution;
            TH1F * stripRates;
            TH1F * hist = new TH1F((chip_id+"_average").c_str(),chip_id.c_str(),120,190,310);//100,0,max_chip_average_value_in_runs+10);
            TH1F * hist2 = new TH1F((chip_id+"_mean").c_str(),chip_id.c_str(),120,190,310);//,100,0,max_chip_strips_mean+10);
            
            bool ValueStillHigherThanThreshold = false;
	    bool firstUsefulValueNotFound = true;
	    bool secondUsefulValueNotFound = true;
	    string ThresholdValueFlag = "";
	    
            for (iter = run_RootFiles_map.begin();iter != run_RootFiles_map.end() ; iter++) {
		int theDeviation = run_MIN_TH_DeviationMap.find(iter->first)->second;
		
                aRoll->setStripsRatesFromTH1FObject(iter->second.find(aRoll->getFullOnlineRollID())->second);
		
		// in case the old algorithm to be used again, encapsualte this two if's 
		
		if (rateMinimumRule + 100 >= aChip->getAverageValueOfStrips() && secondUsefulValueNotFound ){
		  appropriateThrValue = minFEBthresholds.getElementByKeyAsInt(chip_id,1) + theDeviation;
		  secondUsefulValueNotFound = false;
		  ThresholdValueFlag = "ST";
		}
		
                if (rateMinimumRule  >= aChip->getAverageValueOfStrips() && firstUsefulValueNotFound ) {
		  appropriateThrValue = minFEBthresholds.getElementByKeyAsInt(chip_id,1) + theDeviation;
		  firstUsefulValueNotFound = false;
		  ThresholdValueFlag = "FT";
		  //appropriateThrValue = run_threshold_map.find(iter->first)->second;
		  //cout << appropriateThrValue << endl;
                }
                
                if (aChip->getAverageValueOfStrips() > rateMinimumRule){
		  ValueStillHigherThanThreshold = true;
		}
		
		else{
		  ValueStillHigherThanThreshold = false;
		}
		
                run_ProblemChipsRateDistribution.find(iter->first)->second->Fill(aChip->getAverageValueOfStrips());
		
		if(UseDefaultTHRValuesFromFile){
		  
		  BinNum = minFEBthresholds.getElementByKeyAsInt(chip_id,1)+theDeviation;
		  hist->SetBinContent(BinNum-190,aChip->getAverageValueOfStrips());
		  hist->SetBinError(BinNum-190,sqrt(aChip->getMeanValueOfStrips()));
		  //cout << chip_id << " " << BinNum << " " << aChip->getAverageValueOfStrips() << endl;
		  
		}
		
		else{
                hist->SetBinContent(run_threshold_map.find(iter->first)->second-190,aChip->getAverageValueOfStrips());
                hist2->SetBinContent(run_threshold_map.find(iter->first)->second-190,aChip->getMeanValueOfStrips());
                hist->SetBinError(run_threshold_map.find(iter->first)->second-190,sqrt(aChip->getAverageValueOfStrips()));
                hist2->SetBinError(run_threshold_map.find(iter->first)->second-190,sqrt(aChip->getMeanValueOfStrips()));
		}
		
		BinNum = 0;
		
                if (SaveDetailedHistos){
		  
		  distribution = aChip->getDistributionOfChipsStripRateValues(iter->first);
                    stripRates = aChip->getHistoOfChipStrips(iter->first);
                    distribution->SaveAs((outputDestination+"distributions_all/"+chip_id+"_"+iter->first+"_distribution.root").c_str());
                    stripRates->SaveAs((outputDestination+"strips_rates_all/"+chip_id+"_"+iter->first+"_stripRates.root").c_str());
                    distribution->SaveAs((outputDestination+"pics/"+chip_id+"_"+iter->first+"_distribution.jpg").c_str());
                    stripRates->SaveAs((outputDestination+"pics/"+chip_id+"_"+iter->first+"_stripRates.jpg").c_str());
                    // dispose !
                    delete stripRates,distribution;
		    
                }
                
            }
            // if the chip doesn't has single point below the Rate threshold of 100, loop again to find if its below 200 for at least one point
           
	    if (appropriateThrValue == 4000 ){
	      appropriateThrValue = defaultFEBthresholds.getElementByKeyAsInt(chip_id,1);
	      ThresholdValueFlag = "DT";
	    }
	   
	    
            // if the chip_id doesnt exist in the disabled_chips map write it in the file
            
            if (disabled_chips.find(chip_id) == disabled_chips.end()) {
                OFS << chip_id << " " << appropriateThrValue << " "<< aRoll->getLBstring() <<" "<< aRoll->getFullOnlineRollID() << " " << ThresholdValueFlag <<"\n";
		
		
                if (appropriateThrValue != 4000) {
                    OFS_differences << chip_id << " " << ThresholdDumpMap.find(chip_id)->second - appropriateThrValue << " " << ThresholdDumpMap.find(chip_id)->second << " "<< appropriateThrValue <<"\n";

                    if ((ThresholdDumpMap.find(chip_id)->second - appropriateThrValue) > 0) {
                        Differences_cumulative_histo->Fill(1);
                        plus++;
                    }
                    if ((ThresholdDumpMap.find(chip_id)->second - appropriateThrValue) < 0) {
                        Differences_cumulative_histo->Fill(-1);
                        minus++;
                    }
                    if ((ThresholdDumpMap.find(chip_id)->second - appropriateThrValue) == 0) {
                        Differences_cumulative_histo->Fill(0);
                        zero++;
                    }
                    Differences_cumulative_histo_all->Fill(ThresholdDumpMap.find(chip_id)->second - appropriateThrValue);
                }
            }
            
            OFS_differences.clear();
            OFS.clear();
	    
            //TF1 * aFunc;
            definition = "exp([0]*pow(x,[1]))";
            hist->SetMarkerStyle(kCircle);
	    hist->SetMarkerColor(kBlack);
            //TF1 * aFunc = new TF1("f1",definition.c_str(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
            //hist->Fit(aFunc);     //,"","",190,250);
	    
	    if(ValueStillHigherThanThreshold ){ // write only the bad chips 
	      hist->GetXaxis()->SetTitle("Threshold value in mV");
	      hist->GetYaxis()->SetTitle("Average chip rate in Hz");
	      hist->Write(hist->GetName());
	      cout << "Chip " << hist->GetName() << " written !" << counter_chips << endl;
	    }
	    
	    counter_chips++;
	    delete hist;
	    hist = NULL;

        }

        delete aRoll;
    }
    
    Cumulative_Root_File->Close("R");
    
    
    //
    for(map<string,TH1F*>::iterator itr = run_ProblemChipsRateDistribution.begin(); itr != run_ProblemChipsRateDistribution.end();itr++){
      
      string theCanName = itr->second->GetName(),theHistName = itr->second->GetName();
      theCanName += "_can" ; theHistName += "_hist";
      
      TCanvas * aCan__ = new TCanvas(theCanName.c_str(),theCanName.c_str(),1200,700);
      aCan__->cd();
      itr->second->Draw();
      //aCan__->SaveAs((outputDestination+theCanName+"__.root").c_str());
      
    }
    
    diff_cumul_hist_can->cd();
    Differences_cumulative_histo->Draw();
    stringstream S_S;
    string buf__;
    S_S << plus;
    S_S >> buf__;
    S_S.clear();
    buf__ = "Difference positive "+buf__;
    pt->AddText(buf__.c_str());
    buf__ = "";
    S_S << minus;
    S_S >> buf__;
    S_S.clear();
    buf__ = "Difference negative "+buf__;
    pt->AddText(buf__.c_str());
    buf__ = "";
    S_S << zero;
    S_S >> buf__;
    buf__ = "Difference zero "+buf__;
    pt->AddText(buf__.c_str());
    pt->Draw();

    //diff_cumul_hist_can->SaveAs((outputDestination+"Differences_cumulative_histo.root").c_str());
    diff_cumul_hist_can_all->cd();
    Differences_cumulative_histo_all->Draw();
    //diff_cumul_hist_can_all->SaveAs((outputDestination+"Differences_cumulative_histo_allValues.root").c_str());

    OFS.close();
    OFS_differences.close();

}



/** @brief experimental functions */



void RootFilesLoader(string pathTofiles ,string prefix ,string filelist) {
    map<string,map <string,TH1F*> > mapOfTH1FObjects;
    map<string,TH1F*> bufferMap;
    string LINE,fullPathToFile,nameOfRoll;
    ifstream IFS;
    IFS.open(filelist.c_str());
    TFile * file;
    TKey * TheKey;
    TObject *obj;
    TH1F * h1;
    while (getline(IFS,LINE)) {
        fullPathToFile = pathTofiles+prefix+LINE+".root";

        file = new TFile(fullPathToFile.c_str(),"READ");
        TIter nextkey( file->GetListOfKeys() );
        if (file->IsOpen()) {
            while (TheKey = (TKey*)nextkey()) {
                //h1 = new TH1F();
                h1 = (TH1F*)TheKey->ReadObj();
                nameOfRoll = h1->GetName();
                if (nameOfRoll.substr(0,1) == "W" || nameOfRoll.substr(0,2) == "RE") {
                    //bufferMap[nameOfRoll] = new TH1F();
                    bufferMap[nameOfRoll] = h1;
                }
            }
            mapOfTH1FObjects[LINE] = bufferMap;
            bufferMap.clear();
            cout << LINE << endl;
            IFS.clear();
            //file->Close("R");
            //file->Delete();
        }
    }
    IFS.close();

    TH1F * aPointer = mapOfTH1FObjects.find("179563")->second.find("W-1_RB1in_7B")->second;
    cout << "its fine until here 1" << endl;
    cout << aPointer->GetBinContent(5) << endl;


    cout << "its fine until here 2" << endl;

    for (int i = 1; i < 97;i++) {

        cout << i << " "<< aPointer->GetBinContent(i) << endl;
    }

    cout << aPointer->GetBinContent(1) << endl;


    cout << "End !" << endl;

}

void correctionCheck(string RollName, string AreaFile, string RootFileRateSource, int percentForCorrection) {

    double averageRateBeforeCorrection =0,averageRateAfterCorrection=0;

    DataObject area(AreaFile,2);
    ExRoll * aRoll = new ExRoll(RollName);
    aRoll->setStripsRateFromRootFileSource(RootFileRateSource);
    aRoll->setStripsAreaFromSource_cmsswResource(area);
    TH1F * normalHistoOfStripsRates = new TH1F(aRoll->getFullOnlineRollID().c_str(),aRoll->getFullOnlineRollID().c_str(),96,0,96);
    TH1F * correctedHistoOfStripRates = new TH1F((aRoll->getFullOnlineRollID()+"corrected").c_str(),(aRoll->getFullOnlineRollID()+"_corrected").c_str(),96,0,96);
    TCanvas * normal = new TCanvas((aRoll->getFullOnlineRollID()+"_comparison").c_str(),aRoll->getFullOnlineRollID().c_str(),1200,700);
    //TCanvas * corrected = new TCanvas("corrected","corrected",1200,700);
    for (int i=1;i <= 96 ; i++) {
        normalHistoOfStripsRates->SetBinContent(i,aRoll->getStrip(i)->getRate());
    }

    averageRateBeforeCorrection = aRoll->getAvgRatePSCWithoutCorrections();

    aRoll->removeNoisyStripsForAllClonesWithPercentValue(percentForCorrection); // 100 works well but for given pattern , maybe it has to recognize given patterns and then to decide the percent
    aRoll->removeNoisyStripsForAllClonesWithPercentValue(percentForCorrection); // here the double correction helped
    for (int i=1;i <= 96 ; i++) {
        correctedHistoOfStripRates->SetBinContent(i,aRoll->getStrip(i)->getRate());
    }

    averageRateAfterCorrection = aRoll->getAvgRatePSCWithoutCorrections();


    if (averageRateBeforeCorrection != averageRateAfterCorrection) { // in case that the rate has been corrected

        normal->Divide(2,1);
        normal->cd(1);
        normalHistoOfStripsRates->Draw();
        //normal->SaveAs((aRoll->getFullOnlineRollID()+"_normal.root").c_str());
        //normal->SaveAs((aRoll->getFullOnlineRollID()+"_normal.png").c_str());
        //corrected->cd();
        normal->cd(2);
        correctedHistoOfStripRates->Draw();
        //corrected->SaveAs((aRoll->getFullOnlineRollID()+"_corrected.root").c_str());
        //corrected->SaveAs((aRoll->getFullOnlineRollID()+"_corrected.png").c_str());
        normal->SaveAs((aRoll->getFullOnlineRollID()+"_both_corrected_uncorrected.png").c_str());

    }

    else {
        normal->cd();
        normalHistoOfStripsRates->Draw();
        normal->SaveAs((aRoll->getFullOnlineRollID()+"_normal_only.png").c_str());
    }

    delete normalHistoOfStripsRates,correctedHistoOfStripRates;


}

void getHistogramOfExtrapolatedRollIntrinsicRateValues(string fileWithIntrinsicRates, string histogramName) {
    DataObject * intrinsicFile = new DataObject(fileWithIntrinsicRates);
    double maximum=0;
    for (int i = 1; i <= intrinsicFile->getLenght();i++) {
        if (intrinsicFile->getElementAsDouble(i,2) > maximum) {
            maximum = intrinsicFile->getElementAsDouble(i,2);
        }
    }

    TH1F * histo = new TH1F("Distribution",histogramName.c_str(),1000,-1,int(maximum)+1);

    for (int i = 1; i <= intrinsicFile->getLenght();i++) {
        histo->Fill(intrinsicFile->getElementAsDouble(i,2));
    }

    string name=histogramName+".root";

    TCanvas * aCan = new TCanvas("can","Distribution",1200,700);
    histo->SetLineColor(kBlue);
    aCan->cd();
    histo->Draw();
    aCan->SaveAs(name.c_str());

}

void draw2DbackgroundPlot(string folder,string areaFile ,string shortBarrelIDS,string shortEcapIDS,string runList,string ListOfRolls,bool correctionToBeApplied,int correctionInPercents,int BarrelMax,int EcapMax, string CenterOfMassEmergy) {

//   TFile * rootFile = new TFile(fileName.c_str(),"READ","in");
    string currentName,currentFile,singleRollName,lastRunNum;
    TKey * TheKey;
    TObject *obj;
    TH1F * h1;
    DataObject area(areaFile);
    DataObject shortBarrel(shortBarrelIDS);
    DataObject shortEcap(shortEcapIDS);
    DataObject listOfRootFiles(runList);
    DataObject ListRolls(ListOfRolls);
    ExRoll * aRoll;
    int divider = 1;//2;
    
    
    map<string,int> RollName_incrementalCount;
    map<string,double> RollName_RateSum;
    map<string,string> RollNamesOnly;
    //--------------------------------//
    map<string,int> ecap_labels_map;
    map<string,int>barrel_labels_map;
    map<string,TH2F*> allHistograms;
    map<string,ExRoll*> AllRolls;
    
    // Piet definitions
    
    TH1F * wheel_contour = new TH1F("wheel_contour", "", 12, 0.5, 12.5); 
    for (int i=1; i<13; ++i) { 
      if (i!=4 && i!=9 && i!=11) wheel_contour->Fill(i,17);  
      if (i==4)                  wheel_contour->Fill(i,21);
      if (i==9 || i==11)         wheel_contour->Fill(i,15);
    }
    wheel_contour->SetLineColor(kBlack);
    wheel_contour->SetLineWidth(2);

    
     /*
     * 
    */
    
//     cout << "marks" << endl;
    
    TH1F * WP2_histo_masked = new TH1F( "WP2_histo_masked", "WP2_histo_masked", 12, 0.5,12.5);
    TH1F * WP1_histo_masked_hist2 = new TH1F( "WP1_histo_masked_", "WP2_histo_masked", 12, 0.5,12.5);
    TH1F * WP1_histo_masked_hist3 = new TH1F( "WP1_histo_masked__", "WP2_histo_masked", 12, 0.5,12.5);
    //TH1F * WP1_histo_masked_hist3 = new TH1F( "WP1_histo_masked_3", "WP2_histo_masked", 12, 0.5,12.5);
    TH1F * WP1_histo_masked = new TH1F( "WP1_histo_masked", "WP1_histo_masked", 12, 0.5,12.5);
    TH1F * WP0_histo_masked = new TH1F( "WP0_histo_masked", "WP0_histo_masked", 12, 0.5,12.5);
    TH1F * WP0_his_2_masked = new TH1F( "WP0_his_2_masked", "WP0_his_2_masked", 12, 0.5,12.5);
    TH1F * WM1_histo_masked = new TH1F( "WM1_histo_masked", "WM1_histo_masked", 12, 0.5,12.5);
    TH1F * WM1_his_2_masked = new TH1F( "WM1_his_2_masked", "WM1_his_2_masked", 12, 0.5,12.5);
    TH1F * WM2_histo_masked = new TH1F( "WM2_histo_masked", "WM2_histo_masked", 12, 0.5,12.5);
    TH1F * WM2_histo_masked_h2 = new TH1F( "WM2_histo_masked_h2", "WM2_histo_masked", 12, 0.5,12.5);
    
    TH1F * EN3_histo_masked = new TH1F( "EN3_histo_masked", "EN3_histo_masked", 36, 0.5,36.5);
    TH1F * EN2_histo_masked = new TH1F( "EN2_histo_masked", "EN2_histo_masked", 36, 0.5,36.5);
    TH1F * EN1_histo_masked = new TH1F( "EN1_histo_masked", "EN1_histo_masked", 36, 0.5,36.5);
    TH1F * EP1_histo_masked = new TH1F( "EP1_histo_masked", "EP1_histo_masked", 36, 0.5,36.5);
    TH1F * EP2_histo_masked = new TH1F( "EP2_histo_masked", "EP2_histo_masked", 36, 0.5,36.5);
    TH1F * EP3_histo_masked = new TH1F( "EP3_histo_masked", "EP3_histo_masked", 36, 0.5,36.5);
    TH1F * EP4_histo_masked = new TH1F( "EP4_histo_masked", "EP3_histo_masked", 36, 0.5,36.5);
    TH1F * EN4_histo_masked = new TH1F( "EN4_histo_masked", "EP3_histo_masked", 36, 0.5,36.5);
    TH1F * EP1_histo_masked_h2 = new TH1F( "EP1_histo_masked_h2", "EP1_histo_masked", 36, 0.5,36.5);
    //WP2_histo_masked->SetBinContent(8,);  WP2_histo_masked->SetBinError(8,0.5);  WP2_histo_masked->SetFillColor(kGray+1);
    
    // W=2/S01/RB4-/Bwd & W=2/S01/RB4-/Fwd
    // WP2_histo_sgmode->SetBinContent(1,14.5); WP2_histo_sgmode->SetBinError(1,1.0); WP2_histo_sgmode->SetFillColor(kGray+1+3); WP2_histo_sgmode->SetFillStyle(3004);
    
    
    WM2_histo_masked->SetBinContent(5,4.5);    WM2_histo_masked->SetBinError(5,0.5);  WM2_histo_masked->SetFillColor(kGray);
    WM2_histo_masked->SetBinContent(12,3); WM2_histo_masked->SetBinError(12,1.0); WM2_histo_masked->SetFillColor(kGray);
    WM2_histo_masked_h2->SetBinContent(5,16); WM2_histo_masked_h2->SetBinError(5,1.0); WM2_histo_masked_h2->SetFillColor(kGray);
    
    WM1_histo_masked->SetBinContent(4,2.5);  WM1_histo_masked->SetBinError(4,0.5);  WM1_histo_masked->SetFillColor(kGray);
    //WM1_his_2_masked->SetBinContent(4,6.5);    WM1_his_2_masked->SetBinError(4,0.5);  WM1_his_2_masked->SetFillColor(kGray);
    
    WM1_histo_masked->SetBinContent(7,7.5);  WM1_histo_masked->SetBinError(7,0.5); WM1_histo_masked->SetFillColor(kGray);
    WM1_histo_masked->SetBinContent(8,16);WM1_histo_masked->SetBinError(8,1); WM1_histo_masked->SetFillColor(kGray);
    //WM1_histo_masked->SetBinContent(10,);WM1_histo_masked->SetBinError(10,); WM1_histo_masked->SetFillColor(kGray);
    
    WP0_histo_masked->SetBinContent(7,14.5);  WP0_histo_masked->SetBinError(7,0.5);  WP0_histo_masked->SetFillColor(kGray);
    
    WP1_histo_masked->SetBinContent(4,20);  WP1_histo_masked->SetBinError(4,1);  WP1_histo_masked->SetFillColor(kGray);
    WP1_histo_masked_hist3->SetBinContent(4,14);  WP1_histo_masked_hist3->SetBinError(4,1);  WP1_histo_masked_hist3->SetFillColor(kGray);
    WP1_histo_masked_hist2->SetBinContent(4,2.5);  WP1_histo_masked_hist2->SetBinError(4,0.5);  WP1_histo_masked_hist2->SetFillColor(kGray);
    //WP1_histo_masked_hist3->SetBinContent(4,2.5);  WP1_histo_masked_hist3->SetBinError(4,0.5);  WP1_histo_masked_hist3->SetFillColor(kGray);

    WP2_histo_masked->SetBinContent(8,7.5);  WP2_histo_masked->SetBinError(8,0.5);  WP2_histo_masked->SetFillColor(kGray);
    
    //EN1_histo_masked->SetBinContent(4,4.5);    EN1_histo_masked->SetBinError(4,1.5);  EN1_histo_masked->SetFillColor(kGray);
    EN1_histo_masked->SetBinContent(10,5.5);   EN1_histo_masked->SetBinError(10,0.5); EN1_histo_masked->SetFillColor(kGray);    
    
    EP1_histo_masked->SetBinContent(25,4);   EP1_histo_masked->SetBinError(25,2); EP1_histo_masked->SetFillColor(kGray);
    //EP1_histo_masked_h2->SetBinContent(25,4.5);   EP1_histo_masked_h2->SetBinError(25,1.5); EP1_histo_masked_h2->SetFillColor(kGray);
    EP1_histo_masked->SetBinContent(29,1.5);   EP1_histo_masked->SetBinError(29,0.5); EP1_histo_masked->SetFillColor(kGray);
    EP1_histo_masked->SetBinContent(35,1.5);   EP1_histo_masked->SetBinError(35,1.5); EP1_histo_masked->SetFillColor(kGray);    
    EP1_histo_masked->SetBinContent(36,1.5); EP1_histo_masked->SetBinError(36,1.5); EP1_histo_masked->SetFillColor(kGray);

    //EP1_histo_masked->SetBinContent(29,1.5);   EP1_histo_masked->SetBinError(29,0.5); EP1_histo_masked->SetFillColor(kGray);       
    
    
    EN3_histo_masked->SetBinContent(35,3);   EN3_histo_masked->SetBinError(35,3); EN3_histo_masked->SetFillColor(kGray);      
    //EN1_histo_masked->SetBinContent(32,5); EN1_histo_masked->SetBinError(32,1.0); EN1_histo_masked->SetFillColor(kGray);
    
    EP2_histo_masked->SetBinContent(29,5.5);   EP2_histo_masked->SetBinError(29,0.5); EP2_histo_masked->SetFillColor(kGray);    

    
    /*
     * End of efficiency taken masks
     * 
     */
    
    /*
     * Make a map between Histo object name and Piet histogram masks
     * 
     */
//     cout << "marks2" << endl;
    map <string,TH1F*> mapOfMasks ;
    //  <partitionAsString,maskHistogram>
    
    mapOfMasks["W-1"]= WM1_histo_masked;
    mapOfMasks["W-2"]= WM2_histo_masked;
    mapOfMasks["W0"]= WP0_histo_masked;
    mapOfMasks["W1"]= WP1_histo_masked;
    mapOfMasks["W2"]= WP2_histo_masked;
    mapOfMasks["RE3"] = EP3_histo_masked;
    mapOfMasks["RE2"] = EP2_histo_masked;
    mapOfMasks["RE1"] = EP1_histo_masked;
    mapOfMasks["RE-3"] = EN3_histo_masked;
    mapOfMasks["RE-2"] = EN2_histo_masked;
    mapOfMasks["RE-1"] = EN1_histo_masked;
    mapOfMasks["RE-4"] = EN4_histo_masked;
    mapOfMasks["RE4"] = EP4_histo_masked;
    
    // end of map
    
    // Map of masked because of high rates
    
    map <string,TH1F*> mapOfMasks_extremenoise;
    TH1F * WM1_histo_masked_en = new TH1F( "WM1_histo_masked_en", "WM1_histo_masked", 12, 0.5,12.5);
    TH1F * WP1_histo_masked_en = new TH1F( "WP1_histo_masked_en", "WP1_histo_masked", 12, 0.5,12.5);
    TH1F * WM2_histo_masked_en = new TH1F( "WM2_histo_masked_en", "WM1_histo_masked", 12, 0.5,12.5);
    TH1F * WP2_histo_masked_en = new TH1F( "WP2_histo_masked_en", "WM1_histo_masked", 12, 0.5,12.5);
    TH1F * EN2_histo_masked_en = new TH1F( "EN2_histo_masked_en", "EN2_histo_masked", 36, 0.5,36.5);
    TH1F * EN1_histo_masked_en = new TH1F( "EN1_histo_masked_en", "EN2_histo_masked", 36, 0.5,36.5);
    TH1F * EP2_histo_masked_en = new TH1F( "EP2_histo_masked_en", "EN2_histo_masked", 36, 0.5,36.5);
    
    WP1_histo_masked_en->SetFillColor(kBlack);
    //WP1_histo_masked->SetBinContent(1,6); WP1_histo_masked_en->SetBinError(1,0.5);// WP1_histo_masked_en->SetFillColor(kBlack);
    
    /*
    WM1_histo_masked_en->SetBinContent(6,14);  WM1_histo_masked_en->SetBinError(6,1.0);  WM1_histo_masked_en->SetFillColor(kBlack);
    //WM2_histo_masked_en->SetBinContent(3,13.5);  WM2_histo_masked_en->SetBinError(3,0.5);  WM2_histo_masked_en->SetFillColor(kBlack);
    //WP1_histo_masked->SetBinContent(1,5); WP1_histo_masked_en->SetBinError(1,0.5);// WP1_histo_masked_en->SetFillColor(kBlack);
    WP1_histo_masked_en->SetBinContent(4,4.5); WP1_histo_masked_en->SetBinError(4,0.5);// WP1_histo_masked_en->SetFillColor(kBlack);
    WP1_histo_masked_en->SetBinContent(6,8);WP1_histo_masked_en->SetBinError(6,1); //WP1_histo_masked_en->SetFillColor(kBlack);
    WP2_histo_masked_en->SetBinContent(12,14);  WP2_histo_masked_en->SetBinError(12,1.0);  WP2_histo_masked_en->SetFillColor(kBlack);
    EN2_histo_masked_en->SetBinContent(6,1.5); EN2_histo_masked_en->SetBinError(6,0.5); EN2_histo_masked_en->SetFillColor(kBlack);
    EN2_histo_masked_en->SetBinContent(29,0.5); EN2_histo_masked_en->SetBinError(29,0.5); EN2_histo_masked_en->SetFillColor(kBlack);
    EN1_histo_masked_en->SetBinContent(16,5.5); EN1_histo_masked_en->SetBinError(16,0.5); EN1_histo_masked_en->SetFillColor(kBlack);
    EP2_histo_masked_en->SetBinContent(19,4.5) ; EP2_histo_masked_en->SetBinError(19,0.5); EP2_histo_masked_en->SetFillColor(kBlack);
    */
    
    mapOfMasks_extremenoise["W-1"] = WM1_histo_masked_en;
    mapOfMasks_extremenoise["W1"] = WP1_histo_masked_en;
    //mapOfMasks_extremenoise["W-2"] = WM2_histo_masked_en;
    mapOfMasks_extremenoise["W2"] = WP2_histo_masked_en;
    mapOfMasks_extremenoise["RE-2"] = EN2_histo_masked_en;
    mapOfMasks_extremenoise["RE-1"] = EN1_histo_masked_en;
    mapOfMasks_extremenoise["RE2"] = EP2_histo_masked_en;
    
    // end of map    
    // Map of canvases
    
    TCanvas * WP2_canvas = new TCanvas( "WP2_canvas", "WP2_canvas",800,600);
    TCanvas * WP1_canvas = new TCanvas( "WP1_canvas", "WP1_canvas",800,600);
    TCanvas * WP0_canvas = new TCanvas( "WP0_canvas", "WP0_canvas",800,600);
    TCanvas * WM1_canvas = new TCanvas( "WM1_canvas", "WM1_canvas",800,600);
    TCanvas * WM2_canvas = new TCanvas( "WM2_canvas", "WM2_canvas",800,600);
    TCanvas * EN3_canvas = new TCanvas( "EN3_canvas", "EN3_canvas", 800,600);
    TCanvas * EN2_canvas = new TCanvas( "EN2_canvas", "EN2_canvas", 800,600);
    TCanvas * EN1_canvas = new TCanvas( "EN1_canvas", "EN1_canvas", 800,600);
    TCanvas * EP1_canvas = new TCanvas( "EP1_canvas", "EP1_canvas", 800,600);
    TCanvas * EP2_canvas = new TCanvas( "EP2_canvas", "EP2_canvas", 800,600);
    TCanvas * EP3_canvas = new TCanvas( "EP3_canvas", "EP3_canvas", 800,600);
    TCanvas * EP4_canvas = new TCanvas( "EP4_canvas", "EP3_canvas", 800,600);
    TCanvas * EN4_canvas = new TCanvas( "EN4_canvas", "EP3_canvas", 800,600);
    
    map <string,TCanvas*> mapCanvases;
//     cout << "marks3" << endl;
    mapCanvases["W-1"]= WM1_canvas;
    mapCanvases["W-2"]= WM2_canvas;
    mapCanvases["W0"]= WP0_canvas;
    mapCanvases["W1"]= WP1_canvas;
    mapCanvases["W2"]= WP2_canvas;
    mapCanvases["RE4"] = EP4_canvas;
    mapCanvases["RE3"] = EP3_canvas;
    mapCanvases["RE2"] = EP2_canvas;
    mapCanvases["RE1"] = EP1_canvas;
    mapCanvases["RE-4"] = EN4_canvas;
    mapCanvases["RE-3"] = EN3_canvas;
    mapCanvases["RE-2"] = EN2_canvas;
    mapCanvases["RE-1"] = EN1_canvas;
    
    //
    //cout << "marks4" << endl;
    for (int i=1;i<=ListRolls.getLenght();i++) {
        singleRollName = ListRolls.getElement(i,1);
// 	cout << "lol" << endl;
	//cout << singleRollName << endl;
        ExRoll * a_roll = new ExRoll(singleRollName);
	a_roll->allocStrips();
	a_roll->initStrips();
        a_roll->setStripsAreaFromSource_cmsswResource(area);
	
	//cout << a_roll->getRollIDofClone(1) << endl;
	//cout <<  a_roll->getFullOnlineRollID() << endl;;
        AllRolls[singleRollName] = a_roll;
    }
    //cout << " 1 " << endl;
    // its a map of all rolls as objects , and then only their strip rate is changed in the loop over the runs ;)
    
    for (int k = 1; k <= listOfRootFiles.getLenght() ; k++) {
      
        currentFile = listOfRootFiles.getElement(k,1);
	
        //cout << currentFile << endl;
	
        currentFile = folder+"total_"+currentFile+".root";
	
        TFile * rootFile = new TFile(currentFile.c_str(),"READ","in");
	
	//cout << " file is open " << endl;
	
        if (rootFile->IsOpen()) {
            TIter nextkey( rootFile->GetListOfKeys());
            while (TheKey = (TKey*)nextkey()) {
                h1 = (TH1F*)TheKey->ReadObj();
                currentName = h1->GetName();
                if ( currentName.substr(0,1) == "W" || currentName.substr(0,2) == "RE" ) {
                    currentName = h1->GetName();
		    //cout << h1->GetName() << endl;
		    
                    aRoll = AllRolls.find(currentName)->second;
		    //cout << aRoll->getFullOnlineRollID() << endl;
                    aRoll->setStripsRatesFromTH1FObject(h1);
                    RollNamesOnly[currentName] = currentName;
		    
                    for (int i=1;i <= aRoll->getClones();i++) {
			//cout << aRoll->getFullOnlineRollID() << endl;

                        if (aRoll->getAvgRatePSCWithoutCorrectionsForClone(i) < 200) {

                            if (RollName_incrementalCount.find(aRoll->getRollIDofCloneWithNewIdentifiers(i)) == RollName_incrementalCount.end() ) {
                                RollName_incrementalCount[aRoll->getRollIDofCloneWithNewIdentifiers(i)] = 1;
                            }
                            else {
                                RollName_incrementalCount.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second += 1;
                            }
                            
                            if(correctionToBeApplied) aRoll->removeNoisyStripsForCloneWithPercentValue(i,correctionInPercents);

                            if (RollName_RateSum.find(aRoll->getRollIDofCloneWithNewIdentifiers(i)) == RollName_RateSum.end()) {
                                RollName_RateSum[aRoll->getRollIDofCloneWithNewIdentifiers(i)] = aRoll->getAvgRatePSCWithoutCorrectionsForClone(i);
                            }	
                            else {
                                RollName_RateSum.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second += aRoll->getAvgRatePSCWithoutCorrectionsForClone(i);
                            }

                        }
                        //cout << aRoll->getFullOnlineRollID() << endl;
                    }
                    
                }
                delete h1;
            }
        }

        rootFile->Close("R");
        rootFile->Delete();

    }
    
    //cout << "is out" << endl;
    
    TH2F * WM1histo = new TH2F("WM1","W-1",12,0.5,12.5,21,0,21);
    TH2F * WM2histo = new TH2F("WM2","W-2",12,0.5,12.5,21,0,21);
    TH2F * W0histo = new TH2F("WZ","W0",12,0.5,12.5,21,0,21);
    TH2F * WP1histo = new TH2F("WP1","W+1",12,0.5,12.5,21,0,21);
    TH2F * WP2histo = new TH2F("WP2","W+2",12,0.5,12.5,21,0,21);
    TH2F * DP3Endcap = new TH2F("EP3","Disk+3",36,0.5,36.5,6,0,6);
    TH2F * DP2Endcap = new TH2F("EP2","Disk+2",36,0.5,36.5,6,0,6);
    TH2F * DP1Endcap = new TH2F("EP1","Disk+1",36,0.5,36.5,6,0,6);
    TH2F * DM3Endcap = new TH2F("EM3","Disk-3",36,0.5,36.5,6,0,6);
    TH2F * DM2Endcap = new TH2F("EM2","Disk-2",36,0.5,36.5,6,0,6);
    TH2F * DM1Endcap = new TH2F("EM1","Disk-1",36,0.5,36.5,6,0,6);
    TH2F * DP4Endcap = new TH2F("EP4","Disk+4",36,0.5,36.5,6,0,6);
    TH2F * DM4Endcap = new TH2F("EM4","Disk-4",36,0.5,36.5,6,0,6);

    // add the histos to the map
    
    allHistograms["W-2"]= WM2histo;
    allHistograms["W-1"]= WM1histo;
    allHistograms["W0"]= W0histo;
    allHistograms["W1"]= WP1histo;
    allHistograms["W2"]= WP2histo;
    
    allHistograms["RE1"] = DP1Endcap;
    allHistograms["RE-1"] = DM1Endcap;
    allHistograms["RE2"] = DP2Endcap;
    allHistograms["RE-2"] = DM2Endcap;
    allHistograms["RE3"] = DP3Endcap;
    allHistograms["RE-3"] = DM3Endcap;
    allHistograms["RE4"] = DP4Endcap;
    allHistograms["RE-4"] = DM4Endcap;
  
//   cout << "fill barrel map" << endl;
    for (int i=1 ; i <= shortBarrel.getLenght();i++) {
        barrel_labels_map[shortBarrel.getElement(i,1)] = shortBarrel.getElementAsInt(i,2);
        if (i != 8) {
            allHistograms.find("W0")->second->GetYaxis()->SetBinLabel(shortBarrel.getElementAsInt(i,2),shortBarrel.getElement(i,1).c_str());
            allHistograms.find("W1")->second->GetYaxis()->SetBinLabel(shortBarrel.getElementAsInt(i,2),shortBarrel.getElement(i,1).c_str());
            allHistograms.find("W-1")->second->GetYaxis()->SetBinLabel(shortBarrel.getElementAsInt(i,2),shortBarrel.getElement(i,1).c_str());
        }
        if (i != 7) {
            allHistograms.find("W2")->second->GetYaxis()->SetBinLabel(shortBarrel.getElementAsInt(i,2),shortBarrel.getElement(i,1).c_str());
            allHistograms.find("W-2")->second->GetYaxis()->SetBinLabel(shortBarrel.getElementAsInt(i,2),shortBarrel.getElement(i,1).c_str());
        }

    }
    
//   cout << "fill ecap map" << endl;
    for (int i=1 ; i <= shortEcap.getLenght();i++) {
        ecap_labels_map[shortEcap.getElement(i,1)] = shortEcap.getElementAsInt(i,2);
	allHistograms.find("RE4")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
	allHistograms.find("RE-4")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE3")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE2")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE1")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE-3")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE-2")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
        allHistograms.find("RE-1")->second->GetYaxis()->SetBinLabel(shortEcap.getElementAsInt(i,2),shortEcap.getElement(i,1).c_str());
    }
    
    //cout << "map filled" << endl;
    
    gStyle->Reset();
    gStyle->SetPalette(1);
    
    string MainPart;
    TH2F * pointerToHistos;
    int bufferY,bufferX;
    
    for (map<string,string>::iterator iter = RollNamesOnly.begin();iter != RollNamesOnly.end();iter++) {
        //cout << iter->first << " " << iter->second->getFullOnlineRollID() <<  " " << iter->second->getAvgRatePSCWithoutCorrections()<< endl;
        string theName = iter->first;
        aRoll = AllRolls.find(theName)->second;
	if (aRoll->isInEndcap() && theName.find("1_1_") != string::npos) continue;

        //aRoll->setStripsRatesFromTH1FObject(iter->second);
        //aRoll->setStripsAreaFromSource_cmsswResource(area);
        //
	
        for (int i=1;i <= aRoll->getClones();i++) {
            if (aRoll->isInBarrel()) {
                MainPart = aRoll->getWheel_Disk()+aRoll->returnFieldAsString(aRoll->getWheel_Disk_Number());
// 	cout <<  MainPart << " " << barrel_labels_map.find(aRoll->getShortFullIDofClone(i))->second << " " << aRoll->getSector() << endl;
// 	cout << "sector get" << endl;
                bufferX = aRoll->getSector();
// 	cout << "Y label get" << endl;
                bufferY = barrel_labels_map.find(aRoll->getShortFullIDofClone(i))->second;
// 	cout << "histo fill" << endl;
		double valueToInsert1 = aRoll->getAvgRatePSCWithoutCorrectionsForClone(i);
		//((RollName_RateSum.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second) / RollName_incrementalCount.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second) / divider;
		if (valueToInsert1 > BarrelMax) { 
		  //cout << " OOPS " << valueToInsert1 << endl; 
 		  valueToInsert1 = BarrelMax ;
 		}
 		if (valueToInsert1 < 0.3 ) { valueToInsert1 = 0.3 ; } // ugly hack
                allHistograms.find(MainPart.c_str())->second->SetBinContent(bufferX,bufferY,valueToInsert1);
                
                //cout <<  aRoll->getRollIDofCloneWithNewIdentifiers(i)<< " "<< MainPart << " " << aRoll->getShortFullIDofClone(i) << " " << aRoll->getSector()<< " "<< aRoll->getAvgRatePSCWithoutCorrectionsForClone(i) << " "<< valueToInsert1 << " BMAX " << BarrelMax << endl;
		cout << aRoll->getRollIDofCloneWithNewIdentifiers(i) << " " << bufferX << " " << bufferY << endl;
            }
            
            if (aRoll->isInEndcap()) {
                bufferX = atoi(aRoll->getRollID().c_str());
                bufferY = ecap_labels_map.find(aRoll->getShortFullIDofClone(i))->second;
                MainPart = aRoll->getWheel_Disk()+aRoll->returnFieldAsString(aRoll->getWheel_Disk_Number());
		double valueToInsert2 = aRoll->getAvgRatePSCWithoutCorrectionsForClone(i);
// 		((RollName_RateSum.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second)/ RollName_incrementalCount.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second) / divider;
 		if (valueToInsert2 > EcapMax) {
		  //cout << "OOPS" << endl;
 		  valueToInsert2 = EcapMax ;
		  
 		} 
                allHistograms.find(MainPart)->second->SetBinContent(bufferX,bufferY,valueToInsert2);
		cout << aRoll->getRollIDofCloneWithNewIdentifiers(i) << " " << bufferX << " " << bufferY << endl;
// 	cout << aRoll->getRollIDofCloneWithNewIdentifiers(i) << " " << aRoll->getAvgRatePSCWithoutCorrectionsForClone(i) << endl;

// 	cout << bufferX << " " << bufferY << " " << aRoll->getShortFullIDofClone(i) << " " << MainPart << endl;
            }
        }

        //delete aRoll;

    }
    //cout << " mark1 " << endl;
    //allHistograms.find("W0")->second->SetBinContent(1,1,20);
    TCanvas * canvasTest = new TCanvas("can0","can0",1300,700);
    TCanvas * canvasBarrel = new TCanvas("can1","can1",1912,401);
    TCanvas * canvasEndcap = new TCanvas("can2","can2",690,1027);
    
    //CenterOfMassEmergy = "#sqrt{s} = " + CenterOfMassEmergy + " TeV";
    
    // approval definitions
    TPaveText* text = new TPaveText(0.223663, 0.908299, 0.615908, 0.968894, "NDC"); //(0.368779, 0.916955, 0.715578, 0.980436, "NDC")
    //text->AddText("CMS Preliminary");
    text->SetBorderSize(0);
    text->SetFillColor(0);
    TPaveText* secondText = new TPaveText(0.00455419, 0.908299, 0.226037, 0.968894, "NDC");
    //secondText->AddText(CenterOfMassEmergy.c_str());
    secondText->SetBorderSize(0);
    secondText->SetFillColor(0);
    TPaveText * rateText = new TPaveText(0.763289, 0.908299, 0.990109, 0.968894, "NDC");
    rateText->SetFillColor(0);
    rateText->SetBorderSize(0);
    //rateText->SetTextSize();
    rateText->AddText("Rate (Hz/cm^{2})");
    //
    text->SetTextSize(secondText->GetTextSize());
    
    canvasBarrel->Divide(5,1);
    canvasEndcap->Divide(2,4);
    canvasBarrel->SetFillColor(0);
    canvasEndcap->SetFillColor(0);
    int counterBarrel =0 , counterEcap = 0;
    
    // Put two new legends 
    
    
     TLegend * detectoroff;
    detectoroff = new TLegend(0.271243,0.954037,0.502776,0.997364);
    detectoroff->SetFillColor(0);
    detectoroff->SetTextSize(0.025);
    detectoroff->SetBorderSize(0);
    //detectoroff->AddEntry(WM1_histo_masked,"Det Unit Off","f");
    
    TLegend * detectormasked;
    detectormasked = new TLegend(0.591058,0.954037,0.822591,0.997364);
    detectormasked->SetFillColor(0);
    detectormasked->SetTextSize(0.025);
    detectormasked->SetBorderSize(0);
    //detectormasked->SetB
    //detectormasked->AddEntry(WM1_histo_masked_en,"Det Unit overflow","f");
    //cout << " mark2 " << endl;
    
    map <string,int> nameOrderMap;
    
    nameOrderMap["W-2"] = 1;
    nameOrderMap["W-1"] = 2;
    nameOrderMap["W0"] = 3;
    nameOrderMap["W1"] = 4;
    nameOrderMap["W2"] = 5;
    nameOrderMap["RE1"] = 1;
    nameOrderMap["RE-1"] = 2;
    nameOrderMap["RE2"] = 3;
    nameOrderMap["RE-2"] = 4;
    nameOrderMap["RE3"] = 5;
    nameOrderMap["RE-3"] = 6;
    nameOrderMap["RE4"] = 7;
    nameOrderMap["RE-4"] = 8;
    
    for (map<string,TH2F*>::iterator itr = allHistograms.begin();itr != allHistograms.end();itr++) {
    
      itr->second->SetStats(false);
      itr->second->GetXaxis()->SetTitle("Sector");
      itr->second->GetYaxis()->SetTitleOffset(4);
      itr->second->SetMinimum(0);
      
      TPaveText * titleText = new TPaveText(0.044226, 0.906466, 0.941032, 0.950426, "NDC");//(0.211782, 0.922726, 0.345346, 0.983322, "NDC");
      titleText->SetFillColor(0);
      titleText->SetBorderSize(0);
      string textForTitle;// = CenterOfMassEmergy;
      //textForTitle += "      CMS Preliminary      ";
      textForTitle += itr->second->GetTitle();
      titleText->AddText(textForTitle.c_str());
      titleText->SetTextSize(0.08);
      itr->second->SetTitle("");
      itr->second->GetYaxis()->SetLabelSize(itr->second->GetYaxis()->GetLabelSize()*1.2);
      
      if (itr->first.find("W") != string::npos) {
            ++counterBarrel;
            canvasBarrel->cd(nameOrderMap.find(itr->first)->second)->SetFillColor(kWhite);
	    canvasBarrel->cd(nameOrderMap.find(itr->first)->second)->SetFrameFillColor(kWhite);
// 	    canvasBarrel->cd(counterBarrel)->set
// 	    mapCanvases.find(itr->first)->second->cd();
	    
	    itr->second->SetMaximum(BarrelMax);
	    
	    itr->second->GetXaxis()->SetTitleSize(itr->second->GetXaxis()->GetTitleSize()*1.2);
	    itr->second->GetXaxis()->SetTitleOffset(itr->second->GetXaxis()->GetTitleOffset()*0.8);
	    
	    itr->second->GetZaxis()->SetTitleSize(itr->second->GetZaxis()->GetTitleSize()*1.3);
	    itr->second->GetZaxis()->SetTitle("Rate (Hz/cm^{2})");
	    //itr->second->Set
	    itr->second->GetZaxis()->SetTitleOffset(0.45);
	    itr->second->GetZaxis()->CenterTitle();
	    //itr->second->GetZaxis()->SetLabelOffset(0.001);
	    itr->second->GetXaxis()->SetNdivisions(12);
	    itr->second->GetYaxis()->SetTitle("Detector Unit");
	    itr->second->GetYaxis()->SetTitleSize(itr->second->GetZaxis()->GetTitleSize());
	    itr->second->GetYaxis()->SetTitleOffset(1.5);
	    itr->second->GetYaxis()->SetLabelSize(0.032);
	    //itr->second->
	    
            itr->second->Draw("COLZ");
	    wheel_contour->Draw("HSame");
	    wheel_contour->SetName(((string)(wheel_contour->GetName())+" ").c_str());
	    TH1F * histoMask = mapOfMasks.find(itr->first)->second;
	    histoMask->SetMarkerColor(kGray+1);
	    histoMask->Draw("SameE2");
	    
	    if (itr->first == "W-1") { WM1_his_2_masked->SetMarkerColor(kGray+1); WM1_his_2_masked->Draw("SameE2"); }
	    if (itr->first == "W0") { WP0_his_2_masked->SetMarkerColor(kGray+1); WP0_his_2_masked->Draw("SameE2"); }
	    if (itr->first == "W-2") { WM2_histo_masked_h2->Draw("SameE2"); }
	    if (itr->first == "W1") { 
	      WP1_histo_masked_hist3->Draw("SameE2"); }
	    if (itr->first == "RE1") { EP1_histo_masked_h2->Draw("SameE2");}
	    if (mapOfMasks_extremenoise.find(itr->first) != mapOfMasks_extremenoise.end()  ){ mapOfMasks_extremenoise.find(itr->first)->second->Draw("SameE2"); }
	    //text->Draw();
	    //secondText->Draw();
	    detectormasked->Draw();
	    detectoroff->Draw();
	    rateText->SetTextSize(titleText->GetTextSize());
	    //rateText->Draw();
	    titleText->Draw();
	    mapCanvases.find(itr->first)->second->SaveAs((itr->first+".root").c_str());
            //cout << itr->second->GetCorrelationFactor();
        }
        else {
            ++counterEcap;
	    mapCanvases.find(itr->first)->second->cd();
            canvasEndcap->cd(nameOrderMap.find(itr->first)->second)->SetFillColor(0);
	    canvasEndcap->cd(nameOrderMap.find(itr->first)->second)->SetFrameFillColor(0);
	    itr->second->SetMaximum(EcapMax);
            //itr->second->GetYaxis()->SetTitle("Layer");
	    
	    itr->second->GetXaxis()->SetTitleSize(itr->second->GetXaxis()->GetTitleSize()*1.5);
	    itr->second->GetXaxis()->SetTitleOffset(itr->second->GetXaxis()->GetTitleOffset()*0.8);
	    itr->second->GetZaxis()->SetTitle("Rate (Hz/cm^{2})");
	    itr->second->GetZaxis()->SetTitleOffset(0.45);
	    itr->second->GetZaxis()->SetTitleSize(itr->second->GetZaxis()->GetTitleSize()*1.3);
	    itr->second->GetZaxis()->CenterTitle();    
	    itr->second->GetXaxis()->SetNdivisions(36);
	    itr->second->GetXaxis()->SetLabelSize(itr->second->GetXaxis()->GetLabelSize()*0.85);
	    itr->second->GetYaxis()->SetTitle("Detector Unit");
	    itr->second->GetYaxis()->SetTitleSize(itr->second->GetZaxis()->GetTitleSize());
	    itr->second->GetYaxis()->SetTitleOffset(1.1);
	    for (unsigned binnum = 1 ; binnum < itr->second->GetXaxis()->GetNbins() ; binnum++){
	      int moduloOfDivision = (binnum) % 2;
// 	      cout << moduloOfDivision << endl;
	      if ( moduloOfDivision  != 0 ){
		//itr->second->GetXaxis()->SetBinLabel(binnum,"");
	      }
	    }
//	     itr->second->LabelsOption("v");
      for(int k=1; k<37; ++k) {
        if(k%6==0 || k==1) {
          std::stringstream labelstream; labelstream << k;
          std::string label = labelstream.str();
          itr->second->GetXaxis()->SetBinLabel(k,label.c_str());
        }
        else {
          itr->second->GetXaxis()->SetBinLabel(k,"");
        }
      }
	    //itr->second->GetXaxis()->SetBinLabel(1,"");
	    itr->second->SetLabelSize(itr->second->GetLabelSize()*1.5);
            itr->second->Draw("COLZ");
	    TH1F * histoMask = mapOfMasks.find(itr->first)->second;
	    histoMask->SetMarkerColor(kGray+1);
	    histoMask->Draw("SameE2");
	    if (mapOfMasks_extremenoise.find(itr->first) != mapOfMasks_extremenoise.end()  ){ mapOfMasks_extremenoise.find(itr->first)->second->Draw("SameE2"); }
	    //text->Draw();
	    //secondText->Draw();
	    detectormasked->Draw();
	    detectoroff->Draw();
	    rateText->SetTextSize(titleText->GetTextSize());
	    //rateText->Draw();
	    titleText->Draw();
	    mapCanvases.find(itr->first)->second->SaveAs((itr->first+".root").c_str());
        }
        
        
    }
    //cout << " mark3 " << endl;    
    
    lastRunNum = listOfRootFiles.getElement(listOfRootFiles.getLenght(),1);
    canvasBarrel->cd(6)->SetFillColor(kWhite);
    canvasBarrel->cd(6)->SetFrameFillColor(kWhite);
    canvasBarrel->SaveAs(("Barrel_"+lastRunNum+".root").c_str());
    canvasEndcap->SaveAs(("Endcap_"+lastRunNum+".root").c_str());
//     canvasBarrel->SaveAs(("Barrel_"+lastRunNum+".png").c_str());
//     canvasEndcap->SaveAs(("Endcap_"+lastRunNum+".png").c_str());
//     canvasBarrel->SaveAs(("Barrel_"+lastRunNum+".eps").c_str());
//     canvasEndcap->SaveAs(("Endcap_"+lastRunNum+".eps").c_str());
    
    
    
}

void draw2DbackgroundPlot_online(string file, string areaFile, string listOfRolls, string runNumber,string plus_minus_sign,string ForBpart,int percCorrection,bool applyCorrection) {
    // use micheles plots for histograms template
    string currentName,currentFile,singleRollName,MainString;
    TKey * TheKey;
    TObject *obj;
    TH1F * h1;
    DataObject area(areaFile);
    //DataObject listOfRootFiles(runList);
    DataObject ListRolls(listOfRolls);
    ExRoll * aRoll;
    int sectr=0,divider = 1;//2;


    map<string,int> RollName_incrementalCount;
    map<string,double> RollName_RateSum;
    map<string,string> RollNamesOnly;
    //--------------------------------//
    map<string,int> ecap_labels_map;
    map<string,int>barrel_labels_map;
    map<string,TH2F*> allHistogramsEcap;
    map<string,TH2F*> allHistogramsBarrel;


    map<string,ExRoll*> AllRolls;

    TH2F * DP3Endcap = new TH2F("EP3","Disk+3",36,0,36,2,0,2);
    TH2F * DP2Endcap = new TH2F("EP2","Disk+2",36,0,36,2,0,2);
    TH2F * DP1Endcap = new TH2F("EP1","Disk+1",36,0,36,2,0,2);
    TH2F * DM3Endcap = new TH2F("EM3","Disk-3",36,0,36,2,0,2);
    TH2F * DM2Endcap = new TH2F("EM2","Disk-2",36,0,36,2,0,2);
    TH2F * DM1Endcap = new TH2F("EM1","Disk-1",36,0,36,2,0,2);
    TH2F * WM1histo = new TH2F("WM1","W-1",12,0,12,6,0,6);
    TH2F * WM2histo = new TH2F("WM2","W-2",12,0,12,6,0,6);
    TH2F * W0histo = new TH2F("WZ","W0",12,0,12,6,0,6);
    TH2F * WP1histo = new TH2F("WP1","W+1",12,0,12,6,0,6);
    TH2F * WP2histo = new TH2F("WP2","W+2",12,0,12,6,0,6);


    allHistogramsEcap["RE3"] = DP3Endcap;
    allHistogramsEcap["RE2"] = DP2Endcap;
    allHistogramsEcap["RE1"] = DP1Endcap;
    allHistogramsEcap["RE-3"] = DM3Endcap;
    allHistogramsEcap["RE-2"] = DM2Endcap;
    allHistogramsEcap["RE-1"] = DM1Endcap;
    allHistogramsBarrel["W-1"] = WM1histo;
    allHistogramsBarrel["W-2"] = WM2histo;
    allHistogramsBarrel["W2"] = WP2histo;
    allHistogramsBarrel["W1"] = WP1histo;
    allHistogramsBarrel["W0"] = W0histo;


   

    TCanvas * aCanv = new TCanvas ("","",1200,700);
    TCanvas * aCanvBarrel = new TCanvas ("barrel","barrel",1200,700);
    aCanv->Divide(3,2);
    aCanvBarrel->Divide(3,2);
    int ecap_can_iter=0;
    int barrel_can_iter=0;
    int barrel_layer=0;

    for (int i=1;i<=ListRolls.getLenght();i++) {
        singleRollName = ListRolls.getElement(i,1);
        ExRoll * a_roll = new ExRoll(singleRollName);
        a_roll->setStripsAreaFromSource_cmsswResource(area);
        AllRolls[singleRollName] = a_roll;
    }

    TFile * rootFile = new TFile(file.c_str(),"READ","in");

    if (rootFile->IsOpen()) {
        TIter nextkey( rootFile->GetListOfKeys());
        while (TheKey = (TKey*)nextkey()) {
            h1 = (TH1F*)TheKey->ReadObj();
            currentName = h1->GetName();
            if ( currentName.substr(0,1) == "W" || currentName.substr(0,2) == "RE" ) {

                currentName = h1->GetName();
                aRoll = AllRolls.find(currentName)->second;
                aRoll->setStripsRatesFromTH1FObject(h1);

                if (applyCorrection) aRoll->removeNoisyStripsForAllClonesWithPercentValue(percCorrection); // if correction have to be applyed , percCorrection in percents

                if (aRoll->isInEndcap()) {
                    MainString = aRoll->getWheel_Disk();
                    MainString += aRoll->returnFieldAsString(aRoll->getWheel_Disk_Number());
                    sectr=1;
                    if (aRoll->getSector() == 3 ) {
                        sectr = 2;
                    }

                    allHistogramsEcap.find(MainString)->second->SetBinContent(atoi(aRoll->getRollID().c_str()),sectr,aRoll->getAvgRatePSCWithoutCorrections()/ divider);

                }
                if (aRoll->isInBarrel()) {
                    MainString = aRoll->getWheel_Disk();
                    MainString += aRoll->returnFieldAsString(aRoll->getWheel_Disk_Number());
                    if (aRoll->getFullOnlineRollID().find("RB1in") != std::string::npos) barrel_layer = 1;
                    if (aRoll->getFullOnlineRollID().find("RB1out") != std::string::npos) barrel_layer = 2;
                    if (aRoll->getFullOnlineRollID().find("RB2in") != std::string::npos) barrel_layer = 3;
                    if (aRoll->getFullOnlineRollID().find("RB2out") != std::string::npos) barrel_layer = 4;
                    if (aRoll->getFullOnlineRollID().find("RB3") != std::string::npos) barrel_layer = 5;
                    if (aRoll->getFullOnlineRollID().find("RB4") != std::string::npos) barrel_layer = 6;

                    if ((aRoll->getOptionalRollID().size() == 1 && aRoll->getOptionalRollID().find(ForBpart) != string::npos)
                            ||
                            (aRoll->getOptionalRollID().size() == 2 && aRoll->getOptionalRollID().find(ForBpart) != string::npos && aRoll->getOptionalRollID().find(plus_minus_sign) != string::npos)
                       )

                    {
                        allHistogramsBarrel.find(MainString)->second->SetBinContent(aRoll->getSector(),barrel_layer,aRoll->getAvgRatePSCWithoutCorrections()/ divider);
                        /* // check which strips have zero rates in case the whole roll have zero rate - debug block
                         * if(aRoll->getAvgRatePSCWithoutCorrections() == 0){
                          cout << aRoll->getFullOnlineRollID() << endl;
                          for(int ii=1; ii <=96 ; ii++){
                        cout << aRoll->getStrip(ii)->getRate() << endl;

                          }
                        }
                        */
                    }



                }


            }
        }

    }
    
    // set the bin titles on Y axis for barrel histos
    for (map<string,TH2F*>::iterator iter = allHistogramsBarrel.begin() ; iter != allHistogramsBarrel.end() ; iter++) {

        iter->second->GetYaxis()->SetBinLabel(1,"RB1in");
        iter->second->GetYaxis()->SetBinLabel(2,"RB1out");;
        iter->second->GetYaxis()->SetBinLabel(3,"RB2in");;
        iter->second->GetYaxis()->SetBinLabel(4,"RB2out");
        iter->second->GetYaxis()->SetBinLabel(5,"RB3");
        iter->second->GetYaxis()->SetBinLabel(6,"RB4");
//     ++barrel_can_iter;
//     aCanvBarrel->cd(barrel_can_iter);
        iter->second->GetXaxis()->SetTitle("Sector");
        iter->second->SetStats(false);
        if (iter->first == "W-2") barrel_can_iter = 1;
        if (iter->first == "W-1") barrel_can_iter = 2;
        if (iter->first == "W1") barrel_can_iter = 4;
        if (iter->first == "W2") barrel_can_iter = 5;
        if (iter->first == "W0") barrel_can_iter = 3;
        aCanvBarrel->cd(barrel_can_iter);

        iter->second->Draw("colz");

    }
    
    for (map<string,TH2F*>::iterator itr = allHistogramsEcap.begin(); itr != allHistogramsEcap.end();itr++ ) {
        ++ecap_can_iter;
        aCanv->cd(ecap_can_iter);
        itr->second->GetXaxis()->SetTitle("Sector");
        itr->second->SetStats(false);
        itr->second->Draw("colz");
    }
    
    aCanv->SaveAs(("Ecap_online_2D_"+runNumber+".root").c_str());
    aCanvBarrel->SaveAs(("Barrel_online_2D_"+runNumber+".root").c_str());
    
}


void drawLeakRollsNoiseDistribution(string rootFileString,string areaFile, string listOfLeaks, string listOfLeakOut, string listOfNonLeakOut,string listAllRolls,string plotsRootDefinition) {
    string currentName,currentFile,singleRollName;
    ofstream OFS_N,OFS_L;
    
    OFS_N.open(listOfNonLeakOut.c_str());
    OFS_L.open(listOfLeakOut.c_str());
    
    TKey * TheKey;
    TObject *obj;
    TH1F * h1;
    DataObject area(areaFile);
    //DataObject listOfRootFiles();
    DataObject ListRolls(listAllRolls);
    DataObject ListLeaks(listOfLeaks);
    ExRoll * aRoll;
    
    TCanvas * aCan = new TCanvas("Leak","Leak",1200,700);
    TCanvas * aCan1 = new TCanvas("Leak1","Leak1",1200,700);
    TCanvas * aCan2 = new TCanvas("Leak2","Leak2",1200,700);
    TH1F * normal_h = new TH1F("norm","norm",400,0,20);
    TH1F * leak_h = new TH1F("leak_h","leak_h",400,0,20);
    leak_h->SetFillColor(kBlue);
    normal_h->SetXTitle("Rate (Hz/cm^{2})");
    normal_h->SetYTitle("Entries");
    normal_h->SetTitle("");
    normal_h->SetStats(false);
    leak_h->SetStats(false);


    map<string,ExRoll*> AllRolls;
    map<string,string> Leak;

    for (int i=1;i<=ListRolls.getLenght();i++) {
        singleRollName = ListRolls.getElement(i,1);
        ExRoll * a_roll = new ExRoll(singleRollName);
        a_roll->setStripsAreaFromSource_cmsswResource(area);
        AllRolls[singleRollName] = a_roll;
    }

    for (int i=1 ; i <= ListLeaks.getLenght() ; i++) {
        Leak[ListLeaks.getElement(i,1)] = ListLeaks.getElement(i,1);
        //cout << ListLeaks.getElement(i,1) << endl;
    }

    TFile * rootFile = new TFile(rootFileString.c_str(),"READ","in");

    if (rootFile->IsOpen()) {
        TIter nextkey( rootFile->GetListOfKeys());
        while (TheKey = (TKey*)nextkey()) {
            h1 = (TH1F*)TheKey->ReadObj();
            currentName = h1->GetName();
            if ( currentName.substr(0,1) == "W" || currentName.substr(0,2) == "RE" ) {


                aRoll = AllRolls.find(currentName)->second;
                aRoll->setStripsRatesFromTH1FObject(h1);
                //cout << currentName << endl;

                for (int k=1;k <= aRoll->getClones();k++) {
                    if (Leak.find(aRoll->getRollIDofCloneWithNewIdentifiers(k))  != Leak.end()) {
                        leak_h->Fill(aRoll->getAvgRatePSCWithoutCorrectionsForClone(k));
                        if ((aRoll->getAvgRatePSCWithoutCorrectionsForClone(k)) > 0.5 ) {
                            cout <<  aRoll->getRollIDofCloneWithNewIdentifiers(k) << " " << aRoll->getAvgRatePSCWithoutCorrectionsForClone(k) << "\n";

                        }
                        OFS_L << aRoll->getRollIDofCloneWithNewIdentifiers(k) << " " << aRoll->getAvgRatePSCWithoutCorrectionsForClone(k) << "\n";
                        OFS_L.clear();

                    }

                    else {
                        normal_h->Fill(aRoll->getAvgRatePSCWithoutCorrectionsForClone(k));
                        OFS_N << aRoll->getRollIDofCloneWithNewIdentifiers(k) << " " << aRoll->getAvgRatePSCWithoutCorrectionsForClone(k) << "\n";
                        OFS_N.clear();

                    }
                }


            }
        }

    }

    OFS_L.close();
    OFS_N.close();
    
    
    aCan->cd();
    aCan->SetLogy();
    normal_h->SetLineColor(kRed);
    normal_h->Draw();
    leak_h->SetLineColor(kBlue);
    leak_h->Draw("same");
    aCan->SaveAs((plotsRootDefinition+".root").c_str());
    aCan->SaveAs((plotsRootDefinition+".png").c_str());
    aCan1->cd();
    aCan1->SetLogy();
    normal_h->SaveAs((plotsRootDefinition+"normal_h.root").c_str());
    normal_h->Draw();
    aCan2->cd();
    aCan2->SetLogy();
    leak_h->Draw();
    aCan1->SaveAs((plotsRootDefinition+"_normal.root").c_str());
    aCan1->SaveAs((plotsRootDefinition+"_normal.png").c_str());
    leak_h->SaveAs((plotsRootDefinition+"_leak.root").c_str());
    //aCan2->SaveAs((plotsRootDefinition+"_leak.root").c_str());
//     aCan2->SaveAs((plotsRootDefinition+"_leak.png").c_str());


}

void getCorrelationCoefficients(string RateFilesContainer, string LumiFile, string areaFile,string AllRollNames,int percentsOfCut,bool SaveHistograms ,string outputFolder){
  
  
  DataObject allRollNames_(AllRollNames);
  DataObject LumiObject(LumiFile);
  DataObject area(areaFile);
  map<string,TGraph*> AllRollsFinalPlots;
  map<string,ExRoll*> AllRolls;
  int divider = 1;//2;
  string temp_name,outputWithCoefficients=outputFolder+"Coefficients.txt";
  double par1,par2;
  
  double the_gamma=0;
  
  ofstream OFS;
  OFS.open(outputWithCoefficients.c_str());
  
  for(int i=1;i <= allRollNames_.getLenght();i++){
    temp_name = allRollNames_.getElement(i,1);
    ExRoll * aRoll = new ExRoll(temp_name);
    aRoll->allocStrips();
    aRoll->initStrips();
    aRoll->setStripsAreaFromSource_cmsswResource(area);
    AllRolls[allRollNames_.getElement(i,1)] = aRoll;
    
  }
  
  
  TH1F * histoPoint;
  
  TF1 * func = new TF1("f1","pol1",0,50000);
  func->SetParName(0,"par1");
  func->SetParName(1,"par0");
  func->SetLineColor(kRed);
  
  // set params later
  
  int count = 1;
  
  setTDRStyle();
  
  for(map<string,ExRoll*>::iterator iter=AllRolls.begin();iter != AllRolls.end();iter++){
    for(int i=1; i <= iter->second->getClones();i++){
      
      
      //for(int i = 0 ){}
      
      TGraph * gr = new TGraph();
      
      gr->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
      gr->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
      gr->GetXaxis()->CenterTitle();
      gr->GetYaxis()->CenterTitle();
      gr->SetName(iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str());
      gr->SetTitle(iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str());
      gr->SetMarkerStyle(21);
      gr->SetMarkerColor(kBlack);
      
      vector<double*> aVectr;
      
      AllRollsFinalPlots[iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str()] = gr;
//       cout << count << endl;
      count++;
    }    
  }
  
  
  TKey * TheKey;
  TObject *obj;
  TH1F * h1;
  TH2F * rollCloneHisto;
  ExRoll * roll_ptr ;
  double currentLumiValue=0;
  
  int points_counter = 0;
  for(int i=1;i <= LumiObject.getLenght();i++){
    TFile * rFile= new TFile((RateFilesContainer+"total_"+LumiObject.getElement(i,1)+".root").c_str());
    
    currentLumiValue = LumiObject.getElementAsDouble(i,3)/LumiObject.getElementAsDouble(i,2);
    currentLumiValue = currentLumiValue/23.31;
    
    //cout << LumiObject.getElement(i,1) << " " << currentLumiValue << endl;
    
    if(rFile->IsOpen()){
      TIter nextkey( rFile->GetListOfKeys());
      while (TheKey = (TKey*)nextkey()) {
	h1 = (TH1F*)TheKey->ReadObj();

        temp_name = h1->GetName();
	if(/*temp_name.substr(0,1) == "W" || temp_name.substr(0,2) == "RE" && */AllRolls.find(temp_name) != AllRolls.end()){
	  
	  roll_ptr = AllRolls.find(temp_name)->second;
	  roll_ptr->setStripsRatesFromTH1FObject(h1);
	  // decide should you remove the noise
	  roll_ptr->removeNoisyStripsForAllClonesWithPercentValue(percentsOfCut); // so now this value is controlled with argument
	  for(int j=1;j <= roll_ptr->getClones();j++){
	    
	    /** for some runs , the rate for particular rolls is completely 0. In this cases this could ruins the fit, because all the other entries are fine */
	    if (roll_ptr->getAvgRatePSCWithoutCorrectionsForClone(j) > 0 ){
	      
	      AllRollsFinalPlots.find(roll_ptr->getRollIDofCloneWithNewIdentifiers(j))->second->SetPoint(points_counter,currentLumiValue,roll_ptr->getAvgRatePSCWithoutCorrectionsForClone(j));
	    
	    }
	    roll_ptr->getAvgRatePSCWithoutCorrectionsForClone(1);
	  }
	  delete h1;
	}
      }
    }
    else{
      cout << LumiObject.getElement(i,1) << " file cannot be open (missing or corrupted)" << endl;
    }
    rFile->Delete();
    rFile->Close("R");
    
    cout << "File " << LumiObject.getElement(i,1) << " processed and closed" << endl;
    points_counter++;
  }
  
  TFile * resultFile = new TFile((outputFolder+"CorrelationGraphs.root").c_str(),"NEW");
  if(resultFile->IsOpen()){cout << "the result file for histos has been open!" << endl;};
  
  TGraph * agraph;
  
  for (map<string,TGraph* >::iterator itr = AllRollsFinalPlots.begin();itr != AllRollsFinalPlots.end();itr++){
    
    agraph = itr->second;
    agraph->SetLineColor(0);
    agraph->Fit(func,"R");
    agraph->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
    agraph->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
    string someName = "lalala";
    
    agraph->SetTitle(agraph->GetName());
    cout << itr->first << " par0 " << func->GetParameter(0) << " par1 " << func->GetParameter(1) << " " << agraph->GetCorrelationFactor() << endl;
    the_gamma = TMath::Gamma((func->GetNDF()-2)/2,func->GetChisquare()/2);
    OFS <<  itr->first << " " << func->GetParameter(0) << " " << func->GetParameter(1)  << " " << func->GetChisquare() / func->GetNDF() << " " << agraph->GetCorrelationFactor() << " " << func->GetMaximum() << "\n";  
    
      OFS.clear();
      temp_name = itr->first;
      temp_name += ".root";
      //itr->second->SetMarkerColor(kRed);
      //itr->second->SetMarkerStyle(kCircle);
      if(SaveHistograms) {agraph->Write(agraph->GetName());}

      delete itr->second;
      
      //delete itr->second;
  }
  
  OFS.close();
  
  resultFile->Close("R");
  
  AllRollsFinalPlots.clear();
  AllRolls.clear();
  
}

void calculateLumiForRunWithFitCoefficientRefference(string rootFilesFolder, DataObject & area, DataObject & coefficients,string AllRollNames ,DataObject & LumiFile,string & outputContainer,bool saveHisto,double coefficientThreshold,bool use_single_roll,string SingleRollName){
  
  TH1F * difference = new TH1F ("RPCLumi","Distribution of #Delta Lumi",100,-1000,+1000);
  TCanvas * can_dif = new TCanvas("dif","dif",1200,700);
  TKey * TheKey;
  TObject *obj;
  TH1F * h1;
  int divider = 1;//2;
  //DataObject area(areaFile);
  //DataObject coefficients(coefficientFile);
  string currentRollName,rootFile;
  double par1,par2,theMean;
  map<string,double> AllRollsRates;
  map<string,double*> coefficientsMap;
  map<string,ExRoll*> AllRolls;
  DataObject rollNames(AllRollNames);
  ExRoll * aRoll;
  double lumi,lumiFromFile;
  
  //cout << "1" << endl;
  
  for(int i = 1 ; i <= rollNames.getLenght();i++){
    currentRollName = rollNames.getElement(i,1);
    ExRoll * an_roll = new ExRoll(currentRollName);
    an_roll->setStripsAreaFromSource_cmsswResource(area);
    AllRolls[currentRollName] = an_roll;
  }
  
  //cout << "2" << endl;
  
  for(int i =1 ; i <= coefficients.getLenght(); i++){
    double * values = new double[3];
    values[0] = coefficients.getElementAsDouble(i,2);
    values[1] = coefficients.getElementAsDouble(i,3);
    values[2] = coefficients.getElementAsDouble(i,5);
    coefficientsMap[coefficients.getElement(i,1)] = values;
  }
  
  //cout << "3" << endl;
  
  for(int k = 1 ; k <= LumiFile.getLenght() ; k++){
  
    rootFile = rootFilesFolder+"total_"+LumiFile.getElement(k,1)+".root";
    TFile * rFile = new TFile(rootFile.c_str(),"READ","in");
    TH1F * distribution = new TH1F ((LumiFile.getElement(k,1)+"_histo").c_str(),(LumiFile.getElement(k,1)+"_histo").c_str(),7000,0,8000);
    distribution->GetYaxis()->SetTitle("Number of rolls");
    distribution->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
    
    //cout << "4" << endl;
    if(rFile->IsOpen()){
      
      TIter nextkey( rFile->GetListOfKeys());
	while (TheKey = (TKey*)nextkey()) {
	  h1 = (TH1F*)TheKey->ReadObj();
	  currentRollName = h1->GetName();
	  if(currentRollName.substr(0,1) == "W" || currentRollName.substr(0,2) == "RE" ){
	    
	    if  (AllRolls.find(currentRollName) != AllRolls.end()){
	    
	    aRoll = AllRolls.find(currentRollName)->second ;
	    aRoll->setStripsRatesFromTH1FObject(h1);
	    aRoll->removeNoisyStripsForAllClonesWithPercentValue(100);
	    //cout << "5" << endl;
	    for(int i=1;i <= aRoll->getClones();i++){
	      // this line here is heavy hack , if single roll is required for use it would leave only this condition, if not - it would change the roll_passed value
	      // with the current roll name in the for loop, and therefore bypass the roll_name check would pass all the rolls 
	      if(!use_single_roll){SingleRollName = aRoll->getRollIDofCloneWithNewIdentifiers(i);}
	      // so this line should be removed and the logic changed more clearly,

	      if(coefficientsMap.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second[2] > coefficientThreshold && aRoll->getRollIDofCloneWithNewIdentifiers(i) == SingleRollName){
	      lumi = (aRoll->getAvgRatePSCWithoutCorrectionsForClone(i)/ divider - coefficientsMap.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second[0] );
	      //cout << "Subtraction " << lumi << " ";
	      //lumi = lumi/2;
	      lumi = lumi/coefficientsMap.find(aRoll->getRollIDofCloneWithNewIdentifiers(i))->second[1];
	      //cout << "Division " << lumi << endl;
	      distribution->Fill(lumi);
	      
	      }
	    }    
	    }    
	  }
	  //cout << "6" << endl;
	  //next
	  delete h1;
	}
      //cout << " 7" << endl;
      if(saveHisto){
	TCanvas * aCan = new TCanvas((LumiFile.getElement(k,1)+"_can").c_str(),(LumiFile.getElement(k,1)+"_can").c_str(),1200,700);
	distribution->SetFillColor(kBlue);
	distribution->SetLineColor(kBlue);
	aCan->cd();
	distribution->Draw();
	aCan->SaveAs((outputContainer+LumiFile.getElement(k,1)+"_recoveredLumi.root").c_str());
      }
      //cout << "8" << endl;
      
      //cout << "9" << endl;
      theMean = distribution->GetMean();
      //delete distribution;
      //cout << "10" << endl;
      lumiFromFile = LumiFile.getElementAsDouble(k,3)/LumiFile.getElementAsDouble(k,2);
      lumiFromFile = lumiFromFile/23.31;
      
      cout << LumiFile.getElement(k,1) << " File " << lumiFromFile << " Calculated ";
      //cout << "11" << endl;
      //if(lumiFromFile > 800)
      if(distribution->GetMean() != 0){
	difference->Fill(distribution->GetMean() - lumiFromFile);
      }
      cout << distribution->GetMean() << endl;
    }
    else { cout << rootFilesFolder+"total_"+LumiFile.getElement(k,1)+".root cant be open " << endl; }
    //cout << "File " << rFile->GetName() << " closed" << endl;
    rFile->Delete();
    rFile->Close("R");
    
    //delete distribution; //TODO (find this bug) -> cause segfault ? maybe it goes out of scope somewhere and its deleted anyway , to be studied
  }
  
//   TF1 * gausFit = new TF1("f1","[0]*exp(-0.5*((x-[1])/[2])^2))",-400,+400);
  TF1 * gausFit = new TF1("f1","gaus",-400,+400);
  
  gausFit->SetLineColor(kRed);
  
  // rate = a*lumi + b => (rate - b)/a = lumi;
  can_dif->cd();
  difference->SetLineColor(kBlue);
  difference->SetFillColor(kBlue);
  //difference->Fit(gausFit,"R");
  gausFit->GetChisquare();
  gausFit->GetNDF();
//   difference->G
  difference->Draw(); 
  
  difference->GetYaxis()->SetTitle("Number of runs");
  difference->GetXaxis()->SetTitle("#Delta(RPCLumi - DeliveredLumi)");
  
  can_dif->SaveAs((outputContainer+"PlotOnLumiDelta.root").c_str());
  
}

void recoverLumiForEachRunWithCoeffAndRateFiles(string RootContainer, string LumiForRunList,string CoefficientRefference2011, string areaFile,string AllRollNames, string outputContainer){
  
  DataObject RunList(LumiForRunList);
  DataObject CoeffCoeff(CoefficientRefference2011);
  DataObject RollNames(AllRollNames);
  DataObject area(areaFile);
  
  map<string,double*> CoefficientsMap;
  map<string,TH2F*> AllFinalHistosMap;
  map<string,ExRoll*> AllRollsMap;
  
  string currentRollName;
  
  TKey * TheKey;
  TObject *obj;
  TH1F * h1;
  
  for(int i = 1 ;i <= RollNames.getLenght();i++){
    currentRollName = RollNames.getElement(i,1);
    ExRoll * an_roll = new ExRoll(currentRollName);
    an_roll->setStripsAreaFromSource_cmsswResource(area);
    AllRollsMap[currentRollName] = an_roll;
  }
  
  int count=1,divider = 1;
  
  setTDRStyle();
  
  int markerSize_ = 0 ;
  
  
  for(map<string,ExRoll*>::iterator iter=AllRollsMap.begin();iter != AllRollsMap.end();iter++){
    for(int i=1; i <= iter->second->getClones();i++){
      
      TH2F * oneHisto = new TH2F(iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str(),
				 iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str(),
				 1000,0,3600,100,0,10);
      oneHisto->GetXaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");
      oneHisto->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
      oneHisto->SetMarkerStyle(kFullTriangleUp);
      oneHisto->SetMarkerColor(kGreen);
      markerSize_ = oneHisto->GetMarkerSize();
      oneHisto->SetMarkerSize(oneHisto->GetMarkerSize()*2);
      
      AllFinalHistosMap[iter->second->getRollIDofCloneWithNewIdentifiers(i).c_str()] = oneHisto;
      cout << count << endl;
      count++;
    }
  }
  
  string rootFile;
  ExRoll * aRoll;
  double lumi;
  
  for (int i = 1; i <= RunList.getLenght() ; i++){
    rootFile = RootContainer+"total_"+RunList.getElement(i,1)+".root";
    TFile * rFile = new TFile(rootFile.c_str(),"READ","in");
    //cout << "1" << endl; // debug trace
    if(rFile->IsOpen()){
      
      TIter nextkey( rFile->GetListOfKeys());
	while (TheKey = (TKey*)nextkey()) {
	  h1 = (TH1F*)TheKey->ReadObj();
	  currentRollName = h1->GetName();
	  if(currentRollName.substr(0,1) == "W" || currentRollName.substr(0,2) == "RE" ){
	    
	    if  (AllRollsMap.find(currentRollName) != AllRollsMap.end()){
	    
	    aRoll = AllRollsMap.find(currentRollName)->second ;
	    aRoll->setStripsRatesFromTH1FObject(h1);
	    aRoll->removeNoisyStripsForAllClonesWithPercentValue(100);
	    //cout << "2" << endl; // debug trace
	    for(int j=1;j <= aRoll->getClones();j++){
	      
	      lumi = (aRoll->getAvgRatePSCWithoutCorrectionsForClone(j)/ divider - CoeffCoeff.getElementByKeyAsDouble(aRoll->getRollIDofCloneWithNewIdentifiers(j),2));
	      lumi = lumi/CoeffCoeff.getElementByKeyAsDouble(aRoll->getRollIDofCloneWithNewIdentifiers(j),1);	      
	      
	      AllFinalHistosMap.find(aRoll->getRollIDofCloneWithNewIdentifiers(j))->second->Fill(lumi,aRoll->getAvgRatePSCWithoutCorrectionsForClone(j)/ divider);
	      
	      }
	    }    
	  }
	  //cout << "3" << endl;
	  //next
	  delete h1;
	}
  }
  
  cout << rFile->GetName() << endl;
  
  rFile->Delete();
  rFile->Close("R");
  
  }
  
  string temp_name;
    
  for (map<string,TH2F*>::iterator itr = AllFinalHistosMap.begin();itr != AllFinalHistosMap.end();itr++){
      
      temp_name = itr->second->GetName();
      temp_name += "__.root";
      itr->second->SaveAs((outputContainer+temp_name).c_str());
      delete itr->second;
      
  }
}


void RPC_HF_lumi_difference(string sourceFile, string histoObjectName, string outpuFileName){
  
  DataObject Values(sourceFile);
  TF1 * FitFunc = new TF1();
  TH2F * h1 = new TH2F(histoObjectName.c_str(),"",4000,0,4000,100,0,100);
  TCanvas * aCan = new TCanvas (outpuFileName.c_str(),outpuFileName.c_str(),1200,700);
  
  double Yval=0,Xval=0;
  
  for (int i=1 ; i <= Values.getLenght();i++){
    Yval = ((Values.getElementAsDouble(i,2) - Values.getElementAsDouble(i,3))/Values.getElementAsDouble(i,3)) * 100;
    cout << Yval << " " << Xval << endl;
    Xval = Values.getElementAsDouble(i,3);
    if(Values.getElementAsDouble(i,2) ) h1->Fill(Xval,fabs(Yval));
    
  }
  
  setTDRStyle();
  
  h1->GetYaxis()->SetTitle("(RPCLumi - HFLumi)/RPCLumi %");
  h1->GetXaxis()->SetTitle("RPCLumi");
  h1->SetMarkerColor(kBlue);
  h1->SetMarkerStyle(kFullTriangleUp);
  aCan->cd();
  h1->Draw();
  aCan->SaveAs((outpuFileName+".root").c_str());
  
}

void NewOldLBsHistos(string newLBsList, string oldLBsList, string areaFile,string resourceDir, string runNum, string resultPic,string textOutput){
  
  // TODO -> make a map <string,ExRoll*> and then close the root file instead of keep it open until the end of program
  
  DataObject area(areaFile);
  DataObject newLBs(newLBsList);
  DataObject oldLBs(oldLBsList);
  map<string,TH1F*> newLs;
  map<string,TH1F*>oldLs;
  //resourceDir = "/rpctdata/CAF";
  resourceDir += "/run"+runNum+"/total.root"; // on the noise tool server the path is that
  //resourceDir += "total_"+runNum+".root";
  TCanvas * aCan = new TCanvas("canName","",1200,1200);
  TCanvas * aCanDistr = new TCanvas("canDistr"," ",1200,1200);
  aCan->Divide(3,4);
  aCanDistr->Divide(3,4);
  int canvasCounter = 1 ;
  TKey * TheKey;
  TObject *obj;
  TH1F * h1;
  string currentRollName,someName="";
  ofstream OFS;
  OFS.open((textOutput+"_"+runNum+".txt").c_str());
  
  
  ExRoll * aRoll ;
  
  for (int i = 1; i <= newLBs.getLenght() ; i++ ){
    //TH1F * pointTo;
    newLs[newLBs.getElement(i,1)] = new TH1F();
    oldLs[oldLBs.getElement(i,1)] = new TH1F();;
    // since the two DataObject's are with equal size this is true
    
  }
  
  
  //cout << " 1" << endl;
  TFile * rFile = new TFile(resourceDir.c_str(),"READ","in");
    //cout << "4" << endl;
  if(rFile->IsOpen()){
    TIter nextkey( rFile->GetListOfKeys());
    while (TheKey = (TKey*)nextkey()) {
      h1 = (TH1F*)TheKey->ReadObj();
      currentRollName = h1->GetName();
      if(newLs.find(currentRollName) != newLs.end()){
	//cout << " 2" << endl;
	newLs[currentRollName] = h1;
      }
      if(oldLs.find(currentRollName) != oldLs.end()){
	//cout << " 3" << endl;
	oldLs[currentRollName] = h1;
      }
    }
  }
  
  //rFile->Close();
  //rFile->Delete("R");
  
  double currentStripRate = 0 ;
  
  for(map<string,TH1F*>::iterator iter = newLs.begin(); iter != newLs.end();iter++){
    someName = iter->second->GetName();
    aRoll = new ExRoll(someName);
    aRoll->allocStrips();aRoll->initStrips();
    aRoll->setStripsAreaFromSource_cmsswResource(area);
    aRoll->setStripsRatesFromTH1FObject(iter->second);
    
    TH1F * hist0 = new TH1F((aRoll->getFullOnlineRollID()+"_rate").c_str(),aRoll->getFullOnlineRollID().c_str(),96,0,96);
    TH1F * hist = new TH1F((aRoll->getFullOnlineRollID()+"_distr").c_str(),aRoll->getFullOnlineRollID().c_str(),1000,0,40);
    
    for(int i = 1; i <= aRoll->getClones() ; i++){
      for(int j = aRoll->getFirstStripNumberOfClone(i); j <= aRoll->getLastStripNumberOfClone(i);j++){
	currentStripRate = aRoll->getStrip(j)->getRate();
	if(currentStripRate > 0){
	  hist0->SetBinContent(aRoll->getStrip(j)->getOnlineNumber(),currentStripRate / aRoll->getStripsAreaFromClone(i));
	  hist->Fill(currentStripRate/aRoll->getStripsAreaFromClone(i));
	}
	else{
	  hist0->SetBinContent(aRoll->getStrip(j)->getOnlineNumber(),0);
	}
      }
    }
    //hist0->SetTitle(aRoll->getFullOnlineRollID().c_str());
    
    aCan->cd(canvasCounter);
    hist0->GetXaxis()->SetTitle("Strip number");
    hist0->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
    hist0->SetFillColor(kBlue);
    hist0->SetLineColor(kBlue);
    hist0->Draw();
   
    aCanDistr->cd(canvasCounter)->SetLogx();
    hist->GetXaxis()->SetTitle("Hz/cm^{2}");
    hist->GetYaxis()->SetTitle("Number of entries");
    hist->SetFillColor(kBlue);
    hist->SetLineColor(kBlue);
    hist->Draw();    
    
    canvasCounter++;
    
    OFS << aRoll->getFullOnlineRollID() << " " << aRoll->getAvgRatePSCWithoutCorrections() << endl;
    OFS.clear();
    //cout << aRoll->getFullOnlineRollID() << " " << aRoll->getAvgRatePSCWithoutCorrections() << endl;
    
    delete aRoll;
  }
  
  OFS << "-----------------------------OLD LBs-----------------------------------" << endl;
  OFS.clear();
  
  for(map<string,TH1F*>::iterator iter = oldLs.begin(); iter != oldLs.end();iter++){
    //cout << iter->second->GetName() << endl; // do not remove , debug line
    //aCan->cd(canvasCounter);
    //iter->second->SetFillColor(kGreen);// do not remove , debug line
    //iter->second->Draw();// do not remove , debug line
    
    someName = iter->second->GetName();
    aRoll = new ExRoll(someName);
    aRoll->allocStrips();aRoll->initStrips();
    aRoll->setStripsAreaFromSource_cmsswResource(area);
    aRoll->setStripsRatesFromTH1FObject(iter->second);
    
    TH1F * hist0 = new TH1F((aRoll->getFullOnlineRollID()+"_rate").c_str(),aRoll->getFullOnlineRollID().c_str(),96,0,96);
    TH1F * hist = new TH1F((aRoll->getFullOnlineRollID()+"_distr").c_str(),aRoll->getFullOnlineRollID().c_str(),1000,0,40);

    for(int i = 1; i <= aRoll->getClones() ; i++){
      for(int j = aRoll->getFirstStripNumberOfClone(i); j <= aRoll->getLastStripNumberOfClone(i);j++){
	currentStripRate = aRoll->getStrip(j)->getRate();
	if(currentStripRate > 0){
	  hist0->SetBinContent(aRoll->getStrip(j)->getOnlineNumber(),currentStripRate / aRoll->getStripsAreaFromClone(i));
	  hist->Fill(currentStripRate/aRoll->getStripsAreaFromClone(i));
	}
	else{
	  hist0->SetBinContent(aRoll->getStrip(j)->getOnlineNumber(),0);
	}
      }
    }
    //hist0->SetTitle(aRoll->getFullOnlineRollID().c_str());
    
    aCan->cd(canvasCounter);
    hist0->GetXaxis()->SetTitle("Strip number");
    hist0->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
    hist0->SetFillColor(kGreen);
    hist0->SetLineColor(kGreen);
    hist0->Draw();
    
    aCanDistr->cd(canvasCounter)->SetLogx();
    hist->GetXaxis()->SetTitle("Hz/cm^{2}");
    hist->GetYaxis()->SetTitle("Number of entries");
    //hist->GetTitle();
    hist->SetFillColor(kGreen);
    hist->SetLineColor(kGreen);    
    hist->Draw();
    
    canvasCounter++;
    
    OFS << aRoll->getFullOnlineRollID() << " " << aRoll->getAvgRatePSCWithoutCorrections() << endl;
    OFS.clear();
    
    delete aRoll;
  }
  
  OFS.close();
  aCan->SaveAs((resultPic+"_"+runNum+"_rates.png").c_str());
  aCanDistr->SaveAs((resultPic+"_"+runNum+"_distributions.png").c_str());
  
}

void getRatioOfChambersForTwoRuns(string runLowerRateFile, string runHigherRateFile, string area, string allRollsList, string SGrollsMap, int cutValue){
  
  // Local variable declarations
  
  DataObject theArea(area);
  DataObject rollList(allRollsList);
  DataObject SGmap(SGrollsMap);
  string rootFile="",name,currentRollName;
  
  ExRoll * aRollone, *aRolltwo,*pointerOne,*pointerTwo,*an_Roll;
  
  map<string,string> SingleGap_Map; // 
  map<string,ExRoll*> firstRun,secondRun;
  
  TH1F * h1;
  TKey * TheKey;
  TObject *obj;
  
  // Fill map of Single gap rolls
  // Actually - Y U DU DIS gooby ? the DB object IS a map ...
  // TODO change this SG block
  
  for(int i = 1 ; i <= SGmap.getLenght() ; i++ ){
    SingleGap_Map[SGmap.getElement(i,1)] = SGmap.getElement(i,1);
  }
  
  // Open the data files
  TFile * firstRunFile = new TFile(runLowerRateFile.c_str(),"READ","in");
  TFile * secondRunFile = new TFile(runHigherRateFile.c_str(),"READ","in");
  
  // Fill the maps with roll objects for two runs, and their rates
  
  for(int i = 1; i <= rollList.getLenght() ; i++){
    
    name = rollList.getElement(i,1);
    
    aRollone = new ExRoll(name);
    aRolltwo = new ExRoll(name);
    aRollone->allocStrips();aRollone->initStrips();
    aRolltwo->allocStrips();aRolltwo->initStrips();
    aRollone->setStripsAreaFromSource_cmsswResource(theArea);
    aRolltwo->setStripsAreaFromSource_cmsswResource(theArea);
    // get the rate histogram from the first file for the current roll, and remove the noisy strips
    firstRunFile->GetObject(name.c_str(),h1);
    aRollone->setStripsRatesFromTH1FObject(h1);
    aRollone->removeNoisyStripsForAllClonesWithPercentValue(cutValue);
    // get the rate histogram from second file for the current roll
    secondRunFile->GetObject(name.c_str(),h1);
    aRolltwo->setStripsRatesFromTH1FObject(h1);
    aRolltwo->removeNoisyStripsForAllClonesWithPercentValue(cutValue);
    
    firstRun[name] = aRollone;
    secondRun[name] = aRolltwo;
    
  }
  // close the rate files
  firstRunFile->Delete();
  firstRunFile->Close("R");
  secondRunFile->Delete();
  secondRunFile->Close("R");
  
  // make check
  
  if(firstRun.empty() || secondRun.empty()){
    cout << "One of the maps is empty !" << endl;
    assert(0);
    exit(-1);
  }
  
  // continue with representing the results
  
  cout << "First map: " << firstRun.size() << " elements. Second map " << secondRun.size() << " elements" << endl;
  
  // Declare all possible arrays that would be used on the plots
  
  double
  
  sg_array12[37],sg_array22[37],sg_array32[37],
  sg_array13[37],sg_array23[37],sg_array33[37],
  
  sg_Marray12[37],sg_Marray22[37],sg_Marray32[37],
  sg_Marray13[37],sg_Marray23[37],sg_Marray33[37];
  
  double arrayOfRatios13[37],arrayOfRatios23[37],arrayOfRatios33[37],
  arrayOfRatios12[37],arrayOfRatios22[37],arrayOfRatios32[37],
  arrayOfRatiosM13[37],arrayOfRatiosM23[37],arrayOfRatiosM33[37],
  arrayOfRatiosM12[37],arrayOfRatiosM22[37],arrayOfRatiosM32[37]
  ,endcapPhi[37],ex[37],ey[37];
  
  // initialize the error arrays and the 
  
  for (int ii = 0 ; ii < 37 ; ii++) {
            endcapPhi[ii] = (ii*6.28/36);
            //cout << endcapPhi[i] << " " << count<< endl;
            //cout << arrayOfRates[ii] << endl;
            ex[ii]=0;
            ey[ii]=0;
	    sg_array12[ii] = -500;sg_array22[ii] = -500;sg_array32[ii] = -500;
	    sg_array13[ii] = -500;sg_array23[ii] = -500;sg_array33[ii] = -500;
	    sg_Marray12[ii] = -500;sg_Marray22[ii] = -500;sg_Marray32[ii] = -500;
	    sg_Marray13[ii] = -500;sg_Marray23[ii] = -500;sg_Marray33[ii] = -500;
  }
  
  // Make the ratios 
  
  double rollRateRunOne =0, rollRateRunTwo =0, ratioOfTwoRollRates =0;
  int theSector =0;
  double * pointerToSGarray;
  
  for(map<string,ExRoll*>::iterator iter = firstRun.begin();iter != firstRun.end();iter++){
    currentRollName = iter->first;
    pointerOne = firstRun.find(currentRollName)->second;
    pointerTwo = secondRun.find(currentRollName)->second;
    rollRateRunOne = pointerOne->getAvgRatePSCWithoutCorrections();
    rollRateRunTwo = pointerTwo->getAvgRatePSCWithoutCorrections();
    ratioOfTwoRollRates = rollRateRunTwo/rollRateRunOne;
    theSector = atoi(pointerOne->getRollID().c_str())-1 ;
    
    // if the result is going to give infinite OR its too high, make it zero (or better, somehow change the marker)
    
    if(rollRateRunOne == 0 || ratioOfTwoRollRates > 30 ) {ratioOfTwoRollRates = 0;}
    
    // fill the appropriate array with the value - Y U NO USE switch, case ? 
    
    // first, the positive rolls ratios
    
    if(iter->first.find("RE+1_2") != string::npos){
      //cout << atoi(pointerOne->getRollID().c_str()) << endl;
      arrayOfRatios12[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array12[theSector];
    }
    if(iter->first.find("RE+2_2") != string::npos){
      arrayOfRatios22[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array22[theSector];
    }
    if(iter->first.find("RE+3_2") != string::npos){
      arrayOfRatios32[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array32[theSector];
    }
    if(iter->first.find("RE+1_3") != string::npos){
      arrayOfRatios13[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array13[theSector];
    }
    if(iter->first.find("RE+2_3") != string::npos){
      arrayOfRatios23[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array23[theSector];
    }
    if(iter->first.find("RE+3_3") != string::npos){
      arrayOfRatios33[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_array33[theSector];
    }
    
    // the negative side ...
    
    if(iter->first.find("RE-1_2") != string::npos){
      //cout << atoi(pointerOne->getRollID().c_str()) << endl;
      arrayOfRatiosM12[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray12[theSector];
    }
    if(iter->first.find("RE-2_2") != string::npos){
      arrayOfRatiosM22[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray22[theSector];
    }
    if(iter->first.find("RE-3_2") != string::npos){
      arrayOfRatiosM32[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray12[theSector];
    }
    if(iter->first.find("RE-1_3") != string::npos){
      arrayOfRatiosM13[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray13[theSector];
    }
    if(iter->first.find("RE-2_3") != string::npos){
      arrayOfRatiosM23[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray23[theSector];
    }
    if(iter->first.find("RE-3_3") != string::npos){
      arrayOfRatiosM33[theSector] = ratioOfTwoRollRates;
      pointerToSGarray =  & sg_Marray33[theSector];
    }
    // set the single gap marker if the Roll is singleGap
    
    if (SingleGap_Map.find(pointerOne->getFullOnlineRollID()) != SingleGap_Map.end()){
      *pointerToSGarray = ratioOfTwoRollRates;
    }
    
  }
  
  // Create the plot objects, canvases, graphs, legends, styles, etc. This part is more like macro
  
  // Plots title, set it later from the parameters,
  
  string histoTitle="Some title ";
  
  setTDRStyle();
  
  TCanvas *cR2 = new TCanvas("cR2","multigraph",10,10,1200,700);
  cR2->SetFillColor(0);
  TH1F *hr = cR2->DrawFrame(0,0,6.282,15);
  hr->SetXTitle("  #phi (rad) ");
  hr->SetYTitle("Rate ratio");
  hr->SetTitle((histoTitle+" - Endcap+ Rings 2").c_str());
  TCanvas *cR3 = new TCanvas("cR3","multigraph",10,10,1200,700);
  cR3->SetFillColor(0);
  TH1F *hr1 = cR3->DrawFrame(0,0,6.282,15);
  hr1->SetXTitle("  #phi (rad) ");
  hr1->SetYTitle("Rate ratio");
  hr1->SetTitle((histoTitle+" - Endcap+ Rings 3").c_str());
  TCanvas *cMR2 = new TCanvas("cMR2","multigraph",10,10,1200,700);
  cMR2->SetFillColor(0);
  TH1F *hr2 = cMR2->DrawFrame(0,0,6.282,15);
  hr2->SetXTitle("  #phi (rad) ");
  hr2->SetYTitle("Rate ratio");
  hr2->SetTitle((histoTitle+" - Endcap- Rings 2").c_str());
  TCanvas *cMR3 = new TCanvas("cMR3","multigraph",10,10,1200,700);
  cMR3->SetFillColor(0);
  TH1F *hr3 = cMR3->DrawFrame(0,0,6.282,15);
  hr3->SetXTitle("  #phi (rad) ");
  hr3->SetYTitle("Rate ratio");
  hr3->SetTitle((histoTitle+" - Endcap- Rings 3").c_str());
  
  TLegend * ratiosdp2 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  ratiosdp2->SetFillColor(0);ratiosdp2->SetBorderSize(0);
  TLegend * ratiosdp3 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  ratiosdp3->SetFillColor(0);ratiosdp3->SetBorderSize(0);
  TLegend * ratiosdn2 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  ratiosdn2->SetFillColor(0);ratiosdn2->SetBorderSize(0);
  TLegend * ratiosdn3 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  ratiosdn3->SetFillColor(0);ratiosdn3->SetBorderSize(0);
  TGraphErrors * SG_g12 = new TGraphErrors(37,endcapPhi,sg_array12,ex,ey);
  TGraphErrors * SG_g22 = new TGraphErrors(37,endcapPhi,sg_array22,ex,ey);
  TGraphErrors * SG_g32 = new TGraphErrors(37,endcapPhi,sg_array32,ex,ey);
  TGraphErrors * SG_g13 = new TGraphErrors(37,endcapPhi,sg_array13,ex,ey);
  TGraphErrors * SG_g23 = new TGraphErrors(37,endcapPhi,sg_array23,ex,ey);
  TGraphErrors * SG_g33 = new TGraphErrors(37,endcapPhi,sg_array33,ex,ey);
  TGraphErrors * SG_Mg12 = new TGraphErrors(37,endcapPhi,sg_Marray12,ex,ey);
  TGraphErrors * SG_Mg22 = new TGraphErrors(37,endcapPhi,sg_Marray22,ex,ey);
  TGraphErrors * SG_Mg32 = new TGraphErrors(37,endcapPhi,sg_Marray32,ex,ey);
  TGraphErrors * SG_Mg13 = new TGraphErrors(37,endcapPhi,sg_Marray13,ex,ey);
  TGraphErrors * SG_Mg23 = new TGraphErrors(37,endcapPhi,sg_Marray23,ex,ey);
  TGraphErrors * SG_Mg33 = new TGraphErrors(37,endcapPhi,sg_Marray33,ex,ey);
  
  // Set the single gap colors
  
  TGraphErrors * allSGgraphs[12]  = {SG_g12,SG_g22,SG_g32,SG_g13,SG_g23,SG_g33,SG_Mg12,SG_Mg22,SG_Mg32,SG_Mg13,SG_Mg23,SG_Mg33};
  for (int i = 0 ; i < 12 ; i++){
    allSGgraphs[i]->SetMarkerStyle(kOpenSquare);
    allSGgraphs[i]->SetLineColor(0);
    allSGgraphs[i]->SetMarkerSize(1.2);
    allSGgraphs[i]->SetMarkerColor(kRed);
  }
  //  
  
  TGraphErrors * graph12 = new TGraphErrors(37,endcapPhi,arrayOfRatios12,ex,ey);
  cR2->cd();
  graph12->Draw("LP");
  TGraphErrors * graph22 = new TGraphErrors(37,endcapPhi,arrayOfRatios22,ex,ey);
  graph22->SetMarkerColor(kRed);
  graph22->Draw("LP");
  TGraphErrors * graph32 = new TGraphErrors(37,endcapPhi,arrayOfRatios32,ex,ey);
  graph32->SetMarkerColor(kGreen);
  graph32->Draw("LP");
  SG_g12->Draw("LP");
  SG_g22->Draw("LP");
  SG_g32->Draw("LP");
  ratiosdp2->AddEntry(graph12,"Disk +1 Ring 2","LP");
  ratiosdp2->AddEntry(graph22,"Disk +2 Ring 2","LP");
  ratiosdp2->AddEntry(graph32,"Disk +3 Ring 2","LP");
  ratiosdp2->AddEntry(SG_g12,"Single gap roll","LP");
  ratiosdp2->Draw();
  
  cR2->SaveAs("P2.root");
  TGraphErrors * graph13 = new TGraphErrors(37,endcapPhi,arrayOfRatios13,ex,ey);
  cR3->cd();
  graph13->Draw("LP");
  TGraphErrors * graph23 = new TGraphErrors(37,endcapPhi,arrayOfRatios23,ex,ey);
  graph23->SetMarkerColor(kRed);
  graph23->Draw("LP");
  TGraphErrors * graph33 = new TGraphErrors(37,endcapPhi,arrayOfRatios33,ex,ey);
  graph33->SetMarkerColor(kGreen);
  graph33->Draw("LP");
  SG_g13->Draw("LP");
  SG_g23->Draw("LP");
  SG_g33->Draw("LP");
  ratiosdp3->AddEntry(graph13,"Disk +1 Ring 3","LP");
  ratiosdp3->AddEntry(graph23,"Disk +2 Ring 3","LP");
  ratiosdp3->AddEntry(graph33,"Disk +1 Ring 3","LP");
  ratiosdp3->AddEntry(SG_g13,"Single gap roll","LP");
  ratiosdp3->Draw();
  cR3->SaveAs("P3.root");
  TGraphErrors * graphM12 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM12,ex,ey);
  cMR2->cd();
  graphM12->Draw("LP");
  TGraphErrors * graphM22 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM22,ex,ey);
  graphM22->SetMarkerColor(kRed);
  graphM22->Draw("LP");
  TGraphErrors * graphM32 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM32,ex,ey);
  graphM32->SetMarkerColor(kGreen);
  graphM32->Draw("LP");
  SG_Mg12->Draw("LP");
  SG_Mg22->Draw("LP");
  SG_Mg32->Draw("LP");
  ratiosdn2->AddEntry(graphM12,"Disk -1 Ring 2","LP");
  ratiosdn2->AddEntry(graphM22,"Disk -2 Ring 2","LP");
  ratiosdn2->AddEntry(graphM32,"Disk -3 Ring 2","LP");
  ratiosdn2->AddEntry(SG_Mg12,"Single gap roll","LP");
  ratiosdn2->Draw();
  
  cMR2->SaveAs("N2.root");
  TGraphErrors * graphM13 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM13,ex,ey);
  cMR3->cd();
  graphM13->Draw("LP");
  TGraphErrors * graphM23 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM23,ex,ey);
  graphM23->SetMarkerColor(kRed);
  graphM23->Draw("LP");
  TGraphErrors * graphM33 = new TGraphErrors(37,endcapPhi,arrayOfRatiosM33,ex,ey);
  graphM33->SetMarkerColor(kGreen);
  graphM33->Draw("LP");
  SG_Mg13->Draw("LP");
  SG_Mg23->Draw("LP");
  SG_Mg33->Draw("LP");
  ratiosdn3->AddEntry(graphM13,"Disk -1 Ring 3","LP");
  ratiosdn3->AddEntry(graphM23,"Disk -2 Ring 3","LP");
  ratiosdn3->AddEntry(graphM33,"Disk -3 Ring 3","LP");
  ratiosdn2->AddEntry(SG_Mg13,"Single gap roll","LP");
  ratiosdn3->Draw();
  cMR3->SaveAs("N3.root");
  
  
}


void getRatioOfNegativePositiveChambers(string DataFolder,string area,string allRollsList,
					string SGrollsMap,string FileLumi,int cutValue,int referenceRunNumber,string NPtitle)
{
  
  DataObject theArea(area);
  DataObject rollList(allRollsList);
  DataObject SGmap(SGrollsMap);
  DataObject LumiObject(FileLumi);
  
  string rootFile="";
  
  ExRoll *pointerOne,*pointerTwo,*an_Roll;
  string name,currentRollName;
  TH1F * h1;
  TKey * TheKey;
  TObject *obj;
  
  map<string, map<int,double> > RollName_RunRatesmap;
  
  map<string,ExRoll*> allEcapRolls;
  
  map<int,double> CalculatedLumiResource; // run - luminosity value
  
  map<string,double> Roll_ResultRate_Map;
  
  map<string,double> Roll_RollsErrors_map;
  
  map<string,string> SingleGap_Map; // 
  
  for(int i = 1 ; i <= SGmap.getLenght() ; i++ ){
    SingleGap_Map[SGmap.getElement(i,1)] = SGmap.getElement(i,1);
  }
  
  for(int i= 1; i <= LumiObject.getLenght();i++){
    
    CalculatedLumiResource[LumiObject.getElementAsInt(i,1)] = (LumiObject.getElementAsDouble(i,3))/(LumiObject.getElementAsDouble(i,2)*23.31);
    
  }
  
  double valueOfLumiForRefRun = CalculatedLumiResource.find(referenceRunNumber)->second;
  cout << "check the ref lumiValue " << valueOfLumiForRefRun << endl;
  
  valueOfLumiForRefRun = 3000;
  cout << "check the used reference lumi" << valueOfLumiForRefRun << endl;
  
  /** Fill the three maps of */
  
  for(int i = 1; i <= rollList.getLenght() ; i++){
    
    if(rollList.getElement(i,1).find("RE") != string::npos){
      
      name = rollList.getElement(i,1);
      
      an_Roll = new ExRoll(name);
      an_Roll->allocStrips();
      an_Roll->initStrips();
      an_Roll->setStripsAreaFromSource_cmsswResource(theArea);
      
      allEcapRolls[name] = an_Roll;
      
      map<int,double> tmp_m;
      
      RollName_RunRatesmap[name] = tmp_m;
      
    }
  }
  
  int currentRun;
  double buffer_double = 0;
  ExRoll * ptrTo_Roll;
  
  cout << "trace 1" << endl; // its a debug 
  
  // Filling the map -> Roll -> map<run,averageRate>
  
  for(int i=1;i <= LumiObject.getLenght();i++){
    
    rootFile = DataFolder+"total_"+LumiObject.getElement(i,1)+".root";
    TFile * rFile = new TFile(rootFile.c_str(),"READ","in");
    //cout << "4" << endl;
    if(rFile->IsOpen()){
      
      TIter nextkey( rFile->GetListOfKeys());
      currentRun = LumiObject.getElementAsInt(i,1);
      while (TheKey = (TKey*)nextkey()) {
	h1 = (TH1F*)TheKey->ReadObj();
	currentRollName = h1->GetName();
	
	if(currentRollName.find("RE") != string::npos){
	  ptrTo_Roll = allEcapRolls.find(currentRollName)->second;
	  //ptrTo_Roll->ClearStripsConditions(); // no need, actually is redundant operation
	  ptrTo_Roll->setStripsRatesFromTH1FObject(h1);
	  ptrTo_Roll->removeNoisyStripsForAllClonesWithPercentValue(cutValue);
	  
	  // make the rescale here, before filling 
	  // use normalization factor runLumi / referenceLumi
	  
	  RollName_RunRatesmap.find(currentRollName)->second[currentRun] = ptrTo_Roll->getAvgRatePSCWithoutCorrections()*(valueOfLumiForRefRun/CalculatedLumiResource.find(currentRun)->second);
	  
	}
	
      }
      
    }
    
    cout << "closing file " << currentRun << endl;
    rFile->Delete();
    rFile->Close("R");
    
  }
  
  // to be declared elsewhere
  double sumarizer=0,theSumDivider=0,theNegativeCounter=0,meanHistoValue=0;
  
  for (map<string, map<int,double> >::iterator iter_one = RollName_RunRatesmap.begin();iter_one != RollName_RunRatesmap.end() ; iter_one++){
    
    cout << iter_one->first << " " << iter_one->second.size() << " ";
    
    TH1F * err_xy = new TH1F(iter_one->first.c_str(),"",100,0,30);
    
    for (map<int,double>::iterator iter_two = iter_one->second.begin() ; iter_two != iter_one->second.end() ; iter_two++){
      
      // get the errors
      
      if(iter_two->second < 30){
	sumarizer += iter_two->second;
	err_xy->Fill(iter_two->second);
      }
      else{
	cout << endl << iter_two->first << " rescaled rate is " << iter_two->second << " lumi for this run is " << CalculatedLumiResource.find(iter_two->first)->second << " ref lumi is " << valueOfLumiForRefRun << " original rate is " << iter_two->second * (CalculatedLumiResource.find(iter_two->first)->second / valueOfLumiForRefRun ) ;
	theNegativeCounter ++;
      }
    }
    
    Roll_RollsErrors_map[iter_one->first] = err_xy->GetMeanError();
    meanHistoValue = err_xy->GetMean();
    
    delete err_xy;
    
    cout << endl << " "<< sumarizer << " "; 
    theSumDivider = iter_one->second.size() - theNegativeCounter;
    Roll_ResultRate_Map[iter_one->first] = sumarizer / theSumDivider;
    
    cout << " the merged rate is " << Roll_ResultRate_Map.find(iter_one->first)->second << " the mean error is " << Roll_RollsErrors_map.find(iter_one->first)->second << " runs not used " << theNegativeCounter << endl;
    
    meanHistoValue = 0;
    sumarizer = 0;
    theNegativeCounter=0;
  }
  
  // Finally - we have rescaled values in Roll_ResultRate_Map , make the ratios
  
  /** The fill of the shared maps ends here ! */
  
  /** Plots filling starts here ! */
  
  double
  endcapPhi[37],ex[37],ey[37],
  sg_array12[37],sg_array22[37],sg_array32[37],
  sg_array13[37],sg_array23[37],sg_array33[37];
  
  double exN12[37],exN22[37],exN32[37],exN13[37],exN23[37],exN33[37],
         exP12[37],exP22[37],exP32[37],exP13[37],exP23[37],exP33[37];
  
  for (int ii = 0 ; ii < 37 ; ii++) {
            endcapPhi[ii] = (ii*6.28/36);
            ex[ii]=0;
            ey[ii]=0;
	    exN12[ii]=0;exN22[ii]=0;exN32[ii]=0;exN13[ii]=0;exN23[ii]=0;exN33[ii]=0;
	    exP12[ii]=0;exP22[ii]=0;exP32[ii]=0;exP13[ii]=0;exP23[ii]=0;exP33[ii]=0;
            sg_array12[ii] = -500;sg_array22[ii] = -500;sg_array32[ii] = -500;
	    sg_array13[ii] = -500;sg_array23[ii] = -500;sg_array33[ii] = -500;
  }
  
  setTDRStyle();
  
  // negative positive ratios 
  
  TCanvas *Rings3 = new TCanvas("Rings3","multigraph",10,10,1200,700);
  Rings3->SetFillColor(0);
  TH1F *r3s = Rings3->DrawFrame(0,0,6.282,15);
  r3s->SetXTitle("  #phi (rad) ");
  r3s->GetYaxis()->SetTitleOffset(0.925);
  r3s->SetYTitle("Rate ratio");
  r3s->SetTitle(("Negative/Positive chambers rate ratio, Rings 3 "+NPtitle).c_str());
  TCanvas *Rings2 = new TCanvas("Rings2","multigraph",10,10,1200,700);
  Rings2->SetFillColor(0);
  TH1F *r2s = Rings2->DrawFrame(0,0,6.282,15);
  r2s->SetXTitle("  #phi (rad) ");
  r2s->GetYaxis()->SetTitleOffset(0.925);
  r2s->SetYTitle("Rate ratio");
  r2s->SetTitle(("Negative/Positive chambers rate ratio, Rings 2 "+NPtitle).c_str());
  
  TH1F * dis13 = new TH1F("d13","Ratios distribution D1 Ring 3",50,0,5);
  TH1F * dis23 = new TH1F("d23","Ratios distribution D2 Rings 3",50,0,5);
  TH1F * dis33 = new TH1F("d33","Ratios distribution D3 Rings 3",50,0,5);
  TH1F * dis12 = new TH1F("d12","Ratios distribution D1 Rings 2",50,0,5);
  TH1F * dis22 = new TH1F("d22","Ratios distribution D2 Rings 2",50,0,5);
  TH1F * dis32 = new TH1F("d32","Ratios distribution D3 Rings 2",50,0,5);
  
  // this buffer vars are added afterwards for the purpose of the rescaled cumulative histograms
  
  double plusValue=0, minusValue=0;
  int theSector = 0;
  double ratiosRings13[37],ratiosRings23[37],ratiosRings33[37],ratiosRings12[37],ratiosRings22[37],ratiosRings32[37];
  
  TFile * RatiosDistrSave = new TFile((NPtitle+"_RatiosDistributionHistos.root").c_str(),"NEW");
  if(RatiosDistrSave->IsOpen()){cout << "The ratios distribution objects file is open ... " << endl;}
  
  /** Fill the arrays that would represent the final values */
  
  for(map<string,ExRoll*>::iterator itr = allEcapRolls.begin();itr != allEcapRolls.end();itr++){
    
    /** make all the calculations here and pointer assignements*/
    
    currentRollName = itr->first;
    
    /** check if the roll is positive, if its not, skip the exectution */ 
    
    if (currentRollName.find("RE-") != string::npos){
      continue;
    }
    
    pointerOne = allEcapRolls.find(currentRollName)->second;
    pointerTwo = allEcapRolls.find(pointerOne->getOpositeRollIDEcapOnly())->second;
    theSector = atoi(pointerOne->getRollID().c_str()) - 1;
    TH1F * TheRatiosDistribution;
    string 
    positiveSideRollName = pointerOne->getFullOnlineRollID().c_str(),
    negativeSideRollName = pointerOne->getOpositeRollIDEcapOnly();
    
    // references to internal loop variables
    TH1F * pointerToCurrentDistribution;
    double * pointerRatiosRings, *pointerEX_errors , *pointerSGarray;
    
    TheRatiosDistribution = new TH1F(positiveSideRollName.c_str(),positiveSideRollName.c_str(),100,0,10);
    
    for(map<int,double>::iterator iter = RollName_RunRatesmap.find(positiveSideRollName)->second.begin(); iter != RollName_RunRatesmap.find(positiveSideRollName)->second.end(); iter ++ ){
	currentRun = iter->first;
	plusValue = iter->second;
	minusValue = RollName_RunRatesmap.find(negativeSideRollName)->second.find(currentRun)->second;
	
	// try to use references to those arrays, and make the if () statements just once
	if (currentRollName.find("RE+1_3") != string::npos){
	  pointerToCurrentDistribution = dis13;
	  pointerRatiosRings = & ratiosRings13[theSector];
	  pointerEX_errors = & exP13[theSector];
	  pointerSGarray = & sg_array13[theSector];
	}
	if (currentRollName.find("RE+2_3") != string::npos){
	  pointerToCurrentDistribution = dis23;
	  pointerRatiosRings = & ratiosRings23[theSector];
	  pointerEX_errors = & exP23[theSector];
	  pointerSGarray = & sg_array23[theSector];
	}
	if (currentRollName.find("RE+3_3") != string::npos){
	  pointerToCurrentDistribution = dis33;
	  pointerRatiosRings = & ratiosRings33[theSector];
	  pointerEX_errors = & exP33[theSector];
	  pointerSGarray = & sg_array33[theSector];
	}
	if (currentRollName.find("RE+1_2") != string::npos){
	  pointerToCurrentDistribution = dis12;
	  pointerRatiosRings = & ratiosRings12[theSector];
	  pointerEX_errors = & exP12[theSector];
	  pointerSGarray = & sg_array12[theSector];
	}
	if (currentRollName.find("RE+2_2") != string::npos){
	  pointerToCurrentDistribution = dis22;
	  pointerRatiosRings = & ratiosRings22[theSector];
	  pointerEX_errors = & exP22[theSector];
	  pointerSGarray = & sg_array22[theSector];
	}
	if (currentRollName.find("RE+3_2") != string::npos){
	  pointerToCurrentDistribution = dis32;
	  pointerRatiosRings = & ratiosRings32[theSector];
	  pointerEX_errors = & exP32[theSector];
	  pointerSGarray = & sg_array32[theSector];
	}
	
	if(plusValue != 0 && plusValue < 30){
	  
	  TheRatiosDistribution->Fill(minusValue/plusValue);
	  pointerToCurrentDistribution->Fill(minusValue/plusValue);
	  
	}  
      }
      
      // set the values of the 
      *pointerRatiosRings = TheRatiosDistribution->GetMean();
      *pointerEX_errors = TheRatiosDistribution->GetMeanError();
      
      TheRatiosDistribution->Write(TheRatiosDistribution->GetName());
      delete TheRatiosDistribution;
      TheRatiosDistribution = NULL;
      
      // add the SG markers
      if(SingleGap_Map.find(positiveSideRollName) != SingleGap_Map.end() || SingleGap_Map.find(negativeSideRollName) != SingleGap_Map.end() ) {  
	*pointerSGarray = *pointerRatiosRings;
      }
      
  }
  
  RatiosDistrSave->Close("R");
  
  cout << "The distribution file closed !" << endl;
  stringstream ss; // to cast the mean values
  string currentMeanOfHisto;
  
  TCanvas * aCan = new TCanvas("distr","distr",1200,700);
  
  TLegend * meansRing2Legend = new TLegend(0.621818,0.706318,0.936307,0.914668);
  TLegend * meansRing3Legend = new TLegend(0.621818,0.706318,0.936307,0.914668);
  meansRing2Legend->SetTextSize(meansRing2Legend->GetTextSize()*5);
  meansRing3Legend->SetTextSize(meansRing3Legend->GetTextSize()*5);
  meansRing2Legend->SetBorderSize(0);
  meansRing3Legend->SetBorderSize(0);
  
  ss << dis32->GetMean();
  ss >> currentMeanOfHisto;
  meansRing2Legend->AddEntry(dis32,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  ss << double(dis22->GetMean());
  ss >> currentMeanOfHisto;
  meansRing2Legend->AddEntry(dis22,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  ss << dis12->GetMean();
  ss >> currentMeanOfHisto;
  meansRing2Legend->AddEntry(dis12,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  
  ss << dis13->GetMean();
  ss >> currentMeanOfHisto;
  meansRing3Legend->AddEntry(dis23,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  ss << dis23->GetMean();
  ss >> currentMeanOfHisto;
  meansRing3Legend->AddEntry(dis23,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  ss << dis33->GetMean();
  ss >> currentMeanOfHisto;
  meansRing3Legend->AddEntry(dis33,("Mean "+currentMeanOfHisto).c_str());
  ss.clear();
  
  aCan->Divide(2,1);
  aCan->cd(1);
  
  dis32->SetTitle("Ratios distribution Ring 2 - D1R2 red,D2R2 green, D3R2 blue");
  dis32->SetLineColor(kBlue);
  dis32->Draw();
  dis32->GetMeanError(); // ? why ? 
  dis12->SetLineColor(kRed);
  dis12->Draw("same");
  dis22->SetLineColor(kGreen);
  dis22->Draw("same");
  meansRing2Legend->Draw();
  
  aCan->cd(2);
  dis33->SetTitle("Ratios distribution Ring 3 - D1R3 red,D2R3 green, D3R3 blue");
  dis33->SetLineColor(kBlue);
  dis33->Draw();
  dis13->SetLineColor(kRed);
  dis13->Draw("same");
  dis23->SetLineColor(kGreen);
  dis23->Draw("same");
  meansRing3Legend->Draw();
  
  aCan->SaveAs("Distributions.root");
  
  TLegend * legRings3 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  legRings3->SetFillColor(0);legRings3->SetBorderSize(0);
  TLegend * legRings2 = new TLegend(0.218063,0.606759,0.404552,0.93226);
  legRings2->SetFillColor(0);legRings2->SetBorderSize(0);
  
  TGraphErrors * g13 = new TGraphErrors(37,endcapPhi,ratiosRings13,exN13,exP13);
  TGraphErrors * g23 = new TGraphErrors(37,endcapPhi,ratiosRings23,exN23,exP23);
  TGraphErrors * g33 = new TGraphErrors(37,endcapPhi,ratiosRings33,exN33,exP33);
  TGraphErrors * SG_g13 = new TGraphErrors(37,endcapPhi,sg_array13,ex,ey);
  TGraphErrors * SG_g23 = new TGraphErrors(37,endcapPhi,sg_array23,ex,ey);
  TGraphErrors * SG_g33 = new TGraphErrors(37,endcapPhi,sg_array33,ex,ey);
  
  Rings3->cd();
  g23->SetMarkerColor(kViolet);
  g33->SetMarkerColor(kBlue);
  g13->SetMarkerSize(0.8);
  g23->SetMarkerSize(0.8);
  g33->SetMarkerSize(0.8);
  SG_g13->SetMarkerStyle(kOpenSquare);
  SG_g13->SetLineColor(0);
  SG_g13->SetMarkerSize(1.2);
  SG_g13->SetMarkerColor(kRed);
  SG_g23->SetMarkerStyle(kOpenSquare);
  SG_g23->SetLineColor(0);
  SG_g23->SetMarkerSize(1.2);
  SG_g23->SetMarkerColor(kRed);
  SG_g33->SetMarkerStyle(kOpenSquare);
  SG_g33->SetLineColor(0);
  SG_g33->SetMarkerSize(1.2);
  SG_g33->SetMarkerColor(kRed);
  
  g13->Draw("LP");
  g23->Draw("LP");
  g33->Draw("LP");
  SG_g13->Draw("LP");
  SG_g23->Draw("LP");
  SG_g33->Draw("LP");
  
  legRings3->AddEntry(g13,"Disk 1 Ring 3","LP");
  legRings3->AddEntry(g23,"Disk 2 Ring 3","LP");
  legRings3->AddEntry(g33,"Disk 3 Ring 3","LP");
  legRings3->AddEntry(SG_g13,"Single Gap","LP");
  legRings3->Draw();
  Rings3->SaveAs("Rings3.root");
  
  TGraphErrors * g12 = new TGraphErrors(37,endcapPhi,ratiosRings12,exN12,exP12);
  TGraphErrors * g22 = new TGraphErrors(37,endcapPhi,ratiosRings22,exN22,exP22);
  TGraphErrors * g32 = new TGraphErrors(37,endcapPhi,ratiosRings32,exN32,exP32);
  TGraphErrors * SG_g12 = new TGraphErrors(37,endcapPhi,sg_array12,ex,ey);
  TGraphErrors * SG_g22 = new TGraphErrors(37,endcapPhi,sg_array22,ex,ey);
  TGraphErrors * SG_g32 = new TGraphErrors(37,endcapPhi,sg_array32,ex,ey);
  
  Rings2->cd();
  g22->SetMarkerColor(kViolet);
  g32->SetMarkerColor(kBlue);
  g12->SetMarkerSize(0.8);
  g22->SetMarkerSize(0.8);
  g32->SetMarkerSize(0.8);
  
  SG_g12->SetMarkerStyle(kOpenSquare);
  SG_g12->SetLineColor(0);
  SG_g12->SetMarkerSize(1.2);
  SG_g12->SetMarkerColor(kRed);
  SG_g22->SetMarkerStyle(kOpenSquare);
  SG_g22->SetLineColor(0);
  SG_g22->SetMarkerSize(1.2);
  SG_g22->SetMarkerColor(kRed);
  SG_g32->SetMarkerStyle(kOpenSquare);
  SG_g32->SetLineColor(0);
  SG_g32->SetMarkerSize(1.2);
  SG_g32->SetMarkerColor(kRed);
  
  g12->Draw("LP");
  g22->Draw("LP");
  g32->Draw("LP");
  SG_g12->Draw("LP");
  SG_g22->Draw("LP");
  SG_g32->Draw("LP");
  
  legRings2->AddEntry(g12,"Disk 1 Ring 2","LP");
  legRings2->AddEntry(g22,"Disk 2 Ring 2","LP");
  legRings2->AddEntry(g32,"Disk 3 Ring 2","LP");
  legRings2->AddEntry(SG_g12,"Single Gap","LP");
  
  legRings2->Draw();
  Rings2->SaveAs("Rings2.root");
  
  
}

void plotRateOnLumi_Vs_Lumi_Endcap_OfflineAverageForDisk ( string dataFolder, DataObject& LumiFile, DataObject& area, string DiskNumber ) {
  
  //map<int,double> Runnumber_

}


void compareMichelesMirchosCoeff(){
  
}

/* needed for ? */

void drawerFunc(){
  
  TCanvas * aCan = new TCanvas("Leak","Leak",1200,700);
  
  TLegend * leg = new TLegend(0.708517,0.565278,0.894273,0.891667);
  TString n1,n2,n3,n4,n5,nn,runnum,saveit;
  runnum = "180250";
  saveit = runnum+".root";
  
  n1 = runnum+"_CBDTB_98_leak.root";
  n2 = runnum+"_CBDTB_ALL_leak.root";
  n3 = runnum+"_NOT_CONTROLLED_leak.root";
  n4 = runnum+"_SGMB_leak.root";
  n5 = runnum+"_SGME_leak.root";
  nn = runnum+"CBDTB_98normal_h.root";
  
  TH1F * h1, * h2, * h3,* h4,* h5 ,* norm;
  TFile * f1 = new TFile(n1,"READ");
  TFile * f2 = new TFile(n2,"READ");
  TFile * f3 = new TFile(n3,"READ");
  TFile * f4 = new TFile(n4,"READ");
  TFile * f5 = new TFile(n5,"READ");
  
  TFile * nfile = new TFile(nn,"READ");
  
  
  f1->GetObject("leak_h",h1);
  f2->GetObject("leak_h",h2);
  f3->GetObject("leak_h",h3);
  f4->GetObject("leak_h",h4);
  f5->GetObject("leak_h",h5);
  nfile->GetObject("norm",norm);
  
  
  norm->SetLineColor(kRed);
  norm->SetFillColor(kWhite);
  h1->SetLineColor(kGreen);
  h1->SetFillColor(kWhite);
  h2->SetLineColor(kViolet+3);
  h2->SetFillColor(kWhite);
  h3->SetLineColor(kCyan);
  h3->SetFillColor(kWhite);
  h4->SetLineColor(kMagenta);
  h4->SetFillColor(kWhite);
  h5->SetLineColor(kViolet);
  h5->SetFillColor(kWhite);
  
  norm->Draw();
  h5->Draw("same");
  h4->Draw("same");
  h3->Draw("same");
  h2->Draw("same");
  h1->Draw("same");
  
  aCan->cd();
  norm->SetTitle(("run"+runnum));
  
  leg->AddEntry(norm,"All the rest");
  leg->AddEntry(h1,"COntrolled by the DT with eff. > 98%");
  leg->AddEntry(h2,"Controlled by DT-ALL");
  leg->AddEntry(h3,"Not controlled");
  leg->AddEntry(h4,"Single gap mode Barrel");
  leg->AddEntry(h5,"Single gap mode Endcap");
  leg->SetTextFont(10);
  leg->SetTextSize(5);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();
  
  aCan->SaveAs("tryme.root");
  
}

void DeadVsTotalRunTime(string FileWithDead, string FileWithRunTime, string GOODSstring, string outputdirAndSufix,int minLS,int bins){
  
  // introduce 
  
  cout << "before" << endl;
  DataObject GUD(GOODSstring);
  cout << "after"  << endl;
  map<string,string> GUUUD;
  map<string,int> dead_vs_run;
  
  for(int i = 1 ; i <= GUD.getLenght();i++){
    GUUUD[GUD.getElement(i,1)] =GUD.getElement(i,1) ;
  }
  
  DataObject Dead(FileWithDead);
  DataObject Time(FileWithRunTime);
  int deadStrips=0,nLSdelivered=0,nLSrecorded = 0 ,topDead = 0,topLS = 0;
  double timeInSecsDelivered = 0,timeInSecsRecorded = 0 ;
  
  for (int i = 1 ; i <= Time.getLenght() ; i++) {
    
    if(Time.getElementAsInt(i,2) > minLS ){
      dead_vs_run[Time.getElement(i,1)] = Dead.getElementByKeyAsInt(Time.getElement(i,1),1);
    }
    
    if (Time.getElementAsInt(i,2) > topLS){topLS = Time.getElementAsInt(i,2);}
    //cout << topLS << endl;
    if (Dead.getElementByKeyAsInt(Time.getElement(i,1),1) > topDead && Time.getElementAsInt(i,2) > minLS) {topDead = Dead.getElementByKeyAsInt(Time.getElement(i,1),1);}
    //the line above cause element not found and therefore segfault
    //topDead = 2000; 
    
  } 
  
  //topDead = 2000;
  
  cout << "brake 3" << endl;
  
  TH2F * histDelivered = new TH2F("Delivered","Dead strips vs delivered LS",200,0,topLS+10,200,0,topDead+10);
  TH2F * histRecorded = new TH2F("Recorder","Dead strips vs recorded LS",200,0,topLS+10,200,0,topDead+10);
  TH1F * histDeliveredDistribution = new TH1F("DistributionDelivered","Distribution of number of dead strips",bins,500,topDead+200);
  TH1F * hist_RunVsDead = new TH1F("DeadVsRNum","Dead strips vs run number",dead_vs_run.size(),0,dead_vs_run.size());
  
  histDelivered->SetStats(NO);
  histRecorded->SetStats(NO);
  histDeliveredDistribution->SetStats(NO);
  hist_RunVsDead->SetStats(NO);
  
  cout << "size of run vs dead map is " << dead_vs_run.size() << endl;
  
  // omg  'run vs dead' sounds like horor movie :)//
  
  
  TCanvas * can1 = new TCanvas ("can1","the title",1200,700);
  TCanvas * can2 = new TCanvas ("can2","the title",1200,700);
  TCanvas * can3 = new TCanvas ("can3","the title",1200,700);
  TCanvas * can4 = new TCanvas ("can4","the title",1200,700);
    
  
  
  
  for (int i = 1 ; i <= Time.getLenght() ; i++){
    
    deadStrips = Dead.getElementByKeyAsInt(Time.getElement(i,1),1);
    nLSdelivered = Time.getElementAsInt(i,2);
    nLSrecorded = Time.getElementAsInt(i,3);
    
    if(nLSdelivered > 1200 && deadStrips > 1700){
      cout << Time.getElement(i,1) << endl;
    }
    
    if( GUUUD.find(Time.getElement(i,1)) != GUUUD.end() && nLSdelivered > minLS ){
    
    histDelivered->Fill(nLSdelivered,deadStrips);
    histRecorded->Fill(nLSrecorded,deadStrips);
    
    
    histDeliveredDistribution->Fill(deadStrips);
    }
  }
  
  int binctr = 1;
  
  for(map<string,int>::iterator itr = dead_vs_run.begin() ; itr != dead_vs_run.end() ; itr++){
    
    
    hist_RunVsDead->SetBinContent(binctr,itr->second);
    hist_RunVsDead->GetXaxis()->SetBinLabel(binctr,itr->first.c_str());
    cout << itr->first << " " << itr->second << endl;
    binctr++; // bin counter
    
  }
  
  
  histDelivered->SetMarkerColor(kRed);
  histDelivered->SetMarkerStyle(kFullCircle);
  histDelivered->SetYTitle("Number of dead strips");
  histDelivered->SetXTitle("Number of delivered lumi sections");
  
  histRecorded->SetMarkerColor(kBlue);
  histRecorded->SetMarkerStyle(kFullSquare);
  histRecorded->SetYTitle("Number of dead strips");
  histRecorded->SetXTitle("Number of recorded lumi sections");

  histDeliveredDistribution->SetFillColor(kRed);
  histDeliveredDistribution->SetLineColor(kRed);
  histDeliveredDistribution->SetYTitle("Number of entries");
  histDeliveredDistribution->SetXTitle("Number of dead strips");
  
  hist_RunVsDead->SetLineColor(kBlue);
  hist_RunVsDead->SetFillColor(kBlue);
  hist_RunVsDead->SetXTitle("Run number");
  hist_RunVsDead->SetYTitle("Number of dead strips - total");
  
  setTDRStyle();
  
  
  can1->cd();
  histDelivered->Draw();
  can1->SaveAs((outputdirAndSufix+"_delivered.root").c_str());
  can1->SaveAs((outputdirAndSufix+"_delivered.png").c_str());
  can2->cd();
  histRecorded->Draw();
  can2->SaveAs((outputdirAndSufix+"_recorded.root").c_str());
  can2->SaveAs((outputdirAndSufix+"_recorded.png").c_str());
  can3->cd();
  histDeliveredDistribution->Draw();
  can3->SaveAs((outputdirAndSufix+"_delivered_distr.root").c_str());
  can3->SaveAs((outputdirAndSufix+"_delivered_distr.png").c_str());
  can4->cd();
  hist_RunVsDead->Draw();
  can4->SaveAs((outputdirAndSufix+"_run_vs_dead.root").c_str());
  can4->SaveAs((outputdirAndSufix+"_run_vs_dead.png").c_str());
  
  TFile * resultFile = new TFile((outputdirAndSufix+"_savedHistos.root").c_str(),"NEW");
  if(resultFile->IsOpen()){
    
    hist_RunVsDead->Write((outputdirAndSufix+hist_RunVsDead->GetName()).c_str());
    histDelivered->Write((outputdirAndSufix+histDelivered->GetName()).c_str());
    histDeliveredDistribution->Write((outputdirAndSufix+histDeliveredDistribution->GetName()).c_str());
    histRecorded->Write((outputdirAndSufix+histRecorded->GetName()).c_str());
    
  }
  
  resultFile->Close("R");
  
  
}


void print_all_the_strips_for_runlist(string folder,string newTitle,string runlist,string rollList,string exclusionsList,string area,string LumiFile,string Masked_Dead_folder,string minOnX,string maxOnX){
  
  /**
   * Try to loop for strips that are constantly with low value even if the ()
   * 1. luminosity is high ()
   * 2. The neighbours strips are occupied (implement method to search for the neighbours)
   * 3. They have never been masked (at least one time as masked reject them)
   * 4. Their rate is not exactly zero but some value that is relatively 
   * 5. Draw distribution for each strip with its value 
  
  
  * - Start with collecting strips with small rate values in maps
  * - Then start compare conditions
  * - once counted keep the list and check for new 'dead' strips to enter the lists
  * - Use map of the stopped chambers to reject them
  */
  
  
  DataObject area_cmssw(area);
  DataObject runl(runlist);
  DataObject all_rolls(rollList);
  DataObject Lumi(LumiFile);
  DataObject exclList(exclusionsList);
  map<string,string> exList;
  
  string r_name;
  map<string,ExRoll*> rolls_map;
  map<string,DataObject*> maskedStripsMap;
  map<string,DataObject*> deadStripsMap;
  map<int,double> run_lumi_map;
  
  int  firstInt=1000,secondInt=2000,thirdInt=4000,forthInt=7000;
  int runsCounterOne =0 ,runsCounterTwo = 0 , runsCounterThree = 0 , runsCounterFour = 0;
  int currentLumi=0;
  
  ExRoll * an_roll;ExStrip * an_strip;
  
  for(int i=1;i<=all_rolls.getLenght();i++){
    r_name = all_rolls.getElement(i,1);
    ExRoll * aroll = new ExRoll(r_name);
    aroll->setStripsAreaFromSource_cmsswResource(area_cmssw);
    rolls_map[r_name] = aroll;
  }
  
  string curr_run="";
  
  for(int i=1;i <= runl.getLenght();i++){
    curr_run = runl.getElement(i,1);
    maskedStripsMap[curr_run] = new DataObject(Masked_Dead_folder+"Masked_"+curr_run+".txt",2);
    deadStripsMap[curr_run] = new DataObject(Masked_Dead_folder+"Dead_"+curr_run+".txt",2);
  }
  
  for(int i=1; i <= Lumi.getLenght();i++){
    run_lumi_map[Lumi.getElementAsInt(i,1)] = (Lumi.getElementAsDouble(i,3))/(Lumi.getElementAsDouble(i,2)*23.31);
  }
  
  // fill the exclusion map
  
  for(int i=1;i<= exclList.getLenght();i++){
    exList[exclList.getElement(i,1)] = exclList.getElement(i,1);
  }
  
  TH1F  * h1;
  double buffer=0,area_of_strip_in_clone=0,result_rate_RCS=0;
  int nbx=0,counter_of_zeros=0,counter_of_zeros_all=0,counter_masked_only=0,counter_dead=0;
  
  std::string currentName,filename;
  TH1F * aHist = new TH1F(newTitle.c_str(),newTitle.c_str(),100000,atof(minOnX.c_str()),atoi(maxOnX.c_str()));
  TH1F * one = new TH1F("one","one",100000,atof(minOnX.c_str()),atoi(maxOnX.c_str()));
  TH1F * two = new TH1F("two","two",100000,atof(minOnX.c_str()),atoi(maxOnX.c_str()));
  TH1F * three = new TH1F("three","three",100000,atof(minOnX.c_str()),atoi(maxOnX.c_str()));
  TH1F * four = new TH1F("four","four",100000,atof(minOnX.c_str()),atoi(maxOnX.c_str()));
  
  TKey * TheKey;
  DataObject * pointerMask, * pointerDead;
  
  for (int k=1;k <=runl.getLenght();k++){
  curr_run = runl.getElement(k,1);
  filename = folder+"total_"+runl.getElement(k,1)+".root";
  // element K1 ... browse internet for K1 :D 
  currentLumi = run_lumi_map.find(runl.getElementAsInt(k,1))->second;
  
  
  if(exList.find(curr_run) == exList.end()){
  
  TFile * rFile = new TFile(filename.c_str(),"READ","in");
  
  cout << filename <<  " " << currentLumi << endl;
  
  if(rFile->IsOpen()  &&  exList.find(curr_run) == exList.end() ){
    
    pointerMask = maskedStripsMap.find(curr_run)->second;
    pointerDead = deadStripsMap.find(curr_run)->second;
    TIter nextkey( rFile->GetListOfKeys());
    while (TheKey = (TKey*)nextkey()) {
      h1 = (TH1F*)TheKey->ReadObj();
      currentName = h1->GetName();
      
      if( rolls_map.find(currentName) != rolls_map.end() ){

	an_roll = rolls_map.find(currentName)->second;
	an_roll->ClearStripsConditions();
	an_roll->setMaskedStripsFromSource(*pointerMask);
	an_roll->setDeadStripsFromSource(*pointerDead);
	an_roll->setStripsRatesFromTH1FObject(h1);

	for(int i = 1;i<= an_roll->getClones();i++){

	  area_of_strip_in_clone = an_roll->getStripsAreaFromClone(i);
	  
	  for(int j=an_roll->getFirstStripNumberOfClone(i);j<=an_roll->getLastStripNumberOfClone(i);j++){
	    an_strip = an_roll->getStrip(j);
	    result_rate_RCS = an_strip->getRate()/area_of_strip_in_clone;
	    if(! an_strip->getIsDisconnected() && result_rate_RCS == 0){
	      counter_of_zeros_all++;
	    }
	    if(! an_strip->getIsDisconnected() && an_strip->getIsMasked() == 1){
	      counter_masked_only++;
	    }
	    if(! an_strip->getIsDisconnected() && an_strip->getIsDead()){
	      counter_dead++;
	    }
	    
	    if(! an_strip->getIsMasked() && ! an_strip->getIsDisconnected()){
	    //if(! an_roll->getStrip(j)->getIsMasked() && ! an_strip->getIsDead() && ! an_strip->getIsDisconnected()){
	      
	      // do what is wanted here
	      if(currentLumi <= firstInt){
		runsCounterOne++;
		one->Fill(result_rate_RCS);
	      }
	      
	      if(currentLumi > firstInt && currentLumi <= secondInt ){
		two->Fill(result_rate_RCS);
		runsCounterTwo++;
	      }
	      
	      if( currentLumi  > secondInt && currentLumi <= thirdInt){
		three->Fill(result_rate_RCS);
		runsCounterThree++;
	      }
	      
	      if(currentLumi > thirdInt){
		four->Fill(result_rate_RCS);
		runsCounterFour++;
	      }
	      
	      if(result_rate_RCS > 0){
		//cout << curr_run << " " << currentName << " " << an_strip->getOnlineNumber() << endl;
	      }
	      
	      if(result_rate_RCS == 0){
		counter_of_zeros++;
	      }
	      
	      aHist->Fill(result_rate_RCS);
	      
	    }
	  }
	}
      }
      delete h1;
    }
    // ptrs mskd dead to be deleted here
  }
  
  rFile->Close("R");
  rFile->Delete();
  
  
  }
  
  }
  
  TLegend * leg;
  leg = new TLegend(0.218063,0.606759,0.404552,0.93226);
  leg->AddEntry(one,"0 - 1000 10^{30} cm^{-2} s^{-1}","lp");
  leg->AddEntry(two,"1000 - 2000 10^{30} cm^{-2} s^{-1}","lp");
  leg->AddEntry(three,"2000 - 4000 10^{30} cm^{-2} s^{-1}","lp");
  leg->AddEntry(four,"4000 - 7000 10^{30} cm^{-2} s^{-1}","lp");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  
  setTDRStyle();
  
  
  TCanvas * merged = new TCanvas("merged","merged",1200,700);
  
  aHist->SetFillColor(kCyan);
  aHist->SetLineColor(kCyan);
  
  aHist->GetYaxis()->SetTitle("Number of entries");
  aHist->GetXaxis()->SetTitle("Rate (Hz/cm^{2})");
  aHist->SetTitle("Strips rate distribution 2012 JSON");
  newTitle += ".root";
  aHist->SetStats(NO);
  aHist->SaveAs(newTitle.c_str());
  
  cout << "All the zero entries without entries from masked or dead are " <<  counter_of_zeros << endl;
  cout << "All the zero entries are " << counter_of_zeros_all << endl;
  cout << "All masked strips are : " << counter_masked_only << endl;
  cout << "All dead strips are : " << counter_dead << endl;
  
  merged->cd();
  one->SetFillColor(kWhite);
  one->SetLineColor(kRed);
  one->SetStats(NO);
//   one->Scale(runsCounterOne);
  two->SetLineColor(kGreen);
  two->SetFillColor(kWhite);
  two->SetStats(NO);
//   two->Scale(runsCounterTwo);
  
  three->SetFillColor(kWhite);
  three->SetLineColor(kBlue);
  three->SetStats(NO);
//   three->Scale(runsCounterThree);
  four->SetFillColor(kWhite);
  four->SetLineColor(kBlack);
  four->SetTitle("Distribution of strip rates ");
  four->SetStats(NO);
//   four->Scale(runsCounterFour);
  four->GetXaxis()->SetTitle("Rate (Hz/cm^{2})");
  four->GetYaxis()->SetTitle("Number of entries");
  
  four->Draw();
  three->Draw("same");
  two->Draw("same");
  one->Draw("same");
  leg->Draw();
  newTitle = "merged_"+newTitle;
  merged->SaveAs(newTitle.c_str());
  
  one->SaveAs("one.root");
  two->SaveAs("two.root");
  three->SaveAs("three.root");
  four->SaveAs("four.root");
  
}

void print_map_of_online_offline_rolls_and_rollNumbericIDS(string allOnlineRolls, string RAW_IDS){
  
  ExRoll * oneRoll ;
  DataObject rawIDsource(RAW_IDS);
  DataObject rolls(allOnlineRolls);
  string currentLBname;
  
  for(int i = 1; i <= rolls.getLenght() ; i++){ /** loop on all 1232 link boards */
    currentLBname = rolls.getElement(i,1);
    
    oneRoll = new ExRoll(currentLBname);
    oneRoll->allocStrips();
    oneRoll->initStrips();
    oneRoll->setRollRawIDfromSource(RAW_IDS);
    
    for (int j = 1; j <= oneRoll->getClones() ; j++){ /** loop on the number of the sub-rolls of this link board*/
      
      cout << oneRoll->getRawIDofClone(j+1) << " ";
      cout << oneRoll->getFullOnlineRollID() << " ";
      cout << oneRoll->getRollIDofCloneWithNewIdentifiers(j) << endl;
      
    }
    
    //delete oneRoll;
    
  }
  
  
}


void getInstLum(string filename){
  
  DataObject Lum(filename);
  ofstream IFS;
  string newfile = "theOut_"+filename;
  IFS.open(newfile.c_str());
  string buffer;
  
  for (int i = 1 ; i <= Lum.getLenght() ; i++){
    buffer = Lum.getElement(i,1);
    IFS << buffer << " " << (Lum.getElementAsDouble(i,3))/(Lum.getElementAsDouble(i,2)*23.31) << "\n";
    cout << buffer << " " << Lum.getElement(i,2) << " " << (Lum.getElementAsDouble(i,3))/(Lum.getElementAsDouble(i,2)*23.31) << "\n";
    
    IFS.clear();
  }
  
  IFS.close();
  
}

int IsCorruptedFile(string theFile){
  int exit_code=0;
  
  TFile * myFile = new TFile(theFile.c_str(),"READ","in");
  if(myFile->IsZombie()){
    exit_code = EXIT_FAILURE;
    cout << " File is corrupted " << endl;
  }
  else if (!myFile->IsOpen()){
    exit_code = 2;
    cout << "File does not exist" << endl;
  }
  return exit_code;
  
}

void drawDifferentRunDurationLumies(const string& filename){
  
  DataObject lumifile(filename);
  TH1F * normalDuration = new TH1F("","",lumifile.getLenght(),0,lumifile.getLenght());
  TH1F * halfHour = new TH1F("","",lumifile.getLenght(),0,lumifile.getLenght());
  TH1F * oneHour = new TH1F("","",lumifile.getLenght(),0,lumifile.getLenght());
  TCanvas * picassoDrawing = new TCanvas("obj name","obj title",1200,700);
  
  for (int i = 0 ; i < lumifile.getLenght() ; i++){
    
    if(i ==0 || i%5 == 0){
      oneHour->GetXaxis()->SetBinLabel(i+1,lumifile.getElement(i+1,1).c_str());
    }
    
    normalDuration->SetBinContent(i+1,lumifile.getElementAsDouble(i+1,4));
    halfHour->SetBinContent(i+1,lumifile.getElementAsDouble(i+1,3));
    oneHour->SetBinContent(i+1,lumifile.getElementAsDouble(i+2,2));
    
  }
  
  oneHour->GetXaxis()->SetTitle("Run number");
  oneHour->GetYaxis()->SetTitle("Instantaneous luminosity (10^{30} cm^{-2} s^{-1})");  
  normalDuration->SetLineColor(kRed);
  halfHour->SetLineColor(kBlue);
  oneHour->SetLineColor(kGreen-2);  
  picassoDrawing->cd();  
  setTDRStyle();
  oneHour->GetXaxis()->SetTitleOffset(oneHour->GetXaxis()->GetTitleOffset()*2);
  oneHour->GetYaxis()->SetTitleOffset(oneHour->GetXaxis()->GetTitleOffset()*1.2);
  oneHour->SetTitle("Inst. Luminosity one hour(green), half hour(blue), full run()");
  normalDuration->SetStats(NO);
  halfHour->SetStats(NO);
  oneHour->SetStats(NO);
  oneHour->Draw();
  halfHour->Draw("same");
  normalDuration->Draw("same");  
  picassoDrawing->SaveAs("drawing.root");
  
}

void getCorrectedChambersRateDistribution(const string & runlist,const string & runsuffix, const string & areaFile, const string & outputFolder){
  
  
  DataObject areaObj(areaFile);
  DataObject rlist(runlist);
  
  for (int i =0 ; i < rlist.getLenght() ; i++ ){
  
  string runnum = rlist.getElement(i+1,1);
  string fileInput=runsuffix+runnum+".root";
  
  TFile * input = new TFile(fileInput.c_str(),"READ");
  TH1F * h1;
  TIter nextkey( input->GetListOfKeys() );
  TKey *key;
  TObject *obj;
  
  
  
  TH1F * barrelRate = new TH1F ("br","br",6000,0,60);
  TH1F * EcapRate = new TH1F ("er","er",6000,0,60);
  TH1F * totalRate = new TH1F ("tr","tr",6000,0,60);
  TH1F * RE4Plus = new TH1F ("re4","re4",6000,0,60);
  TH1F * EcapPartition = new TH1F ("part","part",6000,0,60);
  TH1F * barrelmtwo = new TH1F ("bmtwo","bmtwo",6000,0,60);
  TH1F * barrelmone = new TH1F ("bmone","bmone",6000,0,60);
  TH1F * barrelzero = new TH1F ("bz","bz",6000,0,60);
  TH1F * barrelpone = new TH1F ("bpo","bpo",6000,0,60);
  TH1F * barrelptwo = new TH1F ("bpt","bpt",6000,0,60);
  TH1F * REp1 = new TH1F ("rep1","rep1",6000,0,60);
  TH1F * REm1 = new TH1F ("rem1","rem1",6000,0,60);
  TH1F * REp2 = new TH1F ("rep2","rep2",6000,0,60);
  TH1F * REm2 = new TH1F ("rem2","rem2",6000,0,60);
  TH1F * REp3 = new TH1F ("rep3","rep3",6000,0,60);
  TH1F * REm3 = new TH1F ("rem3","rem3",6000,0,60);
  TH1F * REp4 = new TH1F ("rep4","rep4",6000,0,60);
  TH1F * REm4 = new TH1F ("rem4","rem4",6000,0,60);
  
  
  
  while ((key = (TKey*)nextkey()) != NULL) {
    
    h1 = dynamic_cast<TH1F*>(key->ReadObj());
    string rollName ;
    double chamberAvgRateCm2;
    rollName = h1->GetName();
    
    if (rollName.substr(0,1) == "W" || rollName.substr(0,2) == "RE" ){
      
      if (rollName.find("RE") != string::npos && rollName.size() == 8){
	rollName = rollName.substr(0,7) + "0" + rollName.substr(7,1);
      }
      
      //cout << rollName << endl;
      
      ExRoll * aRoll = new ExRoll(rollName);
      aRoll->allocStrips();
      aRoll->initStrips();
      
      aRoll->setStripsRatesFromTH1FObject(h1);
      aRoll->setStripsAreaFromSource_cmsswResource(areaObj);
      
      //cout << aRoll->getFullOnlineRollID() << " ";// << aRoll->getAvgRatePSCWithoutCorrections() << " ";
      aRoll->removeNoisyStripsForAllClonesWithPercentValue(100);
      for (int i = 0 ; i < 96 ; i++){
	if( aRoll->getStrip(i+1)->getRate() > 500 ) { 
	  aRoll->getStrip(i+1)->setRate(0);
	}
      }
      
      //cout << aRoll->getAvgRatePSCWithoutCorrections() << endl ;
      double correctedRate = aRoll->getAvgRatePSCWithoutCorrections() ;
      double correctedPartRate = 0;
      
      if (rollName.find(runnum) != string::npos){
	for (int part = 0 ; part < aRoll->getClones() ; part++){
	  correctedPartRate = aRoll->getAvgRatePSCWithoutCorrectionsForClone(part+1);
	  if (correctedPartRate > 0){
	    EcapPartition->Fill(correctedPartRate);    
	  }
	}
      }
      
            
      if (correctedRate > 0){
	totalRate->Fill(correctedRate);
	if ( rollName.substr(0,1) == "W"){
	  // use a map now its so ugly
	  
	  barrelRate->Fill(correctedRate);
	  if ( rollName.substr(1,1) == "0" ) barrelzero->Fill(correctedRate);
	  if( rollName.substr(1,2) == "-2" ) barrelmtwo->Fill(correctedRate);
	  if ( rollName.substr(1,2) == "-1" ) barrelmone->Fill(correctedRate);
	  if ( rollName.substr(1,2) == "+1" ) barrelpone->Fill(correctedRate);
	  if ( rollName.substr(1,2) == "+2" ) barrelptwo->Fill(correctedRate);
	  
	}
	else {
	  EcapRate->Fill(correctedRate);
	  if (rollName.find("-1") != string::npos) REm1->Fill(correctedRate);
	  if (rollName.find("-2") != string::npos) REm2->Fill(correctedRate);
	  if (rollName.find("-3") != string::npos) REm3->Fill(correctedRate);
	  if (rollName.find("-4") != string::npos) REm4->Fill(correctedRate);
	  if (rollName.find("+1") != string::npos) REp1->Fill(correctedRate);
	  if (rollName.find("+2") != string::npos) REp2->Fill(correctedRate);
	  if (rollName.find("+3") != string::npos) REp3->Fill(correctedRate);
	  if (rollName.find("+4") != string::npos) REp4->Fill(correctedRate);
	  
	}
      }
      
      delete aRoll;
      
    }
  }
  
  totalRate->SaveAs((outputFolder+"total_"+runnum+"_corrected.root").c_str());
  EcapRate->SaveAs((outputFolder+"ecap_"+runnum+"_corrected.root").c_str());
  barrelRate->SaveAs((outputFolder+"barrel_"+runnum+"_corrected.root").c_str());
  //EcapPartition->SaveAs((chamberSuffix+"_dontuseanymore.root").c_str());
  
  /*
  barrelzero->SaveAs((outputFolder+"W0_"+runnum+".root").c_str());
  barrelmtwo->SaveAs((outputFolder+"W-2_"+runnum+".root").c_str());
  barrelmone->SaveAs((outputFolder+"W-1_"+runnum+".root").c_str());
  barrelpone->SaveAs((outputFolder+"W+1_"+runnum+".root").c_str());
  barrelptwo->SaveAs((outputFolder+"W+2_"+runnum+".root").c_str());
  REm1->SaveAs((outputFolder+"RE-1_"+runnum+".root").c_str());
  REm2->SaveAs((outputFolder+"RE-2_"+runnum+".root").c_str());
  REm3->SaveAs((outputFolder+"RE-3_"+runnum+".root").c_str());
  REm4->SaveAs((outputFolder+"RE-4_"+runnum+".root").c_str());
  REp1->SaveAs((outputFolder+"RE+1_"+runnum+".root").c_str());
  REp2->SaveAs((outputFolder+"RE+2_"+runnum+".root").c_str());
  REp3->SaveAs((outputFolder+"RE+3_"+runnum+".root").c_str());
  REp4->SaveAs((outputFolder+"RE+4_"+runnum+".root").c_str());
  */ 
  
  delete totalRate;
  delete EcapRate;
  delete barrelRate;
  delete barrelzero;
  delete barrelmtwo;
  delete barrelmone;
  delete barrelpone;
  delete barrelptwo;
  delete REm1;
  delete REm2;
  delete REm3;
  delete REm4;
  delete REp1;
  delete REp2;
  delete REp3;
  delete REp4;
  
  input->Close("R");
  
  }

}

void addZeroSuffixForlbMap(const string & filename){
  
  ifstream IFS;
  IFS.open(filename.c_str());
  
  string LINE,chamber;
  
  while (getline(IFS,LINE)){
    
    if ( LINE.find("RE-") != string::npos || LINE.find("RE+") != string::npos ){
      
      size_t posstart = LINE.find(" RE") + 1;
      size_t length = LINE.size() - posstart;
      
      chamber = LINE.substr(posstart,length);  
      string therest = LINE.substr(0,posstart);
      
      //cout << LINE << endl;
      int sizeis = chamber.size();
      
      //cout << chamber << " ";
      if (sizeis == 9){
	//cout << chamber << " ";
	chamber = chamber.substr(0,7) + "0" + chamber.substr(7,1);
      }
      cout << therest << chamber << endl;
      IFS.clear();
    }
    
    else {
      cout << LINE << endl;
    }
    
  }
  
  
}

void printTowerRE4(const string & towerRooFile){
  
  
  TFile * rfile = new TFile(towerRooFile.c_str(),"READ");
  string towerName;
  if(towerRooFile.find("far") != string::npos){
    towerName = towerRooFile.substr(6,8);
  }
  else {
    towerName = towerRooFile.substr(6,9);
  }
  TIter nextkey( rfile->GetListOfKeys() );
  TKey *key;
  TObject *obj;
  string currentName;
  while ((key = (TKey*)nextkey()) != NULL) {
     currentName = key->GetName();
     if (currentName.size() == 8){
       currentName = currentName.substr(0,7) + "0" + currentName.substr(7,1);
    }
     
     if (currentName.substr(0,4) == "RE-4" || currentName.substr(0,4) == "RE+4"){
       cout << currentName << " " << towerName << endl;
    }
  }
  
  rfile->Close("R");
  rfile->Delete();
}

void DeadMaskedInactivePlot(const string & runlist_input_file,const string & runs_dates_file ,const int & labelModulo, const string & xAxisTitle,const string & dataFolder){
  
  TPaveText * ptext = new TPaveText(0.406627,0.914685,0.608434,0.986014,"NDC");
  ptext->SetFillColor(0);
  ptext->SetBorderSize(0);
  ptext->SetTextSize(0.08);
  ptext->AddText("CMS Preliminary");
  ptext->SetTextFont(42);
  
  DataObject runlist(runlist_input_file);
  DataObject runs_dates_map(runs_dates_file);
  
  
  
  vector<double> vmasked,vdead,vinactive;
  vector<string> vdates;
  
  int bincounter = 0;
  //int totalNumberOfStrips = 132352;
  int totalNumberOfStrips = 118272;
  
  for (int line = 0 ; line < runlist.getLenght() ; line++){
    
    int runnum = runlist.getElementAsInt(line+1,1);
    if (runnum > 210000 ) totalNumberOfStrips = 132352;
    //cout << runnum << endl;
    
    string runasstring = "run"+runlist.getElement(line+1,1);
    string fileWithDead = dataFolder+runasstring+"/AllDead.txt";
    string fileWithMasked = dataFolder+runasstring+"/AllMasked.txt";
    DataObject * deadStripsObject = new DataObject(fileWithDead);
    DataObject * maskedStripsObject = new DataObject(fileWithMasked);
    
    int mskd = deadStripsObject->getLenght();
    int dead = maskedStripsObject->getLenght();
    int allinactive = mskd+dead;
    
    
    
    double inactivefraction = double(allinactive*100)  / double(totalNumberOfStrips);
    double deadfraction = double(dead*100)  / double(totalNumberOfStrips);
    double maskedfraction = double(mskd*100)  / double(totalNumberOfStrips);
    
    cout << runnum << " masked " << maskedfraction << " dead " << deadfraction << " inactive " << inactivefraction << " " << dead << endl;
    
    if (maskedfraction < 4.5 && deadfraction > 0.5 && inactivefraction < 5){
      
      vmasked.push_back(maskedfraction);
      vdead.push_back(deadfraction);
      vinactive.push_back(inactivefraction);
      vdates.push_back(runs_dates_map.getVectorOfValuesForKey(runlist.getElement(line+1,1)).at(0));
      
    }
    
//     historyPlotMasked->SetBinContent(bincounter+1,maskedfraction);
//     historyPlotDead->SetBinContent(bincounter+1,deadfraction);
//     historyPlotInactive->SetBinContent(bincounter+1,inactivefraction);
//     if ( ! ((bincounter+1) % labelModulo) ){
//       historyPlotInactive->GetXaxis()->SetBinLabel(bincounter+1, runs_dates_map.getVectorOfValuesForKey(runlist.getElement(line+1,1)).at(0).c_str());
//     }
//     bincounter++;
    delete deadStripsObject;
    delete maskedStripsObject;
  }
  
  int numberOfRuns = vmasked.size();
  
  TH1F * historyPlotDead = new TH1F("d","Masked",numberOfRuns,0,numberOfRuns);
  TH1F * historyPlotMasked = new TH1F("m","Dead",numberOfRuns,0,numberOfRuns);
  TH1F * historyPlotInactive = new TH1F("i","Inactive",numberOfRuns,0,numberOfRuns);
  TCanvas * canvas = new TCanvas("c","c",1200,700);
  
  for (int i = 0 ; i < numberOfRuns ; i++){
    
    
    historyPlotMasked->SetBinContent(i+1,vmasked.at(i));
    historyPlotDead->SetBinContent(i+1,vdead.at(i));
    historyPlotInactive->SetBinContent(i+1,vinactive.at(i));
    if ( ! ((i+1) % labelModulo) ){
      historyPlotInactive->GetXaxis()->SetBinLabel(i+1, vdates.at(i).c_str());
    }
    
  }
  
  
  TLegend * leg;
  leg = new TLegend(0.673275,0.679851,0.921439,0.84349);
  leg->SetFillColor(0);
  leg->SetTextSize(0.04);
  leg->SetBorderSize(0);
    
  leg->AddEntry(historyPlotInactive,"Total");
  leg->AddEntry(historyPlotMasked,"Masked");
  leg->AddEntry(historyPlotDead,"Inactive");
  historyPlotInactive->SetLineWidth(1);
  historyPlotMasked->SetLineWidth(1);
  historyPlotDead->SetLineWidth(1);
  
  historyPlotInactive->GetXaxis()->SetTitle(xAxisTitle.c_str());
  historyPlotInactive->GetYaxis()->SetTitle("Inactive channels in %");
  historyPlotInactive->GetYaxis()->SetRangeUser(0,5);
  historyPlotInactive->GetXaxis()->SetTitleOffset(2);
  
  historyPlotInactive->SetLineColor(kBlue);
  historyPlotDead->SetLineColor(kRed);
  historyPlotMasked->SetLineColor(kTeal+4); 
  
  
  canvas->cd();
  historyPlotInactive->Draw();
  historyPlotMasked->Draw("same");
  historyPlotDead->Draw("same");
  leg->Draw();
  ptext->Draw();
  
  canvas->SaveAs("dmi.root");
  
}


void WriteIDsMap(const string & inputRolls,const string & RawIDsFile,const string & areaFile, const string & towerFile, const string & chipsMapFile,const string & outputFile){
  // prints out json like string representing useful rpc roll-to-someID map
  DataObject input(inputRolls);
  DataObject raws(RawIDsFile);
  DataObject area(areaFile);
  
  cout << "{" << endl;
  
  for (int i = 0 ; i < input.getLenght() ; i++){
  
    ExRoll * aroll = new ExRoll(input.getElement(i+1,1));
    aroll->allocStrips();	
    aroll->initStrips();
    aroll->setRollRawIDfromSource(raws);
    aroll->setStripsAreaFromSource_cmsswResource(area);
    
    //aroll->setTowerNameFromMap();
    //aroll->assignChipsFromSource();
    
    cout << "\"" << aroll->getFullOnlineRollID() << "\":{";
    
    for (int clone = 0 ; clone < aroll->getClones(); clone++ ){
      
      cout << endl << "     \"" << aroll->getRollIDofCloneWithNewIdentifiers(clone+1) << "\":{\"channels\":[" ;
      
      for (int chNum = aroll->getFirstStripNumberOfClone(clone+1) ; chNum <= aroll->getLastStripNumberOfClone(clone+1); chNum++ ){
	cout << chNum;
	if (chNum < aroll->getLastStripNumberOfClone(clone+1) ) { cout << ","; }
      }
      
      cout << "],\"strips\":[" ; 
      for (int chNum = aroll->getFirstStripNumberOfClone(clone+1) ; chNum <= aroll->getLastStripNumberOfClone(clone+1) ; chNum ++){
	cout << aroll->getStrip(chNum)->getOfflineNumber();
	if (chNum < aroll->getLastStripNumberOfClone(clone+1) ) { cout << ","; }
      }
      
      cout << "]}";
      if (clone+1 < aroll->getClones() ) { cout << ","; } 
    }
    
    
    cout << endl << "}";
    if ( i+1 < input.getLenght()) { cout << "," << endl; }
    
    delete aroll;
    
  }
  cout <<  "}" << endl;
}

void WriteNoiseScanResults(string rootFile, int appliedHVBarrel, int appliedHVendcap, int runnumber, string outputFile, string area){
  
  
  DataObject areafile(area);
  ofstream output;
  //output.open(outputFile.c_str());
  TH1F * h1;
  TKey *key;
  TObject *obj;
  string chamberName;
  string delimiter = " ";
  
  TFile * rFile = new TFile(rootFile.c_str(),"read","in");
  TIter nextkey( rFile->GetListOfKeys() );
  if(rFile->IsOpen()){
      while (key = (TKey*)nextkey() ) {
	h1 = (TH1F*)key->ReadObj();
	chamberName = h1->GetName();
	if( chamberName.substr(0,1) == "W" ||  chamberName.substr(0,2) == "RE" ) {
	  
	  ExRoll * aroll = new ExRoll(chamberName);
	  aroll->allocStrips();	
	  aroll->initStrips();
	  aroll->setStripsAreaFromSource_cmsswResource(areafile);
	  aroll->setStripsRateFromTH1Source(h1);
	  
	  for (int i = 0 ; i < aroll->getClones() ; i++){
	    cout << aroll->getRollIDofCloneWithNewIdentifiers(i+1) << delimiter;
	    cout << aroll->getRollIDofClone_withEtaPartSeparated(i+1) << delimiter;
	    if (aroll->isInBarrel()){
	      cout << appliedHVBarrel << delimiter;
	    }
	    else {
	      cout << appliedHVendcap << delimiter;
	    }
	    cout << aroll->getAvgRatePSCWithoutCorrectionsForClone(i+1) << endl;
	  }
	  
	  //cout << h1->GetName() << endl;
	  delete aroll;
	}
      }
  }
  
}

void PrintDBIDsvsNewIDs(string inputFile, string dataFile)
{
  
  map<string, string> db_newids;
  
  DataObject chamberIDs(inputFile);
  DataObject d_file(dataFile);
  
  for (int i = 0 ; i < chamberIDs.getLenght() ; i++){
    
    string name = chamberIDs.getElement(i+1,1);
    ExRoll * aRoll = new ExRoll(name);
    aRoll->allocStrips();
    aRoll->initStrips();
    for (int j = 0 ; j < aRoll->getClones() ; j++){
      
      string db_id = aRoll->getRollIDofClone_withEtaPartSeparated(j+1);
      string new_id = aRoll->getRollIDofCloneWithNewIdentifiers(j+1);
      db_newids[db_id] = new_id;
    }
    delete aRoll;
  }
  
  for (int i = 0 ; i < d_file.getLenght() ; i++){
    
    string db_id = d_file.getElement(i+1,1) + " " + d_file.getElement(i+1,2);
    cout << db_newids.find(db_id)->second << " " << d_file.getElement(i+1,3) << " " << d_file.getElement(i+1,4) << endl;    
    
  }
  
}


void WriteRateVsLumiPerRollFile(string& LumiFile, string& rootFilesFolder, string& area, string& outputFile, string& rollNamesAdditionalSuffix){
  
  map<string,double> run_lumi_map;
  DataObject LumiDO(LumiFile);
  DataObject areaDO(area);
  double biggestOn_X=0;
  double biggestOn_Y=0;
  map<string, map<double, double> > rollID_LumiToRateMap;
  map<string, double> rollID_highestRateValue;
  
  for (int i = 0 ; i < LumiDO.getLenght() ; i++) {
    string run_num = LumiDO.getElement(i+1,1);
    double currentLumi = (LumiDO.getElementAsDouble(i+1,3)/LumiDO.getElementAsDouble(i+1,2))/23.31;    
    run_lumi_map[run_num] = currentLumi;
    if(currentLumi > biggestOn_X) biggestOn_X = currentLumi;
    cout << "reading run " + run_num << endl;
    TFile * rFile = new TFile((rootFilesFolder+"total_"+run_num+".root").c_str(),"READ");
    if (! rFile->IsOpen()) continue; 
    
    TIter nextkey( rFile->GetListOfKeys() );
    TKey *key;
    TObject *obj;
        
    while (key = (TKey*)nextkey()) {
      
      obj=key->ReadObj();
      string chName = obj->GetName();
      
      if( ! ( chName.substr(0,1) == "W" ||  chName.substr(0,2) == "RE" ) ) continue;
      TH1F * chamberHisto = dynamic_cast<TH1F*>(obj);
      ExRoll * aroll = new ExRoll(chamberHisto->GetName());
      aroll->allocStrips();
      aroll->initStrips();
      aroll->setStripsAreaFromSource_cmsswResource(areaDO);
      aroll->setStripsRatesFromTH1FObject(chamberHisto);
      
      aroll->removeNoisyStripsForAllClonesWithPercentValue(100);
      
      for(int c = 0; c < aroll->getClones(); c++){
	string rollName = aroll->getRollIDofCloneWithNewIdentifiers(c+1);
	if(rollID_LumiToRateMap.find(rollName) == rollID_LumiToRateMap.end()) { 
	  map<double, double> LumiToRateMap;
	  rollID_LumiToRateMap[rollName] = LumiToRateMap ; 
	  rollID_highestRateValue[rollName] = 0;
	}
	
	double rateInRoll = aroll->getAvgRatePSCWithoutCorrectionsForClone(c+1);
	
	rollID_LumiToRateMap[rollName][currentLumi] = rateInRoll;
	if ( rateInRoll >  rollID_highestRateValue.at(rollName)) rollID_highestRateValue[rollName] = rateInRoll;
	
      }
      
      delete aroll;
      
    }
    
    rFile->Close("R");
    rFile->Delete();
  }
  
  TFile * slopesFile = new TFile(outputFile.c_str(),"UPDATE");
  slopesFile->cd();
  
  for(map<string, map<double, double> >::iterator rID_lToRateMapIter = rollID_LumiToRateMap.begin(); rID_lToRateMapIter != rollID_LumiToRateMap.end() ; rID_lToRateMapIter++){
    
    string rollName = rID_lToRateMapIter->first;
    int mapsize = rID_lToRateMapIter->second.size();
    
    double x[mapsize], y[mapsize];
    int cou = 0;
    for(map<double, double>::iterator lumiRateIter = rID_lToRateMapIter->second.begin(); lumiRateIter != rID_lToRateMapIter->second.end(); lumiRateIter++){
      x[cou] = lumiRateIter->first;
      y[cou] = lumiRateIter->second;
      cou++;
    }
    
    TGraph * graphrollratevslumi = new TGraph(mapsize,x,y);
    graphrollratevslumi->SetMarkerStyle(kFullCircle);
    graphrollratevslumi->SetName((rollName+rollNamesAdditionalSuffix).c_str());
    graphrollratevslumi->SetTitle(rollName.c_str());
    graphrollratevslumi->Write();
    graphrollratevslumi->Delete();
  }
  
  slopesFile->Close();
  
}

void SlopeRatiosComparisonForPairsOfIDs(string & IDs_file, string & inputRootFile, string & outputRootFile, string & twoDmap, string & ShortB, string & ShortE){
  
  
  ptree * mapDescription2D = new ptree;
  
  
  DataObject IDs(IDs_file);  
  DataObject twoDmapDO(twoDmap);
  DataObject BarrelLabels(ShortB);
  DataObject EcapLabels(ShortE);
  
  string resultDir = "results/";
  
  map<string, vector<double>> mapRollToCoordinates;
  for(int ii = 0 ; ii < twoDmapDO.getLenght() ; ii++){
    vector<double> coords;
    coords.push_back(twoDmapDO.getElementAsInt(ii+1,2));
    coords.push_back(twoDmapDO.getElementAsInt(ii+1,3));
    mapRollToCoordinates[twoDmapDO.getElement(ii+1,1)] = coords;
  }
  
  TFile * inputRoot = new TFile(inputRootFile.c_str(),"READ");
  
  vector<string> detectorParts; 
  detectorParts.push_back("RE+4");detectorParts.push_back("RE+3");detectorParts.push_back("RE+2");detectorParts.push_back("RE+1");
  detectorParts.push_back("RE-4");detectorParts.push_back("RE-3");detectorParts.push_back("RE-2");detectorParts.push_back("RE-1");
  detectorParts.push_back("RE+4_R2");detectorParts.push_back("RE+4_R3");detectorParts.push_back("RE-4_R2");detectorParts.push_back("RE-4_R3");
  detectorParts.push_back("RE+3_R2");detectorParts.push_back("RE+3_R3");detectorParts.push_back("RE-3_R2");detectorParts.push_back("RE-3_R3");
  detectorParts.push_back("RE+2_R2");detectorParts.push_back("RE+2_R3");detectorParts.push_back("RE-2_R2");detectorParts.push_back("RE-2_R3");
  detectorParts.push_back("RE+1_R2");detectorParts.push_back("RE+1_R3");detectorParts.push_back("RE-1_R2");detectorParts.push_back("RE-1_R3");
  detectorParts.push_back("W-2");detectorParts.push_back("W-1");detectorParts.push_back("W+0");detectorParts.push_back("W+1");detectorParts.push_back("W+2");
  map<string, vector<double> > detectorPartsMap;
  map<string, vector<double> > detectorPartsMap_HL_LHC;
  
  
  for (string & rName : detectorParts){
    vector<double> ratiosDistrVector;
    detectorPartsMap[rName] = ratiosDistrVector;
    detectorPartsMap_HL_LHC[rName] = ratiosDistrVector;
    
  }
  
  vector<string> SummaryPlotNames;
  SummaryPlotNames.push_back("RE-4");SummaryPlotNames.push_back("RE-3");SummaryPlotNames.push_back("RE-2");SummaryPlotNames.push_back("RE-1");
  SummaryPlotNames.push_back("W-2");SummaryPlotNames.push_back("W-1");SummaryPlotNames.push_back("W+0");SummaryPlotNames.push_back("W+1");SummaryPlotNames.push_back("W+2");
  SummaryPlotNames.push_back("RE+1");SummaryPlotNames.push_back("RE+2");SummaryPlotNames.push_back("RE+3");SummaryPlotNames.push_back("RE+4");
  
  ptree Roll_RatioJSON;
  
  for (int i = 0 ; i < IDs.getLenght() ;  i++){
    
    string firstID = IDs.getElement(i+1,1), secondId = IDs.getElement(i+1,2), resultID = IDs.getElement(i+1,3);    
    TGraph * first = dynamic_cast<TGraph*> (inputRoot->Get(firstID.c_str())), * second = dynamic_cast <TGraph*>( inputRoot->Get(secondId.c_str()));
        
    double min16 = TMath::MinElement(second->GetN(),second->GetX());
    double max16 = TMath::MaxElement(second->GetN(),second->GetX()); 
    double min15 = TMath::MinElement(first->GetN(),first->GetX());
    double max15 = TMath::MaxElement(first->GetN(),first->GetX());    
    double biggestOn_X = (max16 > max15) ? max16 : max15;    
    
    TF1 * f1 = new TF1("f","[0]+x*[1]",0,max15);
    TF1 * f2 = new TF1("f2","[0]+x*[1]",0,max16);
    
    first->Fit(f1,"RQ");
    second->Fit(f2,"RQ");    
    
    double height16 = f2->Eval(biggestOn_X), height15 = f1->Eval(biggestOn_X);
    double height16_HL = f2->Eval(50000), height15_HL = f1->Eval(50000);
    double cf1 = first->GetCorrelationFactor(), cf2 = second->GetCorrelationFactor();
    
    mapRollToCoordinates.at(resultID).push_back(0);    
    first->Delete();
    second->Delete();
    f1->Delete();
    f2->Delete();
    
    double the_ratio = height16/height15;  
    if (height15 == 0 || height16 == 0) the_ratio = 0;
    
    Roll_RatioJSON.put<double>(resultID,the_ratio);
    //if ( cf1 < 0.8 ) continue;    
    
    mapRollToCoordinates.at(resultID).at(2) = the_ratio ;
    
    //cout << max15 << " " << max16 << " " << resultID << " " << the_ratio << " " << height16_HL/ height15_HL << " " << cf1 << " " << cf2  << endl;
    
    for (string & ecapp : detectorParts){
      
      if(resultID.find(ecapp) != string::npos) {
	
	detectorPartsMap.at(ecapp).push_back(the_ratio) ;
	detectorPartsMap_HL_LHC.at(ecapp).push_back(height16_HL / height15_HL) ;

      }            
    }        
  }
  
  ofstream ofsss("RollToRateRatios.json");
  boost::property_tree::json_parser::write_json(ofsss,Roll_RatioJSON);
  ofsss.close();
  
  //exit(0);
  
  inputRoot->Close(); 
  
  
  // put here the JSON with the currents ratios
  ifstream ifs("CurrentsRatiosByRoll.json");
  ptree JSONcurrents;
  try {
    
    boost::property_tree::json_parser::read_json(ifs,JSONcurrents);
  
  }
  catch(const ptree_error &e) {
    cout << e.what() << endl;
  }
  
  // put it in a map, it does some shit otherwise
  map <string, double> RollCurrentsMap;
  for (ptree::iterator iter = JSONcurrents.begin() ; iter != JSONcurrents.end() ; iter++){
    RollCurrentsMap[iter->first] = boost::lexical_cast<double>( JSONcurrents.get_child(iter->first).data() );
  }
  
  
  ifs.clear();
  ifs.close();
  
  map<string, vector<string> > binlabelsmap;
  vector < string> axyses;
  
  for (string & rName : detectorParts){
    
    
    
    cout << rName << endl;
    
    TH2F * hist_ptr;
    TH2F * CurrentsRatios;
    
    if (  rName.size() == 4){ 
      if (binlabelsmap.find(rName) == binlabelsmap.end()) binlabelsmap[rName] = axyses;
      hist_ptr = new TH2F((rName+"_2D").c_str(),rName.c_str(),36,0.5,36.5,6,0,6);
      CurrentsRatios = new TH2F((rName+"_Currents_2D").c_str(),rName.c_str(),36,0.5,36.5,6,0,6);
      
     
      
      for (int ei=1 ; ei <= EcapLabels.getLenght();ei++) {
	hist_ptr->GetYaxis()->SetBinLabel(EcapLabels.getElementAsInt(ei,2),EcapLabels.getElement(ei,1).c_str());
	CurrentsRatios->GetYaxis()->SetBinLabel(EcapLabels.getElementAsInt(ei,2),EcapLabels.getElement(ei,1).c_str());
	
	binlabelsmap.at(rName).push_back(EcapLabels.getElement(ei,1));
      }
      
    }
    else if( rName.size() == 3 || rName.size() == 2 ){
      if (binlabelsmap.find(rName) == binlabelsmap.end()) binlabelsmap[rName] = axyses;
      
      
      hist_ptr = new TH2F((rName+"_2D").c_str(),rName.c_str(),12,0.5,12.5,21,0,21);
      CurrentsRatios = new TH2F((rName+"_Currents_2D").c_str(),rName.c_str(),12,0.5,12.5,21,0,21);
      int labelToSkip = (rName.find("W0") || rName.find("W+1") || rName.find("W-1")) ? 8 : 7;
      for (int bi=1 ; bi <= BarrelLabels.getLenght();bi++) {
	if ( bi == labelToSkip ) continue;
	binlabelsmap.at(rName).push_back(BarrelLabels.getElement(bi,1));
	hist_ptr->GetYaxis()->SetBinLabel(BarrelLabels.getElementAsInt(bi,2),BarrelLabels.getElement(bi,1).c_str());
	CurrentsRatios->GetYaxis()->SetBinLabel(BarrelLabels.getElementAsInt(bi,2),BarrelLabels.getElement(bi,1).c_str());
      }
      
    }
    
    
    else continue;
    
    
    hist_ptr->GetXaxis()->SetTitle("Sector");
    hist_ptr->GetZaxis()->SetTitle("2016/2015 extrapolated rate ratio");
    hist_ptr->SetStats(false);
    
    CurrentsRatios->GetXaxis()->SetTitle("Sector");
    CurrentsRatios->GetZaxis()->SetTitle("2016/2015 extrapolated currents ratio");
    CurrentsRatios->SetStats(false);
    
    
    
    for (map<string, vector<double> >::iterator twodim_itr = mapRollToCoordinates.begin(); twodim_itr != mapRollToCoordinates.end() ; twodim_itr++){
      //cout << twodim_itr->first << endl;
      if(twodim_itr->first.find(rName) != string::npos ){
	vector<double> coordinates = twodim_itr->second;
	
	
	
	hist_ptr->SetBinContent(coordinates.at(0),coordinates.at(1),coordinates.at(2));
	
	//double CurrentRatioValue = 0;
	//if (JSONcurrents.find(twodim_itr->first) != JSONcurrents.not_found()){
	if (RollCurrentsMap.find(twodim_itr->first) != RollCurrentsMap.end() ){
	  //float CurrentRatioValue = JSONcurrents.get<float>(twodim_itr->first);
	  double CurrentRatioValue = RollCurrentsMap.at(twodim_itr->first);
	  cout << twodim_itr->first << endl;
	  CurrentsRatios->SetBinContent(coordinates.at(0),coordinates.at(1),CurrentRatioValue);
	}
	
	cout << rName << " " << coordinates.at(0) << " " << coordinates.at(1)<< " " << coordinates.at(2) << endl;
      }        
    }
    
    TCanvas * canvas = new TCanvas( (rName+"_canvas").c_str() , "can",1200,700);
    TCanvas * canvasCurrents = new TCanvas( (rName+"_canvas_").c_str() , "can",1200,700);
    canvas->cd();
    
    hist_ptr->SetMinimum(0);
    hist_ptr->SetMaximum(2);
    hist_ptr->Draw("COLZ");
    canvas->SaveAs((resultDir+rName+"_2D.root").c_str());
    //string picname = resultDir+ rName+"_2D.png",  macrofile = resultDir + rName+"_2d.C";
    //canvas->SaveAs( picname.c_str());
    //canvas->SaveAs(macrofile.c_str());
    hist_ptr->Delete();
    
    canvasCurrents->cd();
    CurrentsRatios->SetMinimum(0.5);
    CurrentsRatios->SetMaximum(1.8);
    CurrentsRatios->Draw("COLZ");
    //canvasCurrents->SaveAs((resultDir+rName+"_Currents_2D.root").c_str());
    //CurrentsRatios->Delete();
  }  
  

  
  for (map<string, vector<string> >::iterator itr = binlabelsmap.begin(); itr != binlabelsmap.end(); itr++){
    cout << itr->first << endl;
    for ( auto & label : itr->second ) { cout << label << "," ; }
    cout << endl;
  }
  
  
  TH1F * Summary = new TH1F ("Ratios","Ratios summary",13,0,13);
  Summary->SetStats(false);
  Summary->GetXaxis()->SetTitle("Detector part");
  Summary->GetYaxis()->SetTitle("2016 rate increase wrt 2015 in %");
  
  for (map<string, vector<double> >::iterator iter = detectorPartsMap.begin() ; iter != detectorPartsMap.end() ; iter++){
    
    string finalName = resultDir + iter->first + "_ratios.root";
    
    TH1F * hist = new TH1F(iter->first.c_str(),iter->first.c_str(),200,0,2);
    TH1F * hist_hl = new TH1F((iter->first+"_HL").c_str(),iter->first.c_str(),200,0,2);
    for (double & val : iter->second){
      hist->Fill(val);      
    }
    
    for (double & vall : detectorPartsMap_HL_LHC.at(iter->first) ){
      hist_hl->Fill(vall);
    }
    
    hist->GetXaxis()->SetTitle("2016/2015 extrapolated rate ratios");
    hist->GetYaxis()->SetTitle("Number of rolls");
    int namePos = 0;
    for (string & partname : SummaryPlotNames){
      namePos++;
      if (partname == iter->first){
	Summary->SetBinContent(namePos, ( hist->GetMean() - 1 )*100 );
	Summary->GetXaxis()->SetBinLabel(namePos,partname.c_str());
	Summary->SetBinError(namePos,(hist->GetRMS()/hist->GetMean())*100);
	//cout << namePos << endl;
	break;
      }      
    }
    
    hist->SaveAs(finalName.c_str());
    
    string pname = resultDir + iter->first+"_ratios.png";
    string mname = resultDir + iter->first+"_ratios.C";
    hist->SaveAs(pname.c_str());
    hist->SaveAs(mname.c_str());
    
  } 
  
  Summary->SetStats(false);
  Summary->SaveAs("SummaryOnRatios.root");
  
  
}


void PositiveNegativePartsRatio(string& IDs_file, string& inputFile){
  
  // RE4 extrapolated rate vs phi for the endcap + vs - 2016. Print roll ID-to-radians from rate vs phi 
  // Ecap + 2015 vs 2016  
  
}

void GetLumiHistogramPerLS(string& lumiFile){
  
  DataObject Lumi(lumiFile);
  
  TH1F * lumiHisto = new TH1F("","",Lumi.getLenght(),0,Lumi.getLenght());
  
  for (int i = 0 ; i < Lumi.getLenght() ; i++){
    
    double lumiNormalized = Lumi.getElementAsDouble(i+1,1);
    lumiNormalized = lumiNormalized/23.31;
    
    lumiHisto->SetBinContent(i+1,lumiNormalized);
    
  }
  
  lumiHisto->GetXaxis()->SetTitle("Number of lumi section");
  lumiHisto->GetYaxis()->SetTitle("Luminosity /ub ");
  lumiHisto->SaveAs("DetailedLumi.root");
  
}

// get rates in JSON to plot with the universal 2D function

void getRateInJSONformatFromRootFile ( const string& rootfilename, const string& JSONoutFileName, const string& areaFileName ) {
  
  TFile * rootf = new TFile(rootfilename.c_str(),"READ","in");
  DataObject area(areaFileName);
  
  TH1F * h1;
  TIter nextkey( rootf->GetListOfKeys() );
  TKey *key;
  TObject *obj;
  string histoCurrentName;
  
  ptree JSONobj;
  
  while (key = (TKey*)nextkey()) {
    
    obj = key->ReadObj();
    h1 =  dynamic_cast<TH1F*>(obj);
    if (!rootf->IsOpen()) break;
    histoCurrentName = h1->GetName();
    // populate the rolls here
    ExRoll * singleRoll;
    if (histoCurrentName.substr(0,1) == "W" || histoCurrentName.substr(0,2) == "RE") {
      singleRoll = new ExRoll(histoCurrentName);
      singleRoll->setStripsAreaFromSource_cmsswResource(area);
      singleRoll->setStripsRatesFromTH1FObject(h1);
      for (unsigned nclone = 0 ; nclone < singleRoll->getClones() ; nclone++){
	double correctedRate = 0;
	string resultID = singleRoll->getRollIDofCloneWithNewIdentifiers(nclone+1);
	singleRoll->removeNoisyStripsForAllClonesWithPercentValue(100);
	correctedRate = singleRoll->getAvgRatePSCWithoutCorrectionsForClone(nclone+1);
	JSONobj.put<double>(resultID,correctedRate);
      }
    }
    delete singleRoll;
  }
  
  ofstream ofs(JSONoutFileName.c_str());
  boost::property_tree::json_parser::write_json(ofs,JSONobj);
  ofs.clear();ofs.close();
  
  rootf->Close("R");
  

}


// universal 2D


void get2DplotsForJSONFileUsingAndJSONmap(const string & JSONdataFile,const string & JSONmapFile, const int & BarrelMax, const int & EndcapMax, const string & ZaxisTitle,const string & fileSuffixes){

    ptree * JSONdata = new ptree;
    ptree * JSONmap = new ptree;
    ifstream ifs(JSONdataFile.c_str());
    boost::property_tree::json_parser::read_json(ifs,*JSONdata);
    ifs.clear();ifs.close(); ifstream ifss(JSONmapFile.c_str());
    boost::property_tree::json_parser::read_json(ifss,*JSONmap);
    ifss.clear();ifss.close();
    
    for ( ptree::iterator iter = JSONmap->begin() ; iter != JSONmap->end() ; iter++ ){
      cout << iter->first << endl;
      string detectorPart = iter->first;
      if (detectorPart == "W0") continue;
      vector<double> dimensions;
      vector<string> Xlabels;
      vector<string> Ylabels;
      
      BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("Xlabels")) {  Xlabels.push_back(v.second.data()); }
      BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("Ylabels")) {  Ylabels.push_back(v.second.data()); }
      BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("BinsAndDimensions")) { dimensions.push_back(boost::lexical_cast<double>( v.second.data() ) ); }      
      
      TH2F * twoDimensionalHisto = new TH2F(detectorPart.c_str(),detectorPart.c_str(),dimensions.at(0),dimensions.at(1),dimensions.at(2),dimensions.at(3),dimensions.at(4),dimensions.at(5));
      
      bool isEndcap = (detectorPart.find("RE") != string::npos);
      twoDimensionalHisto->SetMinimum(0);
      twoDimensionalHisto->SetMaximum(isEndcap ? EndcapMax : BarrelMax);
      
      for (int ii = 0 ; ii < Xlabels.size() ; ii++){ twoDimensionalHisto->GetXaxis()->SetBinLabel(ii+1, Xlabels.at(ii).c_str()); }
      for (int iii = 0; iii < Ylabels.size() ; iii++){ twoDimensionalHisto->GetYaxis()->SetBinLabel(iii+1, Ylabels.at(iii).c_str()); }
      
      BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("XYcoordinates")) {  
	
	string rollName = v.first.data();
	if (JSONdata->find(rollName) == JSONdata->not_found()) continue;
	double value = JSONdata->get<double>(rollName);
	vector<int> coordinates;
	BOOST_FOREACH(boost::property_tree::ptree::value_type &vv, v.second.get_child("")) {  coordinates.push_back(boost::lexical_cast<double>(vv.second.data())); }
	twoDimensionalHisto->SetBinContent(coordinates.at(0),coordinates.at(1),value);
      }
            
      TCanvas * acan = new TCanvas((detectorPart+"_can").c_str(),(detectorPart+"_can").c_str(),1200,700);      
      acan->cd();
      twoDimensionalHisto->SetStats(kFALSE);
      twoDimensionalHisto->GetXaxis()->SetTitle("Sector");
      twoDimensionalHisto->GetYaxis()->SetTitle("Detector Unit");
      double currentTYoffset = twoDimensionalHisto->GetYaxis()->GetTitleOffset();
      twoDimensionalHisto->GetYaxis()->SetTitleOffset(currentTYoffset + currentTYoffset*( isEndcap ? 0.1 : 0.4));
      twoDimensionalHisto->GetZaxis()->SetTitle(ZaxisTitle.c_str());
      twoDimensionalHisto->GetZaxis()->CenterTitle();
      twoDimensionalHisto->GetYaxis()->CenterTitle();
      twoDimensionalHisto->Draw("COLZ");
      acan->SaveAs((detectorPart+fileSuffixes+".root").c_str());
      twoDimensionalHisto->Delete();
    }
    
    delete JSONdata;
    delete JSONmap;
}

// Universal X vs Y 

void plot_X_vs_Y_values_using_JSON_data_and_JSON_config(const string& JSON_data_file, const string& JSON_config_file){
  
  ifstream ifs(JSON_data_file.c_str());
  ptree JSON_data, JSON_config;
  boost::property_tree::json_parser::read_json(ifs,JSON_data);
  ifs.clear();ifs.close();
  ifs.open(JSON_config_file.c_str());
  boost::property_tree::json_parser::read_json(ifs,JSON_config);
  ifs.clear();ifs.close();
  
  TCanvas * acan = new TCanvas("","",1200,700);
  TH1F *hr = acan->DrawFrame(0,0,6.282,48);
  TLegend * leg;
  leg = new TLegend(0.135095,0.58678,0.352423,0.886329);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(6);
  leg->SetTextSize(30);  
  
  int histosCounter = 0;
  
  acan->cd();
  
  for (ptree::iterator iter = JSON_data.begin() ; iter != JSON_data.end() ; iter++){
    cout << iter->first << endl;
    int counter = 0;
    
    vector<double> lumis, rates;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("values")){
      double val = v.second.get_value<double>();
       if (! (counter % 2) ) {lumis.push_back(val);}
       else {rates.push_back(val);}
       counter++;
    }    
    
    TH2F * h2 = new TH2F((iter->first+"_title").c_str(),(iter->first+"_").c_str(),1000,0,11000,500,0,50);
    h2->SetStats(false);
    if ( iter->second.find("title") != iter->second.not_found())  h2->SetName( iter->second.get_child("title").get_value<string>().c_str() );
    if ( iter->second.find("marker") != iter->second.not_found()) h2->SetMarkerStyle(iter->second.get_child("marker").get_value<int>());
    if ( iter->second.find("color") != iter->second.not_found()) h2->SetMarkerColor(iter->second.get_child("color").get_value<int>());
    if ( JSON_config.find("xaxis") != JSON_config.not_found() ) h2->GetXaxis()->SetTitle(JSON_config.get_child("xaxis").get_value<string>().c_str());
    if ( JSON_config.find("yaxis") != JSON_config.not_found() ) h2->GetYaxis()->SetTitle(JSON_config.get_child("yaxis").get_value<string>().c_str());
    if ( JSON_config.find("title") != JSON_config.not_found() ) h2->SetTitle(JSON_config.get_child("title").get_value<string>().c_str());
    
    for (int ctr = 0 ; ctr < lumis.size() ; ctr ++ ){
      h2->Fill(lumis.at(ctr),rates.at(ctr),3);
    }
    
    if (!histosCounter)  h2->Draw();
    else h2->Draw("same");
    histosCounter++;
    leg->AddEntry(h2,h2->GetName(),"p");
  }
  
  leg->Draw();
  acan->SaveAs("try.root");
  
}


void DrawVvsPhiUsingJSONdataAndJSONconfig(const string& JSON_data_fname, const string& JSON_config_fname){
  
  ptree JSONdata;
  ptree JSONconf;
  ifstream IFS(JSON_data_fname);
  boost::property_tree::json_parser::read_json(IFS,JSONdata);
  IFS.clear();IFS.close();
  IFS.open(JSON_config_fname);
  boost::property_tree::json_parser::read_json(IFS,JSONconf);
  IFS.clear();IFS.close();
  
  TCanvas * acan = new TCanvas("can","vVsPhi",10,10,1200,700);
  TH1F *hr = acan->DrawFrame(0,0,6.282,48);
  int grapCount = 0;
  
  for (ptree::iterator iter = JSONdata.begin() ; iter != JSONdata.end() ; iter++){
    //double values[37],errors[37];
    vector<double> values, errors;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, iter->second.get_child("data")){ values.push_back( v.second.get_value<double>()); }
    BOOST_FOREACH(boost::property_tree::ptree::value_type &er, iter->second.get_child("errors")){ errors.push_back( er.second.get_value<double>()); }
    if (values.size() != errors.size()) continue; // sizes not the same, it's an error
    int nSectors = values.size() ; // number of sectors , 12 for Barrel, 36 for Endcap
    double valuesPhi[nSectors+1], xAxisSize[nSectors+1], ex[nSectors+1], ey[nSectors+1];
    
    for (int i = 0 ; i < nSectors ; i++){
      xAxisSize[i] = (i*6.28/nSectors);      
      valuesPhi[i] =  values.at(i);
      cout  <<  xAxisSize[i] << " "  << valuesPhi[i] << endl;
      ex[i] = 0 ; //errors.at(i);
      ey[i] = 0;
      
    }
    
    
    xAxisSize[nSectors] = 6.28;
    valuesPhi[nSectors] = valuesPhi[0];
    ex[nSectors] = ex[0];ey[nSectors] = ey[0];
    cout  <<  xAxisSize[nSectors] << " "  << valuesPhi[nSectors] << endl;
        
     TGraphErrors * graph = new TGraphErrors(nSectors+1,xAxisSize,valuesPhi,ex,ey);
     graph->SetName(iter->second.get_child("title").get_value<string>().c_str());
     
     string gname = graph->GetName();
     //graph->SaveAs((gname+".root").c_str());
     graph->Draw("LP");
//      if (grapCount == 0) {graph->Draw();}
//      else {graph->Draw("same");}
     grapCount++;
     //delete graph;
  }
  
  acan->SaveAs("ValuesVPhi.root");
  

}


void get2DplotsOnRateFromROOTfile ( const string& rootfile, const string & jsonMap,const string& areaFile ,const int & bmax,const int & emax ) {
  
  size_t toRoot = rootfile.find(".root");
  size_t toLastSlash = rootfile.rfind("/");
  // if the file contains total_ usually this is followed by runnumber so get only the run number
  if (rootfile.find("total_") != string::npos) toLastSlash = rootfile.find("total_") + 6; 
  string fileBaseName = rootfile.substr(toLastSlash,rootfile.length()-toRoot+1);
  cout << toLastSlash << " " << toRoot << endl;
  string JSONout = fileBaseName+"roll_rates.json";
  
  getRateInJSONformatFromRootFile(rootfile,JSONout,areaFile);
  get2DplotsForJSONFileUsingAndJSONmap(JSONout,jsonMap,bmax,emax,"Rate (Hz/cm^{2})","_"+fileBaseName+"_2D");
  
  
}



// endof QueryObject methods


