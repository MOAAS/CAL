#pragma once

#include "Graph.h"

class Child
{
	Vertex home;
	Vertex school;
public:
	Child(Vertex home, Vertex school);
	Vertex getHome() const { return home; }
	Vertex getSchool() const { return school; }
};

