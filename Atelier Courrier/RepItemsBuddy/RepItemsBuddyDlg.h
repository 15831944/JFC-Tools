// RepItemsBuddyDlg.h : fichier d'en-tête
//

#pragma once

class CRepItemsBuddyProxy;


// boîte de dialogue CRepItemsBuddyDlg
class CRepItemsBuddyDlg : public CDialog
{
	DECLARE_DYNAMIC(CRepItemsBuddyDlg);
	friend class CRepItemsBuddyProxy;

// Construction
public:
	CRepItemsBuddyDlg(CWnd* pParent = NULL);	// constructeur standard
	virtual ~CRepItemsBuddyDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_REPITEMSBUDDY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge DDX/DDV


// Implémentation
protected:
	CRepItemsBuddyProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
};
