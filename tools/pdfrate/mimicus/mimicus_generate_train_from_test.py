import os
from os import path, listdir, mkdir
import sys
import random
import csv
import numpy as np
# from sklearn.model_selection import KFold

# Change this list to represent the start and end of the testing set in each train/test split
# For example, the following represents three 60/40 splits with the first/middle/end 40% of the input representing the testing set):
# testIndices = [(0, .4), (.3, .7), (.6, 1)]
testIndices = [(0,0)]

dataFolder = "/home/pdfman/Downloads/project/mimicus/data"

numSplits = len(testIndices)
# seed = 0
newCSVFolder = path.join(dataFolder, "newCSVs")

def main():
	raw_input("\nPress Enter to continue.")
	print("")
	# random.seed(seed)
	if not path.exists(newCSVFolder):
		mkdir(newCSVFolder)
	for file in ["surrogate-scaled - original.csv", "surrogate - original.csv", "contagio-scaled - original.csv", "contagio - original.csv"]: # for file in listdir(dataFolder):
		if not path.splitext(file)[1] == ".csv":
			continue
		# First, take the original CSV and store its contents as an array
		ocsv = open(path.join(dataFolder, file), 'r')
		originalCSV = csv.reader(ocsv)
		allOriginalRows = []
		isHeader = True
		for row in originalCSV:
			if isHeader:
				allOriginalRows.append(row)
				isHeader = False
			elif file in ["surrogate-scaled - original.csv", "surrogate - original.csv"]:
				allOriginalRows.append(row)
			else:
				compositePath = "/home/pdfman/Downloads/project/aaron_scripts/split_dir/test/composite"
				fullPathClean = path.join("/home/pdfman/Downloads/project/aaron_scripts/split_dir/test/composite", "CLEAN_"+path.basename(row[1]))
				fullPathDirty = path.join("/home/pdfman/Downloads/project/aaron_scripts/split_dir/test/composite", "DIRTY_"+path.basename(row[1]))
				# if not (path.exists(fullPathClean) or path.exists(fullPathDirty)):
				# 	allOriginalRows.append(row)
				# elif not (path.exists(path.splitext(fullPathClean)[0]) or path.exists(path.splitext(fullPathDirty)[0])):
				# 	newRow = row[:]
				# 	newRow[1] = path.splitext(row[1])[0]
				# 	allOriginalRows.append(newRow)
				if not (path.exists(fullPathClean) or path.exists(fullPathDirty) or path.exists(path.splitext(fullPathClean)[0]) or path.exists(path.splitext(fullPathDirty)[0])):
					allOriginalRows.append(row)
		ocsv.close()
		csvHeader = allOriginalRows.pop(0)
		# Next, look for the first TRUE row
		firstTrue = 0
		for row in allOriginalRows:
			if row[0] == "TRUE":
				break
			firstTrue += 1
		# Then, cross-validate (benign and malicious and done separately and combined at the end to ensure an approx. equal distribution of both within each fold)
		allBenignRows = allOriginalRows[:firstTrue]
		allMaliciousRows = allOriginalRows[firstTrue:]
		numBen = len(allBenignRows)
		numMal = len(allMaliciousRows)
		numOrig = len(allOriginalRows)
		print("Benign:    "+str(numBen))
		print("Malicious: "+str(numMal))
		print("Total:     "+str(numOrig))
		# kfn = KFold(n_splits=numSplits, shuffle=False)
		# benign
		if numBen < numSplits:
			for count in range(1, numSplits+1):
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben "+str(count)+".csv"
				with open(path.join(newCSVFolder, newFileName), 'w') as ncsv:
					newCSV = csv.writer(ncsv)
					newCSV.writerow(csvHeader)
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben attack"+str(count)+".list"
				with open(path.join(newCSVFolder, newFileName), 'w') as newAttackList:
					pass
		else:
			count = 1
			# for train_index, test_index in kfn.split(allBenignRows):
			# 	currTrainingData = np.take(allBenignRows, train_index, axis=0)
			# 	currTestingData = np.take(allBenignRows, test_index, axis=0)
			for test_start_index, test_end_index in testIndices:
				currTrainingData = allBenignRows[:int(numBen*test_start_index)] + allBenignRows[int(numBen*test_end_index):]
				currTestingData = allBenignRows[int(numBen*test_start_index):int(numBen*test_end_index)]
				# Create a new CSV for the benign training data
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben "+str(count)+".csv"
				with open(path.join(newCSVFolder, newFileName), 'w') as ncsv:
					newCSV = csv.writer(ncsv)
					newCSV.writerow(csvHeader)
					newCSV.writerows(currTrainingData)
				# Create a new attack.list for the benign testing data
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben attack"+str(count)+".list"
				with open(path.join(newCSVFolder, newFileName), 'w') as newAttackList:
					for row in currTestingData:
						newAttackList.writelines(row[1]+"\n")
				count += 1
		# malicious
		if numMal < numSplits:
			for count in range(1, numSplits+1):
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal "+str(count)+".csv"
				with open(path.join(newCSVFolder, newFileName), 'w') as ncsv:
					pass
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal attack"+str(count)+".list"
				with open(path.join(newCSVFolder, newFileName), 'w') as newAttackList:
					pass
		else:
			count = 1
			# for train_index, test_index in kfn.split(allMaliciousRows):
			# 	currTrainingData = np.take(allMaliciousRows, train_index, axis=0)
			# 	currTestingData = np.take(allMaliciousRows, test_index, axis=0)
			for test_start_index, test_end_index in testIndices:
				currTrainingData = allMaliciousRows[:int(numMal*test_start_index)] + allMaliciousRows[int(numMal*test_end_index):]
				currTestingData = allMaliciousRows[int(numMal*test_start_index):int(numMal*test_end_index)]
				# Create a new CSV for the malicious training data
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal "+str(count)+".csv"
				with open(path.join(newCSVFolder, newFileName), 'w') as ncsv:
					newCSV = csv.writer(ncsv)
					# newCSV.writerow(csvHeader)
					newCSV.writerows(currTrainingData)
				# Create a new attack.list for the malicious testing data
				newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal attack"+str(count)+".list"
				with open(path.join(newCSVFolder, newFileName), 'w') as newAttackList:
					for row in currTestingData:
						newAttackList.writelines(row[1]+"\n")
				count += 1
		# Finally, combine pairs of benign/malicious into one each
		for i in range(1, numSplits+1):
			benTrain = ""
			malTrain = ""
			benTest = ""
			malTest = ""

			# Training CSV
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben "+str(i)+".csv"
			with open(path.join(newCSVFolder, newFileName), 'r') as ncsv:
				benTrain = ncsv.read()
			os.remove(path.join(newCSVFolder, newFileName))
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal "+str(i)+".csv"
			with open(path.join(newCSVFolder, newFileName), 'r') as ncsv:
				malTrain = ncsv.read()
			os.remove(path.join(newCSVFolder, newFileName))
			allTrain = benTrain + malTrain
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - "+str(i)+".csv"
			with open(path.join(newCSVFolder, newFileName), 'w') as newFile:
				newFile.write(allTrain)
			print("Created new CSV: "+newFileName)

			# Testing (attack) List
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - ben attack"+str(i)+".list"
			with open(path.join(newCSVFolder, newFileName), 'r') as ncsv:
				benTrain = ncsv.read()
			os.remove(path.join(newCSVFolder, newFileName))
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - mal attack"+str(i)+".list"
			with open(path.join(newCSVFolder, newFileName), 'r') as ncsv:
				malTrain = ncsv.read()
			os.remove(path.join(newCSVFolder, newFileName))
			allTrain = benTrain + malTrain
			newFileName = path.splitext(file)[0].replace(" - original", "")+" - attack"+str(i)+".list"
			with open(path.join(newCSVFolder, newFileName), 'w') as newFile:
				newFile.write(allTrain)
			print("Created new attack list: "+newFileName)

if __name__ == '__main__':
	main()