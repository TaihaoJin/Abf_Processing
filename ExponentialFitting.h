// ExponentialFitting.h: interface for the CExponentialFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPONENTIALFITTING_H__69111243_3639_4451_BD44_5D36DC651571__INCLUDED_)
#define AFX_EXPONENTIALFITTING_H__69111243_3639_4451_BD44_5D36DC651571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"
#include "NR.h"
class CExponentialFitting  
{
public:
	void AssignTimeConstants();
	double CalMeanTCA(structExponentialFittingNode aResultNode);
	CExponentialFitting();
	virtual ~CExponentialFitting();
	void SortFittedNode();
	structExponentialFittingNode GetFittedNode();
	void Initialization(structExponentialFittingNode aNode, const vector <structHistNode> avHist0);
	CExponentialFitting(structExponentialFittingNode aNode, const vector <structHistNode> avHist0);
	//This function takes dLambda of aTrialNode as the trial values.
	structExponentialFittingConstraintsNode HandlingConstraints(structExponentialFittingNode aNode);
	void Construct_x(structExponentialFittingNode aNode, structExponentialFittingConstraintsNode aConstraintsNode, Vec_DP &x);
};

#endif // !defined(AFX_EXPONENTIALFITTING_H__69111243_3639_4451_BD44_5D36DC651571__INCLUDED_)
