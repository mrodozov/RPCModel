__author__ = 'rodozov'

import json
import os
import Queue
import threading
from Event import SimpleEvent
from Event import Observer
from EventsHandler import EventsHandler
#from CommandClasses import Command
import time

class CommandThread(threading.Thread):
    def __init__(self, queue, event_handler):
        threading.Thread.__init__(self)
        self.queue = queue
        self.ehandler = event_handler
        print self.ehandler

    def run(self):
        while True:
            command = self.queue.get()
            #pssd_args = command.options
            success = command.execute()
            event = SimpleEvent(command.name, success, command.results)
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
                c.options = event.message
                self.jobsQueue.put(c)

    def startChainWithEvent(self, init_event):
        number_of_threads = 0
        for c in self.commands:
            number_of_threads += len(self.commands[c])
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

''' test unit code, uncomment the Command class import to use '''
'''
class BaseCommand(Command):
    def __init__(self, args=None, name=None, wait_time=None):
        self.stout = None
        self.sterr = None
        self.name = name
        self.log = {}
        self.waittime = wait_time
        self.options = {}
        if args is None or self.args is None:
            self.args = args
            self.exitcode = "NOARGS"
            # some standard errors enum, maybe ?
            # self.execute(opts_to_update)

    def __del__(self):
        pass

    def execute(self):
        retval = False

        starttime = time.time()
        time.sleep(self.waittime)

        self.options['result'] = {
            "decription": self.name + " results: run from " + str(starttime)  + " to " + str(time.time()) + " for " + str(time.time() - starttime) + " seconds and dynamic opts " + str(self.options['passed_opts']),
            "counter": self.waittime}

        retval = True
        self.log["success"] = self.name + " ended with status " + str(retval) + " start time " + str(starttime)
        return retval

    def getStdErr(self):
        raise NotImplementedError

    def getLog(self):
        raise NotImplementedError

'''
if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'  # important
    # print os.environ['LD_LIBRARY_PATH']

    # try some simple queue, update it while looping over it

    bc = BaseCommand('fcArgs', 'firstCommand', 5)
    secondcmmnd = BaseCommand('scArgs', 'secondCommand', 7)
    thirdcmmnd = BaseCommand('thArgs', 'thirdCommand', 3)
    forth = BaseCommand('forthArgs', 'forthCommand', 2)
    fifth = BaseCommand('fifthArgs', 'fifthCommand', 4)
    sixt = BaseCommand('sixtArgs', 'sixtCommand', 15)
    seventh = BaseCommand('sevArgs', 'seventhCommand', 2)
    eight = BaseCommand('eightArgs', 'eightCommand', 3)

    commandsDict = {'initCommand': [bc], 'firstCommand': [secondcmmnd], 'secondCommand': [thirdcmmnd], 'thirdCommand': [forth, fifth, sixt], 'fifthCommand': [seventh], 'forthCommand': [eight]}

    achain = Chain(commandsDict)
    for k, v in commandsDict.iteritems():
        achain.add_commands_for_event_name(v, k)
    initialEvent = SimpleEvent('initCommand', True , 'starting options for first command')
    achain.startChainWithEvent(initialEvent)

    for c  in achain.commands:
        print c
        for l in achain.commands[c]:
            print l.log
            if 'result' in l.options: print l.options['result']
