#pragma once

#include "StdAfx.h"
#include <vector>
#include "Vertex.h"
#include "Edge.h"
#include "Vector4d.h"

class Face
{
public:
	Face();
	~Face();

	void addVertex(Vertex * v);
	void addEdge(Edge * e);
	std::vector<Vertex*> getVertices() const { return vertices; }
	std::vector<Edge*> getEdges() const { return edges; }
	void calcPlane();
	void setNormal(double A, double B, double C, double _D);
	void setD(double _D) { D = _D; }
	void setNormal(const Vector4d & n) { normal = n; }
	Vector4d getNormal() const { return normal; }

	void fill(CDC *pDC, COLORREF c);

	double D;

private:
	std::vector<Vertex*> vertices;
	std::vector<Edge*> edges;
	Vector4d normal;

	// assume that vertices are sorted by y in ascending order
	void fillTriangle(CDC *pDC, const Vertex & v1, const Vertex & v2, const Vertex & v3, COLORREF c);
};

void drawNormal(CDC * pDC, const Face & f, const Matrix4d & screenMat, COLORREF c);
double closestZ(const Face & f);
//void draw(CDC* pDC, const Face & f, const Matrix4d & transMat, const Matrix4d & screenMat, COLORREF c);