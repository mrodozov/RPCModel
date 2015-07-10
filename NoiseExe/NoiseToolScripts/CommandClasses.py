__author__ = 'rodozov'

'''
Abstract command class and different implementations, representing
independent implementations of command objects, to be
'''

import sys, json, subprocess, os

class Command(object):

    args = None
    log = None
    stout = None
    sterr = None
    name = None
    exitcode = None

    def __init__(self, args=None):
        self.args = args
        self.log = None
        self.stout = None
        self.sterr = None
        self.name = None
        self.exitcode = None
        raise NotImplementedError

    def __del__(self):
        pass

    def execute(self):
        raise NotImplementedError

    def getStdErr(self):
        raise NotImplementedError

    def getLog(self):
        raise NotImplementedError

class CheckIfFileIsCorrupted(Command):

    def __init__(self, args = None):
        if args is None and self.args is None:
            self.exitcode = "NOARGS"
            self.log = 'no arguments given' # some standard errors enum, maybe ?

    def execute(self):

        retal = False
        if self.args is not None and len(self.args) >= 2:
            executable = self.args[0]
            file_to_check = self.args[1]
            childp = subprocess.Popen(executable+' '+file_to_check,shell=True,close_fds=True)
            print 'so far so good'
            self.exitcode = childp.returncode
            self.stout, self.sterr = childp.communicate()
            if self.stout is None and self.sterr is None:
                retval = True
            print 'exit code', self.exitcode
        return retval

class NoiseToolMainExe(Command):

    def __init__(self):
        pass

if __name__ == "__main__":
    # test each object

    os.environ['LD_LIBRARY_PATH'] = '/home/rodozov/Programs/ROOT/INSTALL/lib/root'
    print os.environ['LD_LIBRARY_PATH']

    fileIsCorrupted = CheckIfFileIsCorrupted()
    fileIsCorrupted.args = ['resources/CheckCorruptedFile.lnxapp','resources/Histos_YEP3_near_run_220816_2014_4_8__14_55_16.root']
    fileIsCorrupted.execute()

    print fileIsCorrupted.sterr, fileIsCorrupted.stout

