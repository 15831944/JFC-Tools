/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CSineQuaNonApp
//
//	Descriptif:
//	Classe de l'application lanceur de SineQuaNon
*/

// SineQuaNon.h : fichier d'en-t�te principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CSineQuaNonApp�:
// Consultez Items.cpp pour l'impl�mentation de cette classe
//

class CSineQuaNonApp : public CWinApp
{
public:
	CSineQuaNonApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Impl�mentation

	DECLARE_MESSAGE_MAP()
};

extern CSineQuaNonApp theApp;
