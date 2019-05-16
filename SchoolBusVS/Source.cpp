#include "Graph.h"
#include "GraphBuilder.h"
#include "Vehicle.h"
#include "Menu.h"

#include <iostream>
#include "PoIList.h"

using namespace std;

void displayPath(const vector<Vertex*>& path) {
	cout << "Path: ";
	if (path.size() == 0)
		cout << "Does not exist.";
	for (Vertex* vertex : path) {
		cout << vertex->getID() << " ";
	}
	cout << endl;
}

int main() {
	cout << "HELLO WORLD" << endl;

	GraphBuilder builder;
	builder.setNodeFile("../Graphs/testnodes.txt");
	builder.setEdgeFile("../Graphs/testedges.txt");
	//builder.setNodeFile("../Graphs/nodes.txt");
	//builder.setEdgeFile("../Graphs/edges.txt");

	Graph graph = builder.build();
	vector<Vehicle> vehicles;
	vector<Child> kids;
	Vertex* garage = graph.getVertexSet()[0];
	
	PoIList poiList(garage);
	for (Child child : kids)
		poiList.addHome(child);
		
	//graph.dijkstraShortestPath(0);	
	//displayPath(graph.getPath(graph.findVertex(2)));

	cout << "------------" << endl;

	vector<int> testIDs = { 0, 1, 2, 4 };
	//vector<int> testIDs = {90379359, 1108123577 };

	cout << "Available PoIs: ";
	printVector::ofValues(cout, testIDs, " ") << endl;

	PathMatrix matrix = graph.multipleDijkstra(testIDs);

	while (true) {
		int srcID, destID;
		Menu::getInput<int>("srcID: ", srcID);
		Menu::getInput<int>("destID: ", destID);


		displayPath(matrix.getPath(srcID, destID));

		cout << "Distance: " << matrix.getDist(srcID, destID) << endl;

	}
}