# Microsoft Developer Studio Generated NMAKE File, Based on tomeval.dsp
!IF "$(CFG)" == ""
CFG=tomeval - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tomeval - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tomeval - Win32 Release" && "$(CFG)" !=\
 "tomeval - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tomeval.mak" CFG="tomeval - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tomeval - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tomeval - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tomeval.exe" "$(OUTDIR)\tomeval.bsc"

!ELSE 

ALL : "$(OUTDIR)\tomeval.exe" "$(OUTDIR)\tomeval.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Eval.obj"
	-@erase "$(INTDIR)\Eval.sbr"
	-@erase "$(INTDIR)\FicAUX.obj"
	-@erase "$(INTDIR)\FicAUX.sbr"
	-@erase "$(INTDIR)\FicCIB.obj"
	-@erase "$(INTDIR)\FicCIB.sbr"
	-@erase "$(INTDIR)\FicLBX.obj"
	-@erase "$(INTDIR)\FicLBX.sbr"
	-@erase "$(INTDIR)\FicTbl.obj"
	-@erase "$(INTDIR)\FicTbl.sbr"
	-@erase "$(INTDIR)\FicVague.obj"
	-@erase "$(INTDIR)\FicVague.sbr"
	-@erase "$(INTDIR)\Source.obj"
	-@erase "$(INTDIR)\Source.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\stdfonc.obj"
	-@erase "$(INTDIR)\stdfonc.sbr"
	-@erase "$(INTDIR)\tomcalc.obj"
	-@erase "$(INTDIR)\tomcalc.sbr"
	-@erase "$(INTDIR)\tomeval.obj"
	-@erase "$(INTDIR)\tomeval.pch"
	-@erase "$(INTDIR)\tomeval.res"
	-@erase "$(INTDIR)\tomeval.sbr"
	-@erase "$(INTDIR)\tomevalDlg.obj"
	-@erase "$(INTDIR)\tomevalDlg.sbr"
	-@erase "$(INTDIR)\tomfich.obj"
	-@erase "$(INTDIR)\tomfich.sbr"
	-@erase "$(INTDIR)\tomsourc.obj"
	-@erase "$(INTDIR)\tomsourc.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\tomeval.bsc"
	-@erase "$(OUTDIR)\tomeval.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tomeval.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\tomeval.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tomeval.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Eval.sbr" \
	"$(INTDIR)\FicAUX.sbr" \
	"$(INTDIR)\FicCIB.sbr" \
	"$(INTDIR)\FicLBX.sbr" \
	"$(INTDIR)\FicTbl.sbr" \
	"$(INTDIR)\FicVague.sbr" \
	"$(INTDIR)\Source.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\stdfonc.sbr" \
	"$(INTDIR)\tomcalc.sbr" \
	"$(INTDIR)\tomeval.sbr" \
	"$(INTDIR)\tomevalDlg.sbr" \
	"$(INTDIR)\tomfich.sbr" \
	"$(INTDIR)\tomsourc.sbr"

"$(OUTDIR)\tomeval.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\tomeval.pdb" /machine:I386 /out:"$(OUTDIR)\tomeval.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Eval.obj" \
	"$(INTDIR)\FicAUX.obj" \
	"$(INTDIR)\FicCIB.obj" \
	"$(INTDIR)\FicLBX.obj" \
	"$(INTDIR)\FicTbl.obj" \
	"$(INTDIR)\FicVague.obj" \
	"$(INTDIR)\Source.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\stdfonc.obj" \
	"$(INTDIR)\tomcalc.obj" \
	"$(INTDIR)\tomeval.obj" \
	"$(INTDIR)\tomeval.res" \
	"$(INTDIR)\tomevalDlg.obj" \
	"$(INTDIR)\tomfich.obj" \
	"$(INTDIR)\tomsourc.obj"

"$(OUTDIR)\tomeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tomeval.exe" "$(OUTDIR)\tomeval.bsc"

!ELSE 

ALL : "$(OUTDIR)\tomeval.exe" "$(OUTDIR)\tomeval.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Eval.obj"
	-@erase "$(INTDIR)\Eval.sbr"
	-@erase "$(INTDIR)\FicAUX.obj"
	-@erase "$(INTDIR)\FicAUX.sbr"
	-@erase "$(INTDIR)\FicCIB.obj"
	-@erase "$(INTDIR)\FicCIB.sbr"
	-@erase "$(INTDIR)\FicLBX.obj"
	-@erase "$(INTDIR)\FicLBX.sbr"
	-@erase "$(INTDIR)\FicTbl.obj"
	-@erase "$(INTDIR)\FicTbl.sbr"
	-@erase "$(INTDIR)\FicVague.obj"
	-@erase "$(INTDIR)\FicVague.sbr"
	-@erase "$(INTDIR)\Source.obj"
	-@erase "$(INTDIR)\Source.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\stdfonc.obj"
	-@erase "$(INTDIR)\stdfonc.sbr"
	-@erase "$(INTDIR)\tomcalc.obj"
	-@erase "$(INTDIR)\tomcalc.sbr"
	-@erase "$(INTDIR)\tomeval.obj"
	-@erase "$(INTDIR)\tomeval.pch"
	-@erase "$(INTDIR)\tomeval.res"
	-@erase "$(INTDIR)\tomeval.sbr"
	-@erase "$(INTDIR)\tomevalDlg.obj"
	-@erase "$(INTDIR)\tomevalDlg.sbr"
	-@erase "$(INTDIR)\tomfich.obj"
	-@erase "$(INTDIR)\tomfich.sbr"
	-@erase "$(INTDIR)\tomsourc.obj"
	-@erase "$(INTDIR)\tomsourc.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\tomeval.bsc"
	-@erase "$(OUTDIR)\tomeval.exe"
	-@erase "$(OUTDIR)\tomeval.ilk"
	-@erase "$(OUTDIR)\tomeval.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MTd /W3 /Gm /GX /Zi /Od /I "..\common" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tomeval.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\tomeval.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tomeval.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Eval.sbr" \
	"$(INTDIR)\FicAUX.sbr" \
	"$(INTDIR)\FicCIB.sbr" \
	"$(INTDIR)\FicLBX.sbr" \
	"$(INTDIR)\FicTbl.sbr" \
	"$(INTDIR)\FicVague.sbr" \
	"$(INTDIR)\Source.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\stdfonc.sbr" \
	"$(INTDIR)\tomcalc.sbr" \
	"$(INTDIR)\tomeval.sbr" \
	"$(INTDIR)\tomevalDlg.sbr" \
	"$(INTDIR)\tomfich.sbr" \
	"$(INTDIR)\tomsourc.sbr"

"$(OUTDIR)\tomeval.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\tomeval.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tomeval.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Eval.obj" \
	"$(INTDIR)\FicAUX.obj" \
	"$(INTDIR)\FicCIB.obj" \
	"$(INTDIR)\FicLBX.obj" \
	"$(INTDIR)\FicTbl.obj" \
	"$(INTDIR)\FicVague.obj" \
	"$(INTDIR)\Source.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\stdfonc.obj" \
	"$(INTDIR)\tomcalc.obj" \
	"$(INTDIR)\tomeval.obj" \
	"$(INTDIR)\tomeval.res" \
	"$(INTDIR)\tomevalDlg.obj" \
	"$(INTDIR)\tomfich.obj" \
	"$(INTDIR)\tomsourc.obj"

"$(OUTDIR)\tomeval.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "tomeval - Win32 Release" || "$(CFG)" ==\
 "tomeval - Win32 Debug"
SOURCE=.\Eval.cpp
DEP_CPP_EVAL_=\
	".\Eval.h"\
	

"$(INTDIR)\Eval.obj"	"$(INTDIR)\Eval.sbr" : $(SOURCE) $(DEP_CPP_EVAL_)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


SOURCE=.\FicAUX.cpp
DEP_CPP_FICAU=\
	".\FicAUX.h"\
	

"$(INTDIR)\FicAUX.obj"	"$(INTDIR)\FicAUX.sbr" : $(SOURCE) $(DEP_CPP_FICAU)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


SOURCE=.\FicCIB.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_FICCI=\
	"..\common\FicTbl.h"\
	".\FicCIB.h"\
	

"$(INTDIR)\FicCIB.obj"	"$(INTDIR)\FicCIB.sbr" : $(SOURCE) $(DEP_CPP_FICCI)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_FICCI=\
	"..\common\FicTbl.h"\
	".\FicCIB.h"\
	

"$(INTDIR)\FicCIB.obj"	"$(INTDIR)\FicCIB.sbr" : $(SOURCE) $(DEP_CPP_FICCI)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 

SOURCE=.\FicLBX.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_FICLB=\
	"..\common\FicTbl.h"\
	".\FicLBX.h"\
	

"$(INTDIR)\FicLBX.obj"	"$(INTDIR)\FicLBX.sbr" : $(SOURCE) $(DEP_CPP_FICLB)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_FICLB=\
	"..\common\FicTbl.h"\
	".\FicLBX.h"\
	

"$(INTDIR)\FicLBX.obj"	"$(INTDIR)\FicLBX.sbr" : $(SOURCE) $(DEP_CPP_FICLB)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 

SOURCE=..\common\FicTbl.cpp
DEP_CPP_FICTB=\
	"..\common\FicTbl.h"\
	

"$(INTDIR)\FicTbl.obj"	"$(INTDIR)\FicTbl.sbr" : $(SOURCE) $(DEP_CPP_FICTB)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\common\FicVague.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_FICVA=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	

"$(INTDIR)\FicVague.obj"	"$(INTDIR)\FicVague.sbr" : $(SOURCE) $(DEP_CPP_FICVA)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_FICVA=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	

"$(INTDIR)\FicVague.obj"	"$(INTDIR)\FicVague.sbr" : $(SOURCE) $(DEP_CPP_FICVA)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_SOURC=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	".\FicAUX.h"\
	".\FicCIB.h"\
	".\FicLBX.h"\
	".\Source.h"\
	

"$(INTDIR)\Source.obj"	"$(INTDIR)\Source.sbr" : $(SOURCE) $(DEP_CPP_SOURC)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_SOURC=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	".\FicAUX.h"\
	".\FicCIB.h"\
	".\FicLBX.h"\
	".\Source.h"\
	

"$(INTDIR)\Source.obj"	"$(INTDIR)\Source.sbr" : $(SOURCE) $(DEP_CPP_SOURC)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	".\stdfonc.h"\
	".\tom2cup.h"\
	".\tomprot.h"\
	

!IF  "$(CFG)" == "tomeval - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG"\
 /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tomeval.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\tomeval.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

CPP_SWITCHES=/nologo /Zp1 /MTd /W3 /Gm /GX /Zi /Od /I "..\common" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tomeval.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\tomeval.pch" : \
$(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\stdfonc.cpp

"$(INTDIR)\stdfonc.obj"	"$(INTDIR)\stdfonc.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\tomeval.pch"


SOURCE=.\tomcalc.cpp

"$(INTDIR)\tomcalc.obj"	"$(INTDIR)\tomcalc.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\tomeval.pch"


SOURCE=.\tomeval.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_TOMEV=\
	".\Eval.h"\
	".\tomeval.h"\
	".\tomevalDlg.h"\
	

"$(INTDIR)\tomeval.obj"	"$(INTDIR)\tomeval.sbr" : $(SOURCE) $(DEP_CPP_TOMEV)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_TOMEV=\
	".\Eval.h"\
	".\tomeval.h"\
	".\tomevalDlg.h"\
	

"$(INTDIR)\tomeval.obj"	"$(INTDIR)\tomeval.sbr" : $(SOURCE) $(DEP_CPP_TOMEV)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 

SOURCE=.\tomeval.rc
DEP_RSC_TOMEVA=\
	".\res\tomeval.ico"\
	".\res\tomeval.rc2"\
	

"$(INTDIR)\tomeval.res" : $(SOURCE) $(DEP_RSC_TOMEVA) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\tomevalDlg.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_TOMEVAL=\
	".\Eval.h"\
	".\tomeval.h"\
	".\tomevalDlg.h"\
	

"$(INTDIR)\tomevalDlg.obj"	"$(INTDIR)\tomevalDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_TOMEVAL) "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_TOMEVAL=\
	".\Eval.h"\
	".\tomeval.h"\
	".\tomevalDlg.h"\
	

"$(INTDIR)\tomevalDlg.obj"	"$(INTDIR)\tomevalDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_TOMEVAL) "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 

SOURCE=.\tomfich.cpp

"$(INTDIR)\tomfich.obj"	"$(INTDIR)\tomfich.sbr" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\tomeval.pch"


SOURCE=.\tomsourc.cpp

!IF  "$(CFG)" == "tomeval - Win32 Release"

DEP_CPP_TOMSO=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	".\FicAUX.h"\
	".\FicCIB.h"\
	".\FicLBX.h"\
	".\Source.h"\
	

"$(INTDIR)\tomsourc.obj"	"$(INTDIR)\tomsourc.sbr" : $(SOURCE) $(DEP_CPP_TOMSO)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

DEP_CPP_TOMSO=\
	"..\common\FicTbl.h"\
	"..\common\FicVague.h"\
	".\FicAUX.h"\
	".\FicCIB.h"\
	".\FicLBX.h"\
	".\Source.h"\
	

"$(INTDIR)\tomsourc.obj"	"$(INTDIR)\tomsourc.sbr" : $(SOURCE) $(DEP_CPP_TOMSO)\
 "$(INTDIR)" "$(INTDIR)\tomeval.pch"


!ENDIF 


!ENDIF 

