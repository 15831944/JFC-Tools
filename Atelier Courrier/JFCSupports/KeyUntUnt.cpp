#include "StdAfx.h"
#include ".\KeyUntUnt.h"

////////////////////
// les constructeurs

CKeyUntUnt::CKeyUntUnt()
{
	// on r�initialise l'�l�ment
	m_Id1		= 0;
	m_Id2		= 0;
}

CKeyUntUnt::CKeyUntUnt(const CKeyUntUnt & Source)
{
	// on recopie l'�l�ment
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool CKeyUntUnt::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_Id1 && m_Id2);
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

CKeyUntUnt & CKeyUntUnt::operator =(const CKeyUntUnt & Source)
{
	// on recopie l'�l�ment
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

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
// la fonction pour r�initialiser l'�l�ment

JVoid CKeyUntUnt::Reset()
{
	// on r�initialise l'�l�ment
	m_Id1		= 0;
	m_Id2		= 0;
}

/////////////////
// le destructeur

CKeyUntUnt::~CKeyUntUnt()
{
	// on ne fait rien
}
