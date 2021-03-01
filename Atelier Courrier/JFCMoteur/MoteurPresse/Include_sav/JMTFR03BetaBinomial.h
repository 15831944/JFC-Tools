//
// Fichier: JMTFR03BetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    01/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_BETA_BINOMIAL_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_BETA_BINOMIAL_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03Logs.h"

// on inclut les d�finitions n�cessaires
#include "JDistribution.h"

// on inclut les d�finitions n�cessaires
#include "JMTTBLVolContacts.h"

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

class JMTFR03BetaBinomial
{
public:
	// le constructeur
	JMTFR03BetaBinomial();

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fgEtalonnage, JMTFR03Logs* pLogs = 0);

	// la fonction pour calculer la distribution
	JVoid MakeDistribution(JFlt64 VolLectures, JDistribution & Distribution) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03BetaBinomial & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03BetaBinomial();

private:
	// on d�finit les r�sultats
	class JResultats
	{
	public:
		JFlt64 m_Effectif;       // l'effectif de r�f�rence
		JInt32 m_NbVariables;    // le nombre de variables d'�talonnage
		JFlt64 m_VolLectures1Jr; // le volume de lectures � 1 jour
		JFlt64 m_VolLecturesPrb; // le volume de lectures probabiliste
		JBool  m_Validite_001;   // la validit� de l'audience �   1 jour
		JBool  m_Validite_002;   // la validit� de l'audience �   2 jours
		JBool  m_Validite_004;   // la validit� de l'audience �   4 jours
		JBool  m_Validite_007;   // la validit� de l'audience �   7 jours
		JBool  m_Validite_014;   // la validit� de l'audience �  14 jours
		JBool  m_Validite_030;   // la validit� de l'audience �  30 jours
		JBool  m_Validite_060;   // la validit� de l'audience �  60 jours
		JBool  m_Validite_090;   // la validit� de l'audience �  90 jours
		JBool  m_Validite_120;   // la validit� de l'audience � 120 jours
		JBool  m_Validite_180;   // la validit� de l'audience � 180 jours
		JBool  m_Validite_270;   // la validit� de l'audience � 270 jours
		JFlt64 m_Audience_Prb;   // l'audience probabiliste
		JFlt64 m_Audience_001;   // l'audience 1+ �   1 jour
		JFlt64 m_Audience_002;   // l'audience 1+ �   2 jours
		JFlt64 m_Audience_004;   // l'audience 1+ �   4 jours
		JFlt64 m_Audience_007;   // l'audience 1+ �   7 jours
		JFlt64 m_Audience_014;   // l'audience 1+ �  14 jours
		JFlt64 m_Audience_030;   // l'audience 1+ �  30 jours
		JFlt64 m_Audience_060;   // l'audience 1+ �  60 jours
		JFlt64 m_Audience_090;   // l'audience 1+ �  90 jours
		JFlt64 m_Audience_120;   // l'audience 1+ � 120 jours
		JFlt64 m_Audience_180;   // l'audience 1+ � 180 jours
		JFlt64 m_Audience_270;   // l'audience 1+ � 270 jours
		JFlt64 m_Audience_365;   // l'audience 1+ � 365 jours
		JInt32 m_Lambda;         // la param�tre Lambda
		JFlt64 m_Delta;          // le distance quadratique
		JFlt64 m_Alpha;          // le param�tre Alpha
		JFlt64 m_Beta;           // le param�tre Beta
		JFlt64 m_Prob;           // la probabilit�
		JFlt64 m_Disp;           // la dispersion
	};

private:
	// la fonction pour calculer le Lambda
	JInt32 OnComputeLambda(const JMTTBLVolContacts & TBLVolContacts) const;

	// la fonction pour calculer les r�sultats
	JBool OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fgEtalonnage, JResultats & Resultats) const;

	// la fonction pour calculer les param�tres
	JBool OnComputeParametres(JResultats & Resultats, JBool fgEtalonnage) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03BetaBinomial & operator =(const JMTFR03BetaBinomial & Source);

	// le constructeur copie
	JMTFR03BetaBinomial(const JMTFR03BetaBinomial & Source);

private:
	JArray<JResultats> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
