#pragma once

#include "Offre2SupportsOngletBase.h"
#include "ATPColors.h"

// headers STL
#include <map>

class COffre2Supports;
class COffre2MainDialog;

/************************************************************************/
/* Classe COffre2SupportsOngletCtrl										*/
/* qui repr�sentent des onglets de s�lection de l'onglet Supports de	*/
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

		// methodes pour le contr�le des onglets
		BOOL InitDialogs();

		// r�cup�re la page � onglet courante
		COffre2SupportsOngletBase *  GetActiveTab();

		enum eOngletType
		{
			SelectionSource		= 0,
			SelectionSegments	= 1,
			SelectionTypesPoids	= 2,
			NumOnglets			= 3
		};

		// r�cup�re un onglet
		COffre2SupportsOngletBase * GetTab (eOngletType ongletIdx);

		// map indiquant pour un type d'onglet, sont index dans le tabctrl (-1 si onglet non pr�sent)
		std::map<eOngletType, int> m_Onglets;

	protected:

		// variables pour le contr�le des onglets
		int								m_DialogID[NumOnglets];		// tableau d'ID des dialogues des pages � onglets
		COffre2SupportsOngletBase *		m_ppOnglets[NumOnglets];	// tableau de dialogues de pages � onglets
		COffre2SupportsOngletBase *		m_pCurPage;					// la fen�tre active

		CATPColors::APPCOLOR			m_appColor;					// couleur actuelle du dialogue
		eOngletType				m_CurOngletIdx;				// onglet courant
		COffre2Supports *		m_pOngletTitres;			// onglet 'titres' du dialogue de l'offre

		DECLARE_MESSAGE_MAP()

		BOOL OnEraseBkgnd(CDC* pDC);
		void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};


