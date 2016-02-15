; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CExcelProgressSplash
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "atprintpdf.h"
LastPage=0

ClassCount=9
Class1=CATCloseWnd
Class2=CATPrintPDFApp
Class3=CATPrintPDFDoc
Class4=CATPrintPDFView
Class5=CMainFrame
Class6=CPreviewBar
Class7=CVersionApp
Class8=CViewportWindow

ResourceCount=7
Resource1=IDD_ABOUTAPP
Resource2=IDR_MAINFRAME (French (France))
Resource3=IDD_ETUDE_EXEL_SPLASH
Resource4=IDD_ABOUTAPP (English (U.S.))
Resource5=IDD_ETUDE_EXEL_SPLASH (English (U.S.) - IDD_ETUDE_EXEL_SPLASH)
Resource6=IDD_EXCEL_SPLASH
Class9=CExcelProgressSplash
Resource7=IDR_MAINFRAME

[CLS:CATCloseWnd]
Type=0
BaseClass=CWnd
HeaderFile=ATCloseWnd.h
ImplementationFile=ATCloseWnd.cpp
LastObject=CATCloseWnd

[CLS:CATPrintPDFApp]
Type=0
BaseClass=CWinApp
HeaderFile=ATPrintPDF.h
ImplementationFile=ATPrintPDF.cpp

[CLS:CATPrintPDFDoc]
Type=0
BaseClass=CDocument
HeaderFile=ATPrintPDFDoc.h
ImplementationFile=ATPrintPDFDoc.cpp

[CLS:CATPrintPDFView]
Type=0
BaseClass=CView
HeaderFile=ATPrintPDFView.h
ImplementationFile=ATPrintPDFView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CATPrintPDFView

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CPreviewBar]
Type=0
BaseClass=CToolBar
HeaderFile=PreviewBar.h
ImplementationFile=PreviewBar.cpp

[CLS:CVersionApp]
Type=0
BaseClass=CDialog
HeaderFile=VersionApp.h
ImplementationFile=VersionApp.cpp

[CLS:CViewportWindow]
Type=0
BaseClass=CWnd
HeaderFile=ViewportWindow.h
ImplementationFile=ViewportWindow.cpp

[DLG:IDD_ABOUTAPP]
Type=1
Class=CVersionApp
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME (French (France))]
Type=1
Class=?
CommandCount=0

[DLG:IDD_ABOUTAPP (English (U.S.))]
Type=1
Class=CVersionApp
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=?
CommandCount=0

[DLG:IDD_ETUDE_EXEL_SPLASH]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,static,1342177287
Control2=IDC_CUSTOM1,,1342242816

[DLG:IDD_ETUDE_EXEL_SPLASH (English (U.S.) - IDD_ETUDE_EXEL_SPLASH)]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_EXCEL_SPLASH]
Type=1
Class=CExcelProgressSplash
ControlCount=3
Control1=IDB_BITMAP,static,1342177294
Control2=IDC_PROGRESS_TITLE,static,1342308352
Control3=IDC_PROGRESS,msctls_progress32,1350565888

[CLS:CExcelProgressSplash]
Type=0
HeaderFile=ExcelProgressSplash.h
ImplementationFile=ExcelProgressSplash.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CExcelProgressSplash

