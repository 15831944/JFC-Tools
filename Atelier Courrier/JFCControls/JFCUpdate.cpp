// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

JFCUpdate::JFCUpdate(long IdMessage)
{
	// on v�rifie la validit� du message
	if (IdMessage <= 0) throw JInvalidCall::GetInstance();

	// on m�morise l'identifiant du message
	m_IdMessage = IdMessage;
}

JFCUpdate::JFCUpdate(const JFCUpdate & Source)
{
	// on v�rifie l'existance de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'identifiant du message
	m_IdMessage = Source.m_IdMessage;
}

//////////////////////////////////////////////////////////////////////
// les op�rateurs pour recopier les messages
//////////////////////////////////////////////////////////////////////

JFCUpdate & JFCUpdate::operator =(long IdMessage)
{
	// on v�rifie la validit� du message
	if (IdMessage <= 0) throw JInvalidCall::GetInstance();

	// on m�morise l'identifiant du message
	m_IdMessage = IdMessage;

	// on renvoie notre r�f�rence
	return (*this);
}

JFCUpdate & JFCUpdate::operator =(const JFCUpdate & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'identifiant du message
		m_IdMessage = Source.m_IdMessage;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

//////////////////////////////////////////////////////////////////////
// les op�rateurs pour comparer les messages
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
