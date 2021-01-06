#pragma once

#include "Offre2SupportsOngletBase.h"
#include "ATPColors.h"

// headers STL
#include <map>

class COffre2Supports;
class COffre2MainDialog;

/************************************************************************/
/* Classe COffre2SupportsOngletCtrl										*/
/* qui représentent des onglets de sélection de l'onglet Supports de	*/
/* la boite Offre														*/
/************************************************************************/
class COffre2SupportsOngletCtrl : public CTabCtrl
{
		DECLARE_DYNAMIC(COffre2SupportsOngletCtrl)

	public:
		COffre2SupportsOngletCtrl(COffre2Supports  * pOngletTitres, COffre2MainDialog * pDialogOffre2);
		virtual ~COffre2SupportsOngletCtrl();

		// active les onglets
		void ActivateTabDialogs();

		// methodes pour le contrôle des onglets
		BOOL InitDialogs();

		// récupère la page à onglet courante
		COffre2SupportsOngletBase *  GetActiveTab();

		enum eOngletType
		{
			SelectionSource		= 0,
			SelectionSegments	= 1,
			SelectionTypesPoids	= 2,
			NumOnglets			= 3
		};

		// récupère un onglet
		COffre2SupportsOngletBase * GetTab (eOngletType ongletIdx);

		// map indiquant pour un type d'onglet, sont index dans le tabctrl (-1 si onglet non présent)
		std::map<eOngletType, int> m_Onglets;

	protected:

		// variables pour le contrôle des onglets
		int								m_DialogID[NumOnglets];		// tableau d'ID des dialogues des pages à onglets
		COffre2SupportsOngletBase *		m_ppOnglets[NumOnglets];	// tableau de dialogues de pages à onglets
		COffre2SupportsOngletBase *		m_pCurPage;					// la fenêtre active

		CATPColors::APPCOLOR			m_appColor;					// couleur actuelle du dialogue
		eOngletType				m_CurOngletIdx;				// onglet courant
		COffre2Supports *		m_pOngletTitres;			// onglet 'titres' du dialogue de l'offre

		DECLARE_MESSAGE_MAP()

		BOOL OnEraseBkgnd(CDC* pDC);
		void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};


