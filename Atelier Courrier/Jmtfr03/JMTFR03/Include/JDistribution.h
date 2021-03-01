//
// Fichier: JDistribution.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JDISTRIBUTION_H

// on définit la macro pour éviter les inclusions multiples
#define JDISTRIBUTION_H

// on inclut les définitions nécessaires
#include "JLib.h"

// on définit le nombre de classes
#define JDISTRIBUTION_MAX (20l)

class JDistribution
{
public:
	// les constructeurs
	JDistribution();
	JDistribution(const JDistribution & Source);

	// l'opérateur pour recopier les éléments
	JDistribution & operator =(const JDistribution & Source);

	// la fonction pour récupérer les éléments
	const JFlt64x & Item(JInt32 K) const;

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JDistribution();

private:
	// Les moteurs d'évaluation remplissent directement les distributions
	friend class JMTFR03FullBinomial;
	friend class JMTFR03FullBinomialCum;
	friend class JMTFR03BetaBinomial;

private:
	JFlt64x m_Items[JDISTRIBUTION_MAX]; // les éléments
};

// fin de l'inclusion conditionnelle
#endif
