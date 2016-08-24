// DlgModifCNC.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgModifCNC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCNC dialog


CDlgModifCNC::CDlgModifCNC(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifCNC::IDD, pParent)
{

	// Init dernère valeur CNC saisie
	m_LastCNC = "";

	//{{AFX_DATA_INIT(CDlgModifCNC)
	//}}AFX_DATA_INIT
}


void CDlgModifCNC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModifCNC)
	DDX_Control(pDX, IDC_NEWCNC, m_NewCNC);
	DDX_Control(pDX, IDC_LIST_COMPLEXE, m_ListComplexe);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModifCNC, CDialog)
	//{{AFX_MSG_MAP(CDlgModifCNC)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_COMPLEXE, OnSelchangeListComplexe)
	ON_BN_CLICKED(IDC_A, OnA)
	ON_BN_CLICKED(IDC_B, OnB)
	ON_BN_CLICKED(IDC_C, OnC)
	ON_BN_CLICKED(IDC_D, OnD)
	ON_BN_CLICKED(IDC_E, OnE)
	ON_BN_CLICKED(IDC_F, OnF)
	ON_BN_CLICKED(IDC_G, OnG)
	ON_BN_CLICKED(IDC_H, OnH)
	ON_BN_CLICKED(IDC_I, OnI)
	ON_BN_CLICKED(IDC_J, OnJ)
	ON_BN_CLICKED(IDC_K, OnK)
	ON_BN_CLICKED(IDC_L, OnL)
	ON_BN_CLICKED(IDC_M, OnM)
	ON_BN_CLICKED(IDC_N, OnN)
	ON_BN_CLICKED(IDC_O, OnO)
	ON_BN_CLICKED(IDC_P, OnP)
	ON_BN_CLICKED(IDC_Q, OnQ)
	ON_BN_CLICKED(IDC_R, OnR)
	ON_BN_CLICKED(IDC_S, OnS)
	ON_BN_CLICKED(IDC_T, OnT)
	ON_BN_CLICKED(IDC_U, OnU)
	ON_BN_CLICKED(IDC_V, OnV)
	ON_BN_CLICKED(IDC_W, OnW)
	ON_BN_CLICKED(IDC_X, OnX)
	ON_BN_CLICKED(IDC_Y, OnY)
	ON_BN_CLICKED(IDC_Z, OnZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCNC message handlers

BOOL CDlgModifCNC::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Init Controle Edit CNC
	m_NewCNC.SetWindowText("");

	// Chargement des éléments complexe et de leur CNC respectif
	this->ChargeComplexeCNC('A');
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// Chargement des éléments complexes et de leur CNC
void CDlgModifCNC::ChargeComplexeCNC(char Alpha)
{
	CString LibElmt;
		
	// Init Zone saisie CNC
	m_NewCNC.SetWindowText("");

	// Initialisation Liste Complexe Multi-Colonnes Code / Libellé / CNC
	m_ListComplexe.Initialise(UNISTATE,SIMPLE,SIMPLE,GetSysColor(COLOR_3DFACE),0xAA0000,INDEPENDANTE,0,NULL,0);	
	m_ListComplexe.ResetContent();

	// Définition 3 colonnes
	m_ListComplexe.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListComplexe.InitSizeColonne(1,200);
	m_ListComplexe.InitSizeColonne(2,66);
	m_ListComplexe.InitSizeColonne(3,0);
	
	// Table des complexes
	CTableComplexe TableComplexe = theApp.base.GetNewInstanceTableComplexe();
	int size = TableComplexe.GetSize(); 

	// Table des entrées
	CTableNbEntreeComplexe TableEntree = theApp.base.GetNewInstanceTableNbEntree();

	for(int cmplx=0;cmplx<size;cmplx++)
	{	
	
		// Libelle du complexe
		CString LibCmplx = TableComplexe.GetAt(cmplx).GetLibelle(); 

		// Test 1er caractere
		CString Libelle = LibCmplx;
		Libelle.MakeUpper();
		if (Libelle.Left(1) == Alpha)
		{

			// Code du complexe
			CString CodeCmplx = TableComplexe.GetAt(cmplx).GetCode(); 

			// Selon le code on récupère la donnée CNC correspondant
			int NbElmt = TableEntree.GetSize();
			long NbEntree = 0;

			for (int NoElmt = 0;NoElmt < NbElmt;NoElmt++)
			{
			
				// le code a été trouvé, mise à jour de la table des entrées
				tagNbEntree entree = theApp.base.GetTableNbEntree()->GetAt(NoElmt);

				if (entree.m_CodeComplexe == CodeCmplx)
				{
					NbEntree = entree.m_NbEntree;
					break;
				}
			}
			
			// Code Complexe
			CString CodeComplexe;
			CodeComplexe.Format("%s",CodeCmplx);

			// Libellé
			CString LibComplexe;
			LibComplexe.Format("%s",LibCmplx);

			// Constitue la ligne Code / Libelle / Nb Entree Complexe
			CString Ligne;
			//Ligne.Format("%s\t%s\t%d",CodeComplexe,LibComplexe,NbEntree);	
			Ligne.Format("%s\t%s\t%d",LibComplexe,CodeComplexe,NbEntree);	
			
			// ajout ligne tarif élmt
			m_ListComplexe.AddString(Ligne); 		
		}
	}

	// Si au moins 1 élémt on sélectionne le premier
	if (m_ListComplexe.GetCount() > 0)
	{
		// Sélection du 1er complexe (bidouille pour que ça marche, quelme merde MFC
		m_ListComplexe.SetCurSel(0); 
		
		//m_ListComplexe.ShowWindow(SW_HIDE);
		//m_ListComplexe.ShowWindow(SW_SHOW);

		// Validation de la sélection >> affichage CNC Correspondant
		AfficheCNC();
	}

}	

void CDlgModifCNC::OnPaint() 
{
	// device context for painting
	CPaintDC dc(this);

	CBrush fond(RGB_BleuJFC);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

////////////////////////////////////////////////////////////////////////////////////
// Sélection d'un nouveau complexe pour modification CNC
void CDlgModifCNC::OnSelchangeListComplexe() 
{
	// Validation de la sélection >> affichage CNC Correspondant
	AfficheCNC();
}

/////////////////////////////////////////////////////////////////////////////////////
// Validation de la sélection >> affichage CNC Correspondant
void CDlgModifCNC :: AfficheCNC()
{
	CString CNCCur;
	int NoLigneCNC;

	// Recup CNC
	NoLigneCNC = m_ListComplexe.GetCurSel();
	m_ListComplexe.GetText(NoLigneCNC,3,CNCCur);

	//Modification du CNC sélectionné (> affecte CNC à boite  de saisie au dessus)
	m_NewCNC.SetWindowText(CNCCur);
	m_NewCNC.SetFocus();

	// Sauve CNC en cours (en cas d'escape)
	m_LastCNC = CNCCur;

}

/////////////////////////////////////////////////////////////////////////////////////
// Dérivation fonctionalité de certaines touches clavier
BOOL CDlgModifCNC::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// si Escape et boite édit CNC, on revient à la valeur initiale
		if (pMsg->wParam == VK_ESCAPE)
		{
			if (CWnd::GetFocus() == &m_NewCNC)
			{
				// remet valeur CNC initiale dans boite édition CNC
				m_NewCNC.SetWindowText(m_LastCNC);
			}

			// on effectue le traitement standard WINDOWS
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return(TRUE);
		}

		// on teste si on est sur une touche RETURN
		else if (pMsg->wParam == VK_RETURN)
		{
			if (CWnd::GetFocus() == &m_NewCNC)
			{
				// mise à jour grille CNC
				if (m_ListComplexe.GetCurSel() >= 0)
					TransfertNewCNC();
			}

			// on effectue le traitement standard WINDOWS
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return(TRUE);
		}

	}

	// on appelle le gestionnaire de base
	return (CDialog::PreTranslateMessage(pMsg));

}

//////////////////////////////////////////////////////////////////////////////////////////////
// Transfert CNC en cours dans grille CNC
void CDlgModifCNC::TransfertNewCNC()
{
	CString CNCCur;
	CString OldCNC;
	int NoLigneCNC;
	
	// Récup nouveau CNC
	m_NewCNC.GetWindowText(CNCCur);

	// Vérifie si c'est une chaine numérique
	bool IsNumerique = true;
	for(int c=0;c<CNCCur.GetLength();c++)
	{
		if(((CNCCur.GetAt(c)<=47)||(CNCCur.GetAt(c)>=58)) && CNCCur.GetAt(c)!=46) IsNumerique = false;
	}

	if (IsNumerique == true)
	{
		// Réaffecte nouveau CNC à la ligne sélectionnée
		NoLigneCNC = m_ListComplexe.GetCurSel();

		// voir avec ancien CNC de la liste
		m_ListComplexe.GetText(NoLigneCNC,3,OldCNC);
		
		// Si CNC modifié on le sauvegarde
		if (atof(CNCCur) != atof(OldCNC))
		{
			// Refabrique la ligne Code / Libellé / CNC
			CString CodeCmplx,LibCmplx,Ligne;
			m_ListComplexe.GetText(NoLigneCNC,1,LibCmplx);
			m_ListComplexe.GetText(NoLigneCNC,2,CodeCmplx);
			Ligne.Format("%s\t%s\t%s",LibCmplx,CodeCmplx,CNCCur);	

			// Modifie la ligne avec le nouveau nombre d'entrée CNC saisi
			m_ListComplexe.DeleteString(NoLigneCNC); 
			m_ListComplexe.InsertString(NoLigneCNC,Ligne);
			m_ListComplexe.SetCurSel(NoLigneCNC);

			// Modifie CNC dans la table CNC
			SaveCNC(CodeCmplx,CNCCur);

			// Passe au CNC suivant
			int SelCur = m_ListComplexe.GetCurSel();
			if (SelCur < (m_ListComplexe.GetCount()-1))
			{
				m_ListComplexe.SetCurSel(SelCur+1);
				m_ListComplexe.GetText(SelCur+1,3,CNCCur);

				//Modification du CNC sélectionné (> affecte tarif à boite  de saisie au dessus)
				m_NewCNC.SetWindowText(CNCCur);
				m_NewCNC.SetFocus();

				// Sauve CNC en cours (en cas d'escape)
				m_LastCNC = CNCCur;
			}
		}		
	}
	
	else
	{
		// ici tarif non numérique, on revient à l'ancien tarif
		AfxMessageBox("Tarif entré incorrect, resaisissez le....");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde dernier CNC saisi
void CDlgModifCNC::SaveCNC(CString CodeCmplx,CString NbEntree)
{

	// Recherche de la ligne complexe/entrée à modifier
	for (int NoElmt = 0;NoElmt < theApp.base.GetTableNbEntree()->GetSize();NoElmt++)
	{
	
		// le code a été trouvé, mise à jour de la table des entrées
		if (theApp.base.GetTableNbEntree()->GetAt(NoElmt).m_CodeComplexe == CodeCmplx)
		{
			
			// Modifie la valeur Nb entree
			tagNbEntree entree;
			entree = theApp.base.GetTableNbEntree()->GetAt(NoElmt);
			// ou directement (*theApp.base.GetTableNbEntree())[NoElmt].m_NbEntree = atoi(NbEntree);
			entree.m_NbEntree  = atoi(NbEntree);

			theApp.base.GetTableNbEntree()->SetAt(NoElmt,entree);

			// sauve la nouvelle valeur
			theApp.base.GetTableNbEntree()->SetModified(1); 
			theApp.base.GetTableNbEntree()->SaveTable();

			break;
		}
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////
// AFFICHAGE DES COMPLEXES A MODIFIER PAR LOT ALPHABETIQUE
void CDlgModifCNC::OnA() 
{
	ChargeComplexeCNC('A');
}

void CDlgModifCNC::OnB() 
{
	ChargeComplexeCNC('B');
}

void CDlgModifCNC::OnC() 
{
	ChargeComplexeCNC('C');
}

void CDlgModifCNC::OnD() 
{
	ChargeComplexeCNC('D');
}

void CDlgModifCNC::OnE() 
{
	ChargeComplexeCNC('E');
}

void CDlgModifCNC::OnF() 
{
	ChargeComplexeCNC('F');
}

void CDlgModifCNC::OnG() 
{
	ChargeComplexeCNC('G');
}

void CDlgModifCNC::OnH() 
{
	ChargeComplexeCNC('H');
}

void CDlgModifCNC::OnI() 
{
	ChargeComplexeCNC('I');
}

void CDlgModifCNC::OnJ() 
{
	ChargeComplexeCNC('J');
}

void CDlgModifCNC::OnK() 
{
	ChargeComplexeCNC('K');
}

void CDlgModifCNC::OnL() 
{
	ChargeComplexeCNC('L');
}

void CDlgModifCNC::OnM() 
{
	ChargeComplexeCNC('M');
}

void CDlgModifCNC::OnN() 
{
	ChargeComplexeCNC('N');
}

void CDlgModifCNC::OnO() 
{
	ChargeComplexeCNC('O');
}

void CDlgModifCNC::OnP() 
{
	ChargeComplexeCNC('P');
}

void CDlgModifCNC::OnQ() 
{
	ChargeComplexeCNC('Q');
}

void CDlgModifCNC::OnR() 
{
	ChargeComplexeCNC('R');
}

void CDlgModifCNC::OnS() 
{
	ChargeComplexeCNC('S');
}

void CDlgModifCNC::OnT() 
{
	ChargeComplexeCNC('T');
}

void CDlgModifCNC::OnU() 
{
	ChargeComplexeCNC('U');
}

void CDlgModifCNC::OnV() 
{
	ChargeComplexeCNC('V');
}

void CDlgModifCNC::OnW() 
{
	ChargeComplexeCNC('W');
}

void CDlgModifCNC::OnX() 
{
	ChargeComplexeCNC('X');
}

void CDlgModifCNC::OnY() 
{
	ChargeComplexeCNC('Y');
}

void CDlgModifCNC::OnZ() 
{
	ChargeComplexeCNC('Z');
}
