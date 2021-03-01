//
// Fichier: JFR03Sources.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "JFR03Sources.h"

/////////////////////////////////
// l'unique instance de l'élément

JFR03Sources JFR03Sources::m_Instance;

////////////////////
// les constructeurs

JFR03Sources::JFR03Sources()
{
	// on ne fait rien
}

JFR03Sources::JFR03Sources(const JFR03Sources & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JFR03Sources & JFR03Sources::operator =(const JFR03Sources & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////////////////
// la fonction pour récupérer l'instance des éléments

const JFR03Sources & JFR03Sources::GetInstance(JBool Reload)
{
	// l'indicateur de chargement
	static JBool m_Load = false;

	// on teste si on doit faire le chargement
	if (!m_Load || Reload)
	{
		// on initialise les archives
		JArchive* pSources  = 0;
		JArchive* pTerrains = 0;

		// on teste les exceptions
		try
		{
			// on ouvre les archives
			pSources  = JArchive::Create("/Sources.tbl");
			pTerrains = JArchive::Create("/Terrains.tbl");

			// on importe les tables
			JFR03TBLSources  TBLSources;  TBLSources .Import(pSources);
			JFR03TBLTerrains TBLTerrains; TBLTerrains.Import(pTerrains);

			// on permute les tables
			JFR03Sources::m_Instance.m_TBLTerrains.Swap(TBLTerrains);
			JFR03Sources::m_Instance.m_TBLSources .Swap(TBLSources);

			// on met à jour l'indicateur
			m_Load = true;
		}
		catch (...)
		{
			// on libère les archives
			delete pTerrains;
			delete pSources;

			// on propage l'exception
			throw;
		}

		// on libère les archives
		delete pTerrains;
		delete pSources;
	}

	// on renvoie l'instance
	return (m_Instance);
}

/////////////////
// le destructeur

JFR03Sources::~JFR03Sources()
{
	// on ne fait rien
}
