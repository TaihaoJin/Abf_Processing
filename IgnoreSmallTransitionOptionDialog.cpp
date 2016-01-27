// IgnoreSmallTransitionOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "IgnoreSmallTransitionOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIgnoreSmallTransitionOptionDialog dialog


CIgnoreSmallTransitionOptionDialog::CIgnoreSmallTransitionOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CIgnoreSmallTransitionOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIgnoreSmallTransitionOptionDialog)
	m_fCutoff = 1.0f;
	//}}AFX_DATA_INIT
}


void CIgnoreSmallTransitionOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIgnoreSmallTransitionOptionDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fCutoff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIgnoreSmallTransitionOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CIgnoreSmallTransitionOptionDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIgnoreSmallTransitionOptionDialog message handlers

float CIgnoreSmallTransitionOptionDialog::GetCutoff()
{
	return m_fCutoff;
}
