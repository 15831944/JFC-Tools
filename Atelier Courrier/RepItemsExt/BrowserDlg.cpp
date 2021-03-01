// BrowserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BrowserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBrowserDlg dialog

IMPLEMENT_DYNAMIC(CBrowserDlg, CDialog)
CBrowserDlg::CBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserDlg::IDD, pParent)
{
	m_AddrLink = "www.jfc-infomedia.fr";
	m_DlgHeader = "JFC Browser";
}

CBrowserDlg::~CBrowserDlg()
{
}

void CBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RI_EXPLORER, m_Browser);
}


BEGIN_MESSAGE_MAP(CBrowserDlg, CDialog)
	ON_WM_SIZE()
	ON_COMMAND(IDM_RI_PREVIEW, OnPrintPreview)
END_MESSAGE_MAP()


// CBrowserDlg message handlers
BOOL CBrowserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->SetWindowText(m_DlgHeader); 
	COleVariant vtEmpty;

	m_Browser.Navigate(m_AddrLink, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);

	ReSize();

	return true;
}

void CBrowserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	ReSize();	
}

void CBrowserDlg::ReSize()
{
	if (::IsWindow(m_Browser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		m_Browser.MoveWindow(rect);
	}
}

void CBrowserDlg::OnPrintPreview()
{
	// Get the HTMLDocument interface.
	m_Browser.ExecWB(OLECMDID_PRINTPREVIEW,0,NULL,NULL); 
}
