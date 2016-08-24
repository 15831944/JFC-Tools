// DlgBudgetNet.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgBudgetNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBudgetNet dialog


CDlgBudgetNet::CDlgBudgetNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBudgetNet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBudgetNet)
	m_BudgetNet = 0;
	//}}AFX_DATA_INIT
}


void CDlgBudgetNet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBudgetNet)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_Text(pDX, IDC_EDIT1, m_BudgetNet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBudgetNet, CDialog)
	//{{AFX_MSG_MAP(CDlgBudgetNet)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBudgetNet message handlers

void CDlgBudgetNet::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}

void CDlgBudgetNet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));
	/*
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN4);
	fond.CreatePatternBrush(&Bitmap);
	*/
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}
