// EvlComment.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlComment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvlComment dialog


CEvlComment::CEvlComment(CWnd* pParent /*=NULL*/)
	: CDialog(CEvlComment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvlComment)
	m_sComment = _T("Channels:1");
	//}}AFX_DATA_INIT
}


void CEvlComment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvlComment)
	DDX_Text(pDX, IDC_EDIT1, m_sComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEvlComment, CDialog)
	//{{AFX_MSG_MAP(CEvlComment)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvlComment message handlers

CString CEvlComment::GetComment()
{
    return m_sComment;
}
