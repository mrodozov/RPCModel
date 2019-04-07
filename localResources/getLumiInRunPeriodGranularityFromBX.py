import os
import json
import time
import re
import sys
import datetime


def getDeliveredLumiEntriesFromLine(line):

    firstidx = line.index("[1")
    lastidx = line.index("]")
    line = line[firstidx:lastidx]
    line_list = line.split()
    line_list = line_list[1::3]
    line_list_floats = [float(e) for e in line_list]
    return line_list_floats


if __name__ == "__main__":
    #print 'u r cunt'

    #fileWithChambersTimes = sys.argv[1]
    #hugeFile = sys.argv[2]
    chambersInfo = {}
    runnum = sys.argv[1]

    fileWithChambersTimes = 'chambers_delta_times_' + runnum + '.json'
    
    hugeFile = runnum

    allBXlumies = []

    lumiStartTime = None

    with open(hugeFile,'r') as hf:
        lines = hf.readlines()
        for l in lines:
            #print l
            if '[1' in l :
                if lumiStartTime is None:
                    spline = l.split(',')
                    lumiStartTime = spline[2]
                    print lumiStartTime
                ll = getDeliveredLumiEntriesFromLine(l)
                allBXlumies += ll

    chambersLumiSequences = {}

    with open(fileWithChambersTimes) as chamTimes:
        chambersInfo = json.loads(chamTimes.read())

    for chamber in chambersInfo:
        times = chambersInfo[chamber]['times']
        chamberStartTime = chambersInfo[chamber]['startTime']
        timeOffset = int( ( datetime.datetime.strptime(chamberStartTime,'%m/%d/%y %H:%M:%S') - datetime.datetime.strptime(lumiStartTime,'%m/%d/%y %H:%M:%S') ).total_seconds() )
        timeSum = timeOffset
        #print chamber, timeOffset
        chambersLumiSequences.update({chamber:[]})
        timeIntervalNum = 0
        #print chamber, len(times)
        for delta_duration in times:

            offsetInFractions = int( float(timeOffset) / 0.00654 )
            delta_durationInFractions = int ( float(delta_duration) / 0.00654 )

            lumilist = allBXlumies[offsetInFractions:(offsetInFractions + delta_durationInFractions)]
            lumisum = sum(lumilist)
            #print chamber, timeOffset, delta_duration, (timeOffset+delta_duration), lumisum , timeIntervalNum+1
            if delta_duration == 0:
                chambersLumiSequences[chamber].append(0)
            else:
                chambersLumiSequences[chamber].append(round(lumisum/delta_duration, 4)) # to normalize the lumi on the time
            timeOffset += delta_duration + 1
            timeIntervalNum += 1

    with open ('chambers_lumis_'+runnum+'.json','w') as chLumies:
        chLumies.write(json.dumps(chambersLumiSequences ,indent=1 , sort_keys=True))

