// GaussianFitting.h: interface for the CGaussianFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAUSSIANFITTING_H__D55F6167_5CBC_4CAA_A7F7_524142E32B1B__INCLUDED_)
#define AFX_GAUSSIANFITTING_H__D55F6167_5CBC_4CAA_A7F7_524142E32B1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h"
class CGaussianFitting  
{
public:
	structGaussianFittingNode GetFittedNode();
	CGaussianFitting();
	CGaussianFitting(structGaussianFittingNode aNode);
	virtual ~CGaussianFitting();
protected:
	void SortFittedNode();
	void Initialization(structGaussianFittingNode aNode);
};

#endif // !defined(AFX_GAUSSIANFITTING_H__D55F6167_5CBC_4CAA_A7F7_524142E32B1B__INCLUDED_)
