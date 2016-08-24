// DlgSuppCpmlxGroupe.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppCpmlxGroupe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCpmlxGroupe dialog

// L'Application
extern CAdminApp theApp;
extern CString GCheminAccesDonnee;

CDlgSuppCpmlxGroupe::CDlgSuppCpmlxGroupe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppCpmlxGroupe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppCpmlxGroupe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppCpmlxGroupe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppCpmlxGroupe)
	DDX_Control(pDX, IDC_LISTCMPLX_GROUPE_COMPO, m_ListGroupeCompo);
	DDX_Control(pDX, IDC_LISTCOMPLEXE_GROUPE, m_ListGroupeComplexes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppCpmlxGroupe, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppCpmlxGroupe)
	ON_WM_CANCELMODE()
	ON_LBN_SELCHANGE(IDC_LISTCOMPLEXE_GROUPE, OnSelchangeListcomplexeGroupe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCpmlxGroupe message handlers

void CDlgSuppCpmlxGroupe::OnOK() 
{
	// Recup libellé du groupe
	int index = m_ListGroupeComplexes.GetCurSel();

	
	if(index!=LB_ERR)
	{
		// Libellé à supprimer
		CString libgroupe;
		m_ListGroupeComplexes.GetText(index, libgroupe);

		// Message de confirmation
		if (AfxMessageBox("Attention, vous allez supprimer le groupe " + libgroupe, MB_OKCANCEL,MB_ICONEXCLAMATION ) == IDOK)
		{
			// Boucle sur tous les groupes de complexes
			for(int grpcmplx=0; grpcmplx < m_TableGroupeComplexe.GetSize(); grpcmplx++)
			{
				// Récupère le libellé du groupe 
				CString lib= m_TableGroupeComplexe.GetAt(grpcmplx).GetLibelle();

				// On supprime si trouvé
				if (lib == libgroupe)
				{
					// Suppression, et on indique que la table est modifiée
					m_TableGroupeComplexe.RemoveAt(grpcmplx, 1);
					m_TableGroupeComplexe.SetModified(1); 

					// Puis on sauve la nouvelle table
					m_TableGroupeComplexe.SaveTable();

					break;
				}
			}
		}
	}

	
	CDialog::OnOK();
}

BOOL CDlgSuppCpmlxGroupe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Chargement des groupes de complexes
	m_TableGroupeComplexe.LoadTable(GCheminAccesDonnee);
	SetListeGrpComplexes();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// Chargement de la liste des groupes de complexes
bool CDlgSuppCpmlxGroupe::SetListeGrpComplexes()
{

	// Init liste groupe
	m_ListGroupeComplexes.ResetContent();

	// Boucle sur tous les groupes de bacs
	for(int grpcmplx=0; grpcmplx < m_TableGroupeComplexe.GetSize(); grpcmplx++)
	{
		// Récupère le libellé du groupe 
		CString libgroupe = m_TableGroupeComplexe.GetAt(grpcmplx).GetLibelle();

		// On l'ajoute à la liste
		m_ListGroupeComplexes.AddString(libgroupe);
	}

	return true;
}


void CDlgSuppCpmlxGroupe::OnSelchangeListcomplexeGroupe() 
{
	char buf[128];

	// Affichage de la composition du groupe en cours de sélection
	// 
	// Init Liste Composition
	m_ListGroupeCompo.ResetContent();

	// Recup libellé du groupe
	int index = m_ListGroupeComplexes.GetCurSel();

	if(index!=LB_ERR)
	{
		// Libellé à supprimer
		CString libgroupe;
		m_ListGroupeComplexes.GetText(index, libgroupe);

		// Boucle sur tous les groupes de complexes
		for(int grpcmplx=0; grpcmplx < m_TableGroupeComplexe.GetSize(); grpcmplx++)
		{
			// Récupère le libellé du groupe 
			CString lib= m_TableGroupeComplexe.GetAt(grpcmplx).GetLibelle();

			// On supprime si trouvé
			if (lib == libgroupe)
			{
				// Récupére le tableau des elmts complexes
				CArray<int,int> CompoComplexes;
				m_TableGroupeComplexe.GetAt(grpcmplx).GetCompoComplexes(CompoComplexes);
				
				// Boucle et ajoute dans la liste compo
				for (int i=0; i < CompoComplexes.GetSize(); i++)
				{
					// Code complexe
					int CodeCmplx = CompoComplexes.GetAt(i);
					sprintf(buf,"%s%05d","C",CodeCmplx);

					// Récupère libellé complexe 
					CString libellecomplexe = theApp.base.FindLibelleFromCode(buf);

					// On l'ajoute
					m_ListGroupeCompo.AddString(libellecomplexe);
				}

				break;
			}
		}
	}
	
}
