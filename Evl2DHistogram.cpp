// Evl2DHistogram.cpp: implementation of the CEvl2DHistogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "Evl2DHistogram.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvl2DHistogram::CEvl2DHistogram()
{
	m_struct2DHist.fMinLog=-0.65;
	m_struct2DHist.fMaxLog=5.45;
	m_struct2DHist.fDeltaLog=0.05;
	m_struct2DHist.nNumBinsLogO=110;
	m_struct2DHist.nNumBinsLogC=110;

	m_pnLevelStart=NULL;
	m_pnLevelLength=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pfAmplitude=NULL;
	m_pbExcluded=NULL;
}

CEvl2DHistogram::~CEvl2DHistogram()
{
	delete m_pnLevelStart;
	delete m_pnLevelLength;
	delete m_pstEventCurrentLevel;
	delete m_pfAmplitude;
}

void CEvl2DHistogram::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
						short* pstEventCurrentLevel,CString sEvlFileName,bool* pbExcluded)
{
	m_fTimePerPoint=TimePerPoint;
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
	m_pbExcluded=pbExcluded;

	m_struct2DHist.fRecordLength=0.;
	m_struct2DHist.nNumOfChannels=NumChannels;
	m_struct2DHist.nNumEvents=nNumEvents;
	m_struct2DHist.nMaxLevel=NumLevels;

	for(int i=0;i<nNumEvents;i++)
	{
		*(m_pnLevelStart+i)=*(pnLevelStart+i);
		*(m_pnLevelLength+i)=*(pnLevelLength+i);
		*(m_pstEventCurrentLevel+i)=*(pstEventCurrentLevel+i);
		*(m_pfAmplitude+i)=*(pfAmplitude+i);
		m_struct2DHist.fRecordLength+=*(pnLevelLength+i)*TimePerPoint/1000.;
	}
//	m_fpSummaryFile=fpSummaryFile;
}

void CEvl2DHistogram::Make2DHist()
{
	int i,j,k,l,m;
	for(i=0;i<110;i++)
	{
		for(j=0;j<110;j++)
		{
			for(k=0;k<3;k++)
			{
				//For all these matrices, the first and second indeces are closed-time and open time indeces, respectively.
				m_struct2DHist.HistOC[i][j][k]=0.f;
				m_struct2DHist.HistCO[i][j][k]=0.f;
				m_struct2DHist.HistOMinC[i][j][k]=0.f;
				m_struct2DHist.HistOMaxC[i][j][k]=0.f;
			}
			for(k=0;k<2;k++)
			{
				m_struct2DHist.MeanToCOC[i][j][k]=0.f;
				m_struct2DHist.MeanToCOCNX[i][j][k]=0.f;
				m_struct2DHist.MeanToCOCXN[i][j][k]=0.f;
			}
		}
		m_struct2DHist.bNonEmptyRow[i]=false;
		m_struct2DHist.bNonEmptyColumn[i]=false;
		
		for(j=0;j<3;j++)
		{
			m_struct2DHist.MeanToOC[i][j]=0.f;
			m_struct2DHist.MeanToCO[i][j]=0.f;
			m_struct2DHist.MeanToOXC[i][j]=0.f;
			m_struct2DHist.MeanToONC[i][j]=0.f;

			m_struct2DHist.CountsToOC[i][j]=0.f;
			m_struct2DHist.CountsToCO[i][j]=0.f;
			m_struct2DHist.CountsToOXC[i][j]=0.f;
			m_struct2DHist.CountsToONC[i][j]=0.f;
		}
	}

	m_struct2DHist.nIndexLogOMax=0;
	m_struct2DHist.nIndexLogCMax=0;
	m_struct2DHist.nIndexLogOMin=999999;
	m_struct2DHist.nIndexLogCMin=999999;

	float fTime,fMinLog=m_struct2DHist.fMinLog;
	float fDeltaLog=m_struct2DHist.fDeltaLog;
	int nIndexLogO,nIndexLogOMax,nIndexLogCMax,nIndexLogCMin;
	int nIndexLogC0,nIndexLogC2;
	int nLength0,nLength1,nLength2;
	float fTime0,fTime1,fTime2,fTimeMin,fTimeMax;
	int nLevel0,nLevel1,nLevel2;
	double dLog10=log(10.);
	float fTemp;

	for(i=1;i<m_nNumEvents-1;i++)
	{
		nLevel1=*(m_pstEventCurrentLevel+i);
		if(nLevel1==1)
		{
			if(!m_pbExcluded[i])
			{
				nLevel0=*(m_pstEventCurrentLevel+i-1);
				nLevel2=*(m_pstEventCurrentLevel+i+1);
				nLength0=*(m_pnLevelLength+i-1);
				nLength1=*(m_pnLevelLength+i);
				nLength2=*(m_pnLevelLength+i+1);
				fTime0=nLength0*m_fTimePerPoint/1000.f;
				fTime1=nLength1*m_fTimePerPoint/1000.f;
				fTime2=nLength2*m_fTimePerPoint/1000.f;
				nIndexLogO=int((log(fTime1)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogO<0) nIndexLogO=999999;
				nIndexLogC0=int((log(fTime0)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogC0<0) nIndexLogO=999999;
				nIndexLogC2=int((log(fTime2)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogC2<0) nIndexLogO=999999;

				if(nIndexLogO<m_struct2DHist.nNumBinsLogO&&nIndexLogC0<m_struct2DHist.nNumBinsLogC
					&&nIndexLogC2<m_struct2DHist.nNumBinsLogC)
				{
					m_struct2DHist.HistOC[nIndexLogC2][nIndexLogO][0]+=1.f;
					m_struct2DHist.HistCO[nIndexLogC0][nIndexLogO][0]+=1.f;

					m_struct2DHist.HistOC[nIndexLogC2][nIndexLogO][1]+=fTime1;
					m_struct2DHist.HistCO[nIndexLogC0][nIndexLogO][1]+=fTime1;

					m_struct2DHist.HistOC[nIndexLogC2][nIndexLogO][2]+=fTime2;
					m_struct2DHist.HistCO[nIndexLogC0][nIndexLogO][2]+=fTime0;

					m_struct2DHist.MeanToCOC[nIndexLogC0][nIndexLogC2][0]+=1.f;
					m_struct2DHist.MeanToCOC[nIndexLogC0][nIndexLogC2][1]+=fTime1;

					if(nIndexLogC0>nIndexLogC2)
					{
						nIndexLogCMin=nIndexLogC2;
						nIndexLogCMax=nIndexLogC0;
						fTimeMin=fTime2;
						fTimeMax=fTime0;
					}
					else
					{
						nIndexLogCMin=nIndexLogC0;
						nIndexLogCMax=nIndexLogC2;
						fTimeMin=fTime0;
						fTimeMax=fTime2;
					}


					m_struct2DHist.HistOMinC[nIndexLogCMin][nIndexLogO][0]+=1.f;
					m_struct2DHist.HistOMaxC[nIndexLogCMax][nIndexLogO][0]+=1.f;

					m_struct2DHist.HistOMinC[nIndexLogCMin][nIndexLogO][1]+=fTime1;
					m_struct2DHist.HistOMaxC[nIndexLogCMax][nIndexLogO][1]+=fTime1;

					m_struct2DHist.HistOMinC[nIndexLogCMin][nIndexLogO][2]+=fTimeMin;
					m_struct2DHist.HistOMaxC[nIndexLogCMax][nIndexLogO][2]+=fTimeMax;

					m_struct2DHist.bNonEmptyColumn[nIndexLogO]=true;
					m_struct2DHist.bNonEmptyRow[nIndexLogCMin]=true;
					m_struct2DHist.bNonEmptyRow[nIndexLogCMax]=true;

					if(nIndexLogO>m_struct2DHist.nIndexLogOMax)
					{
						m_struct2DHist.nIndexLogOMax=nIndexLogO;
					}
					if(nIndexLogCMax>m_struct2DHist.nIndexLogCMax)
					{
						m_struct2DHist.nIndexLogCMax=nIndexLogCMax;
					}
					if(nIndexLogO<m_struct2DHist.nIndexLogOMin)
					{
						m_struct2DHist.nIndexLogOMin=nIndexLogO;
					}
					if(nIndexLogCMin<m_struct2DHist.nIndexLogCMin)
					{
						m_struct2DHist.nIndexLogCMin=nIndexLogCMin;
					}
				}
			}
		}
	}

//	Calculating Conditional Mean To

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
		{
			fTemp=m_struct2DHist.HistOC[k][j][1];
			m_struct2DHist.MeanToOC[i][1]+=m_struct2DHist.HistOC[i][j][1];
			m_struct2DHist.MeanToCO[i][1]+=m_struct2DHist.HistCO[i][j][1];
			m_struct2DHist.MeanToOXC[i][1]+=m_struct2DHist.HistOMaxC[i][j][1];
			m_struct2DHist.MeanToONC[i][1]+=m_struct2DHist.HistOMinC[i][j][1];

			m_struct2DHist.CountsToOC[i][1]+=m_struct2DHist.HistOC[i][j][0];
			m_struct2DHist.CountsToCO[i][1]+=m_struct2DHist.HistCO[i][j][0];
			m_struct2DHist.CountsToOXC[i][1]+=m_struct2DHist.HistOMaxC[i][j][0];
			m_struct2DHist.CountsToONC[i][1]+=m_struct2DHist.HistOMinC[i][j][0];			
		}
	}

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=m_struct2DHist.nIndexLogCMin;j<i;j++)
		{
			m_struct2DHist.MeanToOC[i][0]+=m_struct2DHist.MeanToOC[j][1];
			m_struct2DHist.MeanToCO[i][0]+=m_struct2DHist.MeanToCO[j][1];
			m_struct2DHist.MeanToOXC[i][0]+=m_struct2DHist.MeanToOXC[j][1];
			m_struct2DHist.MeanToONC[i][0]+=m_struct2DHist.MeanToONC[j][1];

			m_struct2DHist.CountsToOC[i][0]+=m_struct2DHist.CountsToOC[j][1];
			m_struct2DHist.CountsToCO[i][0]+=m_struct2DHist.CountsToCO[j][1];
			m_struct2DHist.CountsToOXC[i][0]+=m_struct2DHist.CountsToOXC[j][1];
			m_struct2DHist.CountsToONC[i][0]+=m_struct2DHist.CountsToONC[j][1];
		}
	}

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=i;j<=m_struct2DHist.nIndexLogCMax;j++)
		{
			m_struct2DHist.MeanToOC[i][2]+=m_struct2DHist.MeanToOC[j][1];
			m_struct2DHist.MeanToCO[i][2]+=m_struct2DHist.MeanToCO[j][1];
			m_struct2DHist.MeanToOXC[i][2]+=m_struct2DHist.MeanToOXC[j][1];
			m_struct2DHist.MeanToONC[i][2]+=m_struct2DHist.MeanToONC[j][1];

			m_struct2DHist.CountsToOC[i][2]+=m_struct2DHist.CountsToOC[j][1];
			m_struct2DHist.CountsToCO[i][2]+=m_struct2DHist.CountsToCO[j][1];
			m_struct2DHist.CountsToOXC[i][2]+=m_struct2DHist.CountsToOXC[j][1];
			m_struct2DHist.CountsToONC[i][2]+=m_struct2DHist.CountsToONC[j][1];
		}
	}

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=0;j<3;j++)
		{
			m_struct2DHist.MeanToOC[i][j]/=m_struct2DHist.CountsToOC[i][j];
			m_struct2DHist.MeanToCO[i][j]/=m_struct2DHist.CountsToCO[i][j];
			m_struct2DHist.MeanToOXC[i][j]/=m_struct2DHist.CountsToOXC[i][j];
			m_struct2DHist.MeanToONC[i][j]/=m_struct2DHist.CountsToONC[i][j];
		}
	}

	//Calculating MeanToCOCNX and MeanToCOCXN

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=m_struct2DHist.nIndexLogCMin;j<=m_struct2DHist.nIndexLogCMax;j++)
		{
			for(k=m_struct2DHist.nIndexLogCMin;k<i;k++)
			{
				for(l=j;l<=m_struct2DHist.nIndexLogCMax;l++)
				{
					for(m=0;m<2;m++)
					{
						m_struct2DHist.MeanToCOCNX[i][j][m]+=m_struct2DHist.MeanToCOC[k][l][m];
					}
				}
			}

			for(k=i;k<=m_struct2DHist.nIndexLogCMax;k++)
			{
				for(l=m_struct2DHist.nIndexLogCMin;l<j;l++)
				{
					for(m=0;m<2;m++)
					{
						m_struct2DHist.MeanToCOCXN[i][j][m]+=m_struct2DHist.MeanToCOC[k][l][m];
					}
				}
			}
		}
	}

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=m_struct2DHist.nIndexLogCMin;j<=m_struct2DHist.nIndexLogCMax;j++)
		{
			m_struct2DHist.MeanToCOCNX[i][j][1]/=m_struct2DHist.MeanToCOCNX[i][j][0];
			m_struct2DHist.MeanToCOCXN[i][j][1]/=m_struct2DHist.MeanToCOCXN[i][j][0];
		}
	}
}

void CEvl2DHistogram::Export2DEvlHistogram()
{
	CalSem();//need to fix this function later. 1/20/2006
	CString sFileName=m_sEvlFileName;
	sFileName=ChangeExt(sFileName,"OCH");
	FILE* fpOC=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"COH");
	FILE* fpCO=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"OXC");
	FILE* fpOXC=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"ONC");
	FILE* fpONC=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"ToM");
	FILE* fpToM=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"CNC");
	FILE* fpCNC=fopen(sFileName,"w");
	sFileName=ChangeExt(sFileName,"CXC");
	FILE* fpCXC=fopen(sFileName,"w");


	fprintf(fpOC,"%s","    Time ");
	fprintf(fpCO,"%s","    Time ");
	fprintf(fpOXC,"%s","    Time ");
	fprintf(fpONC,"%s","    Time ");
	fprintf(fpToM,"%s","    Time ");
	fprintf(fpCNC,"%s","    Time ");
	fprintf(fpCXC,"%s","    Time ");

	fprintf(fpOC,"%s","Index");
	fprintf(fpCO,"%s","Index");
	fprintf(fpOXC,"%s","Index");
	fprintf(fpONC,"%s","Index");
	fprintf(fpToM,"%s","Index");
	fprintf(fpCNC,"%s","Index");
	fprintf(fpCXC,"%s","Index");

	int i,j;
	double dLog10=log(10.);
	float fTime=0.f;
	for(i=m_struct2DHist.nIndexLogOMin;i<=m_struct2DHist.nIndexLogOMax;i++)
	{
		if(m_struct2DHist.bNonEmptyColumn[i])
		{
			fprintf(fpOC,"  %3d",i);
			fprintf(fpCO,"  %3d",i);
			fprintf(fpOXC,"  %3d",i);
			fprintf(fpONC,"  %3d",i);
			fprintf(fpCNC,"   %3d",i);
			fprintf(fpCXC,"   %3d",i);
		}
	}

	for(i=0;i<3;i++)
	{
		fprintf(fpToM,"   ToOC_%1d  SemOC_%1d   ToCO_%1d  SemCO_%1d  ToONC_%1d SemONC_%1d  ToOXC_%1d SemOXC_%1d",i,i,i,i,i,i,i,i);  //07613
//		fprintf(fpToM,"   ToOC_%1d   ToCO_%1d  ToONC_%1d  ToOXC_%1d",i,i,i,i);
	}

	fprintf(fpToM,"  ToCOCNX SemCOCNX  ToCOCXN SemCOCXN");

	for(i=0;i<3;i++)
	{
		fprintf(fpToM,"   ToOC_%1d   ToCO_%1d  ToONC_%1d  ToOXC_%1d",i,i,i,i);
	}

	fprintf(fpToM,"  ToCOCNX  ToCOCXN");


	fprintf(fpOC,"\n");
	fprintf(fpCO,"\n");
	fprintf(fpOXC,"\n");
	fprintf(fpONC,"\n");
	fprintf(fpToM,"\n");
	fprintf(fpCNC,"\n");
	fprintf(fpCXC,"\n");

	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		if(m_struct2DHist.bNonEmptyRow[i])
		{
			fTime=m_struct2DHist.fMinLog+m_struct2DHist.fDeltaLog*double(i);
			fTime=exp(fTime*dLog10);
			fprintf(fpOC," %7.1f ",fTime);
			fprintf(fpCO," %7.1f ",fTime);
			fprintf(fpOXC," %7.1f ",fTime);
			fprintf(fpONC," %7.1f ",fTime);
			fprintf(fpToM," %7.1f ",fTime);
			fprintf(fpCNC," %7.1f ",fTime);
			fprintf(fpCXC," %7.1f ",fTime);
			fprintf(fpOC,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpOC,"% 4.0f",m_struct2DHist.HistOC[i][j][0]);
			}
			fprintf(fpCO,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpCO,"% 4.0f",m_struct2DHist.HistCO[i][j][0]);
			}
			fprintf(fpOXC,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpOXC,"% 4.0f",m_struct2DHist.HistOMaxC[i][j][0]);
			}
			fprintf(fpONC,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpONC,"% 4.0f",m_struct2DHist.HistOMinC[i][j][0]);
			}

			fprintf(fpCNC,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpCNC,"%6.2f",m_struct2DHist.MeanToCOCNX[i][j][1]);
			}

			fprintf(fpCXC,"%5d",i);
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) fprintf(fpCXC,"%6.2f",m_struct2DHist.MeanToCOCXN[i][j][1]);
			}

			fprintf(fpToM,"%5d",i);
			for(j=0;j<3;j++)
			{
				fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToOC[i][j]);
				fprintf(fpToM,"%9.3f",m_struct2DHist.SemToOC[i][j]);
				fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToCO[i][j]);
				fprintf(fpToM,"%9.3f",m_struct2DHist.SemToCO[i][j]);
				fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToONC[i][j]);
				fprintf(fpToM,"%9.3f",m_struct2DHist.SemToONC[i][j]);
				fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToOXC[i][j]);
				fprintf(fpToM,"%9.3f",m_struct2DHist.SemToOXC[i][j]);
			}

			fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToCOCNX[i][i][1]);
			fprintf(fpToM,"%9.3f",m_struct2DHist.SemToCOCNX[i]);
			fprintf(fpToM,"%9.2f",m_struct2DHist.MeanToCOCXN[i][i][1]);
			fprintf(fpToM,"%9.3f",m_struct2DHist.SemToCOCXN[i]);

			for(j=0;j<3;j++)
			{
				fprintf(fpToM,"%9.0f",m_struct2DHist.CountsToOC[i][j]);
				fprintf(fpToM,"%9.0f",m_struct2DHist.CountsToCO[i][j]);
				fprintf(fpToM,"%9.0f",m_struct2DHist.CountsToONC[i][j]);
				fprintf(fpToM,"%9.0f",m_struct2DHist.CountsToOXC[i][j]);
			}

			fprintf(fpToM,"%9.0f",m_struct2DHist.MeanToCOCNX[i][i][0]);
			fprintf(fpToM,"%9.0f",m_struct2DHist.MeanToCOCXN[i][i][0]);


			fprintf(fpOC,"\n");
			fprintf(fpCO,"\n");
			fprintf(fpOXC,"\n");
			fprintf(fpONC,"\n");
			fprintf(fpToM,"\n");
			fprintf(fpCNC,"\n");
			fprintf(fpCXC,"\n");
		}
	}
	fclose(fpOC);
	fclose(fpCO);
	fclose(fpOXC);
	fclose(fpONC);
	fclose(fpToM);
	fclose(fpCNC);
	fclose(fpCXC);
	Export2DEvlHistFor3DPlot();
}


CString CEvl2DHistogram::ChangeExt(CString sFileName, CString sNewExt)
{
	int i=0,nLength=0;
	CString sNewName=sFileName;
	nLength=sFileName.GetLength();
	for(i=0;i<3;i++)
	{
		sNewName.SetAt(nLength-3+i,sNewExt[i]);
	}
	return sNewName;
}


void CEvl2DHistogram::FilteringEvents()
{
	int i,j;
	float fTime;
	float fDeltaLog=m_struct2DHist.fDeltaLog;
	float fTime0,fTime1,fTime2;
	int nLevel0,nLevel1,nLevel2;
	double dLog10=log(10.);
	float fTemp;
	int nLength0,nLength1,nLength2;

	for(i=1;i<m_nNumEvents-1;i++)
	{
		m_pbExcluded[i]=true;
	}

	for(i=1;i<m_nNumEvents-1;i++)
	{
		nLevel1=*(m_pstEventCurrentLevel+i);
		if(nLevel1==1)
		{
			nLevel0=*(m_pstEventCurrentLevel+i-1);
			nLevel2=*(m_pstEventCurrentLevel+i+1);
			nLength0=*(m_pnLevelLength+i-1);
			nLength1=*(m_pnLevelLength+i);
			nLength2=*(m_pnLevelLength+i+1);
			fTime0=nLength0*m_fTimePerPoint/1000.f;
			fTime1=nLength1*m_fTimePerPoint/1000.f;
			fTime2=nLength2*m_fTimePerPoint/1000.f;
			if(fTime0>0.8&&fTime0<1.5&&fTime2>0.8&&fTime2<1.5)
			{
				m_pbExcluded[i]=false;
			}
		}
	}

	for(i=1;i<m_nNumEvents-1;i++)
	{
		nLevel1=*(m_pstEventCurrentLevel+i);
		if(nLevel1==0)
		{
			nLevel0=*(m_pstEventCurrentLevel+i-1);
			nLevel2=*(m_pstEventCurrentLevel+i+1);
			nLength0=*(m_pnLevelLength+i-1);
			nLength1=*(m_pnLevelLength+i);
			nLength2=*(m_pnLevelLength+i+1);
			fTime0=nLength0*m_fTimePerPoint/1000.f;
			fTime1=nLength1*m_fTimePerPoint/1000.f;
			fTime2=nLength2*m_fTimePerPoint/1000.f;
			if(fTime0>7.&&fTime2>7.)
			{
				m_pbExcluded[i]=false;
			}
		}
	}
}

void CEvl2DHistogram::Export2DEvlHistFor3DPlot()
{
	CString sFileName=m_sEvlFileName;
	sFileName=ChangeExt(sFileName,"H3D");
	FILE* fp3D=fopen(sFileName,"w");

	fprintf(fp3D,"%s","          Tc");
	fprintf(fp3D,"%s","          To");
	fprintf(fp3D,"%s","    Conts_OC");
	fprintf(fp3D,"%s","    Conts_CO");
	fprintf(fp3D,"%s","   Conts_OXC");
	fprintf(fp3D,"%s","   Conts_ONC");
	fprintf(fp3D,"\n");

	int i,j;
	double dLog10=log(10.);
	float fTime=0.f;


	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		if(m_struct2DHist.bNonEmptyRow[i])
		{
			for(j=m_struct2DHist.nIndexLogOMin;j<=m_struct2DHist.nIndexLogOMax;j++)
			{
				if(m_struct2DHist.bNonEmptyColumn[j]) 
				{
					fTime=m_struct2DHist.fMinLog+m_struct2DHist.fDeltaLog*double(i);
					fTime=exp(fTime*dLog10);
					fprintf(fp3D,"  %10.2f ",fTime);

					fTime=m_struct2DHist.fMinLog+m_struct2DHist.fDeltaLog*double(j);
					fTime=exp(fTime*dLog10);
					fprintf(fp3D,"  %10.2f ",fTime);

					fprintf(fp3D,"%  10.0f",m_struct2DHist.HistOC[i][j][0]);
					fprintf(fp3D,"%  10.0f",m_struct2DHist.HistCO[i][j][0]);
					fprintf(fp3D,"%  10.0f",m_struct2DHist.HistOMaxC[i][j][0]);
					fprintf(fp3D,"%  10.0f",m_struct2DHist.HistOMinC[i][j][0]);
					fprintf(fp3D,"\n");
				}
			}
		}
	}
	fclose(fp3D);
}

void CEvl2DHistogram::CalSem()
{
	//This function was written on 1/20/2006 and have not checked
	int i,j,k,l,m;
	for(i=0;i<110;i++)
	{
		m_struct2DHist.SemToCOC[i]=0.f;
		m_struct2DHist.SemToCOCNX[i]=0.f;
		m_struct2DHist.SemToCOCXN[i]=0.f;

		for(j=0;j<3;j++)
		{
			m_struct2DHist.SemToOC[i][j]=0.f;
			m_struct2DHist.SemToCO[i][j]=0.f;
			m_struct2DHist.SemToOXC[i][j]=0.f;
			m_struct2DHist.SemToONC[i][j]=0.f;
		}
	}

//	m_struct2DHist.nIndexLogOMax=0;
//	m_struct2DHist.nIndexLogCMax=0;
//	m_struct2DHist.nIndexLogOMin=999999;
//	m_struct2DHist.nIndexLogCMin=999999;

	float fTime,fMinLog=m_struct2DHist.fMinLog;
	float fDeltaLog=m_struct2DHist.fDeltaLog;
	int nIndexLogO,nIndexLogOMax,nIndexLogCMax,nIndexLogCMin;
	int nIndexLogC0,nIndexLogC2;
	int nLength0,nLength1,nLength2;
	float fTime0,fTime1,fTime2,fTimeMin,fTimeMax;
	int nLevel0,nLevel1,nLevel2;
	double dLog10=log(10.);
	float fTemp;

	for(i=1;i<m_nNumEvents-1;i++)
	{
		nLevel1=*(m_pstEventCurrentLevel+i);
		if(nLevel1==1)
		{
			if(!m_pbExcluded[i])
			{
				nLevel0=*(m_pstEventCurrentLevel+i-1);
				nLevel2=*(m_pstEventCurrentLevel+i+1);
				nLength0=*(m_pnLevelLength+i-1);
				nLength1=*(m_pnLevelLength+i);
				nLength2=*(m_pnLevelLength+i+1);
				fTime0=nLength0*m_fTimePerPoint/1000.f;
				fTime1=nLength1*m_fTimePerPoint/1000.f;
				fTime2=nLength2*m_fTimePerPoint/1000.f;
				nIndexLogO=int((log(fTime1)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogO<0) nIndexLogO=999999;
				nIndexLogC0=int((log(fTime0)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogC0<0) nIndexLogO=999999;
				nIndexLogC2=int((log(fTime2)/dLog10-fMinLog)/fDeltaLog);
				if(nIndexLogC2<0) nIndexLogO=999999;

				if(nIndexLogO<m_struct2DHist.nNumBinsLogO&&nIndexLogC0<m_struct2DHist.nNumBinsLogC
					&&nIndexLogC2<m_struct2DHist.nNumBinsLogC)
				{
					if(nIndexLogC0==nIndexLogC2)
					{
						m_struct2DHist.SemToCOC[nIndexLogC0]+=(fTime1-m_struct2DHist.MeanToCOC[nIndexLogC0][nIndexLogC2][1])*(fTime1-m_struct2DHist.MeanToCOC[nIndexLogC0][nIndexLogC2][1]);
						nIndexLogCMin=nIndexLogC2;
						nIndexLogCMax=nIndexLogC0;
						fTimeMin=fTime2;
						fTimeMax=fTime0;
					}
					else if(nIndexLogC0>nIndexLogC2)
					{
						nIndexLogCMin=nIndexLogC2;
						nIndexLogCMax=nIndexLogC0;
						fTimeMin=fTime2;
						fTimeMax=fTime0;
					}
					else
					{
						nIndexLogCMin=nIndexLogC0;
						nIndexLogCMax=nIndexLogC2;
						fTimeMin=fTime0;
						fTimeMax=fTime2;
					}

					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogC2;j++)
					{
//						m_struct2DHist.SemToOC[nIndexLogC2][2]+=(m_struct2DHist.MeanToOC[nIndexLogC2][2]-fTime1)*(m_struct2DHist.MeanToOC[nIndexLogC2][2]-fTime1);
						m_struct2DHist.SemToOC[j][2]+=(m_struct2DHist.MeanToOC[j][2]-fTime1)*(m_struct2DHist.MeanToOC[j][2]-fTime1);
					}
					m_struct2DHist.SemToOC[nIndexLogC2][1]+=(m_struct2DHist.MeanToOC[nIndexLogC2][1]-fTime1)*(m_struct2DHist.MeanToOC[nIndexLogC2][1]-fTime1);
					for(j=nIndexLogC2;j<=m_struct2DHist.nIndexLogCMax;j++)
					{
//						m_struct2DHist.SemToOC[nIndexLogC2][0]+=(m_struct2DHist.MeanToOC[nIndexLogC2][0]-fTime1)*(m_struct2DHist.MeanToOC[nIndexLogC2][0]-fTime1);
						m_struct2DHist.SemToOC[j][0]+=(m_struct2DHist.MeanToOC[j][0]-fTime1)*(m_struct2DHist.MeanToOC[j][0]-fTime1);
					}


					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogC0;j++)
					{
//						m_struct2DHist.SemToCO[nIndexLogC0][2]+=(m_struct2DHist.MeanToCO[nIndexLogC0][2]-fTime1)*(m_struct2DHist.MeanToCO[nIndexLogC0][2]-fTime1);
						m_struct2DHist.SemToCO[j][2]+=(m_struct2DHist.MeanToCO[j][2]-fTime1)*(m_struct2DHist.MeanToCO[j][2]-fTime1);
					}
//					m_struct2DHist.SemToCO[nIndexLogC0][1]+=(m_struct2DHist.MeanToCO[nIndexLogC0][1]-fTime1)*(m_struct2DHist.MeanToCO[nIndexLogC2][1]-fTime1);
					m_struct2DHist.SemToCO[nIndexLogC0][1]+=(m_struct2DHist.MeanToCO[nIndexLogC0][1]-fTime1)*(m_struct2DHist.MeanToCO[nIndexLogC0][1]-fTime1);
					for(j=nIndexLogC0;j<=m_struct2DHist.nIndexLogCMax;j++)
					{
//						m_struct2DHist.SemToCO[nIndexLogC0][0]+=(m_struct2DHist.MeanToCO[nIndexLogC0][0]-fTime1)*(m_struct2DHist.MeanToCO[nIndexLogC0][0]-fTime1);
						m_struct2DHist.SemToCO[j][0]+=(m_struct2DHist.MeanToCO[j][0]-fTime1)*(m_struct2DHist.MeanToCO[j][0]-fTime1);
					}


//					for(j=nIndexLogCMin;j<=m_struct2DHist.nIndexLogCMax;j++)
//					{
//						m_struct2DHist.SemToONC[nIndexLogCMin][2]+=(m_struct2DHist.MeanToONC[nIndexLogCMin][2]-fTime1)*(m_struct2DHist.MeanToONC[nIndexLogCMin][2]-fTime1);
//					}
					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogCMin;j++)
					{
						m_struct2DHist.SemToONC[j][2]+=(m_struct2DHist.MeanToONC[j][2]-fTime1)*(m_struct2DHist.MeanToONC[j][2]-fTime1);
					}
					m_struct2DHist.SemToONC[nIndexLogCMin][1]+=(m_struct2DHist.MeanToONC[nIndexLogCMin][1]-fTime1)*(m_struct2DHist.MeanToONC[nIndexLogCMin][1]-fTime1);
//					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogCMin;j++)
//					{
//						m_struct2DHist.SemToONC[nIndexLogCMin][0]+=(m_struct2DHist.MeanToONC[nIndexLogCMin][0]-fTime1)*(m_struct2DHist.MeanToONC[nIndexLogCMin][0]-fTime1);
//					}

					for(j=nIndexLogCMin;j<=m_struct2DHist.nIndexLogCMax;j++)
					{
						m_struct2DHist.SemToONC[j][0]+=(m_struct2DHist.MeanToONC[j][0]-fTime1)*(m_struct2DHist.MeanToONC[j][0]-fTime1);
					}


//					for(j=nIndexLogCMax;j<=m_struct2DHist.nIndexLogCMax;j++)
//					{
//						m_struct2DHist.SemToOXC[nIndexLogCMax][2]+=(m_struct2DHist.MeanToOXC[nIndexLogCMax][2]-fTime1)*(m_struct2DHist.MeanToOXC[nIndexLogCMax][2]-fTime1);
//					}
//					m_struct2DHist.SemToOXC[nIndexLogCMax][1]+=(m_struct2DHist.MeanToOXC[nIndexLogCMax][1]-fTime1)*(m_struct2DHist.MeanToOXC[nIndexLogCMax][1]-fTime1);
//					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogCMax;j++)
//					{
//						m_struct2DHist.SemToOXC[nIndexLogCMax][0]+=(m_struct2DHist.MeanToOXC[nIndexLogCMax][0]-fTime1)*(m_struct2DHist.MeanToOXC[nIndexLogCMax][0]-fTime1);
//					}

					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogCMax;j++)
					{
						m_struct2DHist.SemToOXC[j][2]+=(m_struct2DHist.MeanToOXC[j][2]-fTime1)*(m_struct2DHist.MeanToOXC[j][2]-fTime1);
					}
					m_struct2DHist.SemToOXC[nIndexLogCMax][1]+=(m_struct2DHist.MeanToOXC[nIndexLogCMax][1]-fTime1)*(m_struct2DHist.MeanToOXC[nIndexLogCMax][1]-fTime1);
					for(j=nIndexLogCMax;j<=m_struct2DHist.nIndexLogCMax;j++)
					{
						m_struct2DHist.SemToOXC[j][0]+=(m_struct2DHist.MeanToOXC[j][0]-fTime1)*(m_struct2DHist.MeanToOXC[j][0]-fTime1);
					}


//					for(j=m_struct2DHist.nIndexLogCMin;j<nIndexLogC0;j++)
//					{
//						for(k=nIndexLogC2;k<=m_struct2DHist.nIndexLogCMax;k++)
//						{
//							m_struct2DHist.SemToCOCNX[j]+=(m_struct2DHist.MeanToCOCNX[j][k][1]-fTime1)*(m_struct2DHist.MeanToCOCNX[j][k][1]-fTime1);
//						}
//					}

					for(j=nIndexLogC0+1;j<nIndexLogC2;j++)
					{
						m_struct2DHist.SemToCOCNX[j]+=(m_struct2DHist.MeanToCOCNX[j][j][1]-fTime1)*(m_struct2DHist.MeanToCOCNX[j][j][1]-fTime1);
					}	


					for(j=nIndexLogC2+1;j<nIndexLogC0;j++)
					{
						m_struct2DHist.SemToCOCXN[j]+=(m_struct2DHist.MeanToCOCXN[j][j][1]-fTime1)*(m_struct2DHist.MeanToCOCXN[j][j][1]-fTime1);
					}
				}
			}
		}
	}
	//	Calculating Conditional Mean To


	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		for(j=0;j<3;j++)
		{
			m_struct2DHist.SemToOC[i][j]=sqrt(m_struct2DHist.SemToOC[i][j]/(m_struct2DHist.CountsToOC[i][j]*(m_struct2DHist.CountsToOC[i][j]-1)));
			m_struct2DHist.SemToCO[i][j]=sqrt(m_struct2DHist.SemToCO[i][j]/(m_struct2DHist.CountsToCO[i][j]*(m_struct2DHist.CountsToCO[i][j]-1)));
			m_struct2DHist.SemToOXC[i][j]=sqrt(m_struct2DHist.SemToOXC[i][j]/(m_struct2DHist.CountsToOXC[i][j]*(m_struct2DHist.CountsToOXC[i][j]-1)));
			m_struct2DHist.SemToONC[i][j]=sqrt(m_struct2DHist.SemToONC[i][j]/(m_struct2DHist.CountsToONC[i][j]*(m_struct2DHist.CountsToONC[i][j]-1)));
		}
	}
	//Calculating MeanToCOCNX and MeanToCOCXN


	for(i=m_struct2DHist.nIndexLogCMin;i<=m_struct2DHist.nIndexLogCMax;i++)
	{
		m_struct2DHist.SemToCOCNX[i]=sqrt(m_struct2DHist.SemToCOCNX[i]/(m_struct2DHist.MeanToCOCNX[i][i][0]*(m_struct2DHist.MeanToCOCNX[i][j][0]-1)));
		m_struct2DHist.SemToCOCXN[i]=sqrt(m_struct2DHist.SemToCOCXN[i]/(m_struct2DHist.MeanToCOCXN[i][i][0]*(m_struct2DHist.MeanToCOCNX[i][j][0]-1)));
	}
}
