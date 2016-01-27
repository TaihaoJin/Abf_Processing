#if !defined(AFX_SEGMENTEXTRACTIONDIALOG1_H__83FE79CB_3BF6_4076_9C15_DAFA4C5EAB50__INCLUDED_)
#define AFX_SEGMENTEXTRACTIONDIALOG1_H__83FE79CB_3BF6_4076_9C15_DAFA4C5EAB50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SegmentExtractionDialog1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog1 dialog

class CSegmentExtractionDialog1 : public CDialog
{
// Construction
public:
	CSegmentExtractionDialog1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSegmentExtractionDialog1)
	enum { IDD = IDD_SegmentDeletionOptions };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegmentExtractionDialog1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSegmentExtractionDialog1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENTEXTRACTIONDIALOG1_H__83FE79CB_3BF6_4076_9C15_DAFA4C5EAB50__INCLUDED_)
