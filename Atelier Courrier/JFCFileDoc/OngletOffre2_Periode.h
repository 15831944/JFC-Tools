#pragma once

#include "resource.h"
#include "OngletOffre2.h"

// Bo�te de dialogue COngletOffre2_Periode

class COngletOffre2_Periode : public COngletOffre2
{
		DECLARE_DYNAMIC(COngletOffre2_Periode)

	public:

		COngletOffre2_Periode();   // constructeur standard
		virtual ~COngletOffre2_Periode();

	// Donn�es de bo�te de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_PERIODE };

		// retourne true si l'�tat de la s�lection est correct
		virtual bool IsSelectionOk() {return true;}

	protected:

		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
