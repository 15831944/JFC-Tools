// TableMEDIA.cpp: implementation of the CTableMEDIA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableMEDIA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableMEDIA::CTableMEDIA()
{

}

CTableMEDIA::~CTableMEDIA()
{

}

CTableMEDIA::CTableMEDIA(const CTableMEDIA &Data)
{
	*this=Data;
}

CTableMEDIA & CTableMEDIA::operator=(const CTableMEDIA &Data)
{
	m_Nom=Data.m_Nom;
	m_Libelle=Data.m_Libelle;
	return *this;
}
