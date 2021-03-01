#pragma once

#include "FileHeader.h"

//////////////////////////////////////////////////////
//		Gestion des marques : Fichier extension F1 
//
class CMarques
{
public:
	// Constructeur
	CMarques(void);

	// Destructeur
	~CMarques(void);

	/// Retour des liens marque / secteur
	JBool GetParentMarques(JMap <JInt32, JInt32> & MapParentMarques);

	/// Load File Marques
	JBool Load(JStringEx &Name);

private:

	/// File Name
	JStringEx				m_sName;
	/// Source ID
	JInt32					m_lSourceID;
	/// Terrain ID
	JInt32					m_lTerrainID;
	/// File Header
	CFileHeader				m_FileHeader;

	/// Nombre d'elements 
	JInt32					m_lNbEnfants;

	/// Liste des Marques et du lien secteur
	JMap <JInt32, JInt32>	m_MapEnfantParent;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);

	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);

	/// Lecture des Marques
	JVoid LoadMarques			(JArchive *pFile, JInt32 Version);
};

