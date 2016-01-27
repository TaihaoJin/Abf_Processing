#if !defined(AFX_BALLROLLINGOPTION_H__6B243C41_992A_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_BALLROLLINGOPTION_H__6B243C41_992A_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BallRollingOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBallRollingOption dialog

class CBallRollingOption : public CDialog
{
// Construction
public:
	float GetAmp();
	int GetnDelta();
	bool TracingMinima();
	bool ApplyingToEntireTrace();
	float GetfTime2();
	float GetfTime1();
	int GetAnchorRank();
	bool SmoothingTrace();
	bool CheckContactingPoints();
	int GetLevelHistWindowSize();
	int GetMaxNumOfSuperiors();
	float GetCriterionOfUpTransitions();
	int GetRank();
	float GetWindowSize();
	CBallRollingOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBallRollingOption)
	enum { IDD = IDD_DIALOG6 };
	float	m_fWindowSize;
	int		m_nRank;
	float	m_fCriterionOfUpTransitions;
	int		m_nLevelHistWindowSize;
	int		m_nMaxNumOfSuperiors;
	BOOL	m_bSmoothing;
	BOOL	m_bCheckContactingPoints;
	BOOL    m_bMinima;
	BOOL    m_bMaxima;
	int		m_nAnchorRank;
	float	m_fTime1;
	float	m_fTime2;
	int		m_nDelta;
	float	m_fAmp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBallRollingOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bApplyingToEntireTrace;

	// Generated message map functions
	//{{AFX_MSG(CBallRollingOption)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLROLLINGOPTION_H__6B243C41_992A_11D4_83DE_00C04F200B5A__INCLUDED_)
