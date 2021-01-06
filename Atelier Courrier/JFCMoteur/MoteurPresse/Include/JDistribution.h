//
// Fichier: JDistribution.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JDISTRIBUTION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JDISTRIBUTION_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

// on d�finit le nombre de classes
#define JDISTRIBUTION_MAX (20l)

class JDistribution
{
public:
	// les constructeurs
	JDistribution();
	JDistribution(const JDistribution & Source);

	// l'op�rateur pour recopier les �l�ments
	JDistribution & operator =(const JDistribution & Source);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64x & Item(JInt32 K) const;

	// la fonction pour r�initialiser les �l�ments
	JVoid Reset();

	// le destructeur
	~JDistribution();

private:
	// Les moteurs d'�valuation remplissent directement les distributions
	friend class JMTFR03FullBinomial;
	friend class JMTFR03FullBinomialCum;
	friend class JMTFR03BetaBinomial;

private:
	JFlt64x m_Items[JDISTRIBUTION_MAX]; // les �l�ments
};

// fin de l'inclusion conditionnelle
#endif
