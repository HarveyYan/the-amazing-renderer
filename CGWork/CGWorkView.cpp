// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include <algorithm>
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;

// Dialogues
#include "MaterialDlg.h"
#include "LightDialog.h"
#include "CameraDlg.h"
#include "MouseSensitivityDlg.h"
#include "DialogChangeColor.h"
#include "DialogFineness.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"

#include "myHelpers.h"
#include "Edge.h"
#include "Face.h"
#include "Vertex.h"
#include "WingedEdgeMesh.h"
#include "Matrix4d.h"
#include "Vector4d.h"

// For Status Bar access
#include "MainFrm.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_AXIS_XY, OnAxisXY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_XY, OnUpdateAxisXY)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	// Mouse events
	ON_WM_LBUTTONDOWN()
	//ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_OBJECT_SPACE, OnObjectSpace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT_SPACE, OnUpdateObjectSpace)
	ON_COMMAND(ID_VIEW_VIEW_SPACE, OnViewSpace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEW_SPACE, OnUpdateViewSpace)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)
	ON_COMMAND(ID_VIEW_SHOWPOLY, &CCGWorkView::OnViewShowpoly)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWPOLY, &CCGWorkView::OnUpdateViewShowpoly)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_OPTIONS_SHOWBOUNDINGBOX, &CCGWorkView::OnOptionsShowboundingbox)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWBOUNDINGBOX, &CCGWorkView::OnUpdateOptionsShowboundingbox)
	ON_COMMAND(ID_OPTIONS_CHANGECOLOR, &CCGWorkView::OnOptionsChangecolor)
	ON_COMMAND(ID_OPTIONS_FINENESSCONTROL, &CCGWorkView::OnOptionsFinenesscontrol)
	ON_COMMAND(ID_OPTIONS_SHOWVERTEXNORMALS, &CCGWorkView::OnOptionsShowvertexnormals)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWVERTEXNORMALS, &CCGWorkView::OnUpdateOptionsShowvertexnormals)
	ON_COMMAND(ID_ACTION_ALLOBJECTS, &CCGWorkView::OnActionAllobjects)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ALLOBJECTS, &CCGWorkView::OnUpdateActionAllobjects)
	ON_COMMAND(ID_OPTIONS_HIGHLIGHTFACE, &CCGWorkView::OnOptionsHighlightface)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_HIGHLIGHTFACE, &CCGWorkView::OnUpdateOptionsHighlightface)
	ON_COMMAND(ID_RENDERING_WIREFRAME, &CCGWorkView::OnRenderingWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_WIREFRAME, &CCGWorkView::OnUpdateRenderingWireframe)
	ON_COMMAND(ID_RENDERING_RENDER, &CCGWorkView::OnRenderingRender)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_RENDER, &CCGWorkView::OnUpdateRenderingRender)
	ON_COMMAND(ID_RENDERING_BACKFACECULLING, &CCGWorkView::OnRenderingBackfaceculling)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_BACKFACECULLING, &CCGWorkView::OnUpdateRenderingBackfaceculling)
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// Constants

// works with ortho
static const double CAMERA_WIDTH = 10;
static const double CAMERA_HEIGHT = 10;
//static const double CAMERA_X = 0;
//static const double CAMERA_Y = 0;
static const double CAMERA_NEAR = 1;
static const double CAMERA_FAR = 15;

static const Vector4d CAMERA_P(0, 0, 10);
static const Vector4d CAMERA_LOOK(0, 0, -1);
static const Vector4d CAMERA_UP(0, 1, 0);
static const double CAMERA_FOVY = 60;
//static const double PCAMERA_NEAR = 1;
//static const double PCAMERA_FAR = 2;

//static const double CAMERA_WIDTH = 10;
//static const double CAMERA_HEIGHT = 10;
//static const double CAMERA_X = 0;
//static const double CAMERA_Y = 0;
//static const double CAMERA_NEAR = 1;
//static const double CAMERA_FAR = 15;
//
//static const Vector4d CAMERA_P(0, 0, -9);
//static const Vector4d CAMERA_LOOK(0, 0, -1);
//static const Vector4d CAMERA_UP(0, 1, 0);
//static const double CAMERA_FOVY = 60;
//static const double PCAMERA_NEAR = 1;
//static const double PCAMERA_FAR = 2;

static const double START_SCALE = 0.5;

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;	
	m_bIsPerspective = false;
	m_tObjectSpace = true;
	m_showPolyNormals = false;
	m_showVertexNormals = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled=true;

	m_lMouseDown = false;
	m_scaleSens = m_mouseDlg.getScaleSens();
	m_rotateSens = m_mouseDlg.getRotateSens();
	m_translateSens = m_mouseDlg.getTranslateSens();

	m_transMat = ID_MAT;
	m_scaleMat = ID_MAT;
	m_rotateMat = ID_MAT;
	m_modelMat = ID_MAT;
	m_viewMat = ID_MAT;
	m_screenMat = ID_MAT;
	m_screenMatInv = ID_MAT;
	m_transformAll = true;

	m_sFactor = 1.0;

	// calculate PAR
	CDC *screen = GetDC();
	m_hScreenSize = GetDeviceCaps(*screen, HORZSIZE); // in mm
	m_vScreenSize = GetDeviceCaps(*screen, VERTSIZE); // in mm
	m_hScreenRes = GetDeviceCaps(*screen, HORZRES); // in pixels
	m_vScreenRes = GetDeviceCaps(*screen, VERTRES); // in pixels
	m_PAR = ((double)m_hScreenSize / m_vScreenSize) * ((double)m_vScreenRes / m_hScreenRes);

	m_cameraWidth = CAMERA_WIDTH;
	m_cameraHeight = CAMERA_HEIGHT;
	m_cameraP = CAMERA_P;
	m_cameraLook = CAMERA_LOOK;
	m_cameraUp = CAMERA_UP;
	m_cameraFov = CAMERA_FOVY;
	m_cameraNear = CAMERA_NEAR;
	m_cameraFar = CAMERA_FAR;

	m_activeObjecti = 0;
	m_showBoundingBox = false;

	m_bgColor = RGB(255, 255, 255);

	m_pHighlightedFace = NULL;
	m_highlighFace = false;

	m_bRender = false;
	m_bWireframe = true;
	m_bBackfaceCulling = false;

	m_bSilhHighligh = true;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	/*CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(GetDC());
	m_pDbBitMap = CreateCompatibleBitmap(m_pDbDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);*/

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox("Couldn't get a valid DC.");
		return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	SetupViewingFrustum();
	SetupViewingOrthoConstAspect();

	m_screenMat =
		translate(m_WindowWidth / 2.0, m_WindowHeight / 2.0, 0) *
		scale(m_WindowWidth / (2.0), -m_WindowHeight / (2.0), 1);

	m_screenMatInv =
		scaleInv(m_WindowWidth / (2.0), -m_WindowHeight / (2.0), 1) *
		translateInv(m_WindowWidth / 2.0, m_WindowHeight / 2.0, 0);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{	
	m_perspCamera.set(
		m_cameraP,
		m_cameraLook,
		m_cameraUp,
		m_cameraFov,
		m_AspectRatio,
		m_cameraNear,
		m_cameraFar);
	if (m_bIsPerspective) {
		m_viewMat = m_perspCamera.get();
	}
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{

	double hw = m_cameraWidth * 0.5;
	if (m_AspectRatio >= 1) {
		m_orthoCamera.set(
			m_cameraP.getX() - hw*m_AspectRatio,
			m_cameraP.getX() + hw*m_AspectRatio,
			m_cameraP.getY() + hw,
			m_cameraP.getY() - hw,
			m_cameraP.getZ() - m_cameraNear, 
			m_cameraP.getZ() - m_cameraFar);
	}
	else {
		m_orthoCamera.set(
			m_cameraP.getX() - hw,
			m_cameraP.getX() + hw,
			m_cameraP.getY() + hw / m_AspectRatio,
			m_cameraP.getY() - hw / m_AspectRatio,
			m_cameraP.getZ() - m_cameraNear,
			m_cameraP.getZ() - m_cameraFar);
	}
	if (!m_bIsPerspective) {
		m_viewMat = m_orthoCamera.get();
	}
	return TRUE;
}





BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.

	// return CView::OnEraseBkgnd(pDC);
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////



void CCGWorkView::OnDraw(CDC* pDC)
{	
	// TODO add button to clear all objects (reset)
	// TODO handle face winding order (clockwise or anti-clockwise)
	// TODO reverse normals option
	// TODO add menu option to change silhoette highligh color

	// TODO return the onsize
	SetupViewingFrustum();
	SetupViewingOrthoConstAspect();

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = 0;

	CRect rect;
	GetClientRect(&rect);

	// Try to set up double buffer. If fails, draw straight to the screen (slow and causes flickering).
	if (!pDC->IsPrinting())
	{
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;
				pOldBitmap = dc.SelectObject(&bitmap);
			}
		}
	}

	pDrawDC->FillSolidRect(&rect, m_bgColor);

	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	    return;

	Matrix4d transMat;
	/*if (m_tObjectSpace) {
		transMat = m_viewMat * m_modelMat;
	}
	else {
		transMat = m_modelMat * m_viewMat;
	}*/

	// Draw highlighted face.
	/*if (m_pHighlightedFace && m_highlighFace) {
		draw(pDrawDC, *m_pHighlightedFace, transMat, m_screenMat, RGB(0, 0, 0));
		paint_hack(pDrawDC, m_WindowWidth, m_WindowHeight, RGB(255, 0, 0), m_bgColor);
	}*/

	// Draw all meshes.
	log_debug("VIEW MAT:\n");
	log_debug_matrix(m_viewMat);
	log_debug("MODEL MAT:\n");
	log_debug_matrix(m_modelMat);
	for (WingedEdgeMesh & obj : OBJECTS)
	{
		if (m_tObjectSpace) {
			if (m_bIsPerspective) {
				transMat = m_perspCamera.normCubeMat * m_perspCamera.cameraMat * m_modelMat * obj.m_modelMat;
			}
			else {
				transMat = m_orthoCamera.normCubeMat * m_orthoCamera.cameraMat * m_modelMat * obj.m_modelMat;
			}
		}
		else {
			if (m_bIsPerspective) {
				transMat = m_perspCamera.normCubeMat * m_modelMat * obj.m_modelMat * m_perspCamera.cameraMat;
			}
			else {
				transMat = m_orthoCamera.normCubeMat * m_modelMat * obj.m_modelMat * m_orthoCamera.cameraMat;
			}
		}

		WingedEdgeMesh tmp_wem(obj);

		tmp_wem.calcBackFaceCulling(m_cameraP, m_modelMat * obj.m_modelMat, m_bIsPerspective);
	
		tmp_wem.transform(transMat);
		tmp_wem.homegenize();
		tmp_wem.transform(m_screenMat);

		if (m_bWireframe) {
			drawMesh(pDrawDC, tmp_wem, false, m_bBackfaceCulling);
		}
		if (m_bRender) {
			drawMesh(pDrawDC, tmp_wem, true, m_bBackfaceCulling);
		}

		drawAxis(pDrawDC, transMat, m_screenMat);

		if (m_showPolyNormals) {
			for (Face * f : tmp_wem.getFaceList()) {
				f->drawNormal(pDrawDC, obj.getNormalColor());
			}
		}

		if (m_showBoundingBox) {
			drawBoundingBox(pDrawDC, tmp_wem, transMat, m_screenMat);
		}

		if (m_showVertexNormals) {
			for (Vertex * v : tmp_wem.getVertexList()) {
				v->drawNormal(pDrawDC, obj.getNormalColor());
			}
		}

		if (m_bSilhHighligh) {
			tmp_wem.highLightSilh(pDrawDC);
		}
	}

	if (pDrawDC != pDC) {
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, rect.left, rect.top, SRCCOPY);
	}

	log_debug("done\n");
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy() 
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad() 
{
	static int lastAdded = -1;

	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, "itd", "*.itd", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		log_info("loading file %s...\n", m_strItdFileName);
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		log_info("Done.\n");
		// Open the file and read it.

		if (lastAdded == -1) {
			m_activeObjectItr = OBJECTS.begin();
		}

		double BBWidth, BBHeight, BBDepth;
		++lastAdded;
		BBSize(std::vector<WingedEdgeMesh>(OBJECTS.begin() + lastAdded, OBJECTS.end()), BBWidth, BBHeight, BBDepth);
		double minLen = min(
			min(m_orthoCamera.getWidth(), m_orthoCamera.getHeight()), 
			min(m_perspCamera.getWidth(), m_perspCamera.getHeight()));
		log_info("min len = %f\n", minLen);
		log_info("persp = %f %f %f\n", m_perspCamera.getWidth(), m_perspCamera.getHeight(), m_perspCamera.getDepth());
		log_info("ortho = %f %f %f\n", m_orthoCamera.getWidth(), m_orthoCamera.getHeight(), m_orthoCamera.getDepth());
		log_info("m_cameraWidth = %f\n", m_cameraWidth);
		m_sFactor = START_SCALE * 
			min(m_orthoCamera.getWidth(), m_orthoCamera.getHeight()) / 
			max(max(BBWidth, BBHeight), BBDepth);
		for (unsigned int i = lastAdded; i < OBJECTS.size(); ++i) {
			OBJECTS[i].transform(scale(m_sFactor, m_sFactor, m_sFactor));
			calculateBoundingBox(OBJECTS[i]);
		}

		lastAdded = OBJECTS.size() - 1;

		Invalidate();	// force a WM_PAINT for drawing.
	} 

}





// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic() 
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_viewMat = m_orthoCamera.get();
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective() 
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	m_viewMat = m_perspCamera.get();
	Invalidate(); // redraw using the new view.
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate() 
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate() 
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale() 
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX() 
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}

void CCGWorkView::OnAxisY() 
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}

void CCGWorkView::OnAxisZ() 
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}

void CCGWorkView::OnAxisXY() {
	m_nAxis = ID_AXIS_XY;
}

void CCGWorkView::OnUpdateAxisXY(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_XY);
}


/********************/
/* Option handlers. */
/********************/



// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat() 
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}

void CCGWorkView::OnLightShadingGouraud() 
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants() 
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{	    
	    dlg.SetDialogData((LightID)id,m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT,m_ambientLight);

	if (dlg.DoModal() == IDOK) 
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
		m_lights[id] = dlg.GetDialogData((LightID)id);
	    }
	    m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}	
	Invalidate();
}



/*************************/
/* Mouse event handlers. */
/*************************/

afx_msg void CCGWorkView::OnLButtonDown(UINT flags, CPoint XY) {
	m_lMouseDown = true;
	m_mouseX = XY.x;
	m_mouseY = XY.y;

	Matrix4d transMat;
	if (m_tObjectSpace) {
		transMat = m_viewMat * m_modelMat;
	}
	else {
		transMat = m_modelMat * m_viewMat;
	}

	double closest = (std::numeric_limits<double>::max)();
	for (std::vector<WingedEdgeMesh>::iterator obj = OBJECTS.begin(); obj != OBJECTS.end(); ++obj) {
		calculateBoundingBox(*obj);
		Vector4d Qmin_screen = obj->getQmin();
		Vector4d Qmax_screen = obj->getQmax();
		Qmin_screen = transMat * obj->m_modelMat * obj->getQmin();
		Qmax_screen = transMat * obj->m_modelMat * obj->getQmax();
		Qmin_screen.homegenize();
		Qmax_screen.homegenize();
		Qmin_screen = m_screenMat * Qmin_screen;
		Qmax_screen = m_screenMat * Qmax_screen;
		double Qz = min(Qmin_screen.getZ(), Qmax_screen.getZ());

		if (Qmin_screen.getX() < m_mouseX && Qmax_screen.getX() > m_mouseX &&
			Qmin_screen.getY() > m_mouseY && Qmax_screen.getY() < m_mouseY &&
			Qz < closest) {
			closest = Qz;
			m_activeObjecti = OBJECTS.end() - obj - 1;
			m_activeObjectItr = obj;
		}
	}
}

afx_msg void CCGWorkView::OnRButtonDblClk(UINT flags, CPoint XY) {
	if (!m_highlighFace) {
		return;
	}

	Matrix4d transMat;
	if (m_tObjectSpace) {
		transMat = m_viewMat * m_modelMat;
	}
	else {
		transMat = m_modelMat * m_viewMat;
	}

	// face highlighting
	for (std::vector<WingedEdgeMesh>::iterator obj = OBJECTS.begin(); obj != OBJECTS.end(); ++obj) {
		// calculate mouse coordinate in normalized view space
		Vector4d mouseViewP1 = m_screenMatInv * Vector4d(XY.x, XY.y, m_cameraP.getZ());
		Vector4d mouseViewP2 = mouseViewP1; mouseViewP2.setZ(m_cameraP.getZ() - 1);

		// iterate over all faces and test if the ray passing through the two mouse points passes through the polygon.
		std::vector<Face*> FL = obj->getFaceList();
		double cz = std::numeric_limits<double>::lowest();
		for (std::vector<Face*>::const_iterator f = FL.begin(); f != FL.end(); ++f) {
			std::vector<Vertex*> faceVertices = (*f)->getVertices();
			std::vector<Vertex> faceVerticesCopy;
			for (std::vector<Vertex*>::const_iterator v = faceVertices.begin(); v != faceVertices.end(); ++v) {
				Vertex vCopy = **v;
				vCopy.transform(transMat);
				faceVerticesCopy.push_back(vCopy);
			}
			Vector4d intersectionPoint;
			if (faceVerticesCopy.size() == 3 &&
				rayTriangleIntersection(
				mouseViewP1,
				mouseViewP2,
				faceVerticesCopy[0].getCoord(),
				faceVerticesCopy[1].getCoord(),
				faceVerticesCopy[2].getCoord(),
				&intersectionPoint))
			{
				//if (dot_product((*f)->getNormal(), mouseViewP2 - mouseViewP1) > 0) {
				if (intersectionPoint.getZ() > cz) {
					cz = closestZ(**f);
					m_pHighlightedFace = *f;
				}
			}
			if (faceVerticesCopy.size() == 4 &&
				rayRectIntersection(
				mouseViewP1,
				mouseViewP2,
				faceVerticesCopy[0].getCoord(),
				faceVerticesCopy[1].getCoord(),
				faceVerticesCopy[2].getCoord(),
				faceVerticesCopy[3].getCoord(),
				&intersectionPoint))
			{
				//if (dot_product((*f)->getNormal(), mouseViewP2 - mouseViewP1) > 0) {
				if (intersectionPoint.getZ() > cz) {
					cz = closestZ(**f);
					m_pHighlightedFace = *f;
				}
			}
		}
	}
	Invalidate();
}

afx_msg void CCGWorkView::OnLButtonUp(UINT flags, CPoint XY) {
	m_lMouseDown = false;
}

afx_msg void CCGWorkView::OnMouseMove(UINT flags, CPoint XY) {
	if (!m_lMouseDown) {
		return;
	}

	// The amount of movment/scaling/rotation in each axis.
	double dx = 0, dy = 0, dz = 0;
	// Moving the mouse from one edge of the screen to the other should cause a movement of 2 in the normalized cube,
	// which should case a movement of camera width in the world. So the world change is in porpotion to "camera size /
	// window size."
	double cw = m_cameraWidth * m_AspectRatio; // camera width
	double ch = m_cameraHeight;
	double worldDx = (double)cw * (m_mouseX - XY.x) / (m_WindowWidth);
	double worldDy = (double)ch * (m_mouseY - XY.y) / (m_WindowHeight);
	switch (m_nAxis) {
	case ID_AXIS_X:
		dx = -worldDx;
		break;
	case ID_AXIS_Y:
		dy = worldDy;
		break;
	case ID_AXIS_Z:
		dz = -worldDx;
		break;
	case ID_AXIS_XY:
		dx = -worldDx;
		dy = worldDy;
		break;
	default:
		log_warning("No known rotation axis: m_nAxis = %d\n", m_nAxis);
	}

	// We use dx/dy/dz to decide how much to scale/rotate/translate in each axis (depending on the mode).
	switch (m_nAction) {
	case ID_ACTION_SCALE:
		// A mouse movement from one edge to the other scales by ~2 (depends on sensitivity).
		if (m_transformAll) {
			m_modelMat *= scale(
				(1 - m_scaleSens * dx / cw),
				(1 - m_scaleSens * dy / cw),
				(1 - m_scaleSens * dz / cw));
		}
		else {
			m_activeObjectItr->m_modelMat *= scale(
				(1 - m_scaleSens * dx / cw),
				(1 - m_scaleSens * dy / cw),
				(1 - m_scaleSens * dz / cw));
		}
		
		break;
	case ID_ACTION_ROTATE:
		// A mouse movment from one edge to other rotates by ~2*PI (full cycle).
		if (m_transformAll) {
			m_modelMat *= rotate(
				m_rotateSens*(2 * dx * M_PI / cw),
				m_rotateSens*(2 * dy * M_PI / cw),
				m_rotateSens*(2 * dz * M_PI / cw));
		}
		else {
			m_activeObjectItr->m_modelMat *= rotate(
				m_rotateSens*(2 * dx * M_PI / cw),
				m_rotateSens*(2 * dy * M_PI / cw),
				m_rotateSens*(2 * dz * M_PI / cw));
		}

		break;
	case ID_ACTION_TRANSLATE:
		// Translation is in porportion to the mouse movement.
		if (m_transformAll) {
			m_modelMat *= translate(m_translateSens * dx, m_translateSens * dy, m_translateSens * dz);
		}
		else {
			m_activeObjectItr->m_modelMat *= translate(m_translateSens * dx, m_translateSens * dy, m_translateSens * dz);
		}
		break;
	default:
		log_warning("Not a known action: m_nAction = %d\n", m_nAction);
	}

	m_mouseX = XY.x;
	m_mouseY = XY.y;

	Invalidate();
}

/*************************/
/* Other event handlers. */
/*************************/

void CCGWorkView::OnObjectSpace()
{
	m_tObjectSpace = true;
	Invalidate();
}

void CCGWorkView::OnUpdateObjectSpace(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_tObjectSpace);
}

void CCGWorkView::OnViewSpace()
{
	m_tObjectSpace = false;
	Invalidate();
}

void CCGWorkView::OnUpdateViewSpace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_tObjectSpace);
}

void CCGWorkView::OnOptionsMousesensitivity()
{
	if (m_mouseDlg.DoModal() == IDOK) {
		m_translateSens = m_mouseDlg.getTranslateSens();
		m_rotateSens = m_mouseDlg.getRotateSens();
		m_scaleSens = m_mouseDlg.getScaleSens();
	}
}

void CCGWorkView::OnViewShowpoly()
{
	m_showPolyNormals = !m_showPolyNormals;
	Invalidate();
}

void CCGWorkView::OnUpdateViewShowpoly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_showPolyNormals);
}

void CCGWorkView::OnOptionsPerspectivecontrol()
{
	CameraDlg dlg(NULL, m_cameraNear, m_cameraFar, m_cameraFov, m_cameraP.getX(), m_cameraP.getY(), m_cameraP.getZ());
	if (dlg.DoModal() == IDOK) {
		m_cameraNear = dlg.m_near;
		m_cameraFar = dlg.m_far;
		m_cameraFov = dlg.m_fov;
		m_cameraP = Vector4d(dlg.m_x, dlg.m_y, dlg.m_z);
		SetupViewingFrustum();
		Invalidate();
	}
}

void CCGWorkView::OnOptionsShowboundingbox()
{
	m_showBoundingBox = !m_showBoundingBox;
	Invalidate();
}

void CCGWorkView::OnUpdateOptionsShowboundingbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_showBoundingBox);
}

void CCGWorkView::OnOptionsChangecolor()
{
	DialogChangeColor dlg;
	if (dlg.DoModal() == IDOK) {
		m_bgColor = dlg.m_bgColor;
		Invalidate();
	}
}

void CCGWorkView::OnOptionsFinenesscontrol()
{
	DialogFineness dlg;
	dlg.Fineness = CGSkelFFCState.FineNess;
	if (dlg.DoModal() == IDOK) {
		CGSkelFFCState.FineNess = dlg.Fineness;
		Invalidate();
	}
}

void CCGWorkView::OnOptionsShowvertexnormals()
{
	m_showVertexNormals = !m_showVertexNormals;
	Invalidate();
}

void CCGWorkView::OnUpdateOptionsShowvertexnormals(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_showVertexNormals);
}

void CCGWorkView::OnActionAllobjects()
{
	m_transformAll = !m_transformAll;
}

void CCGWorkView::OnUpdateActionAllobjects(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_transformAll);
}

void CCGWorkView::OnOptionsHighlightface()
{
	m_highlighFace = !m_highlighFace;
}


void CCGWorkView::OnUpdateOptionsHighlightface(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_highlighFace);
}

void CCGWorkView::OnRenderingWireframe()
{
	m_bWireframe = !m_bWireframe;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingWireframe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bWireframe);
}


void CCGWorkView::OnRenderingRender()
{
	m_bRender = !m_bRender;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingRender(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRender);
}

void CCGWorkView::OnRenderingBackfaceculling()
{
	m_bBackfaceCulling = !m_bBackfaceCulling;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingBackfaceculling(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBackfaceCulling);
}


/********************/
/* Drawing methods. */
/********************/

void CCGWorkView::calculateBoundingBox(WingedEdgeMesh & obj) {
	double x_min = (std::numeric_limits<double>::max)();
	double y_min = (std::numeric_limits<double>::max)();
	double z_min = (std::numeric_limits<double>::max)();
	double x_max = (std::numeric_limits<double>::min)();
	double y_max = (std::numeric_limits<double>::min)();
	double z_max = (std::numeric_limits<double>::min)();

	std::vector<Vertex*> vertex = obj.getVertexList();
	for (std::vector<Vertex*>::iterator ver = vertex.begin(); ver != vertex.end(); ++ver){
		Vertex tmp = **ver;
		x_min = min(x_min, tmp.getX());
		y_min = min(y_min, tmp.getY());
		z_min = min(z_min, tmp.getZ());
		x_max = max(x_max, tmp.getX());
		y_max = max(y_max, tmp.getY());
		z_max = max(z_max, tmp.getZ());
	}

	Vertex v1(x_min, y_min, z_min),
		v2(x_min, y_max, z_min),
		v3(x_max, y_max, z_min),
		v4(x_max, y_min, z_min),
		v5(x_max, y_min, z_max),
		v6(x_min, y_min, z_max),
		v7(x_min, y_max, z_max),
		v8(x_max, y_max, z_max);

	obj.setQmin(v1.getCoord());
	obj.setQmax(v8.getCoord());
}

void drawAxis(CDC* pDC, const Matrix4d & transMat, const Matrix4d & screenMat) {
	Vertex ax(1, 0, 0), ay(0, 1, 0), az(0, 0, -1), P(0,0,0);
	ax.transform(transMat); ay.transform(transMat); az.transform(transMat); P.transform(transMat);
	ax.homegenize(); ay.homegenize(); az.homegenize(); P.homegenize();
	ax.transform(screenMat); ay.transform(screenMat); az.transform(screenMat); P.transform(screenMat);

	draw(pDC, P, ax, RGB(255, 0, 0));
	draw(pDC, P, ay, RGB(0, 255, 0));
	draw(pDC, P, az, RGB(0, 0, 255));
}

