#if !defined(AFX_SEGDELETOPTIONDIALOG_H__60DDCBFB_A574_48BC_B02F_7FF12C0D89AD__INCLUDED_)
#define AFX_SEGDELETOPTIONDIALOG_H__60DDCBFB_A574_48BC_B02F_7FF12C0D89AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SegDeletOptionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSegDeletOptionDialog dialog

class CSegDeletOptionDialog : public CDialog
{
// Construction
public:
	void GetParameters(float & fBegin, float & fAmpBegin, float & fEnd, float & fAmpEnd, bool & bDone);
	CSegDeletOptionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSegDeletOptionDialog)
	enum { IDD = IDD_SegmentDeletionOptions };
	float	m_fBegin;
	float	m_fEnd;
	float	m_fAmpBegin;
	float	m_fAmpEnd;
	bool m_bDone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegDeletOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSegDeletOptionDialog)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGDELETOPTIONDIALOG_H__60DDCBFB_A574_48BC_B02F_7FF12C0D89AD__INCLUDED_)
