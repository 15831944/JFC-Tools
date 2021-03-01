// Boite dialogue ordonnancement des grandeurs échelle
// créé le 24/01/2005
// DlgOrdreGrandEch.h : fichier d'implémentation
//
#pragma once
#include "afxwin.h"

// on inclut les définitions nécessaires
#include "Resource.h"
#include "JATPDocChoixSupport.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include "JFCListCtrl.h"

// Boîte de dialogue CDlgRangResult
class CDlgRangResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgRangResult)

public:
	// Constructeur
	CDlgRangResult(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// destrructeur 
	virtual ~CDlgRangResult();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_RANG_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	// Liste grandeurs en cours
	JList <JUnt32> m_LstGrandeurs;

private:
	// Initialisation boite de dialogue
	virtual BOOL OnInitDialog();

	// Init contrôles boite
	void SetupControls();
	void SetColors();

	// Validation sélection rangs résultats
	CButtonST m_BtnOk;

	// Annulation sélection rangs résultats
	CButtonST m_BtnCancel;

	// Sélection visibilité de tous les rangs grandeur
	CButtonST m_Btn_TousLesRangs;

	// Déselection de tous les rangs pour grandeur
	CButtonST m_Btn_AucunRang;

    // Liste sélection affichage rangs resultats
	JFCListCtrl m_LstRangGrandeur;

	// Titre sélection rangs résultats
	CLabel m_LabelTitre;

	// Fond englobant la liste des grandeurs
	CLabel m_BkRangGrandeur;

	// Lien doument
	JATPDocChoixSupport *pDocChoixSupport;

	// Initialisation liste des grandeurs avec état sélection rang
	void InitListGrandeurs();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCsTousRang();
	afx_msg void OnBnClickedCsAucunRang();
};
