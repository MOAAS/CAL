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

double Vehicle::getPathDist() const {
	double dist = 0;
	for (size_t i = 1; i < path.size(); i++) {
		double x1 = path[i - 1].vertex->getX();
		double y1 = path[i - 1].vertex->getY();

		double x2 = path[i].vertex->getX();
		double y2 = path[i].vertex->getY();

		dist += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}
	return dist;
}

double Vehicle::getReturnDist() const {
	double dist = 0;
	for (size_t i = 1; i < returnPath.size(); i++) {
		double x1 = returnPath[i - 1].vertex->getX();
		double y1 = returnPath[i - 1].vertex->getY();

		double x2 = returnPath[i].vertex->getX();
		double y2 = returnPath[i].vertex->getY();

		dist += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}
	return dist;
}

void Vehicle::assignPath(const vector<VehiclePathVertex>& path, const vector<VehiclePathVertex>& returnPath) {
	this->path = path;
	this->returnPath = returnPath;
}

bool Vehicle::operator<(const Vehicle &v) const {
	return this->getCapacity() > v.getCapacity(); 
}

int Vehicle::NUM_VEHICLES = 0;