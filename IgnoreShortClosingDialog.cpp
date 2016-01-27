// IgnoreShortClosingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "IgnoreShortClosingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIgnoreShortClosingDialog dialog


CIgnoreShortClosingDialog::CIgnoreShortClosingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIgnoreShortClosingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIgnoreShortClosingDialog)
	m_fCutoff = 15.0f;
	//}}AFX_DATA_INIT
}


void CIgnoreShortClosingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIgnoreShortClosingDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fCutoff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIgnoreShortClosingDialog, CDialog)
	//{{AFX_MSG_MAP(CIgnoreShortClosingDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIgnoreShortClosingDialog message handlers

float CIgnoreShortClosingDialog::GetCutoff()
{
	return m_fCutoff;
}
