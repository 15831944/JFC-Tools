/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CSQNProfilingDlg
//
//	Descriptif:
//	Boite de dialogue du lanceur de SQN-Profiling
*/


// SQN-ProfilingDlg.h : fichier d'en-tête
//

#pragma once

#include"ATPComm.h"

#include "RepItems.h"
#include "Profiling.h"

// JFCControls
#include "ATPColors.h"
#include "BtnST.h"
#include "Label.h"

// boîte de dialogue CSQNProfilingDlg
class CSQNProfilingDlg : public CDialog
{
// Construction
public:
	CSQNProfilingDlg(CWnd* pParent = NULL);	// constructeur standard
	~CSQNProfilingDlg();

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
	afx_msg void OnBnClickedAtpRepitem();
	afx_msg void OnBnClickedAtpAnalyseur();
	afx_msg void OnAboutbox();
	afx_msg void OnAtpPath();
	afx_msg LRESULT OnMessageFromModule(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSysColorChange();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//les infos de ligne de commande
	CATPComm* m_pcmdInfo;
	BOOL bFisrt;

	//les éléments de la boite
	CLabel m_bk;
	CButtonST m_btnRItem;
	CButtonST m_btnAnalyseur;

	//Les dlls
	CProfiling*		m_Profiling;
	CRepItems	*	m_RepItems;

	//

	
private:
	void SetupControls();
	void SetColors();
 
	BOOL m_bFirstTime;
    static const CRect s_rectDefault;

public:
//	afx_msg void OnAtpSeuilstat();
};
