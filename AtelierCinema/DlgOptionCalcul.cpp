// DlgOptionCalcul.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgOptionCalcul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalcul dialog


CDlgOptionCalcul::CDlgOptionCalcul(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionCalcul::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionCalcul)
	m_Dememorisation = -1;
	m_Saisonnalite = -1;
	m_DureeTouteActive = -1;
	m_CNCActif_Inactif = -1;
	m_BaseRegionale = -1;
	m_RadioAgglo = -1;
	//}}AFX_DATA_INIT
}


void CDlgOptionCalcul::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionCalcul)
	DDX_Control(pDX, IDC_CADRE12, m_Cadre12);
	DDX_Control(pDX, IDC_CADRE9, m_Cadre9);
	DDX_Control(pDX, IDC_CADRE8, m_cadre8);
	DDX_Control(pDX, IDC_CADRE6, m_Cadre4);
	DDX_Control(pDX, IDC_CADRE5, m_Cadre3);
	DDX_Control(pDX, IDC_CADRE4, m_Cadre2);
	DDX_Radio(pDX, IDC_RADIODEMEMORISATION, m_Dememorisation);
	DDX_Radio(pDX, IDC_RADIOSAISONNALITE, m_Saisonnalite);
	DDX_Radio(pDX, IDC_RADIODUREE, m_DureeTouteActive);
	DDX_Radio(pDX, IDC_RADIOCNCA, m_CNCActif_Inactif);
	DDX_Radio(pDX, IDC_RADIOBASEN, m_BaseRegionale);
	DDX_Radio(pDX, IDC_RADIOAGGLO, m_RadioAgglo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionCalcul, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionCalcul)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalcul message handlers

void CDlgOptionCalcul::OnOK() 
{
	UpdateData();	
	CDialog::OnOK();
}

void CDlgOptionCalcul::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond; //(RGB(23,114,121));
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN4);
	fond.CreatePatternBrush(&Bitmap);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

