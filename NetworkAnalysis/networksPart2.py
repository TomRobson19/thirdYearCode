"""
Code for the Part 2 of the Networks Assignment
Tom Robson - hzwr87
"""
import random
import queue
import numpy as np
import matplotlib.pyplot as plt

def load_graph():
	"""
	Loads a graph from a text file.
	Then returns the graph as a dictionary.
	"""
	graph = open("coauthorship.txt")

	answerGraph = {}

	for line in graph:
		neighbours = line.split(' ')
		node = int(neighbours[0])

		if node not in answerGraph:
			answerGraph[node] = []		

		answerGraph[node] += [int(neighbours[1])]
	return answerGraph

def findKCycles(graph,k):
	cyclesPerNode = {}
	for node in graph:
		for neighbour in graph[node]:
			return 1



(load_graph()