#if !defined(AFX_EVLCOMMENT_H__C6885113_A32E_4521_96A5_223A00BED14D__INCLUDED_)
#define AFX_EVLCOMMENT_H__C6885113_A32E_4521_96A5_223A00BED14D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvlComment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvlComment dialog

class CEvlComment : public CDialog
{
// Construction
public:
	CString GetComment();
	CEvlComment(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvlComment)
	enum { IDD = IDD_Evl_Comments };
	CString	m_sComment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvlComment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEvlComment)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVLCOMMENT_H__C6885113_A32E_4521_96A5_223A00BED14D__INCLUDED_)
