// SmoothingOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "SmoothingOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmoothingOptionDialog dialog


CSmoothingOptionDialog::CSmoothingOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSmoothingOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmoothingOptionDialog)
	m_fFirstPoint = 17310.0f;
	m_fLength = 100.0f;
	m_nSmoothingWindowSize = 10;
	//}}AFX_DATA_INIT
}


void CSmoothingOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmoothingOptionDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fFirstPoint);
	DDX_Text(pDX, IDC_EDIT2, m_fLength);
	DDX_Text(pDX, IDC_EDIT3, m_nSmoothingWindowSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmoothingOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CSmoothingOptionDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmoothingOptionDialog message handlers

void CSmoothingOptionDialog::GetParameters(float &fFirstPoint, float &fLength, int& nSmoothingWindowSize)
{
	fFirstPoint=m_fFirstPoint;
	fLength=m_fLength;
	nSmoothingWindowSize=m_nSmoothingWindowSize;
}
