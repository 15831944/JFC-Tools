//
// Fichier: JATPAFFZoomPlan.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPAFFZoomPlan.h"

////////////////////
// les constructeurs

JATPAFFZoomPlan::JATPAFFZoomPlan()
{
	m_bVisible = false;
	// on ne fait rien
}

JATPAFFZoomPlan::JATPAFFZoomPlan(const JATPAFFZoomPlan & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

void JATPAFFZoomPlan::SetVisible(JBool bVisible)
{
	m_bVisible = bVisible;
}

JBool JATPAFFZoomPlan::IsVisible()
{
	return m_bVisible;
}

	/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPAFFZoomPlan & JATPAFFZoomPlan::operator =(const JATPAFFZoomPlan & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

///////////////////////////////////////////////////
// les fonctions pour manipuler l'affichage du ZoomPlan

JVoid JATPAFFZoomPlan::SetAffZoomPlan(JUnt32 IdxHypothese, const JList<CInsKey> & List)
{
	// on initialise les identifiants et la date
	m_IdxHypothese  = IdxHypothese;
	m_KeyList.Reset();
	for( List.MoveFirst(); List.IsValid(); List.MoveNext())
		m_KeyList.AddTail() = List.GetItem(); 
}

JVoid	JATPAFFZoomPlan::SetAffZoomPlan(JUnt32 IdxHypothese, const CInsKey & Key)
{
	// on initialise les identifiants et la date
	m_IdxHypothese  = IdxHypothese;
	m_KeyList.Reset();
	m_KeyList.AddTail() = Key; 
}

JUnt32 JATPAFFZoomPlan::GetIdxHypothese() const
{
	// on renvoie l'identifiant de l'hypoth�se
	return (m_IdxHypothese);
}

const JList<CInsKey> & JATPAFFZoomPlan::GetKeyList() const
{
	// on renvoie le Insertion Key
	return m_KeyList;	
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPAFFZoomPlan::Swap(JATPAFFZoomPlan & Source)
{
	// on permute l'�l�ment
	JUnt32 IdxHypothese		= m_IdxHypothese;
	m_IdxHypothese			= Source.m_IdxHypothese;
	Source.m_IdxHypothese	= IdxHypothese;

	m_KeyList.Swap(Source.m_KeyList);
}

//////////////////////////////////////////////
// la fonction pour r�initialiser les �l�ments

JVoid JATPAFFZoomPlan::Reset()
{
	// on r�initialise l'�l�ment
	m_IdxHypothese    = 0;
	m_KeyList.Reset();
}

/////////////////
// le destructeur

JATPAFFZoomPlan::~JATPAFFZoomPlan()
{
	// on ne fait rien
}
