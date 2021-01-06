#pragma once

// includes pour les donn�es

// include pour contr�les/dialogues
#include "resource.h"
#include "Offre2OngletBase.h"
#include "SupportList.h"
#include "ATPColors.h"
#include "Label.h"
#include "Offre2Selection.h"
#include "BtnST.h"

// includes C++ STL
#include <map>
#include <list>
#include <set>
#include <vector>
#include "afxwin.h"

// forward declarations
class JSRCTables;
class JSRCPresse;
class JATPOffre;
class JSource;
class JTerrain;
class JSegment;
class COffre2SupportsOngletCtrl;
class COffre2MainDialog;

// Bo�te de dialogue COffre2Supports

class COffre2Supports : public COffre2OngletBase
{
		DECLARE_DYNAMIC(COffre2Supports)

	public:

		COffre2Supports(COffre2MainDialog * pDialogOffre);   // constructeur standard
		virtual ~COffre2Supports();

		enum { IDD = IDD_FD_DLG_OFFRE2_TITRES };

	// nouvelles methodes
		
		// gestion des onglets de s�lection
		void ConstructSelectionTabs();
		afx_msg void OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult);

		// retourne true si l'�tat de la s�lection est correct
		virtual bool IsSelectionOk() const;

		virtual void FillOffre(JATPOffre * pOffre);

		CSupportFilter & GetFilter()			{return m_Filter;}
		const CSupportFilter & GetFilter()const {return m_Filter;}

		// valide la s�lection actuelle (et pr�selectione des elements au cas ou) return true si des modifs ont �t� faites
		bool ValidateSelection(bool bForceSelect = false);

		void LoadTerrains();
		void ValidateTerrains(bool & bModified, bool bForceSelect = false);

		void LoadSegments();
		void ValidateSegments(bool & bModified);

		void LoadTypesPoids();
		void ValidateTypesPoids();

		void LoadPeriodicites();
		void DisplayPeriodicites();
		void SelectPeriodicites();

		void SelectTypesSupports();
		void DisplayTypesSupports();

		// mise � jour de l'affichage des supports
		void UpdateSupportsDisplay();
		
		// mise � jour de l'indicateur
		void UpdateIndicator();	

		// gestion evenements affichage, evenement fen�tres
		virtual BOOL OnInitDialog();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnEnChangeTitleSearchStr();
		afx_msg void OnSelchangeListTitres();
		afx_msg void OnBnClickedRazfilter();
		afx_msg void OnBnClickedToutselect();
		afx_msg void OnLbnSelchangeLstFiltres();
		afx_msg void OnLbnSelchangeLstFiltresElements();
		afx_msg void OnBnClickedCkSupportsFromSource();
		afx_msg LRESULT OnSourceElementsChange(WPARAM wParam, LPARAM lParam);

		HBRUSH GetBackgroundBrush();

		/////////////////////////////
		// elements charg�s

		std::vector<const JTerrain*>	m_Terrains;
		std::vector<const JSegment*>	m_Segments;
		std::vector<const JInt32>		m_TypesPoids;

	protected:

		DECLARE_MESSAGE_MAP()

		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		/////////////////////////////
		// gestion des titres

		const JSRCTables &		m_srcTables;
		const JSRCPresse &		m_srcPresse;

		/////////////////////////////
		// contr�les

		CXListBox					m_FilterFoldersList;	// list single de s�lection de la 'cat�gorie' de filtre
		CJFCXListBox				m_FilterElementsList;	// list multi des elemnts des filtres
		CSupportList				m_TitresList;			// list multi de filtrage des titres

		CLabel						m_ttlTitres;
		CLabel						m_ttlFiltres;
		CButtonST					m_RAZFilterBtn;
		CButtonST					m_RAZSupportsSelBtn;
		CButton						m_SupFromSrcBtn;
		COffre2SupportsOngletCtrl *	m_pTabCtrl;				// le contr�le � onglet (s�lection)
		CEdit						m_indicator;			// indicateur du nombre d'elements s�lectionn�s

		/////////////////////////////
		// gestion des filtres

		// cat�gories possible pour la listbox m_FilterFoldersList
		typedef enum eFilterFolder
		{
			effSelection = 0,				//< 'cat�gorie' Ma S�lection
			effPeriodicite,					//< 'cat�gorie' P�riodicit�s
			effTitres_Couplages_Familles,	//< 'cat�gorie' Titres-Couplages-Familles
			effNumFolders					//< A laisser evidemment en dernier
		};

		eFilterFolder				m_eCurFilter;

		CSupportFilter				m_Filter;				// filtre courant
		std::vector<const IElem*>	m_Periodicites;
		CString						m_sLastSearchStr;		// pr�cedente chaine de recherche
		bool						m_bDontUpdateSearchStringThisTime;

		// impl�mentation priv�e de l'onglet 'supports'
		struct Pimpl;
		Pimpl * m_pImpl;		
};