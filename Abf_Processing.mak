# Microsoft Developer Studio Generated NMAKE File, Based on Abf_Processing.dsp
!IF "$(CFG)" == ""
CFG=Abf_Processing - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Abf_Processing - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Abf_Processing - Win32 Release" && "$(CFG)" != "Abf_Processing - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Abf_Processing.mak" CFG="Abf_Processing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Abf_Processing - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Abf_Processing - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Abf_Processing.exe"


CLEAN :
	-@erase "$(INTDIR)\Abf.obj"
	-@erase "$(INTDIR)\Abf_Processing.obj"
	-@erase "$(INTDIR)\Abf_Processing.pch"
	-@erase "$(INTDIR)\Abf_Processing.res"
	-@erase "$(INTDIR)\Abf_ProcessingDoc.obj"
	-@erase "$(INTDIR)\Abf_ProcessingView.obj"
	-@erase "$(INTDIR)\AbfHeaderToWrite.obj"
	-@erase "$(INTDIR)\AllPointHistgram.obj"
	-@erase "$(INTDIR)\AllPointHistgramDialog.obj"
	-@erase "$(INTDIR)\amoeba.obj"
	-@erase "$(INTDIR)\amotry.obj"
	-@erase "$(INTDIR)\ArtifactHanddlingOptionDlg.obj"
	-@erase "$(INTDIR)\ArtifactSegmentDialog.obj"
	-@erase "$(INTDIR)\ArtifactsHandling.obj"
	-@erase "$(INTDIR)\ArtifactsRemoving.obj"
	-@erase "$(INTDIR)\BallRolling.obj"
	-@erase "$(INTDIR)\BallRollingOption.obj"
	-@erase "$(INTDIR)\BaselineProcessing.obj"
	-@erase "$(INTDIR)\BlineAdjustOptionDialog.obj"
	-@erase "$(INTDIR)\BurstOptionDialog.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\CommonFunctions.obj"
	-@erase "$(INTDIR)\Evl.obj"
	-@erase "$(INTDIR)\EvlSummary.obj"
	-@erase "$(INTDIR)\EvlTimeCourse.obj"
	-@erase "$(INTDIR)\EvlTimeCourseDialog.obj"
	-@erase "$(INTDIR)\FittingSummary.obj"
	-@erase "$(INTDIR)\History.obj"
	-@erase "$(INTDIR)\IgnoreShortClosingDialog.obj"
	-@erase "$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\NumeircalRecipes.obj"
	-@erase "$(INTDIR)\SegDeletOptionDialog.obj"
	-@erase "$(INTDIR)\SegmentExtractionDialog2.obj"
	-@erase "$(INTDIR)\SmoothingOptionDialog.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Abf_Processing.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Abf_Processing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Abf_Processing.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Abf_Processing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Abf_Processing.pdb" /machine:I386 /out:"$(OUTDIR)\Abf_Processing.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Abf.obj" \
	"$(INTDIR)\Abf_Processing.obj" \
	"$(INTDIR)\Abf_ProcessingDoc.obj" \
	"$(INTDIR)\Abf_ProcessingView.obj" \
	"$(INTDIR)\AbfHeaderToWrite.obj" \
	"$(INTDIR)\AllPointHistgram.obj" \
	"$(INTDIR)\AllPointHistgramDialog.obj" \
	"$(INTDIR)\ArtifactHanddlingOptionDlg.obj" \
	"$(INTDIR)\ArtifactSegmentDialog.obj" \
	"$(INTDIR)\ArtifactsHandling.obj" \
	"$(INTDIR)\ArtifactsRemoving.obj" \
	"$(INTDIR)\BallRolling.obj" \
	"$(INTDIR)\BallRollingOption.obj" \
	"$(INTDIR)\BaselineProcessing.obj" \
	"$(INTDIR)\BlineAdjustOptionDialog.obj" \
	"$(INTDIR)\BurstOptionDialog.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CommonFunctions.obj" \
	"$(INTDIR)\Evl.obj" \
	"$(INTDIR)\EvlSummary.obj" \
	"$(INTDIR)\EvlTimeCourse.obj" \
	"$(INTDIR)\EvlTimeCourseDialog.obj" \
	"$(INTDIR)\FittingSummary.obj" \
	"$(INTDIR)\History.obj" \
	"$(INTDIR)\IgnoreShortClosingDialog.obj" \
	"$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SegDeletOptionDialog.obj" \
	"$(INTDIR)\SegmentExtractionDialog2.obj" \
	"$(INTDIR)\SmoothingOptionDialog.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Abf_Processing.res" \
	"$(INTDIR)\NumeircalRecipes.obj" \
	"$(INTDIR)\amotry.obj" \
	"$(INTDIR)\amoeba.obj"

"$(OUTDIR)\Abf_Processing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Abf_Processing.exe" "$(OUTDIR)\Abf_Processing.bsc"


CLEAN :
	-@erase "$(INTDIR)\Abf.obj"
	-@erase "$(INTDIR)\Abf.sbr"
	-@erase "$(INTDIR)\Abf_Processing.obj"
	-@erase "$(INTDIR)\Abf_Processing.pch"
	-@erase "$(INTDIR)\Abf_Processing.res"
	-@erase "$(INTDIR)\Abf_Processing.sbr"
	-@erase "$(INTDIR)\Abf_ProcessingDoc.obj"
	-@erase "$(INTDIR)\Abf_ProcessingDoc.sbr"
	-@erase "$(INTDIR)\Abf_ProcessingView.obj"
	-@erase "$(INTDIR)\Abf_ProcessingView.sbr"
	-@erase "$(INTDIR)\AbfHeaderToWrite.obj"
	-@erase "$(INTDIR)\AbfHeaderToWrite.sbr"
	-@erase "$(INTDIR)\AllPointHistgram.obj"
	-@erase "$(INTDIR)\AllPointHistgram.sbr"
	-@erase "$(INTDIR)\AllPointHistgramDialog.obj"
	-@erase "$(INTDIR)\AllPointHistgramDialog.sbr"
	-@erase "$(INTDIR)\amoeba.obj"
	-@erase "$(INTDIR)\amoeba.sbr"
	-@erase "$(INTDIR)\amotry.obj"
	-@erase "$(INTDIR)\amotry.sbr"
	-@erase "$(INTDIR)\ArtifactHanddlingOptionDlg.obj"
	-@erase "$(INTDIR)\ArtifactHanddlingOptionDlg.sbr"
	-@erase "$(INTDIR)\ArtifactSegmentDialog.obj"
	-@erase "$(INTDIR)\ArtifactSegmentDialog.sbr"
	-@erase "$(INTDIR)\ArtifactsHandling.obj"
	-@erase "$(INTDIR)\ArtifactsHandling.sbr"
	-@erase "$(INTDIR)\ArtifactsRemoving.obj"
	-@erase "$(INTDIR)\ArtifactsRemoving.sbr"
	-@erase "$(INTDIR)\BallRolling.obj"
	-@erase "$(INTDIR)\BallRolling.sbr"
	-@erase "$(INTDIR)\BallRollingOption.obj"
	-@erase "$(INTDIR)\BallRollingOption.sbr"
	-@erase "$(INTDIR)\BaselineProcessing.obj"
	-@erase "$(INTDIR)\BaselineProcessing.sbr"
	-@erase "$(INTDIR)\BlineAdjustOptionDialog.obj"
	-@erase "$(INTDIR)\BlineAdjustOptionDialog.sbr"
	-@erase "$(INTDIR)\BurstOptionDialog.obj"
	-@erase "$(INTDIR)\BurstOptionDialog.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\CommonFunctions.obj"
	-@erase "$(INTDIR)\CommonFunctions.sbr"
	-@erase "$(INTDIR)\Evl.obj"
	-@erase "$(INTDIR)\Evl.sbr"
	-@erase "$(INTDIR)\EvlSummary.obj"
	-@erase "$(INTDIR)\EvlSummary.sbr"
	-@erase "$(INTDIR)\EvlTimeCourse.obj"
	-@erase "$(INTDIR)\EvlTimeCourse.sbr"
	-@erase "$(INTDIR)\EvlTimeCourseDialog.obj"
	-@erase "$(INTDIR)\EvlTimeCourseDialog.sbr"
	-@erase "$(INTDIR)\FittingSummary.obj"
	-@erase "$(INTDIR)\FittingSummary.sbr"
	-@erase "$(INTDIR)\History.obj"
	-@erase "$(INTDIR)\History.sbr"
	-@erase "$(INTDIR)\IgnoreShortClosingDialog.obj"
	-@erase "$(INTDIR)\IgnoreShortClosingDialog.sbr"
	-@erase "$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj"
	-@erase "$(INTDIR)\IgnoreSmallTransitionOptionDialog.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\NumeircalRecipes.obj"
	-@erase "$(INTDIR)\NumeircalRecipes.sbr"
	-@erase "$(INTDIR)\SegDeletOptionDialog.obj"
	-@erase "$(INTDIR)\SegDeletOptionDialog.sbr"
	-@erase "$(INTDIR)\SegmentExtractionDialog2.obj"
	-@erase "$(INTDIR)\SegmentExtractionDialog2.sbr"
	-@erase "$(INTDIR)\SmoothingOptionDialog.obj"
	-@erase "$(INTDIR)\SmoothingOptionDialog.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Abf_Processing.bsc"
	-@erase "$(OUTDIR)\Abf_Processing.exe"
	-@erase "$(OUTDIR)\Abf_Processing.ilk"
	-@erase "$(OUTDIR)\Abf_Processing.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Abf_Processing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Abf_Processing.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Abf_Processing.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Abf.sbr" \
	"$(INTDIR)\Abf_Processing.sbr" \
	"$(INTDIR)\Abf_ProcessingDoc.sbr" \
	"$(INTDIR)\Abf_ProcessingView.sbr" \
	"$(INTDIR)\AbfHeaderToWrite.sbr" \
	"$(INTDIR)\AllPointHistgram.sbr" \
	"$(INTDIR)\AllPointHistgramDialog.sbr" \
	"$(INTDIR)\ArtifactHanddlingOptionDlg.sbr" \
	"$(INTDIR)\ArtifactSegmentDialog.sbr" \
	"$(INTDIR)\ArtifactsHandling.sbr" \
	"$(INTDIR)\ArtifactsRemoving.sbr" \
	"$(INTDIR)\BallRolling.sbr" \
	"$(INTDIR)\BallRollingOption.sbr" \
	"$(INTDIR)\BaselineProcessing.sbr" \
	"$(INTDIR)\BlineAdjustOptionDialog.sbr" \
	"$(INTDIR)\BurstOptionDialog.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\CommonFunctions.sbr" \
	"$(INTDIR)\Evl.sbr" \
	"$(INTDIR)\EvlSummary.sbr" \
	"$(INTDIR)\EvlTimeCourse.sbr" \
	"$(INTDIR)\EvlTimeCourseDialog.sbr" \
	"$(INTDIR)\FittingSummary.sbr" \
	"$(INTDIR)\History.sbr" \
	"$(INTDIR)\IgnoreShortClosingDialog.sbr" \
	"$(INTDIR)\IgnoreSmallTransitionOptionDialog.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\SegDeletOptionDialog.sbr" \
	"$(INTDIR)\SegmentExtractionDialog2.sbr" \
	"$(INTDIR)\SmoothingOptionDialog.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\NumeircalRecipes.sbr" \
	"$(INTDIR)\amotry.sbr" \
	"$(INTDIR)\amoeba.sbr"

"$(OUTDIR)\Abf_Processing.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Abf_Processing.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Abf_Processing.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Abf.obj" \
	"$(INTDIR)\Abf_Processing.obj" \
	"$(INTDIR)\Abf_ProcessingDoc.obj" \
	"$(INTDIR)\Abf_ProcessingView.obj" \
	"$(INTDIR)\AbfHeaderToWrite.obj" \
	"$(INTDIR)\AllPointHistgram.obj" \
	"$(INTDIR)\AllPointHistgramDialog.obj" \
	"$(INTDIR)\ArtifactHanddlingOptionDlg.obj" \
	"$(INTDIR)\ArtifactSegmentDialog.obj" \
	"$(INTDIR)\ArtifactsHandling.obj" \
	"$(INTDIR)\ArtifactsRemoving.obj" \
	"$(INTDIR)\BallRolling.obj" \
	"$(INTDIR)\BallRollingOption.obj" \
	"$(INTDIR)\BaselineProcessing.obj" \
	"$(INTDIR)\BlineAdjustOptionDialog.obj" \
	"$(INTDIR)\BurstOptionDialog.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CommonFunctions.obj" \
	"$(INTDIR)\Evl.obj" \
	"$(INTDIR)\EvlSummary.obj" \
	"$(INTDIR)\EvlTimeCourse.obj" \
	"$(INTDIR)\EvlTimeCourseDialog.obj" \
	"$(INTDIR)\FittingSummary.obj" \
	"$(INTDIR)\History.obj" \
	"$(INTDIR)\IgnoreShortClosingDialog.obj" \
	"$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SegDeletOptionDialog.obj" \
	"$(INTDIR)\SegmentExtractionDialog2.obj" \
	"$(INTDIR)\SmoothingOptionDialog.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Abf_Processing.res" \
	"$(INTDIR)\NumeircalRecipes.obj" \
	"$(INTDIR)\amotry.obj" \
	"$(INTDIR)\amoeba.obj"

"$(OUTDIR)\Abf_Processing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Abf_Processing.dep")
!INCLUDE "Abf_Processing.dep"
!ELSE 
!MESSAGE Warning: cannot find "Abf_Processing.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Abf_Processing - Win32 Release" || "$(CFG)" == "Abf_Processing - Win32 Debug"
SOURCE=.\Abf.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\Abf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\Abf.obj"	"$(INTDIR)\Abf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\Abf_Processing.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\Abf_Processing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\Abf_Processing.obj"	"$(INTDIR)\Abf_Processing.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\Abf_Processing.rc

"$(INTDIR)\Abf_Processing.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Abf_ProcessingDoc.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\Abf_ProcessingDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\Abf_ProcessingDoc.obj"	"$(INTDIR)\Abf_ProcessingDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\Abf_ProcessingView.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\Abf_ProcessingView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\Abf_ProcessingView.obj"	"$(INTDIR)\Abf_ProcessingView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\AbfHeaderToWrite.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\AbfHeaderToWrite.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\AbfHeaderToWrite.obj"	"$(INTDIR)\AbfHeaderToWrite.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\AllPointHistgram.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\AllPointHistgram.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\AllPointHistgram.obj"	"$(INTDIR)\AllPointHistgram.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\AllPointHistgramDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\AllPointHistgramDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\AllPointHistgramDialog.obj"	"$(INTDIR)\AllPointHistgramDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\amoeba.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\amoeba.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\amoeba.obj"	"$(INTDIR)\amoeba.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\amotry.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\amotry.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\amotry.obj"	"$(INTDIR)\amotry.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\ArtifactHanddlingOptionDlg.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\ArtifactHanddlingOptionDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\ArtifactHanddlingOptionDlg.obj"	"$(INTDIR)\ArtifactHanddlingOptionDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\ArtifactSegmentDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\ArtifactSegmentDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\ArtifactSegmentDialog.obj"	"$(INTDIR)\ArtifactSegmentDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\ArtifactsHandling.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\ArtifactsHandling.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\ArtifactsHandling.obj"	"$(INTDIR)\ArtifactsHandling.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\ArtifactsRemoving.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\ArtifactsRemoving.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\ArtifactsRemoving.obj"	"$(INTDIR)\ArtifactsRemoving.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\BallRolling.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\BallRolling.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\BallRolling.obj"	"$(INTDIR)\BallRolling.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\BallRollingOption.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\BallRollingOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\BallRollingOption.obj"	"$(INTDIR)\BallRollingOption.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\BaselineProcessing.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\BaselineProcessing.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\BaselineProcessing.obj"	"$(INTDIR)\BaselineProcessing.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\BlineAdjustOptionDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\BlineAdjustOptionDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\BlineAdjustOptionDialog.obj"	"$(INTDIR)\BlineAdjustOptionDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\BurstOptionDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\BurstOptionDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\BurstOptionDialog.obj"	"$(INTDIR)\BurstOptionDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\CommonFunctions.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\CommonFunctions.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\CommonFunctions.obj"	"$(INTDIR)\CommonFunctions.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\Evl.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\Evl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\Evl.obj"	"$(INTDIR)\Evl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\EvlSummary.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\EvlSummary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\EvlSummary.obj"	"$(INTDIR)\EvlSummary.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\EvlTimeCourse.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\EvlTimeCourse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\EvlTimeCourse.obj"	"$(INTDIR)\EvlTimeCourse.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\EvlTimeCourseDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\EvlTimeCourseDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\EvlTimeCourseDialog.obj"	"$(INTDIR)\EvlTimeCourseDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\FittingSummary.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\FittingSummary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\FittingSummary.obj"	"$(INTDIR)\FittingSummary.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\History.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\History.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\History.obj"	"$(INTDIR)\History.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\IgnoreShortClosingDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\IgnoreShortClosingDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\IgnoreShortClosingDialog.obj"	"$(INTDIR)\IgnoreShortClosingDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\IgnoreSmallTransitionOptionDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\IgnoreSmallTransitionOptionDialog.obj"	"$(INTDIR)\IgnoreSmallTransitionOptionDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\NumeircalRecipes.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\NumeircalRecipes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\NumeircalRecipes.obj"	"$(INTDIR)\NumeircalRecipes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\SegDeletOptionDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\SegDeletOptionDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\SegDeletOptionDialog.obj"	"$(INTDIR)\SegDeletOptionDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\SegmentExtractionDialog2.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\SegmentExtractionDialog2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\SegmentExtractionDialog2.obj"	"$(INTDIR)\SegmentExtractionDialog2.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\SmoothingOptionDialog.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"


"$(INTDIR)\SmoothingOptionDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"


"$(INTDIR)\SmoothingOptionDialog.obj"	"$(INTDIR)\SmoothingOptionDialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Abf_Processing.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Abf_Processing - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Abf_Processing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Abf_Processing.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Abf_Processing - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Abf_Processing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Abf_Processing.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

