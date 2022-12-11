#!/usr/bin/env ipython

# depth_first_ucc.py
#
# Copyright (c) 2020 Carlos Braga
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the MIT License.
#
# See accompanying LICENSE.md or https://opensource.org/licenses/MIT.


import sys
import networkx
import numpy
from matplotlib import pyplot
from ds.graph import Graph
from ds.queue import Queue
from ds.stack import Stack

# -----------------------------------------------------------------------------
class DepthFirstUCC:
    """
    Compute the undirected connected components of a graph using breadth first
    search algorithm.
    """

    # ---- Special methods ----------------------------------------------------
    def __init__(self, graph):
        self.NEW = 0
        self.VISITED = 1
        self._state = [self.NEW for _ in graph.vertices()]
        self._id = [numpy.inf for _ in graph.vertices()]
        self._count = 0
        for v in graph.vertices():
            if not self.is_visited(v):
                self._dfs(graph, v)
                self._count += 1

    def __del__(self):
        pass

    # ---- API ----------------------------------------------------------------
    def is_visited(self, v):
        """ Has the specified vertex been visited? """
        return self._state[v] == self.VISITED

    def connected(self, v, w):
        """ Are v and w connected? """
        return self._id[v] == self._id[w]

    def id(self, v):
        """ Return the component id for the specified vertex """
        return self._id[v]

    def count(self):
        """ Return the number of connnected components """
        return self._count

    def _dfs(self, graph, s):
        """ Compute the vertices reachable from a source vertex s """
        S = Stack()
        S.push(s)
        while not S.is_empty():
            v = S.pop()
            self._state[v] = self.VISITED
            self._id[v] = self._count
            for w in graph.adj(v):
                if self._state[w] == self.NEW:
                    S.push(w)


# -----------------------------------------------------------------------------
def draw(dfs, graph, cc):
    """ Draw the graph using networkx package """
    # Create the networkx graph
    G = networkx.Graph()
    for v in graph.vertices():
        if dfs.id(v) == cc:
            for w in graph.adj(v):
                G.add_edge(v, w)

    # Draw the networkx graph
    fig = pyplot.figure(figsize=(7,6))
    options = {
        "font_size": 10,
        "font_color": "#000000",
        "with_labels": False,
        "node_size": 10,
        "node_shape": "o",
        "node_color": "#ee0000",
        "edge_color": "#1f78b4",
        "linewidths": 1,
        "width": 1,
        "alpha" : 0.6,
    }
    pos = networkx.spring_layout(G, k=2.0, iterations=1000, seed=63)
    networkx.draw_networkx(G, pos, **options)
    pyplot.axis("off")
    pyplot.show()
    # pyplot.savefig('fig.pdf', bbox_inches='tight')


# -----------------------------------------------------------------------------
def main (argv):
    """ DepthFirstUCC test client """

    # Create a graph from stream
    with open('../data/tinyG.txt') as stream:
        graph = Graph.from_stream(stream)
    print(graph)

    # Compute the list of vertices reachable from 0
    dfs = DepthFirstUCC(graph)
    print("\n connected components ", dfs.count(), file=sys.stdout, end=' ')
    for cc in range(dfs.count()):
        draw(dfs, graph, cc)

    # Done
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
