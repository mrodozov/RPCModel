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

    optionsObject = {}
    with open('resources/options_object.txt', 'r') as optobj:
        optionsObject = json.loads(optobj.read())
        optobj.close()

    optionsObject['run'] = rn
    optionsObject['result_folder'] = 'run' + optionsObject['run']

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
                                   dbInput.name : [dbcontentcheck], dbcontentcheck.name: [dbUpload, mergeContent] }
        #,mergeContent.name : [webserver_copy, archive_copy] }

    '''
    runchain = Chain()
    runchain.commands = start_command_on_event_dict
    initialEvent = SimpleEvent('initEvent', True, rnum)
    runchain.startChainWithEvent(initialEvent)
    result_is = chain_args['run']

    #return  chain_args['run']
    #return 'result %' % (result_is)
    '''

class RunProcessPool(object):

    def __init__(self, runlist=[], options = None):
        self.runlist = runlist
        self.options = options
        self.pool = mp.Pool()
        self.queue = mp.Queue()

    def getLogs(self):
        pass

    def processRuns(self, functoapply):
        results = [self.pool.apply_async(functoapply, (rnum, )) for rnum in self.runlist]
        self.pool.close()
        self.pool.join()
        #for r in results:
        #    print '\t', r.get()


'''
testing pool behavior class
'''

if __name__ == "__main__":

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important

    #rlist = ['251643','251638','251718', '220796']
    rlist = ['251643']

    rprocpool = RunProcessPool()
    rprocpool.runlist = rlist

    processSingleChain('251643')
    #rprocpool.processRuns(processSingleChain)

