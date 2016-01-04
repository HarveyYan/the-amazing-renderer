#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "WingedEdgeMesh.h"
#include "Vertex.h"
#include "myHelpers.h"
#include "Edge.h"
#include "Matrix4d.h"
#include "Face.h"
#include "PointTracker.h"


/*********************************/
/* Constructors and destructors. */
/*********************************/

WingedEdgeMesh::WingedEdgeMesh()
{
	m_modelMat = ID_MAT;
	winding = CW;
}

WingedEdgeMesh::WingedEdgeMesh(const WingedEdgeMesh & other) {
	m_modelMat = other.m_modelMat;
	color = other.color;
	normalColor = other.normalColor;
	Qmin = other.Qmin;
	Qmax = other.Qmax;
	winding = other.winding;

	VertexMap verticesMap;
	EdgeMap edgesMap;

	auto other_FL = other.getFaceList();

	// Loop over all faces and deep copy them
	for (auto face_itr = other_FL.begin(); face_itr != other_FL.end(); ++face_itr) {
		auto other_f_vertices = (*face_itr)->getVertices();
		if (other_f_vertices.size() < 3) {
			log_error("Face has less than 3 vertices!\n");
		}
		
		// Create the face.
		Face *pf = new Face();
		pf->normal = (*face_itr)->normal;
		pf->D = (*face_itr)->D;
		pf->normal_pt1 = (*face_itr)->normal_pt1;
		pf->normal_pt2 = (*face_itr)->normal_pt2;

		// Iterate over vertices with two iterators, one pointing at the current vertex and the other at the next one.
		auto v_itr = other_f_vertices.begin();
		auto v_next_itr = other_f_vertices.begin() + 1;

		// Check whether the vertex already exists.
		Vertex *pv = getVertexFromMap(verticesMap, **v_itr);
		if (!pv) {
			pv = new Vertex((*v_itr)->getCoord());
			insertVertexToMap(verticesMap, pv);
			addVertex(pv);
		}

		for (; v_itr != other_f_vertices.end(); ++v_itr, ++v_next_itr) {
			// If we reached the end, connect last vertex with the first one.
			if (v_next_itr == other_f_vertices.end()) {
				v_next_itr = other_f_vertices.begin();
			}

			// Check whether the vertex already exists.
			Vertex *pv_next = getVertexFromMap(verticesMap, **v_next_itr);
			if (!pv_next) {
				pv_next = new Vertex((*v_next_itr)->getCoord());
				insertVertexToMap(verticesMap, pv_next);
				addVertex(pv_next);
			}

			pv->setNormal((*v_itr)->getNormal());

			// Connect current vertex to the next one (if edges does not exist already).
			Edge *pe = getEdgeFromMap(edgesMap, Edge(pv, pv_next));
			if (!pe) {
				pe = getEdgeFromMap(edgesMap, Edge(pv_next, pv));
				if (!pe) {
					pe = new Edge(pv, pv_next);
					insertEdgeToMap(edgesMap, pe);
					addEdge(pe);
				}
			}

			// Update adjacency information.
			pf->addVertex(pv);
			pf->addEdge(pe);
			pv->addFace(pf);
			if (!pe->getF1()) {
				pe->setF1(pf);
			}
			else if (!pe->getF2()) {
				pe->setF2(pf);
			}

			// Advance vertex pointer
			pv = pv_next;
		}
		addFace(pf);
	}
}

WingedEdgeMesh::~WingedEdgeMesh()
{
	deleteMesh(*this);
}

void deleteMesh(WingedEdgeMesh & wem) {
	std::vector<Vertex*> vertexList = wem.getVertexList();
	std::vector<Edge*> edgeList = wem.getEdgeList();
	std::vector<Face*> faceList = wem.getFaceList();
	for (std::vector<Face*>::iterator f = faceList.begin(); f != faceList.end(); ++f) {
		delete *f;
	}
	for (std::vector<Vertex*>::iterator v = vertexList.begin(); v != vertexList.end(); ++v) {
		delete *v;
	}
	for (std::vector<Edge*>::iterator e = edgeList.begin(); e != edgeList.end(); ++e) {
		delete *e;
	}
}

/************************/
/* Getters and setters. */
/************************/

void WingedEdgeMesh::addVertex(Vertex * v) {
	vertexList.push_back(v);
}

void WingedEdgeMesh:: addEdge(Edge * e) {
	edgeList.push_back(e);
}

void WingedEdgeMesh::addFace(Face * f) {
	faceList.push_back(f);
}


void WingedEdgeMesh::setColor(COLORREF c) {
	color = c;
}

/**/
/**/
/**/

void WingedEdgeMesh::transform(const Matrix4d & transMat) {
	for (Vertex * vp : vertexList) {
		vp->transform(transMat);
		vp->transformNormal(transMat);
	}
	
	for (Face * fp : faceList) {
		fp->transformNormal(transMat);
	}
}

void WingedEdgeMesh::calcBackFaceCulling(const Vector4d & cameraP, const Matrix4d & modelMat, bool bIsPerspective) {
	for (auto f_itr = faceList.begin(); f_itr != faceList.end(); ++f_itr) {
		// Normal vector (in world space).
		Vector4d n = modelMat * (*f_itr)->normal_pt2 - modelMat * (*f_itr)->normal_pt1;
		// Vector from the camera to one of the face vertices (in world space).
		Vector4d v = modelMat * (*f_itr)->getVertices().at(0)->getCoord() - cameraP;

		bool perspBackFacing = bIsPerspective &&
			((winding == CW && dot_product(n, v) < 0) || (winding == CCW && dot_product(n, v) > 0));
		bool orthoBackFacing = !bIsPerspective &&
			((winding == CW && n.getZ() < 0) || (winding == CCW && n.getZ() > 0));
		if (perspBackFacing || orthoBackFacing) {
			(*f_itr)->setBackFacing(true);
		}
		else {
			(*f_itr)->setBackFacing(false);
		}
	}
}

void WingedEdgeMesh::homegenize() {
	for (auto v_itr = vertexList.begin(); v_itr != vertexList.end(); ++v_itr) {
		(*v_itr)->homegenize();
	}
	for (auto f_itr = faceList.begin(); f_itr != faceList.end(); ++f_itr) {
		(*f_itr)->homegenizeNormalPts();
	}
}

void WingedEdgeMesh::inverseNormals() {
	for (Vertex * vp : vertexList) {
		vp->inverseNormal();
	}
	for (Face * fp : faceList) {
		fp->inverseNormal();
	}
	if (winding == CW) {
		winding = CCW;
	}
	else {
		winding = CW;
	}
}

/**/
/**/
/**/

double diameter(const WingedEdgeMesh & wem) {
	double d = 0;
	std::vector<Vertex*> VL = wem.getVertexList();
	for (std::vector<Vertex*>::iterator vertex1 = VL.begin(); vertex1 < VL.end(); ++vertex1) {
		for (std::vector<Vertex*>::iterator vertex2 = vertex1 + 1; vertex2 != VL.end(); ++vertex2) {
			double new_dist = distance(**vertex1, **vertex2);
			if (new_dist > d) {
				d = new_dist;
			}
		}
	}
	return d;
}

double diameter(const std::vector<WingedEdgeMesh> & objects) {
	WingedEdgeMesh meshUnion;
	for (std::vector<WingedEdgeMesh>::const_iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		std::vector<Vertex*> VL = obj->getVertexList();
		for (std::vector<Vertex*>::iterator v = VL.begin(); v != VL.end(); ++v) {
			meshUnion.addVertex(*v);
		}
	}
	return diameter(meshUnion);
}

double diameterUpperBound(const std::vector<WingedEdgeMesh> & objects) {
	double dUpperBound = 0;
	for (std::vector<WingedEdgeMesh>::const_iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		dUpperBound += diameter(*obj);
	}
	return dUpperBound;
}

void BBSize(const std::vector<WingedEdgeMesh> & objects, double & width, double & height, double & depth) {
	double x1 = (std::numeric_limits<double>::max)();
	double y1 = std::numeric_limits<double>::lowest();
	double z1 = (std::numeric_limits<double>::max)();
	double x2 = std::numeric_limits<double>::lowest();
	double y2 = (std::numeric_limits<double>::max)();
	double z2 = std::numeric_limits<double>::lowest();
	
	for (std::vector<WingedEdgeMesh>::const_iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		std::vector<Vertex*> VL = obj->getVertexList();
		for (std::vector<Vertex*>::iterator v = VL.begin(); v != VL.end(); ++v) {
			double vx = (*v)->getX();
			double vy = (*v)->getY();
			double vz = (*v)->getZ();
			x1 = min(x1, vx);
			y1 = max(y1, vy);
			z1 = min(z1, vz);
			x2 = max(x2, vx);
			y2 = min(y2, vy);
			z2 = max(z2, vz);
		}
	}
	width = abs(x1 - x2);
	height = abs(y1 - y2);
	depth = abs(z1 - z2);
}

void WingedEdgeMesh::highLightSilh(CDC *pDC) {
	for (Edge * ep : edgeList) {
		Face *fp1 = ep->getF1();
		Face *fp2 = ep->getF2();

		bool isSilhEdge = (fp1 && fp2) && (fp1->isBackFacing() ^ fp2->isBackFacing());
		/*bool isOneManifoldEdge = fp1 && !fp2 || !fp1 && fp2;
		bool hasFacingFace = (fp1 && !fp1->isBackFacing()) || (fp2 && !fp2->isBackFacing());
		if (isSilhEdge || (isOneManifoldEdge && hasFacingFace))*/
		if (isSilhEdge)
		{
			Vector4d p1 = ep->getV1()->getCoord();
			Vector4d p2 = ep->getV2()->getCoord();
			Vector4d r = rotate(0, 0, M_PI / 2) * normalize(p2 - p1);

			std::vector<Vertex> vertices { 
				Vertex(p1 - silhLineThickness*r),
				Vertex(p1 + silhLineThickness*r),
				Vertex(p2 - silhLineThickness*r),
				Vertex(p2 + silhLineThickness*r)};
			std::sort(vertices.begin(), vertices.end(), VertexYGreater());
			fillTriangle(pDC, vertices[0], vertices[1], vertices[2], silhColor);
			fillTriangle(pDC, vertices[1], vertices[2], vertices[3], silhColor);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
		}
	}
}

void drawMesh(CDC *pDC, const WingedEdgeMesh & wem, bool bFill, bool bBackFaceCulling, ZBuffer * zbuf /*=NULL*/) {
	std::vector<Edge*> edgeList = wem.getEdgeList();
	std::vector<Face*> faceList = wem.getFaceList();
	COLORREF c = wem.getColor();
		
	if (bFill) {
		for (auto f_itr = faceList.begin(); f_itr != faceList.end(); ++f_itr) {
			if (!bBackFaceCulling || !(*f_itr)->isBackFacing()) {
				(*f_itr)->fill(pDC, c, zbuf);
			}
		}
	}
	else {
		for (auto e_itr = edgeList.begin(); e_itr != edgeList.end(); ++e_itr) {
			Edge *ep = *e_itr;
			Face *fp1 = ep->getF1();
			Face *fp2 = ep->getF2();
			// Check for backface culling.
			bool hasFacingFace = (fp1 && !fp1->isBackFacing()) || (fp2 && !fp2->isBackFacing());
			if (!bBackFaceCulling || hasFacingFace)
			{
				draw(pDC, *(*e_itr)->getV1(), *(*e_itr)->getV2(), c);
			}
		}
	}
}

void drawBoundingBox(CDC* pDC, WingedEdgeMesh & obj, const Matrix4d & transMat, const Matrix4d & screenMat) {
	Vector4d qmin = obj.getQmin();
	Vector4d qmax = obj.getQmax();
	double x_min = qmin.getX();
	double y_min = qmin.getY();
	double z_min = qmin.getZ();
	double x_max = qmax.getX();
	double y_max = qmax.getY();
	double z_max = qmax.getZ();

	static const int size = 8;
	Vertex vertices[size] = { 
		Vertex(x_min, y_min, z_min),
		Vertex(x_min, y_max, z_min),
		Vertex(x_max, y_max, z_min),
		Vertex(x_max, y_min, z_min),
		Vertex(x_max, y_min, z_max),
		Vertex(x_min, y_min, z_max),
		Vertex(x_min, y_max, z_max),
		Vertex(x_max, y_max, z_max)};

	Matrix4d M = transMat * obj.m_modelMat;
	for (Vertex & v : vertices) {
		v.transform(M);
		v.homegenize();
		v.transform(screenMat);
	}
	for (int i = 0; i < size-1; ++i) {
		draw(pDC, vertices[i], vertices[i + 1], obj.getColor());
	}
	draw(pDC, vertices[0], vertices[5], obj.getColor());
	draw(pDC, vertices[1], vertices[6], obj.getColor());
	draw(pDC, vertices[2], vertices[7], obj.getColor());
	draw(pDC, vertices[0], vertices[3], obj.getColor());
	draw(pDC, vertices[4], vertices[7], obj.getColor());
}

void drawVertexNormals(CDC* pDC, WingedEdgeMesh & obj, const Matrix4d & screenMat) {
	std::vector<Vertex*> VL = obj.getVertexList();
	for (std::vector<Vertex*>::iterator ver = VL.begin(); ver != VL.end(); ++ver) {
		drawNormal(pDC, **ver, screenMat, obj.getNormalColor());
	}
}

void log_debug_vertex_list(const WingedEdgeMesh & wem) {
	log_debug("\n============================== Vertex list =============================\n");
	std::vector<Vertex*> VL = wem.getVertexList();
	for (auto v = VL.begin(); v != VL.end(); ++v) {
		log_debug("v_%d, [%f, %f, %f]\n", v - VL.begin(), (*v)->getX(), (*v)->getY(), (*v)->getZ());
	}
}

void log_debug_edge_list(const WingedEdgeMesh & wem) {
	log_debug("\n============================== Edge list =============================\n");
	for (auto e = wem.getEdgeList().begin(); e != wem.getEdgeList().end(); ++e) {
		log_debug_less("e_%d, ([%f, %f, %f], [%f, %f, %f])\n",
			e - wem.getEdgeList().begin(),
			(*e)->getV1()->getX(), (*e)->getV1()->getY(), (*e)->getV1()->getZ(),
			(*e)->getV2()->getX(), (*e)->getV2()->getY(), (*e)->getV2()->getZ());
	}
}

void log_debug_face_list(const WingedEdgeMesh & wem) {
	log_debug("\n============================== Face list =============================\n");
	std::vector<Face*> FL = wem.getFaceList();
	for (int i = 0; i < FL.size(); ++i) {
		log_debug_less("f_%d, %p : ", i, FL.at(i));
		log_debug_face(*FL.at(i));
	}
}

void log_debug_wem(const WingedEdgeMesh & wem) {
	log_debug_vertex_list(wem);
	log_debug_edge_list(wem);
	log_debug_face_list(wem);
}