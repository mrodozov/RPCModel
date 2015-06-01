# application to carry the chain
# creates the objects required, hook them up and follows a logic
#

from RequirementsManager import EnvHandler


eHandler = EnvHandler('resources/ListOfTunnels.json', 'resources/variables.json', True)
tunnels = eHandler.listOfTunnels
#eHandler.checkListOfTunnels()
for i in tunnels:
    print i.tunnelName, i.localPort

