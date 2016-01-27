// SegDeletOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "SegDeletOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegDeletOptionDialog dialog


CSegDeletOptionDialog::CSegDeletOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSegDeletOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSegDeletOptionDialog)
	m_fBegin = 0.0f;
	m_fEnd = 0.0f;
	m_fAmpBegin = 999999999990.0f;
	m_fAmpEnd = 999999999990.0f;
	m_bDone=false;
	//}}AFX_DATA_INIT
}


void CSegDeletOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegDeletOptionDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fBegin);
	DDX_Text(pDX, IDC_EDIT2, m_fEnd);
	DDX_Text(pDX, IDC_EDIT3, m_fAmpBegin);
	DDX_Text(pDX, IDC_EDIT4, m_fAmpEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSegDeletOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CSegDeletOptionDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegDeletOptionDialog message handlers

void CSegDeletOptionDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_bDone=true;
	
}

void CSegDeletOptionDialog::GetParameters(float &fBegin, float &fAmpBegin, float &fEnd, float &fAmpEnd, bool &bDone)
{
	fBegin=m_fBegin;
	fEnd=m_fEnd;
	fAmpBegin=m_fAmpBegin;
	fAmpEnd=m_fEnd;
	bDone=m_bDone;
}
