// Abf_ProcessingDoc.cpp : implementation of the CAbf_ProcessingDoc class
//
#include "stdafx.h"
#include "Abf_Processing.h"
#include "Abf_ProcessingDoc.h"
#include "BurstOptionDialog.h"
#include "EvlTimeCourseDialog.h"
#include "EvlSummary.h"
#include "FittingSummary.h"
#include "SegmentExtractionDialog2.h"
#include "IgnoreSmallTransitionOptionDialog.h"
#include "EvlHistogramDlg.h"
#include "Structures.h"
#include "nr.h"
#include "QMatrixFitting.h"
//#include "TJCommonUtil.h"
#include "GettingFittedparametersDlg.h"
#include "DataSimulations.h"
#include "EvlComment.h"
#include "BurstingTimeCourseOptionDlg.h"
#include "BurstingTimeCourseOptionDlg.h"
#include "LinearHistogramHandler.h"
#include "BurstDelimiterCalculationDialog.h"
#include "FileNameHandling.h"
//#include "CUF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingDoc

IMPLEMENT_DYNCREATE(CAbf_ProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CAbf_ProcessingDoc, CDocument)
	//{{AFX_MSG_MAP(CAbf_ProcessingDoc)
	ON_COMMAND(ID_FILE_IMPORT_EVENTFILE, OnFileImportEventfile)
	ON_COMMAND(ID_FILE_IMPORT_DATAFILE, OnFileImportDatafile)
	ON_COMMAND(ID_FILE_EXPORT_DATAFILE, OnFileExportDatafile)
	ON_COMMAND(ID_FILE_EXPORT_EVLFILE, OnFileExportEvlfile)
	ON_COMMAND(ID_ANALYSIS_MODESEPERATION, OnAnalysisModeseperation)
	ON_COMMAND(ID_ANALYSIS_BASELINEADJUSTMENT, OnAnalysisBaselineadjustment)
	ON_COMMAND(ID_FILE_EXPORT_ADJUSTEDDATAFILE, OnFileExportAdjusteddatafile)
	ON_COMMAND(ID_ANALYSIS_ALLPOINTHISTOGRAM, OnAnalysisAllpointhistogram)
	ON_COMMAND(ID_ANALYSIS_TRIMMINGMULTIPLEOPENNING, OnAnalysisTrimmingmultipleopenning)
	ON_COMMAND(ID_FILE_EXPORT_BURSTEVL, OnFileExportBurstevl)
	ON_COMMAND(ID_FILE_ATTACH_EVLFILE, OnFileAttachEvlfile)
	ON_COMMAND(ID_FILE_IMPORT_ASCIIASDATA, OnFileImportAsciiasdata)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_TIMECOURSE, OnAnalysisEventlistTimecourse)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_BALLROLLING, OnAnalysisDatafileBallrolling)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_REMOVEARTIFACTS, OnAnalysisDatafileRemoveartifacts)
	ON_COMMAND(ID_FILE_IMPORT_ASCIIFORMATOFTACKFITFILE, OnFileImportAsciiformatoftackfitfile)
	ON_COMMAND(ID_WINDOW_SHOWFILENAMES, OnWindowShowfilenames)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_IGNORESHORTCLOSINGS, OnAnalysisEventlistIgnoreshortclosings)
	ON_COMMAND(ID_FILE_EXPORT_ASCIIFORMOFEVL, OnFileExportAsciiformofevl)
	ON_COMMAND(ID_FILE_EXPORT_EVLSUMMARY, OnFileExportEvlsummary)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_MULTIPLEEVLSUMMARIES, OnAnalysisEventlistMultipleevlsummaries)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_UPDATEADJUSTEDDATA, OnAnalysisDatafileUpdateadjusteddata)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_IGNORESHORTOPENINGS, OnAnalysisEventlistIgnoreshortopenings)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_REPLACESEGMENTS, OnAnalysisDatafileReplacesegments)
	ON_COMMAND(ID_FILE_IMPORT_EVLSEPARATEBODY, OnFileImportEvlseparatebody)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_AMPHISTOGRAM, OnAnalysisEventlistAmphistogram)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_REMOVEDEGENERACY, OnAnalysisEventlistRemovedegeneracy)
	ON_COMMAND(ID_FILE_EXPORT_CONCATENATEDSEGMENTS, OnFileExportConcatenatedsegments)
	ON_COMMAND(ID_FILE_EXPORT_DATAHEADER, OnFileExportDataheader)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_FITTINGSUMMARIES, OnAnalysisEventlistFittingsummaries)
	ON_COMMAND(ID_FILE_EXPORT_COMPARISON, OnFileExportComparison)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_BACKUPDATA, OnAnalysisDatafileBackupdata)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EVLABFCOMPARISON, OnAnalysisEventlistEvlabfcomparison)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EVLEVLABFCOMPARISON, OnAnalysisEventlistEvlevlabfcomparison)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_IGNORESMALLTRANSITIONS, OnAnalysisEventlistIgnoresmalltransitions)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_ABFABFCOMPARISON, OnAnalysisDatafileAbfabfcomparison)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BURSTDELIMITERTEST, OnAnalysisEventlistBurstdelimitertest)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_MULTIPLEEVLBUSTDELIMITERTEST, OnAnalysisEventlistMultipleevlbustdelimitertest)
	ON_COMMAND(ID_FILE_ATTACH_DATAFILE, OnFileAttachDatafile)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORTSEGMENTSBATCH, OnAnalysisEventlistExportsegmentsbatch)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_ABFTODATCONVERSION, OnAnalysisDatafileAbftodatconversion)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORTHISTOGRAM, OnAnalysisEventlistExporthistogram)
	ON_COMMAND(ID_ANALYSIS_TEST_TESTNR, OnAnalysisTestTestnr)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_FITTINGDWELLTIMEBATCH, OnAnalysisEventlistFittingdwelltimebatch)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_FITTINGDWELLTIMEAUTOMATIC, OnAnalysisEventlistFittingdwelltimeautomatic)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_SIMULATEEVENTLIST, OnAnalysisEventlistSimulateeventlist)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_GETFITTEDPARAMETERS, OnAnalysisEventlistGetfittedparameters)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BURSTINGTIMECOURSE, OnAnalysisEventlistBurstingtimecourse)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_DWELLCURVECONSTRUCTION, OnAnalysisEventlistDwellcurveconstruction)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORT_EXPORT2DEVLHISTOGRAM, OnAnalysisEventlistExportExport2devlhistogram)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_COMBINEEVLFILES, OnAnalysisEventlistCombineevlfiles)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORT_EXPORTSTATITICsOFBURSTANALYSIS, OnANALYSISEVENTLISTEXPORTEXPORTSTATITICsOFBURSTANALYSIS)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_COMPARISON0_ABFDWELLINGTIMECOMPARISON, OnAnalysisEventlistComparison0Abfdwellingtimecomparison)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_COMPARISON_EVLAMPABFCOMPARISON, OnAnalysisEventlistComparisonEvlampabfcomparison)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXCLUDESUBCONDUCTANCES, OnAnalysisEventlistExcludesubconductances)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BURSTS_NUMOFOPENINGPERBURST, OnAnalysisEventlistBurstsNumofopeningperburst)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_ExportEvlAsTrace, OnANALYSISEVENTLISTExportEvlAsTrace)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BURSTS_NUMOFOPENINGPERBURST_Batch, OnANALYSISEVENTLISTBURSTSNUMOFOPENINGPERBURSTBatch)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_TOTPB_BATCH, OnAnalysisEventlistBurstTotpbBatch)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_TOTPB, OnAnalysisEventlistBurstTotpb)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BurstDuration, OnANALYSISEVENTLISTBurstDuration)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BurstDurationBatch, OnANALYSISEVENTLISTBurstDurationBatch)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_RLTAFRCONVERT, OnAnalysisDatafileRltafrconvert)
	ON_COMMAND(ID_FILE_EXPORT_LOCALEXTREMAOFDATAFILE, OnFileExportLocalextremaofdatafile)
	ON_COMMAND(ID_ANALYSIS_IDEALIZATION, OnAnalysisIdealization)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EVLCORRECTION, OnAnalysisEventlistEvlcorrection)
	ON_COMMAND(ID_FILE_IMPORT_EVENTFILEFORCOMPARISON, OnFileImportEventfileforcomparison)
	ON_COMMAND(ID_FILE_EXPORT_DUALDATAFILE, OnFileExportDualdatafile)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BURSTS_MEANBURSTDURATIONCORRECTION, OnAnalysisEventlistBurstsMeanburstdurationcorrection)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_SIMULATEDATAFILES, OnAnalysisDatafileSimulatedatafiles)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORT_EXPORT2DREVERSEEVLHISTOGRAM, OnAnalysisEventlistExportExport2dreverseevlhistogram)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_FITTINGQMATRIX, OnAnalysisEventlistFittingqmatrix)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_COMMENTEVL, OnAnalysisEventlistCommentevl)
	ON_COMMAND(ID_ANALYSIS_DATAFILE_APENDADATAFILE, OnAnalysisDatafileAppendadatafile)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_APH_EventIndex, OnANALYSISEVENTLISTAPHEventIndex)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_ATFTOEVL, OnAnalysisEventlistAtftoevl)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORT_BURSTEVOLUSIONS, OnAnalysisEventlistExportBurstevolusions)
	ON_COMMAND(ID_ANALYSIS_UTILITIES_BURSTDELIMITERS, OnAnalysisUtilitiesBurstdelimiters)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_EXPORT_BURSTCORRELATIONS, OnAnalysisEventlistExportBurstcorrelations)
	ON_COMMAND(ID_ANALYSIS_EVENTLIST_BUILDSUBSETS, OnAnalysisEventlistBuildsubsets)
	ON_COMMAND(ID_ANALYSIS_RUNSCRIPTS, OnAnalysisRunscripts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingDoc construction/destruction

CAbf_ProcessingDoc::CAbf_ProcessingDoc()
{
	// TODO: add one-time construction code here
	CAbf m_cAbf();
	CEvl m_cEvl();
	CAbf m_cDualChannelAbf();
	CHistory m_cStoredHistory;
	CHistory m_cRetrievedHistory;
	m_bDualChannelAbf=false;
	m_bTripleChannelAbf=false;
	m_bEvl=false;
	m_bAbf=false;
	m_bOriginalEvl=false;
	m_bComparisonEvl=false;
	
	m_sAbfFileName="Not opened yet.";
	m_sDualChannelAbfFileName="Not opened yet.";
	m_sTripleChannelAbfFileName="Not opened yet.";
	m_sEvlFileName="Not opened yet."; 
	m_sComparisonEvlFileName="Not opened yet."; 
	m_sClampfitReultFileName="Not opened yet.";
	m_bFullIteration=false;
	CEvl m_cOriginalEvl;
	CEvl::m_nSN=0;
}

CAbf_ProcessingDoc::~CAbf_ProcessingDoc()
{
}

BOOL CAbf_ProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingDoc serialization

void CAbf_ProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingDoc diagnostics

#ifdef _DEBUG
void CAbf_ProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAbf_ProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAbf_ProcessingDoc commands


void CAbf_ProcessingDoc::OnFileImportEventfile(CString sTitle) 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	char* pch="Event List Files (*.evl)\0*.Evl\0All Files\0*.*\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sEvlFileName);

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.ReadEvl(sEvlFileName);
		m_sEvlFileName=sEvlFileName;
	}
	m_cOriginalEvl=m_cEvl;
	m_bOriginalEvl=true;
}

void CAbf_ProcessingDoc::OnFileImportEventfile() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Event List Files (*.evl)\0*.Evl\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import an event list file";
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sEvlFileName);

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.ReadEvl(sEvlFileName);
		m_sEvlFileName=sEvlFileName;
	}
	m_cOriginalEvl=m_cEvl;
	m_bOriginalEvl=true;
}


void CAbf_ProcessingDoc::OnFileImportDatafile() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat;*.abf)\0*.dat;*.abf\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a data file";
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sAbfFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}
}

void CAbf_ProcessingDoc::OnFileImportDatafile(CString sTitle) 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp data Files (*.dat;*.abf)\0*.abf;*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sAbfFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}
}

void CAbf_ProcessingDoc::ImportDataFile(CString sTitle) 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp data Files (*.dat;*.abf)\0*.abf;*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sAbfFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}else{
		m_sAbfFileName="Canceled";
	}
}

void CAbf_ProcessingDoc::OnFileExportDatafile() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export a Data File";

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}	
}

void CAbf_ProcessingDoc::OnFileExportDatafile(CString sTitle) 
{
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle=sTitle;
	CString sFileName=ChangeFileExt(m_sAbfFileName,"dat");
//	aFileDlg.m_ofn.lpstrFile=sFileName.GetBuffer(sFileName.GetLength());
//	sFileName.ReleaseBuffer();
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);


	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}	
}

void CAbf_ProcessingDoc::OnFileExportEvlfile() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Event List Files (*.evl)\0*.Evl\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export an event list file";
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sEvlFileName);
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.WriteEvl(sEvlFileName);
	}	
}


void CAbf_ProcessingDoc::OnAnalysisModeseperation() 
{
	// TODO: Add your command handler code here
	CBurstOptionDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		m_cEvl.SetBurstDelimiter(aDlg.GetBurstDelimiter());
		m_cEvl.SetMinimumOpenning(aDlg.GetMinimumOpenning());
		m_cEvl.CheckBursting();
		m_cEvl.ModeSeparated();
	}
}

void CAbf_ProcessingDoc::OnAnalysisBaselineadjustment() 
{
	// TODO: Add your command handler code here
	m_cAbf.BaslieAdjustment();
	
}

void CAbf_ProcessingDoc::OnFileExportAdjusteddatafile() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sAbfFileName;

	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export an adjusted Data File";
	CString sFileName=ChangeFileExt(m_sAbfFileName,"dat");
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);




	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		FILE* fpOut=fopen("test.Mac","w");
		if(fpAbfOut=fopen(sAbfFileName,"wb"))
		{
		    m_cAbf.WriteAdjustedData(fpAbfOut);
		    fclose(fpAbfOut);
			m_cStoredHistory.WriteHistory(fpOut,sAbfFileName);
			fclose(fpOut);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}	
}

void CAbf_ProcessingDoc::OnAnalysisAllpointhistogram() 
{
	// TODO: Add your command handler code here
	CString sTitle="Import a data file for all-point histogram.";
	OnFileImportDatafile();
	m_cAbf.MakeAllPointHistgram(m_sAbfFileName);
}

void CAbf_ProcessingDoc::OnAnalysisTrimmingmultipleopenning() 
{
	// TODO: Add your command handler code here
	m_cEvl.TrimmingMulOpenning();
}

void CAbf_ProcessingDoc::OnFileExportBurstevl() 
{
	// TODO: Add your command handler code here
	CIgnoreShortClosingDialog aDlg;
	float fCutoff=0.f;
	if(aDlg.DoModal()==IDOK)
	{
		fCutoff=aDlg.GetCutoff();
		m_cEvl.IgnoreShortClosing(fCutoff);
		OnFileExportEvlfile();
	}
}

void CAbf_ProcessingDoc::OnFileAttachEvlfile() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Event List Files (*.evl)\0*.Evl\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import an event list file";
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.SetAttachingStatus();
		m_cEvl.ReadEvl(sEvlFileName);
	}	
}

void CAbf_ProcessingDoc::OnFileImportAsciiasdata() 
{
		FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"r");
		m_cAbf.ImportAsciiAsData(fpAbfIn);
		fclose(fpAbfIn);
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistTimecourse() 
{
	// TODO: Add your command handler code here
	if(!m_bEvl)
	{
		OnFileImportEventfile();
	}

	CEvlTimeCourseDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		float fBinSize=aDlg.GetBinSize();
		if(aDlg.HardBoundary())
		{
			m_cEvl.DoTimeCourse0(fBinSize);
		}
		else
		{
			m_cEvl.DoTimeCourse(fBinSize);
		}
	}
}


void CAbf_ProcessingDoc::OnAnalysisDatafileBallrolling() 
{
	// TODO: Add your command handler code here
	m_cAbf.BallRolling(&m_cStoredHistory);
}

void CAbf_ProcessingDoc::OnAnalysisDatafileRemoveartifacts() 
{
	// TODO: Add your command handler code here
	EvlNode aEvlNode=m_cEvl.GetEvlNode();
	m_cAbf.RemoveArtifacts(aEvlNode);	
}

void CAbf_ProcessingDoc::OnFileImportAsciiformatoftackfitfile() 
{
	// TODO: Add your command handler code here
	OnFileImportEventfile();
	CString sEvlFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.ReadAscIIFormatOfTakFitFile(sEvlFileName);
	}	
}

void CAbf_ProcessingDoc::OnWindowShowfilenames() 
{
	// TODO: Add your command handler code here
	CString sMessage="The Abf File is: ";
	sMessage+=m_sAbfFileName;
	sMessage+="\n";
	sMessage+="The DualChannel File is: ";
	sMessage+=m_sDualChannelAbfFileName;
	sMessage+="\n";	
	sMessage+="The TripleChannel File is: ";
	sMessage+=m_sTripleChannelAbfFileName;
	sMessage+="\n";	
	sMessage+="The Evl File is: ";
	sMessage+=m_sEvlFileName;
	sMessage+="\n";
	sMessage+="The Comparison Evl File is: ";
	sMessage+=m_sComparisonEvlFileName;
	sMessage+="\n";

	AfxMessageBox(sMessage,MB_OK,0);
}

void CAbf_ProcessingDoc::OnAnalysisEventlistIgnoreshortclosings() 
{
	// TODO: Add your command handler code here
	CIgnoreShortClosingDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		float fCutoff=aDlg.GetCutoff();
		m_cEvl.IgnoreShortClosing(fCutoff);
	}
}

void CAbf_ProcessingDoc::OnFileExportAsciiformofevl() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
		char* pch="Acsii form of event list files (*.Avl)\0*.Avl\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export ASCII Form of the Event List";
	CString sFileName=ChangeFileExt(m_sEvlFileName,"Avl");
//	aFileDlg.m_ofn.lpstrFile=sFileName.GetBuffer(sFileName.GetLength());
//	sFileName.ReleaseBuffer();
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		FILE* fp=fopen(sEvlFileName,"w");
		m_cEvl.ExportAsciiEvl(fp);
		fclose(fp);
	}	
}

void CAbf_ProcessingDoc::OnFileExportEvlsummary() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		FILE* fp=fopen(sEvlFileName,"a");
		m_cEvl.ExportSummary(fp);
		fclose(fp);
	}		
}

void CAbf_ProcessingDoc::OnAnalysisEventlistMultipleevlsummaries() 
{
	// TODO: Add your command handler code here	
	CString sEvlFileName;
	CString sSummaryFileName;
	CString sFileNames;
	vector <structEvlHistogramNode> structHistSum;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pchFilter="Evl Summary File (*.Sum)\0*.Sum\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pchFilter;
	aFileDlg.m_ofn.lpstrTitle="Outut an Evl Summary File";

	if(aFileDlg.DoModal()==IDOK)
	{
		sSummaryFileName=aFileDlg.GetPathName();
		m_sEvlMultiSumFileName=sSummaryFileName;
		FILE* fp=fopen(sSummaryFileName,"w");

		char* pch="Event List Files (*.EVL)\0*.EVL\0All Files\0*.*\0\0";
		CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrFile=sFileNames.GetBuffer(7200);
		aFileDlg.m_ofn.nMaxFile=7200;
		POSITION aPosition;

		int nNumFiles=0;

		vector <int> vnNumOpenings;
		vector <int> vnNumBursts;
		vector <float> vectfPo;
		vector <float> vectfTo;
		vector <float> vectfAmp;

		vector <float> vectfBPo;
		vector <float> vectfBTo;
		vector <int>   vectnChannelNum;
		vector <float> vectfRecordLength;
		vector <float> vectfBTc;
		vector <float> vectfPoib;
		vector <float> vectfTc;
		vector <float> vectfTcib;
		vector <CString> vectsFileNames;
		float fMinLength=0.f;

		CIgnoreShortClosingDialog aDlg;
		float fCutoff=0.f;
		float fBPo,fBTo,fPoib,fBTc,fTc,fPo;
		if(aDlg.DoModal()==IDOK)
		{
			fCutoff=aDlg.GetCutoff();
		}
		float fTo,fTcib;
		if(aFileDlg.DoModal()==IDOK)
		{
			aPosition=aFileDlg.GetStartPosition();
			float fAmp=0.f;
			int nCount=0;
			while(aPosition!=NULL)
			{
				sEvlFileName=aFileDlg.GetNextPathName(aPosition);
				m_cEvl.ReadEvl(sEvlFileName);
//				m_cEvl.RemoveDegeneracy(2); //remember to turn it off
				structEvlHistogramNode aHistNode=m_cEvl.GetEvlHistogram();
				structHistSum.push_back(aHistNode);
				m_cEvl.ExportSummary(fp,nCount);
				structEvlSummaryNode aNode=m_cEvl.GetSummaryNode();

				if(aNode.nMaxLevel>0)
				{
					vectsFileNames.push_back(sEvlFileName);
					nCount++;
					if(aNode.fRecordLength>fMinLength)
					{
						vectfPo.push_back(aNode.fOpenProb);
						vectfTo.push_back(aNode.fMeanOpenTime);
						vectnChannelNum.push_back(aNode.nNumChannels);
						vectfRecordLength.push_back(aNode.fRecordLength);
						fAmp=aNode.vectfAmplitude[1];
						if(fAmp<90)
						{
							vectfAmp.push_back(fAmp);
						}
					}

					m_cEvl.IgnoreShortClosing(fCutoff);
					m_cEvl.ExportSummary(fp);
					structEvlSummaryNode bNode=m_cEvl.GetSummaryNode();
					if(aNode.fRecordLength>fMinLength)
					{
						vectfBPo.push_back(bNode.fOpenProb);
						vectfBTo.push_back(bNode.fMeanOpenTime);
						nNumFiles++;
					}
					fPo=aNode.fOpenProb;
					fBPo=bNode.fOpenProb;
					fBTo=bNode.fMeanOpenTime;
					fPoib=fPo/fBPo;
					fTo=aNode.fMeanOpenTime;
					fTc=fTo*(1.f/fPo-1.f);
					fTcib=fTo*(1.f/fPoib-1.f);
					fBTc=fBTo*(1.f/fBPo-1.f);
					vectfPoib.push_back(fPoib);
					vectfBTc.push_back(fBTc);
					vectfTc.push_back(fTc);
					vectfTcib.push_back(fTcib);
					vnNumOpenings.push_back(aNode.vectnNumEvents[1]);
					vnNumBursts.push_back(bNode.vectnNumEvents[1]);
				}
			}
		}
		
		structSemNode PoSem=CalSem(vectfPo);
		structSemNode ToSem=CalSem(vectfTo);
		structSemNode TcSem=CalSem(vectfTc);
		structSemNode TcibSem=CalSem(vectfTcib);
		structSemNode AmpSem=CalSem(vectfAmp);

		structSemNode BPoSem=CalSem(vectfBPo);
		structSemNode BToSem=CalSem(vectfBTo);
		structSemNode BTcSem=CalSem(vectfBTc);
		structSemNode PoibSem=CalSem(vectfPoib);
		fprintf(fp,"\n\n");		
		fprintf(fp,"Po:        %12.3f +- %12.5f (n= %2d),     To (ms):  %12.3f +- %12.3f (n= %2d)\n",
				PoSem.fMean,PoSem.fSem,nNumFiles,ToSem.fMean,ToSem.fSem,nNumFiles);
		fprintf(fp,"Tc (ms):   %12.3f +- %12.3f (n= %2d),     Amp:      %12.3f +- %12.3f (n= %2d)\n",
				TcSem.fMean,TcSem.fSem,nNumFiles,AmpSem.fMean,AmpSem.fSem,nNumFiles);
		fprintf(fp,"Tcib (ms): %12.3f +- %12.3f (n= %2d),     Tc:       %12.3f +- %12.3f (n= %2d)\n",
				TcibSem.fMean,TcibSem.fSem,nNumFiles,TcSem.fMean,TcSem.fSem,nNumFiles);
		fprintf(fp,"BPo:       %12.3f +- %12.5f (n= %2d),     BPoib:    %12.3f +- %12.5f (n= %2d),\n",
				BPoSem.fMean,BPoSem.fSem,nNumFiles,PoibSem.fMean,PoibSem.fSem,nNumFiles);
		fprintf(fp,"BTo (ms):  %12.3f +- %12.3f (n= %2d),     BTc (ms): %12.3f +- %12.3f (n= %2d)\nfCutoff: %8.1f\n",
				BToSem.fMean,BToSem.fSem,nNumFiles,BTcSem.fMean,BTcSem.fSem,nNumFiles,
				fCutoff);


		int nSize=vectfPo.size();
		fprintf(fp,"\n\n   i      Po       To           Tc");
		fprintf(fp,"     BPo          BTo          BTc");
		fprintf(fp,"    Poib         Tcib      Amp      Length  Openings    Bursts  NumChann    FileName\n");
		for(int i=0;i<nSize;i++)
		{
			fprintf(fp," %3d  %6.4f  %7.2f  %11.2f ", i+1,vectfPo[i],vectfTo[i],vectfTc[i]);
			fprintf(fp," %6.4f  %11.2f  %11.2f ",vectfBPo[i],vectfBTo[i],vectfBTc[i]);
			fprintf(fp," %6.4f  %11.2f  %7.3f  %10.1f  %8d  %8d  %8d    %s\n",
				vectfPoib[i],vectfTcib[i],vectfAmp[i],vectfRecordLength[i],vnNumOpenings[i],vnNumBursts[i],vectnChannelNum.at(i),vectsFileNames[i]);
		}

				fprintf(fp,"\n\n     PofMean         PoSem       ToMean         ToSem       TcMean         TcSem      AmpMean        AmpSem     TcibMean       TcibSem       BPoMean        BPoSem     PoibMean       PoibSem      BToMean        BToSem      BTcMean        BTcSem  N    fCutoff    Name\n");		
		fprintf(fp,"%12.3f  %12.5f %12.3f  %12.3f ",
				PoSem.fMean,PoSem.fSem,ToSem.fMean,ToSem.fSem);
		fprintf(fp,"%12.3f  %12.3f %12.3f  %12.3f ",
				TcSem.fMean,TcSem.fSem,AmpSem.fMean,AmpSem.fSem);
		fprintf(fp,"%12.3f  %12.3f  ",
				TcibSem.fMean,TcibSem.fSem);
		fprintf(fp,"%12.3f  %12.5f %12.3f  %12.5f ",
				BPoSem.fMean,BPoSem.fSem,PoibSem.fMean,PoibSem.fSem);
		fprintf(fp,"%12.3f  %12.3f %12.3f  %12.3f %2d   %8.1f",
				BToSem.fMean,BToSem.fSem,BTcSem.fMean,BTcSem.fSem,nNumFiles,fCutoff);
		fprintf(fp,"    %s \n",sSummaryFileName);

		fclose(fp);
//		OutputHistSum(structHistSum,sSummaryFileName);
	}		

}

structSemNode CAbf_ProcessingDoc::CalSem(vector <float> vectfVect)
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


structSemNode CAbf_ProcessingDoc::CalSem(vector <double> vectfVect)
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

structSemNode CAbf_ProcessingDoc::CalSem(vector <int> vectfVect)
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

void CAbf_ProcessingDoc::OnAnalysisDatafileUpdateadjusteddata() 
{
	// TODO: Add your command handler code here
	m_cAbf.UpdateAdjustedData();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistIgnoreshortopenings() 
{
	// TODO: Add your command handler code here
	CIgnoreShortClosingDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		float fCutoff=aDlg.GetCutoff();
		m_cEvl.IgnoreShortOpenings(fCutoff);
	}	
}

void CAbf_ProcessingDoc::OnAnalysisDatafileReplacesegments() 
{
	// TODO: Add your command handler code here
	m_cAbf.ReplaceSegments(&m_cStoredHistory);
}

void CAbf_ProcessingDoc::OnFileImportEvlseparatebody() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName1;
	CString sEvlFileName2;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName1=aFileDlg.GetPathName();
	}
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName2=aFileDlg.GetPathName();
		m_cEvl.ReadEvl(sEvlFileName1,sEvlFileName2);
		m_sEvlFileName=sEvlFileName2;
	}	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistAmphistogram() 
{
	// TODO: Add your command handler code here
	OnAnalysisEventlistIgnoreshortclosings();
	AbfNode structTrace=m_cAbf.GetTrace();
	m_cEvl.ReAssignAmp(structTrace);
	m_cEvl.AmpHistogram();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistRemovedegeneracy() 
{
	// TODO: Add your command handler code here
	int nMultp=2;
	m_cEvl.RemoveDegeneracy(nMultp);
}

void CAbf_ProcessingDoc::OnFileExportConcatenatedsegments() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			CSegmentExtractionDialog2 aDlg;
			if(aDlg.DoModal()==IDOK)
			{
				structSegmentExtractionNode aNode;
				float fLength,fBegin,fEnd;
				aNode=aDlg.GetParameters(fLength,fBegin,fEnd);
		    	m_cAbf.ExtractSegments(fpAbfOut,aNode);
		     	fclose(fpAbfOut);
			}
		}
	}		
}

void CAbf_ProcessingDoc::OnFileExportDataheader() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"w")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cAbf.WriteHeader(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}	
		
}

void CAbf_ProcessingDoc::OnAnalysisEventlistFittingsummaries() 
{
	// TODO: Add your command handler code here
	CFittingSummary aSummary;
	aSummary.SummarizeFittings();
}

void CAbf_ProcessingDoc::OnFileExportComparison() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here

	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
//				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
	}

	AbfNode aNode=m_cAbf.GetTrace();
	AbfNode bNode=m_cAbf.GetAdjustedTrace();
	int nSize1=aNode.nSize;
	int nSize2=bNode.nSize;
	float *pfData=new float[nSize1+nSize2];
	int i;
	int nPosition=0;
	int nSize=nSize1;
	if(nSize<nSize2)
	{
		nSize=nSize2;
	}
	for(i=0;i<nSize;i++)
	{
			pfData[nPosition]=aNode.pfData[i];
			nPosition++;
			pfData[nPosition]=bNode.pfData[i];
			nPosition++;
	}

	m_cDualChannelAbf.UpdateSize(2*nSize);
	m_cDualChannelAbf.UpdateData(pfData);
	m_cDualChannelAbf.EqualizeScale();
	delete pfData;

	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

	if(bFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=bFileDlg.GetPathName();
		m_sDualChannelAbfFileName=sAbfFileName;
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cDualChannelAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}	
	
}

void CAbf_ProcessingDoc::OnAnalysisDatafileBackupdata() 
{
	// TODO: Add your command handler code here
	m_cAbf.BackupData();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistEvlabfcomparison() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sTitle;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
			char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
			aFileDlg.m_ofn.lpstrFilter=pch;
			aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}

//	if(!m_bAbf)
//	{
		sTitle="Import a data file for cocmaring with dwelling time";
		OnFileImportDatafile(sTitle);
//	}

		
//	if(!m_bEvl)
//	{
		sTitle="Import a event list file for cocmaring with dwelling time";
		OnFileImportEventfile(sTitle);
//	}
	AbfNode aNode=m_cAbf.GetTrace();
	AbfNode bNode=m_cEvl.GetEvlAsTrace();
//	m_cEvl.Summarize();
//	structEvlSummaryNode aEvlSumNode=m_cEvl.GetSummaryNode();
	int nSize1=aNode.nSize;
	int nSize2=bNode.nSize;
	float *pfData=new float[nSize1+nSize2];
	int i,i0;
	int nPosition=0;
	float fTemp;
	int nSize=nSize1;
	if(nSize>nSize2)
	{
		nSize=nSize2;
	}
	for(i=0;i<nSize;i++)
	{
			pfData[nPosition]=aNode.pfData[i];
			nPosition++;
	//		i0=int(bNode.pfData[i]);
	//		fTemp=aEvlSumNode.vectfAmplitude[i0];
			pfData[nPosition]=100.*bNode.pfData[i];
			nPosition++;
	}

	m_cDualChannelAbf.UpdateSize(2*nSize);
	m_cDualChannelAbf.UpdateData(pfData);
	m_cDualChannelAbf.EqualizeScale();
	delete pfData;

	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	pch="pClamp Data Files (*.dat)\0*.dat\0\0";
	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	bFileDlg.m_ofn.lpstrFilter=pch;
	bFileDlg.m_ofn.lpstrTitle="Save the dual comparison into a data file";
	if(bFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=bFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
	//		AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	   		m_cDualChannelAbf.WriteData(fpAbfOut);
	   		fclose(fpAbfOut);
		}
	}					
}

void CAbf_ProcessingDoc::OnAnalysisEventlistEvlevlabfcomparison() 
{
	// TODO: Add your command handler code here

	FILE *fpAbfIn;
	FILE *fpAbfOut;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a triple channel data file";
//	if(!m_bTripleChannelAbf)
//	{
		if(aFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=aFileDlg.GetPathName();
			m_sTripleChannelAbfFileName=sAbfFileName;
			CString sMessage="Can not open the triple channel data file";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cTripleChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}

			CString sTitle;

			if(!m_bAbf)
			{
				sTitle="Import a data file to compare with the event list files";
				OnFileImportDatafile(sTitle);
				m_bAbf=true;
			}

				
			if(!m_bEvl)
			{
				sTitle="Import a event list file to compare with the abf file";
				OnFileImportEventfile(sTitle);
				m_bEvl=true;
			}

			if(!m_bComparisonEvl)
			{
				m_cComparisonEvl=m_cEvl;
				m_sComparisonEvlFileName=m_sEvlFileName;
				sTitle="Import ather event list file to compare with the abf file";
				OnFileImportEventfile(sTitle);
				m_bOriginalEvl=true;
			}
			AbfNode aNode=m_cAbf.GetTrace();
				
			AbfNode bNode=m_cEvl.GetEvlAsTrace();
			AbfNode cNode=m_cComparisonEvl.GetEvlAsTrace();
		//	m_cEvl.Summarize();
		//	structEvlSummaryNode aEvlSumNode=m_cEvl.GetSummaryNode();
		//	m_cComparisonEvl.Summarize();
		//	structEvlSummaryNode bEvlSumNode=m_cComparisonEvl.GetSummaryNode();
			int nSize1=aNode.nSize;
			int nSize2=bNode.nSize;
			int nSize3=cNode.nSize;
			int i,i0;
			int nPosition=0;
			float fTemp;
			int nSize=nSize1;
			if(nSize>nSize2)
			{
				nSize=nSize2;
			}
			if(nSize>nSize3)
			{
				nSize=nSize3;
			}
			float *pfData=new float[3*nSize];
			for(i=0;i<nSize;i++)
			{
				pfData[nPosition]=aNode.pfData[i];
				nPosition++;
		//		i0=int(bNode.pfData[i]);
		//		fTemp=aEvlSumNode.vectfAmplitude[i0];
				pfData[nPosition]=100.*bNode.pfData[i];
				nPosition++;
		//		i0=int(cNode.pfData[i]);
		//		fTemp=bEvlSumNode.vectfAmplitude[i0];
			//	fTemp=cNode.pfData[i];
				pfData[nPosition]=100.*cNode.pfData[i];
				nPosition++;
			}

			m_cTripleChannelAbf.UpdateSize(3*nSize);
			m_cTripleChannelAbf.UpdateData(pfData);
			m_cTripleChannelAbf.EqualizeScale();
			delete pfData;

			sMessage="Can not open the file. Please check if this file is being used by other applications.";
			CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
			pch="pClamp Data Files (*.dat)\0*.dat\0\0";
			bFileDlg.m_ofn.lpstrFilter=pch;
			bFileDlg.m_ofn.lpstrTitle="Save the triple comparison into a data file";


			if(bFileDlg.DoModal()==IDOK)
			{
				sAbfFileName=bFileDlg.GetPathName();
				if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
				{
			//		AfxFormatString1(sMessage,-1,sAbfFileName);
					AfxMessageBox(sMessage,MB_OK,0);
				}
				else
				{
					m_cTripleChannelAbf.WriteData(fpAbfOut);
	   				fclose(fpAbfOut);
				}
			}			


		}
//		m_bTripleChannelAbf=true;
//	}

}

void CAbf_ProcessingDoc::OnAnalysisEventlistIgnoresmalltransitions() 
{
	// TODO: Add your command handler code here
	
	CIgnoreSmallTransitionOptionDialog aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		float fCutoff=aDlg.GetCutoff();
		m_cEvl.IgnoreSmallTransitions(fCutoff);
	}	
}

void CAbf_ProcessingDoc::OnAnalysisDatafileAbfabfcomparison() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

	if(!m_bDualChannelAbf)
	{
		char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
		if(aFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
//				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import the first data file";
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}

	AbfNode aNode=m_cAbf.GetTrace();

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Import the second data file";
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}

	AbfNode bNode=m_cAbf.GetTrace();
	AbfNode cNode=m_cDualChannelAbf.GetTrace();

	int nSize1=aNode.nSize/aNode.nNumChannels;
	int nSize2=bNode.nSize/bNode.nNumChannels;
	float *pfData=new float[nSize1+nSize2];
	int i;
	int nPosition=0;
	int nSize=nSize1;
	if(nSize>nSize2)
	{
		nSize=nSize2;
	}
	for(i=0;i<nSize;i++)
	{
			pfData[nPosition]=aNode.pfData[i*aNode.nNumChannels];
			nPosition++;
			pfData[nPosition]=bNode.pfData[i*bNode.nNumChannels]*cNode.fAutosampleAdditGain;
			nPosition++;
	}

	m_cDualChannelAbf.UpdateSize(2*nSize);
	m_cDualChannelAbf.UpdateData(pfData);
//	m_cDualChannelAbf.EqualizeScale();
	delete pfData;

	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	
	pch="pClamp Data Files (*.dat)\0*.dat\0\0";

	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	bFileDlg.m_ofn.lpstrFilter=pch;
	bFileDlg.m_ofn.lpstrTitle="Save the comparison into a data file";

	if(bFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=bFileDlg.GetPathName();
		m_sDualChannelAbfFileName=sAbfFileName;
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cDualChannelAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}		
	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistBurstdelimitertest() 
{
	// TODO: Add your command handler code here

		FILE *fpAbfOut;
		CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	structEvlSummaryNode bNode;
	if(bFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=bFileDlg.GetPathName();
		m_sDualChannelAbfFileName=sAbfFileName;
		if(!(fpAbfOut=fopen(sAbfFileName,"w")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			for(float fCutoff=10;fCutoff<5000.;fCutoff+=5)
			{
				m_cEvl.IgnoreShortClosing(fCutoff);
				m_cEvl.Summarize();
				bNode=m_cEvl.GetSummaryNode();
				structEvlSummaryNode bNode=m_cEvl.GetSummaryNode();
				fprintf(fpAbfOut,"%12.2f  %9d  %12.2f  \n",fCutoff,bNode.nNumEvents,bNode.fMeanOpenTime);
			}

	     	fclose(fpAbfOut);
		}
	}		
	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistMultipleevlbustdelimitertest() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here	
	CString sEvlFileName;
	CString sSummaryFileName;
	CString sFileNames;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

	vector <FloatVector> vectfvBPo;
	vector <FloatVector> vectfvBTo;
	vector <FloatVector> vectfvBTc;
	vector <FloatVector> vectfvPoib;
	vector <FloatVector> vectfvTcib;
	vector <IntVector> vectnvNumEvents;
	float fDeltaCutoff=5.f;
	int nIter;

	if(aFileDlg.DoModal()==IDOK)
	{
		sSummaryFileName=aFileDlg.GetPathName();
		FILE* fp=fopen(sSummaryFileName,"w");

		char* pch="Event List Files (*.EVL)\0*.EVL\0All Files\0*.*\0\0";
		CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrFile=sFileNames.GetBuffer(7200);
		aFileDlg.m_ofn.nMaxFile=7200;
		POSITION aPosition;

		int nNumFiles=0;
		int nIterations=0;

		float fMinLength=0.f;

		CIgnoreShortClosingDialog aDlg;
		float fCutoff;
		float fBPo=0.f,fBTo=0.f,fPoib=0.f,fBTc=0.f,fTc=0.f,fPo=0.f;
		float fTo=0.f,fTcib=0.f;
		vector <float> vfCutoff;

		if(aFileDlg.DoModal()==IDOK)
		{
			aPosition=aFileDlg.GetStartPosition();
			while(aPosition!=NULL)
			{
				nIterations=0;
				sEvlFileName=aFileDlg.GetNextPathName(aPosition);
				m_cEvl.ReadEvl(sEvlFileName);

				vector <float> vectfPo;
				vector <float> vectfTo;
				vector <float> vectfAmp;

				vector <float> vectfBPo;
				vector <float> vectfBTo;
				vector <float> vectfBTc;
				vector <float> vectfPoib;
				vector <float> vectfTc;
				vector <float> vectfTcib;
				IntVector vectnNumEvents;
				
				fDeltaCutoff=1.f;
				vfCutoff.clear();
				nIterations=0;
				int nNumEvents;

				for(fCutoff=0.f;fCutoff<1000001.f;fCutoff+=fDeltaCutoff)
				{
					nIterations++;
					m_cEvl.IgnoreShortClosing(fCutoff);
					vfCutoff.push_back(fCutoff);
					m_cEvl.Summarize();
					structEvlSummaryNode bNode=m_cEvl.GetSummaryNode();
					if(bNode.fRecordLength>fMinLength)
					{
						vectfBPo.push_back(bNode.fOpenProb);
						vectfBTo.push_back(bNode.fMeanOpenTime);
						fBPo=bNode.fOpenProb;
						fBTo=bNode.fMeanOpenTime;
						if(nIterations==1)
						{
							fPo=fBPo;
							fTo=fBTo;
							nNumFiles++;
						}
						fPoib=fPo/fBPo;
						fTcib=fTo*(1./fPoib-1);
						fBTc=fBTo*(1./fBPo-1);
						vectfPoib.push_back(fPoib);
						vectfBTc.push_back(fBTc);
						vectfTc.push_back(fTc);
						vectfTcib.push_back(fTcib);
						nNumEvents=bNode.nNumEvents;
						vectnNumEvents.push_back(nNumEvents);
					}
					if(nIterations>1&&((nIterations-1) % 100)==0)
						fDeltaCutoff*=10.;
				}

				vectfvBPo.push_back(vectfBPo);
				vectfvBTo.push_back(vectfBTo);
				vectfvBTc.push_back(vectfBTc);
				vectfvPoib.push_back(vectfPoib);
				vectfvTcib.push_back(vectfTcib);
				vectnvNumEvents.push_back(vectnNumEvents);
			}
		}
		
		vector <float> vectfPo;
		vector <float> vectfTo;
		vector <float> vectfAmp;

		vector <float> vectfBPo;
		vector <float> vectfBTo;
		vector <float> vectfBTc;
		vector <float> vectfPoib;
		vector <float> vectfTc;
		vector <float> vectfTcib;
		IntVector vectnNumEvents;
//		float fBPo,fBTo,fBTc,fPoib,fTcib;
		int nNumEvents=0;
		float fNumEvents=0.f;

		fprintf(fp,"     BPoMean        BPoSem       BToMean        BToSem       BTcMean        BTcSem      PoibMean       PoibSem      TcibMean       TcibSem             N             N       fCutoff    Name\n");		
		for(int i=0;i<nIterations;i++)
		{
			fCutoff=vfCutoff[i];
			vectfBPo.clear();
			vectfBTo.clear();
			vectfBTc.clear();
			vectfPoib.clear();
			vectfTcib.clear();
			vectnNumEvents.clear();

			for(int j=0;j<nNumFiles;j++)
			{
				fBPo=vectfvBPo[j].at(i);
				vectfBPo.push_back(fBPo);
				fBTo=vectfvBTo[j].at(i);
				vectfBTo.push_back(fBTo);
				fBTc=vectfvBTc[j].at(i);
				vectfBTc.push_back(fBTc);
				fPoib=vectfvPoib[j].at(i);
				vectfPoib.push_back(fPoib);
				fTcib=vectfvTcib[j].at(i);
				vectfTcib.push_back(fTcib);
				nNumEvents=vectnvNumEvents[j].at(i);
				vectnNumEvents.push_back(nNumEvents);
			}
			structSemNode BPoSem=CalSem(vectfBPo);
			structSemNode BToSem=CalSem(vectfBTo);
			structSemNode BTcSem=CalSem(vectfBTc);
			structSemNode PoibSem=CalSem(vectfPoib);
			structSemNode TcibSem=CalSem(vectfTcib);
			structSemNode NumEventsSem=CalSem(vectnNumEvents);

			if(i ==0)
			{
				fNumEvents=NumEventsSem.fMean;
			}

			fprintf(fp,"%12.3f  %12.5f  ",BPoSem.fMean,BPoSem.fSem);
			fprintf(fp,"%12.4f  %12.5f  ",
					BToSem.fMean,BToSem.fSem);
			fprintf(fp,"%12.4f  %12.5f  ",BTcSem.fMean/1000.,BTcSem.fSem/1000.);
			fprintf(fp,"%12.3f  %12.5f  ",PoibSem.fMean,PoibSem.fSem);
			fprintf(fp,"%12.4f  %12.5f  ",TcibSem.fMean/1000.,TcibSem.fSem/1000.);
			fprintf(fp,"%12.1f  ",NumEventsSem.fMean);
			fprintf(fp,"%12.8f  %12.4f  ",NumEventsSem.fMean/fNumEvents,fCutoff/1000.);
			fprintf(fp,"  %s \n",sSummaryFileName);
		}

		fclose(fp);
	}			
}

void CAbf_ProcessingDoc::OnFileAttachDatafile() 
{
	// TODO: Add your command handler code here
		FILE *fpAbfIn;
	CString sAbfFileName;
	int i;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");

		CAbf cNewAbf;
		cNewAbf.ReadData(fpAbfIn);
		AbfNode aNode;
		aNode=m_cAbf.GetTrace();
		int nSize1=aNode.nSize;
		AbfNode bNode;
		bNode=cNewAbf.GetTrace();
		int nSize2=bNode.nSize;

		AbfNode NewNode;
		CombiningDataNodes(aNode, bNode, NewNode);

		m_cAbf.UpdateData(NewNode);
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportsegmentsbatch() 
{
	// TODO: Add your command handler code here
		CString sFileNames;
		char* pch="Input File (*.inp)\0*.inp\0All Files\0*.*\0\0";
		CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrFile=sFileNames.GetBuffer(7200);
		aFileDlg.m_ofn.nMaxFile=7200;
		POSITION aPosition;
		CString sInputFileName;
		CString sOutputFileName;
		CString sFieldName;
		CString sEvlFileName;
		int i,nNumChannels,nNumSegments;
		float fTime1, fTime2;
		FILE* fpInput, *fpOutput;
		char* pcFieldName=new char[2000];

		if(aFileDlg.DoModal()==IDOK)
		{
			aPosition=aFileDlg.GetStartPosition();
			while(aPosition!=NULL)
			{
				sInputFileName=aFileDlg.GetNextPathName(aPosition);
				fpInput=fopen(sInputFileName,"r");
				while(!feof(fpInput))
				{
					fscanf(fpInput,"%s%s%s%d",pcFieldName, sEvlFileName, pcFieldName, &nNumSegments);
					m_cEvl.ReadEvl(sEvlFileName);
					for(i=0;i<nNumSegments;i++)
					{
						fscanf(fpInput,"%s%f%s%f%s%d%s%s",pcFieldName, &fTime1,
							                              pcFieldName, &fTime2, pcFieldName, &nNumChannels, pcFieldName, sOutputFileName);
						if(!(fpOutput=fopen(sOutputFileName,"wb")))
						{
							CString sMessage="Error occurred when opening output file:";
							sMessage+=sOutputFileName;
							AfxMessageBox(sMessage,MB_OK,0);
							sMessage=sOutputFileName;
							AfxMessageBox(sMessage,MB_OK,0);
						}
						else
						{
							m_cEvl.OutputSegment(fpOutput,sOutputFileName,fTime1,fTime2,nNumChannels);
							fclose(fpOutput);
						}
					}
				}
			}
		}	
}


void CAbf_ProcessingDoc::OnAnalysisDatafileAbftodatconversion() 
{
	// TODO: Add your command handler code here
	CString sTitle="Import the abf format data file";
	OnFileImportDatafile(sTitle);
	sTitle="Convert:\n";
	sTitle+=m_sAbfFileName;
	OnFileExportDatafile(sTitle);	
}

CString CAbf_ProcessingDoc::ChangeFileExt(CString sFileName, CString sExt)
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

void CAbf_ProcessingDoc::OnAnalysisEventlistExporthistogram() 
{
	// TODO: Add your command handler code here
	FILE *fpHist;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Histogram Files (*.Hst)\0*.Hst\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Output Histograms";
	CString sFileName=ChangeFileExt(m_sEvlFileName,"Hst");
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);


	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpHist=fopen(sAbfFileName,"w")))
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			m_cEvl.ExportHistograms(fpHist);	
	     	fclose(fpHist);
		}
	}	
}


void CAbf_ProcessingDoc::OutputHistSum(vector <structEvlHistogramNode> structHistSum, CString sSummaryFileName)
{
	CString sNewFileName=ChangeFileExt(sSummaryFileName,"Fst");
	int nNumFiles=structHistSum.size();
	int nMaxChannels=0;
	int i,j,k,nTemp;
	for(i=0;i<nNumFiles;i++)
	{
		nTemp=structHistSum[i].nMaxLevel;
		if(nTemp>nMaxChannels)
		{
			nMaxChannels=nTemp;
		}
	}

	FILE* fpHist;
	if(!(fpHist=fopen(sNewFileName,"w")))
	{
		CString sMessage="Can not open the Hst file.";
		AfxMessageBox(sMessage,MB_OK,0);
	}

	int nNumBins=structHistSum[0].nNumBinsLog;
	float fMinLog=structHistSum[0].fMinLog;
	float fDeltaLog=structHistSum[0].fDeltaLog;
	int nN=0;
	structSemNode aSemNode;
	vector <float> afVector;
	float fTime,fTemp,fLength;
	fprintf(fpHist,"    log(t)");
	for(i=0;i<=nMaxChannels;i++)
	{
		fprintf(fpHist,"         Mean%1d        Sem%1d   nN%1d",i,i,i);
	}
	fprintf(fpHist,"\n");

	for(i=0;i<nNumBins;i++)
	{
		fTime=fMinLog+float(i+1)*fDeltaLog;
		fprintf(fpHist,"%10.2f",fTime);

		afVector.clear();
		for(k=0;k<nNumFiles;k++)
		{
			fLength=structHistSum.at(k).fRecordLength/1000.;
			nTemp=structHistSum.at(k).nNumOfChannels;
			if(nTemp==1)
			{
				fTemp=structHistSum.at(k).pfvectHistLog.at(0)[i]/fLength;
				afVector.push_back(fTemp);
			}
		}
		aSemNode=CalSem(afVector);
		nN=aSemNode.nSampleSize;
		fprintf(fpHist,"  %12.6f  %10.6f    %2d",aSemNode.fMean,aSemNode.fSem,nN);

		for(j=1;j<=nMaxChannels;j++)
		{
			afVector.clear();
			for(k=0;k<nNumFiles;k++)
			{
				fLength=structHistSum.at(k).fRecordLength/1000.;
				nTemp=structHistSum.at(k).nNumOfChannels;
				if(nTemp>=j)
				{
					fTemp=structHistSum.at(k).pfvectHistLog.at(j)[i]/fLength;
					afVector.push_back(fTemp);
				}
			}
			aSemNode=CalSem(afVector);
			nN=aSemNode.nSampleSize;
			fprintf(fpHist,"  %12.6f  %10.6f    %2d",aSemNode.fMean,aSemNode.fSem,nN);
		}
		fprintf(fpHist,"\n");
	}
	fclose(fpHist);

	//Now output the sum of all histograms

	sNewFileName=ChangeFileExt(sSummaryFileName,"Hst");
	if(!(fpHist=fopen(sNewFileName,"w")))
	{
		CString sMessage="Can not open the Hst file.";
		AfxMessageBox(sMessage,MB_OK,0);
	}

	nN=0;
	afVector.clear();

	fprintf(fpHist,"    log(t)");
	for(i=0;i<=nMaxChannels;i++)
	{
		fprintf(fpHist,"        Level%1d   nN%1d",i,i);
	}
	fprintf(fpHist,"\n");

	for(i=0;i<nNumBins;i++)
	{
		fTime=fMinLog+float(i+1)*fDeltaLog;
		fprintf(fpHist,"%10.2f",fTime);

		nN=0;
		afVector.clear();
		for(k=0;k<nNumFiles;k++)
		{
			nTemp=structHistSum.at(k).nNumOfChannels;
			if(nTemp==1)
			{
				fTemp=structHistSum.at(k).pfvectHistLog.at(0)[i];
				afVector.push_back(fTemp);
				nN++;
			}
		}

		fTemp=0.f;

		for(k=0;k<nN;k++)
		{
			fTemp+=afVector[k];
		}

		fprintf(fpHist,"  %12.1f    %2d",fTemp,nN);

		for(j=1;j<=nMaxChannels;j++)
		{
			afVector.clear();
			nN=0;
			for(k=0;k<nNumFiles;k++)
			{
				nTemp=structHistSum.at(k).nNumOfChannels;
				if(nTemp>=j)
				{
					fTemp=structHistSum.at(k).pfvectHistLog.at(j)[i];
					afVector.push_back(fTemp);
					nN++;
				}
			}

			fTemp=0.f;

			for(k=0;k<nN;k++)
			{
				fTemp+=afVector[k];
			}

			fprintf(fpHist,"  %12.1f    %2d",fTemp,nN);

		}
		fprintf(fpHist,"\n");
	}
	fclose(fpHist);
}
//#include "NumeircalRecipes.h"
void CAbf_ProcessingDoc::OnAnalysisTestTestnr()
{
	int i,j,k;
	CString sFileName;
	CString sForBuffer;

	FILE *fp;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Text files (*.txt)\0*.txt\0\0";
	CString sTitle="Input a txt file";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle=sTitle;

	DP d, zd, probd, rs, probrs;
	vector <double> vectdX1, vectdY1;
	vector <double> vectdX2, vectdY2;
	vector <double> vectdX, vectdY;
	typedef vector <double> dVect ;
	vector <dVect> dVectData;
	int nRows,nCollomns;
	float dx,dy;
	int nLength=0,nLength0=0;
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
		fp=fopen(sFileName,"r");
		fscanf(fp,"%d%d",&nRows,&nCollomns);
		for(i=0;i<nRows;i++)
		{
			vector <double> dVector;
			for(j=0;j<nCollomns;j++)
			{
				dVector.push_back(0.);
			}
			dVectData.push_back(dVector);
		}

		for(i=0;i<nRows;i++)
		{
			for(j=0;j<nCollomns;j++)
			{
				fscanf(fp,"%f",&dx);
				dVectData[i][j]=dx;
			}
		}

		int nSize=vectdX.size();
//		Vec_I_DP data1(nSize),data2(nSize);//constant double nr array

		fp=fopen("correlation_output.txt","w");

		Vec_DP data1(nRows),data2(nRows); //double nr array
		double dX=0.;
		for(i=0;i<2;i++)
		{
			for(k=0;k<nRows;k++)
			{
				dx=dVectData[k][i];
				data1[k]=dx;
			}

			for(j=i+1;j<nCollomns;j++)
			{
				rs=0.;
				probrs=-1.;
				probd=-1.;
				zd=0.;
				d=0.;

				for(k=0;k<nRows;k++)
				{
				dx=dVectData[k][j];
				data2[k]=dx;
				}

				NR::spear(data1, data2, d, zd, probd, rs, probrs);
				fprintf(fp,"	rs: %8.4f    probrs:  %12.8f i=%2d j=%2d",rs,probrs,i,j);
//				NR::pearsn(data1,data2,rs,probd,zd);
//				fprintf(fp,"	r: %8.4f    probd:  %12.8f i=%2d j=%2d",rs,probd,i,j);
			}
			fprintf(fp,"\n");

		}
		fclose(fp);
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistFittingdwelltimebatch() 
{
	// TODO: Add your command handler code here
	CString sInputFileName;
	CString sOutputFileName;
	CString sEvlFileName;
	CString sBinMethod;
	//The function GetLength() return a fixed value which is the length of the initialized string value.
	//Therefore, you can not initialize a string if you need to use GetLength function after reassign 
	//a new value to the string variable. 

	int nFuncKey=1; //1=Log, 2=AP, 3=HB
	CEvlHistogramDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		nFuncKey=aDlg.GetHistType();
	}

	switch (nFuncKey)
	{
	case 1:
		sBinMethod="_Log";
		break;
	case 2:
		sBinMethod="_AP";
		break;
	case 3:
		sBinMethod="_Hybrid";
		break;
	}



	int i,j,nNumComponents,nLevel,nNumTrials;
	float fLeft,fRight;

	FILE* fpInput, *fpOutput;

	char* pcFieldName=new char[2000];
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	char* pch="Well Fitting Trial File (*.Dwt)\0*.Dwt\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input a Dwell Fitting Trial File";
	float fTimeConstant, fWeight;

	if(aFileDlg.DoModal()==IDOK)
	{

		sInputFileName=aFileDlg.GetPathName();
		fpInput=fopen(sInputFileName,"r");

		CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
		CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
		char* pch="Dwell-time Fitting Result(*.Fit)\0*.Fit\0\0";
		bFileDlg.m_ofn.lpstrFilter=pch;
		bFileDlg.m_ofn.lpstrTitle="Output Dwell-time Fitting Results";
		sOutputFileName=ChangeFileExt(sInputFileName,"Fit");
		strcpy(bFileDlg.m_ofn.lpstrFile,sOutputFileName);
		int nSize=0,nTrial=0;

		if(bFileDlg.DoModal()==IDOK)
		{
			sOutputFileName=bFileDlg.GetPathName();
			sOutputFileName=m_cEvl.ExtendFileName(sOutputFileName,sBinMethod);
			if(!(fpOutput=fopen(sOutputFileName,"w")))
			{
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				CTime startTime = CTime::GetCurrentTime();
				OutputFormattedTime(fpOutput,"Started at:  ", startTime);

				vector <structDwellFittingTrialNode> avTrialNode;
				vector <structDwellFittingTrialNode> bvTrialNode;
				vector <structFittingResultNode> avResultNode;

				while(!feof(fpInput))
				{
					fscanf(fpInput,"%s%s%s%d",pcFieldName, sEvlFileName, pcFieldName, &nNumTrials);
					m_cEvl.ReadEvl(sEvlFileName);

					for(i=0;i<nNumTrials;i++)
					{
						avResultNode.clear();
						avTrialNode.clear();
						avTrialNode.clear();
						
						fscanf(fpInput,"%s%d%s%d%s%f%s%f",pcFieldName, &nLevel, pcFieldName, &nNumComponents
														 ,pcFieldName,&fLeft,pcFieldName,&fRight);
						structDwellFittingTrialNode aTrialNode;
						aTrialNode.fFittingRange.fLeft=fLeft;
						aTrialNode.fFittingRange.fRight=fRight;
						aTrialNode.sEvlFileName=sEvlFileName;
						aTrialNode.nDwellLevel=nLevel;
						aTrialNode.nNumComponents=nNumComponents;
						aTrialNode.nTrialNumber=0;

						for(j=0;j<nNumComponents;j++)
						{
							fscanf(fpInput,"%f%f",&fWeight,&fTimeConstant);
							aTrialNode.fTimeConstants[j]=fTimeConstant;
							aTrialNode.fWeights[j]=fWeight;
						}

						for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
						{
							aTrialNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
							aTrialNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
						}

						structFittingResultNode aResultNode=m_cEvl.FitEvl_Transform(nFuncKey, aTrialNode);
						avResultNode.push_back(aResultNode);
						bvTrialNode.push_back(aTrialNode);

						aResultNode.nTrialNumber=nTrial;

						aTrialNode.fFittingRange.fRight=aResultNode.fFittingRange.fRight;
						avTrialNode=GetDwellFittingTrials(aTrialNode);
//						nSize=avTrialNode.size();
						nSize=0;
						for(nTrial=0;nTrial<nSize;nTrial++)
						{
							aTrialNode=avTrialNode.at(nTrial);
							aTrialNode.nTrialNumber=nTrial+1;
							structFittingResultNode aResultNode=m_cEvl.FitEvl_Transform(nFuncKey, aTrialNode);
							avResultNode.push_back(aResultNode);
							bvTrialNode.push_back(aTrialNode);

						}
						nSize=avResultNode.size();
						int nIndex=0;
						OrderFittingResults(avResultNode);
						ExportDwellCurves(avResultNode);
						for(nIndex=0;nIndex<nSize;nIndex++)
						{
							OutputFittingResults(bvTrialNode.at(nIndex),avResultNode.at(nIndex),fpOutput);
						}
					}
				}
				fclose(fpInput);
				
				OutputFormattedTime(fpOutput,"Started at:  ", startTime);
				CTime endTime = CTime::GetCurrentTime();
				CTimeSpan elapsedTime = endTime - startTime;
				OutputFormattedTime(fpOutput, "Finished at:  ", endTime);
				OutputFormattedTimeSpan(fpOutput, "Elapsed Time: ", elapsedTime);

				fclose(fpOutput);
			}
		}
	}
}


vector <structDwellFittingTrialNode> CAbf_ProcessingDoc::GetDwellFittingTrials_Original(structDwellFittingTrialNode aTrialNode)
{
	vector <structDwellFittingTrialNode> avTrialNode;
	int nComponents=aTrialNode.nNumComponents;
	int nDwellLevel=aTrialNode.nDwellLevel;
	CString sFileName=aTrialNode.sEvlFileName;
	double dTmin=0.,dTmax=0.;
	structFRange fFittingRange=aTrialNode.fFittingRange;
	dTmin=aTrialNode.fFittingRange.fLeft;
	dTmax=aTrialNode.fFittingRange.fRight;
	double dT[20];
	double dW[20];

	if(nComponents==1) 
	{
		structDwellFittingTrialNode aNewTrialNode;
		aNewTrialNode.fTimeConstants[0]=.5*(dTmin+dTmax);
		aNewTrialNode.fWeights[0]=1.;
		aNewTrialNode.nTrialNumber=1;
		aNewTrialNode.nNumComponents=nComponents;
		aNewTrialNode.nDwellLevel=nDwellLevel;
		aNewTrialNode.sEvlFileName=sFileName;
		aNewTrialNode.fFittingRange=fFittingRange;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aNewTrialNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
			aNewTrialNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
		}

		avTrialNode.push_back(aNewTrialNode);
		return avTrialNode;
	}

	double dLeftArray[10]={0.5,1.,2.,3.,4.,16.,32.,0.1,10.,20.};
	double dRightArray0[10]={0.7,0.3,0.1,0.05,0.05,8.,40.,20.,15.};
	double dRightArray1[10]={0.2,0.1,0.05,0.05,0.05,8.,40.,20.,15.};
	const int nLeft=3,nRight0=3,nRight1=3;
	int i,j,k,l;
	double dLeft,dRight0,dRight1,dDelta;

	for(i=0;i<nComponents;i++)
	{
		dW[i]=1./double(nComponents);
	}
	
	double dLog10=log(10.);
	double dTemp=1.;
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
			if(nComponents>=3)
			{
				for(k=0;k<nRight1;k++)
				{
					dRight1=dRight0*dRightArray1[k];
					dT[nComponents-2]=dRight1;
					dRight1=log(dRight1)/dLog10;
					for(l=1;l<nComponents-2;l++)
					{
						dDelta=(dRight1-dLeft)/double(nComponents-2);
						dTemp=dLeft+dDelta*double(l);
						dT[l]=exp(dTemp*dLog10);
					}
					structDwellFittingTrialNode aNode;
					for(l=0;l<nComponents;l++)
					{
						aNode.fTimeConstants[l]=dT[l];
						aNode.fWeights[l]=dW[l];
					}

					for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
					{
						aNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
						aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
					}

					nTrialNumber++;
					aNode.nTrialNumber=1;
					aNode.nNumComponents=nComponents;
					aNode.nDwellLevel=nDwellLevel;
					aNode.sEvlFileName=sFileName;
					aNode.fFittingRange=fFittingRange;
					avTrialNode.push_back(aNode);
				}
			}
			else
			{
				structDwellFittingTrialNode aNode;
				for(l=0;l<nComponents;l++)
				{
					aNode.fTimeConstants[l]=dT[l];
					aNode.fWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}


				nTrialNumber++;
				aNode.nTrialNumber=nTrialNumber;
				aNode.nNumComponents=nComponents;
				aNode.nDwellLevel=nDwellLevel;
				aNode.sEvlFileName=sFileName;
				aNode.fFittingRange=fFittingRange;
				avTrialNode.push_back(aNode);
			}
		}
	}
	return avTrialNode;
}

vector <structDwellFittingTrialNode> CAbf_ProcessingDoc::GetDwellFittingTrials(structDwellFittingTrialNode aTrialNode)
{
	vector <structDwellFittingTrialNode> avTrialNode;
	int nComponents=aTrialNode.nNumComponents;
	int nDwellLevel=aTrialNode.nDwellLevel;
	CString sFileName=aTrialNode.sEvlFileName;
	double dTmin=0.,dTmax=0.;
	structFRange fFittingRange=aTrialNode.fFittingRange;
	dTmin=aTrialNode.fFittingRange.fLeft;
	int nFittingLevel=aTrialNode.nDwellLevel;
	dTmax=m_cEvl.GetLongestLevelLength(fFittingRange, nFittingLevel);

	double dT[20];
	double dW[20];

	if(nComponents==1) 
	{
		structDwellFittingTrialNode aNewTrialNode;
		aNewTrialNode.fTimeConstants[0]=.5*(dTmin+dTmax);
		aNewTrialNode.fWeights[0]=1.;
		aNewTrialNode.nTrialNumber=1;
		aNewTrialNode.nNumComponents=nComponents;
		aNewTrialNode.nDwellLevel=nDwellLevel;
		aNewTrialNode.sEvlFileName=sFileName;
		aNewTrialNode.fFittingRange=fFittingRange;

		for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
		{
			aNewTrialNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
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
					structDwellFittingTrialNode aNode;
					for(l=0;l<nComponents;l++)
					{
						aNode.fTimeConstants[l]=dT[l];
						aNode.fWeights[l]=dW[l];
					}

					for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
					{
						aNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
						aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
					}

					nTrialNumber++;
					aNode.nTrialNumber=1;
					aNode.nNumComponents=nComponents;
					aNode.nDwellLevel=nDwellLevel;
					aNode.sEvlFileName=sFileName;
					aNode.fFittingRange=fFittingRange;
					avTrialNode.push_back(aNode);
				}
			}
			else
			{
				structDwellFittingTrialNode aNode;
				for(l=0;l<nComponents;l++)
				{
					aNode.fTimeConstants[l]=dT[l];
					aNode.fWeights[l]=dW[l];
				}

				for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
				{
					aNode.bFreezeT[nTempIndex]=aTrialNode.bFreezeT[nTempIndex];
					aNode.bFreezeW[nTempIndex]=aTrialNode.bFreezeW[nTempIndex];
				}


				nTrialNumber++;
				aNode.nTrialNumber=nTrialNumber;
				aNode.nNumComponents=nComponents;
				aNode.nDwellLevel=nDwellLevel;
				aNode.sEvlFileName=sFileName;
				aNode.fFittingRange=fFittingRange;
				avTrialNode.push_back(aNode);
			}
		}
	}
	return avTrialNode;
}

void CAbf_ProcessingDoc::OutputFittingResults(structDwellFittingTrialNode aTrialNode, structFittingResultNode aResultNode, FILE* fpOutput)
{
	CString sBinMethod;
	aTrialNode=aResultNode.aTrialNode;
	int nLargestBin=1;
	int j;
	int nNumComponents=aResultNode.nNumComponents;
	int nSignificantComponents=aResultNode.nSignificantComponents;
	fprintf(fpOutput,"%s\n",aResultNode.sFileName);
	fprintf(fpOutput,"Total Record Length (s): %10.4f  NumEvents:  %8d  nMaxLevel: %d   nNumChannels:  %d\n",
		aResultNode.fRecordLength,aResultNode.nNumEvents,aResultNode.nMaxLevel,
		aResultNode.nNumChannels);
	fprintf(fpOutput,"Po: %8.4f,   To (ms): %8.1f,  1-Prob_Sigle: %12.4e\n",
		aResultNode.fOpenProb,aResultNode.fMeanOpenTime,
		aResultNode.fProbSingle);
	fprintf(fpOutput,"CurFile:  %s\n",aResultNode.sCurFileName);
	fprintf(fpOutput,"Bin Method:  %s        Largest Count: %d\n",sBinMethod, nLargestBin);
	fprintf(fpOutput,"Score Ranking: %4d        Trials:  %4d        Cluster Index: %4d\n",aResultNode.nScoreRanking,aResultNode.nTrialNumber, aResultNode.nClusterIndex);

	fprintf(fpOutput,"SignificantComponents:  %2d        Total Events:  %12d\n", nSignificantComponents, aResultNode.nNumEvents);	
	fprintf(fpOutput,"Level:  %2d        Components:  %2d \n",aResultNode.nDwellLevel,aResultNode.nNumComponents);	
	fprintf(fpOutput,"FittingRange(ms):  %7.4f  to  %14.3f\n",aResultNode.fFittingRange.fLeft,
															  aResultNode.fFittingRange.fRight);
	fprintf(fpOutput,"Fitted_Area:  %8.5e            Events_within_Range:   %10d\n",
				aResultNode.dFittedArea,aResultNode.nEventsInRange);
	fprintf(fpOutput,"\n");

	fprintf(fpOutput,"       AreaL       Area        Area RTimeConstL RTimeConst  RTimeConstR\n");
	
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"%8.4e    %8.4e    %8.4e    %8.4e    %8.4e    %8.4e\n",
			aResultNode.fConfidanceRangeW[j].fLeft,aResultNode.dWeights[j], aResultNode.fConfidanceRangeW[j].fRight,
			aResultNode.fConfidanceRangeT[j].fLeft,aResultNode.dTimeConstants[j],aResultNode.fConfidanceRangeT[j].fRight);
	}


	fprintf(fpOutput,"   dLogLikWL   dLogLikWR   dLogLikTL   dLogLikTR\n");
	
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"%8.4e    %8.4e    %8.4e    %8.4e    %8.4e    %8.4e\n",
			aResultNode.fConfidanceRangeWL[j].fLeft, aResultNode.dLogLikelihood, aResultNode.fConfidanceRangeWL[j].fRight,
			aResultNode.fConfidanceRangeTL[j].fLeft, aResultNode.dLogLikelihood, aResultNode.fConfidanceRangeTL[j].fRight);
	}

	fprintf(fpOutput,"       AreaL_I       Area        AreaR_I RTimeConstL_I RTimeConst  RTimeConstR_I\n");
	
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"%8.4e    %8.4e    %8.4e    %8.4e    %8.4e    %8.4e\n",
			aResultNode.fConfidanceRangeWI[j].fLeft,aResultNode.dWeights[j], aResultNode.fConfidanceRangeWI[j].fRight,
			aResultNode.fConfidanceRangeTI[j].fLeft,aResultNode.dTimeConstants[j],aResultNode.fConfidanceRangeTI[j].fRight);
	}


	fprintf(fpOutput,"   dLogLikWL_I   dLogLikWR_I   dLogLikTL_I   dLogLikTR_I\n");
	
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"%8.4e    %8.4e    %8.4e    %8.4e    %8.4e    %8.4e\n",
			aResultNode.fConfidanceRangeWLI[j].fLeft, aResultNode.dLogLikelihood, aResultNode.fConfidanceRangeWLI[j].fRight,
			aResultNode.fConfidanceRangeTLI[j].fLeft, aResultNode.dLogLikelihood, aResultNode.fConfidanceRangeTLI[j].fRight);
	}

//	fprintf(fpOutput,"TimeConstants(ms)ANDWeights:  ");
//	for(j=0;j<nNumComponents;j++)
//	{
//		fprintf(fpOutput,"  %15.5f",dTimeConstants[j]);
//	}
//	for(j=0;j<nNumComponents;j++)
//	{
//		fprintf(fpOutput,"   %8.6f",dWeights[j]);
//	}
//	fprintf(fpOutput,"  %10.4f    %s",aResultNode.dMean,aResultNode.sFileName);
//	fprintf(fpOutput,"\n");

	fprintf(fpOutput,"Burst Delimiters:  ");

	double dTemp;
	for(j=1;j<nNumComponents;j++)
	{
		dTemp=BurstDelimiter(aResultNode.dTimeConstants[j-1], aResultNode.dTimeConstants[j]);
		fprintf(fpOutput,"  %12.4f",dTemp);
	}
	fprintf(fpOutput,"\n");


	fprintf(fpOutput,"Trial Time Constants(ms):  ");
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"  %14.5f",aTrialNode.fTimeConstants[j]);
	}
	fprintf(fpOutput,"\nTrial Weight            :  ");
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"  %14.5f",aTrialNode.fWeights[j]);							
	}

	fprintf(fpOutput,"\n");
	fprintf(fpOutput,"Mean Dwell Time(ms):  %12.4f\n",aResultNode.dMean);
	fprintf(fpOutput,"Number of Iterations:  %10d\n",aResultNode.nNumIterations);
	fprintf(fpOutput,"Log Likelihood:   %17.6f\n",aResultNode.dLogLikelihood);
	fprintf(fpOutput,"Log Likelihood_R:   %17.6f\n",aResultNode.dLogLikelihood_R);
	fprintf(fpOutput,"Mean Binned Dwell Time (Obs and Cal in ms):  %12.4f    %12.4f    \n Mean Dwell Time In Fitted Range(Obs and Cal in ms):    %12.4f    %12.4f\n",
		aResultNode.dMeanDwellTimeH,aResultNode.dMeanDwellTimeHC,aResultNode.dMeanDwellTimeO,aResultNode.dMeanDwellTimeC);
	fprintf(fpOutput,"SDev:  %12.4f     ChiSquare:  %12.4f    SSE:  %12.4f\n",aResultNode.dSDev,aResultNode.dChiSQ,aResultNode.dSSE);
	fprintf(fpOutput,"\n\n\n");
}

void CAbf_ProcessingDoc::OutputFormattedTime(FILE *fpOutput, CString sComments, CTime aTime)
{
	CString sTemp = aTime.Format( "%H:%M:%S On " );
	fprintf(fpOutput,"%s  %s ",sComments,sTemp);
	sTemp = aTime.Format( "%A, %B %d, %Y" );
	fprintf(fpOutput,"%s  \n",sTemp);
}

void CAbf_ProcessingDoc::OutputFormattedTimeSpan(FILE *fpOutput, CString sComments, CTimeSpan aTime)
{
	CString sTemp = aTime.Format( "%D Days, %H Hours, %M Minutes and %S Seconds \n" );
	fprintf(fpOutput,"%s  %s ",sComments,sTemp);
}

CString CAbf_ProcessingDoc::ConvertToString(int nInt)
{
	CString sTring="";
	char cChar;
	int nQuotient=nInt/10;
	int nRemainder=(nInt%10);
	cChar=nRemainder+48;
	sTring.Insert(0,cChar);
	while(nQuotient>0)
	{
		nRemainder=(nQuotient%10);
		cChar=nRemainder+48;
		sTring.Insert(0,cChar);
		nQuotient/=10;
	}
	return sTring;
}

void CAbf_ProcessingDoc::OnAnalysisEventlistFittingdwelltimeautomatic() 
{
	// TODO: Add your command handler code here
	//Attention!
	//This compiler assign the same memory address for all CString objects created in the same scope.
	//One way to get arround it is pass it to a function and get it back as a returned value of a function.
	//The same problem happed if you use a pointer and using new char[] commond. You have to use new char[] commond 
	//without delete the pointor in order to create an independt string, with the price of cause memory leakage.
	int nMaxComponents=9; 
	const int nMinComponents=1; 
	CString sInputFileName;
	CString sOutputFileName;
	CString sBinMethod;

	int nFuncKey=1; //1=Log, 2=AP, 3=HB
	CEvlHistogramDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		nFuncKey=aDlg.GetHistType();
		m_bFilterEvl=aDlg.FilterEvl();
		m_nFilteringKey=aDlg.FilteringKey();
		m_bCalConfidanceRange=aDlg.CalConfidanceRanges();
		m_bFullIteration=aDlg.FullIteration();
	}

	switch (nFuncKey)
	{
	case 1:
		sBinMethod="_Log";
		break;
	case 2:
		sBinMethod="_AP";
		break;
	case 3:
		sBinMethod="_Hybrid";
		break;
	}
	
	int i,j,nNumComponents;
	FILE* fpInput, *fpOutput;


	char* pcFieldName=new char[2000];

	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	char* pch="Files that contain list of evl files (*.Lst)\0*.Lst\0All Files\0*.*\0\0";
	CString sForBuffer;
	aFileDlg.m_ofn.lpstrFile=sForBuffer.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input a list of Evl files for dwell-time fitting";
//	vector <CEvl> avEvl;
	POSITION aPosition;
	if(aFileDlg.DoModal()==IDOK)
	{
		aPosition=aFileDlg.GetStartPosition();

		while(aPosition!=NULL)
		{
			sInputFileName=aFileDlg.GetNextPathName(aPosition);
			fpInput=fopen(sInputFileName,"r");
			FILE* fpAllResults;

			CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
//			CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
//			char* pch="Automated Dwell-time Fitting Result(*.Aft)\0*.Aft\0\0";
//			bFileDlg.m_ofn.lpstrFilter=pch;
//			bFileDlg.m_ofn.lpstrTitle="Output Dwell-time Fitting Results";
			sOutputFileName=ChangeFileExt(sInputFileName,"Aft");
//			strcpy(bFileDlg.m_ofn.lpstrFile,sOutputFileName);
			int nSize=0,nTrial=0;

//			if(bFileDlg.DoModal()==IDOK)
//			{
//				sOutputFileName=bFileDlg.GetPathName();
				sOutputFileName=m_cEvl.ExtendFileName(sOutputFileName,sBinMethod);
//				if(!(fpOutput=fopen(sOutputFileName,"w")))
//				{
//					AfxMessageBox(sMessage,MB_OK,0);
//				}
//				else
//				{
					structFittingResultNode aBestResultNode0, aBestResultNode1;
					fpOutput=fopen(sOutputFileName,"w");
					sOutputFileName=m_cEvl.ChangeExt(sOutputFileName,"dlt");
					FILE* fpBinaryResults=fopen(sOutputFileName,"wb");
					FILE* fpCorrectFittings=fopen("CorrectFittings.txt","w");
					CTime startTime = CTime::GetCurrentTime();
					OutputFormattedTime(fpOutput,"Started at:  ", startTime);
					fprintf(fpOutput,"\n");

					vector <int> vnLevelsToFit;
					vector <structDwellFittingTrialNode> avTrialNode;
					vector <structDwellFittingTrialNode> bvTrialNode;
					int nNumLevels=0;
					int nDwellLevel=0;
					bool bSignificant=true;
					float fBurstDelimiter, fClusterDelimiter;
					int nDf;
					double dProb, dL;
					vector <structFittingResultNode> vBestResultNode;
					vector <structDwellFittingTrialNode> vBestTrialNode;
					structFRange fFittingRange;
					vector <structFittingResultsCollectionNode> avResultsCollections;
					while(!feof(fpInput))
					{
						//Iteration at evl file level
						vnLevelsToFit.clear();
						CString sEvlFileName;
						fscanf(fpInput,"%s%s%s%d",pcFieldName, sEvlFileName, pcFieldName, &nNumLevels);

						structFittingResultsCollectionNode aResultsCollection;
						aResultsCollection.sEvlFileName=PassingString(sEvlFileName);
						aResultsCollection.nNumFittingLevels=nNumLevels;

						m_cEvl.ReadEvl(sEvlFileName);

						fBurstDelimiter=0.;
						fClusterDelimiter=10000000.f;
						if(m_bFilterEvl) 
							m_cEvl.FilteringEvents(m_nFilteringKey,fBurstDelimiter, fClusterDelimiter);
						else 
							m_nFilteringKey=-1;

						fprintf(fpOutput,"Filtering_Key:  %2d    \n", m_nFilteringKey);
						if(m_bFilterEvl) 
							fprintf(fpOutput,"Burst_Delimiter:  %10.2f          Cluster_Delimiter:  %10.2f",fBurstDelimiter, fClusterDelimiter);
						fprintf(fpOutput,"\n\n");


						sEvlFileName=m_cEvl.ExtendFileName(sEvlFileName,sBinMethod);
						fpAllResults=fopen(m_cEvl.ChangeExt(sEvlFileName,"Aft"),"a");	
						
						OutputFormattedTime(fpAllResults,"Started at:  ", startTime);
						fprintf(fpAllResults,"\n\n");

						fprintf(fpAllResults,"  Filtering Key:  %2d    \n", m_nFilteringKey);
						if(m_bFilterEvl) 
							fprintf(fpAllResults,"Burst Delimiter:  %10.2f          Cluster Delimiter:  %10.2f",fBurstDelimiter, fClusterDelimiter);

						fscanf(fpInput,"%s",pcFieldName);
						for(i=0;i<nNumLevels;i++)
						{
							fscanf(fpInput,"%d",&nDwellLevel);
							vnLevelsToFit.push_back(nDwellLevel);
						}
						TotalVVResults aTotalVVResults;
						int nLevelIndex=0;
						for(nLevelIndex=0;nLevelIndex <nNumLevels;nLevelIndex++)
						{
							//Iteration at the fitting-level
//							fFittingRange.fLeft=0.09f;//06411
							fFittingRange.fLeft=0.7f;
	//						fFittingRange.fLeft=0.009f;//For Sigworth
							fFittingRange.fRight=1000000.f;

							nDwellLevel=vnLevelsToFit.at(nLevelIndex);

							fFittingRange=m_cEvl.GetFittingRange(fFittingRange,nDwellLevel);
							vector <VectTrials> avTotalTrials;
							vector <VectResults> avTotalResults;
							vBestResultNode.clear();
							vBestTrialNode.clear();
							
							structDwellFittingTrialNode aTrialNode;
							vector <int> vnNumTrials;

	//						nNumComponents=1;
							nNumComponents=nMinComponents;//For Sigworth
							bSignificant=true;
							int nSigComponents=1;
//							nMaxComponents=5;
							if(nDwellLevel>0)
							{
								nMaxComponents=5; //Temporally
							}
							else
							{
								nMaxComponents=9;
							}
							while(bSignificant&&nNumComponents<=nMaxComponents)
//							while(nNumComponents<=nMaxComponents)
							{
								//Component Level
								vector <structFittingResultNode> avResultNode;
								avTrialNode.clear();
								aTrialNode.fFittingRange.fLeft=fFittingRange.fLeft;
								aTrialNode.fFittingRange.fRight=fFittingRange.fRight;
								aTrialNode.sEvlFileName=PassingString(m_cEvl.GetEvlFileName());

								for(int nTempIndex=0;nTempIndex<20;nTempIndex++)
								{
									aTrialNode.bFreezeT[nTempIndex]=false;
									aTrialNode.bFreezeW[nTempIndex]=false;
								}

								structFittingResultNode aNode;

								aTrialNode.nDwellLevel=nDwellLevel;
								aTrialNode.nNumComponents=nNumComponents;
								aTrialNode.nTrialNumber=0;

								aTrialNode.bConfidanceRange=true;
								avTrialNode=GetDwellFittingTrials(aTrialNode);

								nSize=avTrialNode.size();
								for(nTrial=0;nTrial<nSize;nTrial++)
								{
									aTrialNode=avTrialNode.at(nTrial);
									aTrialNode.nTrialNumber=nTrial;
									structFittingResultNode aResultNode=m_cEvl.FitEvl_Transform(nFuncKey, aTrialNode);
	//								structFittingResultNode aResultNode=m_cEvl.FitEvl(nFuncKey, aTrialNode);
									avResultNode.push_back(aResultNode);
								}

								nSize=avResultNode.size();
								OrderFittingResults(avResultNode);

								aBestResultNode0=aBestResultNode1;
								aBestResultNode1=avResultNode.at(0);

								ExportDwellCurves(avResultNode);
								if(nNumComponents>nMinComponents)
								{
									bSignificant=Significant(aBestResultNode0,aBestResultNode1,dL,nDf,dProb);
									fprintf(fpOutput,"\n Comparison of best fittings (Level-%1d) using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nDwellLevel,nNumComponents-1,nNumComponents,dL,nDf,dProb); 
									fprintf(fpAllResults,"\n Comparison of best fittings (Level-%1d) using %1d and %1d components: \n dL: %8.3f    nDf: %2d    p:  %10.6e\n\n\n\n",nDwellLevel,nNumComponents-1,nNumComponents,dL,nDf,dProb); 
									if(bSignificant)
									{
										nSigComponents=nNumComponents;
									}
								}
								if(m_bFullIteration)
								bSignificant=true;

	//							nIndex=0;
	//							OutputFittingResults(avTrialNode.at(nIndex),avResultNode.at(nIndex),fpOutput);
								nNumComponents++;
								avTotalTrials.push_back(avTrialNode);
								avTotalResults.push_back(avResultNode);
								vnNumTrials.push_back(avResultNode.size());
								//Component Level
							}
							//Iteration at the fitting-level
							vector <structFittingResultNode> avNode;
							nSize=vBestResultNode.size();
							int nMin=nSigComponents-2,nTemp=0;
							if(nMin<0) nMin=0;
							int nMin0=nSigComponents-2;
							if(nMin0<0) nMin0=0;
							nSize=avTotalResults.size();
							int nSize0=0;

							for(i=0;i<nSize;i++)
							{
								nSize0=avTotalResults.at(i).size();
								for(j=0;j<nSize0;j++)
								{
									avTotalResults.at(i).at(j).nSignificantComponents=nSigComponents;
								}
							}
							
							for(nTemp=nMin;nTemp<nSize;nTemp++)
							{
								//Iteration at the component level
								if(m_bCalConfidanceRange) SetConfidanceRange(nFuncKey, avTotalResults[nTemp]);
							}

							for(i=0;i<nSize;i++)
							{
								vBestResultNode.push_back(avTotalResults.at(i).at(0));
								vBestTrialNode.push_back(avTotalResults.at(i).at(0).aTrialNode);								
							}
							
							//Iteration at the fitting level
							for(nTemp=nMin;nTemp<nSize;nTemp++)
							{
								//Iteration at the component level

								structFittingResultNode cNode=vBestResultNode.at(nTemp);
								avNode.push_back(cNode);
								if(nTemp>=nMin0)
								{
									OutputFittingResults(vBestTrialNode.at(nTemp),vBestResultNode.at(nTemp),fpOutput);

									avTrialNode.clear();
									vector <structFittingResultNode> avResultNode;
									avTrialNode=avTotalTrials.at(nTemp);
									avResultNode=avTotalResults.at(nTemp);
									nSize0=avResultNode.size();
									int nIndex=0;
									for(nIndex=0;nIndex<nSize0;nIndex++)
									{
										OutputFittingResults(avTrialNode.at(nIndex),avResultNode.at(nIndex),fpAllResults);
									}

									fprintf(fpAllResults,"Score      Log(Lik)          SDev           SSE         ChiSQ     DwellTO      DeltaT    FittedArea\n");
									for(nIndex=0;nIndex<nSize0;nIndex++)
									{
										fprintf(fpAllResults,"  %3d  %12.3f  %12.4f  %12.4f  %12.4f  %10.2f  %10.2f  %12.5e\n",
											avResultNode.at(nIndex).nScoreRanking,avResultNode.at(nIndex).dLogLikelihood,
											avResultNode.at(nIndex).dSDev, avResultNode.at(nIndex).dSSE,
											avResultNode.at(nIndex).dChiSQ, avResultNode.at(nIndex).dMeanDwellTimeO,
											(avResultNode.at(nIndex).dMeanDwellTimeO-avResultNode.at(nIndex).dMeanDwellTimeC)
											,avResultNode.at(nIndex).dFittedArea);
									}							
								}
								//Iteration at the component level
							}

							aResultsCollection.aTotalVVResults.push_back(avTotalResults);
							aResultsCollection.vvnNumTrials.push_back(vnNumTrials);
							aResultsCollection.vnMaxComponents.push_back(nNumComponents-1);
							aResultsCollection.vnMinComponents.push_back(nMinComponents);
							aResultsCollection.vnSignificantComponents.push_back(nNumComponents-2);
							aResultsCollection.vnFittingLevels.push_back(nDwellLevel);

							nTemp=nSigComponents-1;
							if(nTemp<0) nTemp=0;
							OutputCorrectFittingResults(vBestResultNode.at(nTemp), fpCorrectFittings);
							ExportDwellCurves(avNode);
							//Iteration at the fitting level
						}
						OutputFormattedTime(fpAllResults,"Started at:  ", startTime);
						CTime endTime = CTime::GetCurrentTime();
						CTimeSpan elapsedTime = endTime - startTime;
						OutputFormattedTime(fpAllResults, "Finished at:  ", endTime);
						OutputFormattedTimeSpan(fpAllResults, "Elapsed Time: ", elapsedTime);
						fclose(fpAllResults);
						avResultsCollections.push_back(aResultsCollection);

						//Iteration at evl file level
					}
					fclose(fpInput);				
					OutputFormattedTime(fpOutput,"Started at:  ", startTime);
					CTime endTime = CTime::GetCurrentTime();
					CTimeSpan elapsedTime = endTime - startTime;
					OutputFormattedTime(fpOutput, "Finished at:  ", endTime);
					OutputFormattedTimeSpan(fpOutput, "Elapsed Time: ", elapsedTime);
					WriteFittingResults(avResultsCollections,fpBinaryResults);
//					WriteFittingResults(avResultsCollections,avEvl,fpBinaryResults);
					fclose(fpBinaryResults);			
					fclose(fpOutput);
					fclose(fpCorrectFittings);
//					}
//				}
//			}
		}
	}	
}

bool CAbf_ProcessingDoc::Significant(structFittingResultNode aNode, structFittingResultNode bNode, double& dL, int& nDf, double& dProb)
{
	double dL1=aNode.dLogLikelihood;
	double dL2=bNode.dLogLikelihood;
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

CString CAbf_ProcessingDoc::PassingString(CString sString)
{
	CString sNewString=sString;
	return sNewString;
}

void CAbf_ProcessingDoc::ExportDwellCurves(vector <structFittingResultNode> avResultNode)
{
	//This function is assuming that the fiting result nodes in the array are ordered (assending) according 
	//to the numbers of components used for fitting. All nodes are for the same dwell level.
	CString sFileName=avResultNode.at(0).sFileName;
	int nDwellLevel=avResultNode.at(0).nDwellLevel;
	int nComponents0=avResultNode.at(0).nNumComponents;
	int nSize=avResultNode.size();
	int nComponents1=avResultNode.at(nSize-1).nNumComponents;

	sFileName=m_cEvl.ExtendFileName(sFileName,'_');

	char cTemp;
	cTemp=nDwellLevel+48;
	sFileName=m_cEvl.ExtendFileName(sFileName,cTemp);
	cTemp=nComponents0+48;
	sFileName=m_cEvl.ExtendFileName(sFileName,cTemp);
	if(nComponents1>nComponents0)
	{
		sFileName=m_cEvl.ExtendFileName(sFileName,'_');
		cTemp=nComponents1+48;
		sFileName=m_cEvl.ExtendFileName(sFileName,cTemp);
	}
	sFileName=m_cEvl.ChangeExt(sFileName,"Crv");
	for(int i=0;i<nSize;i++)
	{
		avResultNode.at(i).sCurFileName=sFileName;
	}
	m_cEvl.ExportDwellCurves(sFileName,avResultNode);
}

void CAbf_ProcessingDoc::OrderFittingResults(vector <structFittingResultNode> &avResultNode)
{
	vector <structFittingResultNode> bvResultNode;
	int nSize=avResultNode.size();
	Vec_IO_DP arr(nSize);
	Vec_IO_DP brr(nSize);
	int nTrial;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		arr[nTrial]=-avResultNode.at(nTrial).dLogLikelihood;
		brr[nTrial]=double(nTrial);
	}
	NR::sort2(arr,brr);

	int nIndex=0;
	nIndex=int(brr[0]+0.5);
	double dLogLik0=avResultNode.at(nIndex).dLogLikelihood;
	double dLogLik_R;
	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		structFittingResultNode aNode;
		nIndex=int(brr[nTrial]+0.5);
		aNode=avResultNode.at(nIndex);
		aNode.nScoreRanking=nTrial;
		aNode.nClusterIndex=nTrial;
		dLogLik_R=aNode.dLogLikelihood-dLogLik0;
		aNode.dLogLikelihood_R=dLogLik_R;
		bvResultNode.push_back(aNode);
	}

	for(nTrial=0;nTrial<nSize;nTrial++)
	{
		avResultNode.at(nTrial)=bvResultNode.at(nTrial);
	}
}

double CAbf_ProcessingDoc::CriticalFunction(float Ts, float Tl, float T)
{
	//Two components have equal proportion of mistakes
	double dValue=0.;
	dValue=1.-exp(-T/Tl)-exp(-T/Ts);
	return dValue;
}

double CAbf_ProcessingDoc::CriticalFunction1(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Two components have equal number of mistakes
	double dValue=0.;
	dValue=-fAs*exp(-fT/fTs)+fAl*(1.f-exp(-fT/fTl));
	return dValue;
}

double CAbf_ProcessingDoc::CriticalFunction2(float fTs, float fAs, float fTl, float fAl, float fT)
{
	//Minimize the total number of mistakes
	double dValue=0.;
	dValue=-(fAs/fTs)*exp(-fT/fTs)+(fAl/fTl)*exp(-fT/fTl);
	return dValue;
}

double CAbf_ProcessingDoc::BurstDelimiter(float fTs, float fTl)
{
	float fT1, fT2, fT;
	double dX,dX0;
	fT1=0.; 
	fT2=3.*fTl;
	fT=fT2;
	dX=CriticalFunction(fTs,fTl,fT);
//	fTs=4.9;
//	fTl=31.;
	dX0=dX;
	if(dX<0.)
	{
		dX0=-dX;
	}
	while(dX0>0.000001)
	{
		if(dX>0.)
		{
			fT2=fT;
		}
		else
		{
			fT1=fT;
		}
		fT=0.5*(fT1+fT2);
		dX=CriticalFunction(fTs,fTl,fT);
		dX0=dX;
		if(dX<0.)
		{
			dX0=-dX;
		}
	}
	return fT;
}

double CAbf_ProcessingDoc::BurstDelimiter(double fTs, double fTl)
{
	double fT1, fT2, fT;
	double dX,dX0;
	fT1=0.; 
	fT2=3.*fTl;
	fT=fT2;
	dX=CriticalFunction(fTs,fTl,fT);
//	fTs=4.9;
//	fTl=31.;
	dX0=dX;
	if(dX<0.)
	{
		dX0=-dX;
	}
	while(dX0>0.000001)
	{
		if(dX>0.)
		{
			fT2=fT;
		}
		else
		{
			fT1=fT;
		}
		fT=0.5*(fT1+fT2);
		dX=CriticalFunction(fTs,fTl,fT);
		dX0=dX;
		if(dX<0.)
		{
			dX0=-dX;
		}
	}
	return fT;
}

double CAbf_ProcessingDoc::BurstDelimiter(double fTs, double dAs, double fTl, double dAl, int nKey)
{
	double fT1, fT2, fT;
	double dX,dX0,dXs,dXl;
	fT1=fTs/10.; 
	fT2=10.*fTl;
	fT=0.5*(fTs+fTl);
	switch (nKey)
	{
	case 1:
		dX=CriticalFunction(fTs, fTl, fT);
		break;
	case 2:
		dX=CriticalFunction1(fTs,dAs, fTl, dAl, fT);
		break;
	case 3:
		dX=CriticalFunction2(fTs,dAs, fTl, dAl, fT);
		break;
	default:
		dX=CriticalFunction(fTs,fTl,fT);
		break;
	}

	switch (nKey)
	{
	case 1:
		dXs=CriticalFunction(fTs, fTl, fT1);
		break;
	case 2:
		dXs=CriticalFunction1(fTs,dAs, fTl, dAl, fT1);
		break;
	case 3:
		dXs=CriticalFunction2(fTs,dAs, fTl, dAl, fT1);
		break;
	default:
		dXs=CriticalFunction(fTs,fTl,fT1);
		break;
	}

	switch (nKey)
	{
	case 1:
		dXl=CriticalFunction(fTs, fTl, fT2);
		break;
	case 2:
		dXl=CriticalFunction1(fTs,dAs, fTl, dAl, fT2);
		break;
	case 3:
		dXl=CriticalFunction2(fTs,dAs, fTl, dAl, fT2);
		break;
	default:
		dXl=CriticalFunction(fTs,fTl,fT2);
		break;
	}

//	fTs=4.9;
//	fTl=31.;
	dX0=dX;
	while(fabs(dX0)>0.000001)
	{
		if(dX*dXl>0.)
		{
			fT2=fT;
		}
		else
		{
			fT1=fT;
		}
		fT=0.5*(fT1+fT2);
		switch (nKey)
		{
		case 1:
			dX=CriticalFunction(fTs,fTl,fT);
			break;
		case 2:
			dX=CriticalFunction1(fTs,dAs, fTl, dAl, fT);
			break;
		case 3:
			dX=CriticalFunction2(fTs,dAs, fTl, dAl, fT);
			break;
		default:
			dX=CriticalFunction(fTs,fTl,fT);
			break;
		}
		dX0=dX;
	}
	return fT;
}


void CAbf_ProcessingDoc::OnAnalysisEventlistSimulateeventlist() 
{
	// TODO: Add your command handler code here
	
	CString sTitle="Import an evl file";
	if(m_sEvlFileName=="Not opened yet.")
	{
		OnFileImportEventfile(sTitle);
	}
	int nComponents=5;
	double dT[5]={0.50,2.54,11.86,473.36,4071.02};
	double dW[5]={0.904674,0.066787,0.016314,0.011820,0.000405};
//	double dT[5]={1.4,10.,100.};
//	double dW[5]={0.2,0.1,0.7};
//	double dT[5]={0.02,1.,10.};
//	double dW[5]={0.2,0.1,0.7};
	m_cEvl.SimulateEvl(dT,dW,nComponents);
}

void CAbf_ProcessingDoc::OutputCorrectFittingResults1(structFittingResultNode aResultNode, FILE* fpOutput)
{
	int j;
	int nNumComponents=aResultNode.nNumComponents;

	Vec_IO_DP dTimeConstants(nNumComponents);
	Vec_IO_DP dWeights(nNumComponents);
	for(int nTrial=0;nTrial<nNumComponents;nTrial++)
	{
		dTimeConstants[nTrial]=aResultNode.dTimeConstants[nTrial];
		dWeights[nTrial]=aResultNode.dWeights[nTrial];
	}

	NR::sort2(dTimeConstants,dWeights);

	fprintf(fpOutput,"TimeConstants(ms)ANDWeights:  ");
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"  %10.2f",dTimeConstants[j]);
	}
	for(j=0;j<nNumComponents;j++)
	{
		fprintf(fpOutput,"   %8.6f",dWeights[j]);
	}
	fprintf(fpOutput,"  %10.2f    %s",aResultNode.dMean,aResultNode.sFileName);
	fprintf(fpOutput,"\n");
}

void CAbf_ProcessingDoc::OutputCorrectFittingResults(structFittingResultNode aResultNode, FILE* fpOutput)
{
	fprintf(fpOutput,"  %1d    %2d      %1d      %1d   ",aResultNode.nDwellLevel,aResultNode.nSignificantComponents
		,aResultNode.nNumChannels,aResultNode.nMaxLevel);
	fprintf(fpOutput,"  %10.2f    %8.4",aResultNode.fRecordLength,aResultNode.fOpenProb);
	fprintf(fpOutput,"  %8.3e    %10d",aResultNode.fProbSingle,aResultNode.nNumEvents);
	fprintf(fpOutput,"      %s",aResultNode.sCurFileName);
	fprintf(fpOutput,"\n");
}

void CAbf_ProcessingDoc::OnAnalysisEventlistGetfittedparameters() 
{
	// TODO: Add your command handler code here
	CGettingFittedParametersDlg aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		structFittedParameterNode aNode;
		aNode=aDlg.GetFittedParameter();
		OutputFittedDTParameters(aNode);
		CFittedEvlParamterHandling aHandling(m_avFittingResultsCollection,aNode);
	}
}

void CAbf_ProcessingDoc::GetFittedparameters_Old(structFittedParameterNode aFittedNode)
{
	CString sFileName="NonSense";
	FILE* fpInput;
	CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Automated Dwell-time Fitting Result(*.Aft)\0*.Aft\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results";
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);
	int nComponents=aFittedNode.nNumComponents;
	int nFittedLevel=aFittedNode.nFittedLevel;
	typedef vector<float> FVector;

	int i,j,nTemp;
	float fTemp,fTMean;
	CString sEvlFileName;
	vector <char*> sEvlFileNames;
	char* pcFileName=NULL;

	vector <FVector> fvectTimeConstants;
	vector <FVector> fvectWeights;
	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		fvectTimeConstants.push_back(aV);
		fvectWeights.push_back(bV);
	}

	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
		if(!(fpInput=fopen(sFileName,"r")))
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			CString sTemp="";
			while(!feof(fpInput))
			{
				fscanf(fpInput,"%s",sTemp);
				if(sTemp=="CurFile:")
				{
					fscanf(fpInput,"%s",sEvlFileName);
					sEvlFileName=PassingString(sEvlFileName);
				}
				if(sTemp=="Level:")
				{
					fscanf(fpInput,"%d",&nTemp);
					if(nTemp==nFittedLevel)
					{
						fscanf(fpInput,"%s",sTemp);
						if(sTemp=="Components:")
						{
							fscanf(fpInput,"%d",&nTemp);
							if(nTemp==nComponents)
							{
								CString aString=sEvlFileName;
//								delete pcFileName;
								int nSize=sEvlFileName.GetLength();
								pcFileName=new char[nSize+1];
								//Remember this will cause memory leakage. Need to think of a better way.
								//I have to do this because the pointer always get assigned with the same 
								//memory address if you delete the pointor before the new char[] statement.
								pcFileName[nSize]=0;
								strcpy(pcFileName,sEvlFileName);
//								aString=PassingString(sEvlFileName);
								sEvlFileNames.push_back(pcFileName);
								while(!feof(fpInput))
								{
									fscanf(fpInput,"%s",sTemp);
									if(sTemp=="Area")
									{
										fscanf(fpInput,"%s",sTemp);
										if(sTemp=="TimeConstant")
										{
											for(i=0;i<nComponents;i++)
											{
												fscanf(fpInput,"%e",&fTemp);
												fvectWeights.at(i).push_back(fTemp);
												fscanf(fpInput,"%e",&fTemp);
												fvectTimeConstants.at(i).push_back(fTemp);
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			fclose(fpInput);
			sFileName=m_cEvl.ChangeExt(sFileName,"tnw");
			FILE* fpOutput=fopen(sFileName,"w");
			int nSize=fvectTimeConstants.at(0).size();
			for(i=0;i<nSize;i++)
			{
				fTMean=0.f;
				fprintf(fpOutput,"%10d",i);
				for(j=0;j<nComponents;j++)
				{
					fTemp=fvectTimeConstants.at(j)[i];
					fprintf(fpOutput,"    %14.5f",fTemp);
				}
				for(j=0;j<nComponents;j++)
				{
					fTemp=fvectWeights.at(j)[i];
					fprintf(fpOutput,"    %10.7f",fTemp);
					fTMean+=fTemp*fvectTimeConstants.at(j)[i];
				}
				fprintf(fpOutput,"%12.4f    %s\n",fTMean,sEvlFileNames.at(i));
			}

			vector <float> vectfMean;
			vector <float> vectfSD;
			float fMean,fSD;
			fprintf(fpInput,"%\n\n");
			vector<float> vectfPara;
			double dT[5]={0.02,1.,10.};
			double dW[5]={0.2,0.1,0.7};
//			double dT[5]={0.50,2.54,11.86,473.36,4071.02};
//			double dW[5]={0.904674,0.066787, 0.016314,0.011820,0.000405};

			int nTemp;
			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectTimeConstants.at(i));
				fMean=aNode.fMean;
				vectfMean.push_back(fMean);
				nTemp=aNode.nSampleSize;
				fSD=aNode.fSem*sqrt(double(nTemp));
				vectfSD.push_back(fSD);
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
				vectfPara.push_back(dT[i]);
			}

			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectWeights.at(i));
				fMean=aNode.fMean;
				vectfMean.push_back(fMean);
				nTemp=aNode.nSampleSize;
				fSD=aNode.fSem*sqrt(double(nTemp));
				vectfSD.push_back(fSD);
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
				vectfPara.push_back(dW[i]);
			}

			float fScatter=0.f,fBias=0.f,fPara=0.f;
			for(i=0;i<2*nComponents;i++)
			{
				fPara=vectfPara.at(i);
				fMean=vectfMean.at(i);
				fSD=vectfSD.at(i);
				fScatter+=(fSD*fSD)/(fPara*fPara);
				fBias+=((fMean-fPara)*(fMean-fPara))/(fPara*fPara);
				fprintf(fpOutput,"%12.6f    %12.6f    %12.6f\n",fMean,fPara,(fMean-fPara)/fPara);
			}
			fScatter=sqrt(fScatter/double(2*nComponents));
			fBias=sqrt(fBias/double(2*nComponents));

			fprintf(fpOutput,"fScatter:    %10.6f      fBias:    %10.6f\n",fScatter,fBias);
			fclose(fpOutput);
		}
	}

	
	vector <structFittingResultsCollectionNode> avCollection;
	pch="Automated Dwell-time Fitting Result(*.Aft)\0*.Aft\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		FILE* fp;
		sFileName=aFileDlg.GetPathName();
		if(!(fp=fopen(sFileName,"w")))
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
		}
		fclose(fp);
	}
}


void CAbf_ProcessingDoc::GetFittedparameters1(structFittedParameterNode aFittedNode)
{

	int nComponents=aFittedNode.nNumComponents;
	int nFittedLevel=aFittedNode.nFittedLevel;
	typedef vector<float> FVector;
	vector <FVector> fvectTimeConstants;
	vector <FVector> fvectWeights;
	vector <FVector> fvectTimeConstants0;
	vector <FVector> fvectWeights0;
	int i,j,nTemp;
	float fTemp,fTMean;
	CString sEvlFileName;
	vector <char*> sEvlFileNames;
	char* pcFileName=NULL;

	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		fvectTimeConstants.push_back(aV);
		fvectWeights.push_back(bV);
	}
	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		fvectTimeConstants0.push_back(aV);
		fvectWeights0.push_back(bV);
	}

	vector <float> vfLik;
	vector <float> vfSDev;
	vector <float> vfChiSQ;
	vector <float> vfDeltaDWT;
//	int nSmallestPosition[10];
//	float fSmallestValues[10]={1.e30,1.e30,1.e30,1.e30,1.e30,1.e30,1.e30,1.e30,1.e30,1.e30};
	
	CString sFileName="NonSense";
	FILE* fpInput;
	CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
	char* pch="Automated Dwell-time Fitting Result(*.Aft)\0*.Aft\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results";
//	strcpy(aFileDlg.m_ofn.lpstrFile,sFileNames);
	aFileDlg.m_ofn.lpstrFile=sFileName.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	POSITION aPosition;

	CString sTemp0;
	int nSignificantComponents;
	float fFittedArea,fLogLikelihood,fMeanDTH,fMeanDTHC,fSDev,fChiSQ,fSSE;

	if(aFileDlg.DoModal()==IDOK)
	{
		aPosition=aFileDlg.GetStartPosition();
		while(aPosition!=NULL)
		{
			sEvlFileName=aFileDlg.GetNextPathName(aPosition);
			if(!(fpInput=fopen(sFileName,"r")))
			{
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{

				fvectTimeConstants.clear();
				fvectWeights.clear();
				vfLik.clear();
				vfSDev.clear();
				vfChiSQ.clear();
				vfDeltaDWT.clear();

				CString sTemp="";
				while(!feof(fpInput))
				{
					fscanf(fpInput,"%s",sTemp);
					if(sTemp=="CurFile:")
					{
						fscanf(fpInput,"%s",sEvlFileName);
						sEvlFileName=PassingString(sEvlFileName);
					}
					if(sTemp=="Level:")
					{
						fscanf(fpInput,"%d",&nTemp);
						if(nTemp==nFittedLevel)
						{
							fscanf(fpInput,"%s",sTemp);
							if(sTemp=="Components:")
							{
								fscanf(fpInput,"%d",&nTemp);
								if(nTemp==nComponents)
								{
									CString aString=sEvlFileName;

									fscanf(fpInput,"%s",sTemp);
									while(sTemp!="SignificantComponents:");
									{
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%d",&nSignificantComponents);

									fscanf(fpInput,"%s",sTemp0);
									fscanf(fpInput,"%s",sTemp);
									while(sTemp0!="Fitted"||sTemp!="Area:");
									{
										sTemp0=sTemp;
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%e",&fFittedArea);

	//								delete pcFileName;
									int nSize=sEvlFileName.GetLength();
									pcFileName=new char[nSize+1];
									//Remember this will cause memory leakage. Need to think of a better way.
									//I have to do this because the pointer always get assigned with the same 
									//memory address if you delete the pointor before the new char[] statement.
									pcFileName[nSize]=0;
									strcpy(pcFileName,sEvlFileName);
	//								aString=PassingString(sEvlFileName);
									sEvlFileNames.push_back(pcFileName);

									while(!feof(fpInput))
									{
										fscanf(fpInput,"%s",sTemp);
										if(sTemp=="Area")
										{
											fscanf(fpInput,"%s",sTemp);
											if(sTemp=="TimeConstant")
											{
												for(i=0;i<nComponents;i++)
												{
													fscanf(fpInput,"%e",&fTemp);
													fvectWeights.at(i).push_back(fTemp);
													fscanf(fpInput,"%e",&fTemp);
													fvectTimeConstants.at(i).push_back(fTemp);
												}
												break;
											}
										}
									}
									fscanf(fpInput,"%s",sTemp0);
									fscanf(fpInput,"%s",sTemp);
									while(sTemp0!="Log"||sTemp!="Likelihood:");
									{
										sTemp0=sTemp;
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%f",&fLogLikelihood);

									fscanf(fpInput,"%s",sTemp0);
									fscanf(fpInput,"%s",sTemp);
									while(sTemp0!="in"||sTemp!="ms):");
									{
										sTemp0=sTemp;
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%f%f",&fMeanDTH,&fMeanDTHC);

									fscanf(fpInput,"%s",sTemp);
									while(sTemp!="SDev");
									{
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%f",&fSDev);

									fscanf(fpInput,"%s",sTemp);
									while(sTemp!="Chisquare");
									{
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%f",&fChiSQ);


									fscanf(fpInput,"%s",sTemp);
									while(sTemp!="SSE:");
									{
										fscanf(fpInput,"%s",sTemp);
									}
									fscanf(fpInput,"%f",&fSSE);
								}
							}
						}
					}
				}
				fclose(fpInput);
			}
			sFileName=m_cEvl.ChangeExt(sFileName,"tnw");
			FILE* fpOutput=fopen(sFileName,"w");
			int nSize=fvectTimeConstants.at(0).size();
			for(i=0;i<nSize;i++)
			{
				fTMean=0.f;
				fprintf(fpOutput,"%10d",i);
				for(j=0;j<nComponents;j++)
				{
					fTemp=fvectTimeConstants.at(j)[i];
					fprintf(fpOutput,"    %14.5f",fTemp);
				}
				for(j=0;j<nComponents;j++)
				{
					fTemp=fvectWeights.at(j)[i];
					fprintf(fpOutput,"    %10.7f",fTemp);
					fTMean+=fTemp*fvectTimeConstants.at(j)[i];
				}
				fprintf(fpOutput,"%12.4f    %s\n",fTMean,sEvlFileNames.at(i));
			}

			vector <float> vectfMean;
			vector <float> vectfSD;
			float fMean,fSD;
			fprintf(fpInput,"%\n\n");
			vector<float> vectfPara;
			double dT[5]={0.02,1.,10.};
			double dW[5]={0.2,0.1,0.7};
//			double dT[5]={0.50,2.54,11.86,473.36,4071.02};
//			double dW[5]={0.904674,0.066787, 0.016314,0.011820,0.000405};

			int nTemp;
			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectTimeConstants.at(i));
				fMean=aNode.fMean;
				vectfMean.push_back(fMean);
				nTemp=aNode.nSampleSize;
				fSD=aNode.fSem*sqrt(double(nTemp));
				vectfSD.push_back(fSD);
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
				vectfPara.push_back(dT[i]);
			}

			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectWeights.at(i));
				fMean=aNode.fMean;
				vectfMean.push_back(fMean);
				nTemp=aNode.nSampleSize;
				fSD=aNode.fSem*sqrt(double(nTemp));
				vectfSD.push_back(fSD);
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
				vectfPara.push_back(dW[i]);
			}

			float fScatter=0.f,fBias=0.f,fPara=0.f;
			for(i=0;i<2*nComponents;i++)
			{
				fPara=vectfPara.at(i);
				fMean=vectfMean.at(i);
				fSD=vectfSD.at(i);
				fScatter+=(fSD*fSD)/(fPara*fPara);
				fBias+=((fMean-fPara)*(fMean-fPara))/(fPara*fPara);
				fprintf(fpOutput,"%12.6f    %12.6f    %12.6f\n",fMean,fPara,(fMean-fPara)/fPara);
			}
			fScatter=sqrt(fScatter/double(2*nComponents));
			fBias=sqrt(fBias/double(2*nComponents));

			fprintf(fpOutput,"fScatter:    %10.6f      fBias:    %10.6f\n",fScatter,fBias);
			fclose(fpOutput);
		}
	}	
}

void CAbf_ProcessingDoc::WriteAString(CString aString, FILE *fp)
{
	int nMaxLength=1000;
	int nLength=aString.GetLength();
	if(nLength>nMaxLength)
	{
		nLength=nMaxLength;
	}
	fwrite(&nLength,sizeof(int),1,fp);
	for(int i=0;i<nLength;i++)
	{
		char cTemp=aString[i];
		fwrite(&cTemp,sizeof(char),1,fp);
	}
}

CString CAbf_ProcessingDoc::ReadAString(FILE *fp)
{
	int nLength=0;
	CString aString;
	fread(&nLength,sizeof(int),1,fp);
	for(int i=0;i<nLength;i++)
	{
		char cTemp;
		fread(&cTemp,sizeof(char),1,fp);
		aString+=cTemp;
	}
	return aString;
}


//void CAbf_ProcessingDoc::WriteFittingResults(vector <structFittingResultsCollectionNode> avCollection, vector <CEvl> avEvl, FILE* fp)
void CAbf_ProcessingDoc::WriteFittingResults(vector <structFittingResultsCollectionNode> avCollection, FILE* fp)
{
	int nFileIndex,nLevelIndex,nComponentIndex,nTrialIndex;
	int nNumFittingLevels=0,nMinComponents=0,nMaxComponents=0,nFittingLevel=0,nSignificantComponents;
	int nNumFiles=avCollection.size();
	int nNumTrials=0;
	int nWritingKey=1;
	fwrite(&nNumFiles,sizeof(int),1,fp);
	for(nFileIndex=0;nFileIndex<nNumFiles;nFileIndex++)
	{
//		avEvl.at(nFileIndex).WriteEvl(fp,nWritingKey);
		nNumFittingLevels=avCollection.at(nFileIndex).nNumFittingLevels;
		fwrite(&nNumFittingLevels,sizeof(int),1,fp);
		for(nLevelIndex=0;nLevelIndex<nNumFittingLevels;nLevelIndex++)
		{
			nFittingLevel=avCollection.at(nFileIndex).vnFittingLevels.at(nLevelIndex);
			fwrite(&nFittingLevel,sizeof(int),1,fp);
			nMinComponents=avCollection.at(nFileIndex).vnMinComponents.at(nLevelIndex);
			nMaxComponents=avCollection.at(nFileIndex).vnMaxComponents.at(nLevelIndex);
			nSignificantComponents=avCollection.at(nFileIndex).vnSignificantComponents.at(nLevelIndex);
			fwrite(&nMinComponents,sizeof(int),1,fp);
			fwrite(&nMaxComponents,sizeof(int),1,fp);
			fwrite(&nSignificantComponents,sizeof(int),1,fp);
			int nNumCompnentTrial=nMaxComponents-nMinComponents+1;
			for(nComponentIndex=0;nComponentIndex<nNumCompnentTrial;nComponentIndex++)
			{
				nNumTrials=avCollection.at(nFileIndex).vvnNumTrials.at(nLevelIndex).at(nComponentIndex);
				fwrite(&nNumTrials,sizeof(int),1,fp);
				for(nTrialIndex=0;nTrialIndex<nNumTrials;nTrialIndex++)
				{
					structFittingResultNode aNode=avCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex);
					WriteFittingResults(aNode,fp);
				}
			}
		}
	}
}

void CAbf_ProcessingDoc::WriteFittingResults(structFittingResultNode aResultNode, FILE *fpOutput)
{
	int j;
	int nNumComponents=aResultNode.nNumComponents;

	WriteAString(aResultNode.sFileName,fpOutput);
	fwrite(&aResultNode.fRecordLength,sizeof(float),1,fpOutput);
	fwrite(&aResultNode.nNumEvents,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nMaxLevel,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nNumChannels,sizeof(int),1,fpOutput);

	fwrite(&aResultNode.fOpenProb,sizeof(float),1,fpOutput);
	fwrite(&aResultNode.fMeanOpenTime,sizeof(float),1,fpOutput);
	fwrite(&aResultNode.fProbSingle,sizeof(float),1,fpOutput);
	WriteAString(aResultNode.sCurFileName,fpOutput);
	fwrite(&aResultNode.nFuncKey,sizeof(int),1,fpOutput);

	fwrite(&aResultNode.nScoreRanking,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nTrialNumber,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nClusterIndex,sizeof(int),1,fpOutput);//03/13/2005
	fwrite(&aResultNode.nDwellLevel,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nNumComponents,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.nSignificantComponents,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.fConfidanceRange,sizeof(float),1,fpOutput);//03/13/2005
	fwrite(&aResultNode.bConfidanceRange,sizeof(float),1,fpOutput);//03/13/2005

	fwrite(&aResultNode.nNumEvents,sizeof(int),1,fpOutput);
	fwrite(&aResultNode.fFittingRange.fLeft,sizeof(float),1,fpOutput);
	fwrite(&aResultNode.fFittingRange.fRight,sizeof(float),1,fpOutput);
	fwrite(&aResultNode.dFittedArea,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.nEventsInRange,sizeof(int),1,fpOutput);
	double dT[20];
	double dW[20];
	
	for(j=0;j<nNumComponents;j++)
	{
		dT[j]=aResultNode.dTimeConstants[j];
		dW[j]=aResultNode.dWeights[j];
		fwrite(&aResultNode.dWeights[j],sizeof(double),1,fpOutput);
		fwrite(&aResultNode.dTimeConstants[j],sizeof(double),1,fpOutput);

		fwrite(&aResultNode.fConfidanceRangeT[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeT[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeW[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeW[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

		fwrite(&aResultNode.fConfidanceRangeTL[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeTL[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWL[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWL[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

		fwrite(&aResultNode.fConfidanceRangeTI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeTI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

		fwrite(&aResultNode.fConfidanceRangeTLI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeTLI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWLI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.fConfidanceRangeWLI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
	}


	fwrite(&aResultNode.aTrialNode.fFittingRange.fLeft,sizeof(float),1,fpOutput);//03/13/2005
	fwrite(&aResultNode.aTrialNode.fFittingRange.fRight,sizeof(float),1,fpOutput);//03/13/2005

	for(j=0;j<nNumComponents;j++)
	{
		fwrite(&aResultNode.aTrialNode.fTimeConstants[j],sizeof(float),1,fpOutput);
	}
	for(j=0;j<nNumComponents;j++)
	{
		fwrite(&aResultNode.aTrialNode.fWeights[j],sizeof(float),1,fpOutput);							
	}
	for(j=0;j<nNumComponents;j++)
	{
		fwrite(&aResultNode.aTrialNode.bFreezeT[j],sizeof(bool),1,fpOutput);//03/13/2005
		fwrite(&aResultNode.aTrialNode.bFreezeW[j],sizeof(bool),1,fpOutput);//03/13/2005
	}

	fwrite(&aResultNode.dMean,sizeof(double),1,fpOutput);
	double dLogLikelihood=aResultNode.dLogLikelihood;
	fwrite(&aResultNode.nNumIterations,sizeof(int),1,fpOutput);
	fwrite(&dLogLikelihood,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dMeanDwellTimeH,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dMeanDwellTimeHC,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dMeanDwellTimeO,sizeof(double),1,fpOutput);

	fwrite(&aResultNode.dMeanDwellTimeC,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dSDev,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dChiSQ,sizeof(double),1,fpOutput);
	fwrite(&aResultNode.dSSE,sizeof(double),1,fpOutput);
}

CString CAbf_ProcessingDoc::FileExt(CString sFileName)
{
	CString sExt="abc";
	int nLength=sFileName.GetLength();
	for(int i=0;i<3;i++)
	{
		sExt.SetAt(i,sFileName[nLength-3+i]);
	}
	return sExt;	
}


void CAbf_ProcessingDoc::ReadFittingDTResultsCollection(vector <structFittingResultsCollectionNode> &avCollection, CString sFileName)
{
	CString sMessage="Can not open the input file. Please check if this file is being used by other applications.";
	FILE* fp;
	CString sExt=FileExt(sFileName);
	if(!(fp=fopen(sFileName,"rb")))
	{
		AfxMessageBox(sMessage,MB_OK,0);
	}
	else
	{
		int nFileIndex,nLevelIndex,nComponentIndex,nTrialIndex;
		int nNumFittingLevels=0,nMinComponents=0,nMaxComponents=0,nFittingLevel=0,nSignificantComponents;
		int nNumFiles=avCollection.size();
		int nNumTrials=0;
		avCollection.clear();
		fread(&nNumFiles,sizeof(int),1,fp);
		for(nFileIndex=0;nFileIndex<nNumFiles;nFileIndex++)
		{
			structFittingResultsCollectionNode aCollection;
			fread(&nNumFittingLevels,sizeof(int),1,fp);
			aCollection.nNumFittingLevels=nNumFittingLevels;
			for(nLevelIndex=0;nLevelIndex<nNumFittingLevels;nLevelIndex++)
			{
				fread(&nFittingLevel,sizeof(int),1,fp);
				aCollection.vnFittingLevels.push_back(nFittingLevel);
				fread(&nMinComponents,sizeof(int),1,fp);
				fread(&nMaxComponents,sizeof(int),1,fp);
				fread(&nSignificantComponents,sizeof(int),1,fp);
				aCollection.vnMinComponents.push_back(nMinComponents);
				aCollection.vnMaxComponents.push_back(nMaxComponents);
				aCollection.vnSignificantComponents.push_back(nSignificantComponents);
				vector <int> vnNumTrials;
				TotalVResults aTotalVResults;
				int nNumComponentTrials=nMaxComponents-nMinComponents+1;
				for(nComponentIndex=0;nComponentIndex<nNumComponentTrials;nComponentIndex++)
				{
					fread(&nNumTrials,sizeof(int),1,fp);
					vnNumTrials.push_back(nNumTrials);
					vector <structFittingResultNode> avResultNode;
					for(nTrialIndex=0;nTrialIndex<nNumTrials;nTrialIndex++)
					{
						//The results nodes of different trials are assumed to be desentantly sorted by LogLik.
						structFittingResultNode aNode;
						ReadFittingResultsNode(aNode,fp,sExt);
						avResultNode.push_back(aNode);
						avResultNode.at(nTrialIndex).dLogLikelihood_R=aNode.dLogLikelihood;
						avResultNode.at(nTrialIndex).dLogLikelihood_R-=avResultNode.at(0).dLogLikelihood;
					}
					aTotalVResults.push_back(avResultNode);
				}
				aCollection.vvnNumTrials.push_back(vnNumTrials);
				aCollection.aTotalVVResults.push_back(aTotalVResults);
			}
			avCollection.push_back(aCollection);
		}
		m_avFittingResultsCollection=avCollection;
	}
}


void CAbf_ProcessingDoc::ReadFittingResultsNode(structFittingResultNode &aResultNode, FILE *fpOutput,CString sExt)
{
	int j;
	int nNumComponents=aResultNode.nNumComponents;

	aResultNode.sFileName=ReadAString(fpOutput);
	fread(&aResultNode.fRecordLength,sizeof(float),1,fpOutput);
	fread(&aResultNode.nNumEvents,sizeof(int),1,fpOutput);
	fread(&aResultNode.nMaxLevel,sizeof(int),1,fpOutput);
	fread(&aResultNode.nNumChannels,sizeof(int),1,fpOutput);

	fread(&aResultNode.fOpenProb,sizeof(float),1,fpOutput);
	fread(&aResultNode.fMeanOpenTime,sizeof(float),1,fpOutput);
	fread(&aResultNode.fProbSingle,sizeof(float),1,fpOutput);
	aResultNode.sCurFileName=ReadAString(fpOutput);
	fread(&aResultNode.nFuncKey,sizeof(int),1,fpOutput);

	fread(&aResultNode.nScoreRanking,sizeof(int),1,fpOutput);
	fread(&aResultNode.nTrialNumber,sizeof(int),1,fpOutput);
	if(sExt=="clt"||sExt=="dlt")
		fread(&aResultNode.nClusterIndex,sizeof(int),1,fpOutput);//03/13/2005
	fread(&aResultNode.nDwellLevel,sizeof(int),1,fpOutput);
	fread(&aResultNode.nNumComponents,sizeof(int),1,fpOutput);
	fread(&aResultNode.nSignificantComponents,sizeof(int),1,fpOutput);

	if(sExt=="clt"||sExt=="dlt")
	{
		fread(&aResultNode.fConfidanceRange,sizeof(float),1,fpOutput);//03/13/2005
		fread(&aResultNode.bConfidanceRange,sizeof(float),1,fpOutput);//03/13/2005
	}

	fread(&aResultNode.nNumEvents,sizeof(int),1,fpOutput);
	fread(&aResultNode.fFittingRange.fLeft,sizeof(float),1,fpOutput);
	fread(&aResultNode.fFittingRange.fRight,sizeof(float),1,fpOutput);
	fread(&aResultNode.dFittedArea,sizeof(double),1,fpOutput);
	fread(&aResultNode.nEventsInRange,sizeof(int),1,fpOutput);
	nNumComponents=aResultNode.nNumComponents;
	for(j=0;j<nNumComponents;j++)
	{
		fread(&aResultNode.dWeights[j],sizeof(double),1,fpOutput);
		fread(&aResultNode.dTimeConstants[j],sizeof(double),1,fpOutput);
		if(sExt=="clt"||sExt=="dlt")
		{
			fread(&aResultNode.fConfidanceRangeT[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeT[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeW[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeW[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

			fread(&aResultNode.fConfidanceRangeTL[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeTL[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWL[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWL[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

			fread(&aResultNode.fConfidanceRangeTI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeTI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005

			fread(&aResultNode.fConfidanceRangeTLI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeTLI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWLI[j].fRight,sizeof(float),1,fpOutput);//03/13/2005
			fread(&aResultNode.fConfidanceRangeWLI[j].fLeft,sizeof(float),1,fpOutput);//03/13/2005
		}
	}

	if(sExt=="clt"||sExt=="dlt")
	{
		fread(&aResultNode.aTrialNode.fFittingRange.fLeft,sizeof(float),1,fpOutput);//03/13/2005
		fread(&aResultNode.aTrialNode.fFittingRange.fRight,sizeof(float),1,fpOutput);//03/13/2005
	}


	for(j=0;j<nNumComponents;j++)
	{
		fread(&aResultNode.aTrialNode.fTimeConstants[j],sizeof(float),1,fpOutput);
	}
	for(j=0;j<nNumComponents;j++)
	{
		fread(&aResultNode.aTrialNode.fWeights[j],sizeof(float),1,fpOutput);							
	}

	if(sExt=="clt"||sExt=="dlt")
	{
		for(j=0;j<nNumComponents;j++)
		{
			fread(&aResultNode.aTrialNode.bFreezeT[j],sizeof(bool),1,fpOutput);//03/13/2005
			fread(&aResultNode.aTrialNode.bFreezeW[j],sizeof(bool),1,fpOutput);//03/13/2005
		}
	}

	fread(&aResultNode.dMean,sizeof(double),1,fpOutput);

	fread(&aResultNode.nNumIterations,sizeof(int),1,fpOutput);
	fread(&aResultNode.dLogLikelihood,sizeof(double),1,fpOutput);
	fread(&aResultNode.dMeanDwellTimeH,sizeof(double),1,fpOutput);
	fread(&aResultNode.dMeanDwellTimeHC,sizeof(double),1,fpOutput);
	fread(&aResultNode.dMeanDwellTimeO,sizeof(double),1,fpOutput);

	fread(&aResultNode.dMeanDwellTimeC,sizeof(double),1,fpOutput);
	fread(&aResultNode.dSDev,sizeof(double),1,fpOutput);
	fread(&aResultNode.dChiSQ,sizeof(double),1,fpOutput);
	fread(&aResultNode.dSSE,sizeof(double),1,fpOutput);
}

void CAbf_ProcessingDoc::InputFittedDTParameters()
{
	CString sFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results (binary)";
	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt or *.dlt)\0*.blt;*.clt;*.dlt\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		sFileName=aFileDlg.GetPathName();
		ReadFittingDTResultsCollection(m_avFittingResultsCollection,sFileName);
	}
}

void CAbf_ProcessingDoc::SelectRsultNodes(structFittedParameterNode aFittedNode, vector <structFittingResultNode> &avResultsNode)
{
	avResultsNode.clear();
	bool bSigComponents=false, bNumComponents=false, bFittingLevel=false;
	int nFileIndex,nLevelIndex,nComponentIndex;
	int nNumFittingLevels=0,nMinComponents=0,nMaxComponents=0,nFittingLevel=0,nSignificantComponents;
	int nNumFiles=m_avFittingResultsCollection.size();
	int nNumComponents=0;
	for(nFileIndex=0;nFileIndex<nNumFiles;nFileIndex++)
	{
		nNumFittingLevels=m_avFittingResultsCollection.at(nFileIndex).nNumFittingLevels;
		for(nLevelIndex=0;nLevelIndex<nNumFittingLevels;nLevelIndex++)
		{
			nFittingLevel=m_avFittingResultsCollection.at(nFileIndex).vnFittingLevels.at(nLevelIndex);
			bFittingLevel=(nFittingLevel==aFittedNode.nFittedLevel);
			nMinComponents=m_avFittingResultsCollection.at(nFileIndex).vnMinComponents.at(nLevelIndex);
			nMaxComponents=m_avFittingResultsCollection.at(nFileIndex).vnMaxComponents.at(nLevelIndex);
			nSignificantComponents=m_avFittingResultsCollection.at(nFileIndex).vnSignificantComponents.at(nLevelIndex);
			if(nSignificantComponents<aFittedNode.nSigComponentsL||nSignificantComponents>aFittedNode.nSigComponentsH)
			{
				bSigComponents=false;
			}
			else
			{
				bSigComponents=true;
			}
			int nNumComponentTrials=nMaxComponents-nMinComponents+1;
			for(nComponentIndex=0;nComponentIndex<nNumComponentTrials;nComponentIndex++)
			{
				nNumComponents=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(0).nNumComponents;
				bNumComponents=(nNumComponents==aFittedNode.nNumComponents);
				if(bFittingLevel&&bSigComponents&&bNumComponents)
				{
					avResultsNode.push_back(SelectRsultNodes(m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex)));
				}
			}
		}
	}
}

void CAbf_ProcessingDoc::SelectAllRsultNodes(structFittedParameterNode aFittedNode, vector <structFittingResultNode> &avResultsNode, int &nTrials)
{
	avResultsNode.clear();
	bool bSigComponents=false, bNumComponents=false, bFittingLevel=false;
	int nFileIndex,nLevelIndex,nComponentIndex;
	int nNumFittingLevels=0,nMinComponents=0,nMaxComponents=0,nFittingLevel=0,nSignificantComponents;
	int nNumFiles=m_avFittingResultsCollection.size();
	int nNumComponents=0;
	for(nFileIndex=0;nFileIndex<nNumFiles;nFileIndex++)
	{
		nNumFittingLevels=m_avFittingResultsCollection.at(nFileIndex).nNumFittingLevels;
		for(nLevelIndex=0;nLevelIndex<nNumFittingLevels;nLevelIndex++)
		{
			nFittingLevel=m_avFittingResultsCollection.at(nFileIndex).vnFittingLevels.at(nLevelIndex);
			bFittingLevel=(nFittingLevel==aFittedNode.nFittedLevel);
			nMinComponents=m_avFittingResultsCollection.at(nFileIndex).vnMinComponents.at(nLevelIndex);
			nMaxComponents=m_avFittingResultsCollection.at(nFileIndex).vnMaxComponents.at(nLevelIndex);
			nSignificantComponents=m_avFittingResultsCollection.at(nFileIndex).vnSignificantComponents.at(nLevelIndex);
			if(nSignificantComponents<aFittedNode.nSigComponentsL||nSignificantComponents>aFittedNode.nSigComponentsH)
			{
				bSigComponents=false;
			}
			else
			{
				bSigComponents=true;
			}
			int nNumComponentTrials=nMaxComponents-nMinComponents+1;
			for(nComponentIndex=0;nComponentIndex<nNumComponentTrials;nComponentIndex++)
			{
				nNumComponents=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(0).nNumComponents;
				bNumComponents=(nNumComponents==aFittedNode.nNumComponents);
				if(bFittingLevel&&bSigComponents&&bNumComponents)
				{
					int nSize0=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).size();
					nTrials=nSize0;
					for(int i=0;i<nSize0;i++)
					{
						avResultsNode.push_back(m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex)[i]);
					}
				}
			}
		}
	}
}


structFittingResultNode CAbf_ProcessingDoc::SelectRsultNodes(vector <structFittingResultNode> avResultsNode)
{
	int nTrialIndex;
	int nNumTrials=avResultsNode.size();
	vector <double> dvLogLik;
	vector <double> dvSSE;
	vector <double> dvMeanTO;
	vector <double> dvMeanTC;
	vector <double> dvChiSQ;
	vector <double> dvArea;
	double dMinDev=10.e99,dMTO=0.,dMTC=0.,dA=0.,dDev=0.;
	int nTheOne=0;
	for(nTrialIndex=0;nTrialIndex<nNumTrials;nTrialIndex++)
	{
		structFittingResultNode bNode=avResultsNode.at(nTrialIndex);
//		dvLogLik.push_back(bNode.dLogLikelihood);
//		dvSSE.push_back(bNode.dSSE);
//		dvArea.push_back(bNode.dFittedArea);
//		dvMeanTO.push_back(bNode.dMeanDwellTimeO);
//		dvMeanTC.push_back(bNode.dMeanDwellTimeC);
//		dvChiSQ.push_back(bNode.dChiSQ);
		dMTO=bNode.dMeanDwellTimeO;
		dMTC=bNode.dMeanDwellTimeC;
		dA=bNode.dFittedArea;
		dDev=fabs((dMTO-dMTC)/dMTO);
//		if(dA>0.01&&dDev<dMinDev)
		if(dA>0.01)
		{
			nTheOne=nTrialIndex;
			break;
			dMinDev=dDev;
		}
	}
	structFittingResultNode aNode=avResultsNode.at(nTheOne);
	return aNode;
}

void CAbf_ProcessingDoc::GetFittedparameters(structFittedParameterNode aFittedNode)
{
}

void CAbf_ProcessingDoc::OutputFittedDTParameters(structFittedParameterNode aFittedNode)
{
	CString sFileName;
	vector <CString> sEvlFileNames;
	typedef vector <float> FVector;
	typedef vector <double> DVector;
	vector <structFittingResultNode> avResultsNode;
	InputFittedDTParameters();
	SelectRsultNodes(aFittedNode, avResultsNode);
	int i,j;
	int nComponents=aFittedNode.nNumComponents;
	int nLevel=aFittedNode.nFittedLevel;
		bOutputConfidanceRanges=aFittedNode.bOutputConfidanceRanges;   //bOutputConfidanceRanges is a class data memter

	vector <FVector> fvectTimeConstants;
	vector <FVector> fvectWeights;
	vector <FVector> fvectConfidanceTL;
	vector <FVector> fvectConfidanceTR;
	vector <FVector> fvectConfidanceWL;
	vector <FVector> fvectConfidanceWR;
	vector <FVector> fvectConfidanceTLI;
	vector <FVector> fvectConfidanceTRI;
	vector <FVector> fvectConfidanceWLI;
	vector <FVector> fvectConfidanceWRI;
	vector <DVector> fvectBurstDelimiters;
	vector <int> nvScoreReanking;
	double dFrequency[20];
	vector <double> dvArea;
	vector <structFRange> structvFRange;
	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		vector <float> cV;
		vector <float> dV;
		vector <float> eV;
		vector <float> fV;
		vector <double> gV;
		vector <float> hV;
		vector <float> iV;
		vector <float> jV;
		vector <float> kV;
		fvectTimeConstants.push_back(aV);
		fvectWeights.push_back(bV);
		fvectConfidanceTL.push_back(cV);
		fvectConfidanceTR.push_back(dV);
		fvectConfidanceWL.push_back(eV);
		fvectConfidanceWR.push_back(fV);
		fvectBurstDelimiters.push_back(gV);
		fvectConfidanceTLI.push_back(hV);
		fvectConfidanceTRI.push_back(iV);
		fvectConfidanceWLI.push_back(jV);
		fvectConfidanceWRI.push_back(kV);
	}

	vector <double> dMTO;
	vector <double> dMTC;
	vector <double> dPo;
	vector <double> dLength;
	vector <double> dProbSingle;
	vector <double> dLogLik_R;
	vector <int> vnSig;
	vector <int> vnNumEvents;
	double dTemp;

	int nSize=avResultsNode.size();
	structFittingResultNode aNode;
	for(i=0;i<nSize;i++)
	{
		aNode=avResultsNode.at(i);
		vector <double> vdBurstDelimiters=BurstDelimiters(aNode.dTimeConstants,nComponents);
		for(j=0;j<nComponents;j++)
		{
			fvectTimeConstants.at(j).push_back(aNode.dTimeConstants[j]);
			fvectWeights.at(j).push_back(aNode.dWeights[j]);
			fvectConfidanceTL.at(j).push_back(aNode.fConfidanceRangeT[j].fLeft);
			fvectConfidanceTR.at(j).push_back(aNode.fConfidanceRangeT[j].fRight);
			fvectConfidanceWL.at(j).push_back(aNode.fConfidanceRangeW[j].fLeft);
			fvectConfidanceWR.at(j).push_back(aNode.fConfidanceRangeW[j].fRight);
			fvectConfidanceTLI.at(j).push_back(aNode.fConfidanceRangeTI[j].fLeft);
			fvectConfidanceTRI.at(j).push_back(aNode.fConfidanceRangeTI[j].fRight);
			fvectConfidanceWLI.at(j).push_back(aNode.fConfidanceRangeWI[j].fLeft);
			fvectConfidanceWRI.at(j).push_back(aNode.fConfidanceRangeWI[j].fRight);
			if(j<nComponents-1) 
			{
				dTemp=vdBurstDelimiters[j];
				fvectBurstDelimiters.at(j).push_back(float(dTemp));
			}
		}
		dMTO.push_back(aNode.dMeanDwellTimeO);
		dMTC.push_back(aNode.dMeanDwellTimeC);
		dLength.push_back(aNode.fRecordLength);
		dProbSingle.push_back(aNode.fProbSingle);
		dPo.push_back(aNode.fOpenProb);
		dLogLik_R.push_back(aNode.dLogLikelihood_R);
		sEvlFileNames.push_back(aNode.sCurFileName);
		nvScoreReanking.push_back(aNode.nScoreRanking);
		vnSig.push_back(aNode.nSignificantComponents);
		vnNumEvents.push_back(aNode.nNumEvents);
	}

	CString sMessage="Can not open the output file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Output Fitted Parameters";
	char* pch="Fitted Parameter Files(*.twn)\0*.twn\0\0";
	float fMean;
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		FILE* fpOutput;
		sFileName=aFileDlg.GetPathName();
		if(!(fpOutput=fopen(sFileName,"w")))
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
			nSize=fvectTimeConstants.at(0).size();
			fprintf(fpOutput,"    i     Score     Level");
			for(i=0;i<nComponents;i++)
			{
				if(bOutputConfidanceRanges) 
					fprintf(fpOutput,"   ConfidanceTL%1d",i);
				fprintf(fpOutput,"   TimeConstant%1d",i);
				if(bOutputConfidanceRanges) 
					fprintf(fpOutput,"   ConfidanceTR%1d",i);
			}
			for(i=0;i<nComponents;i++)
			{
				if(bOutputConfidanceRanges) 
					fprintf(fpOutput,"    ConfidanceWL%1d",i);
				fprintf(fpOutput,"          Weight%1d",i);
				if(bOutputConfidanceRanges) 
					fprintf(fpOutput,"    ConfidanceWR%1d",i);
			}

			if(bOutputConfidanceRanges) 
			{					
				for(i=0;i<nComponents;i++)
				{
					fprintf(fpOutput,"   ConfidanceTLI%1d",i);
					fprintf(fpOutput,"   TimeConstant%1d",i);
					fprintf(fpOutput,"   ConfidanceTRI%1d",i);
				}
				for(i=0;i<nComponents;i++)
				{
					fprintf(fpOutput,"    ConfidanceWLI%1d",i);
					fprintf(fpOutput,"          Weight%1d",i);
					fprintf(fpOutput,"    ConfidanceWRI%1d",i);
				}
			}

			for(i=0;i<nComponents;i++)
			{
				fprintf(fpOutput,"       Frequency%1d",i);
			}

			if(nLevel==0)
			{
				for(i=0;i<nComponents-1;i++)
				{
					fprintf(fpOutput,"   BstDelimiter%1d",i);
				}
			}
			fprintf(fpOutput,"  MDwellTime_O  MDwellTime_C    RecordLength   NumEvents  ProbSingle      LogLik_R  SigComponents            Po    EvlFile\n");
			for(i=0;i<nSize;i++)
			{
				fprintf(fpOutput,"%5d%10d%10d",i,nvScoreReanking[i],nLevel);
				for(j=0;j<nComponents;j++)
				{
					if(bOutputConfidanceRanges) 
						fprintf(fpOutput,"  %14.5f",fvectConfidanceTL.at(j)[i]);
					fprintf(fpOutput,"  %14.5f",fvectTimeConstants.at(j)[i]);
					if(bOutputConfidanceRanges) 
						fprintf(fpOutput,"  %14.5f",fvectConfidanceTR.at(j)[i]);
				}
				for(j=0;j<nComponents;j++)
				{
					if(bOutputConfidanceRanges) 
						fprintf(fpOutput,"  %15.7f",fvectConfidanceWL.at(j)[i]);
					fprintf(fpOutput,"  %15.7f",fvectWeights.at(j)[i]);
					if(bOutputConfidanceRanges) 
						fprintf(fpOutput,"  %15.7f",fvectConfidanceWR.at(j)[i]);
				}

				if(bOutputConfidanceRanges) 
				{
					for(j=0;j<nComponents;j++)
					{
						fprintf(fpOutput,"  %14.5f",fvectConfidanceTLI.at(j)[i]);
						fprintf(fpOutput,"  %14.5f",fvectTimeConstants.at(j)[i]);
						fprintf(fpOutput,"  %14.5f",fvectConfidanceTRI.at(j)[i]);
					}
					for(j=0;j<nComponents;j++)
					{
						fprintf(fpOutput,"  %15.7f",fvectConfidanceWLI.at(j)[i]);
						fprintf(fpOutput,"  %15.7f",fvectWeights.at(j)[i]);
						fprintf(fpOutput,"  %15.7f",fvectConfidanceWRI.at(j)[i]);
					}
				}

				Freq_Components(avResultsNode[i].dTimeConstants,avResultsNode[i].dWeights,nComponents,avResultsNode[i].fFittingRange,avResultsNode[i].nEventsInRange
					,avResultsNode[i].dFittedArea,dFrequency,dLength.at(i));
				//The length is in the unit of ms and the frequency will be exported in the unit of mHz
				for(j=0;j<nComponents;j++)
				{
					if(avResultsNode[i].dWeights[j]>0.f)
					{
						fprintf(fpOutput,"  %15.7f",dFrequency[j]*1000.);
						//Now the frequency will be exported in the unit of Hz
					}
					else
					{
						fprintf(fpOutput,"  %15.7f",-100.);
					}
				}
//				for(j=0;j<nComponents;j++)
//				{
//					fprintf(fpOutput,"  %15.7f",fvectWeights.at(j)[i]*vnNumEvents.at(i)*1000.f/dLength.at(i));
//				}

				if(nLevel==0)
				{
					for(j=0;j<nComponents-1;j++)
					{
						dTemp=fvectBurstDelimiters.at(j)[i];
						fprintf(fpOutput,"  %14.5f",dTemp);
					}
				}

				fprintf(fpOutput,"  %12.4f",dMTO.at(i));
				fprintf(fpOutput,"  %12.4f",dMTC.at(i));
				fprintf(fpOutput,"  %14.2f",dLength.at(i));
				fprintf(fpOutput,"  %10d",vnNumEvents.at(i));
				fprintf(fpOutput,"  %10.3e",dProbSingle.at(i));
				fprintf(fpOutput,"  %12.4f",dLogLik_R.at(i));
				fprintf(fpOutput,"%15d",vnSig[i]);
				fprintf(fpOutput,"  %12.4f    %s\n",dPo.at(i),sEvlFileNames.at(i));
			}
			fprintf(fpOutput,"\n\n");

			int nTemp;
			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectTimeConstants.at(i));
				fMean=aNode.fMean;
				nTemp=aNode.nSampleSize;
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
			}
			fprintf(fpOutput,"\n\n");

			for(i=0;i<nComponents;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectWeights.at(i));
				fMean=aNode.fMean;
				nTemp=aNode.nSampleSize;
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
			}
			fprintf(fpOutput,"\n\n");

			for(i=0;i<nComponents-1;i++)
			{
				structSemNode aNode;
				aNode=CalSem(fvectBurstDelimiters.at(i));
				fMean=aNode.fMean;
				nTemp=aNode.nSampleSize;
				fprintf(fpOutput,"%12.6f    %12.6f    %10d\n",fMean,aNode.fSem,nTemp);
			}
			fprintf(fpOutput,"\n\n");

		}

		OutputAllFittedParameters(fpOutput,aFittedNode);
		fclose(fpOutput);
	}
}

void CAbf_ProcessingDoc::Freq_Components(double *dT, double *dW, int nComponents, structFRange fFittingRange, 
												 int nEventsInRange, double dArea, double* dFrequency, float fLength)
{
	int nTotalEvents=nEventsInRange/dArea;
	for(int i=0;i<nComponents;i++)
	{
		dFrequency[i]=nTotalEvents*dW[i]*(exp(-fFittingRange.fLeft/dT[i])-exp(-fFittingRange.fRight/dT[i]))/fLength;
	}
}


void CAbf_ProcessingDoc::OutputAllFittedParameters(FILE* fpOutput, structFittedParameterNode aFittedNode)
{
	CString sFileName;
	vector <CString> sEvlFileNames;
	typedef vector <float> FVector;
	typedef vector <double> DVector;
	vector <structFittingResultNode> avResultsNode;
	int i,j,nTrials;
	int nComponents=aFittedNode.nNumComponents;
	int nLevel=aFittedNode.nFittedLevel;

	SelectAllRsultNodes(aFittedNode, avResultsNode, nTrials);

	vector <FVector> fvectTimeConstants;
	vector <FVector> fvectWeights;
	vector <FVector> fvectConfidanceTL;
	vector <FVector> fvectConfidanceTR;
	vector <FVector> fvectConfidanceWL;
	vector <FVector> fvectConfidanceWR;
	vector <DVector> fvectBurstDelimiters;
	vector <int> nvScoreReanking;
	vector <int> nvClusterIndex;
	for(i=0;i<nComponents;i++)
	{
		vector <float> aV;
		vector <float> bV;
		vector <float> cV;
		vector <float> dV;
		vector <float> eV;
		vector <float> fV;
		vector <double> gV;
		fvectTimeConstants.push_back(aV);
		fvectWeights.push_back(bV);
		fvectConfidanceTL.push_back(cV);
		fvectConfidanceTR.push_back(dV);
		fvectConfidanceWL.push_back(eV);
		fvectConfidanceWR.push_back(fV);
		fvectBurstDelimiters.push_back(gV);
	}


	vector <double> dMTO;
	vector <double> dMTC;
	vector <double> dPo;
	vector <double> dLength;
	vector <double> dProbSingle;
	vector <double> dLogLik_R;
	vector <int> vnSig;
	vector <int> vnNumEvents;
	double dTemp;

	int nSize=avResultsNode.size();
	structFittingResultNode aNode;
	for(i=0;i<nSize;i++)
	{
		aNode=avResultsNode.at(i);
		vector <double> vdBurstDelimiters=BurstDelimiters(aNode.dTimeConstants,nComponents);
		for(j=0;j<nComponents;j++)
		{
			fvectTimeConstants.at(j).push_back(aNode.dTimeConstants[j]);
			fvectWeights.at(j).push_back(aNode.dWeights[j]);
			fvectConfidanceTL.at(j).push_back(aNode.fConfidanceRangeT[j].fLeft);
			fvectConfidanceTR.at(j).push_back(aNode.fConfidanceRangeT[j].fRight);
			fvectConfidanceWL.at(j).push_back(aNode.fConfidanceRangeW[j].fLeft);
			fvectConfidanceWR.at(j).push_back(aNode.fConfidanceRangeW[j].fRight);
			if(j<nComponents-1) 
			{
				dTemp=vdBurstDelimiters[j];
				fvectBurstDelimiters.at(j).push_back(float(dTemp));
			}
		}
		dMTO.push_back(aNode.dMeanDwellTimeO);
		dMTC.push_back(aNode.dMeanDwellTimeC);
		dLength.push_back(aNode.fRecordLength);
		dProbSingle.push_back(aNode.fProbSingle);
		dPo.push_back(aNode.fOpenProb);
		dLogLik_R.push_back(aNode.dLogLikelihood_R);
		sEvlFileNames.push_back(aNode.sCurFileName);
		nvScoreReanking.push_back(aNode.nScoreRanking);
		nvClusterIndex.push_back(aNode.nClusterIndex);
		vnSig.push_back(aNode.nSignificantComponents);
		vnNumEvents.push_back(aNode.nNumEvents);
	}

	nSize=fvectTimeConstants.at(0).size();
	fprintf(fpOutput,"    i     Score     Level     Cluster");

	for(i=0;i<nComponents;i++)
	{
		if(bOutputConfidanceRanges) 
			fprintf(fpOutput,"   ConfidanceTL%1d",i);
		fprintf(fpOutput,"   TimeConstant%1d",i);
		if(bOutputConfidanceRanges) 
			fprintf(fpOutput,"   ConfidanceTL%1d",i);
	}
	for(i=0;i<nComponents;i++)
	{
		if(bOutputConfidanceRanges) 
			fprintf(fpOutput,"    ConfidanceWL%1d",i);
		fprintf(fpOutput,"          Weight%1d",i);
		if(bOutputConfidanceRanges) 
			fprintf(fpOutput,"    ConfidanceWL%1d",i);
	}


	if(nLevel==0)
	{
		for(i=0;i<nComponents-1;i++)
		{
			fprintf(fpOutput,"   BstDelimiter%1d",i);
		}
	}
	fprintf(fpOutput,"  MDwellTime_O  MDwellTime_C    RecordLength   NumEvents  ProbSingle      LogLik_R  SigComponents            Po    EvlFile\n");
	for(i=0;i<nSize;i++)
	{
		if((i%nTrials)==0)
		{
			fprintf(fpOutput,"\n\n");
		}
		fprintf(fpOutput,"%5d%10d%10d%12d",i,nvScoreReanking[i],nLevel,nvClusterIndex[i]);

		for(j=0;j<nComponents;j++)
		{
			if(bOutputConfidanceRanges) 
				fprintf(fpOutput,"  %14.5f",fvectConfidanceTL.at(j)[i]);
			fprintf(fpOutput,"  %14.5f",fvectTimeConstants.at(j)[i]);
			if(bOutputConfidanceRanges) 
				fprintf(fpOutput,"  %14.5f",fvectConfidanceTR.at(j)[i]);
		}
		for(j=0;j<nComponents;j++)
		{
			if(bOutputConfidanceRanges) 
				fprintf(fpOutput,"  %15.7f",fvectConfidanceWL.at(j)[i]);
			fprintf(fpOutput,"  %15.7f",fvectWeights.at(j)[i]);
			if(bOutputConfidanceRanges) 
				fprintf(fpOutput,"  %15.7f",fvectConfidanceWR.at(j)[i]);
		}



		if(nLevel==0)
		{
			for(j=0;j<nComponents-1;j++)
			{
				dTemp=fvectBurstDelimiters.at(j)[i];
				fprintf(fpOutput,"  %14.5f",dTemp);
			}
		}
		fprintf(fpOutput,"  %12.4f",dMTO.at(i));
		fprintf(fpOutput,"  %12.4f",dMTC.at(i));
		fprintf(fpOutput,"  %14.2f",dLength.at(i));
		fprintf(fpOutput,"  %10d",vnNumEvents.at(i));
		fprintf(fpOutput,"  %10.3e",dProbSingle.at(i));
		fprintf(fpOutput,"  %12.4f",dLogLik_R.at(i));
		fprintf(fpOutput,"%15d",vnSig[i]);
		fprintf(fpOutput,"  %12.4f    %s\n",dPo.at(i),sEvlFileNames.at(i));
	}
	fprintf(fpOutput,"\n\n");
}

vector <double> CAbf_ProcessingDoc::BurstDelimiters(double dT[], int nDim)
{
	vector <double> dBurstDelimiters;
	double dTemp;
	for(int i=0;i<nDim-1;i++)
	{
		dTemp=BurstDelimiter(dT[i],dT[i+1]);
		dBurstDelimiters.push_back(dTemp);
	}
	return dBurstDelimiters;
}




void CAbf_ProcessingDoc::OnAnalysisEventlistBurstingtimecourse() 
{
	// TODO: Add your command handler code here
	if(!m_bEvl)
	{
		OnFileImportEventfile();
	}
//	m_cEvl.DoBurstingTimeCourse();//This function should modifies the event list, so it should not run prior to other functions using the event list.
	ImportDataFile("import the data file of the imported event list file");
	AbfNode aNode;

	if(m_sAbfFileName.Compare("Canceled")==0)
	{
		aNode.nSize=0;}else{
		aNode=m_cAbf.GetTrace();
	}

	m_cEvl.DoBurstingTimeCourse_UB(aNode);	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistDwellcurveconstruction() 
{
	// TODO: Add your command handler code here
	//Need to consider the case when evl files are not available
	FILE *fp;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sInputFileName=PassingString(sMessage);

	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Input File (*.Dfc, Dfd or Dfe)\0*.Dfc;*.Dfd;*.Dfe\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Read an input file for dwell fitting curve construction";
	float fRecordLength=0.f;
	float fBurstDelimiter, fClusterDelimiter;
	int nFilteringKey;
	bool bFilterEvl=false;

	if(aFileDlg.DoModal()==IDOK)
	{
		sInputFileName=aFileDlg.GetPathName();
		if(fp=fopen(sInputFileName,"r"))
		{
			int i, nComponents;
			bool bEvl=false;
			CString sEvlFileName=PassingString(sInputFileName);
			char sTemp[400];
			float fFittedArea,fL,fR,fW,fT;
			CString sExt=FileExt(sInputFileName);
			while(!feof(fp))
			{
				fscanf(fp,"%s",sEvlFileName);
				CFileDialog bFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
				char* pch="Event List Files (*.evl)\0*.evl\0\0";
				bFileDlg.m_ofn.lpstrFilter=pch;
				bFileDlg.m_ofn.lpstrTitle="Read the fitted evl file";
				strcpy(bFileDlg.m_ofn.lpstrFile,sEvlFileName);
				bEvl=false;
				if(bFileDlg.DoModal()==IDOK)
				{
					sEvlFileName=bFileDlg.GetPathName();
					m_cEvl.ReadEvl(sEvlFileName);
					bEvl=true;
				}
				structFittingResultNode aResultNode;

				if(sExt=="Dfd")
				{
	//				fscanf(fp,"%s %f %s %d",sTemp, &aResultNode.fRecordLength, sTemp, &aResultNode.nNumEvents);
					//This information will be obtained from the histogram generated using the evl file sEvlFileName
				}
				else
				{
					aResultNode.fRecordLength=-1.f;
					aResultNode.nNumEvents=-1;
				}

				aResultNode.sFileName=PassingString(sEvlFileName);
				fscanf(fp,"%s %d %s %d",sTemp,&aResultNode.nDwellLevel,sTemp,&aResultNode.nNumComponents);
				fscanf(fp,"%s %f %s %f",sTemp,&fL,sTemp,&fR);
				aResultNode.fFittingRange.fLeft=fL;
				aResultNode.fFittingRange.fRight=fR;
				fscanf(fp,"%s %f",sTemp,&fFittedArea);
				aResultNode.dFittedArea=fFittedArea;
				fscanf(fp,"%s %d",sTemp,&aResultNode.nEventsInRange);
				nComponents=aResultNode.nNumComponents;
				float fTemp;

				if(sExt=="Dfe"||sExt=="dfe"||sExt=="DFE")
				{
					fscanf(fp,"%s%d",sTemp,&nFilteringKey);
					if(nFilteringKey>0)
					{
						fscanf(fp,"%s%f%s%f",sTemp,&fBurstDelimiter,sTemp,&fClusterDelimiter);
						bFilterEvl=true;
					}
				}

				if(sExt=="Dfd"||sExt=="Dfe")
				{
					fscanf(fp,"%s %s %s %s %s %s",sTemp,sTemp,sTemp,sTemp,sTemp,sTemp);
					for(i=0;i<nComponents;i++)
					{
						fscanf(fp,"%f%f%f%f%f%f",&fTemp,&fW,&fTemp,&fTemp,&fT,&fTemp);
						aResultNode.dWeights[i]=fW;
						aResultNode.dTimeConstants[i]=fT;
					}
				}
				else
				{
					fscanf(fp,"%s %s",sTemp,sTemp);
					for(i=0;i<nComponents;i++)
					{
						fscanf(fp,"%f%f",&fW,&fT);
						aResultNode.dWeights[i]=fW;
						aResultNode.dTimeConstants[i]=fT;
					}
				}

//				int nFilteringKey=4;
//				CEvlHistogramDlg aDlg;
//				if(aDlg.DoModal()==IDOK)
//				{
//					bFilterEvl=aDlg.FilterEvl();
//					nFilteringKey=aDlg.FilteringKey();
//				}
				if(bFilterEvl) m_cEvl.FilteringEvents(nFilteringKey,fBurstDelimiter, fClusterDelimiter);

				m_cEvl.ExportDwellCurves(aResultNode,bEvl);
			}
		    fclose(fp);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
	}		
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportExport2devlhistogram() 
{
	// TODO: Add your command handler code here
	if(!m_bEvl)
	{
		OnFileImportEventfile();
	}
	m_cEvl.Export2DEvlHistogram();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistCombineevlfiles() 
{
	// TODO: Add your command handler code here
	CString sEvlFileName;
	CString sEvlFileNames;
	char* pch="Event List Files (*.EVL)\0*.EVL\0All Files\0*.*\0\0";
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrFile=sEvlFileNames.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	aFileDlg.m_ofn.lpstrTitle="Import the event list files for combining";
	POSITION aPosition;
	bool bFirstFile=true;
	vector <CString> svEvlFileNames;
	if(aFileDlg.DoModal()==IDOK)
	{
		aPosition=aFileDlg.GetStartPosition();
		while(aPosition!=NULL)
		{
			sEvlFileName=aFileDlg.GetNextPathName(aPosition);
			sEvlFileName=PassingString(sEvlFileName);
			svEvlFileNames.push_back(sEvlFileName);
			if(!bFirstFile) m_cEvl.SetAttachingStatus();
			m_cEvl.ReadEvl(sEvlFileName);
			bFirstFile=false;
		}
	}

	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	pch="Event List Files (*.evl)\0*.Evl\0\0";
	bFileDlg.m_ofn.lpstrFilter=pch;
	bFileDlg.m_ofn.lpstrTitle="Export the combined event list file";
	if(bFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=bFileDlg.GetPathName();
		m_cEvl.WriteEvl(sEvlFileName);
	}
	
	CString sListFileName=ChangeFileExt(sEvlFileName,"txt");
	FILE* fp=fopen(sListFileName,"w");

	fprintf(fp,"The combined evl file(%s) are from the following evl files.\n",sEvlFileName);
	int nSize=svEvlFileNames.size();
	for(int i=0;i<nSize;i++)
	{
		fprintf(fp,"%s\n",svEvlFileNames[i]);
	}
	fclose(fp);
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTEXPORTEXPORTSTATITICsOFBURSTANALYSIS() 
{
	// TODO: Add your command handler code here
	if(!m_bEvl)
	{
		OnFileImportEventfile();
	}
	m_cEvl.ExportBurstStatitics();	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistComparison0Abfdwellingtimecomparison() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
		char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;

		CString sTitle1;
		
//		if(!m_bAbf)
//		{
			sTitle1="Import a data file for cocmaring with dwelling time";
			OnFileImportDatafile(sTitle1);
//		}

		
//		if(!m_bEvl)
//		{
			sTitle1="Import a event list file for cocmaring with dwelling time";
			OnFileImportEventfile(sTitle1);
//		}

//		m_cDualChannelAbf.EqualizeScale();
		m_cDualChannelAbf.SetScale(1,100.f);
		float fMaxAmp=m_cDualChannelAbf.GetMaxAmp(1);

		AbfNode aNode=m_cAbf.GetTrace();
		AbfNode bNode=m_cEvl.GetDwellingTimeAsTrace(fMaxAmp);
		m_cEvl.Summarize();
//		structEvlSummaryNode aEvlSumNode=m_cEvl.GetSummaryNode();
		int nSize1=aNode.nSize;
		int nSize2=bNode.nSize;
		float *pfData=new float[nSize1+nSize2];
		int i,i0;
		int nPosition=0;
		float fTemp;
		int nSize=nSize1;
		if(nSize>nSize2)
		{
			nSize=nSize2;
		}
		for(i=0;i<nSize;i++)
		{
				pfData[nPosition]=aNode.pfData[i];
				nPosition++;
				fTemp=bNode.pfData[i]*100.;
				if(fTemp>fabs(fMaxAmp))
					fTemp=fMaxAmp;
				if(fTemp<-fabs(fMaxAmp))
					fTemp=-fabs(fMaxAmp);
				pfData[nPosition]=fTemp;
				nPosition++;
		}

		m_cDualChannelAbf.UpdateSize(2*nSize);
		m_cDualChannelAbf.UpdateData(pfData);
		m_cDualChannelAbf.ChannelNameUnit("DwellTime","ms",1);
		delete pfData;

		FILE *fpAbfOut;
		CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
		CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
		bFileDlg.m_ofn.lpstrFilter=pch;
		bFileDlg.m_ofn.lpstrTitle="Save the dual comparison into a data file";
		char* pch="pClamp data Files (*.abf)\0*.dat\0\0";
		bFileDlg.m_ofn.lpstrFilter=pch;
		if(bFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=bFileDlg.GetPathName();
			if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
			{
	//			AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				int nFormat=1;
	    		m_cDualChannelAbf.WriteData(fpAbfOut);
	     		fclose(fpAbfOut);
			}
		}			
	}			
}

void CAbf_ProcessingDoc::OnAnalysisEventlistComparisonEvlampabfcomparison() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
		char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}				


		CString sTitle1;
		
//		if(!m_bAbf)
//		{
			sTitle1="Import a data file for cocmaring with dwelling time";
			OnFileImportDatafile(sTitle1);
//		}

		
//		if(!m_bEvl)
//		{
			sTitle1="Import a event list file for cocmaring with dwelling time";
			OnFileImportEventfile(sTitle1);
//		}

		m_cDualChannelAbf.EqualizeScale();

		AbfNode aNode=m_cAbf.GetTrace();
		AbfNode bNode=m_cEvl.GetAmpAsTrace();
		m_cEvl.Summarize();
//		structEvlSummaryNode aEvlSumNode=m_cEvl.GetSummaryNode();
		int nSize1=aNode.nSize;
		int nSize2=bNode.nSize;
		float *pfData=new float[nSize1+nSize2];
		int i,i0;
		int nPosition=0;
		float fTemp;
		int nSize=nSize1;
		if(nSize>nSize2)
		{
			nSize=nSize2;
		}
		for(i=0;i<nSize;i++)
		{
				pfData[nPosition]=aNode.pfData[i];
				nPosition++;
				pfData[nPosition]=bNode.pfData[i];
				nPosition++;
		}

		m_cDualChannelAbf.UpdateSize(2*nSize);
		m_cDualChannelAbf.UpdateData(pfData);
		m_cDualChannelAbf.ChannelNameUnit("DwellTime","ms",1);
		delete pfData;

		FILE *fpAbfOut;
		CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
		CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
		bFileDlg.m_ofn.lpstrFilter=pch;
		bFileDlg.m_ofn.lpstrTitle="Save the dual comparison into a data file";
//		char* pch="pClamp data Files (*.abf)\0*.dat\0\0";
		pch="pClamp data Files (*.abf)\0*.dat\0\0";
		bFileDlg.m_ofn.lpstrFilter=pch;
		if(bFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=bFileDlg.GetPathName();
			if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
			{
	//			AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				int nFormat=1;
	    		m_cDualChannelAbf.WriteData(fpAbfOut);
	     		fclose(fpAbfOut);
			}
		}			
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExcludesubconductances() 
{
	// TODO: Add your command handler code here
	m_cEvl.ExcludeSubconductances();
}

void CAbf_ProcessingDoc::SetConfidanceRange(int nFuncKey, vector <structFittingResultNode> &avResultNodes)
{
	ResultNodeClusters(avResultNodes);
	m_cEvl.SetConfidanceRange(nFuncKey, avResultNodes);
}

void CAbf_ProcessingDoc::ResultNodeClusters(vector <structFittingResultNode> &avResultNodes)
{
	//This function assumes all resultnodes are sorted in ascending order according to dLogLik's.
	structFittingResultNode aNode,bNode;
	int nSize=avResultNodes.size();
	int i,j;
	for(i=0;i<nSize;i++)
	{
		aNode=avResultNodes.at(i);
		for(j=i+1;j<nSize;j++)
		{
			bNode=avResultNodes.at(j);
			if(DegeneratedResultNodes(aNode,bNode))
			{
				avResultNodes.at(j).nClusterIndex=avResultNodes.at(i).nClusterIndex;
			}
		}
	}
}

bool CAbf_ProcessingDoc::DegeneratedResultNodes(structFittingResultNode aResultNode, structFittingResultNode bResultNode)
{
	double dT1, dT2;
	double dTol=0.2;
	int nComp=aResultNode.nNumComponents;
	for(int i=0;i<nComp;i++)
	{
		dT1=aResultNode.dTimeConstants[i];
		dT2=bResultNode.dTimeConstants[i];
		if(fabs((dT1-dT2)/(dT1+dT2))>dTol)
			return false;
	}
	return true;
}
void CAbf_ProcessingDoc::OnAnalysisEventlistBurstsNumofopeningperburst() 
{
	// TODO: Add your command handler code here
	CString sTitle="Input an event list file for burst analysis";
	OnFileImportEventfile(sTitle);
	m_cEvl.FitNOPB();
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTExportEvlAsTrace() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;

		CString sTitle1;
		
		if(!m_bAbf)
		{
			sTitle1="Import a data file for the header section";
			OnFileImportDatafile(sTitle1);
		}

		
		if(!m_bEvl)
		{
			sTitle1="Import a event list file to export as a trace";
			OnFileImportEventfile(sTitle1);
		}


		AbfNode aNode=m_cEvl.GetEvlAsTrace();

		int nSize=aNode.nSize;
		float *pfData=new float[nSize];
		int i,i0;
		float fTemp;
		for(i=0;i<nSize;i++)
		{
				pfData[i]=aNode.pfData[i];
		}

		m_cAbf.UpdateSize(nSize);
		m_cAbf.UpdateData(pfData);
		delete pfData;

		FILE *fpAbfOut;
		CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
		CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
		bFileDlg.m_ofn.lpstrFilter=pch;
		bFileDlg.m_ofn.lpstrTitle="Save the Evl file as an dat file";
		pch="pClamp data Files (*.abf)\0*.dat\0\0";
		bFileDlg.m_ofn.lpstrFilter=pch;
		if(bFileDlg.DoModal()==IDOK)
		{
			sAbfFileName=bFileDlg.GetPathName();
			if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
			{
	//			AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				int nFormat=1;
	    		m_cAbf.WriteData(fpAbfOut);
	     		fclose(fpAbfOut);
			}
		}							
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTBURSTSNUMOFOPENINGPERBURSTBatch() 
{
	// TODO: Add your command handler code here
	int nLevel,nSize,i,j;
	int nComponentIndex,nFileIndex,nLevelIndex,nTrialIndex;
	int nDim,nSize1;
	double dT[20];
	int nMaxOpening=99999999;
	int nMinOpening=0;
	int nBinSize=1;
	float fCutoff;


	CString sFileName;
	CString sEvlFileName=PassingString(sFileName);
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt or *.dlt)\0*.blt;*.clt;*.dlt\0\0";
//	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt)\0*.blt;*.clt\0\0";
	CString sForBuffer;
	aFileDlg.m_ofn.lpstrFile=sForBuffer.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results (binary)";
	POSITION aPosition;
	if(aFileDlg.DoModal()==IDOK)
	{
		//File Diolog Levl
		aPosition=aFileDlg.GetStartPosition();

		while(aPosition!=NULL)
		{
			//Iteration at blt(clt)-file level
			sFileName=aFileDlg.GetNextPathName(aPosition);

			ReadFittingDTResultsCollection(m_avFittingResultsCollection,sFileName);
			CFittedEvlParamterHandling aHandling;
			nLevel=0;
			vector <structDTNodeSignature> avDTNodeSignatures=aHandling.SelectParameterNodes(m_avFittingResultsCollection,0);
			nSize=avDTNodeSignatures.size();

			for(i=0;i<nSize;i++)
			{
				nFileIndex=avDTNodeSignatures[i].nFileIndex;
				nLevelIndex=avDTNodeSignatures[i].nLevelIndex;
				nComponentIndex=avDTNodeSignatures[i].nComponentIndex;
				nTrialIndex=avDTNodeSignatures[i].nTrialIndex;
				sEvlFileName=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).sCurFileName;
				CString sNOPBFileName=ExtendFileName(sEvlFileName,"_NOPB");
				sNOPBFileName=m_cEvl.ChangeExt(sNOPBFileName,"Nob");
				FILE* fpTotal=fopen(sNOPBFileName,"w");			
				fprintf(fpTotal,"   BDLMTER     Events     MeanNOPB ");
				int k;
				for(j=1;j<=4;j++)
				{
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"    %1dWeight%1d",j,k);
					}
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"       %1dRol%1d",j,k);
					}
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"        %1dMu%1d",j,k);
					}
					fprintf(fpTotal,"    MeanNOPB");
				}
				for(j=1;j<4;j++)
				{
					fprintf(fpTotal,"         pValue");
				}
				fprintf(fpTotal,"       Comp       EveFile\n");

				nDim=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).nNumComponents;
				for(j=0;j<nDim;j++)
				{
					dT[j]=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).dTimeConstants[j];
				}
				vector <double> vdBD=BurstDelimiters(dT, nDim);
				nSize1=vdBD.size();
				m_cEvl.ReadEvl(sEvlFileName);
				for(j=0;j<nSize1;j++)
				{
					fCutoff=vdBD[j];
					m_cEvl.FitNOPB(fCutoff,nBinSize,nMaxOpening,nMinOpening,j,fpTotal);
				}
				fclose(fpTotal);
			}
		}
	}
}


CString CAbf_ProcessingDoc::ExtendFileName(CString sFileName, CString sInsert)
{
	CString sNewFileName=sFileName;
	int nLength=sInsert.GetLength();
	for(int i=0;i<nLength;i++)
	{
		sNewFileName=ExtendFileName(sNewFileName,sInsert[i]);
	}
	return sNewFileName;
}


CString CAbf_ProcessingDoc::ExtendFileName(CString sFileName, char ch)
{
	CString sNewFileName=sFileName;
	int nLength=sFileName.GetLength();
	sNewFileName.Insert(nLength-4,ch);
	return sNewFileName;
}



void CAbf_ProcessingDoc::OnAnalysisEventlistBurstTotpbBatch() 
{
	// TODO: Add your command handler code here
	int nLevel,nSize,i,j;
	int nComponentIndex,nFileIndex,nLevelIndex,nTrialIndex;
	int nDim,nSize1;
	double dT[20];
	int nMaxOpening=99999999;
	int nMinOpening=0;
	int nBinSize=1;
	float fCutoff;

	CString sFileName;
	CString sEvlFileName=PassingString(sFileName);
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt or *.dlt)\0*.blt;*.clt;*.dlt\0\0";
//	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt)\0*.blt;*.clt\0\0";
	CString sForBuffer;
	aFileDlg.m_ofn.lpstrFile=sForBuffer.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results (binary)";
	POSITION aPosition;
	if(aFileDlg.DoModal()==IDOK)
	{
		//File Diolog Levl
		aPosition=aFileDlg.GetStartPosition();

		while(aPosition!=NULL)
		{
			//Iteration at blt(clt)-file level
			sFileName=aFileDlg.GetNextPathName(aPosition);

			ReadFittingDTResultsCollection(m_avFittingResultsCollection,sFileName);
			CFittedEvlParamterHandling aHandling;
			nLevel=0;
			vector <structDTNodeSignature> avDTNodeSignatures=aHandling.SelectParameterNodes(m_avFittingResultsCollection,0);
			nSize=avDTNodeSignatures.size();

			for(i=0;i<nSize;i++)
			{
				//Iteration at Evl-file level
				nFileIndex=avDTNodeSignatures[i].nFileIndex;
				nLevelIndex=avDTNodeSignatures[i].nLevelIndex;
				nComponentIndex=avDTNodeSignatures[i].nComponentIndex;
				nTrialIndex=avDTNodeSignatures[i].nTrialIndex;
				sEvlFileName=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).sCurFileName;
				CString sTOTPBFileName=ExtendFileName(sEvlFileName,"_TOTPB");
				sTOTPBFileName=m_cEvl.ChangeExt(sTOTPBFileName,"Tot");
				FILE* fpTotal=fopen(sTOTPBFileName,"w");			
				fprintf(fpTotal,"   BDLMTER     Events      MeanTOT ");
				int k;
				for(j=1;j<=4;j++)
				{
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"    %1dWeight%1d",j,k);
					}
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"       %1dTOT%1d",j,k);
					}
					fprintf(fpTotal,"   MeanTOTPB");
				}
				for(j=1;j<4;j++)
				{
					fprintf(fpTotal,"          pValue");
				}
				fprintf(fpTotal,"       Comp       EveFile\n");

				nDim=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).nNumComponents;
				for(j=0;j<nDim;j++)
				{
					dT[j]=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).dTimeConstants[j];
				}
				vector <double> vdBD=BurstDelimiters(dT, nDim);
				nSize1=vdBD.size();
				m_cEvl.ReadEvl(sEvlFileName);
				int nFuncKey=3;
				for(j=0;j<nSize1;j++)
				{
					fCutoff=vdBD[j];
					m_cEvl.FitTOTPB(fCutoff,nBinSize,nMaxOpening,nMinOpening,j+1,fpTotal,nFuncKey);
				}
				fclose(fpTotal);
				//Iteration at Evl-file level
			}
			//Iteration at blt(clt)-file level
		}
		//File Diolog Levl
	}	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistBurstTotpb() 
{
	// TODO: Add your command handler code here
	int nLevel,nSize,i,j;
	int nComponentIndex,nFileIndex,nLevelIndex,nTrialIndex;
	int nDim,nSize1;
	double dT[20];
	int nMaxOpening=99999999;
	int nMinOpening=0;
	int nBinSize=1;
	float fCutoff;

	CString sFileName;
	CString sEvlFileName=PassingString(sFileName);
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Input an event list file (evl)";
	char *pch="Event List Files(*.Evl)\0*.evl\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		CFittedEvlParamterHandling aHandling;
		nLevel=0;
		CString sTOTPBFileName=ExtendFileName(sEvlFileName,"_TOTPB_");
		sTOTPBFileName=m_cEvl.ChangeExt(sTOTPBFileName,"Tot");
		FILE* fpTotal=fopen(sTOTPBFileName,"w");			
		fprintf(fpTotal,"   BDLMTER     Events      MeanTOT ");
		int k;
		for(j=1;j<=4;j++)
		{
			for(k=1;k<=j;k++)
			{
				fprintf(fpTotal,"    %1dWeight%1d",j,k);
			}
			for(k=1;k<=j;k++)
			{
				fprintf(fpTotal,"       %1dTOT%1d",j,k);
			}
			fprintf(fpTotal,"   MeanTOTPB");
		}
		for(j=1;j<4;j++)
		{
			fprintf(fpTotal,"          pValue");
		}
		fprintf(fpTotal,"       Comp       EveFile\n");

		m_cEvl.ReadEvl(sEvlFileName);
		int nFuncKey=3;
		m_cEvl.FitTOTPB(fpTotal,nFuncKey);
		fclose(fpTotal);
	}		
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTBurstDuration() 
{
	// TODO: Add your command handler code here
	int nLevel,nSize,i,j;
	int nComponentIndex,nFileIndex,nLevelIndex,nTrialIndex;
	int nDim,nSize1;
	double dT[20];
//	int nMaxOpening=99999999;
//	int nMinOpening=0;
	int nBinSize=1;
	float fCutoff;

	CString sFileName;
	CString sEvlFileName=PassingString(sFileName);
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	aFileDlg.m_ofn.lpstrTitle="Input an event list file (evl)";
	char *pch="Event List Files(*.Evl)\0*.evl\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	if(aFileDlg.DoModal()==IDOK)
	{
		sEvlFileName=aFileDlg.GetPathName();
		CFittedEvlParamterHandling aHandling;
		nLevel=0;
		CString sBSTFileName=ExtendFileName(sEvlFileName,"_BST_");
		sBSTFileName=m_cEvl.ChangeExt(sBSTFileName,"Bst");
		FILE* fpTotal=fopen(sBSTFileName,"w");			
		fprintf(fpTotal,"   BDLMTER     Events      MeanBST ");
		int k;
		for(j=1;j<=4;j++)
		{
			for(k=1;k<=j;k++)
			{
				fprintf(fpTotal,"    %1dWeight%1d",j,k);
			}
			for(k=1;k<=j;k++)
			{
				fprintf(fpTotal,"       %1dBST%1d",j,k);
			}
			fprintf(fpTotal,"     MeanBST");
		}
		for(j=1;j<4;j++)
		{
			fprintf(fpTotal,"          pValue");
		}
		fprintf(fpTotal,"       Comp       EveFile\n");

		m_cEvl.ReadEvl(sEvlFileName);
		int nFuncKey=3;
		m_cEvl.FitBST(fpTotal,nFuncKey);
		fclose(fpTotal);
	}			
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTBurstDurationBatch() 
{
	// TODO: Add your command handler code here
	int nLevel,nSize,i,j,k;
	int nComponentIndex,nFileIndex,nLevelIndex,nTrialIndex;
	int nDim,nSize1;
	double dT[20];
	int nMaxOpening=99999999;
	int nMinOpening=0;
	int nBinSize=1;
	float fCutoff;


	CString sFileName;
	CString sEvlFileName=PassingString(sFileName);
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_ENABLESIZING|OFN_ALLOWMULTISELECT , NULL, NULL );
	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt or *.dlt)\0*.blt;*.clt;*.dlt\0\0";
//	char *pch="Automated Dwell-time Fitting Result(Binary)(*.blt or *.clt)\0*.blt;*.clt\0\0";
	CString sForBuffer;
	aFileDlg.m_ofn.lpstrFile=sForBuffer.GetBuffer(7200);
	aFileDlg.m_ofn.nMaxFile=7200;
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Input Dwell-time Fitting Results (binary)";
	POSITION aPosition;
	if(aFileDlg.DoModal()==IDOK)
	{
		//File Diolog Levl
		aPosition=aFileDlg.GetStartPosition();

		while(aPosition!=NULL)
		{
			//Iteration at blt(clt)-file level
			sFileName=aFileDlg.GetNextPathName(aPosition);

			ReadFittingDTResultsCollection(m_avFittingResultsCollection,sFileName);
			CFittedEvlParamterHandling aHandling;
			nLevel=0;
			vector <structDTNodeSignature> avDTNodeSignatures=aHandling.SelectParameterNodes(m_avFittingResultsCollection,0);
			nSize=avDTNodeSignatures.size();

			for(i=0;i<nSize;i++)
			{
				nFileIndex=avDTNodeSignatures[i].nFileIndex;
				nLevelIndex=avDTNodeSignatures[i].nLevelIndex;
				nComponentIndex=avDTNodeSignatures[i].nComponentIndex;
				nTrialIndex=avDTNodeSignatures[i].nTrialIndex;
				sEvlFileName=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).sCurFileName;
				CString sBSTFileName=ExtendFileName(sEvlFileName,"_BST");
				sBSTFileName=m_cEvl.ChangeExt(sBSTFileName,"Bst");
				FILE* fpTotal=fopen(sBSTFileName,"w");			
				fprintf(fpTotal,"   BDLMTER     Events      MeanBST ");//BST = burst duration
				for(j=1;j<=4;j++)
				{
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"    %1dWeight%1d",j,k);
					}
					for(k=1;k<=j;k++)
					{
						fprintf(fpTotal,"       %1dBST%1d",j,k);
					}
					fprintf(fpTotal,"      MeanBST");
				}
				for(j=1;j<4;j++)
				{
					fprintf(fpTotal,"          pValue");
				}
				fprintf(fpTotal,"       Comp       EveFile\n");

				nDim=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).nNumComponents;
				for(j=0;j<nDim;j++)
				{
					dT[j]=m_avFittingResultsCollection.at(nFileIndex).aTotalVVResults.at(nLevelIndex).at(nComponentIndex).at(nTrialIndex).dTimeConstants[j];
				}
				vector <double> vdBD=BurstDelimiters(dT, nDim);
				nSize1=vdBD.size();
				m_cEvl.ReadEvl(sEvlFileName);
				int nFuncKey=3;
				for(j=0;j<nSize1;j++)
				{
					fCutoff=vdBD[j];
					m_cEvl.FitBST(fCutoff,nBinSize,nMaxOpening,nMinOpening,j+1,fpTotal,nFuncKey);
				}
				fclose(fpTotal);
			}
		}
	}	
}

void CAbf_ProcessingDoc::CombiningDataNodes(AbfNode aNode, AbfNode bNode, AbfNode &NewNode)
{
	int i,j;
	bool bRescale=false;
	float fRatio=aNode.fSampleInterval/bNode.fSampleInterval;
	AbfNode cNode;
	float fSampleInterval;
	float *pfData;
	int nSize=0;
	if(fRatio<0.999)
	{
		//need to resale (time scale) aNode
		fSampleInterval=bNode.fSampleInterval;
		NewNode.fSampleInterval=fSampleInterval;
		nSize=int(aNode.fSampleInterval*aNode.nSize/fSampleInterval);
		NewNode.nSize=nSize+bNode.nSize;
		NewNode.pfData=new float[nSize+bNode.nSize];
		int nN=i_gcd(nSize,aNode.nSize);
		int nN1=aNode.nSize/nN;
		int nN2=nSize/nN;
		int nPosition=0;
		int nPosition1=0;
		for(i=0;i<nN;i++)
		{
			for(j=1;j<=nN2;j++)
			{
				nPosition1=i*nN1+int(j*double(nN1/nN2))-1;
				NewNode.pfData[nPosition]=aNode.pfData[nPosition1];
				nPosition++;
			}
		}
		for(i=0;i<bNode.nSize;i++)
		{
			NewNode.pfData[nPosition]=bNode.pfData[i];
			nPosition++;
		}
	}
	else if(fRatio>1.001)
	{
		//need to rescale (time scale) bNode
		fSampleInterval=aNode.fSampleInterval;
		NewNode.fSampleInterval=fSampleInterval;
		nSize=int(bNode.fSampleInterval*bNode.nSize/fSampleInterval);
		NewNode.nSize=nSize+aNode.nSize;
		NewNode.pfData=new float[nSize+aNode.nSize];
		int nN=i_gcd(nSize,bNode.nSize);
		int nN1=bNode.nSize/nN;
		int nN2=nSize/nN;
		int nPosition=0;
		int nPosition1=0;

		for(i=0;i<bNode.nSize;i++)
		{
			NewNode.pfData[nPosition]=aNode.pfData[i];
			nPosition++;
		}

		for(int i=0;i<nN;i++)
		{
			for(j=1;j<=nN2;j++)
			{
				nPosition1=i*nN1+int(j*double(nN1/nN2))-1;
				NewNode.pfData[nPosition]=bNode.pfData[nPosition1];
				nPosition++;
			}
		}
	}
	else 
	{
		//Do not need to rescale (the time scale)
		//need to rescale (time scale) bNode
		fSampleInterval=aNode.fSampleInterval;
		NewNode.fSampleInterval=fSampleInterval;
		NewNode.nSize=aNode.nSize+bNode.nSize;
		NewNode.pfData=new float[NewNode.nSize];
		int nPosition=0;

		for(i=0;i<aNode.nSize;i++)
		{
			NewNode.pfData[nPosition]=aNode.pfData[i];
			nPosition++;
		}

		for(i=0;i<bNode.nSize;i++)
		{
			NewNode.pfData[nPosition]=bNode.pfData[i];
			nPosition++;
		}
	}

	int nNumChannels=aNode.nNumChannels;

	NewNode.nNumChannels=nNumChannels;

	for(i=0;i<nNumChannels;i++)
	{
		if(aNode.fAutosampleAdditGain<bNode.fAutosampleAdditGain)
		{
			NewNode.fAutosampleAdditGain=aNode.fAutosampleAdditGain;
		}
		else
		{
			NewNode.fAutosampleAdditGain=bNode.fAutosampleAdditGain;
		}
	}
}

int i_max(int n1, int n2)
{
	if(n1>=n2)
		return n1;
	else
		return n2;
}

int i_min(int n1, int n2)
{
	if(n1<=n2)
		return n1;
	else
		return n2;
}

int CAbf_ProcessingDoc::i_gcd ( int i, int j )

//******************************************************************************
//
//  Purpose:
//
//    I_GCD finds the greatest common divisor of I and J.
//
//  Discussion:
//
//    Note that only the absolute values of I and J are
//    considered, so that the result is always nonnegative.
//
//    If I or J is 0, I_GCD is returned as max ( 1, abs ( I ), abs ( J ) ).
//
//    If I and J have no common factor, I_GCD is returned as 1.
//
//    Otherwise, using the Euclidean algorithm, I_GCD is the
//    greatest common divisor of I and J.
//
//  Modified:
//
//    25 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, two numbers whose GCD is desired.
//
//    Output, int I_GCD, the greatest common divisor of I and J.
//
{
  int p;
  int q;
  int r;
//
//  Return immediately if either I or J is zero.
//
  if ( i == 0 )
  {
    q = i_max ( 1, abs ( j ) );
    return q;
  }
  else if ( j == 0 )
  {
    q = i_max ( 1, abs ( i ) );
    return q;
  }
//
//  Set IP to the larger of I and J, IQ to the smaller.
//  This way, we can alter IP and IQ as we go.
//
  p = i_max ( abs ( i ), abs ( j ) );
  q = i_min ( abs ( i ), abs ( j ) );
//
//  Carry out the Euclidean algorithm.
//
  for ( ; ; )
  {
    r = p % q;

    if ( r == 0 )
    {
      break;
    }
    p = q;
    q = r;
  }

  return q;
}

void CAbf_ProcessingDoc::OnAnalysisDatafileRltafrconvert() 
{
	// TODO: Add your command handler code here
	structpClampResultNode aResultNode;
	aResultNode=ReadpClampResultFile();
	WriteToAfrFile(aResultNode);
}

structpClampResultNode CAbf_ProcessingDoc::ReadpClampResultFile()
{
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="text form of pClamp9 result Files (*.atf)\0*.atf\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a pClamp9 result file";
	char cTemp,cTemp0;
	cTemp0='Z';
	int nChar=0;
	int nLine=0;
	float fLine[26];
	float fTemp=0.f;
	int nLineLength=26;
	structpClampResultNode aNode;
	aNode.nLineLength=26;
	aNode.vfData.clear();
	aNode.nNumLines=0;
	int i,nChars=0,len=9,position=0,j;
	char name[9];
	char cUnit;

	if(aFileDlg.DoModal()==IDOK)
	{
		m_sClampfitReultFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(m_sClampfitReultFileName,"r");
		bool bt, unitSpecified;
		char sT[]="Time 1 (s";
		unitSpecified=false;
		while(!feof(fpAbfIn))
		{
			nChar++;
			fscanf(fpAbfIn,"%c",&cTemp);

				if(position<len-1){
					name[position]=cTemp;
					position++;
				}else{
					for(j=0;j<len-1;j++){
						name[j]=name[j+1];
					}
					name[position]=cTemp;
				}

				bt=true;
				for(j=0;j<len-1;j++){
					if(name[j]!=sT[j]) bt=false;
				}

				if(bt&&!unitSpecified) {
					cUnit=name[position];
					unitSpecified=true;
				}

			if(cTemp=='A'&&cTemp0=='p')
			{
				for(i=0;i<nLineLength;i++)
				{
					fscanf(fpAbfIn,"%f",&fTemp);
					aNode.vfData.push_back(fTemp);
				}
				nLine++;
			}
			cTemp0=cTemp;
		}
		aNode.nNumLines=nLine;
		aNode.unit=cUnit;
		fclose(fpAbfIn);
	}
	return aNode;
}

void CAbf_ProcessingDoc::WriteToAfrFile(structpClampResultNode aNode)
{
	FILE *fpAfr;
	CString sAbfFileName;
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Artifact Segment Files (*.afr)\0*.afr\0";
	CString sFileName=ChangeFileExt(m_sClampfitReultFileName,"afr");
//	aFileDlg.m_ofn.lpstrFile=sFileName.GetBuffer(sFileName.GetLength());
//	sFileName.ReleaseBuffer();
	strcpy(aFileDlg.m_ofn.lpstrFile,sFileName);
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Append to an artifact segment file";
	int nLine=0;
	int nLineLength=26;
	float fTemp=0.f;

	if(aFileDlg.DoModal()==IDOK)
	{
		int nBasedOnAdjustedData=0;
		int nCheckAmplitude=3;
		int ncheckDelta=0;
		int nCheckOppositeDirrection=0;
		int nCheckEvl=0;
		float fAmpThred=-10000.f, fDeltaThred=0.f, fOppThred=1.f;
		float fTime1=0.f, fTime2=900000000.f, fTime3=0.f, fTime4=0.f;;
		float fY1=0.f,fY2=0.f;
		float fDelta=0.8;
		sAbfFileName=aFileDlg.GetPathName();
		fpAfr=fopen(sAbfFileName,"w");
		int nLines=aNode.nNumLines;
		int nLineLength=aNode.nLineLength;
		double factor=1;
		if(aNode.unit=='s') factor=1000.;

		fprintf(fpAfr,"\nnIterations:    %d\n", nLines+1);
		fprintf(fpAfr,"\n\n");

		for(int i=0;i<nLines;i++)
		{
			fY1=aNode.vfData[i*nLineLength];
			fY2=aNode.vfData[i*nLineLength+1];
			fTime1=aNode.vfData[i*nLineLength+4]*factor;
			fTime2=aNode.vfData[i*nLineLength+5]*factor;
			fTime3=aNode.vfData[i*nLineLength+6]*factor;
			fTime4=aNode.vfData[i*nLineLength+7]*factor;

			if(fTime1>fTime2)
			{
				fTemp=fTime2;
				fTime2=fTime1;
				fTime1=fTemp;
			}

			if(fTime3<0)
			{
				nCheckAmplitude=-2;
			}
			else
			{
				nCheckAmplitude=-22;
			}

			if(fTime4<0)
			{
				nCheckAmplitude=-32;
			}

			if(fTime3<0.&&fTime4<0.)
			{
				nCheckAmplitude=-42; //exclude the segment
			}

			fAmpThred=0.5*(fY1+fY2)-fDelta;
			fprintf(fpAfr,"bBasedOnAdjustedData: 		%1d\n",nBasedOnAdjustedData);
			fprintf(fpAfr,"bCheckAmplitude: 		%3d 	m_fAmplitudeThreshold: 		%4.1f\n",nCheckAmplitude, fAmpThred);
			fprintf(fpAfr,"bCheckDelta:     		%1d 	m_fDeltaThreshold: 		%4.1f\n",ncheckDelta, fDeltaThred);
			fprintf(fpAfr,"bCheckOppositeDirrection:	%1d 	fOppositeDirectionThreshold: 	%4.1f\n",nCheckOppositeDirrection, fOppThred);
			fprintf(fpAfr,"m_bCheckEvl:			%1d\n",nCheckEvl);
			fprintf(fpAfr,"fTime1(ms):			%12.1f 	fTime2(ms):      %12.1f",fTime1, fTime2);
			fprintf(fpAfr,"\n\n");
			nBasedOnAdjustedData=1;
		}

		nCheckOppositeDirrection=0;
		nCheckAmplitude=3;
		fTime1=0.f;
		fTime2=200000000.f;
		fAmpThred=-10000.f;

		fprintf(fpAfr,"bBasedOnAdjustedData: 		%1d\n",nBasedOnAdjustedData);
		fprintf(fpAfr,"bCheckAmplitude: 		%2d 	m_fAmplitudeThreshold: 		%4.1f\n",nCheckAmplitude, fAmpThred);
		fprintf(fpAfr,"bCheckDelta:     		%1d 	m_fDeltaThreshold: 		%4.1f\n",ncheckDelta, fDeltaThred);
		fprintf(fpAfr,"bCheckOppositeDirrection:	%1d 	fOppositeDirectionThreshold: 	%4.1f\n",nCheckOppositeDirrection, fOppThred);
		fprintf(fpAfr,"m_bCheckEvl:			%1d\n",nCheckEvl);
		fprintf(fpAfr,"fTime1(ms):			%12.1f 	fTime2(ms):      %12.1f",fTime1, fTime2);

		aNode.nNumLines=nLine;
		fclose(fpAfr);
	}
}

void CAbf_ProcessingDoc::OnFileExportLocalextremaofdatafile() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="text file (*.txt)\0*.txt\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export the local extrema of the data file";

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cAbf.WriteExtrema(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}		
}

void CAbf_ProcessingDoc::OnAnalysisIdealization() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sTitle;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
			char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
			aFileDlg.m_ofn.lpstrFilter=pch;
			aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}

//	if(!m_bAbf)
//	{
		sTitle="Import a data file to idealize";
		OnFileImportDatafile(sTitle);
		m_bAbf=true;
//	}

		
//	if(!m_bEvl)
//	{
		sTitle="Import a event list file with an appropriate header section";
		OnFileImportEventfile(sTitle);
		m_bEvl=true;
//	}
	m_cEvl.UpdateEvents(m_cAbf.GetEventList());

	CString sTemp="p";
	m_sEvlFileName=ExtendFileName(m_sAbfFileName,sTemp);
	m_sEvlFileName=ChangeFileExt(m_sEvlFileName,"evl");

	OnFileExportEvlfile();
	OnAnalysisEventlistEvlabfcomparison();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistEvlcorrection() 
{
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sTitle;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a triple channel data file";
	if(!m_bTripleChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
			char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
			aFileDlg.m_ofn.lpstrFilter=pch;
			aFileDlg.m_ofn.lpstrTitle="Import a tripple channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sTripleChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cTripleChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bTripleChannelAbf=true;
	}

	if(!m_bAbf)
	{
		sTitle="Import a data that has been idealized";
		OnFileImportDatafile(sTitle);
		m_bAbf=true;
	}

		
	if(!m_bEvl)
	{
		sTitle="Import a event list file to be corrected";
		OnFileImportEventfile(sTitle);
		m_bEvl=true;
	}
	m_cComparisonEvl=m_cEvl;
	m_sComparisonEvlFileName=m_sEvlFileName;
	m_bComparisonEvl=true;
	m_cEvl.EvlCorrection();
	OnFileExportEvlfile();
	OnAnalysisEventlistEvlevlabfcomparison();
}

void CAbf_ProcessingDoc::OnFileImportEventfileforcomparison() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CString sComparisonEvlFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Event List Files (*.evl)\0*.Evl\0All Files\0*.*\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import an event list file";

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	if(aFileDlg.DoModal()==IDOK)
	{
		sComparisonEvlFileName=aFileDlg.GetPathName();
		m_cComparisonEvl.ReadEvl(sComparisonEvlFileName);
		m_sComparisonEvlFileName=sComparisonEvlFileName;
	}
}

void CAbf_ProcessingDoc::OnFileExportDualdatafile() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here

	if(!m_bDualChannelAbf)
	{
		ImportDualdatafile();
	}

	FILE *fpAbfOut;
	CString sAbfFileName;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CFileDialog aFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Export the Dual Data File";

	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cDualChannelAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
	}		
}


void CAbf_ProcessingDoc::ImportDualdatafile()
{
	FILE *fpAbfIn;
	CString sTitle;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
			char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
			aFileDlg.m_ofn.lpstrFilter=pch;
			aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistBurstsMeanburstdurationcorrection() 
{
	// TODO: Add your command handler code here
	//Need to consider the case when evl files are not available
	FILE *fp;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sInputFileName=PassingString(sMessage);

	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Mean Burst Duration Correction Input File (*.Mbc or Mbd)\0*.Mbc;*.Mbd\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Read an input file for dwell fitting curve construction";
	float fRecordLength=0.f;
	float fBDelimiter;
	int nBurstIndex;

	if(aFileDlg.DoModal()==IDOK)
	{
		sInputFileName=aFileDlg.GetPathName();
		CString sBSTFileName=ExtendFileName(sInputFileName,"_BST");
		sBSTFileName=m_cEvl.ChangeExt(sBSTFileName,"Bst");
		FILE* fpTotal=fopen(sBSTFileName,"w");			
		fprintf(fpTotal,"BurstIndex     BDLMTER     Events0       NOPB0      MeanBST0         MToT0      Events        NOPB       MeanBST          MToT");
		fprintf(fpTotal,"        EveFile\n");

		if(fp=fopen(sInputFileName,"r"))
		{
			int i, nComponents;
			bool bEvl=false;
			CString sEvlFileName=PassingString(sInputFileName);
			char sTemp[400];
			float fFittedArea,fL,fR,fW,fT;
			CString sExt=FileExt(sInputFileName);
			while(!feof(fp))
			{
				fscanf(fp,"%s",sEvlFileName);
				CFileDialog bFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
				char* pch="Event List Files (*.evl)\0*.evl\0\0";
				bFileDlg.m_ofn.lpstrFilter=pch;
				bFileDlg.m_ofn.lpstrTitle="Read the fitted evl file";
				strcpy(bFileDlg.m_ofn.lpstrFile,sEvlFileName);
				bEvl=false;
//				if(bFileDlg.DoModal()==IDOK)
//				{
//					sEvlFileName=bFileDlg.GetPathName();
//					m_cEvl.ReadEvl(sEvlFileName);
//					bEvl=true;
//				}
				structFittingResultNode aResultNode;

				if(sExt=="Mbd")
				{
	//				fscanf(fp,"%s %f %s %d",sTemp, &aResultNode.fRecordLength, sTemp, &aResultNode.nNumEvents);
					//This information will be obtained from the histogram generated using the evl file sEvlFileName
				}
				else
				{
					aResultNode.fRecordLength=-1.f;
					aResultNode.nNumEvents=-1;
				}

				aResultNode.sFileName=PassingString(sEvlFileName);
				fscanf(fp,"%s %d %s %d",sTemp,&aResultNode.nDwellLevel,sTemp,&nComponents);
				fscanf(fp,"%s %f %s %f",sTemp,&fL,sTemp,&fR);
				aResultNode.fFittingRange.fLeft=fL;
				aResultNode.fFittingRange.fRight=fR;
				fscanf(fp,"%s %f",sTemp,&fFittedArea);
				aResultNode.dFittedArea=fFittedArea;
				fscanf(fp,"%s %d",sTemp,&aResultNode.nEventsInRange);
				aResultNode.nNumComponents=nComponents;
				float fTemp;
				if(sExt=="Mbd")
				{
					fscanf(fp,"%s %s %s %s %s %s",sTemp,sTemp,sTemp,sTemp,sTemp,sTemp);
					for(i=0;i<nComponents;i++)
					{
						fscanf(fp,"%f%f%f%f%f%f",&fTemp,&fW,&fTemp,&fTemp,&fT,&fTemp);
						aResultNode.dWeights[i]=fW;
						aResultNode.dTimeConstants[i]=fT;
					}
				}
				else
				{
					fscanf(fp,"%s %s",sTemp,sTemp);
					for(i=0;i<nComponents;i++)
					{
						fscanf(fp,"%f%f",&fW,&fT);
						aResultNode.dWeights[i]=fW;
						aResultNode.dTimeConstants[i]=fT;
					}
				}
				int nIterations,it,nKey;
				fscanf(fp,"%s%d",sTemp,&nIterations);
				for(it=0;it<nIterations;it++)
				{
					fscanf(fp,"%f%d",&fBDelimiter,&nBurstIndex);
					m_cEvl.ReadEvl(sEvlFileName);
					m_cEvl.MeanBSTCorrection(fpTotal,aResultNode,fBDelimiter,nBurstIndex);
					fprintf(fpTotal,"\n");
				}
				fscanf(fp,"%s%d",sTemp,&nBurstIndex);
				fBDelimiter=0.09;
//				for(it=1;it<100;it++)
//				{
//					m_cEvl.ReadEvl(sEvlFileName);
//					if(it <= 100)
//					{
//						fBDelimiter+=1.0;
//					}else if(it <= 200)
//					{
//						fBDelimiter+=2.;
//					}else
//					{
//						fBDelimiter+=10.;
//					}
//					m_cEvl.MeanBSTCorrection(fpTotal,aResultNode,fBDelimiter,nBurstIndex);
//					fprintf(fpTotal,"\n");
//				}
				double fTs,fTl,dAs,dAl;
				fTs=aResultNode.dTimeConstants[nBurstIndex-1];
				fTl=aResultNode.dTimeConstants[nBurstIndex];
				dAs=aResultNode.dWeights[nBurstIndex-1];
				dAl=aResultNode.dWeights[nBurstIndex];
			
				for(it=0;it<3;it++)
				{
					nKey=it+1;
					fBDelimiter=BurstDelimiter(fTs, dAs, fTl, dAl, nKey);
					m_cEvl.ReadEvl(sEvlFileName);
					m_cEvl.MeanBSTCorrection(fpTotal,aResultNode,fBDelimiter,nBurstIndex);
					fprintf(fpTotal,"\n");
				}
			}
		    fclose(fp);
		}
		else
		{
			AfxMessageBox(sMessage,MB_OK,0);
		}
		fclose(fpTotal);
	}		
}

void CAbf_ProcessingDoc::OnAnalysisDatafileSimulatedatafiles() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Clamp Data Files (*.dat or *abf)\0*.dat;*.abf\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a data file with appropriate header";
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}

	OnFileImportEventfile("Import an event list file with appropriate header");

	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";
	CString sEvlFileName=ChangeFileExt(sAbfFileName,"Evl");

	CDataSimulations aSimulation(&m_cAbf,&m_cEvl,sEvlFileName);	
}

void CAbf_ProcessingDoc::OnAnalysisDatafileSimulatedatafiles0() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );

//	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a data file with appropriate header";
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		fpAbfIn=fopen(sAbfFileName,"rb");
		m_cAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}

	OnFileImportEventfile("Import an event list file with appropriate header");

	FILE *fpAbfOut;
	CString sMessage="Can not open the file. Please check if this file is being used by other applications.";

	CDataSimulations aSimulation(&m_cAbf);
	
	pch="pClamp Data Files (*.dat)\0*.dat\0\0";

	CFileDialog bFileDlg( false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	bFileDlg.m_ofn.lpstrFilter=pch;
	bFileDlg.m_ofn.lpstrTitle="Save the simulated data into a data file";

	if(bFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=bFileDlg.GetPathName();
		if(!(fpAbfOut=fopen(sAbfFileName,"wb")))
		{
//			AfxFormatString1(sMessage,-1,sAbfFileName);
			AfxMessageBox(sMessage,MB_OK,0);
		}
		else
		{
	    	m_cAbf.WriteData(fpAbfOut);
	     	fclose(fpAbfOut);
		}
		sAbfFileName=ChangeFileExt(sAbfFileName,"Sum");
		aSimulation.OutputStateSummary(sAbfFileName);
	}
	
	m_cEvl.UpdateEvents(aSimulation.GetEventList());
	CString sEvlFileName=ChangeFileExt(sAbfFileName,"Evl");
	m_cEvl.WriteEvl(sEvlFileName);
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportExport2dreverseevlhistogram() 
{
	// TODO: Add your command handler code here
	if(!m_bEvl)
	{
		OnFileImportEventfile();
	}
	m_cEvl.ReverseOpenAndClosed();
	m_cEvl.Export2DEvlHistogram();
}

void CAbf_ProcessingDoc::OnAnalysisEventlistFittingqmatrix() 
{
	// TODO: Add your command handler code here
	CQMatrixFitting aQ;
	aQ.PrepareInput();	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistCommentevl() 
{
	// TODO: Add your command handler code here
	CEvlComment aDlg;
	if(aDlg.DoModal()==IDOK)
	{
		OnFileImportEventfile("import the Evl file to comment");
		CString sComment=aDlg.GetComment();
		m_cEvl.SetComment(sComment);
		m_cEvl.WriteEvl(m_sEvlFileName);
	}	
}

void CAbf_ProcessingDoc::OnAnalysisDatafileAppendadatafile() 
{
	// TODO: Add your command handler code here
	CAbf aAbf;
	CString sOringinalFileName=m_sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	CString sAbfFileName;
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import the data file to attach";
	if(aFileDlg.DoModal()==IDOK)
	{
		sAbfFileName=aFileDlg.GetPathName();
		FILE* fpAbfIn=fopen(sAbfFileName,"rb");
		aAbf.ReadData(fpAbfIn);
		fclose(fpAbfIn);
		m_sAbfFileName=sAbfFileName;
		if(!m_cStoredHistory.Empty())
		{
			m_cStoredHistory.ClearHistory();
		}
		m_cStoredHistory.StoreFileName(m_sAbfFileName);
	}

	AbfNode aNode=m_cAbf.GetTrace();

	AbfNode bNode=aAbf.GetTrace();

	int nSize1=aNode.nSize;
	int nSize2=bNode.nSize;
	float *pfData=new float[nSize1+nSize2];
	int i;
	int nPosition=0;
	int nSize=nSize1+nSize2;

	for(i=0;i<nSize1;i++)
	{
			pfData[nPosition]=aNode.pfData[i];
			nPosition++;
	}
	for(i=0;i<nSize2;i++)
	{
			pfData[nPosition]=bNode.pfData[i];
			nPosition++;
	}

	m_cAbf.UpdateSize(nSize);
	m_cAbf.UpdateData(pfData);
//	m_cDualChannelAbf.EqualizeScale();
	delete pfData;

//	m_sAbfFileName=sOriginalFileName;
}

void CAbf_ProcessingDoc::ImportEvlAbfPair()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	FILE *fpAbfIn;
	CString sAbfFileName;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";
	if(!m_bDualChannelAbf)
	{
		if(aFileDlg.DoModal()==IDOK)
		{
		char* pch="Abf Files\0*.abf\0pClamp Data Files (*.dat)\0*.dat\0\0";
		aFileDlg.m_ofn.lpstrFilter=pch;
		aFileDlg.m_ofn.lpstrTitle="Import a dual channel data file";

			sAbfFileName=aFileDlg.GetPathName();
			m_sDualChannelAbfFileName=sAbfFileName;
			CString sMessage="The DualChannel File Can not be opened";
			if(!(fpAbfIn=fopen(sAbfFileName,"rb")))
			{
//				AfxFormatString1(sMessage,-1,sAbfFileName);
				AfxMessageBox(sMessage,MB_OK,0);
			}
			else
			{
				m_cDualChannelAbf.ReadData(fpAbfIn);
	     		fclose(fpAbfIn);
			}
		}
		m_bDualChannelAbf=true;
	}				


		CString sTitle1;
		
//		if(!m_bAbf)
//		{
			sTitle1="Import a data file for cocmaring with dwelling time";
			OnFileImportDatafile(sTitle1);
//		}

		
//		if(!m_bEvl)
//		{
			sTitle1="Import a event list file for cocmaring with dwelling time";
			OnFileImportEventfile(sTitle1);
//		}

		m_cDualChannelAbf.EqualizeScale();

}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportlevelallpointhistogram() 
{
	// TODO: Add your command handler code here
	
}

void CAbf_ProcessingDoc::OnANALYSISEVENTLISTAPHEventIndex() 
{
	CLinearHistogramDouble APH1, APH2, APHn1, APHn2;
	CLinearHistogramDouble APH1t, APH2t, APHn1t, APHn2t;

	FILE *fpIn,fpInt;
	CString sListFileName,sTemp;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="txt file containing dat and evl files (*.txt)\0*.txt\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="input the list of file names for APH_Event_Index";
	strcpy(aFileDlg.m_ofn.lpstrFile,m_sAbfFileName);

	if(aFileDlg.DoModal()==IDOK)
	{
		CBurstingTimeCourseOptionDlg aDlg;
		if(aDlg.DoModal()==IDOK)
		{
			CFileNameHandling cfh;
			structBurstingTimeCourseOptionNode aNode;
			aDlg.getBurstTimecourseOptions(aNode);
			CString sEvlFile, sDatFile;
			int num;
			sListFileName=aFileDlg.GetPathName();
			fpIn=fopen(sListFileName,"r");
			fscanf(fpIn,"%s%d",sTemp,&num);
			AbfNode aAbfNode;


			CString outputFileName=ChangeFileExt(sListFileName,"LAP");
			outputFileName=cfh.ExtendFileName(outputFileName,aDlg.getBurstEvolutionParametersAsString());
			FILE* fpOutput=fopen(outputFileName,"w");
			CLinearHistogramHandler aHDL;
			int i11,i21,i12,i22;
			double x11,x12,x21,x22;

			CLinearHistogramDouble Hist1, Hist2;
			Hist1.update(-10.,5.,0.02);
			Hist2.update(-10.,5.,0.02);
			int i;

			for(i=0;i<num;i++){

				fscanf(fpIn,"%s",sEvlFile);
				m_cEvl.ReadEvl(sEvlFile);
				m_cEvl.filteringEvents(aNode);
				CString st=ChangeFileExt(sEvlFile,"LAP");
				FILE* fpt=fopen(st,"w");
				fscanf(fpIn,"%s",sDatFile);
				m_cAbf.ReadData(sDatFile);
				aAbfNode=m_cAbf.GetTrace();
				m_cEvl.APH_EventIndex(aNode,aAbfNode, APH1t, APH2t, APHn1t, APHn2t);
				Hist1.appendHist(APH1t);
				Hist2.appendHist(APH2t);

				double xi=-10.,xf=5.,delta=0.1,x1,x2;
				fprintf(fpt,"           Amp          APH1          APH2         APHn1         APHn2");
				fprintf(fpt,"\n");
				double dBase,dDelta,s1,s2,sn1,sn2;
				int nDim1=0,nDim2=0,nDim=0;

				int index;
				s1=APH1t.getTotalCounts();
				s2=APH2t.getTotalCounts();
				sn1=APHn1t.getTotalCounts();
				sn2=APHn2t.getTotalCounts();

				s1=1.;
				s2=1.;
				sn1=1.;
				sn2=1.;
				int ws=1;

	//			APH1.smoothHistogram(ws);
	//			APH2.smoothHistogram(ws);
	//			APHn1.smoothHistogram(ws);
	//			APHn2.smoothHistogram(ws);

				nDim=int((xf-xi)/delta)+1;
				for(int i0=0;i0<nDim;i0++){
					x1=xi+i0*delta;
					x2=x1+delta;
					fprintf(fpt,"  %12.2f  %12.4f  %12.4f",x1,APH1t.getCounts(x1,x2)/s1,APH2t.getCounts(x1,x2)/s2);
					fprintf(fpt,"  %12.4f  %12.4f",APHn1t.getCounts(x1,x2)/sn1,APHn2t.getCounts(x1,x2)/sn2);
					fprintf(fpt,"\n");
				}
				fclose(fpt);


				if(i==0){
					APH1.copyHist(APH1t);
					APH2.copyHist(APH2t);
					APHn1.copyHist(APHn1t);
					APHn2.copyHist(APHn2t);

				}else{
					APH1.mergeHist(APH1t);
					APH2.mergeHist(APH2t);
					APHn1.mergeHist(APHn1t);
					APHn2.mergeHist(APHn2t);
				}
			}

			double xi=-10.,xf=5.,delta=0.1,x1,x2;
			fprintf(fpOutput,"           Amp          APH1          APH2         APHn1         APHn2");
			fprintf(fpOutput,"\n");
			double dBase,dDelta,s1,s2,sn1,sn2;
			int nDim1=0,nDim2=0,nDim=0;

			int index;
			s1=APH1.getTotalCounts();
			s2=APH2.getTotalCounts();
			sn1=APHn1.getTotalCounts();
			sn2=APHn2.getTotalCounts();
/*			s1=1.;
			s2=1.;
			sn1=1.;
			sn2=1.;*/
			int ws=1;
			APH1.smoothHistogram(ws);
			APH2.smoothHistogram(ws);
			APHn1.smoothHistogram(ws);
			APHn2.smoothHistogram(ws);
			nDim=int((xf-xi)/delta)+1;
			for(i=0;i<nDim;i++){
				x1=xi+i*delta;
				x2=x1+delta;
				fprintf(fpOutput,"  %12.2f  %12.4f  %12.4f",x1,APH1.getCounts(x1,x2)/s1,APH2.getCounts(x1,x2)/s2);
//				fprintf(fpOutput,"  %12.4f  %12.4f",APHn1.getCounts(x1,x2)/sn1,APHn2.getCounts(x1,x2)/sn2);
				fprintf(fpOutput,"  %12.4f  %12.4f",Hist1.getCounts(x1,x2)/sn1,Hist2.getCounts(x1,x2)/sn2);
				fprintf(fpOutput,"\n");
			}
			fclose(fpOutput);

		}
	}

}

void CAbf_ProcessingDoc::OnAnalysisEventlistAtftoevl() 
{
	CString sTitle="input an evl file for the heading";
	if(!m_bEvl)OnFileImportEventfile(sTitle);
	FILE *fp;
	CFileDialog aFileDlg( true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL );
	char* pch="the result file of clampfit (*.atf)\0*.atf\0\0";
	aFileDlg.m_ofn.lpstrFilter=pch;
	aFileDlg.m_ofn.lpstrTitle="reading the atf format result file";
	if(aFileDlg.DoModal()==IDOK)
	{
		m_sEvlFileName=aFileDlg.GetPathName();
		m_cEvl.readAtf(m_sEvlFileName);
		OnFileExportEvlfile();
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportBurstevolusions() 
{
	// TODO: Add your command handler code here
	OnFileImportEventfile("import an evl file for burst evolution analysis");
	m_cEvl.exportBurstEvolutions();
}

void CAbf_ProcessingDoc::OnAnalysisUtilitiesBurstdelimiters() 
{
	// TODO: Add your command handler code here
	CBurstDelimiterCalculationDialog aDlg;
	while(aDlg.DoModal()==IDOK){
		aDlg.calculateBurstDelimeters();
	}
}

void CAbf_ProcessingDoc::OnAnalysisEventlistExportBurstcorrelations() 
{
	// TODO: Add your command handler code here
	OnFileImportEventfile("import an evl file for burst evolution analysis");
	m_cEvl.ExportBurstcorrelations();	
}

void CAbf_ProcessingDoc::OnAnalysisEventlistBuildsubsets() 
{
	// TODO: Add your command handler code here
	OnFileImportEventfile("import an evl file to build a subset");
	m_cEvl.buildSubsets();
}

void CAbf_ProcessingDoc::OnAnalysisRunscripts() 
{
	// TODO: Add your command handler code here
	m_cScriptManager.runScripts();
}
