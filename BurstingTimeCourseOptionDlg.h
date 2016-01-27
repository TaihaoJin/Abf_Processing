#if !defined(AFX_BURSTINGTIMECOURSEOPTIONDLG_H__DD01BE59_0D4F_45C0_82BD_816C224B748E__INCLUDED_)
#define AFX_BURSTINGTIMECOURSEOPTIONDLG_H__DD01BE59_0D4F_45C0_82BD_816C224B748E__INCLUDED_
#include "structures.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurstingTimeCourseOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBurstingTimeCourseOptionDlg dialog

class CBurstingTimeCourseOptionDlg : public CDialog
{
// Construction
public:
	CString getBurstEvolutionParametersAsString();
	CString getParameterString();
	void IntegerReader(CString st, vector <int> &vnData);
	void getBurstTimecourseOptions(structBurstingTimeCourseOptionNode &structParNode);
	void initialize(structBurstingTimeCourseOptionNode &structParNode);
	void SetParameters(structBurstingTimeCourseOptionNode structParNode);
	CBurstingTimeCourseOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBurstingTimeCourseOptionDlg)
	enum { IDD = IDD_BurstingTimeCourse };
	float	m_fBurstDelimiter;
	int		m_nBinSize;
	int		m_nMaxOpening;
	int		m_nMinOpening;
	float	m_fClusterDelimiter;
	int		m_nSortingIndex;
	float	m_fBinWidth;
	float	m_fLatancy;
	int		m_nLatancy;
	structBurstingTimeCourseOptionNode m_structParNode;
	int		m_nSubLevelI;
	int		m_nSubLevelF;
	CString	m_sFilteringKey;
	float	m_fMinIBC;
	float	m_fMaxIBC;
	float	m_fMinBurstDuration;
	float	m_fMaxBurstDuration;
	float	m_fMinCTo;
	float	m_fMaxCTo;
	int		m_nMaxNOPC;
	int		m_nMinNOPC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurstingTimeCourseOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nFilteringKey;
	int m_nNumFilteringKeys;

	// Generated message map functions
	//{{AFX_MSG(CBurstingTimeCourseOptionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURSTINGTIMECOURSEOPTIONDLG_H__DD01BE59_0D4F_45C0_82BD_816C224B748E__INCLUDED_)
