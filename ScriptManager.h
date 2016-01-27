// ScriptManager.h: interface for the CScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTMANAGER_H__1BE45951_3CC1_4FFE_B4E1_1CF32F055A4D__INCLUDED_)
#define AFX_SCRIPTMANAGER_H__1BE45951_3CC1_4FFE_B4E1_1CF32F055A4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Abf.h"
#include "Evl.h"
#include "structures.h"
#include "IncidenceSubsetHandler.h"
#include "FileNameHandling.h"
#include "IndexSetHandler.h"
#include <vector>
using namespace std;
class CScriptManager  
{
public:

	int CScriptManager::exportDwellTimeHistograms_Subset();
	void appendSubets(FILE* fp);
	void InitializeSubsetHandler();
	bool matchingClusterSubset(CBurstHandling &aBH, CIncidenceSubsetHandler &ish);
	bool matchingBurstSubset(CBurstHandling &aBH, CIncidenceSubsetHandler &ish);
	bool matchingEvlSubset(CEvl &aEvl, CIncidenceSubsetHandler &ish);
	int buildSubsets(FILE* fp);
	int importAbfFile(CString sFileName);
	CScriptManager();
	virtual ~CScriptManager();
	void runScripts();
	int  exportSubsetsAsTrace(FILE* fp);
	void getAmpRange(double &dMinAmp, double &dMaxAmp, AbfNode &aNode);
; 
protected:
	int CScriptManager::setRisetime (float fRisingTime);
	int CScriptManager::exportAPH_Subset(int nWS, int nIT, double dRatio0);
	int CScriptManager::exportAph_BurstLatancyRangeT(FILE *fpScript);
	int CScriptManager::skipUntil(FILE* fp, CString st0);
	int exportAph_BurstLatancyRangeN(FILE* fpScript);
	void buildBurstNodes();
	bool sameDelimiter(double dTc1, double dTc2);
	void setDelimiters(double Tcb, double Tcc);
	int importEvlFile(CString sFileName);
	bool endScript();
	bool scriptEnd(CString sT);
	int runCommond(FILE* fpScript, CString sCommond);
	CString getEvlFileName(CString sAdditionalTitle);
	CString getAbfFileName(CString sAdditionalTitle);
	int runCommond(FILE* fpScript);
	int runScript(FILE* fpScript);
	CString getScriptFileName();
	bool m_bAbf;
	bool m_bEvl;
	bool m_bEndScript;
	CAbf m_cAbf;
	CEvl m_cEvl;
	CBurstHandling m_cBH;
	double m_dTcb;
	double m_dTcc;
	CFileNameHandling m_cFileNameHandler;
	structBurstNodes m_structBurstNodes;
	structClusterNodes m_structClusterNodes;
	structIncidenceSubsetCollectionNode m_structSubsets;
	CIncidenceSubsetHandler m_cSubsetHandler;
	CIndexSetHandler m_cSetHandler;
	void testPrintAssist();
	AbfNode m_structAbfNode;
	int exportAbf(FILE* fp);
	int exportEvl(FILE*fp);
	int adjustAmp_Abf();
	int adjustAmp_Evl();
};

#endif // !defined(AFX_SCRIPTMANAGER_H__1BE45951_3CC1_4FFE_B4E1_1CF32F055A4D__INCLUDED_)
