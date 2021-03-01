/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS (mise en dll)
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

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef ATPClipboard_H

// on définit la macro pour éviter les inclusions multiples
#define ATPClipboard_H

// on inclut les définitions nécessaires
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// la fonction pour copier un fichier dans le presse papier

__declspec(dllexport) BOOL CopyFileToClipBrd(HWND hWnd, const TCHAR* pFilename);

// fin de l'inclusion conditionelle
#endif
