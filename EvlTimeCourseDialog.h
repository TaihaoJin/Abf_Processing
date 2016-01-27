#if !defined(AFX_EVLTIMECOURSEDIALOG_H__FB410C01_8330_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_EVLTIMECOURSEDIALOG_H__FB410C01_8330_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvlTimeCourseDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvlTimeCourseDialog dialog

class CEvlTimeCourseDialog : public CDialog
{
// Construction
public:
	bool HardBoundary();
	float GetBinSize();
	CEvlTimeCourseDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvlTimeCourseDialog)
	enum { IDD = IDD_DIALOG5 };
	float	m_fBinSize; //(Positive values indicate using seconds as the units while negative values indicate 
	BOOL	m_bHardBoundary;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvlTimeCourseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEvlTimeCourseDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVLTIMECOURSEDIALOG_H__FB410C01_8330_11D4_83DE_00C04F200B5A__INCLUDED_)
