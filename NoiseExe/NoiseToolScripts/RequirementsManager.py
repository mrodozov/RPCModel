__author__ = 'rodozov'

import os,subprocess,os.path,json,copy

#class ProcessDescriptor:

class SSHTunnelDescriptor:
    def __init__(self,tName=None,options=None,localHost=None,localPort=None,remoteHost=None,remotePort=None):
        self.localHost = localHost
        self.localPort = localPort
        self.remoteHost = remoteHost
        self.remotePort = remotePort
        self.tunnelName = tName
        self.options = options
    def tunnelString(self):
        t =  'ssh ' + str(self.options) + ' ' + str(self.localPort) + ':' + str(self.remoteHost) + ':' + str(self.remoteHost) + ' ' + str(self.localHost)
        print t
        return t

class EnvHandler:

    listOfTunnels = []
    listOfProcesses = []
    debug = False

    '''
    class SSHTunnelDescriptor:
        def __init__(self,tName=None,options=None,localHost=None,localPort=None,remoteHost=None,remotePort=None):
            self.__localHost = localHost
            self.__localPort = localPort
            self.__remoteHost = remoteHost
            self.__remotePort = remotePort
            self.__tunnelName = tName
            self.__options = options
        def lHost(self):
            return self.__localHost
        def lPort(self):
            return self.__localPort
        def name(self):
            n = self.__tunnelName
            return n
        def rHost(self):
            return self.__remoteHost
        def rPort(self):
            return self.__remotePort
        def optionsList(self):
            return self.__options
    '''

    def __del__(self):
        if self.debug:
            print 'dunno , print ?'

    def __init__(self,fileWithTunnels,fileWithEnvVars,debug=False):
        self.listOfTunnels = []
        self.listOfProcesses = []
        self.debug = debug
        self.initConfigs(fileWithTunnels)
        self.checkListOfTunnels()

        print fileWithEnvVars

        if debug:
            print 'some debug' #not set yet

    def checkTunnel(self,sshTunnel):
        tstring = 'ssh ' + sshTunnel.options + ' ' + sshTunnel.localPort + ':' + sshTunnel.remoteHost + ':' + sshTunnel.remoteHost + ' ' + sshTunnel.localHost
        print tstring

    def checkListOfTunnels(self):
        for tunnel in self.listOfTunnels:
            tunnelCheck = self.checkTunnel(tunnel)
            if not tunnelCheck:
                self.start(tunnel)
    
    def start(self,tunnel):

        return True

    def initConfigs(self,listConfigs):
        #file better be JSON with similar object attributes
        if not os.path.isfile(listConfigs):
            raise Exception("tunnels config file not found or empty")
        try:
            #check each line structure
            dictOfTunnel = []
            f = open(listConfigs,'r')
            for tunnelConfig in f:
                dic = json.loads(tunnelConfig)
                dictOfTunnel.append(dic)
                sshDescriptor = SSHTunnelDescriptor()
                sshDescriptor.lHost = dic['lhost']
                sshDescriptor.rHost = dic['rhost']
                sshDescriptor.rPort = dic['rport']
                sshDescriptor.lPort = dic['lport']
                sshDescriptor.name = dic['name']
                sshDescriptor.optionsList = dic['options']
                ''' #not valid ? why
                sshDescriptor = SSHTunnelDescriptor(
                dic['name'],
                dic['options'],
                dic['lhost'],
                dic['lport'],
                dic['rhost'],
                dic['rport']
                )
                '''
                self.listOfTunnels.append(sshDescriptor)
            '''
            #for i in dictOfTunnel:
            #   print i
            #print len(self.__listOfTunnels)
            #for sshDescriptor in self.listOfTunnels():
            #    print sshDescriptor.name
            '''

        except Exception, e:
            return e.message


    def initEnvVars(self,listVars):
        #file could be json with name:value attributes
        if not os.path.isfile(listVars) or os.path.getsize(listVars) == 0:
            return 'vars config file not found or empty'
        try:
            f = open(listVars,'r')
            #check structure of file and then if all variables are set. if not set, reset

        except Exception, e:
            return e.message

