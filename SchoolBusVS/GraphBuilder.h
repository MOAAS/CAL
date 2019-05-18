#pragma once

#include "Graph.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class GraphBuilder {
	string nodeFilePath;
	string edgeFilePath;
public:

	GraphBuilder(string nodeFilePath, string edgeFilePath) : nodeFilePath(nodeFilePath), edgeFilePath(edgeFilePath) {}

	Graph* build();
};

