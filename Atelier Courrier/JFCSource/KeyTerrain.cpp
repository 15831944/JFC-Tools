#include "StdAfx.h"
#include ".\KeyTerrain.h"

CKeyTerrain::CKeyTerrain(void)
{
	m_IDSource	= 0;
	m_IDTerrain	= 0;
}

CKeyTerrain::CKeyTerrain(const JUnt32 IDSource,const JUnt32 IDTerrain)
{
	m_IDSource	= IDSource;
	m_IDTerrain	= IDTerrain;
}

CKeyTerrain::CKeyTerrain(const CKeyTerrain & Source)
{
	// on recopie l'élément
	m_IDSource	= Source.m_IDSource;
	m_IDTerrain	= Source.m_IDTerrain;
}

CKeyTerrain::~CKeyTerrain(void)
{
}

void CKeyTerrain::Set (JUnt32 IDSource, JUnt32 IDTerrain)
{
	m_IDSource	= IDSource;
	m_IDTerrain	= IDTerrain;
}

void CKeyTerrain::SetSource (JUnt32 IDSource)
{
	m_IDSource = IDSource;
}

void CKeyTerrain::SetTerrain (JUnt32 IDTerrain)
{
	m_IDTerrain = IDTerrain;
}

bool CKeyTerrain::IsValid()
{
	return (m_IDSource && m_IDTerrain);
}

CKeyTerrain & CKeyTerrain::operator =(const CKeyTerrain & Source)
{
	// on recopie l'élément
	m_IDSource	= Source.m_IDSource;
	m_IDTerrain	= Source.m_IDTerrain;

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool CKeyTerrain::operator <(const CKeyTerrain & Reference) const
{
	if (m_IDSource < Reference.m_IDSource) return (true);
	if (m_IDSource > Reference.m_IDSource) return (false);

	if (m_IDTerrain < Reference.m_IDTerrain) return (true);
	return (false);
}

JBool CKeyTerrain::operator >(const CKeyTerrain & Reference) const
{
	if (m_IDSource > Reference.m_IDSource) return (true);
	if (m_IDSource < Reference.m_IDSource) return (false);

	if (m_IDTerrain > Reference.m_IDTerrain) return (true);
	return (false);
}

