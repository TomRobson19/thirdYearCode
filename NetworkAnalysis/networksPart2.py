"""
Code for the Part 2 of the Networks Assignment
Tom Robson - hzwr87
"""
import random
import queue
import numpy as np
import matplotlib.pyplot as plt

def load_graph(graph_txt):
    """
    Loads a graph from a text file.
    Then returns the graph as a dictionary.
    """
    graph_file = open(graph_txt)
    graph_text = graph_file.read()
    graph_lines = graph_text.split('\n')
    
    graph = {}
    for line in graph_lines:                    #read each edge
        link = line.split()                     #split into node and the neighbour it links to
        node = int(link[0])-1
        neighbour = int(link[1])-1
        if node != neighbour:                                   #check that node is not joined to itself
            if node in graph and neighbour not in graph[node]:  #if node already found and edge not already found 
                graph[node] += [neighbour]                      #add to its list of neighbours
            elif node not in graph:
                graph[node] = [neighbour]                       #if node seen for first time add to graph            
            if neighbour in graph and node not in graph[neighbour]:              
                graph[neighbour] += [node]                      #if neighbour already found add node to its list of neighbours unless already there
            elif neighbour not in graph:
                graph[neighbour] = [node]                       #if neighbour seen for first time add to graph
    print ("Loaded graph with", len(graph), "vertices and", sum([len(graph[vertex]) for vertex in graph])//2 ,"edges")
    return graph

def four_cycles(graph, vertex):
    """counts the number of 4-cycles containing vertex in graph"""
    count = 0
    for i in range(len(graph[vertex])):
        for j in range(i + 1, (len(graph[vertex]))):
        	node1 = graph[vertex][i]
        	node2 = graph[vertex][j]
        	for neighbour in graph[node2]:
        		if neighbour != vertex and neighbour in graph[node1]:
        			count += 1
    return count


def five_cycles(graph, vertex):
    """counts the number of 5-cycles containing vertex in graph"""
    count = 0
    for i in range(len(graph[vertex])):
        for j in range(i + 1, (len(graph[vertex]))):
        	node1 = graph[vertex][i]
        	node2 = graph[vertex][j]
        	for neighbour1 in graph[node1]:
        		if neighbour1 != vertex and neighbour1 != node2:
        			for neighbour2 in graph[node2]:
        				if neighbour2 != vertex and neighbour2 != node1 and neighbour2 in graph[neighbour1]:
		        			count += 1
    return count

graph = load_graph("coauthorship.txt")

for i in graph:
	#print(four_cycles(graph,i))
	print(five_cycles(graph,i))