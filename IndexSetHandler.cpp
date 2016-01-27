// IndexSetHandler.cpp: implementation of the CIndexSetHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "IndexSetHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexSetHandler::CIndexSetHandler()
{

}

CIndexSetHandler::~CIndexSetHandler()
{

}

void CIndexSetHandler::uniformArray(int *pnData, int length, int nV)
{
	for(int i=0;i<length;i++){
		pnData[i]=nV;
	}
}
vector <int> CIndexSetHandler::setUnion(vector <int> setA, vector <int> setB, int nSize){
	int *pnData=new int[nSize];
	uniformArray(pnData,nSize,0);
	int i,index,nA=setA.size(),nB=setB.size();
	for(i=0;i<nA;i++){
		index=setA[i];
		pnData[index]+=1;
	}
	for(i=0;i<nB;i++){
		index=setB[i];
		pnData[index]+=1;
	}
	vector <int> vnIndexes;
	for(i=0;i<nSize;i++){
		if(pnData[i]>0) vnIndexes.push_back(i);
	}
	return vnIndexes;
}
vector <int> CIndexSetHandler::setIntersection(vector <int> setA, vector <int> setB, int nSize){
	int *pnData=new int[nSize];
	uniformArray(pnData,nSize,0);
	int i,index,nA=setA.size(),nB=setB.size();
	for(i=0;i<nA;i++){
		index=setA[i];
		pnData[index]+=1;
	}
	for(i=0;i<nB;i++){
		index=setB[i];
		pnData[index]+=1;
	}
	vector <int> vnIndexes;
	for(i=0;i<nSize;i++){
		if(pnData[i]>1) vnIndexes.push_back(i);
	}
	return vnIndexes;
}
vector <int> CIndexSetHandler::fullSet(int nSize){
	vector <int> fullSet;
	for(int i=0;i<nSize;i++){
		fullSet.push_back(i);
	}
	return fullSet;
}
vector <int> CIndexSetHandler::subtract(vector <int> setA, vector <int> setB, int nSize){//this function returns subset 
	//of setA that belongs to the intersection between setA and setB. nSize is the size of the complete set.
	vector <int> aSet;
	int *pnData=new int[nSize];
	uniformArray(pnData,nSize,0);
	int nA=setA.size();
	int nB=setB.size();
	int i,index;
	for(i=0;i<nA;i++){
		index=setA[i];
		pnData[index]+=1;
	}
	for(i=0;i<nB;i++){
		index=setB[i];
		pnData[index]-=1;
	}
	for(i=0;i<nSize;i++){
		if(pnData[i]==1) aSet.push_back(i);
	}
	return aSet;
}
vector <int> CIndexSetHandler::complement(vector <int> setA,int nSize){
	return subtract(fullSet(nSize),setA,nSize);
}