#pragma once

#include <vector>
#include "Graph.h"
#include "Child.h"
#include "PoIList.h"

using namespace std;


struct VehiclePathVertex {
	VehiclePathVertex(Vertex* vertex) : vertex(vertex) { this->isPoI = false; }
	VehiclePathVertex(Vertex* vertex, POI::POItype type) : vertex(vertex), type(type) { this->isPoI = true; }
	Vertex* vertex;
	bool isPoI;
	POI::POItype type;
};

class Vehicle
{
	static int NUM_VEHICLES;
	int id;
	size_t capacity;
	vector<Child*> children;
	vector<VehiclePathVertex> path;
	vector<VehiclePathVertex> returnPath;
public:
	Vehicle(size_t capacity);
	size_t getCapacity() const;
	vector<Child*> getChildren() const;
	vector<VehiclePathVertex> getPath() const;
	vector<VehiclePathVertex> getReturnPath() const;
	int getID() const;
	double getPathDist() const;
	double getReturnDist() const;

	void assignPath(const vector<VehiclePathVertex>& path, const vector<VehiclePathVertex>& returnPath);
};

