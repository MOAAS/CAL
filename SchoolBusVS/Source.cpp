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

GraphViewer* createGraphViewer(const Graph* graph, bool enableWeights) {
	GraphViewer* gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);
	for (Vertex* vertex : graph->getVertexSet()) {
		gv->addNode(vertex->getID(), (int)vertex->getX() - 527500, (int)vertex->getY() - 4555555);
		//gv->addNode(vertex->getID(), (int)vertex->getX(), (int)vertex->getY());
	}

	for (Vertex* vertex : graph->getVertexSet()) {
		for (Edge* edge : vertex->getAdj()) {
			gv->addEdge(edge->getID(), vertex->getID(), edge->getDest()->getID(), EdgeType::DIRECTED);
			if (enableWeights)
				gv->setEdgeWeight(edge->getID(), (int)edge->getWeight());
		}
	}
	gv->rearrange();
	return gv;
}

void destroyGraphViewer(GraphViewer* gv) {
	gv->closeWindow();
	free(gv);
}

void toggleNodeIDs(GraphViewer* gv, const Graph* graph, const vector<int>& poiIDs) {
	static int enableType = 0;

	enableType = (enableType + 1) % 3;

	switch (enableType) {
		case 0: cout << "Disabling node IDs... "; break;
		case 1: cout << "Disabling non POI ids... "; break;
		case 2: cout << "Enabling all node IDs... "; break;
	}

	for (Vertex* vertex : graph->getVertexSet()) {
		gv->addNode(vertex->getID(), (int)vertex->getX() - 527500, (int)vertex->getY() - 4555555);
		if (enableType == 2)
			gv->setVertexLabel(vertex->getID(), to_string(vertex->getID()));
		else gv->clearVertexLabel(vertex->getID());
	}

	if (enableType == 1) {
		for (int id : poiIDs) 
			gv->setVertexLabel(id, to_string(id));
	}

	cout << "Done." << endl;
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
	Menu::displayColored("Updating graph colors... ", MENU_YELLOW);
	for (Vertex* v : vertexSet) {
		gv->setVertexColor(v->getID(), BLUE);
		for (Edge* e : v->getAdj())
			gv->setEdgeColor(e->getID(), BLACK);
	}
	highlightPoIs(gv, pois);
	Menu::displayColored("Done.", MENU_YELLOW) << endl;
}

/******************************\
|*******  MENU OPTIONS ********|
\******************************/

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
		Menu::getLineInput_CI("Do you wish to continue? (Y / N) ", input, { "Y","N" });
		resetGraphColors(gv, graph->getVertexSet(), poiList);
		if (input == "N")
			break;
	}
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
	else if (input == "Y")
		poiList.addHome(new Child(home, school));
	else cout << "Succesfully cancelled operation" << endl;
}

void setGarage(GraphViewer* gv, Graph* graph, PoIList& poiList, PathMatrix* matrix) {
	int ID; string input;
	Menu::printHeader("Set Garage (Depot) vertex");
	Menu::getInput<int>("Depot ID: ", ID);
	Menu::getLineInput_CI("Depot ID: " + to_string(ID) + ". Proceed? (Y / N) ", input, { "Y","N" });
	Vertex* garage = graph->findVertex(ID);
	if (garage == NULL)
		cout << "Couldn't find garage ID." << endl;
	else if (input == "Y")
		poiList.changeGarage(garage);
	else cout << "Succesfully cancelled operation" << endl;
}

void verifyConnectivity(const vector<int>& ids, PathMatrix* matrix) {
	Menu::printHeader("Connectivity check");
	Menu::displayColored("Checking connectivity for the following PoIs: ", MENU_WHITE);
	printVector::ofValues(cout, ids, " ") << endl;
	int missingPaths = 0;
	for (size_t i = 0; i < ids.size(); i++) {
		for (size_t j = 0; j < ids.size(); j++) {
			if (matrix->getDist(ids[i], ids[j]) == INF) {
				cout << "There is no path from " << ids[i] << " to " << ids[j] << endl;
				missingPaths++;
			}
		}
	}
	if (missingPaths == 0)
		Menu::displayColored("There are paths between every pair of PoIs", MENU_LIGHTGREEN) << endl;
	else if (missingPaths == 1)
		Menu::displayColored("There is a path missing between two PoIs", MENU_LIGHTRED) << endl;
	else Menu::displayColored("There are " + to_string(missingPaths) + " paths missing.", MENU_LIGHTRED) << endl;
}

void articulationPoints(GraphViewer* gv , Graph* graph, const vector<int>& ids) {
	Menu::printHeader("Articulation Points");
	vector<Vertex *> articulationPoints = graph->articulationPoints(ids);
	if (articulationPoints.size() > 0) {
		Menu::displayColored("There are articulation Points between PoIs", MENU_LIGHTRED) << endl;
		for(Vertex * v : articulationPoints)
			gv->setVertexColor(v->getID(), RED);
	}
	else Menu::displayColored("There are no articulation Points between PoIs", MENU_LIGHTGREEN) << endl;
}

void verifyStronglyConnected(Graph* graph) {
	Menu::printHeader("Graph strongly connected check");
	if(graph->stronglyConnected())
		Menu::displayColored("Graph is strongly connected", MENU_LIGHTGREEN);
	else Menu::displayColored("Graph is not strongly connected", MENU_LIGHTRED);
}
Graph* makeGraphFromPoIs(const vector<POI>& poiList, PathMatrix* matrix) {
	Graph* graph = new Graph();

	for (POI poi : poiList) {
		graph->addVertex(poi.getID(), poi.getVertex()->getX(), poi.getVertex()->getY());
	}

	int edgeId = 0;
	for (size_t i = 0; i < poiList.size(); i++) {
		int iID = poiList[i].getID();
		for (size_t j = 0; j < poiList.size(); j++) {
			int jID = poiList[j].getID();
			graph->addEdge(edgeId++, iID, jID, matrix->getDist(iID, jID));
		}
	}
	
	return graph;
}

void showPoIsOnly(const PoIList& poiList, PathMatrix* matrix) {
	Menu::printHeader("Test feature");
	Graph* graph = makeGraphFromPoIs(poiList.getPoIs(), matrix);
	GraphViewer* gv = createGraphViewer(graph, true);
	highlightPoIs(gv, poiList);

	system("pause");


	destroyGraphViewer(gv);
}
/******************************\
|********* LOAD / SAVE ********|
\******************************/

void saveVehicles(const vector<Vehicle*>& vehicles) {
	ofstream f("../Files/vehicles.txt");

	for (Vehicle* vehicle : vehicles) {
		f << vehicle->getCapacity() << " ";
	}
}

vector<Vehicle*> loadVehicles() {
	vector<Vehicle*> vehicles;
	ifstream f("../Files/vehicles.txt");

	int capacity;
	while (f >> capacity)
		vehicles.push_back(new Vehicle(capacity));
	return vehicles;
}


int main() {
	cout << "HELLO WORLD" << endl;
	cout << "Loading Graph..." << endl;
	Graph* graph = GraphBuilder("../Graphs/nodes.txt", "../Graphs/edges.txt").build();

	cout << "Loading PoIs..." << endl;
	PoIList poiList("../Files/pois.txt", graph);

	cout << "Pre-processing..." << endl;
	PathMatrix* matrix = graph->multipleDijkstra(poiList.getIDs());

	cout << "Opening Graph Viewer..." << endl;
	GraphViewer *gv = createGraphViewer(graph, false);
	highlightPoIs(gv, poiList);
	   
	cout << "Loading vehicles..." << endl;
	vector<Vehicle*> vehicles = loadVehicles();

	while (true) {
		int option;
		Menu::printTitle("Choose option", '-');
		cout << " 1 - Shortest Path between two PoIs" << endl;
		cout << " 2 - Add vehicle" << endl;
		cout << " 3 - Add kid" << endl;
		cout << " 4 - Set garage point" << endl;
		cout << " 5 - Verify connectivity " << endl;
		cout << " 6 - Verify graph strong connectivity" << endl;
		cout << " 7 - Update Graph Viewer PoIs" << endl;
		cout << " 8 - Test feature" << endl;
		cout << " 9 - Toggle node IDs" << endl;
		cout << " 10 -Verify Articulation Points" << endl;
		cout << " 0 - Save and quit" << endl;
		Menu::getInput<int>("Option: ", option, 0, 10);

		switch (option) {
			case 1: shortestPathOption(gv, graph, poiList, matrix); break;
			case 2: addVehicle(vehicles); break;
			case 3: addKid(gv, graph, poiList, matrix); break;
			case 4: setGarage(gv, graph, poiList, matrix); break;
			case 5: verifyConnectivity(poiList.getIDs(), matrix); break;
			case 6:	verifyStronglyConnected(graph); break;
			case 7: resetGraphColors(gv, graph->getVertexSet(), poiList); break;
			case 8: showPoIsOnly(poiList, matrix); break;
			case 9: toggleNodeIDs(gv, graph, poiList.getIDs()); break;
			case 10: articulationPoints(gv ,graph, poiList.getIDs()); break;
			case 0: poiList.save("../Files/pois.txt"); saveVehicles(vehicles); return 0;

		}
	}

	destroyGraphViewer(gv);
}