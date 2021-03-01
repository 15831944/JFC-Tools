// Atelier Presse.cpp : Définit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "Atelier Presse.h"
#include "Atelier PresseDlg.h"

#include "JFCSplash.h"
#include "ATPComm.h"
#include "JFCConfig.h"
#include "FormatStr.h"

#include "ItemsConstruitsManager.h"
#include "XTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAtelierPresseApp

BEGIN_MESSAGE_MAP(CAtelierPresseApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// construction CAtelierPresseApp

CAtelierPresseApp::CAtelierPresseApp()
{
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CAtelierPresseApp

CAtelierPresseApp theApp;


// initialisation CAtelierPresseApp

BOOL CAtelierPresseApp::InitInstance()
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

	//on traite la ligne de commande
	CATPComm cmdInfo;
	ParseCommandLine(cmdInfo);
	if(cmdInfo.m_dico == CATPComm::ANG)
	{
		//First free the string allocated by MFC at CWinApp startup.
		//The string is allocated before InitInstance is called.
		free((void*)m_pszAppName);
		//Change the name of the application file.
		//The CWinApp destructor will free the memory.
		m_pszAppName=_tcsdup(_T("AtelierPresse ANG"));
	}

	this->SetRegistryKey(_T("JFC"));

	//On charge la couleur utilisée
	CString path = GetProfileString("", "InstallPath", 0);
	// premiere execution
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
	// premiere execution
	if (path =="")
	{
		GetCurrentDirectory(2048, curDir);
		CString path = curDir;
		path.Append("\\Sources");
		WriteProfileString("Data", "SourcesPath", path);
	}
	SetCurrentDirectory(path);

	path = GetProfileString("Data", "TarifsPath", 0);
	// premiere execution
	if (path =="")
	{
		GetCurrentDirectory(2048, curDir);
		CString path = curDir;
		path.Append("\\Tarifs");
		WriteProfileString("Data", "TarifsPath", path);
	}

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

	if (!JFCConfig::GetInstance().CanStart())
	{
		AfxMessageBox("Fichier de configuration client invalide !", MB_ICONERROR);
		return FALSE;
	}

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

	// on cree la fenetre mère
	CAtelierPresseDlg dlg;
	m_pMainWnd = &dlg;

	// on passe les options au dialogue
	dlg.SetCmdInfo(&cmdInfo);

	// on affiche le splash
	/* A REMETTRE
	JFCSplash *pSplash=new JFCSplash();
	//pas besoin de delete, fait dans la classe
	pSplash->Create(m_pMainWnd, NULL, 0, CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW );
	pSplash->SetBitmap(IDB_ATP_SPLASH,255,0,255);
	pSplash->SetFadeInTime(2000);		
	pSplash->SetFadeOutTime(2000);		
		
	if (cmdInfo.m_bShowSplash)
		pSplash->Show();

    */		

	dlg.LoadApps();

	// A REMETTRE pSplash->Hide();

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

/////////////////////////////////////////////////////////////////////////////////////////
// On sort de l'application
int CAtelierPresseApp::ExitInstance()
{
	// on détruit le manager des Items Construits
	CItemsConstruitsManager::Destroy();

	// on détruit le manager des templates (styles)
	CXTemplateManager::Destroy();

	return CWinApp::ExitInstance();
}
