// TableFAMILLE.cpp: implementation of the CTableFAMILLE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableFAMILLE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableFAMILLE::CTableFAMILLE()
{

}

CTableFAMILLE::~CTableFAMILLE()
{

}

CTableFAMILLE::CTableFAMILLE(const CTableFAMILLE &Data)
{
	*this=Data;
}

CTableFAMILLE & CTableFAMILLE::operator=(const CTableFAMILLE &Data)
{
	m_Numero=Data.m_Numero;
	m_Nom=Data.m_Nom;
	m_Libelle=Data.m_Libelle;
	return *this;
}