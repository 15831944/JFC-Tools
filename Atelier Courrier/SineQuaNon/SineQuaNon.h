/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CSineQuaNonApp
//
//	Descriptif:
//	Classe de l'application lanceur de SineQuaNon
*/

// SineQuaNon.h : fichier d'en-tête principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CSineQuaNonApp :
// Consultez Items.cpp pour l'implémentation de cette classe
//

class CSineQuaNonApp : public CWinApp
{
public:
	CSineQuaNonApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Implémentation

	DECLARE_MESSAGE_MAP()
};

extern CSineQuaNonApp theApp;
