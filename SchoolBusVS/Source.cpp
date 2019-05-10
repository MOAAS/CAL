#include "Graph.h"
#include "GraphBuilder.h"

#include <iostream>

using namespace std;

int main() {
	cout << "HELLO WORLD" << endl;

	GraphBuilder builder;
	builder.setNodeFile("testnodes.txt");
	builder.setEdgeFile("testedges.txt");
	
	Graph graph = builder.build();
	
	graph.dijkstraShortestPath(0);

	vector<int> path = graph.getPath(0, 2);

	for (int i : path)
		cout << i << " ";
}