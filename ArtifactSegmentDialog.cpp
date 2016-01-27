// ArtifactSegmentDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "ArtifactSegmentDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArtifactSegmentDialog dialog


CArtifactSegmentDialog::CArtifactSegmentDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CArtifactSegmentDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArtifactSegmentDialog)
	m_fBeginningOfArtifact = 0.0f;
	m_fCorrectAmplitudeAtBeginning = 0.0f;
	m_fEndOfArtifact = 0.0f;
	m_fCorrectAmplitudeAtEnd = 0.0f;
	m_bDoNotConfirm = FALSE;
	//}}AFX_DATA_INIT
}


void CArtifactSegmentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArtifactSegmentDialog)
	DDX_Text(pDX, IDC_EDIT1, m_fBeginningOfArtifact);
	DDX_Text(pDX, IDC_EDIT2, m_fCorrectAmplitudeAtBeginning);
	DDX_Text(pDX, IDC_EDIT3, m_fEndOfArtifact);
	DDX_Text(pDX, IDC_EDIT4, m_fCorrectAmplitudeAtEnd);
	DDX_Check(pDX, IDC_CHECK1, m_bDoNotConfirm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArtifactSegmentDialog, CDialog)
	//{{AFX_MSG_MAP(CArtifactSegmentDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArtifactSegmentDialog message handlers

void CArtifactSegmentDialog::SetParameters(float fBeginningOfArtifact, float fCorrectAmplitudeAtBeginning, float fEndOfArtifact, float fCorrectAmplitudeAtEnd, bool bDoNotConfirm)
{
	m_fBeginningOfArtifact=fBeginningOfArtifact;
	m_fCorrectAmplitudeAtBeginning=fCorrectAmplitudeAtBeginning;
	m_fEndOfArtifact=fEndOfArtifact;
	m_fCorrectAmplitudeAtEnd=fCorrectAmplitudeAtEnd;
	m_bDoNotConfirm=bDoNotConfirm;
}

void CArtifactSegmentDialog::GetParameters(float& fBeginningOfArtifact, float& fCorrectAmplitudeAtBeginning, float& fEndOfArtifact, float& fCorrectAmplitudeAtEnd, bool& bDoNotConfirm)
{
	fBeginningOfArtifact=m_fBeginningOfArtifact;
	fCorrectAmplitudeAtBeginning=m_fCorrectAmplitudeAtBeginning;
	fEndOfArtifact=m_fEndOfArtifact;
	fCorrectAmplitudeAtEnd=m_fCorrectAmplitudeAtEnd;
	bDoNotConfirm=m_bDoNotConfirm;
}



