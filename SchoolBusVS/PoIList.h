#pragma once

#include <fstream>
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

	POI(const POI& poi);
	POI(Vertex* v, POItype t);
	POI(Child* c);

	POItype getType() const;
	Child* getChild() const;
	Vertex* getVertex() const;
	int getID() const;
private:
	union {
		Child* child;
		Vertex* school;
		Vertex* garage;
	};
	POItype type;
};

class PoIList
{
	vector<POI> pois;
	Vertex* garage;
	bool existsSchool(Vertex* school) const;
public:
	PoIList(Vertex* garage);
	PoIList(string fileName, const Graph* graph);
	void save(string fileName);
	Vertex* getGarage() const;
	vector<Child*> getChildren() const;
	void changeGarage(Vertex* garage);
	void addHome(Child* child);
	vector<int> getIDs() const;
	vector<POI> getPoIs() const;
	vector<Vertex*> getVertexes() const;
};

