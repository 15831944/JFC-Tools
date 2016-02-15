// ArbreREGIONS.cpp: implementation of the CArbreREGIONS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArbreREGIONS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArbreREGIONS::CArbreREGIONS()
{

}

CArbreREGIONS::~CArbreREGIONS()
{

}

CArbreREGIONS::CArbreREGIONS(const CArbreREGIONS &Data)
{
	*this=Data;
}

CArbreREGIONS & CArbreREGIONS::operator=(const CArbreREGIONS &Data)
{
	m_Tronc=Data.m_Tronc;
	m_LibRegionCourt=Data.m_LibRegionCourt;
	m_LibRegion=Data.m_LibRegion;
	m_LibRegionLong=Data.m_LibRegionLong;
	m_CodeZone=Data.m_CodeZone;
	m_IndexRegion=Data.m_IndexRegion;
	m_NbBranche=Data.m_NbBranche;
	m_Branches=Data.m_Branches;
	m_IndexRegionReduit=Data.m_IndexRegionReduit;
	return *this;
}