#include "StdAfx.h"
#include ".\KeySupportDate.h"

////////////////////
// les constructeurs

CKeySupportDate::CKeySupportDate()
{
	// on réinitialise l'élément
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Date.Reset();
}

CKeySupportDate::CKeySupportDate(const CKeySupportDate & Source)
{
	// on recopie l'élément
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Date			= Source.m_Date;	// la date
}

////////////////////////////////////
// la fonction pour tester l'élément
JBool CKeySupportDate::IsValid() const
{
	// on renvoie la validité de l'élément
	return ( m_IdSupport && m_Date.IsValid() );
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément
JVoid CKeySupportDate::Recv(JStream & Stream, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		Stream			.Recv(m_IdSupport);	// l'identifiant du Support
		m_Date			.Recv(Stream);	// la date
		break;

	default:
		TRACE("CParution   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
}


//////////////////////////////////////
// l'opérateur pour recopier l'élément
CKeySupportDate & CKeySupportDate::operator =(const CKeySupportDate & Source)
{
	// on recopie l'élément
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Date			= Source.m_Date;	// la date

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments
JBool CKeySupportDate::operator <(const CKeySupportDate & Reference) const
{
	// on compare la composante id support
	if (m_IdSupport < Reference.m_IdSupport) return (true);
	if (m_IdSupport > Reference.m_IdSupport) return (false);

	// on compare la composante id parent
	if (m_Date < Reference.m_Date) return (true);
	return (false);
}

JBool CKeySupportDate::operator >(const CKeySupportDate & Reference) const
{
	// on compare la composante id support
	if (m_IdSupport > Reference.m_IdSupport) return (true);
	if (m_IdSupport < Reference.m_IdSupport) return (false);

	// on compare la composante id parent
	if (m_Date > Reference.m_Date) return (true);
	return (false);

}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid CKeySupportDate::Reset()
{
	// on réinitialise l'élément
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Date.Reset();			// la date
}

/////////////////
// le destructeur

CKeySupportDate::~CKeySupportDate()
{
	// on ne fait rien
}
