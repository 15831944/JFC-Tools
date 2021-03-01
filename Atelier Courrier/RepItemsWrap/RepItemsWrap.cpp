// RepItemsWrap.cpp : définit les fonctions d'initialisation pour la DLL.
//

#include "stdafx.h"
#include "RepItemsWrap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO : si cette DLL est liée dynamiquement aux DLL MFC,
//		toute fonction exportée de cette DLL qui appelle
//		MFC doit avoir la macro AFX_MANAGE_STATE ajoutée au
//		tout début de la fonction.
//
//		Par exemple :
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// corps de fonction normal ici
//		}
//
//		Il est très important que cette macro se trouve dans chaque
//		fonction, avant tout appel à MFC. Cela signifie qu'elle
//		doit être la première instruction dans la 
//		fonction, avant toute déclaration de variable objet
//		dans la mesure où leurs constructeurs peuvent générer des appels à la DLL
//		MFC.
//
//		Consultez les notes techniques MFC 33 et 58 pour plus de
//		détails.
//


// CRepItemsWrapApp

BEGIN_MESSAGE_MAP(CRepItemsWrapApp, CWinApp)
END_MESSAGE_MAP()


// construction CRepItemsWrapApp

CRepItemsWrapApp::CRepItemsWrapApp()
{
	// TODO : ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CRepItemsWrapApp

CRepItemsWrapApp theApp;


// initialisation de CRepItemsWrapApp

BOOL CRepItemsWrapApp::InitInstance()
{
	CWinApp::InitInstance();

	// Initialiser les bibliothèques OLE
	if (!AfxOleInit())
	{
		AfxMessageBox("Error");
		return FALSE;
	}

	free((void*)m_pszAppName);
	//Change the name of the application file.
	//The CWinApp destructor will free the memory.
	m_pszAppName=_tcsdup(_T("AtelierInternet"));

	SetRegistryKey(_T("JFC"));

	return TRUE;
}
