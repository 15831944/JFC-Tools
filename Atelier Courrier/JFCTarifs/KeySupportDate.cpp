#include "StdAfx.h"
#include ".\KeySupportDate.h"

////////////////////
// les constructeurs

CKeySupportDate::CKeySupportDate()
{
	// on r�initialise l'�l�ment
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Date.Reset();
}

CKeySupportDate::CKeySupportDate(const CKeySupportDate & Source)
{
	// on recopie l'�l�ment
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Date			= Source.m_Date;	// la date
}

////////////////////////////////////
// la fonction pour tester l'�l�ment
JBool CKeySupportDate::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return ( m_IdSupport && m_Date.IsValid() );
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment
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
// l'op�rateur pour recopier l'�l�ment
CKeySupportDate & CKeySupportDate::operator =(const CKeySupportDate & Source)
{
	// on recopie l'�l�ment
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Date			= Source.m_Date;	// la date

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments
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
// la fonction pour r�initialiser l'�l�ment

JVoid CKeySupportDate::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Date.Reset();			// la date
}

/////////////////
// le destructeur

CKeySupportDate::~CKeySupportDate()
{
	// on ne fait rien
}
