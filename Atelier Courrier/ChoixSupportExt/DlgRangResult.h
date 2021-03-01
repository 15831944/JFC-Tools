// Boite dialogue ordonnancement des grandeurs �chelle
// cr�� le 24/01/2005
// DlgOrdreGrandEch.h : fichier d'impl�mentation
//
#pragma once
#include "afxwin.h"

// on inclut les d�finitions n�cessaires
#include "Resource.h"
#include "JATPDocChoixSupport.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include "JFCListCtrl.h"

// Bo�te de dialogue CDlgRangResult
class CDlgRangResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgRangResult)

public:
	// Constructeur
	CDlgRangResult(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// destrructeur 
	virtual ~CDlgRangResult();

	// Donn�es de bo�te de dialogue
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

	// Init contr�les boite
	void SetupControls();
	void SetColors();

	// Validation s�lection rangs r�sultats
	CButtonST m_BtnOk;

	// Annulation s�lection rangs r�sultats
	CButtonST m_BtnCancel;

	// S�lection visibilit� de tous les rangs grandeur
	CButtonST m_Btn_TousLesRangs;

	// D�selection de tous les rangs pour grandeur
	CButtonST m_Btn_AucunRang;

    // Liste s�lection affichage rangs resultats
	JFCListCtrl m_LstRangGrandeur;

	// Titre s�lection rangs r�sultats
	CLabel m_LabelTitre;

	// Fond englobant la liste des grandeurs
	CLabel m_BkRangGrandeur;

	// Lien doument
	JATPDocChoixSupport *pDocChoixSupport;

	// Initialisation liste des grandeurs avec �tat s�lection rang
	void InitListGrandeurs();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCsTousRang();
	afx_msg void OnBnClickedCsAucunRang();
};
