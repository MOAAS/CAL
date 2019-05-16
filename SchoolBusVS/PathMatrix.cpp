#include "PathMatrix.h"

double PathMatrix::getDist(int srcID, int destID) {
	return distances[srcID][destID];
}

vector<Vertex*> PathMatrix::getPath(int srcID, int destID) {
	return paths[srcID][destID];
}

void PathMatrix::setPath(int srcID, int destID, double dist, const vector<Vertex*>& path) {
	paths[srcID][destID] = path;
	distances[srcID][destID] = dist;
}


