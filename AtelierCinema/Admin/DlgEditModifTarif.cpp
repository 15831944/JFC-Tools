// DlgEditModifTarif.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgEditModifTarif.h"
#include "ConstCinema.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CConstCinema ConstCinema;

/////////////////////////////////////////////////////////////////////////////
// CDlgEditModifTarif dialog
CDlgEditModifTarif::CDlgEditModifTarif(const TCHAR* pszTitle, long MaxSel, BOOL Sort, CWnd* pParent) : CDialog(CDlgEditModifTarif::IDD, pParent)
{
	// on initialise le flag de modification
	m_FlagTarifModif = false;

	// pas d'elmt courant au d�part
	m_ElmtActif = -1;
	m_CodeElmt = "";

	// pas de tarif en cours de modif
	m_NoTarif = -1;

	//{{AFX_DATA_INIT(CDlgEditModifTarif)
	//}}AFX_DATA_INIT
}



BEGIN_MESSAGE_MAP(CDlgEditModifTarif, CDialog)
	//{{AFX_MSG_MAP(CDlgEditModifTarif)
	ON_LBN_SELCHANGE(IDD_EDITMODIFTARIF, OnSelChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	ON_LBN_SELCHANGE(IDC_LIST_SEMTARIF, OnSelchangeListSemtarif)
	ON_EN_KILLFOCUS(IDC_EDIT_TARIF, OnKillfocusEditTarif)
	ON_CBN_SELCHANGE(IDC_COMBO_ELMT, OnSelchangeComboElmt)
	ON_CBN_SELCHANGE(IDC_COMBOREGIE, OnSelchangeComboregie)
	ON_WM_VKEYTOITEM()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// la fonction pour ajouter les �l�ments

void CDlgEditModifTarif::AddItem(const TCHAR* pszItem)
{
	// on ajoute l'�l�ment
	if (pszItem != NULL) { m_Index.Add(m_Index.GetSize()); m_Items.Add(pszItem); m_SelItems.Add(0); m_SelExtrn.Add(0); }
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler les s�lections



void CDlgEditModifTarif::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditModifTarif)
	DDX_Control(pDX, IDC_OPTIONREGIE, m_OptionRegie);
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_LIST_SEMTARIF, m_ListSemTarif);
	DDX_Control(pDX, IDC_EDITANNEE, m_AnneeTarif);
	DDX_Control(pDX, IDC_FRAMELMT, m_FrameElmt);
	DDX_Control(pDX, IDC_COMBO_ELMT, m_ComboElmt);
	DDX_Control(pDX, IDC_SPINANNEE, m_SpinAnnee);
	DDX_Control(pDX, IDC_EDIT_TARIF, m_SelTarif);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour trier les �l�ments
void CDlgEditModifTarif::SortItems()
{
	/*
	// on r�cup�re le nombre d'�l�ments
	long taille = m_Index.GetSize();

	// on parcourt tous les �l�ments
	for (long i = 0; i < taille; i += 1)
	{
		// on parcourt les �l�ments suivants
		for (long j = i + 1; j < taille; j += 1)
		{
			// on compare les �l�ments
			if (m_Items[m_Index[i]] > m_Items[m_Index[j]]) { long t = m_Index[i]; m_Index[i] = m_Index[j]; m_Index[j] = t; }
		}
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// chargement des �l�ments tarif�s
bool CDlgEditModifTarif:: ChargeElmt()
{
	
	// Vide la liste des �lmts
	m_ComboElmt.Clear();

	switch (m_TypeElmt)
	{
		case (theApp.TypeComplexe) :
		{
			// chargement des libell�s Complexes
			int size = m_TableComplexe.GetSize(); 
			for(int CptElmt=0; CptElmt<size; CptElmt++)
			{	
				CString libelle = m_TableComplexe.GetLibelle(CptElmt); 
				m_ComboElmt.AddString(libelle);

				// Info Index N� Data dans Table Complexe
				int IndexString = m_ComboElmt.FindStringExact(0, libelle);
				int Ok = m_ComboElmt.SetItemData(IndexString,CptElmt);
				
			}
			break;
		}
		case (theApp.TypeBAC) :
		{
			// chargement des libell�s BAC
			int size = m_TableBAC.GetSize(); 
			for(short CptElmt=0; CptElmt<size; CptElmt++)
			{	
				CString libelle = m_TableBAC.GetLibelle(CptElmt); 
				m_ComboElmt.AddString(libelle); 

				// Info Index N� Data dans Table BAC
				int IndexString = m_ComboElmt.FindStringExact(0, libelle);
				int Ok = m_ComboElmt.SetItemData(IndexString,CptElmt);				
			}
			break;
		}

		case (theApp.TypeProduit) :
		{
			// chargement des libell�s Produits
			int size = m_TableProduit.GetSize(); 
			for(short CptElmt=0; CptElmt<size; CptElmt++)
			{	
				CString libelle = m_TableProduit.GetLibelle(CptElmt); 
				m_ComboElmt.AddString(libelle); 

				// Info Index N� Data dans Table Produit
				int IndexString = m_ComboElmt.FindStringExact(0, libelle);
				int Ok = m_ComboElmt.SetItemData(IndexString,CptElmt);					
			}
			break;
		}

		default :
			AfxMessageBox("Chargement �lmts tarif�s impossible !!!!");
			return false;

	}
	
	// Ok chargement des �lmts tarif�s corrects
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// chargement liste des tarifs par semaine de l'�lmt courant
bool CDlgEditModifTarif::ChargeTarifElmt()
{
	CString LibElmt;
	CString CodeRegie;

	// Init boite de saisie tarif
	InitSaisieTarif();

	// Voir si elmt s�lectionn�
	if (m_ElmtActif != -1)
	{

		// Recup tarif elmt
		if		(m_TypeElmt == theApp.TypeComplexe)
		{
			// Elmt Complexe
			LibElmt.Format("%s%s","Init Tarif : ",m_TableComplexe.GetLibelle(m_ElmtActif));
			m_CodeElmt = m_TableComplexe.GetCode(m_ElmtActif); 
			CodeRegie = m_TableComplexe.GetRegie(m_ElmtActif);
			m_ComboRegie.SetWindowText(CodeRegie);
		}
		else if    (m_TypeElmt == theApp.TypeBAC)
		{
			// Info R�gie
			m_ComboRegie.GetLBText(m_RegieCur,CodeRegie);

			// Elmt BAC
			LibElmt.Format("%s%s","Init Tarif : ",m_TableBAC.GetLibelle(m_ElmtActif));
			m_CodeElmt = m_TableBAC.GetCode(m_ElmtActif) + CodeRegie; 
		}	
		else    
		{
			// Elmt Produit
			LibElmt.Format("%s%s","Init Tarif : ",m_TableProduit.GetLibelle(m_ElmtActif));
			m_CodeElmt = m_TableProduit.GetCode(m_ElmtActif); 
			m_ComboRegie.GetLBText(m_RegieCur,CodeRegie);

		}

		// Chargement tarif elmt
		CString StrAnnee;
		m_AnneeTarif.GetWindowText(StrAnnee);
		
		// On boucle sur m_Tarifs
		int NbTarif = theApp.m_Tarifs.GetSize();
		for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
		{
			CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
			if(nomfich.Find(StrAnnee)>=0)
			{
				m_NoTarif = tarif;
				break;
			}
		}

		if (tarif == theApp.m_Tarifs.GetSize())
		{
			// Nouvelle ann�e - On ajoute une nouvelle table de tarif
			CTarifAnnee * TableTarif = new CTarifAnnee;
			TableTarif->m_NomFich    = "Tarif";
			TableTarif->m_NomFich   += StrAnnee;
			TableTarif->m_NomFich   += ".table";
			
			return false;
		}

		// Initialise tarif en cours
		m_SelTarif.SetWindowText(NULL);

		// Recup la table des tarifs
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(m_CodeElmt); 

		// Cas des BACS
		if ((m_TypeElmt == theApp.TypeBAC) && (NoLigne < 0))
		{
			// on essaie avec l'autre r�gie
			// Info R�gie
			if (CodeRegie == "A")
			{
				m_ComboRegie.SetWindowText("M");
				CodeRegie = "M";
				m_CodeElmt = m_TableBAC.GetCode(m_ElmtActif) + CodeRegie; 
				NoLigne = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(m_CodeElmt); 
			}
			else
			{
				m_ComboRegie.SetWindowText("A");
				CodeRegie = "A";
				m_CodeElmt = m_TableBAC.GetCode(m_ElmtActif) + CodeRegie; 
				NoLigne = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(m_CodeElmt); 
			}
		}


		// Initialisation Liste Complexe Multi-Colonnes Semaines/Tarifs
		m_ListSemTarif.Initialise(UNISTATE,SIMPLE,SIMPLE,GetSysColor(COLOR_3DFACE),0xAA0000,INDEPENDANTE,0,NULL,0);	
		m_ListSemTarif.ResetContent();

		if (NoLigne < 0)
		{
			//AfxMessageBox ("El�ment non existant");
			return false;
		}

		// D�finition 3 colonnes
		m_ListSemTarif.InitNbColonne(3);

		// D�finition taille des colonnes
		m_ListSemTarif.InitSizeColonne(1,70);
		m_ListSemTarif.InitSizeColonne(2,131);
		m_ListSemTarif.InitSizeColonne(3,0);

		// Affichage des tarifs semaine
		CString TxtNoSem;
		CString TxtDate;
		CString TxtTarif;
		CString Ligne;

		// Positionnement 1er jour ann�e courante
		//COleDateTime DateCur = m_DateCourante;
		//DateCur.SetDate(m_DateCourante.GetYear(),1,1);

		COleDateTime DateCur;
		DateCur.SetDate(atoi(StrAnnee),1,1);

		// calage sur 1er Lundi
		while (DateCur.GetDayOfWeek() != 2)
			DateCur = DateCur + COleDateTimeSpan(1,0,0,0);

		// Test date
		int jour  = DateCur.GetDay();
		int mois  = DateCur.GetMonth();
		int annee = DateCur.GetYear();

		// affichage N� semaine + date dd-mm-yyyy + Tarif
		for (int NoSem = 1;NoSem <= CConstCinema::m_MaxNbSemaine;NoSem++)
		{
			TxtNoSem.Format("%02d",NoSem);
			TxtDate = DateCur.Format("%d-%m-%Y");

			double TarifElmt = theApp.m_Tarifs.GetAt(tarif)->GetTarif(NoLigne,NoSem-1); 

			TxtTarif.Format("%f",TarifElmt);
			Ligne.Format("%s\t%s\t%s",TxtNoSem,TxtDate,TxtTarif);	
			
			// ajout ligne tarif �lmt
			m_ListSemTarif.AddString(Ligne); 		

			// Passe � la semaine suivante
			DateCur+=7;

			jour  = DateCur.GetDay();
			mois  = DateCur.GetMonth();
			annee = DateCur.GetYear();

			// Stop si ann�e suivante
			//if (DateCur.GetYear() != m_DateCourante.GetYear()) break;
		}
		return true;
	}
	else
	{
		AfxMessageBox("Aucun elmt s�lectionn� !!!!");
		return false;
	}
	
}


// sauve provisoirement les tarifs pour un �lmt (les 53 semaines)
bool CDlgEditModifTarif::SaveTarifElmt()
{
	double * TarifSemElmt = new double[53];
	CString StrTarif;

	// Recup tarif des 53 semaines
	for (int i=0;i<m_ListSemTarif.GetCount();i++)
	{
		m_ListSemTarif.GetText(i,3,StrTarif);
		TarifSemElmt[i] = atof(StrTarif);
	}

	//Modification du tarif s�lectionn� (> affecte tarif � boite  de saisie au dessus)
	theApp.m_Tarifs.GetAt(m_NoTarif)->ModifyTarif(m_CodeElmt,TarifSemElmt);

	return true;

}

// r�cup�re ancien tarif du type d'�l�mnt en question
bool CDlgEditModifTarif::RecupOldTarif()
{
	switch (m_TypeElmt)
	{
		case (theApp.TypeComplexe) :
		{
			break;
		}
		case (theApp.TypeBAC) :
		{
			break;
		}
		case (theApp.TypeProduit) :
		{
			break;
		}
	}
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour le libell�

void CDlgEditModifTarif::UpdateLibelle()
{
	/*
	// on r�cup�re le curseur
	long Cur = m_Selection.GetCurSel();

	// on v�rifie la validit� du curseur
	if (Cur >= 0)
	{
		// on r�cup�re l'�l�ment du curseur
		long Sel = m_Selection.GetItemData(Cur);

		// on met � jour le libell�
		m_Libelle.SetWindowText(m_Items[Sel]);
	}
	else
	{
		// on vide le libell�
		m_Libelle.SetWindowText(NULL);
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// CDlgEditModifTarif message handlers

BOOL CDlgEditModifTarif::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	// Info R�gies
	m_ComboRegie.EnableWindow(FALSE);
	m_OptionRegie.EnableWindow(FALSE);
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");

	// par d�faut on s�lection R�gie A
	m_RegieCur = 0;
	m_ComboRegie.SetCurSel(0);

	// Affichage du type d'�lmt concern� (Complexe ou BAC ou Produit)
	switch (this->m_TypeElmt)
	{
	case (theApp.TypeComplexe) :
		// affichage libell�s Complexes
		this->SetWindowText("Tarification Complexe");
		this->m_FrameElmt.SetWindowText("Liste des Complexes"); 
		break;

	case (theApp.TypeBAC) :
		// affichage libell�s BAC
		this->SetWindowText("Tarification BAC");
		this->m_FrameElmt.SetWindowText("Liste des BACs"); 

		// Info R�gies indispensable
		m_ComboRegie.EnableWindow(TRUE);
		m_OptionRegie.EnableWindow(TRUE);
		break;

	case (theApp.TypeProduit) :
	// affichage libell�s Produits
		this->SetWindowText("Tarification Produit");
		this->m_FrameElmt.SetWindowText("Liste des Produits"); 
		break;
	}

	// Chargement des �l�ments
	this->ChargeElmt();

	// Info Annee Courante
	InitAnneeTarif();
	CString StrAnnee;
	m_AnneeTarif.GetWindowText(StrAnnee);

	// Sauvegarde des tarifs courants (Complexe + BAC + Produit)
	for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
	{
		CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
	
		if(nomfich.Find(StrAnnee)>=0)
		{
			m_NoTarif = tarif;
			break;
		}
	}

	// Previent utilisateur si aucun tarif
	if (m_NoTarif >= theApp.m_Tarifs.GetSize())
		AfxMessageBox ("Attention aucun tarif existant !!!!!",MB_ICONEXCLAMATION);

	// Init boite de saisie tarif
	InitSaisieTarif();

	// Affichage 1er elmt de la liste
	m_ComboElmt.SetCurSel(0);
	AfficheInfoElmt();

	UpdateData(false);

	/* A VOIR !!!!!
	// Appel chargement liste des tarifs semaine �l�ment courant
	if (this->ChargeTarifElmt())
	{

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
	}

	else
	{
		AfxMessageBox("Probl�me chargement des tarifs <CDlgEditModifTarif>");
		return FALSE;
	}
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE

}

int CDlgEditModifTarif::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	
	/*
	// on teste si on est sur la liste
	if (pListBox == &m_Selection)
	{
		// on teste la touche appuy�e
		if (nKey == VK_SPACE)
		{
			// on change la s�lection
			if (m_MaxSel > 1) this->OnSelChange();

			// on stoppe le traitement standard
			return (-2);
		}
		else if (nKey == VK_UP)
		{
			// on r�cup�re le nombre d'�l�ment
			LONG Max = m_Selection.GetCount();

			// on r�cup�re le curseur courant
			LONG Sel = m_Selection.GetCurSel();

			// on teste si on peut monter le curseur
			if (Sel >= 1 && Sel <= Max - 1)
			{
				// on monte le curseur d'un cran
				m_Selection.SetCurSel(Sel - 1);

				// on change le libell� et/ou la s�lection
				if (m_MaxSel > 1) this->UpdateLibelle(); else this->OnSelChange();
			}

			// on stoppe le traitement standard
			return (-2);
		}
		else if (nKey == VK_DOWN)
		{
			// on r�cup�re le nombre d'�l�ment
			LONG Max = m_Selection.GetCount();

			// on r�cup�re le nouveau curseur
			LONG Sel = m_Selection.GetCurSel();

			// on teste si on peut descendre le curseur
			if (Sel >= 0 && Sel <= Max - 2)
			{
				// on descend le curseur d'un cran
				m_Selection.SetCurSel(Sel + 1);

				// on change le libell� et/ou la s�lection
				if (m_MaxSel > 1) this->UpdateLibelle(); else this->OnSelChange();
			}

			// on stoppe le traitement standard
			return (-2);
		}
	}
	*/

	// on appelle le gestionnaire de base
	return (CDialog::OnVKeyToItem(nKey, pListBox, nIndex));
}

void CDlgEditModifTarif::OnSelChange()
{
	// on met � jour le libell�
	this->UpdateLibelle();

}


void CDlgEditModifTarif::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString TxtAnnee;
	int AnneeCur;
	
	// R�cup derni�re ann�e courante
	m_AnneeTarif.GetWindowText(TxtAnnee);
	AnneeCur = atoi(TxtAnnee);

	// Incr�mentation ou d�cr�mentation de l'ann�e courante
	if (pNMUpDown->iDelta < 0)
		AnneeCur +=1; 
	else
		AnneeCur -=1;

	TxtAnnee.Format("%d",AnneeCur);
	m_AnneeTarif.SetWindowText(TxtAnnee);

	// Mise � jour de la table des tarifs (avec complexe courant)
	m_DateCourante.SetDate(AnneeCur,1,1);
	if (ChargeTarifElmt()==false)
	{
		// Ann�e tarifaire non existante
		AfxMessageBox("Probl�me chargement tarif ann�e courante");

		// retour ann�e pr�c�demment charg�e
		if (pNMUpDown->iDelta < 0)
			AnneeCur -=1; 
		else
			AnneeCur +=1;
		TxtAnnee.Format("%d",AnneeCur);
		m_AnneeTarif.SetWindowText(TxtAnnee);
	}
	
	*pResult = 0;
}


void CDlgEditModifTarif::OnSelchangeListSemtarif() 
{
	CString TarifCur;
	int NoLigneTarif;

	// Recup tarif
	NoLigneTarif = m_ListSemTarif.GetCurSel();
	m_ListSemTarif.GetText(NoLigneTarif,3,TarifCur);

	//Modification du tarif s�lectionn� (> affecte tarif � boite  de saisie au dessus)
	m_SelTarif.SetWindowText(TarifCur);
	m_SelTarif.SetFocus();

	// Sauve tarif en cours (en cas d'escape)
	m_LastTarif = TarifCur;
	
}

void CDlgEditModifTarif::OnKillfocusEditTarif() 
{
}

//////////////////////////////////////////////////////////////////////////////////////
// Changement d'�l�ment en cours de modification tarifaire
void CDlgEditModifTarif::OnSelchangeComboElmt() 
{
	AfficheInfoElmt();
}


void CDlgEditModifTarif::OnSelchangeComboregie() 
{

	m_RegieCur = m_ComboRegie.GetCurSel();

	if (ChargeTarifElmt()==false)
	{
		// Pas de tarif pour ce mode R�gie
		AfxMessageBox("Probl�me chargement tarif pour ce mode r�gie");
	}

}


BOOL CDlgEditModifTarif::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDlgEditModifTarif::OnOK() 
{
	/*
	if (m_FlagTarifModif)
	{
		// Demande Sauvegarde de tous les nouveaux tarifs
		if (AfxMessageBox("Voulez-vous vraiment sauvegarder les derni�res modifs tarifs",MB_YESNO)==IDYES)
			theApp.SaveTarifs();
	}		
	*/
	if (m_FlagTarifModif) theApp.SaveTarifs();

	// on apelle le gestionnaire de base
	CDialog::OnOK();
}


void CDlgEditModifTarif::OnCancel() 
{
	// A faire reprendre les anciens tarifs des BACS (avant entr�e dans ce module)
	RecupOldTarif();
	CDialog::OnCancel();
}


void CDlgEditModifTarif::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// KEY_RETURN
	if (nChar == 13) return;

	// KEY_ESCAPE
	if (nChar == 27) return;
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDlgEditModifTarif::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

/////////////////////////////////////////////////////////////////////////////////////
// D�rivation fonctionalit� de certaines touches clavier
BOOL CDlgEditModifTarif::PreTranslateMessage(MSG* pMsg) 
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		// si Escape et boite �dit Tarif, on revient � la valeur initiale
		if (pMsg->wParam == VK_ESCAPE)
		{
			if (CWnd::GetFocus() == &m_SelTarif)
			{
				// remet valeur tarif initiale dans boite �dition tarif
				m_SelTarif.SetWindowText(m_LastTarif);
			}

			// on effectue le traitement standard WINDOWS
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return(TRUE);
		}

		// on teste si on est sur une touche RETURN
		else if (pMsg->wParam == VK_RETURN)
		{
			if (CWnd::GetFocus() == &m_SelTarif)
			{
				// mise � jour grille tarifaire	
				if (m_ListSemTarif.GetCurSel() >= 0)
					TransfertNewTarif();
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

// Transfert Tarif en cours dans grille tarifaire
void CDlgEditModifTarif::TransfertNewTarif()
{
	CString TarifCur;
	CString OldTarif;
	int NoLigneTarif;
	
	// R�cup nouveau tarif
	m_SelTarif.GetWindowText(TarifCur);

	// V�rifie si c'est une chaine num�rique
	bool IsNumerique = true;
	for(int c=0;c<TarifCur.GetLength();c++)
	{
		//int AscChar = TarifCur.GetAt(c);
		if(((TarifCur.GetAt(c)<=47)||(TarifCur.GetAt(c)>=58)) && TarifCur.GetAt(c)!=46) IsNumerique = false;
	}

	if (IsNumerique == true)
	{
		// R�affecte nouveau tarif � la ligne s�lectionn�e
		NoLigneTarif = m_ListSemTarif.GetCurSel();

		// voir avec ancien tarif de la liste
		m_ListSemTarif.GetText(NoLigneTarif,3,OldTarif);
		if (atof(TarifCur) != atof(OldTarif))
			m_FlagTarifModif = true;

		// Refabrique la ligne tarif
		CString TxtSem,TxtDate,Ligne;
		m_ListSemTarif.GetText(NoLigneTarif,1,TxtSem);
		m_ListSemTarif.GetText(NoLigneTarif,2,TxtDate);
		Ligne.Format("%s\t%s\t%s",TxtSem,TxtDate,TarifCur);	

		// Modifie la ligne avec le nouveau tarif saisi
		m_ListSemTarif.DeleteString(NoLigneTarif); 
		m_ListSemTarif.InsertString(NoLigneTarif,Ligne);
		m_ListSemTarif.SetCurSel(NoLigneTarif);

		// Modifie tarif dans la table tarif / on passe tous les tarifs
		SaveTarifElmt();

		// Passe au tarif suivant
		int SelCur = m_ListSemTarif.GetCurSel();
		if (SelCur < (m_ListSemTarif.GetCount()-1))
		{
			m_ListSemTarif.SetCurSel(SelCur+1);
			m_ListSemTarif.GetText(SelCur+1,3,TarifCur);

			//Modification du tarif s�lectionn� (> affecte tarif � boite  de saisie au dessus)
			m_SelTarif.SetWindowText(TarifCur);
			m_SelTarif.SetFocus();

			// Sauve tarif en cours (en cas d'escape)
			m_LastTarif = TarifCur;
		}
		
	}
	
	else
	{
		// ici tarif non num�rique, on revient � l'ancien tarif
		AfxMessageBox("Tarif entr� incorrect, resaisissez le....");
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dern�re ann�e tarif�e
void CDlgEditModifTarif::InitAnneeTarif()
{
	// Info Annee Courante
	m_DateCourante = COleDateTime::GetCurrentTime();
	CString TxtAnnee;
	TxtAnnee.Format("%d",m_DateCourante.GetYear());

	// Affichage derniere ann�e ayant un fichier tarif
	int NoTarif = -1;
	for (int i= 0;i<10;i++)
	{
		for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
		{
			CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
			if(nomfich.Find(TxtAnnee)>=0)
			{
				NoTarif = tarif;
				break;
			}
		}
		if (NoTarif != -1) break;

		// passe � l'ann�e d'avant
		TxtAnnee.Format("%d",m_DateCourante.GetYear()-i-1);
	}

	if (NoTarif >= theApp.m_Tarifs.GetSize() || NoTarif == -1)
		// sinon affiche ann�e en cours
		TxtAnnee.Format("%d",m_DateCourante.GetYear());

	m_AnneeTarif.SetWindowText(TxtAnnee);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de saisie tarif
void CDlgEditModifTarif::InitSaisieTarif()
{
	m_LastTarif = "";
	m_SelTarif.SetWindowText(m_LastTarif);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Affiche Info Tarifs et autres
void CDlgEditModifTarif::AfficheInfoElmt()
{
	int Cursel		= m_ComboElmt.GetCurSel();
	long ItemData	= m_ComboElmt.GetItemData(Cursel);
	m_ElmtActif		= ItemData;
	if (m_ElmtActif >= 0)
	{
		// Nouvelle s�lection �lmt tarifaire > mise � jour des tarifs
		if (ChargeTarifElmt() == false)	
			AfxMessageBox("Probl�me chargement tarif");
	}
}	

