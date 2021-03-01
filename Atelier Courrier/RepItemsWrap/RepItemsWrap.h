// RepItemsWrap.h : fichier d'en-tête principal pour la DLL RepItemsWrap
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"		// symboles principaux


#include "JLib.h"
#include "RepItemsAPI.h"

// CRepItemsWrapApp
// Consultez RepItemsWrap.cpp pour l'implémentation de cette classe
//

class CRepItemsWrapApp : public CWinApp
{
public:
	CRepItemsWrapApp();

// Substitutions
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
