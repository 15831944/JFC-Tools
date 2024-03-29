// JFCConf.cpp : d�finit les fonctions d'initialisation pour la DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE JFCConfDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Supprimez cet �l�ment si vous utilisez lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Initialisation de JFCConf.DLL !\n");
		
		// Initialisation unique de la DLL d'extension
		if (!AfxInitExtensionModule(JFCConfDLL, hInstance))
			return 0;

		// Ins�rez cette DLL dans la cha�ne de ressource
		// REMARQUE�: Si cette DLL d'extension est li�e implicitement par
		//  une DLL r�guli�re MFC (par exemple un contr�le ActiveX)
		//  au lieu d'une application MFC, supprimez
		//  cette ligne de DllMain et placez-la dans une fonction
		//  s�par�e export�e � partir de cette DLL d'extension. La DLL r�guli�re
		//  qui utilise cette DLL d'extension doit ensuite explicitement appeler cette
		//  fonction pour initialiser cette DLL d'extension. Dans le cas contraire,
		//  l'objet CDynLinkLibrary ne sera pas attach� � la cha�ne de ressource
		//  de la DLL r�guli�re et de nombreux probl�mes
		//  seront g�n�r�s.

		new CDynLinkLibrary(JFCConfDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Arr�t de JFCConf.DLL !\n");

		// Terminez la biblioth�que avant que les destructeurs soient appel�s
		AfxTermExtensionModule(JFCConfDLL);
	}
	return 1;   // ok
}
