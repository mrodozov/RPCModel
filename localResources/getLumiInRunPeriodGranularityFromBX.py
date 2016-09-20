import os
import json
import time
import re
import sys

def getDeliveredLumiEntriesFromLine(line):

    line = line[line.index('[1'):line.index(']')]
    line_list = line.split()
    line_list = line_list[1::3]
    line_list_floats = [float(e) for e in line_list]
    return line_list_floats

if __name__ == "__main__":
    #print 'u r cunt'

    numberOfperiods = int(sys.argv[1])
    lumiFile = sys.argv[2]

    hugeFile = lumiFile
    nlinesToRead = 8

    allBXlumies = []

    with open(hugeFile,'r') as hf:
        lines = hf.readlines()
        for l in lines:
            if not '#' in l and l is not '\n':
                ll = getDeliveredLumiEntriesFromLine(l)
                allBXlumies += ll

    sz = len(allBXlumies)/(numberOfperiods-1)
    lst = allBXlumies
    sortedBXlumies = [lst[i:i+sz] for i in range(0, len(lst), sz)]
    
    for i in range(len(sortedBXlumies)):
        lumiSum = 0
        for j in sortedBXlumies[i]:
            lumiSum += j

        print i+1, lumiSum

