// OneDHistogram.h: interface for the COneDHistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONEDHISTOGRAM_H__4FFA0037_AD2F_4AC3_A9EB_DCDF4BD5B638__INCLUDED_)
#define AFX_ONEDHISTOGRAM_H__4FFA0037_AD2F_4AC3_A9EB_DCDF4BD5B638__INCLUDED_

#include "math.h"
#include "structures.h"
#include <vector>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COneDHistogram  
{
protected:
	int m_nLength;
	float m_fInterval;
	float m_fMean;
	vector <float> m_vfData;
	vector <float> m_vfHist;

	void CalHist();
	void CalRange();
	void InitializeHist();
	float m_fMin;
	float m_fMax;
	int m_nSize;
public:
	structOneDHistogramNode GetHistogram(vector <float> vfData, float fInterval);
	int GetHistSize();
	float GetMinimum();
	structOneDHistogramNode COneDHistogram::GetHistgram();
	COneDHistogram();
	void CAllPointHistgram(vector <float> vfData, float fInterval);
	virtual ~COneDHistogram();
};

#endif // !defined(AFX_ONEDHISTOGRAM_H__4FFA0037_AD2F_4AC3_A9EB_DCDF4BD5B638__INCLUDED_)
