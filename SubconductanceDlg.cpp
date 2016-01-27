// SubconductanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "SubconductanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubconductanceDlg dialog


CSubconductanceDlg::CSubconductanceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubconductanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubconductanceDlg)
	m_fAmp0 = -0.6f;
	m_fAmp1 = 3.1f;
	//}}AFX_DATA_INIT
}


void CSubconductanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubconductanceDlg)
	DDX_Text(pDX, IDC_fAmp0, m_fAmp0);
	DDX_Text(pDX, IDC_fAmp1, m_fAmp1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubconductanceDlg, CDialog)
	//{{AFX_MSG_MAP(CSubconductanceDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubconductanceDlg message handlers

void CSubconductanceDlg::GetAmps(float &fAmp0, float &fAmp1)
{
	fAmp0=m_fAmp0;
	fAmp1=m_fAmp1;
}
