//
// Fichier: JATPAFFResultPlan.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPAFFResultPlan.h"

////////////////////
// les constructeurs

JATPAFFResultPlan::JATPAFFResultPlan()
{
	// on initialise les �l�ments
	m_IdResultPlan        = (JUnt32)1;
}

JATPAFFResultPlan::JATPAFFResultPlan(const JATPAFFResultPlan & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPAFFResultPlan & JATPAFFResultPlan::operator =(const JATPAFFResultPlan & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////////////
// les fonctions pour manipuler le graphe des GRP

JVoid JATPAFFResultPlan::SetGraphGRPJour()
{
	// on modifie le ResultPlan
	m_IdResultPlan = 1;
}

JBool JATPAFFResultPlan::GetGraphGRPJour() const
{
	// on renvoie le ResultPlan
	return (m_IdResultPlan == 1);
}

/////////////////////////////////////////////////
// les fonctions pour manipuler le graphe des GRP

JVoid JATPAFFResultPlan::SetGraphGRPCumul()
{
	// on modifie le ResultPlan
	m_IdResultPlan = 3;
}

JBool JATPAFFResultPlan::GetGraphGRPCumul() const
{
	// on renvoie le ResultPlan
	return (m_IdResultPlan == 3);
}

////////////////////////////////////////////////
// les fonctions pour manipuler la courbe des C%

JVoid JATPAFFResultPlan::SetGraphCouvCumul()
{
	// on modifie le ResultPlan
	m_IdResultPlan = 2;
}

JBool JATPAFFResultPlan::GetGraphCouvCumul() const
{
	// on renvoie le ResultPlan
	return (m_IdResultPlan == 2);
}

////////////////////////////////////////////////
// les fonctions pour manipuler la courbe des C%

JVoid JATPAFFResultPlan::SetGraphCouvJour()
{
	// on modifie le ResultPlan
	m_IdResultPlan = 4;
}

JBool JATPAFFResultPlan::GetGraphCouvJour() const
{
	// on renvoie le ResultPlan
	return (m_IdResultPlan == 4);
}
JVoid JATPAFFResultPlan::Send(JStream & Stream)
{
	Stream.Send(m_IdResultPlan);
}

JVoid JATPAFFResultPlan::Recv(JStream & Stream)
{
	Stream.Recv(m_IdResultPlan);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPAFFResultPlan::Swap(JATPAFFResultPlan & Source)
{
	// on permute les �l�ments
	JUnt32 IdResultPlan		=	m_IdResultPlan;
	m_IdResultPlan			=	Source.m_IdResultPlan;
	Source.m_IdResultPlan   =	IdResultPlan;
}

/////////////////
// le destructeur

JATPAFFResultPlan::~JATPAFFResultPlan()
{
	// on ne fait rien
}
