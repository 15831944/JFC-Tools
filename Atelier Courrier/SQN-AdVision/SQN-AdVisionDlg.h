/*
//	JFC Informatique et M�dia
//	2005
//	Classe:
//	CSQNAdVisionDlg
//
//	Descriptif:
//	Boite de dialogue du lanceur de SQNAdVision
*/

// SQNAdVisionDlg.h : fichier d'en-t�te
//

#pragma once

#include"ATPComm.h"

#include "RepItems.h"
#include "Advision.h"

// JFCControls
#include "ATPColors.h"
#include "BtnST.h"
#include "Label.h"

// bo�te de dialogue CSQNAdVisionDlg
class CSQNAdVisionDlg : public CDialog
{
// Construction
public:
	CSQNAdVisionDlg(CWnd* pParent = NULL);	// constructeur standard
	~CSQNAdVisionDlg();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ATP_DIALOG };

	void SetCmdInfo(CATPComm *cmdInfo);

	void LoadApps();

// Impl�mentation
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge DDX/DDV

	// Fonctions g�n�r�es de la table des messages
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

	//les �l�ments de la boite
	CLabel m_bk;
	CButtonST m_btnRItem;
	CButtonST m_btnAnalyseur;

	//Les dlls
	CAdvision   *	m_Advision;
	CRepItems	*	m_RepItems;
	
private:
	void SetupControls();
	void SetColors();
 
	BOOL m_bFirstTime;
    static const CRect s_rectDefault;

public:
	afx_msg void OnAtpSeuilstat();
};