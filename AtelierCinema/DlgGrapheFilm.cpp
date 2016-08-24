// DlgGrapheFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgGrapheFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGrapheFilm dialog


CDlgGrapheFilm::CDlgGrapheFilm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGrapheFilm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGrapheFilm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGrapheFilm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGrapheFilm)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll);
	DDX_Control(pDX, IDC_GRILLEF4, m_GrilleFilm4);
	DDX_Control(pDX, IDC_GRILLEF3, m_GrilleFilm3);
	DDX_Control(pDX, IDC_GRILLEF2, m_GrilleFilm2);
	DDX_Control(pDX, IDC_GRILLEF1, m_GrilleFilm1);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGrapheFilm, CDialog)
	//{{AFX_MSG_MAP(CDlgGrapheFilm)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGrapheFilm message handlers

void CDlgGrapheFilm::Initialise(CCampagneFilm * pCampagneFilm,int style)
{
	// Récupère Campagne Film pour chacune des grilles
	m_pCampagneFilm = pCampagneFilm;

	m_GrilleFilm1.m_pCampagneFilm = pCampagneFilm;
	m_GrilleFilm1.m_Part=2;

	m_GrilleFilm2.m_pCampagneFilm = pCampagneFilm;
	m_GrilleFilm2.m_Part=3;

	m_GrilleFilm3.m_pCampagneFilm = pCampagneFilm;
	m_GrilleFilm3.m_Part=1;

	m_GrilleFilm4.m_pCampagneFilm = pCampagneFilm;
	m_GrilleFilm4.m_Part=0;

	// Fond des gilles résultats
	m_Cadre.SetFond(RGB(128,128,128));

	// Style des grilles
	m_GrilleFilm1.m_Style = style;
	m_GrilleFilm2.m_Style = style;
	m_GrilleFilm3.m_Style = style;
	m_GrilleFilm4.m_Style = style;

	// préparation des données pour dessiner

	int LargColone;
	int NbColone;
	int HauteurGraphe;
	int OrigineY;
	int HautLigneTitre;

	RECT R;
	m_GrilleFilm2.GetWindowRect(&R);
	LargColone=LARGCOLONE_ACCOGRAP;
	if(style==1)LargColone/=2;

	NbColone      = (R.right-R.left+1)/LargColone;
	HauteurGraphe = R.bottom-R.top-2*INTERECH_ACCOGRAP;
	OrigineY      = R.bottom-R.top-INTERECH_ACCOGRAP;

	m_GrilleFilm4.GetWindowRect(&R);
	HautLigneTitre=(R.bottom-R.top)/2;

	m_GrilleFilm1.m_LargColone    = m_GrilleFilm2.m_LargColone     = m_GrilleFilm3.m_LargColone     = m_GrilleFilm4.m_LargColone=LargColone;
	m_GrilleFilm1.m_NbColone      = m_GrilleFilm2.m_NbColone       = m_GrilleFilm3.m_NbColone       = m_GrilleFilm4.m_NbColone=NbColone;
	m_GrilleFilm1.m_HauteurGraphe = m_GrilleFilm2.m_HauteurGraphe  = m_GrilleFilm3.m_HauteurGraphe  = m_GrilleFilm4.m_HauteurGraphe=HauteurGraphe;
	m_GrilleFilm1.m_OrigineY      = m_GrilleFilm2.m_OrigineY       = m_GrilleFilm3.m_OrigineY       = m_GrilleFilm4.m_OrigineY=OrigineY;
	m_GrilleFilm1.m_HautLigneTitre= m_GrilleFilm2.m_HautLigneTitre = m_GrilleFilm3.m_HautLigneTitre = m_GrilleFilm4.m_HautLigneTitre=HautLigneTitre;

	// les ascenceurs
	
	//m_Grille1.m_hAscH=m_Grille2.m_hAscH=m_Grille3.m_hAscH=m_Grille4.m_hAscH=m_Scroll.m_hWnd;
	InitAscenceur();
}

void CDlgGrapheFilm::InitAscenceur()
{
	m_GrilleFilm1.m_PosAscH = m_GrilleFilm2.m_PosAscH = m_GrilleFilm3.m_PosAscH = m_GrilleFilm4.m_PosAscH=0;

	int maxi=max(0,m_pCampagneFilm->m_NbSemaineCalcul  - m_GrilleFilm1.m_NbColone);
	m_Scroll.SetScrollPos(m_GrilleFilm1.m_PosAscH);
	m_Scroll.SetScrollRange(0,maxi);
}

void CDlgGrapheFilm::MajAffichageACCOGRAP()
{
	m_GrilleFilm1.CalculEchelleACCOGRAP();
	m_GrilleFilm2.CalculEchelleACCOGRAP();
	Invalidate(FALSE);
	UpdateWindow();
}

void CDlgGrapheFilm::OnPaint() 
{
	// Device context
	CPaintDC dc(this); 
	
	// Couleur de fond
	CBrush fond(RGB_BleuJFC);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

void CDlgGrapheFilm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CCampagneGenerique * C=m_pCampagneFilm;
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
		if((m_GrilleFilm1.m_PosAscH=::GetScrollPos(h,SB_CTL))==i)return;
		m_GrilleFilm2.m_PosAscH=m_GrilleFilm3.m_PosAscH=m_GrilleFilm4.m_PosAscH=m_GrilleFilm1.m_PosAscH;
		m_GrilleFilm2.Invalidate();
		m_GrilleFilm4.Invalidate();
		m_GrilleFilm2.UpdateWindow();
		m_GrilleFilm4.UpdateWindow();
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}	
}
