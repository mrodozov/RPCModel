__author__ = 'rodozov'

import json
import os
import Queue
import threading
from Event import SimpleEvent
from Event import Observer
from EventsHandler import EventsHandler
#from CommandClasses import *

import time

# TODO - failed execution of thread hangs the main thread, try to fix this and test it

class CommandThread(threading.Thread):
    def __init__(self, queue, event_handler):
        threading.Thread.__init__(self)
        self.queue = queue
        self.ehandler = event_handler
        # print self.ehandler # debug to check if the EventHandler object is the same for all the threads

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
        #print number_of_threads
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
        return self.log




''' test unit code, uncomment the BaseCommand class import to use '''

if __name__ == "__main__":

    print 'blabla'