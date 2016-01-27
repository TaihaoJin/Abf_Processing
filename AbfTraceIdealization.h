// AbfTraceIdealization.h: interface for the CAbfTraceIdealization class.
//
//////////////////////////////////////////////////////////////////////

#include "structures.h"
#include "OneDHistogram.h"
#if !defined(AFX_ABFTRACEIDEALIZATION_H__B83FA5FC_DF7C_4E17_936B_A1E063F226CC__INCLUDED_)
#define AFX_ABFTRACEIDEALIZATION_H__B83FA5FC_DF7C_4E17_936B_A1E063F226CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAbfTraceIdealization  
{
protected:
	void CAbfTraceIdealization::AttachToEventList(vector <structEventNode> &aList, int nIndexI, int nIndexF);
	void AttachToEventList(vector <structEventNode> &aList, structEventNode aEventNode);
	void AttachToEventList(vector <structEventNode> &aList, vector <structEventNode> bList);
	structEventNode MergeEvents(structEventNode aNode, structEventNode bNode);
	vector <structEventNode> CalEvents_Amp(int nIndexI,int nIndexF);
	int NextAnchor(int nIndex);
	void MergeEvents(structEventNode aNode, int nIndex);
	short WhatLevel_Amp(int nPosition);
	float BiggestPoint(float* pfData, int nSize, int nRank);
	float CalSubConductance(int nNI0, int nNF0, float fDeltaRange);
	float CalSubConductance(int nNI0, int nNF0, float fDeltaRange, short stLevel0);
	float CalConductLMax(int nNI0, int nNF0, short stLevel0);
	float CalConductLMin(int nNI0, int nNF0, short stLevel0);
	vector <structEventNode> CalEvents0(int nIndex0,int nIndex1,float fThreshold, short stLevel0);
	vector <structEventNode> CalEvents(int nIndex0,int nIndex1,float fThreshold, short stLevel0);
	float CalBaseline(int nIndex);
	int NextLongClosedEvent(int nIndex, int nMinLength);
	float CalConductLMax(int nNI, int nNF);
	void SubConductRefinement();
	int CAbfTraceIdealization::ApplyCorrectEvent(structEventNode aNode, int nPosition);
	void CAbfTraceIdealization::RemoveSSC();
	void NextLevel(short stLevel, short &stNextLevel, float fAmp, float &fThreshold, int nSign);
	void CalRunTimeLTC(int nI0, int nLevel);
	void ExportLocalExtremaHist();
	void GetLTC_FromTrace();
	structpClampResultNode ReadpClampResultFile();
	void GetTraceSegments();
	int ClosestPoint_AA(int *pnData, int nLength, int nValue, int nKey);
	void ExtractTrace(int nI0, int &nNI, int &nNF, int &nLevel);
	void SortAsending(float* pfData,int *pnOrder,  int nSize);
	int WhatLevel(float fAmp, int nPhase);
	void LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	structSemNode CalSem_Amp(float *pfData, int nBeginning, int nSize, int nDelta);
	int LevelEnd(short stLevel, short stNextLevel, int nI0);
	bool DownTransition(int nI0, int nLag, short stLevel, int nLevelBeginning, short &stNextLevel, int &nLevelEnd, float &fAmpMean, float &fAmpDev, float &fAmpMin, float & fAmpMax, bool &bAnchor);
	bool UpTransition(int nI0, int nLag, short stLevel, int nLevelBeginning, short &stNextLevel, int &nLevelEnd, float &fAmpMean, float &fAmpDev, float &fAmpMin, float & fAmpMax, bool &bAnchor);
	vector <structLTCNode> m_vstructLTC; //Level Transition Criteria
	vector <structEventNode> m_vstructEvents;
	int m_nMaxLevel;
	int m_nSize;
	float* m_pfData;
	int* m_pnLMaxima;
	int* m_pnLMinima;
	int m_nLMax;  //mumbers of the local maxima
	int m_nLMin;  //mumbers of the local minima
	int m_nChannels;
	float m_fTimePerPoint;
	CString m_sEvlFileName;
	typedef vector <float> VectFloat;
	vector <VectFloat> m_vvfSegMinima;
	vector <VectFloat> m_vvfSegMaxima;
	vector <structOneDHistogramNode> m_vstructSegMinimaHist;
	vector <structOneDHistogramNode> m_vstructSegMaximaHist;
	short m_stCurrentLevels[4][4];
	structpClampResultNode m_structpClampResultNode;
	CString m_sClampfitReultFileName;
	int m_nLTCWindowSize;
	int m_nLMaxI;
	bool 	m_bLTCFromTraces;
	vector <float> m_vfMeanLMinima;
	vector <float> m_vfMeanLMaxima;
	float m_fThreshold0;
	float m_fThreshold;
	bool m_bRemoveSSC;  //SSC Short small conductance events
	bool m_bCalRTLTC; //RTLTC Runtime level transition criteria
	float m_fLargeNegativeAmp;
	vector <float> m_vfAmpThreshold;
	vector <float> m_vfAmpThreshold0;
	bool m_bSubConductRefinement;
	float CalDeltaRange(int nNI);
	bool m_bAmpThredOnly;
		
public:
	structEventListNode GetEventList();
	void GetLTC();
	void UpdateData(float* pfData, int nSize, float fTimePerPoint);
	void Idealization();
	CAbfTraceIdealization();
	virtual ~CAbfTraceIdealization();
private:
	void MergeEvents(int nIndex0, int nIndex1);
	float CalConductLMin(int nNI, int nNF);
};

#endif // !defined(AFX_ABFTRACEIDEALIZATION_H__B83FA5FC_DF7C_4E17_936B_A1E063F226CC__INCLUDED_)
