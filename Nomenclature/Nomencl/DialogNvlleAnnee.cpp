// DialogNvlleAnnee.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "DialogNvlleAnnee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNvlleAnnee dialog


CDialogNvlleAnnee::CDialogNvlleAnnee(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNvlleAnnee::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNvlleAnnee)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogNvlleAnnee::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNvlleAnnee)
	DDX_Control(pDX, IDC_CADRE8, m_Cadre8);
	DDX_Control(pDX, IDC_LIST_ANNEE_POSSIBLE, m_ListAnneePossible);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNvlleAnnee, CDialog)
	//{{AFX_MSG_MAP(CDialogNvlleAnnee)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNvlleAnnee message handlers


void CDialogNvlleAnnee::OnOK() 
{
	int IndexSel;

	// Récupére Année sélectionnée
	IndexSel = m_ListAnneePossible.GetCurSel();
	m_ListAnneePossible.GetText( IndexSel,m_AnneeSelect);		
	if (m_AnneeSelect != "") CDialog::OnOK();
}

BOOL CDialogNvlleAnnee::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Réecriture entete fenetre
	SetWindowText(m_TitreDialogAnnee);
	
	// Remplissage liste des années possibles
	m_ListAnneePossible.ResetContent();

	if (m_TabAnneePossible.GetSize())
	{
		// Ici il s'agit de charger les années possibles à créer ou à dupliquer
		for(int i= 0;i<m_TabAnneePossible.GetSize();i++)
		{
			// Attention dans le cas d'une duplication on élimine l'année courante
			if (m_FlagNvlleAnnee ||
			   (m_FlagNvlleAnnee == false && m_AnneeCur != m_TabAnneePossible[i]))
				m_ListAnneePossible.AddString(m_TabAnneePossible[i]);
		}
		m_ListAnneePossible.SetTopIndex(m_ListAnneePossible.GetCount()-1);
		return TRUE;		
	}		
	
	else return FALSE;

}

void CDialogNvlleAnnee::OnPaint() 
{
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_OrangeFormulaire);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	
	*/
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

}
