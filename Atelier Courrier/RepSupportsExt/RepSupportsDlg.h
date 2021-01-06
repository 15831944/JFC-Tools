/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CRepSupportsDlg
//
//	Descriptif:
//	Boite de base du répartiore des supports, charge les pages spécialisées
*/


#pragma once

#include ".\PageFamille\DlgFamille.h"
#include ".\PageLot\DlgLot.h"
#include ".\PageHS\DlgApparente.h"
#include ".\PageTarif\DlgTarif.h"
#include ".\PageCouplage\DlgCouplage.h"
#include ".\PageApparie\DlgApparie.h"
#include "DlgDataExo.h"

#include <Resizer.h>
#include <BtnST.h>
#include "Resource.h"

#include "ETSLayout.h"

// boîte de dialogue CRepSupportsDlg
class CRepSupportsDlg : public ETSLayoutDialog
{
// Construction
public:
	CRepSupportsDlg(CWnd* pParent = NULL);	// constructeur standard
	virtual ~CRepSupportsDlg();	// destructeur standard

// Données de boîte de dialogue
	enum { IDD = IDD_RS_REPSUPPORTS_DIALOG };


// Implémentation
protected:
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio_FAM();
	afx_msg void OnBnClickedRadio_LOT();
	afx_msg void OnBnClickedRadio_CPL();
	afx_msg void OnBnClickedRadio_HS();
	afx_msg void OnBnClickedRadio_APP();
	afx_msg void OnBnClickedRadio_TAR();
	afx_msg void OnBnClickedRadio_EXO();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy) ;
	afx_msg void OnAppAbout();
	afx_msg void OnUpdatePalette1(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette2(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette3(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette4(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette5(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette6(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette7(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette8(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette9(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette10(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette11(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette12(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette13(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette14(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette15(CCmdUI *pCmdUI);
	afx_msg void OnPalette1();
	afx_msg void OnPalette2();
	afx_msg void OnPalette3();
	afx_msg void OnPalette4();
	afx_msg void OnPalette5();
	afx_msg void OnPalette6();
	afx_msg void OnPalette7();
	afx_msg void OnPalette8();
	afx_msg void OnPalette9();
	afx_msg void OnPalette10();
	afx_msg void OnPalette11();
	afx_msg void OnPalette12();
	afx_msg void OnPalette13();
	afx_msg void OnPalette14();
	afx_msg void OnPalette15();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSysColorChange();

	afx_msg LRESULT OnSupportTreeNeedUpd(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	void SetWindowRelations();
	void SetupControls();
	void SetColors();

	void UpdateDlgPos(BOOL b_show = FALSE);

private:
//	CResizer m_Resize;

	CDialog*		m_pActivePage;///Le pointeur sur la page actuellement affichée
	//Les différentes pages
	CDlgFamille*	m_pPageFamille;
	CDlgLot*		m_pPageLot;
	CDlgApparente*	m_pPageHS;
	CDlgTarif*		m_pPageTarif;
	CDlgCouplage*	m_pPageCouplage;
	CDlgApparie*	m_pPageApparie;
	CDlgDataExo*	m_pPageDataExo;

	CButtonST m_btnFamille;
	CButtonST m_btnLot;
	CButtonST m_btnHS;
	CButtonST m_btnTarif;
	CButtonST m_btnCouplage;
	CButtonST m_btnApparie;
	CButtonST m_btnDataExo;

	CLabel m_bkgBtn;

	BOOL m_bFirstTime;
    static const CRect s_rectDefault;


};
