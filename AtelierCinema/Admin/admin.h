// admin.h : main header file for the ADMIN application
//

#if !defined(AFX_ADMIN_H__890D9E97_6660_4859_B849_34AFE5DEBB64__INCLUDED_)
#define AFX_ADMIN_H__890D9E97_6660_4859_B849_34AFE5DEBB64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "BaseCinema.h"
#include "TarifAnnee.h"
#include "Saisonnalite.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminApp:
// See admin.cpp for the implementation of this class
//

 
class CAdminApp : public CWinApp
{
public:
	CAdminApp();
	~CAdminApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	CString m_Chemin;

	// Tableau des tarifs par année et par type
	CArray<CTarifAnnee*,CTarifAnnee *> m_Tarifs;

	// Tableau des saisonnalités par année
	CArray<CSaisonnalite,CSaisonnalite&> m_Saisons;

	// Flag indiquant que le chargement des régies a déjà été effectué
	BOOL m_FlagBacRegieCharge;
	
	// varaibles globales
	//{{AFX_MSG(CAdminApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int IndexNbEntreeDuCode(LPCSTR code);
	BOOL IsModified();
	int ExisteProduit(LPCSTR libelle, LPCSTR code);
	int SaveSaisonnalite();
	void LoadSaisonnalite();
	void InitCheminAtelierCinema();
	int IndexTarifAnnee(LPCSTR annee);
	int ImportFile(CString NomFich,CString chemin,CString ImportType);

	int SaveTarifs();
	int SaveTarifAnnee(CString NomFich);

	void LoadTarif();

	// A VOIR
	void LoadTarif(int Annee,int Typeelmt);

	void ChercheFichiersTarif();
	int IndexFormatDuLibelle(LPCSTR libelle);
	int IndexTempoDuLibelle(LPCSTR libelle);
	int IndexProduitDuLibelle(LPCSTR libelle);
	int ComplexeAttacheAuBac(CString indexBac);
	void SaveTable();
	int ExisteComplexe(LPCSTR libelle,LPCSTR code);
	int ExisteBac(LPCSTR ibelle,LPCSTR code);
	int IndexBacDuLibelle(LPCSTR libelle);
	LPCSTR LibelleDuCodeBac(LPCSTR codebac);
	LPCSTR CodeBacDuLibelle(LPCSTR libelle);
	LPCSTR CodeBacCompositeDuLibelle(LPCSTR libelle);
	int IndexComplexeDuLibelle(LPCSTR libelle);
	void ImportTarif(CString type);
	CBaseCinema base;
	BOOL CodeBacExiste(LPCSTR codebac);
	CStringArray m_NomFichiersTarif;

	// type de data tarifés
	enum EnumElmtCinema{TypeComplexe=0,TypeBAC=1,TypeProduit=2};

private:
	BOOL InitTable();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMIN_H__890D9E97_6660_4859_B849_34AFE5DEBB64__INCLUDED_)
