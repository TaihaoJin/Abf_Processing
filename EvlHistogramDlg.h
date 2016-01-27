#if !defined(AFX_EVLHISTOGRAMDLG_H__49DA6B79_7D5E_4401_B269_8B3874AB5A43__INCLUDED_)
#define AFX_EVLHISTOGRAMDLG_H__49DA6B79_7D5E_4401_B269_8B3874AB5A43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvlHistogramDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvlHistogramDlg dialog

class CEvlHistogramDlg : public CDialog
{
// Construction
public:
	bool FullIteration();
	bool CalConfidanceRanges();
	int FilteringKey();
	bool FilterEvl();
	int GetHistType();
	CEvlHistogramDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvlHistogramDlg)
	enum { IDD = IDD_EvlFittingHistogramMethod };
	BOOL	m_bFilterEvl;
	int		m_nFilteringKey;
	BOOL	m_bCalConfidanceRanges;
	BOOL	m_bFullIteration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvlHistogramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nHistType;
	// Generated message map functions
	//{{AFX_MSG(CEvlHistogramDlg)
	afx_msg void OnLogHist();
	afx_msg void OnAllEvents();
	afx_msg void OnHybridHist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVLHISTOGRAMDLG_H__49DA6B79_7D5E_4401_B269_8B3874AB5A43__INCLUDED_)
