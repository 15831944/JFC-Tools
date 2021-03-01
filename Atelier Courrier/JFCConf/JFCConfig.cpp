//
// Fichier: JFCConfig.cpp
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCConfig.h"

#include "JFCIniFile.h"

//////////////////////////////////////////////////////////////////////////

bool JFCConfig::m_bBuildForSQN = false;

//////////////////////////////////////////////////////////////////////////

// on met ce dans une définition privée car on ne veut pas de CString dans une méthode exposée
// ie : pas de dépendances externes aux MFC, 
class Pimpl
{
public:
	Pimpl(){}
	~Pimpl(){}
	CString sInstallPath;
	CString sSourcesPath;
	CString sTarifsPath;
	CString sUserPath;
	DWORD	dwLoadedPath;

#if defined ENABLE_INI_FILE
	JFCIniFile m_iniFile;
#endif
};


////////////////////////////////
// l'unique instance des sources

JFCConfig JFCConfig::m_Instance;

enum
{
	INSTALLPATH_LOADED	= 0x00000001,
	SOURCESPATH_LOADED	= 0x00000002,
	TARIFSPATH_LOADED	= 0x00000004,
	USERPATH_LOADED		= 0x00000008,
	ALLPATHS_LOADED		= 0x0000000F
};

////////////////////
// les constructeurs

JFCConfig::JFCConfig()
{
	m_bValid = false;
	m_pImpl = new Pimpl;
}

////////////////////////////////////////////////////

JFCConfig & JFCConfig::GetInstance()
{
	// on initialise l'indicateur
	static bool m_Load = false;

	// on teste si on doit faire le chargement
	if (!m_Load)
	{
		if (m_Instance.LoadPaths() != ALLPATHS_LOADED)
		{
			// problème de chargement des terrains, verifier lequel pose
			// problème avec la valeur de "m_pImpl->dwLoadedPath"
			AfxMessageBox("Impossible de lire dans la base de registre", MB_OK | MB_ICONEXCLAMATION);
			return m_Instance;
		}

		CString path;
		CStdioFile file;
		
		JFCConfig::m_Instance.m_bValid = true;

		m_Load = true;
		SetCurrentDirectory(m_Instance.m_pImpl->sSourcesPath);
	}

	// on renvoie l'instance
	return (m_Instance);
}

#if defined ENABLE_INI_FILE

void JFCConfig::CreateInstance(LPCTSTR iniFile)
{
	m_Instance.m_pImpl->m_iniFile.SetPath(iniFile);

	m_Instance.m_pImpl->m_iniFile.ReadFile();
}

#endif

/////////////////
// le destructeur

JFCConfig::~JFCConfig()
{
	// on détruit le pimpl
	if (m_pImpl) delete m_pImpl; m_pImpl = 0;
}

bool JFCConfig::CanStart()
{
	return GetInstance().m_bValid;
}


LPCTSTR JFCConfig::GetInstallPath()
{
	return m_pImpl->sInstallPath;
}

LPCTSTR JFCConfig::GetSourcesPath()
{
	return m_pImpl->sSourcesPath;
}

LPCTSTR JFCConfig::GetTarifsPath()
{
	return m_pImpl->sTarifsPath;
}

LPCTSTR JFCConfig::GetUserPath()
{
	return m_pImpl->sUserPath;
}

DWORD JFCConfig::LoadPaths()
{
	// TODO : chargements des dossiers depuis des fichiers ini zipppés plutot que depuis la registry
	// on charge les chemins depuis la base de registre pour l'instant
#if !defined BUILD_FOR_ATELIER_INTERNET
	m_pImpl->sInstallPath	= GetValueString(SECTION_DEFAULT,	KEY_INSTALL_PATH,	"?");
	if (!m_bBuildForSQN)
		m_pImpl->sTarifsPath= GetValueString(SECTION_DATA,		KEY_TARIFS_PATH,	"?");
#endif
	m_pImpl->sSourcesPath	= GetValueString(SECTION_DATA,		KEY_SOURCES_PATH,	"?");
	m_pImpl->sUserPath		= GetValueString(SECTION_DATA,		KEY_USER_PATH,		"?");
	m_pImpl->dwLoadedPath	= 0;

	// on attribue la valeur de resultat pour le chargempent des chemins
	if (m_pImpl->sInstallPath != "?")	m_pImpl->dwLoadedPath |= INSTALLPATH_LOADED;
	if (m_pImpl->sSourcesPath != "?")	m_pImpl->dwLoadedPath |= SOURCESPATH_LOADED;
	if (m_pImpl->sTarifsPath != "?")	m_pImpl->dwLoadedPath |= TARIFSPATH_LOADED;
	if (m_pImpl->sUserPath != "?")		m_pImpl->dwLoadedPath |= USERPATH_LOADED;

	return m_pImpl->dwLoadedPath;
}

CString JFCConfig::GetValueString(LPCTSTR section, LPCTSTR keyVal, LPCTSTR defVal)
{
#if defined ENABLE_INI_FILE
	return m_Instance.m_pImpl->m_iniFile.GetValue(section, keyVal, (defVal != NULL) ? defVal : "").c_str();
#else
	return AfxGetApp()->GetProfileString(section, keyVal, defVal);
#endif
}

BOOL JFCConfig::WriteValueString(LPCTSTR section, LPCTSTR keyVal, LPCTSTR value)
{
#if defined ENABLE_INI_FILE
	if (m_Instance.m_pImpl->m_iniFile.SetValue(section, keyVal, value, true))
	{
		m_Instance.m_pImpl->m_iniFile.WriteFile();
		return TRUE;
	}
	else return FALSE;
#else
	return AfxGetApp()->WriteProfileString(section, keyVal, value);
#endif
}

int JFCConfig::GetValueInt(LPCTSTR section, LPCTSTR keyVal, int defVal)
{
#if defined ENABLE_INI_FILE
	return m_Instance.m_pImpl->m_iniFile.GetValueI(section, keyVal, defVal);
#else
	return AfxGetApp()->GetProfileInt(section, keyVal, defVal);
#endif
}

BOOL JFCConfig::WriteValueInt(LPCTSTR section, LPCTSTR keyVal, int value)
{
#if defined ENABLE_INI_FILE
	if (m_Instance.m_pImpl->m_iniFile.SetValueI(section, keyVal, value, true))
	{
		m_Instance.m_pImpl->m_iniFile.WriteFile();
		return TRUE;
	}
	else return FALSE;
#else
	return AfxGetApp()->WriteProfileInt(section, keyVal, value);
#endif
}

void JFCConfig::SetBuildForSQN()
{
	m_bBuildForSQN = true;
}
