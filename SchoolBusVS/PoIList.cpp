#include "PoIList.h"

bool PoIList::existsSchool(Vertex* school) {
	for (auto poi : this->pois) {
		if (poi.getType() == POI::School && poi.getVertex()->getID() == school->getID())
			return true;
	}
	return false;
}

PoIList::PoIList(Vertex * garage) : garage(garage) {
	pois.push_back(POI(garage, POI::Garage));
}

PoIList::PoIList(string fileName, const Graph* graph) {
	ifstream f(fileName);

	int garageID, homeID, schoolID;
	f >> garageID;
	this->garage = graph->findVertex(garageID);
	pois.push_back(POI(garage, POI::Garage));


	while (f >> homeID && f >> schoolID) {
		this->addHome(new Child(graph->findVertex(homeID), graph->findVertex(schoolID)));
	}
}

void PoIList::save(string fileName) {
	ofstream f(fileName);

	f << garage->getID() << endl;

	for (POI poi : pois) {
		if (poi.getType() == POI::Kid)
			f << poi.getChild()->getHome()->getID() << " " << poi.getChild()->getSchool()->getID() << endl;
	}

}

Vertex * PoIList::getGarage() {
	return garage;
}

void PoIList::changeGarage(Vertex * garage) {
	for (int i = 0; i < pois.size(); i++) {
		if (pois[i].getType() == POI::Garage) {
			pois[i].vertex = garage;
			break;
		}
	}
	this->garage = garage;
}

void PoIList::addHome(Child* child) {
	pois.push_back(POI(child, POI::Kid));
	if (!existsSchool(child->getSchool()))
		pois.push_back(POI(child->getSchool(), POI::School));
}

vector<int> PoIList::getIDs() const {
	vector<int> list;

	for (POI poi : pois) {
		list.push_back(poi.getID());
	}

	return list;
}

vector<POI> PoIList::getPoIs() const {
	return pois;
}
