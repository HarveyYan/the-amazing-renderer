#pragma once

#include "stdafx.h"

#include <vector>
#include "Vector4d.h"
#include <map>

class Edge;
class Matrix4d;
class CDC;
class Face;

class Vertex
{
public:	
	Vertex();
	Vertex(double x, double y, double z);
	Vertex(const Vector4d & c);
	~Vertex();

	void set(double x, double y, double z);
	void set(const Vector4d &c);
	double getX() const { return coord.getX(); }
	double getY() const { return coord.getY(); }
	double getZ() const { return coord.getZ(); }
	double getXUnhom() const { return coord.getXUnhom(); }
	double getYUnhom() const { return coord.getYUnhom(); }
	double getZUnhom() const { return coord.getZUnhom(); }
	double getH() const { return coord.getH(); }
	Vector4d getCoord() const { return coord; }
	void transform(const Matrix4d & mat);
	void transformNormal(const Matrix4d & mat);

	void homegenize();

	void addEdge(Edge *pe);
	void addFace(Face *f);
	std::vector<Edge*> getEdges() const { return incidentEdges; }
	std::vector<Face*> gettFaces() const{ return incidentFaces; }

	void setNormal(const Vector4d & n);
	void setNormal(double x, double y, double z);
	void approximateNormal();
	Vector4d getNormal() const { return normal; }
	bool hasNormal() const { return m_hasNormal; }
	void drawNormal(CDC *pDC, COLORREF c);

	/* Operators. */

	friend double distance(const Vertex & v1, const Vertex & v2);

private:
	Vector4d coord;
	std::vector<Edge*> incidentEdges;
	std::vector<Face*> incidentFaces;
	Vector4d normal;
	bool m_hasNormal;
};


void draw(CDC * pDC, Vertex v1, Vertex v2, COLORREF c);
void drawNormal(CDC * pDC, const Vertex & v, const Matrix4d & screenMat, COLORREF c);
double distance(const Vertex & v1, const Vertex & v2);
void log_debug_vertex(const Vertex & v);


// Map 3d coordinates to vertices. Has a limited percision of 4 digits after the decimal point.
typedef std::tuple<long int, long int, long int> VertexKey;
typedef std::map<VertexKey, Vertex*> VertexMap;
Vertex* getVertexFromMap(VertexMap & vHM, const Vertex & v);
void insertVertexToMap(VertexMap & vHM, Vertex * v);


struct PVertexYGreater {
	bool operator()(const Vertex *lhs, const Vertex *rhs) const {
		if (lhs->getY() == rhs->getY()) // break ties to the left
			return lhs->getX() < rhs->getX();
		return lhs->getY() < rhs->getY();
	}
};

struct VertexYGreater {
	bool operator()(const Vertex &lhs, const Vertex &rhs) const {
		if (lhs.getY() == rhs.getY()) // break ties to the left
			return lhs.getX() < rhs.getX();
		return lhs.getY() < rhs.getY();
	}
};