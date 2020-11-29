# MalCompare
6220 Project Overview: MalCompare is an ensemble classifier that simultaneously compares predictions across all of the tools we test in our experiment and computes the probability that a file is malicious. 

## (1) Source Code
Links to open-source code packages:
- PJSCan: https://sourceforge.net/p/pjscan/home/Home/
   * PJSCan is a command-line utility that uses the SVM learning algorithm to detect JavaScript-related malware in PDF Files
   * This tool is written in C++ and uses multiple third-party software libraries including the following: 
    - cmake
    - g++ 
    - libboost-thread-dev
    - libboost-filesystem-dev
    - libpdfjs 
    - libsystem
    - libsvm_oc
    - pjscan-js
- Slayer: https://pralab.diee.unica.it/en/Slayer
   * Slayer is a tool that extracts information from the structure of PDF files and uses this to classify a file as malicious or benign. 
   * This tool requires the following: 
    - Python 2.7 (for Slayer)
    - Python 3 (for calculating the confusion matrix and accuracy)
    - Scikit-Learn
- PDFRate: https://github.com/srndic/mimicus
   * PDFRate utilizes a static analysis of PDF metadata and structural features to identify malware in PDF files.
   * This tool requires the following: 
    - Python 2.7 
    - curl
    - perl 
    - matplotlib >= 1.1.1rc
    - numpy >= 1.6.1
    - scikit_learn >= 0.14.1
    - scipy >= 0.9.0
- Hidost: https://github.com/srndic/hidost
   * Hidost is a malware-detection tool that uses a structural path consolidation strategy to identify similar structures in malicious PDF files and detect malware
   * This tool requires the following:     
    - Python 2.7
    - Java
    - C++11
    - CMake >= 2.8
    - Boost Filesystem
    - Boost Program Options 
    - Boost Regex
    - Boost System
    - Boost Thread
    - Poppler 
    - SWFREtools
    - javac
    - libquickly 
   
## (2) Dataset Description
The Contagio dataset is a collection of malicious and benign PDF files contributed by malware researchers, available for download. The data can be found at http://contagiodump.blogspot.com/2010/08/malicious-documents-archive-for.html. Contact Mila Parkour for the password scheme to the raw data. The data used in this experiment consisted of 9000 benign PDF files and 10,981 malicious PDF files. 

## (3) Performance Measurement Tools
All performance measurement and experimental comparison was done by evaluating the true positives (detection rate), false positives, true negatives, false negatives, and overall accuracy of the models. 
