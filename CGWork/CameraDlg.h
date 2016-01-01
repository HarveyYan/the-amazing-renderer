#pragma once


// CameraDlg dialog

class CameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CameraDlg)

public:
	CameraDlg(CWnd* pParent = NULL);   // standard constructor
	CameraDlg::CameraDlg(CWnd* pParent /*=NULL*/, double n, double f, double fov, double x, double y, double z);
	virtual ~CameraDlg();

// Dialog Data
	enum { IDD = IDD_CAMERA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_near;
	afx_msg void OnEnChangeEdit2();
	double m_far;
	double m_fov;
	double m_x;
	double m_y;
	double m_z;
};
