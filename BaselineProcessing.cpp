// BaselineProcessing.cpp: implementation of the BaselineProcessing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BaselineProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaselineProcessing::CBaselineProcessing(float* pfData, 
									   int nLength, float fSampleInterval)
{
	m_pfData=pfData;
	m_nLength=nLength;
	m_fSampleInterval=fSampleInterval;
	m_pfCorrectedData=new float[m_nLength];
}

CBaselineProcessing::~CBaselineProcessing()
{
}

void CBaselineProcessing::BaselineCorrection(float fTime1, float fWindowSize)
{
	SetWindSize(fWindowSize);
	CalMaxDev(fTime1);
	WindowSliding();
}

void CBaselineProcessing::CalMaxDev(float fTime1)
{
	int nOffset=int(1000.*fTime1/m_fSampleInterval);
	float* pfData=m_pfData+nOffset;
	float fAve;
	float fDev;
	CalAveDev(pfData,m_nWindowSize,fAve,fDev);
	m_fMaxBaselineDev=1.2f*fDev;
}


void CBaselineProcessing::SetWindSize(float fWindowSize)
{
	m_nWindowSize=int(1000.*fWindowSize/m_fSampleInterval+0.5);
}

void CBaselineProcessing::WindowSliding()
{
	float* pfData;
	float fAve;
	float fDev;
	bool  bFirstWindow=true;
	int nSize=0;
	m_nWindowHead=0;

	while (m_nWindowHead<m_nLength)
	{
		pfData=m_pfData+m_nWindowHead;
		nSize=min(m_nWindowSize,m_nLength-m_nWindowHead);
		CalAveDev(pfData,nSize,fAve,fDev);
		if(IsBaseline(fDev))
		{
			if(bFirstWindow)
			{
				m_nFirstPoint=0;
				m_fFirstPoint=fAve;
				bFirstWindow=false;
			}
			else
			{
				m_nSecondPoint=m_nWindowHead+nSize;
				m_fSecondPoint=fAve;
				AdjustBaseline();
				m_nFirstPoint=m_nSecondPoint;
				m_fFirstPoint=m_fSecondPoint;
				m_nWindowHead+=m_nWindowSize;
			}
		}
		else
		{
			if(bFirstWindow)
			{
				m_nFirstPoint=0;
				m_fFirstPoint=fAve;
				bFirstWindow=false;
			}
			else
			{
				m_nWindowHead+=m_nWindowSize;
			}
		}
	}

	if(m_nLength-m_nFirstPoint>m_nWindowSize)
	{
		m_nSecondPoint=m_nLength;
		m_fSecondPoint=fAve;
		AdjustBaseline();
	}
}


void CBaselineProcessing::CalAveDev(float *pData, int nLength, float &fAve, float &fDev)
{
	int j;
	float s,svar;

	fAve=0.0;
	svar=0.0;
	for (j=0;j<nLength;j++)
	{
		fAve+=*(pData+j);
	}

	fAve /= nLength;

	for (j=0;j<nLength;j++) 
	{
		s=*(pData+j)-fAve;
		svar += s*s;
	}
	svar /= (nLength-1);
	fDev=float (sqrt(svar));

}

void CBaselineProcessing::AdjustBaseline()
{
	float fSloap=(m_fSecondPoint-m_fFirstPoint)/float(m_nSecondPoint-m_nFirstPoint);
	for(int i=m_nFirstPoint;i<m_nSecondPoint;i++)
	{
		*(m_pfCorrectedData+i)=*(m_pfData+i)-m_fFirstPoint-fSloap*float(i-m_nFirstPoint);
	}
}

bool CBaselineProcessing::IsBaseline(float fDev)
{
	return (fDev<m_fMaxBaselineDev);
}

void CBaselineProcessing::GetAdjustedData(float* pData)
{
	for(int i=0;i<m_nLength;i++)
	{
		*(pData+i)=*(m_pfCorrectedData+i);
	}
}

