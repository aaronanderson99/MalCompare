HOW TO RUN THE SLAYER MALWARE DETECTION PROGRAM 
Brian Zhu, CS 6220, Georgia Tech College of Computing

REQUIREMENTS: 

1) Python 2.7 (for slayer)
2) Python 3 (for calculating the confusion matrix and accuracy) 
2) Scikit Learn (Download and install it from http://scikit-learn.org/stable/install.html)

RUNNING STEPS:

0. Navigate into the Slayer folder then to the src folder. Run commands from this folder. 

1. Setup a "benign" and "malicious" folder with the full Contagio database. Create an empty folder titled "test" and "test_results". Create empty folders "clustering" and "classifier".  

2. Run the command "python2 split_test_train.py". This command will fill the test folder with the test files
	2a. The train/test ratio can be adjusted by the train_test_split call and the test_size parameter

3. Run the command "python2 slayer.py cluster". A file called "keyword" should have been generated in the "clustering" folder.

4. Run the command "python2 slayer.py train". A file called "classifier.p" should have been generated in the "classifier" folder.

5. Run the command "python2 slayer.py test". A file labeled "test_results.txt" should have been created in the the "test_results" folder. 

6. Run the command "python3 results.py". This will print the confusion matrix based on the test_results.txt file. 






