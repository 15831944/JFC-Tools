//
// Fichier: JATPKEYPeriodeAction.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPKEYPeriodeAction.h"

////////////////////
// les constructeurs

JATPKEYPeriodeAction::JATPKEYPeriodeAction()
{
	// on ne fait rien
}

JATPKEYPeriodeAction::JATPKEYPeriodeAction(const JATPKEYPeriodeAction & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPKEYPeriodeAction & JATPKEYPeriodeAction::operator =(const JATPKEYPeriodeAction & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////
// les fonctions pour manipuler la période

JVoid JATPKEYPeriodeAction::SetPeriode(JDate DateD, JDate DateF)
{
	// on vérifie la validité des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on vérifie la cohérence des dates
		if (DateF < DateD) throw JInvalidCall::GetInstance();
	}

	// on mémorise les dates
	m_DateD = DateD;
	m_DateF = DateF;
}

JDate JATPKEYPeriodeAction::GetDateD() const
{
	// on renvoie la date de début
	return (m_DateD);
}

JDate JATPKEYPeriodeAction::GetDateF() const
{
	// on renvoie la date de fin
	return (m_DateF);
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JATPKEYPeriodeAction::Send(JStream & Stream) const
{
	// on sérialise l'élément
	m_DateD.Send(Stream);
	m_DateF.Send(Stream);
}

JVoid JATPKEYPeriodeAction::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JDate DateD; DateD.Recv(Stream);
	JDate DateF; DateF.Recv(Stream);

	// on vérifie la validité des dates
	if (DateD.IsValid() != DateF.IsValid()) throw JBadSchema::GetInstance();

	// on vérifie la cohérence des dates
	if (DateD.IsValid() && DateF.IsValid()) { if (DateF < DateD) throw JBadSchema::GetInstance(); }

	// on recopie l'élément
	m_DateD = DateD;
	m_DateF = DateF;
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JATPKEYPeriodeAction::Swap(JATPKEYPeriodeAction & Source)
{
	// on permute l'élément
	JDate DateD		= m_DateD;
	m_DateD			= Source.m_DateD;
	Source.m_DateD	= DateD;

	JDate DateF		= m_DateF;
	m_DateF			= Source.m_DateF;
	Source.m_DateF	= DateF;
}

/////////////////
// le destructeur

JATPKEYPeriodeAction::~JATPKEYPeriodeAction()
{
	// on ne fait rien
}
