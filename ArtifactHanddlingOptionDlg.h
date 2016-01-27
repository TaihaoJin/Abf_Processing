#if !defined(AFX_ARTIFACTHANDDLINGOPTIONDLG_H__DA52B6C1_E24D_11D4_AFE1_00C04F200B5A__INCLUDED_)
#define AFX_ARTIFACTHANDDLINGOPTIONDLG_H__DA52B6C1_E24D_11D4_AFE1_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArtifactHanddlingOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArtifactHanddlingOptionDlg dialog

class CArtifactHanddlingOptionDlg : public CDialog
{
// Construction
public:
	void GetBoolParameters(bool& bCheckDelta, bool& bCheckOppositeDirrection, 
						   bool& bCheckAmplitude, bool& bBasedOnAdjustedData,
						   bool &bEntireTrace,float &fTime1,float &fTime2,
						   bool &bCheckEvl, bool  &bReadFromFile);
	void GetThresholds(float& fAmplitudeThreshold,float& fDeltaThreshold,float& fOppositeDirectionThreshold, float& fRange);
	CArtifactHanddlingOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArtifactHanddlingOptionDlg)
	enum { IDD = IDD_DIALOG7 };
	float	m_fAmplitudeThreshold;
	float	m_fDeltaThreshold;
	float	m_fOppositeDirectionThreshold;
	float	m_fRange;
	BOOL	m_bCheckDelta;
	BOOL	m_bCheckOppositeDirrection;
	BOOL	m_bCheckAmplitude;
	BOOL	m_bBasedOnAdjustedData;
	float	m_fTime1;
	float	m_fTime2;
	BOOL    m_bEntireTrace;
	BOOL	m_bCheckEvl;
	BOOL	m_bReadFromFile;
	BOOL	m_bCheckHightAndWidthRange;
	float	m_fHighthreshold;
	float	m_fMinWidth;
	float	m_fMaxWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArtifactHanddlingOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArtifactHanddlingOptionDlg)
	afx_msg void OnApplyingOnlyBetweenT1AndT();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARTIFACTHANDDLINGOPTIONDLG_H__DA52B6C1_E24D_11D4_AFE1_00C04F200B5A__INCLUDED_)
