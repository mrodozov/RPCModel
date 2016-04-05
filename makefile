# Authors : Mircho Rodozov, Michele Gabusi , mrodozov@cern.ch , mgabusi@cern.ch

ROOTINC = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs)
LIBJSON = -ljson_linux_jsoncpp
COMPILE_AS_Cpp0X = $(-std=c++0x)
#MYSQLLIBS = $(shell mysql_config --libs)
#MYSQLINC = $(shell mysql_config --include)

all : RateLumiEcapOffline RateVsLumi_Online CurrentApplication Print_offline_db_text_files GetEcapPlusMinusRatios Print_online_db_text_files GetRatioOfChambersForTwoRuns RateVsPhi_online CalculateLumiForRunWithFitCoefficientRefference RecoverLumiForEachRunWithCoeffAndRateFiles
#all : RateVsLumi_Online

objects = Strip.o DataObject.o Roll.o ExtendedStrip.o ExtendedRoll.o Chip.o Service.o DBconnector.o Applications.o NoiseAnalyzer.o
extensionsObjects = Acquisition.o Crawler.o Run.o

# Mircho objects (main core objects)

main.o : main.cpp ExtendedStrip.o ExtendedRoll.o Service.o DBconnector.o Applications.o 
	g++ -c -Wall main.cpp $(ROOTINC)

Chip.o : core/Chip.cpp ExtendedRoll.o
	g++ -c -Wall core/Chip.cpp $(ROOTINC)

DataObject.o: core/DataObject.cpp
	g++ -c core/DataObject.cpp

Strip.o: core/Strip.cpp
	g++ -c core/Strip.cpp

Roll.o : core/Roll.cpp Strip.o DataObject.o
	g++ -c core/Roll.cpp $(ROOTINC)

ExtendedStrip.o: core/ExtendedStrip.cpp Strip.o
	g++ -c core/ExtendedStrip.cpp $(ROOTINC) 

ExtendedRoll.o: core/ExtendedRoll.cpp Roll.o ExtendedStrip.o DataObject.o
	g++ -c core/ExtendedRoll.cpp $(ROOTINC)

Applications.o : Applications.cpp Service.o ExtendedRoll.o
	g++ -c Applications.cpp $(ROOTINC)

Service.o : ServiceClasses/Service.cpp
	g++ -c ServiceClasses/Service.cpp $(ROOTINC)

DBconnector.o : ServiceClasses/DBconnector.cpp
	g++ -c ServiceClasses/DBconnector.cpp $(MYSQLINC)

main_ratevslumi_online.o : main_func/main_ratevslumi_online.cpp
	g++ -c main_func/main_ratevslumi_online.cpp $(ROOTINC)

main_offline_ecap_ratelumi.o : main_func/main_offline_ecap_ratelumi.cpp
	g++ -c main_func/main_offline_ecap_ratelumi.cpp $(ROOTINC)

main_print_offline_db_files.o : main_func/main_print_offline_db_files.cpp
	g++ -c main_func/main_print_offline_db_files.cpp $(ROOTINC)

main_print_online_db_files.o : main_func/main_print_online_db_files.cpp
	g++ -c main_func/main_print_online_db_files.cpp $(ROOTINC)

main_rateVsPhi_online.o : main_func/main_rateVsPhi_online.cpp
	g++ -c main_func/main_rateVsPhi_online.cpp $(ROOTINC)

# Michele objects (extensions objects)

printRunProperties.o: main_func/printRunProperties.cpp
	g++ -c main_func/printRunProperties.cpp $(ROOTINC)

mainNoise.o: main_func/mainNoise.cpp
	g++ -c main_func/mainNoise.cpp $(ROOTINC)

studyAsimmetry.o: main_func/studyAsimmetry.cpp
	g++ -c main_func/studyAsimmetry.cpp $(ROOTINC)

NoiseAnalyzer.o: Extensions/NoiseAnalyzer.cpp Extensions/NoiseAnalyzer.h
	g++ -c Extensions/NoiseAnalyzer.cpp $(ROOTINC) $(FLAGS)

Crawler.o: Extensions/Crawler.cpp
	g++ -c Extensions/Crawler.cpp $(ROOTINC)

mainCrawler.o: main_func/mainCrawler.cpp
	g++ -c main_func/mainCrawler.cpp $(ROOTINC) $(FLAGS)

producePlot.o: main_func/producePlot.cpp
	g++ -c main_func/producePlot.cpp $(ROOTINC) $(FLAGS)

plotOnPhi.o: main_func/plotOnPhi.cpp
	g++ -c main_func/plotOnPhi.cpp $(ROOTINC) $(FLAGS)

plotOnPhiVaryZ.o: main_func/plotOnPhiVaryZ.cpp
	g++ -c main_func/plotOnPhiVaryZ.cpp $(ROOTINC) $(FLAGS)

importFromTextResources.o: main_func/importFromTextResources.cpp
	g++ -c main_func/importFromTextResources.cpp $(ROOTINC) $(FLAGS)

readContainerFromDO.o: main_func/readContainerFromDO.cpp
	g++ -c main_func/readContainerFromDO.cpp $(ROOTINC) $(FLAGS)

readContainer.o: main_func/readContainer.cpp
	g++ -c main_func/readContainer.cpp $(ROOTINC) $(FLAGS)

fillContainer.o: main_func/fillContainer.cpp
	g++ -c main_func/fillContainer.cpp  $(ROOTINC) $(FLAGS)

Run.o: Extensions/Run.cpp
	g++ -c Extensions/Run.cpp $(ROOTINC) $(FLAGS)

Acquisition.o: Extensions/Acquisition.cpp
	g++ -c Extensions/Acquisition.cpp $(ROOTINC) $(LIBJSON) $(FLAGS)

# here the applications goes

# Mircho applications

# current app is the one in main.cpp

CurrentApplication : main.o $(objects)
	g++ -o CurrentApplication.lnxapp main.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X) 

RateLumiEcapOffline : main_offline_ecap_ratelumi.o $(objects)
	g++ -o RateLumiEcapOffline.lnxapp main_offline_ecap_ratelumi.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X)

RateVsLumi_Online : main_ratevslumi_online.o $(objects)
	g++ -o RateVsLumi_Online.lnxapp main_ratevslumi_online.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X)

RateVsPhi_online : main_rateVsPhi_online.o $(objects)
	g++ -o RateVsPhi_Online.lnxapp main_rateVsPhi_online.o $(objects) $(ROOTLIBS) 

Print_offline_db_text_files : main_print_offline_db_files.o $(objects)
	g++ -o PrintOfflineRollTextFiles.lnxapp main_print_offline_db_files.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X)

Print_online_db_text_files : main_print_online_db_files.o $(objects)
	g++ -o PrintOnlineRollTextFiles.lnxapp main_print_online_db_files.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X)

GetEcapPlusMinusRatios : $(objects)
	g++ -c main_func/main_Ecap_assym.cpp $(ROOTINC)
	g++ -o GetEcapPlusMinusRatios.lnxapp main_Ecap_assym.o $(objects) $(ROOTLIBS)  $(COMPILE_AS_Cpp0X)

GetRatioOfChambersForTwoRuns : $(objects)
	g++ -c main_func/main_chambersRatiosTwoRuns.cpp $(ROOTINC)
	g++ -o GetRatioOfChambersForTwoRuns.lnxapp main_chambersRatiosTwoRuns.o $(objects) $(ROOTLIBS)  

CalculateLumiForRunWithFitCoefficientRefference : $(objects)
	g++ -c main_func/main_calculateLumiForRunWithFitCoefficientRefference.cpp $(ROOTINC)
	g++ -o CalculateLumiForRun.lnxapp main_calculateLumiForRunWithFitCoefficientRefference.o $(objects) $(ROOTLIBS) 

RecoverLumiForEachRunWithCoeffAndRateFiles : $(objects)
	g++ -c main_func/main_recoverLumiForEachRunWithCoeffAndRateFiles.cpp $(ROOTINC)
	g++ -o RecoverLumiForRuns.lnxapp main_recoverLumiForEachRunWithCoeffAndRateFiles.o $(objects) $(ROOTLIBS) 

# Michele applications

fill:
	g++ core/Roll.cpp Extensions/Run.cpp Extensions/Acquisition.cpp core/Strip.cpp core/DataObject.cpp core/ExtendedRoll.cpp core/ExtendedStrip.cpp main_func/fillContainer.cpp Chip.o NoiseAnalyzer.o $(ROOTINC) $(FLAGS) -o Fill $(ROOTLIBS) $(LIBJSON)

Fill:  Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o NoiseAnalyzer.o
	g++ -o Fill fillContainer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

Read: Roll.o Run.o Strip.o DataObject.o ExtendedRoll.o Chip.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o readContainer.o NoiseAnalyzer.o
	g++ -o Read readContainer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

ReadFromDO: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o NoiseAnalyzer.o readContainerFromDO.o
	g++ -o ReadDO readContainerFromDO.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

lumi: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o producePlot.o Acquisition.o NoiseAnalyzer.o
	g++ -o lumi producePlot.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

import: Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o importFromTextResources.o Acquisition.o NoiseAnalyzer.o
	g++ -o import importFromTextResources.o Roll.o Run.o Strip.o DataObject.o  ExtendedRoll.o ExtendedStrip.o Chip.o NoiseAnalyzer.o Acquisition.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pplot: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o producePlot.o Acquisition.o NoiseAnalyzer.o
	g++ -o plot producePlot.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

asim: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o studyAsimmetry.o Acquisition.o NoiseAnalyzer.o
	g++ -o asym studyAsimmetry.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pphiz: plotOnPhiVaryZ.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o  Acquisition.o NoiseAnalyzer.o
	g++ -o pphiz plotOnPhiVaryZ.o Roll.o Run.o Strip.o DataObject.o  Chip.o ExtendedRoll.o ExtendedStrip.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

noise: mainNoise.o NoiseAnalyzer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o 
	g++ -o noise mainNoise.o NoiseAnalyzer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o  $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pphi: plotOnPhi.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o NoiseAnalyzer.o
	g++ -o pphi plotOnPhi.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

crawler: Crawler.o mainCrawler.o DataObject.o
	g++ -o crawl Crawler.o mainCrawler.o DataObject.o  $(ROOTLIBS)

runp: printRunProperties.o DataObject.o Run.o ExtendedRoll.o ExtendedStrip.o Chip.o Strip.o Roll.o Acquisition.o NoiseAnalyzer.o
	g++ -o runp printRunProperties.o DataObject.o Run.o Chip.o ExtendedRoll.o ExtendedStrip.o Strip.o Roll.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON)

# general clean and install

clean:
	find ./ -maxdepth 10 -type f -iname '*.o' -delete;
	find ./ -maxdepth 10 -type f -iname '*~' -delete
	find ./ -maxdepth 10 -type f -iname '*.lnxapp' -delete

install:
	mv *.lnxapp ../executables_and_scripts

# install commands

install_ratelumi_on : 
	mv RateVsLumi_Online.lnxapp ../executables_and_scripts/RateLumiOnline_Offline_ranges.lnxapp

install_ratelumi_offline_ecap :
	mv  RateLumiEcapOffline.lnxapp ../executables_and_scripts/

