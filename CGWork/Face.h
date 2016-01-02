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
	double getD() const { return D; }
	void setNormal(const Vector4d & n);
	Vector4d getNormal() const { return normal; }
	void setBackFacing(bool isBackFacing) { m_bBackFacing = isBackFacing; }
	bool isBackFacing() const { return m_bBackFacing; }

	void fill(CDC *pDC, COLORREF c);
	void drawNormal(CDC *pDC, const Matrix4d & screenMat, COLORREF c);
	void transformNormal(const Matrix4d & transMat);
	void homegenizeNormalPts();

	double D;
	Vector4d normal;
	Vector4d normal_pt1;
	Vector4d normal_pt2;

private:
	std::vector<Vertex*> vertices;
	std::vector<Edge*> edges;	

	// assume that vertices are sorted by y in ascending order
	void fillTriangle(CDC *pDC, const Vertex & v1, const Vertex & v2, const Vertex & v3, COLORREF c);

	bool m_bBackFacing;
};

void drawNormal(CDC * pDC, const Face & f, const Matrix4d & screenMat, COLORREF c);
double closestZ(const Face & f);
//void draw(CDC* pDC, const Face & f, const Matrix4d & transMat, const Matrix4d & screenMat, COLORREF c);

void log_debug_face(const Face & f);