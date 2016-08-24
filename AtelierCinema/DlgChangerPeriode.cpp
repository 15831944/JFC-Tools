// DlgChangerPeriode.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgChangerPeriode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChangerPeriode dialog


CDlgChangerPeriode::CDlgChangerPeriode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangerPeriode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChangerPeriode)
	m_DateDebut = COleDateTime::GetCurrentTime();
	m_NbSemaine = 0;
	//}}AFX_DATA_INIT
	m_DateDebut = COleDateTime::GetCurrentTime();
	int jour=m_DateDebut.GetDayOfWeek(); // 1-> dimanche
	jour=(jour-4+7)%7;
	COleDateTimeSpan span;
	if(jour)
	{
		// On décide que la prochaine campagne sera dans ~3 semaines
		span.SetDateTimeSpan(21-jour,0,0,0);
		m_DateDebut+=span;
	}
	m_NbSemaine=12;
}


void CDlgChangerPeriode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChangerPeriode)
	DDX_Control(pDX, IDC_CADRE, m_Cadre);
	DDX_DateTimeCtrl(pDX, IDC_DATE_DEBUT, m_DateDebut);
	DDX_Text(pDX, IDC_EDIT_DUREE, m_NbSemaine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChangerPeriode, CDialog)
	//{{AFX_MSG_MAP(CDlgChangerPeriode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DUREE, OnDeltaposSpinDuree)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChangerPeriode message handlers

BOOL CDlgChangerPeriode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgChangerPeriode::OnDeltaposSpinDuree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN*  pNMUpDown= (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_NbSemaine-=pNMUpDown->iDelta;

	if(m_NbSemaine<1)m_NbSemaine=1;

	// if(m_NbSemaine>53)m_NbSemaine=53;
	if (m_NbSemaine > NBSEMAINEMAX)
		m_NbSemaine=NBSEMAINEMAX;

	UpdateData(0);
	
	*pResult = 0;
}

void CDlgChangerPeriode::OnOK() 
{
	// on récupère et on test les valeurs
	UpdateData(1);
	if(m_DateDebut.GetDayOfWeek()!=4)
	{
		MessageBox("Les campagnes commencent toujours un mercredi.");
		return;
	}

	// vérification de la durée
	// if(m_NbSemaine<1 || m_NbSemaine>53)
	if(m_NbSemaine<1 || m_NbSemaine>NBSEMAINEMAX)
	{
		MessageBox("La durée de la campagne est invalide","Message",MB_ICONWARNING | MB_OK);
		return;
	}

	// fin de la boîte
	EndDialog(IDOK);
}


void CDlgChangerPeriode::OnPaint() 
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

}

HBRUSH CDlgChangerPeriode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// couleur style bleu JFC
	//pDC->SetBkColor(RGB_BleuJFC);

	return hbr;


}
