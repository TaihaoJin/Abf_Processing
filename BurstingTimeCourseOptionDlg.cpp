// BurstingTimeCourseOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BurstingTimeCourseOptionDlg.h"
#include "FileNameHandling.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBurstingTimeCourseOptionDlg dialog


CBurstingTimeCourseOptionDlg::CBurstingTimeCourseOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBurstingTimeCourseOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBurstingTimeCourseOptionDlg)
	m_fBurstDelimiter = 10.0f;
	m_nBinSize = 1;
	m_nMaxOpening = 99999999;
	m_nMinOpening = 0;
	m_fClusterDelimiter = 0.0f;
	m_nSortingIndex = 0;
	m_fBinWidth = 5.0f;
	m_fLatancy = 0.0f;
	m_nLatancy = 0;
	m_nSubLevelI = 0;
	m_nSubLevelF = 20;
	m_sFilteringKey = _T("-1");
	m_fMinIBC = 0.0f;
	m_fMaxIBC = 9999999.0f;
	m_fMinBurstDuration = 0.0f;
	m_fMaxBurstDuration = 999999999.0f;
	m_fMinCTo = 0.0f;
	m_fMaxCTo = 99999999.0f;
	m_nMaxNOPC = 9999999;
	m_nMinNOPC = 0;
	//}}AFX_DATA_INIT
}


void CBurstingTimeCourseOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurstingTimeCourseOptionDlg)
	DDX_Text(pDX, IDC_EDIT1, m_fBurstDelimiter);
	DDX_Text(pDX, IDC_EDIT2, m_nBinSize);
	DDX_Text(pDX, IDC_MaxOpening, m_nMaxOpening);
	DDX_Text(pDX, IDC_MinOpening, m_nMinOpening);
	DDX_Text(pDX, IDC_EDIT4_ClusterDelimiter, m_fClusterDelimiter);
	DDX_Text(pDX, IDC_EDIT3, m_nSortingIndex);
	DDX_Text(pDX, IDC_EDIT4, m_fBinWidth);
	DDX_Text(pDX, IDC_EDIT5, m_fLatancy);
	DDX_Text(pDX, IDC_EDIT6, m_nLatancy);
	DDX_Text(pDX, IDC_EDIT8, m_nSubLevelI);
	DDX_Text(pDX, IDC_EDIT9, m_nSubLevelF);
	DDX_Text(pDX, IDC_EDIT7, m_sFilteringKey);
	DDX_Text(pDX, IDC_EDIT10, m_fMinIBC);
	DDX_Text(pDX, IDC_EDIT11, m_fMaxIBC);
	DDX_Text(pDX, IDC_EDIT12, m_fMinBurstDuration);
	DDX_Text(pDX, IDC_EDIT13, m_fMaxBurstDuration);
	DDX_Text(pDX, IDC_EDIT14, m_fMinCTo);
	DDX_Text(pDX, IDC_EDIT15, m_fMaxCTo);
	DDX_Text(pDX, IDC_EDIT17, m_nMaxNOPC);
	DDX_Text(pDX, IDC_EDIT16, m_nMinNOPC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurstingTimeCourseOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CBurstingTimeCourseOptionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurstingTimeCourseOptionDlg message handlers

void CBurstingTimeCourseOptionDlg::SetParameters(structBurstingTimeCourseOptionNode structParNode)
{
	m_fBurstDelimiter=structParNode.fBurstDelimiter;
	m_fClusterDelimiter=structParNode.fClusterDelimiter;
	m_nBinSize=structParNode.nBinSize;
	m_nMaxOpening=structParNode.nMaxOpening;
	m_nMinOpening=structParNode.nMinOpening;
	m_nSortingIndex=structParNode.nSortingIndex;
	m_fBinWidth=structParNode.fBinWidth/1000.;
	m_nLatancy=structParNode.nLatancy;
	m_fLatancy=structParNode.fLatancy;
	m_nFilteringKey=structParNode.nFilteringKey;
	m_nSubLevelI=structParNode.nSubConductanceLevelI;
	m_nSubLevelF=structParNode.nSubConductanceLevelF;
	m_sFilteringKey=structParNode.sFilteringKey;
	m_fMinIBC=structParNode.fMinIBC;
	m_fMaxIBC=structParNode.fMaxIBC;
	m_fMinBurstDuration=structParNode.fMinBurstDuration;
	m_fMaxBurstDuration=structParNode.fMaxBurstDuration;
	m_fMinCTo=structParNode.fMinCTo;
	m_fMaxCTo=structParNode.fMaxCTo;
	m_nMinNOPC=structParNode.nMinNOPC;
	m_nMaxNOPC=structParNode.nMaxNOPC;
}

void CBurstingTimeCourseOptionDlg::getBurstTimecourseOptions(structBurstingTimeCourseOptionNode &structParNode)
{
	structParNode.fBurstDelimiter=m_fBurstDelimiter;
	structParNode.fClusterDelimiter=m_fClusterDelimiter;
	structParNode.nBinSize=m_nBinSize;
	structParNode.nMaxOpening=m_nMaxOpening;
	structParNode.nMinOpening=m_nMinOpening;
	structParNode.nSortingIndex=m_nSortingIndex;
	structParNode.fBinWidth=1000.*m_fBinWidth;
	structParNode.nLatancy=m_nLatancy;
	structParNode.fLatancy=m_fLatancy;
	structParNode.nFilteringKey=m_nFilteringKey;
	structParNode.nSubConductanceLevelI=m_nSubLevelI;
	structParNode.nSubConductanceLevelF=m_nSubLevelF;
	IntegerReader(m_sFilteringKey, structParNode.vnFilteringKeys);
	structParNode.nNumFilteringKeys=structParNode.vnFilteringKeys.size();
	structParNode.fMinCTo=m_fMinCTo;
	structParNode.fMaxCTo=m_fMaxCTo;
	structParNode.nMinNOPC=m_nMinNOPC;
	structParNode.nMaxNOPC=m_nMaxNOPC;
	structParNode.fMinBurstDuration=m_fMinBurstDuration;
	structParNode.fMaxBurstDuration=m_fMaxBurstDuration;
	structParNode.bSet=true;
	structParNode.fMinIBC=m_fMinIBC;
	structParNode.fMaxIBC=m_fMaxIBC;


	int keys=structParNode.vnFilteringKeys.size();
	if(keys>0)
		structParNode.nFilteringKey=structParNode.vnFilteringKeys[0];
	else
		structParNode.nFilteringKey=-1;
}
void CBurstingTimeCourseOptionDlg::initialize(structBurstingTimeCourseOptionNode &structParNode)
{
	m_fBurstDelimiter = 10.0f;
	m_nBinSize = 1;
	m_nMaxOpening = 9999999;
	m_nMinOpening = 0;
	m_fClusterDelimiter = 0.0f;
	m_nSortingIndex = 0;
	m_fBinWidth = 5.0f;
	m_fLatancy = 0.0f;
	m_nLatancy = 0;
	m_nSubLevelI = 0;
	m_nSubLevelF = 20;
	m_sFilteringKey = "-1";
	m_fMinIBC = 0.0f;
	m_fMaxIBC = 99999999.0f;
	m_fMinBurstDuration = 0.0f;
	m_fMaxBurstDuration = 999999999.0f;
	m_fMinCTo = 0.0f;
	m_fMaxCTo = 99999999.0f;
	m_nMaxNOPC = 999999;
	m_nMinNOPC = 0;
	structParNode.fBurstDelimiter=m_fBurstDelimiter;
	structParNode.fClusterDelimiter=m_fClusterDelimiter;
	structParNode.nBinSize=m_nBinSize;
	structParNode.nMaxOpening=m_nMaxOpening;
	structParNode.nMinOpening=m_nMinOpening;
	structParNode.nSortingIndex=m_nSortingIndex;
	structParNode.fBinWidth=1000.*m_fBinWidth;
	structParNode.nLatancy=m_nLatancy;
	structParNode.fLatancy=m_fLatancy;
	structParNode.nFilteringKey=m_nFilteringKey;
	structParNode.nSubConductanceLevelI=m_nSubLevelI;
	structParNode.nSubConductanceLevelF=m_nSubLevelF;
	IntegerReader(m_sFilteringKey, structParNode.vnFilteringKeys);
	structParNode.nNumFilteringKeys=structParNode.vnFilteringKeys.size();
	structParNode.fMinIBC=m_fMinIBC;
	structParNode.fMaxIBC=m_fMaxIBC;
	structParNode.fMinCTo=m_fMinCTo;
	structParNode.fMaxCTo=m_fMaxCTo;
	structParNode.nMinNOPC=m_nMinNOPC;
	structParNode.nMaxNOPC=m_nMaxNOPC;
	structParNode.bSet=false;
	structParNode.fMinBurstDuration=m_fMinBurstDuration;
	structParNode.fMaxBurstDuration=m_fMaxBurstDuration;
	int keys=structParNode.vnFilteringKeys.size();
	if(keys>0)
		structParNode.nFilteringKey=structParNode.vnFilteringKeys[0];
	else
		structParNode.nFilteringKey=-1;
}

void CBurstingTimeCourseOptionDlg::IntegerReader(CString st, vector<int> &vnData)
{
	vnData.clear();
	int len=st.GetLength(),length;
	int i,j,digits,factor,it;
	char c,c0;
	int index=0;
	bool bIntegerRead=false;
	length=0;
	c0=st[0];
	vector <int> vnDigits,keys;
	while(index<len){
		vnDigits.clear();
		bIntegerRead=false;
		c=st[index];
		while(c<'0'||c>'9'){
			index++;
			if(index>=len) break;
			c0=c;
			c=st[index];
		}
		digits=0;
		while(c>='0'&&c<='9'){
			if(!bIntegerRead) bIntegerRead=true;
			vnDigits.push_back(c-'0');
			digits++;
			index++;
			if(index>=len) break;
			c=st[index];
		}	
		if(bIntegerRead){
			factor=1;
			it=0;
			for(i=0;i<digits;i++){
				j=digits-1-i;
				it+=factor*vnDigits[j];
				factor*=10;
			}
			if(c0=='-') it=-it;
			vnData.push_back(it);
			length++;
		}
	}
}

CString CBurstingTimeCourseOptionDlg::getParameterString()
{
	return "To be implemented";
}

CString CBurstingTimeCourseOptionDlg::getBurstEvolutionParametersAsString()
{
	CFileNameHandling cfh;
	structBurstingTimeCourseOptionNode initNode;
	CBurstingTimeCourseOptionDlg bDlg;
	bDlg.initialize(initNode);
	CString s;
	s+=" Tcb ";
	s+=cfh.ToString(m_fBurstDelimiter,2);
	if(m_fClusterDelimiter>m_fBurstDelimiter){
		s+=" Tcc ";
		s+=cfh.ToString(m_fClusterDelimiter,2);
	}
	if(m_nMinOpening!=initNode.nMinOpening){
		s+=" minNOPB ";
		s+=cfh.ToString(m_nMinOpening);
	}
	if(m_fMinBurstDuration!=initNode.fMinBurstDuration){
		s+=" minBTo ";
		s+=cfh.ToString(m_fMinBurstDuration,1);
	}
	if(m_fClusterDelimiter>m_fBurstDelimiter){
		s+=" minCTo ";
		s+=cfh.ToString(m_fMinCTo,1);
		s+=" minNOPC ";
		s+=cfh.ToString(m_nMinNOPC);
	}
	if(m_fMinIBC!=initNode.fMinIBC){
		s+=" minIBC ";
		s+=cfh.ToString(m_fMinIBC,1);
	}
	if(m_fMaxIBC!=initNode.fMaxIBC){
		s+=" maxIBC ";
		s+=cfh.ToString(m_fMaxIBC,1);
	}
	return s;
}
