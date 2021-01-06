//
// Fichier: JMTFR03FLTFiltres.h
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_FILTRE_FILTRES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_FILTRE_FILTRES_H

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03FLTFiltres
{
public:
	// le constructeur
	JMTFR03FLTFiltres();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour rechercher les éléments
	JInt32 FindItem(JRef32 IdFiltre) const;

	// la fonction pour récupérer les éléments
	const JRef32 & Item(JInt32 Index) const;

	// le destructeur
	~JMTFR03FLTFiltres();

public:
	// les fonctions pour manipuler la sélection
	JVoid  SetSelItem(JInt32 Index);
	JInt32 GetSelItem() const;

private:
	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03FLTFiltres & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03FLTFiltres & operator =(const JMTFR03FLTFiltres & Source);

	// le constructeur copie
	JMTFR03FLTFiltres(const JMTFR03FLTFiltres & Source);

private:
	// on déclare la classe amie
	friend class JMTFR03TBLModulations;

private:
	// on définit les éléments
	class JItem
	{
	public:
		JRef32 m_IdFiltre; // l'identifiant du filtre d'audience
		JMsk32 m_Etat;     // l'état du filtre d'audience
	};

private:
	JArray<JItem> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
