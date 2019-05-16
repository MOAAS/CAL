#include "PoIList.h"

bool PoIList::existsSchool(Vertex school) {
	for (auto poi : this->pois) {
		if (poi.getType() == School && poi.getVertex() == school)
			return true;
	}
	return false;
}

PoIList::PoIList(Vertex * garage, const vector<Child>& kids) {
	pois.push_back(POI(*garage, Garage));

	for (Child child : kids) {
		pois.push_back(POI(child, Kid));
		if (!existsSchool(child.getSchool()))
			pois.push_back(POI(child.getSchool(), School));
	}
}

void PoIList::addHome(const Child & child) {
	pois.push_back(POI(child, Kid));
	if (!existsSchool(child.getSchool()))
		pois.push_back(POI(child.getSchool(), School));
}