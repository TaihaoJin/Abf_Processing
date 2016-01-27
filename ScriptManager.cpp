// ScriptManager.cpp: implementation of the CScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Abf_Processing.h"
#include "ScriptManager.h"
#include "HouseValues.h"
#include "PrintAssist.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptManager::CScriptManager()
{
	m_bAbf=false;
	m_bEvl=false;
	m_bEndScript=false;
	m_dTcb=-1.;
	m_dTcc=-1.;
	m_structAbfNode.nAbfID=-1;
	m_structAbfNode.nSize=-1;
}


CScriptManager::~CScriptManager()
{

}

CString CScriptManager::getScriptFileName()
{
	CString sFileName;
	char* pch="Abf Script File (*.srp)\0*.srp\0All Files\0*.*\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="input a script file";
//	strcpy(aFileDlg.m_ofn.lpstrFile,m_sFileName);

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
	}else{
		sFileName="No Script File Input";
	}
	return sFileName;
}

int CScriptManager::runScript(FILE* fpScript)
{
	//TODO: implement skiping remarked strings
	CString sTemp;
	m_bEndScript=false;
	fscanf(fpScript,"%s",sTemp);
	int nStatus;
	while(!scriptEnd(sTemp)){
		if(sTemp=="RemarkBegin") skipUntil(fpScript,"RemarkEnd");
		while(!(sTemp=="Commond")){
			fscanf(fpScript,"%s",sTemp);
			if(sTemp=="RemarkBegin") skipUntil(fpScript,"RemarkEnd");
			if(scriptEnd(sTemp)) return -1;
		}
		nStatus=runCommond(fpScript);
		if(nStatus==-1) break;
	}
	return nStatus;
}

int CScriptManager::skipUntil(FILE* fp, CString st0){
	CString st;
	fscanf(fp,"%s",st);
	while(st!=st0){
		fscanf(fp,"%s",st);
		if(feof(fp)) return -1;
	}
	return 1;
}


int CScriptManager::runCommond(FILE *fpScript)
{
	CString sCommond;
	fscanf(fpScript,"%s",sCommond);
	if(scriptEnd(sCommond)) return -1;
	return runCommond(fpScript,sCommond);
}
void CScriptManager::runScripts() 
{
	// TODO: Add your command handler code here
	CString sFileName=getScriptFileName();
	if(!(sFileName=="No Script File Input")){
		FILE* fp=fopen(sFileName,"r");
		runScript(fp);
	}
}

int CScriptManager::importAbfFile(CString sFileName)
{
	FILE* fp;
	int nStatus=m_cAbf.ReadData(sFileName);
	if(nStatus<0){
		sFileName=getAbfFileName("--can not open the file in the script file.");
		while(!(fp=fopen(sFileName,"rb"))){
			sFileName=getAbfFileName("--problems with opening the abf data file.");
			if(sFileName=="Cancel"){
				nStatus=-1;
				break;
			}
		}
		if(nStatus!=-1){
			m_cAbf.ReadData(fp);
			fclose(fp);
		}
	}
	if(nStatus==1) {
		m_bAbf=true;
		m_structAbfNode=m_cAbf.GetTrace();
	}
	return nStatus;
}

CString CScriptManager::getAbfFileName(CString sAdditionalTitle)
{
	CString sFileName;
	char* pch="Abf File (*.abf)\0*.abf\0Dat File\0*.dat\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	CString sTitle="input a file";
	sTitle+=sAdditionalTitle;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
	}else{
		sFileName="Cancel";
	}
	return sFileName;
}

CString CScriptManager::getEvlFileName(CString sAdditionalTitle)
{
	CString sFileName;
	char* pch="Evl File (*.evl)\0*.evl\0All File\0*.*\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	CString sTitle="input an evl file";
	sTitle+=sAdditionalTitle;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
	}else{
		sFileName="Cancel";
	}
	return sFileName;
}


bool CScriptManager::scriptEnd(CString sT)
{
	bool bEnd=false;
	if(sT=="End"||sT=="END"||sT=="end"){
		bEnd=true;
		m_bEndScript=true;
	}
	return bEnd;
}

bool CScriptManager::endScript()
{
	return m_bEndScript;
}

int CScriptManager::importEvlFile(CString sFileName)
{
	FILE* fp;
	int nStatus=m_cEvl.ReadEvl(sFileName);
	if(nStatus<0){
		sFileName=getEvlFileName("--can not open the file in the script file.");
		while(!(fp=fopen(sFileName,"rb"))){
			sFileName=getEvlFileName("--problems with opening the abf data file.");
			if(sFileName=="Cancel"){
				nStatus=-1;
				m_bEndScript=true;
				break;
			}
		}
		if(nStatus!=-1){
			m_cEvl.ReadEvl(fp);
			fclose(fp);
		}
	}
	if(nStatus==1) m_bEvl=true;
	return nStatus;
}

int CScriptManager::buildSubsets(FILE* fpScrip)
{
//	CIncidenceSubsetHandler ish;
	vector <structIncidenceSubsetCriteriaNode> svISCNodes;
	if(!m_bEvl) importEvlFile("spaceFiller");
	m_cEvl.updateIncidenceSubsetHandler(m_cSubsetHandler);
	m_cSubsetHandler.updateBursts(m_structBurstNodes);
	m_cSubsetHandler.updateClusters(m_structClusterNodes);
	CPrintAssist cpa;
	CString sFileName=cpa.readString(fpScrip);
	if(scriptEnd(sFileName)) return -1;

	if(sFileName=="AllEvents"){
		svISCNodes=m_cSubsetHandler.getISC_AllEvents();
	}else if(sFileName=="AllBursts"){
		svISCNodes=m_cSubsetHandler.getISC_AllBursts();
	}else if(sFileName=="AllClusters"){
		svISCNodes=m_cSubsetHandler.getISC_AllClusters();
	}else {
		bool bIncludedInputFile=false;
		if(sFileName=="IncludingInputfile") bIncludedInputFile=true;
		FILE* fp1;
		if(bIncludedInputFile){
			fp1=fpScrip;
		}else{
			fp1=fopen(sFileName,"r");
		}
		svISCNodes=m_cSubsetHandler.readIncidenceSubsetCriteriaFile(fp1);
		if(!bIncludedInputFile) fclose(fp1);
	}

	m_cSubsetHandler.loadIncidenceSubsetCriteriaNodes(svISCNodes);
	m_cSubsetHandler.buildSubsets();
	m_structSubsets=m_cSubsetHandler.getSebsets();
	return 1;
}

void CScriptManager::appendSubets(FILE* fp)
{
//	CIncidenceSubsetHandler ish;
	bool bIncludedInputFile=false;
	CPrintAssist cpa;
	CString sFileName=cpa.readString(fp);
	if(sFileName=="IncludingInputfile") bIncludedInputFile=true;
//	m_cSubsetHandler.appendSubsets(sFileName);
	FILE* fp1;
	if(bIncludedInputFile){
		fp1=fp;
	}else{
		fp1=fopen(sFileName,"r");
	}
	m_cSubsetHandler.appendSubsets(m_cSubsetHandler.readIncidenceSubsetCriteriaFile(fp));
	m_structSubsets=m_cSubsetHandler.getSebsets();
	if(!bIncludedInputFile) fclose(fp1);
}
int CScriptManager::setRisetime (float fRisetime) {
	m_cSubsetHandler.setRisetime(fRisetime);
	return 1;
}
void CScriptManager::InitializeSubsetHandler(){
	if(!m_bEvl) importEvlFile("spaceFiller");
	m_cEvl.updateIncidenceSubsetHandler(m_cSubsetHandler);
	m_cSubsetHandler.updateBursts(m_structBurstNodes);
	m_cSubsetHandler.updateClusters(m_structClusterNodes);
	m_cSubsetHandler.clearSubsets();
}

void CScriptManager::setDelimiters(double Tcb, double Tcc)
{
	m_dTcb=Tcb;
	m_dTcc=Tcc;
}

bool CScriptManager::sameDelimiter(double dTc1, double dTc2)
{
	if(!m_bEvl) return false;
	return (int(dTc1-dTc2)*1000./m_cEvl.getTimePerPoint());
}

void CScriptManager::buildBurstNodes()
{
	CBurstHandling aBH;
	m_cEvl.updateBurstHandler(aBH);
	if(m_dTcc<m_dTcb){
		aBH.MakeBurstNodes(m_dTcb);
		m_structBurstNodes=aBH.getBurstNodes();
	}else{
		aBH.MakeClusterNodes(m_dTcb,m_dTcc);
		m_structBurstNodes=aBH.getBurstNodes();
		m_structClusterNodes=aBH.getClusterNodes();
	}
}

bool CScriptManager::matchingEvlSubset(CEvl &aEvl, CIncidenceSubsetHandler &ish)
{
	int nID1=aEvl.getEvlID();
	int nID2=ish.getEvlID();
	bool bMatching=(nID1==nID2);
	return bMatching;
}
bool CScriptManager::matchingBurstSubset(CBurstHandling &aBH, CIncidenceSubsetHandler &ish)
{
	return(aBH.getBurstID()==ish.getBurstID());
}
bool CScriptManager::matchingClusterSubset(CBurstHandling &aBH, CIncidenceSubsetHandler &ish)
{
	return(aBH.getBurstID()==ish.getClusterID());
}int CScriptManager::runCommond(FILE* fpScript, CString sCommond)
{
	CString sTemp;
	int nStatus=1;
	int nSize;
	CPrintAssist cpa;
	if(sCommond=="importAbf"){
		CString sAbfFileName=cpa.readString(fpScript);
//		fscanf(fpScript,"%s",sFileName);
		if(scriptEnd(sAbfFileName)) return -1;
		nStatus=importAbfFile(sAbfFileName);
	}
	if(sCommond=="importEvl"){
		CString sEvlFileName=cpa.readString(fpScript);
//		fscanf(fpScript,"%s",sFileName);
		if(scriptEnd(sEvlFileName)) return -1;
		nStatus=importEvlFile(sEvlFileName);
	}
	if(sCommond=="buildBurstNodes"){
		float fTcb, fTcc;
		fscanf(fpScript,"%s%f%s%f",sTemp,&fTcb,sTemp,&fTcc);
		setDelimiters(fTcb,fTcc);
		buildBurstNodes();
	}
	if(sCommond=="buildSubsets"){
		return buildSubsets(fpScript);
	}
	if(sCommond=="filteringEvl_Subset"){
		if(matchingEvlSubset(m_cEvl,m_cSubsetHandler)){
			vector <int> vnIndexes=m_cSubsetHandler.getFinalIndexes(0);
			nSize=m_cSubsetHandler.CompleteSetSize(0);
			vnIndexes=m_cSetHandler.complement(vnIndexes,nSize);
			m_cEvl.excludeEvents(vnIndexes);
		}
	}
	if(sCommond=="filteringBursts_Subset"){
		if(matchingBurstSubset(m_cBH,m_cSubsetHandler)){
			vector <int> vnIndexes=m_cSubsetHandler.getFinalIndexes(1);
			nSize=m_cSubsetHandler.CompleteSetSize(0);
			vnIndexes=m_cSetHandler.complement(vnIndexes,nSize);
			m_cBH.excludeBursts(vnIndexes);
		}
	}
	if(sCommond=="filteringClusters_Subset"){
		if(matchingClusterSubset(m_cBH,m_cSubsetHandler)){
			vector <int> vnIndexes=m_cSubsetHandler.getFinalIndexes(2);
			nSize=m_cSubsetHandler.CompleteSetSize(0);
			vnIndexes=m_cSetHandler.complement(vnIndexes,nSize);
			m_cBH.excludeClusters(vnIndexes);
		}
	}
	if(sCommond=="BurstLatancyRange_N_APH"){
		nStatus=exportAph_BurstLatancyRangeN(fpScript);
	}
	if(sCommond=="BurstLatancyRange_T_APH"){
		nStatus=exportAph_BurstLatancyRangeT(fpScript);
	}
	if(sCommond=="HouseValue"){
		CString sInputFileName=cpa.readString(fpScript);
//		fscanf(fpScript,"%s",sInputFileName);
		CHouseValues chv;
		chv.ComputeHouseValue(sInputFileName);
	}
	if(sCommond=="testPrintAssist"){
		testPrintAssist();
	}
	if(sCommond=="exportSubsetsAsTrace"){
		exportSubsetsAsTrace(fpScript);
	}
	if(sCommond=="InitializeSubsetHandler"){
		InitializeSubsetHandler();
	}
	if(sCommond=="exportAPH_Subset"){
		int nWS,nIT;
		CString sTemp;
		float fRatio0;
 		fscanf(fpScript,"%s%d%s%d%s%f",sTemp,&nWS,sTemp,&nIT,sTemp,&fRatio0);
		exportAPH_Subset(nWS,nIT,fRatio0);
	}
	if(sCommond=="clearSubsets"){
		m_cSubsetHandler.clearSubsets();
	}
	if(sCommond=="appendSubsets"){
		appendSubets(fpScript);
	}
	if(sCommond=="exportDwellTimeHistograms"){
		exportDwellTimeHistograms_Subset();
	}	
	if(sCommond=="adjustAmp_Evl"){
		adjustAmp_Evl();
	}	
	if(sCommond=="adjustAmp_Abf"){
		adjustAmp_Abf();
	}	
	if(sCommond=="exportEvl"){
		exportEvl(fpScript);
	}	
	if(sCommond=="exportAbf"){
		exportAbf(fpScript);
	}	
	return nStatus;
}

int CScriptManager::exportAph_BurstLatancyRangeN(FILE *fpScript)
{
	m_cEvl.setAbfNode(m_cAbf.GetTrace());
	int n1,n2,nd,nSeg,nHalf;//nHalf: 1 including the first half of the bursts, -1 the second half, 0 all bursts
	CString sFileName,sTemp;
	fscanf(fpScript,"%s%d%s%d%s%d%s%d%s%s",sTemp,&n1,sTemp,&n2,sTemp,&nd,sTemp,&nHalf,sTemp,sFileName.GetBuffer(300));
	sFileName.ReleaseBuffer();
	nSeg=(n2-n1)/nd+1;
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,m_cSubsetHandler.getISCAsString());	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_n1"+m_cFileNameHandler.ToString(n1));	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_n2"+m_cFileNameHandler.ToString(n2));	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_dn"+m_cFileNameHandler.ToString(nd));	
	FILE* fp=fopen(sFileName,"w");
	int i,j;
	CIncidenceSubsetHandler ish;
	structIncidenceSubsetCriteriaNode sISCNode;
	structIncidenceCriterionNode sICNode0,sICNode1;
	//construct the nodes.
	sISCNode.nApplyingIndex=0;
	sISCNode.nIncidenceType=0;
	sISCNode.nOperationType=1;
	sISCNode.vstructIncidenceCriteria.push_back(sICNode0);
//	sISCNode.vstructIncidenceCriteria.push_back(sICNode1);
	int n0;
	vector <CLinearHistogramDouble> hists;

	vector <int> vnFirstHalf,vnSecondHalf,vnEntireBurst,vnIndexes0,vnIndexes1,vnIndexes;

	vector <int> vnEventIndexes=m_cSubsetHandler.getFinalIndexes(0);

	sICNode0.dV1=1;
	sICNode0.dV2=2;
	sICNode0.nRelatingIndex=0;
	sICNode0.sCriterionID="HarlfBurstN";
	sICNode0.nCriterionID=m_cSubsetHandler.getIncidenceSubsetCriterionID(sICNode0.sCriterionID);
	sISCNode.vstructIncidenceCriteria[0]=sICNode0;

	sICNode1.nRelatingIndex=0;
	sICNode1.sCriterionID="latancyRangeN(openings)";
	sICNode1.nCriterionID=m_cSubsetHandler.getIncidenceSubsetCriterionID(sICNode1.sCriterionID);

	structIncidenceSubsetNode subset=m_cSubsetHandler.buildSubset(sISCNode);
	vnFirstHalf=m_cSubsetHandler.applySubset(vnEventIndexes,0,subset);

	sICNode0.dV1=-1;
	sISCNode.vstructIncidenceCriteria[0]=sICNode0;
	subset=m_cSubsetHandler.buildSubset(sISCNode);
	vnSecondHalf=m_cSubsetHandler.applySubset(vnEventIndexes,0,subset);

	int nSetSize=m_cSubsetHandler.CompleteSetSize(0);

	CIndexSetHandler cISH;

	double dMinAmp, dMaxAmp, dDelta=0.1;
	CPrintAssist cpa;
	m_cSubsetHandler.addjustTraceBaseline(m_structAbfNode);
	getAmpRange(dMinAmp, dMaxAmp, m_structAbfNode);
	
	for(n0=n1;n0<=n2;n0+=nd){
		sICNode1.dV1=n0;
		sICNode1.dV2=n0+nd;
		sISCNode.vstructIncidenceCriteria[0]=sICNode1;
		subset=m_cSubsetHandler.buildSubset(sISCNode);
		vnIndexes0=m_cSubsetHandler.applySubset(vnFirstHalf,0,subset);

		sICNode1.dV1*=-1;
		sICNode1.dV2*=-1;
		sISCNode.vstructIncidenceCriteria[0]=sICNode1;
		subset=m_cSubsetHandler.buildSubset(sISCNode);
		vnIndexes1=m_cSubsetHandler.applySubset(vnSecondHalf,0,subset);

		vnIndexes=cISH.setUnion(vnIndexes0,vnIndexes1,nSetSize);

		if(nHalf>0){
			CLinearHistogramDouble hist0;
			hist0=m_cEvl.obtainEventAPH(vnIndexes0,m_structAbfNode,dMinAmp,dMaxAmp);
			hist0.setTitles("Amp(pA)","Latancy:"+cpa.ToString(n0)+"To"+cpa.ToString(n0+nd-1));
			hists.push_back(hist0);
		}else if(nHalf<0){
			CLinearHistogramDouble hist1;
			hist1=m_cEvl.obtainEventAPH(vnIndexes1,m_structAbfNode,dMinAmp,dMaxAmp);
			hist1.setTitles("Amp(pA)","Latancy:"+cpa.ToString(-n0)+"To"+cpa.ToString(-n0-nd+1));
			hists.push_back(hist1);
		}else{
			CLinearHistogramDouble hist0;
			hist0=m_cEvl.obtainEventAPH(vnIndexes0,m_structAbfNode,dMinAmp,dMaxAmp);
			hist0.setTitles("Amp(pA)","Latancy:"+cpa.ToString(n0)+"To"+cpa.ToString(n0+nd-1));
			hists.push_back(hist0);
			CLinearHistogramDouble hist1;
			hist1=m_cEvl.obtainEventAPH(vnIndexes1,m_structAbfNode,dMinAmp,dMaxAmp);
			hist1.setTitles("Amp(pA)","Latancy:"+cpa.ToString(-n0)+"To"+cpa.ToString(-n0-nd+1));
			hists.push_back(hist1);
		}
	}
	CLinearHistogramHandler clhh;
	int nWS=2;
	clhh.exportHistograms(fp,hists,nWS);
	fclose(fp);
	return 1;
}
void CScriptManager::testPrintAssist(){
	CPrintAssist cpa;
	FILE* fp=fopen("testPrint.txt","w");
	CString st1="string10",st2="integer12",st3="float12.2",st4="double23.3",st5="scientific";
	cpa.printString(fp,st1,10);
	cpa.printString(fp,st2,12);
	cpa.printString(fp,st3,12);
	cpa.printString(fp,st4,23);
	cpa.printString(fp,st5,15);
	cpa.endLine(fp);
	CString st="test";
	int nt=2334;
	float ft=12.88687;
	double dt=32123.43533;
	double dt1=0.012345,dt2=124578943234578.1256;
	cpa.printString(fp,st,10);
	cpa.printNumber(fp,nt,12,0);
	cpa.printNumber(fp,ft,12,2);
//	cpa.printNumber(fp,dt,13,3);
//	cpa.printNumber(fp,dt1,13,3);
	cpa.printNumber(fp,dt2,23,3);
	cpa.printNumberScientific(fp,dt,4,15);
	cpa.endLine(fp);
	cpa.printString(fp,st,10);
	cpa.printNumber(fp,-nt,12,0);
	cpa.printNumber(fp,-ft,12,4);
//	cpa.printNumber(fp,-dt,13,2);
//	cpa.printNumber(fp,-dt1,13,2);
	cpa.printNumber(fp,-dt2,23,2);
	cpa.printNumberScientific(fp,-dt,4,15);
	cpa.endLine(fp);
	fclose(fp);
}

int CScriptManager::exportSubsetsAsTrace(FILE* fp){
	int nStatus=1;
	CPrintAssist cpa;
	CFileNameHandling fnh;
	CString sTemp=cpa.readString(fp), sFileName=cpa.readString(fp);
	CAbf cAbf;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat)\0*.dat\0Abf Files\0*.abf\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="import a dat file for the header section";
	if(!fnh.validFileToRead(sFileName,"dat")&&!fnh.validFileToRead(sFileName,"abf")){
		if(m_bAbf){
			sFileName=m_cAbf.getAbfFileName();
		}
	}
	while(!fnh.validFileToRead(sFileName,"dat")&&!fnh.validFileToRead(sFileName,"abf")){
		if(aFileDlg.DoModal()==IDOK)
		{
			sFileName=aFileDlg.GetFileName();
		}else{
			break;
			nStatus=-1;
			return nStatus;
		}
	}
	cAbf.ReadData(sFileName);
	AbfNode aNode=m_cSubsetHandler.getSubsetsAsTrace(0,m_structAbfNode);
	int nSize=aNode.nSize;
	cAbf.UpdateSize(nSize);
	cAbf.UpdateData(aNode.pfData);

	sFileName=m_cSubsetHandler.getEvlFileName();
	CString st=m_cSubsetHandler.getISCAsString();
	char c='0';
	sFileName=fnh.ExtendFileName(sFileName,st);
	sFileName=fnh.ExtendFileName(sFileName,'_');
	sFileName=fnh.ExtendFileName(sFileName,c);
	sFileName=fnh.ChangeFileExt(sFileName,"Dat");
	int index;
	while(fnh.FileExists(sFileName)){
		c++;
		index=sFileName.GetLength()-5;
		sFileName.SetAt(index,c);
	}
	FILE* fpOut=fopen(sFileName,"wb");
	cAbf.WriteData(fpOut);
	fclose(fpOut);
	return nStatus;
}

void CScriptManager::getAmpRange(double &dMinAmp, double &dMaxAmp, AbfNode &aNode){
	int i,nSize;
	float fAmp;
	nSize=aNode.nSize;
	float *pfData=aNode.pfData;
	dMinAmp=pfData[0];
	dMaxAmp=pfData[0];
	for(i=1;i<nSize;i++){
		fAmp=pfData[i];
		if(fAmp>dMaxAmp) dMaxAmp=fAmp;
		if(fAmp<dMinAmp) dMinAmp=fAmp;
	}
}

int CScriptManager::exportAph_BurstLatancyRangeT(FILE *fpScript)
{
	m_cEvl.setAbfNode(m_cAbf.GetTrace());
	int nSeg,nHalf;//nHalf: 1 including the first half of the bursts, -1 the second half, 0 all bursts
	float fT1,fT2,fDelta;
	CString sFileName,sTemp;
	fscanf(fpScript,"%s%f%s%f%s%f%s%d%s%s",sTemp,&fT1,sTemp,&fT2,sTemp,&fDelta,sTemp,&nHalf,sTemp,sFileName.GetBuffer(300));
	sFileName.ReleaseBuffer();
	nSeg=int((fT2-fT1)/fDelta)+1;
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,m_cSubsetHandler.getISCAsString());	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_T1"+m_cFileNameHandler.ToString(fT1,1));	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_T2"+m_cFileNameHandler.ToString(fT2,1));	
	sFileName=m_cFileNameHandler.ExtendFileName(sFileName,"_dT"+m_cFileNameHandler.ToString(fDelta,1));	
	FILE* fp=fopen(sFileName,"w");
	int i,j;
	CIncidenceSubsetHandler ish;
	structIncidenceSubsetCriteriaNode sISCNode;
	structIncidenceCriterionNode sICNode0,sICNode1;
	//construct the nodes.
	sISCNode.nApplyingIndex=0;
	sISCNode.nIncidenceType=0;
	sISCNode.nOperationType=1;
	sISCNode.vstructIncidenceCriteria.push_back(sICNode0);
//	sISCNode.vstructIncidenceCriteria.push_back(sICNode1);
	int n0;
	vector <CLinearHistogramDouble> hists;

	vector <int> vnFirstHalf,vnSecondHalf,vnEntireBurst,vnIndexes0,vnIndexes1,vnIndexes;

	vector <int> vnEventIndexes=m_cSubsetHandler.getFinalIndexes(0);

	sICNode0.dV1=1;
	sICNode0.dV2=2;
	sICNode0.nRelatingIndex=0;
	sICNode0.sCriterionID="HalfBurstN";
	sICNode0.nCriterionID=m_cSubsetHandler.getIncidenceSubsetCriterionID(sICNode0.sCriterionID);
	sISCNode.vstructIncidenceCriteria[0]=sICNode0;

	sICNode1.nRelatingIndex=0;
	sICNode1.sCriterionID="latancyRangeT(ms)";
	sICNode1.nCriterionID=m_cSubsetHandler.getIncidenceSubsetCriterionID(sICNode1.sCriterionID);

	structIncidenceSubsetNode subset=m_cSubsetHandler.buildSubset(sISCNode);
	vnFirstHalf=m_cSubsetHandler.applySubset(vnEventIndexes,0,subset);

	sICNode0.dV1=-1;
	sISCNode.vstructIncidenceCriteria[0]=sICNode0;
	subset=m_cSubsetHandler.buildSubset(sISCNode);
	vnSecondHalf=m_cSubsetHandler.applySubset(vnEventIndexes,0,subset);

	int nSetSize=m_cSubsetHandler.CompleteSetSize(0);

	CIndexSetHandler cISH;

	double dMinAmp, dMaxAmp, dDelta=0.1;
	CPrintAssist cpa;
	m_cSubsetHandler.addjustTraceBaseline(m_structAbfNode);
	getAmpRange(dMinAmp, dMaxAmp, m_structAbfNode);
	float fT;
	
	for(fT=fT1;fT<=fT2;fT+=fDelta){
		sICNode1.dV1=fT;
		sICNode1.dV2=fT+fDelta;
		sISCNode.vstructIncidenceCriteria[0]=sICNode1;
		subset=m_cSubsetHandler.buildSubset(sISCNode);
		vnIndexes0=m_cSubsetHandler.applySubset(vnFirstHalf,0,subset);

		sICNode1.dV1*=-1;
		sICNode1.dV2*=-1;
		sISCNode.vstructIncidenceCriteria[0]=sICNode1;
		subset=m_cSubsetHandler.buildSubset(sISCNode);
		vnIndexes1=m_cSubsetHandler.applySubset(vnSecondHalf,0,subset);

		vnIndexes=cISH.setUnion(vnIndexes0,vnIndexes1,nSetSize);

		if(nHalf>0){
			CLinearHistogramDouble hist0;
			hist0=m_cEvl.obtainEventAPH(vnIndexes0,m_structAbfNode,dMinAmp,dMaxAmp);
			hist0.setTitles("Amp(pA)","Latancy:"+cpa.ToString(fT,1)+"To"+cpa.ToString(fT+fDelta,1));
			hists.push_back(hist0);
		}else if(nHalf<0){
			CLinearHistogramDouble hist1;
			hist1=m_cEvl.obtainEventAPH(vnIndexes1,m_structAbfNode,dMinAmp,dMaxAmp);
			hist1.setTitles("Amp(pA)","Latancy:"+cpa.ToString(fT,1)+"To"+cpa.ToString(fT+fDelta,1));
			hists.push_back(hist1);
		}else{
			CLinearHistogramDouble hist0;
			hist0=m_cEvl.obtainEventAPH(vnIndexes0,m_structAbfNode,dMinAmp,dMaxAmp);
			hist0.setTitles("Amp(pA)","Latancy:"+cpa.ToString(fT,1)+"To"+cpa.ToString(fT+fDelta,1));
			hists.push_back(hist0);
			CLinearHistogramDouble hist1;
			hist1=m_cEvl.obtainEventAPH(vnIndexes1,m_structAbfNode,dMinAmp,dMaxAmp);
			hist1.setTitles("Amp(pA)","Latancy:"+cpa.ToString(fT,1)+"To"+cpa.ToString(fT+fDelta,1));
			hists.push_back(hist1);
		}
	}
	CLinearHistogramHandler clhh;
	int nWS=2;
	clhh.exportHistograms(fp,hists,nWS);
	fclose(fp);
	return 1;
}

int CScriptManager::exportAPH_Subset(int nWS, int nIT, double dRatio0){//The evl and abf file should have already imported
	if(!m_bAbf||!m_bEvl) return -1;
	m_cSubsetHandler.addjustTraceBaseline(m_structAbfNode);
	double dMinAmp, dMaxAmp;
	getAmpRange(dMinAmp,dMaxAmp,m_structAbfNode);
	m_cSubsetHandler.exportAPH_Subset(dMinAmp,dMaxAmp,m_structAbfNode, nWS, nIT, dRatio0);
	return 1;
}

int CScriptManager::exportDwellTimeHistograms_Subset(){
	m_cSubsetHandler.exportDwellTimeHistograms();
	return 1;
}

int	CScriptManager::exportAbf(FILE* fp){
	CPrintAssist cpa;
	CString sFileName=cpa.readString(fp);
	FILE* fpt=fopen(sFileName,"wb");
	m_cAbf.WriteData(fpt);
	fclose(fpt);
	return 1;
}
int	CScriptManager::exportEvl(FILE*fp){
	CPrintAssist cpa;
	CString sFileName=cpa.readString(fp);
	m_cEvl.WriteEvl(sFileName);
	return 1;
}
int	CScriptManager::adjustAmp_Abf(){
	AbfNode aNode=m_cAbf.GetTrace();
	m_cSubsetHandler.addjustTraceBaseline(aNode);
	int indexI=-1,indexF=1;
	m_cAbf.updateAmp(aNode,indexI,indexF);
	return 1;
}
int	CScriptManager::adjustAmp_Evl(){
	AbfNode aNode=m_cAbf.GetTrace();
	m_cSubsetHandler.addjustTraceBaseline(aNode);
	int indexI=-1,indexF=1;
	m_cSubsetHandler.addjustAmplitude(aNode);
	m_cEvl.updateAmp(m_cSubsetHandler.getAmplitude(),indexI,indexF);
	return 1;
}
