/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTBLTerrains
//
//	Descriptif:
//	Table des terrains
*/

//
// Fichier: JTBLTerrains.h
//
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JLIBPRESSE_TABLE_TERRAINS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JLIBPRESSE_TABLE_TERRAINS_H

#pragma once

// on inclut les d�finitions n�cessaires
#include "JTerrain.h"



class AFX_EXT_CLASS JTBLTerrains : private JList<JTerrain>
{
public:
	/// le constructeur
	JTBLTerrains();

	/// la fonction pour rechercher un �l�ment
	const JTerrain * GetItem(JInt32 IdSource, JInt32 IdTerrain) const;

	/// la fonction pour r�cup�rer un �l�ment
	const JTerrain & GetItem() const;

	JVoid MoveFirst() const;
	JVoid MoveLast() const;
	JBool IsValid() const;
	JVoid MovePrev() const;
	JVoid MoveNext() const;

	// Nombre d'�l�ment
	JInt32 GetCount() const;

	/// les fonctions pour importer les �l�ments
	JVoid Import(CStdioFile& file);

	// Verification du path associ� aux fichiers terrains
	JBool PathAvailable(CString PathLabel);

	/// le destructeur
	~JTBLTerrains();
};

// fin de l'inclusion conditionnelle
#endif
