#if !defined(AFX_IGNORESMALLTRANSITIONOPTIONDIALOG_H__A98B2B4C_0526_4FE9_B8A4_EC112C6B0FF5__INCLUDED_)
#define AFX_IGNORESMALLTRANSITIONOPTIONDIALOG_H__A98B2B4C_0526_4FE9_B8A4_EC112C6B0FF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IgnoreSmallTransitionOptionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIgnoreSmallTransitionOptionDialog dialog

class CIgnoreSmallTransitionOptionDialog : public CDialog
{
// Construction
public:
	float GetCutoff();
	CIgnoreSmallTransitionOptionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIgnoreSmallTransitionOptionDialog)
	enum { IDD = IDD_IgnoreSmallTransitions };
	float	m_fCutoff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIgnoreSmallTransitionOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIgnoreSmallTransitionOptionDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IGNORESMALLTRANSITIONOPTIONDIALOG_H__A98B2B4C_0526_4FE9_B8A4_EC112C6B0FF5__INCLUDED_)
