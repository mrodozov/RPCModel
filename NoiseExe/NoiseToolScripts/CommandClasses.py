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
import json
from DBService import DBService
from RPCMap import RPCMap
from Chain import Chain

# TODO - Write results, log for each class, where results should be the result the command produced or checked
# TODO - See if there is need to format HTML for any reason,
# TODO -

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
        self.log = {}
        if args is None or self.args is None:
            self.args = args
            self.exitcode = "NOARGS"
            # some standard errors enum, maybe ?
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
        complete = False
        files_per_tower_more_than_one = False
        missing_towers_files = []
        total_files_n = 0
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            filespath = self.args[0]
            runnum = optionsToUpdate['run']
            towers = self.args[1]
            # print runnum
            files = [f for f in os.listdir(filespath) if f.endswith('.root') and f.find(runnum) is not -1]
            # print towers
            shortlist = []
            total_files_n = len(files)
            if len(files) > 18:
                files_per_tower_more_than_one = True
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
                #print towerMap
                files = shortlist
            # print files
            filespath += '/'
        optionsToUpdate[self.name]['results'] = [filespath + f for f in files]
        #format the output
        towerslist = self.args[1]
        for t in towerslist:
            match = False
            for f in files:
                if f.find(t) is not -1:
                    match = True
                    break
            if not match:
                missing_towers_files.append(t)

        if missing_towers_files:
            self.log['missing_files'] = missing_towers_files
        if files_per_tower_more_than_one:
            self.log['files_for_run'] = total_files_n
        if len(files) is 0:
            self.log['missing_files'] = 'no files for this run, skip'
            optionsToUpdate[self.name]['result'] = 'Failed'
        if len(files) > 0:
            self.log['files_for_run'] = total_files_n
            complete = True
        return complete


class CheckIfFilesAreCorrupted(Command):
    def execute(self, optionsToUpdate=None):

        complete = False
        goodresult = {}
        corrupted_files = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']

            for file in self.options:
                executable = self.args
                childp = subprocess.Popen(executable + ' ' + file, shell=True, stdout=subprocess.PIPE,
                                          stderr=subprocess.STDOUT, close_fds=True)
                self.stout, self.sterr = childp.communicate()
                self.exitcode = childp.returncode
                overall = {'sterr': self.sterr, 'stout': self.stout, 'exitcode': self.exitcode}

                if self.exitcode == 0 and self.sterr is None:
                    complete = True
                    goodresult[file] = overall
                else:
                    corrupted_files[file] = overall

            self.log = {'good_files':goodresult,'corrupted_files': corrupted_files}
            optionsToUpdate[self.name]['results'] = goodresult
            if not complete:
                optionsToUpdate[self.name]['results'] = 'Failed'

            #print 'exit code', self.exitcode
        return complete


class NoiseToolMainExe(Command):
    '''
    Noise tool main executable command
    If the file check is passed, it process the files (one by one ? really ?)
    '''

    def execute(self, optionsToUpdate):

        complete = False
        results = {'masked':[],'dead':[],'tomask':[],'tounmask':[],'rootfiles':[],'totalroot':''}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']
            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
            self.log = {}
            filesToProcess = [k for k, v in self.options.iteritems() if v['exitcode'] is not '0']
            # print filesToProcess # for debug
            executable = self.args[0]
            arguments = self.args[1] + ' ' + self.args[2] + ' ' + self.args[3]
            results_folder = self.args[3]
            for f in filesToProcess:
                # print executable, f, arguments
                #childp = subprocess.Popen(executable + ' ' + f + ' ' + arguments, shell=True, stdout=subprocess.PIPE,
                #                          stderr=subprocess.STDOUT, close_fds=True)
                #current_stdout, current_stderr = childp.communicate()
                #current_excode = childp.returncode
                current_stdout = ''
                current_excode = 0
                current_stderr = None
                mskdch= 'm'
                deadch='d'
                tomch='t'
                tounmch='tu'
                rootrate='r'

                #if at least one file has finished
                if current_excode == 0 and current_stderr is None:
                    complete = True
                    results['masked'].append(mskdch)
                    results['dead'].append(deadch)
                    results['tomask'].append(tomch)
                    results['tounmask'].append(tounmch)
                    results['rootfiles'].append(rootrate)
                self.log[f] = {'complete': complete,'err': current_stderr,'out':current_stdout,'exitcode':current_excode}
                    # so far, and thanks for all the fish
            #TODO - make the executable create the total.root and fill it
            if not complete:
                results = 'Failed'
            else:
                results['totalroot'] = results_folder+'/total.root'
            optionsToUpdate[self.name]['results'] = results

        return complete


class DBInputPrepare(Command):
    '''

    '''
    # TODO - merge the .root files into total.root, from the executable itself


    def mergeInputFilesByName(self, fileDir, outputFileName, substringToSearch, orderingList, exploder):

        listOfFiles = [f for f in os.listdir(fileDir) if
                       f.endswith('.txt') and f.find(substringToSearch) is not -1 and f.find('All') is -1]
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
                                # print l, f
                                argstowrite = [listElements[a] for a in orderingList]
                                outputFile.write(' '.join(argstowrite) + '\n')
        return True

    def execute(self, optionsToUpdate):
        complete = False
        results = {}
        if optionsToUpdate is not None:
            self.args = optionsToUpdate[self.name]['args']

            self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']

            executable = self.args[0]
            resourcesDir = self.args[1]
            resultsDir = self.args[2]
            rootFile = self.options['totalroot']
            fileToSearch = self.args[3:7]
            areaFile = resourcesDir + self.args[7]
            rawids = resourcesDir + self.args[8]
            inputrolls = resourcesDir + self.args[9]
            for f in fileToSearch:
                listOfOrderedArgs = [6, 3]
                if f == 'ToMask':
                    listOfOrderedArgs = [6, 3, 9, 14]
                if f == 'ToUnmask':
                    listOfOrderedArgs = [6, 3, 15]
                self.mergeInputFilesByName(resultsDir, 'All' + f + '.txt', f, listOfOrderedArgs, ' ')

            file_to_check = rootFile + ' ' + resultsDir + 'AllMasked.txt ' + resultsDir + 'AllDead.txt ' + resultsDir + 'AllToMask.txt ' + resultsDir + 'AllToUnmask.txt ' + areaFile + ' ' + rawids + ' ' + inputrolls
            arguments = rootFile + ' ' + resultsDir + 'database_new.txt ' + resultsDir + 'database_full.txt ' + resultsDir + 'AllMasked.txt ' + resultsDir + 'AllDead.txt ' + resultsDir + 'AllToMask.txt ' + resultsDir + 'AllToUnmask.txt ' + areaFile + ' ' + rawids + ' ' + resultsDir + 'error_in_translation ' + inputrolls
            fnf_list = [fnf for fnf in file_to_check.split(' ') if not os.path.isfile(fnf)]
            if fnf_list:
                # file is missing, write log and abort, so abort
                self.log = 'Files missing: ' + ' '.join(fnf_list) + ', Abort'
                optionsToUpdate[self.name]['results'] = 'Failed'
            else:
                childp = subprocess.Popen(executable + ' ' + arguments, shell=True, stdout=subprocess.PIPE,
                                          stderr=subprocess.STDOUT, close_fds=True)
                current_stdout, current_stderr = childp.communicate()
                current_excode = childp.returncode
                if current_excode == 0:
                    complete = True
                    self.log = 'Completed'
                    # print self.log
                    optionsToUpdate[self.name]['results'] = {'strips_file': resultsDir + 'database_full.txt',
                                                             'rolls_file': resultsDir + 'database_new.txt'}
                    fileList = [optionsToUpdate[self.name]['results']['strips_file'],
                                optionsToUpdate[self.name]['results']['rolls_file']]
                    for finlist in fileList:
                        existingData = None
                        with open(finlist, 'r') as df:
                            existingData = df.read()
                        with open(finlist, 'w') as data_file:
                            timestamp = int(time.time())
                            data_file.write(optionsToUpdate['run'] + ' ' + str(timestamp) + '\n')
                            data_file.write(existingData)

                #print current_stdout, current_stderr, current_excode

        return complete


class DBFilesContentCheck(Command):
    # TODO - pass the patterns from the options, read them from a file maybe

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
                # print run
                contentMeta['correct'] = False
                contentMeta['errors'].append('run data ' + run + ' is incorrect')
        return contentMeta

    def execute(self, optionsToUpdate):
        complete = True
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
                checkResults[f] = filecheck['correct']
                llog[f] = filecheck['errors']
                if not filecheck['correct']:
                    complete = False
        optionsToUpdate[self.name]['results'] = checkResults
        #self.log = filecheck['errors']
        return True

'''

commands that could be executed independent from one another.
So far, make them in a in a queue, and when its possible use threads to parallel processing

'''


class DBDataUpload(Command):
    def execute(self, optionsToUpdate):
        complete = False
        self.args = optionsToUpdate[self.name]['args']
        self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
        # files from results, table names and schemas from options
        dbService = DBService(dbType=self.args['dbType'], host=self.args['hostname'], port=self.args['port'],
                              user=self.args['username'], password=self.args['password'], schema=self.args['schema'],
                              dbName=self.args['dbName'])
        for rec in self.args['dbResources']:
            dataFile = ''.join([f for f in self.options if f.find(rec['file']) is not -1])
            data = self.getDBDataFromFile(dataFile)
            completed = dbService.insertToDB(data, rec['name'], rec['schm'], rec['argsList'])
            optionsToUpdate[self.name]['results'] = {dataFile: completed}
            self.log[dataFile] = completed
            complete = completed
            if not completed:
                optionsToUpdate[self.name]['results'] = 'Failed'
                break

        return complete

    def getDBDataFromFile(self, fileName):

        dataList = []
        with open(fileName, 'r') as data_file:
            fileContent = data_file.readlines()
            runID = fileContent[0].split()[0]
            for line in fileContent[1:5]:
                listToIns = line.split()
                listToIns.insert(0, runID)
                dataList.append(listToIns)

        return dataList


class OutputFilesFormat(Command):
    '''
    format all outputs into single file. just compactify all the data into single json file, so it could be used later and to be the only remaining thing
    in the folder apart from the root files
    '''

    def execute(self, optionsToUpdate):
        complete = False
        #print optionsToUpdate
        self.args = optionsToUpdate[self.name]['args']
        self.options = optionsToUpdate[optionsToUpdate[self.name]['source']]['results']
        rpcMapFile = self.args[0]
        rawmapfile = self.args[1]
        results_folder = self.args[2]
        rolls_json_file = results_folder + self.args[3]
        strips_json_file = results_folder + self.args[4]
        allToUnmaskFile = results_folder + self.args[5]
        allToMaskFile = results_folder + self.args[6]
        fileFromInput = [f for f in self.options]
        detailedFile = fileFromInput[0]
        rollsFile = fileFromInput[1]
        self.log = {'strips_file': None, 'rolls_file': None}
        optionsToUpdate[self.name]['results'] = []

        # first print
        print detailedFile, rollsFile
        rollObject = {}
        stripsobject = {}
        with open(rollsFile, 'r') as rolls_data:
            masked = {}
            dead = {}
            toMask = {}
            toUnmask = {}
            rate = {}
            for rollrecord in rolls_data.read().splitlines():
                listofargs = [rec for rec in rollrecord.split()]
                # print listofargs
                # print rollrecord
                if len(listofargs) < 8 or int(listofargs[2]) == (-99): continue
                # print ' '.join(listofargs)
                rollname = listofargs[1]
                deadstrips = listofargs[2]
                maskedstrips = listofargs[3]
                tounmaskstrips = listofargs[4]
                tomaskstrips = listofargs[5]
                rateofroll = listofargs[6]
                rateofrollcmsquare = listofargs[7]
                if int(deadstrips) > 0:
                    dead[rollname] = deadstrips
                if int(maskedstrips) > 0:
                    masked[rollname] = maskedstrips
                if int(tomaskstrips) > 0:
                    toMask[rollname] = tomaskstrips
                if int(tounmaskstrips) > 0:
                    toUnmask[rollname] = tounmaskstrips
                rate[rollname] = {'rate': rateofroll, 'ratesquarecm': rateofrollcmsquare}
            rollObject['masked'] = masked
            rollObject['dead'] = dead
            rollObject['tomask'] = toMask
            rollObject['tounmask'] = toUnmask
            rollObject['rate'] = rate

        rpcMap = RPCMap(rpcMapFile, rawmapfile)
        allTUmap = {}
        allTMmap = {}
        allMaskedMap = {}
        allDeadMap = {}
        ratesDict = {}

        with open(allToUnmaskFile, 'r') as tounmaskfile:
            for lines in tounmaskfile.read().splitlines():
                line = [a for a in lines.split()]
                rname = line[0]
                channel_num = int(line[1])
                max_rate = float(line[2])
                chamberMap = rpcMap.chamberToRollMap[rname]

                for roll_record in chamberMap:
                    if channel_num in chamberMap[roll_record]['channels']:
                        channel_index = chamberMap[roll_record]['channels'].index(channel_num)
                        stripnum = chamberMap[roll_record]['strips'][channel_index]
                        if not roll_record in allTUmap:
                            allTUmap[roll_record] = {'channels': [], 'strips': [], 'rates': []}
                        allTUmap[roll_record]['channels'].append(channel_num)
                        allTUmap[roll_record]['strips'].append(stripnum)
                        allTUmap[roll_record]['rates'].append(max_rate)
                        break

        with open(allToMaskFile, 'r') as tomaksfile:
            for lines in tomaksfile.read().splitlines():
                line = [a for a in lines.split()]
                rname = line[0]
                channel_num = int(line[1])
                max_rate = float(line[3])
                time_as_noisy = float(line[2])
                chamberMap = rpcMap.chamberToRollMap[rname]

                for roll_record in chamberMap:
                    if channel_num in chamberMap[roll_record]['channels']:
                        channel_index = chamberMap[roll_record]['channels'].index(channel_num)
                        stripnum = chamberMap[roll_record]['strips'][channel_index]
                        if not roll_record in allTMmap:
                            allTMmap[roll_record] = {'channels': [], 'strips': [], 'times': [], 'rates': []}
                        allTMmap[roll_record]['channels'].append(channel_num)
                        allTMmap[roll_record]['strips'].append(stripnum)
                        allTMmap[roll_record]['times'].append(time_as_noisy)
                        allTMmap[roll_record]['rates'].append(max_rate)
                        break

        with open(detailedFile, 'r') as detailed_data:
            for lines in detailed_data.read().splitlines():
                roll_record = [a for a in lines.split()]
                if len(roll_record) < 6 or int(roll_record[3]) < 0: continue
                rawid = roll_record[0]
                roll_name = rpcMap.rawToRollMap[rawid]
                channel_num = roll_record[1]
                strip_num = roll_record[2]
                is_masked = roll_record[3]
                is_dead = roll_record[4]
                ratepcms = roll_record[5]
                if not roll_name in ratesDict:
                    ratesDict[roll_name] = {'channels': [], 'strips': [], 'rates': []}
                ratesDict[roll_name]['channels'].append(channel_num)
                ratesDict[roll_name]['strips'].append(strip_num)
                ratesDict[roll_name]['rates'].append(ratepcms)
                if int(is_masked) == 1:
                    if not roll_name in allMaskedMap:
                        allMaskedMap[roll_name] = {'channels': [], 'strips': []}
                    allMaskedMap[roll_name]['channels'].append(channel_num)
                    allMaskedMap[roll_name]['strips'].append(strip_num)
                if int(is_dead) == 1:
                    if not roll_name in allDeadMap:
                        allDeadMap[roll_name] = {'channels': [], 'strips': []}
                    allDeadMap[roll_name]['channels'].append(channel_num)
                    allDeadMap[roll_name]['strips'].append(strip_num)

        # merge into single object and write into a file
        detailedFileOutput = {'tomask': allMaskedMap, 'dead': allDeadMap, 'tomask': allTMmap, 'tounmask': allTUmap,
                              'rates': ratesDict}

        with open(rolls_json_file, 'w') as rolls_out_file:
            rolls_out_file.write(json.dumps(rollObject, indent=1, sort_keys=True))

        with open(strips_json_file, 'w') as strips_out_file:

            strips_out_file.write('{\n')
            deptone_keys = sorted(detailedFileOutput.keys())
            for deptone in sorted(detailedFileOutput.keys()):
                strips_out_file.write(json.dumps(deptone))
                strips_out_file.write(':{\n')
                keys = sorted(detailedFileOutput[deptone].keys())
                for key in keys:
                    strips_out_file.write('  ')
                    strips_out_file.write(json.dumps(key))
                    strips_out_file.write(':')
                    strips_out_file.write(json.dumps(detailedFileOutput[deptone][key]))
                    if key is not keys[-1]:
                        strips_out_file.write(',')
                    strips_out_file.write('\n')

                strips_out_file.write('}')
                if deptone is not deptone_keys[-1]:
                    strips_out_file.write(',')
                strips_out_file.write('\n')
            strips_out_file.write('}')

        # check if files has been created and not empty
        with open(strips_json_file, 'r') as strips_file_check:
            if json.loads(strips_file_check.read()) and os.stat(strips_json_file) > 0:
                self.log['strips_file'] = 'Completed!'
                optionsToUpdate[self.name]['results'].append(strips_json_file)
        with open(rolls_json_file, 'r') as rolls_file_check:
            if json.loads(rolls_file_check.read()) and os.stat(rolls_json_file) > 0:
                self.log['rolls_file'] = 'Completed!'
                optionsObject[self.name]['results'].append(rolls_json_file)
        complete = True
        for i in self.log:
            if self.log[i] is not 'Completed!':
                optionsObject[self.name]['results'] = 'Failed'
                complete = False
        return complete


class WebResourcesFormat(Command):
    '''
    write local run html files and prepare some run markup, write it as result
    '''

    def execute(self, optionsToUpdate):
        pass


class GarbageRemoval(Command):
    '''
    Compress or erase all outputs, to be executed after all the outputs are finished
    '''

    def __init__(self):
        pass

class UploadWebResources(Command):
    '''
    Upload files on location or writes into a DB, lets see
    '''

    def execute(self, optionsToUpdate):
        pass

class CopyFilesOnRemoteLocation(Command):
    '''
    sends any file to any location, use file name and destination
    '''
    def execute(self, optionsToUpdate):
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

    dbschema = {}
    optionsObject = {}
    with open('resources/options_object.txt', 'r') as optobj:
        optionsObject = json.loads(optobj.read())
    with open('resources/db_tables_schema.txt', 'r') as dbschemafile:
        dbschema = json.loads(dbschemafile.read())
    optionsObject['dbdataupload']['args']['dbResources'] = dbschema
    optionsObject['run'] = '220796'

    getFiles = GetListOfFiles(name='filelister')
    #getFiles.execute(optionsObject)

    fileIsCorrupted = CheckIfFilesAreCorrupted(name='check')
    #passit = fileIsCorrupted.execute(optionsObject)

    noiseExe = NoiseToolMainExe(name='noiseexe')
    #noisepassed = noiseExe.execute(optionsObject)
    # print optionsObject['dbinput']['args']

    dbInput = DBInputPrepare(name='dbinput')
    #dbinpass = dbInput.execute(optionsObject)

    #print optionsObject[dbInput.name]['log']
    #print optionsObject[dbInput.name]['results']

    dbfilescheck = DBFilesContentCheck(name='dbfilescontent')
    #dbfilescheck.execute(optionsObject)

    # print optionsObject[dbfilescheck.name]['results']
    # print optionsObject[dbfilescheck.name]['log']

    #dbUpload = DBDataUpload(name='dbdataupload')
    # dbUpload.execute(optionsObject)

    mergeContent = OutputFilesFormat(name='outputformat')
    #mergeContent.execute(optionsObject)

    #print mergeContent.log
    #print optionsObject[mergeContent.name]['results']


    runchain = Chain()
    arrayofcommands = [getFiles,fileIsCorrupted,noiseExe,dbInput,dbfilescheck,mergeContent]
    runchain.addListOfCommands(arrayofcommands)
    runchain.execute_chain(optionsObject)
    print json.dumps(runchain.log,indent=1)




