// TableUNIVERS.cpp: implementation of the CTableUNIVERS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableUNIVERS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableUNIVERS::CTableUNIVERS()
{
	m_Code=_T("");
	m_Libelle=_T("");
	m_NbVaguesCalU=0;
	m_NomVaguesCalU.RemoveAll();
	m_DureeVaguesCalU.RemoveAll();
	m_NbMedia=0;
	m_TabMedia=_T("");
	m_TypeSuppU=0;
	m_fPresentArbre=' ';
	m_JourDebut=' ';
	m_HeureDebut=0;
	m_DureeFormatBase=0;
	m_ArbreRegions.RemoveAll();
	m_NomNiveauMereCourt=_T("");
	m_NomNiveauMere=_T("");
	m_NomNiveauMereLong=_T("");
	m_NbRegion=0;
	m_CodeZone.RemoveAll();
	m_NrUniqueZone.RemoveAll();
	m_LibRegion.RemoveAll();
	m_LibRegionCourt.RemoveAll();
	m_LibRegionMini.RemoveAll();
	m_LibRegionLong.RemoveAll();
	m_CodeMere.RemoveAll();
	m_Options.RemoveAll();
	m_TypeSuppR.RemoveAll();
	m_fGRP.RemoveAll();
	m_SourceEta.RemoveAll();
	m_SourceRef.RemoveAll();
	m_DescriptionBis.RemoveAll();
	m_SourceEtaBis.RemoveAll();
	m_SourceRefBis.RemoveAll();

	m_NbStation.RemoveAll();
	m_TableStations=NULL;

	m_NbPseudoStation.RemoveAll();
	m_NrPseudoStation=NULL;

	m_NbStationCalcul.RemoveAll();
	m_NrStationCalcul=NULL;
	m_TableStationCalcul=NULL;

	m_TableRegies.RemoveAll();
	m_TableSources.RemoveAll();

	m_NbVague.RemoveAll();
	m_TableVagues=NULL;
	m_NbVagueMatrice.RemoveAll();
}

CTableUNIVERS::~CTableUNIVERS()
{
}

CTableUNIVERS::CTableUNIVERS(const CTableUNIVERS &Data)
{
	*this=Data;
}

CTableUNIVERS & CTableUNIVERS::operator=(const CTableUNIVERS &Data)
{
	m_Code=Data.m_Code;
	m_Libelle=Data.m_Libelle;
	m_NbVaguesCalU=Data.m_NbVaguesCalU;
	m_NomVaguesCalU.Copy(Data.m_NomVaguesCalU);
	m_DureeVaguesCalU.Copy(Data.m_DureeVaguesCalU);
	m_NbMedia=Data.m_NbMedia;
	m_TabMedia=Data.m_TabMedia;
	m_TypeSuppU=Data.m_TypeSuppU;
	m_fPresentArbre=Data.m_fPresentArbre;
	m_JourDebut=Data.m_JourDebut;
	m_HeureDebut=Data.m_HeureDebut;
	m_DureeFormatBase=Data.m_DureeFormatBase;
	m_ArbreRegions.Copy(Data.m_ArbreRegions);
	m_NomNiveauMereCourt=Data.m_NomNiveauMereCourt;
	m_NomNiveauMere=Data.m_NomNiveauMere;
	m_NomNiveauMereLong=Data.m_NomNiveauMereLong;
	m_NbRegion=Data.m_NbRegion;

	m_CodeZone.Copy(Data.m_CodeZone);
	m_NrUniqueZone.Copy(Data.m_NrUniqueZone);
	m_LibRegion.Copy(Data.m_LibRegion);
	m_LibRegionCourt.Copy(Data.m_LibRegionCourt);
	m_LibRegionMini.Copy(Data.m_LibRegionMini);
	m_LibRegionLong.Copy(Data.m_LibRegionLong);
	m_CodeMere.Copy(Data.m_CodeMere);
	m_Options.Copy(Data.m_Options);
	m_TypeSuppR.Copy(Data.m_TypeSuppR);
	m_fGRP.Copy(Data.m_fGRP);

	m_SourceEta.Copy(Data.m_SourceEta);
	m_SourceRef.Copy(Data.m_SourceRef);
	m_DescriptionBis.Copy(Data.m_DescriptionBis);
	m_SourceEtaBis.Copy(Data.m_SourceEtaBis);
	m_SourceRefBis.Copy(Data.m_SourceRefBis);

	m_NbStation.Copy(Data.m_NbStation);
	m_TableStations=Data.m_TableStations;
	m_TableStationCalcul=Data.m_TableStationCalcul;
	m_NrPseudoStation=Data.m_NrPseudoStation;
	m_NrStationCalcul=Data.m_NrStationCalcul;

	m_NbPseudoStation.Copy(Data.m_NbPseudoStation);
	m_NbStationCalcul.Copy(Data.m_NbStationCalcul);

	m_TableSources.Copy(Data.m_TableSources);
	m_TableRegies.Copy(Data.m_TableRegies);

	m_NbVague.Copy(Data.m_NbVague);
	m_TableVagues=Data.m_TableVagues;
	m_NbVagueMatrice.Copy(Data.m_NbVagueMatrice);

	return *this;
}