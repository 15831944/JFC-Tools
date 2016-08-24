// GetCellFromPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetCellFromPoint.h"
#include "GetCellFromPointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetCellFromPointDlg dialog

CGetCellFromPointDlg::CGetCellFromPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetCellFromPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetCellFromPointDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetCellFromPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetCellFromPointDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetCellFromPointDlg, CDialog)
	//{{AFX_MSG_MAP(CGetCellFromPointDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetCellFromPointDlg message handlers

BOOL CGetCellFromPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_grid.AttachGrid( this, IDC_GRID );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGetCellFromPointDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGetCellFromPointDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGetCellFromPointDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGetCellFromPointDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_LBUTTONDOWN )
	{
		CRect rect;
		m_grid.GetWindowRect( rect );

		if ( rect.PtInRect( pMsg->pt ))
		{
			int col = -99;
			long row = -99;
			CPoint point;

			// default implementation of the GetCellFromPoint will not provide
			// cell coordinates of heading cells
			point = pMsg->pt;
			m_grid.m_CUGGrid->ScreenToClient( &point );
			// get cell user clicked on
			m_grid.GetCellFromPoint( point.x, point.y, &col, &row );
			// show results
			if ( col == -99 )
				SetDlgItemText( IDC_COL, "error" );
			else
				SetDlgItemInt( IDC_COL, col );

			if ( row == -99 )
				SetDlgItemText( IDC_ROW, "error" );
			else
				SetDlgItemInt( IDC_ROW, row );

			// ** customized implementation of the GetCellFromPoint
			// reinitialize variables
			col = row = -99;
			// get cell user clicked on
			m_grid.FindCellFromPoint( pMsg->pt, col, row );
			// show results
			if ( col == -99 )
				SetDlgItemText( IDC_CUST_COL, "error" );
			else
				SetDlgItemInt( IDC_CUST_COL, col );

			if ( row == -99 )
				SetDlgItemText( IDC_CUST_ROW, "error" );
			else
				SetDlgItemInt( IDC_CUST_ROW, row );
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

