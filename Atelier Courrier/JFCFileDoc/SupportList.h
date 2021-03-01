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


// structure repr�sentant, � un instant donn�e, une position du filtre des titres,
// c�d quels elements de filtres sont s�lectionn�s
class CSupportFilter
{
	friend class CSupportList;

	public:

		// combinaisons possibles des �tats des checkboxes (titres & couplages)
		static const int DISPLAY_TITRES		= 1;
		static const int DISPLAY_COUPLAGES	= 2;
		static const int DISPLAY_FAMILLES	= 4;
		static const int DISPLAY_ALL		= DISPLAY_TITRES | DISPLAY_COUPLAGES | DISPLAY_FAMILLES;

		// le constructeur demande le nombre maxi de p�riodicit�s existantes (pour optimisation temps de traitement)
		CSupportFilter(const int num_periodicites_ = -1);

		void Reset();
		bool Empty() const;

		// listes d'�l�ments
		std::list<JUnt32>			periodicites;		// la liste des p�riodicit�s s�lectionn�es
		std::list<JUnt32>			segments;			// la liste des segments s�lectionn�es
		std::list<JUnt32>			terrains;			// la liste des terrains s�lectionn�s
		std::list<const JTerrain*>	sourceTerrains;		// la liste des Terrains de la source s�lectionn�e

		// identifiants
		JUnt32						idSource;			// id de la source s�lectionn�e

		//JUnt32						idTerrainCiblage;	// id du terrain de ciblage

		// valeurs enum�r�es
		unsigned int				titre_coupl_famil;	// etat affichage titres et/ou couplages
		bool						supports_from_source;// on affiche seulement les supports de la source?
		bool						onlyselected;

		CString						strSearch;			// chaine de recherche		

	//////////////////////////////////////////////////////////////////////////
	// gestion des modifications (et donc de la mise � jour de la liste)

		// cible de la derni�re modification
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

		/// d�finir ce qui s'est pass� dans la derni�re modif du filtre
		// @param modif : type de la derni�re modification 
		void SetLastModif(eFilterModifTarget modif)
		{
			eLastModifTarget = modif;
			SetModified();
		}
		bool IsModified() {return bDirtyFlag;}

		// � appeler lors du changement de source d'un filtre (permet au filtre de conna�tre ts les ids segments posibles)
		void SetAllSegments(const std::vector<const JSegment*> & all_segments_);
		
		bool GetUseSegments() const						{return bUseSegments;}
		void SetUseSegments(bool bUseSegments_ = true)	{bUseSegments = bUseSegments_;}

		eFilterModifTarget	eLastModifTarget;
		mutable int			num_periodicites;	// nbr maxi de p�riodicit�s existantes
		std::vector<JUnt32>	all_segments;		// tous les segments possibles pour ce filtre

	private:

		// private copy constructor
		CSupportFilter(const CSupportFilter &){ASSERT(FALSE);}

		// private assignment operator
		const CSupportFilter & operator =(const CSupportFilter &){ASSERT(FALSE);}

		void SetModified(bool bModified_ = true){bDirtyFlag = bModified_;}

		bool				bDirtyFlag;				// filtre modifi�?
		bool				bUseSegments;			// utilise-t-on les segments?
};


class CRegroupements;
class JSRCPresse;
class JATPOffre;
class IElem;

//////////////////////////////////////////////////////////////////////////
//								CSupportList							//
//////////////////////////////////////////////////////////////////////////

// contr�le liste permettant d'afficher une s�lection de titres
class CSupportList : public CJFCXListBox
{
	DECLARE_DYNAMIC(CSupportList)

	public:

		CSupportList(CATPColors::APPCOLOR appColor);
		virtual ~CSupportList();

		// chargement des donn�es et initialisation de la listbox
		void InitializeData(std::set<JUnt32> & selectedIds);

		// d�finition de la valeur enum de l 'application appelant
		void SetCallingApp(CATPColors::APPCOLOR callingApp) {m_callingApp = callingApp;}

		// mise � jour des support affich�s
		void UpdateDisplay(CSupportFilter & newfilter);

		// mise � jour des titres appari�s
		void UpdateApparies(const JUnt32 IdSource, const std::list<JUnt32> & terrains, const std::set<JUnt32> & selectedIds);

		// r�cup�re la s�lection compl�te (les supports eux-m�mes)
		void GetWholeSelection(std::list<const IElem *> & selectedElems) const;

		// r�cup�re la s�lection compl�te (uniquement les id des supports)
		void GetWholeSelection(std::set<JUnt32> & selectedIds) const;

		// d�selection de tous les supports
		void ResetSelection();

		// gestion de l'evenement : on a click� sur le bouton 'T' de la s�lection
		bool OnClickOnSelectTout(const bool bIsMaSelection);

		const HXTemplateHandle GetXTemplateTitre ()	const		{return m_TitresXTemplate;}
		const HXTemplateHandle GetXTemplateCouplages () const	{return m_CouplagesXTemplate;}
		const HXTemplateHandle GetXTemplateFamilles () const	{return m_FamillesXTemplate;}

		const HXTemplateHandle GetNotFromSrcXTemplateTitre ()	const	{return m_NotFromSourceTitresXTemplate;}
		const HXTemplateHandle GetNotFromSrcXTemplateCouplages () const	{return m_NotFromSourceCouplagesXTemplate;}
		const HXTemplateHandle GetNotFromSrcXTemplateFamilles () const	{return m_NotFromSourceFamillesXTemplate;}

		// y a-t-il au moins 1 support s�lectionn� (pas forc�ment affich� car peut-�tre filtr�)
		bool HasAtLeastOneSupportSelected() const;

	protected:

		DECLARE_MESSAGE_MAP()

		virtual void PreSubclassWindow();
		int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
		afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
		TCHAR* m_pchTip;
		WCHAR* m_pwchTip;

		// donn�es
		CRegroupements *					m_pRegroupements;
		const JSRCPresse *					m_pSrcPresse;

		CATPColors::APPCOLOR						m_callingApp;

	private:

		// impl�mentation priv�e de la liste de supports
		class Pimpl;
		Pimpl * m_pImpl;

		// templates de style pour les s�lection de titres
		HXTemplateHandle			m_TitresXTemplate;
		HXTemplateHandle			m_CouplagesXTemplate;
		HXTemplateHandle			m_FamillesXTemplate;

		HXTemplateHandle			m_NotFromSourceTitresXTemplate;
		HXTemplateHandle			m_NotFromSourceCouplagesXTemplate;
		HXTemplateHandle			m_NotFromSourceFamillesXTemplate;

		// mise � jour des support affich�s (impl�mentation priv�e)
		void _UpdateDisplay(CSupportFilter & newfilter, std::set<JUnt32> & selectedIds);
};	

// on forward d�clare le pimpl, type opaque pr les fichiers externes
class CSupportList::Pimpl;

