//
// Fichier: JMTFR03TBLEffectifs.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_EFFECTIFS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_EFFECTIFS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03Logs.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYEffectif.h"
#include "JMTFR03DATEffectif.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"
#include "JMTFR03TBLPonderations.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03RequeteE.h"

class JMTFR03TBLEffectifs
{
public:
	// le constructeur
	JMTFR03TBLEffectifs();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	const JMTFR03DATEffectif & Add(const JMTFR03KEYEffectif & KEYEffectif, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, JMTFR03Logs* pLogs = 0, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYEffectif & GetKey() const;
	const JMTFR03DATEffectif & GetItem() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYEffectif & KEYEffectif, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLEffectifs & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLEffectifs();

private:
	// on d�finit les r�sultats
	class JResultats
	{
	public:
		JFlt64 m_Effectif; // l'effectif pond�r�
		JInt32 m_NbCas;    // le nombre de cas
	};

private:
	// la fonction pour calculer les r�sultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03RequeteE & RequeteE, JResultats & Resultats) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLEffectifs & operator =(const JMTFR03TBLEffectifs & Source);

	// le constructeur copie
	JMTFR03TBLEffectifs(const JMTFR03TBLEffectifs & Source);

private:
	JMap<JMTFR03KEYEffectif, JMTFR03DATEffectif> m_TBLEffectifs; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
