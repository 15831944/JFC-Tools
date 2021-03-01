//
// Fichier: JFR03TBLTerrains.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JFR03_TABLE_TERRAINS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JFR03_TABLE_TERRAINS_H

// on inclut les d�finitions n�cessaires
#include "JFR03Terrain.h"

class JFR03TBLTerrains
{
public:
	// le constructeur
	JFR03TBLTerrains();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour rechercher les �l�ments
	JInt32 FindItem(JRef32 IdTerrain) const;

	// la fonction pour r�cup�rer les �l�ments
	const JFR03Terrain & Item(JInt32 Index) const;

	// les fonctions pour importer/exporter les �l�ments
	JVoid Import(JStream* pStream);
	JVoid Export(JStream* pStream) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JFR03TBLTerrains & Source);
	
	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JFR03TBLTerrains();

private:
	// l'op�rateur pour recopier les �l�ments
	JFR03TBLTerrains & operator =(const JFR03TBLTerrains & Source);

	// le constructeur copie
	JFR03TBLTerrains(const JFR03TBLTerrains & Source);

private:
	JArray<JFR03Terrain> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
