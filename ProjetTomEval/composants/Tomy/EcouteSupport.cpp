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
	// on initialise les éléments
	m_NbEcoutes    = 0;
	m_pNoIndividus = NULL;
	m_pValEcoutes  = NULL;
}

void CEcouteSupport::SetNbEcoutes(long NbEcoutes)
{
	// on libère les éléments
	delete [] m_pValEcoutes;
	delete [] m_pNoIndividus;

	// on teste si on doit allouer des écoutes
	if (NbEcoutes > 0)
	{
		// on alloue les éléments
		m_pNoIndividus = new long          [NbEcoutes];
		m_pValEcoutes  = new unsigned short[NbEcoutes];
	}
	else
	{
		// on annule les éléments
		m_pNoIndividus = NULL;
		m_pValEcoutes  = NULL;
	}

	// on mémorise le nombre d'écoutes
	m_NbEcoutes = NbEcoutes;
}

CEcouteSupport::~CEcouteSupport()
{
	// on libère les éléments
	delete [] m_pValEcoutes;
	delete [] m_pNoIndividus;

	// on réinitialise les éléments
	m_pValEcoutes  = NULL;
	m_pNoIndividus = NULL;
	m_NbEcoutes    = 0;
}
