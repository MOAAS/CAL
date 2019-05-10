#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <climits>
#include <cmath>
#include "MutablePriorityQueue.h"

#define INF std::numeric_limits<double>::max()

using namespace std;

class Edge;
class Graph;
class Vertex;

/********************** Edge  ****************************/


class Edge {
	Vertex * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex *d, double w);
	friend class Graph;
	friend class Vertex;
};


/************************* Vertex  **************************/


class Vertex {
	int ID;
	double x, y;
	vector<Edge> adj;  // outgoing edges

	// auxiliary...
	bool visited;         
	double dist = 0;
	Vertex *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue
	bool processing = false;
	void addEdge(Vertex *dest, double w);
public:
	Vertex(int ID, double x, double y);
	bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue
	int getID() const;
	double getX() const;
	double getY() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph;
	friend class MutablePriorityQueue<Vertex>;
};


/*************************** Graph  **************************/

class Graph {
	vector<vector<double>> Dist;
	vector<vector<int>> Path;
	vector<Vertex *> vertexSet;    // vertex set
public:
	Vertex *findVertex(int id) const;
	bool addVertex(int ID, double x, double y);
	bool addEdge(int srcID, int destID, double w);
	int getNumVertex() const;
	vector<Vertex *> getVertexSet() const;

	void dijkstraShortestPath(int sourceID);
	vector<int> getPath(int sourceID, int destID) const;
};


