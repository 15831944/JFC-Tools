/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS (mise en dll)
//	Sylvain SAMMURI (code)
//
//
//	Descriptif:
//	Permet de copier le contenu d'un fichier dans le presse papier
*/

//
// Fichier: ATPClipboard.h
// Date:    18/09/2001
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef ATPClipboard_H

// on d�finit la macro pour �viter les inclusions multiples
#define ATPClipboard_H

// on inclut les d�finitions n�cessaires
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// la fonction pour copier un fichier dans le presse papier

__declspec(dllexport) BOOL CopyFileToClipBrd(HWND hWnd, const TCHAR* pFilename);

// fin de l'inclusion conditionelle
#endif
