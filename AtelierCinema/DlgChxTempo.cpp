// DlgChxTempo.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgChxTempo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChxTempo dialog


CDlgChxTempo::CDlgChxTempo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChxTempo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChxTempo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgChxTempo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChxTempo)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Control(pDX, IDC_COMBO1, m_CB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChxTempo, CDialog)
	//{{AFX_MSG_MAP(CDlgChxTempo)
	ON_WM_PAINT()
	ON_CBN_DBLCLK(IDC_COMBO1, OnDblclkCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChxTempo message handlers

void CDlgChxTempo::OnOK() 
{
	m_Tempo=m_CB.GetCurSel();
	CDialog::OnOK();
}

void CDlgChxTempo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	/*
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN4);
	fond.CreatePatternBrush(&Bitmap);
	*/

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgChxTempo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CB.ResetContent();
	// récupération de la table des tempos
	CTableTempo * TabTempo = theApp.m_TableCinema.GetTableTempo(); 
	for(int i=0;i<TabTempo->GetSize();i++)
	{
		m_CB.AddString(TabTempo->GetLibelle(i));

	}
	m_CB.SetCurSel(m_Tempo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChxTempo::OnDblclkCombo1() 
{
	OnOK();	
}
