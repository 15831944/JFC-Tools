//
// Fichier: JMTFR03FLTFiltres.h
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_FILTRE_FILTRES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_FILTRE_FILTRES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03FLTFiltres
{
public:
	// le constructeur
	JMTFR03FLTFiltres();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour rechercher les �l�ments
	JInt32 FindItem(JRef32 IdFiltre) const;

	// la fonction pour r�cup�rer les �l�ments
	const JRef32 & Item(JInt32 Index) const;

	// le destructeur
	~JMTFR03FLTFiltres();

public:
	// les fonctions pour manipuler la s�lection
	JVoid  SetSelItem(JInt32 Index);
	JInt32 GetSelItem() const;

private:
	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03FLTFiltres & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03FLTFiltres & operator =(const JMTFR03FLTFiltres & Source);

	// le constructeur copie
	JMTFR03FLTFiltres(const JMTFR03FLTFiltres & Source);

private:
	// on d�clare la classe amie
	friend class JMTFR03TBLModulations;

private:
	// on d�finit les �l�ments
	class JItem
	{
	public:
		JRef32 m_IdFiltre; // l'identifiant du filtre d'audience
		JMsk32 m_Etat;     // l'�tat du filtre d'audience
	};

private:
	JArray<JItem> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
