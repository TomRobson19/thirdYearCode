"""
Code for the Part 1 of the Networks Assignment
Tom Robson - hzwr87
"""
import random
import queue
import numpy as np
import matplotlib.pyplot as plt
from collections import Counter

#############################################################################################################################

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

#############################################################################################################################

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

#############################################################################################################################

def diameter(graph):
	distances = []
	
	for node in graph:
		distances += [max_dist(graph, node)]
	
	return max(distances)

#############################################################################################################################

def average_degree_distribution_group_graphs(m,k,p,q,trials):
	distribution = {}
	
	for i in range(trials):
		graph = makeGroupGraph(m,k,p,q)

		degrees = []
		for node in graph:
			degrees += [len(graph[node])]

	degreeCount = {x:degrees.count(x)/trials for x in degrees}

	return degreeCount


#############################################################################################################################

def plot_degree_distribution(degreeCount, title, filename):
	plt.clf()
	plt.vlines(degreeCount.keys(),degreeCount.values(), 1, color='r')
	plt.xlabel("Degree", fontsize = 10)
	plt.ylabel("Occurances", fontsize = 10)
	plt.title(title)
	#plt.show()
	plt.savefig(filename)

#############################################################################################################################

def plot_diameter_vs_p(m, k, q, trials, title):
    """plot diameter of group graph versus internal probability by taking average of k trials for each data point"""
    #create arrays for plotting
    xdata = []
    ydata = []
    for p in [0.1*p for p in range(1,11)]:
        diameters = []
        print (p)
        for idx in range(trials):
            graph = makeGroupGraph(m, k, p, q)
            diam = diameter(graph)
            diameters += [diam]
        xdata += [p]
        ydata += [1.0*sum(diameters)/trials]
    plt.clf() #clears plot
    plt.xlabel('Internal Probability')
    plt.ylabel('Diameter')
    plt.title(title)
    plt.plot(xdata, ydata, marker='.', linestyle='-', color='b')
    plt.savefig('Q1_diameters.png')

#############################################################################################################################

def question1(part1=True, part2=True):
    if part1:
        print ("construct group graphs with 400 vertices, divided into 20 groups of 20")
        print ("let p take the values 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, and q=0.5 - p")
        print ("for each value create 100 graphs, find on average how many vertices have degree d, and plot the distribution")
        for p in [0.25, 0.3, 0.35, 0.4, 0.45, 0.5]:
            print ("making the plot for p="+str(p))
            q = 0.5 - p
            dist = average_degree_distribution_group_graphs(20, 20, p, q, 100)
            plot_degree_distribution(dist, "Degree Distribution of Group Graph with m=k=20, p="+str(p),"Q1_group_graph_p="+str(p)+"_degrees.png")
        print ("repeat with graphs on 400 vertices, divided into 200 groups of 2 with p=0.4")
        dist = average_degree_distribution_group_graphs(200, 2, 0.4, 0.1, 100)
        plot_degree_distribution(dist, "Degree Distribution of Group Graph with m=200, k=2, p=0.4","Q1_group_graph_m=200_degrees.png")
        print ("repeat with graphs on 400 vertices, divided into 2 groups of 200 with p=0.4")
        dist = average_degree_distribution_group_graphs(2, 200, 0.4, 0.1, 100)
        plot_degree_distribution(dist, "Degree Distribution of Group Graph with m=2, k=200, p=0.4","Q1_group_graph_m=2_degrees.png")
        print ("looking at the figures produced, notice that the shapes of the distributions are all very similar and the degrees are all very close to the average")
        print ()
    if part2:
        print ("to investigate the relationship between the diameter and p, construct, for each value of p, 20 group graphs with 80 groups, each on 5 vertices with q=0.05")
        print ("doing the calculations for p=")
        plot_diameter_vs_p(80, 5, 0.05, 20, "Diameter vs p for Group Graphs containing 80 groups of 5 vertices")
        print ("from the plot we see the relationship is rather uninteresting with the diameter not depending (much) on p")
        print ()

#############################################################################################################################

# print("Graph")

# print("Diameter")
# print(diameter(makeGroupGraph(100,10,0.4,0.1)))

# print("Plotting")
# plotDegreeDistribution(makeGroupGraph(100,10,0.4,0.1))

question1()

#############################################################################################################################