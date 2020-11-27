Pdf Malware Slayer
Created by Davide Maiorca 
Pattern Recognition and Applications Lab (PraLAB)
Department of Electrical and Electronic Engineering
University of Cagliari, Italy
davide.maiorca@diee.unica.it

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

This tool is based on the PDFMiner library (see LICENSE file inside the pdfminer folder).

INTRODUCTION
Pdf Malware Slayer is a full automatic system to analyze and detect malicious PDF files. It features a slight different version of the algorithm described
here: http://pralab.diee.unica.it/sites/default/files/maiorca_MLDM12.pdf . For more details about the differences in the algorithm, please contact me at my
mail address.

INSTRUCTIONS TO INSTALL THE SOFTWARE.
This software has been tested on Windows 8.1, Ubuntu 14.04, MacOSX 10.9 Mavericks.
To correctly use PDF Malware Slayer, you have to download and install the following components:

1) Python 2.7 (Python >= 3 is NOT supported)
2) Scikit Learn (Download and install it from http://scikit-learn.org/stable/install.html)

A MODIFIED version of PDFMiner is not included in this package. DO NOT use the official release of the library as it will not fully work on this application.

USING THE SOFTWARE

The execution of the software is divided in three parts. Clustering, Training and Test phase.
Clustering and Training need to be executed only once. Test can be executed on the files that the user want to be classified by the tool.

FIRST RUN

Here are the steps that you first have to do when using this software. This can be executed only once. If you want, you can also skip this phase (only for Mac
and Linux users) and directly replace the "clustering" and "classifier" folders with the ones inside the "premade" one.

You have to own a number of malicious and benign files. Then, you have to put them in the "malicious" and "benign" folder respectively (NOT in other folders).
After that, launch:

python slayer.py cluster

and wait for the process to be finished. A file called "keyword" should have been generated in the "clustering" folder. Then:

python slayer.py train

and wait for the process to be finished. A file called "classifier.p" should have been generated in the "classifier" folder.


TEST PHASE (NORMAL USAGE)

In order to test PDF files, just put them in the "test folder" and run:

python slayer.py test

Or you can also use another folder with the command:

python slayer -f [YOURFOLDER] test

OTHER OPTIONS

You can also enable debug mode with the flag -d True.

For any problems/bugs, please contact me at davide.maiorca@diee.unica.it




