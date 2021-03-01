//
// Fichier: JMTFR03FLTTitres.h
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_FILTRE_TITRES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_FILTRE_TITRES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03FLTTitres
{
public:
	// le constructeur
	JMTFR03FLTTitres();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour rechercher les �l�ments
	JInt32 FindItem(JRef32 IdTitre) const;

	// la fonction pour r�cup�rer les �l�ments
	const JRef32 & Item(JInt32 Index) const;

	// le destructeur
	~JMTFR03FLTTitres();

private:
	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03FLTTitres & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

private:
	// les fonctions pour ajouter les titres
	JVoid OnAddTitres(const JMTFR03TBLFiltres   & TBLFiltres,   JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLReprises  & TBLReprises,  JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLVariables & TBLVariables, JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLLectorats & TBLLectorats, JMap<JRef32, JBool> & TmpItems) const;
	JVoid OnAddTitres(const JMTFR03TBLAttributs & TBLAttributs, JMap<JRef32, JBool> & TmpItems) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03FLTTitres & operator =(const JMTFR03FLTTitres & Source);

	// le constructeur copie
	JMTFR03FLTTitres(const JMTFR03FLTTitres & Source);

private:
	// on d�clare la classe amie
	friend class JMTFR03TBLModulations;

private:
	// on d�finit les �l�ments
	class JItem
	{
	public:
		JRef32 m_IdTitre; // l'identifiant du titre
	};

private:
	JArray<JItem> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
