/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgLot
//
//	Descriptif:
//	Page permettant de créer des lots de titres PLUS UTILISEE
*/

#pragma once

#include "..\DlgFCP.h"

// Boîte de dialogue CDlgLot

class CDlgLot : public CDlgFCP
{
	DECLARE_DYNAMIC(CDlgLot)

public:
	CDlgLot(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgLot();
	void SetColors();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnLbnSelchangeListTitre();
	afx_msg void OnLbnSelchangeListTitreLot();
	afx_msg void OnBtnValider();
	afx_msg void OnBtnEffSas();
	afx_msg void OnBtnGenese();
	afx_msg void OnCtxSuppr();
	afx_msg void OnCtxRename();
	virtual void OnOK(){};
	virtual void OnCancel(){};

private:
	void FillListLot();
	void FillListTitreLot(int idLot);

	int m_IDLot;

	JFC_MultiListBox<CLot const *> m_lstLot;
	JFC_MultiListBox<INode const *> m_lstTitreLot;

	CLabel m_lblTtlTitreLot;
	CLabel m_staticTitreLot;
	CLabel m_statictxtNbTitre;

	void SetWindowRelations();
	void SetupControls();

};
