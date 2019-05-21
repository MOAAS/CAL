#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <climits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include "PathMatrix.h"

#define INF (std::numeric_limits<double>::max)()

using namespace std;

class Edge;
class Graph;
class Vertex;
class PathMatrix;

/********************** Edge  ****************************/


class Edge {
	Vertex * dest;      // destination vertex
	double weight;      // edge weight
	int ID;
public:
	Edge(int ID, Vertex *d, double w);
	Vertex* getDest();
	int getID();
	double getWeight();
	friend class Graph;
	friend class Vertex;
};


/************************* Vertex  **************************/


class Vertex {
	int ID;
	double x, y;
	vector<Edge*> adj;  // outgoing edges

	// auxiliary...
	bool visited;         
	double dist = 0;
	Vertex *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue
	bool processing = false;
	void addEdge(int ID, Vertex *dest, double w);
public:
	Vertex(int ID, double x, double y);
	bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue
	int getID() const;
	double getX() const;
	double getY() const;
	double getDist() const;
	vector<Edge*> getAdj() const;
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
	Vertex* findVertex(int id) const;
	Edge* findEdge(int id) const;
	bool addVertex(int ID, double x, double y);
	bool addEdge(int edgeID, int srcID, int destID, double w);
	int getNumVertex() const;
	vector<Vertex *> getVertexSet() const;

	void BFS(Vertex* s);
	void BFS(Vertex* s, Vertex* removed);
	void transpose(Graph* transposed);
	PathMatrix* multipleDijkstra(const vector<int>& POIids);
	void dijkstraShortestPath(int sourceID);
	vector<Vertex*> getPath(Vertex* v) const;


	vector<Vertex*> calculatePrim();
	bool stronglyConnected();
	bool verifyConnectivity(const vector<int>& POIids, Vertex* removed);
	vector<Vertex *> articulationPoints(const vector<int>& POIids);

};


