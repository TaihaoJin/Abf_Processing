// GettingFittedParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "GettingFittedParametersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGettingFittedParametersDlg dialog


CGettingFittedParametersDlg::CGettingFittedParametersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGettingFittedParametersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGettingFittedParametersDlg)
	m_sFittedParameter = _T("TimeConstantsAndWeights");
	m_nFittedLevel = 0;
	m_nNumComponents = 5;
	m_nSigNumComponentsL = 0;
	m_nSigNumComponentsH = 20;
	m_bOutputConfidanceRanges = FALSE;
	//}}AFX_DATA_INIT
}


void CGettingFittedParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGettingFittedParametersDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sFittedParameter);
	DDX_Text(pDX, IDC_EDIT2, m_nFittedLevel);
	DDX_Text(pDX, IDC_EDIT3, m_nNumComponents);
	DDX_Text(pDX, IDC_EDIT4, m_nSigNumComponentsL);
	DDX_Text(pDX, IDC_EDIT5, m_nSigNumComponentsH);
	DDX_Check(pDX, IDC_CHECK1, m_bOutputConfidanceRanges);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGettingFittedParametersDlg, CDialog)
	//{{AFX_MSG_MAP(CGettingFittedParametersDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGettingFittedParametersDlg message handlers

structFittedParameterNode CGettingFittedParametersDlg::GetFittedParameter()
{
	structFittedParameterNode aNode;
	aNode.sFittedParameter=m_sFittedParameter;
	aNode.nFittedLevel=m_nFittedLevel;
	aNode.nNumComponents=m_nNumComponents;
	aNode.nSigComponentsH=m_nSigNumComponentsH;
	aNode.nSigComponentsL=m_nSigNumComponentsL;
	aNode.bOutputConfidanceRanges=m_bOutputConfidanceRanges;
	return aNode;
}
