// OneDHistogram.cpp: implementation of the COneDHistogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "OneDHistogram.h"
#include "math.h"
#include <vector>


using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COneDHistogram::COneDHistogram()
{

}

COneDHistogram::~COneDHistogram()
{

}


void COneDHistogram::CAllPointHistgram(vector <float> vfData, float fInterval)
{
	m_nLength=vfData.size();
	m_vfData=vfData;
	m_fInterval=fInterval;
	CalHist();	
}

structOneDHistogramNode COneDHistogram::GetHistogram(vector <float> vfData, float fInterval)
{
	m_nLength=vfData.size();
	m_vfData=vfData;
	m_fInterval=fInterval;
	CalHist();	
	return GetHistgram();
}

void COneDHistogram::CalHist()
{
	float fAmp=0.;
	int it;
	CalRange();
	InitializeHist();
	m_fMean=0.f;
	for(int i=0; i<m_nLength; i++)
	{
		fAmp=m_vfData[i];
		it=int((fAmp-m_fMin)/m_fInterval);
		m_vfHist[it]+=1.;
		m_fMean+=fAmp;
	}
	m_fMean/=float(m_nLength);
}

void COneDHistogram::CalRange()
{
	float fAmp;
	m_fMax=-1000000000.f;
	m_fMin=10000000000.f;
	for(int i=0;i<m_nLength;i++)
	{
		fAmp=m_vfData[i];
		if(fAmp>m_fMax)
		{
			m_fMax=fAmp;
		}
		if(fAmp<m_fMin)
		{
			m_fMin=fAmp;
		}
	}

	m_nSize=int((m_fMax-m_fMin)/m_fInterval+1.f);
}

void COneDHistogram::InitializeHist()
{
	m_vfHist.clear();
	for(int i=0;i<m_nSize;i++)
	{
		m_vfHist.push_back(0.f);
	}
}

int COneDHistogram::GetHistSize()
{
	return m_nSize;
}

float COneDHistogram::GetMinimum()
{
	return m_fMin;
}

structOneDHistogramNode COneDHistogram::GetHistgram()
{
	structOneDHistogramNode aNode;
	aNode.fInterval=m_fInterval;
	aNode.fMin=m_fMin;
	float fSum=0.f;
	float fTotalCounts=0.;
	float fCounts=0.f;
	int n5_thIndex=0;
	int nBins=0;
	for(int i=0;i<m_nSize;i++)
	{
		fCounts=m_vfHist[i];

		fSum+=fCounts;
		aNode.vfHist.push_back(fCounts);
		aNode.vfHist_Sum.push_back(fSum);
		if(fCounts>0.1)
				nBins++;
		if(nBins==4)
			n5_thIndex=i;
	}
	aNode.fTotalCounts=fSum;
	aNode.fMean=m_fMean;
	aNode.f5_thBin=m_fMin+4*m_fInterval;
	return aNode;
}
