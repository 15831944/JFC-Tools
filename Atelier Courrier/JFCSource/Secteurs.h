#pragma once

#include "FileHeader.h"

//////////////////////////////////////////////////////
//		Gestion des secteurs : Fichier extension F2 
//
class CSecteurs
{
public:
	// Constructeur
	CSecteurs(void);

	// Destructeur
	~CSecteurs(void);

	/// Retour des liens secteur / marques
	JBool GetEnfantsSecteur(JMap <JInt32, JList<JInt32>> & MapEnfantsSecteur);

	/// Load File Secteurs
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
	JInt32					m_lNbSectors;

	/// Liste des secteurs et des liens
	JMap <JInt32, JList<JInt32>> m_MapSecteurEnfants;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);

	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);

	/// Lecture des secteurs
	JVoid LoadSecteurs			(JArchive *pFile, JInt32 Version);
};
