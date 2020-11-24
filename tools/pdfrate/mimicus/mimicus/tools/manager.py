"""
Confusion Matrix (cm)
[
	[True Pos, False Pos]
	[True Neg, False Neg]
]
"""
class ConfusionMatrix:
	def __init__(self):
		self.cm = [[0,0],[0,0]]

	def add(self, predictionIsPositive, predictionIsCorrect):
		self.cm[not predictionIsPositive][not predictionIsCorrect] += 1

	def get(self):
		return self.cm

	def printCM(self):
		print("     True  False")
		print("Pos "+str(self.cm[0][0]).rjust(5)+"  "+str(self.cm[0][1]).rjust(5))
		print("Neg "+str(self.cm[1][0]).rjust(5)+"  "+str(self.cm[1][1]).rjust(5))