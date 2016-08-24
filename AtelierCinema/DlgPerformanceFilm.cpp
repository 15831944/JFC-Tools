// DlgPerformanceFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgPerformanceFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformanceFilm dialog


CDlgPerformanceFilm::CDlgPerformanceFilm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPerformanceFilm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPerformanceFilm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPerformanceFilm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPerformanceFilm)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Control(pDX, IDC_OB, m_GrilleFilm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPerformanceFilm, CDialog)
	//{{AFX_MSG_MAP(CDlgPerformanceFilm)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformanceFilm message handlers

void CDlgPerformanceFilm::Initialise(CCampagneFilm * pCampagneFilm)
{
	// Récupère la campagne
	m_pCampagneFilm = pCampagneFilm;
	m_GrilleFilm.m_pCampagneFilm = pCampagneFilm;

	// Fond cadre
	m_Cadre.SetFond(RGB(128,128,128));
}
void CDlgPerformanceFilm::MajAffichagePerf()
{

	if(!IsWindowVisible())return;
	Invalidate(FALSE);
	UpdateWindow();

	GetDlgItem(IDC_CIBLE1)->SetWindowText(m_pCampagneFilm->m_LibCible[0]);


}

void CDlgPerformanceFilm::OnPaint() 
{
	// Device context
	CPaintDC dc(this); 
	
	// Couleur de fond
	CBrush fond(RGB_BleuJFC);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
