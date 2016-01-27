#if !defined(AFX_IGNORESHORTCLOSINGDIALOG_H__9EAF8AC4_F44F_4300_A780_B5FA09E7F985__INCLUDED_)
#define AFX_IGNORESHORTCLOSINGDIALOG_H__9EAF8AC4_F44F_4300_A780_B5FA09E7F985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IgnoreShortClosingDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIgnoreShortClosingDialog dialog

class CIgnoreShortClosingDialog : public CDialog
{
// Construction
public:
	float GetCutoff();
	CIgnoreShortClosingDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIgnoreShortClosingDialog)
	enum { IDD = IDD_IgnoreShortClosings };
	float	m_fCutoff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIgnoreShortClosingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIgnoreShortClosingDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IGNORESHORTCLOSINGDIALOG_H__9EAF8AC4_F44F_4300_A780_B5FA09E7F985__INCLUDED_)
