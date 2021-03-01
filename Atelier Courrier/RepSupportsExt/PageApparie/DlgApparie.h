/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgApparie
//
//	Descriptif:
//	Page permettant de créer des titres appariés (titres sans GRP)
*/

#pragma once

#include "..\DlgFCP.h"

#include "TerrainManager.h"
#include "JSRCTables.h"

#include <ClrSelComboBox.h>

// Boîte de dialogue CDlgApparie

class CDlgApparie : public CDlgFCP
{
	DECLARE_DYNAMIC(CDlgApparie)

public:
	CDlgApparie(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgApparie();

	void SetColors();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnLbnSelchangeListTitre();
	afx_msg void OnLbnSelchangeListTitreHors();
	afx_msg void OnCbnSelchangeComboSource();
	afx_msg void OnCbnSelchangeComboTerrain();
	afx_msg void OnBtnValider();
	afx_msg void OnBtnEffSas();
	afx_msg void OnBtnGenese();
	afx_msg void OnCtxSuppr();
	afx_msg void OnCtxRename();
//Ruse pour que la fenetre ne se ferme pas
	virtual void OnOK(){};
	virtual void OnCancel(){};

	//redef
	virtual void FillListTitre();

private:
	void SetAttrib();
	
	void FillListTitreHors();

	void FillListTitreApparie();
	void FillComboSource();
	void FillComboTerrain(int idSource);

	CTerrain * m_pTerrain;
	CTitresApparies* m_pTitresApp;

	JInt32 m_selIDSource;
	JInt32 m_selIDTerrain;
	int m_selTitreHorsID;
	
	int m_IDTAEdited;

	CTitre * m_pTitreTemoin;
	CTitre * m_pTitreHors;

	JFC_MultiListBox<CTitreApparie const *> m_lstTitreApparie;

//	JFC_MultiListBox<IElem const *> m_lstTitreHors;
	TitreInXListBox m_lstTitreHors;

	CClrSelComboBox m_ComboSource;
	CClrSelComboBox m_ComboTerrain;
	
	CLabel m_staticTitreHors;
	CLabel m_lblTtlTitreHors;

	CLabel m_lblTtlTTemoin;
	CLabel m_TitreTemoin;
	CLabel m_TitreApparie;

	CLabel m_staticCombo;
	CLabel m_lblTtlSource;
	CLabel m_lblTtlTerrain;

	int m_MediaWeight;

	void SetWindowRelations();
	void SetupControls();

	JSRCTables* m_srcTables;

};
