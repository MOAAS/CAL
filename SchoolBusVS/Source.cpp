#include "Graph.h"
#include "GraphBuilder.h"
#include "Vehicle.h"
#include "Menu.h"

#include <iostream>
#include "PoIList.h"
#include "graphviewer.h"

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
	builder.setNodeFile("../Graphs/nodes.txt");
	builder.setEdgeFile("../Graphs/edges.txt");
	//builder.setNodeFile("../Graphs/nodes.txt");
	//builder.setEdgeFile("../Graphs/edges.txt");


	Graph graph = builder.build();

	GraphViewer *gv = new GraphViewer(600, 600, false);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	for (Vertex* vertex : graph.getVertexSet()) {
		gv->addNode(vertex->getID(), vertex->getX(), vertex->getY());
	}
	int id = 0;
	for (Vertex* vertex : graph.getVertexSet()) {
		for (Edge edge : vertex->getAdj()) {
			gv->addEdge(id++, vertex->getID(), edge.getDest()->getID(), EdgeType::UNDIRECTED);
		}
	}
	gv->rearrange();

	vector<Vehicle*> vehicles = { new Vehicle(20), new Vehicle(30), new Vehicle(25) };
	vector<Child*> kids;
	//kids.push_back(new Child(graph.findVertex(1), graph.findVertex(4)));
	//kids.push_back(new Child(graph.findVertex(2), graph.findVertex(4)));
	//kids.push_back(new Child(graph.findVertex(3), graph.findVertex(4)));
	//kids.push_back(new Child(graph.findVertex(5), graph.findVertex(4)));
	//Vertex* garage = graph.findVertex(0);
	
	PoIList poiList(graph.getVertexSet()[0]);
//	for (Child* child : kids)
	//	poiList.addHome(child);
		
	//graph.dijkstraShortestPath(0);	
	//displayPath(graph.getPath(graph.findVertex(2)));

	cout << "------------" << endl;

	//vector<int> testIDs = {90379359, 1108123577 };

	cout << "Available PoIs: ";
	printVector::ofValues(cout, poiList.getIDs(), " ") << endl;

	PathMatrix matrix = graph.multipleDijkstra(poiList.getIDs());

	while (true) {
		int srcID, destID;
		Menu::getInput<int>("srcID: ", srcID);
		Menu::getInput<int>("destID: ", destID);


		displayPath(matrix.getPath(srcID, destID));

		cout << "Distance: " << matrix.getDist(srcID, destID) << endl;

	}
}