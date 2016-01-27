// BallRolling.h: interface for the CBallRolling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALLROLLING_H__BDC97681_961A_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_BALLROLLING_H__BDC97681_961A_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "math.h"
#include <list>
#include <vector>
#include "Abf.h"
using namespace std;
struct structContactPointNode{
	int nBegin;
	int nEnd;
	int nContactingPoint;
	int nLevel;
	int nNumOfPeers;
	int nNumOfSuperiors;
};
class CBallRolling  
{
public:
	void FindingAnchors();
	int CalLevel(int nPreviousLevel, int nPreviousPoint, int nCurrentPoint);
//	bool IsContactingPoint(int nCurrentPoint);
	void BaselineCorrection(int nRulerSize, CHistory* pcHistory);
//	int max (int i, int j);
	CBallRolling(float* pfData, float* pfAdjustedData, int nSizeOfData, float fSampleInterval);
	virtual ~CBallRolling();

protected:
	float m_fAmp;
	bool* m_pbTransitionPoints;
	void FindingTranitionPoints();
	double BallAngle(double dX1,double dX2,double dY1,double dY2,double dR);
	int m_nDelta;

	SegmentNode m_structBrigeSeg;
	float SegAverage(float *pfData, int nLength);
	void BrigeSegment(int nBegin, int nEnd);
	void GetRulerSize();
	void ReversePolarity();
	float m_fSegIgnored;
	float m_fTime1;
	float m_fTime2;
	bool m_bApplyingToEntireTrace;
	int m_nAnchorRank;
	void CalSmoothingParameters(float& fMax, float& fMin, int& nSmoothingWindowSize);
	void Smoothing();
	bool BelowContactingPoints(list <structContactPointNode>::iterator pPrevious, 
							   list <structContactPointNode>::iterator pCurrent,
							   list <structContactPointNode>::iterator pNext);
	void EraseFalseContactingPoints();
	void UpdateLevelHists(int nCurrentPosition, 
						  list <structContactPointNode>::iterator pTemp,
						  list <structContactPointNode>::iterator pTempL,
						  list <structContactPointNode>::iterator pTempR);
	int m_nLevelHistWindowSize;
	void InitializeLevelHists();
	void CalNumOfSuperiors_Peers();
	void CalLevelOfContactingPoints();
	bool LocalMaximumLevel(int nContactingPoint);
	int Transition(int nBigin, int nEnd);
	void CheckContactingPoints();
	float m_fCriterionOfUpTransitions;
	float m_fWindowSize;
	int BiggestPoint(float* pData, int nSize, int nRank, int* pnRank);
	float m_fSampleInterval;
	void GetOptionParameters(CHistory* pcHistory);
	int m_nRulerSize;
	void CalCenterPoisition(float X0, float Y0, float X, float Y);
	float* m_pfAdjustedData;
	void BaselineAdjust();
	float m_fUnitY;
	float m_fUnitX;
	void FindingContactingPoints();
	void FindingContactingPoints_BallRolling();
//	int min (int i, int j);
	void LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	int m_nSizeOfData;
	int m_nSizeOfLocalMaxima;
	int m_nSizeOfLocalMinima;
	int m_nSizeOfContactPoints;
	list <structContactPointNode> m_lstContactingPoints;
	int* m_pnLocalMinima;
	int* m_pnLocalMaxima;
	int m_nRank;
	float* m_pfData;
	float m_fCenterX;
	float m_fCenterY;
	float m_fBallRadius;
	float m_fTransitionThreshold;
	int   m_nLevelHist[1000];
	int   m_nMaxNumOfSuperiors;
	vector <list <structContactPointNode>::iterator> m_pvectAnchors;
	int m_nNumOfAnchors;
	bool m_bCheckContactingPoints;
	bool m_bSmoothingTrace;
	bool m_bTracingMinima;
};

#endif // !defined(AFX_BALLROLLING_H__BDC97681_961A_11D4_83DE_00C04F200B5A__INCLUDED_)
