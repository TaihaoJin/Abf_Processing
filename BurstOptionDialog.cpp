// BurstOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BurstOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBurstOptionDialog dialog


CBurstOptionDialog::CBurstOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBurstOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBurstOptionDialog)
	m_fBurstDelimiter = 0.0f;
	m_nMinimumOpenning = 0;
	//}}AFX_DATA_INIT
}


void CBurstOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurstOptionDialog)
	DDX_Text(pDX, IDC_BurstDelimiter, m_fBurstDelimiter);
	DDV_MinMaxFloat(pDX, m_fBurstDelimiter, 0.f, 1.e+010f);
	DDX_Text(pDX, IDC_MinimumOpenning, m_nMinimumOpenning);
	DDV_MinMaxInt(pDX, m_nMinimumOpenning, 1, 1000000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurstOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CBurstOptionDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurstOptionDialog message handlers

float CBurstOptionDialog::GetBurstDelimiter()
{
	return m_fBurstDelimiter;
}

int CBurstOptionDialog::GetMinimumOpenning()
{
	return m_nMinimumOpenning;
}
