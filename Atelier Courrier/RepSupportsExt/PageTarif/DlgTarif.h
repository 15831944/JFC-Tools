/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgTarif
//
//	Descriptif:
//	Page permettant la gestion des tarifs manuels
*/

#pragma once

#include <AllTypeOfNode.h>
#include <AllTypeOfVisitor.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>

#include <JSRCPresse.h>
#include <JCouplageFixe.h>

#include <Tarifs.h>

#include <Resizer.h>
#include <JFC_MultiListBox.h>
#include <Label.h>
#include <BtnST.h>
#include <JFCEdit.h>

#include "TarifItem.h"
#include "..\TitreInXListBox.h"
#include "FormatListBox.h"

// Boîte de dialogue CDlgTarif


class CDlgTarif : public CDialog
{
	DECLARE_DYNAMIC(CDlgTarif)

public:
	CDlgTarif(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgTarif();
	void SetColors();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();

	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnCtxSupprTarif();
	afx_msg void OnCtxSupprFSpe();

	afx_msg void OnBtnValiderFSpe();
	afx_msg void OnBtnGeneseFSpe();

	afx_msg void OnBtnValider();
	afx_msg void OnBtnEffContainer();
	afx_msg void OnBtnEffSas();
	afx_msg void OnBtnGenese();

	afx_msg void OnLbnSelchangeListFormatStd();
	afx_msg void OnLbnSelchangeListFormatSpe();
	afx_msg void OnLbnSelchangeListTitre();
	afx_msg void OnLbnSelchangeListTarif();
	afx_msg void OnLbnSelchangeListCPL();

	afx_msg void OnCtxRename();

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeSearch();
	afx_msg void OnBnClickedBtnEffacer();
//Ruse pour que la fenetre ne se ferme pas
	virtual void OnOK(){};
	virtual void OnCancel(){};

	virtual void FillListSupport();
	virtual void RefreshList();
	virtual void FillListFormatStd();
	virtual void FillListFormatSpe();
	virtual void FillListTitre();
	virtual void FillListCPL();
	virtual void FillListTarif(int ID);

	int GetSelectedSupport();
	int GetSelectedFormat();

	void SaveSel();
	void ResetSel();

protected:
	friend class CFormatListBox;

	CRegroupements* m_pRegroupements;
	JSRCPresse * m_srcpresse;
	CTarifs* m_pTarifs;

	CString m_NomFormatSpe;
	int m_IDFormatSpeEdited;

	TarifItem* m_pTarifEdited;

//DialogBox Items

	int m_selSupportID;
	int m_selFormatID;
	int m_selTarif;

	CFormatListBox m_lstFormatStd;
	JFC_MultiListBox<JFormatSpe const *> m_lstFormatSpe;

	TitreInXListBox m_lstTitre;
	TitreInXListBox m_lstCPL;

	JFC_MultiListBox<TarifItem const *> m_lstTarif;


private:
	
	
	CDateTimeCtrl m_DateD;
	CDateTimeCtrl m_DateF;
	CJFCNumericEdit m_Tarif;
	
	CButtonST m_btnValiderFSpe;
	
	CButtonST m_btnValider;
	CButtonST m_btnEffContainer;
	CButtonST m_btnEffSas;
	CButtonST m_btnGenese;

#define IdTimer_Search WM_USER+101
	CString m_searchStr;
	CButtonST m_btnEffSearch;

	CLabel m_staticCustom;
	CLabel m_staticFormat;
	CLabel m_staticTitre;
	CLabel m_staticCPL;

	CLabel m_lblTtlFormatStd;
	CLabel m_lblTtlFormatSpe;
	CLabel m_lblTtlTitre;
	CLabel m_lblTtlCPL;
	CLabel m_lblTtlFormat;
	CLabel m_lblTtlTarif;
	CLabel m_lblTtlDateD;
	CLabel m_lblTtlDateF;

	CLabel m_staticTxtFormat;


	CResizer m_Resizer;
	void SetWindowRelations();
	void SetupControls();
};
