// GammaFitting.h: interface for the CGammaFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMMAFITTING_H__EC7E6C79_CFA6_4B8F_8A55_09E6ADD00F14__INCLUDED_)
#define AFX_GAMMAFITTING_H__EC7E6C79_CFA6_4B8F_8A55_09E6ADD00F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Structures.h"
#include "nr.h"

class CGammaFitting  
{
public:
	CGammaFitting();
	virtual ~CGammaFitting();
	void SortFittedNode();
	structGammaFittingNode GetFittedNode();
	void Initialization(structGammaFittingNode aNode);
	CGammaFitting(structGammaFittingNode aNode, const vector <structHistNode> avHist0);
	structGammaFittingConstraintsNode HandlingConstraints(structGammaFittingNode aNode);
	void Construct_x(structGammaFittingNode aNode, structGammaFittingConstraintsNode aConstraintsNode, Vec_DP &x);
};

#endif // !defined(AFX_GAMMAFITTING_H__EC7E6C79_CFA6_4B8F_8A55_09E6ADD00F14__INCLUDED_)
