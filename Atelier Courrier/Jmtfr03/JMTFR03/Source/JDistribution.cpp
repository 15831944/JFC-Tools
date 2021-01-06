//
// Fichier: JDistribution.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on inclut les définitions nécessaires
#include "JDistribution.h"

////////////////////
// les constructeurs

JDistribution::JDistribution()
{
	// on ne fait rien
}

JDistribution::JDistribution(const JDistribution & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie les éléments de la source
	for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx] = Source.m_Items[Idx];
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JDistribution & JDistribution::operator =(const JDistribution & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie les éléments de la source
		for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx] = Source.m_Items[Idx];
	}

	// on renvoie notre référence
	return (*this);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JFlt64x & JDistribution::Item(JInt32 K) const
{
	// on vérifie la validité de l'indice
	if (K < 1 || K > JDISTRIBUTION_MAX) throw JInvalidCall::GetInstance();

	// on renvoie l'élément
	return (m_Items[K - 1]);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JDistribution::Reset()
{
	// on réinitialise les éléments
	for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx].Reset();
}

/////////////////
// le destructeur

JDistribution::~JDistribution()
{
	// on ne fait rien
}
