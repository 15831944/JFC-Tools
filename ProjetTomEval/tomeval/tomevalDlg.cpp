// tomevalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tomeval.h"
#include "tomevalDlg.h"
#include <dos.h>
#include <direct.h>
extern CTomevalApp theApp;



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
	virtual BOOL OnInitDialog();
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
// CTomevalDlg dialog

CTomevalDlg::CTomevalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTomevalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTomevalDlg)
	m_NrLien = 0;
	m_fQuickJob = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTomevalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTomevalDlg)
	DDX_Control(pDX, IDC_PROGRESS2, m_Progress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	DDX_Control(pDX, IDC_TOMCDZEND, m_Bouton_TomCDZend);
	DDX_Control(pDX, IDC_TOMCDZ, m_Bouton_TomCDZ);
	DDX_Control(pDX, IDC_EVAL2, m_Bouton_Eval);
	DDX_Control(pDX, IDC_BREAK, m_BoutonBreak);
	DDX_Control(pDX, IDC_AUREVOIR, m_Bouton_Aurevoir);
	DDX_Control(pDX, IDC_ADDZOOM, m_Bouton_AddZoom);
	DDX_Control(pDX, IDC_ADDSPOT, m_Bouton_AddSpot);
	DDX_Text(pDX, IDC_EDIT1, m_NrLien);
	DDV_MinMaxInt(pDX, m_NrLien, 0, 9);
	DDX_Check(pDX, IDC_CHECK1, m_fQuickJob);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTomevalDlg, CDialog)
	//{{AFX_MSG_MAP(CTomevalDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_AUREVOIR, OnBoutonAurevoir)
	ON_BN_CLICKED(IDC_BREAK, OnBoutonBreak)
	ON_BN_CLICKED(IDC_TOMCDZ, OnBouton_TomCDZReady)
	ON_BN_CLICKED(IDC_TOMCDZ2, OnBouton_TomCDZEnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADDSPOT, OnBouton_Addspot)
	ON_BN_CLICKED(IDC_ADDZOOM, OnBouton_AddZoom)
	ON_BN_CLICKED(IDC_GETLIEN, OnBouton_GetNrLien)
	ON_BN_CLICKED(IDC_EVAL2, OnBoutonEvaluation)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSGTOM_TOMJOB_READY, OnMsgTom_TomjobReady)
	ON_MESSAGE(MSGTOM_AUREVOIR, OnMsgTom_AuRevoir)
	ON_MESSAGE(MSGTOM_BREAKCALCUL, OnMsgTom_BreakCalcul)

	ON_MESSAGE(MSGTOM_TOMCDZ_READY, OnMsgTom_TOMCDZReady)
	ON_MESSAGE(MSGTOM_TOMCDZ_END, OnMsgTom_TOMCDZEnd)
	ON_MESSAGE(MSGTOM_TOMCDZ_GETRESTOM, OnMsgTom_TOMCDZGetRestom)
	ON_MESSAGE(MSGTOM_ADDSPOT, OnMsgTom_AddSpot)
	ON_MESSAGE(MSGTOM_ADDZOOM, OnMsgTom_AddZoom)
	ON_MESSAGE(MSGTOM_GETLIEN, OnMsgTom_GetLien)

	ON_MESSAGE(MSGTOM_INFO_LOADHABITUDE, OnMsgTom_Info1)
	ON_MESSAGE(MSGTOM_INFO_PROGRESS, OnMsgTom_Info2)
	ON_MESSAGE(MSGTOM_RESTOM_READY, OnMsgTom_RestomReady)
	ON_MESSAGE(MSGTOM_TOMJOB_HIDE,OnMsgTom_HideTomeval)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTomevalDlg message handlers

BOOL CTomevalDlg::OnInitDialog()
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
	
	// extra initialization here
	
	if(theApp.m_hWndMaitre>0){
		::PostMessage(theApp.m_hWndMaitre,MSGTOM_BONJOUR,0,(LPARAM)MAKELPARAM(m_hWnd,0));
#ifdef OPTION_JFC
		ShowWindow(SW_MINIMIZE);
		PostMessage(MSGTOM_TOMJOB_HIDE);
#else
		ShowWindow(SW_MINIMIZE);
#endif
#ifdef OPTION_MEDIACONSULT
		PostMessage(MSGTOM_TOMJOB_HIDE);
#endif


		m_Bouton_TomCDZend.EnableWindow(FALSE);
		m_Bouton_TomCDZ.EnableWindow(FALSE);
		m_Bouton_Eval.EnableWindow(FALSE);
		m_BoutonBreak.EnableWindow(FALSE);
		//m_Bouton_Aurevoir.EnableWindow(FALSE);
		m_Bouton_AddZoom.EnableWindow(FALSE);
		m_Bouton_AddSpot.EnableWindow(FALSE);
	}
	else {
		// 
		theApp.m_hWndMaitre=m_hWnd;
	}
	
#ifdef OPTION_MEDIATOP
	PostMessage(MSGTOM_TOMJOB_READY);
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTomevalDlg::OnPaint() 
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


UINT LanceJobBackround( LPVOID pParam )
{
	CTomevalDlg	*pDlg=(CTomevalDlg *)pParam;
	pDlg->m_Eval.LanceEval(pDlg->m_hWnd,theApp.m_hWndMaitre,theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM);
    return 0;    // thread completed successfully
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_TomjobReady(WPARAM wParam, LPARAM lParam)
{
#ifdef OPTION_CARAT
	m_Eval.LanceEval(m_hWnd,theApp.m_hWndMaitre,theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM);
	return(0);
#endif

#ifdef OPTION_JFC
	theApp.m_hWndMaitre=(HWND)LOWORD(wParam);
#endif
#ifdef OPTION_MEDIACONSULT
	theApp.m_hWndMaitre=(HWND)LOWORD(wParam);
	LanceJobBackround(this);
	ReplyMessage(1);
	return(0);
#endif

	AfxBeginThread(LanceJobBackround,this);
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_AuRevoir(WPARAM wParam, LPARAM lParam){
#ifdef OPTION_MEDIACONSULT
	// On ne quitte que lorsqu tous les liens ont été liberés
	if(m_Eval.GetNbLienAffecte()>0)return(0);
#endif
	if(m_Eval.TerminerPossible())OnOK();
	return(0);
}


afx_msg LRESULT CTomevalDlg::OnMsgTom_BreakCalcul(WPARAM wParam, LPARAM lParam){
	m_Eval.BreakCalcul();
	return(0);
}

void CTomevalDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CTomevalDlg::OnBoutonAurevoir() 
{
	// TODO: Add your control notification handler code here
	PostMessage(MSGTOM_AUREVOIR,0,0L);
}

void CTomevalDlg::OnBoutonBreak() 
{
	// TODO: Add your control notification handler code here
	PostMessage(MSGTOM_BREAKCALCUL,0,0L);
}


afx_msg LRESULT CTomevalDlg::OnMsgTom_HideTomeval(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	return(0);
}


afx_msg LRESULT CTomevalDlg::OnMsgTom_GetLien(WPARAM wParam, LPARAM lParam)
{
	short r=m_Eval.GetNrLien();
	ReplyMessage(r);
	return(r);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_Info1(WPARAM wParam, LPARAM lParam)
{
	m_Progress1.SetPos(wParam);
	UpdateWindow();
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_Info2(WPARAM wParam, LPARAM lParam)
{
	m_Progress2.SetPos(lParam);
	UpdateWindow();
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_RestomReady(WPARAM wParam, LPARAM lParam)
{
#ifdef OPTION_MEDIATOP
	PostMessage(MSGTOM_AUREVOIR);
	return(0);
#endif

	AfficheMessage("Evaluation terminée !");
	return(0);
}


afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZReady(WPARAM wParam, LPARAM lParam)
{
	// wParam est le numéro du lien recherché
	m_Eval.LanceEvalMarginal((short)wParam,theApp.m_hWndMaitre,theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM,(long)lParam);
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZEnd(WPARAM wParam, LPARAM lParam)
{
	// wParam est le numéro du lien recherché
	m_Eval.TermineEvalMarginal((short)wParam);
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZGetRestom(WPARAM wParam, LPARAM lParam)
{
	// wParam est le numéro du lien recherché
	m_Eval.GetRestom((short)wParam);
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_AddSpot(WPARAM wParam, LPARAM lParam)
{
	if(m_Eval.AjouteSpotMarginal((short)wParam)){
//		AfficheErreur("Erreur: AddSpot dans le vide");	liem 25/11/04
		ReplyMessage(0);
	}
	ReplyMessage(1);
	return(0);
}

afx_msg LRESULT CTomevalDlg::OnMsgTom_AddZoom(WPARAM wParam, LPARAM lParam)
{
	if(m_Eval.AjouteSpotZoom((short)wParam)){
//		AfficheErreur("Erreur: AddSpot dans le vide");	liem 25/11/04
		ReplyMessage(0);
	}
	ReplyMessage(1);
	return(0);
}

void CTomevalDlg::OnBouton_TomCDZReady() 
{
	UpdateData(1);
	long l=0;
	if(m_fQuickJob)l=1;
	SendMessage(MSGTOM_TOMCDZ_READY,m_NrLien,l);
	AfxMessageBox("yo");
	
}

void CTomevalDlg::OnBouton_TomCDZEnd() 
{
	UpdateData(1);
	SendMessage(MSGTOM_TOMCDZ_END,m_NrLien,0L);
}

void CTomevalDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CTomevalDlg::OnBouton_Addspot() 
{
	UpdateData(1);
	SendMessage(MSGTOM_ADDSPOT,m_NrLien,0L);
	AfxMessageBox("yo");
}


void CTomevalDlg::OnBouton_AddZoom() 
{
	UpdateData(1);
	SendMessage(MSGTOM_ADDZOOM,m_NrLien,0L);
	AfxMessageBox("yo");
}

void CTomevalDlg::OnBouton_GetNrLien() 
{
	m_NrLien=(short)SendMessage(MSGTOM_GETLIEN,0,0L);
	UpdateData(0);
}

//char TomevalClassName[]={"EddieGentLaClasse"};
BOOL CTomevalDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
//	cs.lpszClass=TomevalClassName;
	
	return CDialog::PreCreateWindow(cs);
}

void CTomevalDlg::OnBoutonEvaluation() 
{
	MessageBox("Lancement de l'évaluation....");	
	PostMessage(MSGTOM_TOMJOB_READY,0,0L);
}

void CTomevalDlg::OnSysCommand(UINT nID, LPARAM lParam) 
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

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG:  This was added by System Info Component.

	// CG: Following block was added by System Info Component.
	{
		CString strFreeDiskSpace;
		CString strFreeMemory;
		CString strFmt;

		// Fill available memory
		MEMORYSTATUS MemStat;
		MemStat.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&MemStat);
		strFmt.LoadString(CG_IDS_PHYSICAL_MEM);
		strFreeMemory.Format(strFmt, MemStat.dwTotalPhys / 1024L);

		//TODO: Add a static control to your About Box to receive the memory
		//      information.  Initialize the control with code like this:
		SetDlgItemText(IDC_PHYSICAL_MEM, strFreeMemory);

		// Fill disk free information
		struct _diskfree_t diskfree;
		int nDrive = _getdrive(); // use current default drive
		if (_getdiskfree(nDrive, &diskfree) == 0)
		{
			strFmt.LoadString(CG_IDS_DISK_SPACE);
			strFreeDiskSpace.Format(strFmt,
				(DWORD)diskfree.avail_clusters *
				(DWORD)diskfree.sectors_per_cluster *
				(DWORD)diskfree.bytes_per_sector / (DWORD)1024L,
				nDrive-1 + _T('A'));
		}
		else
			strFreeDiskSpace.LoadString(CG_IDS_DISK_SPACE_UNAVAIL);

		//TODO: Add a static control to your About Box to receive the memory
		//      information.  Initialize the control with code like this:
		SetDlgItemText(IDC_DISK_SPACE, strFreeDiskSpace);
	}

	return TRUE;	// CG:  This was added by System Info Component.

}

