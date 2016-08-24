// DlgOptionCalculFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgOptionCalculFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculFilm dialog


CDlgOptionCalculFilm::CDlgOptionCalculFilm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionCalculFilm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptionCalculFilm)
	m_Dememorisation = -1;
	//}}AFX_DATA_INIT
}


void CDlgOptionCalculFilm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionCalculFilm)
	DDX_Radio(pDX, IDC_RADIODEMEMORISATION, m_Dememorisation);
	DDX_Control(pDX, IDC_CADRE4, m_Cadre2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionCalculFilm, CDialog)
	//{{AFX_MSG_MAP(CDlgOptionCalculFilm)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculFilm message handlers

void CDlgOptionCalculFilm::OnOK() 
{
	UpdateData();	
	CDialog::OnOK();
}

void CDlgOptionCalculFilm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CBrush fond; //(RGB(23,114,121));
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN4);
	fond.CreatePatternBrush(&Bitmap);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
