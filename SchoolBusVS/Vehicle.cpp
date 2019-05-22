#include "Vehicle.h"


Vehicle::Vehicle(size_t capacity) {
	this->capacity = capacity;
	this->id = this->NUM_VEHICLES++;
}

size_t Vehicle::getCapacity() const
{
	return capacity;
}

vector<Child*> Vehicle::getChildren() const
{
	return children;
}

vector<VehiclePathVertex> Vehicle::getPath() const
{
	return path;
}

vector<VehiclePathVertex> Vehicle::getReturnPath() const
{
	return returnPath;
}

int Vehicle::getID() const {
	return id;
}

void Vehicle::assignPath(const vector<VehiclePathVertex>& path, const vector<VehiclePathVertex>& returnPath) {
	this->path = path;
	this->returnPath = returnPath;
}

int Vehicle::NUM_VEHICLES = 0;