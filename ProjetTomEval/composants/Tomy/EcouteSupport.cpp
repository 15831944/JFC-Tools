// EcouteSupport.cpp: implementation of the CEcouteSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EcouteSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEcouteSupport::CEcouteSupport()
{
	// on initialise les �l�ments
	m_NbEcoutes    = 0;
	m_pNoIndividus = NULL;
	m_pValEcoutes  = NULL;
}

void CEcouteSupport::SetNbEcoutes(long NbEcoutes)
{
	// on lib�re les �l�ments
	delete [] m_pValEcoutes;
	delete [] m_pNoIndividus;

	// on teste si on doit allouer des �coutes
	if (NbEcoutes > 0)
	{
		// on alloue les �l�ments
		m_pNoIndividus = new long          [NbEcoutes];
		m_pValEcoutes  = new unsigned short[NbEcoutes];
	}
	else
	{
		// on annule les �l�ments
		m_pNoIndividus = NULL;
		m_pValEcoutes  = NULL;
	}

	// on m�morise le nombre d'�coutes
	m_NbEcoutes = NbEcoutes;
}

CEcouteSupport::~CEcouteSupport()
{
	// on lib�re les �l�ments
	delete [] m_pValEcoutes;
	delete [] m_pNoIndividus;

	// on r�initialise les �l�ments
	m_pValEcoutes  = NULL;
	m_pNoIndividus = NULL;
	m_NbEcoutes    = 0;
}
