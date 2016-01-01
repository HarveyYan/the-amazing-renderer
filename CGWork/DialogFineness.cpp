// DialogFineness.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DialogFineness.h"
#include "afxdialogex.h"

static const int MAX_FINENESS = 10000;


// DialogFineness dialog

IMPLEMENT_DYNAMIC(DialogFineness, CDialogEx)

DialogFineness::DialogFineness(CWnd* pParent /*=NULL*/)
: CDialogEx(DialogFineness::IDD, pParent)
, Fineness(0)
{

}

DialogFineness::~DialogFineness()
{
}

void DialogFineness::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINENESS, Fineness);
	DDV_MinMaxDouble(pDX, Fineness, 2, MAX_FINENESS);
}


BEGIN_MESSAGE_MAP(DialogFineness, CDialogEx)
END_MESSAGE_MAP()


// DialogFineness message handlers
