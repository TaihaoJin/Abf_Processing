// IndexOrganizerHist.cpp: implementation of the CIndexOrganizerHist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "IndexOrganizerHist.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexOrganizerHist::CIndexOrganizerHist()
{
	m_bUpdated=false;
	m_log10=log(10.);
}

CIndexOrganizerHist::~CIndexOrganizerHist()
{

}

void CIndexOrganizerHist::update(int nType, int nDim, double dBase, double dDelta)
{//m_nType==1 indicate logrithmic bins, dBase and dDelta are the log of the values.
	m_nType=nType;
	m_bUpdated=true;
	m_nDim=nDim;
	if(nDim<2) m_nDim=2;
	m_dBase=dBase;
	m_dDelta=dDelta;
	m_pvIndexes=new vInt[m_nDim];
	m_dBase1=exp(dBase*m_log10);
}

void CIndexOrganizerHist::addIndex(double value, int index)
{
	addIndex(value,m_nType,index);
}

void CIndexOrganizerHist::addIndex(double value, int nType, int index)
{
	if(nType==1){
		if(value<=0) 
			value=m_dBase-1;
		else
			value=log(value)/m_log10;
	}
	int position=getPosition(value);
	if(position>=0&&position<m_nDim) 
		m_pvIndexes[position].push_back(index);
	else if(position==-1)
		m_vnIndexesLo.push_back(index);
	else if(position==-2)
		m_vnIndexesHi.push_back(index);

}

int CIndexOrganizerHist::getPosition(double value)
{
	return getPosition(value,0);
}

int CIndexOrganizerHist::getPosition(double value, int nType)
{
	int position;
	if(nType==1){
		if(value<=0) 
			value=m_dBase-1;
		else
			value=log(value)/m_log10;
	}
	if(value<m_dBase) return -1;
	position=int((value-m_dBase)/m_dDelta);
	if(position>=m_nDim) position=-2;
	return position;
}

vector <int> CIndexOrganizerHist::getIndexes(int position){
	vector <int> vnEmpty;
	vnEmpty.clear();
	if(position<0){
		if(position==-1) return m_vnIndexesLo;
		if(position==-2) return m_vnIndexesHi;
		return vnEmpty;
	}
	if(position>=m_nDim) return vnEmpty;
	return m_pvIndexes[position];
}

int CIndexOrganizerHist::getDim()
{
	return m_nDim;
}

double CIndexOrganizerHist::getDelta()
{
	return m_dDelta;
}

double CIndexOrganizerHist::getValue(int index)
{
	return getValue(index, m_nType);
}
double CIndexOrganizerHist::getValue(int index, int nType)
{
	double dv=m_dBase+index*m_dDelta;
	if(nType==1) dv=exp(dv*m_log10);
	return dv;
}
void CIndexOrganizerHist::smoothHistogram(int ws)
{
	int indexi=0,indexf=m_nDim-1;
	smoothHistogram(indexi,indexf,ws);
}
void CIndexOrganizerHist::smoothHistogram(int indexi, int indexf, int ws)
{
	bool bLo=false, bHi=false;
	if(indexi<0){
		bLo=true;
		indexi=0;
	}
	if(indexf<0){
		bHi=true;
		indexf=m_nDim-1;
	}
	int i,j,k,ni,nf,nSize;
	vInt* pvIndexes=new vInt[m_nDim];
	vInt vnLo,vnHi;
	vInt aVint;
	for(i=0;i<m_nDim;i++){
		if(i>=indexi&&i<=indexf){
			ni=i-ws;
			if(ni<0) ni=0;
			nf=i+ws;
			if(nf>m_nDim-1) nf=m_nDim-1;
			for(j=ni;j<=nf;j++){
				aVint=m_pvIndexes[j];
				nSize=aVint.size();
				for(k=0;k<nSize;k++){
					pvIndexes[i].push_back(aVint[k]);
				}
			}
		}else{
			aVint=m_pvIndexes[i];
			nSize=aVint.size();
			for(k=0;k<nSize;k++){
				pvIndexes[i].push_back(aVint[k]);
			}
		}
	}
//	delete m_pvIndexes;
	if(bLo){
		for(i=0;i<ws;i++){
			aVint=m_pvIndexes[i];
			nSize=aVint.size();
			for(k=0;k<nSize;k++){
				vnLo.push_back(aVint[k]);
			}
			aVint=m_vnIndexesLo;
			nSize=aVint.size();
			for(k=0;k<nSize;k++){
				pvIndexes[i].push_back(aVint[k]);
			}
		}
		nSize=vnLo.size();
		for(k=0;k<nSize;k++){
			m_vnIndexesLo.push_back(vnLo[k]);
		}
	}
	if(bHi){
		for(i=m_nDim-1-ws;i<m_nDim;i++){
			aVint=m_pvIndexes[i];
			nSize=aVint.size();
			for(k=0;k<nSize;k++){
				vnHi.push_back(aVint[k]);
			}
			aVint=m_vnIndexesHi;
			nSize=aVint.size();
			for(k=0;k<nSize;k++){
				pvIndexes[i].push_back(aVint[k]);
			}
		}
		nSize=vnHi.size();
		for(k=0;k<nSize;k++){
			m_vnIndexesHi.push_back(vnHi[k]);
		}
	}
	m_pvIndexes=pvIndexes;
}




