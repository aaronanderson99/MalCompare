import os
import numpy as np 
import glob
from sklearn.cross_validation import train_test_split
import shutil

all_files = []
truths = []
print("getting all file names")
for f in glob.glob("benign/*.pdf"):
    all_files.append(f)
    truths.append(0)
for f in glob.glob("malicious/*.pdf"):
    all_files.append(f)
    truths.append(1)

print("total number of files ", len(all_files))


X_train, X_test, y_train, y_test = train_test_split(all_files, truths, test_size=0.4, random_state=42)
print("number of train files ", len(X_train))
print("number of test files ", len(X_test))

if len(all_files) == 19981:
    print("moving all files")
    for f in X_test:
        fname = f.split("/")[1]
        #print("moving "+f+" to "+"test/"+fname)
        shutil.move(f, "test/")

    print("done moving files")
    
files_after_move = []

for f in glob.glob("benign/*.pdf"):
    files_after_move.append(f)
for f in glob.glob("malicious/*.pdf"):
    files_after_move.append(f)
    
print("num test after move", len(files_after_move))








