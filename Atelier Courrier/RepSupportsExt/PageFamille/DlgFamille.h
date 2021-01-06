/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgFamille
//
//	Descriptif:
//	Page permettant le création de l'arborescence des familles et des centres d'interêts
*/

#pragma once

#include <AllTypeOfNode.h>
#include <AllTypeOfVisitor.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>

#include <JSRCPresse.h>
#include <Resizer.h>
#include <JFC_MultiListBox.h>
#include <Label.h>
#include <BtnST.h>

#include "PeriodTree.h"
#include "SupportsListCtrl.h"

// Boîte de dialogue CDlgFamille

class CDlgFamille : public CDialog
{
	friend CPeriodTree;
	DECLARE_DYNAMIC(CDlgFamille)

public:
	CDlgFamille(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgFamille();

	void SetColors();

	void RefillTree();

#define IdTimer_Expand WM_USER+100
#define IdTimer_Search WM_USER+101


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheckNA();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBeginDragListTitre(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLbnSelchangeListper();
	afx_msg void OnBnClickedBtnselper();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeSearch();
	afx_msg void OnBnClickedBtnEffacer();

//Ruse pour que la fenetre ne se ferme pas
	virtual void OnOK() {/*rien*/};
	virtual void OnCancel() {/*rien*/};

private:
	void SetupControls();
	void SetWindowRelations();
	void FillListTitres();
	bool TestPeriodicite(int ID);

protected:
	CSupportsListCtrl m_lstTitre;
	JFC_MultiListBox<JPeriodicite const *> m_lstPeriodicite;
	CPeriodTree m_treeFamille;
	BOOL m_bCheckNA;
	CRegroupements* m_pRegroupements;
	JSRCPresse * m_srcpresse;
	CString m_searchStr;

private:
	CResizer m_Resizer;
	CImageList m_ImageList;

	CImageList* m_pDragImage;
	BOOL		m_bDragging;	//T during a drag operation
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	HTREEITEM	m_hDropItem;	//
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)

	CLabel	m_lblTitre;
	CLabel	m_lblPeriod;
	CLabel	m_lblFamille;

	CLabel m_staticFamille;
	CLabel m_staticPeriod;
	CLabel m_staticTitre;
	
	CButtonST m_btnSelPeriodicite;
	CButtonST m_checkNA;
	CButtonST m_btnEffacer;
};
