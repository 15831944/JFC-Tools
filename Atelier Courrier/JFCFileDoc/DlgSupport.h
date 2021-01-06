/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgCible
//
//	Descriptif:
//	Boite fille de DialogChoixOffre
//	Permet la sélection des supports dans l'offre
*/

#pragma once

#include "DialogChoixOffre.h"
#include "RedListBox.h"
#include "RedTree.h"
#include "ETSLayout.h"


// Boîte de dialogue CDlgSupport
class CDlgSupport : public ETSLayoutDialog
{
	friend CRedListBox;
	friend CRedTree;
	DECLARE_DYNAMIC(CDlgSupport)

public:
	CDlgSupport(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgSupport();

	void SetData (CDialogChoixOffre* data);

	bool IsComplete();

	void Init();

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_SUPPORT };

protected:
	enum INTERNALMODE {LOT, TITRE, COUPLAGE};
	INTERNALMODE m_internalMode;

	CRegroupements* m_pRegroupements;
	JSRCPresse * m_srcPresse;

	CDialogChoixOffre* m_pData;
	CString m_searchStr;

//	void FillListLot();
	void FillListTitre();
	void FillListCouplage();

	void FillResultListLot();
	void FillPeriod();
	void FillResultListCouplage();


	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {/*do nothing*/};
	virtual void OnCancel() {/*do nothing*/};
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnLots();
	afx_msg void OnBnClickedBtnTitres();
	afx_msg void OnBnClickedBtnCouplages();
	afx_msg void OnLbnSelchangeListBas();
	afx_msg void OnLbnSelchangeListHaut();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeSearch();
	afx_msg void OnBnClickedRazSearch();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnTvnSelChangedTreeBas(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtnT();
	afx_msg void OnBtnT2();
	afx_msg void OnLbnSelchangeListPeriod();
	afx_msg void OnPaint();

private:

	void ResetSize();
	CLabel m_ttlChoix;
	CLabel m_ttlPeriod;
	CRedListBox m_lstHaut;
	CRedListBox m_lstBas;
	JFC_MultiListBox<IElem const *> m_lstPeriod;
	CButtonST m_btnLot;
	CButtonST m_btnTitres;
	CButtonST m_btnCouplages; 
	CButtonST m_btnRAZSearch;
	CButtonST m_btnAfficheAll;
	CButtonST m_btnT;
	CButtonST m_btnT2;
	CRedTree m_treeSupport;

	void SetupControls();
	void SetColors();
	bool TestPeriodicite(ITitre* iTitre);

#define IdTimer_Search WM_USER+101
};
