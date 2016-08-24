// DlgPerformances.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgPerformances.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances dialog


CDlgPerformances::CDlgPerformances(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPerformances::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPerformances)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCampagne=NULL;
}


void CDlgPerformances::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPerformances)
	DDX_Control(pDX, IDC_COULHYPVALIDE, m_CoulHypValide);
	DDX_Control(pDX, IDC_COULHYPACTIVE, m_CoulHypActive);
	DDX_Control(pDX, IDC_OB, m_Grille);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPerformances, CDialog)
	//{{AFX_MSG_MAP(CDlgPerformances)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances message handlers

// ****************************************
// ****************************************
// ****************************************
// *****   PERFORMANCE   ******************
// ****************************************
// ****************************************
// ****************************************


/*
void DlgPerfInit(HWND hDlg,UINT wParam,DWORD lParam){
	CalculBasePopulation(FenetreActive);
	CalculToutBudget(FenetreActive);
	CalculToutMemo(FenetreActive);

}
*/

void CDlgPerformances::MajAffichagePerf(void){
	if(!IsWindowVisible())return;
	Invalidate(FALSE);
	UpdateWindow();

	m_CoulHypValide.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypValide.Invalidate();
	m_CoulHypActive.m_Numero=m_pCampagne->m_HypotheseActive;
	m_CoulHypActive.Invalidate();

	GetDlgItem(IDC_CIBLE1)->SetWindowText(m_pCampagne->m_LibCible[0]);
}
void CDlgPerformances::MajCouleur(void)
{
	m_CoulHypValide.m_Numero=m_pCampagne->m_HypotheseValide;
	m_CoulHypActive.m_Numero=m_pCampagne->m_HypotheseActive;
	GetDlgItem(IDC_CIBLE1)->SetWindowText(m_pCampagne->m_LibCible[0]);
}
/*
void DlgPerfCoulHypValideDraw(HWND hwnd,WPARAM wParam,LPARAM lParam){
	LPDRAWITEMSTRUCT lpdis;
	lpdis=(DRAWITEMSTRUCT FAR *) lParam;
	FillRect(lpdis->hDC,&lpdis->rcItem,hBrush[CLR_HYPOTHESE1+FenetreActive->HypValide]);
}

void DlgPerfFondDuFondDraw(HWND hwnd,WPARAM wParam,LPARAM lParam){
	LPDRAWITEMSTRUCT lpdis;
	lpdis=(DRAWITEMSTRUCT FAR *) lParam;
	FillRect(lpdis->hDC,&lpdis->rcItem,hBrush[CLR_GRISC]);
}

void DlgPerfCoulHypActiveDraw(HWND hwnd,WPARAM wParam,LPARAM lParam){
	LPDRAWITEMSTRUCT lpdis;
	lpdis=(DRAWITEMSTRUCT FAR *) lParam;
	FillRect(lpdis->hDC,&lpdis->rcItem,hBrush[CLR_HYPOTHESE1+FenetreActive->HypActive]);
}
*/

void CDlgPerformances::Initialise(CCampagne * pCampagne)
{
	m_pCampagne=pCampagne;
	m_Grille.m_pCampagne=pCampagne;

	m_Cadre.SetFond(RGB(128,128,128));
}

void CDlgPerformances::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	/*
	CRect rect;
	GetClientRect(&rect);
	FillRect(dc.m_hDC,rect,theApp.hBrush[CLR_FONDDLGPERF]);
	*/

	CBrush fond(RGB_BleuJFC) ; //RGB(23,114,121));
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);


}
