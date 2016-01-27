#pragma once
#include "structures.h"
#include <vector>
#include "FileNameHandling.h"
#include "LinearHistogramDouble.h"
//#include "Evl.h"
using namespace std;

class CIncidenceSubsetBuilder
{
public:
//	void updateEvlAmp(CEvl &cEvl, int indexI, int indexF);
	CIncidenceSubsetBuilder();
	virtual ~CIncidenceSubsetBuilder();
	vector <structIncidenceSubsetCriteriaNode> getISC_AllEvents();
	vector <structIncidenceSubsetCriteriaNode> getISC_AllBursts();
	vector <structIncidenceSubsetCriteriaNode> getISC_AllClusters();
	float* getAmplitude();
	void setRisetime(float fRisetime);
	void addjustAmplitude(AbfNode aNode);
	void calDwellTimeRanges();
	void exportDwellTimeHistograms();
	void loadIncidenceSubsetCriteriaNodes(vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes);
	void appendSubsets(vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes);
	void appendSubsets(CString sFileName);
	void clearSubsets();
	void loadIncidenceSubsetCriteriaNodes(CString sFileName);
	void loadIncidenceSubsetCriteriaNodes();
	void appendSubsets(structIncidenceSubsetCriteriaNode aISCNode);
	CLinearHistogramDouble obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax);
	CLinearHistogramDouble obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax, double dRatio0);
	void exportAPH_Subset(double dMin, double dMax, AbfNode& abfNode, int nWS, int nIT, double dRatio0);
	void addjustTraceBaseline(AbfNode &aNode);
	AbfNode getSubsetsAsTrace(int nKey, AbfNode structRawTrace);//nKey: 0 for level, 1 for amp.
	CString getEvlFileName();
	CString getISCAsString(structIncidenceSubsetCriteriaNode);
	CString getISCAsString();
	int getClusterID();
	void updateClusters(structClusterNodes structClusterNodes);
	void updateBursts(structBurstNodes structBurstNodes);
	int getBurstID();
	int getEvlID();
	int getISCID();
	void buildSubsets();
	void updateEvl(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
									int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
									short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, int nEvlID, float fRisetime);
	vector <structIncidenceSubsetCriteriaNode> importSubsetCriteriaNodes(CString sFileNone);
	structIncidenceSubsetCollectionNode getSebsets();
	vector <int> getFinalIndexes(int nIncidenceType);
	int CompleteSetSize(int nIncidenceType);
	int getIncidenceSubsetCriterionID(CString sID);
	structIncidenceSubsetNode buildSubset(structIncidenceSubsetCriteriaNode aISCNode);
	vector <int> applySubset(vector <int> aVect, int nIncidenceType, structIncidenceSubsetNode aNode);//nIncidenceType is the type of Incidence pointed by the indexes stored in aVect
	vector <structIncidenceSubsetCriteriaNode> readIncidenceSubsetCriteriaFile(FILE *fp);
protected:
	void fillUpBaseline();
	double interpolation(double x1, double y1, double x2, double y2, double x);
	vector <int> shiftIndexes(vector <int> setA, int nShift, int nSize);
	vector <int> expandIndex(int index, int nType);
	vector <int> expandIndex(int index, int nTypeA, int nTypeB);//nType is th incidence type of the input index
	vector <int> expandIndexes(vector <int> vnIndexes0, int nTypeA, int TypeB);//nTypeA is the incidence type of the indexes of the input indexes in vnIndexes0
																									//nTypeB is the incidence type of the indexes of the returning vector
	void buildClusterNodes(double dTcb, double dTcc);
	bool invalidDelimiter(double dTc);
	bool sameIntervals(double dV1, double dV2);
	vector <int> buildSubset(structIncidenceCriterionNode aICNode);
	bool isClusterDelimiter(CString sName);
	bool isBurstDelimiter(CString sName);
	vector <structIncidenceSubsetCriteriaNode> importSubsetCriteriaNodes();

protected:
	int calLatencyT(int nType);
	int calLatencyN(int nType);
	CString getICAsString(structIncidenceCriterionNode aICNode);
	void updateISCID();
	void buildBurstNodes(double dTcb);
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
	vector<structIncidenceSubsetCriteriaNode> m_vstructISCriteriaNodes;
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
public:
	int getPropertyValue(int nIncidenceType, int nPropertyType, int nIndex, double &dValue, int &nValue, int &nPrintingPrecision);
	int getPropertyType(CString sPropertyName);
};
