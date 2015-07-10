__author__ = 'rodozov'

from CommandClasses import Command

class Chain(object):

    '''
    this class is chain that put commands in list, and executes them in order.

    '''

    def __init__(self):
        self.commands = []

    def add_command(self, command):
        if isinstance(command,Command)
            self.commands.append(command)

    def execute_chain(self):
        for comm in self.commands:
            comm.execute()


