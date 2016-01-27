#if !defined(AFX_ALLPOINTHISTGRAMDIALOG_H__16AA964C_BF3F_4459_9DBB_4494CB4D29D8__INCLUDED_)
#define AFX_ALLPOINTHISTGRAMDIALOG_H__16AA964C_BF3F_4459_9DBB_4494CB4D29D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllPointHistgramDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllPointHistgramDialog dialog

class CAllPointHistgramDialog : public CDialog
{
// Construction
public:
	float GetTransitThreshold();
	int GetPolarity();
	int GetReductionFactor();
	bool ShowTrace();
	float GetSegmentLength();
	float GetFirstPoint();
	CAllPointHistgramDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAllPointHistgramDialog)
	enum { IDD = IDD_DIALOG3 };
	float	m_fFirstPoint;
	float	m_fLength;
	int		m_nReductionFactor;
	float	m_fTransitionThreshold;
	int     m_nPolarity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllPointHistgramDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAllPointHistgramDialog)
	afx_msg void OnNegativeCurrent();
	afx_msg void OnPositiveCurrent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLPOINTHISTGRAMDIALOG_H__16AA964C_BF3F_4459_9DBB_4494CB4D29D8__INCLUDED_)
