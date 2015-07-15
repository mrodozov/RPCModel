__author__ = 'rodozov'

'''

Abstract command class and different implementations, representing
independent implementations of command objects, to be put in the
command queue.

TODO - Many subroutines in execute() method are repetitive.
Try substitute them with a method

'''

import sys, json, subprocess, os, glob


class Command(object):

    args = None
    log = None
    stout = None
    sterr = None
    name = None
    exitcode = None
    options = None

    def __init__(self, args=None, name=None):
        self.stout = None
        self.sterr = None
        self.name = name
        if args == None or self.args == None:
            self.args = args
            self.exitcode = "NOARGS"
            self.log = 'no arguments given' # some standard errors enum, maybe ?
        #self.execute(opts_to_update)

    def __del__(self):
        pass

    def execute(self,optionsToUpdate):
        raise NotImplementedError

    def getStdErr(self):
        raise NotImplementedError

    def getLog(self):
        raise NotImplementedError

class GetListOfFiles(Command):

    def getTowerNameFromFileName(self,filename):
        parts = filename.split('_')
        return parts[1]+'_'+parts[2]

    def execute(self,optionsToUpdate):
        retval = False
        listresults = []
        if optionsToUpdate is not  None:
            self.args = optionsToUpdate[self.name]['args']
            filespath = self.args[0]
            runnum = optionsToUpdate['run']
            towers = self.args[1]
            #print runnum
            files = [f for f in os.listdir(filespath) if f.endswith('.root') and f.find(runnum) is not -1 ]
            #print towers
            shortlist = []
            if len(files) > 18:
                towerMap = {}
                for t in towers:
                    towerMap[t] = {'file':None,'size':0}
                for f in files:
                    tf = self.getTowerNameFromFileName(f)
                    #print tf
                    statinfo = os.stat(filespath+'/'+f)
                    fsize = statinfo.st_size
                    #print f, fsize
                    if towerMap[tf]['file'] is None or fsize > towerMap[tf]['size']:
                        towerMap[tf] = {'file':f,'size':fsize}
                for k in towerMap.keys():
                    shortlist.append(towerMap[k]['file'])
                #print towerMap
                files = shortlist
            #print files
            filespath += '/'
        optionsObject[self.name]['results'] = [filespath+f for f in files]


class CheckIfFilesAreCorrupted(Command):

    def execute(self,optionsToUpdate=None):

        retval = False
        listresult = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options =  optionsToUpdate[optionsToUpdate[self.name]['source']]['results']

            for file in self.options:
                executable = self.args
                childp = subprocess.Popen(executable+' '+file,shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
                self.stout, self.sterr = childp.communicate()
                self.exitcode = childp.returncode
                listresult[file]={'sterr': self.sterr,'stout': self.stout,'exitcode': self.exitcode}

                if self.exitcode == 0 and self.sterr is None:
                    retval = True

            self.log = listresult
            if retval is not True: self.exitcode = 1
            self.log = listresult

            optionsToUpdate[self.name]['results'] = listresult
            optionsToUpdate[self.name]['exitcode'] = self.exitcode
            optionsToUpdate[self.name]['log'] = self.log

            #print 'exit code', self.exitcode
        return retval

class NoiseToolMainExe(Command):

    '''
    Noise tool main executable command
    If the file check is passed, it process the files (one by one ? really ?)
    '''

    def execute(self,optionsToUpdate):

        retval = False
        results = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
            filesToProcess = [k for k, v in self.options.iteritems() if v['exitcode'] is not '0' ]
            #print filesToProcess # for debug
            executable = self.args[0]
            arguments = self.args[1]+' '+self.args[2]+' '+self.args[3]
            for f in filesToProcess:
                #print executable, f, arguments
                childp = subprocess.Popen(executable+' '+f+' '+arguments,shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
                current_stdout, current_stderr = childp.communicate()
                current_excode = childp.returncode
                results[f] = {'sterr': current_stderr, 'exitcode': current_excode, 'stout' : current_stdout}
                if current_excode == 0 and current_stderr is None:
                    retval = True
            #so far, and thanks for all the fish

        return retval

class DBInputPrepare(Command):

    '''

    '''
    def mergeInputFilesByName(self,fileDir,outputFileName,substringToSearch,orderingList,exploder):
        listOfFiles = [f for f in os.listdir(fileDir) if f.endswith('.txt') and f.find(substringToSearch) is not -1 ]
        print listOfFiles
        with open(fileDir+outputFileName,'w') as outputFile:
            if listOfFiles:
                for f in listOfFiles:
                    with open(fileDir+f,'r') as data_file:
                        lines = data_file.readlines()
                        for l in lines :
                            lineToWrite = ''
                            listElements = l.split(exploder)
                            if len(listElements) > 2:
                                for orderElement in orderingList:
                                    lineToWrite += listElements[orderElement]
                                    lineToWrite += ' '
                            lineToWrite += '\n'
                            outputFile.write(lineToWrite)

    def execute(self,optionsToUpdate):
        retval = False
        results = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            #self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
            executable = self.args[0]
            resourcesDir = self.args[1]
            resultsDir = self.args[2]
            fileToSearch = self.args[3:7]
            for f in fileToSearch:
                print f
                self.mergeInputFilesByName(resultsDir,'All'+f+'.txt',f,[6,3],' ')


'''

commands that could be executed independent from one another.
So far, make them in a in a queue, and when

'''

class DBDataUpload(Command):

    def __init__(self):
        pass

class OutputFilesFormat(Command):

    def __init__(self):
        pass

class WebResourcesFormat(Command):
    def execute(self,optionsToUpdate):
        pass

class GarbageRemoval(Command):
    '''
    Compress or erase all outputs, to be executed after all the outputs are finished
    '''
    def __init__(self):
        pass

class OldToNewDataConverter(Command):
    '''
    Converts old resources to new type
    '''


    def __init__(self):
        pass

class NewToOldDataConverter(Command):

    '''
    converts new resources to the old type
    '''

    def __init__(self):
        pass

if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root' # important
    print os.environ['LD_LIBRARY_PATH']

    towers = ["RB+2_far", "RB+2_near", "RB+1_near", "RB+1_far", "RB-1_far", "RB-1_near", "RB-2_far", "RB-2_near", "RB0_far", "RB0_near", "YEN1_far", "YEN1_near", "YEN3_far", "YEN3_near", "YEP1_far",  "YEP1_near", "YEP3_far", "YEP3_near"]

    optionsObject = {'run':'','filelister':{'args':['resources',towers],'source':'','results':''},
                     'check':{'args':'resources/CheckCorruptedFile.lnxapp','source':'filelister','results':''},
                     'noiseexe':{'args':['resources/LBNoise','resources/resources/lb-chamber-withRE4.txt','resources/resources/area_noise_withRE4.txt',
                                         'resources/results'],'source':'check','results':''},
                     'dbinput':{'args':['dbexecutable','resources/','resources/results/','Masked','Dead','ToUnmask','ToMask'],'source': 'noiseexe', 'results': ''}
                     }


    getFiles = GetListOfFiles(name='filelister')
    getFiles.execute(optionsObject)

    fileIsCorrupted = CheckIfFilesAreCorrupted(name='check')
    passit = fileIsCorrupted.execute(optionsObject)

    #noiseExe = NoiseToolMainExe(name='noiseexe')
    #noisepassed = noiseExe.execute(optionsObject)

    print optionsObject['dbinput']['args']

    dbInput = DBInputPrepare(name='dbinput')
    dbinpass = dbInput.execute(optionsObject)




    #print optionsObject

    '''

    argss = ['resources/CheckCorruptedFile.lnxapp','resources/Histos_YEP3_near_run_220816_2014_4_8__14_55_16.root']
    objWithOptions = {}
    fileIsCorrupted = CheckIfFileIsCorrupted(argss,"check",objWithOptions)
    retval = fileIsCorrupted.execute(objWithOptions)
    #print retval
    print fileIsCorrupted.stout, fileIsCorrupted.sterr
    print objWithOptions

    '''


