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

// ------------- GRAPH VIEWER n sei onde pore ------------- //

GraphViewer* createGraphViewer(int width, int height, const Graph* graph) {
	GraphViewer* gv = new GraphViewer(width, height, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	for (Vertex* vertex : graph->getVertexSet()) {
		gv->addNode(vertex->getID(), vertex->getX() - 527500, vertex->getY() - 4555555);
	}
	for (Vertex* vertex : graph->getVertexSet()) {
		for (Edge* edge : vertex->getAdj()) {
			gv->addEdge(edge->getID(), vertex->getID(), edge->getDest()->getID(), EdgeType::DIRECTED);
		}
	}
	gv->rearrange();
	return gv;
}

void destroyGraphViewer(GraphViewer* gv) {
	gv->closeWindow();
	free(gv);
}

void highlightPath(GraphViewer* gv, const vector<Vertex*>& path) {
	for (int i = 0; i < (int)path.size() - 1; i++) {
		gv->setVertexColor(path[i]->getID(), YELLOW);
		for (Edge* e : path[i]->getAdj()) {
			if (e->getDest() == path[i + 1]) {
				gv->setEdgeColor(e->getID(), RED);
				break;
			}
		}
	}
	if (path.size() == 0)
		return;
	gv->setVertexColor(path[0]->getID(), GREEN);
	if (path.size() == 1)
		return;
	gv->setVertexColor(path[path.size() - 1]->getID(), RED);
}

void highlightPoIs(GraphViewer* gv, const PoIList& pois) {
	for (POI poi : pois.getPoIs()) {
		switch (poi.getType()) {
			case POI::Garage: gv->setVertexColor(poi.getID(), LIGHT_GRAY); break;
			case POI::School: gv->setVertexColor(poi.getID(), WHITE); break;
			case POI::Kid: gv->setVertexColor(poi.getID(), ORANGE); break;
		}
	}
}

void resetGraphColors(GraphViewer* gv, const vector<Vertex*>& vertexSet, const PoIList& pois) {
	for (Vertex* v : vertexSet) {
		gv->setVertexColor(v->getID(), BLUE);
		for (Edge* e : v->getAdj())
			gv->setEdgeColor(e->getID(), BLACK);
	}
	highlightPoIs(gv, pois);
}


void shortestPathOption(GraphViewer* gv, Graph* graph, const PoIList& poiList, PathMatrix* matrix) {
	while (true) {
		Menu::printHeader("Shortest Path between two PoIs");
		cout << "Available PoIs: ";
		printVector::ofValues(cout, poiList.getIDs(), " ") << endl;

		int srcID, destID;
		Menu::getInput<int>("Source ID: ", srcID);
		Menu::getInput<int>("Destination ID: ", destID);

		displayPath(matrix->getPath(srcID, destID));
		highlightPath(gv, matrix->getPath(srcID, destID));
		cout << "Distance: " << matrix->getDist(srcID, destID) << endl;

		string input;
		Menu::getLineInput_CI("Do you wish to return? (Y / N) ", input, { "Y","N" });
		if (input != "Y")
			break;
		else cout << "Operation cancelled" << endl;
	}
	resetGraphColors(gv, graph->getVertexSet(), poiList);
}

void addVehicle(vector<Vehicle*>& vehicles) {
	int capacity;
	string input;
	Menu::printHeader("Add vehicle");
	Menu::getInput<int>("Vehicle capacity: ", capacity);
	Menu::getLineInput_CI("Do you really wish to add a capacity " + to_string(capacity) + " vehicle? (Y / N) ", input, { "Y","N" });
	if (input == "Y")
		vehicles.push_back(new Vehicle(capacity));
	else cout << "Operation cancelled" << endl;
}

void addKid(GraphViewer* gv, Graph* graph, PoIList& poiList, PathMatrix* matrix) {
	int homeID, schoolID;
	string input;
	Menu::printHeader("Add child record");
	Menu::getInput<int>("Home ID: ", homeID);
	Menu::getInput<int>("School ID: ", schoolID);
	Menu::getLineInput_CI("Home ID: " + to_string(homeID) + ". School ID: " + to_string(schoolID) + ". Proceed? (Y / N) ", input, { "Y","N" });
	Vertex* home = graph->findVertex(homeID);
	Vertex* school = graph->findVertex(schoolID);
	if (home == NULL)
		cout << "Couldn't find home ID." << endl;
	else if (school == NULL)
		cout << "Couldn't find school ID." << endl;
	else if (input == "Y") {
		poiList.addHome(new Child(home, school));
		highlightPoIs(gv, poiList);
	}
	else cout << "Operation cancelled" << endl;
}

void setGarage(GraphViewer* gv, Graph* graph, PoIList& poiList, PathMatrix* matrix) {
	Menu::printHeader("Set Garage (Depot) vertex");
	int ID;
	string input;
	Menu::getInput<int>("Depot ID: ", ID);
	Menu::getLineInput_CI("Depot ID: " + to_string(ID) + ". Proceed? (Y / N) ", input, { "Y","N" });
	Vertex* garage = graph->findVertex(ID);
	if (garage == NULL)
		cout << "Couldn't find garage ID." << endl;
	else if (input == "Y") {
		poiList.setGarage(garage);
		resetGraphColors(gv, graph->getVertexSet(), poiList);
	}
	else cout << "Operation cancelled" << endl;
}

int main() {
	cout << "HELLO WORLD" << endl;

	GraphBuilder builder;
	builder.setNodeFile("../Graphs/nodes.txt");
	builder.setEdgeFile("../Graphs/edges.txt");

	Graph* graph = builder.build();
	GraphViewer *gv = createGraphViewer(2000, 2000, graph);

	vector<Vehicle*> vehicles = { new Vehicle(20), new Vehicle(30), new Vehicle(25) };
	vector<Child*> kids;
	kids.push_back(new Child(graph->findVertex(135816240), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(135816241), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(135816255), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(135814928), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(497311324), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(497301854), graph->findVertex(135824900)));
	kids.push_back(new Child(graph->findVertex(497311023), graph->findVertex(135824900)));

	kids.push_back(new Child(graph->findVertex(135814938), graph->findVertex(122528351)));
	kids.push_back(new Child(graph->findVertex(133267072), graph->findVertex(122528351)));
	kids.push_back(new Child(graph->findVertex(133267073), graph->findVertex(122528351)));
	Vertex* garage = graph->findVertex(312402720);
	
	PoIList poiList(garage);
    for (Child* child : kids)
		poiList.addHome(child);
		
	PathMatrix* matrix = graph->multipleDijkstra(poiList.getIDs());

	while (true) {
		int option;
		Menu::printTitle("Choose option", '-');
		cout << " 1 - Shortest Path between two PoIs" << endl;
		cout << " 2 - Add vehicle" << endl;
		cout << " 3 - Add kid" << endl;
		cout << " 4 - Set garage point" << endl;
		Menu::getInput<int>("Option: ", option, 1, 4);

		switch (option) {
			case 1: shortestPathOption(gv, graph, poiList, matrix); break;
			case 2: addVehicle(vehicles); break;
			case 3: addKid(gv, graph, poiList, matrix); break;
			case 4: setGarage(gv, graph, poiList, matrix); break;
		}
	}
}