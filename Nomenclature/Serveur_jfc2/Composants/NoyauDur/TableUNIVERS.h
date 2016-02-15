// TableUNIVERS.h: interface for the CTableUNIVERS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEUNIVERS_H__2113CC0C_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEUNIVERS_H__2113CC0C_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ArbreRegions.h"
#include "Sourcedef.h"
#include "TableStation.h"
#include "TableRegie.h"
#include "TableSource.h"
#include "TableVague.h"

class CTableUNIVERS  
{
public:
	CTableUNIVERS();
	virtual ~CTableUNIVERS();
	CTableUNIVERS & operator=(const CTableUNIVERS &Data);
	CTableUNIVERS(const CTableUNIVERS &Data);

	CString m_Code;
	CString m_Libelle;

	short m_NbVaguesCalU; // ou -1 si DATE
	CStringArray m_NomVaguesCalU; //x NbVaguesCalU max:LIBPERIODEMAX+1 (vide si la vague est supprim�e)
	CWordArray m_DureeVaguesCalU; //par NbVaguesCalU, exprim� en mois

	short m_NbMedia;
	CString m_TabMedia; // x NbMedia +1

	short m_TypeSuppU; // 0:defaut 1:T�l� france
	char m_fPresentArbre; // flag de pr�sentation (0:liste 1: arborescence)
	
	char m_JourDebut;
	short m_HeureDebut; // 300->3h00
	short m_DureeFormatBase;

	CArbreRegionsArray m_ArbreRegions;

	CString m_NomNiveauMereCourt;
	CString m_NomNiveauMere;
	CString m_NomNiveauMereLong;

	int m_NbRegion;
	// par r�gion :
	CStringArray m_CodeZone; // max:CODEREGIONMAX+1;
	CWordArray m_NrUniqueZone;
	CStringArray m_LibRegion; //max LIBREGIONMAX+1;
	CStringArray m_LibRegionCourt; //max LIBREGIONCOURT+1;
	CStringArray m_LibRegionMini; //max LIBREGIONMINI+1;
	CStringArray m_LibRegionLong; //max LIBREGIONLONG+1;
	CStringArray m_CodeMere; // max:CODEREGIONMAX+1   0 si R�gion mere...
	CStringArray m_Options; //max TAILLEOPTION
	CWordArray m_TypeSuppR; // idem TypeSuppU
	CStringArray m_fGRP; //{0,1} si 0 ce qui suit n'est pas allou�

	// les sources par r�gion
	CWordArray m_DescriptionBis; // -1 non utilis� 0:�cran 15:QH
	CSourceDefArray m_SourceEta;
	CSourceDefArray m_SourceRef;
	CSourceDefArray m_SourceEtaBis; // NULL
	CSourceDefArray m_SourceRefBis; // NULL

	// toujours par r�gion:
	CWordArray m_NbStation;
	CTableStationArray * m_TableStations;

	// les pseudo-stations
	// UNIQUEMENT EN MONO-UNIVERS
	CWordArray m_NbPseudoStation;
	CWordArray * m_NrPseudoStation;

	// toutes les stations (vraies+pseudos)
	// UNIQUEMENT EN MONO-UNIVERS *********
	CWordArray m_NbStationCalcul;
	CWordArray * m_NrStationCalcul;
	CTableStationArray * m_TableStationCalcul;

	// mis � jour en fc des stations par �tude
	// sinon c'est la liste des r�gies du march�
	CTableRegieArray m_TableRegies;
	// sources et vagues selon affinage
	CTableSourceArray m_TableSources;

	CWordArray m_NbVague;
	CTableVagueArray * m_TableVagues;
	CWordArray m_NbVagueMatrice;
};

typedef CArray<CTableUNIVERS,CTableUNIVERS&> CTableUniversArray;
#endif // !defined(AFX_TABLEUNIVERS_H__2113CC0C_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
