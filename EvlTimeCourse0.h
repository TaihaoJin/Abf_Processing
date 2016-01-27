// EvlTimeCourse0.h: interface for the CEvlTimeCourse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5B__INCLUDED_)
#define AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEvlTimeCourse0  
{
public:
	CEvlTimeCourse0(CString sFileName, float fBinSize, float fTimePerPoint, int nNumEvent, int* pnLevelLength, 
		int* pnLevelStart, short* pstEventCurrentLevel);
	CEvlTimeCourse0();
	virtual ~CEvlTimeCourse0();
	void DoEvlTimeCourse();

protected:
	CString ChangeFileExt(CString sFileName, CString sExt);
	void WriteTimeCourse(FILE* fpOut);
	void PreparePointers();
	void OutputTimeCourse();

	int*   m_pnLevelStart;
	int*   m_pnLevelLength;	
	int	   m_nNumEvent;		 
	short* m_pstEventCurrentLevel;
	float  m_fTimePerPoint;

	float* m_pfNumOpenings;
	float* m_pfTotalOpenTime;
	int* m_pnMaximumOpenLevel;
	float* m_pfElapsedTime;
	float m_fBinSize;
	int   m_nNumBin;
	CString m_sEvlFileName;
};

#endif // !defined(AFX_EVLTIMECOURSE_H__6B243C42_992A_11D4_83DE_00C04F200B5B__INCLUDED_)
