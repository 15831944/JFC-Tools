// RepItemsBuddy.cpp : Définit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "RepItemsBuddyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRepItemsBuddyApp

BEGIN_MESSAGE_MAP(CRepItemsBuddyApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// construction CRepItemsBuddyApp

CRepItemsBuddyApp::CRepItemsBuddyApp()
{
	// TODO : ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CRepItemsBuddyApp

CRepItemsBuddyApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x8F51B7DE, 0x9DB7, 0x4B5E, { 0x97, 0x6C, 0xC, 0xB3, 0x18, 0xAA, 0x4A, 0x96 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// initialisation CRepItemsBuddyApp

BOOL CRepItemsBuddyApp::InitInstance()
{
	CWinApp::InitInstance();

	// Initialiser les bibliothèques OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalités et que vous souhaitez réduire la taille
	// de votre exécutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation spécifiques dont vous n'avez pas besoin.
	// Changez la clé de Registre sous laquelle nos paramètres sont enregistrés.
	// TODO : Modifiez cette chaîne avec des informations appropriées,
	// telles que le nom de votre société ou organisation


	// Analyser la ligne de commande pour rechercher les commutateurs automation ou reg/unreg.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	free((void*)m_pszAppName);
	//Change the name of the application file.
	//The CWinApp destructor will free the memory.
	m_pszAppName=_tcsdup(_T("AtelierInternet"));

	SetRegistryKey(_T("JFC"));
	
	// L'application a été démarrée avec le commutateur /Embedding ou /Automation.
	// Exécutez-la en tant que serveur Automation.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Inscrire les fabriques de classes via CoRegisterClassObject().
		COleTemplateServer::RegisterAll();
		
		CRepItemsBuddyDlg* dlg = new CRepItemsBuddyDlg();
		dlg->Create(CRepItemsBuddyDlg::IDD);
		m_pMainWnd = dlg;
		
		return TRUE;
	}
	// L'application a été lancée avec le commutateur /Unregserver ou /Unregister. Supprimez
	// les entrées dans la base de registres.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// L'application a été démarrée en mode autonome ou avec d'autres commutateurs (par exemple, /Register
	// ou /Regserver). Mettez à jour les entrées de la base de registres, y compris typelibrary.
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;
	}

	//On ne lance jamais l'appli sauf en deBug
#ifdef _DEBUG
	CRepItemsBuddyDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO : Placez ici le code définissant le comportement lorsque la boîte de dialogue est
		//  fermée avec OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO : Placez ici le code définissant le comportement lorsque la boîte de dialogue est
		//  fermée avec Annuler
	}
#endif

	// Lorsque la boîte de dialogue est fermée, retourner FALSE afin de quitter
	//  l'application, plutôt que de démarrer la pompe de messages de l'application.
	return FALSE;
}
