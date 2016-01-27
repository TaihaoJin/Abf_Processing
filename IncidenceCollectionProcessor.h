#pragma once
#include "structures.h"
#include <vector>
#include "FileNameHandling.h"
#include "LinearHistogramDouble.h"
#include "IndexOrganizerHist.h"
//#include "Evl.h"
#include <vector>
using namespace std;

class CIncidenceCollectionProcessor
{
public:
//	void updateEvlAmp(CEvl &cEvl, int indexI, int indexF);
	CIncidenceCollectionProcessor();
	virtual ~CIncidenceCollectionProcessor();
	float* getAmplitude();
	void setRisetime(float fRisetime);
	void addjustAmplitude(AbfNode aNode);
	void calDwellTimeRanges();
	void exportDwellTimeHistograms();
	CLinearHistogramDouble obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax);
	CLinearHistogramDouble obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax, double dRatio0);
	void exportAPH_Subset(double dMin, double dMax, AbfNode& abfNode, int nWS, int nIT, double dRatio0);
	void addjustTraceBaseline(AbfNode &aNode);
	AbfNode getIncidenceCollectionAsTrace(int nKey, AbfNode structRawTrace);//nKey: 0 for level, 1 for amp.
	CString getEvlFileName();
	CString getCollectionName();
	int getClusterID();
	void updateClusters(structClusterNodes structClusterNodes);
	void updateBursts(structBurstNodes structBurstNodes);
	int getBurstID();
	int getEvlID();
	void updateEvl(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
									int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
									short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, int nEvlID, float fRisetime);
	int CompleteSetSize(int nIncidenceType);
protected:
	double interpolation(double x1, double y1, double x2, double y2, double x);
	int calLatencyT(int nType);//returns the latency (in number of sample point) of the an event from the beginning (nType>0) or end (nType<0) of the burst
	int calLatencyN(int nType);//returns the latency (openings) of the an event from the beginning (nType>0) or end (nType<0) of the burst
	int m_nNumChannels;
	int m_nNumLevels;
	float m_fTimePerPoint;
	int m_nNumEvents;
	int* m_pnLevelStart;
	int* m_pnLevelLength;
	float* m_pfAmplitude;
	float* m_pfBaseline;
	short* m_pstEventCurrentLevel;
	CString m_sEvlFileName;
	bool* m_pbExcluded;
	vector <int> m_vnDwellTimeMin;
	vector <int> m_vnDwellTimeMax;
	bool m_bDwellTimeRanges;
	CFileNameHandling m_cFileNameHandler;

	bool m_bClusterNode;
	bool m_bBurstNode;
	bool m_bValidCriteria;
	structBurstNodes m_structBurstNodes;
	structClusterNodes m_structClusterNodes;
	structIncidenceSubsetCollectionNode m_structSubsets;
	int m_nEvlID;
	int m_nBurstID;
	int m_nClusterID;
	int m_nISCID;//incidence subset criteria id
	int m_nRecordingLength;
	static int m_nSN;
	int m_nRising;
	vector <float> m_vfAmp;
	bool m_bRisetime;
	float m_fRisetime;
	AbfNode m_structRawTrace;
	bool m_bForwardLatencyT;
	bool m_bForwardLatencyN;
	bool m_bBackwardLatencyT;
	bool m_bBackwardLatencyN;
	int *m_pnForwardLatencyT;
	int *m_pnForwardLatencyN;
	int *m_pnBackwardLatencyT;
	int *m_pnBackwardLatencyN;
	structIncidenceCollectionNode m_structIncidenceCollection;
	CIndexOrganizerHist m_cIncidenceIndexHist;
	void buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex);
public:
	void updateCollection(structIncidenceCollectionNode aICNode);
	void clearCollection();
	int getPropertyValue(int nIncidenceType, int nPropertyType, int nIndex, double &dValue, int &nValue, int &nPrintingPrecision);
	int getPropertyType(CString sPropertyName);
	void buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex, int nScaleType, double dBase, double dDelta, int nDim);
	void buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex, int nScaleType, vector <structValueRangeDouble> vRanges);
	void exportGroupPropertyStatitics(vector <CString> vsPropertyTypes,vector <int> vnRelatingIndexes,vector <int> vnPrintingPrecisions);
};
