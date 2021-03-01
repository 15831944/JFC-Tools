//
// Fichier: JMTFR03FullBinomialCum.h
//
// Cette classe permet de calcul la distibution cumul�e avec n insertions
// en ajoutant au fur et � mesure les insertions (sans reconstruire � chaque
// fois la maille)

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_FULL_BINOMIALCUM_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_FULL_BINOMIALCUM_H

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

class JMTFR03FullBinomialCum
{
public:
	// le constructeur
	JMTFR03FullBinomialCum();

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, const JUnt32 & NbInsertions, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution niveau n insertions
	JVoid MakeDistribution(JDistribution & Distribution, JUnt32 & NbInsert) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03FullBinomialCum & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03FullBinomialCum();

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
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options,  const JUnt32 & NbInsertions, JArray<JResultats> &TBLItems) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03FullBinomialCum & operator =(const JMTFR03FullBinomialCum & Source);

	// le constructeur copie
	JMTFR03FullBinomialCum(const JMTFR03FullBinomialCum & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
