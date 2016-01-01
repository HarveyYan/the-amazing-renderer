// CameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CameraDlg.h"
#include "afxdialogex.h"
#include <limits>


// CameraDlg dialog

IMPLEMENT_DYNAMIC(CameraDlg, CDialog)

CameraDlg::CameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CameraDlg::IDD, pParent)
	, m_near(0)
	, m_far(0)
	, m_fov(0)
	, m_x(0)
	, m_y(0)
	, m_z(0)
{

}

CameraDlg::CameraDlg(CWnd* pParent /*=NULL*/, double n, double f, double fov, double x, double y, double z)
: CDialog(CameraDlg::IDD, pParent)
, m_near(n)
, m_far(f)
, m_fov(fov)
, m_x(x)
, m_y(y)
, m_z(z)
{

}

CameraDlg::~CameraDlg()
{
}

void CameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_near);
	DDV_MinMaxDouble(pDX, m_near, std::numeric_limits<double>::epsilon(), (std::numeric_limits<double>::max)());
	DDX_Text(pDX, IDC_EDIT2, m_far);
	DDV_MinMaxDouble(pDX, m_far, 0, 100);
	DDX_Text(pDX, IDC_EDIT3, m_fov);
	DDV_MinMaxDouble(pDX, m_fov, 0, 180);
	DDX_Text(pDX, IDC_EDIT6, m_x);
	DDX_Text(pDX, IDC_EDIT5, m_y);
	DDX_Text(pDX, IDC_EDIT4, m_z);
}


BEGIN_MESSAGE_MAP(CameraDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT2, &CameraDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CameraDlg message handlers


void CameraDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
