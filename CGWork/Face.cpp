#include <algorithm>
#include "Face.h"
#include "myHelpers.h"
#include "Vector4d.h"
#include "Vertex.h"
#include "Matrix4d.h"
#include "PointTracker.h"


Face::Face()
{
}

Face::~Face()
{
}

void Face::addVertex(Vertex * v) {
	if (v) {
		vertices.push_back(v);
	}
	else {
		log_warning("v is NULL\n");
	}
}

void Face::addEdge(Edge * e) {
	if (e) {
		edges.push_back(e);
	}
	else {
		log_warning("e is NULL\n");
	}
}

void Face::calcPlane() {
	if (vertices.size() < 3) {
		log_error("Cannot calculate plane, number of vertices is less than 3.");
		return;
	}
	Vector4d A = vertices[0]->getCoord();
	Vector4d B = vertices[1]->getCoord();
	Vector4d C = vertices[2]->getCoord();
	Vector4d norm;
	double D;
	calculatePlane(A, B, C, norm, D);
	setNormal(norm.getX(), norm.getY(), norm.getZ(), D);
}

void Face::setNormal(double A, double B, double C, double _D) {
	normal = Vector4d(A, B, C);
	normal = normalize(normal);
	D = _D;
}

void drawNormal(CDC * pDC, const Face & f, const Matrix4d & screenMat, COLORREF c) {
	Vector4d P;
	std::vector<Vertex*> vertices = f.getVertices();
	for (std::vector<Vertex*>::const_iterator v = vertices.begin(); v != vertices.end(); ++v) {
		P = P + (*v)->getCoord();
	}
	P = P / vertices.size();

	Vector4d norm = P + f.getNormal();

	P.homegenize();
	norm.homegenize();
	P = screenMat * P;
	norm = screenMat * norm;

	draw(pDC, P, norm, c);
}

double closestZ(const Face & f) {
	double cz = std::numeric_limits<double>::lowest();
	std::vector<Vertex*> VL = f.getVertices();
	for (std::vector<Vertex*>::const_iterator v = VL.begin(); v != VL.end(); ++v) {
		if ((*v)->getZ() > cz) {
			cz = (*v)->getZ();
		}
	}
	return cz;
}

//void draw(CDC* pDC, const Face & f, const Matrix4d & transMat, const Matrix4d & screenMat, COLORREF c) {
//	std::vector<Vertex*> VL = f.getVertices();
//	unsigned int i;
//	for (i = 0; i < VL.size()-1; ++i) {
//		draw(pDC, *VL[i], *VL[i+1], transMat, screenMat, c);
//	}
//	draw(pDC, *VL[i], *VL[0], transMat, screenMat, c);
//}

void Face::fillTriangle(CDC *pDC, const Vertex & v1, const Vertex & v2, const Vertex & v3, COLORREF c) {
	log_debug("----12----\n");
	PointTracker PT_12(v1, v2);
	log_debug("----13----\n");
	PointTracker PT_13(v1, v3);

	log_debug("Triangle: \n");
	log_debug_vertex(v1);
	log_debug_vertex(v2);
	log_debug_vertex(v3);
	log_debug("\n");

	if (!PT_12.isValid()) {
		draw(pDC, v1, v2, c);
	}
	
	// Upper triangle
	while (PT_12.isValid() && PT_13.isValid()) {
		int leftx = min(floor(PT_12.x), floor(PT_13.x));
		int rightx = max(floor(PT_12.x), floor(PT_13.x));
		for (int ix = leftx; ix < rightx; ++ix) {
			pDC->SetPixel(ix, PT_12.y, c);
		}
		PT_12.yStep();
		PT_13.yStep();
	 } 

	// Lower triangle
	log_debug("----23----\n");
	PointTracker PT_23(v2, v3);
	if (!PT_23.isValid()) {
		draw(pDC, v2, v3, c);
	}
	while (PT_13.isValid() && PT_23.isValid()) {
		int leftx = min(floor(PT_23.x), floor(PT_13.x));
		int rightx = max(floor(PT_23.x), floor(PT_13.x));
		for (int ix = leftx; ix < rightx; ++ix) {
			pDC->SetPixel(ix, PT_23.y, c);
		}
		PT_13.yStep();
		PT_23.yStep();
	}
}

void Face::fill(CDC *pDC, COLORREF c) {
	// Check if polygon is eye on.
	if (almost_eq(normal.getZ(), 0)) {
		return;
	}

	auto v1 = vertices.begin();
	auto v2 = vertices.begin() + 1;
	auto v3 = vertices.begin() + 2;
	for (; v3 != vertices.end(); ++v2, ++v3) {
		std::vector<Vertex*> vertices;
		vertices.push_back(*v1); vertices.push_back(*v2); vertices.push_back(*v3);
		std::sort(vertices.begin(), vertices.end(), VertexYGreater());
		fillTriangle(pDC, *vertices[0], *vertices[1], *vertices[2], c);
	}
}