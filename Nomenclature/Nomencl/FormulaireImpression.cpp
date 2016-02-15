// FormulaireImpression.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireImpression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormulaireImpression dialog


CFormulaireImpression::CFormulaireImpression(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireImpression::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaireImpression)
	m_ChoixAnnonceurMarque = 0;
	m_CheckCampagne = TRUE;
	//}}AFX_DATA_INIT
}


void CFormulaireImpression::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireImpression)
	DDX_Control(pDX, IDC_RADIO1, m_BtnRadioAnnonceurMarque);
	DDX_Control(pDX, IDC_CADRE6, m_Cadre6);
	DDX_Radio(pDX, IDC_RADIO1, m_ChoixAnnonceurMarque);
	DDX_Check(pDX, IDC_CHECK1, m_CheckCampagne);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireImpression, CDialog)
	//{{AFX_MSG_MAP(CFormulaireImpression)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireImpression message handlers

BOOL CFormulaireImpression::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormulaireImpression::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;

	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_OrangeFormulaire);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	
	*/
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
}

void CFormulaireImpression::OnOK() 
{
	CDialog::OnOK();
	UpdateData(false);		// Actualise les valeurs en fonction de l'état de controle	
}
