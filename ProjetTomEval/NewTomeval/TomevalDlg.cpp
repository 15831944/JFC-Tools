// TomevalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TomevalApp.h"
extern CTomevalApp theApp;
#include "TomevalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTomevalDlg dialog

//##ModelId=3D9AB7D40141
CTomevalDlg::CTomevalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTomevalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTomevalDlg)
	m_NrLien = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//##ModelId=3D9AB7D401CD
void CTomevalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTomevalDlg)
	DDX_Text(pDX, IDC_EDIT1, m_NrLien);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTomevalDlg, CDialog)
	//{{AFX_MSG_MAP(CTomevalDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtomTomjobReady)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonAUREVOIR)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP

	// Les messages du protocole de comunication avec Tomeval
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

//##ModelId=3D9AB7D401E0
BOOL CTomevalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Extra initialization here
	if(theApp.m_hWndMaitre)
	{
#ifndef _DEBUG

		// NOTE: après un long débuggage, je ne sais toujours pas pourquoi, si on ne
		// fais pas ce minimize, l'application ne se cache JAMAIS. Donc cela reste comique,
		// mais il faut minimiser avant de hider.
		// Il faut concevoir que ce défaut ne se constate qu'avec l'Atelier Radio. donc je continue de croire,
		// qu'une des application appelante (CREUSET, CIBLE ou WinTM ?), envoie un showwindow...
		ShowWindow(SW_MINIMIZE);
		PostMessage(MSGTOM_TOMJOB_HIDE);
#endif
		::PostMessage(theApp.m_hWndMaitre,MSGTOM_BONJOUR,0,(LPARAM)MAKELPARAM(m_hWnd,0));
	}
	else 
		theApp.m_hWndMaitre=m_hWnd;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//##ModelId=3D9AB7D401E2
void CTomevalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

//##ModelId=3D9AB7D401F5
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//##ModelId=3D9AB7D401FE
HCURSOR CTomevalDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//##ModelId=3D9AB7D40200
void CTomevalDlg::OnButtomTomjobReady() 
{
	MessageBox("Lancement de l'évaluation....");	
	PostMessage(MSGTOM_TOMJOB_READY,0,0L);
	
}

//##ModelId=3D9AB7D4014A
afx_msg LRESULT CTomevalDlg::OnMsgTom_TomjobReady(WPARAM wParam, LPARAM lParam)
{
#ifdef OPTION_JFC
	// Le numéro de fenêtre de l'application qui est transmis est celui de l'étude
	// en fait les messages doivent êtres transmis à une autre application
	// qui est WinTM dans le cas de l'Atelier Radio.
	theApp.m_hWndMaitre=(HWND)wParam;
	m_TomEval.LanceCalculClassique("DATATM",theApp.m_NomFicRESTOM,"CIBJOB");
#else
	m_TomEval.LanceCalculClassique(theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM);
#endif
#ifndef OPTION_CARAT
	if(theApp.m_hWndMaitre>0)
	{
//		CString txt;
//		txt.Format("Le message MSGTOM_RESTOM_READY va être envoyé à %d",theApp.m_hWndMaitre);
//		AfxMessageBox(txt);
		::PostMessage(theApp.m_hWndMaitre,MSGTOM_RESTOM_READY,0,0L);
	}
#endif
	return(0);
}

//##ModelId=3D9AB7D40154
afx_msg LRESULT CTomevalDlg::OnMsgTom_AuRevoir(WPARAM wParam, LPARAM lParam)
{
	EndDialog(1);
	return(0);
}

//##ModelId=3D9AB7D4015E
afx_msg LRESULT CTomevalDlg::OnMsgTom_BreakCalcul(WPARAM wParam, LPARAM lParam)
{
	return(0);
}

//##ModelId=3D9AB7D40168
afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZReady(WPARAM wParam, LPARAM lParam)
{
	// wParam est le numéro du lien recherché

	//m_Eval.LanceEvalMarginal((short)wParam,theApp.m_hWndMaitre,theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM,(long)lParam);
#ifndef OPTION_CARAT
	m_TomMarginal.CalculJob((short)wParam);
#endif
	return(0);
}
//##ModelId=3D9AB7D4016C
afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZEnd(WPARAM wParam, LPARAM lParam)
{
	// wParam est le numéro du lien recherché
#ifndef OPTION_CARAT
	m_TomMarginal.ReleaseNrLien((short)wParam);
#endif	
	return(0);
}
//##ModelId=3D9AB7D40175
afx_msg LRESULT CTomevalDlg::OnMsgTom_TOMCDZGetRestom(WPARAM wParam, LPARAM lParam)
{
	// Ecrire le fichier restom
#ifndef OPTION_CARAT
	m_TomMarginal.WriteRestom((short)wParam);
#endif	
	return(0);
}
//##ModelId=3D9AB7D4017F
afx_msg LRESULT CTomevalDlg::OnMsgTom_AddSpot(WPARAM wParam, LPARAM lParam)
{
#ifndef OPTION_CARAT
	m_TomMarginal.CalculAddSpot((short)wParam);
	ReplyMessage(1);
#endif
	return(0);
}
//##ModelId=3D9AB7D40190
afx_msg LRESULT CTomevalDlg::OnMsgTom_AddZoom(WPARAM wParam, LPARAM lParam)
{
#ifndef OPTION_CARAT
	m_TomMarginal.CalculZoom((short)wParam);
	ReplyMessage(1);
#endif
	return(0);
}
//##ModelId=3D9AB7D4019A
afx_msg LRESULT CTomevalDlg::OnMsgTom_GetLien(WPARAM wParam, LPARAM lParam)
{
#ifndef OPTION_CARAT
	int NrLien=m_TomMarginal.GetNrLien(theApp.m_NomFicTOMJOB,theApp.m_NomFicRESTOM);
	ReplyMessage(NrLien);
	return(NrLien);
#else
	return 0;
#endif
}

//##ModelId=3D9AB7D401A4
afx_msg LRESULT CTomevalDlg::OnMsgTom_Info1(WPARAM wParam, LPARAM lParam)
{
	return(0);
}
//##ModelId=3D9AB7D401AE
afx_msg LRESULT CTomevalDlg::OnMsgTom_Info2(WPARAM wParam, LPARAM lParam)
{
	return(0);
}
//##ModelId=3D9AB7D401B8
afx_msg LRESULT CTomevalDlg::OnMsgTom_RestomReady(WPARAM wParam, LPARAM lParam)
{
	MessageBox("Réception du message MSGTOM_RESTOM_READY");
	return(0);
}
//##ModelId=3D9AB7D401C2
afx_msg LRESULT CTomevalDlg::OnMsgTom_HideTomeval(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	return(0);
}


//##ModelId=3D9AB7D40208
void CTomevalDlg::OnButtonAUREVOIR() 
{
	PostMessage(MSGTOM_AUREVOIR,0,0L);
}

//##ModelId=3D9AB7D4020A
void CTomevalDlg::OnButton3() 
{
	m_NrLien=SendMessage(MSGTOM_GETLIEN);	
	UpdateData(0);
}

//##ModelId=3D9AB7D40213
void CTomevalDlg::OnButton4() 
{
	UpdateData(1);
	SendMessage(MSGTOM_TOMCDZ_END,m_NrLien,0L);
}

//##ModelId=3D9AB7D4021C
void CTomevalDlg::OnButton5() 
{
	UpdateData(1);
	SendMessage(MSGTOM_TOMCDZ_READY,m_NrLien,0L);
}

//##ModelId=3D9AB7D4021E
void CTomevalDlg::OnButton6() 
{
	UpdateData(1);
	SendMessage(MSGTOM_ADDSPOT,m_NrLien,0L);
}

//##ModelId=3D9AB7D40227
void CTomevalDlg::OnButton7() 
{
	UpdateData(1);
	SendMessage(MSGTOM_ADDZOOM,m_NrLien,0L);
}

//##ModelId=3D9AB7D40230
void CTomevalDlg::OnButton8() 
{
	UpdateData(1);
	SendMessage(MSGTOM_TOMCDZ_GETRESTOM,m_NrLien,0L);
}

void CTomevalDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}
