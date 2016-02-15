# Microsoft Developer Studio Project File - Name="tomlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=tomlib - Win32 TopChrono Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tomlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tomlib.mak" CFG="tomlib - Win32 TopChrono Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tomlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "tomlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tomlib - Win32 TopChrono Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "tomlib - Win32 TopChrono Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tomlib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\tomeval" /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_ZOOMRADIOTELE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "tomlib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\tomeval" /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_ZOOMRADIOTELE" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "tomlib - Win32 TopChrono Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tomlib___Win32_TopChrono_Debug"
# PROP BASE Intermediate_Dir "tomlib___Win32_TopChrono_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "tomlib___Win32_TopChrono_Debug"
# PROP Intermediate_Dir "tomlib___Win32_TopChrono_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\tomeval" /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_TOPCHRONO" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\tomeval" /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_TOPCHRONO" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "tomlib - Win32 TopChrono Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomlib___Win32_TopChrono_Release"
# PROP BASE Intermediate_Dir "tomlib___Win32_TopChrono_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "tomlib___Win32_TopChrono_Release"
# PROP Intermediate_Dir "tomlib___Win32_TopChrono_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\tomeval" /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_TOPCHRONO" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\tomeval" /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_TOPCHRONO" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "tomlib - Win32 Release"
# Name "tomlib - Win32 Debug"
# Name "tomlib - Win32 TopChrono Debug"
# Name "tomlib - Win32 TopChrono Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\tomeval\Convert.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\Eval.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicAUX.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicCIB.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicLBX.cpp
# End Source File
# Begin Source File

SOURCE=..\common\FicTbl.cpp
# End Source File
# Begin Source File

SOURCE=..\common\FicVague.cpp
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# Begin Source File

SOURCE=..\tomeval\Source.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\tomeval\stdfonc.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\tomcalc.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\tomfich.cpp
# End Source File
# Begin Source File

SOURCE=..\tomeval\tomsourc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\tomeval\Convert.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\Eval.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicAUX.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicCIB.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicLBX.h
# End Source File
# Begin Source File

SOURCE=..\common\FicTbl.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\FicVague.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\Source.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\stdfonc.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\tom2cup.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\tom_msg.h
# End Source File
# Begin Source File

SOURCE=..\tomeval\tomprot.h
# End Source File
# End Group
# End Target
# End Project
