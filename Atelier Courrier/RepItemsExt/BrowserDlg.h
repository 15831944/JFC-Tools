#pragma once
#include "resource.h"
#include "HtmlBrowser.h"


// CBrowserDlg dialog

class AFX_EXT_CLASS CBrowserDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrowserDlg)

public:
	CBrowserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrowserDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_RI_BROWSER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ReSize();

	DECLARE_MESSAGE_MAP()
public:
	CHtmlBrowser m_Browser;
	CString m_AddrLink;
	CString m_DlgHeader;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPrintPreview();
};
