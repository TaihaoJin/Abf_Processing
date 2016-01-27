#if !defined(AFX_BLINEADJUSTOPTIONDIALOG_H__50A744C7_67E8_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_BLINEADJUSTOPTIONDIALOG_H__50A744C7_67E8_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlineAdjustOptionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlineAdjustOptionDialog dialog

class CBlineAdjustOptionDialog : public CDialog
{
// Construction
public:
	float GetWindowSize();
	float GetFirstPoint();
	CBlineAdjustOptionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlineAdjustOptionDialog)
	enum { IDD = IDD_DIALOG2 };
	float	m_fFirstPoint;
	float	m_fWindowSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlineAdjustOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlineAdjustOptionDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLINEADJUSTOPTIONDIALOG_H__50A744C7_67E8_11D4_83DE_00C04F200B5A__INCLUDED_)
