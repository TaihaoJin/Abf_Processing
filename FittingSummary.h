// FittingSummary.h: interface for the CFittingSummary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FITTINGSUMMARY_H__A83572D7_7B5F_4CC6_A263_04B9AEB727E0__INCLUDED_)
#define AFX_FITTINGSUMMARY_H__A83572D7_7B5F_4CC6_A263_04B9AEB727E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "Structures.h"
using namespace std;

class CFittingSummary  
{
public:
	double CFittingSummary::CriticalFunction1(float Ts, float fAs, float Tl, float fAl, float T);
	double CFittingSummary::CriticalFunction2(float Ts, float fAs, float Tl, float fAl, float T);
	float CFittingSummary::BurstDelimiter(float fTs, float fTl);	
	double CriticalFunction(float Ts, float Tl, float T);
	void SummarizeFittings();
	CFittingSummary();
	virtual ~CFittingSummary();
protected:
	structSemNode CalSem(vector <float> vectfVect);
	void ExportCurves();
	CString ChangeExt(CString sFileName, CString sNewExt);
	void ReadFittingResults();
	vector <structFittingResultNode> m_vectFitting;
	int m_nDwellLevel;
	int m_nNumComponents;
	int m_nNumFiles;
	CString m_sFittingSummaryFile;
};

#endif // !defined(AFX_FITTINGSUMMARY_H__A83572D7_7B5F_4CC6_A263_04B9AEB727E0__INCLUDED_)
