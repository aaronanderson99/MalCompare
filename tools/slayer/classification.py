'''
Created on 31/lug/2014

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
import os
import pickle
#import cPickle as pickle
from logs import Log
from features import PdfFeatures
from collections import Counter
from collections import OrderedDict
from sklearn.ensemble import RandomForestClassifier, ExtraTreesClassifier
from sklearn import grid_search

class Classification(Log):
    
    def __init__(self, debug):
        '''Constructor.'''
        self.debug = debug
        self.names = []
        self.errors = 0
        self.analyzed = 0
        self.set_logger(debug)
        self.set_keywords()
    
    def set_keywords(self):
        self.keywords = []
        self.inputs = []
        self.outputs = []
        if not os.path.exists("clustering/keywords"):
            self.logger.error("Clustering file not found. Please run clustering phase first")
            raise IOError("Clustering file not found. Please run clustering phase first")
        keywords_file = open("clustering/keywords")
        for keyword in keywords_file.readlines():
            self.keywords.append(keyword[0:-1])
        if self.keywords == []:
            self.logger.error("Empty keywords list. Run clustering phase first")
            raise RuntimeError("Empty keywords list. Run clustering phase first")
    
    def build_features(self, folder, is_benign=None):
        '''Builds the input feature matrix that will be sent to the classifier. When the training phase is
        executed, the outputs for the supervised learning are saved as well. When test is executed, the
        outputs list remains empty.
        '''
        if (os.path.exists(folder)):
            self.logger.info("Analyzing PDF files from folder: %s", folder)
            for sample in os.listdir(folder):
                self.logger.info("Analyzing sample: %s", sample)
                try:
                    features = PdfFeatures(folder+"/"+sample, self.debug)
                    features.get_keyword_frequencies()
                    if features.frequency == OrderedDict():
                        self.logger.error("Error: no objects have been found inside the file. Not a PDF file or Malformed PDF file")
                        continue
                except:
                    self.logger.error("Cannot retrieve feature elements for the file. Malformed PDF file?")
                    features.close_file()
                    self.errors += 1
                    continue
                features.close_file()
                # If the sample is correctly analyzed, increment its number
                self.analyzed += 1
                self.names.append(sample)
                self.logger.debug("Sample Keywords: %s", str(features.frequency))
                # Vector represents the feature vector for a sample, which is added to inputs
                vector = []
                for key in self.keywords:
                    keys = [str(x) for x in features.frequency.keys()]
                    if key in keys:
                        vector.append(features.frequency[key])
                    else:
                        vector.append(0)
                self.logger.debug("Features Vector: " + str(vector))
                self.inputs.append(vector)
                # If training is executed, outputs are marked with 1 for benign files and 0
                if is_benign:
                    self.outputs.append(1)
                elif is_benign == False:
                    self.outputs.append(0)
                else:
                    pass
                

class Training(Classification):
    '''Training class.'''
    
    def train(self):
        '''Builds features for benign and malicious files and trains the system.
        (Supervised learning).
        '''
        self.logger.info("Starting training...")
        self.build_features("benign", True)
        self.benign_analyzed = self.analyzed
        self.benign_errors = self.errors 
        self.build_features("malicious", False)
        self.malicious_analyzed = self.analyzed - self.benign_analyzed
        self.malicious_errors = self.errors - self.benign_errors
        self.logger.info("Number of analyzed benign files: %s", str(self.benign_analyzed)) 
        self.logger.info("Number of errors in benign files: %s", str(self.benign_errors))
        self.logger.info("Number of analyzed malicious files: %s", str(self.malicious_analyzed)) 
        self.logger.info("Number of errors in malicious files: %s", str(self.malicious_errors))
        parameters = {'n_estimators': [i for i in range (1, 50)]}
        self.logger.debug("Estimated parameters: %s", str(parameters))
        self.rand = RandomForestClassifier()
        #self.rand = ExtraTreesClassifier()
        self.classifier = grid_search.GridSearchCV(self.rand, parameters)
        try:
            self.classifier.fit(self.inputs, self.outputs)
        except:
            self.logger.error("Cannot train the classifier! Missing files?")
            raise
        if os.path.exists("classifier/classifier.p"):
            os.remove("classifier/classifier.p")
        pickle.dump(self.classifier, open("classifier/classifier.p", "wb"))
        self.logger.info("Training complete!")

class Test(Classification):
    '''Test class.'''
    
    def print_results(self, folder):
        '''Print classification results'''
        self.logger.info("Now printing results...")
        self.results_names = []
        for result in self.results:
            if (result == 1):
                self.results_names.append("benign")
            else:
                self.results_names.append("malicious")
                
        if os.path.exists("test_results/test_results.txt"):
            os.remove("test_results/test_results.txt")
            
        file1 = open("test_results/test_results.txt", mode='a+')
        
        for i in range(0, len(self.names)):
            self.logger.info("%s: %s", str(self.names[i]), str(self.results_names[i]))
            file_name = str(self.names[i])
            pred = str(self.results_names[i])
            file1.write("{file}: {p} \n".format(file= file_name, p=pred))
        file1.close()
        
        stats = Counter(self.results)
        self.logger.info("Number of analyzed files: %s", str(self.analyzed)) 
        self.logger.info("Number of errors: %s", str(self.errors))
        self.logger.info("Number of malicious files detected: %s", str(stats[0]))
        self.logger.info("Number of benign files detected: %s", str(stats[1]))
            
    def test(self, folder):
        '''Test samples from a chosen folder and returns the results.'''
        self.logger.info("Starting Test...")
        # If folder is default, use test folder
        if (folder == 'default'):
            self.build_features("test")
        else:
            self.build_features("test_small")
        if os.path.exists("classifier/classifier.p"):
            pkl = open("classifier/classifier.p", "rb")
            self.classifier = pickle.load(pkl)
        else:
            self.logger.error("Classifier not found. The system must be trained first. Please execute training")
            raise IOError("Classifier not found. The system must be trained first. Please execute training")
        try:
            self.results = self.classifier.predict(self.inputs)
        except:
            self.logger.error("Cannot test classifier! Missing test files?")
            raise
        self.logger.debug("Result Vector: %s", str(self.results))
        self.logger.info("Test complete!")
            
