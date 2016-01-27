// MeanSem.h: interface for the CMeanSem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEANSEM_H__CB304F28_D849_4404_B721_349037FC2CEC__INCLUDED_)
#define AFX_MEANSEM_H__CB304F28_D849_4404_B721_349037FC2CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "structures.h";
class CMeanSem  
{
public:
	CMeanSem();
	virtual ~CMeanSem();
	static structMeanSemNode calMeanSem(vector <double> vdData);
protected:
	int nSize;
	double dMean, dSem;
};

#endif // !defined(AFX_MEANSEM_H__CB304F28_D849_4404_B721_349037FC2CEC__INCLUDED_)
