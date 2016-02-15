// Nomenclature.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Nomenclature.h"

#include "MainFrm.h"
#include "NomenclatureDoc.h"
#include "NomenclatureView.h"
#include "FormulaireInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CNomenclatureApp

BEGIN_MESSAGE_MAP(CNomenclatureApp, CWinApp)
	//{{AFX_MSG_MAP(CNomenclatureApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_INFOS_APROPOSDENOMENCLATURE, OnInfosAproposdenomenclature)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureApp construction

CNomenclatureApp::CNomenclatureApp()
{
	// Info Société Production Soft
	m_PrintLogo="jfc.bmp";
	m_PrintRaison="JFC Informatique & Média - Tour d'Asnières - 4 av. Laurent Cely - Tél.: 01.46.88.28.10 - Fax: 01.46.88.28.12";
	m_PrintUser="Id.";
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNomenclatureApp object

CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureApp initialization

BOOL CNomenclatureApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Détermine localité pour date
	setlocale( LC_ALL, "English" );

	// Recherche du chemin nomenclature
	SetRegistryKey("JFC");
	theApp.m_PathNomencl = GetProfileString("Configuration","DataPath","");
	
		
	// Recherche du chemin ArtRadio
	//theApp.m_PathArtRadio = GetRegData("Software\\JFC\\Atelier Radio\\Configuration", "ArtRadioPath");
	theApp.m_PathArtRadio = GetRegData("Software\\JFC\\Nomenclature\\Configuration", "NomenclPath");

	// Recherche du chemin ArtTele
	theApp.m_PathArtTele = GetRegData("Software\\JFC\\Atelier Tele\\Configuration", "ArtTelePath");
	
	// Nom utilisateur
	theApp.m_PrintUser = GetProfileString("Configuration","UserName","Jfc");
	
	// Niveau Acces Nomenclature (voir base registe DataAcces) / par défaut minimum
	theApp.m_NiveauAcces = atoi(GetProfileString("Configuration","DataAcces","1")); 

	if(theApp.m_NiveauAcces != 1 && !m_Lock.LockFile(theApp.m_PathNomencl +"acces.lck"))
		return FALSE;

	// Niveau Acces pour Informations Tiers EDI (par défaut Acces sans obligation)
	// Rappel >> 0 : Informations EDI non accessibles
	//           1 : ---------------- accessibles mais non obligatoires
	//           2 : ---------------- accessibles et certains champs obligatoires
	theApp.m_NiveauEDI = atoi(GetProfileString("Configuration","EDIAcces","1")); 

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNomenclatureDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNomenclatureView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->CenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CNomenclatureApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureApp message handlers


void CNomenclatureApp::OnInfosAproposdenomenclature() 
{
	// Menu Info Application
	CFormulaireInfo FormulaireInfo;
	FormulaireInfo.DoModal();
}

CString CNomenclatureApp::GetRegData(CString key, CString value)
{
	HKEY hSubKey;
	char buff[2048];
	DWORD lpType = REG_EXPAND_SZ;
	ULONG ulSize = sizeof(buff);
	
	RegOpenKeyEx(HKEY_CURRENT_USER,key,0,KEY_READ, &hSubKey);
	RegQueryValueEx(hSubKey,value,0, &lpType,(unsigned char*) buff, &ulSize);
	RegCloseKey(hSubKey);	CString csRet;
	csRet.Format ("%s",buff);
	return csRet;
}

// Cryptage mot de passe utilisateur
CString CodageMP(CString MotDePasse)
{
	CString CodeMP;
	CHAR Code[6];

	// Cryptage mot de passe
	for (int i=0;i<MotDePasse.GetLength();i++)
	{
		if (i >= 6) break;
		Code[i]= MotDePasse[i];    //   .GetAt(i);
	}

	for (int i=0;i<MotDePasse.GetLength();i++)
	{
		//Code[i] = Code[i] + ((i+1)*5);
		Code[i] = 255 - Code[i]- ((i+1)*2);
	}

	CodeMP = Code;
	CodeMP = CodeMP.Left(MotDePasse.GetLength());
	
	return CodeMP;

}

// Decryptage mot de passe utilisateur
CString DecodageMP(CString MotCode)
{
	CString MotDePasse;
	CHAR Code[6];

	// Decryptage mot de passe
	for (int i=0;i<MotCode.GetLength();i++)
	{
		if (i >= 6) break;
		Code[i]= MotCode[i];    //   .GetAt(i);
	}

	for (int i=0;i<MotCode.GetLength();i++)
	{
		Code[i] = 255 - Code[i]- ((i+1)*2);
	}

	MotDePasse = Code;
	MotDePasse = MotDePasse.Left(MotCode.GetLength());
	
	return MotDePasse;

}

