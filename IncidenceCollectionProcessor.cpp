#include "stdafx.h"
#include "IncidenceCollectionProcessor.h"
#include "stdafx.h"
#include "Abf_Processing.h"
#include "IncidenceSubsetHandler.h"
#include "BurstHandling.h"
#include "IndexSetHandler.h"
#include "FileNameHandling.h"
#include "nr.h"
#include "WindowSliding.h"
#include "PrintAssist.h"

int CIncidenceCollectionProcessor::m_nSN;
CIncidenceCollectionProcessor::CIncidenceCollectionProcessor()
{
	m_structBurstNodes.dTcb=-1.;
	m_structBurstNodes.nBurstID=-1;
	m_structClusterNodes.dTcc=-1;
	m_pnLevelStart=NULL;
	m_pnLevelLength=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pfAmplitude=NULL;
	m_pfBaseline=NULL;
	m_bValidCriteria=false;
	m_nISCID=-1;
	m_nEvlID=-1;
	m_nBurstID=-1;
	m_nClusterID=-1;
	m_bBurstNode=false;
	m_bClusterNode=false;
	m_bDwellTimeRanges=false;
	m_bForwardLatencyT=false;
	m_bForwardLatencyN=false;
	m_bBackwardLatencyT=false;
	m_bBackwardLatencyN=false;
}

CIncidenceCollectionProcessor::~CIncidenceCollectionProcessor()
{

}


void CIncidenceCollectionProcessor::updateEvl(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
								int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
								short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded,int nEvlID, float fRisetime)
{
	m_fTimePerPoint=TimePerPoint;
	m_sEvlFileName=sEvlFileName;
	m_nNumLevels=NumLevels;
	m_nNumChannels=NumChannels;
	m_fTimePerPoint=TimePerPoint;
	m_nNumEvents=nNumEvents;

	m_pnLevelStart=pnLevelStart;
	m_pnLevelLength=pnLevelLength;
	m_pstEventCurrentLevel=pstEventCurrentLevel;
	m_pfAmplitude=pfAmplitude;
	m_pfBaseline=pfBaseline;
	m_pbExcluded=pbExcluded;

	m_bClusterNode=false;
	m_bBurstNode=false;
	m_structBurstNodes.dTcb=-1.;
	m_structClusterNodes.dTcc=-1;
	m_bValidCriteria=false;
	m_nEvlID=nEvlID;
	m_nRecordingLength=pnLevelStart[m_nNumEvents-1]+pnLevelLength[m_nNumEvents-1];
	m_bDwellTimeRanges=false;
	m_vfAmp.clear();
	for(int i=0;i<m_nNumLevels;i++){
		m_vfAmp.push_back(0.f);
	}
	m_fRisetime=fRisetime;
	setRisetime(m_fRisetime);
	m_bRisetime=true;
}

int CIncidenceCollectionProcessor::CompleteSetSize(int nIncidenceType)
{
	switch (nIncidenceType){
	case 0:
		return m_nNumEvents;
		break;
	case 1:
		return m_structBurstNodes.burstNodes.size();
		break;
	case 2:
		return m_structClusterNodes.clusterNodes.size();
		break;
	default:
		return -1;
		break;
	}
	return -1;
}

int CIncidenceCollectionProcessor::getEvlID()
{
	return m_nEvlID;
}

int CIncidenceCollectionProcessor::getBurstID()
{
	return m_nBurstID;
}

void CIncidenceCollectionProcessor::updateBursts(structBurstNodes structBurstNodes)
{
	m_bBurstNode=true;
	m_structBurstNodes=structBurstNodes;
	m_nBurstID=structBurstNodes.nBurstID;
}


void CIncidenceCollectionProcessor::updateClusters(structClusterNodes structClusterNodes)
{
	if(structClusterNodes.clusterNodes.size()>0) m_bClusterNode=true;
	m_structClusterNodes=structClusterNodes;
	m_nClusterID=structClusterNodes.nClusterID;
}

int CIncidenceCollectionProcessor::getClusterID()
{
	return m_nClusterID;
}



CString CIncidenceCollectionProcessor::getEvlFileName(){
	return m_sEvlFileName;
}

AbfNode CIncidenceCollectionProcessor::getIncidenceCollectionAsTrace(int nKey, AbfNode structRawTrace)//nKey: 0 for level, 1 for amp
{
	int nPolarity=-1;
	int nDataSize,i,j,nPosition;
	nDataSize=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];
	AbfNode aNode;
	aNode.nSize=nDataSize;
	float* pfTemp=new float[nDataSize];

	nPosition=0;
	int nTemp,nLevel;
	int nLength,nBeginning0,nBeginning1;
	float fNoiseAmp=0.4, fAmp,fBaseline;
	int nLen=0;
	fBaseline=0.f;
	for(i=1;i<m_nNumEvents;i++)
	{
		nLevel=m_pstEventCurrentLevel[i-1];
		if(nLevel==0) fBaseline=m_pfBaseline[i];
		if(nKey==0){
			fAmp=float(nPolarity*nLevel);
		}else{
			fAmp=fBaseline+m_pfAmplitude[i];
		}
		nBeginning0=m_pnLevelStart[i-1];
		nBeginning1=m_pnLevelStart[i];
		for(nPosition=nBeginning0;nPosition<nBeginning1;nPosition++)
		{
			pfTemp[nPosition]=fAmp;
		}
	}

	for(i=m_nNumEvents-1;i<m_nNumEvents;i++)
	{
		nLevel=m_pstEventCurrentLevel[i-1];
		if(nLevel==0) fBaseline=m_pfBaseline[i];
		if(nKey==0){
			fAmp=float(nPolarity*nLevel);
		}else{
			fAmp=fBaseline+m_pfAmplitude[i];
		}
		nPosition=m_pnLevelStart[i];
		nTemp=m_pstEventCurrentLevel[i];
		for(j=0;j<m_pnLevelLength[i];j++)
		{
			pfTemp[nPosition]=fAmp;
			nPosition++;
		}
	}
	vector <int> vnIndexes=m_structIncidenceCollection.vnIndexes;
	int nSize=vnIndexes.size(),index;
	int nSeed=-1;
	NR::ran2(nSeed);
	int nTraceLength=structRawTrace.nSize;
	nLength=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1];
	for(i=0;i<nSize;i++){
		index=vnIndexes[i];
		nBeginning0=m_pnLevelStart[index];
		if(index<m_nNumEvents){
			nBeginning1=m_pnLevelStart[index+1];
		}else{
			nBeginning1=nLength;
		}
		if(nTraceLength > 0){
			for(j=nBeginning0;j<nBeginning1;j++){
				pfTemp[j]=structRawTrace.pfData[j];
			}
		}else{
			for(j=nBeginning0;j<nBeginning1;j++){
				pfTemp[j]+=(NR::ran2(nSeed)-0.5)*fNoiseAmp;
			}
		}
	}
	aNode.pfData=pfTemp;
	aNode.nSize=nDataSize;
	return aNode;
}

void CIncidenceCollectionProcessor::addjustTraceBaseline(AbfNode &aNode){
	int nSize=aNode.nSize;
	float fMaxAmp=100.;
	float* pfAve=new float[nSize];
	double dTcb=30.;
	int nRizing=2*m_nRising;
	int nWS=100;
	int nPositionL, nPositionR;
	int nBurstIndex=0;
	int nNumBursts=m_structBurstNodes.burstNodes.size();
	structBurstNode bstNode0,bstNode;
	int nIBCL;
	int i,nLen;
	double dAveL, dAveR;
	CWindowSliding wld;

	int nFirstBurst=0;//first burst with the preceding closing larger than nRize

	for(i=0;i<nNumBursts;i++){
		bstNode=m_structBurstNodes.burstNodes[i];
		nLen=m_pnLevelLength[bstNode.nPrecedingIBC];
		if(nLen>2*nRizing+1){
			nFirstBurst=i;
			break;
		}
	}
	
	bstNode=m_structBurstNodes.burstNodes[nFirstBurst];
	nIBCL=bstNode.nPrecedingIBC;

	int nBPL, nBPR;
	nBPL=m_pnLevelStart[nIBCL]+nRizing;
	nBPR=m_pnLevelStart[bstNode.nFirstOpening]-nRizing;
	wld.calSWAverage(aNode.pfData,pfAve,nBPL,nBPR,1,nWS);	
	dAveL=pfAve[nBPL];
	dAveR=pfAve[nBPR];
	for(i=0;i<nBPL;i++){
		aNode.pfData[i]-=dAveL;
	}
	for(i=nBPL;i<=nBPR;i++){
		aNode.pfData[i]-=pfAve[i];
	}
	int nIBCR=bstNode.nLastOpening+1;//update this line
	double dK;
	int nLastPoint;

	nFirstBurst++;
	nPositionL=nBPR;
	dAveL=dAveR;
	for(nBurstIndex=nFirstBurst;nBurstIndex<nNumBursts;nBurstIndex++){
		bstNode=m_structBurstNodes.burstNodes[nBurstIndex];
		nLen=m_pnLevelLength[bstNode.nPrecedingIBC];
		if(nLen<2*nRizing+1) continue;

		nIBCR=bstNode.nPrecedingIBC;
		nBPL=m_pnLevelStart[nIBCR]+nRizing;
		nBPR=m_pnLevelStart[nIBCR]+m_pnLevelLength[nIBCR]-nRizing;
		wld.calSWAverage(aNode.pfData,pfAve,nBPL,nBPR,1,nWS);	
		dAveR=pfAve[nBPL];
		nPositionR=nBPL;
		dK=dAveR-dAveL;
		nLen=nPositionR-nPositionL;//Between the two point at the begining and end of the burst
		dK/=nLen;
		for(i=1;i<nLen;i++){
			aNode.pfData[nPositionL+i]-=(dAveL+i*dK);//This segment does not include the two points
		}
		for(i=nBPL;i<=nBPR;i++){
			aNode.pfData[i]-=pfAve[i];
		}

		nPositionL=nBPR;
		dAveL=dAveR;
	}
	int nLastLongClosing=m_nNumEvents-1;
	int nLevel=m_pstEventCurrentLevel[nLastLongClosing];
	nLen=m_pnLevelLength[nLastLongClosing];
	while(nLevel!=0||nLen<2*nRizing+1){
		nLastLongClosing--;
		if(nLastLongClosing==nIBCR) break;
		nLevel=m_pstEventCurrentLevel[nLastLongClosing];
		nLen=m_pnLevelLength[nLastLongClosing];
	}
	if(nLastLongClosing>nIBCR){
		nBPL=m_pnLevelStart[nLastLongClosing]+nRizing;
		nBPR=m_pnLevelStart[nLastLongClosing]+m_pnLevelLength[nLastLongClosing]-nRizing;
		wld.calSWAverage(aNode.pfData,pfAve,nBPL,nBPR,1,nWS);	
		dAveR=pfAve[nBPL];
		nPositionR=nBPL;
		dK=dAveR-dAveL;
		nLen=nPositionR-nPositionL;//Between the two point at the begining and end of the burst
		dK/=nLen;
		for(i=1;i<nLen;i++){
			aNode.pfData[nPositionL+i]-=(dAveL+i*dK);//This segment does not include the two points
		}
		for(i=nBPL;i<=nBPR;i++){
			aNode.pfData[i]-=pfAve[i];
		}
	}
	for(i=nBPR+1;i<aNode.nSize;i++){
		aNode.pfData[i]-=dAveR;
	}
}

double CIncidenceCollectionProcessor::interpolation(double x1, double y1, double x2, double y2, double x)
{
	double k=(y2-y1)/(x2-x1);
	return y1+k*(x-x1);
}
void CIncidenceCollectionProcessor::exportAPH_Subset(double dMin, double dMax, AbfNode& abfNode, int nWS, int nIT, double dRatio0){
	CLinearHistogramHandler lhh;
	vector<CLinearHistogramDouble> hists;
	CLinearHistogramDouble hist=obtainEventAPH(m_structIncidenceCollection.vnIndexes,abfNode,dMin, dMax, dRatio0);
	hist.setTitles("Amp(pA)","NormalizedCounts");
	hists.push_back(hist);
	CString sFileName=m_sEvlFileName;
	CFileNameHandling fhh;
	sFileName=fhh.ExtendFileName(sFileName,m_structIncidenceCollection.sCollectionTitle);
	sFileName=fhh.ChangeFileExt(sFileName,"Lap");
	FILE* fp=fopen(sFileName,"w");
	lhh.exportHistograms(fp,hists,nWS,nIT);
	fclose(fp);
}

CLinearHistogramDouble CIncidenceCollectionProcessor::obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax)
{
	double dRatio0=1.;
	return obtainEventAPH(vnIndexes, aNode, dMin, dMax, dRatio0);
}
CLinearHistogramDouble CIncidenceCollectionProcessor::obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax, double dRatio0)
{
	CLinearHistogramDouble aph;
	double dDelta=0.1;
	aph.update(dMin,dMax,dDelta);
	float* pfData=aNode.pfData;
	int i,nSize=vnIndexes.size(),len, nStart,nIndex,lent;
	int nRizing=m_nRising;
	float fLen=0.f,fLen0=0.f,fLent=0.f;
	int nLevel;
	double dRatio;
	if(nSize==0) return aph;
	for(i=0;i<nSize;i++){
		nIndex=vnIndexes[i];
		nLevel=m_pstEventCurrentLevel[nIndex];
		nStart=m_pnLevelStart[nIndex]+nRizing;
		len=m_pnLevelLength[nIndex]-2*nRizing;
		if(len<0) len=0;
		fLen+=len;
		if(nLevel==0){
			dRatio=fLen0/fLent;
			if(dRatio<=dRatio0) {
				lent=int(fLent*dRatio0-fLen0+0.5);
				if(lent<len) len=lent;
				aph.addData(pfData,nStart,1,len);
				fLen0+=len;
				fLent+=len;
			}
		}else{
			aph.addData(pfData,nStart,1,len);
			fLent+=len;
		}
	}
	while(dRatio0<0.99&&dRatio<dRatio0){
		for(i=0;i<m_nNumEvents;i++){
			nLevel=m_pstEventCurrentLevel[i];
			if(nLevel==0){
				if(dRatio<=dRatio0) {
					nStart=m_pnLevelStart[i]+nRizing;
					len=m_pnLevelLength[i]-2*nRizing;
					if(len<0) len=0;
					lent=int(fLent*dRatio0-fLen0+0.5);
					if(lent<1) {
						dRatio=1.001*dRatio0;
						break;
					}
					if(lent<len) len=lent;
					fLen+=len;
					aph.addData(pfData,nStart,1,len);
					fLen0+=len;
					fLent+=len;
					dRatio=fLen0/fLent;
				}else{
					break;
				}
			}
		}
	}
	return aph;
}

void CIncidenceCollectionProcessor::exportDwellTimeHistograms(){
	vector <CLinearHistogramDouble> hists;
	if(!m_bDwellTimeRanges)calDwellTimeRanges();
	int i,nMode=1,nLen,nLevel;
	double dMin,dMax,dDelta=0.05,dLog10=log(10.),dT;
	CString sPositionTitle="DwellTime_",sCountTitle="Counts";
	CPrintAssist cpa;
	for(i=0;i<m_nNumLevels;i++){
		CLinearHistogramDouble hist;
		hist.setTitles(sPositionTitle+cpa.ToString(i),sCountTitle);
		dMin=m_vnDwellTimeMin[i]*m_fTimePerPoint/1000.;
		dMax=m_vnDwellTimeMax[i]*m_fTimePerPoint/1000.;
		dMin=log(dMin)/dLog10;
		dMax=log(dMax)/dLog10;
		hist.update(nMode,dMin,dMax,dDelta);
		hists.push_back(hist);
	}
	vector <int> vnIndexes=m_structIncidenceCollection.vnIndexes;
	int nSize=vnIndexes.size(),index;
	for(i=0;i<nSize;i++){
		index=vnIndexes[i];
		nLevel=m_pstEventCurrentLevel[index];
		nLen=m_pnLevelLength[index];
		dT=nLen*m_fTimePerPoint/1000.;
		hists[nLevel].addData(dT);
	}

	CLinearHistogramHandler lhh;
	CString sFileName=m_sEvlFileName;
	CFileNameHandling fhh;
	sFileName=fhh.ExtendFileName(sFileName,m_structIncidenceCollection.sCollectionTitle);
	sFileName=fhh.ChangeFileExt(sFileName,"Dwt");
	FILE* fp=fopen(sFileName,"w");
	lhh.exportHistograms(fp,hists,0,0);
	fclose(fp);
}

void CIncidenceCollectionProcessor::calDwellTimeRanges(){
	vector <bool> vbFirst;
	int i,nLevel,nLength,nMin,nMax;
	for(i=0;i<m_nNumLevels;i++){
		vbFirst.push_back(true);
		m_vnDwellTimeMin.push_back(0);
		m_vnDwellTimeMax.push_back(0);
	}
	for(i=0;i<m_nNumEvents;i++){
		nLevel=m_pstEventCurrentLevel[i];
		nLength=m_pnLevelLength[i];

		if(vbFirst[nLevel]){
			vbFirst[nLevel]=false;
			m_vnDwellTimeMin[nLevel]=nLength;
			m_vnDwellTimeMax[nLevel]=nLength;
		}else{
			nMin=m_vnDwellTimeMin[nLevel];
			nMax=m_vnDwellTimeMax[nLevel];
			if(nLength<nMin) m_vnDwellTimeMin[nLevel]=nLength;
			if(nLength>nMax) m_vnDwellTimeMax[nLevel]=nLength;
		}
	}
	m_bDwellTimeRanges=true;
}
void CIncidenceCollectionProcessor::setRisetime(float fRisetime){
	m_bRisetime=true;
	m_nRising=int (fRisetime*1000./m_fTimePerPoint)+1;
}
void CIncidenceCollectionProcessor::addjustAmplitude(AbfNode aNode){
	int i,j,l,r,len,nLevel;
	float fAmp;
	vector <float> fvAmp;
	vector <int> nvLen;
	for(i=0;i<m_nNumLevels;i++){
		fvAmp.push_back(0.f);
		nvLen.push_back(0);
	}
	for(i=0;i<m_nNumEvents;i++){
		len=m_pnLevelLength[i]-2*m_nRising;
		if(len<0) continue;
		l=m_pnLevelStart[i]+m_nRising;
		r=l+len;
		fAmp=0.f;
		for(j=l;j<=r;j++){
			fAmp+=aNode.pfData[j];
		}
		nLevel=m_pstEventCurrentLevel[i];
		fvAmp[nLevel]+=fAmp;
		nvLen[nLevel]+=len;
		fAmp/=len;
		m_pfAmplitude[i]=fAmp;
	}
	for(i=0;i<m_nNumLevels;i++){
		fvAmp[i]/=nvLen[i];
	}
	for(i=0;i<m_nNumEvents;i++){
		len=m_pnLevelLength[i]-2*m_nRising;
		if(len>0) continue;
		nLevel=m_pstEventCurrentLevel[i];
		fAmp=fvAmp[nLevel];
		m_pfAmplitude[i]=fAmp;
	}
}

float* CIncidenceCollectionProcessor::getAmplitude(){
	return m_pfAmplitude;
}

int CIncidenceCollectionProcessor::getPropertyType(CString sPropertyName)
{
	int nType=-1;
	/* 
	list of the property types
	level, dwellTimeRange(ms), conductanceRange(pA), latancyT(ms), latancyN(openings)
	IBC(ms), BTo(ms), TTo(ms), NOPB
	ICC(ms), CTo(ms), NOPC(bursts)
	*/
	if(sPropertyName=="level") return nType;//nType==0;
	nType++;
	if(sPropertyName=="dwellTime(ms)") return nType;//nType==1;
	nType++;
	if(sPropertyName=="conductance(pA)") return nType;//nType==2;
	nType++;
	if(sPropertyName=="latancyT(ms)") return nType;//nType==3;
	nType++;
	if(sPropertyName=="latancyN(openings)") return nType;//nType==4;
	nType++;
	if(sPropertyName=="IBC(ms)") return nType;//nType==5;
	nType++;
	if(sPropertyName=="BTo(ms)") return nType;//nType==6;
	nType++;
	if(sPropertyName=="TTo(ms)") return nType;//nType==7;
	nType++;
	if(sPropertyName=="NOPB") return nType;//nType==8;
	nType++;
	if(sPropertyName=="ICC(ms)") return nType;//nType==9;
	nType++;
	if(sPropertyName=="CTo(ms)") return nType;//nType==10;
	nType++;
	if(sPropertyName=="NOPC") return nType;//nType==11;
	nType++;
	if(sPropertyName=="HalfBurstN") return nType;//nType==12;
	nType++;
	if(sPropertyName=="HalfBurstT") return nType;//nType==13;
	nType++;
	if(sPropertyName=="HalfClusterN") return nType;//nType==14;
	nType++;
	if(sPropertyName=="tHalfClusterT") return nType;//nType==15;
	nType++;
	if(sPropertyName=="ElapsedTimeRange(ms)") return nType;//nType=16;
	nType++;
	if(sPropertyName=="AllEvents") return nType;//nType=17;
	nType++;
	if(sPropertyName=="AllBursts") return nType;//nType=18;
	nType++;
	if(sPropertyName=="AllClusters") return nType;//nType=19;
	nType=-1;
	return nType;
}
int CIncidenceCollectionProcessor::getPropertyValue(int nIncidenceType, int nPropertyType, int nIndex, double &dValue, int &nValue, int &nPrintingPrecision)
{
	bool bForward;
	int nType=abs(nPropertyType);
	switch(nType){
		case 0://level
			nValue=m_pstEventCurrentLevel[nIndex];
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 1://dwellTime (sampe points)
			nValue=m_pnLevelLength[nIndex];
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 2://conductance(pA)
			dValue=m_pfAmplitude[nIndex];
			nPrintingPrecision=1;
			break;
		case 3://latancyT(Points)
			if(nPropertyType>0){
				if(!m_bForwardLatencyT) calLatencyT(nPropertyType);
				nValue=m_pnForwardLatencyT[nIndex];
			}else{
				if(!m_bBackwardLatencyT) calLatencyT(nPropertyType);
				nValue=m_pnBackwardLatencyT[nIndex];
			}
			nPrintingPrecision=0;
			dValue=nValue;
			break;
		case 4://latancyRangeN(openings);
			if(nPropertyType>0){
				if(!m_bForwardLatencyN) calLatencyN(nPropertyType);
				nValue=m_pnForwardLatencyN[nIndex];
			}else{
				if(!m_bBackwardLatencyN) calLatencyN(nPropertyType);
				nValue=m_pnBackwardLatencyN[nIndex];
			}
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 5://IBC(Points)
			nValue=m_structBurstNodes.burstNodes[nIndex].nPrecedingIBC;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 6://BTo(ms)
			nValue=m_structBurstNodes.burstNodes[nIndex].fLength;
			dValue=nValue;
			nPrintingPrecision=1;
			break;
		case 7://TTo(ms)
			nValue=m_structBurstNodes.burstNodes[nIndex].fTotalOpenTime;
			dValue=nValue;
			nPrintingPrecision=1;
			break;
		case 8://NOPB
			nValue=m_structBurstNodes.burstNodes[nIndex].nNumOpenings;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 9://ICC(points)
			nValue=m_structClusterNodes.clusterNodes[nIndex].nPrecedingICC;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 10://CTo(ms)
			dValue=m_structClusterNodes.clusterNodes[nIndex].fTotalOpenTime;
			nPrintingPrecision=1;
			break;
		case 11://NOPC
			nValue=m_structClusterNodes.clusterNodes[nIndex].nNumOpenings;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 12://HalfBurstN
			if(!m_bForwardLatencyN) calLatencyN(1);
			if(!m_bBackwardLatencyN) calLatencyN(-1);
			if(m_pnForwardLatencyN[nIndex]-m_pnBackwardLatencyN[nIndex]) 
				nValue=1;
			else
				nValue=-1;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 13://HalfBurstT
			if(!m_bForwardLatencyT) calLatencyT(1);
			if(!m_bBackwardLatencyT) calLatencyT(-1);
			if(m_pnForwardLatencyT[nIndex]-m_pnBackwardLatencyT[nIndex]) 
				nValue=1;
			else
				nValue=-1;
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 14://HalfClusterN, not implemented
			dValue=0.;
			nPrintingPrecision=-1;
			break;
		case 15://HarlfClusterT, not implemented
			dValue=0.;
			nPrintingPrecision=-1;
			break;
		case 16://ElapsedTimeRange(points)
			nValue=m_pnLevelStart[nIndex];
			dValue=nValue;
			nPrintingPrecision=0;
			break;
		case 17://AllEvents, non applicable
			dValue=0.;
			nPrintingPrecision=-1;
			break;
		case 18://AllBursts, non applicable
			dValue=0.;
			nPrintingPrecision=-1;
			break;
		case 19://AllClusters, non applicable
			dValue=0.;
			nPrintingPrecision=-1;
			break;
		default:
			dValue=0.;
			nPrintingPrecision=-1;
			break;
	}
	return 1;
}

int CIncidenceCollectionProcessor::calLatencyT(int nType){
	if(nType>0){
		if(m_bForwardLatencyT) return 0;
		m_bForwardLatencyT=true;
	}else{
		if(m_bBackwardLatencyT) return 0;
		m_bBackwardLatencyT=true;
	}
	int nSize=m_structBurstNodes.burstNodes.size();
	int nFirstOpening,nLastOpening;
	int nLatency;
	int nStart,nEnd;
	structBurstNode bNode;
	bool bForward=(nType>0);
	int i,j;		
			
	for(i=0;i<nSize;i++){
		bNode=m_structBurstNodes.burstNodes[i];
		nFirstOpening=bNode.nFirstOpening;
		nLastOpening=bNode.nLastOpening;
		nStart=m_pnLevelStart[nFirstOpening];
		nEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
		for(j=nFirstOpening;j<=nLastOpening;j++){
			if(bForward) {
				nLatency=m_pnLevelStart[j]-nStart;
				m_pnForwardLatencyT[j]=nLatency;
			}else{//backward, latancy is counted from the end of the burst.
				nLatency=nEnd-m_pnLevelStart[j]-m_pnLevelLength[j];
				m_pnBackwardLatencyT[j]=nLatency;
			}
		}
	}
}
int CIncidenceCollectionProcessor::calLatencyN(int nType){
	if(nType>0){
		if(m_bForwardLatencyN) return 0;
		m_bForwardLatencyN=true;
	}else{
		if(m_bBackwardLatencyN) return 0;
		m_bBackwardLatencyN=true;
	}
	int nSize=m_structBurstNodes.burstNodes.size();
	int nFirstOpening,nLastOpening;
	int nLatency;
	int nStart,nEnd;
	structBurstNode bNode;
	bool bForward=(nType>0);
	int i,j;		
	for(i=0;i<nSize;i++){
		bNode=m_structBurstNodes.burstNodes[i];
		nFirstOpening=bNode.nFirstOpening;
		nLastOpening=bNode.nLastOpening;
		nStart=m_pnLevelStart[nFirstOpening];
		nEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
		for(j=nFirstOpening;j<=nLastOpening;j++){
			if(bForward) {
				nLatency=(j-nFirstOpening)/2;
				m_pnForwardLatencyN[j]=nLatency;
			}else{//backward, latancy is counted from the end of the burst.
				nLatency=(nLastOpening-j)/2;
				m_pnBackwardLatencyN[j]=nLatency;
			}
		}
	}
}

void CIncidenceCollectionProcessor::clearCollection(){
	m_structIncidenceCollection.sCollectionTitle="empty";
	m_structIncidenceCollection.incidenceType=-1;
	m_structIncidenceCollection.vnIndexes.clear();
}

void CIncidenceCollectionProcessor::updateCollection(structIncidenceCollectionNode aICNode){
	m_structIncidenceCollection.sCollectionTitle=aICNode.sCollectionTitle;
	m_structIncidenceCollection.incidenceType=aICNode.incidenceType;
	int nSize=aICNode.vnIndexes.size();
	for(int i=0;i<nSize;i++){
		m_structIncidenceCollection.vnIndexes.push_back(aICNode.vnIndexes[i]);
	}
}

void CIncidenceCollectionProcessor::buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex, int nScaleType, double dBase, double dDelta, int nDim){
	m_cIncidenceIndexHist.update(nScaleType, nDim, dBase, dDelta);
}
void CIncidenceCollectionProcessor::buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex, int nScaleType, vector <structValueRangeDouble> vRanges){
	m_cIncidenceIndexHist.update_ArbitraryRanges(nScaleType, vRanges);
}
void CIncidenceCollectionProcessor::buildIncidenceIndexHist(CString sIndependentProperty, int nRelatingIndex){
	int nPropertyType=getPropertyType(sIndependentProperty);
	int incidenceType=m_structIncidenceCollection.incidenceType;
	vector <int> vnIndexes;
	vnIndexes=m_structIncidenceCollection.vnIndexes;
	int nPrintingPrecision,nValue,indexo,index,nSize=vnIndexes.size();
	int nCompleteSetSize=CompleteSetSize(incidenceType);
	double dValue;
	for(int i=0;i<nSize;i++){
		indexo=vnIndexes[i];
		index=indexo+nRelatingIndex;
		if(index<0||index>=nCompleteSetSize) continue;
		getPropertyValue(incidenceType,nPropertyType,index,dValue,nValue,nPrintingPrecision);
		m_cIncidenceIndexHist.addIndex(indexo,dValue);
	}
}
void CIncidenceCollectionProcessor::exportGroupPropertyStatitics(vector <CString> vsPropertyTypes,vector <int> vnRelatingIndexes,vector <int> vnPrintingPrecisions){
}
