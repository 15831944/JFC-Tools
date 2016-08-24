// DlAjoutFormat.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlAjoutFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CDlAjoutFormat dialog


CDlAjoutFormat::CDlAjoutFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlAjoutFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlAjoutFormat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlAjoutFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlAjoutFormat)
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_COEFFFORMAT, m_coeff);
	DDX_Control(pDX, IDC_LIBFORMAT, m_libelle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlAjoutFormat, CDialog)
	//{{AFX_MSG_MAP(CDlAjoutFormat)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlAjoutFormat message handlers

void CDlAjoutFormat::OnOk() 
{
	// Mise à jour des données
	UpdateData(1);
	CString LibelleFormat,CoeffFormat; 
	m_libelle.GetWindowText(LibelleFormat);
	m_coeff.GetWindowText(CoeffFormat);

	int IsLibelleNumerique=1;
	int IsCoeffNumerique=1;

	//le libellé est il numérique?
	for(int c=0;c<LibelleFormat.GetLength();c++)
	{
		if((LibelleFormat.GetAt(c)<=47)||(LibelleFormat.GetAt(c)>=58))IsLibelleNumerique=0;
	}
	//le coeff est il numérique?
	for(c=0;c<CoeffFormat.GetLength();c++)
	{
		if((CoeffFormat.GetAt(c)<=47)||(CoeffFormat.GetAt(c)>=58))IsCoeffNumerique=0;
	}
	// controle de la saisie
	if(LibelleFormat=="")
	{
		MessageBox("          Saisie incorrecte\nVeuillez indiquer une durée");
		return;
	}
	// le libellé est il numérique?
	if(!IsLibelleNumerique)
	{
		MessageBox("Ceci n'est pas une durée");
		return;

	}
	// le coeff aussi
	if(!IsCoeffNumerique)
	{
		MessageBox("Ceci n'est pas un coefficient");
		return;

	}
	// le format existe t-il?
	if(m_TableFormat.FindIndexFromCode(LibelleFormat)>=0)
	{
		MessageBox("Ce format existe");
		return;
	}
	// ajout du format
	else
	{
		tagFormat frm;
		frm.m_Libelle = LibelleFormat;
		frm.m_Coeff = atof(CoeffFormat);
		m_TableFormat.Add(frm);
		m_TableFormat.SetModified(1); 
	}

	// sauvegarde globale
	theApp.SaveTable();

	EndDialog(IDOK);	
}

void CDlAjoutFormat::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
	
}

BOOL CDlAjoutFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// Limitation du libellé du format à 3 caractères
	m_libelle.SetLimitText(3);
	// Coeff du format 3 caractères
	m_coeff.SetLimitText(3);


	return TRUE;  
}

void CDlAjoutFormat::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
