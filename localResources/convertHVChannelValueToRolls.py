import json
import sys

if __name__ == "__main__":

    RollToHVchMapFile = sys.argv[1] # RollToHVchannelMap.json
    CurrentsRatiosPerHVchfilename = sys.argv[2] # your input
    
    with open(RollToHVchMapFile) as hvtrf:
        RollToHVchMap = json.loads(hvtrf.read())

    with open(CurrentsRatiosPerHVchfilename) as crph:
        HVchCurrentsRatios = json.loads(crph.read())

    result = {}
        
    for i in RollToHVchMap:
        if RollToHVchMap[i] in HVchCurrentsRatios:
            HVch = RollToHVchMap[i]
            value = HVchCurrentsRatios[HVch]
            if value == "NAN": #there were NAN entries
                value = 0
            result.update({i:value})
    
    with open("CurrentsRatiosByRoll.json",'w') as CRBR:
        CRBR.write(json.dumps(result,indent=1, sort_keys=True))
