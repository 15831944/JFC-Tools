#include "StdAfx.h"
#include ".\KeyTarifAuto.h"

////////////////////
// les constructeurs

CKeyTarifAuto::CKeyTarifAuto()
{
	// on réinitialise l'élément
	m_IdSupport		= 0;		// l'identifiant du Support
	m_Version		= 0;		// l'Identifiant de la version
	m_IdFormat		= 0;		// l'Identifiant du format	
}

CKeyTarifAuto::CKeyTarifAuto(const CKeyTarifAuto & Source)
{
	// on recopie l'élément
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Version		= Source.m_Version;		// l'Identifiant de la version
	m_IdFormat		= Source.m_IdFormat;	// l'Identifiant du format	
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool CKeyTarifAuto::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSupport && m_Version && m_IdFormat);
}

//////////////////////////////////////////
// la fonction pour sérialiser l'élément
JVoid CKeyTarifAuto::Recv(JStream & Stream, JInt32 Version)
{
	// on sérialise l'élément
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
// l'opérateur pour recopier l'élément
CKeyTarifAuto & CKeyTarifAuto::operator =(const CKeyTarifAuto & Source)
{
	// on recopie l'élément
	m_IdSupport		= Source.m_IdSupport;	// l'identifiant du Support
	m_Version		= Source.m_Version;		// l'identifiant de la version
	m_IdFormat		= Source.m_IdFormat;	// l'Identifiant du format	

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments
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
// la fonction pour réinitialiser l'élément

JVoid CKeyTarifAuto::Reset()
{
	// on réinitialise l'élément
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
