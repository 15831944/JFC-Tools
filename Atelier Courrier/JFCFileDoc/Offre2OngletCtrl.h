#pragma once

#include "ATPColors.h"

#include "Offre2OngletBase.h"
#include "Offre2MainDialog.h"

#include <map>

/************************************************************************/
/* Classe COffre2OngletCtrl												*/
/* qui repr�sentent des onglets de la boite Offre						*/
/************************************************************************/
class COffre2OngletCtrl : public CTabCtrl
{
		DECLARE_DYNAMIC(COffre2OngletCtrl)

	public:

		COffre2OngletCtrl(COffre2MainDialog * pDialogOffre2);
		virtual ~COffre2OngletCtrl();

		// active les onglets
		void ActivateTabDialogs();

		// methodes pour le contr�le des onglets
		BOOL InitDialogs();

		// r�cup�re la page � onglet active
		COffre2OngletBase *  GetActiveTab();

		// d�finit l'onglet de d�marrage (son identifiant)
		void SetStartTab(COffre2MainDialog::eOngletsDlgOffre ident);

		// r�cup�re un onglet (en fonction de son indentifiant)
		COffre2OngletBase * GetTab (COffre2MainDialog::eOngletsDlgOffre ident);

		// r�cup�re un onglet (en fonction de son index)
		COffre2OngletBase * GetTabFromIndex (const int index);

		// ajouter un onglet
		void AddTab (COffre2MainDialog::eOngletsDlgOffre ident, LPCTSTR caption);

		// it�ration sur les onglets pr�sents
		COffre2OngletBase * GetTab();
		void GoFirstTab();
		void GoNextTab();
		bool IsTabValid() const;
		bool IsTabValid(COffre2MainDialog::eOngletsDlgOffre & ongIdent) const; // retourne l'identifiant de l'onglet test� (s'il est valide)

	private:

		// variables pour le contr�le des onglets
		int					m_DialogID[COffre2MainDialog::eodoNumOnglets];	// tableau d'ID des dialogues des pages � onglets
		COffre2OngletBase *	m_ppOnglets[COffre2MainDialog::eodoNumOnglets];	// tableau de dialogues de pages � onglets
		COffre2OngletBase *	m_pCurPage;										// la fen�tre active

		void DrawItem(LPDRAWITEMSTRUCT lpDIS);

		CATPColors::APPCOLOR							m_appColor;					// couleur actuelle du dialogue
		COffre2MainDialog::eOngletsDlgOffre		m_CurTabIdent;				// onglet courant

		// map donnant pour un identifiant onglet, son index dans le tabcontrol
		typedef std::map<COffre2MainDialog::eOngletsDlgOffre, int> IndexMap;
		IndexMap			m_IndexMap;
		IndexMap::iterator	m_TabIter;

		DECLARE_MESSAGE_MAP()
};


