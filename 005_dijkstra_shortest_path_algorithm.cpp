/*
* Created by 14chanwa on 2017.03.06
*/

/*
* Dijkstra's Shortest Path Algorithm
* This file implements the UNDIRECTED naive version of Dijkstra's Shortest Path Algorithm, that computes 
* the shortest path between two vertexes in an UNDIRECTED graph with complexity O(n*m).
*/

/*
* Remark: works on Visual Studio 2015. May require work for use with other compilers.
*/

// The input file "dijkstraData.txt" is formatted as a set of lines 
// Each line begins with the current vertex id, followed by a series of tuples edge_target, weight
// (current vertex is linked to edge_target with an edge of weight weight) 
// The total number of vertices in the graph has to be known beforehand

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

/*
* ------------------ *
* CLASS DECLARATIONS *
* ------------------ *
*/

class Edge;

class Vertex {
public:
	int m_index;
	int m_Dijkstra_weight;
	std::vector<Edge*> m_in_edges;

	inline Vertex(int _index) : m_index(_index), m_Dijkstra_weight(std::numeric_limits<int>::max()) {}
	inline ~Vertex() {}
};

class Edge {
public:
	Vertex* m_vertex1;
	Vertex* m_vertex2;
	int m_weight;

	inline Edge(Vertex* _h, Vertex* _t, int _w) : m_vertex1(_h), m_vertex2(_t), m_weight(_w) {}
	inline ~Edge() {}

	inline Vertex* getOtherEnd(Vertex* end) {	// Given one end of the Edge, returns the other end. Does NOT check if the provided vertex is one end.
		if (end == m_vertex1) {
			return m_vertex2;
		}
		return m_vertex1;
	}
};

class Graph {
public:
	std::vector<Vertex*> m_vertexes;
	std::vector<Edge*> m_edges;

	inline Graph() {}
	~Graph();
};

struct Counter {
public:
	int value;
	inline Counter() { value = 0; }
};

class DSP_handler_naive {
public:
	Graph* m_graph;
	Vertex* m_startVertex;
	std::map<Vertex*, Edge*> m_vertex_route;
	std::set<Vertex*> m_X;
	
	inline DSP_handler_naive(Graph* _g, Vertex* _s) : m_graph(_g), m_startVertex(_s) {}		// The naive implementation of Dijkstra's algorithm, complexity O(n*m).
	void run();

	std::vector<Edge*>* getReversedPath(Vertex* target);	// Gets the path from the end to the start point

private:

	
	Edge* _getMinimumEdgeWeight();	// Gets the edge with one end in X and not the other, with the minimum Dijkstra weight, nullptr if none found.
	Vertex* _getVertexInX(Edge*);	// Gets the first found end of the vertex which is in X, nullptr if none in X.

};

/*
* ----------------------- *
* METHODS IMPLEMENTATIONS *
* ----------------------- *
*/

Graph::~Graph() {
	for (std::vector<Vertex*>::iterator it = m_vertexes.begin(); it < m_vertexes.end(); it++) {
		delete *it;
	}
	for (std::vector<Edge*>::iterator it = m_edges.begin(); it < m_edges.end(); it++) {
		delete *it;
	}
}

void DSP_handler_naive::run() {
	m_X.insert(m_startVertex);
	m_startVertex->m_Dijkstra_weight = 0;

	while (m_X.size() < m_graph->m_vertexes.size()) {
		Edge* _current_min = _getMinimumEdgeWeight();
		
		// If no edge found
		if (_current_min == nullptr) {
			break;
		}

		// Get target
		Vertex* source = _getVertexInX(_current_min);
		Vertex* target = _current_min->getOtherEnd(source);

		// Update X and shortest path
		m_X.insert(target);
		target->m_Dijkstra_weight = source->m_Dijkstra_weight + _current_min->m_weight;
		m_vertex_route[target] = _current_min;
	}
}

Edge* DSP_handler_naive::_getMinimumEdgeWeight() {
	Edge* _current_min = nullptr;
	int _current_min_weight = std::numeric_limits<int>::max();

	for (Edge* e : m_graph->m_edges) {

		Vertex* _extremityInX = _getVertexInX(e);

		if (_extremityInX != nullptr && m_X.find(e->getOtherEnd(_extremityInX)) == m_X.end()) {
			int _vertexWeight = _extremityInX->m_Dijkstra_weight;
			if (e->m_weight + _vertexWeight < _current_min_weight) {
				_current_min = e;
				_current_min_weight = e->m_weight + _vertexWeight;
			}
		}
	}
	return _current_min;
}

Vertex* DSP_handler_naive::_getVertexInX(Edge* _e) {
	if (m_X.find(_e->m_vertex1) != m_X.end()) {
		return _e->m_vertex1;
	}
	else if (m_X.find(_e->m_vertex2) != m_X.end()) {
		return _e->m_vertex2;
	}
	return nullptr;
}

std::vector<Edge*>* DSP_handler_naive::getReversedPath(Vertex* _t) {
	std::vector<Edge*>* _path = new std::vector<Edge*>();
	Vertex* _current_target = _t;
	while (m_vertex_route.find(_current_target) != m_vertex_route.end()) {
		Vertex* _current_source = m_vertex_route[_current_target]->getOtherEnd(_current_target);
		_path->push_back(m_vertex_route[_current_target]);
		_current_target = _current_source;
	}
	return _path;
}

/*
* ---- *
* MAIN *
* ---- *
*/

int main() {
	// The input file "dijkstraData.txt" is formatted as a set of lines 
	// Each line begins with the current vertex id, followed by a series of tuples edge_target, weight
	// (current vertex is linked to edge_target with an edge of weight weight) 
	// The total number of vertices in the graph has to be known beforehand
	std::ifstream myfile("dijkstraData.txt");
	int vertexCount = 200; // for instance
	std::string* line = new std::string();

	if (myfile.is_open()) {

		// Creating the graph
		Graph* g = new Graph();
		for (int i = 0; i < vertexCount; i++) {
			g->m_vertexes.push_back(new Vertex(i + 1));
		}

		std::cout << "Graph created. Size: " << g->m_vertexes.size() << std::endl;

		std::string delimiter = "\t";

		while (std::getline(myfile, *line)) {
			
			size_t pos = 0;
			std::string token;

			// Read vertex id
			pos = line->find(delimiter);
			token = line->substr(0, pos);
			line->erase(0, pos + delimiter.length());

			int _current_vertex_id = std::stoi(token);
			Vertex* _current_vertex = g->m_vertexes[_current_vertex_id - 1];

			// Read tuples (vertex, weight)
			while ((pos = line->find(delimiter)) != std::string::npos) {
				token = line->substr(0, pos);

				// Append edge
				int _target_vertice_id = std::stoi(token.substr(0, token.find_first_of(",")));
				if (_target_vertice_id > _current_vertex_id) {
					Vertex* _target_vertex = g->m_vertexes[_target_vertice_id - 1];
					int _weight = std::stoi(token.substr(token.find_first_of(",") + 1));
					Edge* _edge = new Edge(_current_vertex, _target_vertex, _weight);

					_current_vertex->m_in_edges.push_back(_edge);
					_target_vertex->m_in_edges.push_back(_edge);
					g->m_edges.push_back(_edge);
				}
				line->erase(0, pos + delimiter.length());
			}

		}
		myfile.close();
		delete line;

		// Run a DSP starting from index n°1 (for instance)
		std::cout << "Starts on vertex " << g->m_vertexes[0]->m_index << std::endl;
		DSP_handler_naive* _DSP = new DSP_handler_naive(g, g->m_vertexes[0]);
		_DSP->run();
		
		// Gets the minimal paths going from 1 to the following indexes
		std::vector<int> _to_check;
		_to_check.push_back(7);
		_to_check.push_back(37);
		_to_check.push_back(59);
		_to_check.push_back(82);
		_to_check.push_back(99);
		_to_check.push_back(115);
		_to_check.push_back(133);
		_to_check.push_back(165);
		_to_check.push_back(188);
		_to_check.push_back(197);

		std::vector<Edge*>* _path;

		// Prints the weights found
		for (int i : _to_check) {
			std::cout << i << ": ";
			Vertex* _target = g->m_vertexes[i - 1];
			_path = _DSP->getReversedPath(_target);

			// If path empty then do nothing
			if (_path->empty()) {
				std::cout << "no path exists" << std::endl;
				continue;
			}

			int sum = 0;
			for (Edge* e : *_path) {
				sum += e->m_weight;
			}
			std::cout << sum << std::endl;
			delete _path;
		}
	}
	else {
		std::cout << "Unable to read file!" << std::endl;
	}

	system("pause");

	return 0;
}
