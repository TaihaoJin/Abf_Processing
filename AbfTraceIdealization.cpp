// AbfTraceIdealization.cpp: implementation of the CAbfTraceIdealization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "AbfTraceIdealization.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbfTraceIdealization::CAbfTraceIdealization()
{
	m_stCurrentLevels[0][0]=0;
	m_stCurrentLevels[0][1]=0;
	m_stCurrentLevels[0][2]=1;
	m_stCurrentLevels[0][3]=2;
	m_stCurrentLevels[1][0]=3;
	m_stCurrentLevels[1][1]=0;
	m_stCurrentLevels[1][2]=4;
	m_stCurrentLevels[1][3]=5;
	m_stCurrentLevels[2][0]=6;
	m_stCurrentLevels[2][1]=7;
	m_stCurrentLevels[2][2]=0;
	m_stCurrentLevels[2][3]=8;
	m_stCurrentLevels[3][0]=9;
	m_stCurrentLevels[3][1]=10;
	m_stCurrentLevels[3][2]=11;
	m_stCurrentLevels[3][3]=0;
	m_pfData=NULL;
	m_pnLMaxima=NULL;
	m_pnLMinima=NULL;
	m_nLTCWindowSize=30;
	m_bLTCFromTraces=false;
	m_bRemoveSSC=true;  //SSC Short small conductance events
	m_bCalRTLTC=true; //RTLTC Runtime level transition criteria

	for(int i=0;i<12;i++)
	{
		VectFloat avfSegMinima;
		VectFloat avfSegMaxima;

		m_vvfSegMinima.push_back(avfSegMinima);
		m_vvfSegMaxima.push_back(avfSegMaxima);
	}
	m_fLargeNegativeAmp=-40000;
}

void CAbfTraceIdealization::UpdateData(float* pfData, int nSize, float fTimePerPoint)
{
	delete m_pfData;
	m_nSize=nSize;
	m_pfData=new float[nSize];
	m_fTimePerPoint=fTimePerPoint;
	m_bLTCFromTraces=false;
	for(int i=0;i<nSize;i++)
	{
		m_pfData[i]=pfData[i];
	}
	m_nLMaxI=0;
}

CAbfTraceIdealization::~CAbfTraceIdealization()
{

}

void CAbfTraceIdealization::Idealization()
{
	GetLTC();
	if(m_bLTCFromTraces)
		GetLTC_FromTrace();
	delete m_pnLMinima;
	delete m_pnLMaxima;
	m_pnLMinima=new int[m_nSize/3];
	m_pnLMaxima=new int[m_nSize/3];
	int lN, lNmin, lNmax;
	lN=m_nSize;
	LocalExtrema(m_pfData,m_pnLMinima,m_pnLMaxima,lN,m_nLMin,m_nLMax);
	int nSize=m_nLMin;
	if(nSize>m_nLMax)
		nSize=m_nLMax;

	int nTemp0=0, nTemp1=0;
	float fTime1,fTime2,fAmp1,fAmp2;
	nTemp0=m_pnLMinima[0];
	nTemp1=m_pnLMaxima[0];
	short stLevel=0;
	int nPhase=0;
	int nLevelBeginning=0,nLevelEnd=0;
	short stNextLevel;
	float fAmpMean, fAmpDev;

	m_vstructEvents.clear();
	fAmp1=m_pfData[nTemp0];
	fAmp2=m_pfData[nTemp1];

	bool bDownTransition=false;
	bool bUpTransition=false;
	bool bAnchor;
	float fAmpMin, fAmpMax;

	int nLag=0;  //0 if local minima come first, 1 if local maxima come first

	if(nTemp0<nTemp1)
	{
		nLag=0;
		stLevel=WhatLevel(fAmp1,0);
		bDownTransition=DownTransition(0,nLag,stLevel,nLevelBeginning, stNextLevel,nLevelEnd,fAmpMean,fAmpDev,fAmpMin,fAmpMax,bAnchor);
		if(bDownTransition)
		{
			structEventNode aEventNode;
			aEventNode.stLevel=stLevel;
			aEventNode.LevelStart=0;
			aEventNode.fAmpDev=fAmpDev;
			aEventNode.fAmpMean=fAmpMean;
			aEventNode.LevelLength=nLevelEnd-nLevelBeginning+1;
			aEventNode.fAmpMax=fAmpMax;
			aEventNode.fAmpMin=fAmpMin;
			aEventNode.bAnchor=bAnchor;
			m_vstructEvents.push_back(aEventNode);
			stLevel=stNextLevel;
			nLevelBeginning=nLevelEnd+1;
		}
	}
	else
	{
		nLag=-1;
		stLevel=WhatLevel(fAmp2,1);
		nLevelBeginning=0;
	}

	
	for(int i=1;i<nSize-1;i++)
	{
		//For the same i, local minimum comes first
		//i <nSize-1 because of DownTransition check the i+1th local maxima
		bUpTransition=UpTransition(i,nLag,stLevel,nLevelBeginning,stNextLevel,nLevelEnd,fAmpMean,fAmpDev,fAmpMin,fAmpMax,bAnchor);
		if(bUpTransition)
		{
			structEventNode aEventNode;
			aEventNode.stLevel=stLevel;
			aEventNode.LevelStart=nLevelBeginning;
			aEventNode.fAmpDev=fAmpDev;
			aEventNode.fAmpMean=fAmpMean;
			aEventNode.LevelLength=nLevelEnd-nLevelBeginning+1;
			aEventNode.fAmpMax=fAmpMax;
			aEventNode.fAmpMin=fAmpMin;
			aEventNode.bAnchor=bAnchor;
			m_vstructEvents.push_back(aEventNode);

			stLevel=stNextLevel;
			nLevelBeginning=nLevelEnd+1;
		}

		bDownTransition=DownTransition(i,nLag,stLevel,nLevelBeginning, stNextLevel,nLevelEnd,fAmpMean,fAmpDev,fAmpMin,fAmpMax,bAnchor);
		if(bDownTransition)
		{
			structEventNode aEventNode;
			aEventNode.stLevel=stLevel;
			aEventNode.LevelStart=nLevelBeginning;
			aEventNode.fAmpDev=fAmpDev;
			aEventNode.fAmpMean=fAmpMean;
			aEventNode.LevelLength=nLevelEnd-nLevelBeginning+1;
			aEventNode.fAmpMax=fAmpMax;
			aEventNode.fAmpMin=fAmpMin;
			aEventNode.bAnchor=bAnchor;
			m_vstructEvents.push_back(aEventNode);

			stLevel=stNextLevel;
			nLevelBeginning=nLevelEnd+1;
		}
	}
	if(m_bRemoveSSC)
		RemoveSSC();
	if(m_bSubConductRefinement && m_bLTCFromTraces) 
		SubConductRefinement();
}

bool CAbfTraceIdealization::UpTransition(int nI0, int nLag, short stLevel, int nLevelBeginning, short &stNextLevel, int &nLevelEnd, float &fAmpMean, float &fAmpDev, float &fAmpMin, float & fAmpMax, bool &bAnchor)
{
	float fProportionMin=0.99;
	float fProportionMin0=0.99;
	float fProportionMin1=0.9999;
	bool bUpTransition=false;
	int nI1=m_pnLMaxima[nI0];
	int nI2=m_pnLMinima[nI0+nLag];
	int nDelta=1;
	float fAmp1=m_pfData[nI1];
	float fAmp2=m_pfData[nI2];
	int nLevelLength, nIndex;
	float fInterval=1.f;
	float fMin=0.f;
	float fTotalCounts;
	float fSum=1.f;
	float fCutoff1=0.f;
	float fCutoff2=0.f;

	float fAmp3;
	if(nI1>0)
		fAmp3=m_pfData[nI1-1];
	else
		fAmp3=m_pfData[nI1];


	float fTime=nI2*m_fTimePerPoint/1000.;

	if(fTime > 252740)
	{
		fTime=fTime;
	}


	for(int i=stLevel;i<m_nMaxLevel;i++)
	{
		fCutoff1=m_vstructLTC[i].fUTMin;
		fCutoff2=m_vstructLTC[i].fUTMax;
		if(fAmp2<fCutoff1)
		{
			bUpTransition=true;
			stNextLevel=i+1;
//			m_fThreshold=fCutoff1;
		}
		else if(fAmp1<fCutoff2)
		{
			if((nI0-m_nLMaxI)>100)
			{
				fProportionMin=fProportionMin1;
			}
			fInterval=m_vstructSegMinimaHist[stLevel].fInterval;
			fTotalCounts=m_vstructSegMinimaHist[stLevel].fTotalCounts;
			fMin=m_vstructSegMinimaHist[stLevel].fMin;
			nIndex=int((fAmp2-fMin)/fInterval);
			fSum=m_vstructSegMinimaHist[stLevel].vfHist_Sum[nIndex];
			if(fSum/fTotalCounts<(1.-fProportionMin))
			{
				bUpTransition=true;
				stNextLevel=i+1;
//				m_fThreshold=fCutoff2;
			}
		}
	}

	if(bUpTransition)
	{
		m_fThreshold=0.5*(fAmp2+fAmp3);
		nLevelEnd=LevelEnd(stLevel, stNextLevel,nI2);

		nLevelLength=nLevelEnd-nLevelBeginning+1;
		if(nLevelLength>0)
		{
			structSemNode aSemNode=CalSem_Amp(m_pfData,nLevelBeginning,nLevelLength,nDelta);

			fAmpMean=aSemNode.fMean;
			fAmpDev=aSemNode.fSem;
			fAmpMax=aSemNode.fMax;
			fAmpMin=aSemNode.fMin;
		}
		else
		{
			stNextLevel=stLevel;
			bUpTransition=false;
		}
	}


	if(bUpTransition)
	{
		if(nI0-m_nLMaxI>m_nLTCWindowSize&&stLevel==0)
		{
			if(m_bCalRTLTC && m_bLTCFromTraces) CalRunTimeLTC(stLevel,nI0);
			if(stLevel==0)
				bAnchor=true;
			else
				bAnchor=false;
		}
		else
		{
			bAnchor=false;
		}
		//RuntimeLTC should not include nI0
		m_nLMaxI=nI0;
	}

	return bUpTransition;
}

bool CAbfTraceIdealization::DownTransition(int nI0, int nLag, short stLevel, int nLevelBeginning, short &stNextLevel, int &nLevelEnd, float &fAmpMean, float &fAmpDev, float &fAmpMin, float & fAmpMax, bool &bAnchor)
{
	bool bDownTransition=false;
	int nI1=m_pnLMaxima[nI0];
	int nI2=m_pnLMinima[nI0+1+nLag];
	float fAmp1=m_pfData[nI1];
	float fAmp2=m_pfData[nI2];

	float fAmp3;
	if(nI2>0)
		fAmp3=m_pfData[nI2-1];
	else
		fAmp3=m_pfData[nI2];

	float fDelta=0.f;
	float fDeltaC=0.f;
	float fThreshold=0.f;
	float fTemp;
	int nSign;
	for(int i=stLevel;i>0;i--)
	{
		fThreshold=m_vstructLTC[i].fDTMax;
		if(fAmp1>fThreshold)
		{
			bDownTransition=true;
			stNextLevel=i-1;

		}
		else
		{
			if(m_bLTCFromTraces) 
			{
				fTemp=0.5*(m_vstructSegMinimaHist[i].f5_thBin+m_vstructSegMinimaHist[i-1].f5_thBin)-4.*m_vstructSegMinimaHist[i].fInterval;
				if(fAmp1>fTemp)
				{
					fDelta=m_pfData[m_pnLMinima[nI0+nLag]]-fAmp1;
					fDeltaC=m_vstructLTC[stLevel].fDTDelta;
					if(fabs(fDelta)>fabs(fDeltaC))
					{
						nSign=-1;
						bDownTransition=true;
						NextLevel(stLevel, stNextLevel, fAmp1, m_fThreshold, nSign);
					}
					break;
				}
			}
		}
	}

	int nDelta=1;
	int nLevelLength=0;

	if(bDownTransition)
	{
		m_fThreshold=0.5*(fAmp1+fAmp3);
		nLevelEnd=LevelEnd(stLevel, stNextLevel, nI1);
		nLevelLength=nLevelEnd-nLevelBeginning+1;
		if(nLevelLength>0)
		{

			structSemNode aSemNode=CalSem_Amp(m_pfData,nLevelBeginning,nLevelLength,nDelta);

			fAmpMean=aSemNode.fMean;
			fAmpDev=aSemNode.fSem;
			fAmpMax=aSemNode.fMax;
			fAmpMin=aSemNode.fMin;
		}
		else
		{
			bDownTransition=false;
			stNextLevel=stLevel;
			m_fThreshold0=m_fThreshold;
		}
	}

	if(bDownTransition)
	{
		if(nI0-m_nLMaxI>m_nLTCWindowSize&&stLevel==0)
		{
			CalRunTimeLTC(stLevel,nI0);
			if(stLevel==0)
				bAnchor=true;
			else
				bAnchor=false;
		}
		else
		{
			bAnchor=false;
		}
		//RuntimeLTC should not include nI0
		m_nLMaxI=nI0;
	}


	return bDownTransition;
}

int CAbfTraceIdealization::LevelEnd(short stLevel, short stNextLevel, int nI0)
{
	int nLevelEnd=0;
	float fAmp, fThreshold;
	int i;
	if(stNextLevel>stLevel)
	{
//		fThreshold=m_vstructLTC[stLevel].fUTMin;
//		fAmp=m_pfData[nI0];
//		if(fAmp>fThreshold)
//			fThreshold=m_vstructLTC[stLevel].fUTMax;
		for(i=nI0-1;i>=0;i--)
		{
			fAmp=m_pfData[i];
			if(fAmp>m_fThreshold)
			{
				nLevelEnd=i;
				break;
			}
		}
	}
	else
	{
//		fThreshold=m_vstructLTC[stLevel].fDTMax;
//		fAmp=m_pfData[nI0];
//		if(fAmp<fThreshold)
//			fThreshold=m_vstructLTC[stLevel].fDTMin;
		for(i=nI0-1;i>=0;i--)
		{
			fAmp=m_pfData[i];
			if(fAmp<m_fThreshold)
			{
				nLevelEnd=i;
				break;
			}
		}
	}
	m_fThreshold0=m_fThreshold;
	return nLevelEnd;
}

structSemNode CAbfTraceIdealization::CalSem_Amp(float *pfData, int nBeginning, int nSize, int nDelta)
{
	if(nSize>4)
	{
		nSize-=4;
		nBeginning+=2;
	}
	else if(nSize>2)
	{
		
		nSize-=2;
		nBeginning+=1;
	}
	int i;
	float fMean=0.f,fSem=0.f,fMin=1.e20,fMax=-1.e20,fTemp;
	if(nBeginning+nSize<m_nSize)
	{
		for(i=0;i<nSize;i++)
		{
			fTemp=pfData[nBeginning+i*nDelta];
			if(fTemp>fMax)
				fMax=fTemp;
			if(fTemp<fMin)
				fMin=fTemp;

			fMean+=fTemp;
		}

		fMean/=float(nSize);
		for(i=0;i<nSize;i++)
		{
			fSem+=(fMean-pfData[nBeginning+i*nDelta])*(fMean-pfData[nBeginning+i*nDelta]);
		}

		if(nSize>1)
		{
			fSem/=float(nSize*(nSize-1));
			fSem=sqrt(fSem);
		}
	}
	else
	{
		i=i; //a break point
	}

	structSemNode aNode;

	aNode.fMean=fMean;
	aNode.fSem=fSem;
	aNode.nSampleSize=nSize;
	aNode.fMax=fMax;
	aNode.fMin=fMin;

	return aNode;
}


void CAbfTraceIdealization::LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax)
{
	float x0,x;
	short nS0,nS;
	int i;


	lNmin=0;
	lNmax=0;
	nS0=0;
	nS=0;

	for (i=1;i<lN;i++)
	{
		x0=*(pData+i-1);
		x=*(pData+i);


		if(x>x0) 
			nS=1;
		else if (x<x0)
			nS=-1;
		else nS=0;

		if(nS0*nS<0)
		{
			if(nS<0)
			{
				*(pLmaxima+lNmax)=i-1;
				lNmax++;
			}
			else
			{
				
				*(pLminima+lNmin)=i-1;
				lNmin++;

			}
		}
		if(nS!=0) nS0=nS;
	}

}

void CAbfTraceIdealization::GetLTC()
{
	// TODO: Add your command handler code here
	FILE *fpIn;
	CString sFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Level Transition Criteria File (*.ltd)\0*.ltd\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import level transition criteria data file";
	char pcField[200];
	float fUTCMin,fUTCMax,fDTCMin,fDTCMax,fAmpThred;
	int nLevels;
	int nChannels;

	int nRemoveSSC;
	int nCalRTLTC;
	int nLTCFromTraces;
	int nAmpThredOnly;

	m_bRemoveSSC=true;  //SSC Short small conductance events
	m_bCalRTLTC=true; //RTLTC Runtime level transition criteria
	m_bSubConductRefinement=true;
	m_bLTCFromTraces=true;
	m_bAmpThredOnly=false;
	int nSCRefinement;


	if(aFileDlg.DoModal()==IDOK)
	{
		m_vfAmpThreshold.clear();
		m_vfAmpThreshold0.clear();
		sFileName=aFileDlg.GetPathName();
		fpIn=fopen(sFileName,"r");
		fscanf(fpIn,"%s  %d",pcField,&nLTCFromTraces);
		if(nLTCFromTraces==0)
			m_bLTCFromTraces=false;
		fscanf(fpIn,"%s  %d",pcField,&nAmpThredOnly);
		fscanf(fpIn,"%s  %d",pcField,&nLevels);
		m_nMaxLevel=nLevels-1;

//		if(m_bLTCFromTraces)  //modified on 5o19
//		{
//			for(int i=0;i<nLevels;i++)
//			{
//				if(i<=m_nMaxLevel)
	//			{
//					structLTCNode aLTC;
//					fscanf(fpIn,"%s  %f  %s  %f  %s  %f  %s  %f  %s  %f",pcField,&fUTCMin,pcField,&fUTCMax,pcField,&fDTCMin,pcField,&fDTCMax,pcField,&fAmpThred);
//					m_vstructLTC[i].fUTMin=fUTCMin;
//					m_vstructLTC[i].fUTMax=fUTCMax;
//					m_vstructLTC[i].fDTMin=fDTCMin;
//					m_vstructLTC[i].fDTMax=fDTCMax;
//					m_vfAmpThreshold.push_back(fAmpThred);
//					m_vfAmpThreshold0.push_back(fAmpThred);
//				}
//			}
//			fscanf(fpIn,"%s    %d     %s    %s",pcField,&nChannels,pcField,sFileName);
//			m_nChannels=nChannels;
//			m_sEvlFileName=sFileName;
//			fclose(fpIn);
//		}
//		else
//		{
			m_vstructLTC.clear();
			for(int i=0;i<nLevels;i++)
			{
				structLTCNode aLTC;
				fscanf(fpIn,"%s  %f  %s  %f  %s  %f  %s  %f  %s  %f",pcField,&fUTCMin,pcField,&fUTCMax,pcField,&fDTCMin,pcField,&fDTCMax,pcField,&fAmpThred);
				aLTC.fUTMin=fUTCMin;
				aLTC.fUTMax=fUTCMax;
				aLTC.fDTMin=fDTCMin;
				aLTC.fDTMax=fDTCMax;
				m_vfAmpThreshold.push_back(fAmpThred);
				m_vfAmpThreshold0.push_back(fAmpThred);
				m_vstructLTC.push_back(aLTC);
			}
			m_vfAmpThreshold.push_back(fAmpThred);
			m_nMaxLevel=nLevels-1;
			fscanf(fpIn,"%s  %d",pcField,&nRemoveSSC);
			fscanf(fpIn,"%s  %d",pcField,&nCalRTLTC);
			fscanf(fpIn,"%s  %d",pcField,&nSCRefinement);
			fscanf(fpIn,"%s    %d     %s    %s",pcField,&nChannels,pcField,sFileName);
			m_nChannels=nChannels;
			m_sEvlFileName=sFileName;

			if(nRemoveSSC==0)
				m_bRemoveSSC=false;
			if(nCalRTLTC==0)
				m_bCalRTLTC=false;
			if(nSCRefinement==0)
				m_bSubConductRefinement=false;

			fclose(fpIn);
//		}

		if(nAmpThredOnly==1) 
			m_bAmpThredOnly=true;
	}


	if(m_bAmpThredOnly)
	{
		for(int i=0;i<nLevels;i++)
		{
			if(i<=m_nMaxLevel)
			{
				structLTCNode aLTC;
				fscanf(fpIn,"%s  %f  %s  %f  %s  %f  %s  %f  %s  %f",pcField,&fUTCMin,pcField,&fUTCMax,pcField,&fDTCMin,pcField,&fDTCMax,pcField,&fAmpThred);
				m_vstructLTC[i].fUTMin=m_vfAmpThreshold[i];
				m_vstructLTC[i].fUTMax=m_vfAmpThreshold[i];
				if(i<m_nMaxLevel) 
				{
					m_vstructLTC[i].fDTMin=m_vfAmpThreshold[i-1];
					m_vstructLTC[i].fDTMax=m_vfAmpThreshold[i-1];
				}
			}
		}
	}

//	m_vfAmpThreshold0=m_vfAmpThreshold;
}

int CAbfTraceIdealization::WhatLevel(float fAmp, int nPhase)
{
	int nLevel,i;
	if(nPhase==0)
	{
		nLevel=m_nMaxLevel;
		for(i=0;i<=m_nMaxLevel;i++)
		{
			if(fAmp>m_vstructLTC[i].fUTMin)
			{
				nLevel=i;
				break;
			}
		}
	}
	else
	{
		nLevel=0;
		for(i=m_nMaxLevel;i>=0;i--)
		{
			if(fAmp<m_vstructLTC[i].fDTMax)
			{
				nLevel=i;
				break;
			}
		}
	}
	return nLevel;
}

structEventListNode CAbfTraceIdealization::GetEventList()
{
	structEventListNode aNode;
	aNode.vstructEventList=m_vstructEvents;
	aNode.nNumLevels=m_nMaxLevel+1;
	aNode.nNumChannels=m_nChannels;
	aNode.sFileName=m_sEvlFileName;
	return aNode;
}

void CAbfTraceIdealization::GetLTC_FromTrace()
{
	float fInterval=0.05;
	float fDelta=0.2;
	float fBigPositive=1.e10f;
	float fBigNegative=-fBigPositive;
	float fUTMin, fUTMax, fDTCMin, fDTCMax;

	GetTraceSegments();
	COneDHistogram acHist;

	structLTCNode aLTC;
	aLTC.fDTMax=fBigPositive;
	aLTC.fDTMin=fBigPositive;
	aLTC.fDTDelta=fBigPositive;
	
	m_vstructLTC.clear();
	m_vstructLTC.push_back(aLTC);
	m_vstructSegMinimaHist.clear();
	m_vstructSegMaximaHist.clear();

	structOneDHistogramNode aHist1=acHist.GetHistogram(m_vvfSegMinima[1],fInterval);
	structOneDHistogramNode aHist=acHist.GetHistogram(m_vvfSegMinima[0],fInterval);

	structOneDHistogramNode bHist1=acHist.GetHistogram(m_vvfSegMaxima[1],fInterval);
	structOneDHistogramNode bHist=acHist.GetHistogram(m_vvfSegMaxima[0],fInterval);

	for(int i=0;i<=m_nMaxLevel;i++)
	{
		if(i<m_nMaxLevel)
		{
			aHist1=acHist.GetHistogram(m_vvfSegMinima[i+1],fInterval);
			bHist1=acHist.GetHistogram(m_vvfSegMaxima[i+1],fInterval);
		}

		m_vstructSegMinimaHist.push_back(aHist);
		fUTMin=aHist.f5_thBin-4.*fInterval-fDelta;
	

		structOneDHistogramNode bHist=acHist.GetHistogram(m_vvfSegMaxima[i],fInterval);
		m_vstructSegMaximaHist.push_back(bHist);
		fUTMax=bHist.f5_thBin-4.*fInterval-fDelta/2.;

		structLTCNode bLTC;
		m_vstructLTC[i].fUTMin=fUTMin;
		m_vstructLTC[i].fUTMax=fUTMax;
//		m_vstructLTC[i].fUTDelta=(aHist1.f5_thBin-aHist.f5_thBin)*.7;
		m_vstructLTC[i].fUTDelta=(aHist1.f5_thBin-bHist.f5_thBin)*.67;
		fDTCMin=fUTMin;
		fDTCMax=fUTMax;

		bLTC.fDTMin=fDTCMin;
		bLTC.fDTMax=fDTCMax;
		bLTC.fDTDelta=(aHist1.f5_thBin-bHist.f5_thBin)*.7;
//		bLTC.fDTDelta=(aHist1.f5_thBin-aHist.f5_thBin)*.7;

		bLTC.fUTMax=fBigNegative;
		bLTC.fUTMin=fBigNegative;
		bLTC.fUTDelta=fBigNegative;
		m_vstructLTC.push_back(bLTC);
		aHist=aHist1;
		bHist=bHist1;
	}
	m_bLTCFromTraces=true;

	ExportLocalExtremaHist();
}


void CAbfTraceIdealization::ExportLocalExtremaHist()
{
	FILE *fpOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="text file (*.txt)\0*.txt\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export the local extrema of the trace segments";

	float fTemp=0.f;
	float fAmp=0.f;
	float fCounts=0.f;
	float bLargestLine=true;
	int i,j,nSize;

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			for(i=0;i<100000;i++)
			{
				bLargestLine=true;

				for(j=0;j<m_nMaxLevel+1;j++)
				{
					fAmp=m_vstructSegMinimaHist[j].fMin+m_vstructSegMinimaHist[j].fInterval*float(i);
					nSize=m_vstructSegMinimaHist[j].vfHist.size();
					if(i<nSize)
					{
						fCounts=m_vstructSegMinimaHist[j].vfHist[i];
						bLargestLine=false;
					}
					else
					{
						fCounts=0.;
					}
					fprintf(fpOut,"  %10.2f    %16.2f", fAmp, fCounts);
				}

				for(j=0;j<m_nMaxLevel+1;j++)
				{
					fAmp=m_vstructSegMaximaHist[j].fMin+m_vstructSegMaximaHist[j].fInterval*float(i);
					nSize=m_vstructSegMaximaHist[j].vfHist.size();
					if(i<nSize)
					{
						fCounts=m_vstructSegMaximaHist[j].vfHist[i];
						bLargestLine=false;
					}
					else
					{
						fCounts=0.;
					}
					fprintf(fpOut,"  %10.2f    %16.2f", fAmp, fCounts);
				}
				fprintf(fpOut,"\n");
				if(bLargestLine)
					break;
			}
	     	fclose(fpOut);
		}
	}	
}


structpClampResultNode CAbfTraceIdealization::ReadpClampResultFile()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="text form of pClamp9 result Files (*.atf)\0*.atf\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a pClamp9 result file";
	char cTemp,cTemp0;
	cTemp0='Z';
	int nChar=0;
	int nLine=0;
	float fLine[26];
	float fTemp=0.f;
	int nLineLength=26;
	structpClampResultNode aNode;
	aNode.nLineLength=26;
	aNode.vfData.clear();
	aNode.nNumLines=0;
	int i;

	if(aFileDlg.DoModal()==IDOK)
	{
		m_sClampfitReultFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(m_sClampfitReultFileName,"r");
		while(!feof(fpAbfIn))
		{
			nChar++;
			fscanf(fpAbfIn,"%c",&cTemp);
			if(cTemp=='A'&&cTemp0=='p')
			{
				for(i=0;i<nLineLength;i++)
				{
					fscanf(fpAbfIn,"%f",&fTemp);
					aNode.vfData.push_back(fTemp);
				}
				nLine++;
			}
			cTemp0=cTemp;
		}
		aNode.nNumLines=nLine;
		fclose(fpAbfIn);
	}
	return aNode;
}


void CAbfTraceIdealization::GetTraceSegments()
{
	m_structpClampResultNode=ReadpClampResultFile();
	int nSize=m_structpClampResultNode.nNumLines;
	int i,j,nNI,nNF,nLevel;

	delete m_pnLMinima;
	delete m_pnLMaxima;

	m_pnLMinima=new int[m_nSize/3];
	m_pnLMaxima=new int[m_nSize/3];
	int lN, lNmin, lNmax;
	lN=m_nSize;
	LocalExtrema(m_pfData,m_pnLMinima,m_pnLMaxima,lN,m_nLMin,m_nLMax);

	for(i=0;i<12;i++)
	{
		m_vvfSegMinima[i].clear();
		m_vvfSegMaxima[i].clear();
	}

	int nRangeI;
	int nRangeF;

	m_nMaxLevel=0;

	float fAmp=0.f;

	for(i=0;i<nSize;i++)
	{
		ExtractTrace(i, nNI, nNF, nLevel);

		if(nLevel>m_nMaxLevel)
			m_nMaxLevel=nLevel;

		nRangeI=ClosestPoint_AA(m_pnLMinima,m_nLMin,nNI,1);
		if(nRangeI<2) nRangeI=2;
		nRangeF=ClosestPoint_AA(m_pnLMinima,m_nLMin,nNF,-1);
		if(nRangeF>m_nLMin-2) nRangeF=m_nLMin-2;

		for(int j=nRangeI;j<nRangeF;j++)
		{
			fAmp=m_pfData[m_pnLMinima[j]];
			m_vvfSegMinima[nLevel].push_back(fAmp);
		}

		nRangeI=ClosestPoint_AA(m_pnLMaxima,m_nLMax,nNI,1);
		if(nRangeI<2) nRangeI=2;
		nRangeF=ClosestPoint_AA(m_pnLMaxima,m_nLMax,nNF,-1);
		if(nRangeF>m_nLMax-2) nRangeF=m_nLMax-2;
		for(j=nRangeI;j<nRangeF;j++)
		{
			fAmp=m_pfData[m_pnLMaxima[j]];
			m_vvfSegMaxima[nLevel].push_back(fAmp);
		}
	}
}

int CAbfTraceIdealization::ClosestPoint_AA(int *pnData, int nLength, int nValue, int nKey)
{
	//This function returns the position in nfData whose value is the largest number (smallest if nKey <0) that is smaller (larger if nKey <0) than nValue in an asending and non-degenerated array nfData
	int nPosition=nLength/2, nI=0, nF=nLength-1;


	if(pnData[0]>=nValue)
		return 0;
	else if(pnData[nLength-1]<=nValue)
		return pnData[nLength-1];

	while(nF>nI+1)
	{
		//nF==nI+1 && pn[nI]<nValue && pn[nF]>nValue when exit this loop without break
		nPosition=(nF+nI)/2;
		if(pnData[nPosition]>nValue)
		{
			nF=nPosition;
		}
		else if(pnData[nPosition]<nValue)
		{
			nI=nPosition;
		}
		else
		{
			//pnData[nPosition]==nValue
			nI=nPosition-1;
			nF=nPosition+1;
			break;
		}
	}

	if(nKey>0)
		nPosition=nI;
		//pnData[nPosition]<nValue  && pnData[nPosition+1]>=nValue
	else
		nPosition=nF;
		//pnData[nPosition]>nValue  && pnData[nPosition-1]<=nValue

	return nPosition;
}

void CAbfTraceIdealization::ExtractTrace(int nI0, int &nNI, int &nNF, int &nLevel)
{
	int i;
	float * pfData=new float[4];
	int * pnOrder=new int[4];
	int nLineLength=m_structpClampResultNode.nLineLength;
	for(i=0;i<4;i++)
	{
		pfData[i]=m_structpClampResultNode.vfData[nI0*nLineLength+4+i];
		pnOrder[i]=i;
	}
	SortAsending(pfData,pnOrder,4);
	nLevel=m_stCurrentLevels[pnOrder[0]][pnOrder[1]];

	nNI=int(pfData[0]*1000./m_fTimePerPoint);
	nNF=int(pfData[1]*1000./m_fTimePerPoint);
}

void CAbfTraceIdealization::SortAsending(float* pfData,int *pnOrder,  int nSize)
{
	int i,j, nTemp, nSmallest;
	float fTemp, fSmallest;
	for(i=0;i<nSize;i++)
	{
		nSmallest=i;
		fSmallest=pfData[i];
		for(j=i+1;j<nSize;j++)
		{
			fTemp=pfData[j];
			if(fTemp<fSmallest)
			{
				fSmallest=fTemp;
				nSmallest=j;
			}
		}
		if(nSmallest>i)
		{
			fTemp=pfData[i];
			pfData[i]=fSmallest;
			pfData[nSmallest]=fTemp;
			pnOrder[i]=nSmallest;
		}
	}
}

void CAbfTraceIdealization::CalRunTimeLTC(int nLevel, int nI0)
{
	//RuntimeLTC should not include nI0
	float fBigNegative=-1.e10, fBigPositive=1.e10;
	float fMinLMax=fBigPositive;
	float fMinLMax0=fBigPositive;
	float fTemp;
	int i,nSize=m_nLTCWindowSize;

	int nNI=nI0-m_nLTCWindowSize;
	if(nNI<0)
		nNI=0;

	nSize=nI0-nNI;

	float *pfData=new float[nSize];

	for(i=nNI;i<nI0;i++)
	{
		fTemp=m_pfData[m_pnLMaxima[i]];
		pfData[i-nNI]=fTemp;
		if(fTemp<fMinLMax0)
			fMinLMax0=fMinLMax;
		if(fTemp<fMinLMax)
		{
			fMinLMax0=fMinLMax;
			fMinLMax=fTemp;
		}
	}

	int nDelta=1;
	structSemNode aNode=CalSem_Amp(pfData,0,nSize,nDelta);
	float fMean=aNode.fMean;
	float fSem=aNode.fSem;


	if(fMinLMax0>m_vstructSegMaximaHist[nLevel].fMean)
	{
		fMinLMax0=m_vstructSegMaximaHist[nLevel].fMean;
	}
	if(fMinLMax0<m_vstructSegMaximaHist[nLevel].fMin)
	{
		fMinLMax0=m_vstructSegMaximaHist[nLevel].fMin;
	}


	m_vstructLTC[nLevel].fUTMax=fMinLMax0-0.15;
	if(nLevel<m_nMaxLevel)
	{
		m_vstructLTC[nLevel+1].fDTMax=fMinLMax0-0.15;
		m_vstructLTC[nLevel+1].fDTMin=fMinLMax0-0.15;
	}
}

void CAbfTraceIdealization::NextLevel(short stLevel, short &stNextLevel, float fAmp, float &fThreshold, int nSign)
{
	int i,j;
	float fTemp;
	if(nSign<0)
	{
		//nSign <0 indicates down transition
		for(i=stLevel;i>0;i--)
		{
			fTemp=0.5*(m_vstructSegMinimaHist[i].f5_thBin+m_vstructSegMinimaHist[i-1].f5_thBin);
			if(fAmp>fTemp)
			{
				fThreshold=fTemp;
				stNextLevel=i-1;
			}
		}
	}
	else
	{
		for(i=stLevel;i<m_nMaxLevel;i++)
		{
			fTemp=0.5*(m_vstructSegMinimaHist[i].f5_thBin+m_vstructSegMinimaHist[i+1].f5_thBin);
			if(fAmp<fTemp)
			{
				fThreshold=fTemp;
				stNextLevel=i+1;
			}
		}
	}
}

void CAbfTraceIdealization::RemoveSSC()
{
//	SSC Short small conductance events
	const int nMinSSCLength=8;
	int i;
	short stLevel0,stLevel;
	float fAmpMin;
	int nLength;
	int nSize=m_vstructEvents.size();
	stLevel0=stLevel=m_vstructEvents[0].stLevel;
	float fUTMin;
	for(i=1;i<nSize;i++)
	{
		stLevel=m_vstructEvents[i].stLevel;
		fAmpMin=m_vstructEvents[i].fAmpMin;
		if(stLevel==stLevel0+1)
		{
			fUTMin=m_vstructLTC[stLevel-1].fUTMin;
			if(fAmpMin>fUTMin)
			{
				nLength=m_vstructEvents[i].LevelLength;
				if(nLength<nMinSSCLength)
				{
					structEventNode aNode;
					aNode=m_vstructEvents[i];
					aNode.stLevel=stLevel0;
					aNode.LevelLength+=2;
					aNode.LevelStart--;
					ApplyCorrectEvent(aNode,i);
					i--;
					nSize=m_vstructEvents.size();
				}
			}
		}
		stLevel0=stLevel;
	}
}


int CAbfTraceIdealization::ApplyCorrectEvent(structEventNode aNode, int nPosition)
{
	int nStart0=aNode.LevelStart;
	int nLength0=aNode.LevelLength;
	short stLevel0=aNode.stLevel;
	int nLeftIndex=nPosition, nRightIndex=nPosition;
	int nSize=m_vstructEvents.size();
	int nStart, nLength;
	int i,j;

	vector<structEventNode>::iterator theIterator, LeftIterator, RightIterator;

	for(i=0;i<nSize;i++)
	{
		nStart=m_vstructEvents[i].LevelStart;
		nLength=m_vstructEvents[i].LevelLength;
		if(nStart<=nStart0&&nStart+nLength>nStart0)
		{
			nLeftIndex=i;
		}
		if(nStart<=nStart0+nLength0-1&&nStart+nLength>nStart0+nLength0-1)
		{
			nRightIndex=i;
			break;
		}
	}

	if(nLeftIndex==nRightIndex)
	{
		structEventNode aTempNode=m_vstructEvents[nRightIndex];
		theIterator=m_vstructEvents.begin()+nRightIndex;
		m_vstructEvents.insert(theIterator,aTempNode);
		nRightIndex++;
	}

	short stLevelL=m_vstructEvents[nLeftIndex].stLevel;
	short stLevelR=m_vstructEvents[nRightIndex].stLevel;

	if(stLevelL==stLevel0)
	{
		m_vstructEvents[nLeftIndex].LevelLength=nStart0+nLength0-m_vstructEvents[nLeftIndex].LevelStart;
		if(stLevel0==stLevelR)
		{
			m_vstructEvents[nLeftIndex].LevelLength=m_vstructEvents[nRightIndex].LevelStart+m_vstructEvents[nRightIndex].LevelLength
																	-m_vstructEvents[nLeftIndex].LevelStart;
			theIterator=m_vstructEvents.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<=nRightIndex;i++)
			{
				m_vstructEvents.erase(theIterator);
			}
		}
		else
		{
			m_vstructEvents[nRightIndex].LevelLength=m_vstructEvents[nRightIndex].LevelStart+m_vstructEvents[nRightIndex].LevelLength
				                                                        -(nStart0+nLength0);
			m_vstructEvents[nRightIndex].LevelStart=nStart0+nLength0;
			theIterator=m_vstructEvents.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_vstructEvents.erase(theIterator);
				theIterator++;
			}
		}
	}
	else
	{
		m_vstructEvents[nLeftIndex].LevelLength=nStart0-m_vstructEvents[nLeftIndex].LevelStart;
		if(stLevel0==stLevelR)
		{
			m_vstructEvents[nRightIndex].LevelLength=m_vstructEvents[nRightIndex].LevelStart+m_vstructEvents[nRightIndex].LevelLength
																	-nStart0;
			m_vstructEvents[nRightIndex].LevelStart=nStart0;
			theIterator=m_vstructEvents.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_vstructEvents.erase(theIterator);
			}
		}
		else
		{
			m_vstructEvents[nRightIndex].LevelLength=m_vstructEvents[nRightIndex].LevelStart+m_vstructEvents[nRightIndex].LevelLength
				                                                        -(nStart0+nLength0);

			m_vstructEvents[nRightIndex].LevelStart=nStart0+nLength0;
			theIterator=m_vstructEvents.begin()+nRightIndex;
			m_vstructEvents.insert(theIterator,aNode);
			theIterator=m_vstructEvents.begin()+nLeftIndex+1;
			for(i=nLeftIndex+1;i<nRightIndex;i++)
			{
				m_vstructEvents.erase(theIterator);
			}
		}
	}
	return nLeftIndex;
}

void CAbfTraceIdealization::SubConductRefinement()
{
	int i, j, nNI, nNF, nIndex, nIndex1, nMinLength, nSegLength, nLevelLength, nNumEvents, nSize1,nLevelStart;
	float fDeltaRange, fConductanceLMax, fConductanceLMin, fSubConductance, fBaseline, fThreshold, fCounductance;
	structSemNode aSemNode;
	vector <structEventNode> vstructNewEventList;
	nMinLength=90;
	short stLevel1=1;
	short stLevel;
	bool bAnchor;
	int nSize=m_vstructEvents.size();

	for(nIndex=0;nIndex<nSize;nIndex++)
	{
		AttachToEventList(vstructNewEventList,m_vstructEvents[nIndex]);
		nLevelLength=m_vstructEvents[nIndex].LevelLength;
		stLevel=m_vstructEvents[nIndex].stLevel;
		bAnchor=m_vstructEvents[nIndex].bAnchor;
		if(bAnchor)
		{
			nLevelStart=m_vstructEvents[nIndex].LevelStart;
			nNF=ClosestPoint_AA(m_pnLMaxima, m_nLMax, nLevelStart+nLevelLength-1, 1);
			CalRunTimeLTC(stLevel, nNF);
			fDeltaRange=CalDeltaRange(nIndex);
			nIndex1=NextAnchor(nIndex);
			if(nIndex1<0)
				nIndex1=nSize-1;
			fSubConductance=CalSubConductance(nIndex,nIndex1,fDeltaRange,stLevel1);
			fBaseline=CalBaseline(nIndex);
			if(fSubConductance<=m_fLargeNegativeAmp)
			{
//				fSubConductance=CalConductLMin(nIndex, nIndex1,stLevel1);
				fSubConductance=2.*m_vfAmpThreshold0[0]-fBaseline;
				//This is to ensure that m_vfAmpThreshold0[0] will be sused as the new threshold
			}
			fThreshold=0.5*(fBaseline+fSubConductance);
			if(fThreshold<m_vstructLTC[stLevel].fUTMax)
			{
				m_vfAmpThreshold[0]=fThreshold;
				vector <structEventNode> vstructEvents=CalEvents_Amp(nIndex,nIndex1);
				AttachToEventList(vstructNewEventList,vstructEvents);
			}
			else
			{
				AttachToEventList(vstructNewEventList,nIndex+1,nIndex1-1);
			}
			if(nIndex1<nSize-1)
				nIndex=nIndex1-1;
			else
				nIndex=nIndex1;
		}
	}

	m_vstructEvents.clear();
	nSize1=vstructNewEventList.size();
	for(i=0;i<nSize1;i++)
	{
		m_vstructEvents.push_back(vstructNewEventList[i]);
	}
}

float CAbfTraceIdealization::CalConductLMin(int nIndex0, int nIndex1, short stLevel0)
{
	int nIndex,nSum;
	float fSum=0.f;
	short stLevel;
	float fTemp;
	float fConductance=m_fLargeNegativeAmp-1.;
	int nNI,nNF,i;
	nSum=0;
	for(i=nIndex0+1;i<nIndex1;i++)
	{
		stLevel=m_vstructEvents[i].stLevel;
		if(stLevel==stLevel0)
		{
			nNI=m_vstructEvents[i].LevelStart;
			nNF=nNI+m_vstructEvents[i].LevelLength-1;
			fTemp=CalConductLMin(nNI,nNF);
			if(fTemp>m_fLargeNegativeAmp)
			{
				fSum+=fTemp;
				nSum++;
			}
		}
	}
	if(nSum>0) fConductance=fSum/double(nSum);
	return fConductance;
}

float CAbfTraceIdealization::CalConductLMax(int nNI0, int nNF0, short stLevel0)
{
	int nIndex,nSum;
	float fSum=0.f;
	short stLevel;

	float fTemp;
	float fConductance=m_fLargeNegativeAmp-1.;
	int nNI,nNF,i;
	nSum=0;
	for(i=nNI0+1;i<nNF0;i++)
	{
		stLevel=m_vstructEvents[i].stLevel;
		if(stLevel==stLevel0)
		{
			nNI=m_vstructEvents[i].LevelStart;
			nNF=nNI+m_vstructEvents[i].LevelLength-1;
			fTemp=CalConductLMax(nNI,nNF);
			if(fTemp>m_fLargeNegativeAmp)
			{
				fSum+=fTemp;
				nSum++;
			}
		}
	}
	if(nSum>0) fConductance=fSum/double(nSum);
	return fConductance;
}


float CAbfTraceIdealization::CalConductLMin(int nNI0, int nNF0)
{
	int nIndex;
	float fConductance=0.f;

	int nNI=ClosestPoint_AA(m_pnLMinima, m_nLMin, nNI0, 1)+1;
	int nNF=ClosestPoint_AA(m_pnLMinima, m_nLMin, nNF0, 1);
	if(m_pnLMinima[nNF+1]==nNF0)
		nNF++;
	for(nIndex=nNI;nIndex<=nNF;nIndex++)
	{
		fConductance+=m_pfData[m_pnLMinima[nIndex]];
	}
	return fConductance/double(nNF-nNI+1);
}

float CAbfTraceIdealization::CalConductLMax(int nNI0, int nNF0)
{
	int nIndex;

	float fConductance=0.f;

	int nNI=ClosestPoint_AA(m_pnLMinima, m_nLMin, nNI0, 1)+1;
	int nNF=ClosestPoint_AA(m_pnLMinima, m_nLMin, nNF0, 1);
	if(m_pnLMaxima[nNF+1]==nNF0)
		nNF++;
	for(nIndex=nNI;nIndex<=nNF;nIndex++)
	{
		fConductance+=m_pfData[m_pnLMaxima[nIndex]];
	}
	return fConductance/double(nNF-nNI+1);
}

float CAbfTraceIdealization::CalDeltaRange(int nNI0)
{
	int nNF=m_vstructEvents[nNI0+1].LevelStart-1;
	int nIndex, nIndex1;
	float fAmp,fAmp0,fAmp1;
	int nRank=5;

	nNF=ClosestPoint_AA(m_pnLMaxima, m_nLMax, nNF, 1);
	int nNI=nNF-m_nLTCWindowSize;
	int nLag=0;
	if(m_pnLMaxima[0]<m_pnLMinima[0])
		nLag=-1;

	int nSize=2*(nNF-nNI);
	float *pfData=new float[nSize];
	int nPosition=0;

	nIndex=m_pnLMaxima[nNI];
	fAmp0=m_pfData[nIndex];

	for(int i=nNI+1;i<=nNF;i++)
	{
		nIndex1=m_pnLMaxima[i];
		nIndex=m_pnLMinima[i+nLag];
		fAmp=m_pfData[nIndex];
		fAmp1=m_pfData[nIndex1];
		pfData[nPosition]=fabs(fAmp-fAmp0);
		pfData[nPosition]=fabs(fAmp1-fAmp);
		nPosition+=2;
		fAmp0=fAmp;
	}

	float fRange=BiggestPoint(pfData, nSize, nRank);
	return fRange;
}

float CAbfTraceIdealization::BiggestPoint(float* pfData, int nSize, int nRank)
// This function returns the nRank-th largest point in pfData
{
	float fTemp,fBiggest;
	int i,j,nBiggest;

	for(i=0;i<nRank;i++)
	{
		nBiggest=i;
		fBiggest=pfData[i];
		for(j=i+1;j<nSize;j++)
		{
			fTemp=pfData[j];
			if(fTemp>fBiggest)
			{
				fBiggest=fTemp;
				nBiggest=j;
			}
		}
		pfData[nBiggest]=pfData[i];
		pfData[i]=fBiggest;
	}
	return fBiggest;
}


float CAbfTraceIdealization::CalSubConductance(int nNI0, int nNF0, float fDeltaRange)
{
	//nNI0 the LevelStart of the event, nNF0 the end of the event
	int nIndex,nIndex1;

	int nNI=ClosestPoint_AA(m_pnLMaxima, m_nLMax, nNI0, 1)+1;
	int nNF=ClosestPoint_AA(m_pnLMaxima, m_nLMax, nNF0, 1);
	float fAmp, fAmp0, fSum, fTemp, fAmp1;
	float fSubConductance=m_fLargeNegativeAmp-2.;
	int nLag=0;
	if(m_pnLMaxima[0]<m_pnLMinima[0])
		nLag=-1;

	nIndex=m_pnLMaxima[nNI];
	fAmp0=m_pfData[nIndex];

	for(int i=nNI+1;i<=nNF;i++)
	{
		nIndex1=m_pnLMaxima[i];
		nIndex=m_pnLMinima[i+nLag];
		fAmp=m_pfData[nIndex];
		fAmp1=m_pfData[nIndex1];
		if(fabs(fAmp-fAmp0)<fDeltaRange&&fabs(fAmp1-fAmp)<fDeltaRange)
		{
			fTemp=(fAmp0+fAmp1)/2.;
			if(fTemp>fSubConductance)
				fSubConductance=fTemp;
		}
		fAmp0=fAmp1;
	}
	return 	fSubConductance;
}

float CAbfTraceIdealization::CalSubConductance(int nNI0, int nNF0, float fDeltaRange, short stLevel0)
{
	//This function is at the moment only used for stlevel==1
	//nNI0 and nNF0 is the index of the two inter burst intervals
	float fTemp;
	float fSubConductance=m_fLargeNegativeAmp-1.;
	int nNI,nNF,i;
	short stLevel;
	for(i=nNI0+1;i<nNF0;i++)
	{
		stLevel=m_vstructEvents[i].stLevel;
		if(stLevel==stLevel0)
		{
			nNI=m_vstructEvents[i].LevelStart;
			nNF=nNI+m_vstructEvents[i].LevelLength-1;
			fTemp=CalSubConductance(nNI,nNF,fDeltaRange);
			if(fTemp>fSubConductance)
				fSubConductance=fTemp;
		}
	}
	return 	fSubConductance;
}


int CAbfTraceIdealization::NextLongClosedEvent(int nIndex, int nMinLength)
{
	//This function returns the index (after nIndex) of the closed event whose LevelLength is larger than nMinLength.
	int nSize=m_vstructEvents.size();
	for(int i=nIndex+1;i<nSize;i++)
	{
		if(m_vstructEvents[i].LevelLength>nMinLength)
		{
			nIndex=i;
			break;
		}
	}
	return nIndex;
}

int CAbfTraceIdealization::NextAnchor(int nIndex)
{
	//This function returns the index (after nIndex) of the next anchor event.
	int nSize=m_vstructEvents.size();
	int nIndex0=-1;
	for(int i=nIndex+1;i<nSize;i++)
	{
		if(m_vstructEvents[i].bAnchor)
		{
			nIndex0=i;
			break;
		}
	}
	return nIndex0;
}

float CAbfTraceIdealization::CalBaseline(int nIndex)
{
	float fBaseline=0.f;

	int nNF=m_vstructEvents[nIndex+1].LevelStart-1;
	nNF=ClosestPoint_AA(m_pnLMaxima, m_nLMax, nNF, 1);
	int nNI=nNF-m_nLTCWindowSize+1;
	for(nIndex=nNI;nIndex<=nNF;nIndex++)
	{
		fBaseline+=m_pfData[m_pnLMaxima[nIndex]];
	}
	return fBaseline/double(nNF-nNI+1);
}

vector <structEventNode> CAbfTraceIdealization::CalEvents(int nIndex0,int nIndex1,float fThreshold, short stLevel0)
{
	vector <structEventNode> vEventList0;
	vector <structEventNode> vEventList;
	int nNI,nNF;
	int nSize,i,j;
	short stLevel;
	for(i=nIndex0+1;i<nIndex1;i++)
	{
		stLevel=m_vstructEvents[i].stLevel;
		nNI=m_vstructEvents[i].LevelStart;
		nNF=nNI+m_vstructEvents[i].LevelLength-1;
		if(stLevel==stLevel0)
		{
			vEventList=CalEvents0(nNI,nNF,fThreshold,stLevel0);
			nSize=vEventList.size();
			for(j=0;j<nSize;j++)
			{
				vEventList0.push_back(vEventList[j]);
			}
		}
		else
		{
				vEventList0.push_back(m_vstructEvents[i]);
		}
	}
	return vEventList;
}


vector <structEventNode> CAbfTraceIdealization::CalEvents0(int nNI0,int nNF0,float fThreshold, short stLevel0)
{
	//nNI0 and nNF0 are inclusive
	vector <structEventNode> vEventList;
	const int nDelta=1;
	int nNI,nNF;
	int nSize,i,j,nLevelStart,nLevelLength;
	short stLevel;
	int nLevel, nLevel0;
	float fAmp, fAmp0;

	fAmp0=m_pfData[nNI0];
	nLevel0==WhatLevel_Amp(nNI0);
	nLevelStart=nNI0;
	structSemNode aSemNode;
	for(i=nNF0+1;i<=nNF0;i++)
	{
		fAmp=m_pfData[i];
		nLevel=WhatLevel_Amp(i);
		if(nLevel!=nLevel0)
		{
			nLevelLength=i-nLevelStart+1;
			aSemNode=CalSem_Amp(m_pfData, nLevelStart, nLevelLength, nDelta);
			structEventNode aNode;
			aNode.fAmpDev=aSemNode.fSem;
			aNode.fAmpMax=aSemNode.fMax;
			aNode.fAmpMean=aSemNode.fMean;
			aNode.fAmpMin=aSemNode.fMin;
			aNode.LevelLength=nLevelLength;
			aNode.LevelStart=nLevelStart;
			aNode.stLevel=nLevel0;

			nLevel0=nLevel;
			nLevelStart=i;
			vEventList.push_back(aNode);
		}
	}

	nLevelLength=nNF0-nLevelStart+1;
	aSemNode=CalSem_Amp(m_pfData, nLevelStart, nLevelLength, nDelta);
	structEventNode aNode;
	aNode.fAmpDev=aSemNode.fSem;
	aNode.fAmpMax=aSemNode.fMax;
	aNode.fAmpMean=aSemNode.fMean;
	aNode.fAmpMin=aSemNode.fMin;
	aNode.LevelLength=nLevelLength;
	aNode.LevelStart=nLevelStart;
	aNode.stLevel=nLevel0;
	vEventList.push_back(aNode);
	
	return vEventList;
}

vector <structEventNode> CAbfTraceIdealization::CalEvents_Amp(int nIndexI,int nIndexF)
{
	//nIndexI and nIndexF are inclusive
	vector <structEventNode> vEventList;
	const int nDelta=1;
	int nNI,nNF,nNI0,nNF0;
	int nSize,i,j,nLevelStart,nLevelLength;
	short stLevel;
	int nLevel, nLevel0;
	float fAmp, fAmp0;

	nNI0=m_vstructEvents[nIndexI+1].LevelStart;
	nNF0=m_vstructEvents[nIndexF].LevelStart-1;

	fAmp0=m_pfData[nNI0];
	nLevel0=WhatLevel_Amp(nNI0);
	nLevelStart=nNI0;

	structSemNode aSemNode;
	for(i=nNI0+1;i<=nNF0;i++)
	{
		fAmp=m_pfData[i];
		nLevel=WhatLevel_Amp(i);
		if(nLevel!=nLevel0)
		{
			nLevelLength=i-nLevelStart;
			aSemNode=CalSem_Amp(m_pfData, nLevelStart, nLevelLength, nDelta);
			structEventNode aNode;
			aNode.fAmpDev=aSemNode.fSem;
			aNode.fAmpMax=aSemNode.fMax;
			aNode.fAmpMean=aSemNode.fMean;
			aNode.fAmpMin=aSemNode.fMin;
			aNode.LevelLength=nLevelLength;
			aNode.LevelStart=nLevelStart;
			aNode.stLevel=nLevel0;
			aNode.bAnchor=false;
			vEventList.push_back(aNode);

			nLevel0=nLevel;
			nLevelStart=i;
		}
	}

	nLevelLength=nNF0-nLevelStart+1;
	aSemNode=CalSem_Amp(m_pfData, nLevelStart, nLevelLength, nDelta);
	structEventNode aNode;
	aNode.fAmpDev=aSemNode.fSem;
	aNode.fAmpMax=aSemNode.fMax;
	aNode.fAmpMean=aSemNode.fMean;
	aNode.fAmpMin=aSemNode.fMin;
	aNode.LevelLength=nLevelLength;
	aNode.LevelStart=nLevelStart;
	aNode.stLevel=nLevel0;
	aNode.bAnchor=false;
	vEventList.push_back(aNode);
	
	return vEventList;
}

short CAbfTraceIdealization::WhatLevel_Amp(int nPosition)
{
	float fAmp=m_pfData[nPosition];
	float fThreshold;
	short stLevel=0;

	for(int i=0;i<m_nMaxLevel;i++)
	{
		fThreshold=m_vfAmpThreshold[i];
		if(fAmp>m_vfAmpThreshold[i])
		{
			stLevel=i;
			break;
		}
		else
			stLevel=i+1;
	}
	return stLevel;
}


void CAbfTraceIdealization::MergeEvents(structEventNode aNode, int nIndex)
{
	const int nDelta=1;
	int nLevelStart=m_vstructEvents[nIndex].LevelStart;
	if(nLevelStart>aNode.LevelStart)
		nLevelStart=aNode.LevelStart;
	int nLevelLength=m_vstructEvents[nIndex].LevelLength+aNode.LevelLength;

	structSemNode aSemNode=CalSem_Amp(m_pfData,nLevelStart,nLevelLength,nDelta);
	m_vstructEvents[nIndex].fAmpDev=aSemNode.fSem;
	m_vstructEvents[nIndex].fAmpMax=aSemNode.fMax;
	m_vstructEvents[nIndex].fAmpMean=aSemNode.fMean;
	m_vstructEvents[nIndex].fAmpMin=aSemNode.fMin;
	m_vstructEvents[nIndex].LevelLength=nLevelLength;
	m_vstructEvents[nIndex].LevelStart=nLevelStart;

	if(aNode.bAnchor||m_vstructEvents[nIndex].bAnchor)
		m_vstructEvents[nIndex].bAnchor=true;
	else 
		m_vstructEvents[nIndex].bAnchor=false;
}


structEventNode CAbfTraceIdealization::MergeEvents(structEventNode aNode, structEventNode bNode)
{
	const int nDelta=1;
	int nLevelStart=bNode.LevelStart;

	if(nLevelStart>aNode.LevelStart)
		nLevelStart=aNode.LevelStart;

	int nLevelLength=aNode.LevelLength+bNode.LevelLength;

	structSemNode aSemNode=CalSem_Amp(m_pfData,nLevelStart,nLevelLength,nDelta);
	aNode.fAmpDev=aSemNode.fSem;
	aNode.fAmpMax=aSemNode.fMax;
	aNode.fAmpMean=aSemNode.fMean;
	aNode.fAmpMin=aSemNode.fMin;
	aNode.LevelLength=nLevelLength;
	aNode.LevelStart=nLevelStart;

	if(aNode.bAnchor||bNode.bAnchor)
		aNode.bAnchor=true;
	else 
		aNode.bAnchor=false;

	return aNode;
}

void CAbfTraceIdealization::MergeEvents(int nIndex0, int nIndex1)
{
	structEventNode aNode=m_vstructEvents[nIndex1];
	MergeEvents(aNode, nIndex0);
	m_vstructEvents.erase(m_vstructEvents.begin()+nIndex1);
}

void CAbfTraceIdealization::AttachToEventList(vector <structEventNode> &aList, vector <structEventNode> bList)
{
	int nSize=bList.size();
	structEventNode aNode;
	for(int i=0;i<nSize;i++)
	{
		aNode=bList[i];
		AttachToEventList(aList,aNode);
	}
}

void CAbfTraceIdealization::AttachToEventList(vector <structEventNode> &aList, int nIndexI, int nIndexF)
{
	structEventNode aNode;
	for(int i=nIndexI;i<=nIndexF;i++)
	{
		aNode=m_vstructEvents[i];
		AttachToEventList(aList,aNode);
	}
}

void CAbfTraceIdealization::AttachToEventList(vector <structEventNode> &aList, structEventNode aEventNode)
{
	int nSize=aList.size();
	int nLevel0;

	if(nSize>0)
		nLevel0=aList[nSize-1].stLevel;
	else
		nLevel0=-100;

	int nLevel1=aEventNode.stLevel;
	if(nLevel0==nLevel1)
	{
		aList[nSize-1]=MergeEvents(aList[nSize-1],aEventNode);
	}
	else
	{
		aList.push_back(aEventNode);
	}
}

