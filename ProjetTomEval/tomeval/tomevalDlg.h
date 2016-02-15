// tomevalDlg.h : header file
//
#include "eval.h"

#if !defined(AFX_TOMEVALDLG_H__E843E167_9722_11D1_A42E_004005327F70__INCLUDED_)
#define AFX_TOMEVALDLG_H__E843E167_9722_11D1_A42E_004005327F70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTomevalDlg dialog

class CTomevalDlg : public CDialog
{

	
	// Construction
public:
	CEval m_Eval;	

	CTomevalDlg(CWnd* pParent = NULL);	// standard constructor
    afx_msg LRESULT OnMsgTom_TomjobReady(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_AuRevoir(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_BreakCalcul(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnMsgTom_TOMCDZReady(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_TOMCDZEnd(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_TOMCDZGetRestom(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_AddSpot(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_AddZoom(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_GetLien(WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnMsgTom_Info1(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_Info2(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_RestomReady(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMsgTom_HideTomeval(WPARAM wParam, LPARAM lParam);

// Dialog Data
	//{{AFX_DATA(CTomevalDlg)
	enum { IDD = IDD_TOMEVAL_DIALOG };
	CProgressCtrl	m_Progress2;
	CProgressCtrl	m_Progress1;
	CButton	m_Bouton_TomCDZend;
	CButton	m_Bouton_TomCDZ;
	CButton	m_Bouton_Eval;
	CButton	m_BoutonBreak;
	CButton	m_Bouton_Aurevoir;
	CButton	m_Bouton_AddZoom;
	CButton	m_Bouton_AddSpot;
	short	m_NrLien;
	BOOL	m_fQuickJob;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTomevalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	char m_fAfficheMessage;

	// Generated message map functions
	//{{AFX_MSG(CTomevalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnBoutonAurevoir();
	afx_msg void OnBoutonBreak();
	afx_msg void OnBouton_TomCDZReady();
	afx_msg void OnBouton_TomCDZEnd();
	afx_msg void OnDestroy();
	afx_msg void OnBouton_Addspot();
	afx_msg void OnBouton_AddZoom();
	afx_msg void OnBouton_GetNrLien();
	afx_msg void OnBoutonEvaluation();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMEVALDLG_H__E843E167_9722_11D1_A42E_004005327F70__INCLUDED_)
