# Microsoft Developer Studio Project File - Name="cinema" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cinema - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cinema.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cinema.mak" CFG="cinema - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cinema - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "cinema - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cinema - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GR /GX /O2 /I "..\composants" /I "UltimateGrid\Include" /D "..\composants" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /i "..\composants" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "cinema - Win32 Debug"

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
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\composants" /I "UltimateGrid\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /i "..\composants" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "cinema - Win32 Release"
# Name "cinema - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseCinema.cpp
# End Source File
# Begin Source File

SOURCE=.\CalculMemoDememo.cpp
# End Source File
# Begin Source File

SOURCE=.\CalculProduitFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlendrierHebdoCinema.cpp
# End Source File
# Begin Source File

SOURCE=.\Campagne.cpp
# End Source File
# Begin Source File

SOURCE=.\CampagneAgglo.cpp
# End Source File
# Begin Source File

SOURCE=.\CampagneFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\CampagneGenerique.cpp
# End Source File
# Begin Source File

SOURCE=.\CarreCouleur.cpp
# End Source File
# Begin Source File

SOURCE=.\CFichierCinema.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Cible.cpp
# End Source File
# Begin Source File

SOURCE=.\cinema.cpp
# End Source File
# Begin Source File

SOURCE=.\cinema.rc
# End Source File
# Begin Source File

SOURCE=.\cinemaDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\cinemaView.cpp
# End Source File
# Begin Source File

SOURCE=.\Couleur.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBeta.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBudgetNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChangerPeriode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChoix.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChxTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCritere.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportMediavision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportScreenvision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFonctionCible.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGraphe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGrapheFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMulticible.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalcul.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalculFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalculMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgParamPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPerfAgglo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPerformanceFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPerformances.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProduitFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRepCible.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelComplexe.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\FilmDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FilmView.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\Other\GradientProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCoeffModa.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCoeffUsure.cpp
# End Source File
# Begin Source File

SOURCE=.\GridVisuFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\Grille.cpp
# End Source File
# Begin Source File

SOURCE=.\Grille_Graphe.cpp
# End Source File
# Begin Source File

SOURCE=.\Grille_PerfAgglo.cpp
# End Source File
# Begin Source File

SOURCE=.\Grille_Performances.cpp
# End Source File
# Begin Source File

SOURCE=.\GrilleFilm_Graphe.cpp
# End Source File
# Begin Source File

SOURCE=.\GrilleFilm_Performances.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\JFCBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\JFCFileException.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBox_Complexe.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\MoveManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCUG.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Btn.cpp
# End Source File
# Begin Source File

SOURCE=.\Obj_Edit.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Gray.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_LBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Printer.cpp
# End Source File
# Begin Source File

SOURCE=.\Print.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProduitFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultatPerformance.cpp
# End Source File
# Begin Source File

SOURCE=.\Saisonnalite.cpp
# End Source File
# Begin Source File

SOURCE=.\Selection.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TableBAC.cpp
# End Source File
# Begin Source File

SOURCE=.\TableComplexe.cpp
# End Source File
# Begin Source File

SOURCE=.\TableFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\TableGroupeBAC.cpp
# End Source File
# Begin Source File

SOURCE=.\TableGroupeComplexe.cpp
# End Source File
# Begin Source File

SOURCE=.\TableNbEntreeComplexe.cpp
# End Source File
# Begin Source File

SOURCE=.\TableProduit.cpp
# End Source File
# Begin Source File

SOURCE=.\TableProduitFilm.cpp
# End Source File
# Begin Source File

SOURCE=.\TableTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\Tarif.cpp
# End Source File
# Begin Source File

SOURCE=.\Testgrid.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseCinema.h
# End Source File
# Begin Source File

SOURCE=.\CalculMemoDememo.h
# End Source File
# Begin Source File

SOURCE=.\CalculProduitFilm.h
# End Source File
# Begin Source File

SOURCE=.\CAlendrierHebdoCinema.h
# End Source File
# Begin Source File

SOURCE=.\Campagne.h
# End Source File
# Begin Source File

SOURCE=.\CampagneAgglo.h
# End Source File
# Begin Source File

SOURCE=.\CampagneFilm.h
# End Source File
# Begin Source File

SOURCE=.\CampagneGenerique.h
# End Source File
# Begin Source File

SOURCE=.\CarreCouleur.h
# End Source File
# Begin Source File

SOURCE=".\AccèsDonnées\CFichierCinema.h"
# End Source File
# Begin Source File

SOURCE=.\CFichierCinema.h
# End Source File
# Begin Source File

SOURCE=.\ChildFilm.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Cible.h
# End Source File
# Begin Source File

SOURCE=.\cinema.h
# End Source File
# Begin Source File

SOURCE=.\cinemaDoc.h
# End Source File
# Begin Source File

SOURCE=.\cinemaView.h
# End Source File
# Begin Source File

SOURCE=.\Couleur.h
# End Source File
# Begin Source File

SOURCE=.\DlgBeta.h
# End Source File
# Begin Source File

SOURCE=.\DlgBudgetNet.h
# End Source File
# Begin Source File

SOURCE=.\DlgChangerPeriode.h
# End Source File
# Begin Source File

SOURCE=.\DlgChoix.h
# End Source File
# Begin Source File

SOURCE=.\DlgChxTempo.h
# End Source File
# Begin Source File

SOURCE=.\DlgCritere.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportMediavision.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportScreenvision.h
# End Source File
# Begin Source File

SOURCE=.\DlgFonctionCible.h
# End Source File
# Begin Source File

SOURCE=.\DlgFormat.h
# End Source File
# Begin Source File

SOURCE=.\DlgGraphe.h
# End Source File
# Begin Source File

SOURCE=.\DlgGrapheFilm.h
# End Source File
# Begin Source File

SOURCE=.\DlgMulticible.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalcul.h
# End Source File
# Begin Source File

SOURCE="E:\Documents and Settings\climbfly\Bureau\AtelierCinema\DlgOptionCalcul.h"
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalculFilm.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionCalculMemo.h
# End Source File
# Begin Source File

SOURCE=.\DlgParamPrint.h
# End Source File
# Begin Source File

SOURCE=.\DlgPerfAgglo.h
# End Source File
# Begin Source File

SOURCE=.\DlgPerformanceFilm.h
# End Source File
# Begin Source File

SOURCE=.\DlgPerformances.h
# End Source File
# Begin Source File

SOURCE=.\DlgProduitFilm.h
# End Source File
# Begin Source File

SOURCE=.\DlgRepCible.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelComplexe.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelection.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectionProduit.h
# End Source File
# Begin Source File

SOURCE=.\FilmDoc.h
# End Source File
# Begin Source File

SOURCE=.\FilmView.h
# End Source File
# Begin Source File

SOURCE=..\Composants\Other\GradientProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCoeffModa.h
# End Source File
# Begin Source File

SOURCE=.\GridCoeffUsure.h
# End Source File
# Begin Source File

SOURCE=.\GridVisuFilm.h
# End Source File
# Begin Source File

SOURCE=.\Grille.h
# End Source File
# Begin Source File

SOURCE=.\Grille_Graphe.h
# End Source File
# Begin Source File

SOURCE=.\Grille_PerfAgglo.h
# End Source File
# Begin Source File

SOURCE=.\Grille_Performances.h
# End Source File
# Begin Source File

SOURCE=.\GrilleFilm_Graphe.h
# End Source File
# Begin Source File

SOURCE=.\GrilleFilm_Performances.h
# End Source File
# Begin Source File

SOURCE=..\Composants\JFCBitmap.h
# End Source File
# Begin Source File

SOURCE=..\Composants\JFCFileException.h
# End Source File
# Begin Source File

SOURCE=.\ListBox_Complexe.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxSelect.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\Composants\MoveManager.h
# End Source File
# Begin Source File

SOURCE=.\MyCUG.h
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Btn.h
# End Source File
# Begin Source File

SOURCE=.\Obj_Edit.h
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Gray.h
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_LBox.h
# End Source File
# Begin Source File

SOURCE=..\Composants\Obj_Printer.h
# End Source File
# Begin Source File

SOURCE=.\Print.h
# End Source File
# Begin Source File

SOURCE=.\PrintFilm.h
# End Source File
# Begin Source File

SOURCE=.\ProduitFilm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultatPerformance.h
# End Source File
# Begin Source File

SOURCE=.\Saisonnalite.h
# End Source File
# Begin Source File

SOURCE=.\Selection.h
# End Source File
# Begin Source File

SOURCE=.\SelectionProduit.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Table.h
# End Source File
# Begin Source File

SOURCE=.\TableBAC.h
# End Source File
# Begin Source File

SOURCE=.\TableComplexe.h
# End Source File
# Begin Source File

SOURCE=.\TableFormat.h
# End Source File
# Begin Source File

SOURCE=.\TableNbEntreeComplexe.h
# End Source File
# Begin Source File

SOURCE=.\TableProduit.h
# End Source File
# Begin Source File

SOURCE=.\TableRegie.h
# End Source File
# Begin Source File

SOURCE=.\TableTempo.h
# End Source File
# Begin Source File

SOURCE=.\Tarif.h
# End Source File
# Begin Source File

SOURCE=.\Testgrid.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Acinema.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Ajoutsup.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Blocchan.cur
# End Source File
# Begin Source File

SOURCE=.\res\Bloccopi.cur
# End Source File
# Begin Source File

SOURCE=.\res\Blocdepl.cur
# End Source File
# Begin Source File

SOURCE=.\res\Bloceffa.cur
# End Source File
# Begin Source File

SOURCE=.\res\Blocmove.cur
# End Source File
# Begin Source File

SOURCE=.\res\Bloctemp.cur
# End Source File
# Begin Source File

SOURCE=.\res\Cible.ico
# End Source File
# Begin Source File

SOURCE=.\res\cinema.ico
# End Source File
# Begin Source File

SOURCE=.\res\cinema.rc2
# End Source File
# Begin Source File

SOURCE=.\res\cinemaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Deplacem.cur
# End Source File
# Begin Source File

SOURCE=.\res\Fleche.ico
# End Source File
# Begin Source File

SOURCE=.\res\Fleche2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Glisseme.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\Interdit.cur
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Logo2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Logo3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\NewLogoJFC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Passage.cur
# End Source File
# Begin Source File

SOURCE=.\res\pattern2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pattern8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Rmb.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Splash Cinéma.bmp"
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarn.bmp
# End Source File
# End Group
# Begin Group "UltimateGrid"

# PROP Default_Filter "cpp;h"
# Begin Group "Include"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\UltimateGrid\Include\UG64Bit.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGCBType.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGCell.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugceltyp.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGCnrBtn.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGCTarrw.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGCtrl.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugdefine.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGDLType.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGDrgDrp.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGDrwHnt.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGDtaSrc.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGEdit.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGEditBase.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugformat.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\uggdinfo.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGGrid.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGHint.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ughscrol.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\uglstbox.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGMEdit.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGMemMan.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGMultiS.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugprint.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugptrlst.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugsidehd.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGStrOp.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugstruct.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugtab.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\UGTopHdg.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugvscrol.h
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Include\ugxpthemes.h
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCBType.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCell.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCelTyp.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCnrBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCTarrw.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGDLType.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGDrgDrp.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGDrwHnt.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGDtaSrc.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGEditBase.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugformat.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\uggdinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGHint.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ughscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugLstBox.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGMEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGMemMan.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGMultiS.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugprint.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugptrlst.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugsidehd.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGStrOp.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugtab.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGTopHdg.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\ugvscrol.cpp
# End Source File
# Begin Source File

SOURCE=.\UltimateGrid\Source\UGXPThemes.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\installation.txt
# End Source File
# Begin Source File

SOURCE=.\version.txt
# End Source File
# End Target
# End Project
# Section cinema : {398C5F21-6AB2-11D3-989D-A234B2A6DBB9}
# 	1:19:IDD_CHANGER_PERIODE:102
# 	2:16:Resource Include:resource.h
# 	2:19:DlgChangerPeriode.h:DlgChangerPeriode.h
# 	2:25:CLASS: CDlgChangerPeriode:CDlgChangerPeriode
# 	2:19:IDD_CHANGER_PERIODE:IDD_CHANGER_PERIODE
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:cinema.h
# 	2:21:DlgChangerPeriode.cpp:DlgChangerPeriode.cpp
# End Section
# Section cinema : {C1841480-2CD2-11D4-989E-004095420987}
# 	2:9:Print.cpp:Print.cpp
# 	2:7:Print.h:Print.h
# 	2:19:Application Include:cinema.h
# 	2:13:CLASS: CPrint:CPrint
# End Section
# Section cinema : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:103
# 	2:21:SplashScreenInsertKey:4.0
# End Section
