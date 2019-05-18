#pragma once

#include <vector>
#include "Graph.h"
#include "Child.h"

using namespace std;

class Vehicle
{
	int capacity;
	vector<Child*> children;
	vector<Vertex*> path;
	vector<Vertex*> returnPath;
public:
	Vehicle(int capacity);
	int getCapacity() const;
	vector<Child*> getChildren() const;
	vector<Vertex*> getPath() const;
	vector<Vertex*> getReturnPath() const;
};

