__author__ = 'rodozov'

from CommandClasses import *
from Chain import Chain
from RPCMap import RPCMap
from Event import SimpleEvent
from RunlistManager import RunlistManager
import multiprocessing as mp
import Queue
import time
import json

'''

Run process creates runlist from each

'''

class RunProcessPool(object):

    def __init__(self, runs_to_process_queue = None, processed_runs = None, sequence_handler_object = None ,options = None):

        self.options = options
        self.comm_static_opts
        self.pool = mp.Pool()
        self.toprocess = runs_to_process_queue
        self.processed_runs = processed_runs
        self.sequence_handler = sequence_handler_object

    def processRuns(self, functoapply):

        # say, we got somehow a queue entry
        runs = self.processed_runs.get()
        runlist = []
        for run in runs.keys():
            onerun = {}
            seq = self.sequence_handler.getSequenceForName(runs[run]['status'])
            onerun['rundetails'] = runs[run]
            onerun['comm_sequence'] = seq
            runlist.append(onerun)

        results = [self.pool.apply_async(functoapply, (args, )) for args in runlist] # or simply call pool.apply_asynch in a loop for each available run in a queue
        self.pool.close()
        self.pool.join()
        #for r in results:
        #    print '\t', r.get()


def processSingleRunChain(args=None):
    '''
    Function to run single runChain object
    Setup the run chain object with the args
    '''
    print 'process is ', mp.current_process().name

    runchain = Chain()
    rn = None
    for k, v in args['rundetails'].iteritems():
        rn = k

    print 'process is ', mp.current_process().name , ' for run ', rn

    runchain.runnumber = rn
    runchain.commands = args['comm_sequence']
    runchain.stat_options_file = args['stat_options']

    initialEvent = SimpleEvent('initEvent', True, rn)
    runchain.startChainWithEvent(initialEvent)
    #result_is = chain_args['run']

    '''
    #return  chain_args['run']
    #return 'result %' % (result_is)
    '''



if __name__ == "__main__":

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important

    rlistMngr = RunlistManager('resources/runlist.json')
    sequence_handler = CommandSequenceHandler('resources/SequenceDictionaries.json','resources/options_object.txt')

    result_folder = 'results/'
    rlisttoprocess = rlistMngr.getListOfRunsToProcess()
    run = rlistMngr.runlist['220796']
    print run.keys()
    status = rlistMngr.runlist['220796']['status']
    c_sequence = sequence_handler.getSequenceForName(status)
    dyn_opts = {'run':'220796', 'result_folder':'results/'}
    # now assemble and pass the dict
    achain = Chain()
    achain.commands = c_sequence
    initialEvent = SimpleEvent('init', True, dyn_opts)
    achain.startChainWithEvent(initialEvent)

    for k in achain.commands.keys():
        for c in achain.commands[k]:
            print c.name
            print c.args
            print c.options
            print c.warnings
            #print c.results

    #for res in


    '''
    runsToProcessQueue = Queue.Queue()
    processedRunsQueue = Queue.Queue()
    sequence_handler = CommandSequenceHandler('resources/SequenceDictionaries.json', 'resources/options_object.txt')
    rlistMngr = RunlistManager('resources/runlist.json')
    rlistMngr.toProcessQueue = runsToProcessQueue
    rlistMngr.processedRunsQueue = processedRunsQueue
    rprocpool = RunProcessPool(runs_to_process_queue=runsToProcessQueue, processed_runs=processedRunsQueue)
    rlistMngr.putRunsOnProcessQueue(rlistMngr.runlist)
    r = rlistMngr.toProcessQueue.get()
    rprocpool = RunProcessPool()
    print r
    '''



