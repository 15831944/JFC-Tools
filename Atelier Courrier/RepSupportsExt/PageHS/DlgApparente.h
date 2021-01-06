/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CDlgApparente
//
//	Descriptif:
//	Page permettant de d�finir les titres apparent�s (titres � cr�er)
*/

#pragma once

#include "..\DlgFCP.h"

// Bo�te de dialogue CDlgApparente

class CDlgApparente : public CDlgFCP
{
	DECLARE_DYNAMIC(CDlgApparente)

public:
	CDlgApparente(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgApparente();

	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnLbnSelchangeListTitre();
	afx_msg void OnBtnValider();
	afx_msg void OnBtnEffSas();
	afx_msg void OnBtnGenese();
	afx_msg void OnCtxSuppr();
	afx_msg void OnCtxRename();

private:
	void FillListTitreApparente();

	JFC_MultiListBox<CTitreApparente const *> m_lstApparente;
	int m_MediaWeight;
	int m_IDTA;

	CLabel m_TitreTuteur;
	CTitre* m_pTitreTuteur;
	CLabel m_lblTtlTTuteur;
	
	void SetWindowRelations();
	void SetupControls();
};
