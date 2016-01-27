// SegmentExtractionDialog1.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "SegmentExtractionDialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog1 dialog


CSegmentExtractionDialog1::CSegmentExtractionDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentExtractionDialog1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSegmentExtractionDialog1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSegmentExtractionDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegmentExtractionDialog1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSegmentExtractionDialog1, CDialog)
	//{{AFX_MSG_MAP(CSegmentExtractionDialog1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog1 message handlers
