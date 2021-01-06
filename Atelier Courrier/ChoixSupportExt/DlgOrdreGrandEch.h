// Boite dialogue ordonnancement des grandeurs échelle
// créé le 29/04/2004
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
#include "afxdtctl.h"
#include "afxcmn.h"
#include "label.h"
#include <afxwin.h>

typedef struct {
	LVITEM* plvi;
	CString sCol2;
} lvItem, *plvItem;
// Boîte de dialogue CDlgOrdreGrandEch

class CDlgOrdreGrandEch : public CDialog
{
	DECLARE_DYNAMIC(CDlgOrdreGrandEch)

public:
    // constructeur
	CDlgOrdreGrandEch(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// destructeur
	virtual ~CDlgOrdreGrandEch();

	// initialisation boite de dialogue
	virtual BOOL OnInitDialog();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_ORDRE_GRAND_ECH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

private:

	///init de la boite
	void SetupControls();
	void SetColors();

	// Label titre grandeur
	CLabel m_LabelTitre;

	// Liste des grandeurs échelle
	JFCListCtrl m_LstGrandEch;

	// Etat elmt liste
	CImageList	m_imageState;

	// Fond boite de diaologue tri grandeurs échelle
	CLabel		m_BkOrdreGrandEch;

	// Bouton validation tri grandeurs échelle
	CButtonST	m_BtnOK;

	// Bouton annulation nouveau tri grandeurs échelle
	CButtonST	m_BtnCancel;

	// Les boutons ordonnancement des grandeurs
	CButtonST	m_Btn_Grandeur_UP;
	CButtonST	m_Btn_Grandeur_DOWN;

	// Sélection rang visible
	// CButtonST	m_Chk_RangVisible;
	CButton m_Chk_RangVisible;
		
	// Bouton check validation rang visible
	
	// Lien doument
	JATPDocChoixSupport *pDocChoixSupport;

	// Initialisation liste des grandeurs avec ordre affichage courant
	void InitListGrandeurs();

	// Initialisation liste des grandeurs avec sélection rang en cours
	void InitRangVisible();

	// Liste en place de la liste grandeur
	void SetListGrandeur();

	// Positionne le checkbox rang visible
	void SetChkRangVisible();

	// Nombre de rangs sélectionnés
	JUnt32 GetNbRangSel();

	// Gestion drag&drop
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

public:
	// Liste grandeurs en cours
	JList <JUnt32> m_LstGrandeurs;

protected :
	/// Drag and drop variables
	int					m_nDragIndex;
	int					m_nDropIndex;
	CWnd *				m_pDragWnd;
	CWnd *				m_pDropWnd;
	bool				m_bDragging;
	CImageList *		m_pDragImage;

	bool				m_Deplace;
	
public:
	afx_msg void OnLvnBegindragCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCsGrdUp();
	afx_msg void OnBnClickedCsGrdDown();
	afx_msg void OnLvnItemchangedCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkRangVisible();
	afx_msg void OnNMClickCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
