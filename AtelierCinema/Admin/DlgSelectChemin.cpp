// DlgSelectChemin.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelectChemin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgSelectChemin dialog


CDlgSelectChemin::CDlgSelectChemin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectChemin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectChemin)
	m_chemin = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSelectChemin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectChemin)
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Text(pDX, IDC_CHEMIN, m_chemin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectChemin, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectChemin)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectChemin message handlers

void CDlgSelectChemin::OnOk() 
{
	UpdateData(1);
	EndDialog(IDOK);
	if (m_chemin!=m_OldChemin)
	{
		m_chemin = m_chemin.Left(m_chemin.GetLength()-1); 
	}
}

void CDlgSelectChemin::OnAnnuler() 
{
	EndDialog(IDCANCEL);
	m_chemin = m_OldChemin;
}


BOOL CDlgSelectChemin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editPath.Initialize(IDC_CHEMIN, this);
	
	return TRUE;  
}

void CDlgSelectChemin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
