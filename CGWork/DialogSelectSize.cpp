// DialogSelectSize.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DialogSelectSize.h"
#include "afxdialogex.h"


// DialogSelectSize dialog

IMPLEMENT_DYNAMIC(DialogSelectSize, CDialogEx)

DialogSelectSize::DialogSelectSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogSelectSize::IDD, pParent)
	, size(0)
{

}

DialogSelectSize::~DialogSelectSize()
{
}

void DialogSelectSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, size);
	DDV_MinMaxInt(pDX, size, 0, 100);
}


BEGIN_MESSAGE_MAP(DialogSelectSize, CDialogEx)
END_MESSAGE_MAP()


// DialogSelectSize message handlers
