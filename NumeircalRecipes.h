// NumeircalRecipes.h: interface for the CNumeircalRecipes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMEIRCALRECIPES_H__5B523E36_F38D_4407_982F_9588BCD4B71F__INCLUDED_)
#define AFX_NUMEIRCALRECIPES_H__5B523E36_F38D_4407_982F_9588BCD4B71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "nrtypes.h"
#include "nr.h"
//#include <iostream>
//#include <iomanip>
#include <cmath>
using namespace std;


class CNumeircalRecipes  
{
public:
	DP bessj0(const DP x);
	DP amotry(Mat_IO_DP &p, Vec_O_DP &y, Vec_IO_DP &psum, DP funk(Vec_I_DP &), const int ihi, const DP fac);
	void amoeba(Mat_IO_DP &p, Vec_IO_DP &y, const DP ftol, DP funk(Vec_I_DP &), int &nfunk);
	void Test();
	double func(Vec_I_DP &x);
	CNumeircalRecipes();
	virtual ~CNumeircalRecipes();
};

#endif // !defined(AFX_NUMEIRCALRECIPES_H__5B523E36_F38D_4407_982F_9588BCD4B71F__INCLUDED_)
