#pragma once

#include <Label.h>
#include <BtnST.h>
#include "AnalyseurDoc.h"

//#include "afxwin.h"


// CDlgParams dialog

class CDlgParams : public CDialog
{
	DECLARE_DYNAMIC(CDlgParams)

public:
	CDlgParams(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgParams();
	void	SetParams(CAnalyseurDoc *pDoc);

// Dialog Data
	enum { IDD = IDD_AN_DLG_PARAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetupControls();
	void SetColors();
	void SetControls();

	CAnalyseurDoc *		m_pDoc;
	bool				m_bCible;
	JUnt32				m_TableStyle;

	CButtonST			m_But_OK;
	CButtonST			m_But_CANCEL;

	CLabel				m_Box1;
	CLabel				m_Label_Base100;
	CButtonST			m_Radio_Base_Univers;
	CButtonST			m_Radio_Base_Niche;

	CLabel				m_Box2;
	CLabel				m_Label_OptionsL;
	CButtonST			m_Check_Option_NicheL;
	CButtonST			m_Check_Option_ContreNicheL;
	CButtonST			m_Check_Option_ItemMoyenL;
	CButtonST			m_Check_Option_ItemsL;
	CLabel				m_Label_OptionsC;
	CButtonST			m_Check_Option_NicheC;
	CButtonST			m_Check_Option_ContreNicheC;
	CButtonST			m_Check_Option_ItemMoyenC;
	CButtonST			m_Check_Option_ItemsC;
	CLabel				m_Label_OptionsP;
	CButtonST			m_Check_Option_NicheP;
	CButtonST			m_Check_Option_ContreNicheP;
	CButtonST			m_Check_Option_ItemMoyenP;
	CButtonST			m_Check_Option_ItemsP;

	CLabel				m_Label_Grandeurs;
	CButtonST			m_Radio_Grandeurs_Ligne;
	CButtonST			m_Radio_Grandeurs_Colonne;

	CLabel				m_Label_Terrains;
	CButtonST			m_Radio_Terrains_Ligne;
	CButtonST			m_Radio_Terrains_Colonne;

	CLabel				m_Label_Surlignage;
	CButtonST			m_Radio_Surlignage_Page;
	CButtonST			m_Radio_Surlignage_Global;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAnBaseUnivers();
	afx_msg void OnBnClickedAnBaseNiche();
	afx_msg void OnBnClickedAnOptionNicheL();
	afx_msg void OnBnClickedAnOptionContrenicheL();
	afx_msg void OnBnClickedAnOptionItemmoyenL();
	afx_msg void OnBnClickedAnOptionItemsL();
	afx_msg void OnBnClickedAnOptionNicheC();
	afx_msg void OnBnClickedAnOptionContrenicheC();
	afx_msg void OnBnClickedAnOptionItemmoyenC();
	afx_msg void OnBnClickedAnOptionItemsC();
	afx_msg void OnBnClickedAnOptionNicheP();
	afx_msg void OnBnClickedAnOptionContrenicheP();
	afx_msg void OnBnClickedAnOptionItemmoyenP();
	afx_msg void OnBnClickedAnOptionItemsP();
	afx_msg void OnBnClickedAnGrandeurLigne();
	afx_msg void OnBnClickedAnGrandeurColonne();
	afx_msg void OnBnClickedAnTerrainLigne();
	afx_msg void OnBnClickedAnTerrainColonne();
	afx_msg void OnBnClickedAnSurlignagePage();
	afx_msg void OnBnClickedAnSurlignageGlobal();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
