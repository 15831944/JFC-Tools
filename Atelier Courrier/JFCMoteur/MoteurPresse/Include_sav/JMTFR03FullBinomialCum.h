//
// Fichier: JMTFR03FullBinomialCum.h
//
// Cette classe permet de calcul la distibution cumulée avec n insertions
// en ajoutant au fur et à mesure les insertions (sans reconstruire à chaque
// fois la maille)

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_FULL_BINOMIALCUM_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_FULL_BINOMIALCUM_H

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

class JMTFR03FullBinomialCum
{
public:
	// le constructeur
	JMTFR03FullBinomialCum();

	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, const JUnt32 & NbInsertions, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution niveau n insertions
	JVoid MakeDistribution(JDistribution & Distribution, JUnt32 & NbInsert) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03FullBinomialCum & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03FullBinomialCum();

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
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options,  const JUnt32 & NbInsertions, JArray<JResultats> &TBLItems) const;

private:
	// l'opérateur pour recopier les éléments
	JMTFR03FullBinomialCum & operator =(const JMTFR03FullBinomialCum & Source);

	// le constructeur copie
	JMTFR03FullBinomialCum(const JMTFR03FullBinomialCum & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
