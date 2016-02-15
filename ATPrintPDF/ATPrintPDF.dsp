# Microsoft Developer Studio Project File - Name="ATPrintPDF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ATPrintPDF - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ATPrintPDF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATPrintPDF.mak" CFG="ATPrintPDF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATPrintPDF - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ATPrintPDF - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ATPrintPDF - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../PdfLib/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 pdflib.lib utzadico.lib /nologo /subsystem:windows /machine:I386 /out:"./Bin/arzaprnt.exe" /libpath:"./PdfLib/"

!ELSEIF  "$(CFG)" == "ATPrintPDF - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../PdfLib/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 pdflib.lib utzadico.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Bin/arzaprnt.exe" /libpath:"./PdfLib/"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "ATPrintPDF - Win32 Release"
# Name "ATPrintPDF - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARML\ARMLAdress.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLContext.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLField.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLHStroke.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLLineField.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLLogo.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLState.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateBegin.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateBody.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateError.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateFooter.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTabCell.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTabpave.cpp
# End Source File
# Begin Source File

SOURCE=.\ARPrintCmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ATCloseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDF.cpp
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDF.rc
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDFDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDFView.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelProgressSplash.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\GestionCouleurs.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCDecimilToUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawAdress.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBeginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBeginPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBody.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawComposite.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawEndPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawEndPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawFooter.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawLineTo.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawLogo.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawMoveTo.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetFillColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetLineWidth.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawText.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCGDIDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCPDFDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PageInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw\PoliceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VersionApp.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewportWindow.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARML\ARMLAdress.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLContext.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLField.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLFrame.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLHStroke.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLLineField.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLLogo.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLState.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateBegin.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateBody.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateEnd.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateError.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateFooter.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateFrame.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateHeader.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLStateTable.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTabCell.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTable.h
# End Source File
# Begin Source File

SOURCE=.\ARML\ARMLTabPave.h
# End Source File
# Begin Source File

SOURCE=.\ARPrintCmdLine.h
# End Source File
# Begin Source File

SOURCE=.\ATCloseWnd.h
# End Source File
# Begin Source File

SOURCE=.\ATPRDICO.H
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDF.h
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDFDoc.h
# End Source File
# Begin Source File

SOURCE=.\ATPrintPDFView.h
# End Source File
# Begin Source File

SOURCE=.\ExcelProgressSplash.h
# End Source File
# Begin Source File

SOURCE=.\Draw\GestionCouleurs.h
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCBitmap.h
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCDecimilToUnit.h
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCDraw.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawAdress.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBeginPage.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBeginPath.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawBody.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawComponent.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawComposite.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawDocument.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawEndPage.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawEndPath.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawFooter.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawFrame.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawHeader.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawLineTo.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawLogo.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawMoveTo.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawPage.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawPicture.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetColor.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetFillColor.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetFont.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawSetLineWidth.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawTableau.h
# End Source File
# Begin Source File

SOURCE=.\Composite\JFCDrawText.h
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCGDIDraw.h
# End Source File
# Begin Source File

SOURCE=.\Draw\JFCPDFDraw.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PageInfo.h
# End Source File
# Begin Source File

SOURCE=.\Draw\PoliceManager.h
# End Source File
# Begin Source File

SOURCE=.\PreviewBar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VersionApp.h
# End Source File
# Begin Source File

SOURCE=.\ViewportWindow.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ATPrintPDF.ico
# End Source File
# Begin Source File

SOURCE=.\res\ATPrintPDF.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ATPrintPDFDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\preview_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
