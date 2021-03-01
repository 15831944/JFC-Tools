// JTitreCouplage.cpp: implementation of the JTitreCouplage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTitreCouplage.h"
#include "JSRCPresse.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTitreCouplage::JTitreCouplage()
{
}

JTitreCouplage::JTitreCouplage(JUnt32 Periodicite, JUnt32 IDAudience, JUnt32 nbInsert) : ITitre(Periodicite, IDAudience)
{
	m_nbInsert = nbInsert;
}

JTitreCouplage::~JTitreCouplage()
{
}

JUnt32 JTitreCouplage::GetNbInsert() const
{
	return m_nbInsert;
}
