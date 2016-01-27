// QMatrixFitting.h: interface for the CQMatrixFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMATRIXFITTING_H__A867D701_6D45_47B8_9B72_33679563AA8B__INCLUDED_)
#define AFX_QMATRIXFITTING_H__A867D701_6D45_47B8_9B72_33679563AA8B__INCLUDED_

#include "Evl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "structures.h"
typedef vector <double> dVect;
typedef vector <int> nVect;
typedef double DP;


class CQMatrixFitting  
{
public:
	structDataSimulationNode GetDataSimulationParameters();
	DP pythag(const DP a, const DP b);
	void UpdataModel();
	void OutputModel(FILE* fp, vector <structQMModelNode> avsNodes);
	void PrepareInput();
	void amoeba(vector <dVect> &p, dVect &y, double ftol, int &nfunc);
//void NR::amoeba(Mat_IO_DP &p, Vec_IO_DP &y, const DP ftol, DP funk(Vec_I_DP &), int &nfunk)
	void UpdateQMatrix();
	void ReadInput();
	CQMatrixFitting();
	virtual ~CQMatrixFitting();
	template<class T>
	inline void SWAP(T &a, T &b)
		{T dum=a; a=b; b=dum;}

	template<class T>
	inline const T SQR(const T a) {return a*a;}

	template<class T>
	inline const T MAX(const T &a, const T &b)
			{return b > a ? (b) : (a);}

	inline float MAX(const double &a, const float &b)
			{return b > a ? (b) : float(a);}

	inline float MAX(const float &a, const double &b)
			{return b > a ? float(b) : (a);}

	template<class T>
	inline const T MIN(const T &a, const T &b)
			{return b < a ? (b) : (a);}

	inline float MIN(const double &a, const float &b)
			{return b < a ? (b) : float(a);}

	inline float MIN(const float &a, const double &b)
			{return b < a ? float(b) : (a);}

	template<class T>
	inline const T SIGN(const T &a, const T &b)
		{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

	inline float SIGN(const float &a, const double &b)
		{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

	inline float SIGN(const double &a, const float &b)
		{return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

	void GetQMatrix(double dQM[200][200], int &nNumLevels, int &nNumStates, vector <int> &vnStates);
protected:
	void StateIndexConversion();
	structDataSimulationNode m_sSimulationParameters;
	CString m_sEvlFileName;
	void UpdateFMatrix();
	void svbksb(vector <dVect> &u, dVect &w, vector <dVect> &v, dVect &b, dVect &x);
	void svdcmp(vector <dVect> &a, dVect &w, vector <dVect> &v);
	void CalCFMElements();
	CString ChangeFileExt(CString sFileName, CString sExt);
	void OutputMatrix(FILE* fp, double pdData[200][200], int nDim1);
	void TrialMinimization();
	void GenerateFreeFMEList();
	int AdjustableConstraint(vector <int> vnPool);
	void FreeCFME(int nFME);
	void ConstrainFME(int nFME, int nFMEC);
	void DownGrabFreeFME(int nIndex);
	void UpPassFreeFME(int nIndex);
	int CommonAncestor(structFMConstraintNode aNode, structFMConstraintNode bNode);
	int FindVectElement(vector <int> nVect, int nI);
	void PassFreeCFME(structFMConstraintNode aDoner, structFMConstraintNode  aAcceptor);
	void CQMatrixFitting::EraseVectElement(vector <int> &aVector, int nIndex);
	int FreeConstrainedFMElements(structFMConstraintNode aNode);
	int SpecifyConstraintIndex(structFMConstraintNode);
	void InitializeConstraints();
	void JoinConstraintPool(vector <nVect> &vvnCPool, int nIndex);
	void ApplyConstraints();
	void TransferVectElement(vector <int> &vnA, vector <int> &vnB, int nIA);
	void InitializeFMEList();
	double amotry(vector <dVect> &p, dVect &y, dVect &psum, const int ihi, const double fac);
	void get_psum(vector <dVect> &p, dVect &psum);
//	inline void get_psum(Mat_I_DP &p, Vec_O_DP &psum)
	double LogLikelihood(dVect x);
	void Fitting(vector <double> dVectT, vector <double> &dVectF);
	double m_dQMatrix[200][200];
	double m_dFMatrix[200][200];
	int m_nQConnection[200][200];
	int m_nFMEPosition[200][200]; //the element is the index of the F Matrix element in the m_vsFElements
	void InitQMatrix();
	int m_nNumStates;
	int m_nInitElements;
	int m_nNumLevels;
	int m_nNumFreeElements;
	vector <nVect> m_vvnStatesInLevels;
	typedef vector <int> nVect;
	vector <nVect> m_vvnCPool;
	double m_dK0;
	CEvl m_cEvl;
	vector <int> m_vnStates;
	vector <structQMElementNode> m_vsInputElements; //The list of the input Q matrix elements.
	vector <structFMElementNode> m_vsFMElements; //The list of the Constrainted F matrix elements.
	vector <int> m_vnFreeFMElements; //The list of the indexes the free F matrix elements.
	vector <int> m_vnCFMElements; //The list of the indexes of the constrainted F matrix elements.
	vector <structQMConstraintNode> m_vsQMConstraints; //The list of the input Q matrix constraints.
	vector <structFMConstraintNode> m_vsFMConstraints; //The list of the input F matrix constraints.
	vector <structQMModelNode> m_vsInitialModel; //Record of the input;
	vector <structQMModelNode> m_vsFinalModel;
	CString m_sInputFile;
	vector <int> m_vnStateIndexO; //The state index in the original (input) order. m_vnStateIndexO[i] store the state number of the input 
		//model whoese internal index is i. 
	vector <int> m_vnStateIndexI; //The internal state index. m_vnStateIndexI[i] stores the internal state index (starting from 0) 
		//the state i (Named by user, starting from 1) of the input model. 
};

#endif // !defined(AFX_QMATRIXFITTING_H__A867D701_6D45_47B8_9B72_33679563AA8B__INCLUDED_)
