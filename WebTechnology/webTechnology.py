'''
Code to summarise a text document

Code based on link provided in lecture slides

https://technowiki.wordpress.com/2011/08/27/latent-semantic-analysis-lsa-tutorial/

text files supplied on the command line

run with python -W ignore webTechnology.py filename.txt
'''

#####################################################################

from scipy.linalg import svd
from scipy.cluster import vq
from scipy.spatial import distance
from math import log
import string
import numpy as np
import matplotlib.pyplot as plt
import sys

np.set_printoptions(formatter={'float': '{: 0.3f}'.format})

#replace these with parsing full text files and full stopword list
#read document in as a text file and split on full stops

class LSA(object):
	def __init__(self, stopwords):
		'''Initialises variables for class'''
		self.stopwords = stopwords 
		self.wordDictionary = {} 
		self.documentCount = 0
		self.translator = str.maketrans({key: None for key in string.punctuation})

	def parseDocument(self, doc):
		'''Reads in the document and populates the word Dictionary after removing punctuation and stopwords'''
		words = doc.split(); 
		for w in words:
			w = w.lower().translate(self.translator) 
			if w in self.stopwords:
				continue
			elif w in self.wordDictionary:
				self.wordDictionary[w].append(self.documentCount)
			else:
				self.wordDictionary[w] = [self.documentCount]
		self.documentCount += 1

	def buildCountMatrix(self):
		'''creates a matrix for all words that appear more than once'''
		self.keys = []
		for k in self.wordDictionary.keys():
			if len(self.wordDictionary[k]) > 1:
				self.keys.append(k)
		self.keys.sort() 
		self.countMatrix = np.zeros([len(self.keys), self.documentCount]) 
		for i, k in enumerate(self.keys):
			for d in self.wordDictionary[k]:
				self.countMatrix[i,d] += 1

	def printCountMatrix(self):
		'''Prints out the count matrix'''
		print (self.countMatrix)

	def TFIDF(self):
		'''Weight words using the Term Frequency – Inverse Document Frequency method'''
		WordsPerDoc = np.sum(self.countMatrix, axis=0)
		DocsPerWord = np.sum(np.asarray(self.countMatrix > 0), axis=1) 
		rows, cols = self.countMatrix.shape
		for i in range(rows):
			for j in range(cols):
				self.countMatrix[i,j] = (self.countMatrix[i,j] / WordsPerDoc[j]) * log(float(cols) / DocsPerWord[i])

	def calculateSVD(self):
		'''Employs singluar value decomposition to create U, S, and Vt'''
		self.U, self.S, self.Vt = svd(self.countMatrix)

	def printSVD(self):
		'''Prints U,S and Vt to show resuults of singular value decomposition'''
		print ('Here are the singular values')
		print (self.S)
		print ('Here are the first 3 columns of the U matrix')
		print (-1*self.U[:, 0:3])
		print ('Here are the first 3 rows of the Vt matrix')
		print (-1*self.Vt[0:3, :])

	def plot(self):
		'''Clusters the documents and plots them using MatPlotLib'''
		self.V = np.transpose(self.Vt)
		ux = -1*self.U[:, 1:2]
		uy = -1*self.U[:, 2:3]
		vx = -1*self.V[:, 1:2]
		vy = -1*self.V[:, 2:3]

		plt.plot(ux,uy,'r.')
		for i in range (0,len(ux)):
			plt.annotate(self.keys[i],xy=(ux[i],uy[i]),xytext=(ux[i]+0.01,uy[i]),fontsize=10)

		plt.plot(vx,vy,'b.')
		for i in range (0,len(vx)):
		 	plt.annotate(str(i+1),xy=(vx[i],vy[i]),xytext=(vx[i]+0.01,vy[i]),fontsize=10)

		coordinates = np.zeros([len(vx),3])
		for i in range(0,len(vx)):
			coordinates[i][0] = vx[i]
			coordinates[i][1] = vy[i]

		#determine number of clusters
		numberOfClusters = round((self.countMatrix.shape[0] * self.countMatrix.shape[1]) / (np.count_nonzero(self.countMatrix)))  

		attempts = 100
		bestCentres = []
		bestLabels = []
		bestRadii = []
		minimumSSE = 0

		for a in range (0, attempts):

			centres,labels = vq.kmeans2(coordinates[: , 0:2],numberOfClusters)
			radii = []
			sse = 0

			for i in range(0,len(coordinates)):
				coordinates[i][2] = labels[i]

			x = centres[:, 0:1]
			y = centres[:, 1:2]

			for i in range(0,numberOfClusters):
				radii.append(0)
				for j in range(0,len(labels)):
					if (labels[j] == i):
						possibleRadius = distance.euclidean((x[i],y[i]),(coordinates[j][0],coordinates[j][1]))
						sse += possibleRadius**2
						if (possibleRadius > radii[i]):
							radii[i] = possibleRadius

			if minimumSSE == 0 or sse < minimumSSE:
				minimumSSE = sse
				bestCentres = centres
				bestLabels = labels
				bestRadii = radii


		x = bestCentres[:, 0:1]
		y = bestCentres[:, 1:2]
		for i in range(0,numberOfClusters):
			if(bestRadii[i] == 0):
				bestRadii[i] = 0.1
			plt.gca().add_patch(plt.Circle((x[i],y[i]),bestRadii[i],fc='y'))

		plt.show()

def main():
	'''Reads in files and calls function for summarisation'''
	file = open(sys.argv[1], "r")
	document = file.read().split('\n')
	file.close()

	#read in stopwords from file
	stopwordsFile = open("stopwords.txt", "r")
	stopwords = stopwordsFile.read().split(' ')
	stopwordsFile.close()

	#create instance of class
	mylsa = LSA(stopwords)
	
	for t in document:
		#parse each document
		mylsa.parseDocument(t)
	
	#create and print count matrix
	mylsa.buildCountMatrix()
	#mylsa.printCountMatrix()

	#weight results using Term Frequency – Inverse Document Frequency
	mylsa.TFIDF()

	#Uses Singular Value decomposition
	mylsa.calculateSVD()
	#mylsa.printSVD()

	#cluster results and plot them	
	mylsa.plot()

if __name__ == "__main__":
	main()