#include "GraphBuilder.h"

struct VertexInfo {
	int ID;
	double x, y;
	VertexInfo(string line) {

		for (size_t i = 0; i < line.size(); i++) {
			if (line[i] == '(' || line[i] == ',' || line[i] == ')')
				line[i] = ' ';
		}
		
		istringstream iss(line);
		iss >> ID >> x >> y;
	}
};

struct EdgeInfo {
	int srcID, destID;
	double weight;
	EdgeInfo(string line) {
		for (size_t i = 0; i < line.size(); i++) {
			if (line[i] == '(' || line[i] == ',' || line[i] == ')')
				line[i] = ' ';
		}

		istringstream iss(line);
		iss >> srcID >> destID;
	}
};

Graph* GraphBuilder::build() {
	Graph* graph = new Graph;

	ifstream nodeFile(this->nodeFilePath);
	ifstream edgeFile(this->edgeFilePath);

	if (nodeFile.fail()) {
		cout << "Couldn't open node file: " << this->nodeFilePath << endl;
		return graph;
	}

	if (edgeFile.fail()) {
		cout << "Couldn't open edge file: " << this->edgeFilePath << endl;
		return graph;
	}

	string line;

	while (getline(nodeFile, line)) {
		VertexInfo v(line);
		graph->addVertex(v.ID, v.x, v.y);
	}

	int id = 0;
	while (getline(edgeFile, line)) {
		EdgeInfo e(line);
		Vertex* src = graph->findVertex(e.srcID);
		Vertex* dest = graph->findVertex(e.destID);
		if (src == NULL) {
			cout << "Error: Couldn't find vertex with srcID = " << e.srcID << endl;
			throw exception();
		}
		if (dest == NULL) {
			cout << "Error: Couldn't find vertex with destID = " << e.destID << endl;
			throw exception();
		}

		double dist = sqrt(pow(src->getX() - dest->getX(), 2) + pow(src->getY() - dest->getY(), 2));
		graph->addEdge(id++, e.srcID, e.destID, dist);
		graph->addEdge(id++, e.destID, e.srcID, dist); // undirected ( test ) :D
	}

	return graph;
}

void GraphBuilder::setNodeFile(string path) {
	this->nodeFilePath = path;
}

void GraphBuilder::setEdgeFile(string path) {
	this->edgeFilePath = path;
}
