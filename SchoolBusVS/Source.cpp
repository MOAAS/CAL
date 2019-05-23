#include "Graph.h"
#include "GraphBuilder.h"
#include "Vehicle.h"
#include "Menu.h"
#include "VehiclePathCalculator.h"

#include <iostream>
#include <chrono>

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
	gv->rearrange();
}

void highlightPath(GraphViewer* gv, const vector<Vertex*>& path) {
	for (int i = 0; i < (int)path.size() - 1; i++) {
		gv->setVertexColor(path[i]->getID(), CYAN);
		for (Edge* e : path[i]->getAdj()) {
			if (e->getDest() == path[i + 1]) {
				gv->setEdgeColor(e->getID(), RED);
				break;
			}
		}
	}
	if (path.size() == 0)
		return;
	gv->setVertexColor(path[0]->getID(), WHITE);
	if (path.size() == 1)
		return;
	gv->setVertexColor(path[path.size() - 1]->getID(), BLACK);
	gv->rearrange();
}

void highlightPoIs(GraphViewer* gv, const PoIList& pois) {
	for (POI poi : pois.getPoIs()) {
		switch (poi.getType()) {
		case POI::Garage: gv->setVertexColor(poi.getID(), LIGHT_GRAY); break;
		case POI::School: gv->setVertexColor(poi.getID(), RED); break;
		case POI::Kid: gv->setVertexColor(poi.getID(), GREEN); break;
		}
	}
	gv->rearrange();
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
	Menu::getInput<int>("Vehicle capacity (Max. 100): ", capacity, 1, 100);
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
		*matrix = *graph->multipleDijkstra(poiList.getIDs());
		highlightPoIs(gv, poiList);
	}
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
	int missingPaths = matrix->getNumMissingPaths(ids, true);
	if (missingPaths == 0)
		Menu::displayColored("There are paths between every pair of PoIs", MENU_LIGHTGREEN) << endl;
	else if (missingPaths == 1)
		Menu::displayColored("There is a path missing between two PoIs", MENU_LIGHTRED) << endl;
	else Menu::displayColored("There are " + to_string(missingPaths) + " paths missing.", MENU_LIGHTRED) << endl;
}

void articulationPoints(GraphViewer* gv, Graph* graph, const PoIList poiList) {
	Menu::printHeader("Articulation Points");
	vector<Vertex *> articulationPoints = graph->articulationPoints(poiList.getIDs());
	if (articulationPoints.size() > 0) {
		Menu::displayColored("There are articulation Points between PoIs", MENU_LIGHTRED) << endl;
		for (Vertex * v : articulationPoints)
			gv->setVertexColor(v->getID(), ORANGE);
	}
	else Menu::displayColored("There are no articulation Points between PoIs", MENU_LIGHTGREEN) << endl;
	gv->rearrange();
	Menu::getLineInput_CI("Do you wish to go back to the main menu? (Y to leave) ", *(new string), { "Y" });	
	resetGraphColors(gv, graph->getVertexSet(), poiList);
}

void verifyStronglyConnected(Graph* graph) {
	Menu::printHeader("Graph strongly connected check");
	if (graph->stronglyConnected())
		Menu::displayColored("Graph is strongly connected", MENU_LIGHTGREEN) << endl;
	else Menu::displayColored("Graph is not strongly connected", MENU_LIGHTRED) << endl;
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

vector<Vehicle*> getUsedVehicles(int kidsLeft, vector<Vehicle*> vehicles) {
	vector<Vehicle*> v;
	vector<Vehicle*>::iterator it = vehicles.begin();

	while (kidsLeft > 0 && vehicles.size() != 0) {
		if (kidsLeft >= (*it)->getCapacity()) {
			kidsLeft -= (*it)->getCapacity();
			v.push_back(*it);
			vehicles.erase(it);
			it = vehicles.begin();
		}
		else {
			it++;
			if (it == vehicles.end() || kidsLeft > (*it)->getCapacity()) {
				it--;
				kidsLeft -= (*it)->getCapacity();
				v.push_back(*it);
				vehicles.erase(it);
			}
		}
	}

	return v;
}

vector<Child *> orderKidsMST(vector<POI> poiList, PathMatrix* matrix) {
	vector<Child *> orderedKids;
	vector<POI>::iterator it;
	for (it = poiList.begin(); it != poiList.end(); it++) {
		if (it->getType() == POI::School) {
			poiList.erase(it);
			it--;
		}
	}
	Graph* graph = makeGraphFromPoIs(poiList, matrix);
	vector<Vertex*> route = graph->calculatePrim();

	for (size_t i = 0; i < route.size(); i++) {
		for (it = poiList.begin(); it != poiList.end(); it++)
			if (route.at(i)->getID() == it->getID()) {
				if (it->getType() == POI::Kid)
					orderedKids.push_back(it->getChild());
				break;
			}
	}

	return orderedKids;
}


void displayVehiclePath(GraphViewer* gv, PoIList& poiList, const vector<VehiclePathVertex>& path, double dist) {
	for (VehiclePathVertex v : path) {
		if (!v.isPoI) {
			Menu::displayColored(to_string(v.vertex->getID()) + " ", MENU_CYAN);
			continue;
		}
		switch (v.type) {
		case POI::Garage: Menu::displayColored(to_string(v.vertex->getID()) + " ", MENU_LIGHTGRAY); break;
		case POI::School: Menu::displayColored(to_string(v.vertex->getID()) + " ", MENU_LIGHTRED); break;
		case POI::Kid: Menu::displayColored(to_string(v.vertex->getID()) + " ", MENU_LIGHTGREEN); break;
		}
	}

	cout << endl;
	Menu::displayColored("Total distance: " + to_string(dist), MENU_WHITE) << endl;

	vector<Vertex*> vertices;
	for (VehiclePathVertex v : path)
		vertices.push_back(v.vertex);

	highlightPath(gv, vertices);
	highlightPoIs(gv, poiList);
}

void logVehiclePath(ofstream& file, const vector<VehiclePathVertex>& path, string title, double totalDist) {
	file << title << endl;
	file << "Start at ID: " << path[0].vertex->getID() << endl;
	for (size_t i = 1; i < path.size(); i++) {
		double x1 = path[i - 1].vertex->getX();
		double y1 = path[i - 1].vertex->getY();

		double x2 = path[i].vertex->getX();
		double y2 = path[i].vertex->getY();

		double M_2PI = 2 * 3.14159265358979323846;
		double dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

		double angle = atan2(y2 - y1, x2 - x1);
		int octant = lround(8 * angle / M_2PI + 8) % 8;

		switch (octant) {
			case 0: file << "Go East "; break;
			case 2: file << "Go South "; break;
			case 4: file << "Go West "; break;
			case 6: file << "Go North "; break;
			case 1: file << "Go Southeast "; break;
			case 3: file << "Go Southwest "; break;
			case 5: file << "Go Northwest "; break;
			case 7: file << "Go Northeast "; break;
		}

		file << dist << " meters, to " << path[i].vertex->getID() << endl;
	}

	file << "End at ID: " << path[path.size() - 1].vertex->getID() << endl;
	file << "Total distance: " << totalDist << " meters." << endl;
}


void pathCalculator(GraphViewer* gv, Graph* graph, PoIList& poiList, PathMatrix* matrix, vector<Vehicle*> vehicles) {
	Menu::printHeader("Route Calculator");

	int missingPaths = matrix->getNumMissingPaths(poiList.getIDs(), false);
	if (missingPaths > 0) {
		if (missingPaths == 1)
			Menu::displayColored("There is a path missing between two PoIs", MENU_LIGHTRED) << endl;
		else Menu::displayColored("There are " + to_string(missingPaths) + " paths missing.", MENU_LIGHTRED) << endl;
		return;
	}

	size_t numChildren = poiList.getChildren().size();
	size_t totalCapacity = 0;
	for (Vehicle* vehicle : vehicles)
		totalCapacity += vehicle->getCapacity();
	
	if (numChildren > totalCapacity) {
		Menu::displayColored("Not enough vehicle space!", MENU_LIGHTRED) << endl;
		Menu::displayColored(to_string(numChildren) + " Kids VS " + to_string(totalCapacity) + " bus slots!", MENU_LIGHTRED) << endl;
		return;
	}


	
	// Algoritmo MST
	auto start = chrono::steady_clock::now();
	vector<Child*> orderedKids = orderKidsMST(poiList.getPoIs(), matrix);

	// Algoritmo Greedy
	vector<Vehicle*> usedVehicles = getUsedVehicles(orderedKids.size(), vehicles);

	// Algoritmo Nearest Insertion
	VehiclePathCalculator* calc = new VehiclePathCalculator(orderedKids, poiList, matrix);
	calc->calculate(usedVehicles);

	auto end = chrono::steady_clock::now();
	cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	system("pause");
	for (Vehicle* vehicle : vehicles) {
		Menu::printTitle("Vehicle ID: " + to_string(vehicle->getID()), '-');
		double distPath = vehicle->getPathDist();
		double distReturn = vehicle->getReturnDist();
		// Path
		Menu::printHeader("Path to school");
		displayVehiclePath(gv, poiList, vehicle->getPath(), distPath);
		system("pause");
		resetGraphColors(gv, graph->getVertexSet(), poiList);

		// Return
		Menu::printHeader("Path from school (Return)");
		displayVehiclePath(gv, poiList, vehicle->getReturnPath(), distReturn);		
		system("pause");
		resetGraphColors(gv, graph->getVertexSet(), poiList);


		// Saving
		ofstream f1("../Files/vehicle" + to_string(vehicle->getID()) + "Go.txt");
		ofstream f2("../Files/vehicle" + to_string(vehicle->getID()) + "Return.txt");

		cout << "Saving path to ../Files/vehicle" << vehicle->getID() << "Go.txt...";
		logVehiclePath(f1, vehicle->getPath(), "Path", distPath);
		cout << "Done." << endl;

		cout << "Saving return path to ../Files/vehicle" << vehicle->getID() << "Return.txt... ";
		logVehiclePath(f2, vehicle->getReturnPath(), "Return Path", distReturn);
		cout << "Done." << endl;


	}
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
	//auto start = chrono::steady_clock::now();
	PathMatrix* matrix = graph->multipleDijkstra(poiList.getIDs());

	//auto end = chrono::steady_clock::now();	
	//cout << chrono::duration_cast<chrono::milliseconds>(end - start).count()  << endl;
	
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
		cout << " 5 - Verify connectivity between PoIs" << endl;
		cout << " 6 - Verify graph strong connectivity" << endl;
		cout << " 7 - Update Graph Viewer PoIs" << endl;
		cout << " 8 - Toggle node IDs" << endl;
		cout << " 9 - Verify Articulation Points" << endl;
		cout << " 10 - Calculate Bus Route" << endl;
		cout << " 0 - Save and quit" << endl;
		Menu::getInput<int>("Option: ", option, 0, 11);

		switch (option) {
			case 1: shortestPathOption(gv, graph, poiList, matrix); break;
			case 2: addVehicle(vehicles); break;
			case 3: addKid(gv, graph, poiList, matrix); break;
			case 4: setGarage(gv, graph, poiList, matrix); break;
			case 5: verifyConnectivity(poiList.getIDs(), matrix); break;
			case 6:	verifyStronglyConnected(graph); break;
			case 7: resetGraphColors(gv, graph->getVertexSet(), poiList); break;
			case 8: toggleNodeIDs(gv, graph, poiList.getIDs()); break;
			case 9: articulationPoints(gv, graph, poiList); break;
			case 10: pathCalculator(gv, graph, poiList, matrix, vehicles); break;
			case 0: poiList.save("../Files/pois.txt"); saveVehicles(vehicles); return 0;

		}
	}

	destroyGraphViewer(gv);
}