// NoyauDur.h: interface for the CNoyauDur class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOYAUDUR_H__590553C3_530C_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_NOYAUDUR_H__590553C3_530C_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "FicTbl.h"
#include "FileUtils.h"
#include "Dictionary.h"
#include "TableStation.h"
#include "TableMedia.h"
#include "TableFamille.h"
#include "TableRegie.h"
#include "TableRegion.h"
#include "TableSource.h"
#include "TableVague.h"
#include "TableUnivers.h"

class CNoyauDur : public CFileUtils
{
#define _SUFFIXEDICOFR			"FR"
#define _INI					FichierIni
#define _INI_Local				"Localisation"
#define _INI_Data				"DATA"
#define _INI_Univers			"UNIVERS"
#define _INI_User				"USER"
#define _INI_Zone				"ZONE"
#define _INI_Source				"SOURCE"
#define _INI_DataTele			"DATETELE"
#define TABLESOURCEUNI			"SRC"		// Nom pour la table des sources par univers
#define LIBPERIODEMAX			10

public:
	CNoyauDur(CString EtudIni, CString Marche, short CodeStation=5);
	CNoyauDur();
	virtual ~CNoyauDur();

	bool Initialisation(CString EtudIni, CString Marche, short CodeStation=5);
	CTableFamilleArray &DonneTableFamilles(void);
	CTableRegieArray &DonneTableRegies(void);
	CTableMediaArray &DonneTableMedias(void);
	CTableRegionArray &DonneTableRegions(void);
	
	// Table des stations d'un marché
//	CTableStationArray ** DonneTableStations(int Univers);

	// Table des univers d'un marché
	CTableUniversArray &DonneMarche(void);
	void LibererMarche(void);
	CTableUNIVERS *DonneUnivers(int Univers);

//	CTableSourceArray * DonneTableSources(int Univers);
//	CTableVagueArray ** DonneTableVagues(int Univers);

protected:
	CDictionary * m_Dico1,
				* m_Dico2,
				* m_Dico3;

	CWordArray * m_NbStation;
	CTableStationArray ** m_TableStations;
	CWordArray m_NbStationMarche;
	CTableStationArray * m_TableStationsMarche;
	CTableUniversArray m_TableUnivers;
	CTableMediaArray m_TableMedias;
	CTableRegieArray m_TableRegies;
	CTableFamilleArray m_TableFamilles;
	CTableRegionArray m_TableRegions;
	CWordArray m_NbSource;
	CTableSourceArray * m_TableSources;
	CWordArray * m_NbVague;
	CWordArray * m_NbVagueMatrice;
	CTableVagueArray ** m_TableVagues;

	CString m_Marche,
			m_Univers;
	CString m_CheminEtudIni,
			m_CheminData,
			m_CheminSource,
			m_CheminUnivers,
			m_CheminUser,
			m_CheminZone,
			m_CheminDataTele;
	bool	m_fUniversUnique;
	short	m_NrUniversUnique;
	bool	m_Init;
	BYTE	m_Universload;			//1=univers charge; sinon 0
	bool	m_Marcheload;			//true=marche charge; sinon 0
	short   m_CodeStation;          //3=code à 3 caractères, 5=code à 5 caractères

protected:
//	bool VerifMarche(CString M);
	void	InitNoyo(void);
	short	ChargerNoyau(bool type);
	short	ChargerUnivers(void);
	bool	RechercheUniversUnique(void);
	short	ChargerMedias(void);
	short	ChargerFamilles(void);
	short	ChargerRegions(void);
	short	ChargerStations(void);
	void	TriTableStations(void);
	short	ChargerPseudoStations(void);
	short	ChargerRegies(void);
	short	ChargerStationsMarche(void);
	short	ChargerTabSourcesUnivers(void);
	CTableSOURCE *	GetListeSource(CString FicSource, short & nbsource);
	short	ChargerTableVagues(void);
	CTableVAGUE *	GetListeVagues(CString NomRep, short & NbVagueOrigine, short & nbvague, bool FiltreV);
	void	LierNoyau(void);
	void	AroserArbre(void);
	void	RechercheBranches(CTableUNIVERS * U,CArbreREGIONS *BrancheM);
	void	MajInfoBidon(void);

	void	LibererUnivers(CTableUNIVERS & U);
	void	LibererStations(CTableStationArray ** S);
	void	LibererSources(CTableSourceArray * Source);
	void	LibererVagues(CTableVagueArray ** Vagues);
	void	LibererDicos(void);
	void	LibererTousUnivers(CTableUniversArray & M);
	void	RAZMembers(void);
private:
};

#endif // !defined(AFX_NOYAUDUR_H__590553C3_530C_11D2_B3F9_006052017E36__INCLUDED_)
