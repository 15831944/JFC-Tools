// DlgSuppBacGroupe.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppBacGroupe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBacGroupe dialog

// L'Application
extern CAdminApp theApp;
extern CString GCheminAccesDonnee;

CDlgSuppBacGroupe::CDlgSuppBacGroupe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppBacGroupe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppBacGroupe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppBacGroupe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppBacGroupe)
	DDX_Control(pDX, IDC_LISTBAC_GROUPE_COMPO, m_ListCompoGroupe);
	DDX_Control(pDX, IDC_LISTBAC_GROUPE, m_ListGroupeBacs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppBacGroupe, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppBacGroupe)
	ON_LBN_SELCHANGE(IDC_LISTBAC_GROUPE, OnSelchangeListbacGroupe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBacGroupe message handlers

void CDlgSuppBacGroupe::OnOK() 
{

	// Recup libellé du groupe
	int index = m_ListGroupeBacs.GetCurSel();

	
	if(index!=LB_ERR)
	{
		// Libellé à supprimer
		CString libgroupe;
		m_ListGroupeBacs.GetText(index, libgroupe);

		// Message de confirmation
		if (AfxMessageBox("Attention, vous allez supprimer le groupe " + libgroupe, MB_OKCANCEL,MB_ICONEXCLAMATION ) == IDOK)
		{
			// Boucle sur tous les groupes de bacs
			for(int grpbac=0; grpbac < m_TableGroupeBac.GetSize(); grpbac++)
			{
				// Récupère le libellé du groupe 
				CString lib= m_TableGroupeBac.GetAt(grpbac).GetLibelle();

				// On supprime si trouvé
				if (lib == libgroupe)
				{
					// Suppression, et on indique que la table est modifiée
					m_TableGroupeBac.RemoveAt(grpbac, 1);
					m_TableGroupeBac.SetModified(1); 

					// Puis on sauve la nouvelle table
					m_TableGroupeBac.SaveTable();

					break;
				}
			}
		}
	}


	CDialog::OnOK();
}

BOOL CDlgSuppBacGroupe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Chargement des groupes de bacs
	m_TableGroupeBac.LoadTable(GCheminAccesDonnee);
	SetListeGrpBacs();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Chargement de la liste des groupes de bacs
bool CDlgSuppBacGroupe::SetListeGrpBacs()
{

	// Init liste groupe
	m_ListGroupeBacs.ResetContent();

	// Boucle sur tous les groupes de bacs
	for(int grpbac=0; grpbac < m_TableGroupeBac.GetSize(); grpbac++)
	{
		// Récupère le libellé du groupe 
		CString libgroupe = m_TableGroupeBac.GetAt(grpbac).GetLibelle();

		// On l'ajoute à la liste
		m_ListGroupeBacs.AddString(libgroupe);
	}

	return true;
}


void CDlgSuppBacGroupe::OnSelchangeListbacGroupe() 
{
	char buf[128];

	// Affichage de la composition du groupe en cours de sélection
	// 
	// Init Liste Composition
	m_ListCompoGroupe.ResetContent();

	// Recup libellé du groupe
	int index = m_ListGroupeBacs.GetCurSel();

	if(index!=LB_ERR)
	{
		// Libellé à supprimer
		CString libgroupe;
		m_ListGroupeBacs.GetText(index, libgroupe);

	
		// Boucle sur tous les groupes de bacs
		for(int grpbac=0; grpbac < m_TableGroupeBac.GetSize(); grpbac++)
		{
			// Récupère le libellé du groupe 
			CString lib= m_TableGroupeBac.GetAt(grpbac).GetLibelle();

			// On supprime si trouvé
			if (lib == libgroupe)
			{
				// Récupére le tableau des elmts bacs
				CArray<long, long> CompoBacs;

				m_TableGroupeBac.GetAt(grpbac).GetCompoBacs(CompoBacs);
				

				// Boucle et ajoute dans la liste compo
				for (int i=0; i < CompoBacs.GetSize(); i++)
				{
					// Code bac
					long CodeBac = CompoBacs.GetAt(i);

					// Code pure bac  (sans l'extension composite)
					CString CodeStr;
					sprintf(buf,"%6d", CodeBac);
					CodeStr = buf;
					CodeBac = CodeBac % 100000;
					
					
					// Créer le code bac pure (type Bxxxxx)
					sprintf(buf,"%s%05d","B",CodeBac);

					// Récupère libellé bac 
					CString libellebac = theApp.base.FindLibelleFromCode(buf);

					// Tient compte du code composite (1/ Normal 2/"A" 3/ "M" 4/"N")
					if (CodeStr.Left(1) == "1")
						libellebac = libellebac;
					else if (CodeStr.Left(1) == "2")
						libellebac = libellebac + " (A)";
					else if (CodeStr.Left(1) == "3")
						libellebac = libellebac + " (M)";
					else if (CodeStr.Left(1) == "4")
						libellebac = libellebac + " (N)";

					// On l'ajoute
					m_ListCompoGroupe.AddString(libellebac);
				}

				break;
			}
		}
	}
}

