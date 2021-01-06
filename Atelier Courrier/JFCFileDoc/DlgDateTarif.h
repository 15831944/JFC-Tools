// Boite dialogue sélection date tarifaire
// créé le 20/04/2004
// DlgDateTarif.h : fichier d'implémentation
//
#pragma once
#include "afxwin.h"

// on inclut les définitions nécessaires
#include "Resource.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include "afxdtctl.h"
#include "ATPColors.h"


// Boîte de dialogue CDlgDateTarif
class AFX_EXT_CLASS CDlgDateTarif : public CDialog
{
	DECLARE_DYNAMIC(CDlgDateTarif)

public:
	// Constructeur
	CDlgDateTarif(CWnd* pParent = NULL); 

	// Destructeur
	virtual ~CDlgDateTarif();

	// Initialisation boite de dialogue
	virtual BOOL OnInitDialog();
	void SetColors(CATPColors::APPCOLOR app);

	// les fonctions pour manipuler la date de tarification
	JVoid SetDateTar(JDate DateTar);
	JDate GetDateTar() const;

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_DATETAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

private:
	// Libellé texte date tarification
	CLabel m_LabelDateTar;

	// La date de tarification
	JDate m_DateTar; 

	// Backgroud dialogue date tarification
	CLabel m_BkDateTar;

protected:
	// Sélecteur date tarification
	CDateTimeCtrl m_CtrlDateTar;

private:
	CButtonST m_BtnOK;
	CButtonST m_BtnCancel;
	CATPColors::APPCOLOR m_appColor;

	///init de la boite
	void SetupControls();
	void SetColors();

public:
	afx_msg void OnOk();
};
