#pragma once

#include <StdAfx.h>
#include <afxcmn.h>

#include "resource.h"
#include "ATPColors.h"

#include "OngletOffre2.h"
#include "JATPOffre.h"
#include "OffreTerrainManager.h"

#include <list>
#include "afxwin.h"

class COffreTabCtrl;

// Boîte de dialogue CDialogOffre2

class AFX_EXT_CLASS CDialogOffre2 : public CDialog
{
		DECLARE_DYNAMIC(CDialogOffre2)

	public:

		CDialogOffre2(CWnd* pParent = NULL);   // constructeur standard
		virtual ~CDialogOffre2();

	// Données de boîte de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2 };


		typedef enum eOngletsDlgOffre
		{
			eodoPeriode	 = 0,	// onglet Période
			eodoTitres,			// onglet Titres
			eodoCibles,			// onglet Cibles
			eodoFormats,		// onglet Formats
			eodoNumOnglets
		};		

	// gestion IHM
		void EnableOK();


	// gestion des données de l'offre

		void SetOffre(JATPOffre& offre);
		JATPOffre& GetOffre();

		COffreTerrainManager * GetOffreTerrainManager() {return m_OffreTerrainManager;}

		void SetStartupMode (eOngletsDlgOffre ongletIdx, CATPColors::APP appColor);
		bool IsModified ()							{return m_bDirtyFlag;}
		void SetModified(bool bModified = true)		{m_bDirtyFlag = bModified;}
		void ResetModified ()						{m_bDirtyFlag = false;}

	public:

		void CheckAllSupports (std::list<JUnt32> & lAddedTerrains) const;
		bool CheckOneSupport (JUnt32 IdSupport, const std::list<JUnt32> & lAddedTerrains, CString &txt) const;

		CBrush & GetBackgroundBrush() {return m_brush;}
		CATPColors::APP GetAppColor() {return m_appColor;}

	protected:

		void SetColor();
		void SetupControls();

		CBrush m_brush;

	protected:

		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()

		afx_msg void OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	private:

		eOngletsDlgOffre		m_CurOngletIdx;		// onglet courant
		CDialog *				m_pCurPage;			// pointeur vers la page courant

		COffreTabCtrl *			m_pTabCtrl;			// le contrôle à onglet
		CATPColors::APP			m_appColor;			// couleur actuelle du dialogue
		bool					m_bDirtyFlag;		// dirty flag (modifié?)
		CButton					m_btnOK;


		// variables impliquées dans la gestion de l'offre
		JATPOffre				m_offre;
		COffreTerrainManager *	m_OffreTerrainManager;

		// Les données pour les supports
		std::list<JUnt32>		m_lstIDLot;
		std::list<JUnt32>		m_lstIDTitre;
		std::list<JUnt32>		m_lstIDCouplage;


		bool m_preselcible;
};