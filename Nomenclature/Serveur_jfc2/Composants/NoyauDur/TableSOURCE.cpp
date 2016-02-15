// TableSOURCE.cpp: implementation of the CTableSOURCE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableSOURCE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableSOURCE::CTableSOURCE()
{

}

CTableSOURCE::~CTableSOURCE()
{

}

CTableSOURCE::CTableSOURCE(const CTableSOURCE &Data)
{
	*this=Data;
}

CTableSOURCE & CTableSOURCE::operator=(const CTableSOURCE &Data)
{
	m_Num=Data.m_Num;
	m_Rep=Data.m_Rep;
	m_Structure=Data.m_Structure;
	m_Media=Data.m_Media;
	m_Role=Data.m_Role;
	m_Nature=Data.m_Nature;
	m_Type=Data.m_Type;
	m_Date=Data.m_Date;
	m_Grp=Data.m_Grp;
	m_NomLong=Data.m_NomLong;
	m_NomCourt=Data.m_NomCourt;
	m_Occupe=Data.m_Occupe;
	m_fTypeUtilisation=Data.m_fTypeUtilisation;
	return *this;
}

