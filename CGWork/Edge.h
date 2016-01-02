#pragma once

#include "StdAfx.h"
#include <vector>
#include <map>

class Vertex;
class Face;
class Matrix4d;
class CDC;

// Edge class for connecting to vertices. Assumes 2-manifold.
class Edge
{
public:
	Edge(Vertex * _v1, Vertex * _v2) : v1(_v1), v2(_v2), f1(0), f2(0) {};
	Edge(Vertex * _v1, Vertex * _v2, Face * _f1, Face * _f2) : v1(_v1), v2(_v2), f1(_f1), f2(_f2) {};
	~Edge();

	// TODO why the const?
	Vertex * getV1() const { return v1; }
	Vertex * getV2() const { return v2; }
	Face * getF1() { return f1; }
	Face * getF2() { return f2; }
	void setF1(Face *f) { f1 = f; }
	void setF2(Face *f) { f2 = f; }

	void transform(const Matrix4d & mat);

private:
	Vertex *v1, *v2;
	Face *f1, *f2;
	std::vector<Edge*> incidentEdges; // TODO information is present in v1, v2 anyway, no?
};

//void draw(CDC* pDC, const Edge & e, const Matrix4d & transMat, const Matrix4d & screenMat, COLORREF c);

// Map edges (pairs of 3d coordinates) to edges. 
// Has a limited percision of 4 digits after the decimal point.
typedef std::tuple<long int, long int, long int, long int, long int, long int> EdgeKey;
typedef std::map<EdgeKey, Edge*> EdgeMap;
Edge* getEdgeFromMap(EdgeMap & eHM, const Edge & e);
void insertEdgeToMap(EdgeMap & eHM, Edge * e);

void log_debug_edge(const Edge & e);