/*
//	JFC Informatique et M�dia
//	2005
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CDlgXLProgress
//
//	Descriptif:
//	Boite de dialogue modale avec un chenillard montrant que le systeme est occupp� � la mise en forme
*/

#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ATPColors.h"
//#include "XLPrintThread.h"
#include "KCBusyProgressCtrl.h"
#include "Label.h"
#include "BtnST.h"

class CXLPrintThread;

#define DLG_ERROR
//#define DLG_AFX

// Bo�te de dialogue CDlgXLProgress

class CDlgXLProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgXLProgress)

public:
	CDlgXLProgress(CXLPrintThread & XLPrintThread, CATPColors::APPCOLOR app, CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgXLProgress();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_XLP_DLGPROGRESS };

	void Error(CString errorMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();

	afx_msg void OnCancel(){};

	afx_msg void OnBnClickedBtnOK();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	static UINT Thread(VOID* pParam);

	void SetColors();

	CATPColors::APPCOLOR m_appColor;

	CXLPrintThread & m_XLPrintThread;

	CKCBusyProgressCtrl m_progress2;
	CLabel m_lbl;
	CButtonST m_btnOK;
};
