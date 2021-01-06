//
// Fichier: JFR03TBLSources.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JFR03_TABLE_SOURCES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JFR03_TABLE_SOURCES_H

// on inclut les d�finitions n�cessaires
#include "JFR03Source.h"

class JFR03TBLSources
{
public:
	// le constructeur
	JFR03TBLSources();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour rechercher les �l�ments
	JInt32 FindItem(JRef32 IdSource) const;

	// la fonction pour r�cup�rer les �l�ments
	const JFR03Source & Item(JInt32 Index) const;

	// les fonctions pour importer/exporter les �l�ments
	JVoid Import(JStream* pStream);
	JVoid Export(JStream* pStream) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JFR03TBLSources & Source);
	
	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JFR03TBLSources();

private:
	// l'op�rateur pour recopier les �l�ments
	JFR03TBLSources & operator =(const JFR03TBLSources & Source);

	// le constructeur copie
	JFR03TBLSources(const JFR03TBLSources & Source);

private:
	JArray<JFR03Source> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
