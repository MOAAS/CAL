/*
 * Graph.h
 */
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

#define INF std::numeric_limits<double>::max()

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;



/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<vector<double>> Dist;
	vector<vector<int>> Path;
	vector<Vertex<T> *> vertexSet;    // vertex set

	int findVertexIdx(const T &in) const;
public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	auto src = findVertex(origin);
	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = NULL;
	}
	src->dist = 0;
	MutablePriorityQueue<Vertex<T>> queue;
	queue.insert(src);
	while (!queue.empty()) {
		src = queue.extractMin();
		for (auto edge : src->adj) {
			if (edge.dest->dist > src->dist + edge.weight) {
				double oldDist = edge.dest->dist;
				edge.dest->dist = src->dist + edge.weight;
				edge.dest->path = src;
				if (oldDist == INF)
					queue.insert(edge.dest);
				else queue.decreaseKey(edge.dest);
			}
		}
	}
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;
	Vertex<T> * beginning = findVertex(origin);
	Vertex<T> * current = findVertex(dest);
	while (current != beginning) {
		res.push_back(current->info);
		current = current->path;
	}
	res.push_back(beginning->info);
	reverse(res.begin(), res.end());
	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
	auto src = findVertex(orig);
	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = NULL;
	}
	src->dist = 0;
	queue<Vertex<T>*> q;
	q.push(src);
	while (!q.empty()) {
		src = q.front();
		q.pop();
		for (auto edge : src->adj) {
			if (edge.dest->dist == INF) {
				edge.dest->dist = src->dist + 1;
				edge.dest->path = src;
				q.push(edge.dest);
			}
		}
	}
}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	auto src = findVertex(orig);
	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = NULL;
	}
	src->dist = 0;
	for (size_t i = 1; i < vertexSet.size(); i++) {
		for (auto vertex : vertexSet) {
			for (auto edge : vertex->adj) {
				if (edge.dest->dist > vertex->dist + edge.weight) {
					edge.dest->dist = vertex->dist + edge.weight;
					edge.dest->path = vertex;
				}
			}
		}
	}
	for (auto vertex : vertexSet) {
		for (auto edge : vertex->adj) {
			if (edge.dest->dist > vertex->dist + edge.weight) {
				cout << "THERE ARE CYCLES WITH NEGATIVE WEIGHT OH NO!" << endl;
			}
		}
	}

}


/**************** All Pairs Shortest Path  ***************/

/*   * Finds the index of the vertex with a given content.  */

template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
	for (size_t i = 0; i < vertexSet.size(); i++) {
		if (vertexSet[i]->info == in)
			return i;
	}
	return -1;
}

template<class T>
void Graph<T>::floydWarshallShortestPath() {
	int V = vertexSet.size();
	this->Dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INF));
	this->Path = vector<vector<int>>(vertexSet.size(), vector<int>(vertexSet.size(), -1));

	for (size_t i = 0; i < vertexSet.size(); i++) {
		Dist[i][i] = 0;
		Path[i][i] = i; // comment?

		for (auto edge : vertexSet[i]->adj) {
			int j = findVertexIdx(edge.dest->info);
			Dist[i][j] = edge.weight;
			Path[i][j] = i;
		}
	}
	for (int k = 0; k < V; k++) {
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				if (Dist[i][k] == INF || Dist[k][j] == INF)
					continue;  // avoid overflow
				if (Dist[i][j] > Dist[i][k] + Dist[k][j]) {
					Dist[i][j] = Dist[i][k] + Dist[k][j];
					Path[i][j] = Path[k][j];
				}
			}
		}
	}
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;
	int beginning = findVertexIdx(orig);
	int current = findVertexIdx(dest);
	if (beginning == -1 || current == -1 || Dist[beginning][current] == INF)
		return res;
	while (current != beginning) {
		res.push_back(vertexSet[current]->info);
		current = Path[beginning][current];
	}
	res.push_back(vertexSet[beginning]->info);
	reverse(res.begin(), res.end());
	return res;
}

