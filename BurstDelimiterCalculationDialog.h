#if !defined(AFX_BURSTDELIMITERCALCULATIONDIALOG_H__4EA8D4D6_CEE0_4D52_9087_26F5DC4594B2__INCLUDED_)
#define AFX_BURSTDELIMITERCALCULATIONDIALOG_H__4EA8D4D6_CEE0_4D52_9087_26F5DC4594B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurstDelimiterCalculationDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBurstDelimiterCalculationDialog dialog

class CBurstDelimiterCalculationDialog : public CDialog
{
// Construction
public:
	void calculateBurstDelimeters();
	CBurstDelimiterCalculationDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBurstDelimiterCalculationDialog)
	enum { IDD = IDD_BurstDelimiterCalculations };
	float	m_fTs;
	float	m_fTl;
	float	m_fAs;
	float	m_fAl;
	float	m_fTc1;
	float	m_fTc2;
	float	m_fTc3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurstDelimiterCalculationDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBurstDelimiterCalculationDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURSTDELIMITERCALCULATIONDIALOG_H__4EA8D4D6_CEE0_4D52_9087_26F5DC4594B2__INCLUDED_)
