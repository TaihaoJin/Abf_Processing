#if !defined(AFX_ARTIFACTSEGMENTDIALOG_H__0976A221_E6F6_11D4_AFE1_00C04F200B5A__INCLUDED_)
#define AFX_ARTIFACTSEGMENTDIALOG_H__0976A221_E6F6_11D4_AFE1_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArtifactSegmentDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArtifactSegmentDialog dialog

class CArtifactSegmentDialog : public CDialog
{
// Construction
public:
	void SetParameters(float fBeginningOfArtifact, float fCorrectAmplitudeAtBeginning, float fEndOfArtifact, float fCorrectAmplitudeAtEnd, bool bDoNotConfirm);
	void GetParameters(float& fBeginningOfArtifact, float& fCorrectAmplitudeAtBeginning, float& fEndOfArtifact, float& fCorrectAmplitudeAtEnd, bool& bDoNotConfirm);
	CArtifactSegmentDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArtifactSegmentDialog)
	enum { IDD = IDD_DIALOG8 };
	float	m_fBeginningOfArtifact;
	float	m_fCorrectAmplitudeAtBeginning;
	float	m_fEndOfArtifact;
	float	m_fCorrectAmplitudeAtEnd;
	BOOL	m_bDoNotConfirm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArtifactSegmentDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArtifactSegmentDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARTIFACTSEGMENTDIALOG_H__0976A221_E6F6_11D4_AFE1_00C04F200B5A__INCLUDED_)
