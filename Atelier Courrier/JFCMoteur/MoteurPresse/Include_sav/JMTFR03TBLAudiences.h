//
// Fichier: JMTFR03TBLAudiences.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_AUDIENCES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_AUDIENCES_H

// on inclut les définitions nécessaires
#include "JMTFR03Logs.h"

// on inclut les définitions nécessaires
#include "JMTFR03KEYAudience.h"
#include "JMTFR03DATAudience.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"
#include "JMTFR03TBLPonderations.h"

// on inclut les définitions nécessaires
#include "JMTFR03Options.h"
#include "JMTFR03Couplage.h"
#include "JMTFR03Maille.h"

// on inclut les définitions nécessaires
#include "JMTFR03RequeteA.h"

class JMTFR03TBLAudiences
{
public:
	// le constructeur
	JMTFR03TBLAudiences();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// les fonctions pour ajouter les éléments
	const JMTFR03DATAudience & Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, JMTFR03Logs* pLogs = 0, JBool Move = false);
	const JMTFR03DATAudience & Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Couplage & Couplage, JMTFR03Logs* pLogs = 0, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYAudience & GetKey() const;
	const JMTFR03DATAudience & GetItem() const;

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYAudience & KEYAudience, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLAudiences & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLAudiences();

private:
	// on définit les résultats
	class JResultats
	{
	public:
		JFlt64 m_AudienceRef;   // l'audience de référence
		JFlt64 m_AudienceProba; // l'audience probabiliste
		JFlt64 m_VolLecture1Jr; // le volume de lecture 1 jour moyen
		JInt32 m_PtEtalonnage;  // le point d'étalonnage de référence
	};

private:
	// la fonction pour calculer les résultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JResultats & Resultats, JBool IsCouplage) const;

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLAudiences & operator =(const JMTFR03TBLAudiences & Source);

	// le constructeur copie
	JMTFR03TBLAudiences(const JMTFR03TBLAudiences & Source);

private:
	JMap<JMTFR03KEYAudience, JMTFR03DATAudience> m_TBLAudiences; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
