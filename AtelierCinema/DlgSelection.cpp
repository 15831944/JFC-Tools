// DlgSelection.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgSelection.h"
#include "DlgChoix.h"
#include "DlgSelComplexe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelection dialog


CDlgSelection::CDlgSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelection)
	//}}AFX_DATA_INIT

}


void CDlgSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelection)
	DDX_Control(pDX, IDC_CADRE2, m_cadre2);
	DDX_Control(pDX, IDC_CADRE, m_cadre);
	DDX_Control(pDX, IDC_LISTSELECT, m_ListSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelection, CDialog)
	//{{AFX_MSG_MAP(CDlgSelection)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_AJOUTPRODUIT, OnAjoutproduit)
	ON_BN_CLICKED(IDC_AJOUTCOMPLX, OnAjoutcomplx)
	ON_BN_CLICKED(IDC_AJOUTBAC, OnAjoutbac)
	ON_BN_CLICKED(IDC_SUPP, OnSupp)
	ON_BN_CLICKED(IDC_DESCENDRE, OnDescendre)
	ON_BN_CLICKED(IDC_MONTER, OnMonter)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelection message handlers

void CDlgSelection::OnOk() 
{

	// la s�lection est invalide
	if(m_Code.GetSize()==0)	
	{
		// On avertit
		CString message;
		message.Format("La s�lection est invalide: Veuillez s�lectionner au moins un complexe,bac ou produit");
		MessageBox(message);
		return;
	}
	EndDialog(IDOK);
	
}

void CDlgSelection::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
	
}

void CDlgSelection::OnAjoutproduit() 
{
	CDlgChoix dlg;
	dlg.SetType(0); 
	// On donne � la boite que les libell�s qui ne font pas partie de la s�lection courante 
	dlg.SetLibelleAjoute(m_Libelle); 
	if(dlg.DoModal()!=IDOK)return;

	// ajout des libell�s s�lectionn�s
	m_Libelle.Append(dlg.GetLibelleAjoute()); 
	UpdateListFromLibelle();
	
}

void CDlgSelection::OnAjoutcomplx() 
{
	CDlgSelComplexe dlg;	//Appelle de la ListBox
	dlg.SetLibelleAjoute(m_Libelle);
	if(dlg.DoModal()!=IDOK)return;
	
	// ajout des libell�s s�lectionn�s
	m_Libelle.Append(dlg.GetLibelleAjoute());
	UpdateListFromLibelle();

}

void CDlgSelection::OnAjoutbac() 
{
	CDlgChoix dlg;

	dlg.SetType(1); 
	dlg.SetLibelleAjoute(m_Libelle); 
	if(dlg.DoModal()!=IDOK)
		return;

	// ajout des libell�s s�lectionn�s
	m_Libelle.Append(dlg.GetLibelleAjoute()); 

	UpdateListFromLibelle();
}

// suppression de la s�lection (s�lection multiple)
void CDlgSelection::OnSupp() 
{
	// r�cup�ration des �l�ments d'une s�lection multiple
	int selcount = m_ListSelect.GetSelCount(); 
	int * indexList = new int[selcount];
	
	m_ListSelect.GetSelItems(selcount,indexList);
	// suppression des chaines de la liste
	for(int idxlist=selcount-1;idxlist>=0;idxlist--)
	{
		m_ListSelect.DeleteString(indexList[idxlist]);
		
	}
	UpdateLibelleFromList();
	delete indexList;		
}

// la s�lection est d�plac�e d'une positon vers le bas
void CDlgSelection::OnDescendre() 
{
	// r�cup�ration des �l�ments d'une s�lection multiple
	int selcount = m_ListSelect.GetSelCount(); 
	int * indexList = new int[selcount];
	
	m_ListSelect.GetSelItems(selcount,indexList);

	// On controle que la s�lection ne forme pas un bloc ind�placable vers le bas
	int idxdeplacement=selcount-1;
	if(indexList[selcount-1]==m_ListSelect.GetCount()-1)
	{
		for(int idxsel=selcount-1;idxsel>=0;idxsel--)
		{
			if(idxsel==0)
			{
				idxdeplacement--;
				break;
			}
			if(indexList[idxsel-1]==indexList[idxsel]-1)idxdeplacement--;
			else 
			{
				idxdeplacement--;
				break;
			}
		}
	}

	// suppression des chaines de la liste
	for(int idxlist=idxdeplacement;idxlist>=0;idxlist--)
	{
		if(indexList[idxlist]==m_ListSelect.GetCount()-1)continue;
		// On r�cup�re la chaine � descendre
		CString ChaineDeplace , ChaineBas;
		m_ListSelect.GetText(indexList[idxlist],ChaineDeplace);
		// On d�cale la chaine suivante vers le haut
		m_ListSelect.GetText(indexList[idxlist]+1,ChaineBas);
		m_ListSelect.DeleteString(indexList[idxlist]); 
		m_ListSelect.InsertString(indexList[idxlist],ChaineBas);
		m_ListSelect.DeleteString(indexList[idxlist]+1); 
		m_ListSelect.InsertString(indexList[idxlist]+1,ChaineDeplace);

	}
	UpdateLibelleFromList();

	// On restitue la s�lection sur les m�mes libell�s
	for(idxlist=0;idxlist<=idxdeplacement;idxlist++)
	{
		// Pour une liste box multi-s�lection
		//m_ListSelect.SetSel(indexList[idxlist]+1,TRUE);  

		// Pour une liste box multi-s�lection �tendue ownner draw
		// On manipule directement la donn�e associ�e � l'ITEM.
		m_ListSelect.SetItemData(indexList[idxlist]+1,1); 

	}
	delete indexList;		
}

// la s�lection est d�plac�e d'une positon vers le haut
void CDlgSelection::OnMonter() 
{
	// r�cup�ration des �l�ments d'une s�lection multiple
	int selcount = m_ListSelect.GetSelCount(); 
	int * indexList = new int[selcount];
	
	m_ListSelect.GetSelItems(selcount,indexList);
	// On controle que la s�lection ne forme pas un bloc ind�placable vers le haut
	int idxdeplacement=0;
	if(indexList[0]==0)
	{
		for(int idxsel=0;idxsel<selcount;idxsel++)
		{
			if(idxsel==selcount-1)
			{
				idxdeplacement++;
				break;
			}
			if(indexList[idxsel+1]==indexList[idxsel]+1)idxdeplacement++;
			else 
			{
				idxdeplacement++;
				break;
			}
		}
	}
	// suppression des chaines de la liste
	for(int idxlist=idxdeplacement;idxlist<selcount;idxlist++)
	{
		// On r�cup�re la chaine � monter
		CString ChaineDeplace , ChaineHaute;
		m_ListSelect.GetText(indexList[idxlist]-1,ChaineHaute);
		// On d�cale la chaine pr�c�dente vers le bas
		m_ListSelect.GetText(indexList[idxlist],ChaineDeplace);
		m_ListSelect.DeleteString(indexList[idxlist]-1); 
		m_ListSelect.InsertString(indexList[idxlist]-1,ChaineDeplace);
		m_ListSelect.DeleteString(indexList[idxlist]); 
		m_ListSelect.InsertString(indexList[idxlist],ChaineHaute);

	}
	UpdateLibelleFromList();

	// On restitue la s�lection sur les m�mes libell�s
	for(idxlist=idxdeplacement;idxlist<selcount;idxlist++)
	{
		// Pour une liste box multi-s�lection
		//m_ListSelect.SetSel(indexList[idxlist]-1,TRUE);  

		// Pour une liste box multi-s�lection �tendue ownner draw
		// On manipule directement la donn�e associ�e � l'ITEM.
		m_ListSelect.SetItemData(indexList[idxlist]-1,1); 
	}
	delete indexList;	
}

BOOL CDlgSelection::OnInitDialog() 
{
	ASSERT(AfxCheckMemory());

	CDialog::OnInitDialog();
	m_ListSelect.SetTextAlign(DT_CENTER);
	m_ListSelect.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);

	// On r�cup�re les libell�s � partir des codes
	for(int idcode=0;idcode<m_Code.GetSize();idcode++)
	{
		CString libelle;
		libelle = theApp.m_TableCinema.FindLibelleFromCode(m_Code[idcode]);
		m_Libelle.Add(libelle);
	}
	// On ajoute les libell�s � la liste 
	UpdateListFromLibelle();
	UpdateData(0);
	return TRUE;  
}

void CDlgSelection::AddCode(CString code)
{
	m_Code.Add(code); 
}

CString CDlgSelection::GetCode(int position)
{
	CString code =  m_Code[position];
	return code;
}

int CDlgSelection::GetNbCode()
{
	return m_Code.GetSize();

}

// Mise � jour de la liste
void CDlgSelection::UpdateListFromLibelle()
{
	m_ListSelect.ResetContent(); 

	// On convertit les libell�s s�lectionn�s en code
	m_Code.RemoveAll(); 


	for(int idxlib=0;idxlib<m_Libelle.GetSize();idxlib++)
	{
		CString code,libelle;

		// Ceci permet de r�cup�rer seulement le libell� du complexe
		int pos = m_Libelle[idxlib].Find("\t");

		if(pos>=0)
		{
			libelle = m_Libelle[idxlib].Left(pos); 
			// ligne d'un complexe d�j� ajout�
			if((libelle=="C")||(libelle=="B")||(libelle=="P"))libelle = m_Libelle[idxlib].Right(m_Libelle[idxlib].GetLength()-(pos+1)); 
		}
		else 
			libelle = m_Libelle[idxlib];

		code = theApp.m_TableCinema.FindCodeFromLibelle(libelle);
		m_Code.Add(code);

		// Type + libell�
		CString ligne;
		ligne = m_Code[idxlib][0];
		ligne+="\t";
		ligne+= libelle;

		// On ajoute uniquement s'il n'extse pas d�j�
		bool LibExistant = false;
		for (int idx = 0; idx < m_ListSelect.GetCount(); idx++)
		{
			CString LibListe;
			m_ListSelect.GetText(idx, LibListe);
			if (LibListe == ligne)
			{
				LibExistant = true;
				break;
			}
		}

		// Non existant dals la liste en cours, on le rajoute 
		if (!LibExistant)
			m_ListSelect.AddString(ligne);

		// Avant d'office
		// m_ListSelect.AddString(ligne);
	}
}

void CDlgSelection::UpdateLibelleFromList()
{
	m_Libelle.RemoveAll(); 

	// On convertit les libell�s s�lectionn�s en code
	m_Code.RemoveAll(); 

	for(int idxlib=0;idxlib<m_ListSelect.GetCount();idxlib++)
	{
		CString libelle;
		m_ListSelect.GetText(idxlib,libelle);
		m_Libelle.Add(libelle);
		int pos = libelle.Find('\t');
		if(pos>=0)
		{	// Ce libell� est un libell� complexe
			libelle = libelle.Right(libelle.GetLength()-(pos+1)); 
		}
		CString code = theApp.m_TableCinema.FindCodeFromLibelle(libelle);
		m_Code.Add(code);

	}
}

void CDlgSelection::OnPaint() 
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
	
}


void CDlgSelection::AddLibelle(CString libelle)
{
	m_Libelle.Add(libelle);
}
