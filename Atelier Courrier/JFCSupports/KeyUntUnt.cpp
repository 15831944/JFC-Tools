#include "StdAfx.h"
#include ".\KeyUntUnt.h"

////////////////////
// les constructeurs

CKeyUntUnt::CKeyUntUnt()
{
	// on réinitialise l'élément
	m_Id1		= 0;
	m_Id2		= 0;
}

CKeyUntUnt::CKeyUntUnt(const CKeyUntUnt & Source)
{
	// on recopie l'élément
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool CKeyUntUnt::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_Id1 && m_Id2);
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

CKeyUntUnt & CKeyUntUnt::operator =(const CKeyUntUnt & Source)
{
	// on recopie l'élément
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool CKeyUntUnt::operator <(const CKeyUntUnt & Reference) const
{
	if (m_Id1 < Reference.m_Id1) return (true);
	if (m_Id1 > Reference.m_Id1) return (false);

	if (m_Id2 < Reference.m_Id2) return (true);
	return (false);
}

JBool CKeyUntUnt::operator >(const CKeyUntUnt & Reference) const
{
	if (m_Id1 > Reference.m_Id1) return (true);
	if (m_Id1 < Reference.m_Id1) return (false);

	if (m_Id2 > Reference.m_Id2) return (true);
	return (false);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid CKeyUntUnt::Reset()
{
	// on réinitialise l'élément
	m_Id1		= 0;
	m_Id2		= 0;
}

/////////////////
// le destructeur

CKeyUntUnt::~CKeyUntUnt()
{
	// on ne fait rien
}
