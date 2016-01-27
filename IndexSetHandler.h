// IndexSetHandler.h: interface for the CIndexSetHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDEXSETHANDLER_H__748378C9_2939_4F7D_B0D6_0824CBA1F166__INCLUDED_)
#define AFX_INDEXSETHANDLER_H__748378C9_2939_4F7D_B0D6_0824CBA1F166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;

class CIndexSetHandler  
{
public:
	static void uniformArray(int *pnData, int length, int nV);
	static vector <int> setUnion(vector <int> setA, vector <int> setB, int nSize);
	static vector <int> setIntersection(vector <int> setA, vector <int> setB, int nSize);
	vector <int> complement(vector <int> setA,int nSize);
	vector <int> subtract(vector <int> setA, vector <int> setB, int nSize);
	vector <int> fullSet(int nSize);
	CIndexSetHandler();
	virtual ~CIndexSetHandler();

};

#endif // !defined(AFX_INDEXSETHANDLER_H__748378C9_2939_4F7D_B0D6_0824CBA1F166__INCLUDED_)
