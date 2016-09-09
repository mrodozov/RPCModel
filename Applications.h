/** Author: Mircho Rodozov mrodozov@cern.ch
 * Date 08.01.2013
 * Info added at SVN revision 76
 * 
*/
#ifndef APPLICATIONS_H
#define APPLICATIONS_H

#include <string>
#include <map>
#include "core/DataObject.h"
#include "ROOT/ROOT.h"
#include "ServiceClasses/Service.h"

#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

using namespace std;// DrawVvsPhiUsingJSONdataAndJSONconfig(jsonDatafile,jsonMapFile); 

void PrintGeometryTable(std::string InputWithRollNames,std::string Ouput,DataObject & Areas,DataObject & RawIDFile);

/**
 * 
 * 
 */

void WriteResultsUsingIOtextFilesAndDataSources(std::string ,std::string ,DataObject &,DataObject &,DataObject &);

/** 
 * One function to substitute the existing one that are searching in order to found their strip rate values.
 * 
 */

void WriteRollsAndStripsFilesForDB_usingRootFile(string rootFile,string OutputRolls,string OutputStrips,string ErrorFile,DataObject& Masked,DataObject& Dead,DataObject& ToMask,DataObject& ToUnmask,DataObject& Areas,DataObject& RawIDFile,string rollNames);

/** 
 * Functions that loops over rolllist and another that loops over runlist. Both 
 * should be able to work with ROOT objects
 */

/**
 * Functions for the intrinsic noise and 
 * rate vs lumi studies
 */

void WriteRollInactiveStripsRatios (string OutputRolls,DataObject& Masked,DataObject& Dead,DataObject& Areas,DataObject& RawIDFile,string rollNames,string runNumber);


TH2F * getRateVsLumiHistoForSingleRoll(DataObject & ,const string &,const int & , const int &);


/**
 * Function for plot HVvsRate from the hv scan,
 * for the intrinsic noise 
 * This function is usefull when the luminosity is const obviously , when applied for different 
 * luminosity nothing interesting could be said
 */

TH2F * getHVvsRatePlotForOneRollForRunList(string & rollName,const string & resourcePath,const string & suffix,DataObject & RunHVTempDataObject,DataObject & areas);

void writeSingleDoubleGapRatioUsingRateForOneRunAndIntrinsicNoiseFileForRun(const string&, const string&,const string&,ofstream & OutputFile);

void extractAverageRatesFromRootFilesForListOfFiles(const string &,const string &,const string &,const string &,const string &,const string &,const int &);

void getMeanValuesFromStripsDistributionsForRunlist(const string & runlist,const string & resourcePath,const string & sufix,const string & Out);

void GetSingleDoubleGapRatios (string MapOfSingleGapRolls,string regex,string DBfilesContainer,string runlist,string LumiFile,string histoTitle,int histoBins,int histocolor,int histoLineColor);

/** @brief this modules here should fulfill the requirements for application that will draw 
 * @brief rate vs luminosity by cmssw part
 */ //cmssw rate vs lumi
 
 // TODO -> NOTE -> note that the exluded strips are taken into consideration when the rate is calculated for RateVsLumi !

/** @brief functions to extrapolate the rateVslumi for every offline and online roll , to save the histos and print two files */

void writeIntrinsicForCMSSW_Rolls(DataObject &,DataObject &,string,string,string,bool,bool);//,double(*getRateCallBackFunc)(int));

void writeIntrinsicForOnlineRolls();

/** */

void plotRateVsLumi_using_the_database_files_rollLevel_Ecap_Offline(DataObject & LumiFile,string filesCont,string outCont,string IntrinsicFile,bool WriteNewIntrinsic,bool subtractIntrinsic,bool averageEntries,string GoodRunsMap,QueryObject * query);

bool rollFilter(string rollName,string disk ,string ring,string orientation,int chamberStart,int chamberEnd,string partOfChamber);

/** @brief mention to be ultimate filter both for online/offline - barrel or endcap.*/
bool universalRollFilter(string barrel_or_ecap
,int Wheel_or_Disk_number_start,int Wheel_or_Disk_number_end
,int EcapRing_or_BarrelLayer_start,int EcapRing_or_BarrelLayer_end
,int Sector_start,int Sector_end // notice here that what means sector in the general case for ecap its still getRollID
,string optionallBarrelID // inc case of ecap , even if something is passed as argument assign it explicitely to empty string
,int partitionStart,int partitionEnd
,bool usedForOffline
);

void plotRateVsLumi_using_the_database_files_stripLevel(DataObject & LumiFile,string filesCont,string outCont,string IntrinsicFile,bool WriteNewIntrinsic);

void plot_X_vs_Y_values_using_JSON_data_and_JSON_config(const string & JSON_data, const string & JSON_config,const string & rootOutputName);

void plotEcap_RateVsPhi(string rateFile,bool subtractIntrinsic,string fileWithIntrinsic,DataObject & area,double cutValueSingleStrip,string SGmapFile,DataObject & LumiFile,QueryObject * query);

map<string,map<string,double> > prepareDataSourceWithRatesAndLumi ( string data_folder, DataObject& lumiFile, bool isOffline );

void plotRateVsLumi_using_root_and_JSON(const map<string,map<string,double> > & run_rollRate_map ,DataObject & lumiFile,int cutThreshold,QueryObject * query,bool isOffline,bool debugOUTPUT,const string & outputJSON);

DEPRECATED void print_online_dbfiles(string rootContainer,string outputContainer ,DataObject & lumiFile,DataObject& area,string RollList,bool correction,double correctionInPercents);

void print_offline_db_files(string rootContainer,string outputContainer ,DataObject & lumiFile,DataObject& area,string RollList,bool correction,double correctionInPercents);

void plotRateVsLumi_using_the_database_files_stripLevel_online();

void WriteHTMLforNoisePageStripConditions(string,string,string,DataObject & ,DataObject & ,DataObject & ,DataObject & ,map<string,string>);

void getRollNamesFromSingleFile(string rootFile,string towerName,string output);

/** @brief This functions is used to extract the Roll names from single Tower like file , in order to get the Roll-Tower map as text file
 * @param rootTowerFileName
 * @param towerName
 * @param outputFile
*/

map <string,map<int,int> > getRollChipsMapFromDataObject(DataObject & lbFebMapFile);

map <string,string> getRollNameLBnameMapFromDataObject(DataObject & LBnameRollNameMapFile);

void writeChipHistosForThresholdScan(string fileList,string pathToFileList,string chipsMapFile,string LBMapFile,
  string run_threshold_value_file,string outputDestination,string ChipsIDSonlyFile,string disabledChipsFile,
  string rollNamesFile,string thresholdDumpFile,string fileWithDefaultTresholds,string fileWithMinThresholds,string SaveDetailedHistosValue,string FitFunctionDefinition,
  string RateMinimumRuleValue,string histogramMaximum,bool UseDefaultThresholdvaluesFromFile);

void correctionCheck(string RollName,string AreaFile,string RootFileRateSource ,int percentForCorrection);

void getHistogramOfExtrapolatedRollIntrinsicRateValues(string fileWithIntrinsicRates,string histogramName);

void draw2DbackgroundPlot(string folder,string areaFile,string shortBarrelIDS,string shortEcapIDS,string runList,string ListOfRolls,bool correctionToBeApplied,int correctionInPercents,int barrelMax,int ecapMax, string CenterOfMassEnergy);

void draw2DbackgroundPlot_online(string file,string area,string listOfRolls,string runNumber,string plus_minus_sign,string ForBpart,int percentageCorrection,bool applyCorrection);

void drawLeakRollsNoiseDistribution(string rootFile,string area,string listOfLeaks,string listOfLeakOut,string listOfNonLeakOut,string listAllRolls,string plotsRootName);

void getCorrelationCoefficients(string RateFilesContainer,string LumiFile,string areaFile,string AllRollNames,int percentOfCut,bool saveHistograms,string outputFolder);

void calculateLumiForRunWithFitCoefficientRefference(string rootFolder,DataObject & areaFile,DataObject & coefficientFile,string AllRolls,DataObject & LumiFile,string & outputContainer,bool saveHisto,double coefficientThreshold,bool use_signle_roll,string roll_passed);

void recoverLumiForEachRunWithCoeffAndRateFiles(string RootContainer,string LumiAsRunList,string CoefficientRefference2011,string areaFile,string AllRollNames,string outputContainer);

void RPC_HF_lumi_difference(string sourceFile,string histoObjectName,string outpuFileName);

void NewOldLBsHistos(string newLBsList,string oldLBsList,string areaFile,string resourceDir,string runNum,string resultPic,string textOutput);

void getRatioOfChambersForTwoRuns(string runLowerRateFile,string runHigherRateFile,string area,string allRollsList,string SGrollsMap,int cutValue);

void getRatioOfNegativePositiveChambers(string dataFolder,string area,string allRollsList,string SGrollsMap,string LumiFile,int cutValue,int referenceRunNumber,string NPtitle);

void plotRateOnLumi_Vs_Lumi_Endcap_OfflineAverageForDisk(string dataFolder,DataObject & LumiFile,DataObject & area ,string DiskNumber);

void compareMichelesMirchosCoeff(); // to be implemented 

/** 
 * @brief Experimental functions
 */

void print_map_of_online_offline_rolls_and_rollNumbericIDS(string allOnlineRolls,string RAW_IDS);

void DeadVsTotalRunTime(string FileWithDead,string FileWithRunTime,string runlistgood,string outputdirAndSufix,int minLS,int bins);

void RootFilesLoader (string pathTofiles,string prefix,string filelist);

void drawerFunc(); // this is a root macro, but it has to have interface

void print_all_the_strips_for_runlist(string folder,string newTitle,string runlist,string rollList,string exclusionList,string area,string LumiFile,string Masked_Dead_folder,string minOnX,string maxOnX);

void getInstLum(string filename);

int IsCorruptedFile(string theFile); // return true if the file is corrupted and returns true if it is

void drawDifferentRunDurationLumies(const string & filename);

void getCorrectedChambersRateDistribution(const string & chamberSuffix,const string &inputFile, const string & areaFile ,const string & outputFolder);

void addZeroSuffixForlbMap(const string & filename); // 

void printTowerRE4(const string & towerRooFile);

void DeadMaskedInactivePlot(const string & runlist,const string & runs_dates_map ,const int & labelModulo, const string & xAxisTitle,const string & dataFolder);

void WriteIDsMap(const string & inputRolls,const string & RawIDsFile,const string & areaFile, const string & towerFile, const string & chipsMapFile,const string & output);

void WriteNoiseScanResults(string rootFile, int appliedHVBarrel, int appliedHVendcap, int runnumber, string outputFile, string area);

void PrintDBIDsvsNewIDs(string inputFile,string dataFile);

void WriteRateVsLumiPerRollFile(string & LumiFile, string & rootFilesFolder, string & area, string & outputFile, string & rollNamesAdditionalSuffix);

void SlopeRatiosComparisonForPairsOfIDs(string & IDs_file, string & inputFile, string & outputFile, string & twoDMap,string & ShortB, string & ShortE); // firstID, secondID, resultID

void GetLumiHistogramPerLS(string & lumiFile);

void getRateInJSONformatFromRootFile(const string & rootfilename,const string & JSONoutFileName,const string & areaFileName);

// analysis 

void compareHistos(TH1F * reference, TH1F * testing);

// drawing functions only

void get2DplotsForJSONFileUsingAndJSONmap(const string & JSONdataFile,const string & JSONmapFile, const int & BarrelMax, const int & EndcapMax, const string & ZaxisTitle,const string & fileSuffixes);

void PositiveNegativePartsRatio(string & IDs_file, string & runlist, const string & runfolder, const string & area, const string & JSON_config); // for a runlist, for each pair get distributions and write it's value

void getJSONinputForRateVsPhiForRun(const string & run_rootfile, const string & JSON_config, const string & JSONout);

void DrawVvsPhiUsingJSONdataAndJSONconfig(const string & JSON_data_fname, const string & JSON_config_fname); // the data file format is {RollPartName:{title:"",marker:"",color:"",data:[v1,v2,v3...],errors[e1,e2,e3,...]},RollPartName2:...} where the points given are put equdistant along 2pi in radiants on X 

// combined - data prep + drawing 

void get2DplotsOnRateFromROOTfile(const string& rootfile, const string & jsonMap,const string& areaFile ,const int & bmax,const int & emax);


/**
 *  Drawing Only Functions
 * 
 */



// detailed study on the rate with the details per strip. the study should show 2015-2016 comparison where for changes in the luminosity that are the same in terms of 
// start/finish lumi values taken in 

//void Detailed

// Possible correction on the rate exploiting luminosity drops, and removing the time intervals hit rates from the LBcounts overal
// Requires a dedicated function to 'recognize' the drops, and to remove determine the initial and final time stamps for a drop. 


#endif
