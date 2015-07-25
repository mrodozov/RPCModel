__author__ = 'rodozov'


import sys, json, subprocess, os

class Chain(object):

    '''
    this class is chain that put commands in list, and executes them in order.

    '''

    def __init__(self, options=None):
        self.commands = []
        self.options = options
        self.log = {}
        self.sterr = None
        self.stout = None
        self.runnumber = None

    def add_command(self, command):
        self.commands.append(command)

    def addListOfCommands(self,commandsList):
        for command in commandsList: self.add_command(command)

    def execute_chain(self, optionsObj):
        #opto = {}
        #opto = optionsObj
        #print json.dumps(optionsObj)
        retval = False
        for commnd in self.commands:
            success = commnd.execute(optionsObj)
            #print json.dumps(optionsObj)
            retval = success
            print commnd.name

            print success
            if not success: break
        self.collectLogs()
        return retval

    def collectLogs(self):
        for command in self.commands:
            cmndname = command.name
            self.log[cmndname] = command.log
            #self.stout[cmndname] = command.stout
#            self.sterr[cmndname] = command.sterr


if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root' # important
    print os.environ['LD_LIBRARY_PATH']


    dbschema = {}
    optionsObject = {}
    with open('resources/options_object.txt', 'r') as optobj:
        optionsObject = json.loads(optobj.read())
    with open('resources/db_tables_schema.txt', 'r') as dbschemafile:
        dbschema = json.loads(dbschemafile.read())
    optionsObject['dbdataupload']['args']['dbResources'] = dbschema
    optionsObject['run'] = '220796'

    runchain = Chain(optionsObject)



