// JFCFileDoc.cpp : définit les fonctions d'initialisation pour la DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE JFCFileDocDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Supprimez cet élément si vous utilisez lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Initialisation de JFCFileDoc.DLL !\n");
		
		// Initialisation unique de la DLL d'extension
		if (!AfxInitExtensionModule(JFCFileDocDLL, hInstance))
			return 0;

		// Insérez cette DLL dans la chaîne de ressource
		// REMARQUE : Si cette DLL d'extension est liée implicitement par
		//  une DLL régulière MFC (par exemple un contrôle ActiveX)
		//  au lieu d'une application MFC, supprimez
		//  cette ligne de DllMain et placez-la dans une fonction
		//  séparée exportée à partir de cette DLL d'extension. La DLL régulière
		//  qui utilise cette DLL d'extension doit ensuite explicitement appeler cette
		//  fonction pour initialiser cette DLL d'extension. Dans le cas contraire,
		//  l'objet CDynLinkLibrary ne sera pas attaché à la chaîne de ressource
		//  de la DLL régulière et de nombreux problèmes
		//  seront générés.

		new CDynLinkLibrary(JFCFileDocDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Arrêt de JFCFileDoc.DLL !\n");

		// Terminez la bibliothèque avant que les destructeurs soient appelés
		AfxTermExtensionModule(JFCFileDocDLL);
	}
	return 1;   // ok
}
