// TableCIBLE.cpp: implementation of the CTableCIBLE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableCIBLE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableCIBLE::CTableCIBLE()
{
	m_Numero=0;
	m_NrSource=0;
	m_NrVague=0;
	m_Occupe=0;
	m_temoin=0;
	m_Blob=_T("");
	m_Nom=_T("");
}

CTableCIBLE::~CTableCIBLE()
{

}

CTableCIBLE::CTableCIBLE(const CTableCIBLE &Data)
{
	*this=Data;
}

CTableCIBLE & CTableCIBLE::operator=(const CTableCIBLE &Data)
{
	m_Numero=Data.m_Numero;
	m_NrSource=Data.m_NrSource;
	m_NrVague=Data.m_NrVague;
	m_Occupe=Data.m_Occupe;
	m_temoin=Data.m_temoin;
	m_Blob=Data.m_Blob;
	m_Nom=Data.m_Nom;

	return *this;
}

