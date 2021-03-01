//
// Fichier: JSRCTables.cpp
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JSRCTables.h"

#include "JFCConfig.h"


////////////////////////////////
// l'unique instance des sources

JSRCTables JSRCTables::m_Instance;

////////////////////
// les constructeurs

JSRCTables::JSRCTables()
{
	// on ne fait rien
}

JSRCTables::JSRCTables(const JSRCTables & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JSRCTables & JSRCTables::operator =(const JSRCTables & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// la fonction pour récupérer l'instance des sources

const JSRCTables & JSRCTables::GetInstance()
{
	// on initialise l'indicateur
	static JBool m_Load = false;

	// on teste si on doit faire le chargement
	if (!m_Load)
	{
		CString path;
		CStdioFile file;
		
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
		// Chargement infos Sources
		path = "Sources.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLSources.Import(file);
			file.Close();
		}

		// Chargement infoss Terrains
		path = "Terrains.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLTerrains.Import(file);
			file.Close();
		}

		//Segments
		path = "Segments.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLSegments.Import(file);
			file.Close();
		}

		//Types de poids
		path = "TypesPoids.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLPoids.Import(file);
			file.Close();
		}

		//Echantillons
		path = "Echantillons.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLEchantillons.Import(file);
			file.Close();
		}

		//Univers de redressement
		path = "UniversRedressement.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLUnivRedressement.Import(file);
			file.Close();
		}

		//Types de dimensions
		JTBLDimensions tbl;
		path = "TypesDimensions.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCTables::m_Instance.m_TBLDimensions.Import(file);
			file.SeekToBegin();
			tbl.Import(file);
			file.Close();
		}

		// Chargement des tables TypeDimension_xx 
		// (voir celles qui sont indispensable / à priori la 18 - table des contextes de lecture)
		for(tbl.MoveFirst(); tbl.IsValid(); tbl.MoveNext() )
		{
			JUnt32 id = tbl.GetItem()->GetID();
			if(id != 1)
			{
				path.Format("TypeDimension_%d.tbl",  id);
				if (file.Open(path, CFile::modeRead | CFile::typeText ))
				{
					JSRCTables::m_Instance.m_TBLDimensions.Import(file);
					file.Close();
				}
			}
		}

		m_Load = true;
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
	}

	// on renvoie l'instance
	return (m_Instance);
}

/////////////////
// le destructeur

JSRCTables::~JSRCTables()
{
	// on ne fait rien
}
