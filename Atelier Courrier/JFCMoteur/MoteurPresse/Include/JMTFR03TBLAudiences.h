//
// Fichier: JMTFR03TBLAudiences.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_AUDIENCES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_AUDIENCES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03Logs.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYAudience.h"
#include "JMTFR03DATAudience.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"
#include "JMTFR03TBLPonderations.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Options.h"
#include "JMTFR03Couplage.h"
#include "JMTFR03Maille.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03RequeteA.h"

class JMTFR03TBLAudiences
{
public:
	// le constructeur
	JMTFR03TBLAudiences();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// les fonctions pour ajouter les �l�ments
	const JMTFR03DATAudience & Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, JMTFR03Logs* pLogs = 0, JBool Move = false);
	const JMTFR03DATAudience & Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Couplage & Couplage, JMTFR03Logs* pLogs = 0, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYAudience & GetKey() const;
	const JMTFR03DATAudience & GetItem() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYAudience & KEYAudience, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLAudiences & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLAudiences();

private:
	// on d�finit les r�sultats
	class JResultats
	{
	public:
		JFlt64 m_AudienceRef;   // l'audience de r�f�rence
		JFlt64 m_AudienceProba; // l'audience probabiliste
		JFlt64 m_VolLecture1Jr; // le volume de lecture 1 jour moyen
		JInt32 m_PtEtalonnage;  // le point d'�talonnage de r�f�rence
	};

private:
	// la fonction pour calculer les r�sultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JResultats & Resultats, JBool IsCouplage) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLAudiences & operator =(const JMTFR03TBLAudiences & Source);

	// le constructeur copie
	JMTFR03TBLAudiences(const JMTFR03TBLAudiences & Source);

private:
	JMap<JMTFR03KEYAudience, JMTFR03DATAudience> m_TBLAudiences; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
