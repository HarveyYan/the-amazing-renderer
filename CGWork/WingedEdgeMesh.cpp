#include "WingedEdgeMesh.h"
#include "Vertex.h"
#include "myHelpers.h"
#include "Edge.h"
#include "Matrix4d.h"
#include "Face.h"


/*********************************/
/* Constructors and destructors. */
/*********************************/

WingedEdgeMesh::WingedEdgeMesh()
{
	m_modelMat = ID_MAT;
}

WingedEdgeMesh::WingedEdgeMesh(const WingedEdgeMesh & other) {
	m_modelMat = other.m_modelMat;
	color = other.color;
	normalColor = other.normalColor;
	Qmin = other.Qmin;
	Qmax = other.Qmax;

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
		pf->setNormal((*face_itr)->getNormal());

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
	for (std::vector<Vertex*>::iterator v = vertexList.begin(); v != vertexList.end(); ++v) {
		(*v)->transform(transMat);
	}
	
	for (std::vector<Face*>::iterator f = faceList.begin(); f != faceList.end(); ++f) {
		Vector4d n = transMat * (*f)->getNormal();
		Vector4d A = (*f)->getVertices().back()->getCoord(); 
		(*f)->setNormal(n);
		(*f)->setD(dot_product(n, A));
	}
}

void WingedEdgeMesh::homegenize() {
	for (auto v_itr = vertexList.begin(); v_itr != vertexList.end(); ++v_itr) {
		(*v_itr)->homegenize();
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

			//if (first) {
			//	x1 = x2 = vx;
			//	y1 = y2 = vy;
			//	first = false;
			//}
			//else if (vx < x1 && vy > y1) { // 1
			//	x1 = vx;
			//	y1 = vy;
			//}
			//else if (vx > x1 && vx < x2 && vy > y1) { // 2
			//	y1 = vy;
			//}
			//else if (vx > x2 && vy > y1) { // 3
			//	y1 = vy;
			//	x2 = vx;
			//}
			//else if (vx > x2 && vy < y1 && vy > y2) { // 4
			//	x2 = vx;
			//}
			//else if (vx > x2 && vy < y2) { // 5
			//	x2 = vx;
			//	y2 = vy;
			//}
			//else if (vx > x1 && vx < x2 && vy < y2) { // 6
			//	y2 = vy;
			//}
			//else if (vx < x1 && vy < y2) { // 7
			//	x1 = vx;
			//	y2 = vy;
			//}
			//else if (vx < x1 && vy < y1 && vy > y2) {
			//	x1 = vx;
			//}
		}
	}
	width = abs(x1 - x2);
	height = abs(y1 - y2);
	depth = abs(z1 - z2);
}

void drawMesh(CDC *pDC, const WingedEdgeMesh & wem, const Matrix4d & screenMat, bool bFill) {
	std::vector<Edge*> edgeList = wem.getEdgeList();
	COLORREF c = wem.getColor();

	if (bFill) {
		auto faceList = wem.getFaceList();
		for (auto f_itr = faceList.begin(); f_itr != faceList.end(); ++f_itr) {
			(*f_itr)->fill(pDC, c);
		}
	}
	//else {
		for (auto e_itr = edgeList.begin(); e_itr != edgeList.end(); ++e_itr) {
			// We do not change the original vertices. Instead, we accumulate all of the changes in the matrices and then
			// apply the transformation to a copy of the original vertices.
			Vertex v1 = *(*e_itr)->getV1();
			Vertex v2 = *(*e_itr)->getV2();
			/*v1.transform(transMat);
			v2.transform(transMat);
			v1.homegenize();
			v2.homegenize();
			v1.transform(screenMat);
			v2.transform(screenMat);*/
			draw(pDC, v1, v2, RGB(0,0,255)); //TODO return true color
		}
	//}
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

	Vertex v1(x_min, y_min, z_min),
		v2(x_min, y_max, z_min),
		v3(x_max, y_max, z_min),
		v4(x_max, y_min, z_min),
		v5(x_max, y_min, z_max),
		v6(x_min, y_min, z_max),
		v7(x_min, y_max, z_max),
		v8(x_max, y_max, z_max);

	Matrix4d M = transMat * obj.m_modelMat;
	v1.transform(M); v2.transform(M); v3.transform(M); v4.transform(M); 
	v5.transform(M); v6.transform(M); v7.transform(M); v8.transform(M);

	draw(pDC, v1, v2, screenMat, obj.getColor());
	draw(pDC, v2, v3, screenMat, obj.getColor());
	draw(pDC, v3, v4, screenMat, obj.getColor());
	draw(pDC, v4, v5, screenMat, obj.getColor());
	draw(pDC, v5, v6, screenMat, obj.getColor());
	draw(pDC, v6, v7, screenMat, obj.getColor());
	draw(pDC, v7, v8, screenMat, obj.getColor());
	draw(pDC, v1, v6, screenMat, obj.getColor());
	draw(pDC, v2, v7, screenMat, obj.getColor());
	draw(pDC, v3, v8, screenMat, obj.getColor());
	draw(pDC, v1, v4, screenMat, obj.getColor());
	draw(pDC, v5, v8, screenMat, obj.getColor());
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
		log_debug("e_%d, ([%f, %f, %f], [%f, %f, %f])\n",
			e - wem.getEdgeList().begin(),
			(*e)->getV1()->getX(), (*e)->getV1()->getY(), (*e)->getV1()->getZ(),
			(*e)->getV2()->getX(), (*e)->getV2()->getY(), (*e)->getV2()->getZ());
	}
}

void log_debug_wem(const WingedEdgeMesh & wem) {
	log_debug_vertex_list(wem);
	log_debug_edge_list(wem);
}