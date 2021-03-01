//
// Fichier: JSRCPresse.cpp
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////
// l'unique instance des sources

JSRCPresse JSRCPresse::m_Instance;

////////////////////
// les constructeurs

JSRCPresse::JSRCPresse()
{
	// on ne fait rien
}

JSRCPresse::JSRCPresse(const JSRCPresse & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JSRCPresse & JSRCPresse::operator =(const JSRCPresse & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// la fonction pour r�cup�rer l'instance des sources

const JSRCPresse & JSRCPresse::GetInstance()
{
	// on initialise l'indicateur
	static JBool m_Load = false;

	// on teste si on doit faire le chargement
	if (!m_Load)
	{
		CString path;
		CStdioFile file;

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		//Filtres d'audiences
		path = "FiltresAudience.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCPresse::m_Instance.m_TBLFiltresAudience.Import(file);
			file.Close();
		}


	
	////////////////
	/////TARIFS/////
	////////////////

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "TarifsPath", 0));
	
		//P�riodicit�s
		path = "Periodicites.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCPresse::m_Instance.m_TBLPeriodicites.Import(file);
			file.Close();
		}

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "TarifsPath", 0));
		//Formats
		path = "Formats.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCPresse::m_Instance.m_TBLFormats.Import(file);
			file.Close();
		}

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "TarifsPath", 0));
		//Titres
		path = "Titres.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCPresse::m_Instance.m_TBLTitres.Import(file);
			file.Close();
		}

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "TarifsPath", 0));
		//Couplages fixes
		path = "CouplagesFixes.tbl";
		if (file.Open(path, CFile::modeRead | CFile::typeText ))
		{
			JSRCPresse::m_Instance.m_TBLTitres.ImportCouplagesFixes(file);
			//JSRCPresse::m_Instance.m_TBLCouplagesFixes.Import(file, JSRCPresse::m_Instance.m_TBLTitres);
			file.Close();
		}

		m_Load = true;
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	}

	// on renvoie l'instance
	return (m_Instance);
}

/////////////////
// le destructeur

JSRCPresse::~JSRCPresse()
{
	// on ne fait rien
}
