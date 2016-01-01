// MouseSensivityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MouseSensivityDlg.h"


// MouseSensivityDlg

IMPLEMENT_DYNAMIC(MouseSensivityDlg, CWnd)

MouseSensivityDlg::MouseSensivityDlg()
{

	EnableAutomation();
}

MouseSensivityDlg::~MouseSensivityDlg()
{
}

void MouseSensivityDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(MouseSensivityDlg, CWnd)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(MouseSensivityDlg, CWnd)
END_DISPATCH_MAP()

// Note: we add support for IID_IMouseSensivityDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {A6B362CB-4DD3-42A5-975F-B7E14997F7F9}
static const IID IID_IMouseSensivityDlg =
{ 0xA6B362CB, 0x4DD3, 0x42A5, { 0x97, 0x5F, 0xB7, 0xE1, 0x49, 0x97, 0xF7, 0xF9 } };

BEGIN_INTERFACE_MAP(MouseSensivityDlg, CWnd)
	INTERFACE_PART(MouseSensivityDlg, IID_IMouseSensivityDlg, Dispatch)
END_INTERFACE_MAP()


// MouseSensivityDlg message handlers


