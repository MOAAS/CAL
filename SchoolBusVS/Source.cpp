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

	PoIList poiList(garage);
	for (Child child : kids)
		poiList.addHome(child);

	
	graph.dijkstraShortestPath(0);

	vector<Vertex*> path = graph.getPath(graph.findVertex(2));

	for (Vertex* vertex : path)
		cout << vertex->getID() << " ";
}