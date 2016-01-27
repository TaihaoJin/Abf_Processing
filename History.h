// History.h: interface for the CHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORY_H__A49AD7E5_1EDC_42D0_BCCA_5F37919488FD__INCLUDED_)
#define AFX_HISTORY_H__A49AD7E5_1EDC_42D0_BCCA_5F37919488FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <queue>
using namespace std;
struct structBallRollingNode
{
	float	fWindowSize;
	int		nRank;
	float	fCriterionOfUpTransitions;
	int		nLevelHistWindowSize;
	int		nMaxNumOfSuperiors;
	BOOL	bSmoothing;
	BOOL	bCheckContactingPoints;
	BOOL    bMinima;
	BOOL    bMaxima;
	int		nAnchorRank;
	float	fTime1;
	float	fTime2;
};

struct structArtifactsHandlingNode
{
	float	fAmplitudeThreshold;
	float	fDeltaThreshold;
	float	fOppositeDirectionThreshold;
	float	fRange;
	BOOL	bCheckDelta;
	BOOL	bCheckOppositeDirrection;
	BOOL	bCheckAmplitude;
	BOOL	bBasedOnAdjustedData;
};

struct structSegmentsReplacingNode
{
	int nSegBegin;
	int nSegLength;
	int nBrigeBegin;
	int nBrigeLength;
};

class CHistory  
{
public:
	void ClearHistory();
	void StoreFileName(CString sFileName);
	bool Empty();
	void ReadHistoryNode(structBallRollingNode aNode);
	void ReadHistoryNode(structArtifactsHandlingNode aNode);
	void ReadHistoryNode(structSegmentsReplacingNode aNode);
	void WriteHistoryNode(structBallRollingNode aNode);
	void WriteHistoryNode(structArtifactsHandlingNode aNode);
	void WriteHistoryNode(structSegmentsReplacingNode aNode);
	void StoreToHistory(structArtifactsHandlingNode aNode);
	void StoreToHistory(structBallRollingNode aNode);
	void StoreToHistory(structSegmentsReplacingNode aNode);
	void GetFromHistory(structArtifactsHandlingNode& aNode);
	void GetFromHistory(structBallRollingNode& aNode);
	void GetFromHistory(structSegmentsReplacingNode& aNode);
	void RetrieveHistory(FILE* fpIn);
	void WriteHistory(FILE* fpOut, CString cFileName);
	CHistory();
	virtual ~CHistory();
protected:
	void ClearCommandNames();
	void ClearBallRollings();
	void ClearArtifactsHandlings();
	void ClearSegmentsReplacings();
	queue <CString> m_queueCommandNames;
	queue <structBallRollingNode> m_queueBallRollings;
	queue <structArtifactsHandlingNode> m_queueArtifactsHandlings;
	queue <structSegmentsReplacingNode> m_queueSegmentsReplacings;
	FILE* m_fpIn;
	FILE* m_fpOut;
	CString m_sDataFileName;
};

#endif // !defined(AFX_HISTORY_H__A49AD7E5_1EDC_42D0_BCCA_5F37919488FD__INCLUDED_)
