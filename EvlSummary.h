// EvlSummary.h: interface for the CEvlSummary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLSUMMARY_H__655F29E1_F5DD_11D4_AFE1_00C04F200B5A__INCLUDED_)
#define AFX_EVLSUMMARY_H__655F29E1_F5DD_11D4_AFE1_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;

typedef vector<float> FloatVector;
typedef vector<int> IntVector;
struct structEvlSummaryNode
{
	CString sFileName;
	int nMaxLevel;
	int nNumChannels;
	float fRecordLength;
	float fOpenProb;
	int nNumEvents;
	int nNumLevels;
	float fMeanOpenTime;
	float fProbSingle;
	FloatVector vectfAmplitude;
	FloatVector vectfResidentTime;
	IntVector vectnNumEvents;
};

class CEvlSummary  
{
public:
	structEvlSummaryNode GetSummaryNode();
	void OutputSummary();
	void OutputSummary(int nCount);
	void ClearSummary();
	void SummarizeEvl();
	structEvlSummaryNode GetEvlSummary();
	void UpdateData(int nNumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
			   int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
			   short* pstEventCurrentLevel,
			   FILE* fpSummaryFile, float fRisetime, CString sEvlFileName);
	void UpdateData(int nNumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
			   int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,
			   short* pstEventCurrentLevel, float fRisetime, CString sEvlFileName);
	CEvlSummary();
	virtual ~CEvlSummary();
protected:
	float Exponential(float X, int N);
	float Exp(float X, int N);
	float Factorial(int N);
	float Binomial(float fProb, int N, int I);
	int m_nNumLevels;
	int m_nNumChannels;
	float m_fTimePerPoint;
	int m_nNumEvents;
	int* m_pnLevelStart;
	float* m_pfAmplitude;
	int* m_pnLevelLength;
//	float* m_pfEventStandardDev;
	short* m_pstEventCurrentLevel;
//	float* m_pfBaseline;
	CString m_sEvlFileName;
	FILE* m_fpSummaryFile;
	float m_fRange;  //in ms
	float m_fRisetime;

	structEvlSummaryNode m_structSummary;
};

#endif // !defined(AFX_EVLSUMMARY_H__655F29E1_F5DD_11D4_AFE1_00C04F200B5A__INCLUDED_)
