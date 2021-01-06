//
// Fichier: JATPKEYPeriodeAction.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPKEYPeriodeAction & JATPKEYPeriodeAction::operator =(const JATPKEYPeriodeAction & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////
// les fonctions pour manipuler la p�riode

JVoid JATPKEYPeriodeAction::SetPeriode(JDate DateD, JDate DateF)
{
	// on v�rifie la validit� des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on v�rifie la coh�rence des dates
		if (DateF < DateD) throw JInvalidCall::GetInstance();
	}

	// on m�morise les dates
	m_DateD = DateD;
	m_DateF = DateF;
}

JDate JATPKEYPeriodeAction::GetDateD() const
{
	// on renvoie la date de d�but
	return (m_DateD);
}

JDate JATPKEYPeriodeAction::GetDateF() const
{
	// on renvoie la date de fin
	return (m_DateF);
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JATPKEYPeriodeAction::Send(JStream & Stream) const
{
	// on s�rialise l'�l�ment
	m_DateD.Send(Stream);
	m_DateF.Send(Stream);
}

JVoid JATPKEYPeriodeAction::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JDate DateD; DateD.Recv(Stream);
	JDate DateF; DateF.Recv(Stream);

	// on v�rifie la validit� des dates
	if (DateD.IsValid() != DateF.IsValid()) throw JBadSchema::GetInstance();

	// on v�rifie la coh�rence des dates
	if (DateD.IsValid() && DateF.IsValid()) { if (DateF < DateD) throw JBadSchema::GetInstance(); }

	// on recopie l'�l�ment
	m_DateD = DateD;
	m_DateF = DateF;
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPKEYPeriodeAction::Swap(JATPKEYPeriodeAction & Source)
{
	// on permute l'�l�ment
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
