#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BtnST.h"
#include "label.h"

#include "Resource.h"
#include "JATPDocument.h"
#include "ATPColors.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Boite de saisie manuel des tarifs supports (par format)	
///////////////////////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CDlgTarifManuel : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgTarifManuel)

public:
	// constructeur standard
	CDlgTarifManuel(CWnd* pParent = NULL, JATPDocument *pDoc = NULL);   

	// Destructeur
	virtual ~CDlgTarifManuel();

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_TARIFMANUEL};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	// Ident du support à modifier
	JUnt32	m_IdSupport;

	// Map des nouveaux tarifs manuels
	JMap <CKeyTarifManuel, JFlt32> m_MapNewTarifManuel;

	// Date début et date fin tarifaire
	JDate m_DateDeb;
	JDate m_DateFin;
	
	// Les formats disponibles
	JMap <JUnt32, JUnt32> *m_pMapIdFormat;

	// Le  format courant sélectionné
	JUnt32 m_IdFormatSel;

private:
	// Cadre englobant les controles saisie tarif
	CLabel m_CadreTarif;

	// Nom du format tarif à modifier
	CLabel m_LblFormat;

	// Tarif à modifier
	CLabel m_LblTarif;

	// Séparation
	CStatic m_Separ;

	// Nom du support
	CLabel m_TxtSupport;

	// Combo list des formats disponible sur le support
	CComboBox m_ComboFormat;

	// Boite de saisie tarif support
	CEdit m_EditTarif;

	// Bouton retour au tarif de base
	CButtonST  m_BtnTarifDeBase;

	// Validation des tarifs manuels
	CButtonST m_BtnOK;

	// Annulation saisie tarif
	CButtonST m_BtnCancel;

	// Lien document
	JATPDocument *m_pDocument;

private :
	///init de la boite
	void SetupControls();

	// Couleur des controles
	void SetColors();

	// Affichage du tarif 
	void AfficheTarif(JUnt32 IdFormat);

	// Récupération tarif de base
	JFlt32 GetTarifSource(JUnt32 IdSupport, JUnt32 IdFormat);

	// Récupération et sauvegarde nouveau tarif
	JBool RecupEtSauveTarif();

	// Récupèration index format du format en cours de sélection sur échelle
	JUnt32 GetIndexFormatSel();

public:
	afx_msg void OnCbnSelchangeCrCbxFormat();
	afx_msg void OnEnChangeCrEditTarif();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCrTarifbase();

private:
	// Couleur application
	CATPColors::APPCOLOR m_appColor;

public:
	// Renseigne couleur application qui appele dialogue tarif
	void SetColors(CATPColors::APPCOLOR app);

	// Positionne les infos tarifaires
	void SetDateTarif(JDate DateDeb, JDate DateFin);

	// Positionne format en cours
	void SetFormatSupport(JMap <JUnt32,JUnt32> *pMapIdFormat, JUnt32 IdFormatSel);
};
