// EvlHistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlHistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvlHistogramDlg dialog


CEvlHistogramDlg::CEvlHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvlHistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvlHistogramDlg)
	m_bFilterEvl = FALSE;
	m_nFilteringKey = 4;
	m_bCalConfidanceRanges = FALSE;
	m_bFullIteration = FALSE;
	//}}AFX_DATA_INIT
	m_nHistType=1;
}


void CEvlHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvlHistogramDlg)
	DDX_Check(pDX, IDC_FilterEvl, m_bFilterEvl);
	DDX_Text(pDX, IDC_nFilteringKey, m_nFilteringKey);
	DDV_MinMaxInt(pDX, m_nFilteringKey, 1, 1000);
	DDX_Check(pDX, IDC_bCalConfidanceRange, m_bCalConfidanceRanges);
	DDX_Check(pDX, IDC_FullIteration, m_bFullIteration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvlHistogramDlg, CDialog)
	//{{AFX_MSG_MAP(CEvlHistogramDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnLogHist)
	ON_BN_CLICKED(IDC_RADIO2, OnAllEvents)
	ON_BN_CLICKED(IDC_RADIO3, OnHybridHist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvlHistogramDlg message handlers

void CEvlHistogramDlg::OnLogHist() 
{
	// TODO: Add your control notification handler code here
	m_nHistType=1;	
}

void CEvlHistogramDlg::OnAllEvents() 
{
	// TODO: Add your control notification handler code here
	
	m_nHistType=2;	
}

void CEvlHistogramDlg::OnHybridHist() 
{
	// TODO: Add your control notification handler code here	
	m_nHistType=3;	
}

int CEvlHistogramDlg::GetHistType()
{
	return m_nHistType;
}

bool CEvlHistogramDlg::FilterEvl()
{
	if(m_bFilterEvl)return true;
	else return false;
}

int CEvlHistogramDlg::FilteringKey()
{
	return m_nFilteringKey;
}

bool CEvlHistogramDlg::CalConfidanceRanges()
{
	if(m_bCalConfidanceRanges)
		return true;
	else
		return false;
}

bool CEvlHistogramDlg::FullIteration()
{
	if(m_bFullIteration)
		return true;
	else
		return false;

}
