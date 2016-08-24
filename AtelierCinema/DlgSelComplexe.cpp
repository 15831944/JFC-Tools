// DlgSelComplexe.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgSelComplexe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelComplexe dialog


CDlgSelComplexe::CDlgSelComplexe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelComplexe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelComplexe)
	//}}AFX_DATA_INIT
}


void CDlgSelComplexe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelComplexe)
	DDX_Control(pDX, IDC_CADRE7, m_cadregrp);
	DDX_Control(pDX, IDC_LIST_GRP_CMPLX, m_ListGrpComplexe);
	DDX_Control(pDX, IDC_COMBOREGIE2, m_combo);
	DDX_Control(pDX, IDC_OPTION, m_CadreOption);
	DDX_Control(pDX, IDC_CADRE, m_cadre);
	DDX_Control(pDX, IDC_LIST, m_ListComplexe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelComplexe, CDialog)
	//{{AFX_MSG_MAP(CDlgSelComplexe)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_SELTOUT2, OnSeltout)
	ON_BN_CLICKED(IDC_INVERSER2, OnInverser)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBOREGIE2, OnFiltreRegie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelComplexe message handlers


void CDlgSelComplexe::OnAnnuler() 
{
	EndDialog(IDCANCEL);
	
}

void CDlgSelComplexe::OnOk() 
{
	char buf[128];

	// On récupère la sélection active
	// récupération des éléments d'une sélection multiple
	int selcount = m_ListComplexe.GetSelCount(); 
	int * indexList = new int[selcount];
	
	m_ListComplexe.GetSelItems(selcount,indexList);
	// On controle que les libellés n'ont pas été ajouté
	for(int idxlist=0;idxlist<selcount;idxlist++)
	{
		CString libelle;
		m_ListComplexe.GetText(indexList[idxlist],1,libelle);

		for(int lib=0;lib<m_LibelleAjoute.GetSize();lib++)
		{
			if(libelle==m_LibelleAjoute[lib])
			{
				// Le libellé ne peut pas être ajouté
				CString message,libelletype;
				
				libelletype="complexe";
								
				message.Format("Le %s %s ne peut être ajouté: Il fait déjà partie de la sélection",libelletype,libelle);
				MessageBox(message);
				return;
			}
		}
	}
	// ajout des chaines 
	for(idxlist=0;idxlist<selcount;idxlist++)
	{
		CString libelle;
		
		m_ListComplexe.GetText(indexList[idxlist],1,libelle);
		m_LibelleAjoute.Add(libelle);
	}
	delete indexList;
	
	// Récupération des éléments complexe suite à la sélection des groupes
	int selcountgrp = m_ListGrpComplexe.GetSelCount(); 
	int * indexListGrp = new int[selcountgrp];

	m_ListGrpComplexe.GetSelItems(selcountgrp, indexListGrp);

	// Boucle sur les groupes sélectionnés
	for(int idxlistgrp=0; idxlistgrp < selcountgrp; idxlistgrp++)
	{
		// Récup libellé du groue
		CString libgroupesel;
		m_ListGrpComplexe.GetText(indexListGrp[idxlistgrp],libgroupesel);

		// Boucle sur tous les groupes de complexes
		for(int grpcmplx=0; grpcmplx < theApp.m_TableCinema.GetTableGroupeComplexe()->GetSize(); grpcmplx++)
		{
			// Récupère le libellé du groupe 
			CString libgroupe = theApp.m_TableCinema.GetTableGroupeComplexe()->GetAt(grpcmplx).GetLibelle();

			if (libgroupesel == libgroupe)
			{
				// On ajoute les elmts complexes de ce groupe
				CArray<int,int> CompoComplexes;
				theApp.m_TableCinema.GetTableGroupeComplexe()->GetAt(grpcmplx).GetCompoComplexes(CompoComplexes);

				// Ajout un par un les complexes
				for (int idxcmplx=0; idxcmplx < CompoComplexes.GetSize(); idxcmplx++)
				{
					// Recup le code complexe à ajouter
					int CodeComplexe = CompoComplexes.GetAt(idxcmplx);
					sprintf(buf,"%s%d","C",CodeComplexe);
					
					// Cherche le libellé correspondant
					CString libellecomplexe = theApp.m_TableCinema.FindLibelleFromCode(buf);

					// On ajoutera le complexe si non déjà sélectioné
					bool AjoutComplexe = true;
					for(int lib=0;lib<m_LibelleAjoute.GetSize();lib++)
					{
						if(libellecomplexe == m_LibelleAjoute[lib])
						{
							AjoutComplexe = false;
							break;
						}
					}

					if (AjoutComplexe)
					{
						m_LibelleAjoute.Add(libellecomplexe);
					}
				}

				break;
			}
		}
	}
	delete indexListGrp;

	EndDialog(IDOK);
	
}

void CDlgSelComplexe::OnSeltout() 
{
	// On boucle sur les index
	for(int idxlist=0;idxlist<m_ListComplexe.GetCount();idxlist++)
	{
		// A VOIR MODIF ALAIN
		//m_ListComplexe.SetSel(idxlist,TRUE);		

		// Pour une liste box multi-sélection étendue ownner draw
		// On manipule directement la donnée associée à l'ITEM.
		m_ListComplexe.SetItemData(idxlist,1); 

	}	
	m_ListComplexe.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}



void CDlgSelComplexe::OnInverser() 
{
	// On sélectionne que ce qui n'a pas été sélectionné
	int count = m_ListComplexe.GetSelCount(); 
	int * SelElem = new int[count];
	m_ListComplexe.GetSelItems(count,SelElem);
	
	// On sélectionne d'abord toutes les lignes
	for(int idxlist=0;idxlist<m_ListComplexe.GetCount();idxlist++)
	{
		//m_ListComplexe.SetSel(idxlist,TRUE);		

		// Pour une liste box multi-sélection étendue ownner draw
		// On manipule directement la donnée associée à l'ITEM.
		m_ListComplexe.SetItemData(idxlist,1); 

	}
	// Et on enlève les lignes sélectionnées
	for(int idxsel=0;idxsel<count;idxsel++)
	{
		//m_ListComplexe.SetSel(SelElem[idxsel],FALSE);		

		m_ListComplexe.SetItemData(SelElem[idxsel],0); 
	}
	m_ListComplexe.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

}



bool CDlgSelComplexe::SetListe(int option)
{
	int NblibelleAjoute=0;
	m_ListComplexe.ResetContent(); 

	// titre
	SetWindowText("Sélection d'un complexe");
	
	// ajout de tous les libellés complexes
	for(int c=0;c<theApp.m_TableCinema.GetTableComplexe()->GetSize();c++)
	{
		CString libelle = theApp.m_TableCinema.GetTableComplexe()->GetLibelle(c);
		char regie = theApp.m_TableCinema.GetTableComplexe()->GetRegie(c);
		if((option==1)&&(regie!='A'))continue;
		if((option==2)&&(regie!='M'))continue;
		if((option==3)&&(regie!='N'))continue;

		//Récupération du complexe puis de son index et	deduction du bac 
		
		tagComplexe Cpx = theApp.m_TableCinema.GetTableComplexe()->GetAt(c);
		WORD indexBac = Cpx.m_indexBAC;
		CString bac = theApp.m_TableCinema.GetTableBac()->GetLibelle(indexBac);	
		
		CString Ligne;
		Ligne.Format("%s\t%c\t%s",libelle,regie,bac);	
		
		// On boucle sur les libellés ajoutés
		for(int idxlib=0;idxlib<m_LibelleAjoute.GetSize();idxlib++)
		{
			if(libelle==m_LibelleAjoute[idxlib])goto nextcomplexe;
		}
		
		m_ListComplexe.AddString(Ligne);
		NblibelleAjoute++;
		nextcomplexe:;
		
	}

		
	if(!NblibelleAjoute)return FALSE; 
	m_LibelleAjoute.RemoveAll();
	return TRUE;
}

//Méthode permettant de connaitre les libelles déja sélectionnés
void CDlgSelComplexe::SetLibelleAjoute(CStringArray &libelle)
{
	m_LibelleAjoute.Copy(libelle);
}

CStringArray& CDlgSelComplexe::GetLibelleAjoute()
{
	return m_LibelleAjoute;
}

BOOL CDlgSelComplexe::OnInitDialog() 
{ 
	CString LibelleType;
	m_ListComplexe.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	CDialog::OnInitDialog();

	if(!SetListe(0))
	{
		CString LibelleType;
		CString message;

		LibelleType="complexes";
		message.Format("Tous les %s ont déjà été ajouté",LibelleType);
		MessageBox(message);
		EndDialog(IDCANCEL);	
	}

	// Définition 3 colonnes
	m_ListComplexe.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListComplexe.InitSizeColonne(1,207);
	m_ListComplexe.InitSizeColonne(2,25);
	m_ListComplexe.InitSizeColonne(3,0);

	m_combo.AddString("Toutes");
	m_combo.AddString("Canal +");
	m_combo.AddString("Médiavision");
	m_combo.AddString("Indépendants");

	m_combo.SetCurSel(0);

	// Chargement des groupes de complexe
	SetListeGrpComplexes();

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Chargement de la liste des groupes de complexes
bool CDlgSelComplexe::SetListeGrpComplexes()
{
	// Init liste groupe
	m_ListGrpComplexe.ResetContent();

	// Boucle sur tous les groupes de complexes
	for(int grpcmplx=0; grpcmplx < theApp.m_TableCinema.GetTableGroupeComplexe()->GetSize(); grpcmplx++)
	{
		// Récupère le libellé du groupe 
		CString libgroupe = theApp.m_TableCinema.GetTableGroupeComplexe()->GetAt(grpcmplx).GetLibelle();

		// On l'ajoute à la liste
		m_ListGrpComplexe.AddString(libgroupe);
	}

	return true;
}

void CDlgSelComplexe::OnPaint() 
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

void CDlgSelComplexe::OnFiltreRegie() 
{
	// Mise a jour de la liste
	if(m_combo.GetCurSel()==0)SetListe(0);
	if(m_combo.GetCurSel()==1)SetListe(1);
	if(m_combo.GetCurSel()==2)SetListe(2);
	if(m_combo.GetCurSel()==3)SetListe(3);
	UpdateData(0);
}
