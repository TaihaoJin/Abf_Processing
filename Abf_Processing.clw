; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAbf_ProcessingDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Abf_Processing.h"
LastPage=0

ClassCount=25
Class1=CAbf_ProcessingApp
Class2=CAbf_ProcessingDoc
Class3=CAbf_ProcessingView
Class4=CMainFrame

ResourceCount=23
Resource1=IDD_BurstingTimeCourse
Resource2=IDD_DIALOG10
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDD_DIALOG6
Class7=CBurstOptionDialog
Resource4=IDD_DIALOG2
Class8=CBlineAdjustOptionDialog
Resource5=IDD_SegmentDeletionOptions
Class9=CAllPointHistgramDialog
Resource6=IDD_ABOUTBOX
Resource7=IDD_Evl_Comments
Class10=CEvlTimeCourseDialog
Resource8=IDD_SubconductanceDlg
Class11=CBallRollingOption
Resource9=IDD_DIALOG4
Class12=CArtifactHanddlingOptionDlg
Resource10=IDR_MAINFRAME
Class13=CArtifactSegmentDialog
Resource11=IDD_DIALOG7
Class14=CSmoothingOptionDialog
Resource12=IDD_DIALOG8
Class15=CIgnoreShortClosingDialog
Resource13=IDD_GettingFittedParameters
Class16=CSegDeletOptionDialog
Resource14=IDD_DIALOG11
Class17=CSegmentExtractionDialog1
Class18=CSegmentExtractionDialog2
Resource15=IDR_ABF_PRTYPE
Class19=CIgnoreSmallTransitionOptionDialog
Resource16=IDD_DIALOG5
Class20=CEvlHistogramDlg
Resource17=IDD_DIALOG1
Class21=CGettingFittedParametersDlg
Resource18=IDD_DIALOG9
Class22=CBurstingTimeCourseOptionDlg
Resource19=IDD_EvlFittingHistogramMethod
Class23=CSubconductanceDlg
Resource20=IDD_IgnoreSmallTransitions
Class24=CEvlComment
Resource21=IDD_IgnoreShortClosings
Resource22=IDD_DIALOG3
Class25=CBurstDelimiterCalculationDialog
Resource23=IDD_BurstDelimiterCalculations

[CLS:CAbf_ProcessingApp]
Type=0
HeaderFile=Abf_Processing.h
ImplementationFile=Abf_Processing.cpp
Filter=N
LastObject=CAbf_ProcessingApp

[CLS:CAbf_ProcessingDoc]
Type=0
HeaderFile=Abf_ProcessingDoc.h
ImplementationFile=Abf_ProcessingDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_ANALYSIS_RUNSCRIPTS

[CLS:CAbf_ProcessingView]
Type=0
HeaderFile=Abf_ProcessingView.h
ImplementationFile=Abf_ProcessingView.cpp
Filter=C
LastObject=CAbf_ProcessingView
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=Abf_Processing.cpp
ImplementationFile=Abf_Processing.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_ABF_PRTYPE]
Type=1
Class=CAbf_ProcessingView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_IMPORT_DATAFILE
Command4=ID_FILE_IMPORT_EVENTFILE
Command5=ID_FILE_IMPORT_EVENTFILEFORCOMPARISON
Command6=ID_FILE_IMPORT_ASCIIASDATA
Command7=ID_FILE_IMPORT_ASCIIFORMATOFTACKFITFILE
Command8=ID_FILE_IMPORT_EVLSEPARATEBODY
Command9=ID_FILE_ATTACH_DATAFILE
Command10=ID_FILE_ATTACH_EVLFILE
Command11=ID_FILE_CLOSE
Command12=ID_FILE_SAVE
Command13=ID_FILE_SAVE_AS
Command14=ID_FILE_EXPORT_DATAFILE
Command15=ID_FILE_EXPORT_DATAHEADER
Command16=ID_FILE_EXPORT_DUALDATAFILE
Command17=ID_FILE_EXPORT_ADJUSTEDDATAFILE
Command18=ID_FILE_EXPORT_EVLFILE
Command19=ID_FILE_EXPORT_BURSTEVL
Command20=ID_FILE_EXPORT_ASCIIFORMOFEVL
Command21=ID_FILE_EXPORT_EVLSUMMARY
Command22=ID_FILE_EXPORT_CONCATENATEDSEGMENTS
Command23=ID_FILE_EXPORT_COMPARISON
Command24=ID_FILE_EXPORT_LOCALEXTREMAOFDATAFILE
Command25=ID_FILE_PRINT
Command26=ID_FILE_PRINT_PREVIEW
Command27=ID_FILE_PRINT_SETUP
Command28=ID_FILE_MRU_FILE1
Command29=ID_APP_EXIT
Command30=ID_EDIT_UNDO
Command31=ID_EDIT_CUT
Command32=ID_EDIT_COPY
Command33=ID_EDIT_PASTE
Command34=ID_VIEW_TOOLBAR
Command35=ID_VIEW_STATUS_BAR
Command36=ID_WINDOW_NEW
Command37=ID_WINDOW_CASCADE
Command38=ID_WINDOW_TILE_HORZ
Command39=ID_WINDOW_ARRANGE
Command40=ID_WINDOW_SHOWFILENAMES
Command41=ID_ANALYSIS_BASELINEADJUSTMENT
Command42=ID_ANALYSIS_ALLPOINTHISTOGRAM
Command43=ID_ANALYSIS_DATAFILE_BALLROLLING
Command44=ID_ANALYSIS_DATAFILE_REMOVEARTIFACTS
Command45=ID_ANALYSIS_DATAFILE_REPLACESEGMENTS
Command46=ID_ANALYSIS_DATAFILE_UPDATEADJUSTEDDATA
Command47=ID_ANALYSIS_DATAFILE_BACKUPDATA
Command48=ID_ANALYSIS_DATAFILE_ABFABFCOMPARISON
Command49=ID_ANALYSIS_DATAFILE_ABFTODATCONVERSION
Command50=ID_ANALYSIS_DATAFILE_RLTAFRCONVERT
Command51=ID_ANALYSIS_DATAFILE_SIMULATEDATAFILES
Command52=ID_ANALYSIS_DATAFILE_APENDADATAFILE
Command53=ID_ANALYSIS_EVENTLIST_TIMECOURSE
Command54=ID_ANALYSIS_EVENTLIST_MULTIPLEEVLSUMMARIES
Command55=ID_ANALYSIS_EVENTLIST_AMPHISTOGRAM
Command56=ID_ANALYSIS_EVENTLIST_FITTINGSUMMARIES
Command57=ID_ANALYSIS_EVENTLIST_FITTINGDWELLTIMEBATCH
Command58=ID_ANALYSIS_EVENTLIST_FITTINGDWELLTIMEAUTOMATIC
Command59=ID_ANALYSIS_EVENTLIST_SIMULATEEVENTLIST
Command60=ID_ANALYSIS_EVENTLIST_GETFITTEDPARAMETERS
Command61=ID_ANALYSIS_EVENTLIST_DWELLCURVECONSTRUCTION
Command62=ID_ANALYSIS_EVENTLIST_EXPORTSEGMENTSBATCH
Command63=ID_ANALYSIS_EVENTLIST_EXPORTHISTOGRAM
Command64=ID_ANALYSIS_EVENTLIST_EXPORT_EXPORT2DEVLHISTOGRAM
Command65=ID_ANALYSIS_EVENTLIST_EXPORT_EXPORTSTATITICsOFBURSTANALYSIS
Command66=ID_ANALYSIS_EVENTLIST_ExportEvlAsTrace
Command67=ID_ANALYSIS_EVENTLIST_EXPORT_EXPORT2DREVERSEEVLHISTOGRAM
Command68=ID_ANALYSIS_EVENTLIST_EXPORT_BURSTEVOLUSIONS
Command69=ID_ANALYSIS_EVENTLIST_EXPORT_BURSTCORRELATIONS
Command70=ID_ANALYSIS_EVENTLIST_COMBINEEVLFILES
Command71=ID_ANALYSIS_EVENTLIST_EVLABFCOMPARISON
Command72=ID_ANALYSIS_EVENTLIST_EVLEVLABFCOMPARISON
Command73=ID_ANALYSIS_EVENTLIST_COMPARISON0_ABFDWELLINGTIMECOMPARISON
Command74=ID_ANALYSIS_EVENTLIST_COMPARISON_EVLAMPABFCOMPARISON
Command75=ID_ANALYSIS_TRIMMINGMULTIPLEOPENNING
Command76=ID_ANALYSIS_EVENTLIST_IGNORESMALLTRANSITIONS
Command77=ID_ANALYSIS_EVENTLIST_REMOVEDEGENERACY
Command78=ID_ANALYSIS_EVENTLIST_IGNORESHORTOPENINGS
Command79=ID_ANALYSIS_EVENTLIST_IGNORESHORTCLOSINGS
Command80=ID_ANALYSIS_MODESEPERATION
Command81=ID_ANALYSIS_EVENTLIST_EXCLUDESUBCONDUCTANCES
Command82=ID_ANALYSIS_EVENTLIST_BURSTDELIMITERTEST
Command83=ID_ANALYSIS_EVENTLIST_BURSTINGTIMECOURSE
Command84=ID_ANALYSIS_EVENTLIST_MULTIPLEEVLBUSTDELIMITERTEST
Command85=ID_ANALYSIS_EVENTLIST_BURSTS_GAMMAFITTING
Command86=ID_ANALYSIS_EVENTLIST_BURSTS_NUMOFOPENINGPERBURST
Command87=ID_ANALYSIS_EVENTLIST_BURSTS_NUMOFOPENINGPERBURST_Batch
Command88=ID_ANALYSIS_EVENTLIST_TOTPB_BATCH
Command89=ID_ANALYSIS_EVENTLIST_TOTPB
Command90=ID_ANALYSIS_EVENTLIST_BurstDuration
Command91=ID_ANALYSIS_EVENTLIST_BurstDurationBatch
Command92=ID_ANALYSIS_EVENTLIST_BURSTS_MEANBURSTDURATIONCORRECTION
Command93=ID_ANALYSIS_EVENTLIST_APH_EventIndex
Command94=ID_ANALYSIS_EVENTLIST_EVLCORRECTION
Command95=ID_ANALYSIS_EVENTLIST_FITTINGQMATRIX
Command96=ID_ANALYSIS_EVENTLIST_COMMENTEVL
Command97=ID_ANALYSIS_EVENTLIST_ATFTOEVL
Command98=ID_ANALYSIS_EVENTLIST_BUILDSUBSETS
Command99=ID_ANALYSIS_IDEALIZATION
Command100=ID_ANALYSIS_TEST_TESTNR
Command101=ID_ANALYSIS_UTILITIES_BURSTDELIMITERS
Command102=ID_ANALYSIS_RUNSCRIPTS
Command103=ID_APP_ABOUT
CommandCount=103

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG1]
Type=1
Class=CBurstOptionDialog
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BurstDelimiter,edit,1350631552
Control4=IDC_MinimumOpenning,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CBurstOptionDialog]
Type=0
HeaderFile=BurstOptionDialog.h
ImplementationFile=BurstOptionDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CBurstOptionDialog
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CBlineAdjustOptionDialog
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552

[CLS:CBlineAdjustOptionDialog]
Type=0
HeaderFile=BlineAdjustOptionDialog.h
ImplementationFile=BlineAdjustOptionDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CAllPointHistgramDialog
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT3,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT4,edit,1350631552
Control13=IDC_NegativeCurrent,button,1342177289
Control14=IDC_PositiveCurrent,button,1342177289

[CLS:CAllPointHistgramDialog]
Type=0
HeaderFile=AllPointHistgramDialog.h
ImplementationFile=AllPointHistgramDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PositiveCurrent
VirtualFilter=dWC

[DLG:IDD_DIALOG4]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG5]
Type=1
Class=CEvlTimeCourseDialog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_HardBoundary,button,1342242819

[CLS:CEvlTimeCourseDialog]
Type=0
HeaderFile=EvlTimeCourseDialog.h
ImplementationFile=EvlTimeCourseDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CEvlTimeCourseDialog
VirtualFilter=dWC

[DLG:IDD_DIALOG6]
Type=1
Class=CBallRollingOption
ControlCount=30
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT4,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT5,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CHECK1,button,1342242819
Control14=IDC_CHECK2,button,1342242819
Control15=IDC_EDIT6,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RADIO1,button,1342177289
Control19=IDC_RADIO2,button,1342177289
Control20=IDC_EDIT7,edit,1350631552
Control21=IDC_EDIT8,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_RADIO3,button,1342177289
Control25=IDC_RADIO4,button,1342177289
Control26=IDC_STATIC,button,1342177287
Control27=IDC_STATIC,static,1342308352
Control28=IDC_EDIT9,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_EDIT10,edit,1350631552

[CLS:CBallRollingOption]
Type=0
HeaderFile=BallRollingOption.h
ImplementationFile=BallRollingOption.cpp
BaseClass=CDialog
Filter=D
LastObject=CBallRollingOption
VirtualFilter=dWC

[DLG:IDD_DIALOG7]
Type=1
Class=CArtifactHanddlingOptionDlg
ControlCount=26
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_EDIT3,edit,1350631552
Control6=IDC_EDIT4,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_CHECK1,button,1342242819
Control12=IDC_CHECK2,button,1342242819
Control13=IDC_CHECK3,button,1342242819
Control14=IDC_CHECK4,button,1342242819
Control15=IDC_ApplyingOnlyBetweenT1AndT,button,1342242819
Control16=IDC_EDIT5,edit,1350631552
Control17=IDC_EDIT6,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_CheckEvl,button,1342242819
Control21=IDC_ReadFromAFile,button,1342242819
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT7,edit,1350631552
Control24=IDC_EDIT8,edit,1350631552
Control25=IDC_EDIT9,edit,1350631552
Control26=IDC_CHECK5,button,1342242819

[CLS:CArtifactHanddlingOptionDlg]
Type=0
HeaderFile=ArtifactHanddlingOptionDlg.h
ImplementationFile=ArtifactHanddlingOptionDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CArtifactHanddlingOptionDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG8]
Type=1
Class=CArtifactSegmentDialog
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_EDIT3,edit,1350631552
Control6=IDC_EDIT4,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_CHECK1,button,1342242819

[CLS:CArtifactSegmentDialog]
Type=0
HeaderFile=ArtifactSegmentDialog.h
ImplementationFile=ArtifactSegmentDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CArtifactSegmentDialog

[DLG:IDD_DIALOG9]
Type=1
Class=CSmoothingOptionDialog
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC,static,1342308352

[CLS:CSmoothingOptionDialog]
Type=0
HeaderFile=SmoothingOptionDialog.h
ImplementationFile=SmoothingOptionDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSmoothingOptionDialog

[DLG:IDD_IgnoreShortClosings]
Type=1
Class=CIgnoreShortClosingDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CIgnoreShortClosingDialog]
Type=0
HeaderFile=IgnoreShortClosingDialog.h
ImplementationFile=IgnoreShortClosingDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CIgnoreShortClosingDialog
VirtualFilter=dWC

[CLS:CSegDeletOptionDialog]
Type=0
HeaderFile=SegDeletOptionDialog.h
ImplementationFile=SegDeletOptionDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSegDeletOptionDialog

[DLG:IDD_DIALOG10]
Type=1
Class=CSegmentExtractionDialog2
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT3,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT4,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT5,edit,1350631552
Control13=IDC_CHECK1,button,1342242819
Control14=IDC_EDIT6,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_CHECK2,button,1342242819

[DLG:IDD_SegmentDeletionOptions]
Type=1
Class=CSegmentExtractionDialog1
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_BUTTON1,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT4,edit,1350631552

[CLS:CSegmentExtractionDialog1]
Type=0
HeaderFile=SegmentExtractionDialog1.h
ImplementationFile=SegmentExtractionDialog1.cpp
BaseClass=CDialog
Filter=D
LastObject=CSegmentExtractionDialog1

[CLS:CSegmentExtractionDialog2]
Type=0
HeaderFile=SegmentExtractionDialog2.h
ImplementationFile=SegmentExtractionDialog2.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSegmentExtractionDialog2

[DLG:IDD_IgnoreSmallTransitions]
Type=1
Class=CIgnoreSmallTransitionOptionDialog
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:CIgnoreSmallTransitionOptionDialog]
Type=0
HeaderFile=IgnoreSmallTransitionOptionDialog.h
ImplementationFile=IgnoreSmallTransitionOptionDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_ANALYSIS_EVENTLIST_IGNORESMALLTRANSITIONS

[DLG:IDD_EvlFittingHistogramMethod]
Type=1
Class=CEvlHistogramDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO1,button,1342177289
Control4=IDC_RADIO2,button,1342177289
Control5=IDC_RADIO3,button,1342177289
Control6=IDC_FilterEvl,button,1342242819
Control7=IDC_STATIC,static,1342308352
Control8=IDC_nFilteringKey,edit,1350631552
Control9=IDC_bCalConfidanceRange,button,1342242819
Control10=IDC_FullIteration,button,1342242819

[CLS:CEvlHistogramDlg]
Type=0
HeaderFile=EvlHistogramDlg.h
ImplementationFile=EvlHistogramDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEvlHistogramDlg

[CLS:CGettingFittedParametersDlg]
Type=0
HeaderFile=GettingFittedParametersDlg.h
ImplementationFile=GettingFittedParametersDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGettingFittedParametersDlg

[DLG:IDD_GettingFittedParameters]
Type=1
Class=CGettingFittedParametersDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_EDIT3,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT4,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT5,edit,1350631552
Control13=IDC_CHECK1,button,1342242819

[DLG:IDD_BurstingTimeCourse]
Type=1
Class=CBurstingTimeCourseOptionDlg
ControlCount=42
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_MinOpening,edit,1350631552
Control8=IDC_MaxOpening,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT4_ClusterDelimiter,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT3,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT4,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT5,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT6,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT7,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT8,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT9,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT10,edit,1350631552
Control28=IDC_EDIT11,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT12,edit,1350631552
Control32=IDC_EDIT13,edit,1350631552
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT14,edit,1350631552
Control36=IDC_EDIT15,edit,1350631552
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352
Control39=IDC_EDIT16,edit,1350631552
Control40=IDC_STATIC,static,1342308352
Control41=IDC_EDIT17,edit,1350631552
Control42=IDC_STATIC,static,1342308352

[CLS:CBurstingTimeCourseOptionDlg]
Type=0
HeaderFile=BurstingTimeCourseOptionDlg.h
ImplementationFile=BurstingTimeCourseOptionDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CBurstingTimeCourseOptionDlg
VirtualFilter=dWC

[DLG:IDD_SubconductanceDlg]
Type=1
Class=CSubconductanceDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_fAmp0,edit,1350631552
Control4=IDC_fAmp1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CSubconductanceDlg]
Type=0
HeaderFile=SubconductanceDlg.h
ImplementationFile=SubconductanceDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSubconductanceDlg

[DLG:IDD_Evl_Comments]
Type=1
Class=CEvlComment
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CEvlComment]
Type=0
HeaderFile=EvlComment.h
ImplementationFile=EvlComment.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEvlComment

[DLG:IDD_DIALOG11]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_EDIT3,edit,1350631552

[DLG:IDD_BurstDelimiterCalculations]
Type=1
Class=CBurstDelimiterCalculationDialog
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=A1,static,1342308352
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT5,edit,1350631552
Control12=IDC_EDIT6,edit,1350631552
Control13=IDC_EDIT7,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352

[CLS:CBurstDelimiterCalculationDialog]
Type=0
HeaderFile=BurstDelimiterCalculationDialog.h
ImplementationFile=BurstDelimiterCalculationDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=A1

