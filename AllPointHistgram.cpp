// AllPoingHistgram.cpp: implementation of the CAllPoingHistgram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "AllPointHistgram.h"
#include "math.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAllPointHistgram::CAllPointHistgram(CString sFileName, float* pfData, int nLength, float fSampleInterval, bool bShowTrace, int nReductionFactor, int nPolarity, float fTransitionThreshold, float fTime0)
{
	m_nLength=nLength;
	m_pfData=pfData;
	m_fInterval=0.05f;
	m_fSampleInterval=fSampleInterval;
	m_pfHist=new float[1];
	m_bShowTrace=bShowTrace;
	m_nReductionFactor=nReductionFactor;
	m_pbProtected=NULL;
	m_pnLMin=NULL;
	m_pnLMax=NULL;
	m_sAbfFileName=sFileName;
	m_nPolarity=nPolarity;
	m_fTime0=fTime0;
	m_fTransitionThreshold=fTransitionThreshold;
	CalHist();
	
	delete m_pbProtected;
	m_pbProtected=new bool[m_nLength];
	for(int i=0;i<m_nLength;i++)
	{
		m_pbProtected[i]=false;
	}

}

CAllPointHistgram::~CAllPointHistgram()
{

}

void CAllPointHistgram::CalHist()
{
	float fAmp=0.;
	int it;
	CalRange();
	InitializeHist();
	for(int i=0; i<m_nLength; i++)
	{
		fAmp=*(m_pfData+i);
		it=int((fAmp-m_fMin)/m_fInterval);
		*(m_pfHist+it)+=1.;
	}
}

void CAllPointHistgram::CalRange()
{
	float fAmp;
	m_fMax=-1000000000.f;
	m_fMin=10000000000.f;
	for(int i=0;i<m_nLength;i++)
	{
		fAmp=*(m_pfData+i);
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

void CAllPointHistgram::InitializeHist()
{
	m_pfHist=new float[m_nSize];
	for(int i=0;i<m_nSize;i++)
	{
		*(m_pfHist+i)=0.f;
	}
}

int CAllPointHistgram::GetHistSize()
{
	return m_nSize;
}

float CAllPointHistgram::GetMinimum()
{
	return m_fMin;
}

float* CAllPointHistgram::GetHistgram()
{
	float* pfHist=new float[m_nSize];
	for(int i=0;i<m_nSize;i++)
	{
		*(pfHist+i)=*(m_pfHist+i);
	}
	return pfHist;
}

void CAllPointHistgram::WriteHist0()
{
	CString sFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	FILE* fp;
	CFileDialog cFileDlg(false, NULL, "*.aph", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
	if(cFileDlg.DoModal()==IDOK)
	{
		sFileName=cFileDlg.GetPathName();
		if(fp=fopen(sFileName,"w"))
		{
			int nLine=0;
	    	for(int i=0;i<m_nLength;i++)
			{
 	 	 	    if(m_bShowTrace)
				{
					if(i%m_nReductionFactor==0)
					{
		    			fprintf(fp,"%12.3f",float(i)*m_fSampleInterval);
						fprintf(fp,"%12.3f",*(m_pfData+i));
						if(nLine<m_nSize)
						{
							fprintf(fp,"%12.3f",float(nLine)*m_fInterval+m_fMin);
					 		fprintf(fp,"%12.3f",*(m_pfHist+nLine));
							nLine++;
						}
						fprintf(fp,"\n");
					}
				}
			    else
				{
				    if(i<m_nSize)
					{
					    fprintf(fp,"%12.3f",float(i)*m_fInterval+m_fMin);
					    fprintf(fp,"%12.3f",*(m_pfHist+i));
					    if(!m_bShowTrace) fprintf(fp,"\n");
					}
				}
			}
		    fclose(fp);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}
}

void CAllPointHistgram::WriteHist()
{
	CString sFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	FILE* fp;
	CFileDialog cFileDlg(false, NULL, "*.aph", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
	sFileName=ChangeFileExt(m_sAbfFileName,"aph");
	strcpy(cFileDlg.m_ofn.lpstrFile,sFileName);
	float fAve=0.f;
	ProtectTransitions();
	int nWindow=2,j,nNum,nI,nF;
	m_nReductionFactor1=m_nLength/(10000);
	if(m_nReductionFactor1<1)
		m_nReductionFactor1=1;
	if(cFileDlg.DoModal()==IDOK)
	{
		sFileName=cFileDlg.GetPathName();
		if(fp=fopen(sFileName,"w"))
		{
			int nLine=0;
	    	for(int i=0;i<m_nLength;i++)
			{
 	 	 	    if(m_bShowTrace)
				{
					if(i%m_nReductionFactor1==0||m_pbProtected[i])
					{
		    			fprintf(fp,"%12.3f",float(i)*m_fSampleInterval+m_fTime0);
						fprintf(fp,"%12.3f",*(m_pfData+i));
						if(nLine<m_nSize)
						{
							fAve=0.;
							nNum=0;
							nI=nLine-nWindow;
							if(nI<0)
								nI=0;
							nF=nLine+nWindow;
							if(nF>m_nSize-1)
								nF=m_nSize-1;
							for(j=nI;j<=nF;j++)
							{
								fAve+=m_pfHist[j];
								nNum++;
							}
							fAve/=double(nNum);

							fprintf(fp,"%12.3f",float(nLine)*m_fInterval+m_fMin);
					 		fprintf(fp,"%12.3f",fAve);
							nLine++;
						}
						fprintf(fp,"\n");
					}
				}
			    else
				{
				    if(i<m_nSize)
					{
						fAve=0.;
						nNum=0;
						nI=nLine-nWindow;
						if(nI<0)
						nI=0;
						nF=nLine+nWindow;
						if(nF>m_nSize-1)
						nF=m_nSize-1;
						for(j=nI;j<=nF;j++)
						{
							fAve+=m_pfHist[j];
							nNum++;
						}
						fAve/=double(nNum);
					    fprintf(fp,"%12.3f",float(i)*m_fInterval+m_fMin);
					    fprintf(fp,"%12.3f",fAve);
					    if(!m_bShowTrace) fprintf(fp,"\n");
					}
				}
			}
		    fclose(fp);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}
}


void CAllPointHistgram::LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax)
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

void CAllPointHistgram::ProtectTransitions()
{
	delete m_pnLMin;
	delete m_pnLMax;
	m_pnLMin=new int[m_nLength/2];
	m_pnLMax=new int[m_nLength/2];
	m_nReductionFactor1=m_nLength/(20000);
	if(m_nReductionFactor1<1) m_nReductionFactor1=1;
	typedef vector <int> nPair;
	vector <nPair> vnProtectedPair;

	vector <bool> vbValidPair;

	int i,j;
	if(m_nPolarity==-1)
	{
		LocalExtrema(m_pfData,m_pnLMin,m_pnLMax,m_nLength,m_nLMin,m_nLMax);
	}
	else
	{
		LocalExtrema(m_pfData,m_pnLMax,m_pnLMin,m_nLength,m_nLMax,m_nLMin);
	}
	float fA0,fA1;
	int nIndex0,nIndex;
	nIndex0=m_pnLMin[0];
	int nTemp;
	fA0=m_pfData[nIndex0];
	m_nNumProtectedPoints=0;
	int nNumProtectedPair=0;
	for(i=1;i<m_nLMin;i++)
	{
		nIndex=m_pnLMin[i];
		fA1=m_pfData[nIndex];
		if(fabs(fA1-fA0)>m_fTransitionThreshold)
		{
			nPair aPair;
			if(fA1>fA0)
			{
				if(m_nPolarity==-1)
				{
					aPair.push_back(nIndex0);

					nTemp=m_pnLMax[i-1];
					if(nTemp>nIndex0)
					{
						aPair.push_back(nTemp);
					}
					else
					{
						nTemp=m_pnLMax[i];
						aPair.push_back(nTemp);
					}
				}
				else
				{
					nTemp=m_pnLMax[i];
					if(nTemp<nIndex)
					{
						aPair.push_back(nTemp);
					}
					else
					{
						nTemp=m_pnLMax[i-1];
						aPair.push_back(nTemp);
					}
					aPair.push_back(nIndex);
				}
			}
			else
			{
				if(m_nPolarity==-1)
				{
					nTemp=m_pnLMax[i];
					if(nTemp<nIndex)
					{
						aPair.push_back(nTemp);
					}
					else
					{
						nTemp=m_pnLMax[i-1];
						aPair.push_back(nTemp);
					}
					aPair.push_back(nIndex);
				}
				else
				{
					aPair.push_back(nIndex0);
					nTemp=m_pnLMax[i-1];
					if(nTemp>nIndex0)
					{
						aPair.push_back(nTemp);
					}
					else
					{
						nTemp=m_pnLMax[i];
						aPair.push_back(nTemp);
					}
				}
			}
			vbValidPair.push_back(true);
			vnProtectedPair.push_back(aPair);
			nNumProtectedPair++;
		}
		fA0=fA1;
		nIndex0=nIndex;
	}

	int nP0=0,nP1=0,nI0=0,nI1=0;
	for(i=0;i<nNumProtectedPair-1;i++)
	{
		nP0=vnProtectedPair[i][0];
		nI0=i;
		nI1=i;
		i++;
		nP1=vnProtectedPair[i][0];
		while(nP1-nP0<m_nReductionFactor1&&i<nNumProtectedPair-1)
		{
			nI1=i;
			i++;
			nP1=vnProtectedPair[i][0];
		}
		for(j=nI0+1;j<nI1;j++)
		{
			vbValidPair[j]=false;
		}
		i=nI1-1;
		if(nI1-1<nI0)
		{
			i=nI0;
		}
	}

	for(i=0;i<nNumProtectedPair-1;i++)
	{
		nP0=vnProtectedPair[i][0];
		nP1=vnProtectedPair[i][1];
		if(vbValidPair[i])
		{
			m_pbProtected[nP0]=true;
			m_pbProtected[nP1]=true;
		}
	}
}

void CAllPointHistgram::ProtectTransitions0()
{
	delete m_pnLMin;
	delete m_pnLMax;
	m_pnLMin=new int[m_nLength/2];
	m_pnLMax=new int[m_nLength/2];
	m_nReductionFactor1=m_nLength/(10000);
//	int nProtectedPair[m_nLength/2][2];
	int i,j;
	if(m_nPolarity==-1)
	{
		LocalExtrema(m_pfData,m_pnLMin,m_pnLMax,m_nLength,m_nLMin,m_nLMax);
	}
	else
	{
		LocalExtrema(m_pfData,m_pnLMax,m_pnLMin,m_nLength,m_nLMax,m_nLMin);
	}
	float fA0,fA1;
	int nIndex0,nIndex;
	nIndex0=m_pnLMin[0];
	int nTemp;
	fA0=m_pfData[nIndex0];
	m_nNumProtectedPoints=0;
	for(i=1;i<m_nLMin;i++)
	{
		nIndex=m_pnLMin[i];
		fA1=m_pfData[nIndex];
		if(fabs(fA1-fA0)>m_fTransitionThreshold)
		{
			if(fA1>fA0)
			{
				if(m_nPolarity==-1)
				{
					m_pbProtected[nIndex0]=true;

					nTemp=m_pnLMax[i-1];
					if(nTemp>nIndex0)
					{
						m_pbProtected[nTemp]=true;
					}
					else
					{
						nTemp=m_pnLMax[i];
						m_pbProtected[nTemp]=true;
					}
				}
				else
				{
					m_pbProtected[nIndex]=true;

					nTemp=m_pnLMax[i];
					if(nTemp<nIndex)
					{
						m_pbProtected[nTemp]=true;
					}
					else
					{
						nTemp=m_pnLMax[i-1];
						m_pbProtected[nTemp]=true;
					}
				}
			}
			else
			{
				if(m_nPolarity==-1)
				{
					m_pbProtected[nIndex]=true;

					nTemp=m_pnLMax[i];
					if(nTemp<nIndex)
					{
						m_pbProtected[nTemp]=true;
					}
					else
					{
						nTemp=m_pnLMax[i-1];
						m_pbProtected[nTemp]=true;
					}
				}
				else
				{
					m_pbProtected[nIndex0]=true;

					nTemp=m_pnLMax[i-1];
					if(nTemp>nIndex0)
					{
						m_pbProtected[nTemp]=true;
					}
					else
					{
						nTemp=m_pnLMax[i];
						m_pbProtected[nTemp]=true;
					}
				}
			}
		}
		fA0=fA1;
		nIndex0=nIndex;
	}
}


CString CAllPointHistgram::ChangeFileExt(CString sFileName, CString sExt)
{
//	char cTemp;
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	for(int i=0;i<3;i++)
	{
		sNewFileName.SetAt(nLength-3+i,sExt[i]);
	}
	return sNewFileName;	
}
