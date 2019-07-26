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

int PathMatrix::getNumMissingPaths(const vector<int>& ids, bool enableLog)
{
	int missingPaths = 0;
	for (size_t i = 0; i < ids.size(); i++) {
		for (size_t j = 0; j < ids.size(); j++) {
			if (this->getDist(ids[i], ids[j]) == INF) {
				missingPaths++;
				if (enableLog)
					cout << "There is no path from " << ids[i] << " to " << ids[j] << endl;
			}
		}
	}

	return missingPaths;

}


