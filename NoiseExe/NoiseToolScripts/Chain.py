__author__ = 'rodozov'

import json
import os
import Queue
import threading
from Event import SimpleEvent
from Event import Observer
from EventsHandler import EventsHandler
from CommandClasses import Command
import time
import weakref
# just use it to test some dummy commands

class CommandThread(threading.Thread):
    def __init__(self, queue, event_handler):
        threading.Thread.__init__(self)
        self.queue = queue
        self.ehandler = event_handler
        print self.ehandler

    def run(self):
        while True:
            command = self.queue.get()
            pssd_args = command.options['passed_opts']
            success = command.execute(pssd_args)
            event = SimpleEvent(command.name, success, command.options['result'])
            self.ehandler.notify(event)
            self.queue.task_done()

class Chain(Observer):
    '''
    this class is chain that put commands in list, and executes them in order.

    '''

    def __init__(self, options=None):
        self.commands = {}
        self.options = options
        self.log = {}
        self.runnumber = None
        self.event_handler = EventsHandler([self])
        self.jobsQueue = Queue.Queue()

    def add_commands_for_event_name(self, commandsList, eventname):
        self.commands[eventname] = commandsList

    def get_commands_static_options_from_dict(self, dict_with_options=None):
        for c in self.commands:
            for cmmnd in self.commands[c]:
                if cmmnd.name in dict_with_options:
                    cmmnd.args = dict_with_options[cmmnd.name]

    def update(self, event=None):
        if event.name in self.commands:
            commands_for_queue = self.commands[event.name]
            for c in commands_for_queue:
                # get options from event
                print 'received update to put ', c.name, ' on the queue'
                c.options['passed_opts'] = event.message
                self.jobsQueue.put(c)

    def startChainWithEvent(self, init_event):
        number_of_threads = len(self.commands)
        print number_of_threads
        self.event_handler.notify(init_event)
        for i in range(number_of_threads):
            t = CommandThread(self.jobsQueue, self.event_handler)
            t.setDaemon(True)
            t.start()
        self.jobsQueue.join()

        #
    def collectLogs(self):
        for command in self.commands:
            cmndname = command.name
            self.log[cmndname] = command.log
            # self.stout[cmndname] = command.stout



# self.sterr[cmndname] = command.sterr


class BaseCommand(Command):
    def __init__(self, args=None, name=None):
        self.stout = None
        self.sterr = None
        self.name = name
        self.log = {}
        self.waittime = 0
        self.options = {}
        if args is None or self.args is None:
            self.args = args
            self.exitcode = "NOARGS"
            # some standard errors enum, maybe ?
            # self.execute(opts_to_update)

    def __del__(self):
        pass

    def execute(self, dynamic_opts):
        retval = False

        if 'passed_opts' in self.options and 'counter' in self.options['passed_opts']:
            self.waittime = int(self.options['passed_opts']['counter'])
        self.waittime += 2
        starttime = time.time()
        time.sleep(self.waittime)
        endtime = time.time()

        self.options['result'] = {
            'decription': 'comming from ' + self.name + ' results: run from ' + str(starttime) + ' to ' + str(
                endtime) + ' for ' + str(endtime - starttime) + ' seconds and dynamic opts ' + str(dynamic_opts),
            'counter': self.waittime}

        retval = True
        self.log['success'] = self.name + ' ended with status ' + str(retval) + ' start time ' + str(starttime) + ' endtime ' + str(endtime)
        return retval

    def getStdErr(self):
        raise NotImplementedError

    def getLog(self):
        raise NotImplementedError


if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important
    # print os.environ['LD_LIBRARY_PATH']

    # try some simple queue, update it while looping over it

    bc = BaseCommand('fcArgs', 'firstCommand')
    bc.execute('resources/')
    secondcmmnd = BaseCommand('scArgs', 'secondCommand')
    secondcmmnd.execute('resources/area')
    thirdcmmnd = BaseCommand('thArgs', 'thirdCommand')
    thirdcmmnd.execute('resources/towers')
    forth = BaseCommand('forthArgs', 'forthCommand')
    forth.execute('resources/rpcMap')
    fifth = BaseCommand('fifthArgs', 'fifthCommand')
    fifth.execute('resources/chips')
    sixt = BaseCommand('sixtArgs', 'sixtCommand')
    sixt.execute('resources/dbSchema')

    commandsDict = {'initCommand': [bc], 'firstCommand': [secondcmmnd], 'secondCommand': [thirdcmmnd], 'thirdCommand': [forth, fifth, sixt]}

    achain = Chain(commandsDict)
    for k, v in commandsDict.iteritems():
        achain.add_commands_for_event_name(v, k)
    initialEvent = SimpleEvent('initCommand', True ,'starting options for first command')
    achain.startChainWithEvent(initialEvent)

    for c  in achain.commands:
        print c
        for l in achain.commands[c]:
            print l.log
            #for d in l.log:
                #print d


    '''
    dbschema = {}
    optionsObject = {}
    with open('resources/options_object.txt', 'r') as optobj:
        optionsObject = json.loads(optobj.read())
    with open('resources/db_tables_schema.txt', 'r') as dbschemafile:
        dbschema = json.loads(dbschemafile.read())
    optionsObject['dbdataupload']['args']['dbResources'] = dbschema
    optionsObject['run'] = '220796'

    runchain = Chain(optionsObj
ct)
    '''
