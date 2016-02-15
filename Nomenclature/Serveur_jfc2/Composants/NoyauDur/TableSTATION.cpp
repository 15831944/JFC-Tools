// TableSTATION.cpp: implementation of the CTableSTATION class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableSTATION.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableSTATION::CTableSTATION()
{
	m_NrUnique=0;
	m_Nom=_T("");
	m_Media=' ';
	m_TypeSuppS=0;
	m_TypeMedia=0;
	m_HeureDebut=0;
	m_Tarif=' ';
	m_Libelle=_T("");
	m_NrStationTete=0;
	m_RegionTete=_T("");
	m_NrOrdre=0;
	m_Logo=_T("");
	m_Famille=0;
//	m_DateDebut=0;
//	m_DateFin=0;

	m_NbOption=0;
	m_Options.RemoveAll();

	m_NbLien=0;
	m_DateLien=0;
	m_NrUniqueRegie.RemoveAll();
	m_NrUniqueRegieValide=0;
	m_NbRegionPseudoPresent=0;
	m_NrRegionPseudoPresent.RemoveAll();
}

CTableSTATION::~CTableSTATION()
{
}

CTableSTATION::CTableSTATION(const CTableSTATION &Data)
{
	*this=Data;
}

CTableSTATION & CTableSTATION::operator=(const CTableSTATION &Data)
{
	m_NrUnique=Data.m_NrUnique;
	m_Nom=Data.m_Nom;
	m_Media=Data.m_Media;
	m_TypeSuppS=Data.m_TypeSuppS;
	m_TypeMedia=Data.m_TypeMedia;
	m_HeureDebut=Data.m_HeureDebut;
	m_Tarif=Data.m_Tarif;
	m_Libelle=Data.m_Libelle;
	m_NrStationTete=Data.m_NrStationTete;
	m_RegionTete=Data.m_RegionTete;
	m_NrOrdre=Data.m_NrOrdre;
	m_Logo=Data.m_Logo;
	m_Famille=Data.m_Famille;
	m_DateDebut=Data.m_DateDebut;
	m_DateFin=Data.m_DateFin;

	m_NbOption=Data.m_NbOption;
	m_Options.Copy(Data.m_Options);

	m_NbLien=Data.m_NbLien;
	m_DateLien=Data.m_DateLien;
	m_NrUniqueRegie.Copy(Data.m_NrUniqueRegie);
	m_NrUniqueRegieValide=Data.m_NrUniqueRegieValide;
	m_NbRegionPseudoPresent=Data.m_NbRegionPseudoPresent;
	m_NrRegionPseudoPresent.Copy(Data.m_NrRegionPseudoPresent);
	return *this;
}