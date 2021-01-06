//
// Fichier: JMTFR03FullBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    08/01/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_FULL_BINOMIAL_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_FULL_BINOMIAL_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03Logs.h"

// on inclut les d�finitions n�cessaires
#include "JDistribution.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"
#include "JMTFR03TBLPonderations.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Options.h"
#include "JMTFR03Maille.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03RequeteA.h"

class JMTFR03FullBinomial
{
public:
	// le constructeur
	JMTFR03FullBinomial();

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fNoDistrib, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution
	JVoid MakeDistribution(JDistribution & Distribution) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03FullBinomial & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03FullBinomial();

private:
	// on d�finit les r�sultats
	class JResultats
	{
	public:
		JArray<JFlt64> m_TBLClasses; // les effectifs par classe
		JFlt64         m_Effectif;   // l'effectif de r�f�rence
	};

private:
	// la fonction pour calculer les r�sultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fNoDistrib, JResultats & Resultats) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03FullBinomial & operator =(const JMTFR03FullBinomial & Source);

	// le constructeur copie
	JMTFR03FullBinomial(const JMTFR03FullBinomial & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
