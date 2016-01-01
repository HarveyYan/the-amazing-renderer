// DialogChangeColor.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DialogChangeColor.h"
#include "afxdialogex.h"
#include "WingedEdgeMesh.h"

extern std::vector<WingedEdgeMesh> OBJECTS;
//extern COLORREF bgColor;

// DialogChangeColor dialog

IMPLEMENT_DYNAMIC(DialogChangeColor, CDialogEx)

DialogChangeColor::DialogChangeColor(CWnd* pParent /*=NULL*/)
: CDialogEx(DialogChangeColor::IDD, pParent), m_bgColor(RGB(255,255,255))
{

}

DialogChangeColor::~DialogChangeColor()
{
}

void DialogChangeColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogChangeColor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WIREFRAME, &DialogChangeColor::OnBnClickedButtonWireframe)
	ON_BN_CLICKED(IDC_BUTTON_BACKGROUND, &DialogChangeColor::OnBnClickedButtonBackground)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &DialogChangeColor::OnBnClickedButtonNormal)
END_MESSAGE_MAP()


// DialogChangeColor message handlers


void DialogChangeColor::OnBnClickedButtonWireframe()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK){
		COLORREF c = dlg.GetColor();
		for (std::vector<WingedEdgeMesh>::iterator obj = OBJECTS.begin(); obj != OBJECTS.end(); ++obj)
			obj->setColor(c);
	}
}


void DialogChangeColor::OnBnClickedButtonBackground()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK){
		m_bgColor = dlg.GetColor();
	}
}


void DialogChangeColor::OnBnClickedButtonNormal()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK){
		COLORREF c = dlg.GetColor();
		for (std::vector<WingedEdgeMesh>::iterator obj = OBJECTS.begin(); obj != OBJECTS.end(); ++obj)
			obj->setNormalColor(c);
	}
}
