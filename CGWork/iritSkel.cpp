#include <iostream>
#include <string>
#include <map>
#include "stdafx.h"
#include "iritSkel.h"

#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

#include "MyHelpers.h"
#ifdef MY_DEBUG
#include <conio.h>
#endif


/*****************************************************************************
* Skeleton for an interface to a parser to read IRIT data files.			 *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 1.0, Feb 2002				 *
* Minimal changes made by Amit Mano			November 2008					 *
******************************************************************************/

IPFreeformConvStateStruct CGSkelFFCState = {
	FALSE,          /* Talkative */
	FALSE,          /* DumpObjsAsPolylines */
	TRUE,           /* DrawFFGeom */
	FALSE,          /* DrawFFMesh */
	{ 10, 10, 10 }, /* 10 isocurves peru/v/w direction. */
	100,            /* 100 point samples along a curve. */
	SYMB_CRV_APPROX_UNIFORM,  /* CrvApproxMethod */
	FALSE,   /* ShowIntrnal */
	FALSE,   /* CubicCrvsAprox */
	20,      /* Polygonal FineNess */
	FALSE,   /* ComputeUV */
	TRUE,    /* ComputeNrml */
	FALSE,   /* FourPerFlat */
	0,       /* OptimalPolygons */
	FALSE,   /* BBoxGrid */
	TRUE,    /* LinearOnePolyFlag */
	FALSE
};

//CGSkelProcessIritDataFiles(argv + 1, argc - 1);


/*****************************************************************************
* DESCRIPTION:                                                               *
* Main module of skeleton - Read command line and do what is needed...	     *
*                                                                            *
* PARAMETERS:                                                                *
*   FileNames:  Files to open and read, as a vector of strings.              *
*   NumFiles:   Length of the FileNames vector.								 *
*                                                                            *
* RETURN VALUE:                                                              *
*   bool:		false - fail, true - success.                                *
*****************************************************************************/
bool CGSkelProcessIritDataFiles(CString &FileNames, int NumFiles)
{
	IPObjectStruct *PObjects;
	IrtHmgnMatType CrntViewMat;

	/* Get the data files: */
	IPSetFlattenObjects(FALSE);
	if ((PObjects = IPGetDataFiles((const char* const *)(LPCTSTR*)&FileNames, 1/*NumFiles*/, TRUE, FALSE)) == NULL)
		return false;
	PObjects = IPResolveInstances(PObjects);

	if (IPWasPrspMat)
		MatMultTwo4by4(CrntViewMat, IPViewMat, IPPrspMat);
	else
		IRIT_GEN_COPY(CrntViewMat, IPViewMat, sizeof(IrtHmgnMatType));

	/* Here some useful parameters to play with in tesselating freeforms: */
	//CGSkelFFCState.FineNess = 20;   /* Res. of tesselation, larger is finer. */
	CGSkelFFCState.ComputeUV = TRUE;   /* Wants UV coordinates for textures. */
	CGSkelFFCState.FourPerFlat = TRUE;/* 4 poly per ~flat patch, 2 otherwise.*/
	CGSkelFFCState.LinearOnePolyFlag = TRUE;    /* Linear srf gen. one poly. */

	/* Traverse ALL the parsed data, recursively. */
	IPTraverseObjListHierarchy(PObjects, CrntViewMat, 
		CGSkelDumpOneTraversedObject);
	return true;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Call back function of IPTraverseObjListHierarchy. Called on every non    *
* list object found in hierarchy.                                            *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:       Non list object to handle.                                   *
*   Mat:        Transformation matrix to apply to this object.               *
*                                                                            *
* RETURN VALUE:                                                              *
*   void									                                 *
*****************************************************************************/
void CGSkelDumpOneTraversedObject(IPObjectStruct *PObj, IrtHmgnMatType Mat)
{
	IPObjectStruct *PObjs;

	if (IP_IS_FFGEOM_OBJ(PObj))
		PObjs = IPConvertFreeForm(PObj, &CGSkelFFCState);
	else
		PObjs = PObj;

	for (PObj = PObjs; PObj != NULL; PObj = PObj -> Pnext)
		if (!CGSkelStoreData(PObj)) 
			exit(1);
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Prints the data from given geometry object.								 *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:       Object to print.                                             *
*   Indent:     Column of indentation.                                       *
*                                                                            *
* RETURN VALUE:                                                              *
*   bool:		false - fail, true - success.                                *
*****************************************************************************/
std::vector<WingedEdgeMesh> OBJECTS;
bool CGSkelStoreData(IPObjectStruct *PObj)
{
	int i;
	const char *Str;
	double color[3], Transp;
	IPPolygonStruct *PPolygon;
	IPVertexStruct *PVertex, *PVertexPrev, *PVertexNext;
	IPAttributeStruct *Attrs = AttrTraceAttributes(PObj -> Attr, PObj -> Attr);
	WingedEdgeMesh myMesh;
	VertexMap verticesMap;
	EdgeMap edgesMap;

	log_debug("in CGSkelStoreData...\n");

	if (PObj -> ObjType != IP_OBJ_POLY) {
		AfxMessageBox("Non polygonal object detected and ignored");
		return true;
	}

	/* You can use IP_IS_POLYGON_OBJ(PObj) and IP_IS_POINTLIST_OBJ(PObj) 
	   to identify the type of the object*/

	if (CGSkelGetObjectColor(PObj, color))
	{
		/* color code */
		myMesh.setColor(RGB(255*color[0], 255*color[1], 255*color[2]));
	}
	if (CGSkelGetObjectTransp(PObj, &Transp))
	{
		/* transparency code */
	}
	if ((Str = CGSkelGetObjectTexture(PObj)) != NULL)
	{
		/* volumetric texture code */
	}
	if ((Str = CGSkelGetObjectPTexture(PObj)) != NULL)
	{
		/* parametric texture code */
	}
	if (Attrs != NULL) 
	{
		while (Attrs) {
			/* attributes code */
			Attrs = AttrTraceAttributes(Attrs, NULL);
		}
	}
	// Initialize mesh.
	for (PPolygon = PObj -> U.Pl; PPolygon != NULL;	PPolygon = PPolygon -> Pnext) 
	{
		if (PPolygon -> PVertex == NULL) {
			AfxMessageBox("Dump: Attemp to dump empty polygon");
			return false;
		}

		/* Count number of vertices (in polygon). */
		for (PVertex = PPolygon -> PVertex -> Pnext, i = 1;
			PVertex != PPolygon -> PVertex && PVertex != NULL;
			PVertex = PVertex -> Pnext, i++);
		/* use if(IP_HAS_PLANE_POLY(PPolygon)) to know whether a normal is defined for the polygon
		access the normal by the first 3 components of PPolygon->Plane */
		
		// We loop over the polygon with 2 pointers, one points at the previous vertex, the other at the current one.
		// Unfortunately, there are a lot of first step cases to handle before the loop starts.
		PVertexPrev = PPolygon->PVertex;
		if (PVertexPrev && PVertexPrev->Pnext)
			PVertex = PVertexPrev->Pnext;
		else
			log_warning("Polygon has only one vertex\n");
		if (PVertex && PVertex->Pnext)
			PVertexNext = PVertex->Pnext;
		else
			log_warning("Polygon has only two vertiecs.\n");
		

		// Check if vertex exists in map.
		Vertex *PMyPrevVertex = getVertexFromMap(verticesMap,
			Vertex(PVertexPrev->Coord[0], PVertexPrev->Coord[1], PVertexPrev->Coord[2]));
		if (PMyPrevVertex == NULL) { // If not, create vertex and add it to the hash map and to the mesh.
			PMyPrevVertex = new Vertex(PVertexPrev->Coord[0], PVertexPrev->Coord[1], PVertexPrev->Coord[2]);
			insertVertexToMap(verticesMap, PMyPrevVertex);
			myMesh.addVertex(PMyPrevVertex);
			if (IP_HAS_NORMAL_VRTX(PVertexPrev)) {
				PMyPrevVertex->setNormal(
					PVertexPrev->Normal[0], 
					PVertexPrev->Normal[1], 
					PVertexPrev->Normal[2]);
			}
		}
	
		Vertex *PFirstVertex = PMyPrevVertex;
		Vertex *PMyVertex;

		Edge *PMyEdge;

		Face *PMyFace = new Face();
		myMesh.addFace(PMyFace);
		PMyFace->addVertex(PMyPrevVertex);
		PMyPrevVertex->addFace(PMyFace);

		// Finally, the loop.
		do { /* Assume at least one edge in polygon! */
		 	 /* code handeling all vertex/normal/texture coords */
			 /* use if(IP_HAS_NORMAL_VRTX(PVertex)) to know whether a normal is defined for the vertex
			    access the vertex coords by PVertex->Coord
			    access the vertex normal by PVertex->Normal */
						
			// Check if vertex exists in vertices hash map.
			PMyVertex = getVertexFromMap(verticesMap, 
				Vertex(PVertex->Coord[0], PVertex->Coord[1], PVertex->Coord[2]));
			if (PMyVertex == NULL) { // If not, create vertex and add it to the hash map.
				PMyVertex = new Vertex(PVertex->Coord[0], PVertex->Coord[1], PVertex->Coord[2]);
				insertVertexToMap(verticesMap, PMyVertex);
				myMesh.addVertex(PMyVertex);
				// Set vertex normal. Later on we approximate the normal if it was not given in the file.
				if (IP_HAS_NORMAL_VRTX(PVertex)) { // TODO I don't think this recognizes that there is no vertex normal defined.
					PMyVertex->setNormal(PVertex->Normal[0], PVertex->Normal[1], PVertex->Normal[2]);
					log_debug("PVertex normal : %f %f %f\n", PVertex->Normal[0], PVertex->Normal[1], PVertex->Normal[2]);
				}
			}

			// Check if edge exists in edges hash map.
			PMyEdge = getEdgeFromMap(edgesMap, Edge(PMyPrevVertex, PMyVertex));
			if (PMyEdge == NULL) {
				PMyEdge = getEdgeFromMap(edgesMap, Edge(PMyVertex, PMyPrevVertex));
				if (PMyEdge == NULL) {
					PMyEdge = new Edge(PMyPrevVertex, PMyVertex);
					insertEdgeToMap(edgesMap, PMyEdge);
					myMesh.addEdge(PMyEdge);
				}
			}

			// Update adjacency information.
			if (PMyEdge->getF1() == NULL) {
				PMyEdge->setF1(PMyFace);
			}
			else if (PMyEdge->getF2() == NULL) {
				PMyEdge->setF2(PMyFace);
			}
			PMyFace->addVertex(PMyVertex);
			PMyFace->addEdge(PMyEdge);
			PMyVertex->addFace(PMyFace);

			// Move on to next vertex.
			PVertexPrev = PVertex;
			PVertex = PVertex -> Pnext;
			PMyPrevVertex = PMyVertex;
		} while (PVertex != PPolygon -> PVertex && PVertex != NULL);
		// Last edge to close the polygon.
		PMyEdge = getEdgeFromMap(edgesMap, Edge(PMyPrevVertex, PFirstVertex));
		if (PMyEdge == NULL) {
			PMyEdge = getEdgeFromMap(edgesMap, Edge(PFirstVertex, PMyPrevVertex));
			if (PMyEdge == NULL) {
				PMyEdge = new Edge(PMyPrevVertex, PFirstVertex);
				insertEdgeToMap(edgesMap, PMyEdge);
				myMesh.addEdge(PMyEdge);
			}
		}

		if (PMyEdge->getF1() == NULL) {
			PMyEdge->setF1(PMyFace);
		}
		else if (PMyEdge->getF2() == NULL) {
			PMyEdge->setF2(PMyFace);
		}

		PMyFace->addEdge(PMyEdge);
		
		// Get face normal.
		if (IP_HAS_PLANE_POLY(PPolygon)) {
			PMyFace->setNormal(PPolygon->Plane[0], PPolygon->Plane[1], PPolygon->Plane[2], -PPolygon->Plane[3]);
		}
		else { // Calculate it if it doesn't exist.
			PMyFace->calcPlane();
		}

		/* Close the polygon. */
	}

	// Approximate vertices normals.
	// TODO add menu option for this
	std::vector<Vertex*> VL = myMesh.getVertexList();
	for (std::vector<Vertex*>::iterator v = VL.begin(); v != VL.end(); ++v) {
		if (!(*v)->hasNormal()) {
			log_debug("Approximating vertex normal...\n");
			(*v)->approximateNormal();
		}
	}

	OBJECTS.push_back(myMesh);
	
	/* Close the object. */

	return true;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the color of an object.                                          *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its color.                                         *
*   RGB:    as 3 floats in the domain [0, 1].                                *
*                                                                            *
* RETURN VALUE:                                                              *
*   int:    TRUE if object has color, FALSE otherwise.                       *
*****************************************************************************/
int CGSkelGetObjectColor(IPObjectStruct *PObj, double RGB[3])
{
	static int TransColorTable[][4] = {
		{ /* BLACK	*/   0,    0,   0,   0 },
		{ /* BLUE	*/   1,    0,   0, 255 },
		{ /* GREEN	*/   2,    0, 255,   0 },
		{ /* CYAN	*/   3,    0, 255, 255 },
		{ /* RED	*/   4,  255,   0,   0 },
		{ /* MAGENTA 	*/   5,  255,   0, 255 },
		{ /* BROWN	*/   6,   50,   0,   0 },
		{ /* LIGHTGRAY	*/   7,  127, 127, 127 },
		{ /* DARKGRAY	*/   8,   63,  63,  63 },
		{ /* LIGHTBLUE	*/   9,    0,   0, 255 },
		{ /* LIGHTGREEN	*/   10,   0, 255,   0 },
		{ /* LIGHTCYAN	*/   11,   0, 255, 255 },
		{ /* LIGHTRED	*/   12, 255,   0,   0 },
		{ /* LIGHTMAGENTA */ 13, 255,   0, 255 },
		{ /* YELLOW	*/   14, 255, 255,   0 },
		{ /* WHITE	*/   15, 255, 255, 255 },
		{ /* BROWN	*/   20,  50,   0,   0 },
		{ /* DARKGRAY	*/   56,  63,  63,  63 },
		{ /* LIGHTBLUE	*/   57,   0,   0, 255 },
		{ /* LIGHTGREEN	*/   58,   0, 255,   0 },
		{ /* LIGHTCYAN	*/   59,   0, 255, 255 },
		{ /* LIGHTRED	*/   60, 255,   0,   0 },
		{ /* LIGHTMAGENTA */ 61, 255,   0, 255 },
		{ /* YELLOW	*/   62, 255, 255,   0 },
		{ /* WHITE	*/   63, 255, 255, 255 },
		{		     -1,   0,   0,   0 }
	};
	int i, j, Color, RGBIColor[3];

	if (AttrGetObjectRGBColor(PObj,
		&RGBIColor[0], &RGBIColor[1], &RGBIColor[2])) {
			for (i = 0; i < 3; i++)
				RGB[i] = RGBIColor[i] / 255.0;

			return TRUE;
	}
	else if ((Color = AttrGetObjectColor(PObj)) != IP_ATTR_NO_COLOR) {
		for (i = 0; TransColorTable[i][0] >= 0; i++) {
			if (TransColorTable[i][0] == Color) {
				for (j = 0; j < 3; j++)
					RGB[j] = TransColorTable[i][j+1] / 255.0;
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the volumetric texture of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its volumetric texture.                            *
*                                                                            *
* RETURN VALUE:                                                              *
*   char *:    Name of volumetric texture map to apply, NULL if none.        *
*****************************************************************************/
const char *CGSkelGetObjectTexture(IPObjectStruct *PObj)
{
	return AttrGetObjectStrAttrib(PObj, "texture");
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the parametric texture of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its parametric texture.                            *
*                                                                            *
* RETURN VALUE:                                                              *
*   char *:    Name of parametric texture map to apply, NULL if none.        *
*****************************************************************************/
const char *CGSkelGetObjectPTexture(IPObjectStruct *PObj)
{
	return AttrGetObjectStrAttrib(PObj, "ptexture");
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the transparency level of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its volumetric texture.                            *
*   Transp: Transparency level between zero and one.                         *
*                                                                            *
* RETURN VALUE:                                                              *
*   int:    TRUE if object has transparency, FALSE otherwise.                *
*****************************************************************************/
int CGSkelGetObjectTransp(IPObjectStruct *PObj, double *Transp)
{
	*Transp = AttrGetObjectRealAttrib(PObj, "transp");

	return !IP_ATTR_IS_BAD_REAL(*Transp);
}

