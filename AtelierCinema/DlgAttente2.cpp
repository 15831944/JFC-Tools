// DlgAttente.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgAttente.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAttente dialog


CDlgAttente::CDlgAttente(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttente::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAttente)
	m_NbPlan1 = 0;
	m_NbPlan2 = 0;
	//}}AFX_DATA_INIT
	m_Progression=0;
}


void CDlgAttente::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAttente)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Control(pDX, IDC_CADRE7, m_Cadre2);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Text(pDX, IDC_NBPLAN1, m_NbPlan1);
	DDX_Text(pDX, IDC_NBPLAN2, m_NbPlan2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAttente, CDialog)
	//{{AFX_MSG_MAP(CDlgAttente)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDSTOPPER, OnStopper)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAttente message handlers

void CDlgAttente::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond; //(RGB(23,114,121));
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN3);
	fond.CreatePatternBrush(&Bitmap);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgAttente::OnCancel() 
{
	m_StopRequest++;
	m_FlagAnnuler=1;
	
	//CDialog::OnCancel();
}

BOOL CDlgAttente::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ProgressBar.SetStartColor(RGB(255,0,0));
	m_ProgressBar.SetEndColor(RGB(0,255,0));
	m_ProgressBar.ShowPercent();
	m_ProgressBar.SetPos(0);
	
	SetTimer(1,1000,NULL);

	if(m_OptimisationTermine)EndDialog(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAttente::OnTimer(UINT nIDEvent) 
{
	if(IsWindowVisible())
	{
		if(m_OptimisationTermine)EndDialog(0);
		UpdateData(0);
		m_ProgressBar.SetPos(m_Progression);
	}
	else 
	{
//		KillTimer(1);
	}
	
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgAttente::OnStopper() 
{
	m_StopRequest++;
	
}
