#pragma once

#include "Graph.h"
#include "Child.h"

class POI {
public:
	enum POItype {
		Kid,
		School,
		Garage
	};

	~POI() {}
	POI(const POI& poi) {}
	POI(Vertex v, POItype t) : vertex(v), type(t) { }
	POI(Child c, POItype t) : child(c), type(t) { }
	POItype getType() const { return type; }
	Child getChild() const { return child; }
	Vertex getVertex() const { return vertex; }
	int getID() const {
		if (type == Kid)
			return child.getHome().getID();
		return vertex.getID();
	}
private:
	union {
		Vertex vertex;
		Child child;
	};
	POItype type;
};

class PoIList
{
	vector<POI> pois;
	Vertex garage;
	bool existsSchool(Vertex school);
public:
	PoIList(Vertex* garage);
	void addHome(const Child& child);
	vector<int> getIDs() const;
};

