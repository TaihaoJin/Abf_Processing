// Evl.h: interface for the CEvl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVL_H__829A0818_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_EVL_H__829A0818_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EvlSummary.h"
#include "math.h"
#include "Abf.h"
#include "Structures.h"	// Added by ClassView
#include "BurstHandling.h"
#include "IncidenceSubsetHandler.h"
class CEvl  
{
public:
	void updateAmp(float *pfAmp, int indexI, int indexF);
	CLinearHistogramDouble obtainEventAPH(vector <int> nvIndexes, AbfNode &aNode, double dMinAmp, double dMaxAmp);
	void excludeEvents(vector<int> vnIndexes);
	void WriteEvlBody(FILE *fpEvlOut, structEventNode aNode);
	const CEvl& operator =( const CEvl& aEvl);
	void ExportDwellCurves(CString sFileName, vector <structFittingResultNode> &avResultNode);
	void CEvl::FilteringEvents1();
	void CEvl::FilteringEvents();
	void WriteEvlHead(FILE* fpEvlOut);
	void ExportDwellCurves_Normalized(CString sFileName, vector <structFittingResultNode> &avResultNode);
	void ExportDwellCurves(structFittingResultNode &aResultNode);
	void ExportDwellCurves(structFittingResultNode &aResultNode, bool bEvlRead);
	CString ExtendFileName(CString sFileName, char cInsert);
//	operator =(CEvl& aEvl);
	CString AttachToFileName(CString sFileName, char ch);
	CString ChangeExt(CString sFineName,CString sNewExt);
	void ModeSeparated();
	void CheckBursting();
	void SetMinimumOpenning(int MinimumOpenning);
	void SetBurstDelimiter(float BurstDelimiter);
	void PrepairPointers();
	void WriteEvl(FILE* fpEvlOut, int nWritingKey);
	void WriteEvl(CString);
	void ReadEvl(FILE* fpEvlIn, int nReadingKey, CString sEvlFileName);
	void ReadEvl(FILE* fpEvlIn);
	int  ReadEvl(CString sEvlFineName);
	void setAbfNode(AbfNode &abfNode);
	void getAmpRange(double &dMinAmp, double &dMaxAmp);
	CEvl();
	virtual ~CEvl();

protected:
	bool m_bExcludingLastClosure;
	void ClearvectFRLogBins();
	double ChiSQ(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	void MeanDwellTimeHist(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, double& dMeanH, double& dMeanHC, int nEventsInRange);
	double SSE(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	double SDev(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	float ClosestSamplePT(float fTime);
	void NewBinBoundary();
	void ExportDwellCurves_Normalized(structFittingResultNode &aResultNode);
	void MakeFLogHist_old(structFRange fFittingRange);	
	void MakeEvlHistogram_HB();
	double CumulatedDwellProb(double dT[20], double dW[20], int nComponents, double dTime);
	void PassingLogHistBinRange(int &nFirstBin, int &nLastBin, int nLevel, structFRange fFittingRange);
	int MaxLevel();
	void MakeEvlHistogram();
	structEvlHistogramNode m_structEvlHist;
	void SetChannelNumbers(int nChannel);
	void MoveEvent(int i0, int i1);
	void MergeToRightEvent(int i0, int i1);
	void MergeThreeEvents(int i0, int  i1);
	void MergeToLeftEvent(int i0, int i1);
	int ConcatenateEvents(int i0, int&  i1);
	bool IsLocalMinimum(int i0, int i1);
	bool IsLocalMaximum(int i0, int i1);
	bool IsLocalExtremum(int i0, int i1);
	void SetChannelNumber();
	void GetChannelNumbers();
	void LocalExtrema(const float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	void ExcludeLowAmp();
	void ResetAttachingStatus();
	void ShiftLevelStart();
	void InsertOldEvl();
	void PreservePointers();
	void WriteBurstBody(FILE* fpOut, int nIndex);
	void WriteEvlBody(FILE *fpEvlOut, int nIndex, int nShiftLevelStart, int nShiftLevelLength);
	void WriteEvlBody(FILE* fpEvlOut, int nIndex);
	int m_nMinimumOpenning;
	int m_nRecordLength_Actual;
		//m_nRecordLength_Actual = the sum of the length of all events
	int m_nRecordLength_Formal;
		//m_nRecordLength_Formal = the sum of the starting position (in the unit of sampling interval) and the length of the last event.
		//Usually there are difference in these two quantities (m_nRecordLength_Formal > m_nRecordLength_Actual), although they are indetical in the physical meaning.
	float m_fBurstDelimiter;
	void CountEventNum(FILE*);
	int m_nNumEvents;
	int m_nNumChannels;
	char m_FileId[9],m_DataName[13],m_ADCUnit[9],m_Comment[129],m_Reserved[69];
	short m_FFversion,m_InterpoloationFacto,m_EpisodeSize,m_TriggerStart,m_TriggerEnd,m_NumLevels;
	float m_AnalysisFilter,m_Holding,m_AcquisitionFilter,m_Gaps;
	float m_TimePerPoint;
	CString m_sEvlFileName;
	int m_nMaxLevel;
	int m_nFilteringKey;
	int m_nReadingKey;
	int m_nWritingKey;
	int*   m_pnLevelStart;			 
	float* m_pfEventStandardDev;
	float* m_pfAmplitude;
	int*   m_pnLevelLength;	
	short* m_pstEventCurrentLevel;
	short* m_pstEventNotes; 
	float* m_pfBaseline;
	bool*  m_pbMultipleOpenning;
	float* m_pfEffLevel;
	int*   m_pnMRGEvents;

	int*   m_pnLevelStart0;
	float* m_pfAmplitude0;
	int*   m_pnLevelLength0;				 
	float* m_pfEventStandardDev0;
	short* m_pstEventCurrentLevel0;
	short* m_pstEventNotes0; 
	float* m_pfBaseline0;
	int m_nNumEvents0;
	int m_nEvlLength0;
	int* m_pnValidEvent0;
	float m_fRecordLenth;

	int*   m_pnBurstStartingEvent;
	int*   m_pnNumOpenningInBurst;
	int*   m_pnBurstEndingEvent;
	int*   m_pnBurstLength;
	bool   m_bModeSeparated;
	bool   m_bAttachingEvl;
	bool   m_bEvlHist;
	bool   m_bSubsets;
	int    m_nNumBursts;
	int*   m_pnValidEvent;
	bool*  m_pbExcluded;
	float m_fAmpThreshold;
	CEvlSummary m_cSummary;
	AbfNode m_structTrace;
	char   m_cNumChannels[12];
	int    m_nChannelNumberPosition;
	int m_nMaxNumEvents;

	bool   m_bHBEvlHist;
	bool   m_bLogEvlHist;
	vector <structIncidenceSubsetCriteriaNode> m_vstructIncidenceSubsetCriteriaNodes;
	vector <structBurstNode> m_vstructBurstNodes;
	vector <structClusterNode> m_vstructClusterNodes;
	int m_nEvlID;
	AbfNode* m_pstructAbfNode;
	bool m_bAbfNode;
	float m_fRisetime;

public:
	void obtainEventAPH(CLinearHistogramDouble);
	void updateBurstHandler(CBurstHandling &aBH);
	float getTimePerPoint();
	void updateID();
	int getEvlID();
	static int m_nSN;
	void updateIncidenceSubsetHandler(CIncidenceSubsetHandler &ish);
	bool isClusterDelimiter(CString sName);
	bool isBurstDelimiter(CString sName);
	int getIncidenceSubsetCriterionID(CString sID);
	void readIncidenceSubsetCriteriaFile(FILE* fp);
	void importSubsetCriteriaNodes();
	void buildSubsets();
	void ExportBurstcorrelations();
	void filteringEvents(structBurstingTimeCourseOptionNode aNode);
	void exportBurstEvolutions();
	void filteringEvents();
	void readAtf(CString fileName);
	void readAtf();
	void APH_EventIndex(structBurstingTimeCourseOptionNode aNode, AbfNode aAbfNode, CLinearHistogramDouble &APH1, CLinearHistogramDouble &APH2,  CLinearHistogramDouble &APHn1,  CLinearHistogramDouble &APHn2);
	void SetComment(CString sComment);
	void DoBurstingTimeCourse_UB(AbfNode aNode);
	void ReverseOpenAndClosed();
	CString GetEvlFileName();
	float RightEvents(float fTc, float fTau, float fFittedArea, double dW, int nEventsInRange, float fTmin, float fTmax);
	float LeftEvents(float fTc, float fTau, float fFittedArea, double dW, int nEventsInRange, float fTmin, float fTmax);
	float MeanDwellTimeInRange(float fT1, float fT2, float fTau);
	float DwellTimeInRange (float fT1, float fT2, float fTau);
	float ClosedTimeInRange(float fMin, float fMax);
	void MeanBSTCorrection(FILE *fpTotal, structFittingResultNode aResultNode, float fBDelimiter, int nBurstIndex);
	structEventListNode GetEventList();
	void EvlCorrection();
	void UpdateEvents(structEventListNode aNode);
	void UpdateInterval(float fTimePerPoint);
	void CEvl::FitBST(float fCutoff0, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal, int nFuncKey);
	//BST = Burst duration
	void CEvl::FitBST(FILE* fpTotal, int nFuncKey);
	//BST = Burst duration
	void CEvl::FitTOTPB(float fCutoff0, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal, int nFuncKey);
	void CEvl::FitTOTPB(FILE* fpTotal, int nFuncKey);
	void FitNOPB(float fCutoff, int nBinSize, int nMaxOpening, int nMinOpening, int nIndex, FILE* fpTotal);
	void FitNOPB();
	void SetConfidanceRange(int nFuncKey, vector <structFittingResultNode> &avResultNodes);
	void ExcludeAllEvents();
	void PickSubconductance(int nKey);
	void FilteringEvents(int &nKey, float &fBurstDelimiter, float &fClusterDelimiter);
	void FilteringEvents(int &nKey, structBurstingTimeCourseOptionNode aNode);
	void ExcludeSubconductances();
	AbfNode GetAmpAsTrace();
	AbfNode GetDwellingTimeAsTrace(float fMaxAmp);
	void ExportBurstStatitics();
	CBurstHandling GetBurstHandlingObject();
	void PrepairEvlHistogram();
	void Export2DEvlHistogram();
	void DoBurstingTimeCourse();
	void IgnoreShortClosing_Original(float fCutoff);
	double MeanDwellTimeInRange(structFittingResultNode aResultNode);
	double MeanFittedDwellTime(structFittingResultNode aResultNode);
	int EventsInRange(int nDwellLevel);
	double Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight);
	void Random_Exp(double dT[], double dW[], int nNumComponents, int nSize, double* pdData);
	void SimulateEvl(double dT[], double dW[], int nNumComponents);
	float GetLongestLevelLength(structFRange aRange, int nFittingLevel);
	structFRange GetFittingRange(structFRange aRange, int nFittingLevel);
	CString ExtendFileName(CString sFileName, CString sInsert);
	CString ConvertToString(int nInt);
	structFittingResultNode FitEvl_Transform(int nFuncKey, structDwellFittingTrialNode aNode);
	structFittingResultNode FitEvl(int nFuncKey, structDwellFittingTrialNode aNode);
	void MakeFLogHist(structFRange fFittingRange);
	structEvlHistogramNode GetEvlHistogram();
	void ExportHistograms(FILE* fpHist);
	void OutputSegment(FILE* fpOutput, CString sFileName, float  fTime1, float fTime2, int nChannelNum);
	void IgnoreSmallTransitions(float fCutoff);
	void Summarize();
	AbfNode GetEvlAsTrace();
	AbfNode GetEvlAsTrace(int nKey);
	EvlNode GetEvlNode();
	void RemoveDegeneracy(int nMultp);
	void ReAssignAmp(AbfNode structTrace);
	void AmpHistogram();
	void ReadEvl(CString sFileName1, CString sFileName2);
	void IgnoreShortOpenings(float fCuttoff);
	structEvlSummaryNode GetSummaryNode();
	void ExportSummary(FILE* fpSummary);
	void ExportSummary(FILE* fpSummary, int nCount);
	void ExportAsciiEvl(FILE* fp);
	void IgnoreShortClosing(float fCutoff);
	void ReadAscIIFormatOfTakFitFile(CString sEvlFileName);
	void DoTimeCourse(float fBinSize);
	void CEvl::DoTimeCourse0(float fBinSize);
	void SetAttachingStatus();
	void WriteBurstAsEvl(FILE* fpOut);
	void TrimmingMulOpenning();
	float calSamplingInterval(vector <double> fvdt, double delta, int sigDigits);
	void round(double &d0, int sigDigits);
	void trim(double &d, int sigDigits);
};

#endif // !defined(AFX_EVL_H__829A0818_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
