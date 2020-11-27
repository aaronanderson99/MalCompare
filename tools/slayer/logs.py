'''
Created on 29/ago/2014
    PDF Malware Slayer - Malicious PDF Files Detector

    Copyright (C) 2014  Davide Maiorca
    Dept. of Electrical and Electronic Engineering, University of Cagliari, Italy.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
@author: davidemaiorca
'''
import logging

class Log(object):
    '''
    Class that manages logging and logfile
    '''

    def __init__(self, params):
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
        