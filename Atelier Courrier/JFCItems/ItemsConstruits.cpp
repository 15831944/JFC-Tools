#include "stdafx.h"
#include "ItemsConstruits.h"
#include "JFCConfig.h"

CItemsConstruits *	CItemsConstruits::m_pInstance;

// Constructeur
CItemsConstruits::CItemsConstruits(void)
{
}

// Destructeur
CItemsConstruits::~CItemsConstruits(void)
{
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération de l'instance manager
CItemsConstruits * CItemsConstruits::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new CItemsConstruits;
	}

	static bool bLoaded = false;

	if (!bLoaded)
	{
		// on essaie de charger
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
void CItemsConstruits::Destroy()
{
	if (m_pInstance)
		delete m_pInstance;
	m_pInstance = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
//// Get an instance of this class
//// this adds one to the reference counter and
//// thus must be released by the caller after use
//CItemsConstruits * CItemsConstruits::GetInstance()
//{
//	// on initialise l'indicateur
//	static JBool bLoaded = false;
//
//	if(!bLoaded)
//	{
//		if(!m_Instance.Load("ItemsConstruits"))
//			m_Instance.CreateEnsemble();
//
//		bLoaded = true;
//	}
//
//	m_RefCount++;
//
//	return & m_Instance;
//}

// Decrements the reference count
//JBool	CItemsConstruits::Release()
//{
//	if(!m_RefCount || --m_RefCount)
//		return false;
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////////////////////
// Chargement des items construits
JBool CItemsConstruits::Load(const char *Name)
{
	m_FileName = Name;
	m_FileName.Append(".10");
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
			AfxMessageBox("File ItemsConstruits.10 not found");
		#endif
		return false;
	}

	m_bDirty	= false;
	m_bLoaded	= true;
	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));

	return true;
}

