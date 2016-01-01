#pragma once
#include "StdAfx.h"

class Vertex;
class Vector4d;

class PointTracker
{
public:
	PointTracker(const Vertex & p1, const Vertex & p2);
	~PointTracker();

	bool yStep();
	bool isValid() const { return valid; }

	double x, y, z;

private:
	double x1, y1, z1, x2, y2, z2;
	double xinc, zinc;
	bool valid;
};

