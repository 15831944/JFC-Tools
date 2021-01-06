#include "StdAfx.h"
#include ".\keymoteur.h"

CKeyMoteur::CKeyMoteur(void)
{
	m_IDSource	= 0;
	m_IDTerrain	= 0;
	m_IDItem	= 0;
}

CKeyMoteur::CKeyMoteur(JUnt32 IDSource, JUnt32 IDTerrain, JUnt32 IDItem)
{
	m_IDSource	= IDSource;
	m_IDTerrain	= IDTerrain;
	m_IDItem	= IDItem;
}

CKeyMoteur::~CKeyMoteur(void)
{
}

void CKeyMoteur::Set (JUnt32 IDSource, JUnt32 IDTerrain, JUnt32 IDItem)
{
	m_IDSource	= IDSource;
	m_IDTerrain	= IDTerrain;
	m_IDItem	= IDItem;
}

void CKeyMoteur::SetSource (JUnt32 IDSource)
{
	m_IDSource = IDSource;
}

void CKeyMoteur::SetTerrain (JUnt32 IDTerrain)
{
	m_IDTerrain = IDTerrain;
}

void CKeyMoteur::SetItem (JUnt32 IDItem)
{
	m_IDItem = IDItem;
}

bool CKeyMoteur::IsValid()
{
	if (m_IDSource	&&	m_IDTerrain	&&	m_IDItem )
		return true;
	else
		return false;
}

