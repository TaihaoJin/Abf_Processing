// DataSimulations.cpp: implementation of the CDataSimulations class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "DataSimulations.h"
#include "nr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataSimulations::CDataSimulations()
{

}

CDataSimulations::~CDataSimulations()
{

}

CDataSimulations::CDataSimulations(CAbf* pAbf,CEvl* pEvl,CString &sEvlFileName)
{
	m_pcAbf=pAbf;
	m_pcEvl=pEvl;
//	GetParameters(sEvlFileName);
	CQMatrixFitting aNode;
	SimulateData1();
	m_sEvlFileName=sEvlFileName;
}
CDataSimulations::CDataSimulations(CAbf* pAbf)
{
	m_pcAbf=pAbf;
	GetSimulationParameters_Q();
	SimulateData();
}

void CDataSimulations::SimulateData()
{

	double dRand1=0.,dRand2;
//	GetNoise();
	m_vstructStateSummary.clear();

	//Form NR::ran1(int &idum)
	//Call with nSeed a negative number to initialize, and do not alter this value between successive callings.
	int i;

	m_nNoiseSeed=-1;

	for(i=0;i<m_nNumStates;i++)
	{
		structStateSummaryNode aNode;
		aNode.dMeanLength=0.;
		aNode.dRecordLength=double(m_nDataSize)*m_fDataSampleInterval;
		aNode.dTLength=0.;
		aNode.nNumEvents=0;
		aNode.fAmp=m_vstructStates[i].fAmp;
		aNode.fDev=m_vstructStates[i].fDev;
		aNode.nLevel=m_vstructStates[i].nLevel;
		aNode.nState=i;
		m_vstructStateSummary.push_back(aNode);
	}

	m_vstructEventList.vstructEventList.clear();
	m_vstructEventList.fTimePerPoint=1000.*m_fDataSampleInterval;
	m_vstructEventList.nNumChannels=1;
	m_vstructEventList.nNumLevels=m_nNumLevels;
	m_vstructEventList.sFileName="None";

	int nK0,nK,nK1; //previous, current and next kinetic states, respectively
	nK=0;

	int nMax=1000000,nFactor=1,nNum=0;
	int nDataSize=m_nDataSize;

	if(m_nDataSize>nMax)
	{
		nFactor=nDataSize/nMax+1;
		m_nDataSize=nDataSize/nFactor;
	}


	float *pfData=new float [m_nDataSize];
	int nSeed=-1;
	CalTMatrix();
	nK0=0;

	int nLevelStart=0;
	nNum=0;

	int nPosition=0;

	for(int in=0;in<10;in++)
	{

		for(i=0;i<nDataSize;i++)
		{
			dRand1=NR::ran2(nSeed);
			dRand2=NR::ran2(nSeed);
			nK=NextState(nK0,dRand1,dRand2);
			if(i-nFactor*(i/nFactor)==0&&nNum<m_nDataSize)
			{
				pfData[nNum]=GetData(nK,nNum);
				nNum++;
			}

			if(nK0!=nK)
			{
				nK=nK;
				structEventNode aNode;
				aNode.bAnchor=false;
				aNode.fAmpDev=m_vstructStates[nK0].fDev;
				aNode.fAmpMax=m_vstructStates[nK0].fAmp;
				aNode.LevelLength=nPosition-nLevelStart;
				aNode.LevelStart=nLevelStart;
				aNode.stLevel=m_vstructStates[nK0].nLevel;
				aNode.stState=nK0;
				m_vstructEventList.vstructEventList.push_back(aNode);

				m_vstructStateSummary[nK0].nNumEvents++;
				m_vstructStateSummary[nK0].dTLength+=double(nPosition-nLevelStart)*m_fDataSampleInterval;
				nLevelStart=nPosition;
			}
			nK0=nK;
			nPosition++;
		}
	}
	
	for(i=0;i<m_nNumStates;i++)
	{
		m_vstructStateSummary[i].dMeanLength=m_vstructStateSummary[i].dTLength/double(m_vstructStateSummary[i].nNumEvents);
	}

	m_pcAbf->UpdateSize(nNum);
	m_pcAbf->UpdateData(pfData);
	m_pcAbf->UpdateSampleInterval(1000.*m_fDataSampleInterval*nFactor);//The unit of sample interval in an abf file is microsecond
}


void CDataSimulations::SimulateData1()
{

	double dRand1=0.,dRand2;
//	GetNoise();
	m_vstructStateSummary.clear();

	//Form NR::ran1(int &idum)
	//Call with nSeed a negative number to initialize, and do not alter this value between successive callings.
	int i;

	m_nNoiseSeed=-1;

	for(i=0;i<m_nNumStates;i++)
	{
		structStateSummaryNode aNode;
		aNode.dMeanLength=0.;
		aNode.dRecordLength=double(m_nDataSize*m_nIterations)*m_fDataSampleInterval;
		aNode.dTLength=0.;
		aNode.nNumEvents=0;
		aNode.fAmp=m_vstructStates[i].fAmp;
		aNode.fDev=m_vstructStates[i].fDev;
		aNode.nLevel=m_vstructStates[i].nLevel;
		aNode.nState=i;
		m_vstructStateSummary.push_back(aNode);
	}

	m_vstructEventList.vstructEventList.clear();
	m_vstructEventList.fTimePerPoint=1000.*m_fDataSampleInterval;
	m_vstructEventList.nNumChannels=1;
	m_vstructEventList.nNumLevels=m_nNumLevels;
	m_vstructEventList.sFileName="None";

	FILE* fpEvl=fopen(m_sEvlFileName,"wb");
	CString sSummaryFileName=ChangeFileExt(m_sEvlFileName,"Sum");
	FILE* fpSum=fopen(sSummaryFileName,"w");
	m_pcEvl->UpdateInterval(1000.*m_fDataSampleInterval);
	m_pcEvl->WriteEvlHead(fpEvl);

	int nK0,nK,nK1; //previous, current and next kinetic states, respectively
	nK=0;


	int nSeed=-1;
	CalTMatrix();
	nK0=0;

	int nLevelStart=0;

	structEventNode aNode;

	int nPosition=0;
	for(int in=0;in<m_nIterations;in++)
	{

		for(i=0;i<m_nDataSize;i++)
		{
			dRand1=NR::ran2(nSeed);
			dRand2=NR::ran2(nSeed);
			nK=NextState(nK0,dRand1,dRand2);

			if(nK0!=nK)
			{
				nK=nK;
				aNode.bAnchor=false;
				aNode.fAmpDev=m_vstructStates[nK0].fDev;
				aNode.fAmpMean=m_vstructStates[nK0].fAmp;
				aNode.LevelLength=nPosition-nLevelStart;
				aNode.LevelStart=nLevelStart;
				aNode.stLevel=m_vstructStates[nK0].nLevel;
				aNode.stState=nK0;

				m_pcEvl->WriteEvlBody(fpEvl,aNode);

				m_vstructStateSummary[nK0].nNumEvents++;
				m_vstructStateSummary[nK0].dTLength+=double(nPosition-nLevelStart)*m_fDataSampleInterval;
				nLevelStart=nPosition;
			}
			nK0=nK;
			nPosition++;
		}
		fclose(fpEvl);
		fpEvl=fopen(m_sEvlFileName,"ab");
	
		for(int i0=0;i0<m_nNumStates;i0++)
		{
			if(m_vstructStateSummary[i0].nNumEvents>0) m_vstructStateSummary[i0].dMeanLength=m_vstructStateSummary[i0].dTLength/double(m_vstructStateSummary[i0].nNumEvents);
		}

		OutputStateSummary(fpSum, in, nSeed);
		fclose(fpSum);
		fpSum=fopen(sSummaryFileName,"a");

	}
	fclose(fpEvl);
	fclose(fpSum);
}


int CDataSimulations::NextState(int nK0, double dRand1, double dRand2)
{
	double dTemp0,dTemp;
	dTemp=m_dTM1[nK0][nK0];
	int nK=nK0;
	if(dRand1>=(1.-dTemp/2.)||dRand1<=dTemp/2.)
		nK=nK0;
	else
	{
		for(int i=0;i<m_nNumStates;i++)
		{
			if(i!=nK0)
			{
				dTemp=m_dTM1[nK0][i];
				if(dTemp>=dRand2)
				{
					nK=i;
					break;
				}else if(i==m_nNumStates-1)
				{
					nK=i;
				}
			}
		}
	}
	return nK;
}

void CDataSimulations::CalTMatrix()
{
	int i,j;
	double dTmep;
	double dT;
	double dpT; //The total probability of transition

	for(i=0;i<m_nNumStates;i++)
	{
		for(j=0;j<m_nNumStates;j++)
		{
			m_dTM[i][j]=0.;
			m_dTM1[i][j]=0.;
		}
	}

	for(i=0;i<m_nNumStates;i++)
	{
		dT=0.;//The sum of transition rate constants pointing away from the state i
		for(j=0;j<m_nNumStates;j++)
		{
			if(i!=j) dT+=m_dQM[i][j];
		}
		dpT=1.-exp(-dT*m_fDataSampleInterval/1000.);//The unit of m_fADCSampleInterval is millisecond while the unit of rate constant is per second.

		double dTemp=0.;
		double dTemp1=0.;
		for(j=0;j<m_nNumStates;j++)
		{
			if(i!=j) 
			{
				m_dTM[i][j]=dTemp+dpT*m_dQM[i][j]/dT;
				m_dTM1[i][j]=dTemp1+m_dQM[i][j]/dT;
				dTemp=m_dTM[i][j];
				dTemp1=m_dTM1[i][j];
			}else
			{
				m_dTM[i][j]=dTemp+1.-dpT;
				dTemp=m_dTM[i][j];
				m_dTM1[i][j]=1.-dpT;
			}
		}
	}
}

float CDataSimulations::GetData(int nK, int nPosition)
{
	return m_vstructStates[nK].fAmp+m_vstructStates[nK].fDev*NR::gasdev(m_nNoiseSeed);
}

void CDataSimulations::GetNoise()
{
	int nSeed=-1;
	m_pfNoise=new float[m_nDataSize];
	double dTemp=NR::gasdev(nSeed);
	for(int i=0;i<m_nDataSize;i++)
	{
		m_pfNoise[i]=NR::gasdev(nSeed);
	}
}


void CDataSimulations::GetParameters(CString &sEvlFileName)
{
	FILE *fp;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sInputFileName;

	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Data Simulation Input Files (*.Dsm)\0*.Dsm\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Read an input file for dwell fitting curve construction";
	float fRecordLength=0.f;

	m_vstructStates.clear();
	CString sTemp;
	long nDataSize;
	float fDataSampleInerval;
	int j;
	float fTemp;
	int i,nNumStates,nLevel;
	int nMaxLevel=-1;

	if(aFileDlg.DoModal()==IDOK)
	{
		sInputFileName=aFileDlg.GetPathName();
		sEvlFileName=ChangeFileExt(sInputFileName,"Evl");
		if(fp=fopen(sInputFileName,"r"))
		{
			fscanf(fp,"%s%d%s%d%s%d%s%f",sTemp, &nNumStates,sTemp,&nDataSize,sTemp,&m_nIterations,sTemp,&fDataSampleInerval);
			m_nDataSize=nDataSize;
			m_nNumStates=nNumStates;
			m_fDataSampleInterval=fDataSampleInerval;
			fscanf(fp,"%s",sTemp);
			for(i=0;i<nNumStates;i++)
			{
				structKineticStateNode aNode;
				fscanf(fp,"%s%d%s%f%s%f",sTemp,&nLevel,sTemp,&aNode.fAmp,sTemp,&aNode.fDev);
				aNode.nLevel=nLevel;
				if(nLevel>nMaxLevel)
					nMaxLevel=nLevel;
				m_vstructStates.push_back(aNode);
			}
			fscanf(fp,"%s",sTemp);
			for(i=0;i<nNumStates;i++)
			{
				fscanf(fp,"%s",sTemp);
				for(j=0;j<nNumStates;j++)
				{
					fscanf(fp,"%f",&fTemp);
					m_dQM[i][j]=fTemp;
				}
			}
		}
	}
	m_nNumLevels=nMaxLevel+1;
}

structEventListNode CDataSimulations::GetEventList()
{
	return m_vstructEventList;
}

vector <structStateSummaryNode> CDataSimulations::GetStateSummary()
{
	return m_vstructStateSummary;
}

void CDataSimulations::OutputStateSummary(CString sFileName)
{
	int i,j;
	FILE* fp=fopen(sFileName,"w");
	fprintf(fp,"  State:  Level:       RecordL        LevelL      Events         MeanL          Prob       Amp       Dev\n");    
	for(i=0;i<m_nNumStates;i++)
	{
		fprintf(fp,"%8d%8d%14.2f%14.2f%12d%14.4f%14.6f%10.2f%10.3f\n",m_vstructStateSummary[i].nState,m_vstructStateSummary[i].nLevel,m_vstructStateSummary[i].dRecordLength,
													m_vstructStateSummary[i].dTLength,m_vstructStateSummary[i].nNumEvents,m_vstructStateSummary[i].dMeanLength,
													m_vstructStateSummary[i].dTLength/m_vstructStateSummary[i].dRecordLength,
													m_vstructStateSummary[i].fAmp, m_vstructStateSummary[i].fDev);
	}

	fprintf(fp,"Mean Burst Durations:  %14.2f\n",(m_vstructStateSummary[1].dTLength+m_vstructStateSummary[2].dTLength)/m_vstructStateSummary[0].nNumEvents);
	fprintf(fp,"NOPB:  %d\n",m_vstructStateSummary[1].nNumEvents/m_vstructStateSummary[0].nNumEvents);
	fprintf(fp,"ToT:   %14.2f\n",m_vstructStateSummary[1].dTLength/m_vstructStateSummary[0].nNumEvents);

	fprintf(fp,"\n\n\n");


	fprintf(fp,"NumStates: %2d    DataSize:  %d    Iterations:  %4d    SampleInterval:  %12.4f\n",m_nNumStates,m_nDataSize,m_nIterations,m_fDataSampleInterval);

	fprintf(fp,"\n\n");
	fprintf(fp,"Q Matrix\n\n");
	for(i=0;i<m_nNumStates;i++)
	{
		fprintf(fp,"Line%2d     ",i);
		for(j=0;j<m_nNumStates;j++)
		{
			fprintf(fp,"%15.6f",m_dQM[i][j]);
		}
		fprintf(fp,"\n");
	}

	
	fprintf(fp,"\n\n");
	fprintf(fp,"T Matrix\n\n");
	for(i=0;i<m_nNumStates;i++)
	{
		fprintf(fp,"Line%2d     ",i);
		for(j=0;j<m_nNumStates;j++)
		{
			fprintf(fp,"%15.6f",m_dTM1[i][j]);
		}
		fprintf(fp,"\n");
	}

	fclose(fp);
}


CString CDataSimulations::ChangeFileExt(CString sFileName, CString sExt)
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


void CDataSimulations::OutputStateSummary(FILE* fp, int nIteration, int nSeed)
{
	int i,j;
	fprintf(fp,"  State:  Level:       RecordL        LevelL      Events         MeanL          Prob       Amp       Dev\n");    
	for(i=0;i<m_nNumStates;i++)
	{
		fprintf(fp,"%8d%8d%14.2f%14.2f%12d%14.4f%14.6f%10.2f%10.3f\n",m_vstructStateSummary[i].nState,m_vstructStateSummary[i].nLevel,m_vstructStateSummary[i].dRecordLength,
													m_vstructStateSummary[i].dTLength,m_vstructStateSummary[i].nNumEvents,m_vstructStateSummary[i].dMeanLength,
													m_vstructStateSummary[i].dTLength/m_vstructStateSummary[i].dRecordLength,
													m_vstructStateSummary[i].fAmp, m_vstructStateSummary[i].fDev);
	}

	fprintf(fp,"Mean Burst Durations:  %14.2f\n",(m_vstructStateSummary[1].dTLength+m_vstructStateSummary[2].dTLength)/m_vstructStateSummary[0].nNumEvents);
	if(m_vstructStateSummary[0].nNumEvents>0) fprintf(fp,"NOPB:  %d\n",m_vstructStateSummary[1].nNumEvents/m_vstructStateSummary[0].nNumEvents);
	if(m_vstructStateSummary[0].nNumEvents>0) fprintf(fp,"ToT:   %14.2f\n",m_vstructStateSummary[1].dTLength/m_vstructStateSummary[0].nNumEvents);

	fprintf(fp,"\n\n\n");


	fprintf(fp,"NumStates: %2d    DataSize:  %d    Iterations:  %4d    SampleInterval:  %12.4f    nSeed:    %d\n",m_nNumStates,m_nDataSize,nIteration,m_fDataSampleInterval,nSeed);

	if(nIteration==0)
	{
		fprintf(fp,"\n\n");
		fprintf(fp,"Q Matrix\n\n");
		for(i=0;i<m_nNumStates;i++)
		{
			fprintf(fp,"Line%2d     ",i);
			for(j=0;j<m_nNumStates;j++)
			{
				fprintf(fp,"%15.6f",m_dQM[i][j]);
			}
			fprintf(fp,"\n");
		}

		
		fprintf(fp,"\n\n");
		fprintf(fp,"T Matrix\n\n");
		for(i=0;i<m_nNumStates;i++)
		{
			fprintf(fp,"Line%2d     ",i);
			for(j=0;j<m_nNumStates;j++)
			{
				fprintf(fp,"%15.6f",m_dTM1[i][j]);
			}
			fprintf(fp,"\n");
		}
		fprintf(fp,"\n\n\n");
	}
}


void CDataSimulations::GetSimulationParameters_Q()
{
	int i,j,nSize;
	CQMatrixFitting aNode;
	m_sSimulationParameters=aNode.GetDataSimulationParameters();	
	m_nDataSize=m_sSimulationParameters.fLength/m_sSimulationParameters.fTimePerPoint;
	m_nNumStates=m_sSimulationParameters.nNumStates;
	m_fDataSampleInterval=m_sSimulationParameters.fTimePerPoint;
	m_nNumLevels=m_sSimulationParameters.nNumLevels;

	for(i=0;i<m_nNumStates;i++)
	{
		structKineticStateNode aNode;
		m_vstructStates.push_back(aNode);
	}

	int nState;
	for(i=0;i<m_nNumLevels;i++)
	{
		nSize=m_sSimulationParameters.vvnStatesInLevels[i].size();
		for(j=0;j<nSize;j++)
		{
			nState=m_sSimulationParameters.vvnStatesInLevels[i].at(j);
			m_vstructStates[nState-1].nLevel=i;
			m_vstructStates[nState-1].fAmp=m_sSimulationParameters.vsLevelProperties[i].fAmp;
			m_vstructStates[nState-1].fDev=m_sSimulationParameters.vsLevelProperties[i].fDev;
		}
	}

	for(i=0;i<m_nNumStates;i++)
	{
		for(j=0;j<m_nNumStates;j++)
		{
			m_dQM[i][j]=m_sSimulationParameters.vvdQMatrix[i][j];
		}
	}
}
