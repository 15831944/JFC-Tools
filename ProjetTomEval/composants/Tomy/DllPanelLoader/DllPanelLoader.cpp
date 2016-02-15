// DllPanelLoader.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DllPanelLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDllPanelLoaderApp

BEGIN_MESSAGE_MAP(CDllPanelLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CDllPanelLoaderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllPanelLoaderApp construction

CDllPanelLoaderApp::CDllPanelLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDllPanelLoaderApp object

CDllPanelLoaderApp thePanelLoaderDLL;
#include "PanelLoader.h"
#pragma  data_seg(".SharedData")
CPanelLoader * SharedPanelLoader=NULL;
int NbInstance=0;
#pragma data_seg()


DllExport CPanelLoader * GetPtrPanelLoader()
{
	return(SharedPanelLoader);
}

BOOL CDllPanelLoaderApp::InitInstance() 
{
	if(SharedPanelLoader==NULL)
		SharedPanelLoader=new CPanelLoader;
	NbInstance++;
	
	return CWinApp::InitInstance();
}

int CDllPanelLoaderApp::ExitInstance() 
{
	NbInstance--;
	if(NbInstance==0 && SharedPanelLoader)
	{
		delete SharedPanelLoader;
		SharedPanelLoader=NULL;
	}
	
	return CWinApp::ExitInstance();
}
