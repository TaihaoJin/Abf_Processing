// BurstDelimiterCalculationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BurstDelimiterCalculationDialog.h"
#include "BurstDelimiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBurstDelimiterCalculationDialog dialog


CBurstDelimiterCalculationDialog::CBurstDelimiterCalculationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBurstDelimiterCalculationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBurstDelimiterCalculationDialog)
	m_fTs = 0.0f;
	m_fTl = 0.0f;
	m_fAs = 0.0f;
	m_fAl = 0.0f;
	m_fTc1 = 0.0f;
	m_fTc2 = 0.0f;
	m_fTc3 = 0.0f;
	//}}AFX_DATA_INIT
}


void CBurstDelimiterCalculationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurstDelimiterCalculationDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fTs);
	DDX_Text(pDX, IDC_EDIT2, m_fTl);
	DDX_Text(pDX, IDC_EDIT3, m_fAs);
	DDX_Text(pDX, IDC_EDIT4, m_fAl);
	DDX_Text(pDX, IDC_EDIT5, m_fTc1);
	DDX_Text(pDX, IDC_EDIT6, m_fTc2);
	DDX_Text(pDX, IDC_EDIT7, m_fTc3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurstDelimiterCalculationDialog, CDialog)
	//{{AFX_MSG_MAP(CBurstDelimiterCalculationDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurstDelimiterCalculationDialog message handlers

void CBurstDelimiterCalculationDialog::calculateBurstDelimeters()
{
	int nKey;
	CBurstDelimiter CBD;
	m_fTc1=CBD.BurstDelimiter(m_fTs,m_fAs,m_fTl,m_fAl,1);
	m_fTc2=CBD.BurstDelimiter(m_fTs,m_fAs,m_fTl,m_fAl,2);
	m_fTc3=CBD.BurstDelimiter(m_fTs,m_fAs,m_fTl,m_fAl,3);
}
