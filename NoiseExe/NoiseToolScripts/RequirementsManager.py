#__author__ = 'rodozov'

import os,subprocess

#class SSHTunnelDescriptor:

#class ProcessDescriptor:

class EnvHandler:
    
    def __init__(self):
        self.listOfTunnels = []
        
    def __init__(self,fileWithTunnels,fileWithProcesses):
        self.listOfTunnels = []
        self.listOfProcesses = []
        
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
