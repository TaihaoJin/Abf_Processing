// AllPointHistgramDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "AllPointHistgramDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllPointHistgramDialog dialog


CAllPointHistgramDialog::CAllPointHistgramDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAllPointHistgramDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllPointHistgramDialog)
	m_fFirstPoint = 0.0f;
	m_fLength = 0.0f;
	m_nReductionFactor = 1;
	m_fTransitionThreshold = 1.5f;
	m_nPolarity=-1;
	//}}AFX_DATA_INIT
}


void CAllPointHistgramDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllPointHistgramDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fFirstPoint);
	DDV_MinMaxFloat(pDX, m_fFirstPoint, -1.e+020f, 1.e+021f);
	DDX_Text(pDX, IDC_EDIT2, m_fLength);
	DDV_MinMaxFloat(pDX, m_fLength, -1.e+020f, 1.e+020f);
	DDX_Text(pDX, IDC_EDIT3, m_nReductionFactor);
	DDX_Text(pDX, IDC_EDIT4, m_fTransitionThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAllPointHistgramDialog, CDialog)
	//{{AFX_MSG_MAP(CAllPointHistgramDialog)
	ON_BN_CLICKED(IDC_NegativeCurrent, OnNegativeCurrent)
	ON_BN_CLICKED(IDC_PositiveCurrent, OnPositiveCurrent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAllPointHistgramDialog message handlers

float CAllPointHistgramDialog::GetFirstPoint()
{
	if(m_fFirstPoint>=0)
	{
		return m_fFirstPoint;
	}
	else
	{
		return -m_fFirstPoint;
	}
}

float CAllPointHistgramDialog::GetSegmentLength()
{
	return m_fLength;
}

bool CAllPointHistgramDialog::ShowTrace()
{
	if(m_fFirstPoint>=0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CAllPointHistgramDialog::GetReductionFactor()
{
	return m_nReductionFactor;
}

void CAllPointHistgramDialog::OnNegativeCurrent() 
{
	// TODO: Add your control notification handler code here
	int m_nPolarity=-1;	
}

void CAllPointHistgramDialog::OnPositiveCurrent() 
{
	// TODO: Add your control notification handler code here
	int m_nPolarity=1;
}

int CAllPointHistgramDialog::GetPolarity()
{
	return m_nPolarity;
}

float CAllPointHistgramDialog::GetTransitThreshold()
{
	return m_fTransitionThreshold;
}
