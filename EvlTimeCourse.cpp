// EvlTimeCourse.cpp: implementation of the CEvlTimeCourse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlTimeCourse.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvlTimeCourse::CEvlTimeCourse()
{
}

CEvlTimeCourse::~CEvlTimeCourse()
{
}

CEvlTimeCourse::CEvlTimeCourse(CString sFileName, float fBinSize, float fTimePerPoint, int nNumEvent, int *pnLevelLength, 
							   int *pnLevelStart, short *pstEventCurrentLevel, float* pfEffLevel, int* pnMRGEvents)
{
	m_fBinSize=fBinSize;
	m_fTimePerPoint=fTimePerPoint/1000.f;
	m_nNumEvent=nNumEvent;
	m_pnLevelLength=pnLevelLength;
	m_pstEventCurrentLevel=pstEventCurrentLevel;
	m_pnLevelStart=pnLevelStart;
	m_pnMRGEvents=pnMRGEvents;
	m_pfEffLevel=pfEffLevel;

	m_pfRealNumOpenings=NULL;
	m_pfEffTotalOpenTime=NULL;


	m_pfNumOpenings=NULL;
	m_pfTotalOpenTime=NULL;
	m_pfElapsedTime=NULL;
	m_pnMaximumOpenLevel=NULL;
	m_pnLastEventofBin=NULL;
	m_pnBinStart=NULL;
	m_pfBinConductance=NULL;
	m_sEvlFileName=sFileName;
	PreparePointers();
	PickClosedEvents();
}

void CEvlTimeCourse::DoEvlTimeCourse()
{
	int nCurrentBin=0;
	int nCurrentEvent=0;
	int nCurrentPoint=0;
	int nNumOpening=0;
	int nStartPointOfBin=0;
	int nNewOpenedLevel=0;
	int nMaxLevel=0;
	int nCurrentClosedPoint=0;
	int nPreviousClosedPoint=0;
	short stCurrentLevel=0;
	short stPreviousLevel=0;
	float fElapsedTime=0.;
	float fTotalOpenTime=0.0f;
	int nPreviousBin=-1;
	int nTemp=0;
	int i;
	
	for (i=0; i<m_nNumBin; i++)
	{
		*(m_pfTotalOpenTime+i)=0.f;
		*(m_pfEffTotalOpenTime+i)=0.f;
		*(m_pfElapsedTime+i)=0.f;
		*(m_pfNumOpenings+i)=0.f;
		*(m_pfRealNumOpenings+i)=0.f;
		*(m_pnMaximumOpenLevel+i)=-100;
	}

	nPreviousClosedPoint=*(m_pnClosedEvents);
	if(nPreviousClosedPoint>0)
	{
		nPreviousClosedPoint=-1;
	}
	for (i=0; i<m_nClosedEvents; i++)
	{
		nCurrentClosedPoint=*(m_pnClosedEvents+i); // In the unit of events
//		nCurrentPoint=*(m_pnLevelStart+nPreviousClosedPoint); // In the unit of sample points

		if(m_fBinSize<0)
		{
			nCurrentBin=nCurrentClosedPoint/int(-1*m_fBinSize+0.5);
			if(nCurrentBin>nPreviousBin)
			{
				nTemp=*(m_pnLevelStart+nCurrentClosedPoint);
				*(m_pnBinStart+nCurrentBin)=nTemp;
			}
		}

		stPreviousLevel=0;

		for(int j=nPreviousClosedPoint;j<nCurrentClosedPoint;j++)
		{
			nCurrentPoint=*(m_pnLevelStart+j);
			if(m_fBinSize>0)
			{
				nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
			}

			stCurrentLevel=*(m_pstEventCurrentLevel+j);
			*(m_pfTotalOpenTime+nCurrentBin)+=float(stCurrentLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
			*(m_pfElapsedTime+nCurrentBin)+=float(*(m_pnLevelLength+j))*m_fTimePerPoint;
		

			nNewOpenedLevel=stCurrentLevel-stPreviousLevel;
			if(stCurrentLevel>*(m_pnMaximumOpenLevel+nCurrentBin))
			{
				*(m_pnMaximumOpenLevel+nCurrentBin)=stCurrentLevel;
			}
			if(nNewOpenedLevel>0)
			{
				*(m_pfNumOpenings+nCurrentBin)+=float(nNewOpenedLevel);
			}
			stPreviousLevel=stCurrentLevel;
		}
		nPreviousClosedPoint=nCurrentClosedPoint;
		nPreviousBin=nCurrentBin;
	}

//	nCurrentClosedPoint=*(m_pnClosedEvents+i);
	nCurrentPoint=*(m_pnLevelStart+nPreviousClosedPoint);
	nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
	stPreviousLevel=0;
	if(m_fBinSize<0)
	{
		nCurrentBin=i/int(-1*m_fBinSize+0.5);
		if(nCurrentBin>nPreviousBin)
		{
			*(m_pnBinStart+nCurrentBin)=*(m_pnLevelStart+nCurrentClosedPoint);
		}
	}

	for(int j=nPreviousClosedPoint;j<m_nNumEvent;j++)
	{
//		nCurrentPoint=*(m_pnLevelStart+j);
		if(m_fBinSize>0)
		{
			nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
			stCurrentLevel=*(m_pstEventCurrentLevel+j);
		}

		stCurrentLevel=*(m_pstEventCurrentLevel+i);

		*(m_pfTotalOpenTime+nCurrentBin)+=float(stCurrentLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
		*(m_pfElapsedTime+nCurrentBin)+=float(*(m_pnLevelLength+j))*m_fTimePerPoint;
		
		nNewOpenedLevel=stCurrentLevel-stPreviousLevel;
		if(stCurrentLevel>*(m_pnMaximumOpenLevel+nCurrentBin))
		{
			*(m_pnMaximumOpenLevel+nCurrentBin)=stCurrentLevel;
		}
		if(nNewOpenedLevel>0)
		{
			*(m_pfNumOpenings+nCurrentBin)+=float(nNewOpenedLevel);
		}
		stPreviousLevel=stCurrentLevel;
	}
	nPreviousClosedPoint=nCurrentClosedPoint;
	OutputTimeCourse();
}


void CEvlTimeCourse::OutputTimeCourse()
{
	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sAbfFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Export the timecourse file (*.Tcr)";
	CString sFileName=ChangeFileExt(m_sEvlFileName,"Tcr");
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(fpAbfOut=fopen(sAbfFileName,"w"))
		{
		    WriteTimeCourse(fpAbfOut);
		    fclose(fpAbfOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}	

}

void CEvlTimeCourse::OutputBurstingTimeCourse()
{
	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sAbfFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Export the timecourse file (*.Tcr)";
	CString sFileName=ChangeFileExt(m_sEvlFileName,"Tcr");
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(fpAbfOut=fopen(sAbfFileName,"w"))
		{
		    WriteBurstingTimeCourse(fpAbfOut);
		    fclose(fpAbfOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}	
}

void CEvlTimeCourse::PreparePointers()
{
	delete m_pfNumOpenings;
	delete m_pfTotalOpenTime;
	delete m_pfRealNumOpenings;
	delete m_pfEffTotalOpenTime;
	delete m_pfElapsedTime;
	delete m_pnMaximumOpenLevel;
	delete m_pnLastEventofBin;
	delete m_pnBinStart;
	delete m_pfBinConductance;


	if(m_fBinSize>0.f)
	{
		m_fBinSize=m_fBinSize*1000.f;
		m_nNumBin=int(((*(m_pnLevelStart+m_nNumEvent-1)+
			*(m_pnLevelLength+m_nNumEvent-1))*m_fTimePerPoint)/m_fBinSize)+1;

		if(m_nNumBin>m_nNumEvent)
		{
			m_nNumBin=m_nNumEvent;
		}
	} else
	{
		m_fBinSize=-m_fBinSize;
		m_nNumBin=m_nNumEvent/int(m_fBinSize);

		if(m_nNumBin*int(m_fBinSize)<m_nNumEvent)
		{
			m_nNumBin+=1;
		}

		if(m_nNumBin>m_nNumEvent)
		{
			m_nNumBin=m_nNumEvent;
		}
		m_fBinSize=-m_fBinSize;
	}

	m_pfRealNumOpenings=new float[m_nNumBin];
	m_pfNumOpenings=new float[m_nNumBin];
	m_pfTotalOpenTime=new float[m_nNumBin];
	m_pfEffTotalOpenTime=new float[m_nNumBin];
	m_pfElapsedTime=new float[m_nNumBin];
	m_pfBinConductance=new float[m_nNumBin];
	m_pnMaximumOpenLevel=new int[m_nNumBin];
	m_pnLastEventofBin=new int[m_nNumBin];
	m_pnClosedEvents=new int[m_nNumEvent];
	m_pnBinStart=new int[m_nNumEvent];
}

void CEvlTimeCourse::WriteTimeCourse(FILE *fpOut)
{
	float fTotalOpenTime;
	float fNumOpening;
	float fElapsedTime;
	float fTime;
	fprintf(fpOut,"          Time      NPo       MTo           MFo    MaxLevel\n");

	for(int i=0; i<m_nNumBin; i++)
	{
		fElapsedTime=*(m_pfElapsedTime+i);
		fTime=m_fBinSize*(float(i)+0.5f)/1000.f;
		if(m_fBinSize<0)
		{
			fTime=*(m_pnBinStart+i)*m_fTimePerPoint;
		}
		fNumOpening=*(m_pfNumOpenings+i);
		fTotalOpenTime=*(m_pfTotalOpenTime+i);
		if(fNumOpening<0.2)
		{
//			fprintf(fpOut,"  %12.4f ",fTime);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",1000.f*fNumOpening/fElapsedTime);
//			fprintf(fpOut,"  %2d \n",*(m_pnMaximumOpenLevel+i));
		}
		else
		{
			fprintf(fpOut,"  %12.4f",fTime);
			fprintf(fpOut,"  %7.4f",fTotalOpenTime/fElapsedTime);
			fprintf(fpOut,"  %8.2f",fTotalOpenTime/fNumOpening);
			fprintf(fpOut,"  %12.4f",1000.f*fNumOpening/fElapsedTime);
			fprintf(fpOut,"          %2d\n",*(m_pnMaximumOpenLevel+i));
		}
	}
}


void CEvlTimeCourse::WriteBurstingTimeCourse(FILE *fpOut)
{
	float fTotalOpenTime,fEffTotalOpenTime;
	float fNumOpening,fRealNumOpening;
	float fElapsedTime;
	float fTime;
	bool bSmoothing=true;
	int nWindowSize=5;
	fprintf(fpOut,"          Time     MPoib         TToib      MBTo         MFoib          Tcib     IBIterval          MOib           MTo           MPo           MFo     MaxLevel\n");

	float *pfNumOpenings=new float[m_nNumBin];
	float *pfRealNumOpenings=new float[m_nNumBin];
	float *pfTotalOpenTime=new float[m_nNumBin];
	float *pfEffTotalOpenTime=new float[m_nNumBin];
	float *pfBinSize=new float[m_nNumBin];
	float *pfElapsedTime=new float[m_nNumBin];
	int nI,nF,j,i;


	if (bSmoothing)
	{
		for(i=0; i<m_nNumBin; i++)
		{
			nI=i-nWindowSize;
			if(nI<0)
				nI=0;
			nF=i+nWindowSize;
			if(nF>m_nNumBin-1)
				nF=m_nNumBin-1;
			pfElapsedTime[i]=0.f;
			pfNumOpenings[i]=0.f;
			pfRealNumOpenings[i]=0.f;
			pfTotalOpenTime[i]=0.f;
			pfEffTotalOpenTime[i]=0.f;
			for(j=nI;j<=nF;j++)
			{
				pfElapsedTime[i]+=*(m_pfElapsedTime+i);
				pfNumOpenings[i]+=*(m_pfNumOpenings+i);
				pfRealNumOpenings[i]+=*(m_pfRealNumOpenings+i);
				pfTotalOpenTime[i]+=*(m_pfTotalOpenTime+i);
				pfEffTotalOpenTime[i]+=*(m_pfEffTotalOpenTime+i);
			}
			pfBinSize[i]=nF-nI+1;
		}

		for(i=0; i<m_nNumBin; i++)
		{
			m_pfElapsedTime[i]=pfElapsedTime[i];
			m_pfNumOpenings[i]=pfNumOpenings[i];
			m_pfRealNumOpenings[i]=pfRealNumOpenings[i];
			m_pfTotalOpenTime[i]=pfTotalOpenTime[i];
			m_pfEffTotalOpenTime[i]=pfEffTotalOpenTime[i];
		}
	}

	for(i=0; i<m_nNumBin; i++)
	{
		fElapsedTime=*(m_pfElapsedTime+i);
		fTime=m_fBinSize*(float(i)+0.5f)/1000.f;
		if(m_fBinSize<0)
		{
			fTime=*(m_pnBinStart+i)*m_fTimePerPoint;
		}
		fNumOpening=*(m_pfNumOpenings+i);
		fRealNumOpening=*(m_pfRealNumOpenings+i);
		fTotalOpenTime=*(m_pfTotalOpenTime+i);
		fEffTotalOpenTime=*(m_pfEffTotalOpenTime+i);

		if(fNumOpening<0.2)
		{
//			fprintf(fpOut,"  %12.4f ",fTime);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",1000.f*fNumOpening/fElapsedTime);
//			fprintf(fpOut,"  %2d \n",*(m_pnMaximumOpenLevel+i));
		}
		else
		{
			fprintf(fpOut,"  %12.4f",fTime);//Time
			fprintf(fpOut,"  %8.5f",fEffTotalOpenTime/fTotalOpenTime);//Mpoib
			fprintf(fpOut,"  %12.2f",fEffTotalOpenTime);//TToib = Total open time in a burst
			fprintf(fpOut,"  %8.2f",fTotalOpenTime/fNumOpening);//MBTo
			fprintf(fpOut,"  %12.4f",1000.f*(fRealNumOpening-1.)/fTotalOpenTime);//MFoib
			fprintf(fpOut,"  %12.4f",(fTotalOpenTime-fEffTotalOpenTime)/fRealNumOpening);//Tcib
			fprintf(fpOut,"  %12.4f",(fElapsedTime-fTotalOpenTime)/fNumOpening);//IBInterval
			fprintf(fpOut,"  %12.2f",fRealNumOpening/fNumOpening);//MOib=Number of opening per burst
			fprintf(fpOut,"  %12.2f",fEffTotalOpenTime/fRealNumOpening);//MTo
			fprintf(fpOut,"  %12.2f",fEffTotalOpenTime/fElapsedTime);//MPo
			fprintf(fpOut,"  %12.2f",1000.*fRealNumOpening/fElapsedTime);//MFo
			fprintf(fpOut,"            %2d\n",*(m_pnMaximumOpenLevel+i));//Maximum open level
		}
	}
}

CString CEvlTimeCourse::ChangeFileExt(CString sFileName, CString sExt)
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

void CEvlTimeCourse::PickClosedEvents()
{
	m_nClosedEvents=0;
	short stTemp;
	for(int i=0;i<m_nNumEvent;i++)
	{
		stTemp=*(m_pstEventCurrentLevel+i);

		if(stTemp==0)
		{
			*(m_pnClosedEvents+m_nClosedEvents)=i;
			m_nClosedEvents++;
		}
	}
}


void CEvlTimeCourse::DoBurstingTimeCourse()
{
	int nCurrentBin=0;
	int nCurrentEvent=0;
	int nCurrentPoint=0;
	int nNumOpening=0;
	int nStartPointOfBin=0;
	int nNewOpenedLevel=0;
	int nMaxLevel=0;
	int nCurrentClosedPoint=0;
	int nPreviousClosedPoint=0;
	short stCurrentLevel=0;
	short stPreviousLevel=0;
	float fElapsedTime=0.;
	float fTotalOpenTime=0.0f;
	float fEffLevel;
	int nPreviousBin=-1;
	int nTemp=0;
	int nBinSize;
	float fDelimiter;
	int i;

	for (i=0; i<m_nNumBin; i++)
	{
		*(m_pfTotalOpenTime+i)=0.f;
		*(m_pfEffTotalOpenTime+i)=0.f;
		*(m_pfElapsedTime+i)=0.f;
		*(m_pfNumOpenings+i)=0.f;
		*(m_pfRealNumOpenings+i)=0.f;
		*(m_pnMaximumOpenLevel+i)=-100;
	}

	nPreviousClosedPoint=*(m_pnClosedEvents);
	int nMRGEvents=0;
	if(nPreviousClosedPoint>0)
	{
//		nPreviousClosedPoint=-1;
		nPreviousClosedPoint=0;//04d16
	}
	for (i=0; i<m_nClosedEvents; i++)
	{
		nCurrentClosedPoint=*(m_pnClosedEvents+i); // In the unit of events
	//	nCurrentPoint=*(m_pnLevelStart+nPreviousClosedPoint); // In the unit of sample points
		if(m_fBinSize<0)
		{
			nCurrentBin=i/int(-1*m_fBinSize+0.5);
			if(nCurrentBin>nPreviousBin)
			{
				nTemp=*(m_pnLevelStart+nCurrentClosedPoint);
				*(m_pnBinStart+nCurrentBin)=nTemp;
			}
		}

		stPreviousLevel=0;

		for(int j=nPreviousClosedPoint;j<nCurrentClosedPoint;j++)
		{
			nCurrentPoint=*(m_pnLevelStart+j);
			stCurrentLevel=*(m_pstEventCurrentLevel+j);
			fEffLevel=m_pfEffLevel[j];
			*(m_pfTotalOpenTime+nCurrentBin)+=float(stCurrentLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
			*(m_pfEffTotalOpenTime+nCurrentBin)+=float(fEffLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
			*(m_pfElapsedTime+nCurrentBin)+=float(*(m_pnLevelLength+j))*m_fTimePerPoint;
			

			nNewOpenedLevel=stCurrentLevel-stPreviousLevel;
			nMRGEvents=m_pnMRGEvents[j];
			if(stCurrentLevel>*(m_pnMaximumOpenLevel+nCurrentBin))
			{
				*(m_pnMaximumOpenLevel+nCurrentBin)=stCurrentLevel;
			}
			if(nNewOpenedLevel>0)
			{
				*(m_pfNumOpenings+nCurrentBin)+=float(nNewOpenedLevel);
				*(m_pfRealNumOpenings+nCurrentBin)+=float(nNewOpenedLevel)+float(nMRGEvents);
			}
			stPreviousLevel=stCurrentLevel;
		}
		nPreviousClosedPoint=nCurrentClosedPoint;
		nPreviousBin=nCurrentBin;
	}
//	nCurrentClosedPoint=*(m_pnClosedEvents+i);
	nCurrentPoint=*(m_pnLevelStart+nPreviousClosedPoint);
	nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
	stPreviousLevel=0;
	if(m_fBinSize<0)
	{
		nCurrentBin=i/int(-1*m_fBinSize+0.5);
		if(nCurrentBin>nPreviousBin)
		{
			*(m_pnBinStart+nCurrentBin)=*(m_pnLevelStart+nCurrentClosedPoint);
		}
	}

	for(int j=nPreviousClosedPoint;j<m_nNumEvent;j++)
	{
//		nCurrentPoint=*(m_pnLevelStart+j);
		if(m_fBinSize>0)
		{
			nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
			stCurrentLevel=*(m_pstEventCurrentLevel+j);
		}

		fEffLevel=m_pfEffLevel[j];
		stCurrentLevel=*(m_pstEventCurrentLevel+i);

		*(m_pfEffTotalOpenTime+nCurrentBin)+=float(fEffLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
		*(m_pfTotalOpenTime+nCurrentBin)+=float(stCurrentLevel**(m_pnLevelLength+j))*m_fTimePerPoint;
		*(m_pfElapsedTime+nCurrentBin)+=float(*(m_pnLevelLength+j))*m_fTimePerPoint;
			
		nNewOpenedLevel=stCurrentLevel-stPreviousLevel;
		if(stCurrentLevel>*(m_pnMaximumOpenLevel+nCurrentBin))
		{
			*(m_pnMaximumOpenLevel+nCurrentBin)=stCurrentLevel;
		}
		if(nNewOpenedLevel>0)
		{
			*(m_pfNumOpenings+nCurrentBin)+=float(nNewOpenedLevel);
			*(m_pfRealNumOpenings+nCurrentBin)+=float(nNewOpenedLevel)+float(m_pnMRGEvents[j]);
		}
		stPreviousLevel=stCurrentLevel;
	}
	nPreviousClosedPoint=nCurrentClosedPoint;
	OutputBurstingTimeCourse();
}
