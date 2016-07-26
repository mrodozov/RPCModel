import json
import sys

if __name__ == "__main__":

    RollToHVchMapFile = sys.argv[1]
    CurrentsRatiosPerHVchfilename = sys.argv[2]    
    
    with open(RollToHVchMapFile) as hvtrf:
        RollToHVchMap = json.loads(hvtrf.read())

    with open(CurrentsRatiosPerHVchfilename) as crph:
        HVchCurrentsRatios = json.loads(crph.read())

    result = {}
        
    for i in RollToHVchMap:
        if RollToHVchMap[i] in HVchCurrentsRatios:
            HVch = RollToHVchMap[i]
            result.update({i:HVchCurrentsRatios[HVch]})
    
    with open("CurrentsRatiosByRoll.json",'w') as CRBR:
        CRBR.write(json.dumps(result,indent=1, sort_keys=True))
