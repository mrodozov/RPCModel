# application to carry the chain
# creates the objects required, hook them up and follows a logic
#

import json
from RequirementsManager import EnvHandler

eHandler = EnvHandler('resources/ListOfTunnels.json','resources/process.json' ,'resources/variables.json', False)

tunnels = eHandler.listOfTunnels
processes = eHandler.listOfProcesses
evars = eHandler.listOfEnvVars

eHandler.checkListOfTunnels()
eHandler.checkListOfProcesses()

for p in processes:
    print p.name

for ev in evars:
    for k, v in ev.items():
        print k,v