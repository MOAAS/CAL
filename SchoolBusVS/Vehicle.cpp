#include "Vehicle.h"


Vehicle::Vehicle(int capacity) {
	this->capacity = capacity;
}

int Vehicle::getCapacity() const
{
	return capacity;
}

vector<Child*> Vehicle::getChildren() const
{
	return children;
}

vector<Vertex*> Vehicle::getPath() const
{
	return path;
}

vector<Vertex*> Vehicle::getReturnPath() const
{
	return returnPath;
}
