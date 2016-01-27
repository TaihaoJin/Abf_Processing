// BurstDelimiter.cpp: implementation of the CBurstDelimiter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BurstDelimiter.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBurstDelimiter::CBurstDelimiter()
{

}

CBurstDelimiter::~CBurstDelimiter()
{

}
double CBurstDelimiter::CriticalFunction(float Ts, float Tl, float T)
{
	//Two components have equal proportion of mistakes
	double dValue=0.;
	dValue=1.-exp(-T/Tl)-exp(-T/Ts);
	return dValue;
}

double CBurstDelimiter::CriticalFunction1(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Two components have equal number of mistakes
	double dValue=0.;
	dValue=-fAs*exp(-fT/fTs)+fAl*(1.f-exp(-fT/fTl));
	return dValue;
}

double CBurstDelimiter::CriticalFunction2(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Minimize the total number of mistakes
	double dValue=0.;
	dValue=-(fAs/fTs)*exp(-fT/fTs)+(fAl/fTl)*exp(-fT/fTl);
	return dValue;
}



double CBurstDelimiter::BurstDelimiter(double fTs, double dAs, double fTl, double dAl, int nKey)
{
	double fT1, fT2, fT;
	double dX,dX0,dXs,dXl;
	fT1=fTs/10.; 
	fT2=10.*fTl;
	fT=0.5*(fTs+fTl);
	switch (nKey)
	{
	case 1:
		dX=CriticalFunction(fTs, fTl, fT);
		break;
	case 2:
		dX=CriticalFunction1(fTs,dAs, fTl, dAl, fT);
		break;
	case 3:
		dX=CriticalFunction2(fTs,dAs, fTl, dAl, fT);
		break;
	default:
		dX=CriticalFunction(fTs,fTl,fT);
		break;
	}

	switch (nKey)
	{
	case 1:
		dXs=CriticalFunction(fTs, fTl, fT1);
		break;
	case 2:
		dXs=CriticalFunction1(fTs,dAs, fTl, dAl, fT1);
		break;
	case 3:
		dXs=CriticalFunction2(fTs,dAs, fTl, dAl, fT1);
		break;
	default:
		dXs=CriticalFunction(fTs,fTl,fT1);
		break;
	}

	switch (nKey)
	{
	case 1:
		dXl=CriticalFunction(fTs, fTl, fT2);
		break;
	case 2:
		dXl=CriticalFunction1(fTs,dAs, fTl, dAl, fT2);
		break;
	case 3:
		dXl=CriticalFunction2(fTs,dAs, fTl, dAl, fT2);
		break;
	default:
		dXl=CriticalFunction(fTs,fTl,fT2);
		break;
	}

//	fTs=4.9;
//	fTl=31.;
	dX0=dX;
	while(fabs(dX0)>0.000001)
	{
		if(dX*dXl>0.)
		{
			fT2=fT;
		}
		else
		{
			fT1=fT;
		}
		fT=0.5*(fT1+fT2);
		switch (nKey)
		{
		case 1:
			dX=CriticalFunction(fTs,fTl,fT);
			break;
		case 2:
			dX=CriticalFunction1(fTs,dAs, fTl, dAl, fT);
			break;
		case 3:
			dX=CriticalFunction2(fTs,dAs, fTl, dAl, fT);
			break;
		default:
			dX=CriticalFunction(fTs,fTl,fT);
			break;
		}
		dX0=dX;
	}
	return fT;
}
