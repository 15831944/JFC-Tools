// Tomeval.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TomevalApp.h"
#include "TomevalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTomevalApp

BEGIN_MESSAGE_MAP(CTomevalApp, CWinApp)
	//{{AFX_MSG_MAP(CTomevalApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTomevalApp construction

//##ModelId=3D9AB7D40262
CTomevalApp::CTomevalApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTomevalApp object

CTomevalApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTomevalApp initialization

//##ModelId=3D9AB7D40276
BOOL CTomevalApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.



	// Lecture dans la ligne de commande de:
	// Num�ro de a fen�tre pilote
	// Nom du fichier Tomjob
	// Nom du fichier de r�sultat
	char Txt1[255],Txt2[255];
	m_hWndMaitre=0;
	long Nr;
	Txt1[0] = Txt2[0]=0;
	sscanf(m_lpCmdLine,"%d %s %s",&Nr,Txt1,Txt2);
	m_hWndMaitre=(HWND)Nr;
	if(strlen(Txt1)>0)m_NomFicTOMJOB=Txt1;
	else m_NomFicTOMJOB="";
	if(strlen(Txt2)>0)m_NomFicRESTOM=Txt2;
	else m_NomFicRESTOM="";


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTomevalDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
