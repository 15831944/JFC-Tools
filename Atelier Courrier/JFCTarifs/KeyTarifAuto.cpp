#include "StdAfx.h"
#include ".\KeyTarifAuto.h"

////////////////////
// les constructeurs

CKeyTarifAuto::CKeyTarifAuto()
{
	// on r�initialise l'�l�ment
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Version		= 0;		// l'Identifiant de la version
	m_IdFormat		= 0;		// l'Identifiant du format	
}

CKeyTarifAuto::CKeyTarifAuto(const CKeyTarifAuto & Source)
{
	// on recopie l'�l�ment
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Version		= Source.m_Version;		// l'Identifiant de la version
	m_IdFormat		= Source.m_IdFormat;	// l'Identifiant du format	
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool CKeyTarifAuto::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSupport && m_Version && m_IdFormat);
}

//////////////////////////////////////////
// la fonction pour s�rialiser l'�l�ment
JVoid CKeyTarifAuto::Recv(JStream & Stream, JInt32 Version)
{
	// on s�rialise l'�l�ment
	switch(Version)
	{
	case 1:
		Stream			.Recv(m_IdSupport);	// l'identifiant du Support
		Stream			.Recv(m_Version);	// l'identifiant de la version
		Stream			.Recv(m_IdFormat);	// l'Identifiant du format	
		break;

	default:
		TRACE("CParution   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
}


//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment
CKeyTarifAuto & CKeyTarifAuto::operator =(const CKeyTarifAuto & Source)
{
	// on recopie l'�l�ment
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Version		= Source.m_Version;		// l'identifiant de la version
	m_IdFormat		= Source.m_IdFormat;	// l'Identifiant du format	

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments
JBool CKeyTarifAuto::operator <(const CKeyTarifAuto & Reference) const
{
	// on compare la composante id support
	if (m_IdSupport < Reference.m_IdSupport) return (true);
	if (m_IdSupport > Reference.m_IdSupport) return (false);

	// on compare la composante version
	if (m_Version < Reference.m_Version) return (true);
	if (m_Version > Reference.m_Version) return (false);

	// on compare la composante Format
	if (m_IdFormat < Reference.m_IdFormat) return (true);
	return (false);
}

JBool CKeyTarifAuto::operator >(const CKeyTarifAuto & Reference) const
{
	// on compare la composante id support
	if (m_IdSupport > Reference.m_IdSupport) return (true);
	if (m_IdSupport < Reference.m_IdSupport) return (false);

	// on compare la composante version
	if (m_Version > Reference.m_Version) return (true);
	if (m_Version < Reference.m_Version) return (false);

	// on compare la composante Format
	if (m_IdFormat > Reference.m_IdFormat) return (true);
	return (false);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid CKeyTarifAuto::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Version		= 0;		// l'Identifiant de la version	
	m_IdFormat		= 0;		// l'Identifiant du format	
}

/////////////////
// le destructeur

CKeyTarifAuto::~CKeyTarifAuto()
{
	// on ne fait rien
}
