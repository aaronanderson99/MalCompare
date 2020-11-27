'''
Created on 21/lug/2014

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

from features import PdfFeatures
from sklearn.cluster import KMeans
from logs import Log
from collections import OrderedDict
import os

class PdfClustering(Log):
    '''This class performs one-feature clustering operations to select
    the most characteristic features.
    '''

    def __init__(self, debug):
        '''
        Constructor
        '''
        self.debug = debug
        self.benign_occurrences = {}
        self.malicious_occurrences = {}
        self.characteristic_occurrences = {}
        self.errors = 0
        self.analyzed = 0
        self.set_logger(debug)
            
    def perform_clustering(self, ben_folder, mal_folder):
        '''Performs a Ward clustering on benign and malicious keywords. Two thresholds are extracted,
        one for malicious samples and one for benign.'''
        self.logger.info("Getting occurrences from benign samples...")
        self.get_occurrences(ben_folder, True)
        self.benign_analyzed = self.analyzed
        self.benign_errors = self.errors 
        self.logger.info("Getting occurrences from malicious samples...")
        self.get_occurrences(mal_folder, False)
        self.malicious_analyzed = self.analyzed - self.benign_analyzed
        self.malicious_errors = self.errors - self.benign_errors
        self.logger.info("Number of analyzed benign files: %s", str(self.benign_analyzed)) 
        self.logger.info("Number of errors in benign files: %s", str(self.benign_errors))
        self.logger.info("Number of analyzed malicious files: %s", str(self.malicious_analyzed)) 
        self.logger.info("Number of errors in malicious files: %s", str(self.malicious_errors))
        self.logger.info("Executing clustering...")
        kmeans = KMeans(2);
        result = self.get_cluster_results(kmeans, self.benign_occurrences)
        self.benign_threshold = len([x for x in result if x == 1])
        result = self.get_cluster_results(kmeans, self.malicious_occurrences)
        self.malicious_threshold= len([x for x in result if x == 1])
    
    def get_occurrences(self, folder, is_benign):
        '''Get keywords occurrences for files. If is_benign flag is set to true,
        then the system expects to get files from benign folders. Otherwise, the value
        folder should point to the malicious files. 
        '''
        if (os.path.exists(folder)):
            for sample in os.listdir(folder):
                self.logger.info("Analyzing sample: %s", sample)
                try:
                    features = PdfFeatures(folder+"/"+sample, self.debug)
                    features.get_keyword_frequencies()
                except:
                    self.logger.error("Cannot retrieve feature elements for the file. Not a PDF file or Malformed PDF?")
                    features.close_file()
                    self.errors += 1
                    continue
                features.close_file()
                # If the sample is correctly analyzed, increment its number
                self.analyzed += 1
                if features.frequency == OrderedDict():
                    self.logger.error("Error: this sample is not a PDF file")
                    continue
                self.logger.debug("Sample Keywords: %s", str(features.frequency))
                for keyword in features.frequency:
                    if is_benign:
                        self.increment_occurrence(keyword, self.benign_occurrences)
                    else:
                        self.increment_occurrence(keyword, self.malicious_occurrences)
            # If is_benign is true, then we are taking keywords from benign files
            if is_benign:
                self.benign_occurrences = sorted(self.benign_occurrences.items(), key=lambda x: x[1], reverse = True)
            else:
                self.malicious_occurrences = sorted(self.malicious_occurrences.items(), key=lambda x: x[1], reverse = True)
    
    def increment_occurrence(self, keyword, occurrences):
        #TODO: This should be put in an Utils class/script
        '''Increment frequency of a specific keyword.'''
        if keyword not in occurrences.iterkeys():
            occurrences[keyword] = 1
        else:
            occurrences[keyword] += 1
    
    def get_cluster_results(self, kmeans, occurrences):
        '''Execute clustering and returns results.'''
        inputs = self.create_inputs(occurrences)
        try:
            result = kmeans.fit_predict(inputs)
        except:
            self.logger.error("ERROR! Cannot execute clustering! Missing files in malicious/benign folders?")
            raise
        return result
        
    def print_occurrences(self):
        '''Print object occurrences.'''
        self.logger.info("Benign occurrences: %s", str(self.benign_occurrences))
        self.logger.info("Malicious occurrences: %s", str(self.malicious_occurrences))
    
    
    def create_inputs(self, occurrences):
        '''Creates a occurrence vector to be used for the Ward clustering.'''
        inputs = []
        for key in occurrences:
            inputs.append([key[1]])
        return inputs
            

    def merge_occurrences(self):
        '''Joins occurrences from benign and malicious characteristic keywords and
        creates a unique list of characteristic keywords.
        MUST BE CALLED AFTER perform_clustering method.
        '''
        # Only characteristic keywords are considered in this phase. The keys are
        # sorted from their highest frequency values, so we can slice using the thresholds
        # obtained in the clustering phase.
        for couple in self.benign_occurrences[0:self.benign_threshold]:
            key = couple[0]
            value = couple[1]
            self.characteristic_occurrences[key] = value
        for couple in self.malicious_occurrences[0:self.malicious_threshold]:
            key = couple[0]
            value = couple[1]
            # If a keyword is found both in malicious and benign samples, its occurrence number is the sum between
            # the number of occurrences in malicious and benign files.    
            if key in self.characteristic_occurrences.iterkeys():
                self.characteristic_occurrences[key] += value
            else:
                self.characteristic_occurrences[key] = value
        self.characteristic_occurrences = OrderedDict(sorted(self.characteristic_occurrences.items(), key=lambda x: x[1], reverse = True))
        self.logger.debug("Occurrences: %s", str(self.characteristic_occurrences))
            
    def dump_keywords(self):
        '''Dumps all keywords in an output file that will be used during the training phase.'''
        self.logger.info("Dumping keywords...")
        if os.path.exists("clustering/keywords"):
            os.remove("clustering/keywords")    
        keywords_file = open("clustering/keywords", "a")
        for key in self.characteristic_occurrences:
            keywords_file.write(str(key) + "\n")
        keywords_file.close()
        self.logger.info("Clustering Phase complete!")

                
            
        
        
        
            
            
        
        
        