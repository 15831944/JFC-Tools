#pragma once


// includes pour les donn�es

// include pour contr�les/dialogues
#include "resource.h"
#include "OngletOffre2.h"
#include "SupportList.h"
#include "ATPColors.h"

// forward declarations
class JSRCTables;
class JSRCPresse;

// Bo�te de dialogue COngletOffre2_Titres

class COngletOffre2_Titres : public COngletOffre2
{
		DECLARE_DYNAMIC(COngletOffre2_Titres)

	public:

		COngletOffre2_Titres();   // constructeur standard
		virtual ~COngletOffre2_Titres();

		enum { IDD = IDD_FD_DLG_OFFRE2_TITRES };

		// methodes pour initialiser/reinitialiser les listes
		void InitLists();

		// retourne true si l'�tat de la s�lection est correct
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
		afx_msg void UpdateCheckboxesState();			// (coh�rence de l'�tat des checkboxes)

		/////////////////////////////
		// gestion de l'interface

		// cat�gories possible pour la liste box m_FilterFoldersList
		typedef enum eFilterFolder
		{
			effTerrains = 0,		//< 'cat�gorie' terrains
			effSegments,			//< 'cat�gorie' segments
			effTypesDePoids,		//< 'cat�gorie' types de poids
			effPeriodicite,			//< 'cat�gorie' periodicit�
			effNumFolders			//< A laisser evidemment en dernier (des fois il vaut mieux le dire qd mm!!!)
		};

	private:

		// remplit la liste des elements � filtrer
		void PopulateFilterElements();
		void ValidateFilterElementsWithCurrentSource();

		// diff�rentes mani�res de remplir la liste des filtres
		void PopulateWithTerrains();
		void PopulateWithSegments();
		void PopulateWithTypesDePoids();
		void PopulateWithPeriodicites();

		// remplit la liste de titres
		void PopulateTitres();

		// pour la gestion des messages pr les cas particuliers dans la s�lection des terrains
		// return true si continuer s�lection
		bool AffichageMsgCasParticuliersTerrains(const int IdxListTerrains);

	private:

		// classe representant les 2 listbox des filtres (single et multi selection)
		// et permettant d'afficher et d'avoir acc�s � l'une comme � l'autre
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

		// contr�les
		CComboBox				m_SourceCombo;			// combo de s�lection de la source
		CXListBox				m_FilterFoldersList;	// list single de s�lection de la 'cat�gorie' de filtre
		CSupportList			m_TitresList;			// list multi de s�lection des titres

		CButton					m_TitresChkbox;			// checkbox (on affiche les titres)
		CButton					m_CouplagesChkbox;		// checkbox (on affiche les couplages)
		CButton					m_SupportsSourceChkbox;	// checkbox (supports de la source)
		kFilterElementList		m_FilterElementsList;	// compos� contenant et gerant les 2 listboxes (single + multiselect) superpos�es des filtres

		CString					m_sSearchString;		// la valeur de la chaine de recherche (filtrage des chaines de la liste)

		// variables repr�sentant les �tats de la boite de dialogue
		CSupportFilter::eTitreCouplageDisplay	m_eLastTitresCouplagesDisplay;	// etat precedent des checkbox 'Titres' et 'Couplages'
		CSupportFilter::eTitreCouplageDisplay	m_eTitresCouplagesDisplay;		// l'etat des checkbox 'Titres' et 'Couplages'
		bool									m_bSupportsDeLaSource;			// bool�en li� au checkbox 'supports de la source'
		eFilterFolder							m_eFolderIdx;					// idx du filter-folder s�lectionn�
		//JUnt32									m_SourceId;						// source courante


		/////////////////////////////
		// gestion des titres

		JSRCTables*		m_srcTables;
		JSRCPresse*		m_srcPresse;

		////////////////////////////
		// divers

		CATPColors::APP	m_AppColor;			// repr�sente l'application appelante
};