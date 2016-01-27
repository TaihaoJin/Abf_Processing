// AbfHeaderToWrite.cpp: implementation of the AbfHeaderToWrite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "AbfHeaderToWrite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbfHeaderToWrite::CAbfHeaderToWrite()
{
	ReadAbfHeaderToWrite();
}

CAbfHeaderToWrite::~CAbfHeaderToWrite()
{

}

void CAbfHeaderToWrite::ReadAbfHeaderToWrite()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	CString Message;
	Message="Please open a DAT file!";
//	MessageBox( NULL, NULL,MB_OK );

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");

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
	char ch=' ';

	for(i=1;i <=m_nNumPointsIgnored;i++) 
	{
		fread(&ch,1,1,fpAbfIn);
	}

		fclose(fpAbfIn);
	}
	m_fScale=1.0f/(m_fInstrumentScaleFactor[0]*m_fAutosampleAdditGain);
	m_fScale*=m_fADCRange;
	m_fScale/=m_lADCResolution;
}
