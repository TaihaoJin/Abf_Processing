// EvlTimeCourse.cpp: implementation of the CEvlTimeCourse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "EvlTimeCourse0.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvlTimeCourse0::CEvlTimeCourse0()
{

}

CEvlTimeCourse0::~CEvlTimeCourse0()
{

}

CEvlTimeCourse0::CEvlTimeCourse0(CString sFileName, float fBinSize, float fTimePerPoint, int nNumEvent, int *pnLevelLength, 
							   int *pnLevelStart, short *pstEventCurrentLevel)
{
	m_sEvlFileName=sFileName;
	m_fBinSize=fBinSize*1000.f;
	m_fTimePerPoint=fTimePerPoint/1000.f;
	m_nNumEvent=nNumEvent;
	m_pnLevelLength=pnLevelLength;
	m_pstEventCurrentLevel=pstEventCurrentLevel;
	m_pnLevelStart=pnLevelStart;

	m_pfNumOpenings=NULL;
	m_pfTotalOpenTime=NULL;
	m_pfElapsedTime=NULL;
	m_pnMaximumOpenLevel=NULL;
	PreparePointers();
}

void CEvlTimeCourse0::DoEvlTimeCourse()
{
	//This function works under pressumtion that the bin size is much bigger that the open time.
	int nCurrentBin=0;
	int nCurrentEvent=0;
	int nCurrentPoint=0;
	int nNumOpening=0;
	int nStartPointOfBin=0;
	int nNewOpenedLevel=0;
	int nMaxLevel=0;
	short stCurrentLevel=0;
	short stPreviousLevel=0;
	float fElapsedTime=0.;
	float fTotalOpenTime=0.0f;
	int i;
	
	for (i=0; i<m_nNumBin; i++)
	{
		*(m_pfTotalOpenTime+i)=0.f;
		*(m_pfElapsedTime+i)=0.f;
		*(m_pfNumOpenings+i)=0.f;
		*(m_pnMaximumOpenLevel+i)=-100;
	}


	for (i=0; i<m_nNumEvent; i++)
	{
		nCurrentPoint=*(m_pnLevelStart+i);
		nCurrentBin=int(float(nCurrentPoint)*m_fTimePerPoint/m_fBinSize);
		stCurrentLevel=*(m_pstEventCurrentLevel+i);

		*(m_pfTotalOpenTime+nCurrentBin)+=float(stCurrentLevel**(m_pnLevelLength+i))*m_fTimePerPoint;
		*(m_pfElapsedTime+nCurrentBin)+=float(*(m_pnLevelLength+i))*m_fTimePerPoint;;
		

		nNewOpenedLevel=stCurrentLevel-stPreviousLevel;
		if(stCurrentLevel>*(m_pnMaximumOpenLevel+nCurrentBin))*(m_pnMaximumOpenLevel+nCurrentBin)=stCurrentLevel;
		if(nNewOpenedLevel>0)
		{
			*(m_pfNumOpenings+nCurrentBin)+=float(nNewOpenedLevel);
		}
		stPreviousLevel=stCurrentLevel;
	}
	OutputTimeCourse();
}


void CEvlTimeCourse0::OutputTimeCourse()
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

void CEvlTimeCourse0::PreparePointers()
{
	m_nNumBin=int(((*(m_pnLevelStart+m_nNumEvent-1)+
		*(m_pnLevelLength+m_nNumEvent-1))*m_fTimePerPoint)/m_fBinSize)+1;
	delete m_pfNumOpenings;
	delete m_pfTotalOpenTime;
	delete m_pfElapsedTime;
	delete m_pnMaximumOpenLevel;

	m_pfNumOpenings=new float [m_nNumBin];
	m_pfTotalOpenTime=new float [m_nNumBin];
	m_pfElapsedTime=new float [m_nNumBin];
	m_pnMaximumOpenLevel=new int [m_nNumBin];
}

CString CEvlTimeCourse0::ChangeFileExt(CString sFileName, CString sExt)
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

void CEvlTimeCourse0::WriteTimeCourse(FILE *fpOut)
{
	const float fZero=0.f;
	float fTotalOpenTime;
	float fNumOpening;
	float fElapsedTime;
	for(int i=0; i<m_nNumBin; i++)
	{
		fElapsedTime=*(m_pfElapsedTime+i);
		fNumOpening=*(m_pfNumOpenings+i);
		fTotalOpenTime=*(m_pfTotalOpenTime+i);

		fprintf(fpOut,"  %12.4f ",m_fBinSize*(float(i)+0.5)/1000.);
		fprintf(fpOut,"  %12.4f ",fTotalOpenTime/m_fBinSize);
		if(fNumOpening<0.01)
		{
			fprintf(fpOut,"  %12.4f ",fZero);
		}
		else
		{
			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/fNumOpening);
		}
		fprintf(fpOut,"  %12.4f ",1000.f*fNumOpening/m_fBinSize);
		fprintf(fpOut,"  %2d \n",*(m_pnMaximumOpenLevel+i));
	}
}
