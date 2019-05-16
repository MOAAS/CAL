#pragma once

#include "Graph.h"
#include "Child.h"

struct POI {
	enum POItype {
		Kid,
		School,
		Garage
	};

	union {
		Vertex vertex;
		Child child;
	};
	POItype type;
public:
	//POI(Vertex v, POItype t) : vertex(v), type(t) { }
	//POI(Child c, POItype t) : child(c), type(t) { }
	POItype getType() const { return type; }
	Child getChild() const { return child; }
	Vertex getVertex() const { return vertex; }
};

class PoIList
{
	vector<POI> pois;
	bool existsSchool(Vertex school);
public:
	PoIList(Vertex* garage, const vector<Child>& kids);
	void addHome(const Child& child);
};

