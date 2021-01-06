/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CPathDlg
//
//	Descriptif:
//	Boite de dialogue pour la saisie des chemins vers les dossiers utilis�s par SineQuaNon (Sources+User)
*/


#pragma once

// JFCControls
#include "ATPColors.h"
#include "BtnST.h"
#include "Label.h"
#include "FolderDialog.h"

// Bo�te de dialogue CPathDlg

class CPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CPathDlg)

public:
	CPathDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CPathDlg();

	virtual BOOL OnInitDialog();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ATP_DLGPATH };

	CString m_pathSrc;
	CString m_pathUser;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

private:
	void SetupControls();
	void SetColors();

	CLabel m_bk;
	CLabel m_ttl;
	CLabel m_lblSrc;
	CLabel m_lblUser;
	CLabel m_edtSrc;
	CLabel m_edtUser;
	CButtonST m_btnPathSrc;
	CButtonST m_btnPathUser;
	CButtonST m_btnOK;
	CButtonST m_btnCancel;

protected:
	afx_msg void OnBnClickedBtnPathSrc();
	afx_msg void OnBnClickedBtnPathUser();
};
