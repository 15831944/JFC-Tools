//
// Fichier: JMTFR03FLTSegments.h
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_FILTRE_SEGMENTS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_FILTRE_SEGMENTS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03FLTSegments
{
public:
	// le constructeur
	JMTFR03FLTSegments();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour rechercher les �l�ments
	JInt32 FindItem(JRef32 IdSegment) const;

	// la fonction pour r�cup�rer les �l�ments
	const JRef32 & Item(JInt32 Index) const;

	// le destructeur
	~JMTFR03FLTSegments();

public:
	// les fonctions pour manipuler la s�lection des �l�ments
	JVoid SetSelItem(JInt32 Index, JBool Selection);
	JBool GetSelItem(JInt32 Index) const;

private:
	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03FLTSegments & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03FLTSegments & operator =(const JMTFR03FLTSegments & Source);

	// le constructeur copie
	JMTFR03FLTSegments(const JMTFR03FLTSegments & Source);

private:
	// on d�clare la classe amie
	friend class JMTFR03TBLModulations;

private:
	// on d�finit les �l�ments
	class JItem
	{
	public:
		JRef32 m_IdSegment; // l'identifiant du segment de population
		JMsk32 m_Etat;      // l'�tat du segment de population
	};

private:
	JArray<JItem> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
