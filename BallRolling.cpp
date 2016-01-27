// BallRolling.cpp: implementation of the CBallRolling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BallRolling.h"
#include "BallRollingOption.h"
#include "SmoothingOptionDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBallRolling::CBallRolling(float* pfData, float* pfAdjustedData, 
						   int nSizeOfData, float fSampleInterval)
{
	m_nSizeOfData=nSizeOfData;
	m_pfData=pfData;
	m_pfAdjustedData=pfAdjustedData;
	m_pnLocalMaxima=new int[m_nSizeOfData/2];
	m_pnLocalMinima=new int[m_nSizeOfData/2];
	m_nRulerSize=60;
	m_fSampleInterval=fSampleInterval;
	m_bCheckContactingPoints=false;
	m_bSmoothingTrace=false;
	m_bTracingMinima=false;
	m_structBrigeSeg.pfData=new float[1];
	m_nDelta=10;
	m_pbTransitionPoints=new bool[1];
}

CBallRolling::~CBallRolling()
{
	delete m_pnLocalMinima;
	delete m_pnLocalMaxima;
}

void CBallRolling::LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax)
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


void CBallRolling::BaselineCorrection(int nRulerSize, CHistory* pcHistory)
{
//	m_nRulerSize=nRulerSize;
	GetOptionParameters(pcHistory);
	if(m_bTracingMinima)
	{
		ReversePolarity();
	}
	LocalExtrema(m_pfData, m_pnLocalMinima, m_pnLocalMaxima, m_nSizeOfData, 
		m_nSizeOfLocalMinima, m_nSizeOfLocalMaxima);
	GetRulerSize();
	FindingContactingPoints_BallRolling();
	if(m_bCheckContactingPoints)
	{
		EraseFalseContactingPoints();
	}
	if(m_bSmoothingTrace)
	{
		Smoothing();
	}

	BaselineAdjust();

	if(m_bTracingMinima)
	{
		ReversePolarity();
		float *pfTemp=m_pfData;
		m_pfData=m_pfAdjustedData;
		ReversePolarity();
		m_pfData=pfTemp;
	}

	if(!m_bApplyingToEntireTrace)
	{
		float fTemp;
		int nBegin=int(m_fTime1*1000./m_fSampleInterval);
		int nEnd=int(m_fTime2*1000./m_fSampleInterval);
		int nLength=nEnd-nBegin+1;
		delete m_structBrigeSeg.pfData;
		m_structBrigeSeg.pfData=new float[nLength];
		for(int i0=0;i0<nLength;i0++)
		{
			fTemp=*(m_pfAdjustedData+nBegin+i0);
			*(m_structBrigeSeg.pfData+i0)=fTemp;
		}
		float fBegin=SegAverage(m_structBrigeSeg.pfData,40);
		float fEnd=SegAverage(m_structBrigeSeg.pfData+nLength-40,40);
		float fSlope=(fEnd-fBegin)/float(nEnd-nBegin-40);
		m_structBrigeSeg.fAmpBegin=fBegin;
		m_structBrigeSeg.fAmpEnd=fEnd;
		m_structBrigeSeg.nLength=nLength;;
		m_structBrigeSeg.fSlope=fSlope;
		BrigeSegment(nBegin,nEnd);
	}
}

void CBallRolling::FindingContactingPoints()
{
	// This function is using sliding Iron method instead of rolling ball method.
	float fFirst=-99999999.f,fSecond=-999999999.f;
	float fTime0=0.f,fTime=0.f;
	float fFactor=1000.f; // For changing time unit from microsecond to millisecond.
	float *pData=new float[int(m_fWindowSize*fFactor/m_fSampleInterval)];
	int nContactingPoint=0;
	int nCurrentPoint=0,i=0;
	bool bNotEndOfRecord=true;
	int* pnRank=NULL;
	structContactPointNode aContactingNode;
	m_nSizeOfContactPoints=0;

	while (bNotEndOfRecord)
	{
		i=0;
		while(fTime-fTime0<m_fWindowSize && bNotEndOfRecord)
		{
			if(nCurrentPoint+i<m_nSizeOfLocalMaxima)
			{
				*(pData+i)=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint+i));
				fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
				i++;
			}
			else
			{
				bNotEndOfRecord=false;
			}
		}
		delete pnRank;
		int* pnRank=new int[i]; //*(pnRank+i) is the position of data point whose rank is i.
		// The biggest data point has rank 0.
//		if(i>m_nRank&&fTime-fTime0>=m_fWindowSize) 11/23/2007
		if(i>m_nRank)  //11/23/2007
		{
			nContactingPoint=BiggestPoint(pData,i,m_nRank,pnRank);
			aContactingNode.nBegin=nCurrentPoint;
			aContactingNode.nEnd=nCurrentPoint+i;
			aContactingNode.nContactingPoint=nCurrentPoint+nContactingPoint;
			aContactingNode.nLevel=-600;
			m_lstContactingPoints.push_back(aContactingNode); 
			fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
		}
//		*(m_pnContactingPoints+m_nSizeOfContactPoints)=*(m_pnLocalMaxima+nCurrentPoint+nContactingPoint);
		nCurrentPoint+=5;
		fTime0=*(m_pnLocalMaxima+nCurrentPoint)*m_fSampleInterval/fFactor;
		m_nSizeOfContactPoints++;
		fFirst=-99999999999.f;
		fSecond=-99999999999.f;
	}
}

void CBallRolling::FindingContactingPoints_BallRolling()
{
	// This function is using sliding Iron method instead of rolling ball method.
	float fFirst=-99999999.f,fSecond=-999999999.f;
	float fTime0=0.f,fTime=0.f;
	float fFactor=1000.f; // For changing time unit from microsecond to millisecond.
	float *pData=new float[int(m_fWindowSize*fFactor/m_fSampleInterval)];
	int *pnOrder=new int[int(m_fWindowSize*fFactor/m_fSampleInterval)];
	int nContactingPoint=0;
	int nCurrentPoint=0,i=0;
	bool bNotEndOfRecord=true;
	int* pnRank=NULL;
	structContactPointNode aContactingNode;
	m_nSizeOfContactPoints=0;

	float fAmpScale=0.5*m_fWindowSize/fabs(m_fAmp);

	//Mark all possible transition points
	delete m_pbTransitionPoints;
	m_pbTransitionPoints=new bool[m_nSizeOfLocalMaxima];
	FindingTranitionPoints();

	//Finding the first contacting poit withing the window size
	i=0;
	while(fTime-fTime0<m_fWindowSize)
	{
		if(nCurrentPoint+i<m_nSizeOfLocalMaxima)
		{
			*(pData+i)=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint+i));
			fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
//			i++;
		}
		else
		{
			bNotEndOfRecord=false;
		}
		i++;
	}
	delete pnRank;
	pnRank=new int[i];
	// The biggest data point has rank 0.
//	if(i>m_nRank&&fTime-fTime0>=m_fWindowSize) 11/23/2007
	if(i>m_nRank)  //11/23/2007
	{
		nContactingPoint=BiggestPoint(pData,i,m_nRank,pnRank);
		aContactingNode.nBegin=nCurrentPoint;
		aContactingNode.nEnd=nCurrentPoint+i;
		aContactingNode.nContactingPoint=nCurrentPoint+nContactingPoint;
		aContactingNode.nLevel=-600;
		m_lstContactingPoints.push_back(aContactingNode); 
		fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
	}
//	*(m_pnContactingPoints+m_nSizeOfContactPoints)=*(m_pnLocalMaxima+nCurrentPoint+nContactingPoint);
	nCurrentPoint+=nContactingPoint;
	fTime0=*(m_pnLocalMaxima+nCurrentPoint)*m_fSampleInterval/fFactor;
	m_nSizeOfContactPoints++;
	fFirst=-99999999999.f;
	fSecond=-99999999999.f;

	double dR=0.5*(0.25*m_fWindowSize*m_fWindowSize+m_fAmp*m_fAmp*fAmpScale*fAmpScale)/(m_fAmp*fAmpScale);
	double dX1,dX2,dY1,dY2;


	//Finding the rest of the contacting points usig ballrolling method.
	int nIt=0;

	while (bNotEndOfRecord)
	{
		i=1;
		nIt=0;
		fTime=*(m_pnLocalMaxima+nCurrentPoint)*m_fSampleInterval/fFactor;
		dX1=fTime;
		dY1=m_pfData[m_pnLocalMaxima[nCurrentPoint]]*fAmpScale;
		
		while(fTime-fTime0<m_fWindowSize && bNotEndOfRecord)
		{
//			if(nCurrentPoint+i<m_nSizeOfLocalMaxima&&!m_pbTransitionPoints[nCurrentPoint+i])
			if(nCurrentPoint+i<m_nSizeOfLocalMaxima)
			{
			
//				if(!m_pbTransitionPoints[nCurrentPoint+i])
//				{
					fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
					dX2=fTime;
					dY2=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint+i))*fAmpScale;
					pData[nIt]=BallAngle(dX1,dX2,dY1,dY2,dR);
					pnOrder[nIt]=i;
					nIt++;
//				}
			}
			else
			{
				bNotEndOfRecord=false;
			}
			i++;
		}
		delete pnRank;
		pnRank=new int[nIt]; //*(pnRank+i) is the position of data point whose rank is i.
		// The biggest data point has rank 0.
//		if(i>m_nRank&&fTime-fTime0>=m_fWindowSize) 11/23/2007
		if(nIt>m_nRank)  //11/23/2007
		{
			nContactingPoint=pnOrder[BiggestPoint(pData,nIt,m_nRank,pnRank)];
			aContactingNode.nBegin=nCurrentPoint;
			aContactingNode.nEnd=nCurrentPoint+i;
			aContactingNode.nContactingPoint=nCurrentPoint+nContactingPoint;
			aContactingNode.nLevel=-600;
			m_lstContactingPoints.push_back(aContactingNode); 
			fTime=*(m_pnLocalMaxima+nCurrentPoint+i)*m_fSampleInterval/fFactor;
		}
//		*(m_pnContactingPoints+m_nSizeOfContactPoints)=*(m_pnLocalMaxima+nCurrentPoint+nContactingPoint);
		nCurrentPoint+=nContactingPoint;
		fTime0=*(m_pnLocalMaxima+nCurrentPoint)*m_fSampleInterval/fFactor;
		m_nSizeOfContactPoints++;
		fFirst=-99999999999.f;
		fSecond=-99999999999.f;
	}
}

void CBallRolling::BaselineAdjust()
{
	float fFirstPoint=0.f,fSecondPoint=0.f,fSlope=0.f;
	int nCurrentPoint=0, nPreviousPoint=0,nDelta=0;
	list <structContactPointNode>::iterator pTemp=m_lstContactingPoints.begin();
	float fTime=0.f,fBeginAdjust=6000.;
	float fFactor=0.001f*m_fSampleInterval;
	nPreviousPoint=*(m_pnLocalMaxima+pTemp->nContactingPoint);
	fFirstPoint=*(m_pfData+nPreviousPoint);
	nPreviousPoint=0;
	int nBegin=int(m_fTime1*1000.f/m_fSampleInterval);
	if(m_fTime1<0.f)
	{
		nBegin=-1;
	}
	int nNumContactPointsInRange=0;
	pTemp++;
	float fTemp=0.f;
	int nEnd=int(m_fTime2*1000.f/m_fSampleInterval);
	for(pTemp;pTemp!=m_lstContactingPoints.end();pTemp++)
	{
		nCurrentPoint=*(m_pnLocalMaxima+pTemp->nContactingPoint);
		if(nCurrentPoint>=nBegin&&nCurrentPoint<=nEnd)
		{
			nNumContactPointsInRange++;
		}
		fSecondPoint=*(m_pfData+nCurrentPoint);
		if(nNumContactPointsInRange==0)
		{
			fFirstPoint=fSecondPoint;
		}
		if(nCurrentPoint>nEnd)
		{
			fSecondPoint=fFirstPoint;
		}

		fSlope=(fSecondPoint-fFirstPoint)/float(nCurrentPoint-nPreviousPoint);
		fTime=fFactor*float(nCurrentPoint);

		for(int j=nPreviousPoint;j<nCurrentPoint;j++)
		{
			if(j>=nBegin&&j<=nEnd)
			{
				nDelta=j-nPreviousPoint;
				fTemp=*(m_pfData+j)-fFirstPoint-float(nDelta)*fSlope;
				*(m_pfAdjustedData+j)=fTemp;
			}
			else
			{
				*(m_pfAdjustedData+j)=*(m_pfData+j);
			}
		}
		nPreviousPoint=nCurrentPoint;
		fFirstPoint=fSecondPoint;
	}

	nCurrentPoint=m_nSizeOfData;

	for(int j=nPreviousPoint;j<nCurrentPoint;j++)
	{
		fTime=fFactor*float(j);
		if(j>=nBegin&&j<=nEnd)
		{
			nDelta=j-nPreviousPoint;
			*(m_pfAdjustedData+j)=*(m_pfData+j)-fFirstPoint-float(nDelta)*fSlope;
		}
	}
}

void CBallRolling::CalCenterPoisition(float X0, float Y0, float X, float Y)
{
	float fDy=Y-Y0;
	float fDx=X-X0;
	float fSy=Y+Y0;
	float fSx=X+X0;
	float fK=fDy/fDx;
	float fDelta=0.5f*(fSx+fK*fSy);
	float fA=1.f+fK*fK;
	float fB=-2.f*(fK*fDelta-X0*fK+Y0);
	float fC=fDelta*fDelta-2.f*X0*fDelta+Y0*Y0+X0*X0-m_fBallRadius*m_fBallRadius;
	m_fCenterY=float(sqrt(fB*fB-4.f*fA*fC)-fB);
	m_fCenterX=fDelta-fK*m_fCenterY;
}


void CBallRolling::GetOptionParameters(CHistory* pcHistory)
{
	float fWindowSize=0.0f;

	int i=0;
	CBallRollingOption aDlg;
	if (aDlg.DoModal()==IDOK)
	{
		fWindowSize=aDlg.GetWindowSize();
		m_fWindowSize=fWindowSize;
		m_nRank=aDlg.GetRank();
		m_fCriterionOfUpTransitions=aDlg.GetCriterionOfUpTransitions();
		m_fTransitionThreshold=aDlg.GetCriterionOfUpTransitions();
		m_nMaxNumOfSuperiors=aDlg.GetMaxNumOfSuperiors();
		m_nLevelHistWindowSize=aDlg.GetLevelHistWindowSize();
		m_bSmoothingTrace=aDlg.SmoothingTrace();
		m_bCheckContactingPoints=aDlg.CheckContactingPoints();
		m_bApplyingToEntireTrace=aDlg.ApplyingToEntireTrace();
		m_nAnchorRank=aDlg.GetAnchorRank();
		m_bTracingMinima=aDlg.TracingMinima();
		m_nDelta=aDlg.GetnDelta();
		m_fAmp=aDlg.GetAmp();
		if(!m_bApplyingToEntireTrace)
		{
			m_fTime1=aDlg.GetfTime1();
			m_fTime2=aDlg.GetfTime2();
		}
		else
		{
			m_fTime1=-100000000000.f;
			m_fTime2=1000000000000.f;
		}
		structBallRollingNode aNode;
		aNode.bCheckContactingPoints=m_bCheckContactingPoints;
		aNode.bMaxima=!m_bTracingMinima;
		aNode.bMinima=m_bTracingMinima;
		aNode.bSmoothing=m_bSmoothingTrace;
		aNode.fCriterionOfUpTransitions=m_fCriterionOfUpTransitions;
		aNode.fTime1=m_fTime1;
		aNode.fTime2=m_fTime2;
		aNode.fWindowSize=m_fWindowSize;
		aNode.nAnchorRank=m_nAnchorRank;
		aNode.nLevelHistWindowSize=m_nLevelHistWindowSize;
		aNode.nMaxNumOfSuperiors=m_nMaxNumOfSuperiors;
		aNode.nRank=m_nRank;
		pcHistory->StoreToHistory(aNode);
	}
	m_fWindowSize=fWindowSize;
}

int CBallRolling::BiggestPoint(float* pfData, int nSize, int nRank, int* pnRank)
// This function returns the position of the data point whose rank (ascending) 
// is nRank.
{
	float fTemp,fBiggest;
	int i,j,nTemp,nBiggest;

	bool *bValid=new bool[nSize];
	bool bTemp=true;

	for(i=0;i<nSize;i++)
	{
		*(pnRank+i)=i;
		bValid[i]=true;
	}

	int nI=0,nF=0;

	for(i=0;i<nRank;i++)
	{
//		nBiggest=*(pnRank+i);
//		fBiggest=*(pData+nBiggest);
		nBiggest=i;
		fBiggest=pfData[nBiggest];
		for(j=i+1;j<nSize;j++)
		{
//			nTemp=*(pnRank+j);
//			fTemp=*(pData+nTemp);
			fTemp=pfData[j];
			bTemp=bValid[j];
			if(fTemp>fBiggest&&bTemp)
			{
//				*(pnRank+j)=nBiggest;
//				*(pnRank+i)=nTemp;
				fBiggest=fTemp;
				nBiggest=j;
			}
		}

		pnRank[i]=nBiggest;
		pfData[nBiggest]=pfData[i];
		pfData[i]=fBiggest;

		nI=nBiggest-m_nDelta;
		if(nI<0)
			nI=0;
		nF=nBiggest+m_nDelta;
		if(nF>=nSize)
			nF=nSize-1;
		for(int it=nI;it<=nF;it++)
			bValid[it]=false;

	}
	return nBiggest;
}

void CBallRolling::CheckContactingPoints()
{
	int nCurrentPosition=0;
	int nNumOfPeers,nNumOfSuperiors;
	list <structContactPointNode>::iterator pTemp;

	CalNumOfSuperiors_Peers();

	for(pTemp=m_lstContactingPoints.begin();pTemp!=m_lstContactingPoints.end();pTemp++)
	{
		nNumOfPeers=pTemp->nNumOfPeers;
		nNumOfSuperiors=pTemp->nNumOfSuperiors;

		if(nNumOfPeers<2||nNumOfSuperiors>m_nMaxNumOfSuperiors)
		{
			pTemp=m_lstContactingPoints.erase(pTemp);
			pTemp--;
		}
		nCurrentPosition++;
		if((nCurrentPosition%10000)==0)
		{
			nCurrentPosition=nCurrentPosition;
		}
	}
}

int CBallRolling::Transition(int nBigin, int nEnd)
{
	//Need to be finished

	return 0;
}

bool CBallRolling::LocalMaximumLevel(int nContactingPoint)
{
	bool bLocalMaximumLevel=true;
	bool bTransition=false;
	int i=nContactingPoint;
	float fPrevious=*(m_pfData+*(m_pnLocalMaxima+nContactingPoint));
	float fCurrent=0.0f;
	float fDelta=0.0f;

	while(!bTransition&&i<m_nSizeOfLocalMaxima-1)
	{
		i++;
		fCurrent=*(m_pfData+*(m_pnLocalMaxima+i));
		fDelta=fCurrent-fPrevious;
		if(fDelta>m_fTransitionThreshold)
		{
			bTransition=true;
			bLocalMaximumLevel=false;
		}
		else if(fDelta < -m_fTransitionThreshold)
		{
			bTransition=true;
		}
	}

	bTransition=false;
	i=nContactingPoint;
	fPrevious=*(m_pfData+*(m_pnLocalMaxima+nContactingPoint));
	while(!bTransition&&i>0)
	{
		i--;
		fCurrent=*(m_pfData+*(m_pnLocalMaxima+i));
		fDelta=fCurrent-fPrevious;
		if(fDelta>m_fTransitionThreshold)
		{
			bTransition=true;
			bLocalMaximumLevel=false;
		}
		else if(fDelta < -m_fTransitionThreshold)
		{
			bTransition=true;
		}
	}
//	float fTime=*(m_pnLocalMaxima+nContactingPoint)*m_fSampleInterval/1000.;
	return bLocalMaximumLevel;
}

void CBallRolling::CalLevelOfContactingPoints()
{
	int nLevel=500;
	list <structContactPointNode>::iterator pTemp;
	pTemp=m_lstContactingPoints.begin();
	pTemp->nLevel=nLevel;
	int nPreviousPoint=pTemp->nContactingPoint;
	int nPreviousLevel=nLevel;
	int nCurrentPoint;
	pTemp++;

	while(pTemp!=m_lstContactingPoints.end())
	{
		nCurrentPoint=pTemp->nContactingPoint;
//		float fTime=*(m_pnLocalMaxima+nCurrentPoint)*m_fSampleInterval/1000.f;
		nLevel=CalLevel(nPreviousLevel,nPreviousPoint,nCurrentPoint);
		if(nLevel>501)
		{
			nLevel=nLevel;
		}
		pTemp->nLevel=nLevel;
		nPreviousPoint=nCurrentPoint;
		nPreviousLevel=nLevel;
		pTemp++;
	}
}

int CBallRolling::CalLevel(int nPreviousLevel, int nPreviousPoint, int nCurrentPoint)
{
	int nLevel=nPreviousLevel;
	float fPreviousPoint=*(m_pfData+*(m_pnLocalMaxima+nPreviousPoint));
	float fCurrentPoint=0.0f;
	int nDelta=0;
	for(int i=nPreviousPoint+1; i<=nCurrentPoint; i++)
	{
		fCurrentPoint=*(m_pfData+*(m_pnLocalMaxima+i));
		if(fCurrentPoint-fPreviousPoint>=0)
		{
			nDelta=-int((fCurrentPoint-fPreviousPoint)/m_fTransitionThreshold+0.3);
		}
		else if(fCurrentPoint-fPreviousPoint<0)
		{
			nDelta=int(-(fCurrentPoint-fPreviousPoint)/m_fTransitionThreshold+0.3);
		}
		if(nDelta!=0)
		{
			nLevel+=nDelta;
		}
		fPreviousPoint=fCurrentPoint;
	}
	return nLevel;
}


void CBallRolling::CalNumOfSuperiors_Peers()
{
	CalLevelOfContactingPoints();
	InitializeLevelHists();
	int nNumOfCantactingPoints=m_lstContactingPoints.size();
	list <structContactPointNode>::iterator pTemp,pTempL,pTempR;
	int nNumOfPeers,nNumOfSuperiors;
	int nCurrentPosition=1;
	int nLevel;
	pTempL=m_lstContactingPoints.begin();
	pTempR=pTempL;

	int nWindowSize=m_nLevelHistWindowSize;
	if(nWindowSize>nNumOfCantactingPoints)
	{
		nWindowSize=nNumOfCantactingPoints;
	}

	for(int i=0;i<nWindowSize;i++)
	{
		pTempR++;
	}

	for(pTemp=m_lstContactingPoints.begin();pTemp!=m_lstContactingPoints.end();pTemp++)
	{
		UpdateLevelHists(nCurrentPosition,pTemp,pTempL,pTempR);
		nLevel=pTemp->nLevel;
		nNumOfPeers=m_nLevelHist[nLevel];
		pTemp->nNumOfPeers=nNumOfPeers;

		nNumOfSuperiors=0;
		nLevel++;
		nNumOfPeers=m_nLevelHist[nLevel];
		while(nNumOfPeers>0)
		{
			nNumOfSuperiors+=nNumOfPeers;
			nLevel++;
			nNumOfPeers=m_nLevelHist[nLevel];
		}

		pTemp->nNumOfSuperiors=nNumOfSuperiors;
		if(nCurrentPosition>nWindowSize)
		{
			pTempL++;
		}

		if(nCurrentPosition<nNumOfCantactingPoints)
		{
			pTempR++;
		}

		nCurrentPosition++;
	}
}


void CBallRolling::InitializeLevelHists()
{

	int i;
	for(i=0;i<1000;i++)
	{
		m_nLevelHist[i]=0;
	}

	int nLevel=0;
	list <structContactPointNode>::iterator pTemp=m_lstContactingPoints.begin();
	int nWindowSize=2*m_nLevelHistWindowSize;
	int nNumOfContactingPoints=m_lstContactingPoints.size();
	if(nWindowSize>nNumOfContactingPoints)
	{
		nWindowSize=nNumOfContactingPoints;
	}

	for(i=0;i<nWindowSize;i++)
	{
		nLevel=pTemp->nLevel;
		m_nLevelHist[nLevel]++;
		pTemp++;
	}
}

void CBallRolling::UpdateLevelHists(int nCurrentPosition,
									list <structContactPointNode>::iterator pTemp,
									list <structContactPointNode>::iterator pTempL,
									list <structContactPointNode>::iterator pTempR)
{
	int nLeft=nCurrentPosition-m_nLevelHistWindowSize;
	int nRight=nCurrentPosition+m_nLevelHistWindowSize;
	int nLevel=pTemp->nLevel;
	int nNumOfContactingPoints=m_lstContactingPoints.size();
	int nLevel_L,nLevel_R;
	if(nLeft>0&&nRight<nNumOfContactingPoints)
	{
		nLevel_L=pTempL->nLevel;
		m_nLevelHist[nLevel_L]--;

		nLevel_R=pTempR->nLevel;
		m_nLevelHist[nLevel_R]++;
	}
}

void CBallRolling::FindingAnchors()
{
	int* pnRank=new int[m_nLevelHistWindowSize];
	float* pfData=new float[m_nLevelHistWindowSize];
	int nCurrentPosition=0;
	int nTemp=0;
	int nRank=0;
	int j,k;
	int nContactingPoint;
	m_nNumOfAnchors=0;
	list <structContactPointNode>::iterator pTemp;
	list <structContactPointNode>::iterator pTempL;
	list <structContactPointNode>::iterator pTempR;
	m_nNumOfAnchors=0;
	for(pTemp=m_lstContactingPoints.begin();pTemp!=m_lstContactingPoints.end();pTemp++)
	{
		nTemp=nCurrentPosition%m_nLevelHistWindowSize;
		if(nTemp==0)
		{
			pTempL=pTemp;
		}
		*(pfData+nTemp)=*(m_pfData+*(m_pnLocalMaxima+pTemp->nContactingPoint));
		if(nTemp==m_nLevelHistWindowSize-1)
		{
			nContactingPoint=BiggestPoint(pfData,m_nLevelHistWindowSize,m_nAnchorRank,pnRank);
			for(j=0;j<m_nRank;j++)
			{
				nRank=*(pnRank+j);
				pTempR=pTempL;
				for(k=0;k<nRank;k++)
				{
					pTempR++;
				}
				if(j<m_nRank-1)
				{
					pTempR->nLevel=-1;
				}
				else
				{
					// j==m_nRank-1
					pTempR->nLevel=0;
				}
			}
			m_pvectAnchors.push_back(pTempR);
			m_nNumOfAnchors++;			
		}
		nCurrentPosition++;
	}
}

void CBallRolling::EraseFalseContactingPoints()
{
	FindingAnchors();
	list <structContactPointNode>::iterator pPrevious;
	list <structContactPointNode>::iterator pCurrent;
	list <structContactPointNode>::iterator pNext;
	pPrevious=m_pvectAnchors[0];
	pCurrent=m_lstContactingPoints.begin();
	while(pCurrent!=pPrevious)
	{
		pCurrent=m_lstContactingPoints.erase(pCurrent);
	}

	int nPreviousPoint=pPrevious->nContactingPoint;
	int nCurrentPoint,nNextPoint;
	for(int i=1;i<m_nNumOfAnchors;i++)
	{
		pNext=m_pvectAnchors[i];
		nNextPoint=pNext->nContactingPoint;
		pCurrent=pPrevious;
		pCurrent++;
		for(;pCurrent!=pNext;pCurrent++)
		{
			nCurrentPoint=pCurrent->nContactingPoint;
//			nLevelL=CalLevel(0,nPreviousPoint,nCurrentPoint);
//			nLevelR=CalLevel(0,nCurrentPoint,nNextPoint);
//			if(nLevelL!=0||nLevelR!=0)
//			{
				if(pCurrent->nLevel==-1)
				{
					pCurrent=m_lstContactingPoints.erase(pCurrent);
					pCurrent--;
				}
				else if(BelowContactingPoints(pPrevious,pCurrent,pNext))
				{
					pCurrent=m_lstContactingPoints.erase(pCurrent);
					pCurrent--;
				}
//			}
		}
		pPrevious=pNext;
		nPreviousPoint=pPrevious->nContactingPoint;
	}

	pCurrent=m_pvectAnchors[m_nNumOfAnchors-1];
	pCurrent++;
	while(pCurrent!=m_lstContactingPoints.end())
	{
		pCurrent=m_lstContactingPoints.erase(pCurrent);
	}
}

bool CBallRolling::BelowContactingPoints(list <structContactPointNode>::iterator pPrevious, 
										 list <structContactPointNode>::iterator pCurrent,
										 list <structContactPointNode>::iterator pNext)
{
	bool bBelowContactingPoints=false;
	int nPrevious=*(m_pnLocalMaxima+pPrevious->nContactingPoint);
	int nCurrent=*(m_pnLocalMaxima+pCurrent->nContactingPoint);
	int nNext=*(m_pnLocalMaxima+pNext->nContactingPoint);
	float fPrevious=*(m_pfData+nPrevious);
	float fCurrent=*(m_pfData+nCurrent);
	float fNext=*(m_pfData+nNext);
	float fSlopeL=(fCurrent-fPrevious)/float(nCurrent-nPrevious);
	float fSlopeR=(fNext-fCurrent)/float(nNext-nCurrent);

	list <structContactPointNode>::iterator pTemp=pPrevious;
	int nNumOfHighContactingPoints=0;
	int nTemp;
	float fTemp;
	for(;pTemp!=pCurrent;pTemp++)
	{
		nTemp=*(m_pnLocalMaxima+pTemp->nContactingPoint);
		fTemp=*(m_pfData+nTemp);
		if(fTemp-0.1*m_fTransitionThreshold>fPrevious+fSlopeL*float(nTemp-nPrevious))
		{
			if(pTemp->nLevel!=-1)
			{
				nNumOfHighContactingPoints++;
			}
		}
	}
	pTemp=pCurrent;
	pTemp++;
	for(;pTemp!=pNext;pTemp++)
	{
		nTemp=*(m_pnLocalMaxima+pTemp->nContactingPoint);
		fTemp=*(m_pfData+nTemp);
		if(fTemp-0.1*m_fTransitionThreshold>fCurrent+fSlopeR*float(nTemp-nCurrent))
		{
			if(pTemp->nLevel!=-1)
			{
				nNumOfHighContactingPoints++;
			}
		}
	}

	if(nNumOfHighContactingPoints>0)
	{
		bBelowContactingPoints=true;
	}
	return bBelowContactingPoints;
}


void CBallRolling::Smoothing()
{
	list <structContactPointNode>::iterator pCurrent;
	list <structContactPointNode>::iterator pNext;
	list <structContactPointNode>::iterator pTemp;
	structContactPointNode aNode;
	int nCurrentPoint,nNextPoint,nTemp,i;
	int nBiggest;
	float fDelta,fMax,fMin,fBiggest,fF0,fF1;
	int nSmoothingWindowSize;
	CalSmoothingParameters(fMax,fMin,nSmoothingWindowSize);
	float* pData=new float[nSmoothingWindowSize];
	pNext=m_lstContactingPoints.begin();
	pNext++;
//	int nSizeOfList=m_lstContactingPoints.size();
	int nCurrentPosition=0;
	for(;pNext!=m_lstContactingPoints.end();pNext++)
	{
		nCurrentPosition++;
		pCurrent=pNext;
		pCurrent--;
		nCurrentPoint=pCurrent->nContactingPoint;
		nNextPoint=pNext->nContactingPoint;
		i=nCurrentPoint;
		fF1=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint+1));
		fF0=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint));
		fDelta=fF1-fF0;
		fBiggest=fF1;
		nBiggest=nCurrentPoint;
		while(fDelta>fMin&&fDelta<fMax&&i<nNextPoint)
		{
			nTemp=(i%nSmoothingWindowSize);
			*(pData+nTemp)=*(m_pfData+*(m_pnLocalMaxima+i));
			fF0=fF1;
			if(nTemp==nSmoothingWindowSize-1)
			{
				aNode.nContactingPoint=nBiggest;
				m_lstContactingPoints.insert(pNext,aNode);
				fBiggest=-100000000.f;
			}
			i++;
			fF1=*(m_pfData+*(m_pnLocalMaxima+i));
			if(fF1>fBiggest)
			{
				fBiggest=fF1;
				nBiggest=i;
			}
			fDelta=fF1-fF0;
		}
	}


		nCurrentPosition++;
		pCurrent=pNext;
		pCurrent--;
		nCurrentPoint=pCurrent->nContactingPoint;
		nNextPoint=m_nSizeOfLocalMaxima;
		i=nCurrentPoint;
		fF1=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint+1));
		fF0=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint));
		fDelta=fF1-fF0;
		fBiggest=fF1;
		nBiggest=nCurrentPoint;
		while(fDelta>fMin&&fDelta<fMax&&i<nNextPoint-1)
		{
			nTemp=(i%nSmoothingWindowSize);
			*(pData+nTemp)=*(m_pfData+*(m_pnLocalMaxima+i));
			fF0=fF1;
			if(nTemp==nSmoothingWindowSize-1)
			{
				aNode.nContactingPoint=nBiggest;
				m_lstContactingPoints.insert(pNext,aNode);
				fBiggest=-100000000.f;
			}
			i++;
			fF1=*(m_pfData+*(m_pnLocalMaxima+i));
			if(fF1>fBiggest)
			{
				fBiggest=fF1;
				nBiggest=i;
			}
			fDelta=fF1-fF0;
		}



	pCurrent=m_lstContactingPoints.begin();
	pCurrent++;
	for(;pCurrent!=m_lstContactingPoints.end();pCurrent++)
	{
		pNext=pCurrent;
		pNext--;
		nCurrentPoint=pCurrent->nContactingPoint;
		nNextPoint=pNext->nContactingPoint;
		i=nCurrentPoint;
		fF1=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint-1));
		fF0=*(m_pfData+*(m_pnLocalMaxima+nCurrentPoint));
		fDelta=fF1-fF0;
		fBiggest=fF1;
		nBiggest=nCurrentPoint;
		while(fDelta>fMin&&fDelta<fMax&&i>nNextPoint)
		{
			nTemp=(i%nSmoothingWindowSize);
			*(pData+nTemp)=*(m_pfData+*(m_pnLocalMaxima+i));
			fF0=fF1;
			if(nTemp==nSmoothingWindowSize-1)
			{
				aNode.nContactingPoint=nBiggest;
				m_lstContactingPoints.insert(pCurrent,aNode);
				fBiggest=-100000000.f;
			}
			i--;
			fF1=*(m_pfData+*(m_pnLocalMaxima+i));
			if(fF1>fBiggest)
			{
				fBiggest=fF1;
				nBiggest=i;
			}
			fDelta=fF1-fF0;
		}
	}

}

void CBallRolling::CalSmoothingParameters(float &fMax, float &fMin, int &nSmoothingWindowSize)
{
	CSmoothingOptionDialog aDlg;
	int i,nFirstPoint,nTemp;
	float fFirstPoint,fLength,fDelta,fF0,fF1;
	fMin=1000000.f;
	fMax=-1000000.f;
	if(aDlg.DoModal()==IDOK)
	{
		aDlg.GetParameters(fFirstPoint,fLength,nSmoothingWindowSize);
	}

	float fTime=0.f;
	i=0;
	while(fTime<fFirstPoint)
	{
		i++;
		fTime=0.001f*m_fSampleInterval*float(*(m_pnLocalMaxima+i));
	}
	nFirstPoint=i;
	float fLastPoint=fFirstPoint+fLength;
	i=nFirstPoint;
	fF0=*(m_pfData+*(m_pnLocalMaxima+i));
	while(fTime<fLastPoint)
	{
		i++;
		nTemp=*(m_pnLocalMaxima+i);
		fTime=0.001f*m_fSampleInterval*float(nTemp);
		fF1=*(m_pfData+nTemp);
		fDelta=fF1-fF0;
		fF0=fF1;
		if(fDelta>fMax)
		{
			fMax=fDelta;
		}
		else if(fDelta<fMin)
		{
			fMin=fDelta;
		}
	}
	if(fMax<0.35f*m_fTransitionThreshold)
	{
		fMax=0.35f*m_fTransitionThreshold;
	}
	if(fMin>-0.35f*m_fTransitionThreshold)
	{
		fMin=-0.35f*m_fTransitionThreshold;
	}

}


void CBallRolling::ReversePolarity()
{
	for(int i=0;i<m_nSizeOfData;i++)
	{
		*(m_pfData+i)=-*(m_pfData+i);
	}
}

void CBallRolling::GetRulerSize()
{
	float fTime=0.0f;
	int i=0;
	float fFactor=1000.f;
	while (fTime < m_fWindowSize*fFactor)
	{
		fTime=*(m_pnLocalMaxima+i)*m_fSampleInterval;
		i++;
	}

//	m_nRulerSize=int(fTime/fFactor+0.5); 
	m_nRulerSize=i; //After 12/28/00

}


float CBallRolling::SegAverage(float *pfData, int nLength)
{
	float fSegAve=0.f;
	for(int i=0;i<abs(nLength);i++)
	{
		fSegAve+=*(pfData+i);
	}
	fSegAve/=float(abs(nLength));
	return fSegAve;
}

void CBallRolling::BrigeSegment(int nBegin, int nEnd)
{
	//Working under assumption that nEnd > nBegin
	const int nSegLength=40;
	float fVal=0.f;
	int i0=0;
	int nLength=nEnd-nBegin+1;
	float fFirst=0.f;
	float fLast=0.f;

	if(nBegin-nSegLength>=0)
	{
		fFirst=SegAverage(m_pfAdjustedData+nBegin-nSegLength,nSegLength);
	}
	else
	{
		fFirst=SegAverage(m_pfAdjustedData+nEnd+1,nSegLength);
	}

	if(nEnd+nSegLength<m_nSizeOfData)
	{
		fLast=SegAverage(m_pfAdjustedData+nEnd+1,nSegLength);
	}
	else
	{

		fLast=SegAverage(m_pfAdjustedData+nBegin-nSegLength,nSegLength);
	}

	float fSlope=(fLast-fFirst)/float(nEnd-nBegin);
	for(int i=0;i<nLength;i++)
	{
		i0=i%m_structBrigeSeg.nLength;
		fVal=fFirst+float(i)*fSlope+*(m_structBrigeSeg.pfData+i0)-m_structBrigeSeg.fAmpBegin-m_structBrigeSeg.fSlope*float(i0);
		*(m_pfAdjustedData+nBegin+i)=fVal;
	}
}

double CBallRolling::BallAngle(double dX1, double dX2, double dY1, double dY2, double dR)
{
	double dPi=3.1415926;
	double dDX=dX2-dX1;
	double dDY=dY2-dY1;
	double dr=sqrt(dDX*dDX+dDY*dDY);
	double dTheta1=180.*asin(dDY/dr)/dPi;
	double dTheta2=0.;
	if(dR>=0.5*dr)
		dTheta2=acos(0.5*dr/dR);
	else dTheta2=0.;
	dTheta2=dTheta2*180/dPi;
	return dTheta1+dTheta2;
}

void CBallRolling::FindingTranitionPoints()
{
	int n,n0,i,j,nNum,nI,nF;
	double dDL,dDR;
	delete m_pbTransitionPoints;
	m_pbTransitionPoints=new bool[m_nSizeOfLocalMaxima];

	for(i=0;i<m_nSizeOfLocalMaxima;i++)
	{
		m_pbTransitionPoints[i]=false;
	}

	float fAmp0,fAmp,dDensity;

	for(i=0;i<m_nSizeOfLocalMaxima;i++)
	{
		n0=m_pnLocalMaxima[i];
		fAmp0=m_pfData[n0];
		nI=i-5;
		if(nI<0)
			nI=0;
		nF=i+5;
		if(nF>m_nSizeOfLocalMaxima-1)
			nF=m_nSizeOfLocalMaxima-1;
		nNum=0;
		for(j=nI;j<=nF;j++)
		{
			n=m_pnLocalMaxima[j];
			fAmp=m_pfData[n];
			if(fabs(fAmp-fAmp0)>fabs(0.5*m_fAmp))
				nNum++;
		}
		dDensity=1.-double(nNum)/double(nF-nI+1);
		if(dDensity<0.7)
			m_pbTransitionPoints[i]=true;
	}
}
