"""
Code for the Part 1 of the Networks Assignment
Tom Robson - hzwr87
"""
import random
import queue
import numpy as np
import matplotlib.pyplot as plt
from collections import Counter

def makeGroupGraph(m,k,p,q):
	random.seed(m*k)
	numberOfNodes = m*k
	graph = {}
	group = {}

	#store groups of the nodes
	for node in range(numberOfNodes):
		group[node] = node//k
		graph[node] = []

	for node in range(numberOfNodes):
		for neighbour in range(node+1, numberOfNodes):
			if(group[node] == group[neighbour] and random.random() < p):
				graph[node] += [neighbour]
				graph[neighbour] += [node]
			elif(group[node] != group[neighbour] and random.random() < q):
				graph[node] += [neighbour]
				graph[neighbour] += [node]

	return graph

def max_dist(graph, source):
	q = queue.Queue()
	found = {}
	distance = {}
	for node in graph:
		found[node] = 0
		distance[node] = -1
	max_distance = 0
	found[source] = 1
	distance[source] = 0
	q.put(source)
	while q.empty() == False:
		current = q.get()
		for neighbour in graph[current]:
			if found[neighbour] == 0:
				found[neighbour] = 1
				distance[neighbour] = distance[current] + 1
				max_distance = distance[neighbour]
				q.put(neighbour)
	return max_distance

def diameter(graph):
	distances = []
	
	for node in graph:
		distances += [max_dist(graph, node)]
	
	return max(distances)

def plotDegreeDistribution(graph):
	degrees = []

	for node in graph:
		degrees += [len(graph[node])]

	degreeCount = {x:degrees.count(x) for x in degrees}

	plt.bar(degreeCount.keys(),degreeCount.values(), 1, color='r')
	plt.xlabel("Degree", fontsize = 10)
	plt.ylabel("Occurances", fontsize = 10)
	plt.title("Degree Distribution", fontsize = 20)
	plt.show()

print("Graph")

print("Diameter")
print(diameter(makeGroupGraph(100,10,0.4,0.1)))

print("Plotting")
plotDegreeDistribution(makeGroupGraph(100,10,0.4,0.1))