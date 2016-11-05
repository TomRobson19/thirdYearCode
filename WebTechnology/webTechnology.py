from scipy.linalg import svd
from scipy.cluster import vq
from scipy.spatial import distance

from math import log

import string
import numpy as np

import matplotlib.pyplot as plt

np.set_printoptions(formatter={'float': '{: 0.3f}'.format})

#replace these with parsing full text files and full stopword list
#read document in as a text file and split on full stops

titles = [ 
"The Neatest Little Guide to Stock Market Investing", 
"Investing For Dummies, 4th Edition", 
"The Little Book of Common Sense Investing: The Only Way to Guarantee Your Fair Share of Stock Market Returns", 
"The Little Book of Value Investing", 
"Value Investing: From Graham to Buffett and Beyond", 
"Rich Dad's Guide to Investing: What the Rich Invest in, That the Poor and the Middle Class Do Not!", 
"Investing in Real Estate, 5th Edition", 
"Stock Investing For Dummies", 
"Rich Dad's Advisors: The ABC's of Real Estate Investing: The Secrets of Finding Hidden Profits Most Investors Miss" 
]


stopwords = ['and','edition','for','in','little','of','the','to']
# stopwordsFile = open("stopwords.txt", "r")
# stopwords = stopwordsFile.read().split(' ')
# stopwordsFile.close()


class LSA(object):
	def __init__(self, stopwords):
		self.stopwords = stopwords 
		self.wdict = {} 
		self.dcount = 0
		self.translator = str.maketrans({key: None for key in string.punctuation})

	def parse(self, doc):
		words = doc.split(); 
		for w in words:
			w = w.lower().translate(self.translator) 
			if w in stopwords:
				continue
			elif w in self.wdict:
				self.wdict[w].append(self.dcount)
			else:
				self.wdict[w] = [self.dcount]
		self.dcount += 1

	def build(self):
		self.keys = [k for k in self.wdict.keys() if len(self.wdict[k]) > 1] 
		self.keys.sort() 
		self.A = np.zeros([len(self.keys), self.dcount]) 
		for i, k in enumerate(self.keys):
			for d in self.wdict[k]:
				self.A[i,d] += 1

	def TFIDF(self):
		WordsPerDoc = np.sum(self.A, axis=0)
		DocsPerWord = np.sum(np.asarray(self.A > 0), axis=1) 
		rows, cols = self.A.shape
		for i in range(rows):
			for j in range(cols):
				self.A[i,j] = (self.A[i,j] / WordsPerDoc[j]) * log(float(cols) / DocsPerWord[i])

	def calc(self):
		self.U, self.S, self.Vt = svd(self.A)

	def printA(self):
		print (self.A)

	def printSVD(self):
		print ('Here are the singular values')
		print (self.S)
		print ('Here are the first 3 columns of the U matrix')
		print (-1*self.U[:, 0:3])
		print ('Here are the first 3 rows of the Vt matrix')
		print (-1*self.Vt[0:3, :])

	def plot(self):
		self.V = np.transpose(self.Vt)
		ux = -1*self.U[:, 1:2]
		uy = -1*self.U[:, 2:3]
		vx = -1*self.V[:, 1:2]
		vy = -1*self.V[:, 2:3]

		plt.plot(ux,uy,'r.')
		for i in range (0,len(ux)):
			plt.annotate(self.keys[i],xy=(ux[i],uy[i]),xytext=(ux[i]+0.01,uy[i]))

		plt.plot(vx,vy,'b.')
		for i in range (0,len(vx)):
		 	plt.annotate(str(i+1),xy=(vx[i],vy[i]),xytext=(vx[i]+0.01,vy[i]))

		coordinates = np.zeros([len(vx),3])
		for i in range(0,len(vx)):
			coordinates[i][0] = vx[i]
			coordinates[i][1] = vy[i]

		#determine number of clusters
		numberOfClusters = int((self.A.shape[0] * self.A.shape[1]) / (np.count_nonzero(self.A)))  

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
			plt.gca().add_patch(plt.Circle((x[i],y[i]),bestRadii[i],fc='y'))


		#need heuristic to determine quality of cluster
		#radius giving wierd results, optimise kmeans with heuristic and then experiment again


		plt.show()

def main():
	mylsa = LSA(stopwords)
	for t in titles:
		mylsa.parse(t)
	mylsa.build()
	#mylsa.printA()
	#mylsa.TFIDF()
	mylsa.calc()
	#mylsa.printSVD()
	mylsa.plot()


main()