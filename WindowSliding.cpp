#include "stdafx.h"
#include "WindowSliding.h"

CWindowSliding::CWindowSliding(void)
{
}

CWindowSliding::~CWindowSliding(void)
{
}
int CWindowSliding::calSWAverage(float* pfData, float* pfAve, int indexI, int indexF, int nDelta, int nWS){//Takce average of 2*nWS+1, nWS points from both sides
	double dAve=0.,dSum=0.;
	float fT;
	int nNum=2*nWS+1;
	int nSize=(indexF-indexI)/nDelta+1;
	if(nSize<=nNum)nNum=nSize;
	int nPosition;
	int i,j;
	nPosition=indexI;
	for(i=0;i<nNum;i++){
		dSum+=pfData[nPosition];
		nPosition+=nDelta;
	}
	dAve=dSum/nNum;
	nPosition=indexI;

	if(nSize<=nNum){
		for(i=0;i<nNum;i++){
			pfAve[nPosition]=dAve;
			nPosition+=nDelta;
		}
		return -1;
	}
	for(i=0;i<nWS;i++){
		pfAve[nPosition]=dAve;
		nPosition+=nDelta;
	}
	pfAve[nPosition]=dAve;
	int len=nWS*nDelta;
	nPosition+=nDelta;
	int nPositionL=nPosition-len-nDelta,nPositionR=nPosition+len;
	while(nPositionR<indexF){
		dSum=dSum+pfData[nPositionR]-pfData[nPositionL];
		pfAve[nPosition]=dSum/nNum;
		nPosition+=nDelta;
		nPositionL+=nDelta;
		nPositionR+=nDelta;
	}
	dAve=dSum/nNum;
	while(nPosition<=indexF){
		pfAve[nPosition]=dAve;
		nPosition+=nDelta;
	}
}
