// DlgOptionCalculMemo.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgOptionCalculMemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculMemo dialog


CDlgOptionCalculMemo::CDlgOptionCalculMemo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionCalculMemo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionCalculMemo)
	m_DebutCalend = 0;
	m_FinCalend = 0;
	m_NrSemaineX = 0;
	m_Dememorisation = -1;
	m_BaseFilmRegionale = -1;
	//}}AFX_DATA_INIT
}


void CDlgOptionCalculMemo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionCalculMemo)
	DDX_Control(pDX, IDC_CADRE9, m_Cadre9);
	DDX_Text(pDX, IDC_EDIT1, m_DebutCalend);
	DDX_Text(pDX, IDC_EDIT2, m_FinCalend);
	DDX_Text(pDX, IDC_EDIT3, m_NrSemaineX);
	DDX_Radio(pDX, IDC_RADIODEMEMORISATION, m_Dememorisation);
	DDX_Control(pDX, IDC_CADRE, m_Cadre1);
	DDX_Control(pDX, IDC_CADRE7, m_Cadre2);
	DDX_Control(pDX, IDC_CADRE4, m_Cadre3);
	DDX_Radio(pDX, IDC_FILMRADIOBASEN, m_BaseFilmRegionale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionCalculMemo, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionCalculMemo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculMemo message handlers

void CDlgOptionCalculMemo::OnOK() 
{
	UpdateData(1);


	if((m_DebutCalend<1)||(m_DebutCalend>=(m_NbSemaineCampagne+1))||(m_FinCalend<1)||(m_FinCalend>=(m_NbSemaineCampagne+1))||(m_FinCalend-m_DebutCalend<0)||(m_NrSemaineX<1)||(m_NrSemaineX>=(m_NbSemaineCampagne+1)))
	{
		MessageBox("Saisie des semaines actives invalide");
		return;
	}
	else
	{
		CDialog::OnOK();
	}
}

void CDlgOptionCalculMemo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));
	
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

}

BOOL CDlgOptionCalculMemo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
