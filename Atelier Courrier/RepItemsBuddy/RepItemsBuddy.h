// RepItemsBuddy.h : fichier d'en-t�te principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"		// symboles principaux


// CRepItemsBuddyApp�:
// Consultez RepItemsBuddy.cpp pour l'impl�mentation de cette classe
//

class CRepItemsBuddyApp : public CWinApp
{
public:
	CRepItemsBuddyApp();

// Substitutions
	public:
	virtual BOOL InitInstance();

// Impl�mentation

	DECLARE_MESSAGE_MAP()
};

extern CRepItemsBuddyApp theApp;
