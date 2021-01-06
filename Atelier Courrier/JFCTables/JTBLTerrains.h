/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
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
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JLIBPRESSE_TABLE_TERRAINS_H

// on définit la macro pour éviter les inclusions multiples
#define JLIBPRESSE_TABLE_TERRAINS_H

#pragma once

// on inclut les définitions nécessaires
#include "JTerrain.h"



class AFX_EXT_CLASS JTBLTerrains : private JList<JTerrain>
{
public:
	/// le constructeur
	JTBLTerrains();

	/// la fonction pour rechercher un élément
	const JTerrain * GetItem(JInt32 IdSource, JInt32 IdTerrain) const;

	/// la fonction pour récupérer un élément
	const JTerrain & GetItem() const;

	JVoid MoveFirst() const;
	JVoid MoveLast() const;
	JBool IsValid() const;
	JVoid MovePrev() const;
	JVoid MoveNext() const;

	// Nombre d'élément
	JInt32 GetCount() const;

	/// les fonctions pour importer les éléments
	JVoid Import(CStdioFile& file);

	// Verification du path associé aux fichiers terrains
	JBool PathAvailable(CString PathLabel);

	/// le destructeur
	~JTBLTerrains();
};

// fin de l'inclusion conditionnelle
#endif
