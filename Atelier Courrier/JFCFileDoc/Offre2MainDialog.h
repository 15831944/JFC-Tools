#pragma once

#include <StdAfx.h>
#include <afxcmn.h>

#include "resource.h"
#include "ATPColors.h"

#include "Offre2OngletBase.h"
#include "Offre2Selection.h"

#include "XTemplate.h"

#include <list>
#include "afxwin.h"

class COffre2OngletCtrl;
class COffreTerrainManager;

// message utilisateurs enregistrés
extern UINT WMU_SOURCE_ELEMENTS_CHANGE;

//=============================================================================	
// Boîte de dialogue COffre2MainDialog
//=============================================================================	

//=============================================================================	
class AFX_EXT_CLASS COffre2MainDialog : public CDialog
{
		DECLARE_DYNAMIC(COffre2MainDialog)

	public:

		COffre2MainDialog(CATPColors::APPCOLOR appColor, CWnd* pParent, bool bPresenceOngletPeriode = true);   // constructeur standard
		virtual ~COffre2MainDialog();

		void SetTitle(const char * pszDlgTitle);

	// Données de boîte de dialogue

		enum { IDD = IDD_FD_DLG_OFFRE2 };


		typedef enum eOngletsDlgOffre
		{
			eodoPeriode	 = 0,		// onglet Période
			eodoTerrainsSupports,	// onglet Terrains - Supports
			eodoCibles,				// onglet Cibles
			eodoFormats,			// onglet Formats
			eodoNumOnglets
		};		

	// gestion IHM

		void EnableOK();

	// gestion des données de l'offre

		void SetOffre(const JATPOffre& offre);
		void GetOffre(JATPOffre& offre);

	// gestion periode

		const JDate & GetPeriodeDebut() const;
		const JDate & GetPeriodeFin() const;
		void SetPeriode(const JDate & dStart, const JDate & dEnd);


		const CSupportsSelection &	GetSupportsSelection() const {return m_SupportsSelection;}
		CSupportsSelection &		GetSupportsSelection()		{return m_SupportsSelection;}

		const CCiblesSelection &	GetCiblesSelection() const {return m_CiblesSelection;}
		CCiblesSelection &			GetCiblesSelection()		{return m_CiblesSelection;}

		COffreTerrainManager * GetOffreTerrainManager() {return m_OffreTerrainManager;}

		void SetStartupMode (eOngletsDlgOffre ongletIdx = eodoTerrainsSupports);
		const HXTemplateHandle GetAppColorTemplate() {return m_appColorXtemplate;}

		// gestion du dirty flag de l'offre
		bool IsModified ()							{return m_bDirtyFlag;}
		void SetModified(bool bModified = true)		{m_bDirtyFlag = bModified;}
		void ResetModified ()						{m_bDirtyFlag = false;}

		// empêche-t-on l'utilisateur de pouvoir modifier la date de début de la période
		void SetPeriodeDateDReadOnly()				{m_bDateDReadOnly = true;}

	public:

		CBrush & GetBackgroundBrush() {return m_brush;}
		CATPColors::APPCOLOR GetAppColor() {return m_appColor;}

	protected:

		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()

		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedInfoSeuils();
		afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

	public:

		static const int SOURCE_CHANGE		= 0x00000001;
		static const int TERRAINS_CHANGE	= 0x00000002;
		static const int SEGMENTS_CHANGE	= 0x00000004;
		static const int TYPEPOIDS_CHANGE	= 0x00000008;

		// appelé par un onglet ou ce dialogue pour notifier l'onglet cibles que les seuils devront être recalculés
		void NotifySourceElementsChange(const int eltChangeMask);

	private:

		COffre2OngletCtrl *		m_pTabCtrl;			// le contrôle à onglet
		CATPColors::APPCOLOR			m_appColor;			// couleur actuelle du dialogue
		bool					m_bDirtyFlag;		// dirty flag (modifié?)
		CButton					m_btnOK;
		char *					m_pszDlgTitle;		// titre du dialogue

		bool					m_bDateDReadOnly;
		JDate					m_dStart, m_dEnd;

		CBrush					m_brush;

		// variables impliquées dans la gestion de l'offre
		COffreTerrainManager *	m_OffreTerrainManager;
		bool					m_preselcible;

		// fichier ini préselections, filtres
		CSupportsSelection		m_SupportsSelection;
		CCiblesSelection		m_CiblesSelection;

		HXTemplateHandle		m_appColorXtemplate;

		eOngletsDlgOffre		m_StartupMode;
		std::list<JUnt32>		m_SelFormatsFromOffre;

		const bool				m_bDisablePeriode;	// onglet periode présent ou non
		const eOngletsDlgOffre	m_MaxOnglet;		// index du dernier onglet
};


//=============================================================================	
// fonction permettant de lancer le nouveau dialogue Offre et d'en recuperer les paramêtres modifés
bool AFX_EXT_API OpenOffreDialog(	LPCTSTR lpszDlgTitle, CATPColors::APPCOLOR appColor,
									JATPOffre & atpOffre, COffre2MainDialog::eOngletsDlgOffre eStartOnglet,
									JDate & dateDeb, JDate & dateFin,
									bool & bDateChanged, bool & bOffreModified, const bool bDateDebReadOnly,
									CWnd * pParent, const bool bOngletPeriodePresent = true);