#if !defined(AFX_SUBCONDUCTANCEDLG_H__8BDC4339_7953_4B22_8C4C_C3F6822024E4__INCLUDED_)
#define AFX_SUBCONDUCTANCEDLG_H__8BDC4339_7953_4B22_8C4C_C3F6822024E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubconductanceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubconductanceDlg dialog

class CSubconductanceDlg : public CDialog
{
// Construction
public:
	void GetAmps(float &fAmp0, float &fAmp1);
	CSubconductanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubconductanceDlg)
	enum { IDD = IDD_SubconductanceDlg };
	float	m_fAmp0;
	float	m_fAmp1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubconductanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubconductanceDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBCONDUCTANCEDLG_H__8BDC4339_7953_4B22_8C4C_C3F6822024E4__INCLUDED_)
