// ITitre.cpp: implementation of the ITitre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ITitre.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ITitre::ITitre(	JUnt32 Periodicite, JUnt32 IDAudience, JBool bAgregat)
{
	m_Periodicite = Periodicite;
	m_IDAudience = IDAudience;
	m_bAgregat = bAgregat;
	m_cplFixe = NULL;
}

ITitre::ITitre()
{
	m_Periodicite = 0;
	m_IDAudience = 0;
	m_cplFixe = NULL;
}

ITitre::~ITitre()
{
	if(m_cplFixe)
		delete 	m_cplFixe;
}

JBool ITitre::IsTitre()
{
	return true;
}

JUnt32 ITitre::GetPeriodicite() const
{
	return m_Periodicite;
}

JUnt32 ITitre::GetIDAudience() const
{
	return m_IDAudience;
}

JBool ITitre::IsAgregat() const
{
	return m_bAgregat;
}

