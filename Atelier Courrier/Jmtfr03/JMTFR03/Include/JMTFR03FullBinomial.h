//
// Fichier: JMTFR03FullBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    08/01/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_FULL_BINOMIAL_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_FULL_BINOMIAL_H

// on inclut les définitions nécessaires
#include "JMTFR03Logs.h"

// on inclut les définitions nécessaires
#include "JDistribution.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"
#include "JMTFR03TBLPonderations.h"

// on inclut les définitions nécessaires
#include "JMTFR03Options.h"
#include "JMTFR03Maille.h"

// on inclut les définitions nécessaires
#include "JMTFR03RequeteA.h"

class JMTFR03FullBinomial
{
public:
	// le constructeur
	JMTFR03FullBinomial();

	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fNoDistrib, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution
	JVoid MakeDistribution(JDistribution & Distribution) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03FullBinomial & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03FullBinomial();

private:
	// on définit les résultats
	class JResultats
	{
	public:
		JArray<JFlt64> m_TBLClasses; // les effectifs par classe
		JFlt64         m_Effectif;   // l'effectif de référence
	};

private:
	// la fonction pour calculer les résultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fNoDistrib, JResultats & Resultats) const;

private:
	// l'opérateur pour recopier les éléments
	JMTFR03FullBinomial & operator =(const JMTFR03FullBinomial & Source);

	// le constructeur copie
	JMTFR03FullBinomial(const JMTFR03FullBinomial & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
