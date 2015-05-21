#__author__ = 'rodozov'

import os,subprocess,os.path,json

#class SSHTunnelDescriptor:

#class ProcessDescriptor:

class EnvHandler:

    class SSHTunnelDescriptor:
        def __init__(self):

        def __init__(self,tName,localHost,localPort,remoteHost,remotePort):
            self.__localHost = localHost
            self.__localPort = localPort
            self.__remoteHost = remoteHost
            self.__remotePort = remotePort
            self.__tunnelName = tName

    def __init__(self):
        self.listOfTunnels = []
    def __del__(self):

    def __init__(self,fileWithTunnels,fileWithEnvVars,debug=False):
        self.listOfTunnels = []
        self.listOfProcesses = []

        if debug:
            print self.__class__.__name__ , "destroyed"

    def checkTunnel(self,tunnelString):
        tunnelString = ''
        
    def checkProcess(self,process):
        process = ''
        
    def checkListOfTunnels(self):
        for tunnel in self.listOfTunnels:
            tunnelCheck = self.checkTunnel(tunnel)
            if not tunnelCheck:
                self.restart(tunnel)

    def chekListOfProcesses(self):
        for process in self.listOfProcesses:
            processCheck = self.checkProcess()
            if not processCheck:
                self.restart(process)
    
    def restart(self,process):
        return True

    def initConfigs(self,listConfigs):
        #file better be JSON with similar object attributes
        if not os.path.isfile(listConfigs):
            return 'tunnels config file not found or empty'
        try:
            f = open(listConfigs,'r')
            #check each line structure
            arrayOfLines = f.readlines()
            #put each line in dictionary
            for tunnelConfig in arrayOfLines:


        except Exception, e:
            return e.message

    def initEnvVars(self,listVars):
        #file could be json with name:value attributes
        if not os.path.isfile(listVars) or os.path.getsize(listVars) == 0:
            return 'vars config file not found or empty'
        try:
            f = open(listVars,'r')
            #check structure of file and then if all variables are set. if not set, reset




