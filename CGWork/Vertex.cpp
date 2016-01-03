#include "Vertex.h"
#include "Matrix4d.h"
#include "myHelpers.h"
#include "stdafx.h"
#include "Face.h"

Vertex::Vertex() : m_hasNormal(false) {}
Vertex::Vertex(double x, double y, double z) : coord(x, y, z), m_hasNormal(false) {};
Vertex::Vertex(const Vector4d & c) : coord(c), m_hasNormal(false) {};

Vertex::~Vertex() {}

void Vertex::set(double x, double y, double z) {
	coord.setX(x);
	coord.setY(y);
	coord.setZ(z);
}

void Vertex::set(const Vector4d & c) {
	coord = c;
}

void Vertex::transform(const Matrix4d & mat) {
	coord = mat * coord;
}

void Vertex::transformNormal(const Matrix4d & mat) {
	normal = mat * normal;
	normal_pt2 = mat * normal_pt2;
}

void Vertex::addEdge(Edge *pe) {
	incidentEdges.push_back(pe);
}

void Vertex::addFace(Face *f) {
	incidentFaces.push_back(f);
}

void Vertex::setNormal(const Vector4d & n) {
	normal = normalize(n);
	calcNormalEndPts();
	m_hasNormal = true;
}

void Vertex::setNormal(double x, double y, double z) {
	normal = normalize(Vector4d(x, y, z));
	calcNormalEndPts();
	m_hasNormal = true;
}

void Vertex::approximateNormal() {
	Vector4d approx_norm(0, 0, 0);
	std::vector<Face*>::const_iterator f;
	for (f = incidentFaces.begin(); f != incidentFaces.end(); ++f) {
		approx_norm = approx_norm + (*f)->getNormal();
	}
	normal = approx_norm / incidentFaces.size();
	--f;
	if (dot_product((*f)->getNormal(), normal) < 0) {
		normal = -normal;
	}
	normal = normalize(normal);
	calcNormalEndPts();
}

void Vertex::calcNormalEndPts() {
	normal_pt2 = coord + normal;
}

void draw(CDC * pDC, Vertex v1, Vertex v2, COLORREF c) {
	midpoint_line_draw(pDC, v1.getX(), v1.getY(), v2.getX(), v2.getY(), c);
}

void drawNormal(CDC * pDC, const Vertex & v, const Matrix4d & screenMat, COLORREF c) {
	Vector4d P = v.getCoord();
	Vector4d norm = P + v.getNormal();

	P.homegenize();
	norm.homegenize();
	P = screenMat * P;
	norm = screenMat * norm;

	draw(pDC, P, norm, c);
}

void Vertex::drawNormal(CDC *pDC, COLORREF c) {
	draw(pDC, coord, normal_pt2, c);
}

double distance(const Vertex & v1, const Vertex & v2) {
	return distance(v1.coord, v2.coord);
}

void Vertex::homegenize() {
	coord.homegenize();
	normal.homegenize();
}

/***********************/
/* Printing functions. */
/***********************/

void log_debug_vertex(const Vertex & v) {
	log_debug_less("[%f %f %f %f]\n", v.getCoord().getXUnhom(), v.getCoord().getYUnhom(), v.getCoord().getZUnhom(), v.getCoord().getH());
}

/**************************/
/* 3D coordinate hashing. */
/**************************/

// The decimal digits percision. For example, if equal to 10000, 1.12345 and 1.12346 will collide.
static const long int HASH_PERCISION = 10000; 
Vertex* getVertexFromMap(VertexMap & vHM, const Vertex & v) {
	long int xi = static_cast<long int>(v.getX() * HASH_PERCISION);
	long int yi = static_cast<long int>(v.getY() * HASH_PERCISION);
	long int zi = static_cast<long int>(v.getZ() * HASH_PERCISION);
	auto v_it = vHM.find(VertexKey(xi, yi, zi));
	if (v_it != vHM.end()) {
		return v_it->second;
	}
	return NULL;
}
void insertVertexToMap(VertexMap & vHM, Vertex * v) {
	long int xi = static_cast<long int>(v->getX() * HASH_PERCISION);
	long int yi = static_cast<long int>(v->getY() * HASH_PERCISION);
	long int zi = static_cast<long int>(v->getZ() * HASH_PERCISION);
	vHM[VertexKey(xi, yi, zi)] = v;
}