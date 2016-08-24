# Microsoft Developer Studio Project File - Name="admin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=admin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "admin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "admin.mak" CFG="admin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "admin - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "admin - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "admin - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /I "..\..\composants" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
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

!ELSEIF  "$(CFG)" == "admin - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\..\composants" /I "C:\Program Files\Stingray Objective Studio\OT602\Src\controls" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /i "..\..\composants" /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "admin - Win32 Release"
# Name "admin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\admin.cpp
# End Source File
# Begin Source File

SOURCE=.\admin.rc
# End Source File
# Begin Source File

SOURCE=..\BaseCinema.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Stingray Objective Studio\Ot602\Src\controls\browedit.cpp"
# End Source File
# Begin Source File

SOURCE=..\CAlendrierHebdoCinema.cpp
# End Source File
# Begin Source File

SOURCE=..\CFichierCinema.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstCinema.cpp
# End Source File
# Begin Source File

SOURCE=.\DlAjoutFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutBac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutBacGroupe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutCmplx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutComplexeGroupe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditModifTarif.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifBac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifCmplx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifCNC.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelBac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectChemin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionBac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionComplexe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppBac.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppBacGroupe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppCmplx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppCpmlxGroupe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSuppTempo.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Stingray Objective Studio\Ot602\Src\utility\filesys.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\composants\JFCFileException.cpp
# End Source File
# Begin Source File

SOURCE=..\ListBox_Complexe.cpp
# End Source File
# Begin Source File

SOURCE=..\ListBoxSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\composants\MoveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\composants\Obj_Gray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\composants\Obj_LBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Saisonnalite.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\TableBAC.cpp
# End Source File
# Begin Source File

SOURCE=..\TableComplexe.cpp
# End Source File
# Begin Source File

SOURCE=..\TableFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\TableGroupeBAC.cpp
# End Source File
# Begin Source File

SOURCE=..\TableGroupeComplexe.cpp
# End Source File
# Begin Source File

SOURCE=..\TableNbEntreeComplexe.cpp
# End Source File
# Begin Source File

SOURCE=..\TableProduit.cpp
# End Source File
# Begin Source File

SOURCE=..\TableTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\tagProduitFilms.cpp
# End Source File
# Begin Source File

SOURCE=..\Tarif.cpp
# End Source File
# Begin Source File

SOURCE=.\TarifAnnee.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\admin.ico
# End Source File
# Begin Source File

SOURCE=.\res\admin.rc2
# End Source File
# Begin Source File

SOURCE=.\res\cinema.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\admin.h
# End Source File
# Begin Source File

SOURCE=..\BaseCinema.h
# End Source File
# Begin Source File

SOURCE=..\CFichierCinema.h
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\ConstCinema.h
# End Source File
# Begin Source File

SOURCE=.\CTableGroupeBAC.h
# End Source File
# Begin Source File

SOURCE=.\CTableGroupeComplexe.h
# End Source File
# Begin Source File

SOURCE=.\DlAjoutFormat.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutBac.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutBacGroupe.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutCmplx.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutComplexeGroupe.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutProduit.h
# End Source File
# Begin Source File

SOURCE=.\DlgAjoutTempo.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditModifTarif.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifBac.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifCmplx.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifCNC.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifProduit.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifTempo.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectChemin.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionBac.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionComplexe.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectProduit.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppBac.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppBacGroupe.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppCmplx.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppCpmlxGroupe.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppFormat.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppProduit.h
# End Source File
# Begin Source File

SOURCE=.\DlgSuppTempo.h
# End Source File
# Begin Source File

SOURCE=.\EditModifTarif.h
# End Source File
# Begin Source File

SOURCE=..\ListBoxSelect.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\Saisonnalite.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Table.h
# End Source File
# Begin Source File

SOURCE=..\TableBAC.h
# End Source File
# Begin Source File

SOURCE=..\TableComplexe.h
# End Source File
# Begin Source File

SOURCE=..\TableFormat.h
# End Source File
# Begin Source File

SOURCE=..\TableGroupeBAC.h
# End Source File
# Begin Source File

SOURCE=..\TableGroupeComplexe.h
# End Source File
# Begin Source File

SOURCE=..\TableNbEntreeComplexe.h
# End Source File
# Begin Source File

SOURCE=..\TableProduit.h
# End Source File
# Begin Source File

SOURCE=..\TableTempo.h
# End Source File
# Begin Source File

SOURCE=..\Tarif.h
# End Source File
# Begin Source File

SOURCE=.\TarifAnnee.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\version.txt
# End Source File
# End Target
# End Project
