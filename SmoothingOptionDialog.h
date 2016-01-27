#if !defined(AFX_SMOOTHINGOPTIONDIALOG_H__08F94161_EAFF_11D4_AFE1_00C04F200B5A__INCLUDED_)
#define AFX_SMOOTHINGOPTIONDIALOG_H__08F94161_EAFF_11D4_AFE1_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmoothingOptionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmoothingOptionDialog dialog

class CSmoothingOptionDialog : public CDialog
{
// Construction
public:
	void GetParameters(float& fFirstPoint, float& fLength, int& nSmoothingWindowSize);
	CSmoothingOptionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmoothingOptionDialog)
	enum { IDD = IDD_DIALOG9 };
	float	m_fFirstPoint;
	float	m_fLength;
	int		m_nSmoothingWindowSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmoothingOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmoothingOptionDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMOOTHINGOPTIONDIALOG_H__08F94161_EAFF_11D4_AFE1_00C04F200B5A__INCLUDED_)
