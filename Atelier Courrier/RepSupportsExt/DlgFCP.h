/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgFCP
//
//	Descriptif:
//	Page de base pour la gestion de la sélection des titres par Famille Centre Périodicité
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

#include "DlgRename.h"

#include "TitreInXListBox.h"
// Boîte de dialogue CDlgFCP

class CDlgFCP : public CDialog
{
	DECLARE_DYNAMIC(CDlgFCP)

public:
	CDlgFCP(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgFCP();
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnTFamille();
	afx_msg void OnBnClickedBtnTCentre();
	afx_msg void OnBnClickedBtnTPeriodicite();
	afx_msg void OnLbnSelchangeListFamille();
	afx_msg void OnLbnSelchangeListCentre();
	afx_msg void OnLbnSelchangeListPeriodicite();
	afx_msg void OnBtnEffContainer();
	afx_msg virtual void OnBtnEffSas() = 0;

	virtual void OnOK() {/*rien*/};
	virtual void OnCancel() {/*rien*/};

	virtual void RefreshList();
	virtual void FillListFamille();
	virtual void FillListCentre();
	virtual void FillListTitre();
	bool TestPeriodicite(int ID);
	bool TestApparente(int ID);
	void SaveSel();
	void ResetSel();

protected:
	bool m_AfficheApparentes;
	CRegroupements* m_pRegroupements;
	JSRCPresse * m_srcpresse;

	CFamille* m_pfamTNA;


	//zone d'édition nom
	CString m_Nom;

//DialogBox Items

	JList<JInt32> m_selFamilleID;
	JList<JInt32> m_selCentreID;
	JList<JInt32> m_selPeriodID;
	JList<JInt32> m_selTitreID;

	JFC_MultiListBox<CFamille const *> m_lstFamille;
	JFC_MultiListBox<CCentre const *> m_lstCentre;
	JFC_MultiListBox<JPeriodicite const *> m_lstPeriodicite;
	TitreInXListBox * m_plstTitre;

	CLabel m_staticCustom;
	CLabel m_staticNBMW;
	CLabel m_staticTtlCustom;

	CResizer m_Resizer;

private:
	CButtonST m_btnValider;
	CButtonST m_btnEffContainer;
	CButtonST m_btnEffSas;
	CButtonST m_btnGenese;

	CLabel m_staticFamille;
	CLabel m_staticCentre;
	CLabel m_staticPeriod;
	CLabel m_staticTitre;

	CLabel m_lblTtlFamille;
	CLabel m_lblTtlCentre;
	CLabel m_lblTtlPeriod;
	CLabel m_lblTtlTitre;

	CButtonST m_btnSelFamille;
	CButtonST m_btnSelCentre;
	CButtonST m_btnSelPeriodicite;

	void SetWindowRelations();
	void SetupControls();
};
