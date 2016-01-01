#include "PointTracker.h"

#include "Vertex.h"
#include "Vector4d.h"
#include "myHelpers.h"

PointTracker::PointTracker(const Vertex & p1, const Vertex & p2)
{
	x1 = p1.getXUnhom(); y1 = p1.getYUnhom(); z1 = p1.getZUnhom();
	x2 = p2.getXUnhom(); y2 = p2.getYUnhom(); z2 = p2.getZUnhom();

	log_debug("%f == %f?\n", y1, y2);
	if (abs(y2-y1) > 1) {
		xinc = (x2 - x1) / (y2 - y1);
		zinc = (z2 - z1) / (y2 - y1);
		log_debug("!eq: xinc, zinc = %f, %f\n", xinc, zinc);
		valid = true;
	}
	else {
		log_debug("eq\n");
		valid = false;
	}

	y = ceil(y1);
	x = x1 + xinc * (y - y1);
	z = z1 + zinc * (y - y1);
}

PointTracker::~PointTracker()
{
}

bool PointTracker::yStep() {
	if (y+1 >= y2 || !valid) {
		valid = false;
		return false;
	}

	y += 1;
	x += xinc;
	z += zinc;
	return true;
}
