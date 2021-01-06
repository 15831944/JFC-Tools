// Boite dialogue ordonnancement des headers grille échelle
// créé le 29/04/2004
// DlgOrdreHeader.h : fichier d'implémentation
//
#pragma once
#include "afxwin.h"

// on inclut les définitions nécessaires
#include "Resource.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include "afxdtctl.h"

// Boîte de dialogue CDlgOrdreHeader
class CDlgOrdreHeader : public CDialog
{
	DECLARE_DYNAMIC(CDlgOrdreHeader)

public:
    // constructeur
	CDlgOrdreHeader(CWnd* pParent = NULL);

	// destructeur
	virtual ~CDlgOrdreHeader();

	// Initialisation
	virtual BOOL OnInitDialog();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_ORDRE_HEADER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

private :
	///init de la boite
	void SetupControls();
	void SetColors();

	// Validation de l'odre des headers
	CButtonST m_BtnOK;

	// Annulation sélection ordre des headers
	CButtonST m_BtnCancel;

	// Fond des contrôles ordre headers
	CLabel m_BkOrdreHeader;

	// Bouton positionnement header ligne 1
	CButtonST m_BtnHeader1;

	// Bouton positionnement header ligne 2
	CButtonST m_BtnHeader2;

	// Bouton positionnement header ligne 3
	CButtonST m_BtnHeader3;

	// Affichage des nouveaux textes header
	void SetTexteHeader();

public:
	afx_msg void OnOk();
	afx_msg void OnBnClickedCsBtnHeader1();
	afx_msg void OnBnClickedCsBtnHeader2();
	afx_msg void OnBnClickedCsBtnHeader3();

	// Ordre des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	CArray <JUnt32> m_OrdreEntete;

};
