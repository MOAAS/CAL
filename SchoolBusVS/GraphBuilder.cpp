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

Graph GraphBuilder::build() {
	Graph graph;

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
		graph.addVertex(v.ID, v.x, v.y);
		//cout << fixed << setprecision(5) << "V: " << v.ID << " " << v.x << " " << v.y << endl;
	}

	while (getline(edgeFile, line)) {
		EdgeInfo e(line);
		Vertex* src = graph.findVertex(e.srcID);
		Vertex* dest = graph.findVertex(e.destID);
		double dist = sqrt(pow(src->getX() - dest->getX(), 2) + pow(src->getY() - dest->getY(), 2));
		graph.addEdge(e.srcID, e.destID, dist);
		//cout << fixed << setprecision(5) << "E: " << e.srcID << " " << e.destID << " " << dist << endl;
	}

	return graph;
}

void GraphBuilder::setNodeFile(string path) {
	this->nodeFilePath = path;
}

void GraphBuilder::setEdgeFile(string path) {
	this->edgeFilePath = path;
}
