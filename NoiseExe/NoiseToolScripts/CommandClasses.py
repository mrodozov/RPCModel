__author__ = 'rodozov'

'''

Abstract command class and different implementations, representing
independent implementations of command objects, to be put in the
command queue.

TODO - Many subroutines in execute() method are repetitive.
Try substitute them with a method

'''

import subprocess
import os
import time
import re
from DBService import DBService

# TODO - Write results, log for each class, where results should be the result the command produced or checked

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
        if args is None or self.args is None:
            self.args = args
            self.exitcode = "NOARGS"
            self.log = 'no arguments given'  # some standard errors enum, maybe ?
            # self.execute(opts_to_update)

    def __del__(self):
        pass

    def execute(self, optionsToUpdate):
        raise NotImplementedError

    def getStdErr(self):
        raise NotImplementedError

    def getLog(self):
        raise NotImplementedError


class GetListOfFiles(Command):
    def getTowerNameFromFileName(self, filename):
        parts = filename.split('_')
        return parts[1] + '_' + parts[2]

    def execute(self, optionsToUpdate):
        retval = False
        listresults = []
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            filespath = self.args[0]
            runnum = optionsToUpdate['run']
            towers = self.args[1]
            # print runnum
            files = [f for f in os.listdir(filespath) if f.endswith('.root') and f.find(runnum) is not -1]
            # print towers
            shortlist = []
            if len(files) > 18:
                towerMap = {}
                for t in towers:
                    towerMap[t] = {'file': None, 'size': 0}
                for f in files:
                    tf = self.getTowerNameFromFileName(f)
                    # print tf
                    statinfo = os.stat(filespath + '/' + f)
                    fsize = statinfo.st_size
                    # print f, fsize
                    if towerMap[tf]['file'] is None or fsize > towerMap[tf]['size']:
                        towerMap[tf] = {'file': f, 'size': fsize}
                for k in towerMap.keys():
                    shortlist.append(towerMap[k]['file'])
                # print towerMap
                files = shortlist
            # print files
            filespath += '/'
        optionsObject[self.name]['results'] = [filespath + f for f in files]


class CheckIfFilesAreCorrupted(Command):
    def execute(self, optionsToUpdate=None):

        retval = False
        listresult = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']

            for file in self.options:
                executable = self.args
                childp = subprocess.Popen(executable + ' ' + file, shell=True, stdout=subprocess.PIPE,
                                          stderr=subprocess.STDOUT, close_fds=True)
                self.stout, self.sterr = childp.communicate()
                self.exitcode = childp.returncode
                listresult[file] = {'sterr': self.sterr, 'stout': self.stout, 'exitcode': self.exitcode}

                if self.exitcode == 0 and self.sterr is None:
                    retval = True

            self.log = listresult
            if retval is not True: self.exitcode = 1
            self.log = listresult

            optionsToUpdate[self.name]['results'] = listresult
            optionsToUpdate[self.name]['exitcode'] = self.exitcode
            optionsToUpdate[self.name]['log'] = self.log

            # print 'exit code', self.exitcode
        return retval


class NoiseToolMainExe(Command):
    '''
    Noise tool main executable command
    If the file check is passed, it process the files (one by one ? really ?)
    '''

    def execute(self, optionsToUpdate):

        retval = False
        results = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
            filesToProcess = [k for k, v in self.options.iteritems() if v['exitcode'] is not '0']
            # print filesToProcess # for debug
            executable = self.args[0]
            arguments = self.args[1] + ' ' + self.args[2] + ' ' + self.args[3]
            for f in filesToProcess:
                # print executable, f, arguments
                childp = subprocess.Popen(executable + ' ' + f + ' ' + arguments, shell=True, stdout=subprocess.PIPE,
                                          stderr=subprocess.STDOUT, close_fds=True)
                current_stdout, current_stderr = childp.communicate()
                current_excode = childp.returncode
                results[f] = {'sterr': current_stderr, 'exitcode': current_excode, 'stout': current_stdout}
                if current_excode == 0 and current_stderr is None:
                    retval = True
                    # so far, and thanks for all the fish

        return retval


class DBInputPrepare(Command):
    '''

    '''
    # TODO - merge the .root files into total.root, from the executable itself


    def mergeInputFilesByName(self, fileDir, outputFileName, substringToSearch, orderingList, exploder):
        # TODO - rewrite this clumsy thing (if possible) !
        listOfFiles = [f for f in os.listdir(fileDir) if f.endswith('.txt') and f.find(substringToSearch) is not -1]
        # print listOfFiles
        # check if files exists
        if not listOfFiles:
            return False
        with open(fileDir + outputFileName, 'w') as outputFile:
            if listOfFiles:
                for f in listOfFiles:
                    with open(fileDir + f, 'r') as data_file:
                        lines = data_file.readlines()
                        for l in lines:
                            listElements = l.split(exploder)
                            if len(listElements) > 2:
                                argstowrite = [listElements[a] for a in orderingList]
                                outputFile.write(' '.join(argstowrite) + '\n')
        return True

    def execute(self, optionsToUpdate):
        retval = False
        results = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            # self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
            executable = self.args[0]
            resourcesDir = self.args[1]
            resultsDir = self.args[2]
            rootFile = resultsDir + 'total.root'
            fileToSearch = self.args[3:7]
            areaFile = resourcesDir + self.args[7]
            rawids = resourcesDir + self.args[8]
            inputrolls = resourcesDir + self.args[9]
            for f in fileToSearch:
                self.mergeInputFilesByName(resultsDir, 'All' + f + '.txt', f, [6, 3], ' ')

            file_to_check = rootFile + ' ' + resultsDir + 'AllMasked.txt ' + resultsDir + 'AllDead.txt ' + resultsDir + 'AllToMask.txt ' + resultsDir + 'AllToUnmask.txt ' + areaFile + ' ' + rawids + ' ' + inputrolls
            arguments = rootFile + ' ' + resultsDir + 'database_new.txt ' + resultsDir + 'database_full.txt ' + resultsDir + 'AllMasked.txt ' + resultsDir + 'AllDead.txt ' + resultsDir + 'AllToMask.txt ' + resultsDir + 'AllToUnmask.txt ' + areaFile + ' ' + rawids + ' ' + resultsDir + 'error_in_translation ' + inputrolls
            fnf_list = [fnf for fnf in file_to_check.split(' ') if not os.path.isfile(fnf)]
            if fnf_list:
                # file is missing, write log and abort, so abort
                self.log = 'Files missing: ' + ' '.join(fnf_list) + ', Abort'
            else:
                childp = subprocess.Popen(executable + ' ' + arguments, shell=True, stdout=subprocess.PIPE,
                                          stderr=subprocess.STDOUT, close_fds=True)
                current_stdout, current_stderr = childp.communicate()
                current_excode = childp.returncode
                if current_excode == 0:
                    retval = True
                    self.log = 'Completed'
                    # print self.log
                    optionsToUpdate[self.name]['results'] = {'strips_file': resultsDir + 'database_full.txt',
                                                             'rolls_file': resultsDir + 'database_new.txt'}
                    fileList = [optionsToUpdate[self.name]['results']['strips_file'], optionsToUpdate[self.name]['results']['rolls_file']]
                    for finlist in fileList:
                        existingData = None
                        with open(finlist, 'r') as df:
                            existingData = df.read()
                        with open(finlist,'w') as data_file:
                            timestamp = int(time.time())
                            data_file.write(optionsToUpdate['run'] + ' ' +  str(timestamp)  + '\n')
                            data_file.write(existingData)

                print current_stdout, current_stderr, current_excode


            optionsToUpdate[self.name]['log'] = self.log

        return retval


class DBFilesContentCheck(Command):
    # TODO - pass the patterns from the options

    def contentCheck(self, file_content, file_type='rolls'):
        contentMeta = {'correct': True, 'errors': []}
        headerPattern = '^[0-9]+[ \t]+[0-9]+$'
        runDataPatternNon_Roll = """
        ^
        ([0-9]+[ \t]+){1}
        [a-zA-Z0-9_\+\-]+[ \t]+
        (-99[ \t]+){5}
        -99[ \t]*
        $
        """
        runDataPattern_Roll = """
        ^
        ([0-9]+[ \t]+){1}
        [a-zA-Z0-9_\+\-]+[ \t]+
        ([0-9]+[ \t]+){4}
        [0-9]+(\.*[0-9]+){0,1}([eE][+-]?\d+)?[ \t]+
        [0-9]+(\.*[0-9]+){0,1}([eE][+-]?\d+)?[ \t]*
        $
        """
        runDataPatternNon_Strips = """
        ^
        ([0-9]+[ \t]+){2}
        ([-]?\d+[ \t]+)+
        (0|-99)[ \t]*
        $
        """
        runDataPattern_Strips = """
        ^
        ([0-9]+[ \t]+){5}
        [0-9]+(\.*[0-9]+){0,1}([eE][+-]?\d+)?[ \t]*
        $
        """
        pattrn = runDataPattern_Roll
        pattrnNon = runDataPatternNon_Roll
        if file_type is 'strips':
            pattrn = runDataPattern_Strips
            pattrnNon = runDataPatternNon_Strips
        if not re.match(headerPattern, file_content[0]):
            contentMeta['correct'] = False
            contentMeta['errors'].append('File header (first line of the file) is incorrect')
        for run in file_content[1:]:
            if not re.match(pattrn, run, re.VERBOSE) or not re.match(pattrnNon, run, re.VERBOSE):
                #print run
                contentMeta['correct'] = False
                contentMeta['errors'].append('run data ' + run + ' is incorrect')
        return contentMeta

    def execute(self, optionsToUpdate):
        retval = True
        self.args = optionsToUpdate[self.name]['args']
        self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
        strips_file = self.options['strips_file']
        rolls_file = self.options['rolls_file']
        cont_files = [strips_file, rolls_file]
        checkResults = {}
        llog = {}

        for f in cont_files:
            with open(f, 'r') as data_file:
                lines = data_file.read().strip().split('\n')
                filename = 'strips'
                if f.find('_new.txt') is not -1:
                    filename = 'rolls'
                filecheck = self.contentCheck(lines, filename)
                checkResults[f] =  filecheck['correct']
                llog[f] = filecheck['errors']
                if not filecheck['correct']:
                    retval = False
        optionsToUpdate[self.name]['results'] = checkResults
        optionsToUpdate[self.name]['log'] = filecheck['errors']
        return retval

'''

commands that could be executed independent from one another.
So far, make them in a in a queue, and when its possible use threads to parallel processing

'''

class DBDataUpload(Command):

    def execute(self, optionsToUpdate):
        retval = True
        self.args = optionsToUpdate[self.name]['args']
        self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
        # files from results, table names and schemas from options
        dbService = DBService(dbType=self.args['dbType'], host=self.args['hostname'], port=self.args['port'], user=self.args['username'], password=self.args['password'], schema=self.args['schema'], dbName=self.args['dbName'])
        for rec in self.args['dbResources']:
            dataFile = ''.join([f for f in self.options if f.find(rec['file']) is not -1])
            data = self.getDBDataFromFile(dataFile)
            completed = dbService.insertToDB(data, rec['name'], rec['schm'], rec['argsList'])
            optionsToUpdate[self.name]['results'] = { dataFile : {'uploadedToDb': completed }}
            if not completed:
                retval = False
        if retval:
            optionsToUpdate[self.name]['log'] = 'completed'
        else:
            optionsToUpdate[self.name]['log'] = 'failed'
        return retval


    def getDBDataFromFile(self, fileName):

        dataList = []
        with open(fileName, 'r') as data_file:
            fileContent = data_file.readlines()
            runID = fileContent[0].split()[0]
            for line in fileContent[1:5]:
                listToIns = line.split()
                listToIns.insert(0,runID)
                dataList.append(listToIns)

        return dataList


class OutputFilesFormat(Command):

    '''
    format all outputs into single file
    '''

    def execute(self, optionsToUpdate):
        pass

class WebResourcesFormat(Command):
    def execute(self, optionsToUpdate):
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

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important
    print os.environ['LD_LIBRARY_PATH']

    towers = ["RB+2_far", "RB+2_near", "RB+1_near", "RB+1_far", "RB-1_far", "RB-1_near", "RB-2_far", "RB-2_near",
              "RB0_far", "RB0_near", "YEN1_far", "YEN1_near", "YEN3_far", "YEN3_near", "YEP1_far", "YEP1_near",
              "YEP3_far", "YEP3_near"]

    dbSchemasDict = [{'name' : 'RPC_NOISE_STRIPS', 'schm': ['run_number', 'raw_id', 'channel_number', 'strip_number', 'is_dead', 'is_masked', 'rate_hz_cm2'], 'argsList': [0, 1, 2, 3, 4, 5, 6], 'file': 'database_full.txt'},
                     {'name' : 'RPC_NOISE_ROLLS', 'schm': ['run_number','raw_id', 'rollName' ,'dead_strips', 'masked_strips', 'strips_to_unmask', 'strips_to_mask','noiseRate' ,'rate_hz_cm2'], 'argsList': [0, 1, 3, 4, 5, 6, 8 ], 'file': 'database_new.txt'}]

    optionsObject = {'run': '220796', 'filelister': {'args': ['resources', towers], 'source': '', 'results': ''},
                     'check': {'args': 'resources/CheckCorruptedFile.lnxapp', 'source': 'filelister', 'results': ''},
                     'noiseexe': {'args': ['resources/LBNoise', 'resources/resources/lb-chamber-withRE4.txt',
                                           'resources/resources/area_noise_withRE4.txt',
                                           'resources/results'], 'source': 'check', 'results': ''},
                     'dbinput': {
                         'args': ['resources/DBInputs.lnxapp', 'resources/resources/', 'resources/results/', 'Masked',
                                  'Dead', 'ToUnmask',
                                  'ToMask', 'area_noise_cmssw_withRE4', 'RawIDs.txt', 'InputRollList.txt'],
                         'source': 'noiseexe', 'results': ''},
                     'dbfilescontent': {'args': '', 'source': 'dbinput', 'results': ''},
                     'dbdataupload' : {'args': { 'dbResources': dbSchemasDict,  'dbType':'', 'hostname':'', 'port':'', 'username':'', 'password':'', 'schema':'', 'dbName':''} ,
                                                 'source': 'dbfilescontent', 'results': ''}
                     'outputformat' : {'args' :'' , 'source':'dbfilescontent', 'results':'' }


                     }

    getFiles = GetListOfFiles(name='filelister')
    getFiles.execute(optionsObject)

    fileIsCorrupted = CheckIfFilesAreCorrupted(name='check')
    passit = fileIsCorrupted.execute(optionsObject)

    # noiseExe = NoiseToolMainExe(name='noiseexe')
    # noisepassed = noiseExe.execute(optionsObject)
    # print optionsObject['dbinput']['args']

    dbInput = DBInputPrepare(name='dbinput')
    dbinpass = dbInput.execute(optionsObject)

    print optionsObject[dbInput.name]['log']
    print optionsObject[dbInput.name]['results']

    dbfilescheck = DBFilesContentCheck(name='dbfilescontent')
    dbfilescheck.execute(optionsObject)

    print optionsObject[dbfilescheck.name]['results']
    #print optionsObject[dbfilescheck.name]['log']

    dbUpload = DBDataUpload(name='dbdataupload')
    dbUpload.execute(optionsObject)



    # print optionsObject

    '''

    argss = ['resources/CheckCorruptedFile.lnxapp','resources/Histos_YEP3_near_run_220816_2014_4_8__14_55_16.root']
    objWithOptions = {}
    fileIsCorrupted = CheckIfFileIsCorrupted(argss,"check",objWithOptions)
    retval = fileIsCorrupted.execute(objWithOptions)
    #print retval
    print fileIsCorrupted.stout, fileIsCorrupted.sterr
    print objWithOptions

    '''
