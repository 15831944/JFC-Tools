#pragma once

#include "ATPColors.h"

#include "Offre2OngletBase.h"
#include "Offre2MainDialog.h"

#include <map>

/************************************************************************/
/* Classe COffre2OngletCtrl												*/
/* qui représentent des onglets de la boite Offre						*/
/************************************************************************/
class COffre2OngletCtrl : public CTabCtrl
{
		DECLARE_DYNAMIC(COffre2OngletCtrl)

	public:

		COffre2OngletCtrl(COffre2MainDialog * pDialogOffre2);
		virtual ~COffre2OngletCtrl();

		// active les onglets
		void ActivateTabDialogs();

		// methodes pour le contrôle des onglets
		BOOL InitDialogs();

		// récupère la page à onglet active
		COffre2OngletBase *  GetActiveTab();

		// définit l'onglet de démarrage (son identifiant)
		void SetStartTab(COffre2MainDialog::eOngletsDlgOffre ident);

		// récupère un onglet (en fonction de son indentifiant)
		COffre2OngletBase * GetTab (COffre2MainDialog::eOngletsDlgOffre ident);

		// récupère un onglet (en fonction de son index)
		COffre2OngletBase * GetTabFromIndex (const int index);

		// ajouter un onglet
		void AddTab (COffre2MainDialog::eOngletsDlgOffre ident, LPCTSTR caption);

		// itération sur les onglets présents
		COffre2OngletBase * GetTab();
		void GoFirstTab();
		void GoNextTab();
		bool IsTabValid() const;
		bool IsTabValid(COffre2MainDialog::eOngletsDlgOffre & ongIdent) const; // retourne l'identifiant de l'onglet testé (s'il est valide)

	private:

		// variables pour le contrôle des onglets
		int					m_DialogID[COffre2MainDialog::eodoNumOnglets];	// tableau d'ID des dialogues des pages à onglets
		COffre2OngletBase *	m_ppOnglets[COffre2MainDialog::eodoNumOnglets];	// tableau de dialogues de pages à onglets
		COffre2OngletBase *	m_pCurPage;										// la fenêtre active

		void DrawItem(LPDRAWITEMSTRUCT lpDIS);

		CATPColors::APPCOLOR							m_appColor;					// couleur actuelle du dialogue
		COffre2MainDialog::eOngletsDlgOffre		m_CurTabIdent;				// onglet courant

		// map donnant pour un identifiant onglet, son index dans le tabcontrol
		typedef std::map<COffre2MainDialog::eOngletsDlgOffre, int> IndexMap;
		IndexMap			m_IndexMap;
		IndexMap::iterator	m_TabIter;

		DECLARE_MESSAGE_MAP()
};


