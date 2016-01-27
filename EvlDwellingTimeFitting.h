// EvlDwellingTimeFitting.h: interface for the CEvlDwellingTimeFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLDWELLINGTIMEFITTING_H__0972D134_99E4_4F5F_A5E1_B51C94090D8A__INCLUDED_)
#define AFX_EVLDWELLINGTIMEFITTING_H__0972D134_99E4_4F5F_A5E1_B51C94090D8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Structures.h"
#include "nr.h"
#include "ExponentialFitting.h"
//	double PartitionFunction(double x[], int nDim);
//	double Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight);
//	void TimeConstantsAndWeights(double dT[], double dW[], Vec_I_DP &x, int nDim);
//	double LogLikelihood_Transform(Vec_I_DP &x);
//	double LogLikelihood_HB_Transform(Vec_I_DP &x);
//	double ChiSQ_HB_Transform(Vec_I_DP &x);
//	double LogLikelihood_AP_Transform(Vec_I_DP &x);

class CEvlDwellingTimeFitting  
{
public:
	void InitialPointT(structDwellFittingTrialNode &aNode, int nIndex, double dLogLik0, double &dLogLikI, int nSign, int nFuncIndex);
	void OutputFormattedTimeSpan(FILE *fpOutput, CString sComments, CTimeSpan aTime);
	void OutputFormattedTime(FILE* fpOutput, CString sComments, CTime aTime);
	void InitialPointW(structDwellFittingTrialNode &aNode, int nIndex, double dLogLik0, double &dLogLikI, int nSign, int nFuncIndex, structEvlDTFittingConstraintsNode aConstraintsNode0);
	void ConfidanceRangeW(structDwellFittingTrialNode aNode, structFittingResultNode &aResultNode, int nIndex, 
											double dLogLik0, int nSign, int nFuncIndex, structEvlDTFittingConstraintsNode aConstraintsNode0);
	void Construct_x(structDwellFittingTrialNode aNode, structEvlDTFittingConstraintsNode aConstraintsNode, Vec_DP &x);
	CEvlDwellingTimeFitting();
	virtual ~CEvlDwellingTimeFitting();
	structFittingResultNode FitEvl_Transform(int nFuncKey, structDwellFittingTrialNode aNode, structEvlHistogramNode structEvlHist);
	structFittingResultNode FitEvl_Transform_Original(int nFuncKey, structDwellFittingTrialNode aNode, structEvlHistogramNode structEvlHist);
	void CEvlDwellingTimeFitting::FitEvl_ConfidanceRange(int nFuncKey, structFittingResultNode &aResultNode, structEvlHistogramNode structEvlHist, int nConfidanceMode);
	//nMode==0 --> seting confindence level  without readjust other parameters;

protected:
	void MakeEvlDTResultNode(structFittingResultNode &bNode, structExponentialFittingNode aFittingNode);
	structExponentialFittingNode MakeExpFittingNode(int nFuncKey, structDwellFittingTrialNode aNode);
double LogLikelihood_Local(structDwellFittingTrialNode aNode, int nFuncIndex);
	void ResetGlobalConstraints(structDwellFittingTrialNode aNode);
	double GetTW(Vec_DP x, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex);
	void PassResultNodeTWL(structFittingResultNode &aResultNode, structFittingResultNode bResultNode);
	void SetConfidanceRangeT(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign);
	void SetConfidanceRangeW(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign);
	void SetConfidanceRangeTI(structFittingResultNode &aResultNode, structDwellFittingTrialNode bTrialNode, double dLogLikI, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign);
	void SetConfidanceRangeWI(structFittingResultNode &aResultNode, structDwellFittingTrialNode bTrialNode, double dLogLikI, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex,int nSign);
	void SetConfidanceRange(structFittingResultNode &aResultNode, structFittingResultNode bResultNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex);
	void ResetConstraint(structDwellFittingTrialNode &aNode, const structEvlDTFittingConstraintsNode aConstraintsNode0);
	void ResetConstraint(structDwellFittingTrialNode &aNode, const structDwellFittingTrialNode bNode);
	void RemoveConstraint(structDwellFittingTrialNode &aNode, int nIndex);
	void SetConstraint(structDwellFittingTrialNode &aNode, structEvlDTFittingConstraintsNode aConstraintsNode, int nIndex);
	void RemoveAllConstraints(structDwellFittingTrialNode &aNode);
	structDwellFittingTrialNode GetNextTrialNode(structDwellFittingTrialNode aNode, Vec_DP &x);
	structDwellFittingTrialNode GetNextTrialNode(structFittingResultNode bNode);
	structEvlDTFittingConstraintsNode HandlingConstraints(structDwellFittingTrialNode aNode);
	double MeanFittedDwellTime(structFittingResultNode aResultNode);
	double Area_Exp(double dT[], double dW[], int nDim, double dLeft, double dRight);
	int EventsInRange(int nDwellLevel);
	void ClearvectFRLogBins();
	double ChiSQ(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	void MeanDwellTimeHist(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, double& dMeanH, double& dMeanHC, int nEventsInRange);
	double SSE(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	double SDev(double dT[], double dW[], int nDim, int nDwellLevel, structFRange fRange, int nEventsInRange);
	float ClosestSamplePT(float fTime);
	void NewBinBoundary();
	void ExportDwellCurves_Normalized(structFittingResultNode &aResultNode);
	void MakeFLogHist_old(structFRange fFittingRange);	
	double CumulatedDwellProb(double dT[20], double dW[20], int nComponents, double dTime);
	void PassingLogHistBinRange(int &nFirstBin, int &nLastBin, int nLevel, structFRange fFittingRange);
	int MaxLevel();
	bool   EvlHist;
	bool   m_bHBEvlHist;
	bool   m_bLogEvlHist;
	structEvlHistogramNode m_structEvlHist;
};

#endif // !defined(AFX_EVLDWELLINGTIMEFITTING_H__0972D134_99E4_4F5F_A5E1_B51C94090D8A__INCLUDED_)
