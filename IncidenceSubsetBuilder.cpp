#include "stdafx.h"
#include "IncidenceSubsetBuilder.h"
#include "stdafx.h"
#include "Abf_Processing.h"
#include "IncidenceSubsetHandler.h"
#include "BurstHandling.h"
#include "IndexSetHandler.h"
#include "FileNameHandling.h"
#include "nr.h"
#include "WindowSliding.h"
#include "PrintAssist.h"

int CIncidenceSubsetBuilder::m_nSN;
CIncidenceSubsetBuilder::CIncidenceSubsetBuilder()
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

CIncidenceSubsetBuilder::~CIncidenceSubsetBuilder()
{

}

vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::importSubsetCriteriaNodes()
{
	FILE *fp;
	CString sFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="incidence subset criteria file (*.isc)\0*.isc\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="import incidence a subset-criteria file";
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);
	vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes;


	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
		fp=fopen(sFileName,"rb");
		vstructISCriteriaNodes=readIncidenceSubsetCriteriaFile(fp);
		fclose(fp);
		m_bValidCriteria=true;
	}
	return vstructISCriteriaNodes;
}


int CIncidenceSubsetBuilder::getIncidenceSubsetCriterionID(CString sID)
{
	/* 
	list of the criteria:
	level, dwellTimeRange(ms), conductanceRange(pA), latancyRangeT(ms), latancyRangeN(openings)
	IBCRange(ms), BToRange(ms), TToRange(ms), NOPBRange
	ICCRanges(ms), CToRanges(ms), NOPCRange(bursts)
	*/
	int nID=0;
	if(sID=="level") return nID;//nID==0;
	nID++;
	if(sID=="dwellTimeRange(ms)") return nID;//nID==1;
	nID++;
	if(sID=="conductanceRange(pA)") return nID;//nID==2;
	nID++;
	if(sID=="latancyRangeT(ms)") return nID;//nID==3;
	nID++;
	if(sID=="latancyRangeN(openings)") return nID;//nID==4;
	nID++;
	if(sID=="IBCRange(ms)") return nID;//nID==5;
	nID++;
	if(sID=="BToRange(ms)") return nID;//nID==6;
	nID++;
	if(sID=="TToRange(ms)") return nID;//nID==7;
	nID++;
	if(sID=="NOPBRange") return nID;//nID==8;
	nID++;
	if(sID=="ICCRanges(ms)") return nID;//nID==9;
	nID++;
	if(sID=="CToRanges(ms)") return nID;//nID==10;
	nID++;
	if(sID=="NOPCRange") return nID;//nID==11;
	nID++;
	if(sID=="HalfBurstN") return nID;//nID==12;
	nID++;
	if(sID=="HalfBurstT") return nID;//nID==13;
	nID++;
	if(sID=="HalfClusterN") return nID;//nID==14;
	nID++;
	if(sID=="tHalfClusterT") return nID;//nID==15;
	nID++;
	if(sID=="ElapsedTimeRange(ms)") return nID;//nID=16;
	nID++;
	if(sID=="AllEvents") return nID;//nID=17;
	nID++;
	if(sID=="AllBursts") return nID;//nID=18;
	nID++;
	if(sID=="AllClusters") return nID;//nID=19;
	nID=-1;
	return nID;
}

vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::readIncidenceSubsetCriteriaFile(FILE *fp)
{
	vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes;
	int nSubsets,nCriteria,i,j,nApplyingIndex;
	CString sTemp;
	CString sID;
	fscanf(fp,"%s%d",sTemp,&nSubsets);
	fscanf(fp,"%s",sTemp);
	int nID=getIncidenceSubsetCriterionID(sTemp);
	int nNumCriteria,nRelatingIndex;
	float fV1,fV2;
	float dTcb,dTcc;
	CString sOperator;
	for(i=0;i<nSubsets;i++){
		while(!(sTemp=="IncidenceType")){
			if(feof(fp)) break;
			fscanf(fp,"%s",sTemp);
		}
		structIncidenceSubsetCriteriaNode aISCNode;
		fscanf(fp,"%d",&nID);
		aISCNode.nIncidenceType=nID;
		fscanf(fp,"%s%D",sTemp,&nApplyingIndex);
		aISCNode.nApplyingIndex=nApplyingIndex;
		fscanf(fp,"%s%s",sTemp,sOperator);
		if(sOperator=="AND") 
			aISCNode.nOperationType=1;
		else
			aISCNode.nOperationType=2;
		vector <structIncidenceCriterionNode> vsC;
		fscanf(fp,"%s%d",sTemp,&nNumCriteria);
		for(j=0;j<nNumCriteria;j++){
			structIncidenceCriterionNode aICNode;
			fscanf(fp,"%s",sTemp);
			nID=getIncidenceSubsetCriterionID(sTemp);
			while(nID==-1){
				fscanf(fp,"%s",sTemp);
				if(feof(fp)) break;
			}
			aICNode.nCriterionID=nID;
			fscanf(fp,"%f%s%f",&fV1,sTemp,&fV2);
			aICNode.dV1=fV1;
			aICNode.dV2=fV2;
			fscanf(fp,"%s%d",sTemp,&nRelatingIndex);
			aICNode.nRelatingIndex=nRelatingIndex;
			aICNode.nIncidenceType=aISCNode.nIncidenceType;
			vsC.push_back(aICNode);
		}
		aISCNode.vstructIncidenceCriteria=vsC;
		vstructISCriteriaNodes.push_back(aISCNode);
	}
	return vstructISCriteriaNodes;
}

bool CIncidenceSubsetBuilder::isBurstDelimiter(CString sName)
{
	return sName=="BurstDelimiter(ms)";
}

bool CIncidenceSubsetBuilder::isClusterDelimiter(CString sName)
{
	return sName=="ClusterDelimiter(ms)";
}
void CIncidenceSubsetBuilder::updateEvl(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
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
	fillUpBaseline();
}
/*void CIncidenceSubsetBuilder::updateEvl(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
								int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
								short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded,int nEvlID, float fRisetime)
{
	m_fTimePerPoint=TimePerPoint;
	m_sEvlFileName=sEvlFileName;
	m_nNumLevels=NumLevels;
	m_nNumChannels=NumChannels;
	m_fTimePerPoint=TimePerPoint;
	m_nNumEvents=nNumEvents;

	delete m_pnLevelStart;
	delete m_pnLevelLength;
	delete m_pstEventCurrentLevel;
	delete m_pfAmplitude;
	delete m_pfBaseline;

	m_pnLevelStart=new int[nNumEvents];
	m_pnLevelLength=new int[nNumEvents];
	m_pstEventCurrentLevel=new short[nNumEvents];
	m_pfAmplitude=new float[nNumEvents];
	m_pfBaseline=new float[nNumEvents];
	m_pbExcluded=new bool[nNumEvents];

	for(int i=0;i<nNumEvents;i++)
	{
		*(m_pnLevelStart+i)=*(pnLevelStart+i);
		*(m_pnLevelLength+i)=*(pnLevelLength+i);
		*(m_pstEventCurrentLevel+i)=*(pstEventCurrentLevel+i);
		*(m_pfAmplitude+i)=*(pfAmplitude+i);
		*(m_pfBaseline+i)=*(pfBaseline+i);
		m_pbExcluded[i]=pbExcluded[i];
	}
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
	fillUpBaseline();
}
*/
void CIncidenceSubsetBuilder::buildSubsets()
{
	if(!m_bValidCriteria){
		loadIncidenceSubsetCriteriaNodes();
	}
	int nSize=m_vstructISCriteriaNodes.size();
	int i;
	m_structSubsets.dTcb=m_structBurstNodes.dTcb;
	m_structSubsets.dTcc=m_structClusterNodes.dTcc;
	for(i=0;i<nSize;i++){
		m_structSubsets.vstructSubsets.push_back(buildSubset(m_vstructISCriteriaNodes[i]));
	}
}
structIncidenceSubsetNode CIncidenceSubsetBuilder::buildSubset(structIncidenceSubsetCriteriaNode aISCNode)
{
	structIncidenceSubsetNode subset;
	structIncidenceCriterionNode cNode;
	int nType=aISCNode.nIncidenceType;
	int nApplyingIndex=aISCNode.nApplyingIndex;
	subset.nIncidenceType=nType;
	subset.nOperationType=aISCNode.nOperationType;
	subset.criteria=aISCNode;
	subset.nApplyingIndex=nApplyingIndex;
	vector <int> vnIndexes,vnIndexes0;
	CIndexSetHandler aISH;
	int nSize,i,j;
//	double dTcb=aISCNode.dTcb,dTcc=aISCNode.dTcc;	
	
	switch (nType){
	case 0:
		nSize=m_nNumEvents;
		break;
	case 1:
//		buildBurstNodes(dTcb);
		nSize=m_structBurstNodes.burstNodes.size();
		break;
	case 2:
//		buildClusterNodes(dTcb,dTcc);
		nSize=m_structClusterNodes.clusterNodes.size();
	}
	for(i=0;i<nSize;i++){
		vnIndexes0.push_back(i);
	}
	int nNumc=aISCNode.vstructIncidenceCriteria.size();
	for(i=0;i<nNumc;i++){
		vnIndexes=aISH.setIntersection(vnIndexes0,buildSubset(aISCNode.vstructIncidenceCriteria[i]),nSize);
		vnIndexes0=vnIndexes;
	}
	subset.vnIndexes=vnIndexes;
	return subset;
}
/*
struct structIncidenceCriterionNode{
	int nRelatingIndex;//an incidence i will be included into the subset if the incidence i+nRelatingIndex match the 
		   //specified criterion.
	CString sCriterionID;//one of the string identification for criterion.
	int nCriterionID;//an integer ID generated by looking up String - int ID table.
	double dV1, dV2; //Only the first value will be used if the ID indicates as a single value criterion. 
				 //Also will be converted
	double dTcb;//burst delimiter in ms
	double dTcc;//cluster delimiter in ms
};

struct structIncidenceSubsetCriteriaNode{
	int nIncidenceType; //0 for event, 1 for burst, and 2 for cluster
	vector <structIncidenceCriterionNode> vstructIncidenceCriteria;
};

struct structSubsetNode{
	int nIncidenceType; //0 for event, 1 for burst, and 2 for cluster
	int nApplyingIndex; //a negative integer for preceding incidence, zero for self and a positive integer for the following incidence.
	bool bIncluding; //true for including and false for excluding the corresponding incidences, respectively.
	vector <int> vnIndexes; //indexes of the incidences.
	structIncidenceSubsetCriteriaNode criteria;//the subset criteria used to construct the subset.
};*/

vector <int> CIncidenceSubsetBuilder::buildSubset(structIncidenceCriterionNode aNode)
{
	int nID=aNode.nCriterionID,i,index;
	int	nRelatingIndex=aNode.nRelatingIndex;
	int incidenceType=aNode.nIncidenceType;
	double lo,hi;
	vector <int> vnIndexes;
	structClusterNode cNode;
	int nLevelc=int(aNode.dV1+0.5);
	int nLevel,nSize,j;
	bool bForward;
	switch(nID){
		case 0://level
			nLevelc=int(aNode.dV1+0.5);
			for(i=0;i<m_nNumEvents;i++){
				index=i+nRelatingIndex;
				if(index<0||index>=m_nNumEvents) continue;
				nLevel=m_pstEventCurrentLevel[index];
				if(nLevel==nLevelc) vnIndexes.push_back(i);
			}
			break;
		case 1://dwellTimeRange(ms)
			lo=aNode.dV1;
			hi=aNode.dV2;
			float fLength;
			for(i=0;i<m_nNumEvents;i++){
				index=i+nRelatingIndex;
				if(index<0||index>=m_nNumEvents) continue;
				fLength=m_pnLevelLength[index]*m_fTimePerPoint/1000.;
				if(fLength<lo||fLength>hi) continue;
				vnIndexes.push_back(i);
			}
			break;
		case 2://conductanceRange(pA)
			lo=aNode.dV1;
			hi=aNode.dV2;
			float fAmp;
			for(i=0;i<m_nNumEvents;i++){
				index=i+nRelatingIndex;
				if(index<0||index>=m_nNumEvents) continue;
				fAmp=m_pfAmplitude[index];
				if(fAmp<lo||fAmp>hi) continue;
				vnIndexes.push_back(i);
			}
			break;
		case 3://latancyRangeT(ms), nRelatingIndex is always zero in practice.
			lo=fabs(aNode.dV1);
			hi=fabs(aNode.dV2);
			float fLatancy;
			double dTcb;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);//the burst and cluster nodes will be generated in the functions that call this function
			nSize=m_structBurstNodes.burstNodes.size();
			int nFirstOpening,nLastOpening;
			int nLatancy1,nLatancy2,nLatancy;
			nLatancy1=int(lo/(m_fTimePerPoint/1000));
			nLatancy2=int(hi/(m_fTimePerPoint/1000));
			int nStart,nEnd,nDelta;
			structBurstNode bNode;
			bForward=(aNode.dV2>0.);
			
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				nFirstOpening=bNode.nFirstOpening;
				nLastOpening=bNode.nLastOpening;
				nStart=m_pnLevelStart[nFirstOpening];
				nEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j+nRelatingIndex;
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=m_pnLevelStart[index]-nStart;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=nEnd-m_pnLevelStart[index]-m_pnLevelLength[index];
					}
					if(nLatancy<nLatancy1||nLatancy>=nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 4://latancyRangeN(openings);Range of latancy: latancy1<=latancy<Latancy2
			lo=abs(aNode.dV1);
			hi=abs(aNode.dV2);
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			nSize=m_structBurstNodes.burstNodes.size();
			nLatancy1=int(lo+0.5);
			nLatancy2=int(hi+0.5);
			bForward=(aNode.dV2>=0.);
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				nFirstOpening=bNode.nFirstOpening;
				nLastOpening=bNode.nLastOpening;

				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j+nRelatingIndex;
//					nLevel=m_pstEventCurrentLevel[nIndex];
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=(index-nFirstOpening)/2;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=(nLastOpening-index)/2;
					}
					if(nLatancy<nLatancy1||nLatancy>=nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 5://IBCRange(ms)
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			double dIBCl, dIBCr;
			nSize=m_structBurstNodes.burstNodes.size();
			bNode=m_structBurstNodes.burstNodes[0];
			dIBCl=bNode.fPrecedingIBC;
			for(i=0;i<nSize-1;i++){
				dIBCr=m_structBurstNodes.burstNodes[i+1].fPrecedingIBC;		
				if(dIBCl<lo||dIBCl>hi){
					dIBCl=dIBCr;
					continue;
				}
				if(dIBCr<lo||dIBCr>hi){
					dIBCl=dIBCr;
					continue;
				}
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
				dIBCl=dIBCr;
			}
			break;
		case 6://BToRange(ms)
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			double dBTo;
			nSize=m_structBurstNodes.burstNodes.size();
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				dBTo=bNode.fLength;
				if(dBTo<lo||dBTo>hi) continue;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 7://TToRange(ms)
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			double dTTo;
			nSize=m_structBurstNodes.burstNodes.size();
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				dTTo=bNode.fTotalOpenTime;
				if(dTTo<lo||dTTo>hi) continue;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 8://NOPBRange
			vnIndexes.clear();
			int nLo,nHi;
			nLo=int(aNode.dV1+0.5);
			nHi=int(aNode.dV2+0.5);
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			int nNOPB;
			nSize=m_structBurstNodes.burstNodes.size();
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				nNOPB=bNode.nNumOpenings;
				if(nNOPB<nLo||nNOPB>nHi) continue;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 9://ICCRanges(ms)
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
			double dTcc;
//			dTcc=aNode.dTcc;
//			dTcb=aNode.dTcb;
			buildClusterNodes(dTcb,dTcc);
			double dICCl, dICCr;
			nSize=m_structClusterNodes.clusterNodes.size();
			cNode=m_structClusterNodes.clusterNodes[0];
			dICCl=cNode.fPrecedingICC;
			for(i=0;i<nSize-1;i++){
				dICCr=m_structClusterNodes.clusterNodes[i+1].fPrecedingICC;		
				if(dICCl<lo||dICCl>hi){
					dICCl=dICCr;
					continue;
				}
				if(dICCr<lo||dICCr>hi){
					dICCl=dICCr;
					continue;
				}
				vnIndexes.push_back(i);
				dICCl=dICCr;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 10://CToRanges(ms)
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
//			dTcc=aNode.dTcc;
//			buildClusterNodes(dTcb,dTcc);
			double dCTo;
			nSize=m_structClusterNodes.clusterNodes.size();
			for(i=0;i<nSize;i++){
				cNode=m_structClusterNodes.clusterNodes[i];
				dCTo=cNode.fLength;	
				if(dCTo<lo||dCTo>hi) continue;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 11://NOPCRange
			vnIndexes.clear();
			lo=aNode.dV1;
			hi=aNode.dV2;
			int nNOPC;
//			dTcc=aNode.dTcc;
//			buildClusterNodes(dTcb,dTcc);
			nSize=m_structClusterNodes.clusterNodes.size();
			for(i=0;i<nSize;i++){
				cNode=m_structClusterNodes.clusterNodes[i];
				nNOPC=cNode.nNumOpenings;	
				if(nNOPC<lo||nNOPC>hi) continue;
				index=i-nRelatingIndex;//different implementation than case 0 to 4. 
				//in the cases 0 to 4, index== (the iteration variable)+nRelatingIndex,
				//and the incidences pointed by index is checked with the criteria and the iteration 
				//variable stored in vnIndexes.
				//In this case, the incidences pointed by the iteration variable is chected with the 
				//criteria and index is stored in vnIndexes
				if(index>=0&&index<=nSize-1) vnIndexes.push_back(index);
			}
			break;
		case 12://HarlfBurstN
			lo=aNode.dV1;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			nSize=m_structBurstNodes.burstNodes.size();
			bForward=(lo>=0.);
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				nFirstOpening=bNode.nFirstOpening;
				nLastOpening=bNode.nLastOpening;
				nLatancy1=0;
				nLatancy2=(nFirstOpening+nLastOpening)/2-nFirstOpening;
				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j-nRelatingIndex;
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=index-nFirstOpening;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=nLastOpening-index;
					}
					if(nLatancy<nLatancy1||nLatancy>nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 13://HarlfBurstT
			lo=aNode.dV1;
//			dTcb=aNode.dTcb;
//			buildBurstNodes(dTcb);
			nSize=m_structBurstNodes.burstNodes.size();
			nFirstOpening,nLastOpening;
			nLatancy1,nLatancy2,nLatancy;
			nStart,nEnd;
			bNode;
			bForward=(lo>=0.);
			int nMidBurst;
			
			for(i=0;i<nSize;i++){
				bNode=m_structBurstNodes.burstNodes[i];
				nFirstOpening=bNode.nFirstOpening;
				nLastOpening=bNode.nLastOpening;
				nStart=m_pnLevelStart[nFirstOpening];
				nEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
				nMidBurst=(nStart+nEnd)/2;
				nLatancy1=0;
				if(bForward)
					nLatancy2=nMidBurst-nStart;
				else
					nLatancy2=nEnd-nMidBurst;
				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j-nRelatingIndex;
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=m_pnLevelStart[index]-nStart;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=nEnd-m_pnLevelStart[index]-m_pnLevelLength[index];
					}
					if(nLatancy<nLatancy1||nLatancy>nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 14://HarlfClusterN
			lo=aNode.dV1;
//			dTcb=aNode.dTcb;
//			dTcc=aNode.dTcc;
//			buildClusterNodes(dTcb,dTcc);
			nSize=m_structClusterNodes.clusterNodes.size();
			int nNumBursts;
			bForward=(lo>=0.);
			for(i=0;i<nSize;i++){
				cNode=m_structClusterNodes.clusterNodes[i];
				nFirstOpening=cNode.svBurstNodes[0].nFirstOpening;
				nNumBursts=cNode.nNumBursts;
				nLastOpening=cNode.svBurstNodes[nNumBursts-1].nLastOpening;
				nLatancy1=0;
				nLatancy2=(nFirstOpening+nLastOpening)/2-nFirstOpening;
				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j-nRelatingIndex;
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=index-nFirstOpening;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=nLastOpening-index;
					}
					if(nLatancy<nLatancy1||nLatancy>nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 15://HarlfClusterT
			lo=aNode.dV1;
//			dTcb=aNode.dTcb;
//			dTcc=aNode.dTcc;
//			buildClusterNodes(dTcb,dTcc);
			nSize=m_structClusterNodes.clusterNodes.size();
			bForward=(lo>=0.);
			for(i=0;i<nSize;i++){
				cNode=m_structClusterNodes.clusterNodes[i];
				nFirstOpening=cNode.svBurstNodes[0].nFirstOpening;
				nNumBursts=cNode.nNumBursts;
				nLastOpening=cNode.svBurstNodes[nNumBursts-1].nLastOpening;
				nStart=m_pnLevelStart[nFirstOpening];
				nEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
				nLatancy1=0;
				nLatancy2=(nEnd+nStart)/2-nStart;
				for(j=nFirstOpening;j<=nLastOpening;j++){
					index=j-nRelatingIndex;
					if(index<0||index>=m_nNumEvents-1) continue;
					if(bForward) {
						nLatancy=index-nFirstOpening;
					}else{//backward, latancy is counted from the end of the burst.
						nLatancy=nLastOpening-index;
					}
					if(nLatancy<nLatancy1||nLatancy>nLatancy2) continue;
					vnIndexes.push_back(index);
				}
			}
			break;
		case 16://ElapsedTimeRange(ms)
			{
				lo=aNode.dV1;
				hi=aNode.dV2;
				int nStart0=int (lo*1000/m_fTimePerPoint);
				int nEnd0=int (hi*1000/m_fTimePerPoint);
				int nStart, nEnd;
				switch (incidenceType){
				case 0://event
					for(i=0;i<m_nNumEvents;i++){
						nStart=m_pnLevelStart[i];
						nEnd=nStart+m_pnLevelLength[i];
						if(nStart<nStart0||nEnd>=nEnd0) continue;
						vnIndexes.push_back(i);
					}
					break;
				case 1://burst
					structBurstNode bstNode;
					for(i=0;i<m_structBurstNodes.burstNodes.size();i++){
						bstNode=m_structBurstNodes.burstNodes[i];
						nStart=bstNode.nFirstOpening;//index of the opening
						nStart=m_pnLevelStart[nStart];
						nEnd=bstNode.nLastOpening;
						nEnd=m_pnLevelStart[nEnd]+m_pnLevelLength[nEnd];
						if(nStart<nStart0||nEnd>=nEnd0) continue;
						vnIndexes.push_back(i);
					}
					break;
				case 2://cluster
//					structClusterNode clstNode;
					for(i=0;i<m_structClusterNodes.clusterNodes.size();i++){
						cNode=m_structClusterNodes.clusterNodes[i];
						nStart=cNode.nFirstOpening;
						nStart=m_pnLevelStart[nStart];
						nEnd=cNode.nLastOpening;
						nEnd=m_pnLevelStart[nEnd]+m_pnLevelLength[nEnd];
						if(nStart<nStart0||nEnd>=nEnd0) continue;
						vnIndexes.push_back(i);
					}
					break;
				default:
					break;
				}
			}
			break;
		case 17://AllEvents
			for(i=0;i<m_nNumEvents;i++){
				vnIndexes.push_back(i);
			}
			break;
		case 18://AllBursts
			for(i=0;i<m_structBurstNodes.burstNodes.size();i++){
				vnIndexes.push_back(i);
			}
			break;
		case 19://AllClusters
			for(i=0;i<m_structClusterNodes.clusterNodes.size();i++){
				vnIndexes.push_back(i);
			}
			break;
		default:
			break;
	}
	return vnIndexes;
}
/*
	int nID=0;
	if(sID=="level") return nID;//nID==0;
	nID++;
	if(sID=="dwellTimeRange(ms)") return nID;//nID==1;
	nID++;
	if(sID=="conductanceRange(pA)") return nID;//nID==2;
	nID++;
	if(sID=="latancyRangeT(ms)") return nID;//nID==3;
	nID++;
	if(sID=="latancyRangeN(openings)") return nID;//nID==4;
	nID++;
	if(sID=="IBCRange(ms)") return nID;//nID==5;
	nID++;
	if(sID=="BToRange(ms)") return nID;//nID==6;
	nID++;
	if(sID=="TToRange(ms)") return nID;//nID==7;
	nID++;
	if(sID=="NOPBRange") return nID;//nID==8;
	nID++;
	if(sID=="ICCRanges(ms)") return nID;//nID==9;
	nID++;
	if(sID=="CToRanges(ms)") return nID;//nID==10;
	nID++;
	if(sID=="NOPCRange") return nID;//nID==11;
	nID++;
	if(sID=="HarlfBurstN") return nID;//nID==12;
	nID++;
	if(sID=="HarlfBurstT") return nID;//nID==13;
	nID++;
	if(sID=="HarlfClusterN") return nID;//nID==14;
	nID++;
	if(sID=="HarlfClusterT") return nID;//nID==15;
	nID=-1;
*/
bool CIncidenceSubsetBuilder::sameIntervals(double dV1, double dV2)
{
	int n1=int(dV1/(m_fTimePerPoint/1000.));
	int n2=int(dV2/(m_fTimePerPoint/1000.));
	return n1==n2;
}

bool CIncidenceSubsetBuilder::invalidDelimiter(double dTc)
{
	return dTc<0.;
}

void CIncidenceSubsetBuilder::buildBurstNodes(double dTcb0)
{
	double dTcb=m_structBurstNodes.dTcb;
	bool bRebuild=false;
	if(!invalidDelimiter(dTcb)){
		if(invalidDelimiter(dTcb0)||sameIntervals(dTcb,dTcb0)){
			bRebuild=false;
		}else{
			bRebuild=true;
			dTcb=dTcb0;
		}
	}else{
		bRebuild=true;
		dTcb=dTcb0;
	}
	if(bRebuild){
		CBurstHandling cBH;
		cBH.UpdateData(m_nNumChannels, m_nNumLevels, m_fTimePerPoint, m_nNumEvents,
								m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,m_pfBaseline,
								m_pstEventCurrentLevel,m_sEvlFileName, m_pbExcluded);
		if(invalidDelimiter(dTcb)) 
			cBH.MakeBurstNodes();
		else
			cBH.MakeBurstNodes(dTcb);
		updateBursts(cBH.getBurstNodes());
	}
}

void CIncidenceSubsetBuilder::buildClusterNodes(double dTcb, double dTcc0)
{
	double dTcc=m_structClusterNodes.dTcb;
	bool bRebuild=false;
	if(!invalidDelimiter(dTcc)){
		if(invalidDelimiter(dTcc0)||sameIntervals(dTcc,dTcc0)){
			bRebuild=false;
		}else{
			bRebuild=true;
			dTcc=dTcc0;
		}
	}else{
		bRebuild=true;
		dTcc=dTcc0;
	}
	if(bRebuild){
		CBurstHandling cBH;
		cBH.UpdateData(m_nNumChannels, m_nNumLevels, m_fTimePerPoint, m_nNumEvents,
								m_pnLevelStart,m_pnLevelLength,m_pfAmplitude,m_pfBaseline,
								m_pstEventCurrentLevel,m_sEvlFileName, m_pbExcluded);
		if(invalidDelimiter(dTcc)) 
			cBH.MakeClusterNodes();
		else
			cBH.MakeClusterNodes(dTcb,dTcc);
		updateClusters(cBH.getClusterNodes());
	}
}

vector <int> CIncidenceSubsetBuilder::getFinalIndexes(int nIncidenceType)
{
	CIndexSetHandler ish;
	int nSize=m_structSubsets.vstructSubsets.size();
	int i,j;
	structIncidenceSubsetNode aSubset;
	int n;
	int nCompleteSize=CompleteSetSize(nIncidenceType);
	vector <int> vnIndexes=ish.fullSet(nCompleteSize),vnT;

	for(i=0;i<nSize;i++){
		aSubset=m_structSubsets.vstructSubsets[i];
		if(aSubset.nIncidenceType<nIncidenceType) continue;
		vnIndexes=applySubset(vnIndexes,nIncidenceType,aSubset);
	}
	return vnIndexes;
}

int CIncidenceSubsetBuilder::CompleteSetSize(int nIncidenceType)
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
vector <int> CIncidenceSubsetBuilder::applySubset(vector <int> aVect, int nIncidenceType, structIncidenceSubsetNode aNode){//nIncidenceType is the type of Incidence pointed by the indexes stored in aVect
	vector <int> vnIndexes;
	int nShift=aNode.nApplyingIndex;
	vnIndexes=aNode.vnIndexes;
	int nCompleteSizeA=CompleteSetSize(aNode.nIncidenceType);
	if(nShift!=0) vnIndexes=shiftIndexes(vnIndexes,nShift,nCompleteSizeA);

	vnIndexes=expandIndexes(vnIndexes, aNode.nIncidenceType, nIncidenceType);

	int nType=aNode.nOperationType;//1 for AND operation, others for OR operation. negative sign indicates to apply the complement the subset.
	int nCompleteSize=CompleteSetSize(nIncidenceType);
	CIndexSetHandler ish;

	if(nType<0){
		vnIndexes=ish.complement(vnIndexes,nCompleteSize);
		nType=-nType;
	}
	if(nType==1)
		vnIndexes=ish.setIntersection(aVect,vnIndexes,nCompleteSize);
	else
		vnIndexes=ish.setUnion(aVect,vnIndexes,nCompleteSize);
	return vnIndexes;
}
vector <int> CIncidenceSubsetBuilder::expandIndexes(vector <int> vnIndexes0, int nTypeA, int nTypeB){//nTypeA is the incidence type of the indexes of the input indexes in vnIndexes0
																									//nTypeB is the incidence type of the indexes of the returning vector
	vector<int> vnIndexes,vn;
	int i,j,index,n;
	int nSize=vnIndexes0.size();
	for(i=0;i<nSize;i++){
		index=vnIndexes0[i];
		vn=expandIndex(index,nTypeA, nTypeB);
		n=vn.size();
		for(j=0;j<n;j++){
			index=vn[j];
			vnIndexes.push_back(index);
		}
	}
	return vnIndexes;
}
vector <int> CIncidenceSubsetBuilder::expandIndex(int index, int nTypeA, int nTypeB){//nTypeA is th incidence type of the input index
	int nType,i,j,n0,n1;
	vector <int> vn1,vn0,vnIndexes;
	vn1.push_back(index);
	vnIndexes.push_back(index);
	for(nType=nTypeA;nType>nTypeB;nType--){
		vnIndexes.clear();
		n1=vn1.size();
		for(i=0;i<n1;i++){
			index=vn1[i];
			vn0=expandIndex(index,nType);
			n0=vn0.size();
			for(j=0;j<n0;j++){
				vnIndexes.push_back(vn0[j]);
			}
		}
		n1=vnIndexes.size();
		vn1.clear();
		for(i=0;i<n1;i++){
			vn1.push_back(vnIndexes[i]);
		}
	}
	return vnIndexes;
}
vector <int> CIncidenceSubsetBuilder::expandIndex(int index, int nType){
	vector <int> vnIndexes;
	int i;
	switch (nType){
	case 0:
		vnIndexes.push_back(index);
		break;
	case 1:
		structBurstNode bNode;
		bNode=m_structBurstNodes.burstNodes[index];
		for(i=bNode.nFirstOpening;i<=bNode.nLastOpening;i++){
			vnIndexes.push_back(i);
		}
		break;
	case 2:
		structClusterNode cNode;
		cNode=m_structClusterNodes.clusterNodes[index];
		for(i=cNode.nFirstBurst;i<=cNode.nLastBurst;i++){
			vnIndexes.push_back(i);
		}
		break;
	}
	return vnIndexes;
}
vector <int> CIncidenceSubsetBuilder::shiftIndexes(vector <int> setA, int nShift, int nSize){
	vector <int> setB;
	int n=setA.size();
	int index;
	for(int i=0;i<n;i++){
		index=setA[i]+nShift;
		if(index<0||index>=nSize) continue;
		setB.push_back(index);
	}
	return setB;
}

void CIncidenceSubsetBuilder::fillUpBaseline(){
	int nLevel;
	float fBaseline=0;
	for(int i=0;i<m_nNumEvents;i++){
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==0)fBaseline=m_pfBaseline[i];
		m_pfBaseline[i]=fBaseline;
	}
}

vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::importSubsetCriteriaNodes(CString sFileName)
{
	FILE* fp;
	if(!m_cFileNameHandler.correctExtension(sFileName,"isc",false)){
		return importSubsetCriteriaNodes();
	}else{
		if(fp=fopen(sFileName,"r")){
			return readIncidenceSubsetCriteriaFile(fp);
			m_bValidCriteria=true;
		}else{
			return importSubsetCriteriaNodes();
		}
	}
}

structIncidenceSubsetCollectionNode CIncidenceSubsetBuilder::getSebsets(){
	return m_structSubsets;
}

void CIncidenceSubsetBuilder::updateISCID()
{
	m_nISCID=m_nSN;
	m_nSN++;
}

int CIncidenceSubsetBuilder::getISCID()
{
	return m_nISCID;
}

int CIncidenceSubsetBuilder::getEvlID()
{
	return m_nEvlID;
}

int CIncidenceSubsetBuilder::getBurstID()
{
	return m_nBurstID;
}

void CIncidenceSubsetBuilder::updateBursts(structBurstNodes structBurstNodes)
{
	m_bBurstNode=true;
	m_structBurstNodes=structBurstNodes;
	m_nBurstID=structBurstNodes.nBurstID;
}


void CIncidenceSubsetBuilder::updateClusters(structClusterNodes structClusterNodes)
{
	if(structClusterNodes.clusterNodes.size()>0) m_bClusterNode=true;
	m_structClusterNodes=structClusterNodes;
	m_nClusterID=structClusterNodes.nClusterID;
}

int CIncidenceSubsetBuilder::getClusterID()
{
	return m_nClusterID;
}

CString CIncidenceSubsetBuilder::getISCAsString()
{
	int nSize,i;
	CString sT;
	float fTcb=m_structSubsets.dTcb,fTcc=m_structSubsets.dTcc;
	if(fTcb>0)sT+="_Tcb_"+m_cFileNameHandler.ToString(fTcb);
	if(fTcc>0)sT+="_Tcc_"+m_cFileNameHandler.ToString(fTcc);
	nSize=m_vstructISCriteriaNodes.size();
	structIncidenceSubsetCriteriaNode aISCNode;
	for(i=0;i<nSize;i++){
		sT+=getISCAsString(m_vstructISCriteriaNodes[i]);
	}
	return sT;
}

CString CIncidenceSubsetBuilder::getISCAsString(structIncidenceSubsetCriteriaNode aISCNode)
{
	int nSize,i;
	CString sT;
	int nOPI=aISCNode.nOperationType,nAPI=aISCNode.nApplyingIndex;//nOPI is operation index of the subset, nAPI: applying index
	if(nOPI!=1) sT+="-OPI-"+m_cFileNameHandler.ToString(nOPI);
	if(nAPI!=0) sT+="-API-"+m_cFileNameHandler.ToString(nAPI);
	nSize=aISCNode.vstructIncidenceCriteria.size();
	for(i=0;i<nSize;i++){
		sT+=getICAsString(aISCNode.vstructIncidenceCriteria[i]);
	}
	return sT;
}

CString CIncidenceSubsetBuilder::getICAsString(structIncidenceCriterionNode aICNode)
{//This function must match with getIncidenceSubsetCriterionID. checked on 10/8/2009
	CString sT;
	int nRI=aICNode.nRelatingIndex;
	if(nRI!=0) sT+="-RI-"+m_cFileNameHandler.ToString(nRI);
	int nID=aICNode.nCriterionID;
	switch(nID){
	case 0:
		nID++;
		sT+="-level-"+m_cFileNameHandler.ToString(int(aICNode.dV1))+"to"+m_cFileNameHandler.ToString(int(aICNode.dV2));
		break;
	case 1:
		sT+="-dwellTime-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 2:
		sT+="-Amp-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 3:
		sT+="-LatancyT-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 4:
		sT+="-LatancyN-"+m_cFileNameHandler.ToString(aICNode.dV1,0)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,0);
		break;
	case 5:
		sT+="-IBCRange-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 6:
		sT+="-BToRange-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 7:
		sT+="-TToRange-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 8:
		sT+="-NOPBRange-"+m_cFileNameHandler.ToString(aICNode.dV1,0)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,0);
		break;
	case 9:
		sT+="-ICCRange-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 10:
		sT+="-ICCRange-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 11:
		sT+="-NOPC-"+m_cFileNameHandler.ToString(aICNode.dV1,0)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,0);
		break;
	case 12:
		sT+="-HalfBursN-"+m_cFileNameHandler.ToString(aICNode.dV1,0)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,0);
		break;
	case 13:
		sT+="-HalfBurstT-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 14:
		sT+="-HalfClusterN-"+m_cFileNameHandler.ToString(aICNode.dV1,0)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,0);
		break;
	case 15:
		sT+="-HalfClusterT-"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);
		break;
	case 16:
		sT+="-ElapsedTimeRange(ms)"+m_cFileNameHandler.ToString(aICNode.dV1,1)+"to"+m_cFileNameHandler.ToString(aICNode.dV2,1);;
		break;
	case 17:
		sT+="-AllEvents";
		break;
	case 18:
		sT+="-AllBursts";
		break;
	case 19:
		sT+="-AllClusters";
		break;
	default:
		break;
	}
	return sT;
}

CString CIncidenceSubsetBuilder::getEvlFileName(){
	return m_sEvlFileName;
}

AbfNode CIncidenceSubsetBuilder::getSubsetsAsTrace(int nKey, AbfNode structRawTrace)//nKey: 0 for level, 1 for amp
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
	vector <int> vnIndexes=getFinalIndexes(0);
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
/*
void CIncidenceSubsetBuilder::addjustTraceBaseline(AbfNode &aNode){
	int nSize=aNode.nSize;
	float fMaxAmp=100.;
	float* pfAve=new float[nSize];
	double dTcb=30.;
	if(m_structBurstNodes.dTcb<0){
		buildBurstNodes(dTcb);
	}
	int nNumBursts=m_structBurstNodes.burstNodes.size();
	int i;
	structBurstNode bstNode=m_structBurstNodes.burstNodes[0];
	int nRizing=2*m_nRising;
	int nWS=100;
	int nIBCL=bstNode.nPrecedingIBC;
	double dAveL, dAveR;
	int nPositionL, nPositionR;
	int nBurstIndex=0;

	CWindowSliding wld;
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
	int nIBCR=bstNode.nLastOpening+1;
	double dK;
	int nLen;
	int nLastPoint;
	while(nIBCR<m_nNumEvents){
		nPositionL=nBPR;
		dAveL=dAveR;
		nBPL=m_pnLevelStart[nIBCR]+nRizing;
		nBPR=m_pnLevelStart[nIBCR]+m_pnLevelLength[nIBCR]-nRizing;
		wld.calSWAverage(aNode.pfData,pfAve,nBPL,nBPR,1,nWS);	
		dAveR=pfAve[nBPL];
		nPositionR=nBPL;
		dK=dAveR-dAveL;
		nLen=nPositionR-nPositionL;
		dK/=nLen;
		for(i=1;i<nLen;i++){
			aNode.pfData[nPositionL+i]-=(dAveL+i*dK);
		}
		for(i=nBPL;i<=nBPR;i++){
			aNode.pfData[i]-=pfAve[i];
		}
		nBurstIndex++;
		if(nBurstIndex>=nNumBursts){
			nLastPoint=m_pnLevelStart[m_nNumEvents-1]+m_pnLevelLength[m_nNumEvents-1]-1;
			for(i=nBPR+1;i<=nLastPoint;i++){
				aNode.pfData[i]-=dAveR;
			}
			break;
		}
		bstNode=m_structBurstNodes.burstNodes[nBurstIndex];
		nIBCR=bstNode.nLastOpening+1;
	}
}
*/
void CIncidenceSubsetBuilder::addjustTraceBaseline(AbfNode &aNode){
	int nSize=aNode.nSize;
	float fMaxAmp=100.;
	float* pfAve=new float[nSize];
	double dTcb=30.;
	if(m_structBurstNodes.dTcb<0){
		buildBurstNodes(dTcb);
	}
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

double CIncidenceSubsetBuilder::interpolation(double x1, double y1, double x2, double y2, double x)
{
	double k=(y2-y1)/(x2-x1);
	return y1+k*(x-x1);
}
void CIncidenceSubsetBuilder::exportAPH_Subset(double dMin, double dMax, AbfNode& abfNode, int nWS, int nIT, double dRatio0){
	CLinearHistogramHandler lhh;
	vector<CLinearHistogramDouble> hists;
	CLinearHistogramDouble hist=obtainEventAPH(getFinalIndexes(0),abfNode,dMin, dMax, dRatio0);
	hist.setTitles("Amp(pA)","NormalizedCounts");
	hists.push_back(hist);
	CString sFileName=m_sEvlFileName;
	CFileNameHandling fhh;
	sFileName=fhh.ExtendFileName(sFileName,getISCAsString());
	sFileName=fhh.ChangeFileExt(sFileName,"Lap");
	FILE* fp=fopen(sFileName,"w");
	lhh.exportHistograms(fp,hists,nWS,nIT);
	fclose(fp);
}

CLinearHistogramDouble CIncidenceSubsetBuilder::obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax)
{
	double dRatio0=1.;
	return obtainEventAPH(vnIndexes, aNode, dMin, dMax, dRatio0);
}
CLinearHistogramDouble CIncidenceSubsetBuilder::obtainEventAPH(vector <int> vnIndexes, AbfNode &aNode, double dMin, double dMax, double dRatio0)
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

void CIncidenceSubsetBuilder::clearSubsets()
{
	m_vstructISCriteriaNodes.clear();
	m_structSubsets.vstructSubsets.clear();
}

void CIncidenceSubsetBuilder::appendSubsets(structIncidenceSubsetCriteriaNode aISCNode){
	m_structSubsets.vstructSubsets.push_back(buildSubset(aISCNode));
}
void CIncidenceSubsetBuilder::loadIncidenceSubsetCriteriaNodes(){
	m_vstructISCriteriaNodes=importSubsetCriteriaNodes();
	updateISCID();
	m_bValidCriteria=true;
}
void CIncidenceSubsetBuilder::loadIncidenceSubsetCriteriaNodes(vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes){
	m_vstructISCriteriaNodes=vstructISCriteriaNodes;
	updateISCID();
	m_bValidCriteria=true;
}
void CIncidenceSubsetBuilder::loadIncidenceSubsetCriteriaNodes(CString sFileName){
	m_vstructISCriteriaNodes=importSubsetCriteriaNodes(sFileName);
	m_bValidCriteria=true;
}
void CIncidenceSubsetBuilder::appendSubsets(CString sFileName){
	vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes=importSubsetCriteriaNodes(sFileName);
	appendSubsets(vstructISCriteriaNodes);
}

void CIncidenceSubsetBuilder::appendSubsets(vector <structIncidenceSubsetCriteriaNode> vstructISCriteriaNodes){
	int nSize=vstructISCriteriaNodes.size();
	int i;
	structIncidenceSubsetCriteriaNode aISCNode;
	for(i=0;i<nSize;i++){
		aISCNode=vstructISCriteriaNodes[i];
		m_vstructISCriteriaNodes.push_back(aISCNode);
		m_structSubsets.vstructSubsets.push_back(buildSubset(aISCNode));
	}
}

void CIncidenceSubsetBuilder::exportDwellTimeHistograms(){
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
	vector <int> vnIndexes=getFinalIndexes(0);
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
	sFileName=fhh.ExtendFileName(sFileName,getISCAsString());
	sFileName=fhh.ChangeFileExt(sFileName,"Dwt");
	FILE* fp=fopen(sFileName,"w");
	lhh.exportHistograms(fp,hists,0,0);
	fclose(fp);
}

void CIncidenceSubsetBuilder::calDwellTimeRanges(){
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
void CIncidenceSubsetBuilder::setRisetime(float fRisetime){
	m_bRisetime=true;
	m_nRising=int (fRisetime*1000./m_fTimePerPoint)+1;
}
void CIncidenceSubsetBuilder::addjustAmplitude(AbfNode aNode){
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

float* CIncidenceSubsetBuilder::getAmplitude(){
	return m_pfAmplitude;
}

vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::getISC_AllEvents(){
	vector <structIncidenceSubsetCriteriaNode> svISC;
	structIncidenceSubsetCriteriaNode ISC;
	ISC.nApplyingIndex=0;
	ISC.nIncidenceType=0;
	ISC.nOperationType=1;
	structIncidenceCriterionNode IC;
	IC.dV1=0.;
	IC.dV2=0.;
	IC.sCriterionID="AllEvents";
	IC.nCriterionID=getIncidenceSubsetCriterionID(IC.sCriterionID);
	IC.nRelatingIndex=0;
	ISC.vstructIncidenceCriteria.push_back(IC);
	svISC.push_back(ISC);
	return svISC;
}
vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::getISC_AllBursts(){
	vector <structIncidenceSubsetCriteriaNode> svISC;
	structIncidenceSubsetCriteriaNode ISC;
	ISC.nApplyingIndex=0;
	ISC.nIncidenceType=0;
	ISC.nOperationType=1;
	structIncidenceCriterionNode IC;
	IC.dV1=0.;
	IC.dV2=0.;
	IC.sCriterionID="AllBursts";
	IC.nCriterionID=getIncidenceSubsetCriterionID(IC.sCriterionID);
	IC.nRelatingIndex=0;
	ISC.vstructIncidenceCriteria.push_back(IC);
	svISC.push_back(ISC);
	return svISC;
}
vector <structIncidenceSubsetCriteriaNode> CIncidenceSubsetBuilder::getISC_AllClusters(){
	vector <structIncidenceSubsetCriteriaNode> svISC;
	structIncidenceSubsetCriteriaNode ISC;
	ISC.nApplyingIndex=0;
	ISC.nIncidenceType=0;
	ISC.nOperationType=1;
	structIncidenceCriterionNode IC;
	IC.dV1=0.;
	IC.dV2=0.;
	IC.sCriterionID="AllClusters";
	IC.nCriterionID=getIncidenceSubsetCriterionID(IC.sCriterionID);
	IC.nRelatingIndex=0;
	ISC.vstructIncidenceCriteria.push_back(IC);
	svISC.push_back(ISC);
	return svISC;
}


int CIncidenceSubsetBuilder::getPropertyType(CString sPropertyName)
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
int CIncidenceSubsetBuilder::getPropertyValue(int nIncidenceType, int nPropertyType, int nIndex, double &dValue, int &nValue, int &nPrintingPrecision)
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

int CIncidenceSubsetBuilder::calLatencyT(int nType){
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
int CIncidenceSubsetBuilder::calLatencyN(int nType){
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