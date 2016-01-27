// Abf_ProcessingDoc.h : interface of the CAbf_ProcessingDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "Abf.h"
#include "Evl.h"
#include "BurstOptionDialog.h"
#include "IgnoreShortClosingDialog.h"
#include "History.h"
#include <vector>
#include "structures.h"
#include "GaussianFitting.h"
#include "FittedEvlParamterHandling.h"
#include "ScriptManager.h"

#if !defined(AFX_ABF_PROCESSINGDOC_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
#define AFX_ABF_PROCESSINGDOC_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;
typedef vector<float> FloatVector;
typedef vector<int> IntVector;


class CAbf_ProcessingDoc : public CDocument
{
	protected: // create from serialization only
	void CAbf_ProcessingDoc::ImportDualdatafile();
	int i_gcd ( int i, int j );
	vector <structGeometricFittingNode> GetGeometricFittingTrials(structGeometricFittingNode aTrialNode);
	void InputFittedDTParameters();
	void SelectAllRsultNodes(structFittedParameterNode aFittedNode, vector <structFittingResultNode> &avResultsNode, int &nTrials);
	void SelectRsultNodes(structFittedParameterNode aFittedNode, vector <structFittingResultNode> &avResultsNode);
	structFittingResultNode SelectRsultNodes(vector <structFittingResultNode> avResultsNode);
	void OutputAllFittedParameters(FILE* fpOutput, structFittedParameterNode aFittedNode);
	void Freq_Components(double *dT, double *dW, int nComponents, structFRange fFittingRange, 
						int nEventsInRange, double dArea, double* dFrequency, float fLength);
	void OutputFittedDTParameters(structFittedParameterNode aFittedNode);


	CString CAbf_ProcessingDoc::ExtendFileName(CString sFileName, char ch);
	CString ExtendFileName(CString sFileName, CString sInsert);
	CString CAbf_ProcessingDoc::FileExt(CString sFileName);
	vector <double> CAbf_ProcessingDoc::BurstDelimiters(double dT[], int nDim);
	void ReadFittingDTResultsCollection(vector <structFittingResultsCollectionNode> &aCollectionNode, CString sFileName);
	void ReadFittingResultsNode(structFittingResultNode &aResultNode, FILE *fpOutput, CString sExt);
	void WriteFittingResults(vector <structFittingResultsCollectionNode> avCollection, FILE* fp);
//	void WriteFittingResults(vector <structFittingResultsCollectionNode> avCollection, vector <CEvl> vEvl, FILE* fp);
	void OutputCorrectFittingResults1(structFittingResultNode aResultNode, FILE* fpOutput);
	void OutputCorrectFittingResults(structFittingResultNode aResultNode, FILE* fpOutput);
	double CriticalFunction1(float Ts, float fAs, float Tl, float fAl, float T);
	double CriticalFunction2(float Ts, float fAs, float Tl, float fAl, float T);
	double BurstDelimiter(float fTs, float fTl);	
	double BurstDelimiter(double fTs, double dAs, double fTl, double dAl, int nKey);
	double BurstDelimiter(double fTs, double fTl);
	void ImportDataFile(CString sTitle); 
	double CriticalFunction(float Ts, float Tl, float T);
	void ExportDwellCurves(vector <structFittingResultNode> avResultNode);
	void OutputFittingResults(structDwellFittingTrialNode aTrialNde, structFittingResultNode aResultNode,FILE* fpOutput);
	void OutputHistSum(vector <structEvlHistogramNode> structHistSum, CString sSummaryFileName);
	vector <structDwellFittingTrialNode> CAbf_ProcessingDoc::GetDwellFittingTrials_Original(structDwellFittingTrialNode aTrialNode);
	vector <structDwellFittingTrialNode> CAbf_ProcessingDoc::GetDwellFittingTrials(structDwellFittingTrialNode aTrialNode);
	void OnFileImportDatafile(CString sTitle);
	void CAbf_ProcessingDoc::OnFileImportEventfile(CString sTitle);	
	void OnFileExportDatafile(CString sTitle);
	CAbf_ProcessingDoc();
	bool m_bDualChannelAbf;
	bool m_bTripleChannelAbf;
	bool m_bEvl;
	bool m_bAbf;
	bool m_bCalConfidanceRange;
	bool m_bFullIteration;
	bool m_bOriginalEvl;
	bool m_bComparisonEvl;
	bool bOutputConfidanceRanges;
	vector <structFittingResultsCollectionNode> m_avFittingResultsCollection;
	DECLARE_DYNCREATE(CAbf_ProcessingDoc)
	CAbf m_cAbf;
	CEvl m_cEvl;
	CAbf m_cTripleChannelAbf;
	CEvl m_cOriginalEvl;
	CEvl m_cComparisonEvl;
	CGaussianFitting m_cGaussian;
	CAbf m_cDualChannelAbf;
	CString m_sAbfFileName;
	CString m_sEvlFileName;
	CString m_sComparisonEvlFileName;
	CString m_sDualChannelAbfFileName;
	CString m_sTripleChannelAbfFileName;
	CHistory m_cStoredHistory;
	CHistory m_cRetrievedHistory;
	CString m_sClampfitReultFileName;
	CString m_sEvlMultiSumFileName;
	int m_nFilteringKey;
	CScriptManager m_cScriptManager;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbf_ProcessingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ImportEvlAbfPair();
	void WriteToAfrFile(structpClampResultNode aNode);
	structpClampResultNode ReadpClampResultFile();
	void WriteFittingResults(structFittingResultNode aNode,  FILE* fp);
	CString ReadAString(FILE* fp);
	void WriteAString(CString aString, FILE* fp);
	CString PassingString(CString sString);
	bool Significant(structFittingResultNode aNode, structFittingResultNode bNode, double& dL, int& nDf, double& dProb);
	CString ConvertToString(int nInt);
	void OutputFormattedTimeSpan(FILE *fpOutput, CString sComments, CTimeSpan aTime);
	void OutputFormattedTime(FILE* fpOutput, CString sComments, CTime aTime);
	CString ChangeFileExt(CString sFileName, CString sExt);
	structSemNode CalSem(vector <float> vectfVect);
	structSemNode CalSem(vector <double> vectfVect);
	structSemNode CalSem(vector <int> vectfVect);
	virtual ~CAbf_ProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void CombiningDataNodes(AbfNode aNode, AbfNode bNode, AbfNode &NewNode);
	bool DegeneratedResultNodes(structFittingResultNode aResultNode, structFittingResultNode bResultNode);
	void ResultNodeClusters(vector <structFittingResultNode> &avResultNodes);
	void SetConfidanceRange(int nFuncKey, vector <structFittingResultNode> &avResultNodes);
	bool m_bFilterEvl;
	void GetFittedparameters1(structFittedParameterNode aFittedNode);
	void GetFittedparameters_Old(structFittedParameterNode aFittedNode);
	void GetFittedparameters(structFittedParameterNode aFittedNode);
	void OrderFittingResults(vector <structFittingResultNode> &avResultNode);
	//{{AFX_MSG(CAbf_ProcessingDoc)
	afx_msg void OnFileImportEventfile();
	afx_msg void OnFileImportDatafile();
	afx_msg void OnFileExportDatafile();
	afx_msg void OnFileExportEvlfile();
	afx_msg void OnAnalysisModeseperation();
	afx_msg void OnAnalysisBaselineadjustment();
	afx_msg void OnFileExportAdjusteddatafile();
	afx_msg void OnAnalysisAllpointhistogram();
	afx_msg void OnAnalysisTrimmingmultipleopenning();
	afx_msg void OnFileExportBurstevl();
	afx_msg void OnFileAttachEvlfile();
	afx_msg void OnFileImportAsciiasdata();
	afx_msg void OnAnalysisEventlistTimecourse();
	afx_msg void OnAnalysisDatafileBallrolling();
	afx_msg void OnAnalysisDatafileRemoveartifacts();
	afx_msg void OnFileImportAsciiformatoftackfitfile();
	afx_msg void OnWindowShowfilenames();
	afx_msg void OnAnalysisEventlistIgnoreshortclosings();
	afx_msg void OnFileExportAsciiformofevl();
	afx_msg void OnFileExportEvlsummary();
	afx_msg void OnAnalysisEventlistMultipleevlsummaries();
	afx_msg void OnAnalysisDatafileUpdateadjusteddata();
	afx_msg void OnAnalysisEventlistIgnoreshortopenings();
	afx_msg void OnAnalysisDatafileReplacesegments();
	afx_msg void OnFileImportEvlseparatebody();
	afx_msg void OnAnalysisEventlistAmphistogram();
	afx_msg void OnAnalysisEventlistRemovedegeneracy();
	afx_msg void OnFileExportConcatenatedsegments();
	afx_msg void OnFileExportDataheader();
	afx_msg void OnAnalysisEventlistFittingsummaries();
	afx_msg void OnFileExportComparison();
	afx_msg void OnAnalysisDatafileBackupdata();
	afx_msg void OnAnalysisEventlistEvlabfcomparison();
	afx_msg void OnAnalysisEventlistEvlevlabfcomparison();
	afx_msg void OnAnalysisEventlistIgnoresmalltransitions();
	afx_msg void OnAnalysisDatafileAbfabfcomparison();
	afx_msg void OnAnalysisEventlistBurstdelimitertest();
	afx_msg void OnAnalysisEventlistMultipleevlbustdelimitertest();
	afx_msg void OnFileAttachDatafile();
	afx_msg void OnAnalysisEventlistExportsegmentsbatch();
	afx_msg void OnAnalysisDatafileAbftodatconversion();
	afx_msg void OnAnalysisEventlistExporthistogram();
	afx_msg void OnAnalysisTestTestnr();
	afx_msg void OnAnalysisEventlistFittingdwelltimebatch();
	afx_msg void OnAnalysisEventlistFittingdwelltimeautomatic();
	afx_msg void OnAnalysisEventlistSimulateeventlist();
	afx_msg void OnAnalysisEventlistGetfittedparameters();
	afx_msg void OnAnalysisEventlistBurstingtimecourse();
	afx_msg void OnAnalysisEventlistDwellcurveconstruction();
	afx_msg void OnAnalysisEventlistExportExport2devlhistogram();
	afx_msg void OnAnalysisEventlistCombineevlfiles();
	afx_msg void OnANALYSISEVENTLISTEXPORTEXPORTSTATITICsOFBURSTANALYSIS();
	afx_msg void OnAnalysisEventlistComparison0Abfdwellingtimecomparison();
	afx_msg void OnAnalysisEventlistComparisonEvlampabfcomparison();
	afx_msg void OnAnalysisEventlistExcludesubconductances();
	afx_msg void OnAnalysisEventlistBurstsNumofopeningperburst();
	afx_msg void OnANALYSISEVENTLISTExportEvlAsTrace();
	afx_msg void OnANALYSISEVENTLISTBURSTSNUMOFOPENINGPERBURSTBatch();
	afx_msg void OnAnalysisEventlistBurstTotpbBatch();
	afx_msg void OnAnalysisEventlistBurstTotpb();
	afx_msg void OnANALYSISEVENTLISTBurstDuration();
	afx_msg void OnANALYSISEVENTLISTBurstDurationBatch();
	afx_msg void OnAnalysisDatafileRltafrconvert();
	afx_msg void OnFileExportLocalextremaofdatafile();
	afx_msg void OnAnalysisIdealization();
	afx_msg void OnAnalysisEventlistEvlcorrection();
	afx_msg void OnFileImportEventfileforcomparison();
	afx_msg void OnFileExportDualdatafile();
	afx_msg void OnAnalysisEventlistBurstsMeanburstdurationcorrection();
	afx_msg void OnAnalysisDatafileSimulatedatafiles();
	afx_msg void OnAnalysisDatafileSimulatedatafiles0();
	afx_msg void OnAnalysisEventlistExportExport2dreverseevlhistogram();
	afx_msg void OnAnalysisEventlistFittingqmatrix();
	afx_msg void OnAnalysisEventlistCommentevl();
	afx_msg void OnAnalysisDatafileAppendadatafile();
	afx_msg void OnAnalysisEventlistExportlevelallpointhistogram();
	afx_msg void OnANALYSISEVENTLISTAPHEventIndex();
	afx_msg void OnAnalysisEventlistAtftoevl();
	afx_msg void OnAnalysisEventlistExportBurstevolusions();
	afx_msg void OnAnalysisUtilitiesBurstdelimiters();
	afx_msg void OnAnalysisEventlistExportBurstcorrelations();
	afx_msg void OnAnalysisEventlistBuildsubsets();
	afx_msg void OnAnalysisRunscripts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABF_PROCESSINGDOC_H__829A080D_5FE1_11D4_83DE_00C04F200B5A__INCLUDED_)
