// Atelier Presse.cpp : D�finit les comportements de classe pour l'application.
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
	// InitCommonControls() est requis sur Windows�XP si le manifeste de l'application
	// sp�cifie l'utilisation de ComCtl32.dll version�6 ou ult�rieure pour activer les
	// styles visuels.  Dans le cas contraire, la cr�ation de fen�tres �chouera.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// on enregistre la classe de fen�tre des contr�les JFC
	if (!JFControl::RegisterClass())
		return (FALSE);

	GXInit();
	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalit�s et que vous souhaitez r�duire la taille
	// de votre ex�cutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation sp�cifiques dont vous n'avez pas besoin.
	// Changez la cl� de Registre sous laquelle nos param�tres sont enregistr�s.
	// telles que le nom de votre soci�t� ou organisation

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

	//On charge la couleur utilis�e
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

	// on d�finit la localit� sur celle du syst�me
	if (0 == setlocale(LC_ALL, ""))	return (FALSE);

	// on r�cup�re les param�tres de la localit�
	struct lconv* pLocale = localeconv(); if (pLocale == 0) return (FALSE);

	// on d�finit le s�parateur d�cimal
	if (!CFormatStr::SetDecimalPoint(*pLocale->decimal_point))
	{
		AfxMessageBox("Le s�parateur d�cimal du syst�me est invalide.", MB_ICONERROR);
		return (FALSE);
	}

	// on cree la fenetre m�re
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

	// Lorsque la bo�te de dialogue est ferm�e, retourner FALSE afin de quitter
	//  l'application, plut�t que de d�marrer la pompe de messages de l'application.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
// On sort de l'application
int CAtelierPresseApp::ExitInstance()
{
	// on d�truit le manager des Items Construits
	CItemsConstruitsManager::Destroy();

	// on d�truit le manager des templates (styles)
	CXTemplateManager::Destroy();

	return CWinApp::ExitInstance();
}
