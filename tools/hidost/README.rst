===================
CS 6220: Team 11
HIDOST TOOL SET-UP
===================

Overview: Hidost is set up in two parts. The first part involves extracting document structures
from the PDF files. The output of part 1 is a series of data files in LibSVM format. This data is
outputted to the data directory for part 2, which involves learning and classification based
on two selected models - SVM and Random Forest 

References: 
Šrndić, N., Laskov, P. Hidost: a static machine-learning-based detector of malicious files. 
EURASIP J. on Info. Security 2016, 22 (2016). https://doi.org/10.1186/s13635-016-0045-0

*Original README files are contained in each sub-directory

============================================================
Part 1: Structural Path Consolidation and Feature Extraction
============================================================

1. REQUIRED DEPENDENCIES
    Hidost was tested on 64-bit Ubuntu 14.04. It consists of source
    files written in C++11, Python 2.7 and Java 7 and uses CMake for
    building.

    Hidost requires Python 2.7, Java and C++11. CMake
    version 2.8 or greater is required for building Hidost.

    The following C++ libraries are required (default versions from
    Ubuntu 14.04):

    - Boost Filesystem
    - Boost Program Options
    - Boost Regex
    - Boost System
    - Boost Thread
    - Poppler

    Hidost depends on the Java library
    `SWFREtools <https://github.com/sporst/SWFREtools>`_ for SWF reading.
    Please download it and make sure its ``dissector.jar`` binary is in
    the system CLASSPATH, e.g., by putting the following line into your
    ``~/.bashrc`` file::

    export CLASSPATH=$CLASSPATH:/path/to/swfretools/dissector.jar

    Please make sure the Java compiler ``javac`` is installed as it is
    required for building the Java part of Hidost.

    Hidost also depends on
    `libquickly <https://github.com/srndic/libquickly>`_, a C++ library
    for multiprocessing. Please install it before installing Hidost.

2. BUILD HIDOST
    Hidost uses the usual CMake procedure for building. Here are the
    individual steps (assuming you are in the file hidost-part-1):

    mkdir build
    cd build
    cmake ..
    make

    After these steps have been run successfully, both the PDF part
    (consisting of C++ executables) and the SWF part (consisting of
    a Python script and a Java class file) should be built.

3. INSTALLATION

    There is no need to install any of the C++ executables as they are
    inteded to be run from their build directory. However, if you would
    wish to do so, you can install them the following way. ``cd`` to
    the build directory as created in the build procedure description
    above and type::

    make install

    On Ubuntu, you should use sudo::

    sudo make install

    There is no installation mechanism for Python scripts and Java
    classes because they too are not required to be installed system-wide.

4. FILE PREPARATION
    Run pathlist.py. This will create two text files, bpdfx.txt and mpdfs.txt,
    which contain a list of path names to the datasets. 
        python3 pathlist.py

    Position these files in the directory where Hidost has been built. 
        cd build/

5. CACHE DIRECTORIES

    In order to avoid having to extract tree structures from PDF
    files multiple times, we will cache the extracted structures
    in cache directories, for benign and malicious files separately:
        
        mkdir cache-ben cache-mal
       ./src/cacher -i bpdfs.txt --compact --values -c cache-ben/ \
       -t10 -m256
       ./src/cacher -i mpdfs.txt --compact --values -c cache-mal/ \
       -t10 -m256

    We will need the absolute paths of all non-empty cached PDF
    structures in the following steps::

       find $PWD/cache-ben -name '*.pdf' -not -empty >cached-bpdfs.txt
       find $PWD/cache-mal -name '*.pdf' -not -empty >cached-mpdfs.txt
       cat cached-bpdfs.txt cached-mpdfs.txt >cached-pdfs.txt

6. STRUCTURAL PATH COUNT
    Now we will count in how many PDF files each of the PDF
    structural paths occur:

    ./src/pathcount -i cached-pdfs.txt -o pathcounts.bin

7. FEATURE SELECTION
    The next step is feature selection. We will only take into account
    structural paths present in at least 1,000 PDF files in our
    dataset:

        ./src/feat-select -i pathcounts.bin -o features.nppf -m1000

8. FEATURE OUTPUT
    Finally, we will extract the selected features from all files and
    store the result in the output file ``data.libsvm``::

        ./src/feat-extract -b cached-bpdfs.txt -m cached-mpdfs.txt \
        -f features.nppf --values -o data.libsvm

    The output file ``data.libsvm`` can now be used for learning and
    classification.

================================================================
Part 2: Training/Testing Machine Learning Classification Models
================================================================

1. DATA 
    The data outputted from Part 1 along with the PDF data should now be 
    moved to hidost-part-2/data

2. TRAINING AND TESTING 
    To reproduce results, navigate to the root directory of part 2 and run
    make all. 
        cd hidost-part-2
        make all

    All plots and results are saved in the plots/ directory

===============================
Part 3: VirusTotal Experiment
===============================

1. RUN SCRIPTS
    Return tc /hidost-part-1 and run virusTotalComparison.py
    Compare model results to antivirus engines on VirusTotal
