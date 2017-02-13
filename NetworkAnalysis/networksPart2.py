"""
Code for the Part 2 of the Networks Assignment
Tom Robson - hzwr87
"""

def load_graph(graph_txt):
	"""
	Loads a graph from a text file.
	Then returns the graph as a dictionary.
	"""
	graph = open(graph_txt)

	answerGraph = {}

	for line in graph:
		neighbours = line.split(' ')
		node = int(neighbours[0])

		if node not in answerGraph:
			answerGraph[node] = []		

		answerGraph[node] += [int(neighbours[1])]
	return answerGraph

load_graph("coauthorship.txt")