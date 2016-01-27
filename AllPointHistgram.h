// AllPoingHistgram.h: interface for the CAllPoingHistgram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLPOINGHISTGRAM_H__1336DDED_76F9_4AF7_857E_A7F6E4409609__INCLUDED_)
#define AFX_ALLPOINGHISTGRAM_H__1336DDED_76F9_4AF7_857E_A7F6E4409609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAllPointHistgram  
{
public:
	CString ChangeFileExt(CString sFileName, CString sExt);
	void WriteHist0();
	void WriteHist();
	float* GetHistgram();
	float GetMinimum();
	int GetHistSize();
	CAllPointHistgram(CString sFileName, float* pfData, int nLength, float fSampleInterval, bool bShowTrace, int nReductionFactor, int nPolarity, float fTransitionThreshold, float fTime0);
	virtual ~CAllPointHistgram();
protected:
	void ProtectTransitions();
	void ProtectTransitions0();
	void LocalExtrema(float *pData, int *pLminima, int *pLmaxima, int lN, int &lNmin, int &lNmax);
	void InitializeHist();
	void CalRange();
	void CalHist();
	int m_nLength;
	int m_nSize;
	int m_nReductionFactor;
	float m_fSampleInterval;
	float* m_pfData;
	float* m_pfHist;
	int* m_pnLMin;
	int* m_pnLMax;
	int    m_nLMin;
	int    m_nLMax;
	bool*  m_pbProtected;
	int    m_nReductionFactor1;
	float  m_fInterval;
	float	   m_fTime0;
	float  m_fMax;
	float  m_fMin;
	bool m_bShowTrace;
	float  m_fTransitionThreshold;
	int    m_nPolarity;
	int    m_nNumProtectedPoints;
	CString m_sAbfFileName;
};

#endif // !defined(AFX_ALLPOINGHISTGRAM_H__1336DDED_76F9_4AF7_857E_A7F6E4409609__INCLUDED_)
