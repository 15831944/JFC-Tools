// TableREGION.cpp: implementation of the CTableREGION class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableREGION.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableREGION::CTableREGION()
{

}

CTableREGION::~CTableREGION()
{

}

CTableREGION::CTableREGION(const CTableREGION &Data)
{
	*this=Data;
}

CTableREGION & CTableREGION::operator=(const CTableREGION &Data)
{
	m_NrUnique=Data.m_NrUnique;
	m_Code=Data.m_Code;
	m_Libelle=Data.m_Libelle;
	m_LibelleLong=Data.m_LibelleLong;
	m_LibelleCourt=Data.m_LibelleCourt;
	m_LibelleMini=Data.m_LibelleMini;
	return *this;
}