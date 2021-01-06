/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgDataExo
//
//	Descriptif:
//	Page permettant la gestion des formulaires des données exogènes
*/

#pragma once
#include "resource.h"

#include "Label.h"
#include "BtnST.h"
#include "ETSLayout.h"
#include "GridCtrl.h"

// Boîte de dialogue CDlgDataExo

class CDlgDataExo : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CDlgDataExo)

public:
	CDlgDataExo(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgDataExo();

// Données de boîte de dialogue
	enum { IDD = IDD_RS_DLGDATAEXO };

	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
//Ruse pour que la fenetre ne se ferme pas
	virtual void OnOK(){};
	virtual void OnCancel(){};
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:


	CLabel m_staticLstData;
	CLabel m_ttlLstData;
	CListBox m_list;

	CLabel m_staticData;
	CLabel m_ttlData;
	CGridCtrl m_gridValeur;

	CLabel m_staticBtn;
	CLabel m_ttlBtn;
	CButtonST m_btnExport;
	CButtonST m_btnImport;
	CButtonST m_btnNewForm;
	CLabel m_lblNbDecimales;
	CComboBox m_combo;

};
