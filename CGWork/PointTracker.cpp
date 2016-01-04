#include "PointTracker.h"

#include "Vertex.h"
#include "Vector4d.h"
#include "myHelpers.h"

// Assumes that p1.y < p2.y
PointTracker::PointTracker(const Vertex & p1, const Vertex & p2)
{
	x1 = p1.getXUnhom(); y1 = p1.getYUnhom(); z1 = p1.getZUnhom();
	x2 = p2.getXUnhom(); y2 = p2.getYUnhom(); z2 = p2.getZUnhom();

	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;

	//if (abs(y2-y1) > 1) {
	if (floor(y2) != floor(y1)) {
		xinc = dx / dy;
		zinc = dz / dy;

		double x_init_d = -(y2 - floor(y2))*(dx / dy);
		double z_init_d = -(y2 - floor(y2))*(dz / dy);

		x = x2 + x_init_d;
		y = floor(y2);
		z = z2 + z_init_d;

		valid = true;
	}
	else {
		valid = false;
		x = y = z = 0;
		xinc = zinc = 0;
	}
	/*y = ceil(y1);
	x = x1 + xinc * (y - y1);
	z = z1 + zinc * (y - y1);*/

	/*y = floor(y1);
	x = x1 - xinc * (y1 - y);
	z = z1 - zinc * (y1 - y);*/
}

PointTracker::~PointTracker()
{
}

bool PointTracker::yStep() {
	y -= 1;
	x -= xinc;
	z -= zinc;

	if (y < y1 || !valid) {
		valid = false;
		return false;
	}
	return true;
}
