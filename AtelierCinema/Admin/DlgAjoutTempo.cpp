// DlgAjoutTempo.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutTempo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutTempo dialog


CDlgAjoutTempo::CDlgAjoutTempo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutTempo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutTempo)
	//}}AFX_DATA_INIT
}


void CDlgAjoutTempo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutTempo)
	DDX_Control(pDX, IDC_PASSAGE92, m_passage92);
	DDX_Control(pDX, IDC_PASSAGE91, m_passage91);
	DDX_Control(pDX, IDC_PASSAGE82, m_passage82);
	DDX_Control(pDX, IDC_PASSAGE81, m_passage81);
	DDX_Control(pDX, IDC_PASSAGE72, m_passage72);
	DDX_Control(pDX, IDC_PASSAGE71, m_passage71);
	DDX_Control(pDX, IDC_PASSAGE62, m_passage62);
	DDX_Control(pDX, IDC_PASSAGE61, m_passage61);
	DDX_Control(pDX, IDC_PASSAGE52, m_passage52);
	DDX_Control(pDX, IDC_PASSAGE51, m_passage51);
	DDX_Control(pDX, IDC_PASSAGE42, m_passage42);
	DDX_Control(pDX, IDC_PASSAGE41, m_passage41);
	DDX_Control(pDX, IDC_PASSAGE32, m_passage32);
	DDX_Control(pDX, IDC_PASSAGE31, m_passage31);
	DDX_Control(pDX, IDC_PASSAGE22, m_passage22);
	DDX_Control(pDX, IDC_PASSAGE21, m_passage21);
	DDX_Control(pDX, IDC_PASSAGE12, m_passage12);
	DDX_Control(pDX, IDC_PASSAGE11, m_passage11);
	DDX_Control(pDX, IDC_PASSAGE102, m_passage102);
	DDX_Control(pDX, IDC_PASSAGE101, m_passage101);
	DDX_Control(pDX, IDC_TEMPO, m_libtempo);
	DDX_Control(pDX, IDC_COMBOPERIODE, m_periode);
	DDX_Control(pDX, IDC_CADRE, m_cadre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutTempo, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutTempo)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_CBN_SELCHANGE(IDC_COMBOPERIODE, OnSelchangeComboperiode)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutTempo message handlers

void CDlgAjoutTempo::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

// ajout du nouveau tempo
void CDlgAjoutTempo::OnOk() 
{
	UpdateData(1);

	CString libelle;
	m_libtempo.GetWindowText(libelle);
	// controle de la saisie
	if(libelle=="")
	{
		MessageBox("                  Saisie incorrecte\nVeuillez préciser un libellé pour le tempo");
		return;

	}
	// le libellé est il déjà utilisé
	if(theApp.base.GetTableTempo()->ExisteLibelleTempo(libelle))
	{
		CString message;
		message.Format("Le libellé %s existe : Veuillez choisir un autre libellé",libelle);
		MessageBox(message);
		return;

	}
	// y a t il des passages pour le tempo?
	if(!IsPassage())
	{
		MessageBox("                Tempo non défini\nVeuillez sélectionner le motif du tempo");
		return;
	
	}
	// ajout du tempo
	else
	{

		tagTempo tempo;
		// chaine de définition
		CString deftempo;
		// variables de comptage pour l'équilibrage
		int count1=0,count2=0;

		// On boucle sur les semaines de la période
		for(int sem=0;sem<10;sem++)
		{
			if(m_passage[sem][0].GetCheck())
			{
				count1++; 
				if(m_passage[sem][1].GetCheck())
				{
					deftempo+="3";
					count2++;
				}
				else
				{ 
					deftempo +="1";
				}
			}
			else 
			{
				if(m_passage[sem][1].GetCheck())
				{
					count2++;
					if(m_passage[sem][0].GetCheck())
					{
						deftempo+="3";
						count1++;
					}
					else
					{
						deftempo +="2";
					}

				}
				// pas de passage pour cette semaine 
				else
				{
					deftempo +="0";
				}
			}
		}
		// le tempo est il équilibré?
		// oui on l'ajoute
		if(count1==count2)
		{
			tempo.m_Libelle = libelle;
			tempo.m_periode = m_periode.GetCurSel()+1;
			tempo.m_Definition = deftempo;
			m_TableTempo.Add(tempo); 
			m_TableTempo.SetModified(1); 
		}
		// non, on avertit l'utilisateur, il est mauvais.
		else
		{
			MessageBox("                         Tempo invalide:\nLe tempo doit contenir le même nombre de passages\n                   sur les deux demi-parc");
			return;
		}

	}
	EndDialog(IDOK);
	
}

BOOL CDlgAjoutTempo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// libellé limité à dix caractères
	m_libtempo.SetLimitText(10); 

	// ajout des périodes à la combo: max 10 semaines
	CString chaine;

	for(int periode=1;periode<10;periode++)
	{
		chaine.Format("%d",periode);
		m_periode.AddString(chaine); 
	}

	// ajout des boutons de passage au tableau
	CWnd * fenetre = m_passage11.FromHandle(m_passage11);
	m_passage[0][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage12);
	m_passage[0][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage21);
	m_passage[1][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage22);
	m_passage[1][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage31);
	m_passage[2][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage32);
	m_passage[2][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage41);
	m_passage[3][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage42);
	m_passage[3][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage51);
	m_passage[4][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage52);
	m_passage[4][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage61);
	m_passage[5][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage62);
	m_passage[5][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage71);
	m_passage[6][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage72);
	m_passage[6][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage81);
	m_passage[7][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage82);
	m_passage[7][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage91);
	m_passage[8][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage92);
	m_passage[8][1].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage101);
	m_passage[9][0].m_hWnd = fenetre->m_hWnd;
	fenetre = m_passage11.FromHandle(m_passage102);
	m_passage[9][1].m_hWnd = fenetre->m_hWnd;

	// période 1 semaine par défaut
	m_periode.SetCurSel(0);
	OnSelchangeComboperiode();
	UpdateData(0);
	
	return TRUE;  
}

void CDlgAjoutTempo::OnSelchangeComboperiode() 
{

	// on met les passages à 0
	for(int sem=0;sem<10;sem++)
	{
		m_passage[sem][0].SetCheck(0); 		
		m_passage[sem][1].SetCheck(0);

	}
	// on active les semaines utilisées
	for(sem=0;sem<m_periode.GetCurSel()+1;sem++)
	{
		m_passage[sem][0].EnableWindow(1); 		
		m_passage[sem][1].EnableWindow(1); 		

	}
	// on grise les semaines inutilisées
	for(sem=m_periode.GetCurSel()+1;sem<10;sem++)
	{
			m_passage[sem][0].EnableWindow(0); 		
			m_passage[sem][1].EnableWindow(0); 		
	}

}

int CDlgAjoutTempo::IsPassage()
{
	for(int sem=0;sem<10;sem++)
	{
		if((m_passage[sem][0].GetCheck())||(m_passage[sem][1].GetCheck()))
		{
			return(1);
		}
	}
	return(0);
}

void CDlgAjoutTempo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	
	
}
