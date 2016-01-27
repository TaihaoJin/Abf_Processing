// History.cpp: implementation of the CHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "History.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistory::CHistory()
{

}

CHistory::~CHistory()
{

}

void CHistory::WriteHistory(FILE *fpOut, CString sFileName)
{
	m_fpOut=fpOut;
	fprintf(fpOut,"Original Data File: %s\n",m_sDataFileName);
	fprintf(fpOut,"Adjusted Data File: %s\n",sFileName);
	CString sCommandName;
	while(!m_queueCommandNames.empty())
	{
		sCommandName=m_queueCommandNames.front();
		if(sCommandName=="BallRolling")
		{
			structBallRollingNode aNode=m_queueBallRollings.front();
			m_queueBallRollings.pop();
			WriteHistoryNode(aNode);
		}
		sCommandName=m_queueCommandNames.front();
		if(sCommandName=="UpdateAdjustedData")
		{
			fprintf(m_fpOut,"UpdataAdjustedData \n");
		}
		sCommandName=m_queueCommandNames.front();
		if(sCommandName=="SegmentsReplacing")
		{
			structSegmentsReplacingNode aNode=m_queueSegmentsReplacings.front();
			m_queueBallRollings.pop();
			WriteHistoryNode(aNode);
		}
		sCommandName=m_queueCommandNames.front();
		if(sCommandName=="ArtifactsHandling")
		{
			structArtifactsHandlingNode aNode=m_queueArtifactsHandlings.front();
			m_queueBallRollings.pop();
			WriteHistoryNode(aNode);
		}
		m_queueCommandNames.pop();
	}
}

void CHistory::RetrieveHistory(FILE *fpIn)
{

}
void CHistory::StoreToHistory(structArtifactsHandlingNode aNode)
{

}

void CHistory::StoreToHistory(structBallRollingNode aNode)
{
	m_queueCommandNames.push("BallRolling");
	m_queueBallRollings.push(aNode);

}
void CHistory::StoreToHistory(structSegmentsReplacingNode aNode)
{

}

void CHistory::GetFromHistory(structArtifactsHandlingNode& aNode)
{

}
void CHistory::GetFromHistory(structBallRollingNode& aNode)
{
	aNode=m_queueBallRollings.front();
	m_queueBallRollings.pop();
}
void CHistory::GetFromHistory(structSegmentsReplacingNode& aNode)
{

}



void CHistory::WriteHistoryNode(structBallRollingNode aNode)
{
	fprintf(m_fpOut,"BallRolling  \n");
	fprintf(m_fpOut,"fWindowSize (ms):  %12.1f  nRank:  %4d  nAnchorRank: %4d\n", aNode.fWindowSize,aNode.nRank,aNode.nAnchorRank);
	fprintf(m_fpOut,"bSmoothing: %1d  bCheckContactingPoints:  %1d  bTracingMinima:  %1d  bTracingMaxima: %1d\n",
		aNode.bSmoothing,aNode.bCheckContactingPoints, aNode.bMinima, aNode.bMaxima);
	fprintf(m_fpOut,"fCriterionOfUpTransitions(pA):  %4.1f  fTime1 (ms):  %12.1f  fTime2 (ms):  %12.1f\n",
		aNode.fCriterionOfUpTransitions, aNode.fTime1, aNode.fTime2);
	fprintf(m_fpOut,"nLevelHistWindowSize %4d  nMaxNumOfSuperiors:  %4d\n",aNode.nLevelHistWindowSize,aNode.nMaxNumOfSuperiors);
}

void CHistory::WriteHistoryNode(structArtifactsHandlingNode aNode)
{
	fprintf(m_fpOut,"ArtifactsHandling \n ");
	fprintf(m_fpOut,"bCheckDelta:  %1d  bCheckOppositeDirection  %1d  bCheckAmplitude  %1d  bBasedOnAdjustedData  1d",
		aNode.bCheckDelta,aNode.bCheckOppositeDirrection,aNode.bCheckAmplitude,aNode.bBasedOnAdjustedData);
	fprintf(m_fpOut,"fAmplitudeThreshold (pA):  %8.1f  fDeltaThreshold(pA):  %8.1f",aNode.fAmplitudeThreshold,aNode.fDeltaThreshold);
	fprintf(m_fpOut,"m_fOppositeDirectionThreshold (pA):  m_fRange(pA):  %8.1f",aNode.fOppositeDirectionThreshold,aNode.fRange);
}
void CHistory::WriteHistoryNode(structSegmentsReplacingNode aNode)
{
	fprintf(m_fpOut,"SegmentsReplacing  \n");
	fprintf(m_fpOut,"nBrigeBegin:  %d8  nBrigeLength:  %8d\n",aNode.nBrigeBegin,
		aNode.nBrigeLength);
	fprintf(m_fpOut,"nSegmentBegin:  %8d  nSegmentLength:  %id\n",aNode.nSegBegin,
		aNode.nSegLength);
}


void CHistory::ReadHistoryNode(structBallRollingNode aNode)
{

}
void CHistory::ReadHistoryNode(structArtifactsHandlingNode aNode)
{

}
void CHistory::ReadHistoryNode(structSegmentsReplacingNode aNode)
{

}

bool CHistory::Empty()
{
	return m_queueCommandNames.empty();
}

void CHistory::StoreFileName(CString sFileName)
{
	m_sDataFileName=sFileName;
	
}

void CHistory::ClearHistory()
{
	ClearCommandNames();
	ClearBallRollings();
	ClearArtifactsHandlings();
	ClearSegmentsReplacings();
}

void CHistory::ClearCommandNames()
{
	while(!m_queueCommandNames.empty())
	{
		m_queueCommandNames.pop();
	}
}

void CHistory::ClearBallRollings()
{
	while(!m_queueBallRollings.empty())
	{
		m_queueBallRollings.pop();
	}
}

void CHistory::ClearArtifactsHandlings()
{
	while(!m_queueArtifactsHandlings.empty())
	{
		m_queueArtifactsHandlings.pop();
	}
}

void CHistory::ClearSegmentsReplacings()
{
	while(!m_queueArtifactsHandlings.empty())
	{
		m_queueArtifactsHandlings.pop();
	}
}
