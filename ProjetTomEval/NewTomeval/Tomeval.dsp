# Microsoft Developer Studio Project File - Name="Tomeval" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Tomeval - Win32 Debug Nova Russe
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tomeval.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tomeval.mak" CFG="Tomeval - Win32 Debug Nova Russe"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tomeval - Win32 Release Carat" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Release JFC" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug JFC" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug Carat" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug CIM belgique" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Release CIM Belgique" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug Russie" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Release Russie" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Release CIM Russe" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug CIM Russe" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Debug Nova Russe" (based on "Win32 (x86) Application")
!MESSAGE "Tomeval - Win32 Release Nova Russe" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_Carat"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_Carat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_Carat"
# PROP Intermediate_Dir "Tomeval___Win32_Release_Carat"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_CARAT" /D "OPTION_TELEFRANCE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_JFC"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_JFC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_JFC"
# PROP Intermediate_Dir "Tomeval___Win32_Release_JFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_TELEFRANCE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_JFC/Tomeval.tele.exe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_JFC0"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_JFC0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_JFC0"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_JFC0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_TELEFRANCE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_Carat"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_Carat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_Carat"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_Carat"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_CARAT" /D "OPTION_TELEFRANCE" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_CIM_belgique"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_CIM_belgique"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_CIM_belgique"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_CIM_belgique"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_ALGERIE" /D "OPTION_CIMBELGE" /D "OPTION_REACH_BELGE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_CIM_Belgique"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_CIM_Belgique"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_CIM_Belgique"
# PROP Intermediate_Dir "Tomeval___Win32_Release_CIM_Belgique"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_TELEFRANCE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_ALGERIE" /D "OPTION_JFC" /D "OPTION_CIMBELGE" /D "OPTION_REACH_BELGE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_JFC/Tomeval.tele.exe"
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_CIM_Belgique/TomevalCIM.exe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_Russie"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_Russie"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_Russie"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_Russie"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_ALGERIE" /D "OPTION_CIMBELGE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Tomeval___Win32_Debug_Russie/TomevalRU.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_Russie"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_Russie"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_Russie"
# PROP Intermediate_Dir "Tomeval___Win32_Release_Russie"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_ALGERIE" /D "OPTION_JFC" /D "OPTION_CIMBELGE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_CIM_Belgique/TomevalCIM.exe"
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_Russie/TomevalRU.exe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_CIM_Russe"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_CIM_Russe"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_CIM_Russe"
# PROP Intermediate_Dir "Tomeval___Win32_Release_CIM_Russe"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_Russie/TomevalRU.exe"
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_CIM_Russe/TomevalCIM.exe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_CIM_Russe"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_CIM_Russe"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_CIM_Russe"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_CIM_Russe"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Tomeval___Win32_Debug_Russie/TomevalRU.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Tomeval___Win32_Debug_CIM_Russe/TomevalCIM.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tomeval___Win32_Debug_Nova_Russe"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Debug_Nova_Russe"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Tomeval___Win32_Debug_Nova_Russe"
# PROP Intermediate_Dir "Tomeval___Win32_Debug_Nova_Russe"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_NOVA_RUSSE" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Tomeval___Win32_Debug_CIM_Russe/TomevalCIM.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Tomeval___Win32_Debug_Nova_Russe/TomevalNovaR.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Tomeval___Win32_Release_Nova_Russe"
# PROP BASE Intermediate_Dir "Tomeval___Win32_Release_Nova_Russe"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Tomeval___Win32_Release_Nova_Russe"
# PROP Intermediate_Dir "Tomeval___Win32_Release_Nova_Russe"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "../composants" /I "../composants/tomy" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OPTION_JFC" /D "OPTION_NOVA_RUSSE" /D "OPTION_RUSSIE" /D "OPTION_CIM_RUSSE" /D "OPTION_DUREEECOUTE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_CIM_Russe/TomevalCIM.exe"
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Tomeval___Win32_Release_Nova_Russe/TomevalNovaR.exe"

!ENDIF 

# Begin Target

# Name "Tomeval - Win32 Release Carat"
# Name "Tomeval - Win32 Release JFC"
# Name "Tomeval - Win32 Debug JFC"
# Name "Tomeval - Win32 Debug Carat"
# Name "Tomeval - Win32 Debug CIM belgique"
# Name "Tomeval - Win32 Release CIM Belgique"
# Name "Tomeval - Win32 Debug Russie"
# Name "Tomeval - Win32 Release Russie"
# Name "Tomeval - Win32 Release CIM Russe"
# Name "Tomeval - Win32 Debug CIM Russe"
# Name "Tomeval - Win32 Debug Nova Russe"
# Name "Tomeval - Win32 Release Nova Russe"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BetaBinomial.cpp
# End Source File
# Begin Source File

SOURCE=.\Cibjob.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\DistributionCroisee.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\DistributionCroiseeEx.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\EcouteSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\FicAEJ.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\FicAQJ.cpp
# End Source File
# Begin Source File

SOURCE=.\FicLBX.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\FicProba.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\FicTbl.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\JFCFileException.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\LiaisonChaineTeleFrance.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\Lien.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\PanelLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\ProbaSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\Restom.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultatJour.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TableConsomationTotalTV.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TablePopulationEtalon.cpp
# End Source File
# Begin Source File

SOURCE=.\TomEval.cpp
# End Source File
# Begin Source File

SOURCE=.\Tomeval.rc
# End Source File
# Begin Source File

SOURCE=.\TomevalApp.cpp
# End Source File
# Begin Source File

SOURCE=.\TomevalCarat.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TomevalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tomjob.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\Tomy.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCIMBelge.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TomyCIMRusse.cpp
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyConsolide.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyConsolideMediadataMediacabsat.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCroisee.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCroiseeMediacabsat.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyMediadata.cpp

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TomyRussie.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BetaBinomial.h
# End Source File
# Begin Source File

SOURCE=.\Cibjob.h

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\DistributionCroisee.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\DistributionCroiseeEx.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\EcouteSupport.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\FicAEJ.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\FicAQJ.h
# End Source File
# Begin Source File

SOURCE=.\FicLBX.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\FicProba.h
# End Source File
# Begin Source File

SOURCE=..\composants\FicTbl.h
# End Source File
# Begin Source File

SOURCE=..\composants\JFCFileException.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\LiaisonChaineTeleFrance.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\Lien.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\PanelLoader.h
# End Source File
# Begin Source File

SOURCE=..\COMPOSANTS\TOMY\ProbaSupport.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Restom.h
# End Source File
# Begin Source File

SOURCE=.\ResultatJour.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TableConsomationTotalTV.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TablePopulationEtalon.h
# End Source File
# Begin Source File

SOURCE=.\tom_msg.h
# End Source File
# Begin Source File

SOURCE=.\tomcarat.h
# End Source File
# Begin Source File

SOURCE=.\Tomeval.h
# End Source File
# Begin Source File

SOURCE=.\TomevalApp.h
# End Source File
# Begin Source File

SOURCE=.\TomevalCarat.h

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TomevalDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tomjob.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\tomy.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCIMBelge.h

!IF  "$(CFG)" == "Tomeval - Win32 Release Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug JFC"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Carat"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Belgique"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Russie"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug CIM Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Debug Nova Russe"

!ELSEIF  "$(CFG)" == "Tomeval - Win32 Release Nova Russe"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TomyCIMRusse.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyConsolide.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyConsolideMediadataMediacabsat.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCroisee.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyCroiseeMediacabsat.h
# End Source File
# Begin Source File

SOURCE=.\TomyMarginal.h
# End Source File
# Begin Source File

SOURCE=..\composants\Tomy\TomyMediadata.h
# End Source File
# Begin Source File

SOURCE=.\TomyRussie.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Tomeval.ico
# End Source File
# Begin Source File

SOURCE=.\res\Tomeval.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\DATATM
# End Source File
# Begin Source File

SOURCE=.\DATATM.tele
# End Source File
# Begin Source File

SOURCE=.\docjobjfc.txt
# End Source File
# Begin Source File

SOURCE=.\jobcarat.txt
# End Source File
# Begin Source File

SOURCE=.\jobjfc.txt01
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\restomcarat.txt
# End Source File
# Begin Source File

SOURCE=.\restomjfc.txt
# End Source File
# Begin Source File

SOURCE=.\restomjfc.txt01
# End Source File
# Begin Source File

SOURCE=.\Version.txt
# End Source File
# End Target
# End Project
# Section Tomeval : {F500E4EE-F641-4E53-88C8-9CB438DF3F8D}
# 	2:10:FicLBX.cpp:FicLBX.cpp
# 	2:14:CLASS: CFicLBX:CFicLBX
# 	2:8:FicLBX.h:FicLBX.h
# 	2:19:Application Include:Tomeval.h
# End Section
