// Abf.cpp: implementation of the CAbf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "Abf.h"
#include "BallRolling.h"
#include "math.h"
#include "BlineAdjustOptionDialog.h"
#include "AllPointHistgram.h"
#include "AllPointHistgramDialog.h"
#include "ArtifactsHandling.h"
#include "AbfTraceIdealization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbf::CAbf()
{
//	m_nDataFormatToWrite=0;
//	m_nDataFormatToWrite;
//	m_fFileVersionNumberToWrite=1.3f;
//	m_fHeaderVersionNumberToWrite=1.3f;
	m_pfData=new float[1];
	m_pfAdjustedData=new float[1];
	m_pfPreviousData=new float[1];
	m_pfPreviousAdjustedData=new float[1];
	m_pfCurrentData=new float[1];
}

CAbf::~CAbf()
{
	delete m_pfData;
	delete m_pfAdjustedData;
	delete m_pfPreviousData;
	delete m_pfPreviousAdjustedData;
}
int CAbf::ReadData(CString sFileName){
	FILE *fpIn;		
	int nStatus=1;
	m_sAbfFileName=sFileName;
	if(!(fpIn=fopen(sFileName,"rb")))
	{
		CString sMessage="Eorror occurred when reading a data file";
		AfxMessageBox(sMessage,MB_OK,0);
		sMessage=sFileName;
		AfxMessageBox(sMessage,MB_OK,0);
		nStatus=-1;
	}else{
		ReadData(fpIn);
	}
	return nStatus;
}

void CAbf::ReadData(FILE* fpAbfIn)
{
	int OffsetOfBytes,i;

//Reading Group 1;

	OffsetOfBytes=0;
	fread(&m_sFileType, 4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fFileVersionNumber,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nOperationMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lActualAcqLength,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nNumPointsIgnored,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lActualEpisodes,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFileStartDate,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFileStartTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lStopWatchTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fHeaderVersionNumber,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nFileType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nMSBinFormat,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 2;

	fread(&m_lDataSectionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lTagSectionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumTagEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lLongDescriptionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lLongDescriptionLines,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDACFilePtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDACFileNumEpisodes,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused68,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDeltaArrayPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumDeltas,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookManEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookAutoEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSynchArrayPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSynchArraySize,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nDataFormat,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused102,18,1,fpAbfIn);
	OffsetOfBytes+=18;

//Reading Group 3;

	fread(&m_nADCNumChannels,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fADCSampleInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4; 
	fread(&m_fADCSecondSampleInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fSynchTimeUnit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fSecondsPerRun,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumSamplesPerEpisode,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lPreTriggerSamples,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lEpisodesPerRun,4,1,fpAbfIn);//So far so good!
	OffsetOfBytes+=4;
	fread(&m_lRunsPerTrial,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumberOfTrials,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nAveragingMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nUndoRunCount,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nFirstEpisodeInRun,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fTriggerThreshold,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nTriggerSource,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTriggerAction,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTriggerPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fScopeOutputInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fEpisodeStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fRunStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fTrialStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lAverageCount,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lClockChange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused198,2,1,fpAbfIn);
	OffsetOfBytes+=2;

// Reading Group 4;

	fread(&m_nDrawingStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTileDisplay,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nEraseStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDataDisplayMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lDisplayAverageUpdate,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nChannelStatsStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lCalculationPeriod,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSamplesPerTrace,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lStartDisplayNum,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFinishDisplayNum,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nMultiColor,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nShowPNRawData,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused234,10,1,fpAbfIn); 
	OffsetOfBytes+=10;

//Reading Group 5;

	fread(&m_fADCRange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fDACRange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lADCResolution,4,1,fpAbfIn);//good!
	OffsetOfBytes+=4;
	fread(&m_lDACResolution,4,1,fpAbfIn);
	OffsetOfBytes+=4;

//Reading Group 6;

	fread(&m_nExperimentType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleInstrument,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fAutosampleAdditGain,4,1,fpAbfIn);//500 good!
	OffsetOfBytes+=4;
	fread(&m_fAutosampleFilter,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fAutosampleMembraneCap,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nManualInfoStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fCellID1,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fCellID2,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fCellID3,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sCreatorInfo,16,1,fpAbfIn);//"Clampex   "
	OffsetOfBytes+=16;
	fread(&m_sFileComment,56,1,fpAbfIn);
	OffsetOfBytes+=56;
	fread(&m_sUnused366,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group 7;

	for(i=0;i<=15;++i)
	{		
		fread(&m_nADCPtoLChannelMap[i],2,1,fpAbfIn);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_nADCSamplingSeq[i],2,1,fpAbfIn);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_sADCChannelName[i],10,1,fpAbfIn);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fread(&m_sADCUnits[i],8,1,fpAbfIn);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fADCProgrammableGain[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fread(&m_fADCDisplayAmplification[i],4,1,fpAbfIn);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fread(&m_fADCDisplayOffset[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fread(&m_fInstrumentScaleFactor[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fInstrumentOffset[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fread(&m_fSignalGain[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fSignalOffset[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{		
		fread(&m_fSignalLowpassFilter[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fSignalHighpassFilter[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_sDACChannelName[i],10,1,fpAbfIn);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_sDACChannelUnits[i],8,1,fpAbfIn);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_fDACScaleFactor[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fread(&m_fDACHoldingLevel[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	fread(&m_nSignalType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1412,10,1,fpAbfIn);
	OffsetOfBytes+=10;

//Reading Group 8;

	fread(&m_nOUTEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nSampleNumberOUT1,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nSampleNumberOUT2,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nFirstEpisodeOUT,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nLastEpisodeOUT,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPulseSamplesOUT1,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPulseSamplesOUT2,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 9;

	fread(&m_nDigitalEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nWaveformSource,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nActiveDACChannel,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nInterEpisodeLevel,2,1,fpAbfIn);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochType[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_fEpochInitLevel[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_fEpochLevelInc[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochInitDuration[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochDurationInc[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}


	fread(&m_nDigitalHolding,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDigitalInterEpisode,2,1,fpAbfIn);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fread(&m_nDigitalValue[i],2,1,fpAbfIn);
		OffsetOfBytes+=2;
	}

	fread(&m_fWaveformOffset,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1612,8,1,fpAbfIn);
	OffsetOfBytes+=8;

//Rreading Group10;

	fread(&m_fDACFileScale,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fDACFileOffset,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1628,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDACFileEpisodeNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDACFileADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sDACFileName,12,1,fpAbfIn);
	OffsetOfBytes+=12;
	fread(&m_sDACFilePath,60,1,fpAbfIn);
	OffsetOfBytes+=60;
	fread(&m_sUnused1706,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group11;

	fread(&m_nConditEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nConditChannel,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lConditNumPulses,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fBaselineDuration,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fBaselineLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fStepDuration,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fStepLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPostTrainPeriod,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPostTrainLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1750,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group12;

	fread(&m_nParamToVary,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sParamValueList,80,1,fpAbfIn);
	OffsetOfBytes+=80;//checked till here.

//Reading Group13;

	fread(&m_nAutopeakEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakSearchMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lAutopeakStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lAutopeakEnd,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nAutopeakSmoothing,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakBaseline,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakAverage,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1866,14,1,fpAbfIn);
	OffsetOfBytes+=14;

//Reading Group14;

	fread(&m_nArithmeticEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fArithmeticUpperLimit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticLowerLimit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nArithmeticADCNumA,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nArithmeticADCNumB,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fArithmeticK1,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK2,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK3,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK4,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sArithmeticOperator,2,1,fpAbfIn);//good!
	OffsetOfBytes+=2;
	fread(&m_sArithmeticUnits,8,1,fpAbfIn);
	OffsetOfBytes+=8;
	fread(&m_fArithmeticK5,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK6,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nArithmeticExpression,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1930,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 15;

	fread(&m_nPNEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNPosition,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNNumPulses,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fPNHoldingLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPNSettlingTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPNInterPulse,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1954,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group 16;

	fread(&m_nListEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1966,80,1,fpAbfIn);
	OffsetOfBytes+=80;

////	char ch=' ';

//	for(i=1;i <=m_nNumPointsIgnored;i++) 
//	{
//		fread(&ch,1,1,fpAbfIn);
//	}

	PreparePointers();


//	m_fScale=1.0f/(m_fInstrumentScaleFactor[0]*m_fAutosampleAdditGain);
//	m_fScale*=m_fADCRange;
//	m_fScale/=m_lADCResolution;


//	short nAmplitude;
//	float fAmplitude=0.f;


//	if(m_nDataFormat==0)
//	{	
//		for(i=0;i<m_lActualAcqLength;i++)
//		{
//			fread(&nAmplitude, sizeof(nAmplitude),1,fpAbfIn);
//			fAmplitude=nAmplitude*m_fScale;
//			*(m_pfData+i)=fAmplitude;
//		}
//	}
//	else
//	{
//		for(i=0;i<m_lActualAcqLength;i++)
//		{		
//			fread(&fAmplitude, sizeof(float),1,fpAbfIn);
////			*(m_pfData+i)=fAmplitude;
//		}

//		int nExtraLine=0;
//		while(feof(fpAbfIn))
//		{					
//			fread(&fAmplitude, sizeof(float),1,fpAbfIn);
//			nExtraLine++;
//		}

//	}








	m_vectfScale.clear();
	for(int k=0;k<m_nADCNumChannels;k++)
	{
		float fScale=1.0f/(m_fInstrumentScaleFactor[k]);
		if(k==m_nAutosampleADCNum)
		{
			if(m_nAutosampleEnable!=0)
			{
				fScale/=m_fAutosampleAdditGain;
			}
		}
		fScale*=m_fADCRange;
		fScale/=m_lADCResolution;
		m_vectfScale.push_back(fScale);
	}

	int nBytesToSkip=(m_lDataSectionPtr-4)*512;
	char cTemp;
	m_vectcPreDataBuffer.clear();
	while(nBytesToSkip>0)
	{
		fread(&cTemp,sizeof(cTemp),1,fpAbfIn);
		nBytesToSkip-=sizeof(cTemp);
		m_vectcPreDataBuffer.push_back(cTemp);
	}

	short nAmplitude;
	float fAmplitude=0.f;
	float fScale=0.f;
	int nPosition=0;
	int i0=0;
	if(m_nDataFormat==0)
	{	
		for(i=0;i<m_lActualAcqLength/m_nADCNumChannels;i++)
//		for(i=0;i<m_lActualAcqLength;i++)
		{
			for(i0=0;i0<m_nADCNumChannels;i0++)
			{
				fread(&nAmplitude, sizeof(nAmplitude),1,fpAbfIn);
				fScale=m_vectfScale[i0];
				*(m_pfData+nPosition)=nAmplitude*fScale;
				m_pbExcluded[nPosition]=false;
				nPosition++;
			}
		}
	}
	else
	{
		for(i=0;i<m_lActualAcqLength/m_nADCNumChannels;i++)
//		for(i=0;i<m_lActualAcqLength;i++)
		{
			for(int i0=0;i0<m_nADCNumChannels;i0++)
			{
				fread(&fAmplitude, sizeof(float),1,fpAbfIn);
				
				*(m_pfData+nPosition)=fAmplitude;
				m_pbExcluded[nPosition]=false;
				nPosition++;
			}
		}
	}		
	
	m_vectcPostDataBuffer.clear();

	int nExtraLine=0;
	while(feof(fpAbfIn))
	{					
		fread(&cTemp, sizeof(char),1,fpAbfIn);
		m_vectcPostDataBuffer.push_back(cTemp);
		nExtraLine++;
	}
}


void CAbf::BaslieAdjustment()
{
	float fWindowSize;
	float fFirstPoint;
	CBaselineProcessing aBaselineProcessing(m_pfData,m_lActualAcqLength,m_fADCSampleInterval);
	CBlineAdjustOptionDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		fFirstPoint=aDlg.GetFirstPoint();
		fWindowSize=aDlg.GetWindowSize();
		aBaselineProcessing.BaselineCorrection(fFirstPoint,fWindowSize);
		aBaselineProcessing.GetAdjustedData(m_pfAdjustedData);
	}
}

void CAbf::WriteAdjustedData(FILE *fpAbfOut)
{
	float* pfData=m_pfData;
	UpdateCurrent(m_pfAdjustedData);
	WriteData(fpAbfOut);
	m_pfData=pfData;
}

void CAbf::PreparePointers()
{
	delete m_pfData;
	delete m_pfAdjustedData;
//	m_pfData=new float[m_lActualAcqLength*m_nADCNumChannels];
//	m_pfAdjustedData=new float[m_lActualAcqLength*m_nADCNumChannels];
//	m_pbExcluded=new float[m_lActualAcqLength*m_nADCNumChannels];

	m_pfData=new float[m_lActualAcqLength];
	m_pfAdjustedData=new float[m_lActualAcqLength];
	m_pbExcluded=new bool[m_lActualAcqLength];
}

void CAbf::MakeAllPointHistgram(CString sFileName)
{
	CAllPointHistgramDialog aDlg;
	bool bShowTrace=true;
	int nLength=0;
	if(aDlg.DoModal()==IDOK)
	{
		float fFirstPoint=aDlg.GetFirstPoint();
		float fLength=aDlg.GetSegmentLength();
		bShowTrace=aDlg.ShowTrace();
		int nFirstPoint=int(1000*fFirstPoint/m_fADCSampleInterval)/m_nADCNumChannels;
		if(fLength >=0.f)
		{
			nLength=int(1000*fLength/m_fADCSampleInterval)/m_nADCNumChannels;
			if(nLength>m_lActualAcqLength-nFirstPoint)
			{
				nLength=m_lActualAcqLength-nFirstPoint;
			}
		}
		else
		{
			nLength=m_lActualAcqLength-nFirstPoint;
		}
		int nReductionFactor=aDlg.GetReductionFactor();
		int nPolarity=aDlg.GetPolarity();
		float fTransitionThreshold=aDlg.GetTransitThreshold();
		float *pfData=new float[nLength];
		for(int i=0;i<nLength;i++)
		{
			pfData[i]=m_pfData[(nFirstPoint+i)*m_nADCNumChannels];
		}
		CAllPointHistgram aHist(sFileName, pfData,nLength,m_fADCSampleInterval*m_nADCNumChannels/1000.f,bShowTrace,nReductionFactor,nPolarity,fTransitionThreshold,fFirstPoint);
		aHist.WriteHist();
	}
}

void CAbf::ImportAsciiAsData(FILE *fpAbfIn)
{
	float st=0.;
	int i=0;
	int nLineNum=0;

	while(!feof(fpAbfIn))
	{
//		fscanf(fpAbfIn,"%e%e",&st,&st);
		fscanf(fpAbfIn,"%e",&st);
		nLineNum++;
		if(nLineNum%10000==1)
		{
			nLineNum=nLineNum;
		}
	}
	rewind(fpAbfIn);
	m_pfData=new float[nLineNum];
	m_lActualAcqLength=nLineNum;

	while(!feof(fpAbfIn))
	{
//		fscanf(fpAbfIn,"%e%e",&st,m_pfData+i);
		fscanf(fpAbfIn,"%e",m_pfData+i);
		i++;
	}
	m_fScale=m_fScale;
	m_bImportedAsciiAsData=true;
}



void CAbf::BallRolling(CHistory* pcHistory)
{
	int nRullerSize=40;
	GetCurrent();
	CBallRolling aBallRolling(m_pfCurrentData, m_pfAdjustedData, m_nNumCurrentPoints, m_fCurrentInterval);
	aBallRolling.BaselineCorrection(nRullerSize,pcHistory);
}

void CAbf::RemoveArtifacts(const EvlNode structEvlNode)
{
	int nLength;
	float *pfData, fInterval;
	GetCurrent();
	CArtifactsHandling aArtifactHandling(m_pfCurrentData, m_pfAdjustedData, m_nNumCurrentPoints, m_fCurrentInterval, m_pbExcluded, structEvlNode);
}

void CAbf::UpdateAdjustedData()
{
	for(int i=0;i<m_lActualAcqLength;i++)
	{
		*(m_pfData+i)=*(m_pfAdjustedData+i);
	}
}

void CAbf::ReplaceSegments(CHistory* pcHistory)
{
	CArtifactsHandling cHandling;
	cHandling.ReplaceSegments(m_pfData,m_pfAdjustedData,
		m_lActualAcqLength,m_fADCSampleInterval,pcHistory);
}

AbfNode CAbf::GetTrace()
{
	AbfNode aNode;
	float fmin=999999.f,fmax=-999999.f;
	float fAmp;
	float* pfData=new float[m_lActualAcqLength];
	int i;
	for(i=0;i<m_lActualAcqLength;i++)
	{
		fAmp=m_pfData[i];
		if(fAmp<fmin) fmin=fAmp;
		if(fAmp>fmax) fmax=fAmp;
		pfData[i]=fAmp;
	}
	aNode.nSize=m_lActualAcqLength;
	aNode.pfData=pfData;
	aNode.fSampleInterval=m_fADCSampleInterval;
	aNode.nNumChannels=m_nADCNumChannels;
	for(i=0;i<m_nADCNumChannels;i++)
	{
		aNode.fAutosampleAdditGain=m_fAutosampleAdditGain;
	}
	aNode.fMaxAmp=fmax;
	aNode.fMinAmp=fmin;
	aNode.sFileName=m_sAbfFileName;
	return aNode;
}

void CAbf::ExtractSegments(FILE *fpAbfOut, structSegmentExtractionNode aNode)
{
	vector <structSegmentNode> vectSweeps;
	int i=0;

//Writing Group 1;

	int lActualAcqLength;

	float* pfData=new float[m_lActualAcqLength];

	lActualAcqLength=0;

	float fTime0=0.f;
	float fTime=0.f;
	float fTemp=0.f;

	int nLength=0;
	int nFirstPoint=-1;
	int nLastPoint;
	float	fMaximum[10];
	int j;
	for(j=0;j<10;j++)
	{
		fMaximum[j]=-10000000000.f;
	}
	float fTemp0,fTemp1;
	int nRank=3,nTemp;
	for(i=0;i<m_lActualAcqLength;i++)
	{
		fTime=float(i)*float(m_fADCSampleInterval)/1000.f;
		if(fTime0>=aNode.fLength)
		{
			fTime0-=aNode.fLength;
			structSegmentNode bNode;
			bNode.nFirstPoint=nFirstPoint;
			bNode.nLastPoint=nLastPoint;
			bNode.nLength=nLength;
			bNode.fMaximum=fMaximum[nRank-1];
			vectSweeps.push_back(bNode);
			nLength=0;
			nFirstPoint=-1;
			for(j=0;j<10;j++)
			{
				fMaximum[j]=-10000000000.f;
			}
		}
		if(fTime0>=aNode.fBegin && fTime0 <aNode.fEnd)
		{
			nTemp=i-1;
			if(nTemp<0)
			{
				nTemp=0;
			}

			fTemp0=*(m_pfData+nTemp);
			fTemp=*(m_pfData+i);
			nTemp=i+1;
			if(nTemp>=m_lActualAcqLength)
			{
				nTemp=i;
			}
			fTemp1=*(m_pfData+nTemp);
			*(pfData+lActualAcqLength)=fTemp;
			if(fTemp>fTemp0 && fTemp > fTemp1)
			{
				for(nTemp=0;nTemp<=nRank-1;nTemp++)
				{
					j=nRank-1-nTemp;
					if(fTemp>fMaximum[j])
					{
						fMaximum[j+1]=fMaximum[j];
						fMaximum[j]=fTemp;
					}
				}
			}
			lActualAcqLength++;
			nLength++;
			if(nFirstPoint==-1)
			{
				nFirstPoint=i;
			}
			nLastPoint=i;
		}
		fTime0+=float(m_fADCSampleInterval)/1000.f;
	}

	int nSize=vectSweeps.size();
	int nFirstSweep=aNode.nFirstSweep;
	int nLastSweep=aNode.nLastWeep;
	if(nLastSweep>nFirstSweep+nSize)
	{
		nLastSweep=nFirstSweep+nSize-1;
	}

	lActualAcqLength=0;

	for(i=nFirstSweep;i<=nLastSweep;i++)
	{
		lActualAcqLength+=vectSweeps[i-1].nLength;
	}


	int OffsetOfBytes=0;
	fwrite(&m_sFileType, 4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fFileVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	short nOperationMode=3;//m_nOperationMode;
	fwrite(&nOperationMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	

	if(m_bImportedAsciiAsData)
	{
		fwrite(&lActualAcqLength,4,1,fpAbfOut);
	}
	else 
	{
		fwrite(&lActualAcqLength,4,1,fpAbfOut);
	}
	OffsetOfBytes+=4;

	if(m_bImportedAsciiAsData)
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	else 
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	
	OffsetOfBytes+=2;
	int lActualEpisodes=1;
	fwrite(&lActualEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartDate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStopWatchTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fHeaderVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nFileType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nMSBinFormat,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 2;
	int lDataSectionPtr=4;
	fwrite(&lDataSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lTagSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumTagEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionLines,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFilePtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFileNumEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused68,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDeltaArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumDeltas,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookManEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookAutoEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	int lSynchArrayPtr=0;
	int lSynchArraySize=0;
	m_lSynchArrayPtr=0;
	fwrite(&lSynchArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	m_lSynchArraySize=0;
	fwrite(&lSynchArraySize,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	short snTemp=m_nDataFormat;
	m_nDataFormat=0;
	fwrite(&m_nDataFormat,2,1,fpAbfOut);
	m_nDataFormat=snTemp;
	OffsetOfBytes+=2;
	fwrite(&m_sUnused102,18,1,fpAbfOut);
	OffsetOfBytes+=18;

//Writing Group 3;

	fwrite(&m_nADCNumChannels,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fADCSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4; 
	fwrite(&m_fADCSecondSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	float fSynchTimeUnit=0.f;
	fwrite(&fSynchTimeUnit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fSecondsPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumSamplesPerEpisode,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lPreTriggerSamples,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lEpisodesPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lRunsPerTrial,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumberOfTrials,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAveragingMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nUndoRunCount,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeInRun,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fTriggerThreshold,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	short nTriggerSource=-1;
	fwrite(&nTriggerSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerAction,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fScopeOutputInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fEpisodeStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fRunStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fTrialStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	int lAverageCount=0;
	fwrite(&lAverageCount,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lClockChange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused198,2,1,fpAbfOut);
	OffsetOfBytes+=2;

// Writing Group 4;

	fwrite(&m_nDrawingStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTileDisplay,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nEraseStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDataDisplayMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lDisplayAverageUpdate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nChannelStatsStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lCalculationPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSamplesPerTrace,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStartDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	int lFinishDisplayNum=0;
	fwrite(&lFinishDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nMultiColor,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nShowPNRawData,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused234,10,1,fpAbfOut); 
	OffsetOfBytes+=10;

//Writing Group 5;

	fwrite(&m_fADCRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lADCResolution,4,1,fpAbfOut);//good!
	OffsetOfBytes+=4;
	fwrite(&m_lDACResolution,4,1,fpAbfOut);
	OffsetOfBytes+=4;

//Writing Group 6;

	fwrite(&m_nExperimentType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleInstrument,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fAutosampleAdditGain,4,1,fpAbfOut);//500 good!
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleFilter,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleMembraneCap,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nManualInfoStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fCellID1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sCreatorInfo,16,1,fpAbfOut);//"Clampex   "
	OffsetOfBytes+=16;
	fwrite(&m_sFileComment,56,1,fpAbfOut);
	OffsetOfBytes+=56;
	fwrite(&m_sUnused366,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 7;

	for(i=0;i<=15;++i)
	{		
		fwrite(&m_nADCPtoLChannelMap[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_nADCSamplingSeq[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_sADCChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fwrite(&m_sADCUnits[i],8,1,fpAbfOut);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fADCProgrammableGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayAmplification[i],4,1,fpAbfOut);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fwrite(&m_fInstrumentScaleFactor[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fInstrumentOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fwrite(&m_fSignalGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

		for (i=0;i<=15;++i)
		{		
			fwrite(&m_fSignalOffset[i],4,1,fpAbfOut);		
			OffsetOfBytes+=4;
		}


	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalLowpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalHighpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelUnits[i],8,1,fpAbfOut);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_fDACScaleFactor[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fwrite(&m_fDACHoldingLevel[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	fwrite(&m_nSignalType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1412,10,1,fpAbfOut);
	OffsetOfBytes+=10;

//Writing Group 8;

	fwrite(&m_nOUTEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nLastEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 9;

	fwrite(&m_nDigitalEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nWaveformSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nActiveDACChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	short nInterEpisodeLevel=0;
	fwrite(&nInterEpisodeLevel,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochType[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochInitLevel[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochLevelInc[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochInitDuration[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochDurationInc[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}


	fwrite(&m_nDigitalHolding,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDigitalInterEpisode,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fwrite(&m_nDigitalValue[i],2,1,fpAbfOut);
		OffsetOfBytes+=2;
	}

	fwrite(&m_fWaveformOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1612,8,1,fpAbfOut);
	OffsetOfBytes+=8;

//RWriting Group10;

	fwrite(&m_fDACFileScale,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACFileOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1628,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileEpisodeNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sDACFileName,12,1,fpAbfOut);
	OffsetOfBytes+=12;
	fwrite(&m_sDACFilePath,60,1,fpAbfOut);
	OffsetOfBytes+=60;
	fwrite(&m_sUnused1706,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group11;

	fwrite(&m_nConditEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nConditChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lConditNumPulses,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1750,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group12;

	fwrite(&m_nParamToVary,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sParamValueList,80,1,fpAbfOut);
	OffsetOfBytes+=80;//checked till here.

//Writing Group13;

	fwrite(&m_nAutopeakEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakSearchMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lAutopeakStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lAutopeakEnd,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAutopeakSmoothing,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakBaseline,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakAverage,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1866,14,1,fpAbfOut);
	OffsetOfBytes+=14;

//Writing Group14;

	fwrite(&m_nArithmeticEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticUpperLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticLowerLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticADCNumA,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nArithmeticADCNumB,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticK1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK4,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sArithmeticOperator,2,1,fpAbfOut);//good!
	OffsetOfBytes+=2;
	fwrite(&m_sArithmeticUnits,8,1,fpAbfOut);
	OffsetOfBytes+=8;
	fwrite(&m_fArithmeticK5,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK6,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticExpression,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1930,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 15;

	fwrite(&m_nPNEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPosition,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNNumPulses,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fPNHoldingLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNSettlingTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNInterPulse,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1954,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 16;

	fwrite(&m_nListEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1966,80,1,fpAbfOut);
	OffsetOfBytes+=80;

	char ch=' ';

	for(i=1;i <=m_nNumPointsIgnored;i++) 
	{
		fwrite(&ch,1,1,fpAbfOut);
	}

	short nAmp=0;


	nSize=vectSweeps.size();
	int nReferenceSweep=aNode.nRefSweep;
	int nFirstReferencePoint;
	nFirstReferencePoint=vectSweeps[nReferenceSweep-1].nFirstPoint;
	nLength=0;
	for(i=0;i<nSize;i++)
	{
		nFirstPoint=vectSweeps[i].nFirstPoint;
		for(j=0;j<vectSweeps[i].nLength;j++)
		{
			fTemp=m_pfData[nFirstPoint+j];
			if(aNode.bSubstractReference)
			{
				fTemp-=m_pfData[nFirstReferencePoint+j];
			}
			if(aNode.bAlignMaximum)
			{
				fTemp-=vectSweeps[i].fMaximum;//-vectSweeps[nReferenceSweep].fMaximum;
			}
			nAmp=short (fTemp/m_fScale);
			fwrite(&nAmp,sizeof(short),1,fpAbfOut);
			nLength++;
		}
	}
	delete pfData;
}

void CAbf::WriteHeader(FILE *fpHeader)
{
	int i=0;

//Writing Group 1;

	int OffsetOfBytes=0;
	fprintf(fpHeader,"m_sFileType  %s\n",m_sFileType);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fFileVersionNumber %f \n",m_fFileVersionNumber);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nOperationMode  %d \n",m_nOperationMode);
	OffsetOfBytes+=2;
	

	if(m_bImportedAsciiAsData)
	{
		fprintf(fpHeader,"m_lActualAcqLength %d\n",m_lActualAcqLength);
	}
	else 
	{
		fprintf(fpHeader,"lActualAcqLength %d\n",m_lActualAcqLength);
	}
	OffsetOfBytes+=4;

	if(m_bImportedAsciiAsData)
	{
		fprintf(fpHeader,"m_nNumPointsIgnored %d\n",m_nNumPointsIgnored);
	}
	else 
	{
		fprintf(fpHeader,"m_nNumPointsIgnored %d\n",m_nNumPointsIgnored);
	}
	
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_lActualEpisodes %d\n",m_lActualEpisodes);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lFileStartDate %d\n",m_lFileStartDate);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lFileStartTime %d\n",m_lFileStartTime);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lStopWatchTime %d\n",m_lStopWatchTime);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fHeaderVersionNumber %f\n",m_fHeaderVersionNumber);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nFileType %d\n",m_nFileType);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nMSBinFormat %d\n",m_nMSBinFormat);
	OffsetOfBytes+=2;

//Writing Group 2;
	fprintf(fpHeader,"m_lDataSectionPtr %d\n",m_lDataSectionPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lTagSectionPtr %d\n",m_lTagSectionPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNumTagEntries %d\n",m_lNumTagEntries);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lLongDescriptionPtr %d\n",m_lLongDescriptionPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lLongDescriptionLines %d\n",m_lLongDescriptionLines);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lDACFilePtr %d\n",m_lDACFilePtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lDACFileNumEpisodes %d\n",m_lDACFileNumEpisodes);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused68 %s\n",m_sUnused68);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lDeltaArrayPtr %d\n",m_lDeltaArrayPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNumDeltas %d\n",m_lNumDeltas);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNotebookPtr %d\n",m_lNotebookPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNotebookManEntries %d\n",m_lNotebookManEntries);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNotebookAutoEntries %d\n",m_lNotebookAutoEntries);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lSynchArrayPtr %d\n",m_lSynchArrayPtr);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lSynchArraySize %d\n",m_lSynchArraySize);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nDataFormat %d\n",m_nDataFormat);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused102 %s\n",m_sUnused102);
	OffsetOfBytes+=18;

//Writing Group 3;

	fprintf(fpHeader,"m_nADCNumChannels %d\n",m_nADCNumChannels);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fADCSampleInterval %f\n",m_fADCSampleInterval);
	OffsetOfBytes+=4; 
	fprintf(fpHeader,"m_fADCSecondSampleInterval %f\n",m_fADCSecondSampleInterval);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fSynchTimeUnit %f\n",m_fSynchTimeUnit);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fSecondsPerRun %f\n",m_fSecondsPerRun);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNumSamplesPerEpisode %d\n",m_lNumSamplesPerEpisode);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lPreTriggerSamples %d\n",m_lPreTriggerSamples);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lEpisodesPerRun %d\n",m_lEpisodesPerRun);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lRunsPerTrial %d\n",m_lRunsPerTrial);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lNumberOfTrials %d\n",m_lNumberOfTrials);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nAveragingMode %d\n",m_nAveragingMode);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nUndoRunCount %d\n",m_nUndoRunCount);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nFirstEpisodeInRun %d\n",m_nFirstEpisodeInRun);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fTriggerThreshold %f\n",m_fTriggerThreshold);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nTriggerSource %d\n",m_nTriggerSource);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nTriggerAction %d\n",m_nTriggerAction);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nTriggerPolarity %d\n",m_nTriggerPolarity);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fScopeOutputInterval %f\n",m_fScopeOutputInterval);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fEpisodeStartToStart %f\n",m_fEpisodeStartToStart);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fRunStartToStart %f\n",m_fRunStartToStart);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fTrialStartToStart %f\n",m_fTrialStartToStart);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lAverageCount %d\n",m_lAverageCount);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lClockChange %d\n",m_lClockChange);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused198 ",m_sUnused198);
	OffsetOfBytes+=2;

// Writing Group 4;

	fprintf(fpHeader,"m_nDrawingStrategy %d\n",m_nDrawingStrategy);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nTileDisplay %d\n",m_nTileDisplay);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nEraseStrategy %d\n",m_nEraseStrategy);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nDataDisplayMode %d\n",m_nDataDisplayMode);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_lDisplayAverageUpdate %d\n",m_lDisplayAverageUpdate);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nChannelStatsStrategy %d\n",m_nChannelStatsStrategy);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_lCalculationPeriod %d\n",m_lCalculationPeriod);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lSamplesPerTrace %d\n",m_lSamplesPerTrace);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lStartDisplayNum %d\n",m_lStartDisplayNum);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lFinishDisplayNum %d\n",m_lFinishDisplayNum);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nMultiColor %d\n",m_nMultiColor);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nShowPNRawData %d\n",m_nShowPNRawData);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused234 %s\n",m_sUnused234); 
	OffsetOfBytes+=10;

//Writing Group 5;

	fprintf(fpHeader,"m_fADCRange %f\n",m_fADCRange);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fDACRange %f\n",m_fDACRange);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lADCResolution %d\n",m_lADCResolution);//good!
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lDACResolution %d\n",m_lDACResolution);
	OffsetOfBytes+=4;

//Writing Group 6;

	fprintf(fpHeader,"m_nExperimentType %d\n",m_nExperimentType);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutosampleEnable %d\n",m_nAutosampleEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutosampleADCNum %d\n",m_nAutosampleADCNum);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutosampleInstrument %d\n",m_nAutosampleInstrument);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fAutosampleAdditGain %f\n",m_fAutosampleAdditGain);//500 good!
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fAutosampleFilter %f\n",m_fAutosampleFilter);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fAutosampleMembraneCap %f\n",m_fAutosampleMembraneCap);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nManualInfoStrategy %d\n",m_nManualInfoStrategy);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fCellID1 %f\n",m_fCellID1);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fCellID2 %f\n",m_fCellID2);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fCellID3 %f\n",m_fCellID3);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sCreatorInfo %s\n",m_sCreatorInfo);//"Clampex   "
	OffsetOfBytes+=16;
	fprintf(fpHeader,"m_sFileComment %s\n",m_sFileComment);
	OffsetOfBytes+=56;
	fprintf(fpHeader,"m_sUnused366 %s\n",m_sUnused366);
	OffsetOfBytes+=12;

//Writing Group 7;

	for(i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_nADCPtoLChannelMap[%2d] %d\n",i,m_nADCPtoLChannelMap[i]);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_nADCSamplingSeq[%2d] %d\n",m_nADCSamplingSeq[i]);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_sADCChannelName[%2d] %5c\n",i,m_sADCChannelName[i]);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fprintf(fpHeader,"m_sADCUnits[%2d] %5c\n",i,m_sADCUnits[i]);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_fADCProgrammableGain[%2d] %f\n",i,m_fADCProgrammableGain[i]);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fprintf(fpHeader,"m_fADCDisplayAmplification[%2d] %f\n",i,m_fADCDisplayAmplification[i]);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fprintf(fpHeader,"m_fADCDisplayOffset[%2d] %f\n",i,m_fADCDisplayOffset[i]);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fprintf(fpHeader,"m_fInstrumentScaleFactor[%2d] %f\n",i,m_fInstrumentScaleFactor[i]);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_fInstrumentOffset[%2d] %f\n",i,m_fInstrumentOffset[i]);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fprintf(fpHeader,"m_fSignalGain[%2d] %f\n",i,m_fSignalGain[i]);		
		OffsetOfBytes+=4;
	}

		for (i=0;i<=15;++i)
		{		
			fprintf(fpHeader,"m_fSignalOffset[%2d] %f\n",i,m_fSignalOffset[i]);		
			OffsetOfBytes+=4;
		}


	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_fSignalLowpassFilter[%2d] %f\n",i,m_fSignalLowpassFilter[i]);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fprintf(fpHeader,"m_fSignalHighpassFilter[%2d] %f\n",i,m_fSignalHighpassFilter[i]);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fprintf(fpHeader,"m_sDACChannelName[%2d] %5c\n",i,m_sDACChannelName[i]);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fprintf(fpHeader,"m_sDACChannelUnits[%2d] %5c\n",i,m_sDACChannelUnits[i]);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fprintf(fpHeader,"m_fDACScaleFactor[%2d] %f\n",i,m_fDACScaleFactor[i]);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fprintf(fpHeader,"m_fDACHoldingLevel[%2d] %f\n",i,m_fDACHoldingLevel[i]);		
		OffsetOfBytes+=4;
	}

	fprintf(fpHeader,"m_nSignalType %d\n",m_nSignalType);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused1412 %s\n",m_sUnused1412);
	OffsetOfBytes+=10;

//Writing Group 8;

	fprintf(fpHeader,"m_nOUTEnable %d\n",m_nOUTEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nSampleNumberOUT1 %d\n",m_nSampleNumberOUT1);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nSampleNumberOUT2 %d\n",m_nSampleNumberOUT2);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nFirstEpisodeOUT %d\n",m_nFirstEpisodeOUT);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nLastEpisodeOUT %d\n",m_nLastEpisodeOUT);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPulseSamplesOUT1 %d\n",m_nPulseSamplesOUT1);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPulseSamplesOUT2 %d\n",m_nPulseSamplesOUT2);
	OffsetOfBytes+=2;

//Writing Group 9;

	fprintf(fpHeader,"m_nDigitalEnable %d\n",m_nDigitalEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nWaveformSource %d\n",m_nWaveformSource);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nActiveDACChannel %d\n",m_nActiveDACChannel);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nInterEpisodeLevel %d\n",m_nInterEpisodeLevel);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fprintf(fpHeader,"m_nEpochType[%2d] %d\n",i,m_nEpochType[i]);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fprintf(fpHeader,"m_fEpochInitLevel[%2d] %f\n",i,m_fEpochInitLevel[i]);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fprintf(fpHeader,"m_fEpochLevelInc[%2d] %f\n",i,m_fEpochLevelInc[i]);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fprintf(fpHeader,"m_nEpochInitDuration[%2d] %d\n",i,m_nEpochInitDuration[i]);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fprintf(fpHeader,"m_nEpochDurationInc[%2d] %d\n",i,m_nEpochDurationInc[i]);
	OffsetOfBytes+=2;
	}


	fprintf(fpHeader,"m_nDigitalHolding %d\n",m_nDigitalHolding);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nDigitalInterEpisode %d\n",m_nDigitalInterEpisode);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fprintf(fpHeader,"m_nDigitalValue[%2d] %d\n",i,m_nDigitalValue[i]);
		OffsetOfBytes+=2;
	}

	fprintf(fpHeader,"m_fWaveformOffset %f\n",m_fWaveformOffset);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused1612 %s\n",m_sUnused1612);
	OffsetOfBytes+=8;

//RWriting Group10;

	fprintf(fpHeader,"m_fDACFileScale %f\n",m_fDACFileScale);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fDACFileScale %f\n",m_fDACFileScale);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused1628 %s\n",m_sUnused1628);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nDACFileEpisodeNum %d\n",m_nDACFileEpisodeNum);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nDACFileADCNum %d\n",m_nDACFileADCNum);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sDACFileName %s\n",m_sDACFileName);
	OffsetOfBytes+=12;
	fprintf(fpHeader,"m_sDACFilePath %s\n",m_sDACFilePath);
	OffsetOfBytes+=60;
	fprintf(fpHeader,"m_sUnused1706 %s\n",m_sUnused1706);
	OffsetOfBytes+=12;

//Writing Group11;

	fprintf(fpHeader,"m_nConditEnable %d\n",m_nConditEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nConditChannel %d\n",m_nConditChannel);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_lConditNumPulses %d\n",m_lConditNumPulses);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fBaselineDuration %f\n",m_fBaselineDuration);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fBaselineLevel %f\n",m_fBaselineLevel);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fStepDuration %f\n",m_fStepDuration);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fStepLevel %f\n",m_fStepLevel);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fPostTrainPeriod %f\n",m_fPostTrainPeriod);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fPostTrainLevel %f\n",m_fPostTrainLevel);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused1750 %s\n",m_sUnused1750);
	OffsetOfBytes+=12;

//Writing Group12;

	fprintf(fpHeader,"m_nParamToVary %d\n",m_nParamToVary);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sParamValueList %s\n",m_sParamValueList);
	OffsetOfBytes+=80;//checked till here.

//Writing Group13;

	fprintf(fpHeader,"m_nAutopeakEnable %d\n",m_nAutopeakEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutopeakPolarity %d\n",m_nAutopeakPolarity);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutopeakADCNum %d\n",m_nAutopeakADCNum);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutopeakSearchMode %d\n",m_nAutopeakSearchMode);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_lAutopeakStart %d\n",m_lAutopeakStart);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_lAutopeakEnd %d\n",m_lAutopeakEnd);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nAutopeakSmoothing %d\n",m_nAutopeakSmoothing);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutopeakBaseline %d\n",m_nAutopeakBaseline);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nAutopeakAverage %d\n",m_nAutopeakAverage);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused1866 %s\n",m_sUnused1866);
	OffsetOfBytes+=14;

//Writing Group14;

	fprintf(fpHeader,"m_nArithmeticEnable %d\n",m_nArithmeticEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fArithmeticUpperLimit %f\n",m_fArithmeticUpperLimit);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fArithmeticLowerLimit %f\n",m_fArithmeticLowerLimit);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nArithmeticADCNumA %d\n",m_nArithmeticADCNumA);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nArithmeticADCNumB %d\n",m_nArithmeticADCNumB);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fArithmeticK1 %f\n",m_fArithmeticK1);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fArithmeticK2 %f\n",m_fArithmeticK2);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fArithmeticK3 %f\n",m_fArithmeticK3);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fArithmeticK4 %f\n",m_fArithmeticK4);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sArithmeticOperator %s\n",m_sArithmeticOperator);//good!
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sArithmeticUnits %s\n",m_sArithmeticUnits);
	OffsetOfBytes+=8;
	fprintf(fpHeader,"m_fArithmeticK5 %f\n",m_fArithmeticK5);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fArithmeticK6 %f\n",m_fArithmeticK6);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_nArithmeticExpression %d\n",m_nArithmeticExpression);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused1930 %s\n",m_sUnused1930);
	OffsetOfBytes+=2;

//Writing Group 15;

	fprintf(fpHeader,"m_nPNEnable %d\n",m_nPNEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPNPosition %d\n",m_nPNPosition);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPNPolarity %d\n",m_nPNPolarity);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPNNumPulses %d\n",m_nPNNumPulses);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_nPNADCNum %d\n",m_nPNADCNum);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_fPNHoldingLevel %f\n",m_fPNHoldingLevel);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fPNSettlingTime %f\n",m_fPNSettlingTime);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_fPNInterPulse %f\n",m_fPNInterPulse);
	OffsetOfBytes+=4;
	fprintf(fpHeader,"m_sUnused1954 %s\n",m_sUnused1954);
	OffsetOfBytes+=12;

//Writing Group 16;

	fprintf(fpHeader,"m_nListEnable %d\n",m_nListEnable);
	OffsetOfBytes+=2;
	fprintf(fpHeader,"m_sUnused1966 %s\n",m_sUnused1966);
	OffsetOfBytes+=80;

}

void CAbf::ReadHeader(FILE *fp)
{
	int OffsetOfBytes,i;

	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
	}


//Reading Group 1;

	OffsetOfBytes=0;

	fread(&m_sFileType, 4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fFileVersionNumber,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nOperationMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lActualAcqLength,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nNumPointsIgnored,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lActualEpisodes,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFileStartDate,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFileStartTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lStopWatchTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fHeaderVersionNumber,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nFileType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nMSBinFormat,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 2;

	fread(&m_lDataSectionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lTagSectionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumTagEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lLongDescriptionPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lLongDescriptionLines,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDACFilePtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDACFileNumEpisodes,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused68,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lDeltaArrayPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumDeltas,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookManEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNotebookAutoEntries,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSynchArrayPtr,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSynchArraySize,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nDataFormat,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused102,18,1,fpAbfIn);
	OffsetOfBytes+=18;

//Reading Group 3;

	fread(&m_nADCNumChannels,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fADCSampleInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4; 
	fread(&m_fADCSecondSampleInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fSynchTimeUnit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fSecondsPerRun,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumSamplesPerEpisode,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lPreTriggerSamples,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lEpisodesPerRun,4,1,fpAbfIn);//So far so good!
	OffsetOfBytes+=4;
	fread(&m_lRunsPerTrial,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lNumberOfTrials,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nAveragingMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nUndoRunCount,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nFirstEpisodeInRun,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fTriggerThreshold,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nTriggerSource,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTriggerAction,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTriggerPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fScopeOutputInterval,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fEpisodeStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fRunStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fTrialStartToStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lAverageCount,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lClockChange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused198,2,1,fpAbfIn);
	OffsetOfBytes+=2;

// Reading Group 4;

	fread(&m_nDrawingStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nTileDisplay,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nEraseStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDataDisplayMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lDisplayAverageUpdate,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nChannelStatsStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lCalculationPeriod,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lSamplesPerTrace,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lStartDisplayNum,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lFinishDisplayNum,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nMultiColor,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nShowPNRawData,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused234,10,1,fpAbfIn); 
	OffsetOfBytes+=10;

//Reading Group 5;

	fread(&m_fADCRange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fDACRange,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lADCResolution,4,1,fpAbfIn);//good!
	OffsetOfBytes+=4;
	fread(&m_lDACResolution,4,1,fpAbfIn);
	OffsetOfBytes+=4;

//Reading Group 6;

	fread(&m_nExperimentType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutosampleInstrument,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fAutosampleAdditGain,4,1,fpAbfIn);//500 good!
	OffsetOfBytes+=4;
	fread(&m_fAutosampleFilter,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fAutosampleMembraneCap,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nManualInfoStrategy,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fCellID1,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fCellID2,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fCellID3,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sCreatorInfo,16,1,fpAbfIn);//"Clampex   "
	OffsetOfBytes+=16;
	fread(&m_sFileComment,56,1,fpAbfIn);
	OffsetOfBytes+=56;
	fread(&m_sUnused366,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group 7;

	for(i=0;i<=15;++i)
	{		
		fread(&m_nADCPtoLChannelMap[i],2,1,fpAbfIn);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_nADCSamplingSeq[i],2,1,fpAbfIn);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_sADCChannelName[i],10,1,fpAbfIn);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fread(&m_sADCUnits[i],8,1,fpAbfIn);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fADCProgrammableGain[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fread(&m_fADCDisplayAmplification[i],4,1,fpAbfIn);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fread(&m_fADCDisplayOffset[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fread(&m_fInstrumentScaleFactor[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fInstrumentOffset[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fread(&m_fSignalGain[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

		for (i=0;i<=15;++i)
		{		
			fread(&m_fSignalOffset[i],4,1,fpAbfIn);		
			OffsetOfBytes+=4;
		}


	for (i=0;i<=15;++i)
	{		
		fread(&m_fSignalLowpassFilter[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fread(&m_fSignalHighpassFilter[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_sDACChannelName[i],10,1,fpAbfIn);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_sDACChannelUnits[i],8,1,fpAbfIn);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fread(&m_fDACScaleFactor[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fread(&m_fDACHoldingLevel[i],4,1,fpAbfIn);		
		OffsetOfBytes+=4;
	}

	fread(&m_nSignalType,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1412,10,1,fpAbfIn);
	OffsetOfBytes+=10;

//Reading Group 8;

	fread(&m_nOUTEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nSampleNumberOUT1,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nSampleNumberOUT2,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nFirstEpisodeOUT,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nLastEpisodeOUT,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPulseSamplesOUT1,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPulseSamplesOUT2,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 9;

	fread(&m_nDigitalEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nWaveformSource,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nActiveDACChannel,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nInterEpisodeLevel,2,1,fpAbfIn);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochType[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_fEpochInitLevel[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_fEpochLevelInc[i],4,1,fpAbfIn);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochInitDuration[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fread(&m_nEpochDurationInc[i],2,1,fpAbfIn);
	OffsetOfBytes+=2;
	}


	fread(&m_nDigitalHolding,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDigitalInterEpisode,2,1,fpAbfIn);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fread(&m_nDigitalValue[i],2,1,fpAbfIn);
		OffsetOfBytes+=2;
	}

	fread(&m_fWaveformOffset,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1612,8,1,fpAbfIn);
	OffsetOfBytes+=8;

//Rreading Group10;

	fread(&m_fDACFileScale,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fDACFileOffset,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1628,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDACFileEpisodeNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nDACFileADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sDACFileName,12,1,fpAbfIn);
	OffsetOfBytes+=12;
	fread(&m_sDACFilePath,60,1,fpAbfIn);
	OffsetOfBytes+=60;
	fread(&m_sUnused1706,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group11;

	fread(&m_nConditEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nConditChannel,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lConditNumPulses,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fBaselineDuration,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fBaselineLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fStepDuration,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fStepLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPostTrainPeriod,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPostTrainLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1750,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group12;

	fread(&m_nParamToVary,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sParamValueList,80,1,fpAbfIn);
	OffsetOfBytes+=80;//checked till here.

//Reading Group13;

	fread(&m_nAutopeakEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakSearchMode,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_lAutopeakStart,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_lAutopeakEnd,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nAutopeakSmoothing,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakBaseline,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nAutopeakAverage,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1866,14,1,fpAbfIn);
	OffsetOfBytes+=14;

//Reading Group14;

	fread(&m_nArithmeticEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fArithmeticUpperLimit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticLowerLimit,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nArithmeticADCNumA,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nArithmeticADCNumB,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fArithmeticK1,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK2,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK3,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK4,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sArithmeticOperator,2,1,fpAbfIn);//good!
	OffsetOfBytes+=2;
	fread(&m_sArithmeticUnits,8,1,fpAbfIn);
	OffsetOfBytes+=8;
	fread(&m_fArithmeticK5,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fArithmeticK6,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_nArithmeticExpression,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1930,2,1,fpAbfIn);
	OffsetOfBytes+=2;

//Reading Group 15;

	fread(&m_nPNEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNPosition,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNPolarity,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNNumPulses,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_nPNADCNum,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_fPNHoldingLevel,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPNSettlingTime,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_fPNInterPulse,4,1,fpAbfIn);
	OffsetOfBytes+=4;
	fread(&m_sUnused1954,12,1,fpAbfIn);
	OffsetOfBytes+=12;

//Reading Group 16;

	fread(&m_nListEnable,2,1,fpAbfIn);
	OffsetOfBytes+=2;
	fread(&m_sUnused1966,80,1,fpAbfIn);
	OffsetOfBytes+=80;

	fclose(fpAbfIn);
}

void CAbf::BackupData()
{
	delete m_pfPreviousData;
	m_pfPreviousData=new float[m_lActualAcqLength];
	int i;
	for(i=0;i<m_lActualAcqLength;i++)
	{
		*(m_pfPreviousData+i)=*(m_pfData+i);
	}

	delete m_pfPreviousAdjustedData;
	m_pfPreviousAdjustedData=new float[m_lActualAcqLength];

	for(i=0;i<m_lActualAcqLength;i++)
	{
		*(m_pfPreviousAdjustedData+i)=*(m_pfAdjustedData+i);
	}
}


void CAbf::WriteComparison(FILE *fpAbfOut, int nKey)
{
	m_lActualAcqLength/=2;
	m_lEpisodesPerRun  = m_lActualAcqLength/m_lNumSamplesPerEpisode;
	m_lActualAcqLength = m_lEpisodesPerRun*m_lNumSamplesPerEpisode;

	float* pfData=m_pfData;
	m_pfData=m_pfPreviousData;
	WriteData(fpAbfOut);
	m_pfData=pfData;	
}

int CAbf::GetDataSize()
{
	return m_lActualAcqLength;
}

void CAbf::UpdateSize(int nSize)
{
	m_lActualAcqLength=nSize;
}

void CAbf::UpdateData(float *pfData)
{
	PreparePointers();
	int i;
	for(i=0;i<m_lActualAcqLength;i++)
	{
		m_pfData[i]=pfData[i];
	}
}

AbfNode CAbf::GetAdjustedTrace()
{
	AbfNode aNode;
	aNode.nSize=m_lActualAcqLength;
	aNode.pfData=m_pfAdjustedData;
	return aNode;
}

void CAbf::EqualizeScale()
{
	for(int i=1;i<m_nADCNumChannels;i++)
	{
		m_vectfScale[i]=m_vectfScale[0];
	}
}

void CAbf::UnitaryScale(int nChannelIndex)
{
	m_vectfScale[nChannelIndex]=1.f;
}

void CAbf::ChannelNameUnit(CString sChannelName, CString sChannelUnits, int nChannelIndex)
{
	int nSize,i;
	nSize=sChannelName.GetLength();
	for(i=0;i<nSize;i++)
	{
		m_sADCChannelName[i][nChannelIndex]=sChannelName[i];
	}
	m_sADCChannelName[nSize][nChannelIndex]='\0';

	nSize=sChannelUnits.GetLength();
	for(i=0;i<nSize;i++)
	{
		m_sADCUnits[i][nChannelIndex]=sChannelUnits[i];
	}
	m_sADCUnits[nSize][nChannelIndex]='\0';
}


void CAbf::SetScale(int nChannelIndex, float fFold)
{
	m_vectfScale[nChannelIndex]*=fFold;	 
	m_fInstrumentScaleFactor[nChannelIndex]/=fFold;
}

float CAbf::GetMaxAmp(int nChannelIndex)
{
	return m_lADCResolution*m_vectfScale[nChannelIndex];
}

void CAbf::ResetSampleInterval(float fNewSampleInterval)
{
	m_fADCSampleInterval=fNewSampleInterval;
}

void CAbf::ResetInstrumentScaleFactor(int nChannelIndex, float NewScaleFactor)
{
	m_fInstrumentScaleFactor[nChannelIndex]=NewScaleFactor;
}

void CAbf::UpdateData(AbfNode NewNode)
{
	UpdateSize(NewNode.nSize);
	ResetSampleInterval(NewNode.fSampleInterval);
	UpdateData(NewNode.pfData);
	m_fAutosampleAdditGain=NewNode.fAutosampleAdditGain;
}

void CAbf::WriteExtrema(FILE *fpOut)
{

	int *pLminima=new int[m_lActualAcqLength/3];
	int *pLmaxima=new int[m_lActualAcqLength/3];
	int lN, lNmin, lNmax;
	lN=m_lActualAcqLength;
	LocalExtrema(m_pfData,pLminima,pLmaxima,lN,lNmin,lNmax);
	int nSize=lNmin;
	if(nSize>lNmax)
		nSize=lNmax;
	int nTemp=0;
	float fTime1,fTime2,fAmp1,fAmp2;
	for(int i=0;i<nSize;i++)
	{
		nTemp=pLminima[i];
		fTime1=nTemp*m_fADCSampleInterval/1000000.;
		fAmp1=m_pfData[nTemp];

		nTemp=pLmaxima[i];
		fTime2=nTemp*m_fADCSampleInterval/1000000.;
		fAmp2=m_pfData[nTemp];

		fprintf(fpOut,"%12.4f    %10.2f    %12.4f    %10.2f\n",fTime1,fAmp1,fTime2,fAmp2);
	}
}


void CAbf::LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax)
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


float CAbf::GetSampleInterval()
{
	return m_fADCSampleInterval;
}

structEventListNode CAbf::GetEventList()
{
	structEventListNode aNode;
	CAbfTraceIdealization aEventList;
	aEventList.UpdateData(m_pfData,m_lActualAcqLength,m_fADCSampleInterval);
	aEventList.Idealization();

	aNode=aEventList.GetEventList();
	aNode.fTimePerPoint=m_fADCSampleInterval;
	return aNode;
}

void CAbf::UpdateSampleInterval(float fSampleInterval)
{
	m_fADCSampleInterval=fSampleInterval;
}

void CAbf::GetCurrent()
{
	m_fCurrentInterval=m_fADCSampleInterval*m_nADCNumChannels;
	m_nNumCurrentPoints=m_lActualAcqLength/m_nADCNumChannels;
	delete m_pfCurrentData;
	m_pfCurrentData=new float[m_nNumCurrentPoints];
	for(int i=0;i<m_nNumCurrentPoints;i++)
	{
		m_pfCurrentData[i]=m_pfData[m_nADCNumChannels*i];
	}
}

void CAbf::UpdateCurrent(float *pfData)
{
	int nNumEx=0,j;
	int nNumCurrentPoints=m_lActualAcqLength/m_nADCNumChannels;
	for(int i=0;i<m_nNumCurrentPoints;i++)
	{
		if(m_pbExcluded[i])
		{
			nNumEx++;
		}
		else
		{
			m_pfData[m_nADCNumChannels*(i-nNumEx)]=pfData[i];
			for(j=1;j<m_nADCNumChannels;j++)
			{
				m_pfData[m_nADCNumChannels*(i-nNumEx)+1]=m_pfData[m_nADCNumChannels*i+1];
			}
		}
	}
	m_lActualAcqLength-=nNumEx*m_nADCNumChannels;
}

CString CAbf::getAbfFileName(){
	return m_sAbfFileName;
}

void CAbf::updateAmp(AbfNode aNode,int indexI, int indexF){//if indexI<0, update the whole trace
	if(indexI<0){
		indexI=0;
		indexF=aNode.nSize-1;
	}
	for(int i=indexI;i<=indexF;i++){
		m_pfData[i]=aNode.pfData[i];
	}
}

void CAbf::WriteData(FILE* fpAbfOut, int nDataFormat)
{
	int nTemp=m_nDataFormat;
	m_nDataFormat=nDataFormat;
	WriteAbfHeader(fpAbfOut);
	WriteAbfBody(fpAbfOut);
	m_nDataFormat=nTemp;
	/*
	int i=0;

//Writing Group 1;

	int OffsetOfBytes=0;
	fwrite(&m_sFileType, 4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fFileVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nOperationMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	

	if(m_bImportedAsciiAsData)
	{
		fwrite(&m_lActualAcqLength,4,1,fpAbfOut);
	}
	else 
	{
		fwrite(&m_lActualAcqLength,4,1,fpAbfOut);
	}
	OffsetOfBytes+=4;

	if(m_bImportedAsciiAsData)
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	else 
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	
	OffsetOfBytes+=2;
	fwrite(&m_lActualEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartDate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStopWatchTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fHeaderVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nFileType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nMSBinFormat,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 2;
//	m_lDataSectionPtr=4;
	fwrite(&m_lDataSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lTagSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumTagEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionLines,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFilePtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFileNumEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused68,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDeltaArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumDeltas,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookManEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookAutoEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSynchArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSynchArraySize,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	short nTemp=m_nDataFormat;
	m_nDataFormat=0;
	fwrite(&m_nDataFormat,2,1,fpAbfOut);
	m_nDataFormat=nTemp;
	OffsetOfBytes+=2;
	fwrite(&m_sUnused102,18,1,fpAbfOut);
	OffsetOfBytes+=18;

//Writing Group 3;

	fwrite(&m_nADCNumChannels,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fADCSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4; 
	fwrite(&m_fADCSecondSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fSynchTimeUnit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fSecondsPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumSamplesPerEpisode,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lPreTriggerSamples,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lEpisodesPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lRunsPerTrial,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumberOfTrials,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAveragingMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nUndoRunCount,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeInRun,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fTriggerThreshold,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nTriggerSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerAction,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fScopeOutputInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fEpisodeStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fRunStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fTrialStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lAverageCount,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lClockChange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused198,2,1,fpAbfOut);
	OffsetOfBytes+=2;

// Writing Group 4;

	fwrite(&m_nDrawingStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTileDisplay,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nEraseStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDataDisplayMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lDisplayAverageUpdate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nChannelStatsStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lCalculationPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSamplesPerTrace,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStartDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFinishDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nMultiColor,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nShowPNRawData,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused234,10,1,fpAbfOut); 
	OffsetOfBytes+=10;

//Writing Group 5;

	fwrite(&m_fADCRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lADCResolution,4,1,fpAbfOut);//good!
	OffsetOfBytes+=4;
	fwrite(&m_lDACResolution,4,1,fpAbfOut);
	OffsetOfBytes+=4;

//Writing Group 6;

	fwrite(&m_nExperimentType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleInstrument,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fAutosampleAdditGain,4,1,fpAbfOut);//500 good!
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleFilter,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleMembraneCap,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nManualInfoStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fCellID1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sCreatorInfo,16,1,fpAbfOut);//"Clampex   "
	OffsetOfBytes+=16;
	fwrite(&m_sFileComment,56,1,fpAbfOut);
	OffsetOfBytes+=56;
	fwrite(&m_sUnused366,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 7;

	for(i=0;i<=15;++i)
	{		
		fwrite(&m_nADCPtoLChannelMap[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_nADCSamplingSeq[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_sADCChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fwrite(&m_sADCUnits[i],8,1,fpAbfOut);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fADCProgrammableGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayAmplification[i],4,1,fpAbfOut);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fwrite(&m_fInstrumentScaleFactor[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fInstrumentOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fwrite(&m_fSignalGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

		for (i=0;i<=15;++i)
		{		
			fwrite(&m_fSignalOffset[i],4,1,fpAbfOut);		
			OffsetOfBytes+=4;
		}


	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalLowpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalHighpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelUnits[i],8,1,fpAbfOut);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_fDACScaleFactor[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fwrite(&m_fDACHoldingLevel[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	fwrite(&m_nSignalType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1412,10,1,fpAbfOut);
	OffsetOfBytes+=10;

//Writing Group 8;

	fwrite(&m_nOUTEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nLastEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 9;

	fwrite(&m_nDigitalEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nWaveformSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nActiveDACChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nInterEpisodeLevel,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochType[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochInitLevel[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochLevelInc[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochInitDuration[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochDurationInc[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}


	fwrite(&m_nDigitalHolding,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDigitalInterEpisode,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fwrite(&m_nDigitalValue[i],2,1,fpAbfOut);
		OffsetOfBytes+=2;
	}

	fwrite(&m_fWaveformOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1612,8,1,fpAbfOut);
	OffsetOfBytes+=8;

//RWriting Group10;

	fwrite(&m_fDACFileScale,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACFileOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1628,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileEpisodeNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sDACFileName,12,1,fpAbfOut);
	OffsetOfBytes+=12;
	fwrite(&m_sDACFilePath,60,1,fpAbfOut);
	OffsetOfBytes+=60;
	fwrite(&m_sUnused1706,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group11;

	fwrite(&m_nConditEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nConditChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lConditNumPulses,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1750,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group12;

	fwrite(&m_nParamToVary,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sParamValueList,80,1,fpAbfOut);
	OffsetOfBytes+=80;//checked till here.

//Writing Group13;

	fwrite(&m_nAutopeakEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakSearchMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lAutopeakStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lAutopeakEnd,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAutopeakSmoothing,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakBaseline,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakAverage,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1866,14,1,fpAbfOut);
	OffsetOfBytes+=14;

//Writing Group14;

	fwrite(&m_nArithmeticEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticUpperLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticLowerLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticADCNumA,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nArithmeticADCNumB,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticK1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK4,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sArithmeticOperator,2,1,fpAbfOut);//good!
	OffsetOfBytes+=2;
	fwrite(&m_sArithmeticUnits,8,1,fpAbfOut);
	OffsetOfBytes+=8;
	fwrite(&m_fArithmeticK5,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK6,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticExpression,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1930,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 15;

	fwrite(&m_nPNEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPosition,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNNumPulses,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fPNHoldingLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNSettlingTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNInterPulse,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1954,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 16;

	fwrite(&m_nListEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1966,80,1,fpAbfOut);
	OffsetOfBytes+=80;

//	char ch=' ';

//	for(i=1;i <=m_nNumPointsIgnored;i++) 
//	{
//		fwrite(&ch,1,1,fpAbfOut);
//	}

	char cTemp;
	int nSize=m_vectcPreDataBuffer.size();
	for(i=0;i<nSize;i++)
	{
		cTemp=m_vectcPreDataBuffer[i];
		fwrite(&cTemp,sizeof(char),1,fpAbfOut);
	}

	float fAmp=0.f;
	short nAmp=0;
	float fScale=0.f;
	int nPosition=0;
	int i0;
	if(m_nDataFormat==0)
	{
		for(i=0;i< m_lActualAcqLength/m_nADCNumChannels;i++)
		{
			for(i0=0;i0<m_nADCNumChannels;i0++)
			{
				fAmp=*(m_pfData+nPosition);
				fScale=m_vectfScale[i0];
				nAmp=short (fAmp/fScale);
				fwrite(&nAmp,sizeof(short),1,fpAbfOut);
				nPosition++;
			}
		}
	}
	else
	{
		for(i=0;i< m_lActualAcqLength/m_nADCNumChannels;i++)
		{
			for(i0=0;i0<m_nADCNumChannels;i0++)
			{
				fAmp=*(m_pfData+nPosition);
				fwrite(&fAmp,sizeof(short),1,fpAbfOut);
				nPosition++;
			}
		}
	}

	nSize=m_vectcPostDataBuffer.size();
	for(i=0;i<nSize;i++)
	{
		cTemp=m_vectcPostDataBuffer[i];
		fwrite(&cTemp,sizeof(char),1,fpAbfOut);
	}
	*/
		//end of WriteData(FILE fpAbfOut, int nDataFormat)
}


void CAbf::WriteAbfHeader(FILE* fpAbfOut)
{
	int i=0;

//Writing Group 1;

	int OffsetOfBytes=0;
	fwrite(&m_sFileType, 4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fFileVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nOperationMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	

	if(m_bImportedAsciiAsData)
	{
		fwrite(&m_lActualAcqLength,4,1,fpAbfOut);
	}
	else 
	{
		fwrite(&m_lActualAcqLength,4,1,fpAbfOut);
	}
	OffsetOfBytes+=4;

	if(m_bImportedAsciiAsData)
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	else 
	{
		fwrite(&m_nNumPointsIgnored,2,1,fpAbfOut);
	}
	
	OffsetOfBytes+=2;
	fwrite(&m_lActualEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartDate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFileStartTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStopWatchTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fHeaderVersionNumber,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nFileType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nMSBinFormat,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 2;
//	m_lDataSectionPtr=4;
	fwrite(&m_lDataSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lTagSectionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumTagEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lLongDescriptionLines,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFilePtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDACFileNumEpisodes,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused68,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lDeltaArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumDeltas,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookManEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNotebookAutoEntries,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSynchArrayPtr,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSynchArraySize,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	short nTemp=m_nDataFormat;
	m_nDataFormat=0;
	fwrite(&m_nDataFormat,2,1,fpAbfOut);
	m_nDataFormat=nTemp;
	OffsetOfBytes+=2;
	fwrite(&m_sUnused102,18,1,fpAbfOut);
	OffsetOfBytes+=18;

//Writing Group 3;

	fwrite(&m_nADCNumChannels,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fADCSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4; 
	fwrite(&m_fADCSecondSampleInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fSynchTimeUnit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fSecondsPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumSamplesPerEpisode,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lPreTriggerSamples,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lEpisodesPerRun,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lRunsPerTrial,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lNumberOfTrials,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAveragingMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nUndoRunCount,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeInRun,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fTriggerThreshold,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nTriggerSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerAction,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTriggerPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fScopeOutputInterval,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fEpisodeStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fRunStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fTrialStartToStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lAverageCount,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lClockChange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused198,2,1,fpAbfOut);
	OffsetOfBytes+=2;

// Writing Group 4;

	fwrite(&m_nDrawingStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nTileDisplay,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nEraseStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDataDisplayMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lDisplayAverageUpdate,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nChannelStatsStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lCalculationPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lSamplesPerTrace,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lStartDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lFinishDisplayNum,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nMultiColor,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nShowPNRawData,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused234,10,1,fpAbfOut); 
	OffsetOfBytes+=10;

//Writing Group 5;

	fwrite(&m_fADCRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACRange,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lADCResolution,4,1,fpAbfOut);//good!
	OffsetOfBytes+=4;
	fwrite(&m_lDACResolution,4,1,fpAbfOut);
	OffsetOfBytes+=4;

//Writing Group 6;

	fwrite(&m_nExperimentType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutosampleInstrument,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fAutosampleAdditGain,4,1,fpAbfOut);//500 good!
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleFilter,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fAutosampleMembraneCap,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nManualInfoStrategy,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fCellID1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fCellID3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sCreatorInfo,16,1,fpAbfOut);//"Clampex   "
	OffsetOfBytes+=16;
	fwrite(&m_sFileComment,56,1,fpAbfOut);
	OffsetOfBytes+=56;
	fwrite(&m_sUnused366,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 7;

	for(i=0;i<=15;++i)
	{		
		fwrite(&m_nADCPtoLChannelMap[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_nADCSamplingSeq[i],2,1,fpAbfOut);		
		OffsetOfBytes+=2;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_sADCChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=15;++i)
	{		
			fwrite(&m_sADCUnits[i],8,1,fpAbfOut);		
			OffsetOfBytes+=8;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fADCProgrammableGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayAmplification[i],4,1,fpAbfOut);			
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{
		fwrite(&m_fADCDisplayOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
	fwrite(&m_fInstrumentScaleFactor[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fInstrumentOffset[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}


	for (i=0;i<=15;++i)
	{
		fwrite(&m_fSignalGain[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

		for (i=0;i<=15;++i)
		{		
			fwrite(&m_fSignalOffset[i],4,1,fpAbfOut);		
			OffsetOfBytes+=4;
		}


	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalLowpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=15;++i)
	{		
		fwrite(&m_fSignalHighpassFilter[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelName[i],10,1,fpAbfOut);		
		OffsetOfBytes+=10;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_sDACChannelUnits[i],8,1,fpAbfOut);		
		OffsetOfBytes+=8;
	}

	for (i=0;i<=3;++i)
	{		
		fwrite(&m_fDACScaleFactor[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	for (i=0;i<=3;++i)
	{
		fwrite(&m_fDACHoldingLevel[i],4,1,fpAbfOut);		
		OffsetOfBytes+=4;
	}

	fwrite(&m_nSignalType,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1412,10,1,fpAbfOut);
	OffsetOfBytes+=10;

//Writing Group 8;

	fwrite(&m_nOUTEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nSampleNumberOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nFirstEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nLastEpisodeOUT,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT1,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPulseSamplesOUT2,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 9;

	fwrite(&m_nDigitalEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nWaveformSource,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nActiveDACChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nInterEpisodeLevel,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochType[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochInitLevel[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_fEpochLevelInc[i],4,1,fpAbfOut);
	OffsetOfBytes+=4;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochInitDuration[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}

	for (i=0;i<=9;++i)
	{
	fwrite(&m_nEpochDurationInc[i],2,1,fpAbfOut);
	OffsetOfBytes+=2;
	}


	fwrite(&m_nDigitalHolding,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDigitalInterEpisode,2,1,fpAbfOut);
	OffsetOfBytes+=2;

	for (i=0;i<=9;++i)
	{
		fwrite(&m_nDigitalValue[i],2,1,fpAbfOut);
		OffsetOfBytes+=2;
	}

	fwrite(&m_fWaveformOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1612,8,1,fpAbfOut);
	OffsetOfBytes+=8;

//RWriting Group10;

	fwrite(&m_fDACFileScale,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fDACFileOffset,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1628,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileEpisodeNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nDACFileADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sDACFileName,12,1,fpAbfOut);
	OffsetOfBytes+=12;
	fwrite(&m_sDACFilePath,60,1,fpAbfOut);
	OffsetOfBytes+=60;
	fwrite(&m_sUnused1706,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group11;

	fwrite(&m_nConditEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nConditChannel,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lConditNumPulses,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fBaselineLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepDuration,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fStepLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainPeriod,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPostTrainLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1750,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group12;

	fwrite(&m_nParamToVary,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sParamValueList,80,1,fpAbfOut);
	OffsetOfBytes+=80;//checked till here.

//Writing Group13;

	fwrite(&m_nAutopeakEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakSearchMode,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_lAutopeakStart,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_lAutopeakEnd,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nAutopeakSmoothing,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakBaseline,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nAutopeakAverage,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1866,14,1,fpAbfOut);
	OffsetOfBytes+=14;

//Writing Group14;

	fwrite(&m_nArithmeticEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticUpperLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticLowerLimit,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticADCNumA,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nArithmeticADCNumB,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fArithmeticK1,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK2,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK3,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK4,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sArithmeticOperator,2,1,fpAbfOut);//good!
	OffsetOfBytes+=2;
	fwrite(&m_sArithmeticUnits,8,1,fpAbfOut);
	OffsetOfBytes+=8;
	fwrite(&m_fArithmeticK5,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fArithmeticK6,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_nArithmeticExpression,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1930,2,1,fpAbfOut);
	OffsetOfBytes+=2;

//Writing Group 15;

	fwrite(&m_nPNEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPosition,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNPolarity,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNNumPulses,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_nPNADCNum,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_fPNHoldingLevel,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNSettlingTime,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_fPNInterPulse,4,1,fpAbfOut);
	OffsetOfBytes+=4;
	fwrite(&m_sUnused1954,12,1,fpAbfOut);
	OffsetOfBytes+=12;

//Writing Group 16;

	fwrite(&m_nListEnable,2,1,fpAbfOut);
	OffsetOfBytes+=2;
	fwrite(&m_sUnused1966,80,1,fpAbfOut);
	OffsetOfBytes+=80;

//	char ch=' ';

//	for(i=1;i <=m_nNumPointsIgnored;i++) 
//	{
//		fwrite(&ch,1,1,fpAbfOut);
//	}

	char cTemp;
	int nSize=m_vectcPreDataBuffer.size();
	for(i=0;i<nSize;i++)
	{
		cTemp=m_vectcPreDataBuffer[i];
		fwrite(&cTemp,sizeof(char),1,fpAbfOut);
	}
		//end of WriteAbfHeader(FILE fpAbfOut)
}


void CAbf::WriteData(FILE* fpAbfOut){
	int nFormat=0;
	WriteData(fpAbfOut,nFormat);
}
void CAbf::WriteAbfBody(FILE* fpAbfOut)
{
	int i=0;

	float fAmp=0.f;
	short nAmp=0;
	float fScale=0.f;
	int nPosition=0;
	int i0;
	if(m_nDataFormat==0)
	{
		for(i=0;i< m_lActualAcqLength/m_nADCNumChannels;i++)
		{
			for(i0=0;i0<m_nADCNumChannels;i0++)
			{
				fAmp=*(m_pfData+nPosition);
				fScale=m_vectfScale[i0];
				nAmp=short (fAmp/fScale);
				fwrite(&nAmp,sizeof(short),1,fpAbfOut);
				nPosition++;
			}
		}
	}
	else
	{
		for(i=0;i< m_lActualAcqLength/m_nADCNumChannels;i++)
		{
			for(i0=0;i0<m_nADCNumChannels;i0++)
			{
				fAmp=*(m_pfData+nPosition);
				fwrite(&fAmp,sizeof(short),1,fpAbfOut);
				nPosition++;
			}
		}
	}

	int nSize=m_vectcPostDataBuffer.size();
	char cTemp;
	for(i=0;i<nSize;i++)
	{
		cTemp=m_vectcPostDataBuffer[i];
		fwrite(&cTemp,sizeof(char),1,fpAbfOut);
	}
		//end of WriteAbfBody(FILE fpAbfOut, int nDataFormat)
}
