# Microsoft Developer Studio Project File - Name="tomeval" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tomeval - Win32 Debug Pologne
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tomeval.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tomeval.mak" CFG="tomeval - Win32 Debug Pologne"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tomeval - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Release Carat" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Release Mediatop" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Release HDE" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Mediaconsult Debug" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Mediaconsult Release" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Debug Carat" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Release Pologne" (based on "Win32 (x86) Application")
!MESSAGE "tomeval - Win32 Debug Pologne" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tomeval - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Carat"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomeval___Win32_Release_Carat"
# PROP BASE Intermediate_Dir "tomeval___Win32_Release_Carat"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Carat"
# PROP Intermediate_Dir "Release_Carat"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_CARAT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Mediatop"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomeval___Win32_Release_Mediatop"
# PROP BASE Intermediate_Dir "tomeval___Win32_Release_Mediatop"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Mediatop"
# PROP Intermediate_Dir "Release_Mediatop"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_MEDIATOP" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release HDE"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomeval___Win32_Release_HDE"
# PROP BASE Intermediate_Dir "tomeval___Win32_Release_HDE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_HDE"
# PROP Intermediate_Dir "Release_HDE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_CHECKUP" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tomeval___Win32_Mediaconsult_Debug"
# PROP BASE Intermediate_Dir "tomeval___Win32_Mediaconsult_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Mediaconsult_Debug"
# PROP Intermediate_Dir "Mediaconsult_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_MEDIACONSULT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomeval___Win32_Mediaconsult_Release"
# PROP BASE Intermediate_Dir "tomeval___Win32_Mediaconsult_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Mediaconsult_Release"
# PROP Intermediate_Dir "Mediaconsult_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_MEDIACONSULT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Carat"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tomeval___Win32_Debug_Carat"
# PROP BASE Intermediate_Dir "tomeval___Win32_Debug_Carat"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Carat"
# PROP Intermediate_Dir "Debug_Carat"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_CARAT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Pologne"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tomeval___Win32_Release_Pologne"
# PROP BASE Intermediate_Dir "tomeval___Win32_Release_Pologne"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "tomeval___Win32_Release_Pologne"
# PROP Intermediate_Dir "tomeval___Win32_Release_Pologne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OPTION_JFC" /D "OPTION_POLOGNE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Pologne"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tomeval___Win32_Debug_Pologne"
# PROP BASE Intermediate_Dir "tomeval___Win32_Debug_Pologne"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "tomeval___Win32_Debug_Pologne"
# PROP Intermediate_Dir "tomeval___Win32_Debug_Pologne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_JFC" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OPTION_JFC" /D "OPTION_POLOGNE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "tomeval - Win32 Release"
# Name "tomeval - Win32 Debug"
# Name "tomeval - Win32 Release Carat"
# Name "tomeval - Win32 Release Mediatop"
# Name "tomeval - Win32 Release HDE"
# Name "tomeval - Win32 Mediaconsult Debug"
# Name "tomeval - Win32 Mediaconsult Release"
# Name "tomeval - Win32 Debug Carat"
# Name "tomeval - Win32 Release Pologne"
# Name "tomeval - Win32 Debug Pologne"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Convert.cpp
# End Source File
# Begin Source File

SOURCE=.\Eval.cpp
# End Source File
# Begin Source File

SOURCE=.\FicAUX.cpp
# End Source File
# Begin Source File

SOURCE=.\FicAUXitaly.cpp
# End Source File
# Begin Source File

SOURCE=.\FicCIB.cpp
# End Source File
# Begin Source File

SOURCE=.\FicCod.cpp
# End Source File
# Begin Source File

SOURCE=.\FicIND.cpp
# End Source File
# Begin Source File

SOURCE=.\FicLBX.cpp
# End Source File
# Begin Source File

SOURCE=..\common\FicTbl.cpp
# End Source File
# Begin Source File

SOURCE=..\common\FicVague.cpp
# End Source File
# Begin Source File

SOURCE=.\Source.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stdfonc.cpp
# End Source File
# Begin Source File

SOURCE=.\tomcalc.cpp
# End Source File
# Begin Source File

SOURCE=.\tomeval.cpp
# End Source File
# Begin Source File

SOURCE=.\tomeval.rc
# End Source File
# Begin Source File

SOURCE=.\tomevalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\tomfich.cpp
# End Source File
# Begin Source File

SOURCE=.\tomsourc.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Convert.h
# End Source File
# Begin Source File

SOURCE=.\Eval.h
# End Source File
# Begin Source File

SOURCE=.\FicAUX.h
# End Source File
# Begin Source File

SOURCE=.\FicAUXitaly.h
# End Source File
# Begin Source File

SOURCE=.\FicCIB.h
# End Source File
# Begin Source File

SOURCE=.\FicCod.h
# End Source File
# Begin Source File

SOURCE=.\FicIND.h
# End Source File
# Begin Source File

SOURCE=.\FicLBX.h
# End Source File
# Begin Source File

SOURCE=..\common\FicTbl.h
# End Source File
# Begin Source File

SOURCE=..\common\FicVague.h
# End Source File
# Begin Source File

SOURCE=.\mediaconsult.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Source.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stdfonc.h
# End Source File
# Begin Source File

SOURCE=.\tom2cup.h
# End Source File
# Begin Source File

SOURCE=.\tom_msg.h
# End Source File
# Begin Source File

SOURCE=.\tomcarat.h
# End Source File
# Begin Source File

SOURCE=.\tomeval.h
# End Source File
# Begin Source File

SOURCE=.\tomevalDlg.h
# End Source File
# Begin Source File

SOURCE=.\tomprot.h
# End Source File
# Begin Source File

SOURCE=.\TraceFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\tomeval.ico
# End Source File
# Begin Source File

SOURCE=.\res\tomeval.rc2
# End Source File
# End Group
# Begin Group "textes"

# PROP Default_Filter "*.txt"
# Begin Source File

SOURCE=.\afaire.txt
# End Source File
# Begin Source File

SOURCE=.\carat.txt
# End Source File
# Begin Source File

SOURCE=.\DATATM
# End Source File
# Begin Source File

SOURCE=.\EvalJob.txt
# End Source File
# Begin Source File

SOURCE=.\info.txt
# End Source File
# Begin Source File

SOURCE=.\mediaconsult.txt
# End Source File
# Begin Source File

SOURCE=.\methode.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Restom
# End Source File
# Begin Source File

SOURCE=.\Restom.txt
# End Source File
# Begin Source File

SOURCE=.\tomjob
# End Source File
# Begin Source File

SOURCE=.\tomjob.txt
# End Source File
# Begin Source File

SOURCE=.\tomjob01
# End Source File
# Begin Source File

SOURCE=.\versions.txt
# End Source File
# End Group
# Begin Source File

SOURCE=.\Mediaconsult_library\Random.lib

!IF  "$(CFG)" == "tomeval - Win32 Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Mediatop"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release HDE"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Pologne"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Pologne"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mediaconsult_library\Dfor.lib

!IF  "$(CFG)" == "tomeval - Win32 Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Mediatop"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release HDE"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Pologne"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Pologne"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mediaconsult_library\Dfport.lib

!IF  "$(CFG)" == "tomeval - Win32 Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Mediatop"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release HDE"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Pologne"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Pologne"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mediaconsult_library\Dfconsol.lib

!IF  "$(CFG)" == "tomeval - Win32 Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Mediatop"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release HDE"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Debug"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Mediaconsult Release"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "tomeval - Win32 Release Pologne"

!ELSEIF  "$(CFG)" == "tomeval - Win32 Debug Pologne"

!ENDIF 

# End Source File
# End Target
# End Project
# Section tomeval : {8F8BCF27-A7A2-11D1-95EA-444553540000}
# 	2:8:FicTbl.h:FicTbl2.h
# 	2:14:CLASS: CFicTbl:CFicTbl
# 	2:10:FicTbl.cpp:FicTbl2.cpp
# 	2:19:Application Include:tomeval.h
# End Section
# Section tomeval : {8F8BCF22-A7A2-11D1-95EA-444553540000}
# 	2:13:FicVagueCIB.h:FicVagueCIB.h
# 	2:15:FicVagueCIB.cpp:FicVagueCIB.cpp
# 	2:19:CLASS: CFicVagueCIB:CFicVagueCIB
# 	2:19:Application Include:tomeval.h
# End Section
# Section tomeval : {00000000-0000-0000-0000-000000000000}
# 	1:17:CG_IDS_DISK_SPACE:104
# 	1:19:CG_IDS_PHYSICAL_MEM:103
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:105
# 	2:14:PhysicalMemory:CG_IDS_PHYSICAL_MEM
# 	2:9:DiskSpace:CG_IDS_DISK_SPACE
# 	2:16:SpaceUnavailable:CG_IDS_DISK_SPACE_UNAVAIL
# 	2:7:NewFunc:1
# 	2:10:SysInfoKey:1234
# End Section
# Section tomeval : {8F8BCF21-A7A2-11D1-95EA-444553540000}
# 	2:10:FicAUX.cpp:FicAUX.cpp
# 	2:19:Application Include:tomeval.h
# 	2:14:CLASS: CFicAUX:CFicAUX
# 	2:8:FicAUX.h:FicAUX.h
# End Section
