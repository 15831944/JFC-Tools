#pragma once

#include "BtnST.h"
#include "Label.h"
#include "ClrSelComboBox.h"
#include "JATPDocCreuset.h"
#include <GridCtrl.h>
#include <GridCellNumClick.h>
#include "Courbes.h"
#include "resource.h"

// Boîte de dialogue CDlgCourbeReponse

class CDlgCourbeReponse : public CDialog
{
	DECLARE_DYNAMIC(CDlgCourbeReponse)

public:
	CDlgCourbeReponse(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgCourbeReponse();

// Données de boîte de dialogue
	enum { IDD = IDD_CR_CRBREPONSE };

	virtual void SetDocument(JATPDocCreuset* pATPDocCreuset);
	void Update();
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	void OnUpdate();

	DECLARE_MESSAGE_MAP()
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBtnX();
	afx_msg void OnSelChangeComboCrb();
	afx_msg void OnClickedBtnCreer();
	afx_msg void OnClickedBtnModif();
	afx_msg void OnClickedBtnSuppr();
	afx_msg void OnClickBtnAuto();
	afx_msg void OnKillFocusBeta();


private:
	void SetupControls();
	void InitGrid();

	void FillCombo(int idSel);

	CClrSelComboBox m_Combo;
	CLabel m_bkg;
	CLabel m_ttl;
	CLabel m_Txt;
	CButtonST m_btnX;
	CButtonST m_btnCreer;
	CButtonST m_btnModif;
	CButtonST m_btnSuppr;
	
	CGridCtrl m_gridValues;

	JATPDocCreuset* m_pATPDocCreuset;
	CCourbes* m_courbes;

	int m_beta;

};
