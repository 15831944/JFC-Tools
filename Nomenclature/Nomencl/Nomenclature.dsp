# Microsoft Developer Studio Project File - Name="Nomenclature" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Nomenclature - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nomenclature.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nomenclature.mak" CFG="Nomenclature - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nomenclature - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Nomenclature - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nomenclature - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "c:\serveur_jfc2\composants\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /i "c:\serveur_jfc2\composants\\" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/Nomencl.exe"

!ELSEIF  "$(CFG)" == "Nomenclature - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "c:\Serveur_Jfc2\composants\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /i "c:\serveur_jfc2\composants\\" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Nomenclature - Win32 Release"
# Name "Nomenclature - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DialogNvlleAnnee.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTiersEDI.cpp
# End Source File
# Begin Source File

SOURCE=.\Formulaire.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireEDI.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireGroupe.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireImpression.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireNom.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireStandard.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireTauxFO.cpp
# End Source File
# Begin Source File

SOURCE=.\FormulaireUtilTaux.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\JFCBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\LockAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\MoveManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Noeud.cpp
# End Source File
# Begin Source File

SOURCE=.\Nomenclature.cpp
# End Source File
# Begin Source File

SOURCE=.\Nomenclature.rc
# End Source File
# Begin Source File

SOURCE=.\NomenclatureDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NomenclatureView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\Obj_Gray.cpp
# End Source File
# Begin Source File

SOURCE=.\Obj_Printer.cpp
# End Source File
# Begin Source File

SOURCE=.\Obj_Printer.h
# End Source File
# Begin Source File

SOURCE=.\Printer.cpp
# End Source File
# Begin Source File

SOURCE=.\PWFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlX.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DialogNvlleAnnee.h
# End Source File
# Begin Source File

SOURCE=.\DialogTiersEDI.h
# End Source File
# Begin Source File

SOURCE=.\Formulaire.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireEDI.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireGroupe.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireImpression.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireInfo.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireNom.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireStandard.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireTauxFO.h
# End Source File
# Begin Source File

SOURCE=.\FormulaireUtilTaux.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\JFCBitmap.h
# End Source File
# Begin Source File

SOURCE=.\LockAccess.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\MoveManager.h
# End Source File
# Begin Source File

SOURCE=.\Noeud.h
# End Source File
# Begin Source File

SOURCE=.\Nomenclature.h
# End Source File
# Begin Source File

SOURCE=.\NomenclatureDoc.h
# End Source File
# Begin Source File

SOURCE=.\NomenclatureView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Serveur_jfc2\Composants\Obj_Gray.h
# End Source File
# Begin Source File

SOURCE=.\Printer.h
# End Source File
# Begin Source File

SOURCE=.\PWFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlX.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ItemImage16.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\LogoJfc.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\LogoJfc2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Nomenclature.ico
# End Source File
# Begin Source File

SOURCE=.\res\Nomenclature.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NomenclatureDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Nomenclature : {442B9C3E-8A92-4615-B6CB-47F274B57C4C}
# 	1:29:IDD_DIALOG_FORMULAIRE_PRODUIT:104
# 	2:16:Resource Include:resource.h
# 	2:29:IDD_DIALOG_FORMULAIRE_PRODUIT:IDD_DIALOG_FORMULAIRE_PRODUIT
# 	2:12:Formulaire.h:Formulaire.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:Nomenclature.h
# 	2:14:Formulaire.cpp:Formulaire.cpp
# 	2:18:CLASS: CFormulaire:CFormulaire
# End Section
# Section Nomenclature : {A5BDEC32-B9ED-4D09-93BB-95499C25C719}
# 	1:25:IDD_DIALOG_FORMULAIRE_NOM:102
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CFormulaireNom:CFormulaireNom
# 	2:10:ENUM: enum:enum
# 	2:15:FormulaireNom.h:FormulaireNom.h
# 	2:19:Application Include:Nomenclature.h
# 	2:25:IDD_DIALOG_FORMULAIRE_NOM:IDD_DIALOG_FORMULAIRE_NOM
# 	2:17:FormulaireNom.cpp:FormulaireNom.cpp
# End Section
# Section Nomenclature : {8D473E67-CA0D-444A-B609-BCFD2751A5A1}
# 	1:30:IDD_DIALOG_FORMULAIRE_STANDARD:103
# 	2:16:Resource Include:resource.h
# 	2:30:IDD_DIALOG_FORMULAIRE_STANDARD:IDD_DIALOG_FORMULAIRE_STANDARD
# 	2:20:FormulaireStandard.h:FormulaireStandard.h
# 	2:26:CLASS: CFormulaireStandard:CFormulaireStandard
# 	2:10:ENUM: enum:enum
# 	2:22:FormulaireStandard.cpp:FormulaireStandard.cpp
# 	2:19:Application Include:Nomenclature.h
# End Section
# Section Nomenclature : {1C94C8D7-8EF5-4B7C-B0ED-A9E4936716AA}
# 	2:18:CLASS: CLockAccess:CLockAccess
# 	2:14:LockAccess.cpp:LockAccess.cpp
# 	2:12:LockAccess.h:LockAccess.h
# 	2:19:Application Include:Nomenclature.h
# End Section
# Section Nomenclature : {10BF6318-DB4D-4215-A45C-D4327646A43D}
# 	2:7:Noeud.h:Noeud.h
# 	2:9:Noeud.cpp:Noeud.cpp
# 	2:13:CLASS: CNoeud:CNoeud
# 	2:19:Application Include:Nomenclature.h
# End Section
# Section Nomenclature : {2592EB93-CECA-464A-AC4F-E65A3C9DD55E}
# 	2:17:CLASS: CTreeCtrlX:CTreeCtrlX
# 	2:11:TreeCtrlX.h:TreeCtrlX.h
# 	2:13:TreeCtrlX.cpp:TreeCtrlX.cpp
# 	2:19:Application Include:Nomenclature.h
# End Section
# Section Nomenclature : {8D34F712-285C-42CD-A629-E07BC9F394C8}
# 	2:8:PWFile.h:PWFile.h
# 	2:14:CLASS: CPWFile:CPWFile
# 	2:10:PWFile.cpp:PWFile.cpp
# 	2:19:Application Include:Nomenclature.h
# End Section
