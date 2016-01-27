// Abf.h: interface for the CAbf class.
//
//////////////////////////////////////////////////////////////////////

#include "BaselineProcessing.h"
#include "Structures.h"
#include "History.h"
#if !defined(AFX_ABF_H__829A0817_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_ABF_H__829A0817_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAbf  
{
public:
	CString getAbfFileName();
protected:
	//Variables in Group 1:
	void LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	vector <char> m_vectcPreDataBuffer;
	vector <char> m_vectcPostDataBuffer;
	char m_sFileType[5];
	short m_nOperationMode,m_nNumPointsIgnored,m_nFileType,m_nMSBinFormat;
	float m_Amplitude,m_fFileVersionNumber,m_fHeaderVersionNumber;
	int m_lActualAcqLength,m_lActualEpisodes,m_lFileStartDate,m_lFileStartTime,m_lStopWatchTime;

	//Variables in Group 2:

	int m_lDataSectionPtr,m_lTagSectionPtr,m_lNumTagEntries,m_lLongDescriptionPtr,m_lLongDescriptionLines,
		m_lDACFilePtr,m_lDACFileNumEpisodes,m_lDeltaArrayPtr,m_lNumDeltas,m_lNotebookPtr,m_lNotebookManEntries,
		m_lNotebookAutoEntries,m_lSynchArrayPtr,m_lSynchArraySize;
	short m_nDataFormat;
	char m_sUnused68[69],m_sUnused102[19];

	//Variables in Group 3:

	short m_nADCNumChannels,m_nAveragingMode,m_nUndoRunCount,m_nFirstEpisodeInRun,m_nTriggerSource,m_nTriggerAction,
		m_nTriggerPolarity;
	float m_fADCSampleInterval,m_fADCSecondSampleInterval,m_fSynchTimeUnit,m_fSecondsPerRun,
		m_fScopeOutputInterval,m_fEpisodeStartToStart,m_fRunStartToStart,m_fTrialStartToStart,
		m_fTriggerThreshold;
	int m_lNumSamplesPerEpisode,m_lPreTriggerSamples,m_lEpisodesPerRun,m_lRunsPerTrial,
		m_lNumberOfTrials,m_lAverageCount,m_lClockChange;
	char m_sUnused198[3]; 

	//Variables in Group 4:

	short m_nDrawingStrategy,m_nTileDisplay,m_nEraseStrategy,m_nDataDisplayMode,m_nChannelStatsStrategy,
		m_nMultiColor,m_nShowPNRawData;
	int m_lDisplayAverageUpdate,m_lCalculationPeriod,m_lSamplesPerTrace,m_lStartDisplayNum,
		m_lFinishDisplayNum;
	char m_sUnused234[11];

	//Variables in Group 5:

	float m_fADCRange,m_fDACRange;
	int m_lADCResolution,m_lDACResolution;

	//Variables in Group 6:

	short m_nExperimentType,m_nAutosampleEnable,m_nAutosampleADCNum,m_nAutosampleInstrument,m_nManualInfoStrategy;
	float m_fAutosampleAdditGain,m_fAutosampleFilter,m_fAutosampleMembraneCap,m_fCellID1,m_fCellID2,m_fCellID3;
	char m_sCreatorInfo[17],m_sFileComment[57],m_sUnused366[13];

	//Variables in Group 7:

	short m_nADCPtoLChannelMap[16],m_nADCSamplingSeq[16],m_nSignalType;
//	char m_sADCChannelName[11][16],m_sADCUnits[16][9]={"","","","","","","","","","","","","","","",""},sDACChannelName[11][4],
	char m_sADCChannelName[11][16],m_sADCUnits[16][9],m_sDACChannelName[11][4],
		m_sDACChannelUnits[9][4],m_sUnused1412[11];
	float m_fADCProgrammableGain[16],m_fADCDisplayAmplification[16],m_fADCDisplayOffset[16],m_fInstrumentScaleFactor[16],
		m_fInstrumentOffset[16],m_fSignalGain[16],m_fSignalOffset[16],m_fSignalLowpassFilter[16],m_fSignalHighpassFilter[16],
		m_fDACScaleFactor[4],m_fDACHoldingLevel[4];

	//Variables in Group 8:

	short m_nOUTEnable,m_nSampleNumberOUT1,m_nSampleNumberOUT2,m_nFirstEpisodeOUT,m_nLastEpisodeOUT,
		m_nPulseSamplesOUT1,m_nPulseSamplesOUT2;

	//Variables in Group 9:

	short m_nDigitalEnable,m_nWaveformSource,m_nActiveDACChannel,m_nInterEpisodeLevel,m_nEpochType[10],
		m_nEpochInitDuration[10],m_nEpochDuration[10],m_nEpochDurationInc[10],m_nDigitalHolding[10],
		m_nDigitalInterEpisode[10],m_nDigitalValue[10];
	float m_fEpochInitLevel[10],m_fEpochLevelInc[10],m_fWaveformOffset;
	char m_sUnused1612[9];

	//Variables in Group 10:

	float m_fDACFileScale,m_fDACFileOffset;
	char m_sUnused1628[3],m_sDACFileName[13],m_sDACFilePath[61],m_sUnused1706[13];
	short m_nDACFileEpisodeNum,m_nDACFileADCNum;

	//Variables in Group 11:

	short m_nConditEnable,m_nConditChannel;
	int m_lConditNumPulses;
	float m_fBaselineDuration,m_fBaselineLevel,m_fStepDuration,m_fStepLevel,m_fPostTrainPeriod,
		m_fPostTrainLevel;
	char m_sUnused1750[13];

	//Variables in Group 12:

	short m_nParamToVary;
	char m_sParamValueList[81];

	//Variables in Group 13:

	short m_nAutopeakEnable,m_nAutopeakPolarity,m_nAutopeakADCNum,m_nAutopeakSearchMode,m_nAutopeakSmoothing,
		m_nAutopeakBaseline,m_nAutopeakAverage;
	int m_lAutopeakStart,m_lAutopeakEnd;
	char m_sUnused1866[15];

	//Variables in Group 14:

	short m_nArithmeticEnable,m_nArithmeticADCNumA,m_nArithmeticADCNumB,m_nArithmeticExpression;
	float m_fArithmeticUpperLimit,m_fArithmeticLowerLimit,m_fArithmeticK1,m_fArithmeticK2,m_fArithmeticK3,
		m_fArithmeticK4,m_fArithmeticK5,m_fArithmeticK6;
	char m_sArithmeticOperator[3],m_sArithmeticUnits[9],m_sUnused1930[3];

	//Variables in Group 15:

	short m_nPNEnable,m_nPNPosition,m_nPNPolarity,m_nPNNumPulses,m_nPNADCNum;
	float m_fPNHoldingLevel,m_fPNSettlingTime,m_fPNInterPulse;
	char m_sUnused1954[13];

	//Variables in Group 16:

	short m_nListEnable;
	char m_sUnused1966[81];

	//End of the header variables.

	float* m_pfData;

protected:
	void RemoveExcludedSegments();
	float* m_pfCurrentData;
	long m_nNumCurrentPoints;
	float m_fCurrentInterval;
	void UpdateCurrent(float *pfData);
	void GetCurrent();
	void ReadHeader(FILE* fp);
	float m_fScale;
	float* m_pfAdjustedData;
	float* m_pfPreviousAdjustedData;
	float* m_pfPreviousData;
//	short m_nDataFormatToWrite;
//	float m_fFileVersionNumberToWrite;
//	float m_fHeaderVersionNumberToWrite;
	bool  m_bImportedAsciiAsData;
	const EvlNode m_structEvlNode;
	vector <float> m_vectfScale;
	bool *m_pbExcluded;
	CString m_sAbfFileName;





public:
	void updateAmp(AbfNode aNode,int indexI, int indexF);
	void UpdateSampleInterval(float nSampleInterval);
	float GetSampleInterval();
	structEventListNode GetEventList();
	void WriteExtrema(FILE *fpOut);
	void UpdateData(AbfNode NewNode);
	void ResetInstrumentScaleFactor(int nChannelIndex, float NewScaleFactor);
	void ResetSampleInterval(float fNewSampleInterval);
	void SetScale(int nChannelIndex, float fFold);
	float GetMaxAmp(int nChannelIndex);
	void WriteData(FILE* fpAbfOut, int nDataFormat);
	void ChannelNameUnit(CString sChannelName, CString sChannelUnit, int nChannelIndex);
	void UnitaryScale(int nChannelIndex);
	void EqualizeScale();
	AbfNode GetAdjustedTrace();
	void UpdateData(float *pfData);
	void UpdateSize(int nSize);
	int GetDataSize();
	void BackupData();
	void WriteComparison(FILE* fp, int nKey);
	void WriteHeader(FILE* fpAbfOut);
	void ExtractSegments(FILE* fp, structSegmentExtractionNode aNode);
	AbfNode GetTrace();
	void ReplaceSegments(CHistory* pcHistory);
	void UpdateAdjustedData();
	void RemoveArtifacts(EvlNode structEvlNode);
	void BallRolling(CHistory* pcHistory);
	void ImportAsciiAsData(FILE* fpAbfIn);
	void MakeAllPointHistgram(CString sFileName);
	void PreparePointers();
	void WriteAdjustedData(FILE* fpAbfOut);
	void BaslieAdjustment();
	void WriteData(FILE*);
	void WriteAbfHeader(FILE*);
	void WriteAbfBody(FILE*);
	int ReadData(CString sFileName);
	void ReadData(FILE*);
	CAbf();
	virtual ~CAbf();

};

#endif // !defined(AFX_ABF_H__829A0817_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
