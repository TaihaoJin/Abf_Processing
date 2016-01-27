// BaselineProcessing.h: interface for the BaselineProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASELINEPROCESSING_H__D6445481_67B9_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_BASELINEPROCESSING_H__D6445481_67B9_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "math.h"

class CBaselineProcessing  
{
public:
	void GetAdjustedData(float* pfData);
	void BaselineCorrection(float fTime1, float fWindowSize);
	CBaselineProcessing(float* pfData, int nLength, float fSamplieInterval);
	virtual ~CBaselineProcessing();
protected:
	bool IsBaseline(float fDev);
	void AdjustBaseline();
	void CalAveDev(float* pData, int nLength, float& fAve, float& fDev);
	void CalMaxDev(float fTime1);
	void WindowSliding();
	void SetWindSize(float fWindowSize);
	float m_fMaxBaselineDev;
	float* m_pfData;
	float* m_pfCorrectedData;
	float m_fFirstPoint;
	float m_fSecondPoint;
	int m_nLength;
	int m_nWindowHead;
	int m_nWindowTail;
	int m_nWindowSize;
	int m_nFirstPoint;
	int m_nSecondPoint;
	float m_fSampleInterval;
};

#endif // !defined(AFX_BASELINEPROCESSING_H__D6445481_67B9_11D4_83DE_00C04F200B5A__INCLUDED_)
