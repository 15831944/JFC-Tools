// DlgCritere.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgCritere.h"

extern CCinemaApp theApp; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCritere dialog


CDlgCritere::CDlgCritere(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCritere::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCritere)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCritere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCritere)
	DDX_Control(pDX, IDC_LISTCRITERE, m_ListCritere);
	DDX_Control(pDX, IDC_CRITEREBASE, m_CritereBase);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCritere, CDialog)
	//{{AFX_MSG_MAP(CDlgCritere)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LISTCRITERE, OnSelchangeListcritere)
	ON_LBN_DBLCLK(IDC_LISTCRITERE, OnDblclkListcritere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCritere message handlers

void CDlgCritere::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Choix de la couleur de fond
	CBrush fond(RGB_BleuJFC); 

	// Remplissage arrière-plan
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

BOOL CDlgCritere::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Par défaut on prend le 1er critère
	m_NoCritere = 0;
	m_CritereBase.SetWindowText(theApp.m_Cible.m_LibCritere[m_NoCritere]); 
	
	// Nb modalité
	m_NbModalite = theApp.m_Cible.m_NbClasse[m_NoCritere];
	
	// Initialisation de la liste critère
	if (ChargeCritere() == false) return FALSE;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des critères disponibles
//
bool CDlgCritere::ChargeCritere()
{

	// Init Liste Critère
	m_ListCritere.ResetContent();

	if (theApp.m_Cible.m_NbCritere > 0)
	{
		// Charge tous les critères existants
		for (int i = 0;i< theApp.m_Cible.m_NbCritere; i++)
		{
			CString txt;
			txt.Format("%s", theApp.m_Cible.m_LibCritere[i]);
			m_ListCritere.AddString(txt); 
		}
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Retour Analyse Campagne Film / renvoi critère de base
//
void CDlgCritere::OnOK() 
{
	
	// MODIF ALAIN 2003
	CString NomCritere;
	m_CritereBase.GetWindowText(NomCritere);

	// Test si sélection critère de base valide
	if (m_ListCritere.FindString( 0,NomCritere) != LB_ERR)
	{

			// Récupère critère de base sélectionné et nb modalités
			CString NomCritCur;

			for (int InxCrit = 0; InxCrit <m_ListCritere.GetCount();InxCrit++)
			{				
				m_ListCritere.GetText(InxCrit,NomCritCur);
				if (NomCritCur == NomCritere)
				{
					m_NoCritere  = InxCrit; 
					break;
				}
			}

			m_NbModalite = theApp.m_Cible.m_NbClasse[m_NoCritere];

			// Sort de la boite sélection
			EndDialog(IDOK);
			return;
	}

	MessageBox("La sélection est invalide.","Erreur :",MB_OK|MB_ICONEXCLAMATION);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Modification sélection critère de base
//
void CDlgCritere::OnSelchangeListcritere() 
{
	// Récupération indice critère
	m_NoCritere  = m_ListCritere.GetCurSel(); 
	m_NbModalite = theApp.m_Cible.m_NbClasse[m_NoCritere];

	// Réaffiche le critère de base courant
	m_CritereBase.SetWindowText(theApp.m_Cible.m_LibCritere[m_NoCritere]);
}

void CDlgCritere::OnDblclkListcritere() 
{
	// MODIF ALAIN 2003
	// passe directement à l'analyse des films
	OnOK();	
}
