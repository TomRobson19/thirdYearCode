"""
Code for the Networks Assignment
Tom Robson - hzwr87
"""
import random


#degree distribution and diameter

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

	print(graph)


def diameter(graph):
	distances = []
	
	for node in graph:
		distances += [max_dist(graph, node)]
	
	return max(distances)

makeGroupGraph(5,4,0.4,0.1)