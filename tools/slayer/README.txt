HOW TO RUN THE SLAYER MALWARE DETECTION PROGRAM 
Brian Zhu, CS 6220, Georgia Tech College of Computing

REQUIREMENTS: 

1) Python 2.7 (for slayer)
2) Python 3 (for calculating the confusion matrix and accuracy) 
2) Scikit Learn (Download and install it from http://scikit-learn.org/stable/install.html)

RUNNING STEPS:

0. Once you have navigated into the Slayer folder, run commands from this folder. 

1. Run the command "./download_contagio.sh". This will download the whole Contagio dataset into the folder. 
	- If there is a permission error run the command "chmod +x download_contagio.sh" and rerun the previous command. 

2. Run the command "./split.sh <path to the clean/benign pdfs> <path to the dirty/malicious pdfs> <train dataset size (0.0, 1.0)>"
	If there is a permission error run the command "chmod +x split.sh" and rerun the previous command. 

3. Run the command "python2 slayer.py cluster". A file called "keyword" should have been generated in the "clustering" folder.

4. Run the command "python2 slayer.py train". A file called "classifier.p" should have been generated in the "classifier" folder.

5. Run the command "python2 slayer.py test". A file labeled "test_results.txt" should have been created in the the "test_results" folder. 

6. Run the command "python3 results.py". This will print the confusion matrix based on the test_results.txt file. 






