#pragma once


// includes pour les données

// include pour contrôles/dialogues
#include "resource.h"
#include "OngletOffre2.h"
#include "SupportList.h"
#include "ATPColors.h"

// forward declarations
class JSRCTables;
class JSRCPresse;

// Boîte de dialogue COngletOffre2_Titres

class COngletOffre2_Titres : public COngletOffre2
{
		DECLARE_DYNAMIC(COngletOffre2_Titres)

	public:

		COngletOffre2_Titres();   // constructeur standard
		virtual ~COngletOffre2_Titres();

		enum { IDD = IDD_FD_DLG_OFFRE2_TITRES };

		// methodes pour initialiser/reinitialiser les listes
		void InitLists();

		// retourne true si l'état de la sélection est correct
		virtual bool IsSelectionOk() {return true;}

	protected:

		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

		// messages de notifications de changement de valeur
		afx_msg void OnComboSourceSelChange();
		afx_msg void OnFilterFolderSelChange();
		afx_msg void OnFilterElementSelChange();
		afx_msg void OnEnChangeTitleSearchStr();

		// messages de notification de click
		afx_msg void OnChkboxSupportsSourceClicked();
		afx_msg void UpdateCheckboxesState();			// (cohérence de l'état des checkboxes)

		/////////////////////////////
		// gestion de l'interface

		// catégories possible pour la liste box m_FilterFoldersList
		typedef enum eFilterFolder
		{
			effTerrains = 0,		//< 'catégorie' terrains
			effSegments,			//< 'catégorie' segments
			effTypesDePoids,		//< 'catégorie' types de poids
			effPeriodicite,			//< 'catégorie' periodicité
			effNumFolders			//< A laisser evidemment en dernier (des fois il vaut mieux le dire qd mm!!!)
		};

	private:

		// remplit la liste des elements à filtrer
		void PopulateFilterElements();
		void ValidateFilterElementsWithCurrentSource();

		// différentes manières de remplir la liste des filtres
		void PopulateWithTerrains();
		void PopulateWithSegments();
		void PopulateWithTypesDePoids();
		void PopulateWithPeriodicites();

		// remplit la liste de titres
		void PopulateTitres();

		// pour la gestion des messages pr les cas particuliers dans la sélection des terrains
		// return true si continuer sélection
		bool AffichageMsgCasParticuliersTerrains(const int IdxListTerrains);

	private:

		// classe representant les 2 listbox des filtres (single et multi selection)
		// et permettant d'afficher et d'avoir accès à l'une comme à l'autre
		class kFilterElementList
		{
			public:

				CXListBox	Single;
				CXListBox	Multi;

				bool		IsSingle() const	{return !m_bIsMulti;}
				bool		IsMulti()  const	{return m_bIsMulti;}
				
				void ShowMulti() {Show(true);}
				void ShowSingle(){Show(false);}
			
			private:	

				void Show(bool bShowMulti)
				{
					Multi.ShowWindow(bShowMulti?SW_SHOW:SW_HIDE);
					Single.ShowWindow(bShowMulti?SW_HIDE:SW_SHOW);
					m_bIsMulti = bShowMulti;
				}

				bool m_bIsMulti;
		};

		// contrôles
		CComboBox				m_SourceCombo;			// combo de sélection de la source
		CXListBox				m_FilterFoldersList;	// list single de sélection de la 'catégorie' de filtre
		CSupportList			m_TitresList;			// list multi de sélection des titres

		CButton					m_TitresChkbox;			// checkbox (on affiche les titres)
		CButton					m_CouplagesChkbox;		// checkbox (on affiche les couplages)
		CButton					m_SupportsSourceChkbox;	// checkbox (supports de la source)
		kFilterElementList		m_FilterElementsList;	// composé contenant et gerant les 2 listboxes (single + multiselect) superposées des filtres

		CString					m_sSearchString;		// la valeur de la chaine de recherche (filtrage des chaines de la liste)

		// variables représentant les états de la boite de dialogue
		CSupportFilter::eTitreCouplageDisplay	m_eLastTitresCouplagesDisplay;	// etat precedent des checkbox 'Titres' et 'Couplages'
		CSupportFilter::eTitreCouplageDisplay	m_eTitresCouplagesDisplay;		// l'etat des checkbox 'Titres' et 'Couplages'
		bool									m_bSupportsDeLaSource;			// booléen lié au checkbox 'supports de la source'
		eFilterFolder							m_eFolderIdx;					// idx du filter-folder sélectionné
		//JUnt32									m_SourceId;						// source courante


		/////////////////////////////
		// gestion des titres

		JSRCTables*		m_srcTables;
		JSRCPresse*		m_srcPresse;

		////////////////////////////
		// divers

		CATPColors::APP	m_AppColor;			// représente l'application appelante
};