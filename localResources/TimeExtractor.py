'''
Original author: Mircho Rodozov mrodozov@cern.ch

This code is used to extract the creation time and the stop time 
for given run number 

@package TimeExtractor
'''

import json,sys,subprocess,os,datetime,time,string

def getRunlistWithGoodRunsFromJSONfile(file,lumisectionsCut):
  json_data = open(file,'r')
  data = json.load(json_data)
  json_data.close()
  
  result_array = []
  lumiSectionsCounter = 0
  
  for i in data:
    for j in data[i]:
      lumiSectionsCounter += j[1] - j[0] + 1
    #print i," ",data[i]
    #print i,lumiSectionsCounter
    
    if (lumiSectionsCounter >= int(lumisectionsCut)):
      
      #print i,lumiSectionsCounter
      result_array.append(str(i))
    
    #reset the counter
    lumiSectionsCounter = 0
    
  result_array.sort()
  return result_array

def getExistingRunsFromFile(file):
  existing_runs = open(file,'r')
  arrayOfFileLines = []
  arrayOfRuns = []
  oneLine = []
  arrayOfFileLines = existing_runs.readlines()
  
  for i in arrayOfFileLines:
    oneLine = i.split()
    arrayOfRuns.append(oneLine[0])
    
  arrayOfRuns.sort()
  return arrayOfRuns
  

def getDifferenceBetweenTwoLists(firstList,secondList):
  missing_runs = []
  missing_runs = list(set(firstList) - set(secondList))
  missing_runs.sort()
  return missing_runs


def getEndTimeForAFileWithName(fileName):
  
  command = ["stat", fileName]
  p = subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
  text = p.stdout.read()
  textArray = []
  
  textArray = text.split('\n')
  
  creationTimeString = ""
  
  for i in textArray:
    if "Modify" in i:
      creationTimeString = i
  
  textArray = creationTimeString.split()
  
  date = textArray[1]
  year = date[:4]
  month = date[5:][:2]
  day = date[5:][3:]
  time = textArray[2][:8]
  hours= time[:2]
  minutes = time[:5][3:]
  seconds = time[6:]
  offset = int(textArray[3][:3])
  
  #print date,time,offset
  #print year,month,day,hours,minutes,seconds
    
  correctedDate = datetime.datetime(int(year),int(month),int(day),int(hours),int(minutes),int(seconds))
  correctedDate = correctedDate - datetime.timedelta(0,0,0,0,0,int(offset),0)
  
  correctedDate = datetime.datetime.strftime(correctedDate,'%m/%d/%y %H:%M:%S')
  
  returnArray = [str(correctedDate),offset]
  
  return returnArray


def getTimeForRunNumber(runnumber):
  
  #first , check if the directory exists
  
  directory = "resources"
  if not os.path.exists(directory):
    #print 'Dir not existing, terminate ...'
    return  False
    #just terminate for this run number
  #else:
    #print 'Directory exists !'
  
  command = ["ls",directory]
  p = subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
  returnValue = True
  
  text = p.stdout.read()
  textArray = text.split('\n')
  noiseFile = ""
  for i in textArray:
    if "Noise" in i:
      noiseFile = i
      returnValue = True
      break
    else:
      returnValue = False
  
  #check if Noise string has been matched at least once. If not, terminate
  if not returnValue:
    return False
  
  datetimeArray = []
  dateArray = []
  timeArray = []
  
  # now, with the name of the file - get the end time of the file and the offset
  
  #change the directory to LBMonitorHistos
  
  if not os.path.exists(directory+"/"+"Histos"+noiseFile[5:]):
    print 'histos dir does not exists'
    return False
  
  endFile = directory+"/"+"Histos"+noiseFile[5:]
  #print endFile
  
  dateEndFile = getEndTimeForAFileWithName(endFile)
  dateStringEndFile = dateEndFile[0]
  offset = dateEndFile[1]
  
  if not str(runnumber) in noiseFile:
    print 'runnumber not found in the directory substrings for run',runnumber
    return False
    #terminate
  
  cuttedFile = noiseFile[string.find(noiseFile,str(runnumber)) + len(str(runnumber)) + 1:]
  cuttedFile = cuttedFile[:string.find(cuttedFile,str(".root"))]
  #print cuttedFile
  
  datetimeArray = cuttedFile.split("__")
  dateArray = datetimeArray[0].split("_")
  timeArray = datetimeArray[1].split("_")
  #print datetimeArray[0],datetimeArray[1]
  dateStringStartFile = datetime.datetime(int(dateArray[0]),int(dateArray[1]),int(dateArray[2]),int(timeArray[0]),int(timeArray[1]),int(timeArray[2]))
  dateStringStartFile = dateStringStartFile - datetime.timedelta(0,0,0,0,0,int(offset),0);
  dateStringStartFile = datetime.datetime.strftime(dateStringStartFile,'%m/%d/%y %H:%M:%S')
  
  fullString = str(runnumber)+ " " + str(dateStringStartFile) + " " + dateStringEndFile
  
  #print dateStringEndFile
  #print fullString
  
  return fullString
  
def appendTextToFile(file,text):
  
  fp = open(file,'a')
  text = text+'\n'
  fp.write(text)
  fp.close()
  
#here the program goes
  
JSONfile = sys.argv[1]
lumiSectionsCut = sys.argv[2]
existingTimesFile = sys.argv[3]

arr = getRunlistWithGoodRunsFromJSONfile(JSONfile,lumiSectionsCut)
runs = getExistingRunsFromFile(existingTimesFile)
missing_runs = getDifferenceBetweenTwoLists(arr,runs)

print missing_runs

for i in missing_runs:
  entryToAdd = getTimeForRunNumber(i)
  if entryToAdd:
    appendTextToFile(existingTimesFile,entryToAdd)
    

