// GeometricFitting.h: interface for the CGeometricFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRICFITTING_H__6786A6C4_56F4_41C7_A9DB_2774C6DF1D14__INCLUDED_)
#define AFX_GEOMETRICFITTING_H__6786A6C4_56F4_41C7_A9DB_2774C6DF1D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Structures.h"
#include "NR.h"



class CGeometricFitting  
{
public:
	double CalMean(structGeometricFittingNode aNode);
	CGeometricFitting();
	virtual ~CGeometricFitting();
	void SortFittedNode();
	structGeometricFittingNode GetFittedNode();
	void Initialization(structGeometricFittingNode aNode, const vector <structHistNode> avHist0);
	CGeometricFitting(structGeometricFittingNode aNode, const vector <structHistNode> avHist0);
	structGeometricFittingConstraintsNode HandlingConstraints(structGeometricFittingNode aNode);
	void Construct_x(structGeometricFittingNode aNode, structGeometricFittingConstraintsNode aConstraintsNode, Vec_DP &x);
};
#endif // !defined(AFX_GEOMETRICFITTING_H__6786A6C4_56F4_41C7_A9DB_2774C6DF1D14__INCLUDED_)
