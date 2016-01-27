// EvlTimeCourseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlTimeCourseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvlTimeCourseDialog dialog


CEvlTimeCourseDialog::CEvlTimeCourseDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEvlTimeCourseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvlTimeCourseDialog)
	m_fBinSize = 5.0f;
	m_bHardBoundary = FALSE;
	//}}AFX_DATA_INIT
}


void CEvlTimeCourseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvlTimeCourseDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fBinSize);
	DDV_MinMaxFloat(pDX, m_fBinSize, -1.e+031f, 1.e+009f);
	DDX_Check(pDX, IDC_HardBoundary, m_bHardBoundary);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvlTimeCourseDialog, CDialog)
	//{{AFX_MSG_MAP(CEvlTimeCourseDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvlTimeCourseDialog message handlers

float CEvlTimeCourseDialog::GetBinSize()
{
	return m_fBinSize;	
	// a negative value indicates using events as units.
}

bool CEvlTimeCourseDialog::HardBoundary()
{
	return m_bHardBoundary;
}
