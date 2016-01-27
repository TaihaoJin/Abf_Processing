// BurstHandling.cpp: implementation of the CBurstHandling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "BurstHandling.h"
#include "math.h"
#include "CUF.h"
#include "GeometricFitting.h"
#include "ExponentialFitting.h"
#include "Abf.h"
#include "MeanSem.h"
#include "FileNameHandling.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
int CBurstHandling::m_nSN;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBurstHandling::CBurstHandling()
{
	m_structAbfNode.nSize=-1;
	m_pnLevelStart=NULL;
	m_pnLevelLength=NULL;
	m_pstEventCurrentLevel=NULL;
	m_pfAmplitude=NULL;
	m_pbExcluded=NULL;

	m_pfTc_Time0=NULL;
	m_pfTo_Time0=NULL;
	m_pfTc_Sem0=NULL;
	m_pfTo_Sem0=NULL;
	m_pfTc_Num0=NULL;
	m_pfTo_Num0=NULL;
	m_pfTc_Time1=NULL;
	m_pfTo_Time1=NULL;
	m_pfTc_Sem1=NULL;
	m_pfTo_Sem1=NULL;
	m_pfTc_Num1=NULL;
	m_pfTo_Num1=NULL;

	m_pfAmp_Ave0=NULL;
	m_pfAmp_Sem0=NULL;
	m_pfAmp_Num0=NULL;
	m_pfAmp_Ave1=NULL;
	m_pfAmp_Sem1=NULL;
	m_pfAmp_Num1=NULL;

	m_pfBMDTc_Time0=NULL;
	m_pfBMDTo_Time0=NULL;
	m_pfBMDTc_Sem0=NULL;
	m_pfBMDTo_Sem0=NULL;
	m_pfBMDTc_Num0=NULL;
	m_pfBMDTo_Num0=NULL;
	m_pfBMDTc_Time1=NULL;
	m_pfBMDTo_Time1=NULL;
	m_pfBMDTc_Sem1=NULL;
	m_pfBMDTo_Sem1=NULL;
	m_pfBMDTc_Num1=NULL;
	m_pfBMDTo_Num1=NULL;

	m_bBurstNode=false;
	m_bClusterNode=false;
	m_bNOPBHist=false;
	m_bTOTPBHist_Log=false;
	m_bTOTPBHist_HB=false;
	m_bBurstDurationHist_Log=false;
	m_bBurstDurationHist_HB=false;
	m_nBurstDelimiterIndex=0;
	m_fRecordLength=0.;
	m_nSortingIndex=-1;
	m_nBurstID=-1;
}

CBurstHandling::~CBurstHandling()
{

}

void CBurstHandling::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
								int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
								short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, AbfNode aAbfNode){
	m_structAbfNode=aAbfNode;
	UpdateData(NumChannels, NumLevels,TimePerPoint,nNumEvents, pnLevelStart, pnLevelLength, pfAmplitude,pfBaseline,
								 pstEventCurrentLevel,sEvlFileName, pbExcluded);
}

void CBurstHandling::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
								int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
								short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded)
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

	m_pnLevelStart=new int[nNumEvents];
	m_pnLevelLength=new int[nNumEvents];
	m_pstEventCurrentLevel=new short[nNumEvents];
	m_pfAmplitude=new float[nNumEvents];
	m_pfBaseline=new float[nNumEvents];
	m_pbExcluded=pbExcluded;
	m_fRecordLength=0.;

	for(int i=0;i<nNumEvents;i++)
	{
		*(m_pnLevelStart+i)=*(pnLevelStart+i);
		*(m_pnLevelLength+i)=*(pnLevelLength+i);
		*(m_pstEventCurrentLevel+i)=*(pstEventCurrentLevel+i);
		*(m_pfAmplitude+i)=*(pfAmplitude+i);
		*(m_pfBaseline+i)=*(pfBaseline+i);
		m_fRecordLength+=*(pnLevelLength+i)*TimePerPoint/1000.;
	}
	m_bClusterNode=false;
	m_bBurstNode=false;
	m_bNOPBHist=false;
	m_bTOTPBHist_Log=false;
	m_bTOTPBHist_HB=false;
	m_bBurstDurationHist_Log=false;
	m_bBurstDurationHist_HB=false;
	m_nBurstDelimiterIndex=0;
	m_bLevelAPHBuilt=false;
	CBurstingTimeCourseOptionDlg aDlg;
	aDlg.initialize(m_structBurstingTimeCourseOptionNode);
	m_bBurstCorrelationHist=false;
}

void CBurstHandling::UpdateData(int NumChannels, int NumLevels,float TimePerPoint,int nNumEvents,
								int* pnLevelStart,int* pnLevelLength,float* pfAmplitude,float* pfBaseline,
								short* pstEventCurrentLevel,CString sEvlFileName, bool* pbExcluded, int nEvlID)
{
	UpdateData(NumChannels, NumLevels, TimePerPoint, nNumEvents, pnLevelStart, pnLevelLength, pfAmplitude,
				pfBaseline,pstEventCurrentLevel,sEvlFileName,pbExcluded);
	m_nEvlID=nEvlID;
}


void CBurstHandling::obtainParameters()
{
	if(!m_structBurstingTimeCourseOptionNode.bSet){
		CBurstingTimeCourseOptionDlg aDlg;
		aDlg.initialize(m_structBurstingTimeCourseOptionNode);
		aDlg.SetParameters(m_structBurstingTimeCourseOptionNode);
		if(aDlg.DoModal()==IDOK)
		{
			aDlg.getBurstTimecourseOptions(m_structBurstingTimeCourseOptionNode);
		}
	}
}

void CBurstHandling::MakeBurstNodes(structBurstingTimeCourseOptionNode aNode)
{
	m_structBurstingTimeCourseOptionNode=aNode;
	MakeBurstNodes();
}

void CBurstHandling::MakeBurstNodes()
{
	if(!m_structBurstingTimeCourseOptionNode.bSet) obtainParameters();
	
	m_structvBurstNodes.clear();
	int i,nTemp;
	int nLevel,nLength;//nIBC0, nIBC: inter-bursting closed event
	float fTime;
	m_fMaxOpenTime=0.f;
	int maxLength=0;

	int nLevel0=0;

	int nPosition;
	int nNumBursts=0;
	for(i=0;i<m_nNumEvents;i++)
	{
		nLevel=m_pstEventCurrentLevel[i];

		if(nLevel==1){
			nLength=m_pnLevelLength[i];
			if(nLength>maxLength) maxLength=nLength;
		}

		if(InterBurstClosing(i))
		{
			structBurstNode aNode;
			aNode.bValid=true;
			aNode.nPrecedingIBC=i;
			aNode.nNumClosings=0;
			aNode.nNumOpenings=0;
			aNode.fTotalOpenTime=0.f;
			aNode.fLength=0.f;
			aNode.fPrecedingIBC=m_pnLevelLength[i]*m_fTimePerPoint/1000.;
			aNode.nFirstOpening=FirstOpening(i);
			m_structvBurstNodes.push_back(aNode);
			if(nNumBursts>0)
			{
				nPosition=i;
				nLevel=m_pstEventCurrentLevel[nPosition];
				while(nLevel!=1&&nPosition>=0)
				{
					nPosition--;
					nLevel=m_pstEventCurrentLevel[nPosition];
				}
				m_structvBurstNodes[nNumBursts-1].nLastOpening=nPosition;
				nTemp=m_structvBurstNodes[nNumBursts-1].nNumOpenings;
				if(nTemp>m_nMaxOpeningNum)
				{
					m_nMaxOpeningNum=nTemp;
				}
			}
			nNumBursts++;
		}
		else
		{
			if(nNumBursts>0)
			{
				nLevel=m_pstEventCurrentLevel[i];
				nLevel0=m_pstEventCurrentLevel[i-1];
				nLength=m_pnLevelLength[i];
				fTime=nLength*m_fTimePerPoint/1000.;
				if(nLevel==0)
				{
					m_structvBurstNodes[nNumBursts-1].nNumClosings++;
				}
//				else if(nLevel==1)//11/30/07
				else
				{
//					m_structvBurstNodes[nNumBursts-1].fTotalOpenTime+=fTime;//11/30/07
//					m_structvBurstNodes[nNumBursts-1].nNumOpenings++;//11/30/07
					m_structvBurstNodes[nNumBursts-1].fTotalOpenTime+=fTime*nLevel;
					if(nLevel>nLevel0) m_structvBurstNodes[nNumBursts-1].nNumOpenings+=nLevel-nLevel0;
				}
				m_structvBurstNodes[nNumBursts-1].fLength+=fTime;
				m_structvBurstNodes[nNumBursts-1].nNumevents++;
			}
		}
	}
	if(nNumBursts>0)
	{
		nPosition=i;
		nLevel=m_pstEventCurrentLevel[nPosition];
		while(nLevel!=1&&nPosition>=0)
		{
			nPosition--;
			nLevel=m_pstEventCurrentLevel[nPosition];
		}
		m_structvBurstNodes[nNumBursts-1].nLastOpening=nPosition;
		nTemp=m_structvBurstNodes[nNumBursts-1].nNumOpenings;
		if(nTemp>m_nMaxOpeningNum)
		{
			m_nMaxOpeningNum=nTemp;
		}
	}
	//remove burst nodes that not meet the filtering requirements.
	if(m_structBurstingTimeCourseOptionNode.fClusterDelimiter>m_structBurstingTimeCourseOptionNode.fBurstDelimiter) MakeClusterNodes(m_structBurstingTimeCourseOptionNode.fClusterDelimiter);
	filteringBurstNodes();
	m_fMaxOpenTime=maxLength*m_fTimePerPoint/1000.;
	m_nNumBins=nNumBursts/m_structBurstingTimeCourseOptionNode.nBinSize;
	CalMeanNOPB();
	m_bBurstNode=true;
	int nSize=m_structvBurstNodes.size();
	m_vnSortedOrder.clear();
	m_vnRanking.clear();
	for(i=0;i<nSize;i++)
	{
		m_vnSortedOrder.push_back(i);
		m_vnRanking.push_back(i);
	}
	if(m_structBurstingTimeCourseOptionNode.nSortingIndex>0)
	{
		SortBurstNodes(m_structBurstingTimeCourseOptionNode.nSortingIndex);
	}
	updateID();
}


void CBurstHandling::MakeClusterNodes(float fCutoff)
{
	//This function assumes the burst nodes have been made.
	int i,nTemp,nSize;
	int nLevel,nLength;//nIBC0, nIBC: inter-bursting closed event
	float fTime;

	m_structvClusterNodes.clear();

	int nPosition;
	int nNumClusters=0;
	nSize=m_structvBurstNodes.size();
	m_nMaxBurstingNum=0;
	int nNumBursts=0;
	structClusterNode cNode;
	for(i=0;i<nSize;i++)
	{
		if(InterClusterClosing(i))
		{
			if(nNumBursts>m_nMaxBurstingNum)
			{
				m_nMaxBurstingNum=nNumBursts;
			}
			if(nNumClusters>0){
				m_structvClusterNodes[nNumClusters-1].nAveNOPB/=nNumBursts;
				m_structvClusterNodes[nNumClusters-1].fAveBstDuration/=nNumBursts;
				m_structvClusterNodes[nNumClusters-1].fAveTOT/=nNumBursts;
			}
			nNumBursts=0;
			structClusterNode aNode;
			aNode.bValid=true;
			aNode.fAveBstDuration=m_structvBurstNodes[i].fLength;
			aNode.fAveTOT=m_structvBurstNodes[i].fTotalOpenTime;
			aNode.nAveNOPB=m_structvBurstNodes[i].nNumOpenings;
			aNode.nNumOpenings=m_structvBurstNodes[i].nNumOpenings;
			aNode.fTotalOpenTime=m_structvBurstNodes[i].fTotalOpenTime;
			aNode.fLength=m_structvBurstNodes[i].fLength;
			aNode.fPrecedingICC=m_structvBurstNodes[i].fPrecedingIBC;
			aNode.nPrecedingICC=m_structvBurstNodes[i].nPrecedingIBC;
			aNode.svBurstNodes.push_back(m_structvBurstNodes[i]);
			aNode.nMaxNOPB=m_structvBurstNodes[i].nNumOpenings;
			aNode.nFirstBurst=i;//09929
			m_structvClusterNodes.push_back(aNode);
			nNumClusters++;
			nNumBursts++;
		}
		else
		{
			if(nNumClusters>0)
			{
				m_structvClusterNodes[nNumClusters-1].fLength+=m_structvBurstNodes[i].fLength+m_structvBurstNodes[i].fPrecedingIBC;
				m_structvClusterNodes[nNumClusters-1].fAveTOT+=m_structvBurstNodes[i].fLength+m_structvBurstNodes[i].fTotalOpenTime;
				m_structvClusterNodes[nNumClusters-1].fTotalOpenTime+=m_structvBurstNodes[i].fTotalOpenTime;
				m_structvClusterNodes[nNumClusters-1].svBurstNodes.push_back(m_structvBurstNodes[i]);
				m_structvClusterNodes[nNumClusters-1].nAveNOPB+=m_structvBurstNodes[i].nNumOpenings;
				m_structvClusterNodes[nNumClusters-1].nNumOpenings+=m_structvBurstNodes[i].nNumOpenings;
				m_structvClusterNodes[nNumClusters-1].fAveBstDuration+=m_structvBurstNodes[i].fLength;
				m_structvClusterNodes[nNumClusters-1].nLastBurst=i;//09929
				if(m_structvBurstNodes[i].nNumOpenings > m_structvClusterNodes[nNumClusters-1].nMaxNOPB)
				{
					m_structvClusterNodes[nNumClusters-1].nMaxNOPB=m_structvBurstNodes[i].nNumOpenings;
				}
				nNumBursts++;
			}
		}
	}	
	typedef vector <structClusterNode> ClusterVecter;
	ClusterVecter::iterator theIterator=m_structvClusterNodes.begin()+nNumClusters-1;
	m_structvClusterNodes.erase(theIterator);
	m_bClusterNode=true;
	filteringClusters();
}


bool CBurstHandling::InterBurstClosing(int nIndex)
{
	bool bTemp=false;
	int nLevel=m_pstEventCurrentLevel[nIndex];
	int nLength;
	float fTime;
	if(nLevel==0)
	{
		nLength=m_pnLevelLength[nIndex];
		fTime=nLength*m_fTimePerPoint/1000.;	
		if(fTime>m_structBurstingTimeCourseOptionNode.fBurstDelimiter)
		{
			bTemp=true;
		}
	}
	return bTemp;
}

void CBurstHandling::ExportBurstStatitics()
{
	MakeBurstNodes();
	OutputBurstingTimeCourse();
}

void CBurstHandling::OutputBurstingTimeCourse()
{
	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sAbfFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Export the timecourse file (*.Tcr)";
	CString sFileName=ChangeFileExt(m_sEvlFileName,"Tcr");
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);
//	FilteringBurstNodes_NOPB(50,600000); //comment it off later 05/07

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(fpAbfOut=fopen(sAbfFileName,"w"))
		{
		    WriteBurstingTimeCourse(fpAbfOut);
		    fclose(fpAbfOut);
			m_sTcrFileName=sAbfFileName;
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}

		sAbfFileName=ChangeFileExt(sAbfFileName,"Crr");
		if(fpAbfOut=fopen(sAbfFileName,"w"))
		{
		    WriteSortedHistogram(fpAbfOut);
		    fclose(fpAbfOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}


		sAbfFileName=ChangeFileExt(sAbfFileName,"TUB");
		if(fpAbfOut=fopen(sAbfFileName,"w"))
		{
			MakeBurstTimeCourse_UB(m_structBurstingTimeCourseOptionNode.fBinWidth);
		    WriteBurstingTimeCourse_UB(fpAbfOut,sAbfFileName);
		    fclose(fpAbfOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}



		sFileName=ChangeFileExt(sAbfFileName,"BST");
		if(fpAbfOut=fopen(sFileName,"w"))
		{
			CalMDT_EventIndex();
			CalMDTSem_EventIndex();
			CalAmp_EventIndex();
			CalAmpSem_EventIndex();
		    WriteBMDT(fpAbfOut);
		    fclose(fpAbfOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}

		if(m_structBurstingTimeCourseOptionNode.fClusterDelimiter>m_structBurstingTimeCourseOptionNode.fBurstDelimiter)
		{
			sFileName=ChangeFileExt(sAbfFileName,"CST");
			if(fpAbfOut=fopen(sFileName,"w"))
			{
				MakeClusterNodes(m_structBurstingTimeCourseOptionNode.fClusterDelimiter);
				CalMDT_EventIndex_Cluster();
				CalMDTSem_EventIndex_Cluster();
				WriteBMDT_Cluster(fpAbfOut);
				fclose(fpAbfOut);
			}
			else
			{
				AfxMessageBox(sMessage,MB_OK,0);
			}

			sFileName=ChangeFileExt(sAbfFileName,"CCR");
			if(fpAbfOut=fopen(sFileName,"w"))
			{
				CalClusterAve();
				WriteClusterTimeCourse(fpAbfOut);
				fclose(fpAbfOut);
			}
			else
			{
			}
			sFileName=ChangeFileExt(sAbfFileName,"TMP");
			if(fpAbfOut=fopen(sFileName,"w")){
				WriteTMP(fpAbfOut);
				fclose(fpAbfOut);
			}else{
					AfxMessageBox(sMessage,MB_OK,0);
			}
		}
		sFileName=ChangeFileExt(sAbfFileName,"LAH");
		if(fpAbfOut=fopen(sFileName,"w")){
			WriteLevelAmpHist(fpAbfOut);
			fclose(fpAbfOut);
		}else{
				AfxMessageBox(sMessage,MB_OK,0);
		}
		sFileName=ChangeFileExt(sAbfFileName,"LA2");
		if(fpAbfOut=fopen(sFileName,"w")){
			WriteLevelAmpHist2(fpAbfOut);
			fclose(fpAbfOut);
		}else{
				AfxMessageBox(sMessage,MB_OK,0);
		}
	}
}

void CBurstHandling::WriteLevelAmpHist(FILE* fpOutput){
	buildLevelAmpHist();
	bool bDoAPH=true;
	if(m_structAbfNode.nSize==0) bDoAPH=false;
	if (bDoAPH) fprintf(fpOutput,"           Amp          APH1          APH2        APH1r         APH2r       APHSum1       APHSum2");
	fprintf(fpOutput,"           Amp        hist01        hist02       hist01r       hist02r        hist11        hist12       hist11r       hist12r          sum1          sum2");
	fprintf(fpOutput,"\n");
	double dBase,dDelta,dSum;
	int nDim1=0,nDim2=0,nDim=0;
	if(bDoAPH) m_cAPH1.getParameters(dBase,dDelta,nDim1);
	m_cLevelAmpHist01.getParameters(dBase,dDelta,nDim2);
	nDim=nDim1;
	if(nDim2>nDim1) nDim=nDim2;
	int i;
	
	for(i=0;i<nDim;i++){
		if(bDoAPH) fprintf(fpOutput,"  %12.2f  %12.4f  %12.4f  %12.4f  %12.4f",m_cAPH1.getPosition(i), m_cAPH1.getCounts(i)/m_cAPH1.getTotalCounts(), m_cAPH2.getCounts(i)/m_cAPH2.getTotalCounts(), m_cAPH1r.getCounts(i)/m_cAPH1r.getTotalCounts(), m_cAPH2r.getCounts(i)/m_cAPH2r.getTotalCounts());
		if(bDoAPH) fprintf(fpOutput,"  %12.4f  %12.4f",(m_cAPH1.getCounts(i)+m_cAPH1r.getCounts(i))/(m_cAPH1r.getTotalCounts()+m_cAPH1.getTotalCounts()),(m_cAPH2.getCounts(i)+m_cAPH2r.getCounts(i))/(m_cAPH2.getTotalCounts()+m_cAPH2r.getTotalCounts()));
		if(i<nDim2)
		{
			fprintf(fpOutput,"  %12.2f  %12.4f  %12.4f  %12.4f  %12.4f",m_cLevelAmpHist01.getPosition(i), m_cLevelAmpHist01.getCounts(i)/m_cLevelAmpHist01.getTotalCounts(), m_cLevelAmpHist02.getCounts(i)/m_cLevelAmpHist02.getTotalCounts(), m_cLevelAmpHist11.getCounts(i)/m_cLevelAmpHist11.getTotalCounts(), m_cLevelAmpHist12.getCounts(i)/m_cLevelAmpHist12.getTotalCounts());
			fprintf(fpOutput,"  %12.4f  %12.4f  %12.4f  %12.4f",m_cLevelAmpHist01r.getCounts(i)/m_cLevelAmpHist01r.getTotalCounts(), m_cLevelAmpHist02r.getCounts(i)/m_cLevelAmpHist02r.getTotalCounts(), m_cLevelAmpHist11r.getCounts(i)/m_cLevelAmpHist11r.getTotalCounts(), m_cLevelAmpHist12r.getCounts(i)/m_cLevelAmpHist12r.getTotalCounts());
			fprintf(fpOutput,"  %12.4f  %12.4f",(m_cLevelAmpHist11.getCounts(i)+m_cLevelAmpHist11r.getCounts(i))/(m_cLevelAmpHist11.getTotalCounts()+m_cLevelAmpHist11r.getTotalCounts()), (m_cLevelAmpHist12.getCounts(i)+m_cLevelAmpHist12r.getCounts(i))/(m_cLevelAmpHist12.getTotalCounts()+m_cLevelAmpHist12r.getTotalCounts()));
		}
		fprintf(fpOutput,"\n");
	}
}

void CBurstHandling::WriteLevelAmpHist2(FILE* fpOutput){
	double xi=-10.,xf=5.,delta=0.1,x1,x2;
	buildLevelAmpHist();
	bool bDoAPH=true;
	if(m_structAbfNode.nSize==0) bDoAPH=false;
	if (bDoAPH) fprintf(fpOutput,"           Amp         APHs1         APHs2         APHs1         APHs2         APH01         APH02        APHn1         APHn2");
	fprintf(fpOutput,"\n");
	double dBase,dDelta,dSum;
	int nDim1=0,nDim2=0,nDim=0;

	int i,index;

	CLinearHistogramHandler aHandler;
	CLinearHistogramDouble APHs1,APHs2,APH01,APH02,APHn1,APHn2;
	APHs1.copyHist(m_cAPH1);
	APHs1.mergeHist(m_cAPH1r);

	APH01.copyHist(APHs1);
	aHandler.update(APH01);
	index=aHandler.getLastSignificantPeak();
	APH01.resetBinPosition(index,0.);

	APHn1.copyHist(APHs1);
	if(APHs1.getTotalCounts()>0.1) aHandler.normalizePeakRange(-4.,0.,APHn1);

	APHs2.copyHist(m_cAPH2);
	APHs2.mergeHist(m_cAPH2r);

	APH02.copyHist(APHs2);
	aHandler.update(APH02);
	index=aHandler.getLastSignificantPeak();
	APH02.resetBinPosition(index,0.);

	APHn2.copyHist(APHs2);
	if(APHs2.getTotalCounts()>0.1) aHandler.normalizePeakRange(-4.,0.,APHn2);
	
	nDim=int((xf-xi)/delta)+1;
	for(i=0;i<nDim;i++){
		x1=xi+i*delta;
		x2=x1+delta;
		fprintf(fpOutput,"  %12.2f  %12.4f  %12.4f",x1,(m_cAPH1.getCounts(i)+m_cAPH1r.getCounts(i)),(m_cAPH2.getCounts(i)+m_cAPH2r.getCounts(i)));
		fprintf(fpOutput,"  %12.4f  %12.4f",APHs1.getCounts(x1,x2),APHs2.getCounts(x1,x2));
		fprintf(fpOutput,"  %12.4f  %12.4f",APH01.getCounts(x1,x2),APH02.getCounts(x1,x2));
		fprintf(fpOutput,"  %12.4f  %12.4f",APHn1.getCounts(x1,x2),APHn2.getCounts(x1,x2));
		fprintf(fpOutput,"\n");
	}
}

void CBurstHandling::WriteBurstingTimeCourse(FILE *fpOut)
{
	float fTime,fElapsedTime;
	int i,j,k;
	int nIndex0,nIndex1,nTemp;
	fprintf(fpOut,"          Time     MPoib         TToib      MBTo         MFoib          Tcib     IBIterval          MOib           MTo           MPo           MFo          MAmp     MaxLevel");
	for(k=0;k<2;k++)
	{
		for(j=0;j<20;j++)
		{
			if(j<10)
			{
				fprintf(fpOut,"       MDT_%1d_%1d",k,j);
			}
			else
			{
				fprintf(fpOut,"      MDT_%1d_%2d",k,j);
			}
		}
	}
	fprintf(fpOut,"\n");

	int nSize=m_structvBurstNodes.size();
	int nI,nF;

	for(i=0; i< nSize; i++)
	{
		nIndex0=i-m_structBurstingTimeCourseOptionNode.nBinSize/2;
		nIndex1=i+m_structBurstingTimeCourseOptionNode.nBinSize/2;
		if(nIndex0<0)
			nIndex0=0;

		if (nIndex1>nSize-1) nIndex1=nSize-1;
		fElapsedTime=ElapsedTime(nIndex0,nIndex1);

		nTemp=m_structvBurstNodes[i].nPrecedingIBC;
		fTime=m_pnLevelStart[nTemp]*m_fTimePerPoint/1000.;

			fprintf(fpOut,"  %12.4f",fTime);//Time
			fprintf(fpOut,"  %8.5f",0.00001);//Mpoib
			fprintf(fpOut,"  %12.2f",0.01);//TToib = Total open time in a burst
			fprintf(fpOut,"  %8.2f",0.01);//MBTo
			fprintf(fpOut,"  %12.4f",0.001);//MFoib
			fprintf(fpOut,"  %12.4f",0.001);//MTcib
			fprintf(fpOut,"  %12.4f",0.001);//IBInterval
			fprintf(fpOut,"  %12.2f",0.01);//MOib=Number of opening per burst
			fprintf(fpOut,"  %12.2f",0.01);//MTo
			fprintf(fpOut,"  %12.2f",0.01);//MPo
			fprintf(fpOut,"  %12.2f",0.01);//MFo
			fprintf(fpOut,"  %12.2f",0.01);//Amplitude
			fprintf(fpOut,"           %2d",0);//Maximum open level, must be one for this time of burst analysis.
			for(k=0;k<2;k++)
			{
				for(j=1;j<21;j++)
				{
					fprintf(fpOut,"  %12.2f",0.);
				}
			}
			fprintf(fpOut,"\n");


		nTemp=m_structvBurstNodes[i].nFirstOpening;
		fTime=m_pnLevelStart[nTemp]*m_fTimePerPoint/1000.;

			fprintf(fpOut,"  %12.4f",fTime);//Time
			fprintf(fpOut,"  %8.5f",TotalOpenTime(nIndex0, nIndex1)/TotalBurstLength(nIndex0, nIndex1));//Mpoib
			fprintf(fpOut,"  %12.2f",TotalOpenTime(nIndex0, nIndex1));//TToib = Total open time in a burst
			fprintf(fpOut,"  %8.2f",MBTo_0(nIndex0, nIndex1));//MBTo
			fprintf(fpOut,"  %12.4f",MFoib_0(nIndex0, nIndex1));//MFoib
			fprintf(fpOut,"  %12.4f",MTcib_0(nIndex0, nIndex1));//MTcib
			fprintf(fpOut,"  %12.4f",MIBC_0(nIndex0, nIndex1));//IBInterval
			fprintf(fpOut,"  %12.2f",Moib_0(nIndex0, nIndex1));//MOib=Number of opening per burst
			fprintf(fpOut,"  %12.2f",MTo_0(nIndex0, nIndex1));//MTo
			fprintf(fpOut,"  %12.2f",TotalOpenTime(nIndex0, nIndex1)/TotalLength(nIndex0, nIndex1));//MPo
			fprintf(fpOut,"  %12.2f",MFo_0(nIndex0, nIndex1));//MFo
			fprintf(fpOut,"  %12.2f",MAmp_0(nIndex0, nIndex1));//Amplitude
			fprintf(fpOut,"           %2d",1);//Maximum open level, must be one for this time of burst analysis.
			for(k=0;k<2;k++)
			{
				for(j=1;j<21;j++)
				{
					fprintf(fpOut,"  %12.2f",MDT(nIndex0,nIndex1,j,k));
				}
			}
			fprintf(fpOut,"\n");
	}
}



void CBurstHandling::WriteBurstingTimeCourse1(FILE *fpOut)
{
	float fTime,fElapsedTime;
	int i,j,k;
	int nIndex0,nIndex1,nTemp;
	fprintf(fpOut,"          Time     MPoib         TToib      MBTo         MFoib          Tcib     IBIterval          MOib           MTo           MPo           MFo          MAmp     MaxLevel");
	for(k=0;k<2;k++)
	{
		for(j=0;j<20;j++)
		{
			if(j<10)
			{
				fprintf(fpOut,"       MDT_%1d_%1d",k,j);
			}
			else
			{
				fprintf(fpOut,"      MDT_%1d_%2d",k,j);
			}
		}
	}
	fprintf(fpOut,"\n");

	int nSize=m_structvBurstNodes.size();
	int nI,nF;

	for(i=0; i< nSize; i++)
	{
		nIndex0=i-m_structBurstingTimeCourseOptionNode.nBinSize/2;
		nIndex1=i+m_structBurstingTimeCourseOptionNode.nBinSize/2;
		if(nIndex0<0)
			nIndex0=0;

		if (nIndex1>nSize-1) nIndex1=nSize-1;
		fElapsedTime=ElapsedTime(nIndex0,nIndex1);

		nTemp=m_structvBurstNodes[nIndex0].nFirstOpening;
		fTime=m_pnLevelStart[nTemp]*m_fTimePerPoint/1000.;

//		if(fNumOpening<0.2)
//		{
//			fprintf(fpOut,"  %12.4f ",fTime);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",fTotalOpenTime/1.);
//			fprintf(fpOut,"  %12.4f ",1000.f*fNumOpening/fElapsedTime);
//			fprintf(fpOut,"  %2d \n",*(m_pnMaximumOpenLevel+i));
//		}
//		else
//		{
			fprintf(fpOut,"  %12.4f",fTime);//Time
			fprintf(fpOut,"  %8.5f",TotalOpenTime(nIndex0, nIndex1)/TotalBurstLength(nIndex0, nIndex1));//Mpoib
			fprintf(fpOut,"  %12.2f",TotalOpenTime(nIndex0, nIndex1));//TToib = Total open time in a burst
			fprintf(fpOut,"  %8.2f",MBTo_0(nIndex0, nIndex1));//MBTo
			fprintf(fpOut,"  %12.4f",MFoib_0(nIndex0, nIndex1));//MFoib
			fprintf(fpOut,"  %12.4f",MTcib_0(nIndex0, nIndex1));//MTcib
			fprintf(fpOut,"  %12.4f",MIBC_0(nIndex0, nIndex1));//IBInterval
			fprintf(fpOut,"  %12.2f",Moib_0(nIndex0, nIndex1));//MOib=Number of opening per burst
			fprintf(fpOut,"  %12.2f",MTo_0(nIndex0, nIndex1));//MTo
			fprintf(fpOut,"  %12.2f",TotalOpenTime(nIndex0, nIndex1)/TotalLength(nIndex0, nIndex1));//MPo
			fprintf(fpOut,"  %12.2f",MFo_0(nIndex0, nIndex1));//MFo
			fprintf(fpOut,"  %12.2f",MAmp_0(nIndex0, nIndex1));//Amplitude
			fprintf(fpOut,"           %2d",1);//Maximum open level, must be one for this time of burst analysis.
			for(k=0;k<2;k++)
			{
				for(j=1;j<21;j++)
				{
					fprintf(fpOut,"  %12.2f",MDT(nIndex0,nIndex1,j,k));
				}
			}
			fprintf(fpOut,"\n");
//		}
	}
}




CString CBurstHandling::ChangeFileExt(CString sFileName, CString sExt)
{
//	char cTemp;
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	for(int i=0;i<3;i++)
	{
		sNewFileName.SetAt(nLength-3+i,sExt[i]);
	}
	return sNewFileName;	
}

float CBurstHandling::MTo(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;
	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNum+=m_structvBurstNodes[i].nNumOpenings;
		fTime+=m_structvBurstNodes[i].fTotalOpenTime;
	}
	float fMDT=fTime/nNum; //Mean dwelling time
	return fMDT;
}

float CBurstHandling::MTc(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNum+=m_structvBurstNodes[i].nNumClosings+1;
		fTime+=(m_structvBurstNodes[i].fLength-m_structvBurstNodes[i].fTotalOpenTime);
		fTime+=m_structvBurstNodes[i].fPrecedingIBC;
	}
	float fMDT=fTime/nNum; //Mean dwelling time
	return fMDT;
}


float CBurstHandling::MFo(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNum+=m_structvBurstNodes[i].nNumOpenings;
		fTime+=m_structvBurstNodes[i].fLength;
		fTime+=m_structvBurstNodes[i].fPrecedingIBC;
	}
	float fMFo=nNum/fTime; //Mean frequency of opening (intra burst);
	return fMFo;
}


float CBurstHandling::MIBC(int nIndex0, int nIndex1)
{
	//Mean Inter-burst interval
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	double dNum=1.;//09929

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		dNum+=1.;
		fTime+=m_structvBurstNodes[i].fPrecedingIBC;//09929
	}
	double dMIBC=fTime/dNum; 
	return dMIBC;
}

float CBurstHandling::ElapsedTime(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		fTime+=m_structvBurstNodes[i].fLength;
		fTime+=m_structvBurstNodes[i].fPrecedingIBC;
	}
	return fTime;
}

float CBurstHandling::TotalOpenTime(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		fTime+=m_structvBurstNodes[i].fTotalOpenTime;
	}
	return fTime;
}

float CBurstHandling::TotalBurstLength(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		fTime+=m_structvBurstNodes[i].fLength;
	}
	return fTime;
}

float CBurstHandling::MBTo(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	double dNum=0.;
	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		fTime+=m_structvBurstNodes[i].fLength;
		dNum+=1.;
	}
	return fTime/dNum;
}

float CBurstHandling::MFoib(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNum+=m_structvBurstNodes[i].nNumOpenings;
		fTime+=m_structvBurstNodes[i].fLength;
	}
	float fMFo=nNum/fTime; //Mean frequency of opening (intra burst);
	return fMFo;
}

float CBurstHandling::MTcib(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNum+=m_structvBurstNodes[i].nNumClosings;
		fTime+=(m_structvBurstNodes[i].fLength-m_structvBurstNodes[i].fTotalOpenTime);
	}
	float fMDT=fTime/nNum; //Mean dwelling time
	return fMDT;
}

float CBurstHandling::Moib(int nIndex0, int nIndex1)
{
	int i,nNum=0;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	double dNum=0.;
	for(i=nIndex0;i<=nIndex1;i++)
	{
		nNum+=m_structvBurstNodes[i].nNumOpenings;
		dNum+=1.;
	}
 
	return double(nNum)/dNum;//09929
}

float CBurstHandling::TotalLength(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		fTime+=m_structvBurstNodes[i].fLength;
		fTime+=m_structvBurstNodes[i].fPrecedingIBC;
	}
	return fTime;
}

int CBurstHandling::FirstOpening(int nPosition)
{
	//This function returns the index for the first open event after the event at nPosition.
	//This function does not check the event at nPosition.
	int i=nPosition,nLevel=0;
	while(nLevel!=1&&i<m_nNumEvents)
	{
		i++;
		nLevel=m_pstEventCurrentLevel[i];
	}
	if(nLevel==1)
	{
		return i;
	}
	else //end of event list and no openings after nPosition
	{
		return nPosition-1;
	}
}

float CBurstHandling::MDT(int nIndex0, int nIndex1, int nEventIndex, int nLevel)
{
	//This function return the average open time of the n-the dwelling time (at level=nLevel)
	//of each burst, where n=nEventIndex.
	//It count from the end of each burst when nEventIndex is negative.
	//abs(nEventIndex) should be >=1
	//nLevel should be 0 or 1
	//The average is for the burst nodes between nIndex0 and nIndex1, inclusively.
	int i,nNum=0,nNumOpening=0,nPosition=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNumOpening=m_structvBurstNodes[i].nNumOpenings;
//		if((nNumOpening+nLevel)/2>=abs(nEventIndex))
//		if((nNumOpening+nLevel)/2>=abs(nEventIndex)&&nNumOpening>=m_structBurstingTimeCourseOptionNode.nMinOpening&&nNumOpening<=m_structBurstingTimeCourseOptionNode.nMaxOpening&&FirstBurst(i))// A temporal modification
		if((nNumOpening+nLevel)/2>=abs(nEventIndex)&&nNumOpening>=m_structBurstingTimeCourseOptionNode.nMinOpening&&nNumOpening<=m_structBurstingTimeCourseOptionNode.nMaxOpening)// A temporal modification
		{
			nNum++;
			if(nEventIndex>0)
			{
				nPosition=NextEvent(m_structvBurstNodes[i].nFirstOpening,nLevel,nEventIndex-1);
				fTime+=m_pnLevelLength[nPosition]*m_fTimePerPoint/1000.;
			}
			else if(nEventIndex<0)
			{
				nPosition=NextEvent(m_structvBurstNodes[i].nFirstOpening,nLevel,nNumOpening+nEventIndex-1+nLevel);
				fTime+=m_pnLevelLength[nPosition]*m_fTimePerPoint/1000.;
			}
		}
	}
	float fMDT=0.f; //Mean dwelling time
	if(nNum>0)
	{
		fMDT=fTime/double(nNum);
	}
	else
	{
		fMDT=-1.f;
	}
	return fMDT;
}
float CBurstHandling::MeanAmp(int nIndex0, int nIndex1, int nEventIndex, int nLevel)
{

	//This function return the average amplitude of the n-the dwelling time (at level=nLevel)
	//of each burst, where n=nEventIndex.
	//It count from the end of each burst when nEventIndex is negative.
	//abs(nEventIndex) should be >=1
	//nLevel should be 0 or 1
	//The average is for the nodes between nIndex0 and nIndex1
	int i,nNum=0,nNumOpening=0,nPosition=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(m_structvBurstNodes[i].bValid) continue;//09929
		nNumOpening=m_structvBurstNodes[i].nNumOpenings;
//		if((nNumOpening+nLevel)/2>=abs(nEventIndex))
//		if((nNumOpening+nLevel)/2>=abs(nEventIndex)&&nNumOpening>=m_structBurstingTimeCourseOptionNode.nMinOpening&&nNumOpening<=m_structBurstingTimeCourseOptionNode.nMaxOpening&&FirstBurst(i))// A temporal modification
		if((nNumOpening+nLevel)/2>=abs(nEventIndex)&&nNumOpening>=m_structBurstingTimeCourseOptionNode.nMinOpening&&nNumOpening<=m_structBurstingTimeCourseOptionNode.nMaxOpening)// A temporal modification
		{
			nNum++;
			if(nEventIndex>0)
			{
				nPosition=NextEvent(m_structvBurstNodes[i].nFirstOpening,nLevel,nEventIndex-1);
				fTime+=m_pnLevelLength[nPosition]*m_fTimePerPoint/1000.;
			}
			else if(nEventIndex<0)
			{
				nPosition=NextEvent(m_structvBurstNodes[i].nFirstOpening,nLevel,nNumOpening+nEventIndex-1+nLevel);
				fTime+=m_pnLevelLength[nPosition]*m_fTimePerPoint/1000.;
			}
		}
	}
	float fMDT=0.f; //Mean dwelling time
	if(nNum>0)
	{
		fMDT=fTime/double(nNum);
	}
	else
	{
		fMDT=-1.f;
	}
	return fMDT;
}

float CBurstHandling::MDT_Cluster(int nIndex0, int nIndex1, int nLevel)
{
	//nIndex0 is the index for the cluster node, and nIndex1 is the index for the burst in the cluster node.
	//This function returns the mean dwelling time (at level=nLevel)
	//of the n-th burst of the cluster nodes nIndex0, where n=nIndex1.
	//It counts from the end of the cluster when nIndex1 is negative.
	//abs(nEventIndex) should be >=1
	//nLevel should be 0 or 1
	int i,nIndex;
	float fTime=-1.f;

	int nSize=m_structvClusterNodes[nIndex0].svBurstNodes.size();
	if(abs(nIndex1)>nSize)
	{
		return -1.f;
	}

	if(nIndex1>0)
	{
		nIndex=nIndex1-1;
	}
	else if(nIndex1<0)
	{
		nIndex=nSize+nIndex1;
	}

	int nNumOpenings=m_structvClusterNodes[nIndex0].svBurstNodes[nIndex].nNumOpenings;
	float fTotalOpenTime=m_structvClusterNodes[nIndex0].svBurstNodes[nIndex].fTotalOpenTime;
	float fLength=m_structvClusterNodes[nIndex0].svBurstNodes[nIndex].fLength;

	if(nLevel==0)
	{
		fTime=fLength-fTotalOpenTime;
		if(nNumOpenings>1)
		{
			fTime/=double(nNumOpenings-1);
		}
		else
		{
			fTime=-1.f;
		}
	}
	else if(nLevel==1)
	{
		fTime=fTotalOpenTime/double(nNumOpenings);
	}

	return fTime;
}

int CBurstHandling::NextEvent(int nPosition, int nLevel0, int nShift0, int delta)
{
	//Including nPosition, the function returns the nPosition of the nShift0-the event whose level 
	//is nLevel0. e. g., it return the first it find when nShift==1, and the second when nShift==2, etc.
	//it search backward if delta <0.
	int i=nPosition;
	int nLevel=m_pstEventCurrentLevel[i];
	int nShift=0;
	if(nLevel==nLevel0) nShift+=delta;
	while(abs(nShift)<abs(nShift0))
	{
		i+=delta;
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==nLevel0)
		{
			nShift++;
			nPosition=i;
		}
		if(i<0||i>=m_nNumEvents) return -1;
	}
	return nPosition;
}

int CBurstHandling::NextEvent(int nPosition, int nLevel0, int nShift0)
{
	//Including nPosition, the function returns the nPosition of the nShift0-the event whose level 
	//is nLevel0. e. g., it return the first it find when nShift==1, and the second when nShift==2, etc.
	//it search backward if nShift <0
	int i=nPosition;
	int nLevel=m_pstEventCurrentLevel[i];
	int nShift=0;
	int delta=1;
	if(nShift0<0) delta=-1;
	if(nLevel==nLevel0) nShift++;
	while(nShift<=nShift0&&i<m_nNumEvents)
	{
		i+=delta;
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==nLevel0)
		{
			nShift++;
			nPosition=i;
		}
	}
	return nPosition;
}

void CBurstHandling::buildLevelAmpHist()
{
	int i,j,nLevel;
	int nCutoff=m_structBurstingTimeCourseOptionNode.nLatancy;
	int nNOP=0;
	int length;
	int nSize=m_structvBurstNodes.size();
	int nWs=1;

	int histIndex;
	bool bDoAPH=true;
	if(m_structAbfNode.nSize==0) bDoAPH=false;

	double dmin,dmax,dDelta1,dDelta2;
	int nDim1,nDim2;

	dDelta1=0.1;
	dDelta2=0.2;
	if(bDoAPH)
	{
		dmin=m_structAbfNode.fMinAmp;
		dmax=m_structAbfNode.fMaxAmp;
	}else{
		evlAmpRange(dmin,dmax);
	}
	dmin=-10.;
	dmax=5.;

	if(bDoAPH)
	{
		m_cAPH0.update(dmin,dmax,dDelta1);
		m_cAPH1.update(dmin,dmax,dDelta1);
		m_cAPH2.update(dmin,dmax,dDelta1);
		m_cAPH1r.update(dmin,dmax,dDelta1);
		m_cAPH2r.update(dmin,dmax,dDelta1);
	}

	m_cLevelAmpHist01.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist02.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist01r.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist02r.update(dmin,dmax,dDelta2);

	m_cLevelAmpHist11.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist12.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist11r.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist12r.update(dmin,dmax,dDelta2);

	nDim1=m_cAPH1.getSize();
	nDim2=m_cLevelAmpHist01.getSize();
	
	float *pfData=m_structAbfNode.pfData;


	float fTemp;
	int minLength=5;
	int maxLength=10000;
	int nBaselineLength=500, nFirtOpening;
	int index0,index1,index0r,index1r,firstIndex,num;
	int nRising=int(330/m_fTimePerPoint)+1;
	int nFirstOpening,nIBC;
	nRising=0;//Take this into account later.
	for(i=0;i<nSize;i++)
	{
		structBurstNode aNode=m_structvBurstNodes[i];
		if(m_structvBurstNodes[i].bValid) continue;//09929


		nFirstOpening=aNode.nFirstOpening;
		nFirstOpening=m_pnLevelStart[nFirstOpening];
		nIBC=aNode.nPrecedingIBC;
		nIBC=m_pnLevelLength[nIBC];

		if(nIBC>nBaselineLength) nIBC=nBaselineLength;
		index0=nFirstOpening-nIBC;
		m_cAPH0.addData(pfData,index0,1,nIBC);

		nLevel=1;
		nNOP=aNode.nNumOpenings;
		if(nNOP<m_structBurstingTimeCourseOptionNode.nMinOpening||nNOP>m_structBurstingTimeCourseOptionNode.nMaxOpening) nNOP=-1;
		index1=aNode.nPrecedingIBC;
		index0=aNode.nFirstOpening;
		index0r=aNode.nLastOpening;
		index1r=aNode.nLastOpening;
		int maxL,maxLr;


		for(j=0;j<=nNOP/2;j++)
		{
			fTemp=Amp(i,j+1);
			length=LevelLength(i,j+1);
			index1=NextEvent(index1+1,1,1,1);
			index0=NextEvent(index1,0,1,1);
			index0r=NextEvent(index0r-1,0,1,-1);
			index1r=NextEvent(index0r,1,1,1);
			maxL=m_pnLevelLength[index1]+20;
			maxLr=m_pnLevelLength[index1r]+20;

			if(nNOP==1&&length>minLength){
				j=j;
			}

			if(j<=nCutoff){
				m_cLevelAmpHist11.addData(fTemp,length);
				m_cLevelAmpHist01.addData(m_pfAmplitude[index0],m_pnLevelLength[index0]);
				if(bDoAPH&&length>minLength&&length<=maxLength)
				{
					firstIndex=m_pnLevelStart[index1]+nRising;
					num=m_pnLevelStart[index0]+m_pnLevelLength[index0]-firstIndex-nRising;
					if(num>maxL) num=maxL;
					m_cAPH1.addData(pfData,firstIndex,1,num);
				}
			}else
			{
				m_cLevelAmpHist12.addData(fTemp,length);
				m_cLevelAmpHist02.addData(m_pfAmplitude[index0],m_pnLevelLength[index0]);
				if(bDoAPH&&length>minLength&&length<=maxLength)
				{
					firstIndex=m_pnLevelStart[index1]+nRising;
					num=m_pnLevelStart[index0]+m_pnLevelLength[index0]-firstIndex-nRising;
					if(num>maxL) num=maxL;
					m_cAPH2.addData(pfData,firstIndex,1,num);
				}
			}

			fTemp=Amp(i,-(j+1));
			length=LevelLength(i,j+1);

			if(j<=nCutoff){
				m_cLevelAmpHist11r.addData(fTemp,length);
				m_cLevelAmpHist01r.addData(m_pfAmplitude[index0r],m_pnLevelLength[index0r]);
				if(bDoAPH&&length>minLength)
				{
					firstIndex=m_pnLevelStart[index0r]+nRising;
					num=m_pnLevelStart[index1r]+m_pnLevelLength[index1r]-firstIndex-nRising;
					if(num>maxLr) num=maxLr;
					m_cAPH1r.addData(pfData,firstIndex,1,num);
				}
			}else
			{
				m_cLevelAmpHist12r.addData(fTemp,length);
				m_cLevelAmpHist02r.addData(m_pfAmplitude[index0r],m_pnLevelLength[index0r]);
				if(bDoAPH&&length>minLength)
				{
					firstIndex=m_pnLevelStart[index0r]+nRising;
					num=m_pnLevelStart[index1r]+m_pnLevelLength[index1r]-firstIndex-nRising;
					if(num>maxLr) num=maxLr;
					m_cAPH2r.addData(pfData,firstIndex,1,num);
				}
			}
		}
	}
}

void CBurstHandling::evlAmpRange(double &dmin, double &dmax){
	dmin=999999999.;
	dmax=-dmin;
	double amp;
	for(int i=0;i<m_nNumEvents;i++)
	{
		amp=m_pfAmplitude[i];
		if(amp<dmin) dmin=amp;
		if(amp>dmax) dmax=amp;
	}
}
void CBurstHandling::CalAmp_EventIndex()
{
	int i,j,nLevel;
	float fTemp;
	delete m_pfAmp_Ave0;
	delete m_pfAmp_Num0;
	delete m_pfAmp_Ave1;
	delete m_pfAmp_Num1;


	m_pfAmp_Ave0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Ave1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Num1=new float[(m_nMaxOpeningNum+1)/2];
	float fTime=0.f;

	for(i=0;i<(m_nMaxOpeningNum+1)/2;i++)
	{
		m_pfAmp_Ave0[i]=0.f;
		m_pfAmp_Num0[i]=0.f;
		m_pfAmp_Ave1[i]=0.f;
		m_pfAmp_Num1[i]=0.f;
	}

	int nNOP=0;
	int length;
	int nSize=m_structvBurstNodes.size();
	int histIndex;
	structBurstNode aNode;
	for(i=0;i<nSize;i++)
	{
		aNode=m_structvBurstNodes[i];
		if(!aNode.bValid) continue;

		nLevel=1;
		nNOP=aNode.nNumOpenings;
		fTime=m_pnLevelStart[m_structvBurstNodes[i].nFirstOpening]*m_fTimePerPoint/1000.;
		if(nNOP<m_structBurstingTimeCourseOptionNode.nMinOpening||nNOP>m_structBurstingTimeCourseOptionNode.nMaxOpening) nNOP=-1;
		for(j=0;j<nNOP/2;j++)
		{
			fTemp=Amp(i,j+1);
			length=LevelLength(i,j+1);
//			m_pfAmp_Ave0[j]+=fTemp;
//			m_pfAmp_Num0[j]+=1.;
			m_pfAmp_Ave0[j]+=fTemp*length;
			m_pfAmp_Num0[j]+=length;

			fTemp=Amp(i,-(j+1));
			length=LevelLength(i,j+1);
//			m_pfAmp_Ave1[j]+=fTemp;
//			m_pfAmp_Num1[j]+=1.;
			m_pfAmp_Ave1[j]+=fTemp*length;
			m_pfAmp_Num1[j]+=length;
		}
	}

	for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
	{
		m_pfAmp_Ave0[j]/=m_pfAmp_Num0[j];
		m_pfAmp_Ave1[j]/=m_pfAmp_Num1[j];
	}
	m_nEvlID=-1;
}

void CBurstHandling::CalMDT_EventIndex()
{
	int i,j,nLevel;
	float fTemp;
	delete m_pfTc_Time0;
	delete m_pfTo_Time0;
	delete m_pfTc_Num0;
	delete m_pfTo_Num0;
	delete m_pfTc_Time1;
	delete m_pfTo_Time1;
	delete m_pfTc_Num1;
	delete m_pfTo_Num1;

	m_pfTc_Time0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Time0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Time1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Time1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Num1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Num1=new float[(m_nMaxOpeningNum+1)/2];

	for(i=0;i<(m_nMaxOpeningNum+1)/2;i++)
	{
		m_pfTc_Time0[i]=0.f;
		m_pfTo_Time0[i]=0.f;
		m_pfTc_Num0[i]=0.f;
		m_pfTo_Num0[i]=0.f;
		m_pfTc_Time1[i]=0.f;
		m_pfTo_Time1[i]=0.f;
		m_pfTc_Num1[i]=0.f;
		m_pfTo_Num1[i]=0.f;
	}

	int nNOP=0;

	int nSize=m_structvBurstNodes.size();
	for(i=0;i<nSize;i++)
	{
			nLevel=0;
			if(!m_structvBurstNodes[i].bValid) continue;
			for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
			{
				fTemp=MDT(i,i,j+1,nLevel);
				if(fTemp>0)
				{
					m_pfTc_Time0[j]+=fTemp;
					m_pfTc_Num0[j]+=1.;
				}
				fTemp=MDT(i,i,-(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfTc_Time1[j]+=fTemp;
					m_pfTc_Num1[j]+=1.;
				}
			}

			nLevel=1;
			for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
			{
				fTemp=MDT(i,i,j+1,nLevel);
				if(fTemp>0)
				{
					m_pfTo_Time0[j]+=fTemp;
					m_pfTo_Num0[j]+=1.;
				}
				fTemp=MDT(i,i,-(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfTo_Time1[j]+=fTemp;
					m_pfTo_Num1[j]+=1.;
				}
			}
	}

	for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
	{
		m_pfTc_Time0[j]/=m_pfTc_Num0[j];
		m_pfTc_Time1[j]/=m_pfTc_Num1[j];
		m_pfTo_Time0[j]/=m_pfTo_Num0[j];
		m_pfTo_Time1[j]/=m_pfTo_Num1[j];
	}
}


void CBurstHandling::CalMDT_EventIndex_Cluster()
{
	int i,j,nLevel;
	float fTemp;
	delete m_pfBMDTc_Time0;
	delete m_pfBMDTo_Time0;
	delete m_pfBMDTc_Num0;
	delete m_pfBMDTo_Num0;
	delete m_pfBMDTc_Time1;
	delete m_pfBMDTo_Time1;
	delete m_pfBMDTc_Num1;
	delete m_pfBMDTo_Num1;

	m_pfBMDTc_Time0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Time0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTc_Num0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Num0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTc_Time1=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Time1=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTc_Num1=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Num1=new float[(m_nMaxBurstingNum+1)/2];

	for(i=0;i<(m_nMaxBurstingNum+1)/2;i++)
	{
		m_pfBMDTc_Time0[i]=0.f;
		m_pfBMDTo_Time0[i]=0.f;
		m_pfBMDTc_Num0[i]=0.f;
		m_pfBMDTo_Num0[i]=0.f;
		m_pfBMDTc_Time1[i]=0.f;
		m_pfBMDTo_Time1[i]=0.f;
		m_pfBMDTc_Num1[i]=0.f;
		m_pfBMDTo_Num1[i]=0.f;
	}

	int nSize0=m_structvClusterNodes.size();
	int nSize1,nDepth;

	for(i=0;i<nSize0;i++)
	{
		nLevel=0;
		if(!m_structvBurstNodes[i].bValid) continue;
		nSize1=m_structvClusterNodes[i].svBurstNodes.size();
		nDepth=(nSize1+1)/2;
		if(m_structvClusterNodes[i].nMaxNOPB<20)
			nDepth=-1;


		nLevel=0;

		if(nDepth>=2)
		{

			for(j=0;j<nDepth;j++)
			{
				fTemp=MDT_Cluster(i,(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfBMDTc_Time0[j]+=fTemp;
					m_pfBMDTc_Num0[j]+=1.;
				}

				fTemp=MDT_Cluster(i,-(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfBMDTc_Time1[j]+=fTemp;
					m_pfBMDTc_Num1[j]+=1.;
				}
			}

			nLevel=1;
			for(j=0;j<nDepth;j++)
			{
				fTemp=MDT_Cluster(i,(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfBMDTo_Time0[j]+=fTemp;
					m_pfBMDTo_Num0[j]+=1.;
				}
				fTemp=MDT_Cluster(i,-(j+1),nLevel);
				if(fTemp>0)
				{
					m_pfBMDTo_Time1[j]+=fTemp;
					m_pfBMDTo_Num1[j]+=1.;
				}
			}
		}
	}

	for(j=0;j<(m_nMaxBurstingNum+1)/2;j++)
	{
		m_pfBMDTc_Time0[j]/=m_pfBMDTc_Num0[j];
		m_pfBMDTc_Time1[j]/=m_pfBMDTc_Num1[j];
		m_pfBMDTo_Time0[j]/=m_pfBMDTo_Num0[j];
		m_pfBMDTo_Time1[j]/=m_pfBMDTo_Num1[j];
	}
}

void CBurstHandling::WriteBMDT(FILE *fpOutput)
{
	int i;
	fprintf(fpOutput,"       i     MTo(i-th)         Sem         N    MTo(-i-th)         Sem         N     MTc(i-th)         Sem         N    MTc(-i-th)         Sem         N     Amp(i-th)         Sem         N    Amp(-i-th)         Sem         N\n");
	vector <float> fvTemp;	
	for(i=0;i<(m_nMaxOpeningNum+1)/2;i++)
	{
		fprintf(fpOutput,"%8d  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f\n",
			i,m_pfTo_Time0[i],m_pfTo_Sem0[i],m_pfTo_Num0[i],m_pfTo_Time1[i],m_pfTo_Sem1[i],m_pfTo_Num1[i]
			 ,m_pfTc_Time0[i],m_pfTc_Sem0[i],m_pfTc_Num0[i],m_pfTc_Time1[i],m_pfTc_Sem1[i],m_pfTc_Num1[i]
			 ,m_pfAmp_Ave0[i],m_pfAmp_Sem0[i],m_pfAmp_Num0[i],m_pfAmp_Ave1[i],m_pfAmp_Sem1[i],m_pfAmp_Num1[i]);
	}
}

void CBurstHandling::WriteBMDT_Cluster(FILE *fpOutput)
{
	int i;
	fprintf(fpOutput,"       i     MTo(i-th)         Sem         N    MTo(-i-th)         Sem         N     MTc(i-th)         Sem         N    MTc(-i-th)         Sem         N\n");
	vector <float> fvTemp;	
	for(i=0;i<(m_nMaxBurstingNum+1)/2;i++)
	{
		fprintf(fpOutput,"%8d  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f  %12.2f  %10.2f  %8.0f\n",
			i,m_pfBMDTo_Time0[i],m_pfBMDTo_Sem0[i],m_pfBMDTo_Num0[i],m_pfBMDTo_Time1[i],m_pfBMDTo_Sem1[i],m_pfBMDTo_Num1[i]
			 ,m_pfBMDTc_Time0[i],m_pfBMDTc_Sem0[i],m_pfBMDTc_Num0[i],m_pfBMDTc_Time1[i],m_pfBMDTc_Sem1[i],m_pfBMDTc_Num1[i]);
	}
}

void CBurstHandling::CalMDTSem_EventIndex()
{
	int i,j,nLevel;
	float fTemp,fTemp1;
	delete m_pfTc_Sem0;
	delete m_pfTo_Sem0;
	delete m_pfTc_Num0;
	delete m_pfTo_Num0;
	delete m_pfTc_Sem1;
	delete m_pfTo_Sem1;
	delete m_pfTc_Num1;
	delete m_pfTo_Num1;

	m_pfTc_Sem0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Sem0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Sem1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Sem1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTc_Num1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfTo_Num1=new float[(m_nMaxOpeningNum+1)/2];

	for(i=0;i<(m_nMaxOpeningNum+1)/2;i++)
	{
		m_pfTc_Sem0[i]=0.f;
		m_pfTo_Sem0[i]=0.f;
		m_pfTc_Num0[i]=0.f;
		m_pfTo_Num0[i]=0.f;
		m_pfTc_Sem1[i]=0.f;
		m_pfTo_Sem1[i]=0.f;
		m_pfTc_Num1[i]=0.f;
		m_pfTo_Num1[i]=0.f;
	}

	int nSize=m_structvBurstNodes.size();
	for(i=0;i<nSize;i++)
	{
		nLevel=0;
		if(!m_structvBurstNodes[i].bValid) continue;
		for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
		{
			fTemp=MDT(i,i,j+1,nLevel);
			fTemp1=m_pfTc_Time0[j];
			if(fTemp>0)
			{
				m_pfTc_Sem0[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
				m_pfTc_Num0[j]+=1.;
			}
			fTemp=MDT(i,i,-(j+1),nLevel);
			fTemp1=m_pfTc_Time1[j];
			if(fTemp>0)
			{
				m_pfTc_Sem1[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
				m_pfTc_Num1[j]+=1.;
			}
		}

		nLevel=1;
		for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
		{
			fTemp=MDT(i,i,j+1,nLevel);
			fTemp1=m_pfTo_Time0[j];
			if(fTemp>0)
			{
				m_pfTo_Sem0[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
				m_pfTo_Num0[j]+=1.;
			}
			fTemp=MDT(i,i,-(j+1),nLevel);
			fTemp1=m_pfTo_Time1[j];
			if(fTemp>0)
			{
				m_pfTo_Sem1[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
				m_pfTo_Num1[j]+=1.;
			}
		}
	}
	
	for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
	{
		m_pfTc_Sem0[j]=sqrt(m_pfTc_Sem0[j]/(m_pfTc_Num0[j]*(m_pfTc_Num0[j]-1.)));
		m_pfTc_Sem1[j]=sqrt(m_pfTc_Sem1[j]/(m_pfTc_Num1[j]*(m_pfTc_Num1[j]-1.)));
		m_pfTo_Sem0[j]=sqrt(m_pfTo_Sem0[j]/(m_pfTo_Num0[j]*(m_pfTo_Num0[j]-1.)));
		m_pfTo_Sem1[j]=sqrt(m_pfTo_Sem1[j]/(m_pfTo_Num1[j]*(m_pfTo_Num1[j]-1.)));
	}
}

void CBurstHandling::CalAmpSem_EventIndex()
{
	int i,j,nLevel;
	float fTemp,fTemp1;
	delete m_pfAmp_Sem0;
	delete m_pfAmp_Num0;
	delete m_pfAmp_Sem1;
	delete m_pfAmp_Num1;

	m_pfAmp_Sem0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Num0=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Sem1=new float[(m_nMaxOpeningNum+1)/2];
	m_pfAmp_Num1=new float[(m_nMaxOpeningNum+1)/2];

	for(i=0;i<(m_nMaxOpeningNum+1)/2;i++)
	{
		m_pfAmp_Sem0[i]=0.f;
		m_pfAmp_Num0[i]=0.f;
		m_pfAmp_Sem1[i]=0.f;
		m_pfAmp_Num1[i]=0.f;
	}

	int nSize=m_structvBurstNodes.size();
	int nNOP;
	for(i=0;i<nSize;i++)
	{
		nLevel=1;
		if(!m_structvBurstNodes[i].bValid) continue;
		nNOP=m_structvBurstNodes[i].nNumOpenings;
		if(nNOP<m_structBurstingTimeCourseOptionNode.nMinOpening||nNOP>m_structBurstingTimeCourseOptionNode.nMaxOpening) nNOP=-1;
		for(j=0;j<nNOP/2;j++)
		{
			fTemp=Amp(i,j+1);
			fTemp1=m_pfAmp_Ave0[j];
			m_pfAmp_Sem0[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
			m_pfAmp_Num0[j]+=1.;

			fTemp=Amp(i,-(j+1));
			fTemp1=m_pfAmp_Ave1[j];
			m_pfAmp_Sem1[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
			m_pfAmp_Num1[j]+=1.;
		}
	}
	
	for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
	{
		m_pfAmp_Sem0[j]=sqrt(m_pfAmp_Sem0[j]/(m_pfAmp_Num0[j]*(m_pfAmp_Num0[j]-1.)));
		m_pfAmp_Sem1[j]=sqrt(m_pfAmp_Sem1[j]/(m_pfAmp_Num1[j]*(m_pfAmp_Num1[j]-1.)));
	}
}
void CBurstHandling::CalMDTSem_EventIndex_Cluster()
{
	int i,j,nLevel;
	float fTemp,fTemp1;
	delete m_pfBMDTc_Sem0;
	delete m_pfBMDTo_Sem0;
//	delete m_pfBMDTc_Num0;
//	delete m_pfBMDTo_Num0;
	delete m_pfBMDTc_Sem1;
	delete m_pfBMDTo_Sem1;
//	delete m_pfBMDTc_Num1;
//	delete m_pfBMDTo_Num1;

	m_pfBMDTc_Sem0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Sem0=new float[(m_nMaxBurstingNum+1)/2];
//	m_pfBMDTc_Num0=new float[(m_nMaxBurstingNum+1)/2];
//	m_pfBMDTo_Num0=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTc_Sem1=new float[(m_nMaxBurstingNum+1)/2];
	m_pfBMDTo_Sem1=new float[(m_nMaxBurstingNum+1)/2];
//	m_pfBMDTc_Num1=new float[(m_nMaxBurstingNum+1)/2];
//	m_pfBMDTo_Num1=new float[(m_nMaxBurstingNum+1)/2];

	for(i=0;i<(m_nMaxBurstingNum+1)/2;i++)
	{
		m_pfBMDTc_Sem0[i]=0.f;
		m_pfBMDTo_Sem0[i]=0.f;
//		m_pfBMDTc_Num0[i]=0.f;
//		m_pfBMDTo_Num0[i]=0.f;
		m_pfBMDTc_Sem1[i]=0.f;
		m_pfBMDTo_Sem1[i]=0.f;
//		m_pfBMDTc_Num1[i]=0.f;
//		m_pfBMDTo_Num1[i]=0.f;
	}

	int nSize0=m_structvClusterNodes.size();
	int nSize1,nDepth;
	for(i=0;i<nSize0;i++)
	{
		nLevel=0;
		if(!m_structvClusterNodes[i].bValid) continue;
		nSize1=m_structvClusterNodes[i].svBurstNodes.size();
		nDepth=(nSize1+1)/2;

		if(nDepth>=1)
		{

			for(j=0;j<nDepth;j++)
			{
				fTemp=MDT_Cluster(i,j+1,nLevel);
				fTemp1=m_pfBMDTc_Time0[j];
				if(fTemp>0)
				{
					m_pfBMDTc_Sem0[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
	//				m_pfBMDTc_Num0[j]+=1.;
				}
				fTemp=MDT_Cluster(i,-(j+1),nLevel);
				fTemp1=m_pfBMDTc_Time1[j];
				if(fTemp>0)
				{
					m_pfBMDTc_Sem1[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
	//				m_pfBMDTc_Num1[j]+=1.;
				}
			}

			nLevel=1;
			for(j=0;j<(m_nMaxOpeningNum+1)/2;j++)
			{
				fTemp=MDT_Cluster(i,j+1,nLevel);
				fTemp1=m_pfBMDTo_Time0[j];
				if(fTemp>0)
				{
					m_pfBMDTo_Sem0[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
	//				m_pfBMDTo_Num0[j]+=1.;
				}

				fTemp=MDT_Cluster(i,-(j+1),nLevel);
				fTemp1=m_pfBMDTo_Time1[j];
				if(fTemp>0)
				{
					m_pfBMDTo_Sem1[j]+=(fTemp-fTemp1)*(fTemp-fTemp1);
	//				m_pfBMDTo_Num1[j]+=1.;
				}
			}
		}
	}
	
	for(j=0;j<(m_nMaxBurstingNum+1)/2;j++)
	{
		m_pfBMDTc_Sem0[j]=sqrt(m_pfBMDTc_Sem0[j]/(m_pfBMDTc_Num0[j]*(m_pfBMDTc_Num0[j]-1.)));
		m_pfBMDTc_Sem1[j]=sqrt(m_pfBMDTc_Sem1[j]/(m_pfBMDTc_Num1[j]*(m_pfBMDTc_Num1[j]-1.)));
		m_pfBMDTo_Sem0[j]=sqrt(m_pfBMDTo_Sem0[j]/(m_pfBMDTo_Num0[j]*(m_pfBMDTo_Num0[j]-1.)));
		m_pfBMDTo_Sem1[j]=sqrt(m_pfBMDTo_Sem1[j]/(m_pfBMDTo_Num1[j]*(m_pfBMDTo_Num1[j]-1.)));
	}
}

float CBurstHandling::MAmp(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fAmp=0.f;
	int j, nTemp0;
	float fBaseline;
	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nTemp0=m_structvBurstNodes[i].nPrecedingIBC;
		fBaseline=m_pfAmplitude[nTemp0];
		for(j=m_structvBurstNodes[i].nFirstOpening;j<=m_structvBurstNodes[i].nLastOpening;j++)
		{
			if(m_pstEventCurrentLevel[j]==1)
			{
				fAmp+=m_pfAmplitude[j]-fBaseline;
				nNum++;
			}
		}
	}
	return fAmp/double(nNum);
}

float CBurstHandling::Amp(int nBurstIndex, int nEventIndex)
{
	float fAmp=0.f;
	structBurstNode aNode=m_structvBurstNodes[nBurstIndex];
	int nOpenings=aNode.nNumOpenings;
	if(nEventIndex>nOpenings||-nEventIndex>nOpenings) return fAmp;
	int nShift=nEventIndex-1;
	if(nEventIndex<0) nShift=nOpenings+nEventIndex;
	int i=NextEvent(aNode.nFirstOpening,1,nShift);
	float base=m_pfAmplitude[aNode.nPrecedingIBC];
	if(nBurstIndex+1<m_structvBurstNodes.size()){
		base+=m_pfAmplitude[m_structvBurstNodes[nBurstIndex+1].nPrecedingIBC];
		base/=2.;
	}
	fAmp=m_pfAmplitude[i]-base;
	return fAmp;
}

int CBurstHandling::LevelLength(int nBurstIndex, int nEventIndex)
{
	int nOpenings=m_structvBurstNodes[nBurstIndex].nNumOpenings;
	if(nEventIndex>nOpenings||-nEventIndex>nOpenings) return -1;
	int nShift=nEventIndex-1;
	if(nEventIndex<0) nShift=nOpenings+nEventIndex;
	int i=NextEvent(m_structvBurstNodes[nBurstIndex].nFirstOpening,1,nShift);
	return m_pnLevelLength[i];
}

void CBurstHandling::PickInterBurstIntervals(bool *pbExcluded)
{
	int nLevel=0,i;
	ExcludeAllEvents(nLevel);
	CheckBurst();
	int nSize=m_structvBurstNodes.size();
	float fTime;
	for(i=1;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		if(m_structvBurstNodes[i-1].bValid&&m_structvBurstNodes[i].bValid)
		{
			m_pbExcluded[m_structvBurstNodes[i].nPrecedingIBC]=false;
			fTime=m_pnLevelStart[m_structvBurstNodes[i].nPrecedingIBC]*m_fTimePerPoint/1000.;
		}
	}
}

void CBurstHandling::CheckBurst()//practically obsolete
{
	int nSize=m_structvBurstNodes.size();
	int nNum;
	for(int i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nNum=m_structvBurstNodes[i].nNumOpenings;
		if(nNum<m_structBurstingTimeCourseOptionNode.nMinOpening||nNum>m_structBurstingTimeCourseOptionNode.nMaxOpening)
		{
			m_structvBurstNodes[i].bValid=false;
		}
	}
}

void CBurstHandling::ExcludeAllEvents(int nLevel)
{
	int nTemp;
	for(int i=0;i<m_nNumEvents;i++)
	{
		nTemp=m_pstEventCurrentLevel[i];
		if(nTemp==nLevel||nLevel<0)
		{
			m_pbExcluded[i]=true;
		}
	}
}

void CBurstHandling::MakeNOPBHist()
{
	//This function makes a histgram of number of opening per burst.
	if(!m_bBurstNode) MakeBurstNodes();
//	const double dLogE=0.43429448190325182765112891891661;
	int *pnNOPB=new int[m_nMaxOpeningNum+1];
	int i,nTemp;
	for(i=0;i<m_nMaxOpeningNum;i++)
	{
		pnNOPB[i]=0;
	}
	int nSize=m_structvBurstNodes.size();
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nTemp=m_structvBurstNodes[i].nNumOpenings;
		pnNOPB[nTemp]++;
	}
	m_vNumOpenPerBurstHist.clear();
	for(i=1;i<=m_nMaxOpeningNum;i++)
	{
		nTemp=pnNOPB[i];
		if(nTemp>0)
		{
			structHistNode aNode;
			aNode.dData=nTemp;
			aNode.dLeft=i;
			aNode.dRight=i;
			m_vNumOpenPerBurstHist.push_back(aNode);
		}
	}
	m_bNOPBHist=true;
}

void CBurstHandling::MakeTOTHist_HB()
{
	//This function makes a histgram of Total open time of bursts.
	if(!m_bBurstNode) MakeBurstNodes();
	structHBHistParNode aNode;
	aNode.dLinMin=0.79;
	aNode.dLinDelta=0.2;
	aNode.dLogMin=1.3;
	aNode.dLogMax=6.;
	aNode.dLogDelta=0.05;

	int nNumLinBins=0;
	double dLinLength=CUF::Exp(10.,aNode.dLogMin)-aNode.dLinMin;

	if(dLinLength>0.)
	{
		nNumLinBins=int(dLinLength/aNode.dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}

	int nNumLogBins=int((aNode.dLogMax-aNode.dLogMin)/aNode.dLogDelta)+1;

	int nNumBins=nNumLinBins+nNumLogBins;

	int i;
	m_vTotalOpenTimeHist_HB.clear();
	for(i=0;i<nNumBins;i++)
	{
		structHistNode aHistNode;
		aHistNode.dData=0;
		aHistNode.dLeft=CalBinPosition(aNode,i);
		m_vTotalOpenTimeHist_HB.push_back(aHistNode);
	}

	int nSize=m_structvBurstNodes.size();
	double dTemp,nIndex;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dTemp=m_structvBurstNodes[i].fTotalOpenTime;
		nIndex=CalIndex_HB(aNode,dTemp);
		m_vTotalOpenTimeHist_HB[nIndex].dData+=1.;
	}
	m_vTotalOpenTimeHist_HB=RemoveEmptyBins(m_vTotalOpenTimeHist_HB);
	m_bTOTPBHist_HB=true;
}

void CBurstHandling::MakeTOTHist_Log()
{
	//This function makes a histgram of Total open time of bursts.
	if(!m_bBurstNode) MakeBurstNodes();
	structHBHistParNode aNode;
	aNode.dLinMin=0.89; //To make nNumLinBins=0
	aNode.dLinDelta=0.2;
	aNode.dLogMin=-0.1;
	aNode.dLogMax=6.;
	aNode.dLogDelta=0.05;

	int nNumLinBins=0;
	double dLinLength=CUF::Exp(10.,aNode.dLogMin)-aNode.dLinMin;

	if(dLinLength>0.)
	{
		nNumLinBins=int((dLinLength-aNode.dLinMin)/aNode.dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}

	int nNumLogBins=int((aNode.dLogMax-aNode.dLogMin)/aNode.dLogDelta)+1;

	int nNumBins=nNumLinBins+nNumLogBins;

	int i;
	m_vTotalOpenTimeHist_Log.clear();
	for(i=0;i<nNumBins;i++)
	{
		structHistNode aHistNode;
		aHistNode.dData=0;
		aHistNode.dLeft=CalBinPosition(aNode,i);
		m_vTotalOpenTimeHist_Log.push_back(aHistNode);
	}

	int nSize=m_structvBurstNodes.size();
	double dTemp,nIndex;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dTemp=m_structvBurstNodes[i].fTotalOpenTime;
		nIndex=CalIndex_HB(aNode,dTemp);
		m_vTotalOpenTimeHist_Log[nIndex].dData+=1.;
	}
	m_vTotalOpenTimeHist_Log=RemoveEmptyBins(m_vTotalOpenTimeHist_Log);
	m_bTOTPBHist_Log=true;
}

void CBurstHandling::MakeBurstDurationHist_HB()
{
	//This function makes a histgram of Total open time of bursts.
	if(!m_bBurstNode) MakeBurstNodes();
	structHBHistParNode aNode;
	aNode.dLinMin=0.79;
	aNode.dLinDelta=0.2;
	aNode.dLogMin=1.3;
	aNode.dLogMax=6.;
	aNode.dLogDelta=0.05;

	int nNumLinBins=0;
	double dLinLength=CUF::Exp(10.,aNode.dLogMin)-aNode.dLinMin;

	if(dLinLength>0.)
	{
		nNumLinBins=int(dLinLength/aNode.dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}

	int nNumLogBins=int((aNode.dLogMax-aNode.dLogMin)/aNode.dLogDelta)+1;

	int nNumBins=nNumLinBins+nNumLogBins;

	int i;
	m_vBurstDurationHist_HB.clear();
	for(i=0;i<nNumBins;i++)
	{
		structHistNode aHistNode;
		aHistNode.dData=0;
		aHistNode.dLeft=CalBinPosition(aNode,i);
		m_vBurstDurationHist_HB.push_back(aHistNode);
	}

	int nSize=m_structvBurstNodes.size();
	double dTemp,nIndex;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dTemp=m_structvBurstNodes[i].fTotalOpenTime;
		nIndex=CalIndex_HB(aNode,dTemp);
		m_vBurstDurationHist_HB[nIndex].dData+=1.;
	}
	m_vBurstDurationHist_HB=RemoveEmptyBins(m_vBurstDurationHist_HB);
	m_bBurstDurationHist_HB=true;
}


void CBurstHandling::MakeBurstDurationHist_Log()
{
	//This function makes a histgram of Total open time of bursts.
	if(!m_bBurstNode) MakeBurstNodes();
	structHBHistParNode aNode;
	aNode.dLinMin=0.89; //To make nNumLinBins=0
	aNode.dLinDelta=0.2;
	aNode.dLogMin=-0.1;
	aNode.dLogMax=6.;
	aNode.dLogDelta=0.05;

	int nNumLinBins=0;
	double dLinLength=CUF::Exp(10.,aNode.dLogMin)-aNode.dLinMin;

	if(dLinLength>0.)
	{
		nNumLinBins=int((dLinLength-aNode.dLinMin)/aNode.dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}

	int nNumLogBins=int((aNode.dLogMax-aNode.dLogMin)/aNode.dLogDelta)+1;

	int nNumBins=nNumLinBins+nNumLogBins;

	int i;
	m_vBurstDurationHist_Log.clear();
	for(i=0;i<nNumBins;i++)
	{
		structHistNode aHistNode;
		aHistNode.dData=0;
		aHistNode.dLeft=CalBinPosition(aNode,i);
		m_vBurstDurationHist_Log.push_back(aHistNode);
	}

	int nSize=m_structvBurstNodes.size();
	double dTemp,nIndex;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dTemp=m_structvBurstNodes[i].fLength;
		nIndex=CalIndex_HB(aNode,dTemp);
		m_vBurstDurationHist_Log[nIndex].dData+=1.;
	}
	m_bBurstDurationHist_Log=true;
}


int CBurstHandling::CalIndex_HB(structHBHistParNode aNode, double dX0)
{
	int nIndex;
	double dLinMin=aNode.dLinMin,dLinDelta=aNode.dLinDelta;
	double dLogMin=aNode.dLogMin,dLogDelta=aNode.dLogDelta,dLogMax=aNode.dLogMax;

	double dExpLogMin=CUF::Exp(10.,dLogMin);
	double dLogX0=CUF::Log(10.,dX0);

	double dMin=dLinMin;

	if(dExpLogMin<dMin)
		dMin=dExpLogMin;

	if(dLogX0>dLogMax||dX0<dMin) 
		return -1;

	int nNumLinBins=0;
	double dLinLength=dExpLogMin-dLinMin;

	if(dLinLength>0.)
	{
		nNumLinBins=int(dLinLength/dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}


	if(dLogX0>=dLogMin)
	{
		nIndex=int((dLogX0-dLogMin)/dLogDelta);
		nIndex+=nNumLinBins;
	}
	else
	{
		nIndex=int((dX0-dLinMin)/dLinDelta);
	}
	return nIndex;
}


double CBurstHandling::CalBinPosition(structHBHistParNode aNode, int nIndex)
{
	double dLinMin=aNode.dLinMin,dLinDelta=aNode.dLinDelta;
	double dLogMin=aNode.dLogMin,dLogDelta=aNode.dLogDelta;
	double dBinPosition=0.;
	double dLinLength=CUF::Exp(10.,dLogMin)-dLinMin;
	int nNumLinBins;

	if(dLinLength>0.)
	{
		nNumLinBins=int(dLinLength/dLinDelta)+1;
	}
	else
	{
		nNumLinBins=0;
	}
	
	if(nIndex>nNumLinBins-1)
	{
		double dLog=dLogMin+dLogDelta*double(nIndex-nNumLinBins);
		dBinPosition=CUF::Exp(10.,dLog);
	}
	else
	{
		dBinPosition=dLinMin+dLinDelta*double(nIndex);
	}
	return dBinPosition;
}

void CBurstHandling::FitNumOpeningPerBurst(FILE* fpTotal)
{
	// TODO: Add your command handler code here
	//Attention!
	//This compiler assign the same memory address for all CString objects created in the same scope.
	//One way to get arround it is pass it to a function and get it back as a returned value of a function.
	//The same problem happed if you use a pointer and using new char[] commond. You have to use new char[] commond 
	//without delete the pointor in order to create an independt string, with the price of cause memory leakage.
	if(!m_bNOPBHist) MakeNOPBHist();

	int nMaxComponents=6; 
	const int nMinComponents=1; 
	CString sOutputFileName;

	int i,j,nNumComponents;

	bool bSignificant=true;
	int nSigComponents=1;
	int nSize,nTrial;
	const int nBigInteger=9999999;
//	nMaxComponents=4;//Correct it later 05424

	structNumOpenPerBurstFittingNode aNOPBFittingNode;

	aNOPBFittingNode.fBurstDelimiter=m_structBurstingTimeCourseOptionNode.fBurstDelimiter;
	aNOPBFittingNode.sEvlFileName=m_sEvlFileName;
	aNOPBFittingNode.nMinComponents=nMinComponents;
	char cTemp;
	CString sCurFileName0=CUF::ExtendFileName(m_sEvlFileName,"_NOPB_");
	cTemp=m_nBurstDelimiterIndex+48;
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,cTemp);
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,"_");
	sCurFileName0=CUF::ChangeFileExt(sCurFileName0,"Cur");
	CString sResultFileName=CUF::ChangeFileExt(sCurFileName0,"Nob");
	FILE* fpOutput=fopen(sResultFileName,"w");
	fprintf(fpOutput,"Evl File: %s\n",m_sEvlFileName);

	CString sCurFileName=CUF::PassingString(sCurFileName0);

	nNumComponents=nMinComponents;
	while(bSignificant&&nNumComponents<=nMaxComponents)
//	while(nNumComponents<=nMaxComponents) //Correct it later 05424
	{
		vector <structGeometricFittingNode> avTrialNodes;
		vector <structGeometricFittingNode> avResultNodes;

		structGeometricFittingNode aTrialNode;
		aTrialNode.fFittingRange.fLeft=1;
		aTrialNode.fFittingRange.fRight=nBigInteger;
		aTrialNode.nFuncKey=1;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aTrialNode.bFreezeR[nTempIndex]=false;
			aTrialNode.bFreezeW[nTempIndex]=false;
		}

		aTrialNode.nNumComponents=nNumComponents;
		aTrialNode.nTrialNumber=0;

		avTrialNodes=GetGeometricFittingTrials(aTrialNode);

		nSize=avTrialNodes.size();
		for(nTrial=0;nTrial<nSize;nTrial++)
		{
			aTrialNode=avTrialNodes.at(nTrial);
			aTrialNode.nTrialNumber=nTrial;
			CGeometricFitting NOPBFitting(aTrialNode, m_vNumOpenPerBurstHist);
			structGeometricFittingNode aResultNode=NOPBFitting.GetFittedNode();
			aResultNode.nNumTotalEvents=m_structvBurstNodes.size();
			avResultNodes.push_back(aResultNode);
		}

		nSize=avResultNodes.size();
		SortFittingResults(avResultNodes);
//		if(m_bCalConfidanceRange) SetConfidanceRange(nFuncKey, avResultNodes);

		double dL, dProb;
		int nDf;
//		ExportDwellCurves(avResultNodes);
		aNOPBFittingNode.vResultNodeCollections.push_back(avResultNodes);
		aNOPBFittingNode.vTrialNodeCollections.push_back(avTrialNodes);
		aNOPBFittingNode.vnNumOfTrials.push_back(avResultNodes.size());
		nSize=aNOPBFittingNode.vResultNodeCollections.size();
		if(nNumComponents>nMinComponents)
		{
			bSignificant=Significant(aNOPBFittingNode.vResultNodeCollections.at(nSize-2)[0],aNOPBFittingNode.vResultNodeCollections.at(nSize-1)[0],dL,nDf,dProb);
			fprintf(fpOutput,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			fprintf(fpAllResults,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			OutputNOPBFittingResults(avTrialNoes[0],avResultNodes[0],fpOutput);
			if(bSignificant)
			{
				nSigComponents=nNumComponents;
			}
			aNOPBFittingNode.vpValue.push_back(dProb);
		}
		cTemp=48+nNumComponents;
		sCurFileName=CUF::ExtendFileName(sCurFileName0,cTemp);
//		ExportGeometricCurves(sCurFileName,avResultNodes,m_vNumOpenPerBurstHist);
		nNumComponents++;
	}

	aNOPBFittingNode.nNumSigComponents=nSigComponents;

	ExportGeometricCurves(sCurFileName0,aNOPBFittingNode,m_vNumOpenPerBurstHist);
	int nDetailIndex=0;
	OutputNOPBFittingResults(aNOPBFittingNode,fpOutput,nDetailIndex);
	nDetailIndex=1;
	if(fpTotal!=NULL)
	{
		OutputNOPBFittingResults(aNOPBFittingNode,fpTotal,nDetailIndex);
	}
	fclose(fpOutput);
}

void CBurstHandling::SortFittingResults(vector <structGeometricFittingNode> &avResultNode)
{
	vector <structGeometricFittingNode> bvResultNode;
	int nSize=avResultNode.size();
	Vec_IO_DP arr(nSize);
	Vec_IO_DP brr(nSize);
	int nTrial;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		arr[nTrial]=-avResultNode.at(nTrial).dLogLik;
		brr[nTrial]=double(nTrial);
	}
	NR::sort2(arr,brr);

	int nIndex=0;
	nIndex=int(brr[0]+0.5);
	double dLogLik0=avResultNode.at(nIndex).dLogLik;
	double dLogLik_R;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		structGeometricFittingNode aNode;
		nIndex=int(brr[nTrial]+0.5);
		aNode=avResultNode.at(nIndex);
		aNode.nScoreRanking=nTrial;
//		aNode.nClusterIndex=nTrial;
		dLogLik_R=aNode.dLogLik-dLogLik0;
		aNode.dLogLik_R=dLogLik_R;
		bvResultNode.push_back(aNode);
	}

	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		avResultNode.at(nTrial)=bvResultNode.at(nTrial);
	}
}

void CBurstHandling::SortFittingResults(vector <structExponentialFittingNode> &avResultNode)
{
	vector <structExponentialFittingNode> bvResultNode;
	int nSize=avResultNode.size();
	Vec_IO_DP arr(nSize);
	Vec_IO_DP brr(nSize);
	int nTrial;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		arr[nTrial]=-avResultNode.at(nTrial).dLogLik;
		brr[nTrial]=double(nTrial);
	}
	NR::sort2(arr,brr);

	int nIndex=0;
	nIndex=int(brr[0]+0.5);
	double dLogLik0=avResultNode.at(nIndex).dLogLik;
	double dLogLik_R;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		structExponentialFittingNode aNode;
		nIndex=int(brr[nTrial]+0.5);
		aNode=avResultNode.at(nIndex);
		aNode.nScoreRanking=nTrial;
//		aNode.nClusterIndex=nTrial;
		dLogLik_R=aNode.dLogLik-dLogLik0;
		aNode.dLogLik_R=dLogLik_R;
		bvResultNode.push_back(aNode);
	}

	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		avResultNode.at(nTrial)=bvResultNode.at(nTrial);
	}
}



vector <structGeometricFittingNode> CBurstHandling::GetGeometricFittingTrials(structGeometricFittingNode aTrialNode)
{
	vector <structGeometricFittingNode> avTrialNode;
	int nComponents=aTrialNode.nNumComponents;
	int nFuncKey=aTrialNode.nFuncKey;
	double dRmin=0.,dRmax=0.;
	structFRange fFittingRange=aTrialNode.fFittingRange;
	dRmin=0.0001;
	double dMaxOpenings=double(m_nMaxOpeningNum);
	dRmax=double(dMaxOpenings-1)/double(dMaxOpenings);
	double dR[20];
	double dW[20];

	if(nComponents==1) 
	{
		structGeometricFittingNode aNewTrialNode;
		aNewTrialNode.dRol[0]=.5*(dRmin+dRmax);
		aNewTrialNode.dWeights[0]=1.;
		aNewTrialNode.nTrialNumber=1;
		aNewTrialNode.nNumComponents=nComponents;
		aNewTrialNode.fFittingRange=fFittingRange;
		aNewTrialNode.nFuncKey=nFuncKey;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aNewTrialNode.bFreezeR[nTempIndex]=aTrialNode.bFreezeR[nTempIndex];
			aNewTrialNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
		}

		avTrialNode.push_back(aNewTrialNode);
		return avTrialNode;
	}

	double dLeftArray[6]={1.,2000.,4000.,200.,1000.};
	double dRightArray0[10]={1.,0.3,0.1,0.1,0.05,0.05,8.,40.,20.,15.};
	const int nLeft=3,nRight0=3;
	int i,j,l;
	double dLeft,dDelta;

	for(i=0;i<nComponents;i++)
	{
		dW[i]=1./double(nComponents);
	}
	
	double dLog10=log(10.);
	double dTemp=1.;
	double dOpeningsR0;
	double dOpeningsR;
	double dOpeningsL0;
	int nTrialNumber=0;
	for(i=0;i<nLeft;i++)
	{
		for(j=0;j<nRight0;j++)
		{
			dLeft=dRmin*dLeftArray[i];

			dOpeningsR0=double(m_nMaxOpeningNum)*dRightArray0[j];

			if(dOpeningsR0<2.)
				dOpeningsR0=2.;
			
			dR[0]=dLeft;
			dR[nComponents-1]=double(dOpeningsR0-1)/double(dOpeningsR0);;
			dOpeningsR0=log(dOpeningsR0)/dLog10;  //converted to Logarithmic scale
			dOpeningsL0=log(2.)/dLog10;
			if(nComponents>=3)
			{
				for(l=1;l<nComponents-1;l++)
				{
					dDelta=(dOpeningsR0-dOpeningsL0)/double(nComponents-1);
					dTemp=dOpeningsL0+dDelta*double(l); //Logarithmic scale
					dOpeningsR=exp(dTemp*dLog10); //converted to linear scale
					dR[l]=(dOpeningsR-1.)/dOpeningsR;
				}
				structGeometricFittingNode aNode;
				for(l=0;l<nComponents;l++)
				{
					aNode.dRol[l]=dR[l];
					aNode.dWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeR[nTempIndex]=aTrialNode.bFreezeR[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}

				nTrialNumber++;
				aNode.nTrialNumber=1;
				aNode.nNumComponents=nComponents;
				aNode.fFittingRange=fFittingRange;
				aNode.nFuncKey=nFuncKey;
				avTrialNode.push_back(aNode);
			}
			else
			{
				structGeometricFittingNode aNode;
				for(l=0;l<nComponents;l++)
				{
					aNode.dRol[l]=dR[l];
					aNode.dWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeR[nTempIndex]=aTrialNode.bFreezeR[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}

				nTrialNumber++;
				aNode.nTrialNumber=nTrialNumber;
				aNode.nNumComponents=nComponents;
				aNode.fFittingRange=fFittingRange;
				aNode.nFuncKey=nFuncKey;
				avTrialNode.push_back(aNode);
			}
		}
	}
	return avTrialNode;
}

bool CBurstHandling::Significant(structGeometricFittingNode aNode, structGeometricFittingNode bNode, double& dL, int& nDf, double& dProb)
{
	double dL1=aNode.dLogLik;
	double dL2=bNode.dLogLik;
	int nDf1=2*aNode.nNumComponents-1;
	int nDf2=2*bNode.nNumComponents-1;
	nDf=nDf2-nDf1;
	dL=dL2-dL1;
	dProb=NR::gammq(0.5*nDf,fabs(dL));
	//gammq is the chi square probability function. 2dL have a chi^2 distributions with nDf
	//the function gammq has been tested using nDf=2 for dProb values 0.05, 0.025, 0.01 and nDf=6 for p value 0.0
	//This function was double checked on 1/10/2006
	if(dProb<0.05&&dL>0.) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool CBurstHandling::Significant(structExponentialFittingNode aNode, structExponentialFittingNode bNode, double& dL, int& nDf, double& dProb)
{
	double dL1=aNode.dLogLik;
	double dL2=bNode.dLogLik;
	int nDf1=2*aNode.nNumComponents-1;
	int nDf2=2*bNode.nNumComponents-1;
	nDf=nDf2-nDf1;
	dL=dL2-dL1;
	dProb=NR::gammq(0.5*nDf,fabs(dL));
	//gammq is the chi square probability function. 2L have a chi^2 distributions with nDf
	//the function gammq has been tested using nDf=2 for dProb values 0.05, 0.025, 0.01 and nDf=6 for p value 0.0
	if(dProb<0.05&&dL>0.) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void CBurstHandling::ExportNOPBCurves(structNumOpenPerBurstFittingNode aNodeCollection)
{
	//This function is assuming that the fiting result nodes in the array are ordered (assending) according 
	//to the numbers of components used for fitting. All nodes are for the same dwell level.
	CString sFileName=aNodeCollection.sCurveFileName;
	vector <structGeometricFittingNode> avResultNode;
	int nSize=aNodeCollection.vResultNodeCollections.size();
	int i,nMin;
	nMin=nSize-3;
	if(nMin<0) nMin=0;

	for(i=nMin;i<nSize;i++)
	{
		avResultNode.push_back(aNodeCollection.vResultNodeCollections.at(i)[0]);
	}

	nSize=avResultNode.size();
	int nComponents0=avResultNode.at(0).nNumComponents;
	char cTemp=nComponents0+48;
	sFileName=CUF::ExtendFileName(sFileName,cTemp);
	int nComponents1=avResultNode.at(nSize-1).nNumComponents;
	sFileName=CUF::ExtendFileName(sFileName,'_');
	cTemp=nComponents1+48;
	sFileName=CUF::ExtendFileName(sFileName,cTemp);

	sFileName=CUF::ChangeFileExt(sFileName,"Crv");
	ExportGeometricCurves(sFileName,avResultNode,m_vNumOpenPerBurstHist);
}

double CBurstHandling::Geometric(double dRol[], double dW[], int nDim, double dX)
{
	double dResult=0.;
	double dX0,dX1;
	for(int i=0;i<nDim;i++)
	{
		dX0=dRol[i];
		dX1=dW[i];
		dResult+=dX1*(1-dX0)*CUF::Exp(dX0,dX-1.);
	}
	return dResult;
}

double CBurstHandling::Geometric(double dRol, double dX)
{
	double dResult=(1-dRol)*CUF::Exp(dRol,dX-1.);
	return dResult;
}


void CBurstHandling::ExportGeometricCurves(CString sFileName, vector <structGeometricFittingNode> avResultNodes,
										   vector <structHistNode> aGeoHist)
{
// This function assumes all Geometric fitting nodes in the array have the same number of components.
	int nFuncKey=0;
	structGeometricFittingNode aResultNode;
	int nComponents=aResultNode.nNumComponents;
	FILE* fp=fopen(sFileName,"w");

	int i,j;
	double dTemp,dX,dTemp0;
	double dDelta=0.01f;

	int nNumNodes=avResultNodes.size();

	for(i=0;i<nNumNodes;i++)
	{
		nComponents=avResultNodes.at(i).nNumComponents;
		fprintf(fp,"    NumOpenings");
		for(j=0;j<nComponents;j++)
		{
			fprintf(fp,"     %2dComp:",j+1);
		}
		fprintf(fp,"      %2dSum",i);

		fprintf(fp,"  NumOpenings      Counts");
	}
	fprintf(fp,"\n");

	int nNumBins=aGeoHist.size();
	double dTotalEstimatedEvents=0;
	double dTotalEvents=0;
	for(i=0;i<nNumBins;i++)
	{
		dTotalEvents+=aGeoHist[i].dData;
	}

	int nLine=0;

	nFuncKey=avResultNodes.at(0).nFuncKey;

	dDelta=0.2;
	double dProb=1.;
	dX=0;
	double dRol,dWeight;
	int nMaxOpenings=aGeoHist[nNumBins-1].dRight;
	int nNumOpenings=0;
	int nBinIndex=0;
	bool bEmptyBin=true;
	double dData=0;
	double dScale=1.;
	int nDelta=1;
	while(dProb>0.0001&&nLine<20*nMaxOpenings||nLine<nMaxOpenings)
	{
		if(nLine>100)
		{
			dScale=CUF::Log(10.,double(nLine+1));
			dScale=CUF::Exp(10.,double(int(dScale)-1));
			//this is to reduced the number of lines to export
		}

		dX+=dDelta*dScale;
		dProb=.000001;

		if(nBinIndex<nNumBins)
		{
			nNumOpenings=aGeoHist[nBinIndex].dLeft;
			if(nLine+1==nNumOpenings)
			{
				dData=aGeoHist[nBinIndex].dData;
				bEmptyBin=false;
			}
			else
			{
				dData=0.;
				bEmptyBin=true;
			}
//			fprintf(fp," %12.0f%12.0f",nLine,sqrt(dTemp));
		}
		else
		{
			dData=0;
			bEmptyBin=true;
		}

		for(int nNodeNumber=0;nNodeNumber<nNumNodes;nNodeNumber++)
		{
			nComponents=avResultNodes.at(nNodeNumber).nNumComponents;
			dTotalEstimatedEvents=dTotalEvents/avResultNodes.at(nNodeNumber).dArea;
			fprintf(fp,"%15.1f",dX);

			dTemp=0.f;
			for(j=0;j<nComponents;j++)
			{
				dRol=avResultNodes[nNodeNumber].dRol[j];
				dWeight=avResultNodes[nNodeNumber].dWeights[j];
				dTemp0=dWeight*dTotalEstimatedEvents*Geometric(dRol,dX);
				fprintf(fp,"  %10.3f",sqrt(dTemp0));
				dTemp+=dTemp0;
			}
			fprintf(fp," %10.3f",sqrt(dTemp));
			if(dTemp>dProb)
			{
				dProb=dTemp;
			}

			fprintf(fp," %12.0f%12.3f",double(nLine+1),sqrt(dData));
		}
		if(!bEmptyBin)
			nBinIndex++;
		nLine++;
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void CBurstHandling::OutputNOPBFittingResults(structGeometricFittingNode aTrialNode, structGeometricFittingNode aResultNode, FILE* fpOutput, int nDetailIndex)
{
	int i,j,nFreezed;
	int nNumComponents=aResultNode.nNumComponents;
	switch (nDetailIndex)
	{
	case 0:
		fprintf(fpOutput,"ScoreRanking:%3d,    TrialNumber:%3d,    Components:%3d,    Iterations:%5d\n",
						  aResultNode.nScoreRanking, aResultNode.nTrialNumber, aResultNode.nNumComponents, aResultNode.nIterations);
		fprintf(fpOutput,"RolFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeR[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}

		fprintf(fpOutput,"\n");
		fprintf(fpOutput,"WgtFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeW[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Rol:    ");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.6f",aTrialNode.dRol[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Mu:    ");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.1f",1./(1.-aTrialNode.dRol[i]));
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Weights:");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.6f",aTrialNode.dWeights[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"dLogLik:  %12.2f,    dLogLik_R:  %8.2f    ",aResultNode.dLogLik,aResultNode.dLogLik_R);
		fprintf(fpOutput,"FittedRange: %5.0f  to  %10.0f\n",aResultNode.fFittingRange.fLeft, aResultNode.fFittingRange.fRight);
		fprintf(fpOutput,"NumEvents:  %8d    FittedArea:  %6.4f\n",aResultNode.nNumTotalEvents,aResultNode.dArea);
		fprintf(fpOutput,"       Weights           Rol          Mean\n");
		double dRol;
		for(i=0;i<nNumComponents;i++)
		{
			dRol=aResultNode.dRol[i];
			fprintf(fpOutput,"    %10.6f    %10.6f    %10.0f\n",aResultNode.dWeights[i],dRol,1./(1.-dRol));
		}

		break;
	case 1:
		for(i=0;i<nNumComponents;i++)
		{
			dRol=aResultNode.dRol[i];
			fprintf(fpOutput,"  %10.6f",aResultNode.dWeights[i]);
		}

		for(i=0;i<nNumComponents;i++)
		{
			dRol=aResultNode.dRol[i];
			fprintf(fpOutput,"  %10.6f",dRol);
		}

		for(i=0;i<nNumComponents;i++)
		{
			dRol=aResultNode.dRol[i];
			fprintf(fpOutput,"  %10.0f",1./(1.-dRol));
		}

		fprintf(fpOutput,"  %10.1f",aResultNode.dMeanCA);
		break;
	}
}

void CBurstHandling::OutputNOPBFittingResults(structNumOpenPerBurstFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex)
{
	int i,j;
	int nSize=aResultCollection.vnNumOfTrials.size();
	int nSize0=0;
	switch (nDetailIndex)
	{
	case 0:
		for(i=0;i<nSize;i++)
		{
			nSize0=aResultCollection.vnNumOfTrials[i];
			for(j=0;j<nSize0;j++)
			{
				fprintf(fpOutput,"\n\n\n %s:\n",aResultCollection.sEvlFileName);
				fprintf(fpOutput,"Fitting of the number of opening per burst (burst delimiter %6.1f ms))\n",aResultCollection.fBurstDelimiter);
				fprintf(fpOutput,"Significant Components: %2d\n",aResultCollection.nNumSigComponents);
				OutputNOPBFittingResults(aResultCollection.vTrialNodeCollections.at(i)[j],aResultCollection.vResultNodeCollections.at(i)[j],fpOutput, nDetailIndex);
			}
		}
		break;
	case 1:
		fprintf(fpOutput,"  %8.1f ",aResultCollection.fBurstDelimiter);
		fprintf(fpOutput,"  %8d   ",aResultCollection.vResultNodeCollections.at(0)[0].nNumTotalEvents);
		fprintf(fpOutput,"  %8.1f ",m_dMeanNOPB);
		for(i=0;i<nSize;i++)
		{
			OutputNOPBFittingResults(aResultCollection.vTrialNodeCollections.at(i)[0],aResultCollection.vResultNodeCollections.at(i)[0],fpOutput, nDetailIndex);
		}
		for(i=0;i<nSize-1;i++)
		{
			fprintf(fpOutput,"    %10.4e",aResultCollection.vpValue[i]);
		}
		fprintf(fpOutput,"         %2d  ",aResultCollection.nNumSigComponents);
		fprintf(fpOutput,"     %s:  ",aResultCollection.sEvlFileName);
		break;
	}
	fprintf(fpOutput,"\n");
}

void CBurstHandling::CalMeanNOPB()
{
	int nSize,i,nTemp;
	double dMean;
	float fTemp;
	nSize=m_structvBurstNodes.size();
	dMean=0.;
	m_dMeanTOT=0.;
	m_dMeanBurstDuration=0.;
	m_fMaxTOT=-1.f;
	m_fMaxBurstDuration=-1.f;
	m_nMaxOpeningNum=-1;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dMean+=m_structvBurstNodes[i].nNumOpenings;
		m_dMeanTOT+=m_structvBurstNodes[i].fTotalOpenTime;
		m_dMeanBurstDuration+=m_structvBurstNodes[i].fLength;
		nTemp=m_structvBurstNodes[i].nNumOpenings;
		fTemp=m_structvBurstNodes[i].fTotalOpenTime;
		if(nTemp>m_nMaxOpeningNum)
		{
			m_nMaxOpeningNum=nTemp;
		}

		fTemp=m_structvBurstNodes[i].fTotalOpenTime;
		if(fTemp>m_fMaxTOT)
		{
			m_fMaxTOT=fTemp;
		}

		fTemp=m_structvBurstNodes[i].fLength;
		if(fTemp>m_fMaxBurstDuration)
		{
			m_fMaxBurstDuration=fTemp;
		}
	}
	dMean/=nSize;
	m_dMeanTOT/=nSize;
	m_dMeanBurstDuration/=nSize;
	m_dMeanNOPB=dMean;
}

void CBurstHandling::FitTotalOpentimePerBurst(FILE *fpTotal, int nFuncKey)
{
	//TODO: Add your command handler code here
	//Attention!
	//This compiler assign the same memory address for all CString objects created in the same scope.
	//One way to get arround it is pass it to a function and get it back as a returned value of a function.
	//The same problem happed if you use a pointer and using new char[] commond. You have to use new char[] commond 
	//without delete the pointor in order to create an independt string, with the price of cause memory leakage.
	if(!m_bNOPBHist) MakeNOPBHist();

	if(!m_bTOTPBHist_Log)
	{
		MakeTOTHist_Log();
	}
	if(!m_bTOTPBHist_HB)
	{
		MakeTOTHist_HB();
	}

	int nMaxComponents=9; 
	const int nMinComponents=1; 
	CString sOutputFileName;

//	int nFuncKey=1; //1=Log, 2=AP, 3=HB
	
	int i,j,nNumComponents;

	bool bSignificant=true;
	int nSigComponents=1;
	int nSize,nTrial;
	const int nBigInteger=9999999;
	nMaxComponents=9;//Correct it later 05424

	structTOTPBFittingNode aTOTPBFittingNode;

	aTOTPBFittingNode.fBurstDelimiter=m_structBurstingTimeCourseOptionNode.fBurstDelimiter;
	aTOTPBFittingNode.sEvlFileName=m_sEvlFileName;
	aTOTPBFittingNode.nMinComponents=nMinComponents;
	char cTemp;
	CString sCurFileName0=CUF::ExtendFileName(m_sEvlFileName,"_TOTPB_");
	cTemp=m_nBurstDelimiterIndex+48;
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,cTemp);
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,"_");
	sCurFileName0=CUF::ChangeFileExt(sCurFileName0,"Cur");
	CString sResultFileName=CUF::ChangeFileExt(sCurFileName0,"Tot");
	FILE* fpOutput=fopen(sResultFileName,"w");
	fprintf(fpOutput,"Evl File: %s\n",m_sEvlFileName);

	CString sCurFileName=CUF::PassingString(sCurFileName0);

	nNumComponents=nMinComponents;
//	while(bSignificant&&nNumComponents<=nMaxComponents)
	while(nNumComponents<=nMaxComponents&&bSignificant) 
	{
		vector <structExponentialFittingNode> avTrialNodes;
		vector <structExponentialFittingNode> avResultNodes;

		structExponentialFittingNode aTrialNode;
		aTrialNode.fFittingRange.fLeft=0.79; //Remember this 4/29/05
		aTrialNode.fFittingRange.fRight=m_fRecordLength;
		aTrialNode.nFuncKey=nFuncKey;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aTrialNode.bFreezeL[nTempIndex]=false;
			aTrialNode.bFreezeW[nTempIndex]=false;
		}

		aTrialNode.nNumComponents=nNumComponents;
		aTrialNode.nTrialNumber=0;

		avTrialNodes=GetTOTPBTrials(aTrialNode);

		nSize=avTrialNodes.size();
		for(nTrial=0;nTrial<nSize;nTrial++)
		{
			aTrialNode=avTrialNodes.at(nTrial);
			aTrialNode.nTrialNumber=nTrial;
			CExponentialFitting TOTPBFitting(aTrialNode, m_vTotalOpenTimeHist_HB);
			structExponentialFittingNode aResultNode=TOTPBFitting.GetFittedNode();
			aResultNode.nNumTotalEvents=m_structvBurstNodes.size();
			avResultNodes.push_back(aResultNode);
		}

		nSize=avResultNodes.size();
		SortFittingResults(avResultNodes);
//		if(m_bCalConfidanceRange) SetConfidanceRange(nFuncKey, avResultNodes);

		double dL, dProb;
		int nDf;
//		ExportDwellCurves(avResultNodes);
		aTOTPBFittingNode.vResultNodeCollections.push_back(avResultNodes);
		aTOTPBFittingNode.vTrialNodeCollections.push_back(avTrialNodes);
		aTOTPBFittingNode.vnNumOfTrials.push_back(avResultNodes.size());
		nSize=aTOTPBFittingNode.vResultNodeCollections.size();
		if(nNumComponents>nMinComponents)
		{
			bSignificant=Significant(aTOTPBFittingNode.vResultNodeCollections.at(nSize-2)[0],aTOTPBFittingNode.vResultNodeCollections.at(nSize-1)[0],dL,nDf,dProb);
			fprintf(fpOutput,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			fprintf(fpAllResults,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			OutputNOPBFittingResults(avTrialNoes[0],avResultNodes[0],fpOutput);
			if(bSignificant)
			{
				nSigComponents=nNumComponents;
			}
			aTOTPBFittingNode.vpValue.push_back(dProb);
		}
		cTemp=48+nNumComponents;
		sCurFileName=CUF::ExtendFileName(sCurFileName0,cTemp);
//		ExportExponentialCurves(sCurFileName,avResultNodes,m_vTotalOpenTimeHist_Log);
		nNumComponents++;
	}
	aTOTPBFittingNode.nNumSigComponents=nSigComponents;

	int nDetailIndex=0;
	OutputTOTPBFittingResults(aTOTPBFittingNode,fpOutput,nDetailIndex);

	ExportExponentialCurves(sCurFileName0,aTOTPBFittingNode,m_vTotalOpenTimeHist_Log);

	nDetailIndex=1;
	if(fpTotal!=NULL)
	{
		OutputTOTPBFittingResults(aTOTPBFittingNode,fpTotal,nDetailIndex);
	}
	fclose(fpOutput);
}



vector <structExponentialFittingNode> CBurstHandling::GetTOTPBTrials(structExponentialFittingNode aTrialNode)
{
	vector <structExponentialFittingNode> avTrialNode;
	int nComponents=aTrialNode.nNumComponents;
	double dTmin=0.,dTmax=0.;
	structFRange fFittingRange=aTrialNode.fFittingRange;
	dTmin=aTrialNode.fFittingRange.fLeft;
	dTmax=m_fMaxTOT;
	if(aTrialNode.nFuncKey==1||aTrialNode.nFuncKey==3)
	{
		aTrialNode.nFuncKey=2;
	}
	else
	{
		aTrialNode.nFuncKey=1;
	}

	double dT[20];
	double dW[20];

	if(nComponents==1) 
	{
		structExponentialFittingNode aNewTrialNode;
		aNewTrialNode.nFuncKey=aTrialNode.nFuncKey;
		aNewTrialNode.dLambda[0]=2./(dTmin+dTmax);
		aNewTrialNode.dTimeConstants[0]=.5*(dTmin+dTmax);
		aNewTrialNode.dWeights[0]=1.;
		aNewTrialNode.nTrialNumber=1;
		aNewTrialNode.nNumComponents=nComponents;
		aNewTrialNode.fFittingRange=fFittingRange;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aNewTrialNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
			aNewTrialNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
		}
		avTrialNode.push_back(aNewTrialNode);
		return avTrialNode;
	}

	double dLeftArray[10]={0.5,1.,2.,3.,4.,16.,32.,0.1,10.,20.};
	double dRightArray0[10]={0.7,0.3,0.1,0.05,0.05,8.,40.,20.,15.};
	double dRightArray1[10]={1.,0.5,1.5,0.05,0.05,8.,40.,20.,15.};
	const int nLeft=3,nRight0=3,nRight1=3;
	int i,j,k,l;
	double dLeft,dRight0,dRight1,dDelta;

	for(i=0;i<nComponents;i++)
	{
		dW[i]=1./double(nComponents);
	}
	
	double dLog10=log(10.);
	double dTemp=1.;
	double dUniform;
	int nTrialNumber=0;
	for(i=0;i<nLeft;i++)
	{
		for(j=0;j<nRight0;j++)
		{
			dLeft=dTmin*dLeftArray[i];
			dRight0=dTmax*dRightArray0[j];
			
			dT[0]=dLeft;
			dT[nComponents-1]=dRight0;
			dLeft=log(dLeft)/dLog10;
			dRight0=log(dRight0)/dLog10;
			if(nComponents>=3)
			{
				for(k=0;k<nRight1;k++)
				{
					dUniform=(dRight0-dLeft)/double(nComponents-1);
					dRight1=dRight0-dUniform*dRightArray1[k];
					dT[nComponents-2]=exp(dRight1*dLog10);
//					dRight1=log(dRight1)/dLog10;
					for(l=1;l<nComponents-2;l++)
					{
						dDelta=(dRight1-dLeft)/double(nComponents-2);
						dTemp=dLeft+dDelta*double(l);
						dT[l]=exp(dTemp*dLog10);
					}
					structExponentialFittingNode aNode;
					aNode.nFuncKey=aTrialNode.nFuncKey;
					for(l=0;l<nComponents;l++)
					{
						aNode.dLambda[l]=1./dT[l];
						aNode.dTimeConstants[l]=dT[1];
						aNode.dWeights[l]=dW[l];
					}

					for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
					{
						aNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
						aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
					}

					nTrialNumber++;
					aNode.nNumComponents=nComponents;
					aNode.fFittingRange=fFittingRange;
					avTrialNode.push_back(aNode);
				}
			}
			else
			{
				structExponentialFittingNode aNode;
				aNode.nFuncKey=aTrialNode.nFuncKey;
				for(l=0;l<nComponents;l++)
				{
					aNode.dLambda[l]=1./dT[l];
					aNode.dTimeConstants[l]=dT[l];
					aNode.dWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}


				nTrialNumber++;
				aNode.nNumComponents=nComponents;
				aNode.fFittingRange=fFittingRange;
				avTrialNode.push_back(aNode);
			}
		}
	}
	return avTrialNode;
}

vector <structExponentialFittingNode> CBurstHandling::GetBSTTrials(structExponentialFittingNode aTrialNode)
{
	vector <structExponentialFittingNode> avTrialNode;
	int nComponents=aTrialNode.nNumComponents;
	double dTmin=0.,dTmax=0.;
	structFRange fFittingRange=aTrialNode.fFittingRange;
	dTmin=aTrialNode.fFittingRange.fLeft;
	dTmax=m_fMaxBurstDuration;
	if(aTrialNode.nFuncKey==1||aTrialNode.nFuncKey==3)
	{
		aTrialNode.nFuncKey=2;
	}
	else
	{
		aTrialNode.nFuncKey=1;
	}

	double dT[20];
	double dW[20];

	if(nComponents==1) 
	{
		structExponentialFittingNode aNewTrialNode;
		aNewTrialNode.nFuncKey=aTrialNode.nFuncKey;
		aNewTrialNode.dLambda[0]=2./(dTmin+dTmax);
		aNewTrialNode.dTimeConstants[0]=.5*(dTmin+dTmax);
		aNewTrialNode.dWeights[0]=1.;
		aNewTrialNode.nTrialNumber=1;
		aNewTrialNode.nNumComponents=nComponents;
		aNewTrialNode.fFittingRange=fFittingRange;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aNewTrialNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
			aNewTrialNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
		}
		avTrialNode.push_back(aNewTrialNode);
		return avTrialNode;
	}

	double dLeftArray[10]={0.5,1.,2.,3.,4.,16.,32.,0.1,10.,20.};
	double dRightArray0[10]={0.7,0.3,0.1,0.05,0.05,8.,40.,20.,15.};
	double dRightArray1[10]={1.,0.5,1.5,0.05,0.05,8.,40.,20.,15.};
	const int nLeft=3,nRight0=3,nRight1=3;
	int i,j,k,l;
	double dLeft,dRight0,dRight1,dDelta;

	for(i=0;i<nComponents;i++)
	{
		dW[i]=1./double(nComponents);
	}
	
	double dLog10=log(10.);
	double dTemp=1.;
	double dUniform;
	int nTrialNumber=0;
	for(i=0;i<nLeft;i++)
	{
		for(j=0;j<nRight0;j++)
		{
			dLeft=dTmin*dLeftArray[i];
			dRight0=dTmax*dRightArray0[j];
			
			dT[0]=dLeft;
			dT[nComponents-1]=dRight0;
			dLeft=log(dLeft)/dLog10;
			dRight0=log(dRight0)/dLog10;
			if(nComponents>=3)
			{
				for(k=0;k<nRight1;k++)
				{
					dUniform=(dRight0-dLeft)/double(nComponents-1);
					dRight1=dRight0-dUniform*dRightArray1[k];
					dT[nComponents-2]=exp(dRight1*dLog10);
//					dRight1=log(dRight1)/dLog10;
					for(l=1;l<nComponents-2;l++)
					{
						dDelta=(dRight1-dLeft)/double(nComponents-2);
						dTemp=dLeft+dDelta*double(l);
						dT[l]=exp(dTemp*dLog10);
					}
					structExponentialFittingNode aNode;
					aNode.nFuncKey=aTrialNode.nFuncKey;
					for(l=0;l<nComponents;l++)
					{
						aNode.dLambda[l]=1./dT[l];
						aNode.dTimeConstants[l]=dT[1];
						aNode.dWeights[l]=dW[l];
					}

					for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
					{
						aNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
						aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
					}

					nTrialNumber++;
					aNode.nNumComponents=nComponents;
					aNode.fFittingRange=fFittingRange;
					avTrialNode.push_back(aNode);
				}
			}
			else
			{
				structExponentialFittingNode aNode;
				aNode.nFuncKey=aTrialNode.nFuncKey;
				for(l=0;l<nComponents;l++)
				{
					aNode.dLambda[l]=1./dT[l];
					aNode.dTimeConstants[l]=dT[l];
					aNode.dWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeL[nTempIndex]=aTrialNode.bFreezeL[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}


				nTrialNumber++;
				aNode.nNumComponents=nComponents;
				aNode.fFittingRange=fFittingRange;
				avTrialNode.push_back(aNode);
			}
		}
	}
	return avTrialNode;
}


void CBurstHandling::ExportExponentialCurves(CString sFileName, vector <structExponentialFittingNode> avResultNodes,
										   vector <structHistNode> aExpHist)
{
// This function assumes all Geometric fitting nodes in the array have the same number of components.
	int nFuncKey=0;
	structExponentialFittingNode aResultNode;
	int nComponents;
	FILE* fp=fopen(sFileName,"w");

	int i,j;
	double dTemp,dX,dTemp0,dTime;
	double dDelta=0.01f;
	double fBinWidth=0.05f;

	int nNumNodes=avResultNodes.size();


	int nNumBins=aExpHist.size();
	double dTotalEstimatedEvents=0;
	double dTotalEvents=0;
	for(i=0;i<nNumBins;i++)
	{
		dTotalEvents+=aExpHist[i].dData;
	}

	int nLine=0;
	int nIndex;

	nFuncKey=avResultNodes.at(0).nFuncKey;

	double dMax=aExpHist[nNumBins-1].dRight;
	double dProb=1.;
	double dL[20],dW[20];
	const int nDim=1;
	dX=-0.95;
	double dRol,dWeight;
	double dData=0;
	dDelta=0.05;
	double dT,dTime0,dTime1;
	const double dStep=0.01;

	for(i=0;i<nNumNodes;i++)
	{
		nComponents=avResultNodes.at(i).nNumComponents;
		fprintf(fp,"         Log(t)       Time(ms)");
		for(j=0;j<nComponents;j++)
		{
			fprintf(fp,"    %2dComp:%1d",i,j+1);
		}
		fprintf(fp,"         %2dSum",i);
		fprintf(fp,"        Log(t)      Time(ms)        Counts");
	}
	fprintf(fp,"\n");

	while(dProb>0.001||nLine<nNumBins)
	{
		dX+=dStep;
		dTime0=CUF::Exp(10.,dX);
		dTime1=CUF::Exp(10.,dX+dDelta);

		if(nLine<nNumBins)
		{
			dTime=aExpHist[nLine].dLeft;
			dData=aExpHist[nLine].dData;
		}
		else
		{
			dTime+=.01;
			dData=0.;
		}

		for(int nNodeNumber=0;nNodeNumber<nNumNodes;nNodeNumber++)
		{
			nComponents=avResultNodes.at(nNodeNumber).nNumComponents;
			dTotalEstimatedEvents=dTotalEvents/avResultNodes.at(nNodeNumber).dArea;
			fprintf(fp,"%15.3f%15.3f",dX,CUF::Exp(10.,dX));

			dTemp=0.f;
			for(j=0;j<nComponents;j++)
			{
				dT=avResultNodes[nNodeNumber].dTimeConstants[j];
				dWeight=avResultNodes[nNodeNumber].dWeights[j];
				dTemp0=dWeight*dTotalEstimatedEvents*(exp(-dTime0/dT)-exp(-dTime1/dT));
				fprintf(fp,"%12.3f",sqrt(dTemp0));
				dTemp+=dTemp0;
			}
			fprintf(fp,"%14.3f",sqrt(dTemp));
			dProb=dTemp;

			fprintf(fp,"%14.3f%14.3f%14.3f",CUF::Log(10.,dTime),dTime,sqrt(dData));
		}
		nLine++;
		fprintf(fp,"\n");
	}
	fclose(fp);
}


void CBurstHandling::ExportExponentialCurves(CString sFileName, structTOTPBFittingNode aResultCollection,
										   vector <structHistNode> aExpHist)
{
// This function assumes all Geometric fitting nodes in the array have the same number of components.
	vector <structExponentialFittingNode> avResultNodes;
	int nSize=aResultCollection.vResultNodeCollections.size();
	for(int i=0;i<nSize;i++)
	{
		avResultNodes.push_back(aResultCollection.vResultNodeCollections.at(i)[0]);
	}
	ExportExponentialCurves(sFileName,avResultNodes,aExpHist);
}

void CBurstHandling::ExportExponentialCurves(CString sFileName, structBSTFittingNode aResultCollection,
										   vector <structHistNode> aExpHist)
{
// This function assumes all Geometric fitting nodes in the array have the same number of components.
	vector <structExponentialFittingNode> avResultNodes;
	int nSize=aResultCollection.vResultNodeCollections.size();
	for(int i=0;i<nSize;i++)
	{
		avResultNodes.push_back(aResultCollection.vResultNodeCollections.at(i)[0]);
	}
	ExportExponentialCurves(sFileName,avResultNodes,aExpHist);
}


vector <structHistNode> CBurstHandling::RemoveEmptyBins(vector <structHistNode> avHist)
{
	vector <structHistNode> NewHist;
	int nSize=avHist.size();
	int i,j;
	double dTemp;
	for(i=0;i<nSize;i++)
	{
		dTemp=avHist[i].dData;
		if(dTemp>0.01)
		{
			NewHist.push_back(avHist[i]);
		}
	}

	nSize=NewHist.size();

	for(i=0;i<nSize-1;i++)
	{
		NewHist[i].dRight=NewHist[i+1].dLeft;
	}
	NewHist[nSize-1].dRight=m_fRecordLength;
	return NewHist;
}


void CBurstHandling::OutputTOTPBFittingResults(structExponentialFittingNode aTrialNode, structExponentialFittingNode aResultNode, FILE* fpOutput, int nDetailIndex)
{
	int i,j,nFreezed;
	int nNumComponents=aResultNode.nNumComponents;
	switch (nDetailIndex)
	{
	case 0:
		fprintf(fpOutput,"ScoreRanking:%3d,    TrialNumber:%3d,    Components:%3d,    Iterations:%5d\n",
						  aResultNode.nScoreRanking, aResultNode.nTrialNumber, aResultNode.nNumComponents, aResultNode.nIterations);
		fprintf(fpOutput,"RolFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeL[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}

		fprintf(fpOutput,"\n");
		fprintf(fpOutput,"WgtFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeW[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Tol:    ");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.1f",aTrialNode.dTimeConstants[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Weights:");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.6f",aTrialNode.dWeights[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"dLogLik:  %12.2f,    dLogLik_R:  %8.2f    ",aResultNode.dLogLik,aResultNode.dLogLik_R);
		fprintf(fpOutput,"FittedRange: %5.0f  to  %10.0f\n",aResultNode.fFittingRange.fLeft, aResultNode.fFittingRange.fRight);
		fprintf(fpOutput,"NumEvents:  %8d    FittedArea:  %6.4f\n",aResultNode.nNumTotalEvents,aResultNode.dArea);
		fprintf(fpOutput,"       Weights     TimeConstants\n");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"    %10.6f      %12.4f\n",aResultNode.dWeights[i],aResultNode.dTimeConstants[i]);
		}
		break;
	case 1:
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %10.6f",aResultNode.dWeights[i]);
		}

		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"%12.2f",aResultNode.dTimeConstants[i]);
		}

		fprintf(fpOutput,"  %10.1f",aResultNode.dMeanTCA);
		break;
	}
}

void CBurstHandling::OutputTOTPBFittingResults(structTOTPBFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex)
{
	int i,j;
	int nSize=aResultCollection.vnNumOfTrials.size();
	int nSize0=0;
	switch (nDetailIndex)
	{
	case 0:
		for(i=0;i<nSize;i++)
		{
			nSize0=aResultCollection.vnNumOfTrials[i];
			for(j=0;j<nSize0;j++)
			{
				fprintf(fpOutput,"\n\n\n %s:\n",aResultCollection.sEvlFileName);
				fprintf(fpOutput,"Fitting of the total open time per burst (burst delimiter %6.1f ms))\n",aResultCollection.fBurstDelimiter);
				fprintf(fpOutput,"Significant Components: %2d\n",aResultCollection.nNumSigComponents);
				OutputTOTPBFittingResults(aResultCollection.vTrialNodeCollections.at(i)[j],aResultCollection.vResultNodeCollections.at(i)[j],fpOutput, nDetailIndex);
			}
		}
		break;
	case 1:
		double dDummyP=-1.;
		fprintf(fpOutput,"  %8.1f ",aResultCollection.fBurstDelimiter);
		fprintf(fpOutput,"  %8d   ",aResultCollection.vResultNodeCollections.at(0)[0].nNumTotalEvents);
		fprintf(fpOutput,"  %8.1f ",m_dMeanTOT);
		for(i=0;i<nSize;i++)
		{
			OutputTOTPBFittingResults(aResultCollection.vTrialNodeCollections.at(i)[0],aResultCollection.vResultNodeCollections.at(i)[0],fpOutput, nDetailIndex);
		}
		for(i=nSize;i<4;i++)
		{
			OutputTOTPBFittingResults(DummyResultNode(i+1),DummyResultNode(i+1),fpOutput, nDetailIndex);
		}
		for(i=0;i<nSize-1;i++)
		{
			fprintf(fpOutput,"  %14.4e",aResultCollection.vpValue[i]);
		}
		for(i=nSize;i<4;i++)
		{
			fprintf(fpOutput,"  %14.4e",dDummyP);
		}
		fprintf(fpOutput,"         %2d  ",aResultCollection.nNumSigComponents);
		fprintf(fpOutput,"     %s:  ",aResultCollection.sEvlFileName);
		break;
	}
	fprintf(fpOutput,"\n");
}

structExponentialFittingNode CBurstHandling::DummyResultNode(int nComponents)
{
	structExponentialFittingNode aResultNode;
	aResultNode.nNumComponents=nComponents;
	for(int i=0;i<nComponents;i++)
	{
		aResultNode.dLambda[i]=-1.;
		aResultNode.dWeights[i]=-1.;
		aResultNode.dTimeConstants[i]=-1.;
	}
	aResultNode.dArea=-1.;
	aResultNode.dMeanTCA=-1.;
	return aResultNode;
}


void CBurstHandling::ExportGeometricCurves(CString sFileName, structNumOpenPerBurstFittingNode aResultCollection,
										   vector <structHistNode> aGeoHist)
{
	vector <structGeometricFittingNode> avResultNodes;
	int nSize=aResultCollection.vResultNodeCollections.size();
	for(int i=0;i<nSize;i++)
	{
		avResultNodes.push_back(aResultCollection.vResultNodeCollections.at(i)[0]);
	}
	ExportGeometricCurves(sFileName,avResultNodes,aGeoHist);
}


void CBurstHandling::FitBurstDurations(FILE *fpTotal, int nFuncKey)
{
	//TODO: Add your command handler code here
	//Attention!
	//This compiler assign the same memory address for all CString objects created in the same scope.
	//One way to get arround it is pass it to a function and get it back as a returned value of a function.
	//The same problem happed if you use a pointer and using new char[] commond. You have to use new char[] commond 
	//without delete the pointor in order to create an independt string, with the price of cause memory leakage.
	if(!m_bNOPBHist) MakeNOPBHist();

	if(!m_bBurstDurationHist_Log)
	{
		MakeBurstDurationHist_Log();
	}
	if(!m_bBurstDurationHist_HB)
	{
		MakeBurstDurationHist_HB();
	}

	int nMaxComponents=9; 
	const int nMinComponents=1; 
	CString sOutputFileName;

//	int nFuncKey=1; //1=Log, 2=AP, 3=HB
	
	int i,j,nNumComponents;

	bool bSignificant=true;
	int nSigComponents=1;
	int nSize,nTrial;
	const int nBigInteger=9999999;
	nMaxComponents=9;//Correct it later 05424

	structBSTFittingNode aBSTFittingNode;

	aBSTFittingNode.fBurstDelimiter=m_structBurstingTimeCourseOptionNode.fBurstDelimiter;
	aBSTFittingNode.sEvlFileName=m_sEvlFileName;
	aBSTFittingNode.nMinComponents=nMinComponents;
	char cTemp;
	CString sCurFileName0=CUF::ExtendFileName(m_sEvlFileName,"_BST_");
	cTemp=m_nBurstDelimiterIndex+48;
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,cTemp);
	sCurFileName0=CUF::ExtendFileName(sCurFileName0,"_");
	sCurFileName0=CUF::ChangeFileExt(sCurFileName0,"Cur");
	CString sResultFileName=CUF::ChangeFileExt(sCurFileName0,"Bst");
	FILE* fpOutput=fopen(sResultFileName,"w");
	fprintf(fpOutput,"Evl File: %s\n",m_sEvlFileName);

	CString sCurFileName=CUF::PassingString(sCurFileName0);

	nNumComponents=nMinComponents;
//	while(bSignificant&&nNumComponents<=nMaxComponents)
	while(nNumComponents<=nMaxComponents&&bSignificant) 
	{
		vector <structExponentialFittingNode> avTrialNodes;
		vector <structExponentialFittingNode> avResultNodes;

		structExponentialFittingNode aTrialNode;
		aTrialNode.fFittingRange.fLeft=0.79; //Remember this 4/29/05
		aTrialNode.fFittingRange.fRight=m_fRecordLength;
		aTrialNode.nFuncKey=nFuncKey;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aTrialNode.bFreezeL[nTempIndex]=false;
			aTrialNode.bFreezeW[nTempIndex]=false;
		}

		aTrialNode.nNumComponents=nNumComponents;
		aTrialNode.nTrialNumber=0;

		avTrialNodes=GetBSTTrials(aTrialNode);

		nSize=avTrialNodes.size();
		for(nTrial=0;nTrial<nSize;nTrial++)
		{
			aTrialNode=avTrialNodes.at(nTrial);
			aTrialNode.nTrialNumber=nTrial;
			CExponentialFitting BSTFitting(aTrialNode, m_vBurstDurationHist_HB);
			structExponentialFittingNode aResultNode=BSTFitting.GetFittedNode();
			aResultNode.nNumTotalEvents=m_structvBurstNodes.size();
			avResultNodes.push_back(aResultNode);
		}

		nSize=avResultNodes.size();
		SortFittingResults(avResultNodes);
//		if(m_bCalConfidanceRange) SetConfidanceRange(nFuncKey, avResultNodes);

		double dL, dProb;
		int nDf;
//		ExportDwellCurves(avResultNodes);
		aBSTFittingNode.vResultNodeCollections.push_back(avResultNodes);
		aBSTFittingNode.vTrialNodeCollections.push_back(avTrialNodes);
		aBSTFittingNode.vnNumOfTrials.push_back(avResultNodes.size());
		nSize=aBSTFittingNode.vResultNodeCollections.size();
		if(nNumComponents>nMinComponents)
		{
			bSignificant=Significant(aBSTFittingNode.vResultNodeCollections.at(nSize-2)[0],aBSTFittingNode.vResultNodeCollections.at(nSize-1)[0],dL,nDf,dProb);
			fprintf(fpOutput,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			fprintf(fpAllResults,"\n Comparison of best fittings using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nNumComponents-1,nNumComponents,dL,nDf,dProb); 
//			OutputNOPBFittingResults(avTrialNoes[0],avResultNodes[0],fpOutput);
			if(bSignificant)
			{
				nSigComponents=nNumComponents;
			}
			aBSTFittingNode.vpValue.push_back(dProb);
		}
		cTemp=48+nNumComponents;
//		sCurFileName=CUF::ExtendFileName(sCurFileName0,cTemp);
//		ExportExponentialCurves(sCurFileName,avResultNodes,m_vTotalOpenTimeHist_Log);
		nNumComponents++;
	}
	aBSTFittingNode.nNumSigComponents=nSigComponents;

	int nDetailIndex=0;
	OutputBSTFittingResults(aBSTFittingNode,fpOutput,nDetailIndex);

	ExportExponentialCurves(sCurFileName0,aBSTFittingNode,m_vBurstDurationHist_Log);

	nDetailIndex=1;
	if(fpTotal!=NULL)
	{
		OutputBSTFittingResults(aBSTFittingNode,fpTotal,nDetailIndex);
	}
	fclose(fpOutput);
}


void CBurstHandling::OutputBSTFittingResults(structExponentialFittingNode aTrialNode, structExponentialFittingNode aResultNode, FILE* fpOutput, int nDetailIndex)
{
	int i,j,nFreezed;
	int nNumComponents=aResultNode.nNumComponents;
	switch (nDetailIndex)
	{
	case 0:
		fprintf(fpOutput,"ScoreRanking:%3d,    TrialNumber:%3d,    Components:%3d,    Iterations:%5d\n",
						  aResultNode.nScoreRanking, aResultNode.nTrialNumber, aResultNode.nNumComponents, aResultNode.nIterations);
		fprintf(fpOutput,"RolFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeL[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}

		fprintf(fpOutput,"\n");
		fprintf(fpOutput,"WgtFreezed:");
		for(i=0;i<nNumComponents;i++)
		{
			if(aResultNode.bFreezeW[i])
				nFreezed=1;
			else
				nFreezed=0;
			fprintf(fpOutput,"%3d",nFreezed);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Tol:    ");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.1f",aTrialNode.dTimeConstants[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"Trial_Weights:");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %8.6f",aTrialNode.dWeights[i]);
		}
		fprintf(fpOutput,"\n");

		fprintf(fpOutput,"dLogLik:  %12.2f,    dLogLik_R:  %8.2f    ",aResultNode.dLogLik,aResultNode.dLogLik_R);
		fprintf(fpOutput,"FittedRange: %5.0f  to  %10.0f\n",aResultNode.fFittingRange.fLeft, aResultNode.fFittingRange.fRight);
		fprintf(fpOutput,"NumEvents:  %8d    FittedArea:  %6.4f\n",aResultNode.nNumTotalEvents,aResultNode.dArea);
		fprintf(fpOutput,"       Weights     TimeConstants\n");
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"    %10.6f      %12.4f\n",aResultNode.dWeights[i],aResultNode.dTimeConstants[i]);
		}
		break;
	case 1:
		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"  %10.6f",aResultNode.dWeights[i]);
		}

		for(i=0;i<nNumComponents;i++)
		{
			fprintf(fpOutput,"%12.2f",aResultNode.dTimeConstants[i]);
		}

		fprintf(fpOutput,"  %10.1f",aResultNode.dMeanTCA);
		break;
	}
}

void CBurstHandling::OutputBSTFittingResults(structBSTFittingNode aResultCollection, FILE* fpOutput, int nDetailIndex)
{
	int i,j;
	int nSize=aResultCollection.vnNumOfTrials.size();
	int nSize0=0;
	switch (nDetailIndex)
	{
	case 0:
		for(i=0;i<nSize;i++)
		{
			nSize0=aResultCollection.vnNumOfTrials[i];
			for(j=0;j<nSize0;j++)
			{
				fprintf(fpOutput,"\n\n\n %s:\n",aResultCollection.sEvlFileName);
				fprintf(fpOutput,"Fitting of the Burst Durations (burst delimiter %6.1f ms))\n",aResultCollection.fBurstDelimiter);
				fprintf(fpOutput,"Significant Components: %2d\n",aResultCollection.nNumSigComponents);
				OutputBSTFittingResults(aResultCollection.vTrialNodeCollections.at(i)[j],aResultCollection.vResultNodeCollections.at(i)[j],fpOutput, nDetailIndex);
			}
		}
		break;
	case 1:
		double dDummyP=-1.;
		fprintf(fpOutput,"  %8.1f ",aResultCollection.fBurstDelimiter);
		fprintf(fpOutput,"  %8d   ",aResultCollection.vResultNodeCollections.at(0)[0].nNumTotalEvents);
		fprintf(fpOutput,"  %8.1f ",m_dMeanBurstDuration);
		for(i=0;i<nSize;i++)
		{
			OutputBSTFittingResults(aResultCollection.vTrialNodeCollections.at(i)[0],aResultCollection.vResultNodeCollections.at(i)[0],fpOutput, nDetailIndex);
		}
		for(i=nSize;i<4;i++)
		{
			OutputBSTFittingResults(DummyResultNode(i+1),DummyResultNode(i+1),fpOutput, nDetailIndex);
		}
		for(i=0;i<nSize-1;i++)
		{
			fprintf(fpOutput,"  %14.4e",aResultCollection.vpValue[i]);
		}
		for(i=nSize;i<4;i++)
		{
			fprintf(fpOutput,"  %14.4e",dDummyP);
		}
		fprintf(fpOutput,"         %2d  ",aResultCollection.nNumSigComponents);
		fprintf(fpOutput,"     %s:  ",aResultCollection.sEvlFileName);
		break;
	}
	fprintf(fpOutput,"\n");
}

bool CBurstHandling::InterClusterClosing(int nIndex)
{
	bool bResult=false;
	float fIBC=m_structvBurstNodes[nIndex].fPrecedingIBC;
	if(fIBC<m_structBurstingTimeCourseOptionNode.fClusterDelimiter)
	{
		bResult=false;
	}else
	{
		bResult=true;
	}
	return bResult;
}

void CBurstHandling::CalClusterAve()
{
	float fTotalOpenTime;
	float fLength;
	int   nNumBursts;
	int	  nAveNOPB;
	float fAveTOT;
	float fAveBstDuration;
	int nSize0=m_structvClusterNodes.size();
	int i,j,nSize1;
	for(i=0;i<nSize0;i++)
	{
		if(!m_structvClusterNodes[i].bValid) continue;
		nSize1=m_structvClusterNodes[i].svBurstNodes.size();
		m_structvClusterNodes[i].nNumBursts=nSize1;
		nAveNOPB=0;
		fAveTOT=0.f;
		fAveBstDuration=0.f;
		for(j=0;j<nSize1;j++)
		{
			nAveNOPB+=m_structvClusterNodes[i].svBurstNodes[j].nNumOpenings;
			fAveBstDuration+=(m_structvClusterNodes[i].svBurstNodes[j].fLength+fAveBstDuration+m_structvClusterNodes[i].svBurstNodes[j].fPrecedingIBC);
			fAveTOT+=fAveBstDuration=m_structvClusterNodes[i].svBurstNodes[j].fTotalOpenTime;
		}
		nAveNOPB/=nSize1;
		fAveBstDuration/=double(nSize1);
		fAveTOT/=double(nSize1);
		m_structvClusterNodes[i].nAveNOPB=nAveNOPB;
		m_structvClusterNodes[i].fAveBstDuration=fAveBstDuration;
		m_structvClusterNodes[i].fAveTOT=fAveTOT;
	}
}


void CBurstHandling::WriteClusterTimeCourse(FILE *fpOut)
{
	fprintf(fpOut,"BstDuration   TOT           NOPB          Length        PrecedingICC  NumBst\n") ;
	int nSize=m_structvClusterNodes.size();
	for(int i=0;i<nSize;i++)
	{
		if(!m_structvClusterNodes[i].bValid) continue;
		fprintf(fpOut,"%14.2f",m_structvClusterNodes[i].fAveBstDuration);
		fprintf(fpOut,"%14.2f",m_structvClusterNodes[i].fAveTOT);
		fprintf(fpOut,"%14d",m_structvClusterNodes[i].nAveNOPB);
		fprintf(fpOut,"%14.2f",m_structvClusterNodes[i].fLength);
		fprintf(fpOut,"%14.2f",m_structvClusterNodes[i].fPrecedingICC);
		fprintf(fpOut,"%14d",m_structvClusterNodes[i].nNumBursts);
		fprintf(fpOut,"\n");
	}
}




float CBurstHandling::MBTo_0(int nIndex0, int nIndex1)
{
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	double dNum=0.;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		fTime+=MBTo(i, i);
	}
	return fTime/dNum;
}

float CBurstHandling::MFoib_0(int nIndex0, int nIndex1)
{
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	float fMFo=0.;
	double dNum=0.;

	for(int i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		fMFo+=MFoib(i,i);
	}
	fMFo/=dNum; //Mean frequency of opening (intra burst);
	return fMFo;
}

float CBurstHandling::MTcib_0(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fMDT=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	double dNum=0.;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		fMDT+=MTcib(i,i); //Mean dwelling time
	}
	return fMDT/dNum;
}

float CBurstHandling::Moib_0(int nIndex0, int nIndex1)
{
	int i,nNum=0;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	double dNum=0.;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		nNum+=m_structvBurstNodes[i].nNumOpenings;
	}
 
	return double(nNum)/dNum;
}

float CBurstHandling::MAmp_0(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fAmp=0.f;
	int j, nTemp0;
	double dNum=0.;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		fAmp+=MAmp(i,i);
		nNum++;
	}
	return fAmp/dNum;
}

float CBurstHandling::MIBC_0(int nIndex0, int nIndex1)
{
	//Mean Inter-burst interval
	int i;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	double dNum=0.;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		dNum+=1.;
		fTime+=MIBC(i,i);
	}
	double dMIBC=fTime/dNum; 
	return dMIBC;
}


float CBurstHandling::MTo_0(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;
	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;
	for(i=nIndex0;i<=nIndex1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nNum++;
		fTime+=MTo(i,i);
	}
	float fMDT=fTime/nNum; //Mean dwelling time
	return fMDT;
}


float CBurstHandling::MFo_0(int nIndex0, int nIndex1)
{
	int i,nNum=0;
	float fTime=0.f;

	if(nIndex0<0)
		nIndex0=0;
	int nSize=m_structvBurstNodes.size();
	if(nIndex1>nSize-1)
		nIndex1=nSize-1;

	for(i=nIndex0;i<=nIndex1;i++)
	{
		nNum++;
		fTime+=MFo(i,i);
	}
	float fMFo=fTime/double(nNum); //Mean frequency of opening (intra burst);
	return fMFo;
}

void	CBurstHandling::PickEvents_NOPB(int nNOPB1,int nNOPB2, bool *pbExcluded)//practically obsolete
{
	int nSize,i,nTemp,nI,nF;
	nSize=m_structvBurstNodes.size();
	int nNumOpenings;
	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nNumOpenings=m_structvBurstNodes[i].nNumOpenings;
		if(nNumOpenings<nNOPB1||nNumOpenings>nNOPB2)
		{
			nI=m_structvBurstNodes[i].nFirstOpening;
			nF=m_structvBurstNodes[i].nLastOpening;
			for(nTemp=nI;nTemp<=nF;nTemp++)
			{
				if(m_pstEventCurrentLevel[nTemp]==1)
				{
					pbExcluded[nTemp]=true;
				}
			}
		}
	}
}

void	CBurstHandling::PickIntraBust(int nEdge,bool *pbExcluded)
{
	int nSize,i,nTemp,nI,nF,nNum;
	nSize=m_structvBurstNodes.size();
	int nNumOpenings;

//	for(i=0;i<m_nNumEvents;i++)
//	{
//		pbExcluded[i]=true;
//	}

	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nNumOpenings=m_structvBurstNodes[i].nNumOpenings;
		nI=m_structvBurstNodes[i].nFirstOpening;
		nF=m_structvBurstNodes[i].nLastOpening;
		nNum=0;
		for(nTemp=nI;nTemp<=nF;nTemp++)
		{
			if(m_pstEventCurrentLevel[nTemp]==1)
			{
				nNum++;
				if(nNum<nEdge||nNum>=nNumOpenings-nEdge)
				{
					pbExcluded[nTemp]=true;
				}
			}
		}
	}
}

bool CBurstHandling::FirstBurst(int nBurstIndex)
{
	float fIBC=m_structvBurstNodes[nBurstIndex].fPrecedingIBC;
	bool bResult=true;
	if(fIBC>m_structBurstingTimeCourseOptionNode.fClusterDelimiter)
		bResult=true;
	else
		bResult=false;
	return bResult;
}

void CBurstHandling::PickFirstOpenings(bool *pbExcluded)
{
	int nSize,i,nTemp,nI,nF,nNum;
	nSize=m_structvBurstNodes.size();
	int nNumOpenings;

	for(i=0;i<m_nNumEvents;i++)
	{
		pbExcluded[i]=true;
	}

	float fTc=0.f;

	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nI=m_structvBurstNodes[i].nFirstOpening;
		fTc=m_structvBurstNodes[i].fPrecedingIBC;
		if(fTc<m_structBurstingTimeCourseOptionNode.fClusterDelimiter) 
			pbExcluded[nI]=false;
	}
}

void	CBurstHandling::PickIsolatedOpenings(bool *pbExcluded)
{
	int nLength,nLevel0,nLevel,nLevel1,nCutoff1, nCutoff2,i;
	nCutoff1=m_structBurstingTimeCourseOptionNode.fBurstDelimiter/(m_fTimePerPoint/1000.);
	nCutoff2=m_structBurstingTimeCourseOptionNode.fClusterDelimiter/(m_fTimePerPoint/1000.);

	for(i=0;i<m_nNumEvents;i++)
	{
		pbExcluded[i]=true;
	}

	int nNumValidEvents=0;

	for(i=1;i<m_nNumEvents-1;i++)
	{
		nLevel0=m_pstEventCurrentLevel[i-1];
		nLevel=m_pstEventCurrentLevel[i];
		if(nLevel==1&&nLevel0==0)
		{
			nLength=m_pnLevelLength[i-1];
			if(nLength>=nCutoff1&&nLength<nCutoff2)
			{
				nLevel1=m_pstEventCurrentLevel[i+1];
				nLevel=m_pstEventCurrentLevel[i];
				if(nLevel==1&&nLevel1==0)
				{
					nLength=m_pnLevelLength[i+1];
					if(nLength>=nCutoff1&&nLength<nCutoff2)
					{
						pbExcluded[i]=false;
						nNumValidEvents++;
					}
				}
			}
		}
		if(i-nNumValidEvents*2-1>0)
		{
			i=i;
		}

	}
}

void CBurstHandling::PickLastOpenings(bool *pbExcluded)
{
	int nSize,i,nTemp,nI,nF,nNum;
	nSize=m_structvBurstNodes.size();
	int nNumOpenings;

	for(i=0;i<m_nNumEvents;i++)
	{
		pbExcluded[i]=true;
	}

	float fTc=0.f;

	for(i=0;i<nSize-1;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nF=m_structvBurstNodes[i].nLastOpening;
		fTc=m_structvBurstNodes[i+1].fPrecedingIBC;
		if(fTc<m_structBurstingTimeCourseOptionNode.fClusterDelimiter) 
			pbExcluded[nF]=false;
	}
}

void CBurstHandling::SortBurstNodes(int nIndex0)
{
	int nSize=m_structvBurstNodes.size();
	Vec_IO_DP arr(nSize);
	Vec_IO_DP brr(nSize);
	Vec_IO_DP crr(nSize);
	int i;
	for(i=0;i<nSize;i++)
	{
		arr[i]=GetSortingVariable(i);
		brr[i]=double(i);
		crr[i]=double(i);
	}

	NR::sort2(arr,brr);
	for(i=0;i<nSize;i++)
	{
		m_vnSortedOrder[i]=int(brr[i]+0.5);
	}

	NR::sort2(brr,crr);
	for(i=0;i<nSize;i++)
	{
		m_vnRanking[i]=int(crr[i]+0.5);
	}
}

void CBurstHandling::FindingRange(double &dMin, double &dMax, int &nMin, int &nMax)
{
	int i,nOrder;
	double dTemp;
	int nSize=m_structvBurstNodes.size();
	for(i=0;i<nSize;i++)
	{
		nOrder=m_vnSortedOrder[i];
		dTemp=GetSortingVariable(nOrder);
		if(dTemp>=dMin)
		{
			nMin=i;
			break;
		}
	}

	for(i=nSize-1;i>=0;i--)
	{
		nOrder=m_vnSortedOrder[i];
		dTemp=GetSortingVariable(nOrder);
		if(dTemp<dMax||dMin==dMax&&dTemp==dMax)
		{
			nMax=i;
			break;
		}
	}
}

double CBurstHandling::GetSortingVariable(int nIndex)
{
	double dTemp;
	switch (m_nSortingIndex)
	{
	case 1:
		dTemp=double(m_structvBurstNodes.at(nIndex).nNumOpenings);
		break;
	case 2:
		dTemp=m_structvBurstNodes.at(nIndex).fTotalOpenTime;
		break;
	case 3:
		dTemp=m_structvBurstNodes.at(nIndex).fTotalOpenTime;
		break;
	default:
		dTemp=double(m_structvBurstNodes.at(nIndex).nNumOpenings);
		break;
	}
	return dTemp;
}

structBurstSemNode CBurstHandling::CalBurstSem(int nIndexI, int nIndexF)
{
	vector <double> vAmp;
	vector <double> vBMTo;
	vector <double> vMTo;
	vector <double> vNOPB;
	vector <double> vToT;
	int i,nOrder,nNumOpenings,nFirstOpening,nLastOpening,j;
	for(i=nIndexI;i<=nIndexF;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		nOrder=m_vnSortedOrder[i];
		nNumOpenings=m_structvBurstNodes[nOrder].nNumOpenings;
		vBMTo.push_back(m_structvBurstNodes[nOrder].fLength);
		vToT.push_back(m_structvBurstNodes[nOrder].fTotalOpenTime);
		vNOPB.push_back(double(nNumOpenings));
		vMTo.push_back(MTo(nOrder,nOrder));
		vAmp.push_back(MAmp(nOrder,nOrder));
	}
	structBurstSemNode aNode;
	aNode.fSemAmp=CalSem(vAmp);
	aNode.fSemMBTo=CalSem(vBMTo);
	aNode.fSemMTo=CalSem(vMTo);
	aNode.fSemNOPB=CalSem(vNOPB);
	aNode.fSemToT=CalSem(vToT);
	return aNode;
}

structSemNode CBurstHandling::CalSem(vector <double> vectfVect)
{
	int nSize=vectfVect.size();
	int i;

	float fMean=0.f,fSem=0.f;
	for(i=0;i<nSize;i++)
	{
		fMean+=vectfVect[i];
	}

	fMean/=float(nSize);
	for(i=0;i<nSize;i++)
	{
		fSem+=(fMean-vectfVect[i])*(fMean-vectfVect[i]);
	}

	if(nSize>1)
	{
		fSem/=float(nSize*(nSize-1));
		fSem=sqrt(fSem);
	}

	structSemNode aNode;

	aNode.fMean=fMean;
	aNode.fSem=fSem;
	aNode.nSampleSize=nSize;

	return aNode;
}

void CBurstHandling::WriteSortedHistogram(FILE *fpOut)
{
	vector <structHistNode> vHist;
	switch (m_nSortingIndex)
	{
	case 1:
		MakeNOPBHist();
		vHist=m_vNumOpenPerBurstHist;
		break;
	case 2:
		MakeTOTHist_HB();
		vHist=m_vTotalOpenTimeHist_HB;
		break;
	case 3:
		MakeTOTHist_Log();
		vHist=m_vTotalOpenTimeHist_Log;
		break;
	default:
		MakeNOPBHist();
		vHist=m_vNumOpenPerBurstHist;
		break;
	}
	fprintf(fpOut,"          i:      MNOPB:     SemNOPB:       MToT:      SemToT:       MAmp:      SemAmp:       MBTo:      SemBTo:        MTo:       SemTo:          N:\n");
	int nSize=vHist.size();
	structHistNode aNode;
	structBurstSemNode aBurstSemNode;
	int i,j,nMin,nMax;
	for(i=0;i<nSize;i++)
	{
		aNode=vHist[i];
		FindingRange(aNode.dLeft,aNode.dRight,nMin,nMax);
		aBurstSemNode=CalBurstSem(nMin,nMax);
		fprintf(fpOut,"%12d%12.2f%13.3f%12.2f%13.3f%12.2f%13.3f%12.2f%13.3f%12.2f%13.3f%12d\n",i,aBurstSemNode.fSemNOPB.fMean,aBurstSemNode.fSemNOPB.fSem,aBurstSemNode.fSemToT.fMean,aBurstSemNode.fSemToT.fSem,
			                                                                               aBurstSemNode.fSemAmp.fMean,aBurstSemNode.fSemAmp.fSem,aBurstSemNode.fSemMBTo.fMean,aBurstSemNode.fSemMBTo.fSem,
																						   aBurstSemNode.fSemMTo.fMean,aBurstSemNode.fSemMTo.fSem,nMax-nMin+1);
	}
}

void CBurstHandling::FilteringBurstNodes_NOPB(int nNOPB1,int nNOPB2)//practically obsolete, now use subsets
{
	int nNumOpenings;
	vector <structBurstNode>::iterator theIterator;
	for(theIterator=m_structvBurstNodes.begin();theIterator!=m_structvBurstNodes.end();theIterator++)
	{
		nNumOpenings=theIterator->nNumOpenings;
		if(nNumOpenings<nNOPB1||nNumOpenings>nNOPB2)
		{
			theIterator=m_structvBurstNodes.erase(theIterator);
			theIterator--;
		}
	}
}

vector <structBurstTCBinNode> CBurstHandling::GetBurstTimeCourst_UB(float fBinWidth)
{
	MakeBurstTimeCourse_UB(fBinWidth);
	return m_vstructBurstTCBinNodes;
}

void CBurstHandling::MakeBurstTimeCourse_UB(float fBinWidth)
{
	m_vstructBurstTCBinNodes.clear();
	float nNumBins=int(m_fRecordLength/fBinWidth)+1;
	int i,j,k;
	for(i=0;i<nNumBins;i++)
	{
		structBurstTCBinNode aNode;
		aNode.nFirstBurst=-1;
		aNode.nLastBurst=-1;
		aNode.nSpanningBurst=-1;
		aNode.fTOT=0.f;
		aNode.nNumBursts=0;
		aNode.nNumOpenings=0;
		m_vstructBurstTCBinNodes.push_back(aNode);
	}

	int nNumBursts=m_structvBurstNodes.size();

	float fBstBegin; //Beginning of the first opening;
	float fBstEnd;//Ending point of the last opening;
	structBurstNode aBstNode;
	structBurstNode pBstNode=m_structvBurstNodes[0];
	int nStartingBin,nEndingBin,nNumOpening0,nNumOpening1;
	int nFirstOpening, nLastOpening, nBinStart, nBinEnd,nOpeningStart;
	int nBstBegin,nBstEnd;
	int nFirstBurst,nLastBurst;
	int nSpanningBurst;
	int nTOT,nNumOpenings;
	float fTOT,fMBTo,fMFo,fMTo,fMTOTPB,fNOPB;
	int nBinWidth=int(m_structBurstingTimeCourseOptionNode.fBinWidth*1000/m_fTimePerPoint);
	float fMBTc,fMPob,fMFob,fMPoib;



	for(i=0;i<nNumBursts;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		aBstNode=m_structvBurstNodes[i];
		nFirstOpening=aBstNode.nFirstOpening;
		nLastOpening=aBstNode.nLastOpening;
		nBstBegin=m_pnLevelStart[nFirstOpening];
		nBstEnd=m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening];
		nStartingBin=nBstBegin/nBinWidth;
		nEndingBin=nBstEnd/nBinWidth;

		nNumOpening0=0;
		nNumOpening1=0;

		if(nStartingBin<nEndingBin)
		{
			nBinEnd=(nStartingBin+1)*nBinWidth; //End point of the bin in which the burst started

			nOpeningStart=nBstBegin;			
			while(nOpeningStart<nBinEnd)
			{
				nNumOpening0++;
				nOpeningStart=m_pnLevelStart[nFirstOpening+2*nNumOpening0];
			}

			nBinStart=nEndingBin*nBinWidth;//The starting point of the bin in which the burst ended.
			nOpeningStart=m_pnLevelStart[nLastOpening];			
			while(nOpeningStart>nBinStart)
			{
				nNumOpening1++;
				nOpeningStart=m_pnLevelStart[nLastOpening-2*nNumOpening1];
			}
			if(nNumOpening1==0)
			{
				nEndingBin--;
			}

			for(j=nStartingBin+1;j<nEndingBin;j++)
			{
				m_vstructBurstTCBinNodes[j].nSpanningBurst=i;
			}
		}

		structBurstTCBinInfoNode aInfoNode;
		aInfoNode.nStartingBin=nStartingBin;
		aInfoNode.nEndingBin=nEndingBin;
		aInfoNode.nNumOpenings0=nNumOpening0;
		aInfoNode.nNumOpenings1=nNumOpening1;
		aBstNode.structBinInfo=aInfoNode;
		m_structvBurstNodes[i]=aBstNode;

		if(nNumOpening1>0)
		{
			m_vstructBurstTCBinNodes[nEndingBin].nFirstBurst=i;
		}

		m_vstructBurstTCBinNodes[nEndingBin].nLastBurst=i;
		m_vstructBurstTCBinNodes[nStartingBin].nLastBurst=i;

		if(i==0)
		{
			m_vstructBurstTCBinNodes[nStartingBin].nFirstBurst=i;
		}
		else if(nStartingBin>pBstNode.structBinInfo.nEndingBin)
		{
			m_vstructBurstTCBinNodes[nStartingBin].nFirstBurst=i;
		}	
		pBstNode=aBstNode;
	}


	int nLevel,nLevel0;
	for(i=0;i<nNumBins;i++)
	{
		nBinStart=int(i*fBinWidth*1000./m_fTimePerPoint);
		nBinEnd=int((i+1)*fBinWidth*1000./m_fTimePerPoint);

		nFirstBurst=m_vstructBurstTCBinNodes[i].nFirstBurst;
		nLastBurst=m_vstructBurstTCBinNodes[i].nLastBurst;
		nSpanningBurst=m_vstructBurstTCBinNodes[i].nSpanningBurst;
		fTOT=0.f;
		nTOT=0;
		nNumOpenings=0;
		fMBTc=0.;
		fMFob=0.;
		fMPoib=0.;
		fMPob=0.;
		if(nSpanningBurst>=0)//The spanning burst covered entire bin.
		{
			nNumBursts=1;
			aBstNode=m_structvBurstNodes[nSpanningBurst];
			nFirstOpening=aBstNode.nFirstOpening;
			nLastOpening=aBstNode.nLastOpening;
			for(j=nFirstOpening;j<=nLastOpening;j+=2)
			{
				nOpeningStart=m_pnLevelStart[j];
				if(nOpeningStart>=nBinStart&&nOpeningStart<nBinEnd)
				{
					nNumOpenings++;
					nTOT+=m_pnLevelLength[j];
				}
			}	
			fTOT=nTOT*m_fTimePerPoint/1000.;
			fMBTo=(m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening]-m_pnLevelStart[nFirstOpening])*m_fTimePerPoint/1000.;
			fMFo=nNumOpenings/fBinWidth;
			fMTo=fTOT/nNumOpenings;
			fMTOTPB=aBstNode.fTotalOpenTime;
			fNOPB=aBstNode.nNumOpenings;
			fMBTc=0.;
			fMPob=1.;
			fMFob=0.;
			fMPoib=fTOT/fMBTo;
		}
		else if(nFirstBurst==-1||nLastBurst==-1) //An empty bin
		{
			fTOT=0.;
			fMBTo=0.;
			fMFo=0.;
			fMTo=0.;
			fMTOTPB=0.;
			fNOPB=0.;
		}
		else
		{
			fTOT=0.;
			fMBTo=0.;
			fMFo=0.;
			fMTo=0.;
			fMTOTPB=0.;
			fNOPB=0.;
			nTOT=0;
			nNumOpenings=0;
			fMBTc=0.;
			fMFob=0.;
			fMPoib=0.;
			fMPob=0.;
			nNumBursts=nLastBurst-nFirstBurst+1;
			for(j=nFirstBurst;j<=nLastBurst;j++)
			{
				aBstNode=m_structvBurstNodes[j];
				nFirstOpening=aBstNode.nFirstOpening;
				nLastOpening=aBstNode.nLastOpening;
				for(k=nFirstOpening;k<=nLastOpening;k+=2)
				{
					nOpeningStart=m_pnLevelStart[k];
					if(nOpeningStart>=nBinStart&&nOpeningStart<nBinEnd)
					{
						nNumOpenings++;
						nTOT+=m_pnLevelLength[k];
					}
				}	
				fMBTo+=(m_pnLevelStart[nLastOpening]+m_pnLevelLength[nLastOpening]-m_pnLevelStart[nFirstOpening])*m_fTimePerPoint/1000.;
				fMTOTPB+=aBstNode.fTotalOpenTime;
				fNOPB+=aBstNode.nNumOpenings;
				fMBTc+=aBstNode.fPrecedingIBC;
			}
			fTOT=nTOT*m_fTimePerPoint/1000.;
			fMBTo/=nNumBursts;
			fMFo=nNumOpenings/fBinWidth;
			fMTo=fTOT/nNumOpenings;
			fMTOTPB/=nNumBursts;
			fNOPB/=nNumBursts;
			fMFob=nNumBursts/fBinWidth;
			fMPoib=fMTo*fNOPB/fMBTo;
			fMBTc/=nNumBursts;
			fMPob=fMBTo*nNumBursts/fBinWidth; //Need to make this more general later 09/19/20
		}
		m_vstructBurstTCBinNodes[i].fTOT=fTOT;
		m_vstructBurstTCBinNodes[i].fMBTo=fMBTo;
		m_vstructBurstTCBinNodes[i].fMFo=fMFo;
		m_vstructBurstTCBinNodes[i].fMTOTPB=fMTOTPB;
		m_vstructBurstTCBinNodes[i].fNOPB=fNOPB;
		m_vstructBurstTCBinNodes[i].fPo=fTOT/fBinWidth;
		m_vstructBurstTCBinNodes[i].fMTo=fMTo;
		m_vstructBurstTCBinNodes[i].fMBTc=fMBTc;
		m_vstructBurstTCBinNodes[i].fMPob=fMPob;
		m_vstructBurstTCBinNodes[i].fMFob=fMFob;
		m_vstructBurstTCBinNodes[i].fMPoib=fMPoib;
		m_vstructBurstTCBinNodes[i].nNumBursts=nNumBursts;
	}
}

void CBurstHandling::WriteBurstingTimeCourse_UB(FILE *fpOut, CString sFileName)
{
	SmoothingBTC_UB();
	float fTime,fElapsedTime;
	int i,j,k;
	int nIndex0,nIndex1,nTemp;
	fprintf(fpOut,"        Time          MBTo        MTOTPB         MNOPB      NumBurst          MPob          MFob         MPoib          MBTc           MFo           MTo           MPo          File:");
	fprintf(fpOut,"\n");

	int nSize=m_vstructBurstTCBinNodes.size();
	for(i=0;i<nSize;i++)
	{
		fprintf(fpOut,"%12.2f  %12.2f  %12.2f  %12.2f  %12.2f  %12.4f  %12.4f  %12.2f  %12.2f  %12.4f  %12.2f  %12.4f          %s\n",(i+0.5)*m_structBurstingTimeCourseOptionNode.fBinWidth, m_vstructBurstTCBinNodes[i].fMBTo,
			m_vstructBurstTCBinNodes[i].fMTOTPB, m_vstructBurstTCBinNodes[i].fNOPB, float(m_vstructBurstTCBinNodes[i].nNumBursts), 
			m_vstructBurstTCBinNodes[i].fMPob, 1000.*m_vstructBurstTCBinNodes[i].fMFob, m_vstructBurstTCBinNodes[i].fMPoib, 
			m_vstructBurstTCBinNodes[i].fMBTc, m_vstructBurstTCBinNodes[i].fMFo,
			m_vstructBurstTCBinNodes[i].fMTo, m_vstructBurstTCBinNodes[i].fPo,sFileName);
	}


	int nNumOpenings=0,nTOT=0;
	float fMBTo=0.f, fMTOTPB=0.f, fMNOPB=0.f, fMFo=0.f, fMTo=0.f, fMPo=0.f,fBurstDuration=0.f;
	structBurstNode aBstNode;

	int nNumBursts=m_structvBurstNodes.size();
	float fMBTc=0.;
	for(i=0;i<nNumBursts;i++)
	{
		aBstNode=m_structvBurstNodes[i];
		fMBTo+=aBstNode.fLength;
		fMTOTPB+=aBstNode.fTotalOpenTime;
		fMBTc+=aBstNode.fPrecedingIBC;
		nNumOpenings+=aBstNode.nNumOpenings;
	}


	fMBTo/=nNumBursts;
	fMTo=fMTOTPB/nNumOpenings;
	fMPo=fMTOTPB/m_fRecordLength;
	fMTOTPB/=nNumBursts;
	fMBTc/=nNumBursts;
	fMNOPB=double(nNumOpenings)/double(nNumBursts);
	fMFo=nNumOpenings/m_fRecordLength;
	fprintf(fpOut,"        Time          MBTo        MTOTPB         MNOPB      NumBurst          MPob          MFob         MPoib          MBTc           MFo           MTo           MPo          File:");
	fprintf(fpOut,"\n");
	fprintf(fpOut,"%12.2f  %12.2f  %12.2f  %12.2f  %12.2f  %12.4f  %12.4f  %12.2f  %12.2f  %12.4f  %12.2f  %12.4f          %s\n",i*m_structBurstingTimeCourseOptionNode.fBinWidth, fMBTo,fMTOTPB, fMNOPB, float(nNumBursts), fMBTo/(fMBTo+fMBTc), 1000./(fMBTo+fMBTc), fMTo*fMNOPB/fMBTo, fMBTc, fMFo,fMTo, fMPo,sFileName);
}



int CBurstHandling::GetNumBursts()
{
	return m_structvBurstNodes.size();
}

void CBurstHandling::SmoothingBTC_UB()
{
	int nSize=m_vstructBurstTCBinNodes.size();
	int nI,nF;
	float fNumOpenings=0.f;
	float fNumBursts=0.f;
	float fMBTo=0.f;
	float fMTOTPB=0.f;
	float fMBTc=0.f;
	int i,j;
	int nWindow=5;
	for(i=0;i<nSize;i++)
	{
		nI=i-nWindow;
		nF=i+nWindow;
		if(nI<0)
			nI=0;
		if(nF>nSize-1)
			nF=nSize-1;
		fMBTo=0.f;
		fMTOTPB=0.f;
		fNumBursts=0.f; 
		fMBTc=0.f; 
		fNumOpenings=0.f;

		for(j=nI;j<=nF;j++)
		{
			fMBTo+=m_vstructBurstTCBinNodes[i].fMBTo*m_vstructBurstTCBinNodes[i].nNumBursts;
			fMTOTPB+=m_vstructBurstTCBinNodes[i].fMTOTPB*m_vstructBurstTCBinNodes[i].nNumBursts;
			fNumOpenings+=m_vstructBurstTCBinNodes[i].fNOPB*m_vstructBurstTCBinNodes[i].nNumBursts; 
			fNumBursts+=m_vstructBurstTCBinNodes[i].nNumBursts;
//			fMPob+=m_vstructBurstTCBinNodes[i].fMPob;
//			fMFob+=m_vstructBurstTCBinNodes[i].fMFob; 
//			fMPoib+=m_vstructBurstTCBinNodes[i].fMPoib; 
			fMBTc+=m_vstructBurstTCBinNodes[i].fMBTc*m_vstructBurstTCBinNodes[i].nNumBursts;; 
//			fMFo+=m_vstructBurstTCBinNodes[i].fMFo;
//			fMTo+=m_vstructBurstTCBinNodes[i].fMTo; 
//			fPo+=m_vstructBurstTCBinNodes[i].fPo;
		}
		m_vstructBurstTCBinNodes[i].fNOPB=fNumOpenings/fNumBursts; 
		m_vstructBurstTCBinNodes[i].nNumBursts=fNumBursts; 
		m_vstructBurstTCBinNodes[i].fMPob=fMBTo/(m_structBurstingTimeCourseOptionNode.fBinWidth*double(nF-nI+1));
		m_vstructBurstTCBinNodes[i].fMFob=fNumBursts/(m_structBurstingTimeCourseOptionNode.fBinWidth*double(nF-nI+1)); 
		m_vstructBurstTCBinNodes[i].fMPoib=fMTOTPB/fMBTo; 
		m_vstructBurstTCBinNodes[i].fMBTc=fMBTc/fNumBursts; 
		m_vstructBurstTCBinNodes[i].fMFo=fNumOpenings/(m_structBurstingTimeCourseOptionNode.fBinWidth*double(nF-nI+1));
		m_vstructBurstTCBinNodes[i].fMTo=fMTOTPB/fNumOpenings; 
		m_vstructBurstTCBinNodes[i].fPo=fMTOTPB/(m_structBurstingTimeCourseOptionNode.fBinWidth*double(nF-nI+1));
		m_vstructBurstTCBinNodes[i].fMBTo=fMBTo/fNumBursts;
		m_vstructBurstTCBinNodes[i].fMTOTPB=fMTOTPB/fNumBursts;
	}
}

	void CBurstHandling::WriteTMP(FILE* fpOutput)
	{
		double fLogTen=log(10.);
		typedef vector <double> vDouble ;
		vector<vDouble> vAmps;

		int i,j,nLevel,binIndex;
		float fMinOpenTime=1.f;
		int maxIndex=logDTIndex(1.,m_fMaxOpenTime,20);

		for(i=0;i<=maxIndex;i++){
			vector <double> vf;
			vAmps.push_back(vf);
		}

		int nNOP=0,num=0;

		structBurstNode aNode;
		int nSize=m_structvBurstNodes.size();
		int index,nOpenings;
		float base,fLength;
		for(i=0;i<nSize;i++)
		{
				nLevel=1;
				aNode=m_structvBurstNodes[i];
				nNOP=aNode.nNumOpenings;
				if(nNOP<m_structBurstingTimeCourseOptionNode.nMinOpening||nNOP>m_structBurstingTimeCourseOptionNode.nMaxOpening) continue;
				index=aNode.nPrecedingIBC;
				num=0;
				base=m_pfAmplitude[index];
				while(num<nNOP){
					index=NextEvent(index+1,nLevel,0);
					num++;
					fLength=m_pnLevelLength[index]*m_fTimePerPoint/1000.;
					binIndex=logDTIndex(fMinOpenTime,fLength,20);
					if(binIndex>=0) vAmps[binIndex].push_back(Amp(index));
				}
		}
		vector <double> vd;
		structSemNode bNode;
		int N;
		fprintf(fpOutput,"            To           Amp          Sem             N\n");
		for(i=0;i<=maxIndex;i++){
			vd=vAmps[i];
			bNode=CalSem(vd);
			N=bNode.nSampleSize;
			fprintf(fpOutput,"  %12.2f  %12.2f  %12.2f  %12d\n",fMinOpenTime*exp(i*fLogTen/20.),bNode.fMean,bNode.fSem,N);
		}
	}

	float CBurstHandling::Amp(int index){
		int index0=NextEvent(index,0,1,-1);
		int index1=NextEvent(index,0,1,1);
		float fAmp=0.;
		double num=0;
		if(index0>=0){
			num+=1.;
			fAmp+=m_pfAmplitude[index]-m_pfAmplitude[index0];
		}
		if(index1>=0){
			num+=1.;
			fAmp+=m_pfAmplitude[index]-m_pfAmplitude[index1];
		}
		return fAmp/num;
	}
	int CBurstHandling::logDTIndex(float minDT, float fDT, int nDivPerDecade){
		double fLogTen=log(10.);
		return int(nDivPerDecade*log(fDT/minDT)/fLogTen);
	}

void CBurstHandling::getEventIndexAPH(structBurstingTimeCourseOptionNode aNode, CLinearHistogramDouble &APH1, CLinearHistogramDouble &APH2, CLinearHistogramDouble &APHn1, CLinearHistogramDouble &APHn2)
{
	MakeBurstNodes(aNode);//this implementation is for negative current. need to modify to be used for positive current. also need to set a dialog box to be used for general purpose.
//	buildLevelAmpHist();
	buildLevelAmpHist_latancy();
	int index;

	CLinearHistogramHandler aHandler, aHDL;
	CLinearHistogramDouble APH,APHn;

	APH1.copyHist(m_cAPH1);
	APH1.mergeHist(m_cAPH1r);

	APH2.copyHist(m_cAPH2);
	APH2.mergeHist(m_cAPH2r);

	APHn1.copyHist(APH1);
	APHn2.copyHist(APH2);

	APH.copyHist(APH1);
	APH.mergeHist(APH2);

	aHandler.update(APH);
	if(APH.getTotalCounts()>0.1) index=aHandler.getLastSignificantPeak();
	if(APH.getTotalCounts()>0.1) APH.resetBinPosition(index,0.);

	aHandler.update(m_cAPH0);
	index=aHandler.getMainPeak();
//	m_cAPH0.resetBinPosition(index,0.);


	APHn.copyHist(APH);
	if(APHn.getTotalCounts()>0.1) aHandler.normalizePeakRange(-4.,0.,APHn);

	double shift,scale;

//	APH.getCoordinateTransformations(shift,scale);
	m_cAPH0.getCoordinateTransformations(shift,scale);

//	APH1.transformCoordinate(shift,scale);
//	APH2.transformCoordinate(shift,scale);

	APHn.getCoordinateTransformations(shift,scale);

	APHn1.transformCoordinate(shift,scale);
	APHn2.transformCoordinate(shift,scale);
}

void CBurstHandling::triangulize(float *pfData, int size)
{
	double b=-1, a=1, d=0.1, temp;
	int nSeed=-1;
	temp=NR::ran1(nSeed);
	int n=int ((a-b)/d+0.5);
	for(int i=0;i<size;i++){
//		temp=b+(i%n)*d+0.03123;
		temp=NR::ran1(nSeed);
		pfData[i]=temp;
	}
}

void CBurstHandling::buildLevelAmpHist_latancy()
{	/* this function build amplitude histograms for events of different levels (0 and 1) that happened in
	four different areas within the bursts. The events within a burst are grouped four groups. Group 1: the ones taking place 
	within dLatancy (ms) from the starting of the burst. Group 2: the ones taking place after dLatancy but befor the mid 
	point of the burst. Group 1r: the ones taking place dLatancy (ms) before the end of the burst. Group 2r: the ones
	taking place after the mid point of the burst and more than dLatancy (ms) earlier than the end of the burst. These 
	histograms for level i (0 or 1) are m_cLevelAmpHisti1, m_cLevelAmpHisti2, m_cLevelAmpHisti1r, and
	m_cLevelAmpHisti2r, respectively.

	In the case the parameter bDoAPH is true, all-point histograms are also generated for the four group of events. 
	The four histograms are m_cAPH1, m_cAPH2, m_cAPH1r and m_cAPH2r, respectively. An all-point histogram of all 
	closed (level 0) events, m_cAPH0, is also generated.

	The parameter dLatancy is a parameter input through burst timecourse option dialog.
	*/
	if(!m_bBurstNode) MakeBurstNodes();
	int i,j,nLevel;
	int nCutoff=0;
	int nNOP=0;
	int length;
	int nSize=m_structvBurstNodes.size();
	int nWs=1;
	double dLatancy=m_structBurstingTimeCourseOptionNode.fLatancy;//in ms

	int histIndex;
	bool bDoAPH=true;
	if(m_structAbfNode.nSize==0) bDoAPH=false;

	double dmin,dmax,dDelta1,dDelta2;
	int nDim1,nDim2;

	dDelta1=0.1;
	dDelta2=0.2;
	if(bDoAPH)
	{
		dmin=m_structAbfNode.fMinAmp;
		dmax=m_structAbfNode.fMaxAmp;
	}else{
		evlAmpRange(dmin,dmax);
	}
	dmin=-10.;
	dmax=5.;

	m_pfAmpc=new float[m_nNumEvents];
	for(i=0;i<m_nNumEvents;i++){
		m_pfAmpc[i]=0.f;
	}

	float tRising=0.333;
	int nRising=int(tRising*1000/m_fTimePerPoint)+1;

	if(bDoAPH)
	{
		m_cAPH0.update(dmin,dmax,dDelta1);
		m_cAPH1.update(dmin,dmax,dDelta1);
		m_cAPH2.update(dmin,dmax,dDelta1);
		m_cAPH1r.update(dmin,dmax,dDelta1);
		m_cAPH2r.update(dmin,dmax,dDelta1);
	}

	m_cLevelAmpHist01.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist02.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist01r.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist02r.update(dmin,dmax,dDelta2);

	m_cLevelAmpHist11.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist12.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist11r.update(dmin,dmax,dDelta2);
	m_cLevelAmpHist12r.update(dmin,dmax,dDelta2);

	nDim1=m_cAPH1.getSize();
	nDim2=m_cLevelAmpHist01.getSize();
	
	float *pfData=m_structAbfNode.pfData;


	float fTemp;
	int minLength=5;
	int maxLength=10000;
	int nBaselineLength=500, nFirtOpening;
	int index0,index1,index0r,index1r,firstIndex,num;
	nRising=int(330/m_fTimePerPoint)+1;
	int nFirstOpening,nIBC;
//	nRising=0;//Take this into account later.

	int nLatancy=int(dLatancy*1000./m_fTimePerPoint+0.5);
	int nLastOpening;
	int nBurstStart,nBurstEnd,nMidBurst;
	int nEventi,nEventf;
	int ni,nf,eventi,eventf;
	double xl,xr;
	float fBaseline;


	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		structBurstNode aNode=m_structvBurstNodes[i];
//		if(aNode.nFirstOpening<m_structBurstingTimeCourseOptionNode.nMinOpening) continue;

		nFirstOpening=aNode.nFirstOpening;
		if(nFirstOpening>0) eventi=nFirstOpening;
		nFirstOpening=m_pnLevelStart[nFirstOpening];
		nBurstStart=nFirstOpening;

		nLastOpening=aNode.nLastOpening;
		eventf=nLastOpening;
		nBurstEnd=m_pnLevelLength[nLastOpening];
		nLastOpening=m_pnLevelStart[nLastOpening];
		nBurstEnd+=nLastOpening;

		nMidBurst=(nBurstStart+nBurstEnd);


		nIBC=aNode.nPrecedingIBC;
		nIBC=m_pnLevelLength[nIBC];


		if(nIBC>nBaselineLength) nIBC=nBaselineLength;

		nLevel=1;
		nNOP=aNode.nNumOpenings;
		if(nNOP<m_structBurstingTimeCourseOptionNode.nMinOpening||nNOP>m_structBurstingTimeCourseOptionNode.nMaxOpening) nNOP=-1;
		index1=aNode.nPrecedingIBC;
		index0=aNode.nFirstOpening;
		index0r=aNode.nLastOpening;
		index1r=aNode.nLastOpening;
		int len0=10,len0r=10;
		int it;


		burstBaseline(i,ni,nf,xl,xr);
		xl=0.;
		xr=0.;
		index0=nFirstOpening-nIBC;
		m_cAPH0.addData(pfData,index0,1,nIBC);
		for(it=index0;it<index0+nIBC;it++){
			m_cAPH0.addData(pfData[it]-interpolation(ni,xl,nf,xr,it));
		}
		
		float fAmpc=0.f;
		int nAmpc=0;
		for(j=eventi-1;j<=eventf+1;j++){
			if(j<0||j>m_nNumEvents) continue;
			fAmpc=0.f;
			nAmpc=0;
			for(it=m_pnLevelStart[j]+nRising;it<=m_pnLevelStart[j]+m_pnLevelLength[j]-nRising;it++){
				fAmpc+=pfData[it]-interpolation(ni,xl,nf,xr,it);
				nAmpc+=1;
			}
			if(nAmpc>1) fAmpc/=nAmpc;
			if(nAmpc==0) fAmpc=999.;
			m_pfAmpc[j]=fAmpc;
		}


		int maxL,maxLr;
		for(j=0;j<nNOP;j++) 
		{
			fTemp=Amp(i,j+1);
			length=LevelLength(i,j+1);

			index1=NextEvent(index1+1,1,1,1);
			index0=NextEvent(index1,0,1,1);
			index0r=NextEvent(index0r-1,0,1,-1);
			index1r=NextEvent(index0r,1,1,1);

			nEventi=m_pnLevelStart[index1];
			nEventf=nEventi+m_pnLevelLength[index1];

			len0=m_pnLevelLength[index1];
			len0r=m_pnLevelLength[index1r];
			maxL=m_pnLevelLength[index1]+len0;
			maxLr=m_pnLevelLength[index1r]+len0r;

			if(nEventi-nBurstStart<=nLatancy){
				m_cLevelAmpHist11.addData(fTemp,length);
				m_cLevelAmpHist01.addData(m_pfAmplitude[index0],m_pnLevelLength[index0]);
				if(bDoAPH&&length>minLength&&length<=maxLength)
				{
					firstIndex=m_pnLevelStart[index1]+nRising;
					num=m_pnLevelStart[index0]+m_pnLevelLength[index0]-firstIndex-nRising;
					if(num>maxL)num=maxL;
//					m_cAPH1.addData(pfData,firstIndex,1,num);
					for(it=firstIndex;it<firstIndex+num;it++){
						m_cAPH1.addData(pfData[it]-interpolation(ni,xl,nf,xr,it));
					}
				}
			}else if(nEventi<nMidBurst)
			{
				m_cLevelAmpHist12.addData(fTemp,length);
				m_cLevelAmpHist02.addData(m_pfAmplitude[index0],m_pnLevelLength[index0]);
				if(bDoAPH&&length>minLength&&length<=maxLength)
				{
					firstIndex=m_pnLevelStart[index1]+nRising;
					num=m_pnLevelStart[index0]+m_pnLevelLength[index0]-firstIndex-nRising;
					if(num>maxL)num=maxL;
//					m_cAPH2.addData(pfData,firstIndex,1,num);
					for(it=firstIndex;it<firstIndex+num;it++){
						m_cAPH2.addData(pfData[it]-interpolation(ni,xl,nf,xr,it));
					}
				}
			}

			fTemp=Amp(i,-(j+1));
			length=LevelLength(i,j+1);

			nEventi=m_pnLevelStart[index1r];
			nEventf=nEventi+m_pnLevelLength[index1r];

			if(nBurstEnd-nEventf<nLatancy){
				m_cLevelAmpHist11r.addData(fTemp,length);
				m_cLevelAmpHist01r.addData(m_pfAmplitude[index0r],m_pnLevelLength[index0r]);
				if(bDoAPH&&length>minLength)
				{
					firstIndex=m_pnLevelStart[index0r]+nRising;
					num=m_pnLevelStart[index1r]+m_pnLevelLength[index1r]-firstIndex-nRising;
					if(num>maxLr){
						num=maxLr;
						firstIndex=m_pnLevelStart[index1r]-len0r;
					}
//					m_cAPH1r.addData(pfData,firstIndex,1,num);
					for(it=firstIndex;it<firstIndex+num;it++){
						m_cAPH1r.addData(pfData[it]-interpolation(ni,xl,nf,xr,it));
					}
				}
			}else if(nEventi>nMidBurst)
			{
				m_cLevelAmpHist12r.addData(fTemp,length);
				m_cLevelAmpHist02r.addData(m_pfAmplitude[index0r],m_pnLevelLength[index0r]);
				if(bDoAPH&&length>minLength)
				{
					firstIndex=m_pnLevelStart[index0r]+nRising;
					num=m_pnLevelStart[index1r]+m_pnLevelLength[index1r]-firstIndex-nRising;
					if(num>maxLr){
						num=maxLr;
						firstIndex=m_pnLevelStart[index1r]-len0r;
					}
//					m_cAPH2r.addData(pfData,firstIndex,1,num);
					for(it=firstIndex;it<firstIndex+num;it++){
						m_cAPH2r.addData(pfData[it]-interpolation(ni,xl,nf,xr,it));
					}
				}
			}
		}
	}
}

void CBurstHandling::burstBaseline(int index, int& ni, int& nf, double& dl, double& dr)
{
	structBurstNode bNode=m_structvBurstNodes[index];
	int nfo=bNode.nFirstOpening;
	int nlo=bNode.nLastOpening;
	int blen=200;
	int ji,jf;
	int nrising=3;
	double dm=0.,sum=0.;
	dl=0.;
	dr=0.;
	double sl=0.;
	double sr=0.;

	float* pfdata=m_structAbfNode.pfData;

	jf=m_pnLevelStart[nfo]-1;
	ni=jf;
	int len,i;
	if(nfo>0){
		len=m_pnLevelLength[nfo-1];
		if(len>blen) len=blen;
		ji=jf-len+nrising;
		jf-=nrising;

		int i;
		for(i=ji;i<=jf;i++){
			sl+=1.;
			dl+=pfdata[i];
		}
	}else{
		sl=-1.;
	}

	if(nlo<m_nNumEvents-1){
		ji=m_pnLevelStart[nlo+1];
		nf=ji;
		len=m_pnLevelStart[nlo+1];
		if(len>blen) len=blen;
		jf=ji+len-nrising;
		ji+=nrising;

		for(i=ji;i<=jf;i++){
			sr+=1.;
			dr+=pfdata[i];
		}
	}else{
		sr=-1;
	}
	if(sl>0) 
		dl/=sl;
	else
		dl=dr/sr;
	if(sr>0)
		dr/=sr;
	else
		dr=dl;
}

double CBurstHandling::interpolation(double x1, double y1, double x2, double y2, double x)
{
	double k=(y2-y1)/(x2-x1);
	return y1+k*(x-x1);
}

void CBurstHandling::pickIntraburstEvents(bool *pbExcluded, int nLatancy)
{
	int i,j,nLevel,nFirstOpening,nLastOpening;
	int nCutoff=0;
	int nSize=m_structvBurstNodes.size();
	structBurstNode aNode;

	for(i=0;i<m_nNumEvents;i++){
		pbExcluded[i]=true;
	}

	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		aNode=m_structvBurstNodes[i];
		if(aNode.nNumOpenings<2*nLatancy) continue;
		nFirstOpening=aNode.nFirstOpening;
		nFirstOpening=NextEvent(nFirstOpening,1,nLatancy,1);
//		nFirstOpening=m_pnLevelStart[nFirstOpening];
//		nBurstStart=nFirstOpening;

		nLastOpening=aNode.nLastOpening;
		nLastOpening=NextEvent(nLastOpening,1,nLatancy,-1);
//		nBurstEnd=m_pnLevelLength[nLastOpening];
//		nLastOpening=m_pnLevelStart[nLastOpening];
//		nBurstEnd+=nLastOpening;

		for(j=nFirstOpening+1;j<nLastOpening;j++)
		{
			pbExcluded[j]=false;
		}
	}

}

void CBurstHandling::pickIntraburstEvents(bool *pbExcluded, double dLatancy)
{
	int i,j,nLevel,nFirstOpening,nLastOpening,ni,nf,nBurstStart,nBurstEnd;
	int nCutoff=0;
	int nSize=m_structvBurstNodes.size();
	int nLatancy=int (dLatancy*1000/m_fTimePerPoint);
	structBurstNode aNode;

	for(i=0;i<m_nNumEvents;i++){
		pbExcluded[i]=true;
	}

	for(i=0;i<nSize;i++)
	{
		if(!m_structvBurstNodes[i].bValid) continue;
		aNode=m_structvBurstNodes[i];
		nFirstOpening=aNode.nFirstOpening;
		nBurstStart=m_pnLevelStart[nFirstOpening];

		nLastOpening=aNode.nLastOpening;
		nBurstEnd=m_pnLevelStart[nLastOpening];
		nBurstEnd+=m_pnLevelLength[nLastOpening];
		for(j=nFirstOpening;j<=nLastOpening;j++){
			ni=m_pnLevelStart[j];
			nf=ni+m_pnLevelLength[j];
			if(ni-nBurstStart>nLatancy&&nBurstEnd-nf>nLatancy) pbExcluded[j]=false;
		}
	}
}

void CBurstHandling::buildBurstEvolutionHist()
{
//	reverseRecording();
//	randomizeIntraburstEvents(0);
//	randomizeIntraburstEvents(0);
//	MakeBurstNodes();
	double dLogMin=0.,dDelLog=0.1;
	double dMinT=0.01,dDelT=20.;
	int nMin=0,nDelta=1;
	int nDim1=150,nDim2=150;
	m_cLatancyIndexesTf.update(0,nDim1,dMinT,dDelT);
	m_cLatancyIndexesNf.update(0,nDim2,nMin,nDelta);
	m_cLatancyIndexesTr.update(0,nDim1,dMinT,dDelT);
	m_cLatancyIndexesNr.update(0,nDim2,nMin,nDelta);
	m_cLatancyIndexesT.update(0,nDim1,dMinT,dDelT);
	m_cLatancyIndexesN.update(0,nDim2,nMin,nDelta);

	int i,j,nLevel,nFirstOpening,nLastOpening,ni,nf,nBurstStart,nBurstEnd;
	int nCutoff=0;
	int nSize=m_structvBurstNodes.size();
	int nLatancy;
	int nMidBurstEvent;//the middle event
	int nMidBurstPoint;//the middle time point of the burst
	double dLatancy;
	structBurstNode aNode;
	int nOpenings,nClosings,nNumOpenings;

	for(i=0;i<nSize;i++)
	{
		aNode=m_structvBurstNodes[i];
		if(!aNode.bValid) continue;
		nOpenings=0;
		nClosings=0;
		nFirstOpening=aNode.nFirstOpening;
		nBurstStart=m_pnLevelStart[nFirstOpening];

		nLastOpening=aNode.nLastOpening;
		nBurstEnd=m_pnLevelStart[nLastOpening];
		nBurstEnd+=m_pnLevelLength[nLastOpening];
		nMidBurstEvent=(nFirstOpening+nLastOpening)/2;
		nMidBurstPoint=(nBurstStart+nBurstEnd)/2;
		nNumOpenings=aNode.nNumOpenings;
		float fMaxTo=50.f;
		int nMaxTo=fMaxTo*1000/m_fTimePerPoint;
		for(j=nFirstOpening;j<=nLastOpening;j++){
			ni=m_pnLevelStart[j];
			nf=ni+m_pnLevelLength[j];
			nLevel=m_pstEventCurrentLevel[j];
			if(nLevel==1&&m_pnLevelLength[j]>nMaxTo){
				continue;
			}
			if(j<nMidBurstEvent){
				if(nLevel==1){
					if(m_pnLevelLength[j]>nMaxTo) continue;
					nLatancy=nOpenings;
					m_cLatancyIndexesNf.addIndex(nLatancy,j);
					m_cLatancyIndexesN.addIndex(nLatancy,j);
					nOpenings++;
				}
				if(nLevel==0){
					nLatancy=nClosings;
					m_cLatancyIndexesNf.addIndex(nLatancy,j);
					m_cLatancyIndexesN.addIndex(nLatancy,j);
					nClosings++;
				}
			}else{
				if(nLevel==1){
					if(m_pnLevelLength[j]>nMaxTo) continue;
					nLatancy=nNumOpenings-nOpenings-1;
					m_cLatancyIndexesNr.addIndex(nLatancy,j);
					m_cLatancyIndexesN.addIndex(nLatancy,j);
					nOpenings++;
				}
				if(nLevel==0){
					nLatancy=nNumOpenings-2-nClosings;
					m_cLatancyIndexesNr.addIndex(nLatancy,j);
					m_cLatancyIndexesN.addIndex(nLatancy,j);
					nClosings++;
				}
			}
			if(ni<nMidBurstPoint){
				dLatancy=(ni-nBurstStart)*m_fTimePerPoint/1000.;
				m_cLatancyIndexesTf.addIndex(dLatancy,j);
				m_cLatancyIndexesT.addIndex(dLatancy,j);
			}else{
//				dLatancy=(nBurstEnd-ni)*m_fTimePerPoint/1000.;
				dLatancy=(nBurstEnd-nf)*m_fTimePerPoint/1000.;
				m_cLatancyIndexesTr.addIndex(dLatancy,j);
				m_cLatancyIndexesT.addIndex(dLatancy,j);
			}
		}
	}
	int wsn=1;
	int wst=1;
	m_cLatancyIndexesTf.smoothHistogram(-1,nDim1-wst-1,wst);
//	m_cLatancyIndexesNf.smoothHistogram(0,nDim1-wst-1,wst);
	m_cLatancyIndexesTr.smoothHistogram(-1,nDim1-wst-1,wst);
//	m_cLatancyIndexesNr.smoothHistogram(wsn);
	m_cLatancyIndexesT.smoothHistogram(-1,nDim1-wst-1,wst);
//	m_cLatancyIndexesN.smoothHistogram(wsn);
}

void CBurstHandling::pickSubConductance(bool *pbExcluded, int lI, int lF)
{
	int i,j,it,level;
	for(i=0;i<m_nNumEvents;i++){
		pbExcluded[i]=true;
	}
	float time, amp, fLength;
	int nLevel;

	for(i=0;i<m_nNumEvents-1;i++){
		nLevel=m_pstEventCurrentLevel[i];
		fLength=m_pnLevelLength[i]*m_fTimePerPoint/1000;
		if(nLevel==0||fLength<1.) continue;
		if(nLevel==0) continue;
		level=conductanceLevel(i);
		time=m_pnLevelStart[i]*m_fTimePerPoint/1000.;
		amp=m_pfAmplitude[i]+m_pfBaseline[i];
		if(level>=lI&&level<=lF){
			for(j=-1;j<=1;j++){
				it=i+j;
				if(j==0)
					pbExcluded[it]=false;
				else
					if(m_pstEventCurrentLevel[it]==0) pbExcluded[it]=false;
			}
		}
	}
}

void CBurstHandling::importMatchingDataFile()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp data Files (*.dat;*.abf)\0*.abf;*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="import data file of the evl file";
	strcpy(aFileDlg.m_ofn.lpstrFile,ChangeFileExt(m_sEvlFileName,"dat"));
	CAbf cAbf;

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_structAbfNode=cAbf.GetTrace();
	}
}

void CBurstHandling::buildConductanceLevels()
{
	if(m_structAbfNode.nSize<0) importMatchingDataFile();
	if(!m_bLevelAPHBuilt) buildLevelAmpHist_latancy();
	CLinearHistogramDouble APH;

	APH.copyHist(m_cAPH1);
	APH.mergeHist(m_cAPH1r);
	APH.mergeHist(m_cAPH2);
	APH.mergeHist(m_cAPH2r);

	m_cLAHHandler.update(APH);
	m_cLAHHandler.findSignificantPeaks();
	m_bLevelAPHBuilt=true;
}

int CBurstHandling::conductanceLevel(int index)
{
	if(!m_bLevelAPHBuilt) buildConductanceLevels();
	float fAmp=m_pfAmplitude[index]+m_pfBaseline[index];
	int n=m_cLAHHandler.getNumValleys();
	double x;
	for(int i=0;i<n;i++){
		x=m_cLAHHandler.getValleyPosition(i);
		if(fAmp<x) return i;
	}
	return n;
}


void CBurstHandling::exportBurstEvolutions()
{
	if(!m_bBurstNode) MakeBurstNodes();
	bool bDataFileAvailable=false;
	int nLevels;
	if(bDataFileAvailable){
		if(!m_bLevelAPHBuilt) buildConductanceLevels();
		nLevels=m_cLAHHandler.getNumPeaks();
	}
	buildBurstEvolutionHist();
	CString sFileName=m_sEvlFileName;
	CBurstingTimeCourseOptionDlg aDlg;
	aDlg.SetParameters(m_structBurstingTimeCourseOptionNode);
	CFileNameHandling cfh;
	sFileName=cfh.ExtendFileName(sFileName,aDlg.getBurstEvolutionParametersAsString());
	FILE* fpEvt,*fpEvn;	fpEvt=fopen(ChangeFileExt(sFileName,"Evt"),"w");
	fpEvn=fopen(ChangeFileExt(sFileName,"Evn"),"w");

	int nDimT=m_cLatancyIndexesTf.getDim();
	int nDimN=m_cLatancyIndexesNf.getDim();
	structMeanSemNode ToNodef, TcNodef, AmpNodef;
	structMeanSemNode ToNoder, TcNoder, AmpNoder;
	structMeanSemNode ToNode, TcNode, AmpNode;
	int nConductanceLevels;
	double *pdConductanceLevelWeights,dLatancy;
	int i,j,nSize,nLatancy;
	vector <int> vnIndexes;
	if(bDataFileAvailable)pdConductanceLevelWeights=new double[nLevels];
	fprintf(fpEvn,"Latancy     TofMean      TofSem   Numf");
	fprintf(fpEvn,"     TcfMean      TcfSem   Numf");
	fprintf(fpEvn,"    AmpfMean     AmpfSem   Numf");

	fprintf(fpEvn,"   Latancy     TorMean      TorSem   Numf");
	fprintf(fpEvn,"     TcrMean      TcrSem   Numf");
	fprintf(fpEvn,"    AmprMean     AmprSem   Numf");

	fprintf(fpEvn,"   Latancy      ToMean       ToSem   Numf");
	fprintf(fpEvn,"      TcMean       TcSem   Numf");
	fprintf(fpEvn,"     AmpMean      AmpSem   Numf");
	fprintf(fpEvn,"\n");
	for(i=0;i<nDimN;i++){
		nLatancy=int(m_cLatancyIndexesNf.getValue(i)+0.5);
		vnIndexes=m_cLatancyIndexesNf.getIndexes(i);
		nSize=vnIndexes.size();
		TcNodef=getDwellTimeMS(vnIndexes,0);
		ToNodef=getDwellTimeMS(vnIndexes,1);
//		TcNodef=getDwellTimeMS(vnIndexes,0,10);
//		ToNodef=getDwellTimeMS(vnIndexes,1,10);
		AmpNodef=getAmpMS(vnIndexes);
		fprintf(fpEvn," %6d  %10.3f  %10.3f %6d",nLatancy,ToNodef.dMean,ToNodef.dSem,ToNodef.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",TcNodef.dMean,TcNodef.dSem,TcNodef.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",fabs(AmpNodef.dMean),AmpNodef.dSem,AmpNodef.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvn,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}

		nLatancy=int(m_cLatancyIndexesNr.getValue(i)+0.5);
		vnIndexes=m_cLatancyIndexesNr.getIndexes(i);
		nSize=vnIndexes.size();
		TcNoder=getDwellTimeMS(vnIndexes,0);
		ToNoder=getDwellTimeMS(vnIndexes,1);
//		TcNoder=getDwellTimeMS(vnIndexes,0,10);
//		ToNoder=getDwellTimeMS(vnIndexes,1,10);
		AmpNoder=getAmpMS(vnIndexes);
		fprintf(fpEvn,"    %6d  %10.3f  %10.3f %6d",nLatancy,ToNoder.dMean,ToNoder.dSem,ToNoder.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",TcNoder.dMean,TcNoder.dSem,TcNoder.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",fabs(AmpNoder.dMean),AmpNoder.dSem,AmpNoder.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvn,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}

		nLatancy=int(m_cLatancyIndexesN.getValue(i)+0.5);
		vnIndexes=m_cLatancyIndexesN.getIndexes(i);
		nSize=vnIndexes.size();
		TcNode=getDwellTimeMS(vnIndexes,0);
		ToNode=getDwellTimeMS(vnIndexes,1);
//		TcNode=getDwellTimeMS(vnIndexes,0,10);
//		ToNode=getDwellTimeMS(vnIndexes,1,10);
		AmpNode=getAmpMS(vnIndexes);
		fprintf(fpEvn,"    %6d  %10.3f  %10.3f %6d",nLatancy,ToNode.dMean,ToNode.dSem,ToNode.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",TcNode.dMean,TcNode.dSem,TcNode.nSize);  
		fprintf(fpEvn,"  %10.3f  %10.3f %6d",fabs(AmpNode.dMean),AmpNode.dSem,AmpNode.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvn,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}

		fprintf(fpEvn,"\n");
	}

	fprintf(fpEvt,"Latancy     TofMean      TofSem   Numf");
	fprintf(fpEvt,"     TcfMean      TcfSem   Numf");
	fprintf(fpEvt,"    AmpfMean     AmpfSem   Numf");

	fprintf(fpEvt,"   Latancy     TorMean      TorSem   Numf");
	fprintf(fpEvt,"     TcrMean      TcrSem   Numf");
	fprintf(fpEvt,"    AmprMean     AmprSem   Numf");

	fprintf(fpEvt,"   Latancy      ToMean       ToSem   Numf");
	fprintf(fpEvt,"      TcMean       TcSem   Numf");
	fprintf(fpEvt,"     AmpMean      AmpSem   Numf");
	fprintf(fpEvt,"\n");
	for(i=-1;i<nDimT;i++){
		dLatancy=m_cLatancyIndexesTf.getValue(i);
		vnIndexes=m_cLatancyIndexesTf.getIndexes(i);
		nSize=vnIndexes.size();
		TcNodef=getDwellTimeMS(vnIndexes,0);
		ToNodef=getDwellTimeMS(vnIndexes,1);
		AmpNodef=getAmpMS(vnIndexes);
		fprintf(fpEvt," %6.1f  %10.3f  %10.3f %6d",dLatancy,ToNodef.dMean,ToNodef.dSem,ToNodef.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",TcNodef.dMean,TcNodef.dSem,TcNodef.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",fabs(AmpNodef.dMean),AmpNodef.dSem,AmpNodef.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvt,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}

		dLatancy=m_cLatancyIndexesTr.getValue(i);
		vnIndexes=m_cLatancyIndexesTr.getIndexes(i);
		nSize=vnIndexes.size();
		TcNoder=getDwellTimeMS(vnIndexes,0);
		ToNoder=getDwellTimeMS(vnIndexes,1);
		AmpNoder=getAmpMS(vnIndexes);
		fprintf(fpEvt,"    %6.1f  %10.3f  %10.3f %6d",dLatancy,ToNoder.dMean,ToNoder.dSem,ToNoder.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",TcNoder.dMean,TcNoder.dSem,TcNoder.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",fabs(AmpNoder.dMean),AmpNoder.dSem,AmpNoder.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvt,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}

		dLatancy=m_cLatancyIndexesT.getValue(i);
		vnIndexes=m_cLatancyIndexesT.getIndexes(i);
		nSize=vnIndexes.size();
		TcNode=getDwellTimeMS(vnIndexes,0);
		ToNode=getDwellTimeMS(vnIndexes,1);
		AmpNode=getAmpMS(vnIndexes);
		fprintf(fpEvt,"    %6.1f  %10.3f  %10.3f %6d",dLatancy,ToNode.dMean,ToNode.dSem,ToNode.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",TcNode.dMean,TcNode.dSem,TcNode.nSize);  
		fprintf(fpEvt,"  %10.3f  %10.3f %6d",fabs(AmpNode.dMean),AmpNode.dSem,AmpNode.nSize);  
		if(bDataFileAvailable){
			calSubCondLevelWeights(pdConductanceLevelWeights,vnIndexes,nLevels);
			for(j=0;j<nLevels;j++){
				fprintf(fpEvt,"  %8.3f",pdConductanceLevelWeights[j]);
			}
		}


		fprintf(fpEvt,"\n");
	}

	fclose(fpEvt);
	fclose(fpEvn);
}
void CBurstHandling::calSubCondLevelWeights(double *pdLW, vector <int> vnIndexes, int nLevels){
	int i,nSize=vnIndexes.size(),index,level;
	for(i=0;i<nLevels;i++){
		pdLW[i]=0.;
	}
	for(i=0;i<nSize;i++){
		index=vnIndexes[i];
		level=conductanceLevel(index);
		pdLW[level]+=1.;
	}
	for(i=0;i<nLevels;i++){
		pdLW[i]/=nSize;
	}
}
structMeanSemNode CBurstHandling::getDwellTimeMS(vector <int> vnIndexes, int nLevel){
	int nSize=vnIndexes.size();
	int i,nL,index;
	vector <double> vdT;
	float fTimePerPoint=m_fTimePerPoint/1000.;
	for(i=0;i<nSize;i++){
		index=vnIndexes[i];
		nL=m_pstEventCurrentLevel[index];
		if(nL==nLevel){
			vdT.push_back(m_pnLevelLength[index]*fTimePerPoint);
//			vdT.push_back(log(m_pnLevelLength[index]*fTimePerPoint));
		}
	}
	return CMeanSem::calMeanSem(vdT);
}
structMeanSemNode CBurstHandling::getDwellTimeMS(vector <int> vnIndexes, int nLevel, int nGroupSize){
	int nSize=vnIndexes.size()/nGroupSize;
	int i,j,nL,no,index;
	vector <double> vdT,vdTt;
	structMeanSemNode ms;
	float fTimePerPoint=m_fTimePerPoint/1000.;
	double dMean;
	for(i=0;i<nSize;i++){
		no=i*nGroupSize;
		vdTt.clear();
		for(j=0;j<nGroupSize;j++){
			index=vnIndexes[no+j];
			nL=m_pstEventCurrentLevel[index];
			if(nL==nLevel){
				vdTt.push_back(m_pnLevelLength[index]*fTimePerPoint);
			}
		}
		ms=CMeanSem::calMeanSem(vdTt);
		dMean=ms.dMean;
		vdT.push_back(dMean);
	}
	return CMeanSem::calMeanSem(vdT);
}
structMeanSemNode CBurstHandling::getAmpMS(vector <int> vnIndexes){
	int nSize=vnIndexes.size();
	int i,index;
	int nLevel;
	int tMin=1.;
	int nMin=int(tMin/(m_fTimePerPoint/1000.));
	vector <double> vdT;
	for(i=0;i<nSize;i++){
		index=vnIndexes[i];
		nLevel=m_pstEventCurrentLevel[index];
		if(m_pnLevelLength[index]<nMin)continue;
		if(nLevel!=1) continue;
		vdT.push_back(m_pfAmplitude[index]);
	}
	return CMeanSem::calMeanSem(vdT);
}

void CBurstHandling::filteringBurstNodes()
{
	int n=m_structvBurstNodes.size();
	vector <structBurstNode> bursts;
	structBurstNode aNode;
	int nMinOpening=m_structBurstingTimeCourseOptionNode.nMinOpening;
	int nMaxOpening=m_structBurstingTimeCourseOptionNode.nMaxOpening;
	float fMinIBC=m_structBurstingTimeCourseOptionNode.fMinIBC;
	float fMaxIBC=m_structBurstingTimeCourseOptionNode.fMaxIBC;
	float fMinBTo=m_structBurstingTimeCourseOptionNode.fMinBurstDuration;
	float fMaxBTo=m_structBurstingTimeCourseOptionNode.fMaxBurstDuration;
	int i,no;
	float fIbc0,fIbc1,fBTo;
	fIbc0=m_structvBurstNodes[0].fPrecedingIBC;
	for(i=0;i<n-1;i++){
		aNode=m_structvBurstNodes[i];
		no=aNode.nNumOpenings;
		if(no<nMinOpening||no>nMaxOpening) continue;
		fIbc0=aNode.fPrecedingIBC;
		fIbc1=m_structvBurstNodes[i+1].fPrecedingIBC;
		if(fIbc0<fMinIBC||fIbc0>fMaxIBC) continue;
		if(fIbc1<fMinIBC||fIbc1>fMaxIBC) continue;
		fBTo=aNode.fLength;
		if(fBTo<fMinBTo||fBTo>fMaxBTo) continue;
		bursts.push_back(aNode);
	}
	aNode=m_structvBurstNodes[n-1];
	fIbc0=aNode.fPrecedingIBC;
	no=aNode.nNumOpenings;
	if(no>=nMinOpening&&no<=nMaxOpening&&fIbc0>=fMinIBC&&fIbc0<=fMaxIBC) bursts.push_back(aNode);
	m_structvBurstNodes.clear();
	m_structvBurstNodes=bursts;
	n=m_structvBurstNodes.size();
}

structBurstingTimeCourseOptionNode CBurstHandling::getInitializedParameters()
{
	structBurstingTimeCourseOptionNode aNode;
	CBurstingTimeCourseOptionDlg aDlg;
	aDlg.initialize(aNode);
	return aNode;
}

void CBurstHandling::filteringClusters()
{
	int nClusters=m_structvClusterNodes.size();
	int i,j,n;
	structClusterNode aNode;
	vector <structClusterNode> cNodes;
	vector <structBurstNode> bNodes;
	vector <structClusterNode> cNodest;
	vector <structBurstNode> bNodest;
	structBurstNode bNode;
	int nMinNOPC=m_structBurstingTimeCourseOptionNode.nMinNOPC;
	int nMaxNOPC=m_structBurstingTimeCourseOptionNode.nMaxNOPC;
	float fMinCTo=m_structBurstingTimeCourseOptionNode.fMinCTo;
	float fMaxCTo=m_structBurstingTimeCourseOptionNode.fMaxCTo;
	float fCTo;
	int nNOPC;
	for(i=0;i<nClusters;i++){
		aNode=m_structvClusterNodes[i];
		nNOPC=aNode.nAveNOPB*aNode.svBurstNodes.size();
		fCTo=aNode.fLength;
		if(nNOPC<nMinNOPC||nNOPC>nMaxNOPC) continue;
		if(fCTo<fMinCTo||fCTo>fMaxCTo) continue;
		aNode.bValid=true;
		cNodes.push_back(aNode);
		n=aNode.svBurstNodes.size();
		for(j=0;j<n;j++){
			bNodes.push_back(aNode.svBurstNodes[j]);
		}
	}
	cNodest=m_structvClusterNodes;
	bNodest=m_structvBurstNodes;
	m_structvClusterNodes=cNodes;
	m_structvBurstNodes=bNodes;
}

void CBurstHandling::reverseRecording()
{
	int nNumEvents=m_nNumEvents;
	int *pnLevelStart=new int[nNumEvents];
	int *pnLevelLength=new int[nNumEvents];
	short *pstEventCurrentLevel=new short[nNumEvents];
	float *pfAmplitude=new float[nNumEvents];
	float *pfBaseline=new float[nNumEvents];
	bool *pbExcluded=new bool[nNumEvents];

	int i,it,length, start=0;
	for(i=0;i<nNumEvents;i++)
	{
		it=nNumEvents-i-1;
		*(pnLevelStart+i)=start;
		length=*(m_pnLevelLength+it);
		*(pnLevelLength+i)=length;
		*(pstEventCurrentLevel+i)=*(m_pstEventCurrentLevel+it);
		*(pfAmplitude+i)=*(m_pfAmplitude+it);
		*(pfBaseline+i)=*(m_pfBaseline+it);
		*(pbExcluded+i)=*(m_pbExcluded+it);
		start+=length;
	}	
	for(i=0;i<nNumEvents;i++)
	{
		*(m_pnLevelStart+i)=*(pnLevelStart+i);
		*(m_pnLevelLength+i)=*(pnLevelLength+i);
		*(m_pstEventCurrentLevel+i)=*(pstEventCurrentLevel+i);
		*(m_pfAmplitude+i)=*(pfAmplitude+i);
		*(m_pfBaseline+i)=*(pfBaseline+i);
		*(m_pbExcluded+i)=*(pbExcluded+i);
		start+=length;
	}	
}

void CBurstHandling::randomizeIntraburstEvents(int nLatancy)
{
	int nSeed=-1,i,it;
	structBurstNode aNode;
	for(i=0;i<m_nNumEvents;i++){
		it=NR::ran1(nSeed);
	}
	int num=m_structvBurstNodes.size();
	for(i=0;i<num;i++){
		aNode=m_structvBurstNodes[i];
		randomizeEvents(aNode.nFirstOpening+2*nLatancy,aNode.nLastOpening-2*nLatancy,nSeed);
	}
}

void CBurstHandling::randomizeEvents(int index0, int index1, int &nSeed)
{
	typedef vector <int> vn;
	vn* pvn=new vn[m_nNumLevels];

	int nNumEvents=index1-index0+1;
	int i,j,n,level0,level1;
	for(i=0;i<m_nNumLevels;i++){
		vn vnt;
		pvn[i]=vnt;
	}

	for(i=index0;i<=index1;i++)
	{
		level0=m_pstEventCurrentLevel[i];
		pvn[level0].push_back(i);
	}

	vn avn;
	int it;
	vn::iterator ito;
	int num=index1-index0+1;

	int *pnt=new int[num];
	for(i=0;i<num;i++){
		pnt[i]=0;
	}
	for(i=index0;i<=index1;i++)
	{
		level0=m_pstEventCurrentLevel[i];
		avn=pvn[level0];
		n=avn.size();
		it=int(n*NR::ran1(nSeed));
		j=avn[it];
		ito=pvn[level0].begin();
		ito+=it;
		pvn[level0].erase(ito);
		if(pnt[j-index0]==0){
			pnt[j-index0]=j;
		}else{
			j=j;
		}
		exchangeEvents(i,j);
	}
	for(i=0;i<num;i++){
		if(pnt[i]==0){
			i=i;
		}
	}
	adjustEventStarts(index0,index1);
}

void CBurstHandling::exchangeEvents(int index0, int index1, bool bAdjustEventStart)
{
	exchangeEvents(index0, index1);
	adjustEventStarts(index0,index1);
}

void CBurstHandling::exchangeEvents(int index0, int index1)
{
	exchange(m_pnLevelLength,index0,index1);
	exchange(m_pstEventCurrentLevel,index0,index1);
	exchange(m_pfAmplitude,index0,index1);
	exchange(m_pfBaseline,index0,index1);
	exchange(m_pbExcluded,index0,index1);
}

void CBurstHandling::exchange(int *pnData, int index0, int index1)
{
	int nt=pnData[index0];
	pnData[index0]=pnData[index1];
	pnData[index1]=nt;
}

void CBurstHandling::exchange(float *pfData, int index0, int index1)
{
	float ft=pfData[index0];
	pfData[index0]=pfData[index1];
	pfData[index1]=ft;
}

void CBurstHandling::exchange(short *pstData, int index0, int index1)
{
	short st=pstData[index0];
	pstData[index0]=pstData[index1];
	pstData[index1]=st;
}

void CBurstHandling::exchange(bool *pbData, int index0, int index1)
{
	bool bt=pbData[index0];
	pbData[index0]=pbData[index1];
	pbData[index1]=bt;
}

void CBurstHandling::adjustEventStarts(int index0, int index1)
{
	int start=m_pnLevelStart[index0];
	for(int i=index0;i<=index1;i++){
		m_pnLevelStart[i]=start;
		start+=m_pnLevelLength[i];
	}
}


void CBurstHandling::ExportBurstcorrelations()
{
	if(!m_bBurstNode) MakeBurstNodes();
	bool bDataFileAvailable=false;
	int nLevels;
	if(bDataFileAvailable){
		if(!m_bLevelAPHBuilt) buildConductanceLevels();
		nLevels=m_cLAHHandler.getNumPeaks();
	}
	buildBurstCorrelationHists();
	CString sFileName=m_sEvlFileName;
	CBurstingTimeCourseOptionDlg aDlg;
	aDlg.SetParameters(m_structBurstingTimeCourseOptionNode);
	CFileNameHandling cfh;
	sFileName=cfh.ExtendFileName(sFileName,aDlg.getBurstEvolutionParametersAsString());
	FILE* fp=fopen(ChangeFileExt(sFileName,"Bcr"),"w");

	int nDimT=m_cBurstDurationIndexes.getDim();
	int nDimN=m_cNOPBIndexes.getDim();
	structMeanSemNode ToNodeT, TcNodeT, AmpNodeT;
	structMeanSemNode ToNodeN, TcNodeN, AmpNodeN;
	double dMBTo;
	int i,j,nSize,nNOPB;
	vector <int> vnIndexes;
	fprintf(fp,"       MBTo         MTo       ToSem    Num");
	fprintf(fp,"         MTc       TcSem    Num");
	fprintf(fp,"     AmpMean      AmpSem    Num");

	fprintf(fp,"   NOPB         MTo       ToSem    Num");
	fprintf(fp,"         MTc       TcSem    Num");
	fprintf(fp,"     AmpMean      AmpSem    Num");
	fprintf(fp,"\n");
	int nDim=nDimT;
	if(nDimN>nDim) nDim=nDimN;
	for(i=-1;i<nDim;i++){
		dMBTo=m_cBurstDurationIndexes.getValue(i);
		vnIndexes=m_cBurstDurationIndexes.getIndexes(i);
		nSize=vnIndexes.size();
		TcNodeT=getDwellTimeMS(vnIndexes,0);
		ToNodeT=getDwellTimeMS(vnIndexes,1);
		AmpNodeT=getAmpMS(vnIndexes);
		fprintf(fp," %10.1f  %10.3f  %10.3f %6d",dMBTo,ToNodeT.dMean,ToNodeT.dSem,ToNodeT.nSize);  
		fprintf(fp,"  %10.3f  %10.3f %6d",TcNodeT.dMean,TcNodeT.dSem,TcNodeT.nSize);  
		fprintf(fp,"  %10.3f  %10.3f %6d",fabs(AmpNodeT.dMean),AmpNodeT.dSem,AmpNodeT.nSize); 
		
		nNOPB=m_cNOPBIndexes.getValue(i);

		vnIndexes=m_cNOPBIndexes.getIndexes(i);
		nNOPB=int (m_cNOPBIndexes.getValue(i)+0.5);
		nSize=vnIndexes.size();
		TcNodeN=getDwellTimeMS(vnIndexes,0);
		ToNodeN=getDwellTimeMS(vnIndexes,1);
		AmpNodeN=getAmpMS(vnIndexes);
		fprintf(fp," %6d  %10.3f  %10.3f %6d",nNOPB,ToNodeN.dMean,ToNodeN.dSem,ToNodeN.nSize);  
		fprintf(fp,"  %10.3f  %10.3f %6d",TcNodeN.dMean,TcNodeN.dSem,TcNodeN.nSize);  
		fprintf(fp,"  %10.3f  %10.3f %6d",fabs(AmpNodeN.dMean),AmpNodeN.dSem,AmpNodeN.nSize); 
		fprintf(fp,"\n");
	}
	fclose(fp);
	ExportBurstcorrelations_scattered();
}

void CBurstHandling::buildBurstCorrelationHists()
{
	double dLogMin=1.,dDelLog=0.2;
	double dMinT=0.01,dDelT=20.;
	int nMin=0,nDelta=1;
	int nDim1=30,nDim2=150;
	m_cBurstDurationIndexes.update(1,nDim1,dLogMin,dDelLog);
	m_cNOPBIndexes.update(0,nDim2,nMin,nDelta);
	m_cBurstDurationIndexes_b.update(1,nDim1,dLogMin,dDelLog);
	m_cNOPBIndexes_b.update(0,nDim2,nMin,nDelta);

	int i,j;
	int nSize=m_structvBurstNodes.size();
	double dBTo;
	structBurstNode aNode;
	int nOpenings,nClosings,nNumOpenings;
	int nFirstOpening,nLastOpening;
	for(i=0;i<nSize;i++)
	{
		aNode=m_structvBurstNodes[i];
		if(!aNode.bValid) continue;
		nNumOpenings=aNode.nNumOpenings;
		dBTo=aNode.fLength;
		nFirstOpening=aNode.nFirstOpening;
		nLastOpening=aNode.nLastOpening;
		m_cBurstDurationIndexes_b.addIndex(dBTo,i);
		m_cNOPBIndexes_b.addIndex(nNumOpenings,i);
		for(j=nFirstOpening;j<=nLastOpening;j++){
			m_cBurstDurationIndexes.addIndex(dBTo,j);
			m_cNOPBIndexes.addIndex(nNumOpenings,j);
		}
	}
	int wst=1;
	m_bBurstCorrelationHist=true;
	m_cBurstDurationIndexes.smoothHistogram(-1,nDim1-wst-1,wst);
	m_cNOPBIndexes.smoothHistogram(-1,nDim1-wst-1,wst);
}

void CBurstHandling::ExportBurstcorrelations_scattered()
{
	if(!m_bBurstNode) MakeBurstNodes();
	bool bDataFileAvailable=false;
	int nLevels;
	if(bDataFileAvailable){
		if(!m_bLevelAPHBuilt) buildConductanceLevels();
		nLevels=m_cLAHHandler.getNumPeaks();
	}
	if(!m_bBurstCorrelationHist)buildBurstCorrelationHists();
	CString sFileName=m_sEvlFileName;
	CBurstingTimeCourseOptionDlg aDlg;
	aDlg.SetParameters(m_structBurstingTimeCourseOptionNode);
	CFileNameHandling cfh;
	sFileName=cfh.ExtendFileName(sFileName,aDlg.getBurstEvolutionParametersAsString());
	FILE* fp=fopen(ChangeFileExt(sFileName,"Bsk"),"w");

	int nDimT=m_cBurstDurationIndexes.getDim();
	int nDimN=m_cNOPBIndexes.getDim();
	structMeanSemNode ToNodeT, TcNodeT, AmpNodeT;
	structMeanSemNode ToNodeN, TcNodeN, AmpNodeN;
	double dMBTo;
	int i,j,nSize,nNOPB;
	vector <int> vnIndexes,vnIndexes_b;
	fprintf(fp,"      dMBTo       MBTo    NOPB         MTo       ToSem    Num");
	fprintf(fp,"         MTc       TcSem    Num");
	fprintf(fp,"     AmpMean      AmpSem    Num");
/*
	fprintf(fp,"   NOPB         MTo       ToSem    Num");
	fprintf(fp,"         MTc       TcSem    Num");
	fprintf(fp,"     AmpMean      AmpSem    Num");*/
	fprintf(fp,"\n");
	int nDim=nDimT;
	if(nDimN>nDim) nDim=nDimN;
	int index;
	structBurstNode aNode;
	
	for(i=-1;i<nDim;i++){
		dMBTo=m_cBurstDurationIndexes.getValue(i);
		vnIndexes_b=m_cBurstDurationIndexes_b.getIndexes(i);
		nSize=vnIndexes_b.size();
		for(j=0;j<nSize;j++){
			index=vnIndexes_b[j];
			vnIndexes=getEventIndexes(index);
			TcNodeT=getDwellTimeMS(vnIndexes,0);
			ToNodeT=getDwellTimeMS(vnIndexes,1);
			AmpNodeT=getAmpMS(vnIndexes);
			fprintf(fp," %10.1f %10.1f  %6d  %10.3f  %10.3f %6d",dMBTo,m_structvBurstNodes[index].fLength,m_structvBurstNodes[index].nNumOpenings,ToNodeT.dMean,ToNodeT.dSem,ToNodeT.nSize);  
			fprintf(fp,"  %10.3f  %10.3f %6d",TcNodeT.dMean,TcNodeT.dSem,TcNodeT.nSize);  
			fprintf(fp,"  %10.3f  %10.3f %6d",fabs(AmpNodeT.dMean),AmpNodeT.dSem,AmpNodeT.nSize); 
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
}

vector <int> CBurstHandling::getEventIndexes(int burstIndex){
	vector <int> vnIndexes;
	int nFirstOpening,nLastOpening;
	structBurstNode aNode=m_structvBurstNodes[burstIndex];
	nFirstOpening=aNode.nFirstOpening;
	nLastOpening=aNode.nLastOpening;
	for(int index=nFirstOpening;index<=nLastOpening;index++){
		vnIndexes.push_back(index);
	}
	return vnIndexes;
}


structBurstNodes CBurstHandling::getBurstNodes()
{
	structBurstNodes aNode;
	aNode.dTcb=m_structBurstingTimeCourseOptionNode.fBurstDelimiter;
	aNode.burstNodes=m_structvBurstNodes;
	aNode.nBurstID=m_nBurstID;
	aNode.nEvlID=m_nEvlID;
	return aNode;
}

structClusterNodes CBurstHandling::getClusterNodes()
{
	structClusterNodes aNode;
	aNode.dTcc=m_structBurstingTimeCourseOptionNode.fClusterDelimiter;
	aNode.clusterNodes=m_structvClusterNodes;
	return aNode;
}
void CBurstHandling::MakeBurstNodes(double dTcb)
{
	m_structBurstingTimeCourseOptionNode.bSet=true;
	m_structBurstingTimeCourseOptionNode.fBurstDelimiter=dTcb;
	MakeBurstNodes();
}

void CBurstHandling::MakeClusterNodes()
{
	MakeBurstNodes();//this function also make cluster nodes
}

void CBurstHandling::MakeClusterNodes(double dTcb, double dTcc)
{
	m_structBurstingTimeCourseOptionNode.bSet=true;
	m_structBurstingTimeCourseOptionNode.fBurstDelimiter=dTcb;
	m_structBurstingTimeCourseOptionNode.fClusterDelimiter=dTcc;
	MakeBurstNodes();//this function also make cluster nodes when dTcc is greater than dTcb
}

void CBurstHandling::updateID()
{
	m_nBurstID=m_nSN;
	m_nSN++;
}

int CBurstHandling::getBurstID()
{
	return m_nBurstID;
}

int CBurstHandling::getClusterID()
{
	return m_nBurstID;
}
void CBurstHandling::excludeBursts(vector <int> vnIndexes){
	int nSize=vnIndexes.size();
	int index;
	for(int i=0;i<nSize;i++){
		index=vnIndexes[i];
		m_structvBurstNodes[index].bValid=false;
	}
	updateID();
}
void CBurstHandling::excludeClusters(vector <int> vnIndexes){
	int nSize=vnIndexes.size();
	int index;
	for(int i=0;i<nSize;i++){
		index=vnIndexes[i];
		m_structvClusterNodes[index].bValid=false;
	}
	updateID();
}