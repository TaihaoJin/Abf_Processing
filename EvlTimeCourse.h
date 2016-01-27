// EvlTimeCourse.h: interface for the CEvlTimeCourse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BurstingTimeCourseOptionDlg.h"

class CEvlTimeCourse  
{
public:
	void DoBurstingTimeCourse();
	CEvlTimeCourse(CString sFileName, float fBinSize, float fTimePerPoint, int nNumEvent, int* pnLevelLength, 
		int* pnLevelStart, short* pstEventCurrentLevel, float* pfEffLevel, int* pnMRGEvents);
	CEvlTimeCourse();
	virtual ~CEvlTimeCourse();
	void DoEvlTimeCourse();

protected:
	CString CEvlTimeCourse::ChangeFileExt(CString sFileName, CString sExt);
	void PickClosedEvents();
	void WriteTimeCourse(FILE* fpOut);
	void WriteBurstingTimeCourse(FILE* fpOut);
	void PreparePointers();
	void OutputTimeCourse();
	void OutputBurstingTimeCourse();

	int*   m_pnLevelStart;
	int*   m_pnLevelLength;
	int	   m_nNumEvent;
	short* m_pstEventCurrentLevel;
	float  m_fTimePerPoint;
	CString m_sEvlFileName;

	float* m_pfRealNumOpenings;//For bursting time course
	float* m_pfNumOpenings;
	float* m_pfTotalOpenTime;
	float* m_pfEffTotalOpenTime;//For bursting time course
	int* m_pnMaximumOpenLevel;
	int* m_pnLastEventofBin;
	int* m_pnClosedEvents;
	float* m_pfElapsedTime;
	int* m_pnBinStart;
	float m_fBinSize;
	float* m_pfBinConductance;
	int   m_nNumBin;
	int	  m_nClosedEvents;
	float* m_pfEffLevel;
	int*  m_pnMRGEvents;
};

#endif // !defined(AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5A__INCLUDED_)
