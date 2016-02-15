// ATPrintPDF.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ATPrintPDF.h"
#include "ATCloseWnd.h"
#include "MainFrm.h"
#include "ATPrintPDFDoc.h"
#include "ATPrintPDFView.h"
#include "ARPrintCmdLine.h"
#include "VersionApp.h"
// #include "PdfLib/utzadico.h"
#include "ATPRDICO.H"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFApp

BEGIN_MESSAGE_MAP(CATPrintPDFApp, CWinApp)
	//{{AFX_MSG_MAP(CATPrintPDFApp)
	ON_COMMAND(ID_ABOUTBOX, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_ABOUTBOX, OnUpdateAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFApp construction

CATPrintPDFApp::CATPrintPDFApp()
{
	m_ModePaysage = FALSE;
	JFCDraw::SetCodePage(1252);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CATPrintPDFApp object

CATPrintPDFApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFApp initialization

BOOL CATPrintPDFApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// on recupere la valeur du nuancier de gris
	this->SetRegistryKey(_T("JFC"));
	GestionCouleurs::SetContraste(this->GetProfileInt("Settings", "NuanceGris",    GestionCouleurs::GetContraste()));
	FontManager    ::SetEmbedding(this->GetProfileInt("Settings", "EmbeddingFont", FontManager    ::GetEmbedding()));

	//CString langage = GetProfileString("Configuration", "langue");
	char langage[64];
	(::GetPrivateProfileString("Inter", "Langue", "FR", langage, 64, ".\\ATAAETUD.INI"));

	char tampon[256];
	sprintf(tampon,"ATPRDICO.%s",langage);
	m_NumDico = InitDico(tampon);


//	sprintf(tampon,"%s",GetDico(m_NumDico,SD_EXCELACTIF));

	for (int No = 1; No <= 16; No +=1)
	{
		int j,n;
		int coul[3];
		char buffer[80],CPalette[180];
		sprintf(buffer,"Palette inactive%d",No);
		(::GetPrivateProfileString("Couleurs",buffer, "255,255,255", CPalette, 180, ".\\ATAAETUD.INI"));
		for (j=0; j<3; j++) coul[j]=255;
		n=strlen(CPalette);
		for (j=0; j<n; j++) if (CPalette[j]==',') CPalette[j]=' ';
		sscanf(CPalette,"%d %d %d",coul,coul+1,coul+2);
		GestionCouleurs::SetColor(100 + No, coul[0], coul[1],coul[2]);
	}

	int j,n;
	int coul_[3];
	char buffer[80],CPalette[180];

	sprintf(buffer,"Palette edition titre");
	(::GetPrivateProfileString("Couleurs",buffer, "160,160,160", CPalette, 180, ".\\ATAAETUD.INI"));
	for (j=0; j<3; j++) coul_[j]=160;
	n=strlen(CPalette);
	for (j=0; j<n; j++) if (CPalette[j]==',') CPalette[j]=' ';
	sscanf(CPalette,"%d %d %d",coul_,coul_+1,coul_+2);
	GestionCouleurs::SetColor(3, coul_[0], coul_[1],coul_[2]);
	GestionCouleurs::SetColor(31, coul_[0]+30*2, coul_[1]+30*2,coul_[2]+30*2);
	GestionCouleurs::SetColor(32, coul_[0]+30*1, coul_[1]+30*1,coul_[2]+30*1);
	GestionCouleurs::SetColor(33, coul_[0], coul_[1],coul_[2]);
	GestionCouleurs::SetColor(34, coul_[0]-30*1, coul_[1]-30*1,coul_[2]-30*1);
	GestionCouleurs::SetColor(35, coul_[0]-30*2, coul_[1]-30*2,coul_[2]-30*2);

	sprintf(buffer,"Palette edition corp");
	(::GetPrivateProfileString("Couleurs",buffer, "128,128,128", CPalette, 180, ".\\ATAAETUD.INI"));
	for (j=0; j<3; j++) coul_[j]=128;
	n=strlen(CPalette);
	for (j=0; j<n; j++) if (CPalette[j]==',') CPalette[j]=' ';
	sscanf(CPalette,"%d %d %d",coul_,coul_+1,coul_+2);
	GestionCouleurs::SetColor(1,  coul_[0], coul_[1],coul_[2]);
	GestionCouleurs::SetColor(11, coul_[0]+30*2, coul_[1]+30*2,coul_[2]+30*2);
	GestionCouleurs::SetColor(12, coul_[0]+30*1, coul_[1]+30*1,coul_[2]+30*1);
	GestionCouleurs::SetColor(13, coul_[0], coul_[1],coul_[2]);
	GestionCouleurs::SetColor(14, coul_[0]-30*1, coul_[1]-30*1,coul_[2]-30*1);
	GestionCouleurs::SetColor(15, coul_[0]-30*2, coul_[1]-30*2,coul_[2]-30*2);
	// on teste le langage
	if      (CString(langage) == "FR") JFCDraw::SetCodePage(1252);
	else if (CString(langage) == "PL") JFCDraw::SetCodePage(1250);
	else if (CString(langage) == "RU") JFCDraw::SetCodePage(1251);
	else JFCDraw::SetCodePage(1252);

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	if (FALSE == CATCloseWnd::RegisterClass()) return (FALSE);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CATPrintPDFDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CATPrintPDFView));
	AddDocTemplate(pDocTemplate);

	//_asm{int 3};
	// Parse command line for standard shell commands, DDE, file open
	CARPrintCmdLine cmdInfo;
	ParseCommandLine(cmdInfo);
	// Define printer orientation
	if      (cmdInfo.m_Orientation == 1) { this->SetPortrait(); m_ModePaysage = FALSE; }
	else if (cmdInfo.m_Orientation == 2) { this->SetPaysage();  m_ModePaysage = TRUE;  }

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo)) this->OnFileNew();

	// on teste le mode d'ouverture (document ou impression directe)
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FilePrint) return FALSE;

	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing) return FALSE;
	// on affiche la fenêtre et on la met à jour
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// on renvoie le code de succés
	return TRUE;
}

/////////////////////////////////////////////////////
// les fonctions pour modifier l'orientation courante

void CATPrintPDFApp::SetPaysage()
{
	PRINTDLG Infos;

	// on initialise la taille des infos
	Infos.lStructSize = (DWORD)sizeof(PRINTDLG);

	// on récupère la configuration courante
	if (this->GetPrinterDeviceDefaults(&Infos))
	{
		// on verrouille la configuration courante
		DEVMODE* pDevMode=(DEVMODE*)::GlobalLock(m_hDevMode);

		// on vérifie la validité du pointeur
		if (pDevMode != NULL)
		{
			// on change l'orientation
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;

			// on libère la configuration courante
			::GlobalUnlock(m_hDevMode);
		}
	}
}

void CATPrintPDFApp::SetPortrait()
{
	PRINTDLG Infos;

	// on initialise la taille des infos
	Infos.lStructSize = (DWORD)sizeof(PRINTDLG);

	// on récupère la configuration courante
	if (this->GetPrinterDeviceDefaults(&Infos))
	{
		// on verrouille la configuration courante
		DEVMODE* pDevMode=(DEVMODE*)::GlobalLock(m_hDevMode);

		// on vérifie la validité du pointeur
		if (pDevMode != NULL)
		{
			// on change l'orientation
			pDevMode->dmOrientation = DMORIENT_PORTRAIT;

			// on libère la configuration courante
			::GlobalUnlock(m_hDevMode);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFApp message handlers

int CATPrintPDFApp::ExitInstance()
{
	// on sauvegarde la valeur du nuancier de gris
	this->WriteProfileInt("Settings", "NuanceGris", GestionCouleurs::GetContraste());

	return CWinApp::ExitInstance();
}

void CATPrintPDFApp::OnAppAbout()
{
	CVersionApp aboutDlg;
	aboutDlg.DoModal();
}

void CATPrintPDFApp::OnUpdateAppAbout(CCmdUI* pCmdUI)
{

}
