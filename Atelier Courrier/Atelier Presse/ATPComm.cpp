// ATPComm.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ATPComm.h"


// CATPComm

CATPComm::CATPComm()
{
	m_appToStart = COM_NOTHING;
	#ifdef _DEBUG
	m_bShowSplash = FALSE;
	#endif
}

CATPComm::~CATPComm()
{
}


// Fonctions membres CATPComm
void CATPComm::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	CString param = pszParam;

	m_dico = DEFAULT;
	if (param.CompareNoCase("ANG") == 0)
		m_dico = ANG;

	if (param.CompareNoCase("cs") == 0)
		m_appToStart = COM_CHOIXSUPPORT;
	if (param.CompareNoCase("p") == 0)
		m_appToStart = COM_PLAN;
	if (param.CompareNoCase("c") == 0)
		m_appToStart = COM_CREUSET;
	if (param.CompareNoCase("i") == 0)
		m_appToStart = COM_ITEM;
	if (param.CompareNoCase("s") == 0)
		m_appToStart = COM_SUPPORT;
	if (param.CompareNoCase("n") == 0)
		m_appToStart = COM_NOMENCLATURE;
	if (param.CompareNoCase("a") == 0)
		m_appToStart = COM_ANALYSEUR;

	if (param.CompareNoCase("nosplash") == 0)
		m_bShowSplash = FALSE;


	if (param.Find(".atp") != -1)
		m_appToStart = COM_CREUSET;
	if (param.Find(".ofr") != -1)
		m_appToStart = COM_CHOIXSUPPORT;
	if (param.Find(".anl") != -1)
		m_appToStart =COM_ANALYSEUR;

	if (param.Find(".atp") != -1 || param.Find(".ofr") != -1 || param.Find(".anl") != -1)
	{
		m_strFileName == "";
		if (param.Find("\\") == -1)
		{
			char * curDir;
			curDir = new char[2048];
			GetCurrentDirectory(2048, curDir);
			m_strFileName = curDir;
			m_strFileName += "\\";
			delete curDir;
		}

		m_strFileName += param;
		m_bShowSplash = TRUE;
	#ifdef _DEBUG
		m_bShowSplash = FALSE;
	#endif
	}

}
