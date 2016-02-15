// SourceDEF.cpp: implementation of the CSourceDEF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SourceDEF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSourceDEF::CSourceDEF()
{
	m_Nom=_T("");
	m_Index=0;
	m_NbModaGeofrac=0;
	m_TabModaGeofrac.RemoveAll();
	m_NrRegionGeofrac=0;
    m_Vagues.RemoveAll();
    m_NrIndexVague.RemoveAll();
}

CSourceDEF::~CSourceDEF()
{

}

CSourceDEF::CSourceDEF(const CSourceDEF &Data)
{
	*this=Data;
}

CSourceDEF & CSourceDEF::operator=(const CSourceDEF &Data)
{
	m_Nom=Data.m_Nom;
	m_Index=Data.m_Index;
	m_NbModaGeofrac=Data.m_NbModaGeofrac;
	m_TabModaGeofrac.Copy(Data.m_TabModaGeofrac);
	m_NrRegionGeofrac=Data.m_NrRegionGeofrac;
    m_Vagues.Copy(Data.m_Vagues);
    m_NrIndexVague.Copy(Data.m_NrIndexVague);

	return *this;
}