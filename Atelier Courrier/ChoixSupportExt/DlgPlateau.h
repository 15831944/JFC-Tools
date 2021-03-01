//
// Fichier: DlgPlateau.h
// Date:    05/04/2004
// Gestion Plateau Choix Supports
//
#pragma once

// on inclut les définitions nécessaires
#include "Resource.h"
#include "JATPDocChoixSupport.h"
#include "DialogChoixOffre.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"


// Boîte de dialogue CDlgPlateau
class CDlgPlateau : public CDialog
{
	DECLARE_DYNAMIC(CDlgPlateau)

public:
	// le constructeur
	CDlgPlateau(JATPDocChoixSupport & ATPDocChoixSupport, CWnd* pParent = NULL);

	// le destructeur
	virtual ~CDlgPlateau();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_PLATEAU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	//{{AFX_MSG(CATPEtude)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	// la fonction pour mettre à jour le plateau
	BOOL OnUpdatePlateau();

protected:
	// la fonction pour mettre à jour la boîte
	VOID OnUpdate();

private:
	// le document choix supports
	JATPDocChoixSupport & m_ATPDocChoixSupport; 

private:
	// Date de tarification
	JDate m_DateTar;

	//0 pas sel, 1 presel, 2 sel
	int bCompleteOffre;
	int bCompleteFormat;

	//init de la boite
	void SetupControls();
	void SetColors();
	void SetIco();

	// Titre cartouche boite plateau
	CLabel m_Titre;

	// Backgroud boite de dialogue 
	CLabel m_FondDlg;

	// Bouton sélection date tarification
	CButtonST m_BtnDateTarif;

	// Bouton sélection offre
	CButtonST m_BtnOffre;

	// Bouton sélection des formats
	CButtonST m_BtnFormat;

	// Bouton Validation de la sélection
	CButtonST m_BtnOK;

	// Bouton annulation sélection
	CButtonST m_BtnCancel;


protected:

public:
	afx_msg void OnDatetar();
	afx_msg void OnOffre();
	afx_msg void OnFormat();
};
