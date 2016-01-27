#if !defined(AFX_BURSTOPTIONDIALOG_H__604A4D9D_8C46_4F5E_B054_11B10469E0DB__INCLUDED_)
#define AFX_BURSTOPTIONDIALOG_H__604A4D9D_8C46_4F5E_B054_11B10469E0DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurstOptionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBurstOptionDialog dialog

class CBurstOptionDialog : public CDialog
{
// Construction
public:
	int GetMinimumOpenning();
	float GetBurstDelimiter();
	CBurstOptionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBurstOptionDialog)
	enum { IDD = IDD_DIALOG1 };
	float	m_fBurstDelimiter;
	int		m_nMinimumOpenning;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurstOptionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBurstOptionDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURSTOPTIONDIALOG_H__604A4D9D_8C46_4F5E_B054_11B10469E0DB__INCLUDED_)
