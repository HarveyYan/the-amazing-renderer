#pragma once


// MouseSensivityDlg

class MouseSensivityDlg : public CWnd
{
	DECLARE_DYNAMIC(MouseSensivityDlg)

public:
	MouseSensivityDlg();
	virtual ~MouseSensivityDlg();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


