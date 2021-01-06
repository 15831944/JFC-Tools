//
// Fichier: JDistribution.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on inclut les d�finitions n�cessaires
#include "JDistribution.h"

////////////////////
// les constructeurs

JDistribution::JDistribution()
{
	// on ne fait rien
}

JDistribution::JDistribution(const JDistribution & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie les �l�ments de la source
	for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx] = Source.m_Items[Idx];
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JDistribution & JDistribution::operator =(const JDistribution & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie les �l�ments de la source
		for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx] = Source.m_Items[Idx];
	}

	// on renvoie notre r�f�rence
	return (*this);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64x & JDistribution::Item(JInt32 K) const
{
	// on v�rifie la validit� de l'indice
	if (K < 1 || K > JDISTRIBUTION_MAX) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment
	return (m_Items[K - 1]);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JDistribution::Reset()
{
	// on r�initialise les �l�ments
	for (JInt32 Idx = 0; Idx < JDISTRIBUTION_MAX; Idx += 1) m_Items[Idx].Reset();
}

/////////////////
// le destructeur

JDistribution::~JDistribution()
{
	// on ne fait rien
}
