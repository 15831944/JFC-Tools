#pragma once

#include "ATPColors.h"

#include "OngletOffre2_Cibles.h"
#include "OngletOffre2_Titres.h"
#include "OngletOffre2_Formats.h"
#include "OngletOffre2_Periode.h"

#include "OngletOffre2.h"
#include "DlgOffre2.h"

class COngletOffre2;

class COffreTabCtrl : public CTabCtrl
{
		DECLARE_DYNAMIC(COffreTabCtrl)

	public:

		COffreTabCtrl(CDialogOffre2 * pDialogOffre2);
		virtual ~COffreTabCtrl();

		// active les onglets
		void ActivateTabDialogs();

		// methodes pour le contr�le des onglets
		BOOL InitDialogs();

		// r�cup�re la page � onglet courante
		COngletOffre2 *  GetActiveTab();

		// d�finit l'onglet de d�marrage
		void SetStartTab(CDialogOffre2::eOngletsDlgOffre eOngletIdx);

		// callback appel� lorsqu'on change de page
		afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

		// r�cup�re un onglet
		COngletOffre2 * GetTab (CDialogOffre2::eOngletsDlgOffre ongletIdx);

	protected:

		// variables pour le contr�le des onglets
		int				m_DialogID[CDialogOffre2::eodoNumOnglets];	// tableau d'ID des dialogues des pages � onglets
		COngletOffre2 *	m_ppOnglets[CDialogOffre2::eodoNumOnglets];	// tableau de dialogues de pages � onglets
		COngletOffre2 *	m_pCurPage;									// la fen�tre active

		CATPColors::APP						m_appColor;				// couleur actuelle du dialogue
		CDialogOffre2::eOngletsDlgOffre		m_CurOngletIdx;			// onglet courant

		DECLARE_MESSAGE_MAP()
};


