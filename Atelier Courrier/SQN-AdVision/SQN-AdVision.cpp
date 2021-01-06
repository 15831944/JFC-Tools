// SQNAdVision.cpp : Définit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "SQN-AdVision.h"
#include "SQN-AdVisionDlg.h"
#include "FormatStr.h"

#include "JFCConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSQNAdVisionApp

BEGIN_MESSAGE_MAP(CSQNAdVisionApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// construction CSQNAdVisionApp

CSQNAdVisionApp::CSQNAdVisionApp()
{
	// Placez toutes les initialisations significatives dans InitInstance

	// on definit build_for_sqn
	JFCConfig::SetBuildForSQN();
}


// Seul et unique objet CSQNAdVisionApp

CSQNAdVisionApp theApp;


// initialisation CSQNAdVisionApp

BOOL CSQNAdVisionApp::InitInstance()
{
	// InitCommonControls() est requis sur Windows XP si le manifeste de l'application
	// spécifie l'utilisation de ComCtl32.dll version 6 ou ultérieure pour activer les
	// styles visuels.  Dans le cas contraire, la création de fenêtres échouera.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// on enregistre la classe de fenêtre des contrôles JFC
	if (!JFControl::RegisterClass())
		return (FALSE);

	GXInit();
	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalités et que vous souhaitez réduire la taille
	// de votre exécutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation spécifiques dont vous n'avez pas besoin.
	// Changez la clé de Registre sous laquelle nos paramètres sont enregistrés.
	// telles que le nom de votre société ou organisation
	this->SetRegistryKey(_T("JFC"));

	//on traite la ligne de commande
	CATPComm cmdInfo;
	ParseCommandLine(cmdInfo);


	//On charge la couleur utilisée
	CString path = GetProfileString("", "InstallPath", 0);
	//premiere execution
	char * curDir;
	curDir = new char[2048];
	if (path =="")
	{
		GetCurrentDirectory(2048, curDir);
		CString path = curDir;
		WriteProfileString("", "InstallPath", path);
	}

	//on charge les dossiers de travail
	path = GetProfileString("Data", "SourcesPath", 0);
	//premiere execution
	if (path =="")
	{
		GetCurrentDirectory(2048, curDir);
		CString path = curDir;
		path.Append("\\Sources");
		WriteProfileString("Data", "SourcesPath", path);
	}
	SetCurrentDirectory(path);



	path = GetProfileString("Data", "UserPath", 0);
	//premiere execution
	if (path =="")
	{
		GetCurrentDirectory(2048, curDir);
		CString path = curDir;
		path.Append("\\User");
		WriteProfileString("Data", "UserPath", path);
	}
	delete curDir;

	// on définit la localité sur celle du système
	if (0 == setlocale(LC_ALL, ""))	return (FALSE);

	// on récupère les paramètres de la localité
	struct lconv* pLocale = localeconv(); if (pLocale == 0) return (FALSE);

	// on définit le séparateur décimal
	if (!CFormatStr::SetDecimalPoint(*pLocale->decimal_point))
	{
		AfxMessageBox("Le séparateur décimal du système est invalide.", MB_ICONERROR);
		return (FALSE);
	}

	//on cree la fenetre mère
	CSQNAdVisionDlg dlg;
	m_pMainWnd = &dlg;

	//on passe les options au dialogue
	dlg.SetCmdInfo(&cmdInfo);

	//on affiche le splash
		JFCSplash *pSplash=new JFCSplash();
		//pas besoin de delete, fait dans la classe
		pSplash->Create(m_pMainWnd, NULL, 0, CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW );
		pSplash->SetBitmap(IDB_ATP_SPLASH,255,0,255);
		pSplash->SetFadeInTime(2000);		
		pSplash->SetFadeOutTime(2000);		
		
		//pSplash->SetText("Version RC_5 25/04/2003");
		//pSplash->SetTextFont("Monotype Corsiva",250,CSS_TEXT_ITALIC|CSS_TEXT_BOLD);
		//POINT pt = {10,-10};
		//SIZE sz = {1000,1000};
		//pSplash->SetTextRect(CRect(pt,sz));
		//pSplash->SetTextColor(RGB(0,74,132));
		//pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER);

	if (cmdInfo.m_bShowSplash)
		pSplash->Show();

	dlg.LoadApps();

	pSplash->Hide();


	//et c parti!!!
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Lorsque la boîte de dialogue est fermée, retourner FALSE afin de quitter
	//  l'application, plutôt que de démarrer la pompe de messages de l'application.
	return FALSE;
}
