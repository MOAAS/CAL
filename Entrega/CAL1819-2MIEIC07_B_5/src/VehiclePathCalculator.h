#pragma once

#include <vector>
#include "PathMatrix.h"
#include "PoIList.h"
#include "Vehicle.h"
#include "Menu.h"
class VehiclePathCalculator {
	const vector<Child*> orderedKids;
	PoIList poiList;
	PathMatrix* matrix;
public:
	VehiclePathCalculator(const vector<Child*>& orderedKids, const PoIList& poiList, PathMatrix* matrix);

	void calculate(vector<Vehicle*>& vehicles);
	void assignKidGo(Child* child, vector<POI>& path, PathMatrix* matrix);
	void assignKidReturn(Child* child, vector<POI>& path, PathMatrix* matrix);
	void assignSchoolGo(Vertex* school, vector<POI>& path, PathMatrix* matrix);
	void assignSchoolReturn(Vertex* school, vector<POI>& returnPath, PathMatrix* matrix);
	void assignKids(vector<Child*>& kidsLeft, Vehicle* vehicle, Vertex* garage, PathMatrix* matrix);

};

