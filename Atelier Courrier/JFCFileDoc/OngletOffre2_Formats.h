#pragma once

#include "resource.h"
#include "OngletOffre2.h"

// Bo�te de dialogue COngletOffre2_Formats

class COngletOffre2_Formats : public COngletOffre2
{
		DECLARE_DYNAMIC(COngletOffre2_Formats)

	public:

		COngletOffre2_Formats();   // constructeur standard
		virtual ~COngletOffre2_Formats();

	// Donn�es de bo�te de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_FORMATS };

		// retourne true si l'�tat de la s�lection est correct
		virtual bool IsSelectionOk() {return true;}

	protected:

		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
