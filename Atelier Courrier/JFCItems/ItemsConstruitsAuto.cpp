#include "stdafx.h"
#include "ItemsConstruitsAuto.h"
#include "JFCConfig.h"


CItemsConstruitsAuto * CItemsConstruitsAuto::m_pInstance;

// Constructeur
CItemsConstruitsAuto::CItemsConstruitsAuto(void)
{
}

// Destructeur
CItemsConstruitsAuto::~CItemsConstruitsAuto(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération de l'instance des items construits automatiques
CItemsConstruitsAuto * CItemsConstruitsAuto::GetInstance()
{
	if (m_pInstance == 0)
	{
		// Création du pointeur de l'instance
		m_pInstance = new CItemsConstruitsAuto;
	}

	// Flag de chargement
	static bool bLoaded = false;

	if (!bLoaded)
	{
		// on essaie de charger le fichier correspondant
		if (!m_pInstance->Load("ItemsConstruits"))
		{
			m_pInstance->CreateEnsemble();
		}

		// Le fichier est bien chargé
		bLoaded = true;
	}

	return m_pInstance;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de l'instance des items construits
void CItemsConstruitsAuto::Destroy()
{
	if (m_pInstance)
		delete m_pInstance;
	m_pInstance = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Chargement des items construits automatisés
JBool CItemsConstruitsAuto::Load(const char *Name)
{
	m_FileName = Name;
	m_FileName.Append(".10a");
	m_sName = "/";
	m_sName.Append (m_FileName);

	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));

	CFileStatus Status;
	if(CFile::GetStatus(m_FileName.AsJCharPtr(),Status))
		m_ThisTime = Status.m_mtime ;
	else
		m_ThisTime=CTime::GetCurrentTime();

	JArchive * pFile = 0;

	try
	{
		pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadItems			(pFile, m_FileHeader.GetVersion());
		LoadItemsIndependant(pFile, m_FileHeader.GetVersion());
		LoadPalettess		(pFile, m_FileHeader.GetVersion());
		LoadQuestionInds	(pFile, m_FileHeader.GetVersion());
		LoadCritereMods		(pFile, m_FileHeader.GetVersion());
		LoadCritereQuants	(pFile, m_FileHeader.GetVersion());
		LoadCritereScores	(pFile, m_FileHeader.GetVersion());
		LoadItemsPonderes	(pFile, m_FileHeader.GetVersion());

		delete pFile;
	}
	catch(...)
	{
		delete pFile;
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));

		#ifdef _DEBUG
			AfxMessageBox("File ItemsConstruits.10a not found");
		#endif
		return false;
	}

	m_bDirty	= false;
	m_bLoaded	= true;
	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));

	return true;
}

