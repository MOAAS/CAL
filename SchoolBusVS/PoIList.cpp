#include "PoIList.h"

bool PoIList::existsSchool(Vertex school) {
	for (auto poi : this->pois) {
		if (poi.getType() == POI::School && poi.getVertex().getID() == school.getID())
			return true;
	}
	return false;
}

PoIList::PoIList(Vertex * garage, const vector<Child>& kids) {
	pois.push_back(POI(*garage, POI::Garage));

	for (Child child : kids) {
		pois.push_back(POI(child, POI::Kid));
		if (!existsSchool(child.getSchool()))
			pois.push_back(POI(child.getSchool(), POI::School));
	}
}

void PoIList::addHome(const Child & child) {
	pois.push_back(POI(child, POI::Kid));
	if (!existsSchool(child.getSchool()))
		pois.push_back(POI(child.getSchool(), POI::School));
}