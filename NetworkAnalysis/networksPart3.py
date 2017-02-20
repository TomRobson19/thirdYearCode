"""
Code for the Part 3 of the Networks Assignment
Tom Robson - hzwr87
"""
import random
import queue
import numpy as np
import matplotlib.pyplot as plt

#############################################################################################################################

random.seed()

def make_group_graph(m,k,p,q):
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

def make_random_graph(num_nodes, prob):
    """Returns a dictionary to a random graph with the specified number of nodes
    and edge probability.  The nodes of the graph are numbered 0 to
    num_nodes - 1.  For every pair of nodes, i and j, the pair is considered
    twice: once to add an edge (i,j) with probability prob, and then to add an
    edge (j,i) with probability prob. 
    """
    #initialize empty graph
    random_graph = {}
    #consider each vertex
    for i in range(num_nodes):
        random_graph[i] = []

    for vertex in range(num_nodes):
        for neighbour in range(vertex+1, num_nodes):
            random_number = random.random()
            if random_number < prob:
                random_graph[vertex] += [neighbour]
                random_graph[neighbour] += [vertex]        
        #add vertex with list of out_ neighbours

    return random_graph

#############################################################################################################################

class PATrial:
    """
    Used when each new node is added in creation of a PA graph.
    Maintains a list of node numbers with multiple instances of each number.
    The number of instances of each node number are in proportion to the
    probability that it is linked to.
    Uses random.choice() to select a node number from this list for each trial.
    """

    def __init__(self, num_nodes):
        """
        Initialize a PATrial object corresponding to a 
        complete graph with num_nodes nodes
        
        Note the initial list of node numbers has num_nodes copies of
        each node number
        """
        self._num_nodes = num_nodes
        self._node_numbers = [node for node in range(num_nodes) for dummy_idx in range(num_nodes)]


    def run_trial(self, num_nodes):
        """
        Conduct num_node trials using by applying random.choice()
        to the list of node numbers
        
        Updates the list of node numbers so that the number of instances of
        each node number is in the same ratio as the desired probabilities
        
        Returns:
        Set of nodes
        """       
        #compute the neighbors for the newly-created node
        new_node_neighbors = set()
        for dummy_idx in range(num_nodes):
            new_node_neighbors.add(random.choice(self._node_numbers))
        # update the list of node numbers so that each node number 
        # appears in the correct ratio
        self._node_numbers.append(self._num_nodes)
        self._node_numbers.extend(list(new_node_neighbors))        
        #update the number of nodes
        self._num_nodes += 1
        return list(new_node_neighbors)
    
def make_complete_graph(num_nodes):
    """Takes the number of nodes num_nodes and returns a dictionary
    corresponding to a complete directed graph with the specified number of
    nodes. A complete graph contains all possible edges subject to the
    restriction that self-loops are not allowed. The nodes of the graph should
    be numbered 0 to num_nodes - 1 when num_nodes is positive. Otherwise, the
    function returns a dictionary corresponding to the empty graph."""
    #initialize empty graph
    complete_graph = {}
    #consider each vertex
    for vertex in range(num_nodes):
        #add vertex with list of neighbours
        complete_graph[vertex] = list(set([j for j in range(num_nodes) if j != vertex]))
    return complete_graph

def make_PA_Graph(total_nodes, out_degree):
    """creates a PA_graph on total_nodes where each vertex is iteratively
    connected to a number of existing nodes equal to out_degree"""
    #initialize graph by creating complete graph and trial object
    PA_graph = make_complete_graph(out_degree)
    trial = PATrial(out_degree)
    
    for vertex in range(out_degree, total_nodes):
        neighbours = trial.run_trial(out_degree)

        for x in neighbours:
            PA_graph[x] += [vertex]
        PA_graph[vertex] = neighbours

    for x in PA_graph:
        PA_graph[x] = list(PA_graph[x])
    
    return PA_graph

#############################################################################################################################

def search_random_graph(random_graph,start,end):

#############################################################################################################################

def search_PA_graph(PA_graph,start,end):

#############################################################################################################################

def search_group_graph(group_graph,start,end):

#############################################################################################################################

random_graph = make_random_graph(1560, 0.035)

PA_graph = make_PA_Graph(1560, 36) 

group_graph = make_group_graph(40, 39, 0.45, 0.05)

#############################################################################################################################