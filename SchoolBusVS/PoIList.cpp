#include "PoIList.h"

bool PoIList::existsSchool(Vertex school) {
	for (auto poi : this->pois) {
		if (poi.getType() == POI::School && poi.getVertex().getID() == school.getID())
			return true;
	}
	return false;
}

PoIList::PoIList(Vertex * garage) : garage(*garage) {
	pois.push_back(POI(*garage, POI::Garage));
}

void PoIList::addHome(const Child & child) {
	pois.push_back(POI(child, POI::Kid));
	if (!existsSchool(child.getSchool()))
		pois.push_back(POI(child.getSchool(), POI::School));
}

vector<int> PoIList::getIDs() const {
	vector<int> list;

	for (POI poi : pois) {
		list.push_back(poi.getID());
	}

	return list;
}
