#pragma once

//#include "XListBox.h"
#include "JFCXListBox.h"
#include "ATPColors.h"

// includes STL
#include <list>
#include <set>
#include <vector>
#include <algorithm>

// forward declarations
class CSupportList;
class JTerrain;
class JSegment;

//////////////////////////////////////////////////////////////////////////
//								CSupportFilter							//
//////////////////////////////////////////////////////////////////////////


// structure représentant, à un instant donnée, une position du filtre des titres,
// càd quels elements de filtres sont sélectionnés
class CSupportFilter
{
	friend class CSupportList;

	public:

		// combinaisons possibles des états des checkboxes (titres & couplages)
		static const int DISPLAY_TITRES		= 1;
		static const int DISPLAY_COUPLAGES	= 2;
		static const int DISPLAY_FAMILLES	= 4;
		static const int DISPLAY_ALL		= DISPLAY_TITRES | DISPLAY_COUPLAGES | DISPLAY_FAMILLES;

		// le constructeur demande le nombre maxi de périodicités existantes (pour optimisation temps de traitement)
		CSupportFilter(const int num_periodicites_ = -1);

		void Reset();
		bool Empty() const;

		// listes d'élèments
		std::list<JUnt32>			periodicites;		// la liste des périodicités sélectionnées
		std::list<JUnt32>			segments;			// la liste des segments sélectionnées
		std::list<JUnt32>			terrains;			// la liste des terrains sélectionnés
		std::list<const JTerrain*>	sourceTerrains;		// la liste des Terrains de la source sélectionnée

		// identifiants
		JUnt32						idSource;			// id de la source sélectionnée

		//JUnt32						idTerrainCiblage;	// id du terrain de ciblage

		// valeurs enumérées
		unsigned int				titre_coupl_famil;	// etat affichage titres et/ou couplages
		bool						supports_from_source;// on affiche seulement les supports de la source?
		bool						onlyselected;

		CString						strSearch;			// chaine de recherche		

	//////////////////////////////////////////////////////////////////////////
	// gestion des modifications (et donc de la mise à jour de la liste)

		// cible de la dernière modification
		typedef enum eFilterModifTarget
		{
			efmta_TITRES_COUPLAGES_FAMILLES	= 0x00000001,
			efmta_SOURCE					= 0x00000002,
			efmta_PERIODICITES				= 0x00000004,
			efmta_SEGMENTS					= 0x00000008,
			efmta_TERRAINS					= 0x00000010,
			efmta_SEARCH_STRING				= 0x00000020,
			efmta_SUPPORTS_FROM_SOURCE		= 0x00000040,
			efmta_ALL						= 0x000000FF
		};

		/// définir ce qui s'est passé dans la dernière modif du filtre
		// @param modif : type de la dernière modification 
		void SetLastModif(eFilterModifTarget modif)
		{
			eLastModifTarget = modif;
			SetModified();
		}
		bool IsModified() {return bDirtyFlag;}

		// à appeler lors du changement de source d'un filtre (permet au filtre de connaître ts les ids segments posibles)
		void SetAllSegments(const std::vector<const JSegment*> & all_segments_);
		
		bool GetUseSegments() const						{return bUseSegments;}
		void SetUseSegments(bool bUseSegments_ = true)	{bUseSegments = bUseSegments_;}

		eFilterModifTarget	eLastModifTarget;
		mutable int			num_periodicites;	// nbr maxi de périodicités existantes
		std::vector<JUnt32>	all_segments;		// tous les segments possibles pour ce filtre

	private:

		// private copy constructor
		CSupportFilter(const CSupportFilter &){ASSERT(FALSE);}

		// private assignment operator
		const CSupportFilter & operator =(const CSupportFilter &){ASSERT(FALSE);}

		void SetModified(bool bModified_ = true){bDirtyFlag = bModified_;}

		bool				bDirtyFlag;				// filtre modifié?
		bool				bUseSegments;			// utilise-t-on les segments?
};


class CRegroupements;
class JSRCPresse;
class JATPOffre;
class IElem;

//////////////////////////////////////////////////////////////////////////
//								CSupportList							//
//////////////////////////////////////////////////////////////////////////

// contrôle liste permettant d'afficher une sélection de titres
class CSupportList : public CJFCXListBox
{
	DECLARE_DYNAMIC(CSupportList)

	public:

		CSupportList(CATPColors::APPCOLOR appColor);
		virtual ~CSupportList();

		// chargement des données et initialisation de la listbox
		void InitializeData(std::set<JUnt32> & selectedIds);

		// définition de la valeur enum de l 'application appelant
		void SetCallingApp(CATPColors::APPCOLOR callingApp) {m_callingApp = callingApp;}

		// mise à jour des support affichés
		void UpdateDisplay(CSupportFilter & newfilter);

		// mise à jour des titres appariés
		void UpdateApparies(const JUnt32 IdSource, const std::list<JUnt32> & terrains, const std::set<JUnt32> & selectedIds);

		// récupère la sélection complète (les supports eux-mêmes)
		void GetWholeSelection(std::list<const IElem *> & selectedElems) const;

		// récupère la sélection complète (uniquement les id des supports)
		void GetWholeSelection(std::set<JUnt32> & selectedIds) const;

		// déselection de tous les supports
		void ResetSelection();

		// gestion de l'evenement : on a clické sur le bouton 'T' de la sélection
		bool OnClickOnSelectTout(const bool bIsMaSelection);

		const HXTemplateHandle GetXTemplateTitre ()	const		{return m_TitresXTemplate;}
		const HXTemplateHandle GetXTemplateCouplages () const	{return m_CouplagesXTemplate;}
		const HXTemplateHandle GetXTemplateFamilles () const	{return m_FamillesXTemplate;}

		const HXTemplateHandle GetNotFromSrcXTemplateTitre ()	const	{return m_NotFromSourceTitresXTemplate;}
		const HXTemplateHandle GetNotFromSrcXTemplateCouplages () const	{return m_NotFromSourceCouplagesXTemplate;}
		const HXTemplateHandle GetNotFromSrcXTemplateFamilles () const	{return m_NotFromSourceFamillesXTemplate;}

		// y a-t-il au moins 1 support sélectionné (pas forcément affiché car peut-être filtré)
		bool HasAtLeastOneSupportSelected() const;

	protected:

		DECLARE_MESSAGE_MAP()

		virtual void PreSubclassWindow();
		int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
		afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
		TCHAR* m_pchTip;
		WCHAR* m_pwchTip;

		// données
		CRegroupements *					m_pRegroupements;
		const JSRCPresse *					m_pSrcPresse;

		CATPColors::APPCOLOR						m_callingApp;

	private:

		// implémentation privée de la liste de supports
		class Pimpl;
		Pimpl * m_pImpl;

		// templates de style pour les sélection de titres
		HXTemplateHandle			m_TitresXTemplate;
		HXTemplateHandle			m_CouplagesXTemplate;
		HXTemplateHandle			m_FamillesXTemplate;

		HXTemplateHandle			m_NotFromSourceTitresXTemplate;
		HXTemplateHandle			m_NotFromSourceCouplagesXTemplate;
		HXTemplateHandle			m_NotFromSourceFamillesXTemplate;

		// mise à jour des support affichés (implémentation privée)
		void _UpdateDisplay(CSupportFilter & newfilter, std::set<JUnt32> & selectedIds);
};	

// on forward déclare le pimpl, type opaque pr les fichiers externes
class CSupportList::Pimpl;

