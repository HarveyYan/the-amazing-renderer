#pragma once


// MouseSensitivityDlg dialog
static const double SLIDER_UB = 100;
static const double SLIDER_LB = 1;
static const double SCALE_SENS = SLIDER_UB / 7;
static const double ROTATE_SENS = SLIDER_UB / 8 ;
static const double TRANSLATE_SENS = SLIDER_UB / 10;
static double TRANSLATE_SENS_LB = 0.1;
static double TRANSLATE_SENS_UB = 10;
static double ROTATE_SENS_LB = 0.1;
static double ROTATE_SENS_UB = 10;
static double SCALE_SENS_LB = 0.1;
static double SCALE_SENS_UB = 10;

class MouseSensitivityDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MouseSensitivityDlg)

public:
	MouseSensitivityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MouseSensitivityDlg();

// Dialog Data
	enum { IDD = IDD_SENSITIVITY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_translateSensitivity;
	int m_rotateSensitivity;
	int m_scaleSensitivity;

	double getTranslateSens() const {
		return TRANSLATE_SENS_LB + (TRANSLATE_SENS_UB - TRANSLATE_SENS_LB)*m_translateSensitivity / SLIDER_UB;
	}
	double getRotateSens() const {
		return ROTATE_SENS_LB + (ROTATE_SENS_UB - ROTATE_SENS_LB)*m_rotateSensitivity / SLIDER_UB;
	}
	double getScaleSens() const {
		return SCALE_SENS_LB + (SCALE_SENS_UB - SCALE_SENS_LB)*m_scaleSensitivity / SLIDER_UB;
	}
};
