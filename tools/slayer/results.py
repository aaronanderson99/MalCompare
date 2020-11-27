import os
import numpy as np 
from sklearn.metrics import confusion_matrix

fp = "test_results/test_results.txt"
result_arr = []

with open(fp) as file:
    line = file.readline()
    while line:
        splitted = line.split(":")
        pred = splitted[1]
        if "benign" in pred:
            pred = "benign"
        elif "malicious" in pred:
            pred = "malicious"
        else:
            pred = ""
        result_arr.append((line[0:5], pred))
        line = file.readline()
ground_truth = []
predictions = []

for result in result_arr:
    if result[0] == "CLEAN":
        ground_truth.append(0)
    else:
        ground_truth.append(1)

    if result[1] == "benign":
        predictions.append(0)
    else:
        predictions.append(1)

ground_truth = np.array(ground_truth)
predictions = np.array(predictions)

cm = confusion_matrix(ground_truth, predictions)

tn, fp, fn, tp = confusion_matrix(ground_truth, predictions).ravel()
print(cm)
print((tn, fp, fn, tp))
# print(result_arr[0])
# print(result_arr)
# print(len(result_arr))
correct = 0
for i in range(len(ground_truth)):
    if ground_truth[i] == predictions[i]:
        correct += 1
print("accuracy ", float(correct) / len(ground_truth))
file.close()









