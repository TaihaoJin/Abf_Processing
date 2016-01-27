// BurstHandling.h: interface for the CBurstHandling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BURSTHANDLING_H__51D55847_C975_4D21_9B9F_0A532E82331A__INCLUDED_)
#define AFX_BURSTHANDLING_H__51D55847_C975_4D21_9B9F_0A532E82331A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"
#include <vector>
#include <list>
#include "LinearHistogramDouble.h"
#include "LinearHistogramHandler.h"
#include "BurstingTimeCourseOptionDlg.h"
#include "IndexOrganizerHist.h"

using namespace std;

class CBurstHandling  
{
public:
	void excludeClusters(vector <int> vnIndexes);
	void excludeBursts(vector <int> vnIndexes);
	int getClusterID();
	int getBurstID();
	void updateID();
	void MakeBurstNodes(double dTcb, double dTcc);
	void MakeClusterNodes(double dTcb, double dTcc);
	void MakeClusterNodes();
	structClusterNodes getClusterNodes();
	structBurstNodes getBurstNodes();
	void MakeBurstNodes(double dTcb);
	vector <int> getEventIndexes(int burstIndex);
	void ExportBurstcorrelations_scattered();
	void ExportBurstcorrelations();
	void adjustEventStarts(int index0, int index1);
	void exchange(short* pstData, int index0, int index1);
	void exchange(float* pfData, int index0, int index1);
	void exchange(int* pnData, int index0, int index1);
	void exchange(bool* pbData, int index0, int index1);
	void exchangeEvents(int index0, int index1, bool bAdjustEventStart);
	void exchangeEvents(int index0, int index1);
	void randomizeEvents(int index0,int index1, int& nSeed);
	void reverseRecording();
	void filteringClusters();
	structBurstingTimeCourseOptionNode getInitializedParameters();
	void filteringBurstNodes();
	void obtainParameters();
	void calSubCondLevelWeights(double *pdLW, vector <int> vnIndexes, int nLevels);
	structMeanSemNode getDwellTimeMS(vector <int> vnIndexes, int nLevel, int nGroupSize);
	structMeanSemNode getDwellTimeMS(vector <int> vnIndexes, int nLevel);
	structMeanSemNode CBurstHandling::getAmpMS(vector <int> vnIndexes);
	void exportBurstEvolutions();
	int conductanceLevel(int index);
	void buildConductanceLevels();
	void importMatchingDataFile();
	void pickSubConductance(bool* pbExcluded, int lI, int lF);
	typedef vector<int> intv;
	void buildBurstEvolutionHist();
	void pickIntraburstEvents(bool *pbExcluded, double dLatancy);
	void pickIntraburstEvents(bool* pbExcluded, int nLatancy);
	double interpolation(double x1, double y1, double x2, double y2, double x);
	void burstBaseline(int index, int& ni, int& nf, double& dl, double& dr);
	void getEventIndexAPH(structBurstingTimeCourseOptionNode aNode, CLinearHistogramDouble &APH1,CLinearHistogramDouble &APH2, CLinearHistogramDouble &APHn1,CLinearHistogramDouble &APHn2);
	void SmoothingBTC_UB();
	int GetNumBursts();
	void MakeBurstTimeCourse_UB(float fBinWidth);
	vector <structBurstTCBinNode> GetBurstTimeCourst_UB(float fBinWidth);
	void FilteringBurstNodes_NOPB(int nNOPB1,int nNOPB2);
	void FindingRange(double& dMin, double& dMax, int& nMin, int& nMax);
	void SortBurstNodes(int nIndex0);
	void PickIsolatedOpenings(bool *pbExcluded);
	void PickLastOpenings(bool *pbExcluded);
	void PickFirstOpenings(bool *pbExculed);
	void PickIntraBust(int nEdge,bool *pbExcluded);
	void PickEvents_NOPB(int nNOPB1,int nNOPB2, bool* pbExcluded);
		//This function picks openings in burst with NOPB in the range of [nNOPB1,nNOPB2]
	structExponentialFittingNode DummyResultNode(int nComponents);
	void FitTotalOpentimePerBurst(FILE* fpTotal, int nFuncKey);
	void FitBurstDurations(FILE* fpTotal, int nFuncKey);
	void ExportGeometricCurves(CString sFileName, structNumOpenPerBurstFittingNode aResultCollection,
										   vector <structHistNode> aGeoHist);
	void ExportGeometricCurves(CString sFileName, vector <structGeometricFittingNode> avResultNodes,
										   vector <structHistNode> aGeoHist);
	double Geometric(double dRol, double dX);
	void FitNumOpeningPerBurst(FILE* fpTotal);
	void ExcludeAllEvents(int nLevel);
	void CheckBurst();
	void PickInterBurstIntervals(bool *pbExcluded);
	void OutputBurstingTimeCourse();
	void ExportBurstStatitics();
	void MakeClusterNodes(float fCutoff);
	void MakeBurstNodes(structBurstingTimeCourseOptionNode aNode);
	void MakeBurstNodes();
	CBurstHandling();
	void UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
						short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, AbfNode aNode);
	void UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
						short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded);
	void UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
						short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, int nEvlID);
	virtual ~CBurstHandling();
protected:
	void buildBurstCorrelationHists();
	void randomizeIntraburstEvents(int nLatancy);
	float* m_pfAmpc;
	void triangulize(float * pfData, int szie);
	void WriteBurstingTimeCourse_UB(FILE *fpOut, CString sFileName);
	void WriteBurstingTimeCourse(FILE *fpOut);
	void WriteSortedHistogram(FILE* fpAbfOut);
	structSemNode CalSem(vector <double> vectfVect);
	structBurstSemNode CalBurstSem(int nIndexI, int nIndexF);
	structBurstingTimeCourseOptionNode m_structBurstingTimeCourseOptionNode;
	double GetSortingVariable(int nIndex);
	bool FirstBurst(int nBurstIndex);
	float MFo_0(int nIndex0, int nIndex1);
	float MTo_0(int nIndex0, int nIndex1);
	float MIBC_0(int nIndex0, int nIndex1);
	float MAmp_0(int nIndex0, int nIndex1);
	float Moib_0(int nIndex0, int nIndex1);
	float MTcib_0(int nIndex0, int nIndex1);
	float MFoib_0(int nIndex0, int nIndex1);
	float MBTo_0(int nIndex0, int nIndex1);
	void WriteClusterTimeCourse(FILE *fpOut);
	intv* m_ivEventHistT, m_ivEventHistN;
	void CalClusterAve();
	bool InterClusterClosing(int nIndex);
	void OutputBSTFittingResults(structBSTFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex);
	void OutputBSTFittingResults(structExponentialFittingNode aTrialNode, structExponentialFittingNode aResultNode, FILE* fpOutput, int nDetailIndex);
	void OutputTOTPBFittingResults(structTOTPBFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex);
	void OutputTOTPBFittingResults(structExponentialFittingNode aTrialNode, structExponentialFittingNode aResultNode, FILE* fpOutput, int nDetailIndex);
	void ExportExponentialCurves(CString sFileName, vector <structExponentialFittingNode> avResultNodes,
										   vector <structHistNode> aExpHist);
	void ExportExponentialCurves(CString sFileName, structTOTPBFittingNode aResultCollection,
										   vector <structHistNode> aExpHist);
	void ExportExponentialCurves(CString sFileName, structBSTFittingNode aResultCollection,
										   vector <structHistNode> aExpHist);
	vector <structHistNode> RemoveEmptyBins(vector <structHistNode> avHist);
	vector <structExponentialFittingNode> GetTOTPBTrials(structExponentialFittingNode aTrialNode);
	vector <structExponentialFittingNode> GetBSTTrials(structExponentialFittingNode aTrialNode);
	void CalMeanNOPB();
	void OutputNOPBFittingResults(structNumOpenPerBurstFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex);
	void OutputNOPBFittingResults(structGeometricFittingNode aTrialNode, structGeometricFittingNode aResultNode, FILE* fpOutput, int nDetailIndex);
	double CBurstHandling::Geometric(double dRol[], double dW[], int nDim, double dX);
	void ExportNOPBCurves(structNumOpenPerBurstFittingNode aNodeCollection);
	bool Significant(structExponentialFittingNode aNode, structExponentialFittingNode bNode, double& dL, int& nDf, double& dProb);
	bool Significant(structGeometricFittingNode aNode, structGeometricFittingNode bNode, double& dL, int& nDf, double& dProb);
	vector <structGeometricFittingNode> GetGeometricFittingTrials(structGeometricFittingNode aTrialNode);
	void SortFittingResults(vector <structGeometricFittingNode> &avResultNode);
	void SortFittingResults(vector <structExponentialFittingNode> &avResultNode);
	double CalBinPosition(structHBHistParNode aNode, int nIndex);
	int CalIndex_HB(structHBHistParNode aNode, double dX0);
	void MakeBurstDurationHist_HB();
	void MakeBurstDurationHist_Log();
	void MakeTOTHist_HB();
	void MakeTOTHist_Log();
	void MakeNOPBHist();
	float MAmp(int nIndex0, int nIndex1);
	void CalMDTSem_EventIndex();
	void CalMDTSem_EventIndex_Cluster();
	void WriteBMDT_Cluster(FILE *fpOutput);
	void WriteBMDT(FILE* fpOutput);
	void CalAmpSem_EventIndex();
	void CalAmp_EventIndex();
	float Amp(int index);
	int LevelLength(int nBurstIndex, int nEventIndex);
	void WriteLevelAmpHist(FILE* fpOutput);
	void WriteLevelAmpHist2(FILE* fpOutput);
	void evlAmpRange(double &dmin, double &dmax);
	void buildLevelAmpHist_latancy();
	void buildLevelAmpHist();
	float Amp(int nBurstIndex, int nEventIndex);
	void CalMDT_EventIndex();
	void CalMDT_EventIndex_Cluster();
	float MeanAmp(int nIndex0, int nIndex1, int nEventIndex, int nLevel);
	int CBurstHandling::NextEvent(int nPosition, int nLevel0, int nShift0, int delta);
	int NextEvent(int nPosition, int nLevel, int nShift);
		//Including nPosition, the function returns the nPosition of the (nShift0+1)-the event whose level 
		//is nLevel0. e. g., it return the first it find when nShift==0, and the second when nShift==1, etc.
	float MDT(int nIndex0, int nIndex1, int nEventIndex, int nLevel);
		//This function return the average open time of the n-the dwelling time (at level=nLevel)
		//of each burst, where n=nEventIndex.
		//It count from the end of each burst when nEventIndex is negative.
		//abs(nEventIndex) should be >=1
		//nLevel should be 0 or 1
	float CBurstHandling::MDT_Cluster(int nIndex0, int nIndex1, int nLevel);
		//This function returns the mean dwelling time (at level=nLevel)
		//of the n-th burst of the cluster nodes nIndex0, where n=nEventIndex1.
		//It counts from the end of the cluster when nIndex1 is negative.
		//abs(nEventIndex) should be >=1
		//nLevel should be 0 or 1

	int FirstOpening(int nPosition);
		//This function returns the index for the first open event after the event at nPosition.
		//This function does not check the event at nPosition.
	int logDTIndex(float minDT, float fDT, int nDivPerDecade);
	void WriteTMP(FILE* fpOutput);
	float TotalLength(int nIndex0, int nIndex1);
	float Moib(int nIndex0,  int nIndex1);
	float MTcib(int nIndex0, int nIndex1);
	float MFoib(int nIndex0, int nIndex1);
	float MBTo(int nIndex0, int nIndex1);
	float TotalBurstLength(int Index0, int nIndex1);
	float TotalOpenTime(int nIndex0, int nIndex1);
	float ElapsedTime(int nIndex0, int nIndex1);
	float MIBC(int nIndex0, int nIndex1);
	float MTo(int nIndex0, int nIndex1);
	float MTc(int nIndex0, int nIndex1);
	float MFo(int nIndex0, int nIndex1);
//	void WriteBurstingTimeCourse(FILE *fpOut);
	void WriteBurstingTimeCourse1(FILE *fpOut);
	CString ChangeFileExt(CString sFileName, CString sExt);
	bool InterBurstClosing(int nIndex);
	vector <structBurstNode> m_structvBurstNodes;
	vector <bool> m_vbValidBurstNodes;
	vector <structClusterNode> m_structvClusterNodes;
	CString ChangeExt(CString sFileName, CString sNewExt);
	int m_nNumLevels;
//	int m_nBinSize;
	int m_nNumBins;
	int m_nNumChannels;
	float m_fTimePerPoint;
	float m_fMaxBurstDuration;
	float m_fMaxOpenTime;
	float m_fMaxTOT;
	int m_nNumEvents;
	int* m_pnLevelStart;
	float* m_pfAmplitude;
	float* m_pfBaseline;
//	float m_fBinWidth;
	int* m_pnLevelLength;
	short* m_pstEventCurrentLevel;
	bool* m_pbExcluded;
	bool m_bBurstNode;
	bool m_bClusterNode;
	bool m_bNOPBHist;
	bool m_bTOTPBHist_HB;
	bool m_bTOTPBHist_Log;
	bool m_bBurstDurationHist_HB;
	bool m_bBurstDurationHist_Log;
	bool m_bBurstCorrelationHist;
//	float m_fBurstDelimiter;
//	float m_fClusterDelimiter;
	CString m_sEvlFileName;
	float m_fRecordLength;
	CString m_sTcrFileName;
	AbfNode m_structAbfNode;
	vector <structBurstTCBinNode> m_vstructBurstTCBinNodes;

	float *m_pfTc_Time0,*m_pfTo_Time0;
		//m_pfTc_Time0, m_pfTc_Time1 store the mean closed-time of the n-th closed events 
		// (0 and 1 count from the begining and end of a burst).
		//m_pfTo_Time0, m_pfTo_Time1 store the mean closed-time of the n-th open events 
		// (0 and 1 count from the begining and end of a burst).
	float *m_pfTc_Sem0,*m_pfTo_Sem0;
	float *m_pfTc_Num0,*m_pfTo_Num0;
	float *m_pfTc_Time1,*m_pfTo_Time1;
	float *m_pfTc_Sem1,*m_pfTo_Sem1;
	float *m_pfTc_Num1,*m_pfTo_Num1;


	CLinearHistogramDouble m_cLevelAmpHist01,m_cLevelAmpHist01r,m_cLevelAmpHist02,m_cLevelAmpHist02r,m_cLevelAmpHist11,m_cLevelAmpHist11r,m_cLevelAmpHist12,m_cLevelAmpHist12r;
	CLinearHistogramDouble m_cAPH1,m_cAPH2,m_cAPH1r,m_cAPH2r;
	CLinearHistogramDouble m_cAPH0;//histogram of level zero
	

	float *m_pfAmp_Ave0;
	float *m_pfAmp_Sem0;
	float *m_pfAmp_Num0;
	float *m_pfAmp_Ave1;
	float *m_pfAmp_Sem1;
	float *m_pfAmp_Num1;

	float *m_pfBMDTc_Time0,*m_pfBMDTo_Time0;
		//m_pfBMDTc_Time0, m_pfBMDTc_Time1 store the mean closed-time of the n-th burst in the clusters 
		// (0 and 1 count from the begining and end of a burst).
	float *m_pfBMDTc_Sem0,*m_pfBMDTo_Sem0;
	float *m_pfBMDTc_Num0,*m_pfBMDTo_Num0;
	float *m_pfBMDTc_Time1,*m_pfBMDTo_Time1;
	float *m_pfBMDTc_Sem1,*m_pfBMDTo_Sem1;
	float *m_pfBMDTc_Num1,*m_pfBMDTo_Num1;

	CLinearHistogramHandler m_cLAHHandler;
	int m_nMaxOpeningNum;
		//The largest number of opening in a burst observed in m_structvBurstNodes.
	int m_nMaxBurstingNum;
		//The largest number of burst in a cluster observed in m_structvClusterNodes.
//	int m_nMaxOpening;
		//Maximum number of opening in a burst to include the bursting event in the statistics. 
		//This is a user inputed cutoff value.
//	int m_nMinOpening;
		//Minimum number of opening in a burst to include the bursting event in the statistics.
		//This is a user inputed cutoff value.
	int m_nNumConductanceLevels;
	bool m_bLevelAPHBuilt;
	int m_nBurstDelimiterIndex;
	double m_dMeanTOT;//Mean total open time per burst
	double m_dMeanBurstDuration;
	double m_dMeanNOPB; //Mean number of opening per burst
	vector <structHistNode> m_vPoHist_Log;
	vector <structHistNode> m_vNumOpenPerBurstHist;
	vector <structHistNode> m_vTotalOpenTimeHist_HB;
	vector <structHistNode> m_vBurstDurationHist_HB;
	vector <structHistNode> m_vTotalOpenTimeHist_Log;
	vector <structHistNode> m_vBurstDurationHist_Log;
	vector <int> m_vnSortedOrder;  //nOrder=m_vnSortedOrder[i] indicates that the nOrder-th burt node has the i-th smallest value of the variable corresponding to m_nSortingIndex, e.g mean open time or amplitude.  
	vector <int> m_vnRanking; //nRanking=m_vnRanking[i] indicates that the i-th burst node has the nRanking-th smallest value of the variable corresponding to m_nSortingIndex, e.g mean open time or amplitude.
	int    m_nSortingIndex;
	CIndexOrganizerHist m_cLatancyIndexesTf,m_cLatancyIndexesNf,m_cLatancyIndexesN;
	CIndexOrganizerHist m_cLatancyIndexesTr,m_cLatancyIndexesNr,m_cLatancyIndexesT;
	CIndexOrganizerHist m_cBurstDurationIndexes,m_cNOPBIndexes;
	CIndexOrganizerHist m_cBurstDurationIndexes_b,m_cNOPBIndexes_b;
	int m_nEvlID;
	int m_nBurstID;
	static int m_nSN;
};

#endif // !defined(AFX_BURSTHANDLING_H__51D55847_C975_4D21_9B9F_0A532E82331A__INCLUDED_)
