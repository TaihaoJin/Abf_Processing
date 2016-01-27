// SegmentExtractionDialog2.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "SegmentExtractionDialog2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog2 dialog


CSegmentExtractionDialog2::CSegmentExtractionDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentExtractionDialog2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSegmentExtractionDialog2)
	m_fSweepLength = 5000.0f;
	m_fBegin = 250.0f;
	m_fEnd = 750.0f;
	m_nFirstSweep = 1;
	m_nLastSweep = 50000000;
	m_nReferenceSweep = 0;
	m_bSubstractReferenceSweep = FALSE;
	m_bAlignMaximum = FALSE;
	//}}AFX_DATA_INIT
}


void CSegmentExtractionDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegmentExtractionDialog2)
	DDX_Text(pDX, IDC_EDIT1, m_fSweepLength);
	DDX_Text(pDX, IDC_EDIT2, m_fBegin);
	DDX_Text(pDX, IDC_EDIT3, m_fEnd);
	DDX_Text(pDX, IDC_EDIT4, m_nFirstSweep);
	DDX_Text(pDX, IDC_EDIT5, m_nLastSweep);
	DDX_Text(pDX, IDC_EDIT6, m_nReferenceSweep);
	DDX_Check(pDX, IDC_CHECK1, m_bSubstractReferenceSweep);
	DDX_Check(pDX, IDC_CHECK2, m_bAlignMaximum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSegmentExtractionDialog2, CDialog)
	//{{AFX_MSG_MAP(CSegmentExtractionDialog2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegmentExtractionDialog2 message handlers

structSegmentExtractionNode CSegmentExtractionDialog2::GetParameters(float &fLength, float &fBegin, float &fEnd)
{
	structSegmentExtractionNode aNode;
	aNode.bAlignMaximum=m_bAlignMaximum;
	aNode.fBegin=m_fBegin;
	aNode.fEnd=m_fEnd;
	aNode.fLength=m_fSweepLength;
	aNode.nFirstSweep=m_nFirstSweep;
	aNode.nLastWeep=m_nLastSweep;
	aNode.nRefSweep=m_nReferenceSweep;
	aNode.bSubstractReference=m_bSubstractReferenceSweep;
	fLength=m_fSweepLength;
	fBegin=m_fBegin;
	fEnd=m_fEnd;
	return aNode;
}
