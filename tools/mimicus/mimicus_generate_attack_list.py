from os import path, listdir, walk
import sys

def main():
	currFolder = path.dirname(path.realpath(__file__))

	if len(sys.argv) < 2:
		# print("Test Directory not passed in as command line argument. Using default directory.")
		testDir = path.join(currFolder, "data", "test-set")
	else:
		testDir = sys.argv[1]
	# print("Generating new attack list from files in "+testDir)

	attackList = open(path.join(currFolder, "data", "attack.list"), "w")
	count = 0
	for root, dirs, files in walk(testDir):
		for file in files:
			attackList.writelines(path.join(root, file)+"\n")
			count += 1
	# print("Created new attack.list with "+str(count)+" files.")
	attackList.close()

if __name__ == '__main__':
	main()