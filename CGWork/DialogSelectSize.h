#pragma once


// DialogSelectSize dialog

class DialogSelectSize : public CDialogEx
{
	DECLARE_DYNAMIC(DialogSelectSize)

public:
	DialogSelectSize(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogSelectSize();

// Dialog Data
	enum { IDD = IDD_SELECT_SIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int size;
};
