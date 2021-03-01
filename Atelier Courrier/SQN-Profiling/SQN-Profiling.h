/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CSQNProfilingApp
//
//	Descriptif:
//	Classe de l'application lanceur de SQN-Profiling
*/


// SQN-Profiling.h : fichier d'en-t�te principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CSQNProfilingApp�:
// Consultez SQN-Profiling.cpp pour l'impl�mentation de cette classe
//

class CSQNProfilingApp : public CWinApp
{
public:
	CSQNProfilingApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Impl�mentation

	DECLARE_MESSAGE_MAP()
};

extern CSQNProfilingApp theApp;
