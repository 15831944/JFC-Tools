// SQNAdVision.cpp : D�finit les comportements de classe pour l'application.
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
	this->SetRegistryKey(_T("JFC"));

	//on traite la ligne de commande
	CATPComm cmdInfo;
	ParseCommandLine(cmdInfo);


	//On charge la couleur utilis�e
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

	//on cree la fenetre m�re
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

	// Lorsque la bo�te de dialogue est ferm�e, retourner FALSE afin de quitter
	//  l'application, plut�t que de d�marrer la pompe de messages de l'application.
	return FALSE;
}
