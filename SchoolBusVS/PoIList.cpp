#include "PoIList.h"

POI::POI(const POI& poi) {
	switch (poi.type) {
		case Kid: this->child = poi.child; break;
		case School: this->school = poi.school; break;
		case Garage: this->garage = poi.garage; break;
	}
	this->type = poi.type;
}

POI::POI(Vertex* v, POItype t) : type(t) {
	if (type == Garage)
		garage = v;
	else if (type == School)
		school = v;
	else logic_error("Type can't be Kid");
}

POI::POI(Child* c) : child(c) {
	this->type = Kid;
}


POI::POItype POI::getType() const {
	return type; 
}

Child* POI::getChild() const { 
	return child;
}

Vertex* POI::getVertex() const {
	switch (type) {
		case Kid: return child->getHome();
		case School: return school;
		case Garage: return garage;
		default: return NULL;
	}
}

int POI::getID() const {
	return this->getVertex()->getID();
}


/************* List **************/


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
	pois.push_back(POI(this->garage, POI::Garage));


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
	for (size_t i = 0; i < pois.size(); i++) {
		if (pois[i].getType() == POI::Garage) {
			pois[i].garage = garage;
			break;
		}
	}
	this->garage = garage;
}

void PoIList::addHome(Child* child) {
	pois.push_back(POI(child));
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

vector<Vertex*> PoIList::getVertexes() const {
	vector<Vertex*> vertexList;
	for (POI poi : pois)
		vertexList.push_back(poi.getVertex());
}
