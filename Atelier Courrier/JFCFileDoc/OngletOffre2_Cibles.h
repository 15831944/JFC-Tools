#pragma once

#include "resource.h"
#include "OngletOffre2.h"

// Boîte de dialogue COngletOffre2_Cibles

class COngletOffre2_Cibles : public COngletOffre2
{
		DECLARE_DYNAMIC(COngletOffre2_Cibles)

	public:

		COngletOffre2_Cibles();   // constructeur standard
		virtual ~COngletOffre2_Cibles();

	// Données de boîte de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_CIBLES };

		// retourne true si l'état de la sélection est correct
		virtual bool IsSelectionOk() {return true;}

	protected:

		virtual BOOL OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
