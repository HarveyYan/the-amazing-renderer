#pragma once

#include "StdAfx.h"

// DialogChangeColor dialog

class DialogChangeColor : public CDialogEx
{
	DECLARE_DYNAMIC(DialogChangeColor)

public:
	DialogChangeColor(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogChangeColor();

	// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWireframe();
	afx_msg void OnBnClickedButtonBackground();
	afx_msg void OnBnClickedButtonNormal();

	COLORREF m_bgColor;
};
