// ArtifactHanddlingOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "ArtifactHanddlingOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArtifactHanddlingOptionDlg dialog


CArtifactHanddlingOptionDlg::CArtifactHanddlingOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArtifactHanddlingOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArtifactHanddlingOptionDlg)
	m_fAmplitudeThreshold = -15.0f;
	m_fDeltaThreshold = -3.5f;
	m_fOppositeDirectionThreshold = 0.5f;
	m_fRange = 0.7f;
	m_bCheckDelta = TRUE;
	m_bCheckOppositeDirrection = TRUE;
	m_bCheckAmplitude = TRUE;
	m_bBasedOnAdjustedData = TRUE;
	m_fTime1 = 0.0f;
	m_fTime2 = 0.0f;
	m_bCheckEvl = FALSE;
	m_bReadFromFile = FALSE;
	m_bCheckHightAndWidthRange = FALSE;
	m_fHighthreshold = 0.0f;
	m_fMinWidth = 0.0f;
	m_fMaxWidth = 0.0f;
	//}}AFX_DATA_INIT
}


void CArtifactHanddlingOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArtifactHanddlingOptionDlg)
	DDX_Text(pDX, IDC_EDIT1, m_fAmplitudeThreshold);
	DDV_MinMaxFloat(pDX, m_fAmplitudeThreshold, -1.e+011f, 1.e+011f);
	DDX_Text(pDX, IDC_EDIT2, m_fDeltaThreshold);
	DDV_MinMaxFloat(pDX, m_fDeltaThreshold, -1.e+011f, 1.e+011f);
	DDX_Text(pDX, IDC_EDIT3, m_fOppositeDirectionThreshold);
	DDV_MinMaxFloat(pDX, m_fOppositeDirectionThreshold, -1.e+006f, 1.e+007f);
	DDX_Text(pDX, IDC_EDIT4, m_fRange);
	DDV_MinMaxFloat(pDX, m_fRange, -1.e+011f, 1.e+011f);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckDelta);
	DDX_Check(pDX, IDC_CHECK2, m_bCheckOppositeDirrection);
	DDX_Check(pDX, IDC_CHECK3, m_bCheckAmplitude);
	DDX_Check(pDX, IDC_CHECK4, m_bBasedOnAdjustedData);
	DDX_Text(pDX, IDC_EDIT5, m_fTime1);
	DDX_Text(pDX, IDC_EDIT6, m_fTime2);
	DDX_Check(pDX, IDC_CheckEvl, m_bCheckEvl);
	DDX_Check(pDX, IDC_ReadFromAFile, m_bReadFromFile);
	DDX_Check(pDX, IDC_CHECK5, m_bCheckHightAndWidthRange);
	DDX_Text(pDX, IDC_EDIT7, m_fHighthreshold);
	DDX_Text(pDX, IDC_EDIT8, m_fMinWidth);
	DDX_Text(pDX, IDC_EDIT9, m_fMaxWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArtifactHanddlingOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CArtifactHanddlingOptionDlg)
	ON_BN_CLICKED(IDC_ApplyingOnlyBetweenT1AndT, OnApplyingOnlyBetweenT1AndT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArtifactHanddlingOptionDlg message handlers

void CArtifactHanddlingOptionDlg::GetThresholds(float &fAmplitudeThreshold, float &fDeltaThreshold, float &fOppositeDirectionThreshold, float &fRange)
{
	fAmplitudeThreshold=m_fAmplitudeThreshold;
	fDeltaThreshold=m_fDeltaThreshold;
	fOppositeDirectionThreshold=m_fOppositeDirectionThreshold;
	fRange=m_fRange;
}

void CArtifactHanddlingOptionDlg::GetBoolParameters(bool &bCheckDelta, 
													bool &bCheckOppositeDirrection, 
													bool &bCheckAmplitude,
													bool &bBasedOnAdjustedData,
													bool &bEntireTrace,
													float &fTime1,
													float &fTime2,
													bool &bCheckEvl,
													bool  &bReadFromFile)
{
	bReadFromFile=m_bReadFromFile;
	bCheckDelta=m_bCheckDelta;
	bCheckOppositeDirrection=m_bCheckOppositeDirrection;
	bCheckAmplitude=m_bCheckAmplitude;
	bBasedOnAdjustedData=m_bBasedOnAdjustedData;
	fTime1=m_fTime1;
	fTime2=m_fTime2;
	bEntireTrace=m_bEntireTrace;
	bCheckEvl=m_bCheckEvl;
}

void CArtifactHanddlingOptionDlg::OnApplyingOnlyBetweenT1AndT() 
{
	// TODO: Add your control notification handler code here
	m_bEntireTrace=false;	
}
