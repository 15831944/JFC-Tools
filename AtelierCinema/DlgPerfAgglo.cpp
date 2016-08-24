// DlgPerfAgglo.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgPerfAgglo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPerfAgglo dialog


CDlgPerfAgglo::CDlgPerfAgglo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPerfAgglo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPerfAgglo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPerfAgglo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPerfAgglo)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Control(pDX, IDC_OB, m_Grille);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPerfAgglo, CDialog)
	//{{AFX_MSG_MAP(CDlgPerfAgglo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPerfAgglo message handlers

void CDlgPerfAgglo::Initialise(CCampagneAgglo * pCampagne)
{
	m_pCampagne=pCampagne;
	m_Grille.m_pCampagne=pCampagne;

//	m_Cadre.SetFond(RGB(128,128,128));
}


void CDlgPerfAgglo::MajAffichagePerf(void)
{
	if(!IsWindowVisible())return;
	Invalidate(FALSE);
	UpdateWindow();
}

void CDlgPerfAgglo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC) ; //RGB(23,114,121));
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}
