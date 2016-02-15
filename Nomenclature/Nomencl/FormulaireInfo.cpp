// FormulaireInfo.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormulaireInfo dialog


CFormulaireInfo::CFormulaireInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaireInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormulaireInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireInfo)
	DDX_Control(pDX, IDC_CADRE2, m_Cadre2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireInfo, CDialog)
	//{{AFX_MSG_MAP(CFormulaireInfo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireInfo message handlers


void CFormulaireInfo::OnPaint() 
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
