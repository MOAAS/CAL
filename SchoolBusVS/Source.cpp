#include "Graph.h"
#include "GraphBuilder.h"
#include "Vehicle.h"

#include <iostream>
#include "PoIList.h"

using namespace std;

int main() {
	cout << "HELLO WORLD" << endl;

	GraphBuilder builder;
	builder.setNodeFile("testnodes.txt");
	builder.setEdgeFile("testedges.txt");
	
	Graph graph = builder.build();
	vector<Vehicle> vehicles;
	vector<Child> kids;
	Vertex* garage = graph.getVertexSet()[0];

	PoIList poiList(garage, kids);

	
	graph.dijkstraShortestPath(0);

	vector<int> path = graph.getPath(0, 2);

	for (int i : path)
		cout << i << " ";
}