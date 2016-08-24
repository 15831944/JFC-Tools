// DlgGraphe.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgGraphe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphe dialog


CDlgGraphe::CDlgGraphe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGraphe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphe)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll);
	DDX_Control(pDX, IDC_GRILLE4, m_Grille4);
	DDX_Control(pDX, IDC_GRILLE3, m_Grille3);
	DDX_Control(pDX, IDC_GRILLE2, m_Grille2);
	DDX_Control(pDX, IDC_GRILLE1, m_Grille1);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphe, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphe)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphe message handlers

BOOL CDlgGraphe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGraphe::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
	FillRect(dc.m_hDC,rect,theApp.hBrush[CLR_FONDDLGGRAPH]);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CDlgGraphe::Initialise(CCampagne * pCampagne,int style)
{
	m_pCampagne=pCampagne;

	m_Grille1.m_pCampagne=pCampagne;
	m_Grille1.m_Part=2;

	m_Grille2.m_pCampagne=pCampagne;
	m_Grille2.m_Part=3;

	m_Grille3.m_pCampagne=pCampagne;
	m_Grille3.m_Part=1;

	m_Grille4.m_pCampagne=pCampagne;
	m_Grille4.m_Part=0;

	m_Cadre.SetFond(RGB(128,128,128));

	m_Grille1.m_Style=m_Grille2.m_Style=m_Grille3.m_Style=m_Grille4.m_Style=style;

	// préparation des données pour dessiner

	int LargColone;
	int NbColone;
	int HauteurGraphe;
	int OrigineY;
	int HautLigneTitre;

	RECT R;
	m_Grille2.GetWindowRect(&R);
	LargColone=LARGCOLONE_ACCOGRAP;
	if(style==1)LargColone/=2;
	NbColone=(R.right-R.left+1)/LargColone;
	HauteurGraphe=R.bottom-R.top-2*INTERECH_ACCOGRAP;
	OrigineY=R.bottom-R.top-INTERECH_ACCOGRAP;
	m_Grille4.GetWindowRect(&R);
	HautLigneTitre=(R.bottom-R.top)/2;

	m_Grille1.m_LargColone    = m_Grille2.m_LargColone     = m_Grille3.m_LargColone     = m_Grille4.m_LargColone=LargColone;
	m_Grille1.m_NbColone      = m_Grille2.m_NbColone       = m_Grille3.m_NbColone       = m_Grille4.m_NbColone=NbColone;
	m_Grille1.m_HauteurGraphe = m_Grille2.m_HauteurGraphe  = m_Grille3.m_HauteurGraphe  = m_Grille4.m_HauteurGraphe=HauteurGraphe;
	m_Grille1.m_OrigineY      = m_Grille2.m_OrigineY       = m_Grille3.m_OrigineY       = m_Grille4.m_OrigineY=OrigineY;
	m_Grille1.m_HautLigneTitre= m_Grille2.m_HautLigneTitre = m_Grille3.m_HautLigneTitre = m_Grille4.m_HautLigneTitre=HautLigneTitre;

	// les ascenceurs
	
	//m_Grille1.m_hAscH=m_Grille2.m_hAscH=m_Grille3.m_hAscH=m_Grille4.m_hAscH=m_Scroll.m_hWnd;
	InitAscenceur();
}

void CDlgGraphe::InitAscenceur()
{
	m_Grille1.m_PosAscH=m_Grille2.m_PosAscH=m_Grille3.m_PosAscH=m_Grille4.m_PosAscH=0;

	int maxi=max(0,m_pCampagne->m_NbSemaineCalcul  - m_Grille1.m_NbColone);
	m_Scroll.SetScrollPos(m_Grille1.m_PosAscH);
	m_Scroll.SetScrollRange(0,maxi);
}


void CDlgGraphe::MajAffichageACCOGRAP()
{
	m_Grille1.CalculEchelleACCOGRAP();
	m_Grille2.CalculEchelleACCOGRAP();
	Invalidate(FALSE);
	UpdateWindow();

}


void CDlgGraphe::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CCampagneGenerique * C=m_pCampagne;
	if(pScrollBar==&m_Scroll)
	{
		HWND h=m_Scroll.m_hWnd;
		int i, nb, mini, maxi;


		i=::GetScrollPos(h,SB_CTL);
		nb=nPos;
		::GetScrollRange(h,SB_CTL,&mini,&maxi);

		switch(nSBCode) {
			case SB_BOTTOM: 	::SetScrollPos(h,SB_CTL,maxi,1); break;
			case SB_ENDSCROLL: 	break;
			case SB_LINEDOWN: 	::SetScrollPos(h,SB_CTL,min(i+1,maxi),1); break;
			case SB_LINEUP:		::SetScrollPos(h,SB_CTL,max(i-1,mini),1); break;
			case SB_PAGEDOWN:	::SetScrollPos(h,SB_CTL,min(i+(C->m_NbColone),maxi),1); break;
			case SB_PAGEUP:		::SetScrollPos(h,SB_CTL,max(i-(C->m_NbColone),mini),1); break;
			case SB_TOP:		::SetScrollPos(h,SB_CTL,mini,1); break;

			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:	::SetScrollPos(h,SB_CTL,nb,1); break;
		}

		// au besoin on redessine le bouton à qui il est associé
		if((m_Grille1.m_PosAscH=::GetScrollPos(h,SB_CTL))==i)return;
		m_Grille2.m_PosAscH=m_Grille3.m_PosAscH=m_Grille4.m_PosAscH=m_Grille1.m_PosAscH;
		m_Grille2.Invalidate();
		m_Grille4.Invalidate();
		m_Grille2.UpdateWindow();
		m_Grille4.UpdateWindow();
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
