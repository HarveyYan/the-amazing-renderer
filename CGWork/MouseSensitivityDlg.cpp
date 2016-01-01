// MouseSensitivityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MouseSensitivityDlg.h"
#include "afxdialogex.h"


// MouseSensitivityDlg dialog

IMPLEMENT_DYNAMIC(MouseSensitivityDlg, CDialogEx)

MouseSensitivityDlg::MouseSensitivityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MouseSensitivityDlg::IDD, pParent)
	, m_translateSensitivity((int)TRANSLATE_SENS)
	, m_rotateSensitivity((int)ROTATE_SENS)
	, m_scaleSensitivity((int)SCALE_SENS)
{
}

MouseSensitivityDlg::~MouseSensitivityDlg()
{
}

void MouseSensitivityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_TRANSLATE_SENSITIVITY, m_translateSensitivity);
	DDV_MinMaxInt(pDX, m_translateSensitivity, 1, 100);
	DDX_Slider(pDX, IDC_ROTATE_SENSITIVITY, m_rotateSensitivity);
	DDV_MinMaxInt(pDX, m_rotateSensitivity, 1, 100);
	DDX_Slider(pDX, IDC_SCALE_SENSITIVITY, m_scaleSensitivity);
	DDV_MinMaxInt(pDX, m_scaleSensitivity, 1, 100);
}


BEGIN_MESSAGE_MAP(MouseSensitivityDlg, CDialogEx)
END_MESSAGE_MAP()


// MouseSensitivityDlg message handlers
