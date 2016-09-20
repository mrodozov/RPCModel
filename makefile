# Authors : Mircho Rodozov, Michele Gabusi , mrodozov@cern.ch , mgabusi@cern.ch

ROOTINC = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs)
ROOFITLIBS = -l RooFitCore -l RooFit -l RooStats
LIBJSON = -ljson_linux_jsoncpp
CC = g++ -std=c++14
boost_libs = -lboost_thread-mt
#MYSQLLIBS = $(shell mysql_config --libs)
#MYSQLINC = $(shell mysql_config --include)

all : RateLumiEcapOffline RateVsLumi_Online CurrentApplication Print_offline_db_text_files GetEcapPlusMinusRatios Print_online_db_text_files GetRatioOfChambersForTwoRuns RateVsPhi_online CalculateLumiForRunWithFitCoefficientRefference RecoverLumiForEachRunWithCoeffAndRateFiles 2DBK Tutorial DataSource
#all : RateVsLumi_Online

objects = Strip.o DataObject.o Roll.o ExtendedStrip.o ExtendedRoll.o Chip.o Service.o DBconnector.o Applications.o NoiseAnalyzer.o
extensionsObjects = Acquisition.o Crawler.o Run.o
ds_objects = LocalFileClient.o CurrentData.o RateData.o DataSource.o

# Mircho objects (main core objects)

main.o : main.cpp ExtendedStrip.o ExtendedRoll.o Service.o DBconnector.o Applications.o 
	$(CC) -c -Wall main.cpp $(ROOTINC)

Chip.o : core/Chip.cpp ExtendedRoll.o
	$(CC) -c -Wall core/Chip.cpp $(ROOTINC)

DataObject.o: core/DataObject.cpp
	$(CC) -c core/DataObject.cpp

Strip.o: core/Strip.cpp
	$(CC) -c core/Strip.cpp $(ROOTINC)

Roll.o : core/Roll.cpp Strip.o DataObject.o
	$(CC) -c core/Roll.cpp $(ROOTINC)

ExtendedStrip.o: core/ExtendedStrip.cpp Strip.o
	$(CC) -c core/ExtendedStrip.cpp $(ROOTINC) 

ExtendedRoll.o: core/ExtendedRoll.cpp Roll.o ExtendedStrip.o DataObject.o
	$(CC) -c core/ExtendedRoll.cpp $(ROOTINC)

Applications.o : Applications.cpp Service.o ExtendedRoll.o
	$(CC) -c Applications.cpp $(ROOTINC)

Service.o : ServiceClasses/Service.cpp
	$(CC) -c ServiceClasses/Service.cpp $(ROOTINC)

DBconnector.o : ServiceClasses/DBconnector.cpp
	$(CC) -c ServiceClasses/DBconnector.cpp $(MYSQLINC)

main_ratevslumi_online.o : main_func/main_ratevslumi_online.cpp
	$(CC) -c main_func/main_ratevslumi_online.cpp $(ROOTINC)

main_offline_ecap_ratelumi.o : main_func/main_offline_ecap_ratelumi.cpp
	$(CC) -c main_func/main_offline_ecap_ratelumi.cpp $(ROOTINC)

main_print_offline_db_files.o : main_func/main_print_offline_db_files.cpp
	$(CC) -c main_func/main_print_offline_db_files.cpp $(ROOTINC)

main_print_online_db_files.o : main_func/main_print_online_db_files.cpp
	$(CC) -c main_func/main_print_online_db_files.cpp $(ROOTINC)

main_rateVsPhi_online.o : main_func/main_rateVsPhi_online.cpp
	$(CC) -c main_func/main_rateVsPhi_online.cpp $(ROOTINC)

main_data_source.o : main_func/main_ds.cpp
	$(CC) -c main_func/main_ds.cpp -o main_data_source.o $(ROOTINC)

RateData.o : core/DataSource/RPCDSRateData.cpp
	$(CC) -c core/DataSource/RPCDSRateData.cpp -o RateData.o
	
CurrentData.o : core/DataSource/RPCDSCurrentData.cpp
	$(CC) -c core/DataSource/RPCDSCurrentData.cpp -o CurrentData.o

LocalFileClient.o : core/DataSource/RPCDSLocalFileClient.cpp 
	$(CC) -c core/DataSource/RPCDSLocalFileClient.cpp -o LocalFileClient.o
	
DataSource.o : core/DataSource/RPCDSDataSource.cpp RateData.o CurrentData.o
	$(CC) -c core/DataSource/RPCDSDataSource.cpp -o DataSource.o

# Michele objects (extensions objects)

printRunProperties.o: main_func/printRunProperties.cpp
	$(CC) -c main_func/printRunProperties.cpp $(ROOTINC)

mainNoise.o: main_func/mainNoise.cpp $(ds_objects)
	$(CC) -c main_func/mainNoise.cpp $(ds_objects) $(ROOTINC)

studyAsimmetry.o: main_func/studyAsimmetry.cpp
	$(CC) -c main_func/studyAsimmetry.cpp $(ROOTINC)

NoiseAnalyzer.o: Extensions/NoiseAnalyzer.cpp Extensions/NoiseAnalyzer.h
	$(CC) -c Extensions/NoiseAnalyzer.cpp $(ROOTINC) $(FLAGS)

Crawler.o: Extensions/Crawler.cpp
	$(CC) -c Extensions/Crawler.cpp $(ROOTINC)

mainCrawler.o: main_func/mainCrawler.cpp
	$(CC) -c main_func/mainCrawler.cpp $(ROOTINC) $(FLAGS)

producePlot.o: main_func/producePlot.cpp
	$(CC) -c main_func/producePlot.cpp $(ROOTINC) $(FLAGS)

plotOnPhi.o: main_func/plotOnPhi.cpp
	$(CC) -c main_func/plotOnPhi.cpp $(ROOTINC) $(FLAGS)

plotOnPhiVaryZ.o: main_func/plotOnPhiVaryZ.cpp
	$(CC) -c main_func/plotOnPhiVaryZ.cpp $(ROOTINC) $(FLAGS)

importFromTextResources.o: main_func/importFromTextResources.cpp
	$(CC) -c main_func/importFromTextResources.cpp $(ROOTINC) $(FLAGS)

readContainerFromDO.o: main_func/readContainerFromDO.cpp
	$(CC) -c main_func/readContainerFromDO.cpp $(ROOTINC) $(FLAGS)

readContainer.o: main_func/readContainer.cpp
	$(CC) -c main_func/readContainer.cpp $(ROOTINC) $(FLAGS)

fillContainer.o: main_func/fillContainer.cpp
	$(CC) -c main_func/fillContainer.cpp  $(ROOTINC) $(FLAGS)

Run.o: Extensions/Run.cpp
	$(CC) -c Extensions/Run.cpp $(ROOTINC) $(FLAGS)

Acquisition.o: Extensions/Acquisition.cpp
	$(CC) -c Extensions/Acquisition.cpp $(ROOTINC) $(LIBJSON) $(FLAGS)

# here the applications goes

# Mircho applications

# current app is the one in main.cpp

DataSource : main_data_source.o $(ds_objects)
	$(CC) -o main_ds.lnx main_data_source.o $(ds_objects) $(ROOTLIBS)

CurrentApplication : main.o $(objects)
#	 $(CC) -o CurrentApplication.lnxapp main.o $(objects) $(ROOTLIBS) $(ROOFITLIBS) 
	$(CC) -o CurrentApplication.lnxapp main.o $(objects) $(ROOTLIBS)

RateLumiEcapOffline : main_offline_ecap_ratelumi.o $(objects)
	$(CC) -o RateLumiEcapOffline.lnxapp main_offline_ecap_ratelumi.o $(objects) $(ROOTLIBS)

RateVsLumi_Online : main_ratevslumi_online.o $(objects)
	$(CC) -o RateVsLumi_Online.lnxapp main_ratevslumi_online.o $(objects) $(ROOTLIBS)
	

2DBK :  main_func/main_2dBK.cpp $(objects)
	$(CC) main_func/main_2dBK.cpp -o 2dbk.lnxpapp $(objects) $(ROOTINC) $(ROOTLIBS)
	
RateVsPhi_online : main_rateVsPhi_online.o $(objects)
	$(CC) -o RateVsPhi_Online.lnxapp main_rateVsPhi_online.o $(objects) $(ROOTLIBS) 

Print_offline_db_text_files : main_print_offline_db_files.o $(objects)
	$(CC) -o PrintOfflineRollTextFiles.lnxapp main_print_offline_db_files.o $(objects) $(ROOTLIBS)

Print_online_db_text_files : main_print_online_db_files.o $(objects)
	$(CC) -o PrintOnlineRollTextFiles.lnxapp main_print_online_db_files.o $(objects) $(ROOTLIBS) 

GetEcapPlusMinusRatios : $(objects)
	$(CC) -c main_func/main_Ecap_assym.cpp $(ROOTINC)
	$(CC) -o GetEcapPlusMinusRatios.lnxapp main_Ecap_assym.o $(objects) $(ROOTLIBS)

GetRatioOfChambersForTwoRuns : $(objects)
	$(CC) -c main_func/main_chambersRatiosTwoRuns.cpp $(ROOTINC)
	$(CC) -o GetRatioOfChambersForTwoRuns.lnxapp main_chambersRatiosTwoRuns.o $(objects) $(ROOTLIBS)  

CalculateLumiForRunWithFitCoefficientRefference : $(objects)
	$(CC) -c main_func/main_calculateLumiForRunWithFitCoefficientRefference.cpp $(ROOTINC)
	$(CC) -o CalculateLumiForRun.lnxapp main_calculateLumiForRunWithFitCoefficientRefference.o $(objects) $(ROOTLIBS) 

RecoverLumiForEachRunWithCoeffAndRateFiles : $(objects)
	$(CC) -c main_func/main_recoverLumiForEachRunWithCoeffAndRateFiles.cpp $(ROOTINC)
	$(CC) -o RecoverLumiForRuns.lnxapp main_recoverLumiForEachRunWithCoeffAndRateFiles.o $(objects) $(ROOTLIBS) 

Tutorial: Tutorial.cpp $(objects)
	$(CC) Tutorial.cpp -o Tutorial.o $(objects) $(ROOTINC) $(ROOTLIBS)
	
	
# Michele applications

fill:
	$(CC) core/Roll.cpp Extensions/Run.cpp Extensions/Acquisition.cpp core/Strip.cpp core/DataObject.cpp core/ExtendedRoll.cpp core/ExtendedStrip.cpp main_func/fillContainer.cpp Chip.o NoiseAnalyzer.o $(ROOTINC) $(FLAGS) -o Fill $(ROOTLIBS) $(LIBJSON)

Fill:  Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o Fill fillContainer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

Read: Roll.o Run.o Strip.o DataObject.o ExtendedRoll.o Chip.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o readContainer.o NoiseAnalyzer.o
	$(CC) -o Read readContainer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

ReadFromDO: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o fillContainer.o Acquisition.o NoiseAnalyzer.o readContainerFromDO.o
	$(CC) -o ReadDO readContainerFromDO.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

lumi: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o producePlot.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o lumi producePlot.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

import: Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o importFromTextResources.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o import importFromTextResources.o Roll.o Run.o Strip.o DataObject.o  ExtendedRoll.o ExtendedStrip.o Chip.o NoiseAnalyzer.o Acquisition.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pplot: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o producePlot.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o plot producePlot.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

asim: Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o studyAsimmetry.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o asym studyAsimmetry.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o Service.o Applications.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pphiz: plotOnPhiVaryZ.o Roll.o Run.o Strip.o DataObject.o Chip.o  ExtendedRoll.o ExtendedStrip.o  Acquisition.o NoiseAnalyzer.o
	$(CC) -o pphiz plotOnPhiVaryZ.o Roll.o Run.o Strip.o DataObject.o  Chip.o ExtendedRoll.o ExtendedStrip.o  Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

noise: mainNoise.o NoiseAnalyzer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o 
	$(CC) -o noise mainNoise.o NoiseAnalyzer.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o  $(ROOTLIBS) $(LIBJSON) $(FLAGS)

pphi: plotOnPhi.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o pphi plotOnPhi.o Roll.o Run.o Strip.o DataObject.o Chip.o ExtendedRoll.o ExtendedStrip.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON) $(FLAGS)

crawler: Crawler.o mainCrawler.o DataObject.o
	$(CC) -o crawl Crawler.o mainCrawler.o DataObject.o  $(ROOTLIBS)

runp: printRunProperties.o DataObject.o Run.o ExtendedRoll.o ExtendedStrip.o Chip.o Strip.o Roll.o Acquisition.o NoiseAnalyzer.o
	$(CC) -o runp printRunProperties.o DataObject.o Run.o Chip.o ExtendedRoll.o ExtendedStrip.o Strip.o Roll.o Acquisition.o NoiseAnalyzer.o $(ROOTLIBS) $(LIBJSON)

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

