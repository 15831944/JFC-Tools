//
// Fichier: JFR03TBLSources.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JFR03_TABLE_SOURCES_H

// on définit la macro pour éviter les inclusions multiples
#define JFR03_TABLE_SOURCES_H

// on inclut les définitions nécessaires
#include "JFR03Source.h"

class JFR03TBLSources
{
public:
	// le constructeur
	JFR03TBLSources();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour rechercher les éléments
	JInt32 FindItem(JRef32 IdSource) const;

	// la fonction pour récupérer les éléments
	const JFR03Source & Item(JInt32 Index) const;

	// les fonctions pour importer/exporter les éléments
	JVoid Import(JStream* pStream);
	JVoid Export(JStream* pStream) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JFR03TBLSources & Source);
	
	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JFR03TBLSources();

private:
	// l'opérateur pour recopier les éléments
	JFR03TBLSources & operator =(const JFR03TBLSources & Source);

	// le constructeur copie
	JFR03TBLSources(const JFR03TBLSources & Source);

private:
	JArray<JFR03Source> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
