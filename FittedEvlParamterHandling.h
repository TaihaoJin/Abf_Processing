// FittedEvlParamterHandling.h: interface for the CFittedEvlParamterHandling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FITTEDEVLPARAMTERHANDLING_H__720EAA54_2F3F_40FA_92A7_03A9683CC723__INCLUDED_)
#define AFX_FITTEDEVLPARAMTERHANDLING_H__720EAA54_2F3F_40FA_92A7_03A9683CC723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"
#include "GaussianFitting.h"
#include <vector>
#include "BurstDelimiter.h"

using namespace std;


struct structDegeneracyNode{
	int nFileIndex;
	int nComponentindex;
	int nGausComponent;
};

struct structDTNodeSignature{
	int nFileIndex;
	int nLevelIndex;
	int nComponentIndex;
	int nTrialIndex;
};

typedef vector <int> VectInt;
class CFittedEvlParamterHandling  
{
public:
	CString ChangeFileExt(CString sFileName, CString sExt);
	void RemoveEmpyComponents();
	void OutputFittedDTParameters(CString sFileName);
	void RefineFitting();
	double CriticalFunction(float Ts, float Tl, float T);
	structSemNode CalSem(vector <double> vectfVect);
	vector <double> BurstDelimiters(double dT[], int nDim);
	double BurstDelimiter(float fTs, float fTl);
	double BurstDelimiter(double fTs, double fTl);
	structSemNode CalSem(vector <float> vectfVect);
	void OutputFittedDTParameters();
	void TimeConstDistribution();
	vector <structFittingResultsCollectionNode> m_avCollection;
	vector <structDTNodeSignature> m_avSignatures;
	vector <structFittingResultNode> m_avNodes;
	vector <structFittingResultNode> m_avNodes_Rearranged;
	vector <double> m_vdData;
	void TimeConstDistribution0();
	int m_nLevel;
	int m_nMaxComponents;
	double m_dMeanTimeConstants[20];
	CFittedEvlParamterHandling();
	CFittedEvlParamterHandling(vector <structFittingResultsCollectionNode> avCollection, structFittedParameterNode aFittedNode);
	vector <structDTNodeSignature> CFittedEvlParamterHandling::SelectParameterNodes(vector <structFittingResultsCollectionNode> m_avCollection, int m_nLevel);
	typedef vector <int> IntVect;
	vector <IntVect> Configurations(int nNumPositions, int nParticles);
	virtual ~CFittedEvlParamterHandling();
protected:
	void Freq_Components(double *dT, double *dW, int nComponents, structFRange fFittingRange, 
						int nEventsInRange, double dArea, double* dFrequency, float fLength);
	CString m_sFileName;
	bool m_bFreezeDev;
	bool Degenerated();
	int m_nFittedComponents;
	int HighestDegeneracy();
	vector <structDegeneracyNode> m_vDegeneracy;
	void RearrangeTW();
	double GaussianProb(double dX, int nIndex);
	vector <VectInt> m_vivComponents;
	void ResolvingDegeneracy();
	void AssignComponents();
	void AssignComponents0();
	void CalMeanTimeConstants();
	void SelectParameterNodes();
	structGaussianFittingNode m_structFittedNode;
	structGaussianFittingNode m_structTrialNode;
};

#endif // !defined(AFX_FITTEDEVLPARAMTERHANDLING_H__720EAA54_2F3F_40FA_92A7_03A9683CC723__INCLUDED_)
