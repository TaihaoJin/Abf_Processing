// BurstDelimiter.h: interface for the CBurstDelimiter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BURSTDELIMITER_H__4FD2D18F_3498_4000_933D_13D06D817A07__INCLUDED_)
#define AFX_BURSTDELIMITER_H__4FD2D18F_3498_4000_933D_13D06D817A07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBurstDelimiter  
{
public:
double BurstDelimiter(double fTs, double dAs, double fTl, double dAl, int nKey);
	CBurstDelimiter();	
	virtual ~CBurstDelimiter();
protected:
	double CriticalFunction2(float fTs, float fAs, float fTl, float fAl, float fT);
	double CriticalFunction1(float fTs, float fAs, float fTl, float fAl, float fT);
	double CriticalFunction(float Ts, float Tl, float T);
};

#endif // !defined(AFX_BURSTDELIMITER_H__4FD2D18F_3498_4000_933D_13D06D817A07__INCLUDED_)
