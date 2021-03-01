
/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CAtelierPresseApp
//
//	Descriptif:
//	Classe de l'application lanceur de l'Atelier Presse
*/


// Atelier Presse.h : fichier d'en-tête principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CAtelierPresseApp :
// Consultez Atelier Presse.cpp pour l'implémentation de cette classe
//

class CAtelierPresseApp : public CWinApp
{
public:
	CAtelierPresseApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Implémentation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAtelierPresseApp theApp;
