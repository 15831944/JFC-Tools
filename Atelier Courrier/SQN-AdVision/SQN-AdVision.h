// SQN-AdVision.h : fichier d'en-t�te principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CSQNAdVisionApp�:
// Consultez SQN-AdVision.cpp pour l'impl�mentation de cette classe
//

class CSQNAdVisionApp : public CWinApp
{
public:
	CSQNAdVisionApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Impl�mentation

	DECLARE_MESSAGE_MAP()
};

extern CSQNAdVisionApp theApp;
