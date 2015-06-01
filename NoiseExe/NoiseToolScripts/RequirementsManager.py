__author__ = 'rodozov'

import os,subprocess,os.path,json,copy

#class ProcessDescriptor:

class SSHTunnelDescriptor:
    def __init__(self,tName=None,options=None,localHost=None,localPort=None,remoteHost=None,remotePort=None,debug=False):
        self.localHost = localHost
        self.localPort = localPort
        self.remoteHost = remoteHost
        self.remotePort = remotePort
        self.tunnelName = tName
        self.options = options
        self.debug = debug
    def tunnelString(self):
        t =  str(self.localPort) + ':' + str(self.remoteHost) + ':' + str(self.remoteHost) + ' ' + str(self.localHost)
        if self.debug:
            print t

        return t
    def __del__(self):
        if self.debug:
            print 'SSHDescriptorDestructor called for tunnel', self.tunnelName

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

        if self.debug:
            print 'tunnels file', fileWithTunnels
            print 'env vars file', fileWithEnvVars
            print 'debug in  EnvHandler __init__' #not set yet

    def checkTunnel(self, sshTunnel):
        tstring = sshTunnel.tunnelString()
        p = subprocess.Popen("ps -ef | grep ssh | grep '"+tstring+"' | grep -v grep", shell = True, stdout = subprocess.PIPE)
        out, err = p.communicate()
        retval = False
        if not out:
            retval = False
            if self.debug:
                print 'output for', sshTunnel.tunnelName, 'is', out
        else:
            retval = True
        return retval

    def checkListOfTunnels(self):
        for tunnel in self.listOfTunnels:
            tunnelCheck = self.checkTunnel(tunnel)
            if not tunnelCheck:
                #self.start(tunnel) # do not execute when in dev mode, it would start ssh tunnels
                if self.debug:
                    print 'starting tunnel', tunnel.tunnelName

    
    def start(self, tunnel):


        return True

    def initConfigs(self, listConfigs):
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

                #not valid ? why # because you are dumb f**k and check your last commit to see that there was no lHost rHost members, moron
                sshDescriptor = SSHTunnelDescriptor(
                dic['name'],
                dic['options'],
                dic['lhost'],
                dic['lport'],
                dic['rhost'],
                dic['rport'],
                self.debug
                )

                self.listOfTunnels.append(sshDescriptor)

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

