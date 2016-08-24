// DlgMulticible.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgMulticible.h"
#include "cinemaDoc.h"
#include "cinemaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCinemaApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgMulticible dialog


CDlgMulticible::CDlgMulticible(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMulticible::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMulticible)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMulticible::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMulticible)
	DDX_Control(pDX, IDC_CIBLE5, m_Cible5);
	DDX_Control(pDX, IDC_CIBLE4, m_Cible4);
	DDX_Control(pDX, IDC_CIBLE3, m_Cible3);
	DDX_Control(pDX, IDC_CIBLE2, m_Cible2);
	DDX_Control(pDX, IDC_CIBLE1, m_Cible1);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMulticible, CDialog)
	//{{AFX_MSG_MAP(CDlgMulticible)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CIBLE1, OnSelchangeCible1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMulticible message handlers

#define LIBELLENOTUSED ("Non utilisée")
BOOL CDlgMulticible::OnInitDialog() 
{
	
	CString LibelleCible;
	CDialog::OnInitDialog();

	m_Cible1.ResetContent();
	m_Cible2.ResetContent();
	m_Cible3.ResetContent();
	m_Cible4.ResetContent();
	m_Cible5.ResetContent();
	
	m_Cible2.AddString(LIBELLENOTUSED);
	m_Cible3.AddString(LIBELLENOTUSED);
	m_Cible4.AddString(LIBELLENOTUSED);
	m_Cible5.AddString(LIBELLENOTUSED);
	m_Cible1.AddString(theApp.m_Cible.m_LibCible[theApp.m_Cible.GetIndexFromNumero(m_NumeroCible[0])]);
	
	// récupération des cibles de l'application
	for(int cible=0;cible<theApp.m_Cible.m_LibCible.GetSize();cible++)
	{
		LibelleCible = theApp.m_Cible.m_LibCible[cible];
		m_Cible2.AddString(LibelleCible);
		m_Cible3.AddString(LibelleCible);
		m_Cible4.AddString(LibelleCible);
		m_Cible5.AddString(LibelleCible);		
	}

	// TODO FRED: restaurer la sélection des cibles
	m_Cible1.SetCurSel(0);
	for(int combo=1;combo<5;combo++)
	{
		int num=0,index=0;
		CComboBox * CB=NULL;
		switch(combo)
		{
			case 0 : CB=&m_Cible1;break;
			case 1 : CB=&m_Cible2;break;
			case 2 : CB=&m_Cible3;break;
			case 3 : CB=&m_Cible4;break;
			case 4 : CB=&m_Cible5;break;
			default:ASSERT(0);
		}

		num = m_NumeroCible[combo];
		if(num!=-1)
		{
			LibelleCible = theApp.m_Cible.m_LibCible[theApp.m_Cible.GetIndexFromNumero(num)];

		}
		else 
		{
			LibelleCible=LIBELLENOTUSED;

		}
		if((index=CB->FindString(-1,LibelleCible))>=0)
		{
			CB->SetCurSel(index);
		}
	}
	UpdateData(0);

	return TRUE;  
}

void CDlgMulticible::OnPaint() 
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

void CDlgMulticible::OnSelchangeCible1() 
{
	UpdateData(0);	
}

void CDlgMulticible::OnOK() 
{
	// note La première cible est INVARIABLE
	for(int cible=0;cible<NBCIBLECALCUL;cible++)
	{
		CString txt;
		// Passage des valeurs dans la campagne
		CComboBox * CB=NULL;
		switch(cible)
		{
			case 0 : CB=&m_Cible1;break;
			case 1 : CB=&m_Cible2;break;
			case 2 : CB=&m_Cible3;break;
			case 3 : CB=&m_Cible4;break;
			case 4 : CB=&m_Cible5;break;

			// default:ASSERT(0);
		}

		// @@@@ Modif Alain
		if (cible <= 4)
		{
			CB->GetWindowText(txt);
			if(txt==LIBELLENOTUSED)
			{
				m_NumeroCible[cible]=-1;
			}
			else 
			{
				m_NumeroCible[cible]=GetNumero(txt); 
			}
		}
	}
	CDialog::OnOK();
}

int CDlgMulticible::GetNumero(CString libelle)
{
	int size=theApp.m_Cible.m_NumCible.GetSize();  
	for(int cible=0;cible<size;cible++)
	{
		if(theApp.m_Cible.m_LibCible[cible]==libelle)
		{
			int no = theApp.m_Cible.m_NumCible[cible]; 
			return (theApp.m_Cible.m_NumCible[cible]);
		}
	}
	return (-1);

}
