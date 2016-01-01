
#include "Edge.h"
#include "Vertex.h"
#include "stdafx.h"
#include "myHelpers.h"
#include "Matrix4d.h"


Edge::~Edge()
{
}

void Edge::transform(const Matrix4d & mat) {
	v1->transform(mat);
	v2->transform(mat);
}

//void draw(CDC* pDC, const Edge & e, const Matrix4d & transMat, const Matrix4d & screenMat, COLORREF c) {
//	draw(pDC, *e.getV1(), *e.getV2(), transMat, screenMat, c);
//}

/**************************/
/* 3D coordinate hashing. */
/**************************/

// The decimal digits percision. For example, if equal to 10000, 1.12345 and 1.12346 will collide.
static const long int HASH_PERCISION = 10000;
Edge* getEdgeFromMap(EdgeMap & eHM, const Edge & e) {
	long int xi1 = static_cast<long int>(e.getV1()->getX() * HASH_PERCISION);
	long int yi1 = static_cast<long int>(e.getV1()->getY() * HASH_PERCISION);
	long int zi1 = static_cast<long int>(e.getV1()->getZ() * HASH_PERCISION);
	long int xi2 = static_cast<long int>(e.getV2()->getX() * HASH_PERCISION);
	long int yi2 = static_cast<long int>(e.getV2()->getY() * HASH_PERCISION);
	long int zi2 = static_cast<long int>(e.getV2()->getZ() * HASH_PERCISION);
	auto e_it = eHM.find(EdgeKey(xi1, yi1, zi1, xi2, yi2, zi2));
	if (e_it != eHM.end()) {
		return e_it->second;
	}
	return NULL;
}
void insertEdgeToMap(EdgeMap & eHM, Edge * e) {
	long int xi1 = static_cast<long int>(e->getV1()->getX() * HASH_PERCISION);
	long int yi1 = static_cast<long int>(e->getV1()->getY() * HASH_PERCISION);
	long int zi1 = static_cast<long int>(e->getV1()->getZ() * HASH_PERCISION);
	long int xi2 = static_cast<long int>(e->getV2()->getX() * HASH_PERCISION);
	long int yi2 = static_cast<long int>(e->getV2()->getY() * HASH_PERCISION);
	long int zi2 = static_cast<long int>(e->getV2()->getZ() * HASH_PERCISION);
	eHM[EdgeKey(xi1, yi1, zi1, xi2, yi2, zi2)] = e;
}

/***********************/
/* Printing functions. */
/***********************/

void log_debug_edge(const Edge & e) {
	Vector4d v1 = e.getV1()->getCoord();
	Vector4d v2 = e.getV2()->getCoord();
	log_debug_less("[%f %f %f] -> [%f %f %f]\n", v1.getXUnhom(), v1.getYUnhom(), v1.getZUnhom(), 
		v2.getXUnhom(), v2.getYUnhom(), v2.getZUnhom());
}