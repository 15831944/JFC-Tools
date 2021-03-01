//
// Fichier: JMTFR03BetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    01/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_BETA_BINOMIAL_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_BETA_BINOMIAL_H

// on inclut les définitions nécessaires
#include "JMTFR03Logs.h"

// on inclut les définitions nécessaires
#include "JDistribution.h"

// on inclut les définitions nécessaires
#include "JMTTBLVolContacts.h"

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

class JMTFR03BetaBinomial
{
public:
	// le constructeur
	JMTFR03BetaBinomial();

	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fgEtalonnage, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution
	JVoid MakeDistribution(JFlt64 VolLectures, JDistribution & Distribution) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03BetaBinomial & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03BetaBinomial();

private:
	// on définit les résultats
	class JResultats
	{
	public:
		JFlt64 m_Effectif;       // l'effectif de référence
		JInt32 m_NbVariables;    // le nombre de variables d'étalonnage
		JFlt64 m_VolLectures1Jr; // le volume de lectures à 1 jour
		JFlt64 m_VolLecturesPrb; // le volume de lectures probabiliste
		JBool  m_Validite_001;   // la validité de l'audience à   1 jour
		JBool  m_Validite_002;   // la validité de l'audience à   2 jours
		JBool  m_Validite_004;   // la validité de l'audience à   4 jours
		JBool  m_Validite_007;   // la validité de l'audience à   7 jours
		JBool  m_Validite_014;   // la validité de l'audience à  14 jours
		JBool  m_Validite_030;   // la validité de l'audience à  30 jours
		JBool  m_Validite_060;   // la validité de l'audience à  60 jours
		JBool  m_Validite_090;   // la validité de l'audience à  90 jours
		JBool  m_Validite_120;   // la validité de l'audience à 120 jours
		JBool  m_Validite_180;   // la validité de l'audience à 180 jours
		JBool  m_Validite_270;   // la validité de l'audience à 270 jours
		JFlt64 m_Audience_Prb;   // l'audience probabiliste
		JFlt64 m_Audience_001;   // l'audience 1+ à   1 jour
		JFlt64 m_Audience_002;   // l'audience 1+ à   2 jours
		JFlt64 m_Audience_004;   // l'audience 1+ à   4 jours
		JFlt64 m_Audience_007;   // l'audience 1+ à   7 jours
		JFlt64 m_Audience_014;   // l'audience 1+ à  14 jours
		JFlt64 m_Audience_030;   // l'audience 1+ à  30 jours
		JFlt64 m_Audience_060;   // l'audience 1+ à  60 jours
		JFlt64 m_Audience_090;   // l'audience 1+ à  90 jours
		JFlt64 m_Audience_120;   // l'audience 1+ à 120 jours
		JFlt64 m_Audience_180;   // l'audience 1+ à 180 jours
		JFlt64 m_Audience_270;   // l'audience 1+ à 270 jours
		JFlt64 m_Audience_365;   // l'audience 1+ à 365 jours
		JInt32 m_Lambda;         // la paramètre Lambda
		JFlt64 m_Delta;          // le distance quadratique
		JFlt64 m_Alpha;          // le paramètre Alpha
		JFlt64 m_Beta;           // le paramètre Beta
		JFlt64 m_Prob;           // la probabilité
		JFlt64 m_Disp;           // la dispersion
	};

private:
	// la fonction pour calculer le Lambda
	JInt32 OnComputeLambda(const JMTTBLVolContacts & TBLVolContacts) const;

	// la fonction pour calculer les résultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fgEtalonnage, JResultats & Resultats) const;

	// la fonction pour calculer les paramètres
	JBool OnComputeParametres(JResultats & Resultats, JBool fgEtalonnage) const;

private:
	// l'opérateur pour recopier les éléments
	JMTFR03BetaBinomial & operator =(const JMTFR03BetaBinomial & Source);

	// le constructeur copie
	JMTFR03BetaBinomial(const JMTFR03BetaBinomial & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
