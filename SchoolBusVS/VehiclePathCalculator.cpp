#include "VehiclePathCalculator.h"



void VehiclePathCalculator::calculate(vector<Vehicle*>& vehicles) {
	vector<Child*> kidsLeft = this->orderedKids;
	for (Vehicle* vehicle : vehicles) {
		this->assignKids(kidsLeft, vehicle, poiList.getGarage(), matrix);
	}
}

VehiclePathCalculator::VehiclePathCalculator(const vector<Child*>& orderedKids, const PoIList & poiList, PathMatrix * matrix) : orderedKids(orderedKids), poiList(poiList) {
	this->matrix = matrix;
}


void VehiclePathCalculator::assignKidGo(Child* child, vector<POI>& path, PathMatrix* matrix) {
	if (path.size() == 1) {
		path.insert(path.begin() + 1, POI(child));
		return;
	}

	int homeID = child->getHome()->getID();
	int assignedSpot = 1;
	double distIncrease = matrix->getDist(path[0].getID(), homeID) + matrix->getDist(homeID, path[1].getID()) - matrix->getDist(path[0].getID(), path[1].getID());

	for (int i = 2; i < (int)path.size(); i++) {
		double oldDist = matrix->getDist(path[i - 1].getID(), path[i].getID());
		double newDist = matrix->getDist(path[i - 1].getID(), homeID) + matrix->getDist(homeID, path[i].getID());
		if (newDist - oldDist < distIncrease) {
			assignedSpot = i;
			distIncrease = newDist - oldDist;
		}
		if (path[i].getType() == POI::School && path[i].getVertex() == child->getSchool()) {
			path.insert(path.begin() + assignedSpot, POI(child));
			return;
		}
	}

	if (matrix->getDist(path[path.size() - 1].getID(), homeID) < distIncrease)
		assignedSpot = path.size();

	path.insert(path.begin() + assignedSpot, POI(child));
}

void VehiclePathCalculator::assignSchoolGo(Vertex* school, vector<POI>& path, PathMatrix* matrix) {
	for (POI poi : path)
		if (poi.getType() == POI::School && poi.getVertex() == school)
			return;

	int schoolID = school->getID();
	int assignedSpot = path.size();
	double distIncrease = matrix->getDist(path[path.size() - 1].getID(), schoolID);

	for (int i = path.size() - 1; i >= 1; i--) {
		if (path[i].getType() == POI::Kid && path[i].getChild()->getSchool() == school)
			break;
		double oldDist = matrix->getDist(path[i - 1].getID(), path[i].getID());
		double newDist = matrix->getDist(path[i - 1].getID(), schoolID) + matrix->getDist(schoolID, path[i].getID());
		if (newDist - oldDist < distIncrease) {
			assignedSpot = i;
			distIncrease = newDist - oldDist;
		}
	}

	path.insert(path.begin() + assignedSpot, POI(school, POI::School));
}

void VehiclePathCalculator::assignKidReturn(Child* child, vector<POI>& returnPath, PathMatrix* matrix) {
	int homeID = child->getHome()->getID();
	int assignedSpot = returnPath.size();
	double distIncrease = matrix->getDist(returnPath[returnPath.size() - 1].getID(), homeID);

	for (int i = assignedSpot - 1; i >= 1; i--) {
		if (returnPath[i].getType() == POI::School && returnPath[i].getVertex() == child->getSchool())
			break;
		double oldDist = matrix->getDist(returnPath[i - 1].getID(), returnPath[i].getID());
		double newDist = matrix->getDist(returnPath[i - 1].getID(), homeID) + matrix->getDist(homeID, returnPath[i].getID());
		if (newDist - oldDist < distIncrease) {
			assignedSpot = i;
			distIncrease = newDist - oldDist;
		}
	}
	returnPath.insert(returnPath.begin() + assignedSpot, POI(child));
}

void VehiclePathCalculator::assignSchoolReturn(Vertex* school, vector<POI>& returnPath, PathMatrix* matrix) {
	for (POI poi : returnPath)
		if (poi.getType() == POI::School && poi.getVertex() == school)
			return;

	if (returnPath.size() == 1) {
		returnPath.insert(returnPath.begin() + 1, POI(school, POI::School));
		return;
	}

	int schoolID = school->getID();
	int assignedSpot = 1;
	double distIncrease = matrix->getDist(returnPath[0].getID(), schoolID) + matrix->getDist(schoolID, returnPath[1].getID()) - matrix->getDist(returnPath[0].getID(), returnPath[1].getID());

	for (int i = 2; i < (int)returnPath.size(); i++) {
		double oldDist = matrix->getDist(returnPath[i - 1].getID(), returnPath[i].getID());
		double newDist = matrix->getDist(returnPath[i - 1].getID(), schoolID) + matrix->getDist(schoolID, returnPath[i].getID());
		if (newDist - oldDist < distIncrease) {
			assignedSpot = i;
			distIncrease = newDist - oldDist;
		}
		if (returnPath[i].getType() == POI::Kid && returnPath[i].getChild()->getSchool() == school) {
			returnPath.insert(returnPath.begin() + assignedSpot, POI(school, POI::School));
			return;
		}
	}

	if (matrix->getDist(returnPath[returnPath.size() - 1].getID(), schoolID) < distIncrease)
		assignedSpot = returnPath.size();


	returnPath.insert(returnPath.begin() + assignedSpot, POI(school, POI::School));
}

void VehiclePathCalculator::assignKids(vector<Child*>& kidsLeft, Vehicle* vehicle, Vertex* garage, PathMatrix* matrix) {
	// Make path
	vector<POI> path;
	path.push_back(POI(garage, POI::Garage));

	for (int i = 0; i < (int)kidsLeft.size() && i < vehicle->getCapacity(); i++) {
		assignKidGo(kidsLeft[i], path, matrix);
		assignSchoolGo(kidsLeft[i]->getSchool(), path, matrix);
	}

	//for (Child* child : kidsLeft) {
	//	assignSchoolGo(child->getSchool(), path, matrix);
	//}

	if (path[path.size() - 1].getType() == POI::Kid)
		cout << "AAAAAAAAAAAAAAAAAAAAAA" << endl;

	// Make return path
	vector<POI> returnPath;
	returnPath.push_back(path[path.size() - 1]);

	for (int i = 0; i < (int)kidsLeft.size() && i < vehicle->getCapacity(); i++) {
		assignKidReturn(kidsLeft[i], returnPath, matrix);
		assignSchoolReturn(kidsLeft[i]->getSchool(), returnPath, matrix);

	}

	//for (Child* child : kidsLeft) {
	//	assignSchoolReturn(child->getSchool(), returnPath, matrix);
	//}

	returnPath.push_back(POI(garage, POI::Garage));

	// Make vehicle path
	vector<VehiclePathVertex> fullPath, fullReturnPath;
	for (int i = 0; i < (int)path.size() - 1; i++) {
		vector<Vertex*> pathBetweenPoIs = matrix->getPath(path[i].getID(), path[i + 1].getID());
		fullPath.push_back(VehiclePathVertex(path[i].getVertex(), path[i].getType()));
		if (pathBetweenPoIs.size() > 2)
			fullPath.insert(fullPath.end(), pathBetweenPoIs.begin() + 1, pathBetweenPoIs.end() - 1);
	}

	fullPath.push_back(VehiclePathVertex(path[path.size() - 1].getVertex(), path[path.size() - 1].getType()));
		
	// Make vehicle return path
	for (int i = 0; i < (int)returnPath.size() - 1; i++) {
		vector<Vertex*> pathBetweenPoIs = matrix->getPath(returnPath[i].getID(), returnPath[i + 1].getID());
		fullReturnPath.push_back(VehiclePathVertex(returnPath[i].getVertex(), returnPath[i].getType()));
		if (pathBetweenPoIs.size() > 2)
			fullReturnPath.insert(fullReturnPath.end(), pathBetweenPoIs.begin() + 1, pathBetweenPoIs.end() - 1);
	}

	fullReturnPath.push_back(VehiclePathVertex(returnPath[returnPath.size() - 1].getVertex(), returnPath[returnPath.size() - 1].getType()));

	if (vehicle->getCapacity() <= kidsLeft.size())
		kidsLeft.erase(kidsLeft.begin(), kidsLeft.begin() + vehicle->getCapacity());
	else kidsLeft.clear();

	vehicle->assignPath(fullPath, fullReturnPath);
}