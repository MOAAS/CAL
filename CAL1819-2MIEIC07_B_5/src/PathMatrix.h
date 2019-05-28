#pragma once

#include <unordered_map>

#include "Graph.h"

using namespace std;

class Vertex;
class Graph;

class PathMatrix
{
	unordered_map<int, unordered_map<int, vector<Vertex*>>> paths;
	unordered_map<int, unordered_map<int, double>> distances;
public:
	double getDist(int srcID, int destID);
	vector<Vertex*> getPath(int srcID, int destID);

	void setPath(int srcID, int destID, double dist, const vector<Vertex*>& path);

	int getNumMissingPaths(const vector<int>& ids, bool enableLog);
};

