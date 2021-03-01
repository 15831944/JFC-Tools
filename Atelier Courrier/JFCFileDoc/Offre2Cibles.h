#pragma once

#include "resource.h"
#include "Offre2OngletBase.h"
#include "afxcmn.h"
#include "afxwin.h"

// headers des contrôles
#include "BtnST.h"
#include "label.h"
#include "XListBox.h"
#include "TargetList.h"

// headers STL
#include <map>
#include <set>
#include <list>
#include <vector>

class COffre2MainDialog;
//class CItemsConstruitsBase;
class CTerrain;
class IItem;

//=============================================================================	
// Boîte de dialogue COffre2Cibles


//=============================================================================	
class COffre2Cibles : public COffre2OngletBase
//=============================================================================	
{
		DECLARE_DYNAMIC(COffre2Cibles)

	public:

		COffre2Cibles(COffre2MainDialog * pDialogOffre);   // constructeur standard
		virtual ~COffre2Cibles();

	// Données de boîte de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_CIBLES };

		// retourne true si l'état de la sélection est correct
		virtual bool IsSelectionOk() const{return true;}

		// remplit la partie de l'offre correspondant à l'onglet
		virtual void FillOffre(JATPOffre * pOffre);

		typedef enum eFilterFolder		// choix items ou palettes (exclusif)
		{
			effItems = 0,
			effPalettes,
			effNumFolders
		};

		// états d'affichage des tree items
		typedef enum eTreeItemStates
		{
			etisNotSelected	= 0x0001,
			etisSelected	= 0x0002,
			etisHalfSelected= 0x0003
		};

		// définit avec quels elements le dialogue s'affiche en premier lieu
		void SetStartupMode(const eFilterFolder startup_mode);

		// définitions de types
		//typedef struct IItemSel { IItem const *	pItem; bool bSelected; };
		typedef std::vector<JInt32>						IdVector_t;
		typedef std::map<CString, IdVector_t*>			RubrikMap_t;
		typedef RubrikMap_t::iterator					RubrikIter;
		typedef RubrikMap_t::const_iterator				RubrikConstIter;
		typedef std::map<JInt32, bool>					DisplayMap_t;
		typedef DisplayMap_t::iterator					DisplayIter;
		typedef std::map<CString, eTreeItemStates>		RubrikSelectionMap_t;

		// notifie l'onglet qu'il vient d'être sélectionné/déselectionné
		virtual void NotifySelectionChange(bool bSelect);

	protected:

		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		// provoque le chargement de la source et des terrains sélectionnés
		void ReloadTerrainsSelection();

		/////////////////////////////
		// messages
		afx_msg void OnEnChangeCiblesSearchStr();
		afx_msg void OnLbnSelchangeDisplayChoiceLst();
		afx_msg void OnBnClickedCkItemsFromSource();
		afx_msg void OnNMClickFdTreeCibles(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnBnClickedToutselect();
		afx_msg void OnLbnSelchangeLstCibles();
		afx_msg LRESULT OnSourceElementsChange(WPARAM wParam, LPARAM lParam);

		/////////////////////////////
		// gestion de l'interface
		void DisplayItems();					// on choisit d'afficher les items
		void DisplayPalettes();					// on choisit d'afficher les palettes
		void SetImageList();					// définitions des couleurs pour l'imagelist
		void UpdateTargetList();				// mise à jour des elements affichés dans la liste des cibles
		void UpdateIndicator();					// mise à jour de l'indicateur

		/////////////////////////////
		// contrôles
		CTreeCtrl			m_TreeCibles;		// arbre affichage items/palettes
		CXListBox			m_DisplayChoiceList;// liste de filtrage d'affichage entre items et palettes
		CTargetList			m_TargetList;		// la liste des cibles
		CString				m_SearchStr;		// chaine de recherche

		//std::list<CString>	m_TargetsToDisplay;	
		CTargetFilter		m_Filter;			// le filtre d'affichage des cibles
		CString				m_sLastSearchStr;	// précedente valeur du filtre de recherche

		CEdit				m_indicator;		// indicateur du nombre d'elements sélectionnés
		CButtonST			m_RAZCiblesSelBtn;
		CLabel				m_ttlCibles;
		CLabel				m_ttlFiltres;
		eFilterFolder		m_CurDisplay;
		CImageList			m_imageList;

		/////////////////////////////
		// données
		//CItemsConstruitsBase *	m_pItemsConstruits;
		std::list<CTerrain*>	m_SelectedTerrains;

		IItemList_t				m_AllItems;
		IItemList_t				m_AllPalettes;

		// rubriques
		RubrikMap_t				m_RubriquesItems;
		RubrikMap_t				m_RubriquesPalettes;
		RubrikSelectionMap_t	m_RubrikItemsSelection;
		RubrikSelectionMap_t	m_RubrikPalettesSelection;

		// Map des items à afficher
		DisplayMap_t			m_DisplayedItems;
		DisplayMap_t			m_DisplayedPalettes;

		// selections
		IItemList_t				m_SelectionItems;
		IItemList_t				m_SelectionPalettes;

		// vecteur d'index séléctionnés
		std::set<int>			m_lastIndexes;
		std::set<int>			m_Indexes;

		bool					m_bItemsLoaded;
		bool					m_bNewItemsReloaded;
		bool					m_bPalettesLoaded;
		bool					m_bNewPalettesReloaded;
		
		bool					m_bIsFirstDisplayItems;
		bool					m_bIsFirstDisplayPalettes;
		bool					m_bFirstInit;

		// Les rubriques racine et divers
		HTREEITEM				m_hRoot;
		HTREEITEM				m_hRubriqueDivers;

	private:

		// indique la rubrique rootenum
		enum { eidtrRoot = 0 };

		void AssertItemsDependantOnTerrainsLoaded();

		bool m_bItemsDependOnTerrainsLoaded;
		bool m_bPalettesDependOnTerrainsLoaded;

		// charge les items qui dépendent des terrains
		void ReloadNewItems();

		// chargement fichiers items/palettes
		bool LoadRubriques();

		// chargement entête fichier
		void LoadFileHeader(JArchive *pFile);

		// chargement rubriques
		void LoadRubriques(JArchive *pFile, RubrikMap_t & mRubriks, DisplayMap_t & mDisplay);

		// Chargement fichier rubriques
		bool LoadFileRubriques();

		// libère la map et les elements qu'elle contient
		void FreeRubrikMap(const eFilterFolder folder);

		// supprime tous les enfants
		void DeleteAllChildrenOf(HTREEITEM htParent);

		bool HasActifChildren(HTREEITEM hti);
		bool HasInactifChildren(HTREEITEM hti);
		void ToggleParentItemState(HTREEITEM hti,int iImage);
		void ToggleChildItemState(HTREEITEM hti,int iImage);
		void ToggleItemState(HTREEITEM hti);
		void SetSelectTree(HTREEITEM hti);
		void SetOpenTree(HTREEITEM hti);

		int ComputeRootSelection();

		void SelectRubrik(HTREEITEM hti, eTreeItemStates state);

		DECLARE_MESSAGE_MAP()
};
