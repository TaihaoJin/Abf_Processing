// Evl2DHistogram.h: interface for the CEvl2DHistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVL2DHISTOGRAM_H__6F345C86_7284_42F7_9949_2A062567F449__INCLUDED_)
#define AFX_EVL2DHISTOGRAM_H__6F345C86_7284_42F7_9949_2A062567F449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"

class CEvl2DHistogram  
{
public:
	void Export2DEvlHistFor3DPlot();
	void FilteringEvents();
	void Make2DHist();
	void Export2DEvlHistogram();
	CEvl2DHistogram();
	virtual ~CEvl2DHistogram();
	void UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
						int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
						short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded);
	struct2DEvlHistogramNode m_struct2DHist;

protected:
	void CalSem();
	CString ChangeExt(CString sFileName, CString sNewExt);
	int m_nNumLevels;
	int m_nNumChannels;
	float m_fTimePerPoint;
	int m_nNumEvents;
	int* m_pnLevelStart;
	float* m_pfAmplitude;
	int* m_pnLevelLength;
	short* m_pstEventCurrentLevel;
	bool* m_pbExcluded;
	CString m_sEvlFileName;
};

#endif // !defined(AFX_EVL2DHISTOGRAM_H__6F345C86_7284_42F7_9949_2A062567F449__INCLUDED_)
