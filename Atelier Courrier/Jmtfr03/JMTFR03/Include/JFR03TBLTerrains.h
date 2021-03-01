//
// Fichier: JFR03TBLTerrains.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JFR03_TABLE_TERRAINS_H

// on définit la macro pour éviter les inclusions multiples
#define JFR03_TABLE_TERRAINS_H

// on inclut les définitions nécessaires
#include "JFR03Terrain.h"

class JFR03TBLTerrains
{
public:
	// le constructeur
	JFR03TBLTerrains();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour rechercher les éléments
	JInt32 FindItem(JRef32 IdTerrain) const;

	// la fonction pour récupérer les éléments
	const JFR03Terrain & Item(JInt32 Index) const;

	// les fonctions pour importer/exporter les éléments
	JVoid Import(JStream* pStream);
	JVoid Export(JStream* pStream) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JFR03TBLTerrains & Source);
	
	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JFR03TBLTerrains();

private:
	// l'opérateur pour recopier les éléments
	JFR03TBLTerrains & operator =(const JFR03TBLTerrains & Source);

	// le constructeur copie
	JFR03TBLTerrains(const JFR03TBLTerrains & Source);

private:
	JArray<JFR03Terrain> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
