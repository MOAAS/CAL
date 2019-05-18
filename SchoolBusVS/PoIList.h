#pragma once

#include "Graph.h"
#include "Child.h"

class POI {
	friend class PoIList;
public:
	enum POItype {
		Kid,
		School,
		Garage
	};

	~POI() {}
	POI(const POI& poi) {
		if (poi.type == Kid)
			this->child = poi.child;
		else this->vertex = poi.vertex;
		this->type = poi.type;
	}
	POI(Vertex* v, POItype t) : vertex(v), type(t) { }
	POI(Child* c, POItype t) : child(c), type(t) { }
	POItype getType() const { return type; }
	Child* getChild() const { return child; }
	Vertex* getVertex() const { return vertex; }
	int getID() const {
		if (type == Kid)
			return child->getHome()->getID();
		return vertex->getID();
	}
private:
	union {
		Vertex* vertex;
		Child* child;
	};
	POItype type;
};

class PoIList
{
	vector<POI> pois;
	Vertex* garage;
	bool existsSchool(Vertex* school);
public:
	PoIList(Vertex* garage);
	Vertex* getGarage();
	void setGarage(Vertex* garage);
	void addHome(Child* child);
	vector<int> getIDs() const;
	vector<POI> getPoIs() const;
};

