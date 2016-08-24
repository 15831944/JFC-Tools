// DlgFonctionCible.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "DlgFonctionCible.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFonctionCible dialog


CDlgFonctionCible::CDlgFonctionCible(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFonctionCible::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFonctionCible)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFonctionCible::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFonctionCible)
	DDX_Control(pDX, IDC_DECHARGER, m_BtnDecharger);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre);
	DDX_Control(pDX, IDC_LIST1, m_LB);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFonctionCible, CDialog)
	//{{AFX_MSG_MAP(CDlgFonctionCible)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DECHARGER, OnDecharger)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFonctionCible message handlers

BOOL CDlgFonctionCible::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString TxtCibleParDefaut = "";
	
	for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
	{
		if (c==0)
			TxtCibleParDefaut = theApp.m_Cible.m_LibCible[c];

		m_LB.AddString(theApp.m_Cible.m_LibCible[c]);
	}

	
	switch(m_Mode)
	{
		case 0:
		{
			SetWindowText("Charger une cible");

			// prendre au moins 1 cible par défaut
			for(int c=0;c<m_LB.GetCount() ;c++)
			{
				CString Txt;
				m_LB.GetText(c,Txt);
				if (Txt == TxtCibleParDefaut)
				{

					m_LB.SetCurSel(c);
					m_Edit.SetWindowText(Txt);
					break;
				}					
			}				
		}
		break;

		case 4:
			SetWindowText("Charger une cible complémentaire");
			m_BtnDecharger.ShowWindow(SW_NORMAL);
		break;

		case 1:
			SetWindowText("Mémoriser une cible");
		break;

		case 2:
			SetWindowText("Renommer une cible");
			if(m_Nom!="")
			{
				m_LB.SetCurSel(m_LB.FindString(-1,m_Nom));
				m_Edit.SetWindowText(m_Nom);
				m_Edit.SetSel(0,-1);
			}
		break;

		case 3:
			SetWindowText("Supprimer une cible");
			if(m_Nom!="")
			{
				m_LB.SetCurSel(m_LB.FindString(-1,m_Nom));
				m_Edit.SetWindowText(m_Nom);
				m_Edit.SetSel(0,-1);
			}

		break;
		default:
			ASSERT(0);

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFonctionCible::OnSelchangeList1() 
{
	CString txt;
	m_LB.GetText(m_LB.GetCurSel(),txt);
	m_Edit.SetWindowText(txt);
}

void CDlgFonctionCible::OnOK() 
{
	switch(m_Mode)
	{
		case 4:
		case 0:
		{
			m_Edit.GetWindowText(m_Nom);
			for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
			{
				if(theApp.m_Cible.m_LibCible[c]==m_Nom)
				{
					m_NumCible=theApp.m_Cible.m_NumCible[c];
					EndDialog(IDOK);
					return;
				}
			}
			m_NumCible=-1;
			MessageBox("La sélection est invalide.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		break;
		case 1:
		{
			m_Edit.GetWindowText(m_Nom);
			m_Nom.TrimRight();
			if(m_Nom=="")
			{
				MessageBox("Le libellé est invalide.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
				return;
			}
			for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
			{
				if(theApp.m_Cible.m_LibCible[c]==m_Nom)
				{
					MessageBox("Une cible existe déjà sous ce libellé.\nChoisissez un autre libellé.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
		}
		break;
		case 2:
		{
			// Recherche du nom;
			m_Edit.GetWindowText(m_Nom);
			m_Nom.TrimRight();
			if(m_Nom=="")
			{
				MessageBox("Le libellé est invalide.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
				return;
			}
			for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
			{
				if(theApp.m_Cible.m_LibCible[c]==m_Nom)
				{
					MessageBox("Une cible existe déjà sous ce libellé.\nChoisissez un autre libellé.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
					break;
				}
			}
			CString txt;
			int idx=m_LB.GetCurSel();
			if(idx<0)
			{
				MessageBox("Sélectionnez la cible à renomer dans la liste","Erreur :",MB_OK|MB_ICONEXCLAMATION);
				return;
			}
			m_LB.GetText(idx,txt);
			for(c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
			{
				if(theApp.m_Cible.m_LibCible[c]==txt)
				{
					m_NumCible=theApp.m_Cible.m_NumCible[c];
				}
			}
		}
		break;
		case 3:
		{
			m_Edit.GetWindowText(m_Nom);
			for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
			{
				if(theApp.m_Cible.m_LibCible[c]==m_Nom)
				{
					m_NumCible=theApp.m_Cible.m_NumCible[c];
					EndDialog(IDOK);
					return;
				}
			}
			m_NumCible=-1;
			MessageBox("La sélection est invalide.","Erreur :",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		break;
		default:
			ASSERT(0);

	}
	
	CDialog::OnOK();
}

void CDlgFonctionCible::OnDblclkList1() 
{
	if(m_Mode==0 || m_Mode==4)OnOK();	
}

void CDlgFonctionCible::OnPaint() 
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

void CDlgFonctionCible::OnDecharger() 
{
	m_Nom="";
	m_NumCible=-1;
	EndDialog(IDOK);
}
