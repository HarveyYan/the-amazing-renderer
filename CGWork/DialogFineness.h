#pragma once


// DialogFineness dialog

class DialogFineness : public CDialogEx
{
	DECLARE_DYNAMIC(DialogFineness)

public:
	DialogFineness(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogFineness();

	// Dialog Data
	enum { IDD = IDD_FINENESS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double Fineness;
};
