/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CAtelierPresseDlg
//
//	Descriptif:
//	Boite de dialogue du lanceur de l'Atelier Presse
*/

// Atelier PresseDlg.h : fichier d'en-tête
//

#pragma once

#include"ATPComm.h"

#include "RepItems.h"
#include "RepSupports.h"
#include "Analyseur.h"
#include "Creuset.h"
#include "ChoixSupport.h"

// JFCControls
#include "ATPColors.h"
#include "BtnST.h"
#include "Label.h"
#include "ETSLayout.h"

// boîte de dialogue CAtelierPresseDlg
class CAtelierPresseDlg : public ETSLayoutDialog
{
// Construction
public:
	CAtelierPresseDlg(CWnd* pParent = NULL);	// constructeur standard
	~CAtelierPresseDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_ATP_DIALOG };

	void SetCmdInfo(CATPComm *cmdInfo);

	void LoadApps();

// Implémentation
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge DDX/DDV

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();
	virtual BOOL ContinueModal();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedAtpCreuset();
	afx_msg void OnBnClickedAtpPlan();
	afx_msg void OnBnClickedAtpRepitem();
	afx_msg void OnBnClickedAtpRepsupport();
	afx_msg void OnBnClickedAtpNomencl();
	afx_msg void OnBnClickedAtpAnalyseur();
	afx_msg void OnBnClickedChoixsupport();
	// afx_msg void OnBnClickedQuickPlan();
	afx_msg void OnAboutbox();
	afx_msg void OnAtpPath();
	afx_msg void OnClose();
	afx_msg LRESULT OnMessageFromModule(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSysColorChange();
	afx_msg void OnAtpSeuilstat();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()


private:
	//les infos de ligne de commande
	CATPComm* m_pcmdInfo;
	BOOL bFisrt;

	//les éléments de la boite
	CLabel m_bk;
	CButtonST m_btnCreuset;
	CButtonST m_btnPlan;
	CButtonST m_btnRItem;
	CButtonST m_btnRSupport;
	// CButtonST m_btnNomencl;
	CButtonST m_btnAnalyseur;
	CButtonST m_btnChoixSupport;
	// CButtonST m_btnQuickPlan;

	//Les dlls
	CCreuset		m_Creuset;
	CCreuset		m_QuickPlan;
	CChoixSupport*	m_ChoixSupport;
	CAnalyseur*		m_Analyseur;
	CRepItems	*	m_RepItems;
	CRepSupports*	m_RepSupports;

	//
	CString m_nomencPath;
	PROCESS_INFORMATION m_ProcessInfo;

	
private:
	void SetupControls();
	void SetColors();
	void ValidNomenc();
 
	BOOL m_bFirstTime;
    static const CRect s_rectDefault;

};
