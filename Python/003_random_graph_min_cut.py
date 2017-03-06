#
# Created by 14chanwa on 2017.03.02
#

# Implements undirected graphs (composed of vertexes and edges)

# Implements Karger's random minimum cut algorithm (randomly cut an edge until the graph is partitionned in 2).
# If the graph has n vertexes and the random min cut algorithm is run n**2 * log(n) times, the probability of NOT 
# founding the minimum cut is inferior to 1/n.

import math
import random


class Vertex:
    """
    A class implementing vertexes in an undirected graph.
    ## ATTRIBUTES
    index: int: the index of the vertex (arbitrary, must be unique).
    incident_edges: set: a set of pointers to the edges incident to this vertex.
    current_partition: int: the index of the current partition to which this vertex belongs. Default: index.
    """
    def __init__(self, index):
        """
        Builds a vertex and attributes the given index.
        :param index:
        """
        self.index = index
        self.incident_edges = set()
        self.current_partition = index

    def add_edge(self, edge):
        """
        Adds the provided edge pointer to the incident edges of the vertex.
        :param edge:
        :return:
        """
        self.incident_edges.add(edge)

    def remove_edge(self, edge):
        """
        Removes the provided edge pointer from the incident edges of the vertex.
        Will NOT throw exception if the edge does not belong to the incident edges.
        :param edge:
        :return:
        """
        if edge in self.incident_edges:
            self.incident_edges.remove(edge)

    def clear_edges(self):
        """
        Removes ALL edges from incident edges.
        :return:
        """
        self.incident_edges.clear()


class Edge:
    """
    A class implementing edges in an undirected graph.
    Convention: the index of vertex1 should be inferior to the index of vertex2.
    ## ATTRIBUTES:
    vertex1: Vertex: one of the extremities of the edge.
    vertex2: Vertex: the other extremity of the edge.
    """
    def __init__(self, vertex1, vertex2):
        """
        Builds an edge linking vertex1 to vertex2.
        :param vertex1:
        :param vertex2:
        """
        self.vertex1 = vertex1
        self.vertex2 = vertex2


class Graph:
    """
    A class implementing an undirected graph.
    vertexes: dict: a dictionnary with couples (key, vertex) where key is the index of the vertex.
    edges: set: a set containing all the edges of the graph in its current state. It is modified on run.
    edges_base: set: a set containing all the edges of the graph in its start state. Never modified after init.
    partition_indexes: the current active partition ids for the random min cut algorithm. Init size: 1..n, end size: 2.
    """
    def __init__(self, graph_array):
        """
        Initializes the graph given an integer array.
        :param graph_array: should be formatted as a list of lines, each line corresponding to one vertex. On each line,
        the first integer should be the index of the vertex. The following integers should be the indexes of the
        vertexes the current vertex shares an edge with. For instance:
        1 3 4
        2 4
        3 1
        4 1 2
        """
        self.vertexes = {}
        self.edges = set()
        self.edges_base = set()
        # Create vertexes
        for graph_array_line in graph_array:
            self.vertexes[graph_array_line[0]] = Vertex(graph_array_line[0])
        # Create edges
        for graph_array_line in graph_array:
            for i in range(1, len(graph_array_line)):
                if graph_array_line[0] < graph_array_line[i]:
                    tmp_edge = Edge(self.vertexes[graph_array_line[0]], self.vertexes[graph_array_line[i]])
                    self.edges_base.add(tmp_edge)
                    self.vertexes[graph_array_line[0]].add_edge(tmp_edge)
                    self.vertexes[graph_array_line[i]].add_edge(tmp_edge)
        # Init edges
        self.init_edges()
        # Init partition indexes
        self.partition_indexes = set()
        self.init_partitions()

    def init_edges(self):
        """
        Reinitializes the edges using the initial stored edge state.
        """
        self.edges.clear()
        for key, vertex in self.vertexes.items():
            vertex.clear_edges()
        for edge in self.edges_base:
            self.edges.add(edge)
            edge.vertex1.add_edge(edge)
            edge.vertex2.add_edge(edge)

    def init_partitions(self):
        """
        Reinitializes the partition indexes of each vertex and of the graph.
        """
        self.partition_indexes.clear()
        for key, vertex in self.vertexes.items():
            vertex.current_partition = key
            self.partition_indexes.add(key)

    def is_partitionned_in_2(self):
        """
        Checks whether the graph is partitionned in 2 or not (random min cut algorithm finishing condition).
        """
        if len(self.partition_indexes) == 2:
            return True
        return False

    def merge_edge(self, edge):
        """
        Merges the partitions at each end of the provided edge, i.e. the partitions will bear the same partition index.
        The edge will then be deleted. Further more, the edges looping over the new partition will be deleted.
        :param edge:
        """
        # print(self.partition_indexes)
        if edge.vertex1.current_partition != edge.vertex2.current_partition:
            # Keep the first vertex's partition number
            index_to_delete = edge.vertex2.current_partition
            self.partition_indexes.remove(index_to_delete)
            for key, vertex in self.vertexes.items():
                if vertex.current_partition == index_to_delete:
                    vertex.current_partition = edge.vertex1.current_partition
            edge.vertex1.remove_edge(edge)
            edge.vertex2.remove_edge(edge)
        self.edges.remove(edge)
        self.check_loops()

    def check_loops(self):
        """
        Removes loops among the current partitions states in the graph.
        """
        tmp_edges = set(self.edges)
        for edge in tmp_edges:
            if edge.vertex1.current_partition == edge.vertex2.current_partition:
                edge.vertex1.remove_edge(self)
                edge.vertex2.remove_edge(self)
                self.edges.remove(edge)

    def reset(self):
        """
        Resets the graph to its initial state.
        """
        self.init_edges()
        self.init_partitions()

    def print_graph(self):
        """
        Prints each vertex and all its incident edges.
        """
        for key, vertex in self.vertexes.items():
            print(key, [(e.vertex1.index, e.vertex2.index) for e in vertex.incident_edges])


def random_min_cut(g):
    """
    Perform a random minimum cut iteration. Leaves the graph untouched in the end (resets to its initial state).
    :param g:
    :return: the current iteration's minimum cut count.
    """
    while not g.is_partitionned_in_2():
        edge = random.sample(g.edges, 1)[0]
        g.merge_edge(edge)
    result = len(g.edges)
    g.reset()
    return result
