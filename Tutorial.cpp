#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <new>
#include <stdlib.h>
/** non-standard headers , the model , the wrappers and ROOT */
#include "core/ExtendedStrip.h"
#include "core/ExtendedRoll.h"
#include "Applications.h"
#include "ROOT/ROOT.h"
#include "core/DataObject.h"

/**
 * Welcome people :) This tutorial aims to get you familiar with this code 
 * so that you don't freak out when you see there are a lot of things in the headers. 
 * First we start with an explanation on how the data for the rate is structured which
 * reveals the idea how the model is built. After a run is finished, automatic procedure
 * analyzes the RAW data on the counts (the total number of hits) recorded by the detector
 * over the run, where the data is structured as time intervals for each channel for which 
 * interval the total hits registered by the channel are given. The time lenght of the
 * interval is also given, so that one have the time and the counts for each interval 
 * for the given channel. For single channel what is available at the end of the analysis
 * is the average rate in Hz for the entire run. The channels are grouped by chambers,
 * where every chamber has exactly 96 channels associated with it, even if the channel
 * is not working, never worked, or regardless any other reason - a channel belongs to a
 * chamber, there are specifics associated with it concerning also the chamber. 
 * Another thing associated with the chamber is the so called LinkBoard that is the 
 * electronic equipment reading the signals from the channels in a chamber. 
 * The LinkBoard and the chamber in this sense can be assumed to be the same thing,
 * as they have the same ID and all the channels associated with them are cited as
 * 'of chamber with ID' some LinkBoard/chamber ID. 
 * 
 * The analysis on the rate produces 5 outputs 
 * 	1. Root file with the rates for each LinkBoard, that contains 1 TH1F histogram per LinkBoard with 96 entries for each channel rate in Hz, and the channel number corresponding to the bin num
 * 	2. Text files for the channels 'To Mask'. All the channels that has rate over specific threshold are marked as noisy and written in that files - 18 files for each 'Tower'
 *      3. Text files for the channels 'To Unmask'. All the channels that are masked, but does not behave as noisy anymore, 18 files for each tower
 *      4. Text files for channels that are considered 'Masked'. All channels found to be masked (based on some rate measurements). 18 files per each tower
 *      5. Text files for channels that are considered 'Dead', meaning they have no signal even if we are sure the channel is not masked. 18 files per each tower
 * 
 * Here we focus on the rate histograms only. Example file with the rate for 1 run can be found in  testdata/total_277218.root. 
 * The text files, or the 'strips conditions data' also can be used but the rate is the most imp.
 * 
 * The model used here follows the chamber/LinkBoard convention, and the center of this model is the ExRoll class (the name is from ExtendedRoll) which inherits from Roll 
 * (that contains some basic functionality). The name used is misleading, as one ExRoll object is representing a chamber, with 96 channels in all cases, regardless what
 * Roll actually mean. This comes from a time i was not familiar with the system and the name remained as it was. 
 * 
 * In short, as the data was structured over the LinkBoard/chamber model, the class ExRoll represents a model of one chamber. It's functionality provides the map to the chamber's Rolls
 * and from LinkBoard channel number to Strip number from a Roll (Channel number and strip number are not the same thing as chamber and roll are not the same). 
 * The other important class is the ExStrip class. Every ExRoll object has exactly 96 ExStrip objects that has their own specifics as rate, ch number, strip number, area etc.
 * The name (ExStrp) again is from ExtendedStrip, which inherits from Strip, where strip has basic functionality. Together this two classes handle the processing of data
 * and you only have to care about those two. Their code is in 'core' folder. All the other classes and functions can be considered not important for now.
 * 
 * A ExRoll object is constructed usually always using it LinkBoard/chamber ID (again, chamber and LinkBoard ID is the same thing) passed to it as string. All the IDs that are used can be found in 
 * 'localResources/InputRollList.txt' file. Once constructed, the object has to alloc and init it's ExStrip objects which are used for almost everything in the ExRoll suroutines.
 * The ExStrip objects are not allocated/initialized by the constructor because of two reasons: 
 * 	1. the possibility to extend further both ExStrip and ExRoll for more 'clear beginning' every time one wants to use the code, meaning there is mechanism to extend the internal member ExStrip 
 * 	with more functionality without changing the existing code (read what is Liskov Substitution Principle if you like). In short, it gives you the ability to do as you wish without 
 *	the risk of braking my stuff. Another reason was that new functionalities are required during the way - and one cannot anticipate them all, but can provide a convenient mechanism to extend
 * 	
 * 	2. the other reason why one need to call methods outside of the constructor - my bad design at the time and the laziness that followed, lol. 
 * 
 * Lets see some examples now
*/


int main (int argc ,char *argv[]){
  
  
  // construct new ExRoll object. First we need valid ID, select any from 'localResources/InputRollList.txt'
  
  string chmaberID = "RE-4_2_35";    // it's RE Disk+4 Ring2 sector 25. It contains 3 rolls
  ExRoll * aroll = new ExRoll(chmaberID); // Done. The object is constructed, it takes only a string with the ID
  aroll->allocStrips();
  aroll->initStrips(); // Don't forget to alloc and init the strips and please don't put them in the base constructor. Some day i'll fix this perhaps :)
  
  /**
   * After the object is constructed, it 'knows' it relation to the roll partitions it contains. 
   * There is a method to get the number of rolls the object represents. 
   */
  
  cout << "number of rolls (clones)  " << aroll->getClones() << endl; // getClones gives the number of rolls the chamber contains. Don't remember why i called it 'clone' at the time. In all Endcap chambers the rolls are 3
  cout << "chamber ID " << aroll->getFullOnlineRollID() << endl; // prints the chamber ID, as given to the constructor
  
  /**
   * We go into details with the rolls and how the strips are distributed within the rolls
   * Constructing the chamber obj with ID allows the object to know it's roll IDs and related strips, for further maps we'll feed it with additional configs
   */
  
  
  
  for(int i = 0 ; i < aroll->getClones() ; i++){
    
    // a clone number when used always starts from 1, not from zero, as there is no such thing as 'roll number 0'. Same for the strips
    
    cout << aroll->getRollIDofCloneWithNewIdentifiers(i+1) << endl; // the official CMSSW ID for the rolls of this chamber. This is what is used mostly
    cout << aroll->getRollIDofCloneWithConstructionDBidentifiers(i+1) << endl; // another roll ID, used for a DB we use called constructionDB
    cout << aroll->getRollIDofClone_withEtaPartSeparated(i+1) << endl; // and another one, with different purpose
    
    // get the channels that belong for specific roll, using methods that return the first and the last channel numbers that belong to this roll.     
    
    for (int j = aroll->getFirstStripNumberOfClone(i+1) ; j <= aroll->getLastStripNumberOfClone(i+1) ; j++){
      
      cout << " channel number in the chamber " << aroll->getStrip(j)->getOnlineNumber() << " strip number in the roll "; // the number of this channel associated with the chamber
      cout << aroll->getStrip(j)->getOfflineNumber() << endl; // the STRIP number of this channel, associated with the roll it belongs. Note the difference.
      
      // the names online for chamber convention and offline for CMSSW/roll convention were adopted again for unknown reasons, 
      // it was the jargon used at the time to distinguis chambers and rolls (and channels from strips)
      
    }
    
  }
  
  
  /**
   * To perform any analysis, the chamber objects has to be given two things
   * 1. Initial configs data. This includes additional IDs, area per strip for a given chamber, tower name where the chamber belongs, ChipIDs that are the chip numbers for that chamber 
   * (there are 14000+ chips to control the LV, 12 per chamber, each controlling 8 channels LV). This configs are used only when needed, and they are static - once set there is not need 
   * to change them
   * 2. The dynamic data that changes from run to run. This includes rate data, current, strip conditions data (masked, dead, to mask, to unmask) and anything that depends on the run specifics.
   * In short, we feed the roll objects with configs only once, but we change the data for the rate per run for example. 
   * 
   * Let's perform some config setup. For this reason we'll use objects from another class called DataObject. The DataObject object is basically a matrix, 
   * convenient way to deal with widely used text files with rows and columns. Most of the setup needed is done as we first give a file to a DataObject instance
   * and then give the instance to the chamber object. 
   */
  
  DataObject area_resource("localResources/area_noise_cmssw_withRE4"); // here we give to the DataObject object contructor a local text file containing the strip area for every strip
  aroll->setStripsAreaFromSource_cmsswResource(area_resource); // and we pass the DO to the chamber object. The chamber finds it's strips areas and sets them up. 
  DataObject numerical_IDS("localResources/RawIDs_New.txt"); // another local file given to DataObject. Then we feed the chamber again
  aroll->setRollRawIDs_UsingNewIDsFile(numerical_IDS); // the numerical IDs are unique numbers for every CMSSW roll and are used as identifiers (in the WBM for example)  
  // the DataObject is easy to use and it's convenient for table data in text files - it takes the file name and it creates a N x M matrix matching the rows and columns in the file. Used like that:
  cout << "print the Roll IDs and their numerical IDs" << endl;
  for (int i=0; i < numerical_IDS.getLenght() ; i++){
    
    //cout << numerical_IDS.getElement(i+1,1) << " " << numerical_IDS.getElement(i+1,2) << endl; // just give row and column indexes, element returned as string.
    
  }
  cout << "---------------------------------" << endl;
  /**
   * Let's see how we feed the chamber with rate. Usually we have a TFile that is open and for each TH1F histogram we get it's chamber (or create the chamber for that ID) and set the rate,
   * now we have only one chamber so we need the TH1F object first taken from the TFile. If a chamber is switched off, the rate histogram is not written
   * and the TH1F object does not exist in the TFile, so that's why we never asume the TH1F obj with the rate exists in the TFile, we check that. 
   */
  
  TFile * rate_file = new TFile("testdata/total_277218.root","READ");
  // we use the chamber ID of the roll to search for it's 
  TH1F * rate_histo = dynamic_cast<TH1F*>( rate_file->Get(aroll->getFullOnlineRollID().c_str()) );// we use the chamber ID of the roll to search for TH1F object with the same name in the TFile
  // now we have each strips rate set, we have set the area and we go for the rate in Hz/cm^2
   aroll->setStripsRatesFromTH1FObject(rate_histo); // set the rates from the histogram
  rate_file->Close("R");
  rate_file->Delete();
  
  cout << "print the strips rates" << endl;
  for (int i = 0 ; i < aroll->getClones() ; i++){
    cout << aroll->getRollIDofCloneWithConstructionDBidentifiers(i+1) << endl;
    
    ExStrip * astrip;
    
    for (int j = aroll->getFirstStripNumberOfClone(i+1) ; j <= aroll->getLastStripNumberOfClone(i+1) ; j++){
      astrip = aroll->getStrip(j);
      cout << " channel number " << astrip->getOnlineNumber() << " strip number " << astrip->getOfflineNumber() << " rate " << astrip->getRate() << " rate/cm^2 " << astrip->getRate()/aroll->getStripsAreaFromClone(i+1) << endl;
      // all strips in a given roll (clone) has the same area, thats why the strip does not have area property, the chamber keeps the area of strips in a given roll and gets it with 'aroll->getStripsAreaFromClone(i+1)'
    }
      
  }
  
  cout << "---------------------------------" << endl;
  // reset the strip rates to 0 and use JSON file to feed the chamber with rates
  aroll->ClearStripsConditions();  
  
  ifstream ss;
  ptree JSONobject;
  ss.open("testdata/output_strips275764.json");
  
  boost::property_tree::json_parser::read_json(ss,JSONobject);
  
  
  aroll->setStripsRateFromJSONsource(JSONobject.get_child("rates")); // set rates from JSON object. This object contain the rate and strip conditions data for all rolls for a given run
  ss.clear();
  ss.close();
  // once the rates values are set, one can try to reduce the noise, where the responsible for the reduction should be the chamber object, as it has control over 
  // the rates of all channels. The existing noise reduction method is far from perfect and we could think on better one. What it does now is take the strip rates
  // per roll as the rate is expected to be similar for strips in one roll (and the avg rate is also different for the strips in the other rolls as the strips areas are different)
  // plot the values on TH1F histogram, get the mean of the distribution and then loop again over the strips in this roll, comparing the mean with every single strip rate.
  // If a strip rate is higher than given percent when compared with the mean , the strip rate is set to 0, thus supressing single noisy strips contribution
  
  cout << "without correction " << aroll->getAvgRatePSCWithoutCorrections() << endl;
  aroll->removeNoisyStripsForAllClonesWithPercentValue(100); // 100 means if a strip has rate 100% higher than the strip rates mean in given roll
  cout << "with correction " << aroll->getAvgRatePSCWithoutCorrections() << endl;
  
  delete aroll;
  
  /**
   * Example of more common case where we open a TFile and create ExRoll object for each histogram
   */
  
  rate_file = new TFile("testdata/total_277218.root","READ"); // rate_file is existing pointer of type TFile
  
  TIter nextkey(rate_file->GetListOfKeys());
  TKey * key;
  TObject * obj1;

  while (key = (TKey*)nextkey()){ // we loop here on all the objects found in the file.
    obj1 = key->ReadObj();
    TH1F * rate_histo = dynamic_cast<TH1F*>(obj1);
    string chamber_id = rate_histo->GetName();
    // setup chamber object using the current histogram name
    ExRoll * roll = new ExRoll(chamber_id);
    roll->allocStrips();
    roll->initStrips();
    // give the chamber object some config (strips area config)
    roll->setStripsAreaFromSource_cmsswResource(area_resource); // the area_resource is existing DataObject we used already
    // get the rate from the histogram
    roll->setStripsRateFromTH1Source(rate_histo);
    // now do whatever is required, write data or whatever, , delete the chamber object and that's usually it
    cout << roll->getFullOnlineRollID() << " chamber's not corrected avg rate " << roll->getAvgRatePSCWithoutCorrections();
    roll->removeNoisyStripsForAllClonesWithPercentValue(100); // reduce some noise
    cout << " corrected rate " << roll->getAvgRatePSCWithoutCorrections() << endl;
    
    delete roll;
    
  }
  
  rate_file->Close("R");
  
  /**
   * In Apllications.cpp is where most of the implementations are done. In general the different applications there are not much different 
   * from the example above - get the data and process it in convenient way using the ExRoll and ExStrip classes,
   * then use the data to plot something. Many other methods for this two classes are useful, for example the strip conditions data.
   * 
   * The latest implementation that allow us now to get the data from JSON files would reduce a lot of digging for data over servers,
   * so let's hope we could focus on something more 'physical' now. Take a look in the Applications.cpp, should be easier at that point.
   * 
   */
  
  
}