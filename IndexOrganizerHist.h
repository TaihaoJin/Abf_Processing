// IndexOrganizerHist.h: interface for the CIndexOrganizerHist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDEXORGANIZERHIST_H__E292A88D_B6CE_438C_9025_7E663989610F__INCLUDED_)
#define AFX_INDEXORGANIZERHIST_H__E292A88D_B6CE_438C_9025_7E663989610F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;
typedef vector <int> vInt;

class CIndexOrganizerHist  
{
public:
	double getValue(int index, int nType);
	int getPosition(double value, int nType);
	void addIndex(double value, int nType, int index);
	void smoothHistogram(int indexi, int indexf, int ws);
	void smoothHistogram(int ws);
	double getValue(int index);
	double getDelta();
	int getDim();
	int getPosition(double value);
	vector <int> getIndexes(int position);
	void addIndex(double value, int index);
	void update(int nType, int nDim, double dBase, double dDelta);
	CIndexOrganizerHist();
	virtual ~CIndexOrganizerHist();
protected: 
	int m_nType; //0 for linear and 1 for logorithmic
	int m_nDim;
	bool m_bUpdated;
	double m_dBase,m_dDelta;
	double m_dBase1;
	double m_log10;
	vInt* m_pvIndexes;
	vInt m_vnIndexesLo;//collected indexes whose corresponding values are smaller than m_dBase
	vInt m_vnIndexesHi;//collected indexes whose corresponding values are larger than m_dBase+m_dDelta*m_nDim
};

#endif // !defined(AFX_INDEXORGANIZERHIST_H__E292A88D_B6CE_438C_9025_7E663989610F__INCLUDED_)
