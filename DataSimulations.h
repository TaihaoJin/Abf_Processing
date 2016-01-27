// DataSimulations.h: interface for the CDataSimulations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASIMULATIONS_H__75BA6FFF_C3F2_44A3_A67D_76504277AB2C__INCLUDED_)
#define AFX_DATASIMULATIONS_H__75BA6FFF_C3F2_44A3_A67D_76504277AB2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Abf.h"
#include "Evl.h"
#include "History.h"
#include <vector>
#include "structures.h"
#include "QMatrixFitting.h"


class CDataSimulations  
{
public:
	void GetSimulationParameters_Q();
	void OutputStateSummary(FILE* fp, int nIteration, int nSeed);
	void OutputStateSummary(CString sFileName);
	structEventListNode GetEventList();
	CDataSimulations(CAbf*);
	CDataSimulations(CAbf*,CEvl*,CString &cEvlFileName);
	CDataSimulations();
	vector <structStateSummaryNode> GetStateSummary();
	virtual ~CDataSimulations();

protected:
	structDataSimulationNode m_sSimulationParameters;
	CString ChangeFileExt(CString sFileName, CString sExt);
	CString m_sEvlFileName;
	CEvl* m_pcEvl;
	void SimulateData1();
	void GetParameters(CString &sEvlFileName);
	void GetNoise();
	float GetData(int nK, int nPosition);
	void CalTMatrix();
	int NextState(int nK, double dRand);
	int NextState(int nK, double dRand1, double dRand2);
	int m_nIterations;
	void SimulateData();
	CAbf* m_pcAbf;
	double m_dQM[200][200];
	double m_dTM[200][200];
	float *m_pfNoise;
	long m_nDataSize;
	vector <structKineticStateNode> m_vstructStates;
	int m_nNumStates;
	float m_fDataSampleInterval;
	double m_dTM1[200][200];
	vector <structStateSummaryNode> m_vstructStateSummary;
	structEventListNode m_vstructEventList;
	int m_nNumLevels;
	int m_nNoiseSeed;
	CQMatrixFitting m_cQF;
};

#endif // !defined(AFX_DATASIMULATIONS_H__75BA6FFF_C3F2_44A3_A67D_76504277AB2C__INCLUDED_)
