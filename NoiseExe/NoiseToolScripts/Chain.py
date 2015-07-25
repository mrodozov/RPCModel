__author__ = 'rodozov'

from CommandClasses import GetListOfFiles,CheckIfFileIsCorrupted, NoiseToolMainExe

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

    def execute_chain(self):
        retval = False
        for commnd in self.commands:
            success = commnd.execute(self.options)
            retval = success
            if not success: break
        self.collectLogs()
        return retval

    def collectLogs(self):
        for command in self.commands:
            cmndname = command.name
            self.log[cmndname] = command.log
            self.stout[cmndname] = command.stout
            self.sterr[cmndname] = command.sterr




if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root' # important
    print os.environ['LD_LIBRARY_PATH']

    runoptions = {}

    aChain = Chain(runoptions)
    argss = ['resources/CheckCorruptedFile.lnxapp','resources/Histos_YEP3_near_run_220816_2014_4_8__14_55_16.root']
    listFiles = GetListOfFiles(argss)


    listOfCommands = [listFiles,CheckIfFileIsCorrupted(),NoiseToolMainExe()]
