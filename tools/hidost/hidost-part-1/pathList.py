# CS 6220
# run this Python script to create two text files, bpdfs.txt and mpdfs.txt, with paths to 
# benign and malicious files 

import os
import shutil
results = open('hidost_pred_SVM.txt', 'w')

for file in os.listdir("./CLEAN"):
    if file.endswith(".pdf"):
        benign.write("%s\n" % (os.path.join("../CLEAN", file)))
# for file in os.listdir("./MALICIOUS"):
#     if file.endswith(".pdf"):
#         benign.write("%s\n" % (os.path.join("./MALICIOUS", file)))

benign.close()
malicious.close()
pdf_files = ['bpdfs.txt', 'mpdfs.txt']
for f in pdf_files: 
    shutil.move(f, 'build_temp')