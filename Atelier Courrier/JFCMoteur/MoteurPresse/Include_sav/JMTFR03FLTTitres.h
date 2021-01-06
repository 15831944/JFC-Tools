//
// Fichier: JMTFR03FLTTitres.h
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_FILTRE_TITRES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_FILTRE_TITRES_H

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03FLTTitres
{
public:
	// le constructeur
	JMTFR03FLTTitres();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour rechercher les éléments
	JInt32 FindItem(JRef32 IdTitre) const;

	// la fonction pour récupérer les éléments
	const JRef32 & Item(JInt32 Index) const;

	// le destructeur
	~JMTFR03FLTTitres();

private:
	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03FLTTitres & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

private:
	// les fonctions pour ajouter les titres
	JVoid OnAddTitres(const JMTFR03TBLFiltres   & TBLFiltres,   JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLReprises  & TBLReprises,  JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLVariables & TBLVariables, JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLLectorats & TBLLectorats, JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLAttributs & TBLAttributs, JMap<JRef32, JBool> & TmpItems) const;

private:
	// l'opérateur pour recopier les éléments
	JMTFR03FLTTitres & operator =(const JMTFR03FLTTitres & Source);

	// le constructeur copie
	JMTFR03FLTTitres(const JMTFR03FLTTitres & Source);

private:
	// on déclare la classe amie
	friend class JMTFR03TBLModulations;

private:
	// on définit les éléments
	class JItem
	{
	public:
		JRef32 m_IdTitre; // l'identifiant du titre
	};

private:
	JArray<JItem> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
