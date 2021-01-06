/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgCouplage
//
//	Descriptif:
//	Page permattant de créer des couplages libres
*/

#pragma once

#include "..\DlgFCP.h"

#include <GridCtrl.h>
#include <GridCellNumClick.h>

// Boîte de dialogue CDlgCouplage

class CDlgCouplage : public CDlgFCP
{
	DECLARE_DYNAMIC(CDlgCouplage)

public:
	CDlgCouplage(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgCouplage();
	void SetColors();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnLbnSelchangeListTitre();
	afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBtnValider();
	afx_msg void OnBtnEffSas();
	afx_msg void OnBtnGenese();
	afx_msg void OnCtxSuppr();
	afx_msg void OnCtxRename();

private:
	void FillListCouplage();
	void FillListTitreCouplage(int idCouplage);
	
	void ComputeColumnWidth();

	int m_IDCpl;

	JFC_MultiListBox<CCouplageLibre const *> m_lstCouplage;
	CListCtrl m_lstNbInsert;
	CListCtrl m_lstAbat;

	CGridCtrl m_gridTitreCpl;

	CLabel m_statictxtNbTitre;
	CLabel m_lblTtlTitreCouplage;
	CLabel m_lblTtlNbInsert;
	CLabel m_lblTtlAbat;
	CLabel m_staticCouplage;

	void SetWindowRelations();
	void SetupControls();

};
