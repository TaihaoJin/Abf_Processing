// BallRollingOption.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BallRollingOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBallRollingOption dialog


CBallRollingOption::CBallRollingOption(CWnd* pParent /*=NULL*/)
	: CDialog(CBallRollingOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBallRollingOption)
	m_fWindowSize = 100.0f;
	m_nRank = 2;
	m_fCriterionOfUpTransitions = 2.2f;
	m_nLevelHistWindowSize = 20;
	m_nMaxNumOfSuperiors = 3;
	m_bSmoothing = FALSE;
	m_bCheckContactingPoints = FALSE;
	m_bMinima=false;
	m_bMaxima=TRUE;
	m_nAnchorRank = 2;
	m_fTime1 = 0.0f;
	m_fTime2 = 0.0f;
	m_nDelta = 0;  //When choose the m_nRank-th lagerst point, the i-th largest point must be at least nDelta local maximum points away from the (i-1)-th largest point. 
	m_bApplyingToEntireTrace=TRUE;
	m_fAmp = 2.0f;
	//}}AFX_DATA_INIT
}


void CBallRollingOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBallRollingOption)
	DDX_Text(pDX, IDC_EDIT1, m_fWindowSize);
	DDV_MinMaxFloat(pDX, m_fWindowSize, 0.f, 1.e+022f);
	DDX_Text(pDX, IDC_EDIT2, m_nRank);
	DDV_MinMaxInt(pDX, m_nRank, 1, 2147483647);
	DDX_Text(pDX, IDC_EDIT3, m_fCriterionOfUpTransitions);
	DDV_MinMaxFloat(pDX, m_fCriterionOfUpTransitions, 0.f, 1.e+010f);
	DDX_Text(pDX, IDC_EDIT4, m_nLevelHistWindowSize);
	DDX_Text(pDX, IDC_EDIT5, m_nMaxNumOfSuperiors);
	DDX_Check(pDX, IDC_CHECK1, m_bSmoothing);
	DDX_Check(pDX, IDC_CHECK2, m_bCheckContactingPoints);
	DDX_Text(pDX, IDC_EDIT6, m_nAnchorRank);
	DDX_Text(pDX, IDC_EDIT7, m_fTime1);
	DDX_Text(pDX, IDC_EDIT8, m_fTime2);
	DDX_Text(pDX, IDC_EDIT9, m_nDelta);
	DDX_Text(pDX, IDC_EDIT10, m_fAmp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBallRollingOption, CDialog)
	//{{AFX_MSG_MAP(CBallRollingOption)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBallRollingOption message handlers

float CBallRollingOption::GetWindowSize()
{
	return m_fWindowSize;
}

int CBallRollingOption::GetRank()
{
	return m_nRank;
}

float CBallRollingOption::GetCriterionOfUpTransitions()
{
	return m_fCriterionOfUpTransitions;
}

int CBallRollingOption::GetMaxNumOfSuperiors()
{
	return m_nMaxNumOfSuperiors;
}

int CBallRollingOption::GetLevelHistWindowSize()
{
	return m_nLevelHistWindowSize;
}

bool CBallRollingOption::CheckContactingPoints()
{
	return m_bCheckContactingPoints;
}

bool CBallRollingOption::SmoothingTrace()
{
	return m_bSmoothing;
}

void CBallRollingOption::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_bApplyingToEntireTrace=true;	
}

void CBallRollingOption::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_bApplyingToEntireTrace=false;
}

int CBallRollingOption::GetAnchorRank()
{
	return m_nAnchorRank;
}

float CBallRollingOption::GetfTime1()
{
	return m_fTime1;
}

float CBallRollingOption::GetfTime2()
{
	return m_fTime2;
}

bool CBallRollingOption::ApplyingToEntireTrace()
{
	return m_bApplyingToEntireTrace;
}


void CBallRollingOption::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_bMinima=true;
}

void CBallRollingOption::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_bMaxima=true;
}

bool CBallRollingOption::TracingMinima()
{
	return m_bMinima;
}

int CBallRollingOption::GetnDelta()
{
	return m_nDelta;
}

float CBallRollingOption::GetAmp()
{
	return m_fAmp;
}
