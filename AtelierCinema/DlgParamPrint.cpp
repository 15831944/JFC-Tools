// DlgParamPrint.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgParamPrint.h"
#include "campagne.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParamPrint dialog


CDlgParamPrint::CDlgParamPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParamPrint)
	m_NomCampagne = _T("");
	m_NomCentrale = _T("");
	m_NomProduit = _T("");
	m_NomAnnonceur = _T("");
	m_Graphe = FALSE;
	m_Calendrier = FALSE;
	m_Performances = FALSE;
	m_PerfAgglo = FALSE;
	//}}AFX_DATA_INIT

}


void CDlgParamPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParamPrint)
	DDX_Control(pDX, IDC_CADRELIBELLE, m_CadreLibelle);
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Text(pDX, IDC_NOMCAMPAGNE, m_NomCampagne);
	DDX_Text(pDX, IDC_NOMCENTRALE, m_NomCentrale);
	DDX_Text(pDX, IDC_NOMPRODUIT, m_NomProduit);
	DDX_Text(pDX, IDC_NOMANNONCEUR, m_NomAnnonceur);
	DDX_Check(pDX, IDC_GRAPHE, m_Graphe);
	DDX_Check(pDX, IDC_CALENDRIER, m_Calendrier);
	DDX_Check(pDX, IDC_PERFORMANCES, m_Performances);
	DDX_Check(pDX, IDC_PERFAGGLO, m_PerfAgglo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParamPrint, CDialog)
	//{{AFX_MSG_MAP(CDlgParamPrint)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCIBLES, OnCibles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParamPrint message handlers

void CDlgParamPrint::OnOK() 
{
	UpdateData();

	if((!m_Calendrier)&&(!m_Performances)&&(!m_Graphe)&&(!m_PerfAgglo))
	{
		MessageBox("La sélection est invalide.");
		return;
	}
	CDialog::OnOK();
}

void CDlgParamPrint::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	/*
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN7);
	fond.CreatePatternBrush(&Bitmap);
	*/

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgParamPrint::OnCibles() 
{
	// Appel de boite CDlgMulticible pour le choix des cibles à imprimer	
	if(m_DlgMulticible.DoModal()!=IDOK)return;
}

BOOL CDlgParamPrint::OnInitDialog() 
{
	m_Graphe=1;
	m_Calendrier=1;
	m_Performances=1;
	m_PerfAgglo=1;
	CDialog::OnInitDialog();
	return TRUE;  
}
