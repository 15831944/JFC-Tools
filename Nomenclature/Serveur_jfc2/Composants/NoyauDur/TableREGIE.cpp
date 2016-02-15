// TableREGIE.cpp: implementation of the CTableREGIE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableREGIE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableREGIE::CTableREGIE()
{

}

CTableREGIE::~CTableREGIE()
{

}

CTableREGIE::CTableREGIE(const CTableREGIE &Data)
{
	*this=Data;
}

CTableREGIE & CTableREGIE::operator=(const CTableREGIE &Data)
{	
	m_NrUnique=Data.m_NrUnique;
	m_Nom=Data.m_Nom;
	m_Libelle=Data.m_Libelle;
	m_Remise=Data.m_Remise;
	m_NrOrdre=Data.m_NrOrdre;
	m_Logo=Data.m_Logo;
	m_DateDebut=Data.m_DateDebut;
	m_DateFin=Data.m_DateFin;
	return *this;
}