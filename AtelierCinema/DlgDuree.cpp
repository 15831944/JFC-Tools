// DlgDuree.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog


CDlgFormat::CDlgFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormat)
	DDX_Control(pDX, IDC_CADRE, m_Duree);
	DDX_Control(pDX, IDC_LIST2, m_LB2);
	DDX_Control(pDX, IDC_LIST1, m_LB1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormat, CDialog)
	//{{AFX_MSG_MAP(CDlgFormat)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat message handlers

void CDlgFormat::OnOK() 
{
	m_ListFormat.RemoveAll();

	for(int i=0;i<m_LB1.GetCount();i++)
	{
		CString txt;
		m_LB1.GetText(i,txt);
		m_ListFormat.Add(txt);
	}

	int nb=m_ListFormat.GetSize();
	if(nb==0)
	{
		MessageBox("La sélection est invalide.");
		return;
	}
	if(nb>5)
	{
		CString txt;
		txt.Format("Pas plus de %d formats !",5);
		MessageBox(txt);
		return;
	}
	
	CDialog::OnOK();
}

void CDlgFormat::OnSelchangeList1() 
{
	
	m_LB1.DeleteString(m_LB1.GetCurSel());
}


void CDlgFormat::OnSelchangeList2() 
{
	CString txt;
	m_LB2.GetText(m_LB2.GetCurSel(),txt);
	m_LB1.AddString(txt);
}

void CDlgFormat::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond; //(RGB(23,114,121));
	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_PATTERN4);
	fond.CreatePatternBrush(&Bitmap);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTableFormat * tabFormat = theApp.m_TableCinema.GetTableFormat();
	for(int i=0;i<tabFormat->GetSize();i++)
	{
		m_LB2.AddString(tabFormat->GetLibelle(i));
	}

	for(i=0;i<m_ListFormat.GetSize();i++)
	{
		m_LB1.AddString(m_ListFormat[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
