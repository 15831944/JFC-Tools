// DlgPerformances2.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgPerformances2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances2 dialog


CDlgPerformances2::CDlgPerformances2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPerformances2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPerformances2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCampagne=NULL;
}

void CDlgPerformances2::MajAffichagePerf(void){
	if(!IsWindowVisible())return;
	m_Grille1.Invalidate();
	m_Grille2.Invalidate();
	m_Grille1.UpdateWindow();
	m_Grille2.UpdateWindow();


	m_CoulHypValide1.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypValide1.Invalidate();
	m_CoulHypActive1.m_Numero=m_pCampagne->m_HypotheseActive;
	m_CoulHypActive1.Invalidate();
	m_CoulHypValide2.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypValide2.Invalidate();
	m_CoulHypActive2.m_Numero=m_pCampagne->m_HypotheseActive;
	m_CoulHypActive2.Invalidate();

	GetDlgItem(IDC_CIBLE1)->SetWindowText(m_pCampagne->m_LibCible[0]);
	GetDlgItem(IDC_CIBLE2)->SetWindowText(m_pCampagne->m_LibCible[1]);
}

void CDlgPerformances2::MajCouleur(void){
	m_CoulHypValide1.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypActive1.m_Numero=m_pCampagne->m_HypotheseActive;
	m_CoulHypValide2.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypActive2.m_Numero=m_pCampagne->m_HypotheseActive;
	GetDlgItem(IDC_CIBLE1)->SetWindowText(m_pCampagne->m_LibCible[0]);
	GetDlgItem(IDC_CIBLE2)->SetWindowText(m_pCampagne->m_LibCible[1]);
}

void CDlgPerformances2::Initialise(CCampagne * pCampagne)
{
	m_pCampagne=pCampagne;
	m_Grille1.m_pCampagne=pCampagne;
	m_Grille2.m_pCampagne=pCampagne;
	m_Grille1.m_NrCible=0;
	m_Grille2.m_NrCible=1;
	m_Cadre.SetFond(RGB(128,128,128));
}

void CDlgPerformances2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPerformances2)
	DDX_Control(pDX, IDC_COULHYPVALIDE2, m_CoulHypValide2);
	DDX_Control(pDX, IDC_COULHYPVALIDE1, m_CoulHypValide1);
	DDX_Control(pDX, IDC_COULHYPACTIVE2, m_CoulHypActive2);
	DDX_Control(pDX, IDC_COULHYPACTIVE1, m_CoulHypActive1);
	DDX_Control(pDX, IDC_OB2, m_Grille2);
	DDX_Control(pDX, IDC_OB, m_Grille1);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPerformances2, CDialog)
	//{{AFX_MSG_MAP(CDlgPerformances2)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances2 message handlers

void CDlgPerformances2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	/*	
	CRect rect;
	GetClientRect(&rect);
	FillRect(dc.m_hDC,rect,theApp.hBrush[CLR_FONDDLGPERF]);
	*/

	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);


}
