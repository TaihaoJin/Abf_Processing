// BlineAdjustOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BlineAdjustOptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlineAdjustOptionDialog dialog


CBlineAdjustOptionDialog::CBlineAdjustOptionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBlineAdjustOptionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlineAdjustOptionDialog)
	m_fFirstPoint = 0.0f;
	m_fWindowSize = 0.0f;
	//}}AFX_DATA_INIT
}


void CBlineAdjustOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlineAdjustOptionDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fFirstPoint);
	DDV_MinMaxFloat(pDX, m_fFirstPoint, 0.f, 1.e+013f);
	DDX_Text(pDX, IDC_EDIT2, m_fWindowSize);
	DDV_MinMaxFloat(pDX, m_fWindowSize, 0.1f, 1.e+011f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlineAdjustOptionDialog, CDialog)
	//{{AFX_MSG_MAP(CBlineAdjustOptionDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlineAdjustOptionDialog message handlers

float CBlineAdjustOptionDialog::GetFirstPoint()
{
	return m_fFirstPoint;
}

float CBlineAdjustOptionDialog::GetWindowSize()
{
	return m_fWindowSize;
}
