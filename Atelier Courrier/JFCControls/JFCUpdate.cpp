// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCUpdate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// les constructeurs
//////////////////////////////////////////////////////////////////////

JFCUpdate::JFCUpdate()
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

JFCUpdate::JFCUpdate(long IdMessage)
{
	// on vérifie la validité du message
	if (IdMessage <= 0) throw JInvalidCall::GetInstance();

	// on mémorise l'identifiant du message
	m_IdMessage = IdMessage;
}

JFCUpdate::JFCUpdate(const JFCUpdate & Source)
{
	// on vérifie l'existance de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'identifiant du message
	m_IdMessage = Source.m_IdMessage;
}

//////////////////////////////////////////////////////////////////////
// les opérateurs pour recopier les messages
//////////////////////////////////////////////////////////////////////

JFCUpdate & JFCUpdate::operator =(long IdMessage)
{
	// on vérifie la validité du message
	if (IdMessage <= 0) throw JInvalidCall::GetInstance();

	// on mémorise l'identifiant du message
	m_IdMessage = IdMessage;

	// on renvoie notre référence
	return (*this);
}

JFCUpdate & JFCUpdate::operator =(const JFCUpdate & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'identifiant du message
		m_IdMessage = Source.m_IdMessage;
	}

	// on renvoie notre référence
	return (*this);
}

//////////////////////////////////////////////////////////////////////
// les opérateurs pour comparer les messages
//////////////////////////////////////////////////////////////////////

bool JFCUpdate::operator ==(const JFCUpdate & Reference) const
{
	// on compare les identifiants des messages
	return (m_IdMessage == Reference.m_IdMessage);
}

bool JFCUpdate::operator !=(const JFCUpdate & Reference) const
{
	// on compare les identifiants des messages
	return (m_IdMessage != Reference.m_IdMessage);
}

//////////////////////////////////////////////////////////////////////
// le destructeur
//////////////////////////////////////////////////////////////////////

JFCUpdate::~JFCUpdate()
{
	// on ne fait rien
}
