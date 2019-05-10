#include "Graph.h"

// -- Edge -- //

Edge::Edge(Vertex *d, double w) : dest(d), weight(w) {

}

// -- Vertex -- //

void Vertex::addEdge(Vertex *d, double w) {
	adj.push_back(Edge(d, w));
}

inline Vertex::Vertex(int ID, double x, double y) {
	this->ID = ID;
	this->x = x;
	this->y = y;
}

bool Vertex::operator<(Vertex & vertex) const {
	return this->dist < vertex.dist;
}

int Vertex::getID() const {
	return this->ID;
}

double Vertex::getX() const {
	return this->x;
}

double Vertex::getY() const {
	return this->y;
}

double Vertex::getDist() const {
	return this->dist;
}

Vertex *Vertex::getPath() const {
	return this->path;
}

// -- Graph -- //

int Graph::getNumVertex() const {
	return vertexSet.size();
}

vector<Vertex *> Graph::getVertexSet() const {
	return vertexSet;
}

Vertex * Graph::findVertex(int ID) const {
	for (auto v : vertexSet) {
		if (v->ID == ID)
			return v;
	}
	return NULL;
}

bool Graph::addVertex(int ID, double x, double y) {
	if (findVertex(ID) != NULL)
		return false;
	vertexSet.push_back(new Vertex(ID, x, y));
	return true;
}

bool Graph::addEdge(int srcID, int destID, double w) {
	auto v1 = findVertex(srcID);
	auto v2 = findVertex(destID);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

void Graph::dijkstraShortestPath(int sourceID) {
	auto src = findVertex(sourceID);
	if (src == NULL) {
		cout << "Warning... Dijkstra shortest path from NULL." << endl;
		return;
	}
	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = NULL;
	}
	src->dist = 0;
	MutablePriorityQueue<Vertex> queue;
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

vector<int> Graph::getPath(int sourceID, int destID) const {
	vector<int> res;
	Vertex* v = findVertex(destID);
	if (v == NULL || v->dist == INF)
		return res;
	while (v != NULL) {
		res.push_back(v->ID);
		v = v->path;
	}
	reverse(res.begin(), res.end());
	return res;
}
