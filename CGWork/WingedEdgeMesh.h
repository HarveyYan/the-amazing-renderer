#pragma once

#include "StdAfx.h"
#include <vector>
#include "Vector4d.h"
#include "Matrix4d.h"
class Vertex;
class Edge;
class Face;

// WingedEdgeMesh holds all of the vertices, edges, and faces. Includes all of the adjacency information.
class WingedEdgeMesh
{
public:
	WingedEdgeMesh();
	WingedEdgeMesh(const WingedEdgeMesh & other);
	~WingedEdgeMesh();

	void addVertex(Vertex * v);
	void addEdge(Edge * e);
	void addFace(Face * f);
	std::vector<Vertex*> getVertexList() const { return vertexList; }
	std::vector<Edge*> getEdgeList() const { return edgeList; }
	// Returns a dynamically allocated copy of the edges list (and the vertices that make up the edges).
	// Caller must free these.
	std::vector<Face*> getFaceList() const { return faceList; }
	void setColor(COLORREF c);
	COLORREF getColor() const { return color; }
	COLORREF getNormalColor() const { return normalColor; }
	void setNormalColor(COLORREF c){
		normalColor = c;
	}
	// Bounding box.
	Vector4d getQmin() const { return Qmin; }
	Vector4d getQmax() const { return Qmax; }
	void setQmin(Vector4d & v) { Qmin = v; }
	void setQmax(Vector4d & v) { Qmax = v; }

	void transform(const Matrix4d & transMat);
	void homegenize();

	Matrix4d m_modelMat;

private:
	std::vector<Vertex*> vertexList;
	std::vector<Edge*> edgeList;
	std::vector<Face*> faceList;
	COLORREF color;
	COLORREF normalColor = RGB(0, 0, 0);
	Vector4d Qmin, Qmax;
};

double diameter(const WingedEdgeMesh & wem);
double diameter(const std::vector<WingedEdgeMesh> & objects);
double diameterUpperBound(const std::vector<WingedEdgeMesh> & objects);
void BBSize(const std::vector<WingedEdgeMesh> & objects, double & width, double & height, double & depth);
void drawMesh(CDC *pDC, const WingedEdgeMesh & wem, const Matrix4d & screenMat, bool bFill);
void drawBoundingBox(CDC* pDC, WingedEdgeMesh & obj, const Matrix4d & transMat, const Matrix4d & screenMat);
void drawVertexNormals(CDC* pDC, WingedEdgeMesh & obj, const Matrix4d & screenMat);

// debug functions
void log_debug_vertex_list(const WingedEdgeMesh & wem);
void log_debug_edge_list(const WingedEdgeMesh & wem);
void log_debug_wem(const WingedEdgeMesh & wem);

WingedEdgeMesh copyMesh(const WingedEdgeMesh & wem);
void deleteMesh(WingedEdgeMesh & wem);