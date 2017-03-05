/*
* Created by 14chanwa on 2017.03.04
*/

/*
* Strongly connected components in a directed graph
* This file implements Kosaraju's Two-Pass Algorithm, that computes the SCCs of a directed graph using two Depth-First Search loops.
*/

/*
* Remark: works on Visual Studio 2015. May require work for use with other compilers.
* Remark: for very big graphs, the stack size should be increased (>1MB). On Visual Studio 2015, change the "Stack Reserve Size"
* parameter in Property Pages -> Configuration Properties -> Linker -> System.
*/

// The input file "SCC.txt" is formatted as a set of lines 
// Each line depicts an edge ; it is composed of 2 numbers, the tail index and the head index.
// For instance: "2 510" is an edge going from 2 to 510.
// The total number of vertices in the graph has to be known!

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

/*
* ------------------ *
* CLASS DECLARATIONS *
* ------------------ *
*/

class Edge;

class Vertex {
public:
	int _index;
	std::vector<Edge*> _in_edges;
	std::vector<Edge*> _out_edges;
	bool _explored;

	inline Vertex(int _index) : _index(_index), _explored(false) {}
	inline ~Vertex() {}
};

class Edge {
public:
	Vertex* _head;
	Vertex* _tail;

	inline Edge(Vertex* _h, Vertex* _t) : _head(_h), _tail(_t) {}
	inline ~Edge() {}
};

class Graph {
public:
	std::vector<Vertex*> _vertexes;
	std::vector<Edge*> _edges;

	inline Graph() {}
	~Graph();

	void ResetVertexes();
};

struct Counter {
public:
	int value;
	inline Counter() { value = 0; }
};

class DFS_handler {
public:
	inline DFS_handler(Graph* _g) : _graph(_g) {}

	void DFS_Loop_Reversed();
	void DFS_Loop_SCC();
	inline std::vector<int>* getSCC() {
		std::sort(_SCC_sizes.begin(), _SCC_sizes.end());
		return &_SCC_sizes;
	}

private:
	Graph* _graph;
	std::vector<Vertex*> _stack_finish;
	std::vector<int> _SCC_sizes;

	void DFS_Reversed(Vertex*);
	void DFS_SCC(Vertex*, Counter*);
	
};

/*
* ----------------------- *
* METHODS IMPLEMENTATIONS *
* ----------------------- *
*/

Graph::~Graph() {
	for (std::vector<Vertex*>::iterator it = _vertexes.begin(); it < _vertexes.end(); it++) {
		delete *it;
	}
	for (std::vector<Edge*>::iterator it = _edges.begin(); it < _edges.end(); it++) {
		delete *it;
	}
}

void Graph::ResetVertexes() {
	for (std::vector<Vertex*>::iterator it = _vertexes.begin(); it < _vertexes.end(); it++) {
		(*it)->_explored = false;
	}
}

void DFS_handler::DFS_Loop_Reversed() {
	_graph->ResetVertexes();
	_stack_finish.clear();
	for (Vertex* _vertex : _graph->_vertexes) {
		if (!_vertex->_explored) {
			DFS_Reversed(_vertex);
		}
	}
}

void DFS_handler::DFS_Reversed(Vertex* _vertex) {
	if (!_vertex->_explored) {
		_vertex->_explored = true;
		for (Edge* _edge : _vertex->_in_edges) {
			DFS_Reversed(_edge->_tail);
		}
		_stack_finish.push_back(_vertex);
	}
}

void DFS_handler::DFS_Loop_SCC() {
	_graph->ResetVertexes();
	_SCC_sizes.clear();
	Counter* _counter = new Counter();
	for (std::vector<Vertex*>::reverse_iterator rit = _stack_finish.rbegin(); rit < _stack_finish.rend(); rit++) {
		if (!(*rit)->_explored) {
			_counter->value = 0;
			DFS_SCC(*rit, _counter);
			_SCC_sizes.push_back(_counter->value);
		}
	}
	delete _counter;
}

void DFS_handler::DFS_SCC(Vertex* _vertex, Counter* _counter) {
	if (!_vertex->_explored) {
		_vertex->_explored = true;
		_counter->value += 1;
		for (Edge* _edge : _vertex->_out_edges) {
			DFS_SCC(_edge->_head, _counter);
		}
	}
}

/*
* ---- *
* MAIN *
* ---- *
*/

int main() {
  // The input file "SCC.txt" is formatted as a set of lines 
  // Each line depicts an edge ; it is composed of 2 numbers, the tail index and the head index.
  // For instance: "2 510" is an edge going from 2 to 510.
  // The total number of vertices in the graph has to be known!
	std::ifstream myfile("SCC.txt");
  int vertexCount = 875714; // for instance
	std::string* line = new std::string();

	if (myfile.is_open()) {

    // Creating the graph
		Graph* g = new Graph();
		for (int i = 0; i < vertexCount ; i++) {
			g->_vertexes.push_back(new Vertex(i + 1));
		}

		std::cout << "Graph created. Size: " << g->_vertexes.size() << std::endl;

		while (std::getline(myfile, *line)) {
			//std::cout << line << std::endl;
			std::istringstream iss(*line);
			int tail;
			int head;
			iss >> tail;
			iss >> head;
			Edge* edge = new Edge(g->_vertexes.at(head - 1), g->_vertexes.at(tail - 1));
			g->_vertexes.at(head - 1)->_in_edges.push_back(edge);
			g->_vertexes.at(tail - 1)->_out_edges.push_back(edge);
		}
		myfile.close();
		delete line;

		DFS_handler* dfsHandler = new DFS_handler(g);
		dfsHandler->DFS_Loop_Reversed();
		dfsHandler->DFS_Loop_SCC();

		std::vector<int>::reverse_iterator rit = dfsHandler->getSCC()->rbegin();
		for (int i = 0; i < 10; i++) {
			std::cout << *rit << std::endl;
			rit++;
		}

	} else {
		std::cout << "Unable to read file!" << std::endl;
	}

	system("pause");
	
    return 0;
}
