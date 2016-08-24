// DlgBeta.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgBeta.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBeta dialog


CDlgBeta::CDlgBeta(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBeta::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBeta)
	m_Beta = 0;
	//}}AFX_DATA_INIT
}


void CDlgBeta::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBeta)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Text(pDX, IDC_EDIT1, m_Beta);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBeta, CDialog)
	//{{AFX_MSG_MAP(CDlgBeta)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBeta message handlers

void CDlgBeta::OnOK() 
{
	UpdateData(1);
	// Controle de la valeur du beta entre 0 et 100
	if((m_Beta<=0)||(m_Beta>101))
	{
		MessageBox("Veuillez saisir une valeur comprise entre 1 et 100 (compris)");
		return;
	}
	CDialog::OnOK();
}

void CDlgBeta::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));
	
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}
