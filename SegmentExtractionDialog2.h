#if !defined(AFX_SEGMENTEXTRACTIONDIALOG2_H__21F64645_3325_4C83_8260_28BC1FCCBE14__INCLUDED_)
#define AFX_SEGMENTEXTRACTIONDIALOG2_H__21F64645_3325_4C83_8260_28BC1FCCBE14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SegmentExtractionDialog2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog2 dialog
#include "structures.h"
class CSegmentExtractionDialog2 : public CDialog
{
// Construction
public:
	structSegmentExtractionNode GetParameters(float& fLength, float& fBegin, float& fEnd);
	CSegmentExtractionDialog2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSegmentExtractionDialog2)
	enum { IDD = IDD_DIALOG10 };
	float	m_fSweepLength;
	float	m_fBegin;
	float	m_fEnd;
	int		m_nFirstSweep;
	int		m_nLastSweep;
	int		m_nReferenceSweep;
	BOOL	m_bSubstractReferenceSweep;
	BOOL	m_bAlignMaximum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegmentExtractionDialog2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSegmentExtractionDialog2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENTEXTRACTIONDIALOG2_H__21F64645_3325_4C83_8260_28BC1FCCBE14__INCLUDED_)
