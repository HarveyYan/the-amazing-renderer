// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"
#include "Matrix4d.h"
#include "Vector4d.h"
#include "Camera.h"
#include "MouseSensitivityDlg.h"

class WingedEdgeMesh;
class Face;

class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:

private:
	int m_nAxis;					 // Axis of Action, X Y or Z
	int m_nAction;					 // Rotate, Translate, Scale
	int m_nView;					 // Orthographic, perspective
	bool m_bIsPerspective;			 // is the view perspective
	bool m_tObjectSpace;			 // transformations take place in object space
	bool m_showPolyNormals;
	bool m_showVertexNormals;
	
	CString m_strItdFileName;		 // file name of IRIT data

	int m_nLightShading;			 // shading: Flat, Gouraud.

	double m_lMaterialAmbient;		 // The Ambient in the scene
	double m_lMaterialDiffuse;		 // The Diffuse in the scene
	double m_lMaterialSpecular;		 // The Specular in the scene
	int m_nMaterialCosineFactor;	 // The cosine factor for the specular

	LightParams m_lights[MAX_LIGHT]; //configurable lights array
	LightParams m_ambientLight;		 //ambient light (only RGB is used)

	bool m_highlighFace;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// Mouse event handlers.
	virtual afx_msg void OnLButtonDown(UINT flags, CPoint XY);
	/*virtual afx_msg void OnRButtonDown(UINT flags, CPoint XY);*/
	//virtual afx_msg void OnRButtonDown(UINT flags, CPoint XY);
	virtual afx_msg void OnRButtonDblClk(UINT flags, CPoint XY);
	virtual afx_msg void CCGWorkView::OnLButtonUp(UINT flags, CPoint XY);
	virtual afx_msg void CCGWorkView::OnMouseMove(UINT flags, CPoint XY);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	int m_hScreenSize;      // whole screen horizontal in mm (not just the window)
	int m_vScreenSize;      // whole screen vertical in mm
	int m_hScreenRes;       // whole screen horizontal in pixels
	int m_vScreenRes;       // whole screen vertical in pixels
	double m_AspectRatio;	// hold the fixed Aspect Ratio
	double m_PAR;           // hold the pixel aspect ratio
	COLORREF m_bgColor;

	OrthoCamera m_orthoCamera;
	PerspCamera m_perspCamera;
	double m_cameraWidth;
	double m_cameraHeight;
	Vector4d m_cameraP;
	Vector4d m_cameraLook;
	Vector4d m_cameraUp;
	double m_cameraFov;
	double m_cameraNear;
	double m_cameraFar;

	Matrix4d m_transMat;
	Matrix4d m_scaleMat;
	Matrix4d m_rotateMat;
	Matrix4d m_modelMat;
	Matrix4d m_viewMat;
	Matrix4d m_screenMat;
	Matrix4d m_screenMatInv;
	bool m_transformAll;

	double m_sFactor;

	int m_mouseX;
	int m_mouseY;
	bool m_lMouseDown;
	MouseSensitivityDlg m_mouseDlg;
	double m_scaleSens;
	double m_rotateSens;
	double m_translateSens;
	int m_activeObjecti;
	std::vector<WingedEdgeMesh>::iterator m_activeObjectItr;
	Face *m_pHighlightedFace;

	bool m_bRender, m_bWireframe;
	bool m_bBackfaceCulling;

	bool m_bSilhHighligh;

	bool m_bApproxVertexNormals;
	bool m_bInverseNormals;

// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnAxisXY();
	afx_msg void OnUpdateAxisXY(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	afx_msg void OnObjectSpace();
	afx_msg void OnUpdateObjectSpace(CCmdUI* pCmdUI);
	afx_msg void OnViewSpace();
	afx_msg void OnUpdateViewSpace(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOptionsMousesensitivity();
	afx_msg void OnViewShowpoly();
	afx_msg void OnUpdateViewShowpoly(CCmdUI *pCmdUI);
	afx_msg void OnOptionsPerspectivecontrol();

	void calculateBoundingBox(WingedEdgeMesh & obj);
	bool m_showBoundingBox;
	afx_msg void OnOptionsShowboundingbox();
	afx_msg void OnUpdateOptionsShowboundingbox(CCmdUI *pCmdUI);
	afx_msg void OnOptionsChangecolor();
	afx_msg void OnOptionsFinenesscontrol();
	afx_msg void OnOptionsShowvertexnormals();
	afx_msg void OnUpdateOptionsShowvertexnormals(CCmdUI *pCmdUI);
	afx_msg void OnActionAllobjects();
	afx_msg void OnUpdateActionAllobjects(CCmdUI *pCmdUI);
	afx_msg void OnOptionsHighlightface();
	afx_msg void OnUpdateOptionsHighlightface(CCmdUI *pCmdUI);
	afx_msg void OnRenderingWireframe();
	afx_msg void OnUpdateRenderingWireframe(CCmdUI *pCmdUI);
	afx_msg void OnRenderingRender();
	afx_msg void OnUpdateRenderingRender(CCmdUI *pCmdUI);
	afx_msg void OnRenderingBackfaceculling();
	afx_msg void OnUpdateRenderingBackfaceculling(CCmdUI *pCmdUI);
	afx_msg void OnNormalApproximatevertexnormals();
	afx_msg void OnUpdateNormalApproximatevertexnormals(CCmdUI *pCmdUI);
	afx_msg void OnNormalInversenormals();
	afx_msg void OnUpdateNormalInversenormals(CCmdUI *pCmdUI);
	afx_msg void OnOptionsShowsilhouette();
	afx_msg void OnUpdateOptionsShowsilhouette(CCmdUI *pCmdUI);
	afx_msg void OnActionClearall();
};

void drawAxis(CDC* pDC, const Matrix4d & transMat, const Matrix4d & screenMat);

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
