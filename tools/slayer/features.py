'''
Created on 18/lug/2014

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

from collections import OrderedDict

from pdfminer.pdfdocument import PDFDocument
from pdfminer.pdfdocument import PDFXRef
from pdfminer.pdfdocument import PDFXRefFallback
from pdfminer.pdfdocument import PDFXRefStream
from pdfminer.pdfparser import PDFParser
from pdfminer.pdftypes import PDFStream
from pdfminer.psparser import PSLiteral
from logs import Log



class PdfFeatures(Log):
    '''Extracts PDF file features (PDF Miner).'''


    def __init__(self, filename, debug):
        '''Constructor.'''
        self.pdf = open(filename, "rb")
        parser = PDFParser(self.pdf)
        self.doc = PDFDocument(parser)
        self.frequency = {}
        self.vulns = 0
        self.set_logger(debug)
    
    def close_file(self):
        '''Closes PDF file.'''
        self.pdf.close()
            
    def dump_obj_to_file(self, logname, obj):
        '''Prints the object to a log file.'''
        dump = open(logname,"a")
        dump.write("\n")
        dump.write(str(obj))
        dump.flush()
        dump.close()
    
    def get_keyword_frequencies(self):
        '''Get all name object frequencies. To optimize the algorithm, we
        will only consider the 'value' type in the couple 'key': 'value'
        in the dictionary.
        
        '''
        visited = set()
        # Check all xref starting from the last ones
        for xref in self.doc.xrefs:
            for objid in xref.get_objids():
                # Make sure the object is always referenced
                obj = None
                # If the object has already been visited (i.e., it is part of an older version),
                # skip it.
                if objid in visited: 
                    continue
                visited.add(objid)
                # Otherwise get the object
                try:
                    obj = self.doc.getobj(objid)
                except:
                    self.logger.warning("Cannot get object id %s (parsing error?)", str(objid))
                if obj is None: 
                    continue
                self.get_keywords_objects(obj)
        self.frequency = OrderedDict(sorted(self.frequency.items(), key=lambda x: x[1], reverse = True))
                    
                
    def get_keywords_objects(self, obj): 
        '''Return all frequencies of objects that are not streams. Only
        name objects are retrieved and only the value of the couple
        'key : value' is considered.
        
        '''
        # If the object analyzed is a list, we recursively iterate through it
        if type(obj) is list:
            for key in obj:
                self.get_keywords_objects(key)
        # If it is a dict, we iterate through key and values recursively
        elif type(obj) is dict:
            for key in obj:
                self.get_keywords_objects(key)
                self.get_keywords_objects(obj[key])
        # If we only have a name object, increase its frequency
        elif type(obj) is PSLiteral:
            self.increment_keyword_frequency(obj)
        # If we deal with stream objects, make a separate analysis in relation to 
        # their attributed
        elif type(obj) is PDFStream:
            for key in obj.attrs:
                self.get_keywords_objects(obj.attrs[key])
        else:
            pass
    
    
    def get_generic_features(self):
        '''Retrieves generic features related to the PDF file characteristics'''
                              
            
    def increment_keyword_frequency(self, keyword):
        '''Increment frequency of a specific keyword.'''
        if str(keyword) not in self.frequency.iterkeys():
            self.frequency[str(keyword)] = 1
        else:
            self.frequency[str(keyword)] += 1
        
        
        