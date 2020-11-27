'''
Created on 29/ago/2014

@author: davidemaiorca
'''
import logging

class Log(object):
    '''
    Class that manages logging and logfile
    '''

    def __init__(self):
        '''
        Constructor
        '''

    def _add_handler(self, formatter, handler):
        '''Adds handler and specifies a formatter.'''
        handler.setFormatter(formatter)
        self.logger.addHandler(handler)

    def set_logger(self, debug=None):
        '''Prepares two handlers for the logger, one that redirects on the stdout and the other one
        that redirects to a file.
        '''
        formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
        self.logger = logging.getLogger(type(self).__name__)
        # Make sure no handler is saved when set_logger is called
        self.logger.handlers = []
        handler = logging.FileHandler('logs.log')
        self._add_handler(formatter, handler)
        handler = logging.StreamHandler()
        self._add_handler(formatter, handler)
        if (debug):
            self.logger.setLevel(logging.DEBUG)
        else:
            self.logger.setLevel(logging.INFO)
        