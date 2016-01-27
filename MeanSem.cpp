// MeanSem.cpp: implementation of the CMeanSem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "MeanSem.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeanSem::CMeanSem()
{

}

CMeanSem::~CMeanSem()
{

}
structMeanSemNode CMeanSem::calMeanSem(vector <double> vdData){
	int nSize=vdData.size();
	int i,j;
	double dv,dm=0.,dsum2=0.,dsem=0.;
	for(i=0;i<nSize;i++){
		dv=vdData[i];
		dm+=dv;
		dsum2+=dv*dv;
	}
	structMeanSemNode ms;
	if(nSize>0) dm/=nSize;
	ms.dMean=dm;
	if(nSize>1) dsem=sqrt((dsum2-nSize*dm*dm)/(nSize*(nSize-1)));
	ms.dSem=dsem;
	ms.nSize=nSize;
	return ms;
}