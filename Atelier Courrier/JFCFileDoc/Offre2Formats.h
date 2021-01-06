#pragma once

#include "resource.h"
#include "Offre2OngletBase.h"
#include "afxwin.h"

#include "JFCXListBox.h"
#include "BtnST.h"
#include "Label.h"

#include <list>

//=============================================================================	
// forward declarations
class COffre2MainDialog;
class JSRCPresse;
class CTarifs;
class CRegroupements;

//=============================================================================	
// Boîte de dialogue COffre2Formats
//
class COffre2Formats : public COffre2OngletBase
//=============================================================================	
{
		DECLARE_DYNAMIC(COffre2Formats)

	public:

		COffre2Formats(COffre2MainDialog * pDialogOffre);   // constructeur standard
		virtual ~COffre2Formats();

	// Données de boîte de dialogue
		enum { IDD = IDD_FD_DLG_OFFRE2_FORMATS };

		// retourne true si l'état de la sélection est correct
		virtual bool IsSelectionOk() const;

		// remplit la partie de l'offre correspondant à l'onglet
		virtual void FillOffre(JATPOffre * pOffre);

		// remplit la listbox des formats
		void FillListFormats();

		void GetFormats(JList<JUnt32> & lstIDFormat) const;

		// pour définir la sélection par défaut
		void SetDefaultSelection(const std::list<JUnt32> & defaultSel);

	protected:

		virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

		DECLARE_MESSAGE_MAP()
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnLbnSelchangeLstFormats();
		virtual BOOL OnInitDialog();

		// contrôles
		CJFCXListBox	m_FormatsList;					// listbox des formats
		CLabel			m_ttlFormats;					// label titres formats

		// gestion de l'UI
		std::list<JUnt32>	m_allFormats;			// formats existants dans la boite
		std::list<JUnt32>	m_selectedFormats;		// formats sélectionnés

		// données
		JSRCPresse * m_srcPresse;
};
