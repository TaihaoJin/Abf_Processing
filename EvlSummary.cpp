// EvlSummary.cpp: implementation of the CEvlSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlSummary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvlSummary::CEvlSummary()
{
	m_pnLevelStart=NULL;
	m_pnLevelLength=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pfAmplitude=NULL;
	m_fRange=250000000.f;
}

void CEvlSummary::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
						short* pstEventCurrentLevel,FILE* fpSummaryFile,float fRisetime, CString sEvlFileName)
{
	m_fpSummaryFile=fpSummaryFile;
	UpdateData(NumChannels,NumLevels,TimePerPoint,nNumEvents,pnLevelStart,pnLevelLength,pfAmplitude,pstEventCurrentLevel,fRisetime,sEvlFileName);
}

void CEvlSummary::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
						short* pstEventCurrentLevel,float fRisetime,CString sEvlFileName)
{
	m_fRisetime=fRisetime;
	m_sEvlFileName=sEvlFileName;
	m_nNumLevels=NumLevels;
	m_nNumChannels=NumChannels;
	m_fTimePerPoint=TimePerPoint;
	m_nNumEvents=nNumEvents;

	delete m_pnLevelStart;
	delete m_pnLevelLength;
	delete m_pstEventCurrentLevel;
	delete m_pfAmplitude;

	m_pnLevelStart=new int[nNumEvents];
	m_pnLevelLength=new int[nNumEvents];
	m_pstEventCurrentLevel=new short[nNumEvents];
	m_pfAmplitude=new float[nNumEvents];

	for(int i=0;i<nNumEvents;i++)
	{
		*(m_pnLevelStart+i)=*(pnLevelStart+i);
		*(m_pnLevelLength+i)=*(pnLevelLength+i);
		*(m_pstEventCurrentLevel+i)=*(pstEventCurrentLevel+i);
		*(m_pfAmplitude+i)=*(pfAmplitude+i);
	}
}

CEvlSummary::~CEvlSummary()
{
	delete m_pnLevelStart;
	delete m_pnLevelLength;
	delete m_pstEventCurrentLevel;
	delete m_pfAmplitude;
	ClearSummary();
}

structEvlSummaryNode CEvlSummary::GetEvlSummary()
{
	return m_structSummary;
}

void CEvlSummary::SummarizeEvl()
{
	int nLengthCutoff=2*(int(m_fRisetime*1000./m_fTimePerPoint)+1);
	int nNumAmpEvents=0;
	float fRecordLength=0.f,fOpenTime=0.f;
	int i,nLevel,nMaxLevel,nTotalOpening;
	vector <int> vectnNumAmp;

	for(i=0;i<m_nNumLevels;i++)
	{
		m_structSummary.vectfResidentTime.push_back(0.f);
		m_structSummary.vectnNumEvents.push_back(0);
		m_structSummary.vectfAmplitude.push_back(0.f);
		vectnNumAmp.push_back(0);
	}
	
	m_structSummary.nNumLevels=m_nNumLevels;
	nMaxLevel=-1;
	nTotalOpening=0;
	int nLevel0=0,nLength=0;
	float fAmp=0.f;
	fRecordLength=0.f;
	i=0;
	while(i<m_nNumEvents&&fRecordLength<=m_fRange)
	{
		nLevel=*(m_pstEventCurrentLevel+i);	
		m_structSummary.vectnNumEvents[nLevel]++;
		m_structSummary.vectfResidentTime[nLevel]+=float(*(m_pnLevelLength+i))*m_fTimePerPoint/1000.f;
		fRecordLength+=float(*(m_pnLevelLength+i))*m_fTimePerPoint/1000.f;
		if(nLevel>nMaxLevel)
		{
			nMaxLevel=nLevel;
		}
		if(nLevel>nLevel0)
		{
			nTotalOpening+=nLevel-nLevel0;
		}

		nLevel0=nLevel;
		nLength=*(m_pnLevelLength+i);

		if(nLength>nLengthCutoff)
		{
			fAmp=*(m_pfAmplitude+i);
			m_structSummary.vectfAmplitude[nLevel]+=float(nLength)*fAmp;
			vectnNumAmp[nLevel ]+=nLength;
		}
		i++;
	}


	fRecordLength=0.f;
	for(i=0;i<m_nNumLevels;i++)
	{
		fRecordLength+=m_structSummary.vectfResidentTime[i];
		fOpenTime+=float(i)*m_structSummary.vectfResidentTime[i];
		if(vectnNumAmp[i]>0)
		{
			m_structSummary.vectfAmplitude[i]/=float(vectnNumAmp[i]);
		}
		else
		{
			m_structSummary.vectfAmplitude[i]=99.99f;
		}
	}
	if(m_nNumChannels<0)
	{
		m_nNumChannels=nMaxLevel;
	}
	m_structSummary.fOpenProb=fOpenTime/(fRecordLength*m_nNumChannels);
	m_structSummary.fMeanOpenTime=fOpenTime/float(nTotalOpening);
	m_structSummary.fRecordLength=fRecordLength;
	m_structSummary.nMaxLevel=nMaxLevel;
	m_structSummary.nNumChannels=m_nNumChannels;
	m_structSummary.nNumEvents=m_nNumEvents;
	m_structSummary.sFileName=m_sEvlFileName;
	
	float fProbSingle=(1.f-m_structSummary.fOpenProb)/(1.f-0.5f*m_structSummary.fOpenProb);

	m_structSummary.fProbSingle=Exponential(fProbSingle,m_structSummary.vectnNumEvents[1]);
}

void CEvlSummary::ClearSummary()
{
	m_structSummary.vectfResidentTime.clear();
	m_structSummary.vectnNumEvents.clear();
}

void CEvlSummary::OutputSummary()
{
	fprintf(m_fpSummaryFile,"\n%s:\n",m_sEvlFileName);
	fprintf(m_fpSummaryFile,"Total Record Length (s): %10.4f  NumEvents:  %8d  nMaxLevel: %d\n",
		m_structSummary.fRecordLength,m_structSummary.nNumEvents,m_structSummary.nMaxLevel);
	fprintf(m_fpSummaryFile,"Po: %8.4f,   To (ms): %8.1f,  1-Prob_Sigle: %12.4e\n",
		m_structSummary.fOpenProb,m_structSummary.fMeanOpenTime,
		m_structSummary.fProbSingle);
	for(int i=0; i<m_structSummary.nMaxLevel+1;i++)
	{
		fprintf(m_fpSummaryFile,"Level%d: To(ms): %10.1f Amp: %5.2f Po: %10.8f  Po (bino): %10.8f  NumEvents: %8d\n",
			i,
			m_structSummary.vectfResidentTime[i]/float(m_structSummary.vectnNumEvents[i]),
			m_structSummary.vectfAmplitude[i],
			m_structSummary.vectfResidentTime[i]/m_structSummary.fRecordLength,
			Binomial(m_structSummary.fOpenProb,m_structSummary.nMaxLevel,i),
			m_structSummary.vectnNumEvents[i]);
	}
}
void CEvlSummary::OutputSummary(int nCount)
{
	fprintf(m_fpSummaryFile,"\n%3d  %s:\n",nCount,m_sEvlFileName);
	fprintf(m_fpSummaryFile,"Total Record Length (s): %10.4f  NumEvents:  %8d  nMaxLevel: %d   nNumChannels:  %d\n",
		m_structSummary.fRecordLength,m_structSummary.nNumEvents,m_structSummary.nMaxLevel,m_structSummary.nNumChannels);
	fprintf(m_fpSummaryFile,"Po: %8.4f,   To (ms): %8.1f,  1-Prob_Sigle: %12.4e\n",
		m_structSummary.fOpenProb,m_structSummary.fMeanOpenTime,
		m_structSummary.fProbSingle);
	for(int i=0; i<m_structSummary.nMaxLevel+1;i++)
	{
		fprintf(m_fpSummaryFile,"Level%d: To(ms): %10.1f Amp: %5.2f Po: %10.8f  Po (bino): %10.8f  NumEvents: %8d\n",
			i,
			m_structSummary.vectfResidentTime[i]/float(m_structSummary.vectnNumEvents[i]),
			m_structSummary.vectfAmplitude[i],
			m_structSummary.vectfResidentTime[i]/m_structSummary.fRecordLength,
			Binomial(m_structSummary.fOpenProb,m_structSummary.nMaxLevel,i),
			m_structSummary.vectnNumEvents[i]);
	}
}

float CEvlSummary::Binomial(float fProb, int N, int I)
{
	float fBinomialProb=0.f;
	fBinomialProb=Factorial(N)/Factorial(I);
	fBinomialProb/=Factorial(N-I);
	fBinomialProb*=Exp(fProb,I);
	fBinomialProb*=Exp(1.f-fProb,N-I);
	return fBinomialProb;
}

float CEvlSummary::Factorial(int N)
{
	float fVal=1.f;
	for(int i=1;i<=N;i++)
	{
		fVal*=float(i);
	}
	return fVal;
}

float CEvlSummary::Exp(float X, int N)
{
	float fVal=1.f;
	for(int i=0;i<N;i++)
	{
		fVal*=X;
	}
	return fVal;
}

structEvlSummaryNode CEvlSummary::GetSummaryNode()
{
	return m_structSummary;
}

float CEvlSummary::Exponential(float X, int N)
{
	float fExp=1.f;
	for(int i=0;i<N;i++)
	{
		fExp*=X;
	}

	return fExp;
}
