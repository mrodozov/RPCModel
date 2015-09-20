__author__ = 'rodozov'

from CommandClasses import *
from Chain import Chain
from RPCMap import RPCMap
from Event import SimpleEvent
import multiprocessing as mp
import time
import json

'''

In idea, run process should be multi process queue,
spawning new process for each run in the runlist, until the number of processors is reached.

'''

def processSingleChain(chain_args=None):
    '''
    Function to run single runChain object
    Setup the run chain object with the args
    '''
    print 'process is ', mp.current_process().name
    rn = chain_args

    dbschema = {}
    optionsObject = {}
    with open('resources/options_object.txt', 'r') as optobj:
        optionsObject = json.loads(optobj.read())
        optobj.close()
    with open('resources/db_tables_schema.txt', 'r') as dbschemafile:
        dbschema = json.loads(dbschemafile.read())
    optionsObject['run'] = rn
    optionsObject['result_folder'] = 'run' + optionsObject['run']
    optionsObject['dbdataupload']['dbResources'] = dbschema
    rnum = rn

    optionsObject['webserver_remote']['ssh_credentials']['password'] = 'BAKsho___321'
    optionsObject['lxplus_archive_remote']['ssh_credentials']['password'] = 'BAKsho___321'

    opts = optionsObject['filelister']
    listFiles = GetListOfFiles(name='filelister', args=opts)

    fileIsCorrupted = CheckIfFilesAreCorrupted(name='check', args=optionsObject['check']['exe'])

    noiseExe = NoiseToolMainExe(name='noiseexe',args=optionsObject['noiseexe'])

    dbInput = DBInputPrepare(name='dbinput', args=optionsObject['dbinput'])

    dbcontentcheck = DBFilesContentCheck(name='dbfilecontent', args=optionsObject['dbfilecontent'])

    dbUpload = DBDataUpload(name='dbdataupload', args=optionsObject['dbdataupload'])

    mergeContent = OutputFilesFormat(name='outputformat', args=optionsObject['outputformat'])

    webserver_copy = CopyFilesOnRemoteLocation(name='webserver_remote', args=optionsObject['webserver_remote'])

    archive_copy = CopyFilesOnRemoteLocation(name='lxplus_archive_remote', args=optionsObject['lxplus_archive_remote'])

    start_command_on_event_dict = {'initEvent' : [listFiles], listFiles.name: [fileIsCorrupted]  ,
                                   fileIsCorrupted.name: [noiseExe], noiseExe.name: [dbInput],
                                   dbInput.name : [dbcontentcheck], dbcontentcheck.name: [dbUpload, mergeContent],
                                   mergeContent.name : [webserver_copy, archive_copy] }

    runchain = Chain()
    runchain.commands = start_command_on_event_dict
    initialEvent = SimpleEvent('initEvent', True, rnum)
    runchain.startChainWithEvent(initialEvent)

    result_is = chain_args['run']

    return 'result %' % (result_is)


class RunProcessPool(object):

    def __init__(self, runlist=[], options = None):
        self.runlist = runlist
        self.options = options
        self.pool = mp.Pool()
        self.queue = mp.Queue()

    def getLogs(self):
        pass

    def processRuns(self):
        results = [self.pool.apply_async(processSingleChain, (rnum, )) for rnum in self.runlist]
        for r in results:
            print '\t', r.get()

'''
testing pool behavior class
'''

def gosleep(secs):
    td = TimeDelay(secs)
    td.gotosleep()
    return '%s lasted %s' % (mp.current_process().name, td.time_delay)


class TimeDelay(object):

    def __init__(self, delay_in_seconds=0):
        self.time_delay = delay_in_seconds

    def gotosleep(self):
        time.sleep(self.time_delay)
        return self.time_delay


class Collector(object):

    def __init__(self):
        self.delays = []
        print mp.cpu_count()
        self.ppool = mp.Pool(mp.cpu_count())

    def runone(self, delay):
        d = TimeDelay(delay)
        return '%s lasted %s' % (mp.current_process().name, self.delay)

    def runAll(self):
        results = [self.ppool.apply_async(gosleep, (d, )) for d in self.delays]
        for r in results:
            print '\t', r.get()

if __name__ == "__main__":


    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important

    rlist = ['251718']

    rprocpool = RunProcessPool()
    rprocpool.runlist = rlist

    rprocpool.processRuns()
