// ArtitactsHandling.h: interface for the CArtitactsHandling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARTITACTSHANDLING_H__982156E1_DC16_11D4_AFE0_00C04F200B5A__INCLUDED_)
#define AFX_ARTITACTSHANDLING_H__982156E1_DC16_11D4_AFE0_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SegDeletOptionDialog.h"
#include "History.h"
#include <list>
#include "Structures.h"
using namespace std;


class CArtifactsHandling  
{
public:
	int ClosestPoint_AA(int *nfData, int nLength, int nValue, int nKey);
	//This function returns the position in nfData whose value is the largest number (smallest if nKey <0) that is smaller (larger if nKey <0) than nValue in an asending array nfData
	void DefineBaselineSegment(int nBegin, int nEnd);
	CArtifactsHandling();
	CArtifactsHandling(float* pfData, float* pfAdjustedData, int nDataSize, 
		float fSampleInterval, const EvlNode structEvlNode);
	CArtifactsHandling::CArtifactsHandling(float* pfData, float* pfAdjustedData, int nDataSize, 
			float fSampleInterval, bool *pbExcluded, const EvlNode structEvlNode);
	void ReplaceSegments(float *pfData, float *pfAdjustedData, int nSizeOfData, float fSampleInterval, CHistory* pcHistory);
	virtual ~CArtifactsHandling();

protected:
	void ExcludeSegment(int nI, int nF);
	void MakeBaseline(int nBegin, int nEnd);
	void MakeBaseline0(int nBegin, int nEnd);
	void ExtendDetectedArtifacts();
	void ConcatenateArtifacts();
	void CheckEvl();
	void CheckEventList();
	void ConsolidateArtifacts();
	float SegAverage(int nLength, vector <float> vfData);
	void BrigeSegment(int nBegin, int nEnd);
	float SegAverage(int nBegin, int nLength);
	float SegAverage0(int nBegin, int nLength);
	void GetThresholds();
	void RemoveArtifacts();
	float* m_pfData;
	float* m_pfAdjustedData;
	int	   m_nDataSize;
	float  m_fSampleInterval;
	int    m_nNumOfLocalMinima;
	int    m_nNumOfLocalMaxima;
	int*   m_pnLocalMinima;
	int*   m_pnLocalMaxima;
	int    m_nBrigeSegStart;
	int    m_nBrigeSegLength;
	int    m_nMinArtifactInterval;
	float  m_fBrigeAmpBegin;
	float  m_fBrigeAmpEnd;
	float  m_fBrigeSlope;
	float  m_fTime1;
	float  m_fTime2;
	int	   m_nArtifactExtention;
	int m_nBLMax;
	int *m_pnBLMax;
	void CheckOppositeDirection(int nType);
	void CheckAbnormalValues(int nAmpType);
	void CheckAbnormalTransition();
	void CheckAbnormalTransitionAndValues();
	void LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	list <structSegmentNode> m_lstArtifactSegments;
	float m_fRange; // Peak to peak noise level to replace the artifact segments.
	float m_fAmplitudeThreshold; // Segments whose amplitudes are exceed this threshold 
								 // will be treated as candidates of artifacts.
	float m_fDeltaThreshold;	 // If the difference between two consecutive local exteama
								 // exceed this threshold, the segment will be treated 
								 // as candidate of artifact.
	float m_fOppositeDirectionThreshold; // If the current in opposite dirrection of applied 
										// voltage exceed this threshold, the segment will 
	bool  m_bDoNotConfirm;										// be treated as candidate of artifact.
	bool  m_bCheckDelta;
	bool  m_bCheckOppositeDirrection;
	bool  m_bCheckAmplitude;
	bool  m_bBasedOnAdjustedData;
	bool  m_bEntireTrace;
	bool  m_bCheckEvl;
	bool  m_bReadFromFile;
	bool  m_bBaselineDefined;
	bool  *m_pbExcluded;

	structSegmentsReplacingNode m_structSegmentsReplacingNode;

	float TruncateTo(float fValue, float fReturnValue, float fRange);
	EvlNode m_structEvlNode;
	int m_nBegin;
	int m_nEnd;
	vector <float> m_vfBaselineSegment;
};

#endif // !defined(AFX_ARTITACTSHANDLING_H__982156E1_DC16_11D4_AFE0_00C04F200B5A__INCLUDED_)
