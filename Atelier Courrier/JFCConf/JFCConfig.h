/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCConfig
//
//	Descriptif:
//	Point d'entrée de la dll vérifie la présence du fichier de configuration et le charge.
*/

#pragma once

#include "..\Config\BuildConfig.h"

#define GETPROFILESTRINGD(section, keyVal, defVal)	JFCConfig::GetInstance().GetValueString(section, keyVal, defVal)
#define GETPROFILESTRING(section, keyVal)			JFCConfig::GetInstance().GetValueString(section, keyVal, 0)

#define WRITEPROFILESTRING(section, keyVal, val)	JFCConfig::GetInstance().WriteValueString(section, keyVal, val)

#define GETPROFILEINTD(section, keyVal, defVal)	JFCConfig::GetInstance().GetValueInt(section, keyVal, defVal)
#define GETPROFILEINT(section, keyVal)			JFCConfig::GetInstance().GetValueInt(section, keyVal, 0)

#define WRITEPROFILEINT(section, keyVal, val)	JFCConfig::GetInstance().WriteValueInt(section, keyVal, val)


#define SECTION_DEFAULT		""

#define KEY_INSTALL_PATH	"InstallPath"

#define SECTION_DATA		"Data"

#define KEY_SOURCES_PATH	"SourcesPath"
#define KEY_TARIFS_PATH		"TarifsPath"
#define KEY_USER_PATH		"UserPath"

#define SECTION_OPTIONS		"Options"

#define KEY_COLOR_PERSO_LIGHT	"ColorPersoLight"
#define KEY_COLOR_PERSO_MEDIUM	"ColorPersoMedium"
#define KEY_COLOR_PERSO_DARK	"ColorPersoDark"
#define KEY_COLOR_PERSO_SELECT	"ColorPersoSelect"

#define KEY_COLOR_REP_ITEM		"ColorRepItem"

#define KEY_OP_ICONS			"OpIcons"
#define KEY_SOURCE_ID			"SourceId"
#define KEY_TERRAIN_ID			"TerrainId"
#define KEY_LEFT_SPLIT_VERT		"LeftSplitVert"

#define KEY_ICON				"Icon"
#define KEY_MAX					"Max"


#define SECTION_OPTIONS_REPITEM		"Options\\RepItem"
#define SECTION_OPTIONS_ANALYSEUR	"Options\\Analyseur"

#define KEY_RECT				"Rect"
#define KEY_RECT_PUPITRE		"RectPupitre"


class Pimpl;

class AFX_EXT_CLASS JFCConfig
{
private:
	/// le constructeur
	JFCConfig();
	/// le destructeur
	~JFCConfig();

	/// l'unique instance des sources
	static JFCConfig m_Instance;

	DWORD LoadPaths();

	bool m_bValid;
	static bool m_bBuildForSQN;

	Pimpl * m_pImpl;

public:

	/// la fonction pour récupérer l'instance des sources
	static JFCConfig & GetInstance();

#if defined ENABLE_INI_FILE
	/// la fonction pour créer l'instance de configuration
	static void CreateInstance(LPCTSTR iniFile);
#endif

	bool CanStart();

	LPCTSTR GetInstallPath();
	LPCTSTR GetSourcesPath();
	LPCTSTR GetTarifsPath();
	LPCTSTR GetUserPath();

	CString GetValueString(LPCTSTR section, LPCTSTR keyVal, LPCTSTR defVal);
	int     GetValueInt(LPCTSTR section, LPCTSTR keyVal, int defVal);

	BOOL	WriteValueString(LPCTSTR section, LPCTSTR keyVal, LPCTSTR value);
	BOOL	WriteValueInt(LPCTSTR section, LPCTSTR keyVal, int value);

	static void SetBuildForSQN();
};
