// ArtitactsHandling.cpp: implementation of the CArtitactsHandling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "ArtifactsHandling.h"
#include "ArtifactHanddlingOptionDlg.h"
#include "ArtifactSegmentDialog.h"
#include "math.h"
#include "SmoothingOptionDialog.h"
#include "nr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArtifactsHandling::~CArtifactsHandling()
{

}

CArtifactsHandling::CArtifactsHandling(float* pfData, float* pfAdjustedData, int nDataSize, 
		float fSampleInterval, bool *pbExcluded, const EvlNode structEvlNode)
{
	int i;
	m_structEvlNode=structEvlNode;
	m_fAmplitudeThreshold=-15.f;
	m_fDeltaThreshold=-3.5f;
	m_fOppositeDirectionThreshold=0.7f;
	m_fRange=0.8f;
	m_pfData=pfData;
	m_pfAdjustedData=pfAdjustedData;
	m_nDataSize=nDataSize;
	m_fSampleInterval=fSampleInterval;
	m_pnLocalMinima=new int[m_nDataSize/2];
	m_pnLocalMaxima=new int[m_nDataSize/2];
	m_bDoNotConfirm=false;
	m_nMinArtifactInterval=600;
	m_nArtifactExtention=60;
	m_bBaselineDefined=false;
	m_pnBLMax=new int[1];
	m_pbExcluded=pbExcluded;
	int nAmpType=3;
	int nOppType=3;

	for(i=0;i<nDataSize;i++)
	{
		m_pbExcluded[i]=false;
	}

	GetThresholds();
	if(m_bReadFromFile)
	{	
		FILE* fp;
		CString sFileName;
		CFileDialog cFileDlg(true, NULL, "*.afr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
		CString sCriterian;
		int nIterations;
		int nTemp;
		if(cFileDlg.DoModal()==IDOK)
		{
			m_bEntireTrace=false;
			m_bDoNotConfirm=true;
			sFileName=cFileDlg.GetPathName();
			if(fp=fopen(sFileName,"r"))
			{

				m_lstArtifactSegments.clear();
				fscanf(fp,"%s%d",sCriterian,&nIterations);

				LocalExtrema(m_pfData,m_pnLocalMinima,m_pnLocalMaxima,m_nDataSize,
								 m_nNumOfLocalMinima,m_nNumOfLocalMaxima);

				for(int it=0;it<nIterations;it++)
				{
					fscanf(fp,"%s%d",sCriterian,&nTemp);
					if(nTemp!=0)
					{
						m_bBasedOnAdjustedData=true;
					}
					else
					{
						m_bBasedOnAdjustedData=false;
					}

					if(!m_bBasedOnAdjustedData)
					{
						for(i=0;i<m_nDataSize;i++)
						{
							*(m_pfAdjustedData+i)=*(m_pfData+i);
						}
					}
					else
					{
						for(i=0;i<m_nDataSize;i++)
						{
							*(m_pfData+i)=*(m_pfAdjustedData+i);
						}
					}



					fscanf(fp,"%s%d%s%f",sCriterian,&nAmpType,sCriterian,&m_fAmplitudeThreshold);
					if(nAmpType!=0)
					{
						m_bCheckAmplitude=true;
					}
					else
					{
						m_bCheckAmplitude=false;
					}

					fscanf(fp,"%s%d%s%f",sCriterian,&nTemp,sCriterian,&m_fDeltaThreshold);
					if(nTemp!=0)
					{
						m_bCheckDelta=true;
					}
					else
					{
						m_bCheckDelta=false;
					}

					fscanf(fp,"%s%d%s%f",sCriterian,&nOppType,
						sCriterian,&m_fOppositeDirectionThreshold);
										
					if(nOppType!=0)
					{
						m_bCheckOppositeDirrection=true;
					}
					else
					{
						m_bCheckOppositeDirrection=false;
					}


					fscanf(fp,"%s%d",sCriterian,&nTemp);
					if(nTemp!=0)
					{
						m_bCheckEvl=true;
					}
					else
					{
						m_bCheckEvl=false;
					}
					
					fscanf(fp,"%s%f%s%f",sCriterian,&m_fTime1,sCriterian,&m_fTime2);


					m_nBegin=int((m_fTime1*1000.f)/m_fSampleInterval);
					m_nEnd=int((m_fTime2*1000.f)/m_fSampleInterval);

					if(m_nEnd>m_nDataSize-1)
						m_nEnd=m_nDataSize-1;

					if(m_bCheckAmplitude)
					{
						CheckAbnormalValues(nAmpType);
        			}

					if(m_bCheckDelta)
					{
						CheckAbnormalTransition();
        			}
					
					if(m_bCheckOppositeDirrection)
					{
						CheckOppositeDirection(nOppType);
					}
										
       				if(m_bCheckEvl)
        			{
        				CheckEvl();
        			}

					RemoveArtifacts();

					m_lstArtifactSegments.clear();
				}
			}

		}

	}
	else
	{

		if(m_bBasedOnAdjustedData)
		{
			for(int i=0;i<m_nDataSize;i++)
			{
				*(m_pfData+i)=*(m_pfAdjustedData+i);
			}
		}
		else
		{
			for(int i=0;i<m_nDataSize;i++)
			{
				*(m_pfAdjustedData+i)=*(m_pfData+i);
			}
		}
		LocalExtrema(m_pfData,m_pnLocalMinima,m_pnLocalMaxima,m_nDataSize,
					m_nNumOfLocalMinima,m_nNumOfLocalMaxima);
		if(m_bCheckDelta)
		{
			CheckAbnormalTransition();
		}
		if(m_bCheckOppositeDirrection)
		{
			CheckOppositeDirection(nOppType);
		}
		if(m_bCheckAmplitude)
		{
			CheckAbnormalValues(nAmpType);
		}
		if(m_bCheckEvl)
		{
			CheckEvl();
		}
		RemoveArtifacts();
	}
}

float CArtifactsHandling::TruncateTo(float fValue, float fReturnValue, float fRange)
{
	// This function returns: fReturnValue + Mod(fValue, fRange) - fRange/2
	float fTemp=fValue-float(int(fValue/fRange)*fRange);
	fTemp=fReturnValue+fTemp-0.5f*fRange;
	return fTemp;
}

void CArtifactsHandling::LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, 
									  int &lNmin, int &lNmax)
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


void CArtifactsHandling::CheckAbnormalTransition()
{
	bool bEndOfArtifactSegment=true;
	int i,nFirstPoint,nLastPoint;
	int nPosition;
	float fCurrent,fPrevious,fFirstPoint,fLastPoint;
	int nCurrentPoint,nCurrentPosition,nPrevious;
	fPrevious=*(m_pfData+*(m_pnLocalMinima));
	int nRangeI=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nBegin,1);
	if(nRangeI<2) nRangeI=2;
	int nRangeF=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nEnd,-1);
	if(nRangeF>m_nNumOfLocalMinima-2) nRangeF=m_nNumOfLocalMinima-2;
//	for(i=2;i<m_nNumOfLocalMinima-2;i++)
	for(i=nRangeI;i<nRangeF;i++)
	{
		nPosition=*(m_pnLocalMinima+i);
		if(nPosition>=m_nBegin&&nPosition<=m_nEnd)
		{

			fPrevious=*(m_pfData+*(m_pnLocalMinima+i-1));
			nCurrentPoint=*(m_pnLocalMinima+i);
			fCurrent=*(m_pfData+nCurrentPoint);
	
			if(fCurrent-fPrevious < m_fDeltaThreshold)
			{
				nFirstPoint=*(m_pnLocalMinima+i-2);
				fFirstPoint=SegAverage(nFirstPoint,-40);
				bEndOfArtifactSegment=false;
				while(i<m_nNumOfLocalMinima-1 && !bEndOfArtifactSegment)
				{
					i++;
					nLastPoint=*(m_pnLocalMinima+i);
					fLastPoint=*(m_pfData+nLastPoint);
					if(fLastPoint-fFirstPoint>-m_fRange/2.)
					{
						structSegmentNode aNode;
						aNode.fFirstPoint=fFirstPoint;
						aNode.fLastPoint=SegAverage(nLastPoint+2,40);
						aNode.nFirstPoint=nFirstPoint;
						aNode.nLastPoint=nLastPoint+2; //Because the segment is define as [n1,n2);
						aNode.nArtifactType=1;
						m_lstArtifactSegments.push_back(aNode);
						bEndOfArtifactSegment=true;
					}
				}
				nPrevious=nLastPoint;
			}
			else if(fPrevious-fCurrent < m_fDeltaThreshold)
			{
				nCurrentPosition=i;
				nLastPoint=*(m_pnLocalMinima+i+1);
				fLastPoint=SegAverage(nLastPoint,40);
				bEndOfArtifactSegment=false;
				while(i>2 && !bEndOfArtifactSegment)
				{
					i--;
					nFirstPoint=*(m_pnLocalMinima+i);
					fFirstPoint=*(m_pfData+nFirstPoint);
					if(fFirstPoint-fLastPoint>-m_fRange/2.)
					{
						structSegmentNode aNode;
						aNode.fLastPoint=fLastPoint;
						aNode.fFirstPoint=SegAverage(nFirstPoint+2,40);
						aNode.nFirstPoint=nFirstPoint;
						aNode.nLastPoint=nLastPoint-2; //Because the segment is define as [n1,n2);
						aNode.nArtifactType=1;
						m_lstArtifactSegments.push_back(aNode);
						bEndOfArtifactSegment=true;
					}
				}
				i=nCurrentPosition;
				nPrevious=nLastPoint;
			}
		}
	}
}

void CArtifactsHandling::CheckAbnormalTransitionAndValues()
{
	bool bEndOfArtifactSegment=true;
	int i,nFirstPoint,nLastPoint;
	float fCurrent,fPrevious,fFirstPoint,fLastPoint;
	int nCurrentPoint;
	fPrevious=*(m_pfData+*(m_pnLocalMinima));
	int nRangeI=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nBegin,1);
	if(nRangeI<1) nRangeI=1;
	int nRangeF=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nEnd,-1);
	for(i=nRangeI;i<nRangeF;i++)
	{
		fPrevious=*(m_pfData+*(m_pnLocalMinima+i-1));
		nCurrentPoint=*(m_pnLocalMinima+i);
		fCurrent=*(m_pfData+nCurrentPoint);
		if(fCurrent-fPrevious < m_fDeltaThreshold&&fCurrent<m_fAmplitudeThreshold)
		{
			nFirstPoint=*(m_pnLocalMinima+i-2);
			fFirstPoint=SegAverage(nFirstPoint,-40);
			bEndOfArtifactSegment=false;
			while(i<m_nNumOfLocalMinima-1 && !bEndOfArtifactSegment)
			{
				i++;
				nLastPoint=*(m_pnLocalMinima+i);
				fLastPoint=*(m_pfData+nLastPoint);
				if(fLastPoint-fFirstPoint>-m_fRange/2.)
				{
					structSegmentNode aNode;
					aNode.fFirstPoint=fFirstPoint;
					aNode.fLastPoint=SegAverage(nLastPoint+2,40);
					aNode.nFirstPoint=nFirstPoint;
					aNode.nLastPoint=nLastPoint+2; //Because the segment is define as [n1,n2);
					aNode.nArtifactType=2;
					m_lstArtifactSegments.push_back(aNode);
					bEndOfArtifactSegment=true;
				}
			}
		}
	}
}


void CArtifactsHandling::CheckAbnormalValues(int nAmpType)
{
	//m_nBegin and m_nEnd were just defined before this function was called
	bool bEndOfArtifactSegment=true;
	int i,nFirstPoint,nLastPoint;
	int nPosition;
	float fCurrent,fPrevious,fFirstPoint,fLastPoint;
	fPrevious=*(m_pfData+*(m_pnLocalMinima));
	switch (nAmpType)
	{
	case -22:
		structSegmentNode aNode;
		aNode.fFirstPoint=0.f;
		aNode.fLastPoint=0.f;
		aNode.nFirstPoint=m_nBegin;
		aNode.nLastPoint=m_nEnd; //Because the segment is define as [n1,n2);
		aNode.nArtifactType=nAmpType;
		m_lstArtifactSegments.push_back(aNode);		
		break;
	case -32:
		DefineBaselineSegment(m_nBegin, m_nEnd);
		break;
	case -42:
		ExcludeSegment(m_nBegin, m_nEnd);
		break;
	default:
		int nRangeF=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nEnd,-1);
		int nRangeI=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nBegin,1);
		if(nRangeI<1) nRangeI=1;
		for(i=nRangeI;i<nRangeF;i++)
		{
			nPosition=*(m_pnLocalMinima+i);
			if(nPosition>=m_nBegin&&nPosition<=m_nEnd)
			{
				fCurrent=*(m_pfData+nPosition);
				if(fCurrent < m_fAmplitudeThreshold)
				{
					nFirstPoint=*(m_pnLocalMinima+i-1);
					fFirstPoint=*(m_pfData+nFirstPoint);
					bEndOfArtifactSegment=false;
					while(i<m_nNumOfLocalMinima-1 && !bEndOfArtifactSegment)
					{
						i++;
						nLastPoint=*(m_pnLocalMinima+i);
						fLastPoint=*(m_pfData+nLastPoint);
						if(fLastPoint > m_fAmplitudeThreshold||i>=m_nNumOfLocalMinima-1)
						{
							structSegmentNode aNode;
							aNode.fFirstPoint=fFirstPoint;
							aNode.fLastPoint=fLastPoint;
							aNode.nFirstPoint=nFirstPoint;
							aNode.nLastPoint=*(m_pnLocalMinima+i); //Because the segment is define as [n1,n2);
							aNode.nArtifactType=nAmpType;
							m_lstArtifactSegments.push_back(aNode);
							bEndOfArtifactSegment=true;
						}
					}
				}
			}				
		}
	}
}

void CArtifactsHandling::CheckOppositeDirection(int nType)
{
	bool bEndOfArtifactSegment=true;
	int i,nFirstPoint,nLastPoint,nPosition;
	float fCurrent,fPrevious,fFirstPoint,fLastPoint;
	fPrevious=*(m_pfData+*(m_pnLocalMaxima));
	int nRangeF=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nEnd,-1);
	int nRangeI=ClosestPoint_AA(m_pnLocalMinima,m_nNumOfLocalMinima,m_nBegin,1);
	if(nRangeI<1) nRangeI=1;

	int nShift=0;
	switch (nType)

	{
	case -82:
		if(m_pnLocalMaxima[0]<m_pnLocalMinima[0])
			nShift=-1;

		for(i=nRangeI;i<nRangeF;i++)
		{
			nPosition=*(m_pnLocalMinima+i);
			if(nPosition>=m_nBegin&&nPosition<=m_nEnd)
			{
				fCurrent=*(m_pfData+*(m_pnLocalMinima+i));
				if(fCurrent > m_fOppositeDirectionThreshold)
				{
					nFirstPoint=*(m_pnLocalMinima+i-1);
					fFirstPoint=*(m_pfData+nFirstPoint);
					bEndOfArtifactSegment=false;
					while(i<nRangeF-1 && !bEndOfArtifactSegment)
					{
						i++;
						nLastPoint=*(m_pnLocalMinima+i);
						fLastPoint=*(m_pfData+nLastPoint);
						if(fLastPoint < m_fOppositeDirectionThreshold || i>= m_nNumOfLocalMaxima-1)
						{
							if(nType<0)
							{
								structSegmentNode aNode;
								aNode.fFirstPoint=fFirstPoint;
								aNode.fLastPoint=fLastPoint;
								aNode.nFirstPoint=nFirstPoint;
								aNode.nLastPoint=nLastPoint; 
								aNode.nArtifactType=nType;
								m_lstArtifactSegments.push_back(aNode);		
								bEndOfArtifactSegment=true;
							}
						}
					}
				}
			}
		}
		break;
	default:
		if(m_pnLocalMaxima[0]<m_pnLocalMinima[0])
			nShift=-1;

		for(i=nRangeI;i<nRangeF;i++)
		{
			nPosition=*(m_pnLocalMaxima+i);
			if(nPosition>=m_nBegin&&nPosition<=m_nEnd)
			{
				fCurrent=*(m_pfData+*(m_pnLocalMaxima+i));
				if(fCurrent > m_fOppositeDirectionThreshold)
				{
					nFirstPoint=*(m_pnLocalMinima+i+nShift);
					fFirstPoint=*(m_pfData+nFirstPoint);
					bEndOfArtifactSegment=false;
					while(i<nRangeF-1 && !bEndOfArtifactSegment)
					{
						i++;
						nLastPoint=*(m_pnLocalMinima+i+nShift+1);
						fLastPoint=*(m_pfData+nLastPoint);
						if(fLastPoint < m_fOppositeDirectionThreshold || i>= m_nNumOfLocalMaxima-1)
						{
							if(nType<0)
							{
								structSegmentNode aNode;
								aNode.fFirstPoint=fFirstPoint;
								aNode.fLastPoint=fLastPoint;
								aNode.nFirstPoint=nFirstPoint;
								aNode.nLastPoint=nLastPoint; //Because the segment is define as [n1,n2);
								aNode.nArtifactType=nType;
								m_lstArtifactSegments.push_back(aNode);		
								bEndOfArtifactSegment=true;
							}
							else
							{
								structSegmentNode aNode;
								aNode.fFirstPoint=fFirstPoint;
								aNode.fLastPoint=fLastPoint;
								aNode.nFirstPoint=nFirstPoint;
								aNode.nLastPoint=nLastPoint+1; //Because the segment is define as [n1,n2);
								aNode.nArtifactType=-1;
								m_lstArtifactSegments.push_back(aNode);
								bEndOfArtifactSegment=true;
							}
						}
					}
				}
			}
		}
		break;
	}
}

void CArtifactsHandling::RemoveArtifacts()
{
	int nNumNodes=m_lstArtifactSegments.size();
	if(nNumNodes>1) ConcatenateArtifacts();
	if(nNumNodes>0) ExtendDetectedArtifacts();
	float fSlope,fFirstPoint,fLastPoint,fValue,fReturnValue,fAdjustedValue;
	float fBeginningOfArtifact,fEndOfArtifact;
	int   nFirstPoint,nLastPoint,i;
	
	int nBegin=m_nBegin;
	int nEnd=m_nEnd;
	int nLag=0;
	int nArtifactType=0;
	float fMaxAmp=-10000.f;
	float fMinAmp=200.f;
	float nCutoff=3000./m_fSampleInterval;
	int nSize=m_lstArtifactSegments.size();
	CArtifactSegmentDialog aDlg;
	list <structSegmentNode>::iterator pTemp;
	list <structSegmentNode>::iterator pTemp0;
	list <structSegmentNode>::iterator pTemp1;
	list <structSegmentNode>::iterator pTempI;
	int nIter=0;
	float fTime=0.f;
	pTemp=m_lstArtifactSegments.begin();
	while(nIter<nSize)
	{
		nIter++;
		fFirstPoint=pTemp->fFirstPoint;
		fLastPoint=pTemp->fLastPoint;
		nFirstPoint=pTemp->nFirstPoint;
		fBeginningOfArtifact=0.001f*nFirstPoint*m_fSampleInterval;
		nLastPoint=pTemp->nLastPoint;
		fEndOfArtifact=0.001f*nLastPoint*m_fSampleInterval;
		nLag=nFirstPoint-nLastPoint-10;
		nArtifactType=pTemp->nArtifactType;
		bool bBurst;
		int nType1,nI0,nI1,nTemp1;

		switch(nArtifactType)
		{
		case -22:
			MakeBaseline(nFirstPoint, nLastPoint);
			break;
		case -52:
			//Remove a burst of small positive conductance;
			bBurst=false;
			nI0=pTemp->nFirstPoint;
			nLastPoint=pTemp->nLastPoint;
			pTemp1=pTemp;
			pTemp1++;
			nI1=pTemp1->nFirstPoint;
			nType1=pTemp1->nArtifactType;
			pTemp0=pTemp;
			while(nI1-nLastPoint<nCutoff&&nIter<nSize&&nType1==-52)
			{
				bBurst=true;
				pTemp++;
				nLastPoint=pTemp->nLastPoint;
				pTemp1=pTemp;
				pTemp1++;
				nI1=pTemp1->nFirstPoint;
				nTemp1=pTemp1->nArtifactType;
				nIter++;
			}
			if(bBurst)
			{
				fMaxAmp=-10000.;
				for(i=nI0;i<=nLastPoint;i++)
				{
					if(m_pfData[i]>fMaxAmp)
						fMaxAmp=m_pfData[i];
				}
				if(fMaxAmp<=fMinAmp) 
					MakeBaseline(nI0,nLastPoint);

				//				for(pTempI=pTemp0;pTempI!=pTemp1;pTempI++)
//				{
//					nFirstPoint=pTempI->nFirstPoint;
//					nLastPoint=pTempI->nLastPoint;
//					fMaxAmp=-10000.;
//					for(i=nFirstPoint;i<=nLastPoint;i++)
//					{
//						if(m_pfData[i]>fMaxAmp)
//							fMaxAmp=m_pfData[i];
//					}
//					if(fMaxAmp<=fMinAmp) 
//						MakeBaseline(nI0,nLastPoint);
//					else break;
//				}

//				for(pTempI=pTemp1;pTempI!=pTemp0;pTempI--)
//				{
//					nFirstPoint=pTempI->nFirstPoint;
//					nLastPoint=pTempI->nLastPoint;
//					fMaxAmp=-10000.;
//					for(i=nFirstPoint;i<=nLastPoint;i++)
//					{
//						if(m_pfData[i]>fMaxAmp)
//							fMaxAmp=m_pfData[i];
//					}
//					if(fMaxAmp<=fMinAmp) 
//						MakeBaseline(nI0,nLastPoint);
//					else break;
//				}
//			}
			}
			break;

		case -62:
			//Remove a burst of small positive conductance;
			bBurst=false;
			nI0=pTemp->nFirstPoint;
			nLastPoint=pTemp->nLastPoint;
			pTemp1=pTemp;
			pTemp1++;
			nI1=pTemp1->nFirstPoint;
			nType1=pTemp1->nArtifactType;
			pTemp0=pTemp;
			while(nI1-nLastPoint<nCutoff&&nIter<nSize&&nType1==-52)
			{
				bBurst=true;
				pTemp++;
				nLastPoint=pTemp->nLastPoint;
				pTemp1=pTemp;
				pTemp1++;
				nI1=pTemp1->nFirstPoint;
				nTemp1=pTemp1->nArtifactType;
				nIter++;
			}
			fMaxAmp=-10000.;
			for(i=nI0;i<=nLastPoint;i++)
			{
				if(m_pfData[i]>fMaxAmp)
					fMaxAmp=m_pfData[i];
			}
			if(fMaxAmp<=fMinAmp) 
				MakeBaseline(nI0,nLastPoint);
			break;
		case -72:
			bBurst=false;
			nI0=pTemp->nFirstPoint;
			nLastPoint=pTemp->nLastPoint;
			pTemp1=pTemp;
			pTemp1++;
			nI1=pTemp1->nFirstPoint;
			nType1=pTemp1->nArtifactType;
			pTemp0=pTemp;
			while(nI1-nLastPoint<nCutoff&&nIter<nSize&&nType1==-52)
			{
				bBurst=true;
				pTemp++;
				nLastPoint=pTemp->nLastPoint;
				pTemp1=pTemp;
				pTemp1++;
				nI1=pTemp1->nFirstPoint;
				nTemp1=pTemp1->nArtifactType;
				nIter++;
			}
			fTime=0.001f*(nLastPoint-nI0)*m_fSampleInterval;
			if(fTime>6.)
			{
				fMaxAmp=-10000.;
				for(i=nI0;i<=nLastPoint;i++)
				{
					if(m_pfData[i]>fMaxAmp)
						fMaxAmp=m_pfData[i];
				}
				if(fMaxAmp<=fMinAmp) 
					MakeBaseline(nI0,nLastPoint);
			}
			break;


		default:

			if(m_bEntireTrace||nFirstPoint>=nBegin&&nLastPoint<=nEnd)
			{
				if(m_bDoNotConfirm)
				{
	//				nFirstPoint=int(1000.f*fBeginningOfArtifact/m_fSampleInterval);
	//				nLastPoint=int(1000.f*fEndOfArtifact/m_fSampleInterval);
					fSlope=(fLastPoint-fFirstPoint)/float(nLastPoint-nFirstPoint);
					for(i=nFirstPoint;i<nLastPoint;i++)
					{
						
						fValue=*(m_pfData+i);
						fReturnValue=fFirstPoint+fSlope*float(i-nFirstPoint);
						if(i+nLag<0)
						{
							fAdjustedValue=TruncateTo(fValue,fReturnValue,m_fRange);
						}
						else
						{
							fAdjustedValue=*(m_pfAdjustedData+i+nLag);;
						}
						*(m_pfAdjustedData+i)=fAdjustedValue;
					}
				}
				else
				{
					aDlg.SetParameters(fBeginningOfArtifact,fFirstPoint,fEndOfArtifact,fLastPoint,m_bDoNotConfirm);
					if(aDlg.DoModal()==IDOK)
					{
						aDlg.GetParameters(fBeginningOfArtifact,fFirstPoint,fEndOfArtifact,fLastPoint,m_bDoNotConfirm);
						nFirstPoint=int(1000.f*fBeginningOfArtifact/m_fSampleInterval);
						nLastPoint=int(1000.f*fEndOfArtifact/m_fSampleInterval);
						fSlope=(fLastPoint-fFirstPoint)/float(nLastPoint-nFirstPoint);
						for(i=nFirstPoint;i<nLastPoint;i++)
						{
							fValue=*(m_pfData+i);
							fReturnValue=fFirstPoint+fSlope*float(i-nFirstPoint);
							if(i+nLag<0)
							{
								fAdjustedValue=TruncateTo(fValue,fReturnValue,m_fRange);
							}
							else
							{
								fAdjustedValue=*(m_pfAdjustedData+i+nLag);;
							}
							*(m_pfAdjustedData+i)=fAdjustedValue;
						}
					}
				}
			}
		}
		pTemp++;
	}
}

void CArtifactsHandling::GetThresholds()
{
	CArtifactHanddlingOptionDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		aDlg.GetThresholds(m_fAmplitudeThreshold,m_fDeltaThreshold,
						   m_fOppositeDirectionThreshold,m_fRange);
		aDlg.GetBoolParameters(m_bCheckDelta,m_bCheckOppositeDirrection,
						   m_bCheckAmplitude,m_bBasedOnAdjustedData,m_bEntireTrace,
						   m_fTime1,m_fTime2,m_bCheckEvl,m_bReadFromFile);
	}
}

void CArtifactsHandling::ReplaceSegments(float *pfData, float *pfAdjustedData, int nSizeOfData, float fSampleInterval, CHistory* pcHistory)
{
	const int nLength=40;
	bool bDone=false;
	float fBegin=0.f,fEnd=0.f,fAmpBegin=0.f,fAmpEnd=0.f;
	int nFirstPoint=0,nLastPoint=0;
	int nWindowSize=0;
	float fLength=0.f;

	m_pfData=pfData;
	m_pfAdjustedData=pfAdjustedData;
	m_nDataSize=nSizeOfData;
	m_fSampleInterval=fSampleInterval;
	CSmoothingOptionDialog aSmoothDlg;

	if(aSmoothDlg.DoModal()==IDOK)
	{
		aSmoothDlg.GetParameters(fBegin,fLength,nWindowSize);
		m_nBrigeSegStart=int(fBegin*1000.f/m_fSampleInterval);
		m_nBrigeSegLength=int(fLength*1000.f/m_fSampleInterval);
		m_fBrigeAmpBegin=SegAverage(m_nBrigeSegStart,nLength);
		m_fBrigeAmpEnd=SegAverage(m_nBrigeSegStart+m_nBrigeSegLength,-nLength);
		m_fBrigeSlope=(m_fBrigeAmpEnd-m_fBrigeAmpBegin)/float(m_nBrigeSegLength);
		m_structSegmentsReplacingNode.nBrigeBegin=m_nBrigeSegStart;
		m_structSegmentsReplacingNode.nBrigeLength=m_nBrigeSegLength;

		while(!bDone)
		{
			CSegDeletOptionDialog aDlg;
			if(aDlg.DoModal()==IDOK)
			{
				aDlg.GetParameters(fBegin,fAmpBegin,fEnd,fAmpEnd,bDone);
				if(fEnd>fBegin)
				{
					nFirstPoint=int(fBegin*1000.f/m_fSampleInterval);
					nLastPoint=int(fEnd*1000.f/m_fSampleInterval)+1; //Because the segment is define as [n1,n2);
					if(fAmpBegin>999999)
					{
						fAmpBegin=*(m_pfData+nFirstPoint);
						fAmpEnd=*(m_pfData+nLastPoint);
					}
					m_structSegmentsReplacingNode.nSegBegin=nFirstPoint;
					m_structSegmentsReplacingNode.nSegLength=nLastPoint-nFirstPoint+1;
					BrigeSegment(nFirstPoint, nLastPoint);
					pcHistory->WriteHistoryNode(m_structSegmentsReplacingNode);
				}
				else if(fBegin<0)
				{
					CString sAbfFileName;
					CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
					if(aFileDlg.DoModal()==IDOK)
					{
						sAbfFileName=aFileDlg.GetPathName();
						FILE* fp=fopen(sAbfFileName,"r");
						while(!feof(fp))
						{
							fscanf(fp,"%f%f",&fBegin,&fEnd);
							nFirstPoint=int(fBegin*1000.f/m_fSampleInterval);
							nLastPoint=int(fEnd*1000.f/m_fSampleInterval)+1; //Because the segment is define as [n1,n2);
							BrigeSegment(nFirstPoint,nLastPoint);
							m_structSegmentsReplacingNode.nSegBegin=nFirstPoint;
							m_structSegmentsReplacingNode.nSegLength=nLastPoint-nFirstPoint+1;
							pcHistory->WriteHistoryNode(m_structSegmentsReplacingNode);
						}
						bDone=true;
					}
				}
				else
				{
					bDone=true;
				}
			}
		}
	}
}

float CArtifactsHandling::SegAverage(int nBegin, int nLength)
{
	int nI=nBegin;
	if(nLength<0)
	{
		nI=nBegin+nLength+1;
	}
	float fSegAve=0.f;
	for(int i=0;i<abs(nLength);i++)
	{
		fSegAve+=*(m_pfData+nI+i);
	}
	fSegAve/=float(abs(nLength));
	return fSegAve;
}


float CArtifactsHandling::SegAverage0(int nBegin, int nEnd)
{
	float fSegAve=0.f;
	int nLength=nEnd-nBegin+1;
	for(int i=nBegin;i<=nEnd;i++)
	{
		fSegAve+=*(m_pfData+i);
	}
	fSegAve/=float(abs(nLength));
	return fSegAve;
}



float CArtifactsHandling::SegAverage(int nLength, vector <float> vfData)
{
	//This function assumes nLength<vfData.size()
	int nSize=vfData.size();
	int nI=0;
	if(nLength<0)
	{
		nI=nSize+nLength;
	}
	float fSegAve=0.f;
	for(int i=0;i<abs(nLength);i++)
	{
		fSegAve+=vfData[nI+i];
	}
	fSegAve/=float(abs(nLength));
	return fSegAve;
}

void CArtifactsHandling::BrigeSegment(int nBegin, int nEnd)
{
	//Working under assumption that nEnd > nBegin
	const int nSegLength=40;
	float fVal=0.f;
	int i0=0;
	int nLength=nEnd-nBegin+1;
	float fFirst=SegAverage(nBegin-1,-nSegLength);
	float fLast=SegAverage(nEnd+1,nSegLength);
	float fSlope=(fLast-fFirst)/float(nEnd-nBegin);
	for(int i=0;i<nLength;i++)
	{
		i0=i%m_nBrigeSegLength;
		fVal=fFirst+float(i)*fSlope+*(m_pfData+i0+m_nBrigeSegStart)-m_fBrigeAmpBegin-m_fBrigeSlope*float(i0);
		*(m_pfAdjustedData+nBegin+i)=fVal;
	}
}

CArtifactsHandling::CArtifactsHandling()
{

}


void CArtifactsHandling::ConsolidateArtifacts()
{

}

void CArtifactsHandling::CheckEventList()
{

}

void CArtifactsHandling::CheckEvl()
{
	int nPreviousLevel;
	int nCurrentLevel;
	int nNextLevel;
	int nStart;
	int nLength;
	float fAmp0,fAmp,fAmp1;
	for(int i=1;i<m_structEvlNode.nNumEvents-1;i++)
	{
		nPreviousLevel=*(m_structEvlNode.pstEventCurrentLevel+i-1);
		nCurrentLevel=*(m_structEvlNode.pstEventCurrentLevel+i);
		nNextLevel=*(m_structEvlNode.pstEventCurrentLevel+i+1);
		if(nCurrentLevel>nPreviousLevel&&nCurrentLevel>nNextLevel)
		{
			fAmp0=*(m_structEvlNode.pfAmplitude+i-1);
			fAmp=*(m_structEvlNode.pfAmplitude+i);
			fAmp1=*(m_structEvlNode.pfAmplitude+i+1);
			if((fAmp-fAmp0)<m_fAmplitudeThreshold&&(fAmp-fAmp1)<m_fAmplitudeThreshold)
			{
				structSegmentNode aNode;
				nStart=*(m_structEvlNode.pnLevelStart+i);
				nLength=*(m_structEvlNode.pnLevelLength+i);
				aNode.fFirstPoint=fAmp0;
				aNode.fLastPoint=fAmp1;
				aNode.nFirstPoint=nStart-10;
				aNode.nLastPoint=nStart+nLength+10;
				m_lstArtifactSegments.push_back(aNode);
			}
		}
	}
}

void CArtifactsHandling::ConcatenateArtifacts()
{
	float fLastPoint;
	int   nFirstPoint,nLastPoint;
//	int nBegin=int(m_fTime1*1000.f/m_fSampleInterval);
	int nEnd=int(m_fTime2*1000.f/m_fSampleInterval);
	int nPrevious,nPreviousFirst;
	int nCurrentArtifactType,nPreviousArtifactType;
	CArtifactSegmentDialog aDlg;
	list <structSegmentNode>::iterator pTemp,pTemp0;
	pTemp0=m_lstArtifactSegments.begin();
	pTemp0++;

	for(pTemp=pTemp0;pTemp!=m_lstArtifactSegments.end();pTemp++)
	{
		nFirstPoint=pTemp->nFirstPoint;

		nLastPoint=pTemp->nLastPoint;
		nCurrentArtifactType=pTemp->nArtifactType;

		pTemp0=pTemp;
		pTemp0--;
		nPrevious=pTemp0->nLastPoint;
		nPreviousFirst=pTemp0->nFirstPoint;
		nPreviousArtifactType=pTemp0->nArtifactType;
		if(nFirstPoint-nPrevious<m_nMinArtifactInterval&&nFirstPoint>nPreviousFirst)
		{
			if(nPreviousArtifactType>0&&nCurrentArtifactType>0)
			{
				fLastPoint=pTemp->fLastPoint;
				nLastPoint=pTemp->nLastPoint;
				pTemp0->nLastPoint=nLastPoint;
				pTemp0->fLastPoint=fLastPoint;
				pTemp0->nLength=nLastPoint-pTemp0->nFirstPoint+1;
				pTemp=m_lstArtifactSegments.erase(pTemp);
				pTemp--;
			}
		}		
	}
}

void CArtifactsHandling::ExtendDetectedArtifacts()
{
	int   nFirstPoint,nLastPoint;
	CArtifactSegmentDialog aDlg;
	list <structSegmentNode>::iterator pTemp,pTemp0;
	pTemp0=m_lstArtifactSegments.begin();
	int nArtifactType;
	for(pTemp=pTemp0;pTemp!=m_lstArtifactSegments.end();pTemp++)
	{
		nArtifactType=pTemp->nArtifactType;

		if(nArtifactType>-2)
		{

			nFirstPoint=pTemp->nFirstPoint-m_nArtifactExtention;
			if(nFirstPoint<0) nFirstPoint=0;

			nLastPoint=pTemp->nLastPoint+m_nArtifactExtention;
			if(nLastPoint>m_nDataSize-1) nLastPoint=m_nDataSize-1;
//			if(nFirstPoint<m_nBegin) nFirstPoint=m_nBegin;
//			if(nLastPoint>m_nEnd) nLastPoint=m_nEnd;

			pTemp->nLastPoint=nLastPoint;
			pTemp->nFirstPoint=nFirstPoint;
			pTemp->nLength+=2*m_nArtifactExtention;
		}
	}
}

void CArtifactsHandling::DefineBaselineSegment(int nBegin, int nEnd)
{
	m_vfBaselineSegment.clear();
	int nLength=nEnd-nBegin;
	delete m_pnBLMax;
	m_pnBLMax=new int[nLength/2];
	int *pnLMin=new int[nLength/2];
	int nLN;
	LocalExtrema(m_pfData+nBegin,pnLMin,m_pnBLMax,nLength,nLength,m_nBLMax);
	for(int i=nBegin;i<=nEnd;i++)
	{
		m_vfBaselineSegment.push_back(m_pfData[i]);
	}
	m_bBaselineDefined=true;
}

void CArtifactsHandling::MakeBaseline(int nBegin, int nEnd)
{
	//Working under assumption that nEnd > nBegin
	int nSeed=-1,nPosition=nBegin;
	int nIt, i;
	double dRand;
	int nI,nF;
	float fVal;

	if(m_bBaselineDefined)
	{
		while (nPosition>=nBegin&&nPosition<=nEnd)
		{
			dRand=NR::ran2(nSeed);;
			nIt=int(dRand*m_nBLMax);
			if(nIt<m_nBLMax-1)
			{
				nI=m_pnBLMax[nIt];
				nF=m_pnBLMax[nIt+1];
			}
			else
			{
				nI=m_pnBLMax[nIt-1];
				nF=m_pnBLMax[nIt];
			}

			for(i=nI;i<nF;i++)
			{
				fVal=m_vfBaselineSegment[i];
				m_pfAdjustedData[nPosition]=fVal;
				nPosition++;
			}
		}
	}
	else
	{
		CString sMessage="The baseline segment has not been defined. The requirement has been ignored!";
		AfxMessageBox(sMessage,MB_OK,0);
	}
}

void CArtifactsHandling::MakeBaseline0(int nBegin, int nEnd)
{
	//Working under assumption that nEnd > nBegin

	if(m_bBaselineDefined)
	{
		const int nSegLength=20;
		bool bFirst=true;
		bool bLast=true;

		float fFirst=0.f;
		float fLast=0.f;
		float fVal=0.f;
		int i0=0;
		int nLength=nEnd-nBegin+1;
		int nI=nBegin-nSegLength;
		int nF=nBegin-1;
		if(nI>=0) 
			fFirst=SegAverage0(nI,nF);
		else
			bFirst=false;

		nI=nEnd;
		nF=nEnd+nSegLength;
		if(nF<m_nDataSize) 
			fLast=SegAverage0(nI,nF);
		else
			bLast=false;

		if(!bFirst)
			fFirst=fLast;
		if(!bLast)
			fLast=fFirst;

		float fSlope=(fLast-fFirst)/float(nBegin-nEnd);
		int nBaselineLength=m_vfBaselineSegment.size();

		float fBFirst=0.f, fBEnd=0.f, fBSlope=0.f;
		fBFirst=SegAverage(400,m_vfBaselineSegment);
		fBEnd=SegAverage(-400,m_vfBaselineSegment);
		fBSlope=(fBEnd-fBFirst)/float(nBaselineLength);

		for(int i=0;i<nLength;i++)
		{
			i0=i%nBaselineLength;
//			fVal=fFirst+float(i)*fSlope+m_vfBaselineSegment[i0]-fBFirst-fBSlope*float(i0);
			fVal=m_vfBaselineSegment[i0];
			*(m_pfAdjustedData+nBegin+i)=fVal;
		}
	}
	else
	{
		CString sMessage="The baseline segment has not been defined. The requirement has been ignored!";
		AfxMessageBox(sMessage,MB_OK,0);
	}
}

int CArtifactsHandling::ClosestPoint_AA(int *pnData, int nLength, int nValue, int nKey)
{
	//This function returns the position in nfData whose value is the largest number (smallest if nKey <0) that is smaller (larger if nKey <0) than nValue in an asending and non-degenerated array nfData
	int nPosition=nLength/2, nI=0, nF=nLength-1;

	while(nF>nI+1)
	{
		nPosition=(nF+nI)/2;
		if(pnData[nPosition]>=nValue)
		{
			nF=nPosition;
		}
		else
		{
			nI=nPosition;
		}
	}

	if(nKey>0)
	{
		if(pnData[nI]<nValue)
			nPosition=nI;
		else 
			nPosition=nI-1;
	}
	else
		nPosition=nF;

	if(nPosition<0) nPosition=0;
	if(nPosition>nLength-1) nPosition=nLength-1;

	return nPosition;
}

void CArtifactsHandling::ExcludeSegment(int nI, int nF)
{
	for(int i=nI;i<=nF;i++)
	{
		m_pbExcluded[i]=true;
	}
}
