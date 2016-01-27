// Evl.cpp: implementation of the CEvl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "Evl.h"
#include "EvlTimeCourse.h"
#include "EvlHistogramDlg.h"
#include "EvlTimeCourse0.h"
#include "Evl2DHistogram.h"
#include "BurstHandling.h"
#include "SubconductanceDlg.h"
#include "EvlDwellingTimeFitting.h"
#include "EvlCorrection.h"
#include "BurstingTimeCourseOptionDlg.h"
#include "IncidenceSubsetHandler.h"
#include "LinearHistogramDouble.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
int CEvl::m_nSN;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvl::CEvl()
{
	m_nWritingKey=0;
	m_nReadingKey=0;
	m_pnLevelStart=NULL;
	m_pfAmplitude=NULL;
	m_pnLevelLength=NULL;				 
	m_pfEventStandardDev=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pstEventNotes=NULL; 
	m_pfBaseline=NULL;
	m_pnBurstStartingEvent=NULL;
	m_pnBurstEndingEvent=NULL;
	m_pnBurstLength=NULL;
	m_nMinimumOpenning=2;
	m_fBurstDelimiter=3.f;
	m_pbMultipleOpenning=NULL;
	m_pnNumOpenningInBurst=NULL;
	m_pnMRGEvents=NULL;
	m_pfEffLevel=NULL;
	m_nNumEvents0=0;
	m_bAttachingEvl=false;
	m_nEvlLength0=0;
	m_bEvlHist=false;
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	m_bModeSeparated=false;

	m_pnLevelStart0=NULL;
	m_pfAmplitude0=NULL;
	m_pnLevelLength0=NULL;				 
	m_pfEventStandardDev0=NULL;
	m_pstEventCurrentLevel0=NULL;
	m_pstEventNotes0=NULL; 
	m_pfBaseline0=NULL;
	m_pnValidEvent=NULL;
	m_pbExcluded=NULL;
	m_fAmpThreshold=0.5f;
	m_pnValidEvent0=NULL;
	strcpy(m_cNumChannels, "Channels: 2");
	m_nChannelNumberPosition=-1;
	m_nMaxNumEvents=10000000;
	m_bExcludingLastClosure=false;
	m_nEvlID=-1;
	m_bAbfNode=false;
}

CEvl::~CEvl()
{	
	delete m_pnLevelStart;			 
	delete m_pfEventStandardDev;
	delete m_pfAmplitude;
	delete m_pnLevelLength;	
	delete m_pstEventCurrentLevel;
	delete m_pstEventNotes; 
	delete m_pfBaseline;
	delete m_pbMultipleOpenning;
	delete m_pfEffLevel;
	delete m_pnMRGEvents;
	delete m_pnLevelStart0;
	delete m_pfAmplitude0;
	delete m_pnLevelLength0;				 
	delete m_pfEventStandardDev0;
	delete m_pstEventCurrentLevel0;
	delete m_pstEventNotes0; 
	delete m_pfBaseline0;
	delete m_pnValidEvent0;
	delete m_pnBurstStartingEvent;
	delete m_pnNumOpenningInBurst;
	delete m_pnBurstEndingEvent;
	delete m_pnBurstLength;
	delete m_pnValidEvent;
	delete m_pbExcluded;
}

int CEvl::ReadEvl(CString sEvlFileName)
{
	int nStatus=1;
	if(!m_bAttachingEvl)
	{
		m_sEvlFileName=sEvlFileName;
	}

	FILE *fpEvlIn;		
	if(!(fpEvlIn=fopen(sEvlFileName,"rb")))
	{
		CString sMessage="Eorror occurred when reading an evl file";
		AfxMessageBox(sMessage,MB_OK,0);
		sMessage=sEvlFileName;
		AfxMessageBox(sMessage,MB_OK,0);
		nStatus=-1;
	}else{
		ReadEvl(fpEvlIn);
		fclose(fpEvlIn);
	}
	return nStatus;
}

void CEvl::ReadEvl(FILE *fpEvlIn)
{
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	m_bEvlHist=false;
	
	CountEventNum(fpEvlIn);

	PrepairPointers();
	fread(&m_FileId, sizeof(m_FileId)-1,1,fpEvlIn);
	fread(&m_FFversion, sizeof(m_FFversion),1, fpEvlIn);
	fread(&m_DataName, sizeof(m_DataName)-1,1,fpEvlIn);
	fread(&m_ADCUnit, sizeof(m_ADCUnit)-1,1,fpEvlIn);
	fread(&m_Comment, sizeof(m_Comment)-1,1,fpEvlIn);
	fread(&m_TimePerPoint,sizeof(m_TimePerPoint),1,fpEvlIn);
	fread(&m_InterpoloationFacto,sizeof(m_InterpoloationFacto),1,fpEvlIn);
	fread(&m_AnalysisFilter,sizeof(m_AnalysisFilter),1,fpEvlIn);
	fread(&m_EpisodeSize,sizeof(m_EpisodeSize),1,fpEvlIn);
	fread(&m_TriggerStart,sizeof(m_TriggerStart),1,fpEvlIn);
	fread(&m_Holding,sizeof(m_Holding),1,fpEvlIn);
	fread(&m_AcquisitionFilter,sizeof(m_AcquisitionFilter),1,fpEvlIn);
	fread(&m_Gaps,sizeof(m_Gaps),1,fpEvlIn);
	fread(&m_TriggerEnd,sizeof(m_TriggerEnd),1,fpEvlIn);
	fread(&m_NumLevels,sizeof(m_NumLevels),1,fpEvlIn);
	fread(&m_Reserved,sizeof(m_Reserved)-1,1,fpEvlIn);
	m_fRisetime=1000.*0.3396/m_AcquisitionFilter;//using ms as the unit. T10-90 rising time, Colquhoun and Sigworth 1994 p478

	m_nRecordLength_Actual=0;

	if(m_nNumEvents>m_nMaxNumEvents)
	{
		m_nNumEvents=m_nMaxNumEvents;
	}

	int nShift=0;
	float baseline=0.f;
	bool bfirst=true;
	for(int i=m_nNumEvents0;i<m_nNumEvents;i++)
	{
		fread(m_pnLevelStart+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfAmplitude+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pnLevelLength+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfEventStandardDev+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pstEventCurrentLevel+i+nShift, sizeof(short),1,fpEvlIn);
		fread(m_pstEventNotes+i+nShift,sizeof(short),1,fpEvlIn);
		m_nRecordLength_Actual+=m_pnLevelLength[i+nShift];
		if (*(m_pstEventCurrentLevel+i+nShift)==0) 
		{
			fread(m_pfBaseline+i+nShift,sizeof(float),1,fpEvlIn);
			baseline=m_pfBaseline[i+nShift];
			if(bfirst){
				for(int it=0;it<i;it++){
					m_pfBaseline[it+nShift]=baseline;
					bfirst=false;
				}
			}
		}else{
			m_pfBaseline[i+nShift]=baseline;
		}

		if(i==m_nNumEvents0)
		{
			if(m_pstEventCurrentLevel[0]==0&&m_bExcludingLastClosure)
			{
				nShift=-1;
				m_nRecordLength_Actual=0;//11/28/2007
			}
		}
	}

	if(m_bExcludingLastClosure)
		m_nNumEvents-=(1-nShift); //11/28/2007


	m_nRecordLength_Formal=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];

	if(m_bAttachingEvl) 
	{
		ShiftLevelStart();
		ResetAttachingStatus();
	}

	fclose(fpEvlIn);
	GetChannelNumbers();
	updateID();
}
void CEvl::ReadEvl(FILE* fpEvlIn, int nReadingKey, CString sEvlFileName)
{
	m_nReadingKey==nReadingKey;
	if(!m_bAttachingEvl)
	{
		m_sEvlFileName=sEvlFileName;
	}
	ReadEvl(fpEvlIn);
}
/*
void CEvl::ReadEvl(FILE* fpEvlIn, int nReadingKey, CString sEvlFileName)
{

	m_nReadingKey=nReadingKey;
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	m_bEvlHist=false;
	if(!m_bAttachingEvl)
	{
		m_sEvlFileName=sEvlFileName;
	}	

	PrepairPointers();
	fread(&m_FileId, sizeof(m_FileId)-1,1,fpEvlIn);
	fread(&m_FFversion, sizeof(m_FFversion),1, fpEvlIn);
	fread(&m_DataName, sizeof(m_DataName)-1,1,fpEvlIn);
	fread(&m_ADCUnit, sizeof(m_ADCUnit)-1,1,fpEvlIn);
	fread(&m_Comment, sizeof(m_Comment)-1,1,fpEvlIn);
	fread(&m_TimePerPoint,sizeof(m_TimePerPoint),1,fpEvlIn);
	fread(&m_InterpoloationFacto,sizeof(m_InterpoloationFacto),1,fpEvlIn);
	fread(&m_AnalysisFilter,sizeof(m_AnalysisFilter),1,fpEvlIn);
	fread(&m_EpisodeSize,sizeof(m_EpisodeSize),1,fpEvlIn);
	fread(&m_TriggerStart,sizeof(m_TriggerStart),1,fpEvlIn);
	fread(&m_Holding,sizeof(m_Holding),1,fpEvlIn);
	fread(&m_AcquisitionFilter,sizeof(m_AcquisitionFilter),1,fpEvlIn);
	fread(&m_Gaps,sizeof(m_Gaps),1,fpEvlIn);
	fread(&m_TriggerEnd,sizeof(m_TriggerEnd),1,fpEvlIn);
	fread(&m_NumLevels,sizeof(m_NumLevels),1,fpEvlIn);
	fread(&m_Reserved,sizeof(m_Reserved)-1,1,fpEvlIn);
	if(m_nReadingKey==1)
		fread(&m_nNumEvents, sizeof(int),1,fpEvlIn);

	m_nRecordLength_Actual=0;


//	for(int i=0;i<m_nNumEvents;i++)
//	{
//		fread(m_pnLevelStart+i, sizeof(int),1,fpEvlIn);
//		fread(m_pfAmplitude+i, sizeof(float),1,fpEvlIn);
//		fread(m_pnLevelLength+i, sizeof(int),1,fpEvlIn);
//		fread(m_pfEventStandardDev+i, sizeof(float),1,fpEvlIn);
//		fread(m_pstEventCurrentLevel+i, sizeof(short),1,fpEvlIn);
//		fread(m_pstEventNotes+i,sizeof(short),1,fpEvlIn);
//		m_nRecordLength_Actual+=m_pnLevelLength[i];
//		if (*(m_pstEventCurrentLevel+i)==0) 
//		{
//			fread(m_pfBaseline+i,sizeof(float),1,fpEvlIn);
//		}
//		if(m_nReadingKey==1)
//			fread(m_pbExcluded+i,sizeof(bool),1,fpEvlIn);		
//	}

	int nShift=0;
	float baseline=0.f;
	bool bfirst=true;
	for(int i=0;i<m_nNumEvents-1;i++)
	{
		fread(m_pnLevelStart+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfAmplitude+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pnLevelLength+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfEventStandardDev+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pstEventCurrentLevel+i+nShift, sizeof(short),1,fpEvlIn);
		fread(m_pstEventNotes+i+nShift,sizeof(short),1,fpEvlIn);
		m_nRecordLength_Actual+=m_pnLevelLength[i+nShift];
		if (*(m_pstEventCurrentLevel+i+nShift)==0) 
		{
			fread(m_pfBaseline+i+nShift,sizeof(float),1,fpEvlIn);
		}
		if(i==m_nNumEvents0)
		{
			if(m_pstEventCurrentLevel[0]==0&&m_bExcludingLastClosure)
			{
				nShift=-1;
				m_nRecordLength_Actual=0;//11/28/2007
			baseline=m_pfBaseline[i+nShift];
			if(bfirst){
				for(int it=0;it<i;it++){
					m_pfBaseline[it+nShift]=baseline;
					bfirst=false;
				}
			}
		}else{
			m_pfBaseline[i+nShift]=baseline;
		}

		}
	}

	if(m_bExcludingLastClosure)
		m_nNumEvents-=(1-nShift); //11/28/2007


	m_nRecordLength_Formal=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];

	fclose(fpEvlIn);
	GetChannelNumbers();
}
*/
void CEvl::CountEventNum(FILE * fpEvlIn)
{
	fread(&m_FileId, sizeof(m_FileId)-1,1,fpEvlIn);
	fread(&m_FFversion, sizeof(m_FFversion),1, fpEvlIn);
	fread(&m_DataName, sizeof(m_DataName)-1,1,fpEvlIn);
	fread(&m_ADCUnit, sizeof(m_ADCUnit)-1,1,fpEvlIn);
	fread(&m_Comment, sizeof(m_Comment)-1,1,fpEvlIn);
	fread(&m_TimePerPoint,sizeof(m_TimePerPoint),1,fpEvlIn);
	fread(&m_InterpoloationFacto,sizeof(m_InterpoloationFacto),1,fpEvlIn);
	fread(&m_AnalysisFilter,sizeof(m_AnalysisFilter),1,fpEvlIn);
	fread(&m_EpisodeSize,sizeof(m_EpisodeSize),1,fpEvlIn);
	fread(&m_TriggerStart,sizeof(m_TriggerStart),1,fpEvlIn);
	fread(&m_Holding,sizeof(m_Holding),1,fpEvlIn);
	fread(&m_AcquisitionFilter,sizeof(m_AcquisitionFilter),1,fpEvlIn);
	fread(&m_Gaps,sizeof(m_Gaps),1,fpEvlIn);
	fread(&m_TriggerEnd,sizeof(m_TriggerEnd),1,fpEvlIn);
	fread(&m_NumLevels,sizeof(m_NumLevels),1,fpEvlIn);
	fread(&m_Reserved,sizeof(m_Reserved)-1,1,fpEvlIn);

	int Myint;
	float Myfloat;
	short Myshort1,Myshort2;
	int nNumEvents=0;
	m_nMaxLevel=-1;

	while(!feof(fpEvlIn)&&nNumEvents<=m_nMaxNumEvents)
	{
		nNumEvents++;
		fread(&Myint, sizeof(int),1,fpEvlIn);
		fread(&Myfloat, sizeof(float),1,fpEvlIn);
		fread(&Myint, sizeof(int),1,fpEvlIn);
		fread(&Myfloat, sizeof(float),1,fpEvlIn);
		fread(&Myshort1, sizeof(short),1,fpEvlIn);
		if(Myshort1>m_nMaxLevel) m_nMaxLevel=Myshort1;
		fread(&Myshort2,sizeof(short),1,fpEvlIn);
		if (Myshort1==0) 
		{
			fread(&Myfloat,sizeof(float),1,fpEvlIn);
		}
	}

	m_nNumEvents=m_nNumEvents0+nNumEvents-1;

	rewind(fpEvlIn);
}

void CEvl::WriteEvl(CString sEvlFileName)
{
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	FILE* fpEvlOut;
	if(fpEvlOut=fopen(sEvlFileName,"wb"))
	{

	    WriteEvlHead(fpEvlOut);
//		    FILE* fp=fopen("c:\\axon\\data\\btest.evl","w");
//		    FILE* fp1=fopen("c:\\axon\\data\\btestb.evl","w");
//		    FILE* fp2=fopen("c:\\axon\\data\\btestn.evl","w");
	
//		ExcludeLowAmp();
	    for(int i=0;i<m_nNumEvents;i++)
		{
//			if(!*(m_pbExcluded+i))
			{
				WriteEvlBody(fpEvlOut,i);
//				fprintf(fp,"%8d  %6d %2d %2d,%f\n ",*(m_pnLevelStart+i),*(m_pnLevelLength+i),
//			    *(m_pstEventCurrentLevel+i),*(m_pstEventNotes+i),*(m_pfAmplitude+i));
			}
		}


	
	    fclose(fpEvlOut);
//	    fclose(fp);
	}
	else
	{
			AfxMessageBox(sMessage,MB_OK,0);
	}
}

void CEvl::WriteEvl(FILE* fpEvlOut, int nWritingKey)
{
	m_nWritingKey=nWritingKey;

	WriteEvlHead(fpEvlOut);	
	if(m_nWritingKey==1)
	{
		fwrite(&m_nNumEvents, sizeof(int),1,fpEvlOut);
	}
	for(int i=0;i<m_nNumEvents;i++)
	{
		WriteEvlBody(fpEvlOut,i);
		if(m_nWritingKey==1)
		{
			fwrite(m_pbExcluded+i, sizeof(bool),1,fpEvlOut);
		}
	}
}


void CEvl::PrepairPointers()
{
	if(m_bAttachingEvl)
	{
		PreservePointers();
	}

	delete m_pfAmplitude;
	delete m_pnLevelLength;				 
	delete m_pfEventStandardDev;
	delete m_pstEventCurrentLevel;
	delete m_pstEventNotes; 
	delete m_pfBaseline;
	delete m_pnValidEvent;
	delete m_pnLevelStart;

	delete m_pnBurstStartingEvent;
	delete m_pnBurstEndingEvent;
	delete m_pnBurstLength;
	delete m_pbMultipleOpenning;
	delete m_pnNumOpenningInBurst;
	delete m_pbExcluded;

	m_pnLevelStart=new int[m_nNumEvents];
	m_pfAmplitude=new float[m_nNumEvents];
	m_pnLevelLength=new int[m_nNumEvents];				 
	m_pfEventStandardDev=new float[m_nNumEvents];
	m_pstEventCurrentLevel=new short[m_nNumEvents];
	m_pstEventNotes=new short[m_nNumEvents]; 
	m_pfBaseline=new float[m_nNumEvents];
	m_pnBurstStartingEvent=new int[m_nNumEvents];
	m_pnBurstEndingEvent=new int[m_nNumEvents];
	m_pnBurstLength=new int[m_nNumEvents];
	m_pbMultipleOpenning=new bool[m_nNumEvents];
	m_pnNumOpenningInBurst=new int[m_nNumEvents];
	m_pnValidEvent=new int[m_nNumEvents];
	m_pbExcluded=new bool[m_nNumEvents];

	if(m_bAttachingEvl)
	{
		InsertOldEvl();
	}

	for(int i=0;i<m_nNumEvents;i++)
	{
		*(m_pbMultipleOpenning+i)=false;
		*(m_pbExcluded+i)=false;
	}
}

void CEvl::SetBurstDelimiter(float BurstDelimiter)
{
	m_fBurstDelimiter=BurstDelimiter;
}

void CEvl::SetMinimumOpenning(int MinimumOpenning)
{
	m_nMinimumOpenning=MinimumOpenning;
}

void CEvl::CheckBursting()
{
	int nBurstDelimiter=int(m_fBurstDelimiter/(m_TimePerPoint/1000.f)+0.5);
	int nStart=0;
	int nEnd=0;
	bool bNewBurst=true;
	m_nNumBursts=0;
	for(int i=0;i<m_nNumEvents;i++)
	{
		if(bNewBurst)
		{
			if(*(m_pstEventCurrentLevel+i)>0)
			{
				*(m_pnBurstStartingEvent+m_nNumBursts)=i+1;
				nStart=i+1;
				bNewBurst=false;
			}
		}
		else
		{
			if(*(m_pstEventCurrentLevel+i)==0)
			{
				if(*(m_pnLevelLength+i)>nBurstDelimiter)
				{
					bNewBurst=true;
					nEnd=i;
					*(m_pnBurstEndingEvent+m_nNumBursts)=i;
					*(m_pnBurstLength+m_nNumBursts)=*(m_pnLevelStart+i)-
						*(m_pnLevelStart+*(m_pnBurstStartingEvent+m_nNumBursts)-1);
					*(m_pnNumOpenningInBurst+m_nNumBursts)=(nEnd-nStart)/2+1;
					m_nNumBursts++;
				}
			}
		}
	}

	if(*(m_pstEventCurrentLevel+m_nNumEvents-1)>0) 
	{
		*(m_pnBurstEndingEvent+m_nNumBursts)=m_nNumEvents;
		m_nNumBursts++;
	}
}




void CEvl::ModeSeparated()
{
	m_bModeSeparated=true;
}

CString CEvl::ChangeExt(CString sFileName, CString sNewExt)
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

CString CEvl::ExtendFileName(CString sFileName, char cInsert)
{
	int nLength=0;
	CString sNewName=sFileName;
	nLength=sFileName.GetLength();
	sNewName.Insert(nLength-4,cInsert);
	return sNewName;
}

CString CEvl::ExtendFileName(CString sFileName, CString sInsert)
{
	int i=0,nLength=0;
	char cChar;
	CString sNewName=sFileName;
	//the function Getlength() of CString makes mistake when a character '0' appears in a string. 
	//It counts the length of the left part of '0'.
	//Not really because of '0'.
	nLength=sNewName.GetLength();
	int nLength1=sInsert.GetLength();
	for(i=0;i<nLength1;i++)
	{
		cChar=sInsert.GetAt(nLength1-1-i);
		sNewName.Insert(nLength-4,cChar);
	}
	return sNewName;
}

CString CEvl::AttachToFileName(CString sFileName, char ch)
{
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	sNewFileName.Insert(nLength-4,ch);
	return sNewFileName;
}

void CEvl::TrimmingMulOpenning()
{
	int i;
	for(i=0;i<m_nNumEvents;i++)
	{
		if((*(m_pstEventCurrentLevel+i)>1))
		{
			int j=i;
			while((*(m_pstEventCurrentLevel+j)>0)&&(j>=0))
			{
				*(m_pbMultipleOpenning+j)=true;
				j--;
			}
			j=i+1;
			while((*(m_pstEventCurrentLevel+j)>0)&&j<m_nNumEvents)
			{
				*(m_pbMultipleOpenning+j)=true;
				j++;
			}
			i=j;
		}
	}

	int* pnLevelStart=new int[m_nNumEvents];
	int* pnLevelLength=new int[m_nNumEvents];
	short* pstCurrentLevel=new short[m_nNumEvents];

	int nNewIndex=0;
	int nNumTrimmedEvents=0;
	int nShifted=0;

	for(i=0;i<m_nNumEvents;i++)
	{
		if(*(m_pbMultipleOpenning+i))
		{
			nNumTrimmedEvents++;
			nShifted+=*(m_pnLevelLength+i);
		}
		else
		{
			*(pnLevelStart+nNewIndex)=*(m_pnLevelStart+i)-nShifted;
			*(pnLevelLength+nNewIndex)=*(m_pnLevelLength+i);
			*(pstCurrentLevel+nNewIndex)=*(m_pstEventCurrentLevel+i);
			nNewIndex++;
		}
	}

//	delete m_pnLevelStart;
//	delete m_pnLevelLength;
//	delete m_pstEventCurrentLevel;
		
	m_pnLevelStart=pnLevelStart;
	m_pnLevelLength=pnLevelLength;
	m_pstEventCurrentLevel=pstCurrentLevel;
	m_nNumEvents-=nNumTrimmedEvents;
}


void CEvl::WriteEvlHead(FILE *fpEvlOut)
{
	fwrite(&m_FileId, sizeof(m_FileId)-1,1,fpEvlOut);
	fwrite(&m_FFversion, sizeof(m_FFversion),1, fpEvlOut);
	fwrite(&m_DataName, sizeof(m_DataName)-1,1,fpEvlOut);
	fwrite(&m_ADCUnit, sizeof(m_ADCUnit)-1,1,fpEvlOut);
	fwrite(&m_Comment, sizeof(m_Comment)-1,1,fpEvlOut);
	fwrite(&m_TimePerPoint,sizeof(m_TimePerPoint),1,fpEvlOut);
	fwrite(&m_InterpoloationFacto,sizeof(m_InterpoloationFacto),1,fpEvlOut);
	fwrite(&m_AnalysisFilter,sizeof(m_AnalysisFilter),1,fpEvlOut);
	fwrite(&m_EpisodeSize,sizeof(m_EpisodeSize),1,fpEvlOut);
	fwrite(&m_TriggerStart,sizeof(m_TriggerStart),1,fpEvlOut);
	fwrite(&m_Holding,sizeof(m_Holding),1,fpEvlOut);
	fwrite(&m_AcquisitionFilter,sizeof(m_AcquisitionFilter),1,fpEvlOut);
	fwrite(&m_Gaps,sizeof(m_Gaps),1,fpEvlOut);
	fwrite(&m_TriggerEnd,sizeof(m_TriggerEnd),1,fpEvlOut);
	fwrite(&m_NumLevels,sizeof(m_NumLevels),1,fpEvlOut);
	fwrite(&m_Reserved,sizeof(m_Reserved)-1,1,fpEvlOut);
}

void CEvl::WriteEvlBody(FILE *fpEvlOut, int nIndex)
{
		fwrite(m_pnLevelStart+nIndex, sizeof(int),1,fpEvlOut);
		fwrite(m_pfAmplitude+nIndex, sizeof(float),1,fpEvlOut);
		fwrite(m_pnLevelLength+nIndex, sizeof(int),1,fpEvlOut);
		fwrite(m_pfEventStandardDev+nIndex, sizeof(float),1,fpEvlOut);
		fwrite(m_pstEventCurrentLevel+nIndex, sizeof(short),1,fpEvlOut);
		fwrite(m_pstEventNotes+nIndex,sizeof(short),1,fpEvlOut);
		if (*(m_pstEventCurrentLevel+nIndex)==0) 
		{
			fwrite(m_pfBaseline+nIndex,sizeof(float),1,fpEvlOut);
		}
}

void CEvl::WriteEvlBody(FILE *fpEvlOut, structEventNode aNode)
{
		int nLevelStart=aNode.LevelStart;
		int nLevelLength=aNode.LevelLength;
		float fAmpMean=aNode.fAmpMean;
		float fAmpDev=aNode.fAmpDev;
		fwrite(&nLevelStart, sizeof(int),1,fpEvlOut);
		fwrite(&fAmpMean, sizeof(float),1,fpEvlOut);
		fwrite(&nLevelLength, sizeof(int),1,fpEvlOut);
		fwrite(&fAmpDev, sizeof(float),1,fpEvlOut);
		short stLevel=aNode.stLevel;
		fwrite(&stLevel, sizeof(short),1,fpEvlOut);
		short stNotes=0;
		fwrite(&stNotes,sizeof(short),1,fpEvlOut);
		if (stLevel==0) 
		{
			fwrite(&fAmpMean,sizeof(float),1,fpEvlOut);
		}
}

void CEvl::WriteEvlBody(FILE *fpEvlOut, int nIndex, int nShiftLevelStart, int nShiftLevelLength)
{
		int nLevelStart=*(m_pnLevelStart+nIndex)+nShiftLevelStart;
		int nLevelLength=*(m_pnLevelLength+nIndex)+nShiftLevelLength;
		fwrite(&nLevelStart, sizeof(int),1,fpEvlOut);
		fwrite(m_pfAmplitude+nIndex, sizeof(float),1,fpEvlOut);
		fwrite(&nLevelLength, sizeof(int),1,fpEvlOut);
		fwrite(m_pfEventStandardDev+nIndex, sizeof(float),1,fpEvlOut);
		fwrite(m_pstEventCurrentLevel+nIndex, sizeof(short),1,fpEvlOut);
		fwrite(m_pstEventNotes+nIndex,sizeof(short),1,fpEvlOut);
		if (*(m_pstEventCurrentLevel+nIndex)==0) 
		{
			fwrite(m_pfBaseline+nIndex,sizeof(float),1,fpEvlOut);
		}
}

void CEvl::WriteBurstAsEvl(FILE* fpOut)
{
	int nMinOpenning=1;
	int i;
	FILE* fp=fopen("C:\\Axon\\Data\\btestevl.evl","w");
	int nIndex=0;
	short nLevel=1;
	WriteEvlHead(fpOut);
	for(i=0;i<*(m_pnBurstStartingEvent)-1;i++)
	{
		WriteEvlBody(fpOut,i);
		fprintf(fp,"%12d %12d %4d\n",*(m_pnLevelStart+i),*(m_pnLevelLength+i),
			*(m_pstEventCurrentLevel+i));
	}

	for(i=0;i<m_nNumBursts;i++)
	{
		if(*(m_pnNumOpenningInBurst+i)>nMinOpenning)
		{
			WriteBurstBody(fpOut,i);
			nIndex=*(m_pnBurstEndingEvent+i);
			fprintf(fp,"%12d %12d %4d\n",*(m_pnLevelStart+*(m_pnBurstStartingEvent+i)-1),
				*(m_pnBurstLength+i),nLevel);
			if(nIndex<m_nNumEvents) 
			{
				WriteEvlBody(fpOut,nIndex);
				fprintf(fp,"%12d %12d %4d\n",*(m_pnLevelStart+nIndex),
					*(m_pnLevelLength+nIndex),*(m_pstEventCurrentLevel+nIndex));
			}
		}
	}
}

void CEvl::WriteBurstBody(FILE *fpOut, int nIndex)
{
	float fAmp=-1.f;
	short stLevel=1;
	float fDev=0.f;
	short stNotes=0;
	fwrite(m_pnLevelStart+*(m_pnBurstStartingEvent+nIndex)-1, sizeof(int),1,fpOut);
	fwrite(&fAmp, sizeof(float),1,fpOut);
	fwrite(m_pnBurstLength+nIndex, sizeof(int),1,fpOut);
	fwrite(&fDev, sizeof(float),1,fpOut);
	fwrite(&stLevel, sizeof(short),1,fpOut);
	fwrite(&stNotes,sizeof(short),1,fpOut);
}

void CEvl::SetAttachingStatus()
{
	m_bAttachingEvl=true;
	m_nNumEvents0=m_nNumEvents;
	m_nEvlLength0=*(m_pnLevelStart+m_nNumEvents-1)+*(m_pnLevelLength+m_nNumEvents-1);
}

void CEvl::PreservePointers()
{
	delete m_pnLevelStart0;
	delete m_pfAmplitude0;
	delete m_pnLevelLength0;			 
	delete m_pfEventStandardDev0;
	delete m_pstEventCurrentLevel0;
	delete m_pstEventNotes0;
	delete m_pfBaseline0;
	delete m_pnValidEvent0;

	m_pnLevelStart0=new int[m_nNumEvents0];
	m_pfAmplitude0=new float[m_nNumEvents0];
	m_pnLevelLength0=new int[m_nNumEvents0];				 
	m_pfEventStandardDev0=new float[m_nNumEvents0];
	m_pstEventCurrentLevel0=new short[m_nNumEvents0];
	m_pstEventNotes0=new short[m_nNumEvents0]; 
	m_pfBaseline0=new float[m_nNumEvents0];
	m_pnValidEvent0=new int[m_nNumEvents0];

	for(int i=0;i<m_nNumEvents0;i++)
	{
		m_pnLevelStart0[i]=m_pnLevelStart[i];
		m_pfAmplitude0[i]=m_pfAmplitude[i];
		m_pnLevelLength0[i]=m_pnLevelLength[i];				 
		m_pfEventStandardDev0[i]=m_pfEventStandardDev[i];
		m_pstEventCurrentLevel0[i]=m_pstEventCurrentLevel[i];
		m_pstEventNotes0[i]=m_pstEventNotes[i]; 
		m_pfBaseline0[i]=m_pfBaseline[i];
		m_pnValidEvent0[i]=m_pnValidEvent[i];
	}
}

void CEvl::InsertOldEvl()
{
	for(int i=0;i<m_nNumEvents0;i++)
	{
		*(m_pnLevelStart+i)=*(m_pnLevelStart0+i);
		*(m_pfAmplitude+i)=*(m_pfAmplitude0+i);
		*(m_pnLevelLength+i)=*(m_pnLevelLength0+i);
		*(m_pstEventCurrentLevel+i)=*(m_pstEventCurrentLevel0+i);
		*(m_pstEventNotes+i)=*(m_pstEventNotes0+i);
		*(m_pfBaseline+i)=*(m_pfBaseline0+i);
		*(m_pnValidEvent+i)=*(m_pnValidEvent0+i);
	}
}



void CEvl::ShiftLevelStart()
{
	int nTemp;
	for(int i=m_nNumEvents0;i<m_nNumEvents;i++)
	{
		nTemp=*(m_pnLevelStart+i);
		*(m_pnLevelStart+i)+=m_nEvlLength0;
		nTemp=*(m_pnLevelStart+i);
	}
}

void CEvl::ResetAttachingStatus()
{
	m_bAttachingEvl=false;
	m_nNumEvents0=0;
}

void CEvl::DoTimeCourse(float fBinSize)
{
	CEvlTimeCourse cTC(m_sEvlFileName,fBinSize,m_TimePerPoint,m_nNumEvents,
		m_pnLevelLength,m_pnLevelStart,m_pstEventCurrentLevel, m_pfEffLevel, m_pnMRGEvents);
	cTC.DoEvlTimeCourse();
	DoBurstingTimeCourse();
}

void CEvl::DoTimeCourse0(float fBinSize)
{
	CEvlTimeCourse0 cTC(m_sEvlFileName,fBinSize,m_TimePerPoint,m_nNumEvents,
		m_pnLevelLength,m_pnLevelStart,m_pstEventCurrentLevel);
	cTC.DoEvlTimeCourse();
}

void CEvl::ReadAscIIFormatOfTakFitFile(CString sEvlFileName)
{
	FILE* fp=fopen(sEvlFileName,"r");
	int nSweep;
	short stLevel;
	int nLevelStart,nLevelLength;
	float fTime,fDuration,fAmplitude,fPreAmp,fPostAmp,fTemp;
	int nLineNum=0;
	while(!feof(fp))
	{
		fscanf(fp,"%d%f%f%e%e%e%d%e",&nSweep,&fTime,&fDuration,&fPreAmp,
			&fPostAmp,&fAmplitude,&stLevel,&fTemp);
		nLineNum++;
	}
	rewind(fp);
	m_nNumEvents0=0;
	m_nNumEvents=nLineNum;
	PrepairPointers();

	nLineNum=0;
	while(!feof(fp))
	{
		fscanf(fp,"%d%f%f%e%e%e%d%e",&nSweep,&fTime,&fDuration,&fPreAmp,
			&fPostAmp,&fAmplitude,&stLevel,&fTemp);
		nLevelStart=int(fTime*1000000.f/m_TimePerPoint+0.5);
		*(m_pnLevelStart+nLineNum)=nLevelStart;
		*(m_pfAmplitude+nLineNum)=fAmplitude;
		nLevelLength=int(fDuration*1000000.f/m_TimePerPoint+0.5);
		*(m_pnLevelLength+nLineNum)=nLevelLength;
		*(m_pfEventStandardDev+nLineNum)=0.;
		if(stLevel<0)
		{
			*(m_pstEventCurrentLevel+nLineNum)=-stLevel;
		}
		else
		{
			*(m_pstEventCurrentLevel+nLineNum)=stLevel;
		}
		*(m_pstEventNotes+nLineNum)=0;
		if (*(m_pstEventCurrentLevel+nLineNum)==0) 
		{
			*(m_pfBaseline+nLineNum)=0;
		}
		nLineNum++;
	}
}

void CEvl::IgnoreShortClosing_Original(float fCutoff)
{
	int nIgnoredEvents=10000;
	int i;
	short nCurrentLevel,nPreviousLevel,nNextLevel;
	int* pnValidEvent=new int[m_nNumEvents];
	int nShift;
//	int nMinimalLength=20;
	float fLength,fTime;
	
	delete m_pnMRGEvents;
	delete m_pfEffLevel;

	m_pnMRGEvents=new int[m_nNumEvents];
	m_pfEffLevel=new float[m_nNumEvents];
	int nLength, nLengthT, nLastPosition, nMRGEvents, nTemp;
	float fEffLevel;

	for(i=0;i<m_nNumEvents;i++)
	{
		*(pnValidEvent+i)=1;
		m_pnMRGEvents[i]=0;
		m_pfEffLevel[i]=float(m_pstEventCurrentLevel[i]);
	}

	bool bTransStateIgnored=false;
	while(nIgnoredEvents>=0)
	{
//		int nPreviousEvent=0;
		nPreviousLevel=*(m_pstEventCurrentLevel);
		nShift=0;
		for(i=1;i<m_nNumEvents-1;i++)
		{
			if(nIgnoredEvents>0)
			{
				nCurrentLevel=*(m_pstEventCurrentLevel+i);
				nNextLevel=*(m_pstEventCurrentLevel+i+1);
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				if(nCurrentLevel==nPreviousLevel)
				{
					*(pnValidEvent+i)=-1;
				}
				else if(nCurrentLevel<nPreviousLevel&&nCurrentLevel<nNextLevel&&fLength<fCutoff)
				{
					*(pnValidEvent+i)=-1;
					if(nNextLevel>nPreviousLevel)
					{
						*(pnValidEvent+i)=-1;
						nCurrentLevel=nPreviousLevel;
					}
					else
					{
						pnValidEvent[i]=-2;  //Need to update the level later
						nCurrentLevel=nNextLevel;
					}
				}
			}
			else
			{
				nCurrentLevel=*(m_pstEventCurrentLevel+i);
				nNextLevel=*(m_pstEventCurrentLevel+i+1);
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				if(nCurrentLevel<nPreviousLevel&&nCurrentLevel>nNextLevel&&fLength<3.f)
				{
					*(pnValidEvent+i)=-1;
				}
				else if(nCurrentLevel>nPreviousLevel&&nCurrentLevel<nNextLevel&&fLength<3.f)
				{
					*(pnValidEvent+i+1)=-1;
					*(m_pstEventCurrentLevel+i)=nNextLevel;
				}
			}

			nPreviousLevel=nCurrentLevel;
		}

		if(nIgnoredEvents==0)
			bTransStateIgnored=true;

		nIgnoredEvents=0;

		bool bFirstValidEvent=false;
		int nLastValidEvent=0;
		for(i=0;i<m_nNumEvents;i++)
		{
			if(*(pnValidEvent+i)!=-1)
			{
				if(bFirstValidEvent)
				{
					*(m_pnLevelLength+nLastValidEvent)=
						*(m_pnLevelStart+i)-*(m_pnLevelStart+nLastValidEvent);
					if(pnValidEvent[nLastPosition]==-2)//Added
					{
						m_pstEventCurrentLevel[nLastValidEvent]=m_pstEventCurrentLevel[nLastPosition+1];
					}
					nLengthT=0;
					nMRGEvents=0;
					for(nTemp=nLastPosition;nTemp<i;nTemp++)
					{
						nLength=m_pnLevelStart[nTemp];
						nLengthT+=nLength;
						fEffLevel+=double(nLength)*m_pfEffLevel[nTemp];

						if(nTemp>nLastPosition)
						{
							nCurrentLevel=m_pstEventCurrentLevel[nTemp];
							nPreviousLevel=m_pstEventCurrentLevel[nTemp-1];
							if(nPreviousLevel>nCurrentLevel)
							{
								nMRGEvents++;
							}
						}
					}
					fEffLevel/=double(nLengthT);
					m_pfEffLevel[nLastValidEvent]=fEffLevel;
					m_pnMRGEvents[nLastValidEvent]=nMRGEvents;
						
					bFirstValidEvent=false;
				}
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				nShift=-nIgnoredEvents;
				*(pnValidEvent+i+nShift)=*(pnValidEvent+i);
				*(m_pnLevelLength+i+nShift)=*(m_pnLevelLength+i);
				*(m_pnLevelStart+i+nShift)=*(m_pnLevelStart+i);
				*(m_pstEventCurrentLevel+i+nShift)=*(m_pstEventCurrentLevel+i);
				*(m_pfAmplitude+i+nShift)=*(m_pfAmplitude+i);
				*(m_pfBaseline+i+nShift)=*(m_pfBaseline+i);
				m_pfEffLevel[i+nShift]=m_pfEffLevel[i];
				m_pnMRGEvents[i+nShift]=m_pnMRGEvents[i];
			}
			else
			{
				if(!bFirstValidEvent)
				{
					nLastValidEvent=i-1+nShift;
					nLastPosition=i-1;
				}
				bFirstValidEvent=true;
				nIgnoredEvents++;
			}
		}
		m_nNumEvents-=nIgnoredEvents;
//		if(bTransStateIgnored)
			nIgnoredEvents=-1;
	}
	delete pnValidEvent;
}

void CEvl::IgnoreShortClosing(float fCutoff)
{
	int nIgnoredEvents=10000;
	int i;
	short nCurrentLevel,nPreviousLevel,nNextLevel;
	int* pnValidEvent=new int[m_nNumEvents];
	int nShift;
//	int nMinimalLength=20;
	float fLength,fTime;
	
	delete m_pnMRGEvents;
	delete m_pfEffLevel;

	m_pnMRGEvents=new int[m_nNumEvents];
	m_pfEffLevel=new float[m_nNumEvents];
	int nLength, nLengthT, nLastPosition, nMRGEvents, nTemp;
	float fEffLevel;

	for(i=0;i<m_nNumEvents;i++)
	{
		*(pnValidEvent+i)=1;
		m_pnMRGEvents[i]=0;
		m_pfEffLevel[i]=float(m_pstEventCurrentLevel[i]);
	}

	bool bTransStateIgnored=false;
	while(nIgnoredEvents>=0)
	{
//		int nPreviousEvent=0;
		nPreviousLevel=*(m_pstEventCurrentLevel);
		nShift=0;
		for(i=1;i<m_nNumEvents-1;i++)
		{
			if(nIgnoredEvents>0)
			{
				nCurrentLevel=*(m_pstEventCurrentLevel+i);
				nNextLevel=*(m_pstEventCurrentLevel+i+1);
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				if(nCurrentLevel==nPreviousLevel)
				{
					*(pnValidEvent+i)=-1;
				}
				else if(nCurrentLevel<nPreviousLevel&&nCurrentLevel<nNextLevel&&fLength<fCutoff)
				{
					*(pnValidEvent+i)=-1;
					if(nNextLevel>nPreviousLevel)
					{
						nCurrentLevel=nPreviousLevel;
					}
					else
					{
						nCurrentLevel=nNextLevel;
					}
				}
			}
			else
			{
				nCurrentLevel=*(m_pstEventCurrentLevel+i);
				nNextLevel=*(m_pstEventCurrentLevel+i+1);
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				if(nCurrentLevel<nPreviousLevel&&nCurrentLevel>nNextLevel&&fLength<3.f)
				{
					*(pnValidEvent+i)=-1;
				}
				else if(nCurrentLevel>nPreviousLevel&&nCurrentLevel<nNextLevel&&fLength<3.f)
				{
					*(pnValidEvent+i+1)=-1;
					*(m_pstEventCurrentLevel+i)=nNextLevel;
				}
			}

			nPreviousLevel=nCurrentLevel;
		}

		if(nIgnoredEvents==0)
			bTransStateIgnored=true;

		nIgnoredEvents=0;

		bool bFirstValidEvent=false;
		int nLastValidEvent=0;
		for(i=0;i<m_nNumEvents;i++)
		{
			if(*(pnValidEvent+i)!=-1)
			{
				if(bFirstValidEvent)
				{
					*(m_pnLevelLength+nLastValidEvent)=
						*(m_pnLevelStart+i)-*(m_pnLevelStart+nLastValidEvent);
					nLengthT=0;
					nMRGEvents=0;
					fEffLevel=0.f;
					for(nTemp=nLastPosition;nTemp<i;nTemp++)
					{
						nLength=m_pnLevelLength[nTemp];
						nLengthT+=nLength;
						fEffLevel+=double(nLength)*m_pfEffLevel[nTemp];

						if(nTemp>nLastPosition)
						{
							nCurrentLevel=m_pstEventCurrentLevel[nTemp];
							nPreviousLevel=m_pstEventCurrentLevel[nTemp-1];
							if(nPreviousLevel>nCurrentLevel)
							{
								nMRGEvents+=(nPreviousLevel-nCurrentLevel);
							}
						}
					}
					fEffLevel/=double(nLengthT);
					m_pfEffLevel[nLastValidEvent]=fEffLevel;
					m_pnMRGEvents[nLastValidEvent]=nMRGEvents;
						
					bFirstValidEvent=false;
				}
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				nShift=-nIgnoredEvents;
				*(pnValidEvent+i+nShift)=*(pnValidEvent+i);
				*(m_pnLevelLength+i+nShift)=*(m_pnLevelLength+i);
				*(m_pnLevelStart+i+nShift)=*(m_pnLevelStart+i);
				*(m_pstEventCurrentLevel+i+nShift)=*(m_pstEventCurrentLevel+i);
				*(m_pfAmplitude+i+nShift)=*(m_pfAmplitude+i);
				*(m_pfBaseline+i+nShift)=*(m_pfBaseline+i);
				m_pfEffLevel[i+nShift]=m_pfEffLevel[i];
				m_pnMRGEvents[i+nShift]=m_pnMRGEvents[i];
			}
			else
			{
				if(!bFirstValidEvent)
				{
					//First invalid event
					nLastValidEvent=i-1+nShift;
					nLastPosition=i-1;
				}
				bFirstValidEvent=true;
				//No longer the first invalid event
				nIgnoredEvents++;
			}
		}
		m_nNumEvents-=nIgnoredEvents;
//		if(bTransStateIgnored)
			nIgnoredEvents=-1;
	}
	delete pnValidEvent;
}

void CEvl::ExportAsciiEvl(FILE *fp)
{
	float fTime=0,fDwellTime,fAmp,fAmpDev,fBaseline;
	short nLevel;
	float fTimePerPoint=m_TimePerPoint/1000.f;
	fprintf(fp,"      Time   DwellTime   Level      Amp   AmpDev  BaseLine     Event\n");
	for(int i=0;i<m_nNumEvents;i++)
	{
		fTime=*(m_pnLevelStart+i)*fTimePerPoint;
		fDwellTime=*(m_pnLevelLength+i)*fTimePerPoint;
		nLevel=*(m_pstEventCurrentLevel+i);
		fAmp=*(m_pfAmplitude+i);
		fAmpDev=*(m_pfEventStandardDev+i);
		if(nLevel==0)
		{
			fBaseline=*(m_pfBaseline+i);
		}
		fprintf(fp,"%10.1f  %10.1f  %6d  %7.2f  %7.2f   %7.2f  %8d\n",fTime,fDwellTime,nLevel,fAmp,fAmpDev,fBaseline,i+1);
	}
}

void CEvl::ExportSummary(FILE *fpSummary, int nCount)
{
	m_cSummary.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,fpSummary,m_fRisetime,m_sEvlFileName);
	m_cSummary.ClearSummary();
	m_cSummary.SummarizeEvl();
	m_cSummary.OutputSummary(nCount);	
}

void CEvl::ExportSummary(FILE *fpSummary)
{
	m_cSummary.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,fpSummary,m_fRisetime,m_sEvlFileName);
	m_cSummary.ClearSummary();
	m_cSummary.SummarizeEvl();
	m_cSummary.OutputSummary();	
}

structEvlSummaryNode CEvl::GetSummaryNode()
{
	return m_cSummary.GetSummaryNode();
}

void CEvl::IgnoreShortOpenings(float fCutoff)
{
	int nIgnoredEvents=10000;
	int i,nCurrentLevel,nPreviousLevel,nNextLevel;
	int* pnValidEvent=new int[m_nNumEvents];
	int nShift;
	float fLength,fTime;
	for(i=0;i<m_nNumEvents;i++)
	{
		*(pnValidEvent+i)=1;
	}
	while(nIgnoredEvents>0)
	{
//		int nPreviousEvent=0;
		nPreviousLevel=*(m_pstEventCurrentLevel);
		nShift=0;
		for(i=1;i<m_nNumEvents-1;i++)
		{
			nCurrentLevel=*(m_pstEventCurrentLevel+i);
			nNextLevel=*(m_pstEventCurrentLevel+i+1);
			fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
			fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
			if(nCurrentLevel==nPreviousLevel)
			{
				*(pnValidEvent+i)=-1;
			}
			else if(nCurrentLevel>nPreviousLevel&&nCurrentLevel>nNextLevel&&fLength<fCutoff)
			{
				*(pnValidEvent+i)=-1;
				if(nNextLevel<nPreviousLevel)
				{
					nCurrentLevel=nPreviousLevel;
				}
				else
				{
					nCurrentLevel=nNextLevel;
				}
			}
			nPreviousLevel=nCurrentLevel;
		}

		nIgnoredEvents=0;
		bool bFirstValidEvent=false;
		int nLastValidEvent=0;
		for(i=0;i<m_nNumEvents;i++)
		{
			if(*(pnValidEvent+i)!=-1)
			{
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				nShift=-nIgnoredEvents;
				*(pnValidEvent+i+nShift)=*(pnValidEvent+i);
				*(m_pnLevelLength+i+nShift)=*(m_pnLevelLength+i);
				*(m_pnLevelStart+i+nShift)=*(m_pnLevelStart+i);
				*(m_pstEventCurrentLevel+i+nShift)=*(m_pstEventCurrentLevel+i);
				*(m_pfAmplitude+i+nShift)=*(m_pfAmplitude+i);
				*(m_pfBaseline+i+nShift)=*(m_pfBaseline+i);
				if(bFirstValidEvent)
				{
					*(m_pnLevelLength+nLastValidEvent)=
						*(m_pnLevelStart+i)-*(m_pnLevelStart+nLastValidEvent);
					bFirstValidEvent=false;
				}
			}
			else
			{
				if(!bFirstValidEvent)
				{
					nLastValidEvent=i-1+nShift;
				}
				bFirstValidEvent=true;
				nIgnoredEvents++;
			}
		}
		m_nNumEvents-=nIgnoredEvents;
	}
	delete pnValidEvent;

}

void CEvl::ExcludeLowAmp()
{
	float fAmp0=*(m_pfAmplitude);
	float fAmp=*(m_pfAmplitude+1);
	float fAmp1;
	short nLevel0=*(m_pstEventCurrentLevel);
	short nLevel=*(m_pstEventCurrentLevel+1);
	int nLength;
	short nLevel1;
	int nIgnored=0;
	int i;
	for(i=1;i<m_nNumEvents-1;i++)
	{
		fAmp1=*(m_pfAmplitude+i+1);
		nLevel1=*(m_pstEventCurrentLevel+i+1);
		if(fabs(fAmp-fAmp0)<m_fAmpThreshold&&fabs(fAmp-fAmp1)<m_fAmpThreshold)
		{
			*(m_pbExcluded+i)=true;
			nLength=*(m_pnLevelLength+i);
			if(nLevel0==nLevel1)
			{
				*(m_pstEventCurrentLevel+i)=nLevel0;
				nLevel=nLevel0;
				nIgnored++;
			}
		}
		fAmp0=fAmp;
		fAmp=fAmp1;
		nLevel0=nLevel;
		nLevel=nLevel1;
	}

	int nIgnoredEvents=10000;
	int nCurrentLevel,nPreviousLevel,nNextLevel;
	int* pnValidEvent=new int[m_nNumEvents];
	int nShift;
	float fLength,fTime;
	for(i=0;i<m_nNumEvents;i++)
	{
		*(pnValidEvent+i)=1;
	}
	while(nIgnoredEvents>0)
	{
//		int nPreviousEvent=0;
		nPreviousLevel=*(m_pstEventCurrentLevel);
		nShift=0;
		for(i=1;i<m_nNumEvents-1;i++)
		{
			nCurrentLevel=*(m_pstEventCurrentLevel+i);
			nNextLevel=*(m_pstEventCurrentLevel+i+1);
			fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
			fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
			if(nCurrentLevel==nPreviousLevel)
			{
				*(pnValidEvent+i)=-1;
			}
			nPreviousLevel=nCurrentLevel;
		}

		nIgnoredEvents=0;
		bool bFirstValidEvent=false;
		int nLastValidEvent=0;
		for(i=0;i<m_nNumEvents;i++)
		{
			if(*(pnValidEvent+i)!=-1)
			{
				fTime=*(m_pnLevelStart+i)*m_TimePerPoint/1000.f;
				fLength=*(m_pnLevelLength+i)*m_TimePerPoint/1000.f;
				nShift=-nIgnoredEvents;
				*(pnValidEvent+i+nShift)=*(pnValidEvent+i);
				*(m_pnLevelLength+i+nShift)=*(m_pnLevelLength+i);
				*(m_pnLevelStart+i+nShift)=*(m_pnLevelStart+i);
				*(m_pstEventCurrentLevel+i+nShift)=*(m_pstEventCurrentLevel+i);
				*(m_pfAmplitude+i+nShift)=*(m_pfAmplitude+i);
				*(m_pfBaseline+i+nShift)=*(m_pfBaseline+i);
				if(bFirstValidEvent)
				{
					*(m_pnLevelLength+nLastValidEvent)=
						*(m_pnLevelStart+i)-*(m_pnLevelStart+nLastValidEvent);
					bFirstValidEvent=false;
				}
			}
			else
			{
				if(!bFirstValidEvent)
				{
					nLastValidEvent=i-1+nShift;
				}
				bFirstValidEvent=true;
				nIgnoredEvents++;
			}
		}
		m_nNumEvents-=nIgnoredEvents;
	}
	delete pnValidEvent;
}

void CEvl::ReadEvl(CString sEvlFileName1, CString sEvlFileName2)
{//This function is used only in the case that the head and the body of the event list read from separate files.
 
	m_bEvlHist=false;
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	if(!m_bAttachingEvl)
	{
		m_sEvlFileName=sEvlFileName2;
	}

	FILE *fpEvlIn;		
	fpEvlIn=fopen(sEvlFileName1,"rb");
	FILE *fpEvlIn2;		
	fpEvlIn2=fopen(sEvlFileName2,"rb");

	int nNumEvents;
	fread(&nNumEvents,sizeof(int),1,fpEvlIn2);

	m_nNumEvents=m_nNumEvents0+nNumEvents-1;
	PrepairPointers();

	fread(&m_FileId, sizeof(m_FileId)-1,1,fpEvlIn);
	fread(&m_FFversion, sizeof(m_FFversion),1, fpEvlIn);
	fread(&m_DataName, sizeof(m_DataName)-1,1,fpEvlIn);
	fread(&m_ADCUnit, sizeof(m_ADCUnit)-1,1,fpEvlIn);
	fread(&m_Comment, sizeof(m_Comment)-1,1,fpEvlIn);
	fread(&m_TimePerPoint,sizeof(m_TimePerPoint),1,fpEvlIn);
	fread(&m_InterpoloationFacto,sizeof(m_InterpoloationFacto),1,fpEvlIn);
	fread(&m_AnalysisFilter,sizeof(m_AnalysisFilter),1,fpEvlIn);
	fread(&m_EpisodeSize,sizeof(m_EpisodeSize),1,fpEvlIn);
	fread(&m_TriggerStart,sizeof(m_TriggerStart),1,fpEvlIn);
	fread(&m_Holding,sizeof(m_Holding),1,fpEvlIn);
	fread(&m_AcquisitionFilter,sizeof(m_AcquisitionFilter),1,fpEvlIn);
	fread(&m_Gaps,sizeof(m_Gaps),1,fpEvlIn);
	fread(&m_TriggerEnd,sizeof(m_TriggerEnd),1,fpEvlIn);
	fread(&m_NumLevels,sizeof(m_NumLevels),1,fpEvlIn);
	fread(&m_Reserved,sizeof(m_Reserved)-1,1,fpEvlIn);

	fclose(fpEvlIn);
	GetChannelNumbers();



	int nShift=0;
	m_nRecordLength_Actual=0;
	for(int i=m_nNumEvents0;i<m_nNumEvents-1;i++)
	{
		fread(m_pnLevelStart+i, sizeof(int),1,fpEvlIn2);
		fread(m_pfAmplitude+i, sizeof(float),1,fpEvlIn2);
		fread(m_pnLevelLength+i, sizeof(int),1,fpEvlIn2);
		fread(m_pfEventStandardDev+i, sizeof(float),1,fpEvlIn2);
		fread(m_pstEventCurrentLevel+i, sizeof(short),1,fpEvlIn2);
		fread(m_pstEventNotes+i,sizeof(short),1,fpEvlIn2);
		m_nRecordLength_Actual+=m_pnLevelLength[i];
		if (*(m_pstEventCurrentLevel+i)==0) 
		{
			fread(m_pfBaseline+i,sizeof(float),1,fpEvlIn2);
		}
		if(i==m_nNumEvents0)
		{
			if(m_pstEventCurrentLevel[0]==0&&m_bExcludingLastClosure)
			{
				nShift=-1;
				m_nRecordLength_Actual=0;//11/28/2007
			}
		}
	}
/*
	for(int i=m_nNumEvents0;i<m_nNumEvents-1;i++)
	{
		fread(m_pnLevelStart+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfAmplitude+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pnLevelLength+i+nShift, sizeof(int),1,fpEvlIn);
		fread(m_pfEventStandardDev+i+nShift, sizeof(float),1,fpEvlIn);
		fread(m_pstEventCurrentLevel+i+nShift, sizeof(short),1,fpEvlIn);
		fread(m_pstEventNotes+i+nShift,sizeof(short),1,fpEvlIn);
		m_nRecordLength_Actual+=m_pnLevelLength[i+nShift];
		if (*(m_pstEventCurrentLevel+i+nShift)==0) 
		{
			fread(m_pfBaseline+i+nShift,sizeof(float),1,fpEvlIn);
		}
		if(i==m_nNumEvents0)
		{
			if(m_pstEventCurrentLevel[0]==0&&m_bExcludingLastClosure)
			{
				nShift=-1;
				m_nRecordLength_Actual=0;//11/28/2007
			}
		}
	}
*/
	if(m_bExcludingLastClosure)
		m_nNumEvents-=(1-nShift); //11/28/2007




	m_nRecordLength_Formal=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];

	if(m_bAttachingEvl) 
	{
		ShiftLevelStart();
		ResetAttachingStatus();
	}

	fclose(fpEvlIn2);
}

void CEvl::AmpHistogram()
{
	float fHist[1000];
	float fHist1[1000];
	float fDelta=0.1f;
	int nHighestCell=0;
	int it,it1,nLength,i;
	
	for(i=0;i<1000;i++)
	{
		fHist[i]=0.f;
		fHist1[i]=0.f;
	}

	for(i=1;i<m_nNumEvents;i++)
	{
		it=abs(int(*(m_pfAmplitude+i)/fDelta));
		it1=abs(int((*(m_pfAmplitude+i)-*(m_pfAmplitude+i-1))/fDelta));
		fHist[it]+=1.f;
		fHist1[it1]+=1.f;
		if(it1>21)
		{
			it1=it1;
		}
		if(it>nHighestCell)
		{
			nHighestCell=it;
		}
	}
	CString sHistFileName=m_sEvlFileName;
	nLength=sHistFileName.GetLength();
	sHistFileName.SetAt(nLength-3,'h');
	sHistFileName.SetAt(nLength-2,'s');
	sHistFileName.SetAt(nLength-1,'t');
	FILE* fp=fopen(sHistFileName,"w");
	for(i=0;i<=nHighestCell;i++)
	{
		fprintf(fp,"%8.2f  %8.1f  %8.1f\n",-i*fDelta,fHist[i],fHist1[i]);
	}

	fclose(fp);
}

void CEvl::LocalExtrema(const float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax)
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

void CEvl::ReAssignAmp(AbfNode structTrace)
{
	int nCurrentPoint;
	int nLength;
	int nStart;
	int* pnLMinima=new int[structTrace.nSize/2];
	int* pnLMaxima=new int[structTrace.nSize/2];
	int lMin,lMax;
	int it,nSize;
	float fAmp,fBiggest;
	LocalExtrema(structTrace.pfData,pnLMinima,pnLMaxima,structTrace.nSize,lMin,lMax);
	it=0;
	for(int i=1;i<m_nNumEvents;i++)
	{
		nStart=*(m_pnLevelStart+i);
		nLength=*(m_pnLevelLength+i);
		nCurrentPoint=*(pnLMinima+it);
		nSize=0;
		fAmp=100000.f;
		fBiggest=1111.f;

		while(nCurrentPoint<nStart+nLength)
		{
			nSize++;
			fAmp=*(structTrace.pfData+nCurrentPoint);
			if(fAmp<fBiggest)
				fBiggest=fAmp;
			it++;
			if(it<lMin)
			{
				nCurrentPoint=*(pnLMinima+it);
			}
			else
			{
				nCurrentPoint=structTrace.nSize+2;
			}
		}

//		if(nSize>0)
//		{
//			fAmp/=nSize;
//			*(m_pfAmplitude+i)=fAmp;
//		}

		*(m_pfAmplitude+i)=fBiggest;
	}	
}

void CEvl::RemoveDegeneracy(int nMultp)
{
	short stLevel;
	for(int i=1;i<m_nNumEvents;i++)
	{
		stLevel=*(m_pstEventCurrentLevel+i);
		*(m_pstEventCurrentLevel+i)=(stLevel+nMultp-1)/nMultp;
	}	
	m_NumLevels/=nMultp;
}

EvlNode CEvl::GetEvlNode()
{
	EvlNode aEvlNode;
	aEvlNode.fTimePerPoint=m_TimePerPoint;
	aEvlNode.nNumEvents=m_nNumEvents;
	aEvlNode.pfAmplitude=m_pfAmplitude;
	aEvlNode.pfBaseline=m_pfBaseline;
	aEvlNode.pnLevelLength=m_pnLevelLength;
	aEvlNode.pnLevelStart=m_pnLevelStart;
	aEvlNode.pstEventCurrentLevel=m_pstEventCurrentLevel;
	return aEvlNode;
}

void CEvl::GetChannelNumbers()
{
	char sTemp[10]="Channels:";
	char sTemp1[10]="";
	int nTemp;
	m_nNumChannels=-1;
	for(int i=0;i<120;i++)
	{
		for(int j=0;j<9;j++)
		{
			sTemp1[j]=m_Comment[i+j];
		}
		if(strcmp(sTemp,sTemp1)==0)
		{
			nTemp=m_Comment[i+9]-48;
			m_nChannelNumberPosition=i+9;

			int k=i+8;
			while(nTemp<0 || nTemp>9)
			{
				k++;
				nTemp=m_Comment[k]-48;
				m_nChannelNumberPosition=i+k;
			}
			m_nNumChannels=nTemp;
		}
	}
	if(m_nNumChannels==-1)
	{
		m_nNumChannels=MaxLevel();
	}
}

AbfNode CEvl::GetEvlAsTrace()
{
	int nPolarity=-1;
	int nDataSize,i,j,nPosition;
	nDataSize=m_nRecordLength_Formal;
	AbfNode aNode;
	aNode.nSize=nDataSize;
	float* pfTemp=new float[nDataSize];

	nPosition=0;
	int nTemp;
	int nLength,nBeginning0,nBeginning1;
	for(i=1;i<m_nNumEvents;i++)
	{
		nTemp=m_pstEventCurrentLevel[i-1];
		nBeginning0=m_pnLevelStart[i-1];
		nBeginning1=m_pnLevelStart[i];
		nLength=m_pnLevelLength[i-1];
		if(nLength<=0)
		{
			i=i;
		}
		if(i==95273)
		{
			i=i;
		}
		for(j=m_pnLevelStart[i-1];j<m_pnLevelStart[i];j++)
		{
			pfTemp[nPosition]=float(nPolarity*nTemp);
			nPosition++;
		}
	}

	for(i=m_nNumEvents-1;i<m_nNumEvents;i++)
	{
		nTemp=m_pstEventCurrentLevel[i];
		for(j=0;j<m_pnLevelLength[i];j++)
		{
			pfTemp[nPosition]=float(nPolarity*nTemp);
			nPosition++;
		}
	}

	aNode.pfData=pfTemp;
	aNode.nSize=nPosition;
	return aNode;
}

AbfNode CEvl::GetEvlAsTrace(int nKey)
{
	int nDataSize,i,j,nPosition;
	nDataSize=m_nRecordLength_Formal;
	AbfNode aNode;
	aNode.nSize=nDataSize;
	float* pfTemp=new float[nDataSize];

	nPosition=0;
	float fTemp;
	for(i=0;i<m_nNumEvents;i++)
	{
		fTemp=m_pfAmplitude[i];
		for(j=0;j<m_pnLevelLength[i];j++)
		{
			pfTemp[nPosition]=fTemp;
			nPosition++;
		}
	}
	aNode.pfData=pfTemp;
	aNode.nSize=nPosition;
	return aNode;
}

void CEvl::Summarize()
{
	FILE* fpSummary=NULL;
	m_cSummary.ClearSummary();
	m_cSummary.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,fpSummary,m_fRisetime,m_sEvlFileName);
	m_cSummary.SummarizeEvl();
}

//const CEvl& CEvl::operator= (const CEvl& aEvl)
const CEvl& CEvl::operator= (const CEvl& aEvl)
{
	m_nWritingKey=0;
	m_nReadingKey=0;
	m_pnLevelStart=NULL;
	m_pfAmplitude=NULL;
	m_pnLevelLength=NULL;				 
	m_pfEventStandardDev=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pstEventNotes=NULL; 
	m_pfBaseline=NULL;
	m_pnBurstStartingEvent=NULL;
	m_pnBurstEndingEvent=NULL;
	m_pnBurstLength=NULL;
	m_nMinimumOpenning=2;
	m_fBurstDelimiter=3.f;
	m_pbMultipleOpenning=NULL;
	m_pnNumOpenningInBurst=NULL;
	m_pnMRGEvents=NULL;
	m_pfEffLevel=NULL;
	m_nNumEvents0=0;
	m_bAttachingEvl=false;
	m_nEvlLength0=0;
	m_bEvlHist=false;
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	m_bModeSeparated=false;

	m_pnLevelStart0=NULL;
	m_pfAmplitude0=NULL;
	m_pnLevelLength0=NULL;				 
	m_pfEventStandardDev0=NULL;
	m_pstEventCurrentLevel0=NULL;
	m_pstEventNotes0=NULL; 
	m_pfBaseline0=NULL;
	m_pnValidEvent=NULL;
	m_pbExcluded=NULL;
	m_fAmpThreshold=0.5f;
	m_pnValidEvent0=NULL;
	strcpy(m_cNumChannels, "Channels: 2");
	m_nChannelNumberPosition=-1;
	m_nMaxNumEvents=10000000;
	//06707 //Can not destruct the class
	strcpy(m_FileId,aEvl.m_FileId);
	m_FFversion=aEvl.m_FFversion;
	strcpy(m_DataName,aEvl.m_DataName);
	strcpy(m_ADCUnit,aEvl.m_ADCUnit);
	strcpy(m_Comment,aEvl.m_Comment);
	m_TimePerPoint=aEvl.m_TimePerPoint;
	m_InterpoloationFacto=aEvl.m_InterpoloationFacto;
	m_AnalysisFilter=aEvl.m_AnalysisFilter;
	m_EpisodeSize=aEvl.m_EpisodeSize;
	m_TriggerStart=aEvl.m_TriggerStart;
	m_Holding=aEvl.m_Holding;
	m_AcquisitionFilter=aEvl.m_AcquisitionFilter;
	m_Gaps=aEvl.m_Gaps;
	m_TriggerEnd=aEvl.m_TriggerEnd;
	m_NumLevels=aEvl.m_NumLevels;
	strcpy(m_Reserved,aEvl.m_Reserved);

	
	m_nNumChannels=aEvl.m_nNumChannels;
	m_nNumEvents=aEvl.m_nNumEvents;
	m_sEvlFileName=aEvl.m_sEvlFileName;

	delete m_pnLevelStart;
	delete m_pnLevelLength;
	delete m_pfAmplitude;
	delete m_pstEventCurrentLevel;
	delete m_pbExcluded;

	delete m_pfEventStandardDev;
	delete m_pstEventNotes;
	delete m_pfBaseline;



	m_pnLevelStart=new int[m_nNumEvents];
	m_pnLevelLength=new int[m_nNumEvents];
	m_pfAmplitude=new float[m_nNumEvents];
	m_pstEventCurrentLevel=new short[m_nNumEvents];
	m_pbExcluded=new bool[m_nNumEvents];

	m_pfEventStandardDev=new float[m_nNumEvents];
	m_pstEventNotes=new short[m_nNumEvents];
	m_pfBaseline=new float[m_nNumEvents];

	for(int i=0;i<m_nNumEvents;i++)
	{
		m_pnLevelStart[i]=aEvl.m_pnLevelStart[i];
		m_pnLevelLength[i]=aEvl.m_pnLevelLength[i];
		m_pfAmplitude[i]=aEvl.m_pfAmplitude[i];
		m_pstEventCurrentLevel[i]=aEvl.m_pstEventCurrentLevel[i];
		m_pbExcluded[i]=aEvl.m_pbExcluded[i];
		m_pfEventStandardDev[i]=m_pfEventStandardDev[i];
		m_pstEventNotes[i]=m_pstEventNotes[i];
		m_pfBaseline[i]=m_pfBaseline[i];
	}
	m_nRecordLength_Formal=aEvl.m_nRecordLength_Formal;
	m_nRecordLength_Actual=aEvl.m_nRecordLength_Actual;
	return *this;
}

void CEvl::IgnoreSmallTransitions(float fCutoff)
{
	int i, i0, nDelta;
	float fAmp0,fAmp, fAmp1, fDeltal, fDeltar;
	i0=0;
	for(i=1;i<m_nNumEvents-1;i++)
	{
		nDelta=1;
		if(IsLocalExtremum(i0,i))
		{
			fAmp0=m_pfAmplitude[i0];
			fAmp=m_pfAmplitude[i];
			fAmp1=m_pfAmplitude[i+1];
			fDeltal=fAmp0-fAmp;
			fDeltar=fAmp-fAmp1;
		
			if(fDeltal>fCutoff||-fDeltal>fCutoff)
			{
				MoveEvent(i0+1,i);
			}
			else if(fDeltar>fCutoff||-fDeltar>fCutoff)
			{
				MoveEvent(i0+1,i);
			}
			else
			{
				nDelta=ConcatenateEvents(i0,i);
			}
		}
		else
		{
			MoveEvent(i0+1,i);
		}
		i0+=nDelta;
	}
	m_nNumEvents=i0+1;
	int nTemp=0;
	for(i=0;i<m_nNumEvents;i++)
	{
		nTemp+=m_pnLevelLength[i];
	}
	m_nRecordLength_Actual=nTemp;
}

bool CEvl::IsLocalExtremum(int i0, int i1)
{
	int nCurrentLevel=m_pstEventCurrentLevel[i1];
	int nPreviousLevel=m_pstEventCurrentLevel[i0];
	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	if(nCurrentLevel > nPreviousLevel && nCurrentLevel > nNextLevel)
	{
		return true;
	}
	else if(nCurrentLevel<nPreviousLevel && nCurrentLevel < nNextLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CEvl::IsLocalMaximum(int i0, int i1)
{
	int nCurrentLevel=m_pstEventCurrentLevel[i1];
	int nPreviousLevel=m_pstEventCurrentLevel[i0];
	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	if(nCurrentLevel>nPreviousLevel&&nCurrentLevel>nNextLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CEvl::IsLocalMinimum(int i0, int i1)
{
	int nCurrentLevel=m_pstEventCurrentLevel[i1];
	int nPreviousLevel=m_pstEventCurrentLevel[i0];
	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	if(nCurrentLevel<nPreviousLevel && nCurrentLevel < nNextLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CEvl::ConcatenateEvents(int i0, int& i1)
{
	int nCurrentLevel=m_pstEventCurrentLevel[i1];
	int nPreviousLevel=m_pstEventCurrentLevel[i0];
	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	int nDelta;
	if(nPreviousLevel == nNextLevel)
	{
		MergeThreeEvents(i0, i1);
		nDelta=0;
		i1++;
	}
	else if((nPreviousLevel-nNextLevel)*(nPreviousLevel-nCurrentLevel) > 0)
	{
		MergeToRightEvent(i0,i1);
		nDelta=1;
		i1++;
	}
	else
	{
		MergeToLeftEvent(i0,i1);
		nDelta=0;
	}
	return nDelta;
}


void CEvl::MergeThreeEvents(int i0, int i1)
{
//	int nCurrentLevel=m_pstEventCurrentLevel[i1];
//	int nPreviousLevel=m_pstEventCurrentLevel[i0];
//	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	float fNewAmp=0.f;
	float fNewDev=0.f;
	short stNewLevel=m_pstEventCurrentLevel[i0];
	int nNewLength, nNewStart;
	float fNewBaseline;

	nNewStart=m_pnLevelStart[i0];
	nNewLength=m_pnLevelLength[i0]+m_pnLevelLength[i1]+m_pnLevelLength[i1+1];
	fNewAmp=(m_pfAmplitude[i0]*m_pnLevelLength[i0]+m_pfAmplitude[i1]*m_pnLevelLength[i1]+m_pfAmplitude[i1+1]*m_pnLevelLength[i1+1])/float(nNewLength);
	fNewDev=m_pfEventStandardDev[i0]*m_pfEventStandardDev[i0];
	fNewDev+=m_pfEventStandardDev[i1]*m_pfEventStandardDev[i1];
	fNewDev+=m_pfEventStandardDev[i1+1]*m_pfEventStandardDev[i1+1];
	fNewDev+=sqrt(fNewDev/3.f);
	fNewBaseline=(m_pfBaseline[i0]+m_pfBaseline[i1]+m_pfBaseline[i1+1])/float(nNewLength);

	m_pstEventCurrentLevel[i0]=stNewLevel;
	m_pnLevelStart[i0]=nNewStart;
	m_pnLevelLength[i0]=nNewLength;
	m_pfAmplitude[i0]=fNewAmp;
	m_pfEventStandardDev[i0]=fNewDev;
}

void CEvl::MergeToLeftEvent(int i0, int i1)
{
//	int nCurrentLevel=m_pstEventCurrentLevel[i1];
//	int nPreviousLevel=m_pstEventCurrentLevel[i0];
//	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	float fNewAmp=0.f;
	float fNewDev=0.f;
	short stNewLevel=m_pstEventCurrentLevel[i0];
	int nNewLength, nNewStart;
	float fNewBaseline;

	nNewStart=m_pnLevelStart[i0];
	nNewLength=m_pnLevelLength[i0]+m_pnLevelLength[i1];
	fNewAmp=(m_pfAmplitude[i0]*m_pnLevelLength[i0]+m_pfAmplitude[i1]*m_pnLevelLength[i1])/float(nNewLength);
	fNewDev=m_pfEventStandardDev[i0]*m_pfEventStandardDev[i0];
	fNewDev+=m_pfEventStandardDev[i1]*m_pfEventStandardDev[i1];
	fNewDev+=sqrt(fNewDev/2.f);
	fNewBaseline=(m_pfBaseline[i0]+m_pfBaseline[i1])/float(nNewLength);

	m_pstEventCurrentLevel[i0]=stNewLevel;
	m_pnLevelStart[i0]=nNewStart;
	m_pnLevelLength[i0]=nNewLength;
	m_pfAmplitude[i0]=fNewAmp;
	m_pfEventStandardDev[i0]=fNewDev;
}

void CEvl::MergeToRightEvent(int i0, int i1)
{
//	int nCurrentLevel=m_pstEventCurrentLevel[i1];
//	int nPreviousLevel=m_pstEventCurrentLevel[i0];
//	int nNextLevel=m_pstEventCurrentLevel[i1+1];
	float fNewAmp=0.f;
	float fNewDev=0.f;
	short stNewLevel=m_pstEventCurrentLevel[i0];
	int nNewLength, nNewStart;
	float fNewBaseline;

	nNewStart=m_pnLevelStart[i1];
	nNewLength=m_pnLevelLength[i1]+m_pnLevelLength[i1+1];
	fNewAmp=(m_pfAmplitude[i1]*m_pnLevelLength[i1]+m_pfAmplitude[i1+1]*m_pnLevelLength[i1+1])/float(nNewLength);
	fNewDev=m_pfEventStandardDev[i1]*m_pfEventStandardDev[i1];
	fNewDev+=m_pfEventStandardDev[i1+1]*m_pfEventStandardDev[i1+1];
	fNewDev+=sqrt(fNewDev/2.f);
	fNewBaseline=(m_pfBaseline[i1]*m_pnLevelLength[i1]+m_pfBaseline[i1+1]*m_pnLevelLength[i1+1])/float(nNewLength);

	m_pstEventCurrentLevel[i0+1]=stNewLevel;
	m_pnLevelStart[i0+1]=nNewStart;
	m_pnLevelLength[i0+1]=nNewLength;
	m_pfAmplitude[i0+1]=fNewAmp;
	m_pfEventStandardDev[i0+1]=fNewDev;
}

void CEvl::MoveEvent(int i0, int i1)
{
	float fNewAmp=0.f;
	float fNewDev=0.f;
	short stNewLevel=m_pstEventCurrentLevel[i1];
	int nNewLength, nNewStart;
	float fNewBaseline;

	nNewStart=m_pnLevelStart[i1];
	nNewLength=m_pnLevelLength[i1];
	fNewAmp=m_pfAmplitude[i1];
	fNewDev=m_pfEventStandardDev[i1];
	fNewBaseline=m_pfBaseline[i1];

	m_pstEventCurrentLevel[i0]=stNewLevel;
	m_pnLevelStart[i0]=nNewStart;
	m_pnLevelLength[i0]=nNewLength;
	m_pfAmplitude[i0]=fNewAmp;
	m_pfEventStandardDev[i0]=fNewDev;
}

void CEvl::OutputSegment(FILE *fpOutput, CString sFileName, float fTime1, float fTime2, int nChannelNum)
{
	SetChannelNumbers(nChannelNum);
	WriteEvlHead(fpOutput);

	int i,nBegin, nEnd, nFirstEvent, nLastEvent, nTemp, nTemp1, nLength;
	int nFirstLengthAdjust=0, nLastLengthAdjust=0;
	
	nBegin=fTime1*1000000./m_TimePerPoint; //The unit of fTime is Sec, and the unit of m_TimePerPoint is microsecond
	nEnd=fTime2*1000000./m_TimePerPoint; //The unit of fTime is Sec, and the unit of m_TimePerPoint is microsecond
	
	nFirstEvent=0;
	nLastEvent=m_nNumEvents-1;
	float fTime=0.f;
	bool bFirstEvent=false, bLastEvent=false;
	for(i=0;i<m_nNumEvents;i++)
	{
		nTemp=*(m_pnLevelStart+i);
		nLength=*(m_pnLevelLength+i);
		nTemp1=nTemp+nLength;
		if(nTemp<=nBegin&&nTemp1>=nBegin)
		{
			nFirstEvent=i;
			nFirstLengthAdjust=nBegin-nTemp;
			bFirstEvent=true;
		}
		if(nTemp<=nEnd&&nTemp1>=nEnd)
		{
			nLastEvent=i;
			nLastLengthAdjust=nTemp1-nEnd;
			bLastEvent=true;
		}
	}
	CString sMessage="The required segment is out of range: ";
	sMessage+=sFileName;
	if(!bFirstEvent||!bLastEvent)
	{
//		AfxFormatString1(sMessage,-1,sAbfFileName);
		AfxMessageBox(sMessage,MB_OK,0);
	}

	fTime=nTemp*m_TimePerPoint/1000.f;

	WriteEvlBody(fpOutput, nFirstEvent, -*(m_pnLevelStart+nFirstEvent), -nFirstLengthAdjust);
	for(i=nFirstEvent+1;i<nLastEvent;i++)
	{
		WriteEvlBody(fpOutput, i, -nBegin, 0);
	}
	WriteEvlBody(fpOutput, nLastEvent, -nBegin, -nLastLengthAdjust);
}

void CEvl::SetChannelNumbers(int nNumChannels)
{
	m_cNumChannels[10]=nNumChannels+48;
	for(int i=0;i<11;i++)
	{
		m_Comment[117+i]=m_cNumChannels[i];
	}
}

void CEvl::MakeEvlHistogram()
{
	float fTimeMin=0.7f;
//	float fTimeMin=0.01f;  //Only for comparing with the paper of Sigworth
	float fMinLog=-1.1f,fMaxLog=6.f,fMinLinear=0.1f,fMaxLinear=10000.f,fDeltaLog=0.05f,fDeltaLinear=0.2f;
//	float fMinLog=-2.1f,fMaxLog=6.f,fMinLinear=0.005f,fMaxLinear=10000.f,fDeltaLog=0.05f,fDeltaLinear=0.01f;
	int nNumEvents=m_nNumEvents, nNumBinsLinear=0,nNumBinsLog=0;
	int nMaxLevel=m_nMaxLevel;
	int i=0,j=0,nTemp=-1,nTemp1=-1,nIndexLinear=-1,nIndexLog=-1;
	float fTime=0.f;
	const double dLogE=0.43429448190325182765112891891661;
	nNumBinsLinear=int((fMaxLinear-fMinLinear)/fDeltaLinear)+1;
	nNumBinsLog=int((fMaxLog-fMinLog)/fDeltaLog)+1;

	PrepairEvlHistogram();

	float fRecordLength=0.f;
	for(i=0;i<m_nNumEvents;i++)
	{
		nTemp=*(m_pstEventCurrentLevel+i);
		nTemp1=*(m_pnLevelLength+i);
		fTime=nTemp1*m_TimePerPoint/1000.f;

		if(!m_pbExcluded[i])
		{
			if(fTime>fTimeMin) m_structEvlHist.vectfDwellLength.at(nTemp).push_back(fTime);
			nIndexLinear=int((fTime-fMinLinear)/fDeltaLinear);
			nIndexLog=int((dLogE*log(fTime)-fMinLog)/fDeltaLog);

			if(nIndexLinear<nNumBinsLinear)
			{
				m_structEvlHist.pfvectHistLinear.at(nTemp)[nIndexLinear]+=1.f;
			}

			if(nIndexLog<nNumBinsLog)
			{
				m_structEvlHist.pfvectHistLog.at(nTemp)[nIndexLog]+=1.f;
			}
			if(fTime<m_structEvlHist.fRDataRange[nTemp].fLeft)
			{
				m_structEvlHist.fRDataRange[nTemp].fLeft=fTime;
			}
			else if(fTime>m_structEvlHist.fRDataRange[nTemp].fRight)
			{
				m_structEvlHist.fRDataRange[nTemp].fRight=fTime;
			}
		}
		fRecordLength+=fTime;
	}
	m_structEvlHist.fDeltaLinear=fDeltaLinear;
	m_structEvlHist.nNumOfChannels=m_nNumChannels;
	m_structEvlHist.fDeltaLog=fDeltaLog;
	m_structEvlHist.fMaxLinear=fMaxLinear;
	m_structEvlHist.fMaxLog=fMaxLog;
	m_structEvlHist.fMinLinear=fMinLinear;
	m_structEvlHist.fMinLog=fMinLog;
	m_structEvlHist.fRecordLength=fRecordLength;
	m_structEvlHist.nNumBinsLinear=nNumBinsLinear;
	m_structEvlHist.nNumBinsLog=nNumBinsLog;
	m_structEvlHist.nNumEvents=nNumEvents;
	m_structEvlHist.nMaxLevel=nMaxLevel;
	m_bEvlHist=true;
}

void CEvl::ExportHistograms(FILE *fpHist)
{
	if(!m_bEvlHist)
	{
		MakeEvlHistogram();
	}
	int nNumBinsLinear=m_structEvlHist.nNumBinsLinear;
	int nNumBinsLog=m_structEvlHist.nNumBinsLog;
	int nMaxBins=nNumBinsLog;
	if(nMaxBins<nNumBinsLinear)
	{
		nMaxBins=nNumBinsLinear;
	}
	int i=0,j=0;
	int nMaxLevel=m_structEvlHist.nMaxLevel;
	fprintf(fpHist,"   Time(Log)      Time(ms)");
	for(i=0;i<=nMaxLevel;i++)
	{
		fprintf(fpHist,"     Level:%1d",i);
	}

	fprintf(fpHist,"   Time(Lin)");
	for(i=0;i<=nMaxLevel;i++)
	{
		fprintf(fpHist,"     Level:%1d",i);
	}
	fprintf(fpHist,"\n");
	float fTime=0,fMinLinear=m_structEvlHist.fMinLinear,fMinLog=m_structEvlHist.fMinLog;
	float fDeltaLinear=m_structEvlHist.fDeltaLinear, fDeltaLog=m_structEvlHist.fDeltaLog;
	float fTemp=0.f;
	for(i=0;i<nMaxBins;i++)
	{
		if(i<nNumBinsLog)
		{
			fTime=fMinLog+fDeltaLog*float(i);
			fprintf(fpHist,"%12.2f",fTime);
			fTime=exp(log(10.)*fTime);
			fprintf(fpHist,"%14.4f",fTime);
			for(j=0;j<=nMaxLevel;j++)
			{
				fTemp=sqrt(m_structEvlHist.pfvectHistLog.at(j)[i]);
				fprintf(fpHist,"%12.2f",fTemp);
			}
		}
		else
		{
			fTime=fMinLog+fDeltaLog*float(i);
			fprintf(fpHist,"%12.2f",fTime);
			fTime=fMinLog+fDeltaLog*float(i);
			fprintf(fpHist,"%12.4f",fTime);
			for(j=0;j<=nMaxLevel;j++)
			{
				fTemp=0.f;
				fprintf(fpHist,"%12.4f",fTemp);
			}
		}

		if(i<nNumBinsLinear)
		{
			fTime=fMinLinear+fDeltaLinear*float(i);
			fprintf(fpHist,"%12.4f",fTime);
			for(j=0;j<=nMaxLevel;j++)
			{
				fTemp=sqrt(m_structEvlHist.pfvectHistLinear.at(j)[i]);
				fprintf(fpHist,"%12.4f",fTemp);
			}
		}
		else
		{
			fTime=fMinLinear+fDeltaLinear*float(i);
			fprintf(fpHist,"%12.4f",fTime);
			for(j=0;j<=nMaxLevel;j++)
			{
				fTemp=0.f;
				fprintf(fpHist,"%12.4f",fTemp);
			}
		}
		fprintf(fpHist,"\n");
	}
}

structEvlHistogramNode CEvl::GetEvlHistogram()
{
	if(!m_bEvlHist)
	{
		MakeEvlHistogram();
	}
	return m_structEvlHist;
}

int CEvl::MaxLevel()
{
	int nMaxLevel=0;
	int nLevel;
	for(int i=0;i<m_nNumEvents;i++)
	{
		nLevel=*(m_pstEventCurrentLevel+i);
		if(nLevel>nMaxLevel)
		{
			nMaxLevel=nLevel;
		}
	}
	return nMaxLevel;
}

void CEvl::MakeFLogHist(structFRange fFittingRange)
{
	if(!m_bEvlHist)
	{
		MakeEvlHistogram();
	}
	float fMinLog=m_structEvlHist.fMinLog;
	float fMaxLog=m_structEvlHist.fMaxLog;
	float fDeltaLog=m_structEvlHist.fDeltaLog;
	int nMaxLevel=m_nMaxLevel;
	int i=0,j=0;
//	const double dLogE=0.43429448190325182765112891891661;

	int nSize=m_structEvlHist.vectFLogHist.size();

	double fLogTime=0.f;
	double fTemp=0.f;
	double dLog10=log(10.);
	double fLogMin=fFittingRange.fLeft;
	fLogMin=log(fLogMin)/dLog10;
	double dTimeMax=50000.;//upper limit of the histogram.
	double dMaxLog=log(dTimeMax)/dLog10;

	int nBin=0;
	float fEventsInRange=0.f;
	for(i=0;i<=nMaxLevel;i++)
	{
		fEventsInRange=0.f;
		fLogTime=fMinLog;
		nBin=0;
//		nBin=1; //For Sigworth
		while(fLogTime<dMaxLog)
		{
			fLogTime=fMinLog+double(nBin)*fDeltaLog;
			fLogTime=exp(fLogTime*dLog10);
			fLogTime=ClosestSamplePT(fLogTime);
			fLogTime=log(fLogTime)/dLog10;

			fTemp=m_structEvlHist.pfvectHistLog.at(i)[nBin];
			if(fTemp>0.001&&fLogTime>fLogMin)
			{
				m_structEvlHist.vectFLogHist.at(i).push_back(fTemp);
				structFRange aRange;
				aRange.fLeft=fLogTime;
				aRange.fRight=fLogTime+fDeltaLog;
				m_structEvlHist.vectFRLogBins.at(i).push_back(aRange);
				fEventsInRange+=fTemp;
			}
			nBin++;
		}
		m_structEvlHist.nEventsInRange_Log[i]=int(fEventsInRange+0.5);
	}

	double fLeft=0.,fRight=0.,dTemp=0.;
	nSize=0;
	for(i=0;i<=nMaxLevel;i++)
	{
		nSize=m_structEvlHist.vectFLogHist.at(i).size();
		for(j=1;j<nSize;j++)
		{
			fRight=m_structEvlHist.vectFRLogBins.at(i)[j-1].fRight;
			fLeft=m_structEvlHist.vectFRLogBins.at(i)[j].fLeft;
			fLeft=exp(fLeft*dLog10);
			fRight=exp(fRight*dLog10);
			dTemp=0.5*(fLeft+fRight);
//			dTemp=log(dTemp)/dLog10;

//			dTemp=fLeft;
			m_structEvlHist.vectFRLogBins.at(i)[j-1].fRight=dTemp;
			m_structEvlHist.vectFRLogBins.at(i)[j].fLeft=dTemp;
//			m_structEvlHist.vectFRLogBins.at(i)[j-1].fRight=fRight;
//			m_structEvlHist.vectFRLogBins.at(i)[j].fLeft=fLeft;
		}
	}
	m_bLogEvlHist=true;
}

void CEvl::PassingLogHistBinRange(int &nFirstBin, int &nLastBin, int nLevel, structFRange fFittingRange)
{
	nFirstBin=100000000;
	nLastBin=1;
	float fTemp;
	float fLogMin=log(fFittingRange.fLeft)/log(10.f);
	int nNumBinsLog=m_structEvlHist.nNumBinsLog;
	float fMinLog=m_structEvlHist.fMinLog;
	float fDeltaLog=m_structEvlHist.fDeltaLog;
	float fTemp1=0.f;
	for(int i=0;i<nNumBinsLog;i++)
	{
		fTemp=m_structEvlHist.pfvectHistLog.at(nLevel)[i];
		fTemp1=fMinLog+float(i)*fDeltaLog;
		if(fTemp>0.00001f&&fTemp1>fLogMin)
		{
			if(i<nFirstBin) nFirstBin=i;
			if(i>nLastBin) nLastBin=i;
		}
	}
}


namespace EvlGlobal {
	structDwellFittingTrialNode g_TrialNode;
	structEvlHistogramNode g_EvlHistNode;
	structFittingResultNode g_ResultNode;

	double CumulatedDwellProb(double dT[], double dW[], int nComponents, double dTime)
	{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult-=dTemp;
		}

		dResult+=1.;

		return dResult;
	}

	double CumulatedDwellProb_C(double dT[], double dW[], int nComponents, double dTime)
	{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult+=dTemp;
		}
		return dResult;
	}
};

#include "nr.h"
using namespace EvlGlobal;
double LogLikelihood(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.0000000000000000000000000000000000001;
	//Remembr to implement a soft boundary condition later at this point

	int nComponents=nSize/2+1;
	for(i=0;i<nComponents;i++)
	{
		dTemp=x[i];
		if(dTemp<dTiny) return -dBigNegative;
		dTimeConstants[i]=dTemp;
		if(i<nComponents-1)
		{
			dTemp=x[i+nComponents];
			if(dTemp < dTiny) return -dBigNegative;
			dWeights[i]=dTemp;
		}
	}

	dTemp=0.;
	for(i=0;i<nComponents-1;i++)
	{
		dTemp+=dWeights[i];
	}

	if(dTemp>=1.) return -dBigNegative;

	dWeights[nComponents-1]=1.-dTemp;

	int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);

	double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

	for(i=0;i<nNumBins;i++)
	{
		dLeft=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fLeft;
		dRight=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dRight);
		dTemp-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dLeft);
		if(dTemp<dTiny) return -dBigNegative;
		dTemp1=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
		dTemp=dTemp1*log(dTemp/dArea)/dLog10;
		dLogLikelihood+=dTemp;
	}

	if(dArea<dTiny) return -dBigNegative;
//	dLogLikelihood-=nNumBins*log(dArea)/dLog10;
//	dLogLikelihood-=nNumBins*log(dArea)/dLog10;
	
//	FILE* fp=fopen("Track.txt","a");
//	for(i=0;i<nComponents;i++)
//	{
//		fprintf(fp,"  %12.2f",dTimeConstants[i]);
//	}
	
//	for(i=0;i<nComponents;i++)
//	{
//		fprintf(fp,"  %12.2f",dWeights[i]);
//	}

//	fprintf(fp,"  %12.2f\n", dLogLikelihood);

//	fclose(fp);
		

	return -dLogLikelihood;
}

double PartitionFunction(double x[], int nDim)
{
	double dResult=0.f;
	for(int i=0;i<nDim;i++)
	{
		dResult+=exp(-x[i]);
	}
	return dResult;
}

double Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight)
{
	double dArea=0.;
	for(int i=0;i<nDim;i++)
	{
		dArea+=dW[i]*(exp(-dLeft/dT[i])-exp(-dRight/dT[i]));
	}
	return dArea;
}

void TimeConstantsAndWeights(double dT[], double dW[], Vec_I_DP &x, int nDim)
{
	double dG[20];
	double dG0=1.;
	int i;
	dG[0]=dG0;
	for(i=1;i<nDim;i++)
	{
		dG[i]=x[nDim+i-1];
	}

	double dZ=PartitionFunction(dG,nDim);
	for(i=0;i<nDim;i++)
	{
		dT[i]=g_TrialNode.fTimeConstants[i]*exp(x[i]-1);
		dW[i]=exp(-dG[i])/dZ;
	}

	double dTemp=0.;
	for(i=1;i<nDim;i++)
	{
		dTemp+=dW[i];
	}

	dW[0]=1.-dTemp;
}

double LogLikelihood_Transform(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.0000000000000000000000000000000000001;
	//Remembr to implement a soft boundary condition later at this point

	int nComponents=nSize/2+1;
	TimeConstantsAndWeights(dTimeConstants,dWeights,x,nComponents);

	int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);

	//For Sigworth
//	for(i=0;i<nComponents;i++)
//	{
//		if(dTimeConstants[i]<dTmin) 
//		{
//			return -dBigNegative;
//		}
//	}

	double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

	for(i=0;i<nNumBins;i++)
	{
		dLeft=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fLeft;
		dRight=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dRight);
		dTemp-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dLeft);
		if(dTemp<dTiny) return -dBigNegative;
		dTemp1=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
		dTemp=dTemp1*log(dTemp/dArea)/dLog10;
		dLogLikelihood+=dTemp;
	}

	if(dArea<dTiny) return -dBigNegative;
		

	return -dLogLikelihood;
}


double LogLikelihood_HB(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.0000000000000000000000000000000000001;
	//Remembr to implement a soft boundary condition later at this point

	int nComponents=nSize/2+1;
	for(i=0;i<nComponents;i++)
	{
		dTemp=x[i];
		if(dTemp<dTiny) return -dBigNegative;
		dTimeConstants[i]=dTemp;
		if(i<nComponents-1)
		{
			dTemp=x[i+nComponents];
			if(dTemp < dTiny) return -dBigNegative;
			dWeights[i]=dTemp;
		}
	}

	dTemp=0.;
	for(i=0;i<nComponents-1;i++)
	{
		dTemp+=dWeights[i];
	}

	if(dTemp>=1.) return -dBigNegative;

	dWeights[nComponents-1]=1.-dTemp;

	int nNumBins=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);

	double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

	for(i=0;i<nNumBins;i++)
	{
		dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fLeft;
		dRight=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dRight);
		dTemp-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dLeft);
		if(dTemp<dTiny) return -dBigNegative;
		dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel)[i];
		dTemp=dTemp1*log(dTemp/dArea)/dLog10;
		dLogLikelihood+=dTemp;
	}

	if(dArea<dTiny) return -dBigNegative;
	return -dLogLikelihood;
}

double LogLikelihood_HB_Transform(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.0000000000000000000000000000000000001;
	//Remembr to implement a soft boundary condition later at this point

	int nComponents=nSize/2+1;
	TimeConstantsAndWeights(dTimeConstants,dWeights,x,nComponents);

	int nNumBins=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);
//	dArea=1.;

	double dLogLikelihood=0.,dLeft=0.,dRight=0.,dTemp1=0.;

	for(i=0;i<nNumBins;i++)
	{
		dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fLeft;
		dRight=g_EvlHistNode.vectFRLogBins_HB.at(nFittingLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dRight);
		dTemp-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dLeft);
		if(dTemp<dTiny) return -dBigNegative;
		dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nFittingLevel)[i];
		dTemp=dTemp1*log(dTemp/dArea)/dLog10;
		dLogLikelihood+=dTemp;
	}

	if(dArea<dTiny) return -dBigNegative;
	return -dLogLikelihood;
}


double ChiSQ_HB_Transform(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	int nDwellLevel=g_TrialNode.nDwellLevel;
	const double dLog10=log(10.);
	double dT[20],dW[20];
	int i=0;

	int nComponents=x.size()/2+1;
	TimeConstantsAndWeights(dT,dW,x,nComponents);

	int nSize=g_EvlHistNode.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[0].fLeft;
	dMin=exp(dMin*dLog10);
	double dMax=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[nSize-1].fRight;
	dMax=exp(dMax*dLog10);
	double dChiSQ=0.,dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nComponents, dMin, dMax);
	double dEventsInRange=double(g_EvlHistNode.nEventsInRange_HB[nDwellLevel]);
	for(i=0;i<nSize;i++)
	{
		dLeft=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=g_EvlHistNode.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nComponents, dLeft, dRight)/dArea;
		dTemp0*=dEventsInRange;
		dTemp1=g_EvlHistNode.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dChiSQ+=dTemp2*dTemp2/dTemp0;
	}
	return dChiSQ;
}



double LogLikelihood_AP(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	//This function works on all-events instead of on histogram.

	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i,j;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.00000000000000000000000000000001;

	int nComponents=nSize/2+1;
	for(i=0;i<nComponents;i++)
	{
		dTemp=x[i];
		if(dTemp<dTiny) return -dBigNegative;
		dTimeConstants[i]=dTemp;
		if(i<nComponents-1)
		{
			dTemp=x[i+nComponents];
			if(dTemp < 0.) return -dBigNegative;
			dWeights[i]=dTemp;
		}
	}

	dTemp=0.;
	for(i=0;i<nComponents-1;i++)
	{
		dTemp+=dWeights[i];
	}

	if(dTemp>=1.) return -dBigNegative;

	dWeights[nComponents-1]=1.-dTemp;

	int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(dTimeConstants,dWeights,nComponents,dTmin);
	if(dArea<dTiny) return -dBigNegative;		

	nSize=g_EvlHistNode.vectfDwellLength.at(nFittingLevel).size();

	double dLogLikelihood=0.,dTime=0.;

	for(i=0;i<nSize;i++)
	{
		dTemp=0.;
		dTime=g_EvlHistNode.vectfDwellLength.at(nFittingLevel)[i];
		for(j=0;j<nComponents;j++)
		{
			dTemp+=dWeights[j]*exp(-dTime/dTimeConstants[j])/dTimeConstants[j];
		}
		dLogLikelihood+=log(dTemp/dArea);
	}

	return -dLogLikelihood/dLog10;
}


double LogLikelihood_AP_Transform(Vec_I_DP &x)
{
	//This function returns the negative value of the LogLikelihood
	//This function works on all-events instead of on histogram.

	int nFittingLevel=g_TrialNode.nDwellLevel;
	int nSize=x.size();
	double dTimeConstants[20],dWeights[20];
	int i,j;
	double dTemp=0.;
	double dBigNegative=-9999999999999999999999999999999999999.;
	double dTiny=0.00000000000000000000000000000001;

	int nComponents=nSize/2+1;
	TimeConstantsAndWeights(dTimeConstants,dWeights,x,nComponents);

	int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb_C(dTimeConstants,dWeights,nComponents,dTmin);
	if(dArea<dTiny) return -dBigNegative;		

	nSize=g_EvlHistNode.vectfDwellLength.at(nFittingLevel).size();
	double dLogLikelihood=0.,dTime=0.;
	double dW,dTol;
//	double dLestLength=1000000000000000000.;

	for(i=0;i<nSize;i++)
	{
		dTemp=0.;
		dTime=g_EvlHistNode.vectfDwellLength.at(nFittingLevel)[i];
//		if(dLestLength>dTime)
//		{
//			dLestLength=dTime;
//		}
		for(j=0;j<nComponents;j++)
		{
			dW=dWeights[j];
			dTol=dTimeConstants[j];
			dTemp+=(dW*exp(-dTime/dTol))/dTol;
		}
		dLogLikelihood+=log(dTemp/dArea);
	}
	dLogLikelihood=dLogLikelihood/dLog10;

	return -dLogLikelihood;
}

structFittingResultNode CEvl::FitEvl(int nFuncKey, structDwellFittingTrialNode aNode)
{
	// nFuncKey 1=log: logarithmic bin, 2=AP: all events fitting:, 3=HB: hibrid bin
	using namespace EvlGlobal;
	int nComponents=aNode.nNumComponents;

	structFRange fFittingRange;
	fFittingRange=aNode.fFittingRange;

	if(!m_bEvlHist)
	{
		MakeEvlHistogram();
	}
	if(!m_bLogEvlHist)
	{
		MakeFLogHist(fFittingRange);
	}
	if(!m_bHBEvlHist)
	{
		MakeEvlHistogram_HB();
	}

	g_TrialNode=aNode;
	g_EvlHistNode=m_structEvlHist;


	int MP=2*nComponents,NP=2*nComponents-1;
    const DP FTOL=1.0e-10;
    int i,nfunc,j;
    Vec_DP x(NP),y(MP);
    Mat_DP p(MP,NP);
	double temp[40];

	double dTemp;

	for(i=0;i<nComponents;i++)
	{
		dTemp=aNode.fTimeConstants[i];
		temp[i]=dTemp;
		if(i<nComponents-1) 
		{
			dTemp=aNode.fWeights[i];
			temp[i+nComponents]=dTemp;
		}
	}

	for(j=0;j<NP;j++)
	{
		dTemp=temp[j];
		x[j]=temp[j];
		p[0][j]=temp[j];
	}

	switch (nFuncKey)
	{
		case 1:
			y[0]=LogLikelihood(x);
			break;
		case 2:
			y[0]=LogLikelihood_AP(x);
			break;
		case 3:
			y[0]=LogLikelihood_HB(x);
			break;
	}

	for(i=1;i<MP;i++)
	{
		for(j=0;j<NP;j++)
		{
			if(j+1==i)
			{
//				if(j<nComponents-1)
				{
					dTemp=1.1*temp[j];
//					dTemp=1.5*temp[j];
				}
//				else
				{
					dTemp=1.1*temp[j];
//					dTemp=0.;
				}
			}
			else
			{
//				dTemp=temp[j];
				dTemp=0.;
			}
			x[j]=dTemp;
			p[i][j]=dTemp;
		}
		switch (nFuncKey)
		{
			case 1:
				dTemp=LogLikelihood(x);
				y[i]=dTemp;
				break;
			case 2:
				dTemp=LogLikelihood_AP(x);
				y[i]=dTemp;
				break;
			case 3:
				dTemp=LogLikelihood_HB(x);
				y[i]=dTemp;
				break;
		}
	}


	switch (nFuncKey)
	{
		case 1:
			NR::amoeba(p,y,FTOL,LogLikelihood,nfunc);
			break;
		case 2:
			NR::amoeba(p,y,FTOL,LogLikelihood_AP,nfunc);
			break;
		case 3:
			NR::amoeba(p,y,FTOL,LogLikelihood_HB,nfunc);
			break;
	}
	
	structFittingResultNode bNode;

	int nNumComponents=aNode.nNumComponents;

	for(i=0;i<nComponents;i++)
	{
		dTemp=p[0][i];
		bNode.dTimeConstants[i]=dTemp;
		if(i<nComponents-1)
		{
			dTemp=p[0][i+nComponents];
			bNode.dWeights[i]=dTemp;
		}
	}

	dTemp=0.;
	for(i=0;i<nComponents-1;i++)
	{
		dTemp+=bNode.dWeights[i];
	}

	bNode.dWeights[nComponents-1]=1.-dTemp;

	int nFittingLevel=aNode.nDwellLevel;
	int nNumBins=g_EvlHistNode.vectFLogHist.at(nFittingLevel).size();
	double dTmin=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[0].fLeft;
	double dTmax=g_EvlHistNode.vectFRLogBins.at(nFittingLevel)[nNumBins-1].fRight;
	double dLog10=log(10.);
	dTmin=exp(dTmin*dLog10);
	dTmax=exp(dTmax*dLog10);

	double dArea=CumulatedDwellProb(bNode.dTimeConstants,bNode.dWeights,nComponents,dTmax);
	dArea-=CumulatedDwellProb(bNode.dTimeConstants,bNode.dWeights,nComponents,dTmin);

	double dEventsInRange=0;
	for(i=0;i<nNumBins;i++)
	{
		dEventsInRange+=g_EvlHistNode.vectFLogHist.at(nFittingLevel)[i];
	};

	double dMean=0.;
	for(i=0;i<nComponents;i++)
	{
		dMean+=bNode.dTimeConstants[i]*bNode.dWeights[i];
	}

	nFittingLevel=aNode.nDwellLevel;
	for(i=0;i<=nFittingLevel;i++)
	{
		bNode.nLargestHybridBin[i]=m_structEvlHist.nLargestHybridBin[i];
		bNode.nLargestLogBin[i]=m_structEvlHist.nLargestLogBin[i];
	}

	bNode.nFuncKey=nFuncKey;
	bNode.dFittedArea=dArea;
	bNode.fFittingRange.fLeft=dTmin;
	bNode.fFittingRange.fRight=dTmax;
	bNode.dLogLikelihood=-y[0];
	bNode.dMean=dMean;
	bNode.nDwellLevel=nFittingLevel;
	bNode.nEventsInRange=int(dEventsInRange+0.5);
	bNode.nNumComponents=nNumComponents;
	bNode.nNumEvents=m_nNumEvents;
	bNode.nNumIterations=nfunc;
	bNode.sFileName=aNode.sEvlFileName;
	bNode.nTrialNumber=aNode.nTrialNumber;


	return bNode;
}

structFittingResultNode CEvl::FitEvl_Transform(int nFuncKey, structDwellFittingTrialNode aNode)
{
	// nFuncKey 1=log: logarithmic bin, 2=AP: all events fitting:, 3=HB: hibrid bin
	int nComponents=aNode.nNumComponents;

	structFRange fFittingRange;
	fFittingRange=aNode.fFittingRange;

	if(!m_bEvlHist&&nFuncKey==1)
	{
		MakeEvlHistogram();
		MakeFLogHist(fFittingRange);
//		NewBinBoundary();
	}
	if(!m_bLogEvlHist&&nFuncKey==2)
	{
		MakeEvlHistogram();
		MakeFLogHist(fFittingRange);
	}
	if(!m_bHBEvlHist&&nFuncKey==3)
	{
		if(!m_bLogEvlHist)
		{
			MakeFLogHist(fFittingRange);
		}

		if(!m_bHBEvlHist)
		{
			MakeEvlHistogram_HB();
		}
		NewBinBoundary();
	}


	g_TrialNode=aNode;
	g_EvlHistNode=m_structEvlHist;

	CEvlDwellingTimeFitting aFitting;	
	structFittingResultNode bNode=aFitting.FitEvl_Transform(nFuncKey, aNode, m_structEvlHist);

	bNode.nNumEvents=m_nNumEvents;
	Summarize();
	structEvlSummaryNode aEvlSummaryNode=GetSummaryNode();
	bNode.fRecordLength=aEvlSummaryNode.fRecordLength;
	bNode.nNumEvents=aEvlSummaryNode.nNumEvents;
	bNode.nMaxLevel=aEvlSummaryNode.nMaxLevel;
	bNode.nNumChannels=aEvlSummaryNode.nNumChannels;
	bNode.fOpenProb=aEvlSummaryNode.fOpenProb;
	bNode.fMeanOpenTime=aEvlSummaryNode.fMeanOpenTime;
	bNode.fProbSingle=aEvlSummaryNode.fProbSingle;
	bNode.dMeanDwellTimeO=MeanDwellTimeInRange(bNode);
	bNode.dMeanDwellTimeC=MeanFittedDwellTime(bNode);
	bNode.aTrialNode=aNode;


//	ExportDwellCurves(bNode);

	return bNode;
}


double CEvl::CumulatedDwellProb(double dT[], double dW[], int nComponents, double dTime)
{
		double dResult=0.;
		double dTemp;
		for(int i=0;i<nComponents;i++)
		{
			dTemp=exp(-dTime/dT[i]);
			dTemp*=dW[i];
			dResult-=dTemp;
		}

		dResult+=1.;

		return dResult;
}

void CEvl::ExportDwellCurves(structFittingResultNode &aResultNode)
{
		//need nEventsInRange, dFittedArea, dTimeConstants, dWeights of aResultNode

	vector <VectF>  vectFLogHist_HB;
	vector <VectFR> vectFRLogBins_HB;

	vector <VectF>  *pvectFLogHist;
	vector <VectFR> *pvectFRLogBins;


	int nLargestBin;
	int nFittingLevel=aResultNode.nDwellLevel;
	int nFuncKey=aResultNode.nFuncKey;
	switch (nFuncKey)
	{
	case 1:
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 2:
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 3:
		pvectFLogHist=&m_structEvlHist.vectFLogHist_HB;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins_HB;
		break;
	}

//	double dSQRT=sqrt(double(nLargestBin));
	double dSQRT=1.;

	CString sFileName=aResultNode.sFileName;
	sFileName=ExtendFileName(sFileName,'_');
	int nDwellLevel=aResultNode.nDwellLevel;
	int nComponents=aResultNode.nNumComponents;
	int nScoreRanking=aResultNode.nScoreRanking;
	char cTemp;
	cTemp=nDwellLevel+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	cTemp=nComponents+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	sFileName=ExtendFileName(sFileName,'_');
	cTemp=nScoreRanking+48;
	CString sTring=ConvertToString(nScoreRanking);
	sFileName=ExtendFileName(sFileName,sTring);
	sFileName=ChangeExt(sFileName,"Crv");
	aResultNode.sCurFileName=sFileName;
	FILE* fp=fopen(sFileName,"w");

	int j;
	double fTs,fTl;
	double fTemp,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	double fBinWidth=0.05f;

	dX=-1.f;
	fprintf(fp,"  log(t)      time(ms)");
	for(j=0;j<nComponents;j++)
	{
		fprintf(fp,"   Comp:%1d",j+1);
	}
	fprintf(fp,"      Sum");

	for(j=1;j<nComponents;j++)
	{
		fTs=aResultNode.dTimeConstants[j-1];
		fTl=aResultNode.dTimeConstants[j];
//		fTc=BurstDelimiter(fTs,fTl);
//		fprintf(fp,"    Tc%1d:  %10.2f",j+1,fTc);
	}
	fprintf(fp,"          log(t)        time(ms)          Counts");
	fprintf(fp,"\n");

	int nTotalExtimatedEvents=aResultNode.nEventsInRange/aResultNode.dFittedArea;
	int nLine=0;
	int nNumBins=g_EvlHistNode.vectFLogHist.at(nDwellLevel).size();
	float fTime=0.f;
	while(dX<6.)
	{
		dX+=dDelta;
		dX2=dX+fBinWidth;
		dX2=exp(dX2*log(10.));
		dX1=exp(dX*log(10.));
		fprintf(fp,"%8.2f  %12.2f",dX, exp(dX*log(10.)));

		fTemp=0.f;
		for(j=0;j<nComponents;j++)
		{
			fTimeConstant=aResultNode.dTimeConstants[j];
			fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*aResultNode.dWeights[j];
			fprintf(fp," %8.2f",sqrt(fTemp0)/dSQRT);
			fTemp+=fTemp0;
		}
		fprintf(fp," %8.2f",sqrt(fTemp)/dSQRT);

		if(nLine<nNumBins)
		{
			fTemp=m_structEvlHist.pfvectHistLog.at(nDwellLevel)[nLine];
			fTemp0=m_structEvlHist.fMinLog+double(nLine)*m_structEvlHist.fDeltaLog;
			fTime=exp(fTemp0*log(10.));
			fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
		}
		nLine++;
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void CEvl::ExportDwellCurves(structFittingResultNode &aResultNode, bool bEvlRead)
{
		//need nEventsInRange, dFittedArea, dTimeConstants, dWeights of aResultNode

	vector <VectF>  vectFLogHist_HB;
	vector <VectFR> vectFRLogBins_HB;

	vector <VectF>  *pvectFLogHist;
	vector <VectFR> *pvectFRLogBins;


	int nLargestBin;
	int nFittingLevel=aResultNode.nDwellLevel;
	int nFuncKey=aResultNode.nFuncKey;

	if(nFuncKey<1||nFuncKey>3)
		nFuncKey=1;

	if(bEvlRead)
	{
		if(!m_bEvlHist)
		{
			MakeEvlHistogram();
		}
		if(!m_bLogEvlHist)
		{
			MakeFLogHist(aResultNode.fFittingRange);
		}
		if(!m_bHBEvlHist)
		{
			MakeEvlHistogram_HB();
		}
	}

	switch (nFuncKey)
	{
	case 1:
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 2:
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 3:
		pvectFLogHist=&m_structEvlHist.vectFLogHist_HB;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins_HB;
		break;
	}


//	double dSQRT=sqrt(double(nLargestBin));
	double dSQRT=1.;

	CString sFileName=aResultNode.sFileName;
	sFileName=ExtendFileName(sFileName,'_');
	int nDwellLevel=aResultNode.nDwellLevel;
	int nComponents=aResultNode.nNumComponents;
	int nScoreRanking=aResultNode.nScoreRanking;
	char cTemp;
	cTemp=nDwellLevel+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	sFileName=ExtendFileName(sFileName,'_');
	cTemp=nComponents+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	sFileName=ExtendFileName(sFileName,'_');
	cTemp=nScoreRanking+48;
	CString sTring=ConvertToString(nScoreRanking);
	sFileName=ExtendFileName(sFileName,sTring);
	sFileName=ChangeExt(sFileName,"Crv");
	aResultNode.sCurFileName=sFileName;
	FILE* fp=fopen(sFileName,"w");

	int j;
	double fTs,fTl;
	double fTemp,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	double fBinWidth=0.05f;

	dX=-1.f;
	fprintf(fp,"  log(t)      time(ms)");
	for(j=0;j<nComponents;j++)
	{
		fprintf(fp,"     Comp:%1d",j+1);
	}
	fprintf(fp,"      Sum(Sqrt)");
	fprintf(fp,"          log(t)        time(ms)    Counts(Sqrt)");



	fprintf(fp,"  log(t)      time(ms)");
	for(j=0;j<nComponents;j++)
	{
		fprintf(fp,"   Comp:%1d",j+1);
	}
	fprintf(fp,"    Sum(Freq)");	
	fprintf(fp,"          log(t)        time(ms)       Freq(mHz)");


	fprintf(fp,"\n");

	int nTotalExtimatedEvents=aResultNode.nEventsInRange/aResultNode.dFittedArea;
	int nLine=0;
	int nNumLogBins=m_structEvlHist.nNumBinsLog;
	float fTime=0.f;
	float fMinLog=m_structEvlHist.fMinLog;
	float fLength=m_structEvlHist.fRecordLength/1000000.;
	while(dX<6.)
	{
		dX+=dDelta;
		dX2=dX+fBinWidth;
		dX2=exp(dX2*log(10.));
		dX1=exp(dX*log(10.));
		fprintf(fp,"%8.2f  %12.2f",dX, exp(dX*log(10.)));

		fTemp=0.f;
		for(j=0;j<nComponents;j++)
		{
			fTimeConstant=aResultNode.dTimeConstants[j];
			fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*aResultNode.dWeights[j];
			fprintf(fp,"   %8.4f",sqrt(fTemp0)/dSQRT);
			fTemp+=fTemp0;
		}
		fprintf(fp,"   %12.4f",sqrt(fTemp)/dSQRT);

		if(nLine<nNumLogBins&&bEvlRead)
		{
			fTemp=m_structEvlHist.pfvectHistLog.at(nDwellLevel)[nLine];
			if(fTemp < 0.01)
				fTemp=0.01;
			fTemp0=fMinLog+double(nLine)*m_structEvlHist.fDeltaLog;
			fTime=exp(fTemp0*log(10.));
			fprintf(fp,"    %12.3f    %12.2f    %12.4f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
		}
		else
		{
			fTemp0=8.f;
			fTime=10000000.f;
			fTemp=0.01;
			fprintf(fp,"    %12.3f    %12.2f    %12.4f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
		}

//Exporting the "frequency (every 1000 Seconds) of the events, i.e., kHz"

		fprintf(fp,"%8.2f  %12.2f",dX, exp(dX*log(10.)));
		fTemp=0.f;
		for(j=0;j<nComponents;j++)
		{
			fTimeConstant=aResultNode.dTimeConstants[j];
			fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*aResultNode.dWeights[j];
			fprintf(fp," %8.2f",fTemp0/fLength);
			fTemp+=fTemp0;
		}

		fprintf(fp," %12.2f",fTemp/fLength);

		if(nLine<nNumLogBins&&bEvlRead)
		{
			fTemp=m_structEvlHist.pfvectHistLog.at(nDwellLevel)[nLine];
			if(fTemp < 0.01)
				fTemp=0.01;
			fTemp0=fMinLog+double(nLine)*m_structEvlHist.fDeltaLog;
			fTime=exp(fTemp0*log(10.));
			fprintf(fp,"    %12.3f    %12.2f    %12.4f",fTemp0,fTime,fTemp/fLength);
		}
		else
		{
			fTemp0=8.f;
			fTime=10000000.f;
			fTemp=0.01;
			fprintf(fp,"    %12.3f    %12.2f    %12.4f",fTemp0,fTime,fTemp/fLength);
		}
		nLine++;
		fprintf(fp,"\n");

	}
	fclose(fp);
}


void CEvl::ExportDwellCurves_Normalized(structFittingResultNode &aResultNode)
{
	vector <VectF>  vectFLogHist_HB;
	vector <VectFR> vectFRLogBins_HB;

	vector <VectF>  *pvectFLogHist;
	vector <VectFR> *pvectFRLogBins;

	int nLargestBin;
	int nFittingLevel=aResultNode.nDwellLevel;
	int nFuncKey=aResultNode.nFuncKey;
	if(nFuncKey<1||nFuncKey>3)
		nFuncKey=1;

	switch (nFuncKey)
	{
	case 1:
		nLargestBin=aResultNode.nLargestLogBin[nFittingLevel];
		pvectFLogHist=&g_EvlHistNode.vectFLogHist;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins;
		break;
	case 2:
		nLargestBin=aResultNode.nLargestLogBin[nFittingLevel];
		pvectFLogHist=&g_EvlHistNode.vectFLogHist;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins;
		break;
	case 3:
		nLargestBin=aResultNode.nLargestHybridBin[nFittingLevel];
		pvectFLogHist=&g_EvlHistNode.vectFLogHist_HB;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins_HB;
		break;
	}
//	double dSQRT=sqrt(double(nLargestBin));
	double dSQRT=1.;

	CString sFileName=aResultNode.sFileName;
	sFileName=ExtendFileName(sFileName,'_');
	int nDwellLevel=aResultNode.nDwellLevel;
	int nComponents=aResultNode.nNumComponents;
	int nScoreRanking=aResultNode.nScoreRanking;
	char cTemp;
	cTemp=nDwellLevel+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	cTemp=nComponents+48;
	sFileName=ExtendFileName(sFileName,cTemp);
	sFileName=ExtendFileName(sFileName,'_');
	cTemp=nScoreRanking+48;
	CString sTring=ConvertToString(nScoreRanking);
	sFileName=ExtendFileName(sFileName,sTring);
	sFileName=ChangeExt(sFileName,"Crv");
	aResultNode.sCurFileName=sFileName;
	FILE* fp=fopen(sFileName,"w");

	int j;
	double fTs,fTl;
	double fTemp,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	double fBinWidth=0.05f;

	dX=-1.f;
	fprintf(fp,"  log(t)      time(ms)");
	for(j=0;j<nComponents;j++)
	{
		fprintf(fp,"   Comp:%1d",j+1);
	}
	fprintf(fp,"      Sum");

	for(j=1;j<nComponents;j++)
	{
		fTs=aResultNode.dTimeConstants[j-1];
		fTl=aResultNode.dTimeConstants[j];
//		fTc=BurstDelimiter(fTs,fTl);
//		fprintf(fp,"    Tc%1d:  %10.2f",j+1,fTc);
	}
	fprintf(fp,"          log(t)        time(ms)          Counts");
	fprintf(fp,"\n");

	int nTotalExtimatedEvents=aResultNode.nEventsInRange/aResultNode.dFittedArea;
	int nLine=0;
	int nNumBins=g_EvlHistNode.vectFLogHist.at(nDwellLevel).size();
	float fTime=0.f;
	while(dX<6.)
	{
		dX+=dDelta;
		dX2=dX+fBinWidth;
		dX2=exp(dX2*log(10.));
		dX1=exp(dX*log(10.));
		fprintf(fp,"%8.2f  %12.2f",dX, exp(dX*log(10.)));

		fTemp=0.f;
		for(j=0;j<nComponents;j++)
		{
			fTimeConstant=aResultNode.dTimeConstants[j];
			fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*aResultNode.dWeights[j];
			fprintf(fp," %8.2f",sqrt(fTemp0)/dSQRT);
			fTemp+=fTemp0;
		}
		fprintf(fp," %8.2f",sqrt(fTemp)/dSQRT);

		if(nLine<nNumBins)
		{
			fTemp=pvectFLogHist->at(nDwellLevel)[nLine];
			fTime=m_structEvlHist.vectFRLogBins.at(nDwellLevel)[nLine].fLeft;
			fTemp0=log(fTime)/log(10.);
//			if(nLine==0)
//			{
//				fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,0.);
//			}
			fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
//			fTemp0=g_EvlHistNode.vectFRLogBins.at(nDwellLevel)[nLine].fRight;
//			fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
		}
		nLine++;
		fprintf(fp,"\n");
	}
	fclose(fp);
}


void CEvl::ExportDwellCurves(CString sFileName, vector <structFittingResultNode> &avResultNode)
{
	//This function assumes that all result nodes in the vector are for the same dwell level and 
	//that they were obtained using the same hitogram type (nFuncKey)
	bool bRescaleHist=false;
	bool bNormalizeHist=false;
	int nLargestBin;
	int nFuncKey=0;
	vector <VectF>  *pvectFLogHist;
	vector <VectFR> *pvectFRLogBins;

	structFittingResultNode aResultNode;
	int nDwellLevel=aResultNode.nDwellLevel;
	int nComponents=aResultNode.nNumComponents;
	FILE* fp=fopen(sFileName,"w");

	int i,j;
	double fTemp,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	double fBinWidth=0.05f;
	double dTemp;

	int nSize=avResultNode.size();

	for(i=0;i<nSize;i++)
	{
		nComponents=avResultNode.at(i).nNumComponents;
		fprintf(fp," %3dlog(t)   %3dtime(ms)",i,i);
		for(j=0;j<nComponents;j++)
		{
			fprintf(fp,"  %3dComp:%1d",i,j+1);
		}
		fprintf(fp,"   %3dSum",i);

		fprintf(fp,"       %3dlog(t)     %3dtime(ms)       %3dCounts",i,i,i);
	}
	fprintf(fp,"\n");

	int nTotalExtimatedEvents=0;
	int nLine=0;
	int nIndex;
	nDwellLevel=avResultNode.at(0).nDwellLevel;

	nFuncKey=avResultNode.at(0).nFuncKey;
	switch (nFuncKey)
	{
	case 1:
		nLargestBin=avResultNode.at(0).nLargestLogBin[nDwellLevel];
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 2:
		nLargestBin=avResultNode.at(0).nLargestLogBin[nDwellLevel];
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	case 3:
		nLargestBin=avResultNode.at(0).nLargestHybridBin[nDwellLevel];
//		pvectFLogHist=&m_structEvlHist.vectFLogHist_HB;
//		pvectFRLogBins=&m_structEvlHist.vectFRLogBins_HB;
		pvectFLogHist=&m_structEvlHist.vectFLogHist;
		pvectFRLogBins=&m_structEvlHist.vectFRLogBins;
		break;
	}

	double dSQRT;
	if(bNormalizeHist)
	{
		dSQRT=sqrt(double(nLargestBin));
	}
	else
	{
		dSQRT=1.;
	}

	int nNumBins=pvectFLogHist->at(nDwellLevel).size();
	float fTime=0.f;
	dX=-1.f;
	while(dX<6.)
	{
		dX+=dDelta;
		dX2=dX+fBinWidth;
		dX2=exp(dX2*log(10.));
		dX1=exp(dX*log(10.));
		for(int nNodeNumber=0;nNodeNumber<nSize;nNodeNumber++)
		{
			nComponents=avResultNode.at(nNodeNumber).nNumComponents;
			nTotalExtimatedEvents=avResultNode.at(nNodeNumber).nEventsInRange/avResultNode.at(nNodeNumber).dFittedArea;
			fprintf(fp,"%10.5f  %12.2f",dX, exp(dX*log(10.)));

			fTemp=0.f;
			for(j=0;j<nComponents;j++)
			{
				fTimeConstant=avResultNode.at(nNodeNumber).dTimeConstants[j];
				fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*avResultNode.at(nNodeNumber).dWeights[j];
				fprintf(fp,"   %10.4f",sqrt(fTemp0)/dSQRT);
				fTemp+=fTemp0;
			}
			fprintf(fp," %10.6f",sqrt(fTemp)/dSQRT);

			if(nLine<nNumBins)
			{
				fTemp=m_structEvlHist.pfvectHistLog.at(nDwellLevel)[nLine];
				fTemp0=m_structEvlHist.fMinLog+double(nLine)*m_structEvlHist.fDeltaLog;
				fTime=exp(fTemp0*log(10.));
				fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
			}
			else
			{
				fTemp=-1;
				fTemp0=m_structEvlHist.fMinLog+double(nNumBins-1)*m_structEvlHist.fDeltaLog;
				fTime=exp(fTemp0*log(10.));
				fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
			}
			nLine++;
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void CEvl::ExportDwellCurves_Normalized(CString sFileName, vector <structFittingResultNode> &avResultNode)
{
	//This function assumes that all result nodes in the vector are for the same dwell level and 
	//that they were obtained using the same hitogram type (nFuncKey)
	bool bRescaleHist=false;
	bool bNormalizeHist=true;
	int nLargestBin;
	int nFuncKey=0;
	vector <VectF>  *pvectFLogHist;
	vector <VectFR> *pvectFRLogBins;

	structFittingResultNode aResultNode;
	int nDwellLevel=aResultNode.nDwellLevel;
	int nComponents=aResultNode.nNumComponents;
	FILE* fp=fopen(sFileName,"w");

	int i,j;
	double fTemp,fTimeConstant,dX1,dX2,dDelta,dX,fTemp0;
	dDelta=0.01f;
	double fBinWidth=0.05f;
	double dTemp;

	int nSize=avResultNode.size();

	for(i=0;i<nSize;i++)
	{
		nComponents=avResultNode.at(i).nNumComponents;
		fprintf(fp," %3dlog(t)   %3dtime(ms)",i,i);
		for(j=0;j<nComponents;j++)
		{
			fprintf(fp,"  %3dComp:%1d",i,j+1);
		}
		fprintf(fp,"   %3dSum",i);

		fprintf(fp,"       %3dlog(t)     %3dtime(ms)       %3dCounts",i,i,i);
	}
	fprintf(fp,"\n");

	int nTotalExtimatedEvents=0;
	int nLine=0;
	int nIndex;
	nDwellLevel=avResultNode.at(0).nDwellLevel;

	nFuncKey=avResultNode.at(0).nFuncKey;
	switch (nFuncKey)
	{
	case 1:
		nLargestBin=avResultNode.at(0).nLargestLogBin[nDwellLevel];
		pvectFLogHist=&g_EvlHistNode.vectFLogHist;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins;
		break;
	case 2:
		nLargestBin=avResultNode.at(0).nLargestLogBin[nDwellLevel];
		pvectFLogHist=&g_EvlHistNode.vectFLogHist;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins;
		break;
	case 3:
		nLargestBin=avResultNode.at(0).nLargestHybridBin[nDwellLevel];
//		pvectFLogHist=&g_EvlHistNode.vectFLogHist_HB;
//		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins_HB;
		pvectFLogHist=&g_EvlHistNode.vectFLogHist;
		pvectFRLogBins=&g_EvlHistNode.vectFRLogBins;
		break;
	}

	double dSQRT;
	if(bNormalizeHist)
	{
		dSQRT=sqrt(double(nLargestBin));
	}
	else
	{
		dSQRT=1.;
	}

	int nNumBins=pvectFLogHist->at(nDwellLevel).size();
	float fTime=0.f;
	dX=-1.f;
	while(dX<6.)
	{
		dX+=dDelta;
		dX2=dX+fBinWidth;
		dX2=exp(dX2*log(10.));
		dX1=exp(dX*log(10.));
		for(int nNodeNumber=0;nNodeNumber<nSize;nNodeNumber++)
		{
			nComponents=avResultNode.at(nNodeNumber).nNumComponents;
			nTotalExtimatedEvents=avResultNode.at(nNodeNumber).nEventsInRange/avResultNode.at(nNodeNumber).dFittedArea;
			fprintf(fp,"%10.5f  %12.2f",dX, exp(dX*log(10.)));

			fTemp=0.f;
			for(j=0;j<nComponents;j++)
			{
				fTimeConstant=avResultNode.at(nNodeNumber).dTimeConstants[j];
				fTemp0=nTotalExtimatedEvents*(exp(-dX1/fTimeConstant)-exp(-dX2/fTimeConstant))*avResultNode.at(nNodeNumber).dWeights[j];
				fprintf(fp,"   %10.6f",sqrt(fTemp0)/dSQRT);
				fTemp+=fTemp0;
			}
			fprintf(fp," %10.6f",sqrt(fTemp)/dSQRT);

			if(nLine<=2*nNumBins)
			{
				if(nLine==0)
				{
					nIndex=0;
					fTemp0=pvectFRLogBins->at(nDwellLevel)[nIndex].fLeft;
					fTime=exp(fTemp0*log(10.));
					fprintf(fp,"    %12.5f    %12.2f    %12.6f",fTemp0,fTime,0.);
				}
				else
				{
					nIndex=(nLine-1)/2;
					dTemp=pvectFRLogBins->at(nDwellLevel)[nIndex].fRight-fTemp0;
					fTemp=pvectFLogHist->at(nDwellLevel)[nIndex];
					fTime=pvectFRLogBins->at(nDwellLevel)[nIndex].fLeft;
					fTemp0=log(fTime)/log(10.);
					if(bRescaleHist)
					{
						fTemp=fTemp*fBinWidth/dTemp;
					}
					if((nLine%2)==1)
					{
						fprintf(fp,"    %12.5f    %12.2f    %12.6f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
					}
					else
					{
						fTemp0+=dTemp;
						fTime=exp(fTemp0*log(10.));
						fprintf(fp,"    %12.5f    %12.2f    %12.6f",fTemp0,fTime,sqrt(fTemp)/dSQRT);
					}
				}
			}
			else
			{
				if(nLine==2*nNumBins+1)
				{
					nIndex=(nLine-2)/2;
					fTemp0=pvectFRLogBins->at(nDwellLevel)[nIndex].fRight;
					fTime=exp(fTemp0*log(10.));
					fTemp=0.;
					fprintf(fp,"    %12.5f    %12.2f    %12.6f",fTemp0,fTime,fTemp);
				}
				else
				{
					fTemp=-100.;
//					fprintf(fp,"    %12.3f    %12.2f    %12.2f",fTemp,fTemp,fTemp);
					fprintf(fp,"    %12.5f    %12.2f    %12.6f",fTemp,fTemp,fTemp);
				}
			}
		}
		nLine++;
		fprintf(fp,"\n");
	}
	fclose(fp);
}


CString CEvl::ConvertToString(int nInt)
{
	CString sTring;
	char cChar;
	int nQuotient=nInt/10;
	int nRemainder=(nInt%10);
	cChar=nRemainder+48;
	sTring.Insert(0,cChar);
	while(nQuotient>0)
	{
		nRemainder=(nQuotient%10);
		cChar=nRemainder+48;
		sTring.Insert(0,cChar);
		nQuotient/=10;
	}
	return sTring;
}

structFRange CEvl::GetFittingRange(structFRange aRange, int nFittingLevel)
{
	if(!m_bLogEvlHist)
	{
		MakeFLogHist(aRange);
	}
	int nSize=m_structEvlHist.vectFRLogBins.at(nFittingLevel).size();
	aRange.fLeft=exp(m_structEvlHist.vectFRLogBins.at(nFittingLevel)[0].fLeft*log(10.));
//	aRange.fRight=exp(m_structEvlHist.vectFRLogBins.at(nFittingLevel)[nSize-1].fRight*log(10.));
	aRange.fRight=m_TimePerPoint*(m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1])/1000.;;
	return aRange;
}

float CEvl::GetLongestLevelLength(structFRange aRange, int nFittingLevel)
{
	if(!m_bLogEvlHist)
	{
		MakeFLogHist(aRange);
	}
	int nSize=m_structEvlHist.vectFRLogBins.at(nFittingLevel).size();
	float fResult=m_structEvlHist.fRDataRange[nFittingLevel].fRight;
	return fResult;
}



void CEvl::MakeEvlHistogram_HB()
{
	if(!m_bEvlHist)
	{
		MakeEvlHistogram();
	}
	int nLargestHybridBin=0;
	double dLog10=log(10.);
//	float fMinLog=1.2f,fMaxLog=6.f,fMinLinear=0.09f,fMaxLinear=15.99f,fDeltaLog=0.05f,fDeltaLinear=0.2f;//06411
	float fMinLog=1.2f,fMaxLog=6.f,fMinLinear=0.79f,fMaxLinear=15.99f,fDeltaLog=0.05f,fDeltaLinear=0.2f;
//	float fMinLog=-0.3f,fMaxLog=6.f,fMinLinear=0.009f,fMaxLinear=0.509f,fDeltaLog=0.05f,fDeltaLinear=0.01f;
			//To compare with Sigworth's paper, change it back later to the commented line
	double dMinLog0=log(fMaxLinear)/dLog10;
	int nNumBinsLinear=0,nNumBinsLog=0;
	int nMaxLevel=m_nMaxLevel;
	int i=0,j=0,nTemp=-1,nTemp1=-1,nIndexLinear=-1,nIndexLog=-1;
	vector <float*> pfvectHistLinear;
	vector <float*> pfvectHistLog;
	nNumBinsLinear=int((fMaxLinear-fMinLinear)/fDeltaLinear)+1;
	nNumBinsLog=int((fMaxLog-fMinLog)/fDeltaLog+0.5)+1;
	int nSize=m_structEvlHist.vectFLogHist_HB.size();

	for(i=0;i<=m_nMaxLevel;i++)
	{
		float* pfTemp=new float[nNumBinsLinear];
		float* pfTemp1=new float[nNumBinsLog];
		for(j=0;j<nNumBinsLinear;j++)
		{
			pfTemp[j]=0.f;
		}

		for(j=0;j<nNumBinsLog;j++)
		{
			pfTemp1[j]=0.f;
		}
		pfvectHistLinear.push_back(pfTemp);
		pfvectHistLog.push_back(pfTemp1);
	}

	double dLogTime=0.;
	double dTime=0.;
	double dTimeMax=50000.;//upper limit of the histogram.
	double dMaxLog=log(dTimeMax)/dLog10;

	for(i=0;i<m_nNumEvents;i++)
	{
		nTemp=*(m_pstEventCurrentLevel+i);
		nTemp1=*(m_pnLevelLength+i);
		dTime=nTemp1*m_TimePerPoint/1000.;
		dLogTime=log(dTime)/dLog10;
		if(!m_pbExcluded[i])
		{
			if(dTime>=fMinLinear&&dTime<fMaxLinear)
			{
				nIndexLinear=int((dTime-fMinLinear)/fDeltaLinear);
				pfvectHistLinear.at(nTemp)[nIndexLinear]+=1.f;
			}
			else
			if(dLogTime>=fMinLog&&dLogTime<dMaxLog)//events included into the histogram only if shorter than the limit, dTimeMax.
			{
				nIndexLog=int((dLogTime-fMinLog)/fDeltaLog);		
				pfvectHistLog.at(nTemp)[nIndexLog]+=1.f;
			}
			else
			if(dLogTime>=dMinLog0&&dLogTime<fMinLog)
			{
				nIndexLog=0;		
				pfvectHistLog.at(nTemp)[nIndexLog]+=1.f;
			}
		}
	}

	float fTemp=0.f;
	int nSize0=0;
	float fEventsInRange=0.f;
	for(i=0;i<=nMaxLevel;i++)
	{
		nLargestHybridBin=0;
		fEventsInRange=0.f;
		for(j=0;j<nNumBinsLinear;j++)
		{
			fTemp=pfvectHistLinear.at(i)[j];
			if(fTemp>nLargestHybridBin)
			{
				nLargestHybridBin=int(fTemp+0.5);
			}
			if(fTemp>0.001) //this is a non-empty bin
			{
				m_structEvlHist.vectFLogHist_HB.at(i).push_back(fTemp);
				structFRange aRange;
				aRange.fLeft=log(fMinLinear+double(j)*fDeltaLinear)/dLog10;
				aRange.fRight=log(fMinLinear+double(j+1)*fDeltaLinear)/dLog10;
				m_structEvlHist.vectFRLogBins_HB.at(i).push_back(aRange);
				fEventsInRange+=fTemp;
			}
		}
		nSize0=m_structEvlHist.vectFLogHist_HB.at(i).size();
		for(j=0;j<nNumBinsLog;j++)
		{
			fTemp=pfvectHistLog.at(i)[j];
			if(fTemp>nLargestHybridBin)
			{
				nLargestHybridBin=int(fTemp+0.5);
			}
			if(fTemp>0.001)
			{
				m_structEvlHist.vectFLogHist_HB.at(i).push_back(fTemp);
				structFRange aRange;
				aRange.fLeft=fMinLog+double(j)*fDeltaLog;
				aRange.fRight=fMinLog+double(j+1)*fDeltaLog;
				m_structEvlHist.vectFRLogBins_HB.at(i).push_back(aRange);
				nSize=m_structEvlHist.vectFLogHist_HB.at(i).size();
				fEventsInRange+=fTemp;
			}
		}
		m_structEvlHist.nEventsInRange_HB[i]=int(fEventsInRange+0.5);
		m_structEvlHist.vectFRLogBins_HB.at(i)[nSize0].fLeft=dMinLog0;
		m_structEvlHist.nLargestHybridBin[i]=nLargestHybridBin;
	}
	float fLeft=0.f,fRight=0.f;
	nSize=0;
	for(i=0;i<=nMaxLevel;i++)
	{
		nSize=m_structEvlHist.vectFLogHist_HB.at(i).size();
		for(j=1;j<nSize;j++)
		{
			fRight=m_structEvlHist.vectFRLogBins_HB.at(i)[j-1].fRight;
			fLeft=m_structEvlHist.vectFRLogBins_HB.at(i)[j].fLeft;
			fTemp=0.5*(fLeft+fRight);
			m_structEvlHist.vectFRLogBins_HB.at(i)[j-1].fRight=fTemp;
			m_structEvlHist.vectFRLogBins_HB.at(i)[j].fLeft=fTemp;
		}
	}
	m_bHBEvlHist=true;
	//This program has just been checked
}


void CEvl::SimulateEvl(double dT0[], double dW0[], int nNumComponents0)
{
	FILE* fpEvlOut;
	CString sFileName0="Simu.evl";
	CString sFileName="";
	CString sTring="Nothing";
	int nFileSize=56000;
	int nSize=1*nFileSize,i;
	double dT1[1]={10.};
	double dW1[1]={1.};
	int nNumComponents1=1;
	double *pdData0=new double[nSize];
	double *pdData1=new double[nSize];
	Random_Exp(dT0, dW0, nNumComponents0, nSize, pdData0);
	Random_Exp(dT1, dW1, nNumComponents1, nSize, pdData1);
	float fAmp=-1.f;
	short stLevel=0;
	float fDev=0.2f;
	short stNotes=0;
	float fBaseline=0.f;
	int nLevelStart=0,nLevelLength=0;
	short stNumLevels=m_NumLevels;
	m_NumLevels=2;
	float TimePerPoint=m_TimePerPoint;
//	m_TimePerPoint=200;
//	m_TimePerPoint=10;
	double dLength;
	int nRounds=0;
	fpEvlOut=fopen(sFileName0,"wb");
	FILE* fpList=fopen("List.Lst","w");
	for(i=0;i<nSize;i++)
	{
		if((i%nFileSize)==0)
		{
			fclose(fpEvlOut);
			nRounds=i/nFileSize;
			sTring=ConvertToString(nRounds);
			sFileName=ExtendFileName(sFileName0,sTring);
			fpEvlOut=fopen(sFileName,"wb");
			WriteEvlHead(fpEvlOut);
			fprintf(fpList,"EvlFile:  %s  LevelsToFit: 1\nLevels: 0\n",sFileName);
		}

		stLevel=0;
		dLength=pdData0[i];
		nLevelLength=int(dLength*1000./m_TimePerPoint);
		fwrite(&nLevelStart, sizeof(int),1,fpEvlOut);
		nLevelStart+=nLevelLength;
		fwrite(&fAmp, sizeof(float),1,fpEvlOut);
		fwrite(&nLevelLength, sizeof(int),1,fpEvlOut);
		fwrite(&fDev, sizeof(float),1,fpEvlOut);
		fwrite(&stLevel, sizeof(short),1,fpEvlOut);
		fwrite(&stNotes,sizeof(short),1,fpEvlOut);
		if (stLevel==0) 
		{
			fwrite(&fBaseline,sizeof(float),1,fpEvlOut);
		}

		fAmp=2.f;
		stLevel=1;
		dLength=pdData1[i];
		nLevelLength=int(dLength*1000./m_TimePerPoint);
		fwrite(&nLevelStart, sizeof(int),1,fpEvlOut);
		nLevelStart+=nLevelLength;
		fwrite(&fAmp, sizeof(float),1,fpEvlOut);
		fwrite(&nLevelLength, sizeof(int),1,fpEvlOut);
		fwrite(&fDev, sizeof(float),1,fpEvlOut);
		fwrite(&stLevel, sizeof(short),1,fpEvlOut);
		fwrite(&stNotes,sizeof(short),1,fpEvlOut);
		if (stLevel==0) 
		{
			fwrite(&fBaseline,sizeof(float),1,fpEvlOut);
		}

	}
	fclose(fpEvlOut);
	fclose(fpList);
	m_NumLevels=stNumLevels;
	m_TimePerPoint=TimePerPoint;
}

void CEvl::Random_Exp(double dT[], double dW[], int nNumComponents, int nSize, double *pdData)
{
	double dCuW[20]; //Cumulated Weights
	int i,j;
	double dRand0=0.,dTemp=0.,dRand=0.;
	float fTemp;
	for(i=0;i<nNumComponents;i++)
	{
		dTemp+=dW[i];
		dCuW[i]=dTemp;
	}
	int nSeed=-1;
	fTemp=NR::ran1(nSeed);

	for(i=0;i<nSize;i++)
	{
		dRand0=NR::ran1(nSeed);
		dRand=NR::ran1(nSeed);
		for(j=0;j<nNumComponents;j++)
		{
			if(dRand0<=dCuW[j])
			{
				fTemp=-log(dRand)*dT[j];
				pdData[i]=fTemp;
				break;
			}
		}
	}
}

void CEvl::NewBinBoundary()
{
	int nMaxLevel=m_nMaxLevel;
	int nSize=0;
	int i,j;
	double dLog10=log(10.);
	float fLeft=0.f,fTemp=0.f;

	for(i=0;i<=nMaxLevel;i++)
	{
		fLeft=m_structEvlHist.vectFRLogBins_HB.at(i)[0].fLeft;
		fLeft=exp(fLeft*dLog10);
		fTemp=ClosestSamplePT(fLeft);
		fTemp=log(fTemp)/dLog10;
		m_structEvlHist.vectFRLogBins_HB.at(i)[0].fLeft=fTemp;

		nSize=m_structEvlHist.vectFLogHist_HB.at(i).size();
		for(j=1;j<nSize;j++)
		{
			fLeft=m_structEvlHist.vectFRLogBins_HB.at(i)[j].fLeft;
			fLeft=exp(fLeft*dLog10);
			fTemp=ClosestSamplePT(fLeft);
			fTemp=log(fTemp)/dLog10;
			m_structEvlHist.vectFRLogBins_HB.at(i)[j-1].fRight=fTemp;
			m_structEvlHist.vectFRLogBins_HB.at(i)[j].fLeft=fTemp;
		}
		fTemp=m_structEvlHist.fRecordLength;
		m_structEvlHist.vectFRLogBins_HB.at(i)[nSize-1].fRight=log(fTemp)/dLog10;
	}

	for(i=0;i<=nMaxLevel;i++)
	{
		fLeft=m_structEvlHist.vectFRLogBins.at(i)[0].fLeft;
		fLeft=exp(fLeft*dLog10);
		fTemp=ClosestSamplePT(fLeft);
		fTemp=log(fTemp)/dLog10;
		m_structEvlHist.vectFRLogBins.at(i)[0].fLeft=fTemp;

		nSize=m_structEvlHist.vectFLogHist.at(i).size();
		for(j=1;j<nSize;j++)
		{
			fLeft=m_structEvlHist.vectFRLogBins.at(i)[j].fLeft;
			fLeft=exp(fLeft*dLog10);
			fTemp=ClosestSamplePT(fLeft);
			fTemp=log(fTemp)/dLog10;
			m_structEvlHist.vectFRLogBins.at(i)[j-1].fRight=fTemp;
			m_structEvlHist.vectFRLogBins.at(i)[j].fLeft=fTemp;
		}
		fTemp=m_structEvlHist.fRecordLength;
		m_structEvlHist.vectFRLogBins.at(i)[nSize-1].fRight=log(fTemp)/dLog10;
	}

}

float CEvl::ClosestSamplePT(float fTime)
{
	fTime*=1000.;
	fTime=m_TimePerPoint*double(int(fTime/m_TimePerPoint)+1);
	fTime/=1000.;
	return fTime;
}

double CEvl::SSE(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dSSE=0.,dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dSSE+=dTemp2*dTemp2;
	}
	return dSSE;
}

double CEvl::ChiSQ(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dChiSQ=0.,dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dChiSQ+=dTemp2*dTemp2/dTemp0;
	}
	return dChiSQ;
}

void CEvl::MeanDwellTimeHist(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, double& dMeanH, double& dMeanHC, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dTemp0=0.,dTemp1=0.;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	double dLog10=log(10.);
	int i;
	dMeanH=0.;
	dMeanHC=0.;
	double dTotal=0.;
	double dTotalC=0.;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTotal+=dTemp1;
		dTotalC+=dTemp0;

		dMeanH+=dTemp1*0.5*(dLeft+dRight);
		dMeanHC+=dTemp0*0.5*(dLeft+dRight);
	}
	dMeanH/=dTotal;
	dMeanHC/=dTotalC;
}

double CEvl::SDev(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange)
{
	int nSize=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel).size();
	double dLog10=log(10.);
	double dMin=fRange.fLeft;
	double dMax=fRange.fRight;
	double dTemp0=0.,dTemp1=0.,dTemp2;
	double dLeft,dRight;
	double dArea=Area_Exp(dT, dW, nDim, dMin, dMax);
	vector <double> vectdDev;
	int i;
	double dMeanDev=0.;
	for(i=0;i<nSize;i++)
	{
		dLeft=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fLeft;
		dRight=m_structEvlHist.vectFRLogBins_HB.at(nDwellLevel)[i].fRight;
		dLeft=exp(dLeft*dLog10);
		dRight=exp(dRight*dLog10);
		dTemp0=Area_Exp(dT, dW, nDim, dLeft, dRight)/dArea;
		dTemp0*=double(nEventsInRange);
		dTemp1=m_structEvlHist.vectFLogHist_HB.at(nDwellLevel)[i];
		dTemp2=dTemp1-dTemp0;
		dMeanDev+=dTemp2;
		vectdDev.push_back(dTemp2);
	}

	dMeanDev/=double(nSize);

	double dSDev=0.;

	for(i=0;i<nSize;i++)
	{
		dTemp0=vectdDev[i]-dMeanDev;
		dSDev+=dTemp0*dTemp0;
	}

	dSDev=sqrt(dSDev/double(nSize));

	return dSDev;
}

double CEvl::Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight)
{
	double dArea=0.;
	for(int i=0;i<nDim;i++)
	{
		dArea+=dW[i]*(exp(-dLeft/dT[i])-exp(-dRight/dT[i]));
	}
	return dArea;
}

int CEvl::EventsInRange(int nDwellLevel)
{
	int nSize=m_structEvlHist.vectFLogHist.at(nDwellLevel).size();
	double dTotal=0.;
	for(int i=0;i<nSize;i++)
	{		
		dTotal+=m_structEvlHist.vectFLogHist.at(nDwellLevel)[i];
	}
	return int(dTotal+0.5);
}

double CEvl::MeanFittedDwellTime(structFittingResultNode aResultNode)
{
	double dTmin=aResultNode.fFittingRange.fLeft;
	double dBigTime=1.e99;
	double *dT=aResultNode.dTimeConstants;
	double *dW=aResultNode.dWeights;
	int nDim=aResultNode.nNumComponents;
	double dArea=Area_Exp(dT, dW, nDim, dTmin, dBigTime);

	double dMean=0.;
	for(int i=0;i<nDim;i++)
	{
		dMean+=dT[i]*dW[i]*exp(-dTmin/dT[i]);
	}
	dMean/=dArea;
	dMean+=dTmin;
	return dMean;
}

double CEvl::MeanDwellTimeInRange(structFittingResultNode aResultNode)
{
	double dTmin=aResultNode.fFittingRange.fLeft;
	double dMean=0.;
	double dTime=0.;
	int nDwellLevel=aResultNode.nDwellLevel;
	int nNum=0,i,nLength=0,nLevel=0;
	for(i=0;i<m_nNumEvents;i++)
	{
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==nDwellLevel)
		{
			nLength=m_pnLevelLength[i];
			dTime=nLength*m_TimePerPoint/1000.;
			if(dTime>dTmin)
			{
				nNum++;
				dMean+=dTime;
			}
		}
	}
	return dMean/double(nNum);
}

void CEvl::DoBurstingTimeCourse()
{
	int nBinSize,nMaxOpening,nMinOpening,nSortingIndex,nLatancy;
	float fCutoff0,fCutoff1,fLatancy;
	CBurstingTimeCourseOptionDlg aDlg;
	float fBinWidth;
	structBurstingTimeCourseOptionNode aNode;
	if(aDlg.DoModal()==IDOK)
	{
//		aDlg.GetBurstingTimeCourseOptions(fCutoff0,fCutoff1,nBinSize,fBinWidth,nMaxOpening,nMinOpening,nSortingIndex,fLatancy,nLatancy);
		aDlg.getBurstTimecourseOptions(aNode);
	}
	float fBinSize=-float(aNode.nBinSize);
//	IgnoreShortClosing(fCutoff0);  //09417 I commented this line out on 09417. I don't know why I had this line. 
	IgnoreShortClosing(aNode.fBurstDelimiter);  //09417 This line is required to make burst time course. bad initial design. 
	CEvlTimeCourse cTC(m_sEvlFileName,fBinSize,m_TimePerPoint,m_nNumEvents,
		m_pnLevelLength,m_pnLevelStart,m_pstEventCurrentLevel, m_pfEffLevel, m_pnMRGEvents);
	cTC.DoBurstingTimeCourse();
}

void CEvl::Export2DEvlHistogram()
{
	CEvl2DHistogram a2DHist;
	filteringEvents();
	a2DHist.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,m_sEvlFileName,m_pbExcluded);
	a2DHist.Make2DHist();
	a2DHist.Export2DEvlHistogram();
}


void CEvl::FilteringEvents()
{
	int i,j;
	float fTime;
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
			fTime0=nLength0*m_TimePerPoint/1000.f;
			fTime1=nLength1*m_TimePerPoint/1000.f;
			fTime2=nLength2*m_TimePerPoint/1000.f;
			if(fTime0>31.88&&fTime0<808.66&&fTime2<31.88)
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
			fTime0=nLength0*m_TimePerPoint/1000.f;
			fTime1=nLength1*m_TimePerPoint/1000.f;
			fTime2=nLength2*m_TimePerPoint/1000.f;
			if(fTime0>7.&&fTime2>7.)
			{
				m_pbExcluded[i]=false;
			}
		}
	}
}

void CEvl::FilteringEvents1()
{
	int i,j;
	float fTime;
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
		if(nLevel1==0)
		{
			nLevel0=*(m_pstEventCurrentLevel+i-1);
			nLevel2=*(m_pstEventCurrentLevel+i+1);
			nLength0=*(m_pnLevelLength+i-1);
			nLength1=*(m_pnLevelLength+i);
			nLength2=*(m_pnLevelLength+i+1);
			fTime0=nLength0*m_TimePerPoint/1000.f;
			fTime1=nLength1*m_TimePerPoint/1000.f;
			fTime2=nLength2*m_TimePerPoint/1000.f;
//			if(fTime0>10.&&fTime2>10.)
			if(fTime0>15.)
			{
				m_pbExcluded[i]=false;
			}
		}
	}
}

void CEvl::ClearvectFRLogBins()
{
	int nSize,nSize1,i,j;
	nSize=m_structEvlHist.vectFRLogBins.size();
	if(nSize>0)
	{
		for(i=0;i<nSize;i++)
		{
			nSize1=m_structEvlHist.vectFRLogBins[i].size();
//			if(nSize1>0)
//			{
				m_structEvlHist.vectFRLogBins[i].clear();
//			}
		}
	}
}

void CEvl::PrepairEvlHistogram()
{
	float fTimeMin=0.7f;
//	float fTimeMin=0.01f;  //Only for comparing with the paper of Sigworth
	float fMinLog=-1.1f,fMaxLog=6.f,fMinLinear=0.1f,fMaxLinear=10000.f,fDeltaLog=0.05f,fDeltaLinear=0.2f;
//	float fMinLog=-2.1f,fMaxLog=6.f,fMinLinear=0.005f,fMaxLinear=10000.f,fDeltaLog=0.05f,fDeltaLinear=0.01f;
	int nNumEvents=m_nNumEvents, nNumBinsLinear=0,nNumBinsLog=0;
	int i=0,j=0,nTemp=-1,nTemp1=-1,nIndexLinear=-1,nIndexLog=-1;
	float fTime=0.f;
	const double dLogE=0.43429448190325182765112891891661;
	nNumBinsLinear=int((fMaxLinear-fMinLinear)/fDeltaLinear)+1;
	nNumBinsLog=int((fMaxLog-fMinLog)/fDeltaLog)+1;
	
	int nSize;

//	m_structEvlHist.vectfDwellLength.clear();
//	m_structEvlHist.pfvectHistLinear.clear();
//	m_structEvlHist.pfvectHistLog.clear();
	
	nSize=m_structEvlHist.pfvectHistLinear.size();
	for(i=0;i<=m_nMaxLevel;i++)
	{
		if(i<nSize)
		{
			delete m_structEvlHist.pfvectHistLinear[i];
			m_structEvlHist.pfvectHistLinear[i]=new float[nNumBinsLinear];
			delete m_structEvlHist.pfvectHistLog[i];
			m_structEvlHist.pfvectHistLog[i]=new float[nNumBinsLog];
			m_structEvlHist.vectfDwellLength[i].clear();
		}
		else
		{
			float* pfTemp=new float[nNumBinsLinear];
			float* pfTemp1=new float[nNumBinsLog];
			m_structEvlHist.pfvectHistLinear.push_back(pfTemp);
			m_structEvlHist.pfvectHistLog.push_back(pfTemp1);
			vector <float> vectfDwell;
			m_structEvlHist.vectfDwellLength.push_back(vectfDwell);
		}

		for(j=0;j<nNumBinsLinear;j++)
		{
			m_structEvlHist.pfvectHistLinear.at(i)[j]=0.f;
		}

		for(j=0;j<nNumBinsLog;j++)
		{
			m_structEvlHist.pfvectHistLog.at(i)[j]=0.f;
		}
	}

//	m_structEvlHist.vectFLogHist_HB.clear();
//	m_structEvlHist.vectFRLogBins_HB.clear();
//  clear the histogram made for the previous evl file.

	int nSize1=0;
	nSize=m_structEvlHist.vectFLogHist_HB.size();
	for(i=0;i<=m_nMaxLevel;i++)
	{
		if(i<nSize)
		{
			m_structEvlHist.vectFLogHist_HB[i].clear();
			m_structEvlHist.vectFRLogBins_HB[i].clear();
		}
		else
		{
			vector <float> fVector;
			vector <structFRange> frVector;
			m_structEvlHist.vectFLogHist_HB.push_back(fVector);
			m_structEvlHist.vectFRLogBins_HB.push_back(frVector);
		}
	}


//	m_structEvlHist.vectFLogHist.clear();
//	ClearvectFRLogBins();

	nSize=m_structEvlHist.vectFLogHist.size();
	for(i=0;i<=m_nMaxLevel;i++)
	{
		if(i<nSize)
		{
			m_structEvlHist.vectFLogHist[i].clear();
			m_structEvlHist.vectFRLogBins[i].clear();
		}
		else
		{
			vector <float> fVector;
			vector <structFRange> frVector;
			m_structEvlHist.vectFLogHist.push_back(fVector);
			m_structEvlHist.vectFRLogBins.push_back(frVector);
		}
	}

	for(i=0;i<=m_nMaxLevel;i++)
	{
		m_structEvlHist.fRDataRange[i].fLeft=9999999999.f;
		m_structEvlHist.fRDataRange[i].fRight=-9999999999.f;
	}
}

void CEvl::ExportBurstStatitics()
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	aBH.ExportBurstStatitics();
}

CBurstHandling CEvl::GetBurstHandlingObject()
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	return aBH;
}


AbfNode CEvl::GetDwellingTimeAsTrace(float fMaxAmp)
{
	int nDataSize,i,j,nPosition;
	nDataSize=m_nRecordLength_Formal;
	AbfNode aNode;
	aNode.nSize=nDataSize;
	float* pfTemp=new float[nDataSize];

	nPosition=0;
	int nTemp;
	int nLength;
	float fTime;
	for(i=1;i<m_nNumEvents;i++)
	{
		nTemp=m_pstEventCurrentLevel[i-1];
		nLength=m_pnLevelLength[i-1];
		fTime=float(nLength*m_TimePerPoint/1000.);
		if(fabs(fTime)>fabs(fMaxAmp))
		{
			fTime=fabs(fMaxAmp);
		}
		if(nTemp<0||nTemp>m_NumLevels-1)
		{
			nTemp=nTemp;
		}
		for(j=m_pnLevelStart[i-1];j<m_pnLevelStart[i];j++)
		{
			if(nTemp==0)
			{
				pfTemp[nPosition]=fTime;
			}
			else if(nTemp>0)
			{
				pfTemp[nPosition]=-fTime;
			}
			else
			{
				pfTemp[nPosition]=0.f;
			}
			nPosition++;
		}
	}

	nTemp=m_pstEventCurrentLevel[m_nNumEvents-1];
	nLength=m_pnLevelLength[m_nNumEvents-1];

	for(j=0;j<nLength;j++)
	{
		if(nTemp==0)
		{
			pfTemp[nPosition]=fTime;
		}
		else if(nTemp>0)
		{
			pfTemp[nPosition]=-fTime;
		}
		else
		{
			pfTemp[nPosition]=0.f;
		}
		nPosition++;
	}


	aNode.pfData=pfTemp;
	aNode.nSize=nPosition;
	return aNode;
}

AbfNode CEvl::GetAmpAsTrace()
{
	int nDataSize,i,j,nPosition;
	nDataSize=m_nRecordLength_Formal;
	AbfNode aNode;
	aNode.nSize=nDataSize;
	float* pfTemp=new float[nDataSize];

	nPosition=0;
	int nTemp;
	int nLength;
	float fTime,fAmp;
	for(i=1;i<m_nNumEvents;i++)
	{
		fAmp=m_pfAmplitude[i-1];
		for(j=m_pnLevelStart[i-1];j<m_pnLevelStart[i];j++)
		{
			if(nPosition <nDataSize)
			{
				pfTemp[nPosition]=fAmp;
				nPosition++;
			}
		}
	}
	
	nLength=m_pnLevelLength[m_nNumEvents-1];
	fAmp=m_pfAmplitude[m_nNumEvents-1];
	for(j=0;j<nLength;j++)
	{
		if(nPosition <nDataSize)
		{
			pfTemp[nPosition]=fAmp;
			nPosition++;
		}
	}
	aNode.pfData=pfTemp;
	aNode.nSize=nPosition;
	return aNode;
}

void CEvl::ExcludeSubconductances()
{
	float fAmp,fAmp0,fAmp1;
	CSubconductanceDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		aDlg.GetAmps(fAmp0,fAmp1);
		for(int i=0;i<m_nNumEvents;i++)
		{
			fAmp=m_pfAmplitude[i];
			if(fAmp>fAmp0&&fAmp<fAmp1)
			{
				m_pbExcluded[i]=true;
			}
		}
	}
}
void CEvl::FilteringEvents(int &nKey, float &fBurstDelimeter, float &fClusterDelimeter)
{
	structBurstingTimeCourseOptionNode aNode;
	aNode.fBurstDelimiter=fBurstDelimeter;
	aNode.fClusterDelimiter=fClusterDelimeter;
	FilteringEvents(nKey,aNode);
}
void CEvl::FilteringEvents(int &nKey, structBurstingTimeCourseOptionNode aNode)
{
	float fBurstDelimiter=aNode.fBurstDelimiter, fClusterDelimiter=aNode.fClusterDelimiter;
	switch (nKey)
	{ 
	case 1:
		ExcludeSubconductances();
		break;

	case 2:
		{CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickInterBurstIntervals(m_pbExcluded);}
		break;

	case 3:
		PickSubconductance(0);
		break;

	case 4:
		{int nNOPB1=30;
		int nNOPB2=500000;
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickEvents_NOPB(nNOPB1,nNOPB2,m_pbExcluded);}
		//This function picks openings in burst with NOPB in the range of [nNOPB1,nNOPB2].
		break;

	case 5:
		{int nEdge=10;
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickIntraBust(nEdge,m_pbExcluded);}
		break;

	case 6:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickFirstOpenings(m_pbExcluded);}
		break;

	case 7:
		{int nNOPB1=1;
		int nNOPB2=1;
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickEvents_NOPB(nNOPB1,nNOPB2,m_pbExcluded);}
		//This function picks openings in burst with NOPB in the range of [nNOPB1,nNOPB2].
		break;

	case 8:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickLastOpenings(m_pbExcluded);}
		break;

	case 9:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickIsolatedOpenings(m_pbExcluded);}
		break;

	case 10:
		{
		ReverseOpenAndClosed();
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickIsolatedOpenings(m_pbExcluded);
		ReverseOpenAndClosed();}
		break;

	case 11:
		{
		ReverseOpenAndClosed();
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.PickFirstOpenings(m_pbExcluded);
		ReverseOpenAndClosed();}
		break;
	case 12:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.pickIntraburstEvents(m_pbExcluded,aNode.nLatancy);}
		break;
	case 13:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.pickIntraburstEvents(m_pbExcluded,aNode.fLatancy);}
		break;
	case 14:
		{
		CBurstHandling aBH;
		aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
		aBH.MakeBurstNodes(aNode);
		aBH.pickSubConductance(m_pbExcluded,aNode.nSubConductanceLevelI,aNode.nSubConductanceLevelF);}
		break;
	default:
		//Do nothing;
		nKey=0;
		break;
	}
}

void CEvl::PickSubconductance(int nKey)
{
	float fAmp,fAmp0,fAmp1;
	int nLevel;
	CSubconductanceDlg aDlg;
	ExcludeAllEvents();
	if(aDlg.DoModal()==IDOK)
	{
		aDlg.GetAmps(fAmp0,fAmp1);
		for(int i=0;i<m_nNumEvents;i++)
		{
			fAmp=m_pfAmplitude[i];
			nLevel=m_pstEventCurrentLevel[i];
			if(nLevel==1)
			{
				if(fAmp>fAmp0&&fAmp<fAmp1)
				{
					m_pbExcluded[i]=false;
					if(i>0) m_pbExcluded[i-1]=false;
					if(i<m_nNumEvents-1) m_pbExcluded[i+1]=false;
				}
			}
		}
	}
}

void CEvl::ExcludeAllEvents()
{
	for(int i=0;i<m_nNumEvents;i++)
	{
		m_pbExcluded[i]=true;
	}
}

void CEvl::SetConfidanceRange(int nFuncKey, vector <structFittingResultNode> &avResultNodes)
{
	// nFuncKey 1=log: logarithmic bin, 2=AP: all events fitting:, 3=HB: hibrid bin
	int nConfidanceMode=1;
	// nConfidanceMode==0 --> The confidance range will be set without readjusting other parameters (non refitting)
	int nSigComponents=avResultNodes[0].nSignificantComponents;
	int nComponents=avResultNodes[0].aTrialNode.nNumComponents;

	structFRange fFittingRange;
	fFittingRange=avResultNodes[0].aTrialNode.fFittingRange;

	if(!m_bEvlHist&&nFuncKey==1)
	{
		MakeEvlHistogram();
		MakeFLogHist(fFittingRange);
//		NewBinBoundary();
	}
	if(!m_bLogEvlHist&&nFuncKey==2)
	{
		MakeEvlHistogram();
		MakeFLogHist(fFittingRange);
	}
	if(!m_bHBEvlHist&&nFuncKey==3)
	{
		if(!m_bLogEvlHist)
		{
			MakeFLogHist(fFittingRange);
		}

		if(!m_bHBEvlHist)
		{
			MakeEvlHistogram_HB();
		}
		NewBinBoundary();
	}

	structFittingResultNode aResultNode;
	CEvlDwellingTimeFitting aFitting;
	int nSize=avResultNodes.size();

	double dTol=0.1;

	if(nSigComponents==nComponents)
	{
		dTol=2.1;
	}

	double dLogLik_R;
	int nScoreIndex,nClusterIndex;
//	nSize=1; //Temporally
	for(int i=0;i<nSize;i++)
	{
		aResultNode=avResultNodes.at(i);
		dLogLik_R=aResultNode.dLogLikelihood_R;
		nClusterIndex=aResultNode.nClusterIndex;
		nScoreIndex=aResultNode.nScoreRanking;
		if(fabs(dLogLik_R)<dTol&&nClusterIndex==nScoreIndex)
		{
			nConfidanceMode=1;
		}
		else
		{
			nConfidanceMode=0;
		}
		aFitting.FitEvl_ConfidanceRange(nFuncKey, avResultNodes.at(i), m_structEvlHist, nConfidanceMode);
	}
}

void CEvl::FitNOPB()
{
	//This function fit and output number of openings per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	FILE* fpTotal=NULL;
	aBH.FitNumOpeningPerBurst(fpTotal);
}

void CEvl::FitNOPB(float fCutoff0, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal)
{
	//This function fit and output number of openings per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	structBurstingTimeCourseOptionNode aNode=aBH.getInitializedParameters();
	aNode.fBurstDelimiter=fCutoff0;
	aNode.nBinSize=nBinSize;
	aNode.nMaxOpening=nMaxOpening;
	aNode.nMinOpening=nMinOpening;
	aNode.nSortingIndex=nIndex;
	aNode.bSet=true;
	aBH.MakeBurstNodes(aNode);
	if(aBH.GetNumBursts()>2)
		aBH.FitNumOpeningPerBurst(fpTotal);
}

void CEvl::FitTOTPB(float fCutoff0, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal, int nFuncKey)
{
	//This function fit and output total open time per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	structBurstingTimeCourseOptionNode aNode=aBH.getInitializedParameters();
	aNode.fBurstDelimiter=fCutoff0;
	aNode.nBinSize=nBinSize;
	aNode.nMaxOpening=nMaxOpening;
	aNode.nMinOpening=nMinOpening;
	aNode.nSortingIndex=nIndex;
	aNode.bSet=true;
	aBH.MakeBurstNodes(aNode);
	if(aBH.GetNumBursts()>2)
			aBH.FitTotalOpentimePerBurst(fpTotal,nFuncKey);
}


void CEvl::FitBST(float fCutoff0, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal, int nFuncKey)
{
	//This function fit and output total open time per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	structBurstingTimeCourseOptionNode aNode=aBH.getInitializedParameters();
	aNode.fBurstDelimiter=fCutoff0;
	aNode.nBinSize=nBinSize;
	aNode.nMaxOpening=nMaxOpening;
	aNode.nMinOpening=nMinOpening;
	aNode.nSortingIndex=nIndex;
	aNode.bSet=true;
	aBH.MakeBurstNodes(aNode);
	if(aBH.GetNumBursts()>2)
		aBH.FitBurstDurations(fpTotal,nFuncKey);
}

void CEvl::FitTOTPB(FILE* fpTotal, int nFuncKey)
{
	//This function fit and output total open time per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	aBH.MakeBurstNodes();
	if(aBH.GetNumBursts()>2)
		aBH.FitTotalOpentimePerBurst(fpTotal,nFuncKey);
}


void CEvl::FitBST(FILE* fpTotal, int nFuncKey)
{
	//This function fit and output total open time per burst (NOPB) of the current evl.
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	aBH.MakeBurstNodes();
	if(aBH.GetNumBursts()>2)
		aBH.FitBurstDurations(fpTotal,nFuncKey);
}

void CEvl::UpdateInterval(float fTimePerPoint)
{
	m_TimePerPoint=fTimePerPoint;
}
void CEvl::UpdateEvents(structEventListNode aNode)
{
	CString sEvlFileName=aNode.sFileName;
	float fTimePerPoint=aNode.fTimePerPoint;
	m_nNumChannels=aNode.nNumChannels;
	int nNumLevels=aNode.nNumLevels;

	m_TimePerPoint=fTimePerPoint;
	m_bHBEvlHist=false;
	m_bLogEvlHist=false;
	m_bEvlHist=false;

	m_sEvlFileName=sEvlFileName;

	m_nNumEvents=aNode.vstructEventList.size();

	PrepairPointers();
	m_NumLevels=nNumLevels;

	m_nRecordLength_Actual=0;

	for(int i=m_nNumEvents0;i<m_nNumEvents;i++)
	{
		m_pnLevelStart[i]=aNode.vstructEventList[i].LevelStart;
		m_pfAmplitude[i]=aNode.vstructEventList[i].fAmpMean;
		m_pnLevelLength[i]=aNode.vstructEventList[i].LevelLength;
		m_pfEventStandardDev[i]=aNode.vstructEventList[i].fAmpDev;
		m_pstEventCurrentLevel[i]=aNode.vstructEventList[i].stLevel;
		m_pstEventNotes[i]=1;
		m_nRecordLength_Actual+=m_pnLevelLength[i];
		if (m_pstEventCurrentLevel[i]==0) 
		{
			m_pfBaseline[i]=aNode.vstructEventList[i].fAmpMean;
		}
	}

	m_nRecordLength_Formal=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];
	SetChannelNumber();
	m_nEvlID=m_nSN;
	m_nSN++;
}

void CEvl::EvlCorrection()
{
	CEvlCorrection aEvlCorrection;
	aEvlCorrection.UpdateEventList(GetEventList());
	aEvlCorrection.CorrectEventList();
	UpdateEvents(aEvlCorrection.GetEventList());
}

structEventListNode CEvl::GetEventList()
{
	structEventListNode aListNode;
	aListNode.sFileName=m_sEvlFileName;
	aListNode.fTimePerPoint=m_TimePerPoint;
	aListNode.nNumChannels=m_nNumChannels;
	aListNode.nNumLevels=m_nMaxLevel+1;

	for(int i=0;i<m_nNumEvents;i++)
	{
		structEventNode aNode;
		aNode.LevelStart=m_pnLevelStart[i];
		aNode.fAmpMean=m_pfAmplitude[i];
		aNode.LevelLength=m_pnLevelLength[i];
		aNode.fAmpDev=m_pfEventStandardDev[i];
		aNode.stLevel=m_pstEventCurrentLevel[i];
		aListNode.vstructEventList.push_back(aNode);
	}
	return aListNode;
}


void CEvl::SetChannelNumber()
{
	char sTemp[12]="Channels: 2";
	int nTemp;
	sTemp[10]=m_nNumChannels+48;
	for(int i=0;i<120;i++)
	{
		if(i<11)
		{
			m_Comment[i]=sTemp[i];
		}
		else
		{
			m_Comment[i]=' ';
		}
	}
}

void CEvl::MeanBSTCorrection(FILE *fpTotal, structFittingResultNode aResultNode, float fBDelimiter, int nBurstIndex)
{
	//This function fit and output total open time per burst (NOPB) of the current evl.
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	float fTmin=aResultNode.fFittingRange.fLeft;
	float fTmax=aResultNode.fFittingRange.fRight;

	float fCTInRange=ClosedTimeInRange(fTmin, fTmax);
	int nComponents=aResultNode.nNumComponents;
	float fDwellTimeInRange[20];
	float fTau=0.f;


	float TimePerPoint=m_TimePerPoint/1000.;

	int nTemp=int(fBDelimiter/TimePerPoint)+1;
	fBDelimiter=(double(nTemp)-0.001)*TimePerPoint;//This midification is to reflect discrete aspect of dwell time

	int i;

	float fDT=0.f;
	float fTDT=0.f;

	for(i=0;i<nComponents;i++)
	{
		fTau=aResultNode.dTimeConstants[i];
		fDT=aResultNode.dWeights[i]*DwellTimeInRange(fTmin,fTmax,fTau);
		fDwellTimeInRange[i]=fDT;
		fTDT+=fDT;
	}

	float fLeftEvents[20];
	float fRightEvents[20];
	int nEventsInRange=aResultNode.nEventsInRange;
	float fFittedArea=aResultNode.dFittedArea;
	float fTc=fBDelimiter;
	double dW;

	for(i=0;i<nComponents;i++)
	{
		fTau=aResultNode.dTimeConstants[i];
		dW=aResultNode.dWeights[i];
		fLeftEvents[i]=LeftEvents(fTc, fTau, fFittedArea, dW, nEventsInRange, fTmin, fTmax);
		fRightEvents[i]=RightEvents(fTc, fTau, fFittedArea, dW, nEventsInRange, fTmin, fTmax);
	}

	float fR0=fCTInRange/fTDT;

	m_cSummary.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,NULL,m_sEvlFileName);
	m_cSummary.ClearSummary();
	m_cSummary.SummarizeEvl();
	structEvlSummaryNode aSummary0=m_cSummary.GetSummaryNode();

	IgnoreShortClosing(fBDelimiter);

	m_cSummary.UpdateData(m_nNumChannels,m_NumLevels,m_TimePerPoint,m_nNumEvents,
						m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,
						m_pstEventCurrentLevel,NULL,m_sEvlFileName);
	m_cSummary.ClearSummary();
	m_cSummary.SummarizeEvl();
	structEvlSummaryNode aSummary=m_cSummary.GetSummaryNode();

	float fTBT0=aSummary.vectfResidentTime[1]; //Total burst duration before correction
	int nNB0=aSummary.vectnNumEvents[1]; //Total number of bursts before correction
	float fMean;

	fMean=MeanDwellTimeInRange(0,fBDelimiter,aResultNode.dTimeConstants[nBurstIndex]);
	float fTBT=fTBT0-fMean*fLeftEvents[nBurstIndex];
	for(i=0;i<nBurstIndex;i++)
	{
//		fMean=MeanDwellTimeInRange(fBDelimiter,100.*fTmax,aResultNode.dTimeConstants[i]);
		fTau=aResultNode.dTimeConstants[i];
		fMean=fTau+fBDelimiter;
		fTBT+=fMean*fRightEvents[i];
	}

	int nNB=nNB0+int(fLeftEvents[nBurstIndex]+0.5);
	
	float fMTo=aSummary0.fMeanOpenTime;

	for(i=0;i<nBurstIndex;i++)
	{
		nNB-=int(fRightEvents[i]+0.5);
	}
	int nNOPB0=aSummary0.nNumEvents/(2*nNB0);
	float fToT0=nNOPB0*fMTo;

	int nNOPB=aSummary0.nNumEvents/(2*nNB);
	float fToT=nNOPB*fMTo;

	fprintf(fpTotal,"%10d%12.2f%12d%12d%14.2f%14.2f%12d%12d%14.2f%14.2f        %s",nBurstIndex,fBDelimiter,nNB0,nNOPB0,aSummary.fMeanOpenTime,fToT0,nNB,nNOPB,fTBT/double(nNB),fToT,m_sEvlFileName);
}

float CEvl::ClosedTimeInRange(float fMin, float fMax)
{
	float fTCTInRange=0.f;
	int nLength,nTLength=0;
	int nMin=fMin/(m_TimePerPoint/1000.);
	int nMax=fMax/(m_TimePerPoint/1000.);
	int nLevel;
	for(int i=0;i<m_nNumEvents;i++)
	{
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==0)
		{
			nLength=m_pnLevelLength[i];
			if(nLength>=nMin&&nLength<=nMax)
			{
				nTLength+=nLength;
			}
		}
	}
	fTCTInRange=nTLength*m_TimePerPoint/1000.;
	return fTCTInRange;
}

float CEvl::DwellTimeInRange(float fT1, float fT2, float fTau)
{
	float fResults=(fT1+fTau)*exp(-fT1/fTau)-(fT2+fTau)*exp(-fT2/fTau);
	return fResults;
}

float CEvl::MeanDwellTimeInRange(float fT1, float fT2, float fTau)
{
	float fResult=(fT1+fTau)*exp(-fT1/fTau)-(fT2+fTau)*exp(-fT2/fTau);
	fResult/=(exp(-fT1/fTau)-exp(-fT2/fTau));
	return fResult;
}

float CEvl::LeftEvents(float fTc, float fTau, float fFittedArea, double dW, int nEventsInRange, float fTmin, float fTmax)
{
	float fA=dW*(exp(-fTmin/fTau)-exp(-fTmax/fTau));
	float fN=nEventsInRange*fA/fFittedArea;
	float fLeftEvents=fN*dW*(1-exp(-fTc/fTau))/fA;
	return fLeftEvents;
}

float CEvl::RightEvents(float fTc, float fTau, float fFittedArea, double dW, int nEventsInRange, float fTmin, float fTmax)
{
	float fA=dW*(exp(-fTmin/fTau)-exp(-fTmax/fTau));
	float fN=nEventsInRange*fA/fFittedArea;
	float fRightEvents=fN*dW*exp(-fTc/fTau)/fA;
	return fRightEvents;
}

CString CEvl::GetEvlFileName()
{
	return m_sEvlFileName;
}

void CEvl::ReverseOpenAndClosed()
{
	for(int i=0;i<m_nNumEvents;i++)
	{
		if(m_pstEventCurrentLevel[i]==0)
			m_pstEventCurrentLevel[i]=1;
		else if(m_pstEventCurrentLevel[i]==1)
			m_pstEventCurrentLevel[i]=0;
	}
}

void CEvl::DoBurstingTimeCourse_UB(AbfNode aAbfNode)
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded, aAbfNode);
	aBH.ExportBurstStatitics();
}

void CEvl::SetComment(CString sComment)
{
	int nLength=sComment.GetLength();
	if(nLength>128) nLength=128;
	for(int i=0;i<nLength;i++)
	{
		m_Comment[i]=sComment[i];
	}
}

void CEvl::APH_EventIndex(structBurstingTimeCourseOptionNode aNode, AbfNode aAbfNode, CLinearHistogramDouble &APH1, CLinearHistogramDouble &APH2, CLinearHistogramDouble &APHn1, CLinearHistogramDouble &APHn2)
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded, aAbfNode);
	aBH.getEventIndexAPH(aNode, APH1, APH2, APHn1, APHn2);
}

void CEvl::readAtf()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="the result file of clampfit (*.atf)\0*.atf\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="reading the atf format result file";

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		readAtf(sAbfFileName);
	}
}


void CEvl::readAtf(CString sFileName)
{
	FILE* fp=fopen(sFileName,"r");
	int n=0;
	int sigDigits=1;
	CString stemp;
	int level,maxLevel;
	double t0,t1,deltaT;
	maxLevel=-1;
	vector <double> fvt0,fvt1,fvamp,fvampdev,nvlevel,fvdt;
	double dmin,dt;
	bool first=true;
	float ft0,ft1,amp,ampDev;
	while(!feof(fp)){
		fscanf(fp,"%s%s%d%s%f%f%f%f%s%s%s",stemp,stemp,&level,stemp,&ft0,&ft1,&amp,&ampDev,stemp,stemp,stemp);
		fvt0.push_back(ft0);
		fvt1.push_back(ft1);
		fvamp.push_back(amp);
		fvampdev.push_back(ampDev);
		nvlevel.push_back(level);
		if(level>maxLevel) maxLevel=level;
		dt=ft1-ft0;
		if(first){
			dmin=dt;
			first=false;
		}
		if(dt<dmin) {
			dmin=dt;
		}
		fvdt.push_back(dt);
		n++;
	}
	fclose(fp);

	float tol=0.00001;
	m_TimePerPoint=calSamplingInterval(fvdt,dmin,sigDigits);
	if(m_TimePerPoint<10) m_TimePerPoint*=1000.;//the unit should be in microsecond.
	m_nNumEvents=n;
//	m_Comment="";
	m_NumLevels=maxLevel+1;

	PrepairPointers();
	m_nRecordLength_Actual=0;

	if(m_nNumEvents>m_nMaxNumEvents)
	{
		m_nNumEvents=m_nMaxNumEvents;
	}

	int nShift=0;
	int l;
	for(int i=m_nNumEvents0;i<m_nNumEvents;i++)
	{
		t0=fvt0[i];
		t1=fvt1[i];
		level=nvlevel[i];
		amp=fvamp[i];
		ampDev=fvampdev[i];
		l=int(t0*1000./m_TimePerPoint+0.5);
		m_pnLevelStart[i+nShift]=l;
		m_pfAmplitude[i+nShift]=amp;
		l=int(t1*1000./m_TimePerPoint+0.5)-l;
		m_pnLevelLength[i+nShift]=l;
		if(l<=0){
			l=l;
		}
		m_pfEventStandardDev[i+nShift]=ampDev;
		m_pstEventCurrentLevel[i+nShift]=level;
//		m_pstEventNotes[i+nShift]='';
		m_nRecordLength_Actual+=m_pnLevelLength[i+nShift];
		if (*(m_pstEventCurrentLevel+i+nShift)==0) 
		{
			m_pfBaseline[i+nShift]=0.f;
		}
		if(i==m_nNumEvents0)
		{
			if(m_pstEventCurrentLevel[0]==0&&m_bExcludingLastClosure)
			{
				nShift=-1;
				m_nRecordLength_Actual=0;//11/28/2007
			}
		}
	}
	fclose(fp);
}

float CEvl::calSamplingInterval(vector <double> fvdt, double delta, int sigDigits){
	double dmin,dt,del;
	round(delta,sigDigits);
	dmin=fabs(delta);
	int size=fvdt.size();
	int i,j;
	double tol=delta/10.;
	for(i=0;i<size;i++){
		for(j=0;j<i;j++){
			dt=fabs(fvdt[i]-fvdt[j]);
			if(dt<dmin&&dt>tol) dmin=dt;
		}
	}
	round(dmin,sigDigits);
	return dmin;
}

void CEvl::trim(double &d0, int sigDigits){
	int D=1;
	double d=fabs(d0);
	for(int i=1;i<sigDigits;i++){
		D*=10;
	}
	int D10=D*10;
	double factor;

	double folds=1.;
	if(d>D10){
		factor=0.1;
		while(d>D10){
			d*=factor;
			folds*=factor;
		}
	}

	if(d<D){
		factor=10.;
		while(d<D){
			d*=factor;
			folds*=factor;
		}
	}

	d0*=folds;
	d0=int(d0);
	d0/=folds;
}

void CEvl::round(double &d0, int sigDigits){
	int D=1;
	double d=fabs(d0);
	for(int i=1;i<sigDigits;i++){
		D*=10;
	}
	int D10=D*10;
	double factor;

	double folds=1.;
	if(d>D10){
		factor=0.1;
		while(d>D10){
			d*=factor;
			folds*=factor;
		}
	}

	if(d<D){
		factor=10.;
		while(d<D){
			d*=factor;
			folds*=factor;
		}
	}

	d0*=folds;
	if(d0>0)
		d0=int(d0+0.5);
	else if(d0<0)
		d0=int(d0-0.5);
	d0/=folds;
}

void CEvl::filteringEvents()
{
	CBurstingTimeCourseOptionDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		structBurstingTimeCourseOptionNode aNode;
		aDlg.getBurstTimecourseOptions(aNode);
	}
}

void CEvl::exportBurstEvolutions()
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	aBH.exportBurstEvolutions();
}

void CEvl::filteringEvents(structBurstingTimeCourseOptionNode aNode)
{
	int i,j,key;
	bool *pbExcl=new bool[m_nNumEvents];
	bool b1,b2;
	for(i=0;i<m_nNumEvents;i++){
		pbExcl[i]=false;
	}
	int size=aNode.vnFilteringKeys.size();
	for(i=0;i<size;i++){
		aNode.nFilteringKey=aNode.vnFilteringKeys[i];
		key=aNode.vnFilteringKeys[i];
		FilteringEvents(aNode.nFilteringKey,aNode);
		for(j=0;j<m_nNumEvents;j++){
			b1=pbExcl[j];
			b2=m_pbExcluded[j];		
			if(key>0)
				pbExcl[j]=b1||b2;//need to pass both filtering, or an event will be excluded by any of the filtering.
			if(key<0)
				pbExcl[j]=b1&&b2;
		}
	}
	delete m_pbExcluded;
	m_pbExcluded=pbExcl;
}

void CEvl::ExportBurstcorrelations()
{
	CBurstHandling aBH;
	float fCutoff=0.f; //This function can concatenate some events erroneously assigned the same event level the the 
	              //preceding events by pClamp.
	IgnoreShortClosing(fCutoff);
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded);
	aBH.ExportBurstcorrelations();
}

void CEvl::buildSubsets()
{
	m_bSubsets=true;
	CIncidenceSubsetHandler aHandler;
	aHandler.updateEvl(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded, m_nEvlID,m_fRisetime);
	aHandler.buildSubsets();
}


void CEvl::updateIncidenceSubsetHandler(CIncidenceSubsetHandler &ish)
{
	ish.updateEvl(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded,m_nEvlID,m_fRisetime);
}

int CEvl::getEvlID()
{
	return m_nEvlID;
}

void CEvl::updateID()
{
	m_nEvlID=m_nSN;
	m_nSN++;
}

float CEvl::getTimePerPoint()
{
	return m_TimePerPoint;
}

void CEvl::updateBurstHandler(CBurstHandling &aBH)
{
	aBH.UpdateData(m_nNumChannels, m_NumLevels, m_TimePerPoint, m_nNumEvents,
						m_pnLevelStart, m_pnLevelLength, m_pfAmplitude,m_pfBaseline,
						m_pstEventCurrentLevel, m_sEvlFileName, m_pbExcluded,m_nEvlID);
}
void CEvl::excludeEvents(vector<int> vnIndexes){
	int nSize=vnIndexes.size();
	int index;
	int length;
	for(int i=0;i<nSize;i++){
		index=vnIndexes[i];
		length=m_pnLevelLength[index];
		m_pbExcluded[index]=true;
	}
}

CLinearHistogramDouble CEvl::obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax)
{
	CLinearHistogramDouble aph;
	double dDelta=0.1,dAmp;
	aph.update(dMin,dMax,dDelta);
	float* pfData=aNode.pfData;
	int i,j,nSize=vnIndexes.size(),len, nStart,nIndex;
	for(i=0;i<nSize;i++){
		nIndex=vnIndexes[i];
		nStart=m_pnLevelStart[nIndex];
		len=m_pnLevelLength[nIndex];
		aph.addData(pfData,nStart,1,len);
	}
	return aph;
}

void CEvl::getAmpRange(double &dMinAmp, double &dMaxAmp){
	int i,nSize;
	float fAmp;
	if(m_bAbfNode){
		nSize=m_pstructAbfNode->nSize;
		float *pfData=m_pstructAbfNode->pfData;
		dMinAmp=pfData[0];
		dMaxAmp=pfData[0];
		for(i=1;i<nSize;i++){
			fAmp=pfData[i];
			if(fAmp>dMaxAmp) dMaxAmp=fAmp;
			if(fAmp<dMinAmp) dMinAmp=fAmp;
		}
	}else{
		dMinAmp=m_pfAmplitude[0];
		dMaxAmp=m_pfAmplitude[0];
		for(i=1;i<m_nNumEvents;i++){
			fAmp=m_pfAmplitude[i];
			if(fAmp>dMaxAmp) dMaxAmp=fAmp;
			if(fAmp<dMinAmp) dMinAmp=fAmp;
		}
	}
}

void CEvl::setAbfNode(AbfNode &abfNode){
	m_bAbfNode=true;
	m_pstructAbfNode=&abfNode;
}

void CEvl::updateAmp(float *pfAmp, int indexI, int indexF){
	if(indexI<0){
		indexI=0;
		indexF=m_nNumEvents-1;
	}
	for(int i=indexI;i<=indexF;i++){
		m_pfAmplitude[i]=pfAmp[i];
	}
}

