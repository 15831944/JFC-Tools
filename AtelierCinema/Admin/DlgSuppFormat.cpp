// DlgSuppFormat.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppFormat dialog


CDlgSuppFormat::CDlgSuppFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppFormat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppFormat)
	DDX_Control(pDX, IDC_LISTFORMAT, m_ListFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppFormat, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppFormat)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppFormat message handlers

void CDlgSuppFormat::OnOk() 
{
	UpdateData(1);

	// Suppression du format sélectionné
	int index = m_ListFormat.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un format");
	}
	else
	{
		CString libelle;
		m_ListFormat.GetText(index,libelle);
		index = theApp.IndexFormatDuLibelle(libelle);
		m_TableFormat.RemoveAt(index,1); 
		m_TableFormat.SetModified(1); 

	}
	EndDialog(IDOK);	
}

void CDlgSuppFormat::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
}

BOOL CDlgSuppFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// Ajout des libellés de format 	
	int size = m_TableFormat.GetSize(); 
	for(int format=0;format<size;format++)
	{	
		CString libelle = m_TableFormat.GetLibelle(format);
		m_ListFormat.AddString(libelle); 		
	}
	// on trie les libellés par ordre de durée croissante
	//TriParDuree();
	UpdateData(0);
	return TRUE;  
}

void CDlgSuppFormat::TriParDuree()
{
	CString libformat,libformat2;

	for(int format=0;format<m_ListFormat.GetCount();format++)
	{	
		m_ListFormat.GetText(format,libformat);

		// Pour un élément donné, on observe s'il existe une durée + petite dans les formats non triés
		for(int format2=format+1;format2<m_ListFormat.GetCount();format2++)
		{
			m_ListFormat.GetText(format2,libformat2);
			// Si la durée est inférieure: permutation des éléments
			if(atoi(libformat2)<atoi(libformat))
			{
				m_ListFormat.InsertString(format,libformat2); 
				m_ListFormat.InsertString(format2,libformat); 
				break;
			}
		}
	}
}

void CDlgSuppFormat::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
