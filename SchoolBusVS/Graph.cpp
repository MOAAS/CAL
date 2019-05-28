#include "Graph.h"

// -- Edge -- //

Edge::Edge(int ID, Vertex *d, double w) : ID(ID), dest(d), weight(w) {

}

Vertex * Edge::getDest() {
	return dest;
}

int Edge::getID() {
	return ID;
}

double Edge::getWeight() {
	return weight;
}

// -- Vertex -- //

void Vertex::addEdge(int ID, Vertex *d, double w) {
	for (Edge* e : adj) {		
		if (e->dest == d)
			return;
	}
	adj.push_back(new Edge(ID, d, w));
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

vector<Edge*> Vertex::getAdj() const {
	return adj;
}

Vertex *Vertex::getPath() const {
	return this->path;
}

// -- Graph -- //

size_t Graph::getNumVertex() const {
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

Edge* Graph::findEdge(int ID) const {
	for (Vertex* v : vertexSet) {
		for (Edge* e : v->getAdj()) {
			if (e->getID() == ID)
				return e;
		}
	}
	return NULL;
}

bool Graph::addVertex(int ID, double x, double y) {
	if (findVertex(ID) != NULL)
		return false;
	vertexSet.push_back(new Vertex(ID, x, y));
	return true;
}

bool Graph::addEdge(int edgeID, int srcID, int destID, double w) {
	Vertex* v1 = findVertex(srcID);
	Vertex* v2 = findVertex(destID);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(edgeID, v2, w);
	return true;
}


/*** Breadth First Search***/

void Graph::BFS(Vertex* s)
{
	// Mark all the vertices as not visited 
	for (auto v : vertexSet) {
		v->visited = false; // known(v) in slides	
	}

	// Create a queue for BFS 
	queue<Vertex*> q;

	// Mark the current node as visited and enqueue it 
	s->visited = true;
	q.push(s);

	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	list<int>::iterator i;

	while (!q.empty())
	{
		// Dequeue a vertex from queue
		s = q.front();
		q.pop();

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for(auto edge : s->adj)
		{
			
			if (!(edge->dest->visited))
			{
				edge->dest->visited = true;
				q.push(edge->dest);
			}
		}
	}
}

/*** Breadth First Search (ignores one vertex)***/

void Graph::BFS(Vertex* s, Vertex* removed)
{
	// Mark all the vertices as not visited 
	for (auto v : vertexSet) {
		v->visited = false; // known(v) in slides	
	}

	// Create a queue for BFS 
	queue<Vertex*> q;

	// Mark the current node as visited and enqueue it 
	s->visited = true;
	q.push(s);

	while (!q.empty())
	{
		// Dequeue a vertex from queue
		s = q.front();
		q.pop();

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for (auto edge : s->adj)
		{

			if (!(edge->dest->visited) && edge->dest != removed) //!
			{
				edge->dest->visited = true;
				q.push(edge->dest);
			}
		}
	}
}

/*** Transpose Graph***/
void Graph::transpose(Graph* transposed) {
	for (Vertex* vertex : vertexSet)
		for (auto edge : vertex->adj){
			transposed->addVertex(vertex->ID, vertex->x, vertex->y);
			transposed->addEdge(edge->ID, edge->dest->ID, vertex->ID, edge->weight);
		}
}

/*** Shortest Path between POIs ***/

PathMatrix* Graph::multipleDijkstra(const vector<int>& POIids) {
	PathMatrix* matrix = new PathMatrix();
	for (int srcID : POIids) {
		dijkstraShortestPath(srcID);
		for (int destID : POIids) {
			Vertex* dest = findVertex(destID);
			matrix->setPath(srcID, destID, dest->dist, this->getPath(dest));
		}
	}
	return matrix;
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
			if (edge->dest->dist > src->dist + edge->weight) {
				double oldDist = edge->dest->dist;
				edge->dest->dist = src->dist + edge->weight;
				edge->dest->path = src;
				if (oldDist == INF)
					queue.insert(edge->dest);
				else queue.decreaseKey(edge->dest);
			}
		}
	}
}

vector<Vertex *> Graph::getPath(Vertex* dest) const {
	vector<Vertex *> res;
	Vertex* v = dest;
	if (v == NULL || v->dist == INF)
		return res;
	while (v != NULL) {
		res.push_back(v);
		v = v->path;
	}
	reverse(res.begin(), res.end());
	return res;
}

/***** P R I M ****/

vector<Vertex*> Graph::calculatePrim() {
	if (vertexSet.size() == 0)
		return this->vertexSet;
	// Reset auxiliary info
	for(auto v : vertexSet) {
		v->dist = INF;v->path = nullptr;
		v->visited = false; // known(v) in slides	
	}
	
	// start with an arbitrary vertex
	Vertex* s = vertexSet.front();
	s->dist = 0;
	
	// initializepriority queue
	MutablePriorityQueue<Vertex> q;
	q.insert(s);

	vector<Vertex*> order;

	// process vertices in the priority queue
	while (!q.empty()) {
		Vertex* v = q.extractMin();
		v->visited = true;
		order.push_back(v);
		for (Edge* e : v->adj) {
			Vertex* w = e->dest;
			if (!w->visited) {
				auto oldDist = w->dist;
				if (e->weight < w->dist) {
					w->dist = e->weight;
					w->path = v;
					if (oldDist == INF)
						q.insert(w);
					else q.decreaseKey(w);
				}
			}
		}
	}
	return order;
}


/***** Strongly Connected ****/

bool Graph::stronglyConnected() {
	BFS(vertexSet.front());
	for (auto v : vertexSet) {
		if (!v->visited)
			return false;
	}

	Graph transposed;

	transpose(&transposed);
	transposed.BFS(transposed.vertexSet.front());
	for (auto v : transposed.vertexSet) {
		if (!v->visited)
			return false;
	}

	return true;
}


/***** Connected between points of interest (ignoring a vertex) ****/

bool Graph::verifyConnectivity(const vector<Vertex*>& POIs, Vertex* removed) {
	for (Vertex* v : POIs)
		if (v->getID() == removed->getID())
			return true;
	BFS(POIs[0], removed);
	for (Vertex* v : POIs) {
		if (v->visited == false)
			return false;
	}
	return true;
}

/***** Articulation Points (between PoI) ****/
vector<Vertex *> Graph::articulationPoints(const vector<Vertex*>& POIs) {
	vector<Vertex *> articulationPoints;
	if (POIs.size() == 0)
		return articulationPoints;
	for (auto v : vertexSet){
		if (!verifyConnectivity(POIs, v))
			articulationPoints.push_back(v);
	}
	return articulationPoints;
}
