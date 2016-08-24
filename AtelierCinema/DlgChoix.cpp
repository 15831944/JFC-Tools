// DlgChoix.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgChoix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgChoix dialog


CDlgChoix::CDlgChoix(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChoix::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChoix)
	//}}AFX_DATA_INIT
}

void CDlgChoix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChoix)
	DDX_Control(pDX, IDC_CADRE7, m_Cadre7);
	DDX_Control(pDX, IDC_SELTOUT2,		m_Seltout);
	DDX_Control(pDX, IDC_INVERSER2,		m_Inverser);
	DDX_Control(pDX, IDC_ANNULER,		m_Cancel);
	DDX_Control(pDX, IDC_OK,			m_OK);
	DDX_Control(pDX, IDC_REGIE,			m_Label);
	DDX_Control(pDX, IDC_BARRE,			m_Barre);
	DDX_Control(pDX, IDC_COMBOREGIE,	m_ComboRegie);
	DDX_Control(pDX, IDC_CADRE2,		m_Cadre2);
	DDX_Control(pDX, IDC_CADRE,			m_Cadre);
	DDX_Control(pDX, IDC_LIST,			m_List);
	DDX_Control(pDX, IDC_LIST_GRP_BACS, m_List_Grp_Bacs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChoix, CDialog)
	//{{AFX_MSG_MAP(CDlgChoix)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SELTOUT2, OnSeltout)
	ON_BN_CLICKED(IDC_INVERSER2, OnInverser)
	ON_CBN_SELCHANGE(IDC_COMBOREGIE, OnFiltreRegie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChoix message handlers

void CDlgChoix::OnOk() 
{
	char buf[128];

	// On récupère la sélection active
	// récupération des éléments d'une sélection multiple
	int selcount = m_List.GetSelCount(); 
	int * indexList = new int[selcount];
	
	m_List.GetSelItems(selcount,indexList);

	// On controle que les libellés n'ont pas été ajouté
	for(int idxlist=0;idxlist<selcount;idxlist++)
	{
		CString libelle;
		m_List.GetText(indexList[idxlist],libelle);

		for(int lib=0;lib<m_LibelleAjoute.GetSize();lib++)
		{
			if(libelle == m_LibelleAjoute[lib])
			{
				// Le libellé ne peut pas être ajouté
				CString message,libelletype;
				switch(m_Type)
				{
					case 0:
						libelletype="produit";
					break;
					case 1:
						libelletype="bac";
					break;
					case 2:
						libelletype="complexe";
					break;

				}
				message.Format("Le %s %s ne peut être ajouté: Il fait déjà partie de la sélection",libelletype,libelle);
				MessageBox(message);
				return;
			}
		}
	}

	

	m_LibelleAjoute.RemoveAll();

	// ajout des chaines dans m_ListChoix
	for(idxlist=0; idxlist<selcount; idxlist++)
	{
		CString libelle;
		m_List.GetText(indexList[idxlist],libelle);
		m_LibelleAjoute.Add(libelle);
	}
	delete indexList;


	// Récupération des éléments bac suite à la sélection des groupes
	int selcountgrp = m_List_Grp_Bacs.GetSelCount(); 
	int * indexListGrp = new int[selcountgrp];

	m_List_Grp_Bacs.GetSelItems(selcountgrp, indexListGrp);

	// Boucle sur les groupes sélectionnés
	for(int idxlistgrp=0; idxlistgrp < selcountgrp; idxlistgrp++)
	{
		// Récup libellé du groue
		CString libgroupesel;
		m_List_Grp_Bacs.GetText(indexListGrp[idxlistgrp],libgroupesel);

		
		// Boucle sur tous les groupes de bacs
		for(int grpbac=0; grpbac < theApp.m_TableCinema.GetTableGroupeBac()->GetSize(); grpbac++)
		{
			// Récupère le libellé du groupe 
			CString libgroupe = theApp.m_TableCinema.GetTableGroupeBac()->GetAt(grpbac).GetLibelle();

			if (libgroupesel == libgroupe)
			{
				// On ajoute les elmts bacs de ce groupe
				CArray<long, long> CompoBacs;
				theApp.m_TableCinema.GetTableGroupeBac()->GetAt(grpbac).GetCompoBacs(CompoBacs);

				// Ajout un par un les bacs
				for (int idxbac=0; idxbac < CompoBacs.GetSize(); idxbac++)
				{
					// Recup le code bac à ajouter
					int CodeBac = CompoBacs.GetAt(idxbac);

					// Code pure bac  (sans l'extension composite)
					CString CodeStr;
					sprintf(buf,"%6d", CodeBac);
					CodeStr = buf;

					// Recup Code pure sans la partie numérique composite
					CodeBac = CodeBac % 100000;
					
					// Créer le code bac pure (type Bxxxxx)
					sprintf(buf,"%s%05d","B",CodeBac);

					// Cherche le libellé correspondant
					CString libellebac = theApp.m_TableCinema.FindLibelleFromCode(buf);

					// Tient compte du code composite (1/ Normal 2/"A" 3/ "M" 4/"N")
					if (CodeStr.Left(1) == "1")
						libellebac = libellebac;
					else if (CodeStr.Left(1) == "2")
						libellebac = libellebac + " (A)";
					else if (CodeStr.Left(1) == "3")
						libellebac = libellebac + " (M)";
					else if (CodeStr.Left(1) == "4")
						libellebac = libellebac + " (N)";
						

					// On ajoutera le bac si non déjà sélectionné
					bool AjoutBac = true;
					for(int lib=0;lib<m_LibelleAjoute.GetSize();lib++)
					{
						if(libellebac == m_LibelleAjoute[lib])
						{
							AjoutBac = false;
							break;
						}
					}

					if (AjoutBac)
					{
						m_LibelleAjoute.Add(libellebac);
					}
				}

				break;
			}
		}
	}
	delete indexListGrp;

	
	EndDialog(IDOK);	
	
}

void CDlgChoix::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
	
}

BOOL CDlgChoix::OnInitDialog() 
{

	// Initialisation de la liste box étendue JFC
	m_List.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);

	// Initialisation de la liste box étendue JFC pour les groupes de Bacs
	m_List_Grp_Bacs.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	
	CDialog::OnInitDialog();

	// Type de régie
	CTableRegie * regie = theApp.m_TableCinema.GetTableRegie();
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString(regie->GetLibelle("T"));
	m_ComboRegie.AddString(regie->GetLibelle("P"));
	m_ComboRegie.AddString(regie->GetLibelle("A"));
	m_ComboRegie.AddString(regie->GetLibelle("M"));
	m_ComboRegie.AddString(regie->GetLibelle("N"));
	m_ComboRegie.SetCurSel(0);

	if(!SetListe())
	{
		CString LibelleType;
		switch(m_Type)
		{
		case 0:
			LibelleType="produit";
			break;
		case 1:
			LibelleType="bac";
			break;
		case 2:
			LibelleType="complexe";
			break;
		}
		CString message;
		message.Format("Tous les %s ont déjà été ajouté",LibelleType);
		MessageBox(message);
		EndDialog(IDCANCEL);	
	}

	// Si la boite est utilisé pour sélectionner un produit, on désactive le filter régie.
	if(m_Type==0)
	{
		m_ComboRegie.ModifyStyle(WS_VISIBLE,NULL,0);
		m_Cadre2.ModifyStyle(WS_VISIBLE,NULL,0);
		m_Label.ModifyStyle(WS_VISIBLE,NULL,0);
		m_Barre.ModifyStyle(WS_VISIBLE,NULL,0);

		// Décalage des boutons
		RECT rectOK,rectINV,rectTOUT,rectCANCEL,rectDIALOG;

		m_Inverser.GetWindowRect(&rectINV);
		m_Seltout.GetWindowRect(&rectTOUT);
		m_Cancel.GetWindowRect(&rectCANCEL);
		m_OK.GetWindowRect(&rectOK);
		GetClientRect(&rectDIALOG);

		POINT pt;
		pt.x = rectINV.left;
		pt.y = rectINV.top;
		ScreenToClient(&pt);
		::SetWindowPos(m_Inverser.m_hWnd,HWND_NOTOPMOST,pt.x-13,pt.y+74,rectINV.right-rectINV.left,rectINV.bottom-rectINV.top,SWP_NOZORDER|SWP_SHOWWINDOW);

		pt.x = rectTOUT.left;
		pt.y = rectTOUT.top;
		ScreenToClient(&pt);
		::SetWindowPos(m_Seltout.m_hWnd,HWND_NOTOPMOST,pt.x-13,pt.y+74,rectTOUT.right-rectTOUT.left,rectTOUT.bottom-rectTOUT.top,SWP_NOZORDER|SWP_SHOWWINDOW);

		pt.x = rectCANCEL.left;
		pt.y = rectCANCEL.top;
		ScreenToClient(&pt);
		::SetWindowPos(m_Cancel.m_hWnd,HWND_NOTOPMOST,pt.x-13,pt.y,rectCANCEL.right-rectCANCEL.left,rectCANCEL.bottom-rectCANCEL.top,SWP_NOZORDER|SWP_SHOWWINDOW);

		pt.x = rectOK.left;
		pt.y = rectOK.top;
		ScreenToClient(&pt);
		::SetWindowPos(m_OK.m_hWnd,HWND_NOTOPMOST,pt.x-13,pt.y,rectOK.right-rectOK.left,rectOK.bottom-rectOK.top,SWP_NOZORDER|SWP_SHOWWINDOW);

		int xborder = ::GetSystemMetrics(SM_CXBORDER);
		int yborder = ::GetSystemMetrics(SM_CYBORDER);
		int ytitre  = ::GetSystemMetrics(SM_CYMENU);

		// Décalage de la fenetre
		RECT rect;
		GetWindowRect(&rect);

		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,(rectDIALOG.right-rectDIALOG.left)-20+2*xborder,(rectDIALOG.bottom-rectDIALOG.top)+ytitre+2*yborder+7,SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOMOVE);

		CWnd * parent = GetParent();
		RECT rectparent;
		parent->GetWindowRect(&rectparent);

		pt.x = rectparent.left +xborder+2;//+ (rectparent.right-rectparent.left)/2-(rect.right-rect.left)/2;
		pt.y = rectparent.top + (rectparent.bottom-rectparent.top)/2-(rect.bottom - rect.top)/2;

		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,pt.x,pt.y,0,0,SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOSIZE);

		return TRUE;
	}

	// Chargement des groupes de bacs
	SetListeGrpBacs();

	UpdateData(0);
	return TRUE;  
}

void CDlgChoix::SetType(int type)
{
	m_Type=type;
}

void CDlgChoix::SetLibelleAjoute(CStringArray& libelle)
{
	m_LibelleAjoute.Copy(libelle); 
}

CStringArray& CDlgChoix::GetLibelleAjoute()
{
	return m_LibelleAjoute; 
}

bool CDlgChoix::SetListe(const CString regie)
{
	int NblibelleAjoute=0;
	m_List.ResetContent();

	switch(m_Type)
	{
		// produit
		case 0:
		{
			// titre
			SetWindowText("Sélection d'un produit");
			// ajout de tous les libellés produits
			for(int p=0;p<theApp.m_TableCinema.GetTableProduit()->GetSize();p++)
			{
				CString libelle = theApp.m_TableCinema.GetTableProduit()->GetLibelle(p);
				// On boucle sur les libellés ajoutés
				for(int idxlib=0;idxlib<m_LibelleAjoute.GetSize();idxlib++)
				{
					if(libelle==m_LibelleAjoute[idxlib])goto nextproduit;
				}
				m_List.AddString(libelle);
				NblibelleAjoute++;
nextproduit:;
			}
		}
		break;

		// bac
		case 1:
		{
			// titre
			SetWindowText("Sélection d'un bac");

			// Nb elmt table bacs
			int Nb = theApp.m_TableCinema.GetTableBac()->GetSize();

			for(int bac=0;bac<theApp.m_TableCinema.GetTableBac()->GetSize();bac++)
			{
				CString code = theApp.m_TableCinema.GetTableBac()->GetCodeComposite(bac);
				tagBAC b = theApp.m_TableCinema.GetTableBac()->GetAt(bac);

				// ajout de tous les libellés bacs
				if(code.GetLength()!=7)
				{
#ifdef _DEBUG
					MessageBox("Ceci n'est pas un code composite");
#endif
				}

				if(b.m_Quota==0)continue;

				if(regie=="A")
				{
					if(code[6]!='A')continue;
				}
				if(regie=="M")
				{
					if(code[6]!='M')continue;
				}
				if(regie=="N")
				{
					if(code[6]!='N')continue;
				}
				if(regie=="P")
				{
					if(code[6]!='P')continue;
				}

				CString libelle = theApp.m_TableCinema.GetTableBac()->GetLibelle(bac);
				// On boucle sur les libellés ajoutés
				for(int idxlib=0;idxlib<m_LibelleAjoute.GetSize();idxlib++)
				{
					if(libelle==m_LibelleAjoute[idxlib])goto nextbac;
				}
				m_List.AddString(libelle);
				NblibelleAjoute++;
nextbac:;
			}
		}
		break;
		// complexe
		case 2:
		{
			// titre
			SetWindowText("Sélection d'un complexe");
			// ajout de tous les libellés complexes
			for(int c=0;c<theApp.m_TableCinema.GetTableComplexe()->GetSize();c++)
			{
				CString libelle = theApp.m_TableCinema.GetTableComplexe()->GetLibelle(c);
				// On boucle sur les libellés ajoutés
				for(int idxlib=0;idxlib<m_LibelleAjoute.GetSize();idxlib++)
				{
					if(libelle==m_LibelleAjoute[idxlib])goto nextcomplexe;
				}
				m_List.AddString(libelle);
				NblibelleAjoute++;
nextcomplexe:;
			}

		}
		break;
		default:
			ASSERT(0);
			break;

	}
	if(!NblibelleAjoute)return FALSE; 
	//m_LibelleAjoute.RemoveAll();
	return TRUE;
}

// Chargement de la liste des groupes de bacs
bool CDlgChoix::SetListeGrpBacs()
{

	// Init liste groupe
	m_List_Grp_Bacs.ResetContent();

	// Boucle sur tous les groupes de bacs
	for(int grpbac=0; grpbac < theApp.m_TableCinema.GetTableGroupeBac()->GetSize(); grpbac++)
	{
		// Récupère le libellé du groupe 
		CString libgroupe = theApp.m_TableCinema.GetTableGroupeBac()->GetAt(grpbac).GetLibelle();

		// On l'ajoute à la liste
		m_List_Grp_Bacs.AddString(libgroupe);
	}

	return true;
}

void CDlgChoix::OnPaint() 
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

// On sélectionne tout
void CDlgChoix::OnSeltout() 
{
	// On boucle sur les index
	for(int idxlist=0;idxlist<m_List.GetCount();idxlist++)
	{
		//m_List.SetSel(idxlist,TRUE);		
		m_List.SetItemData(idxlist,1);		
	}
	m_List.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

}
// Inversion de la sélection
void CDlgChoix::OnInverser() 
{
	// On sélectionne que ce qui n'a pas été sélectionné
	int count = m_List.GetSelCount(); 
	int * SelElem = new int[count];
	m_List.GetSelItems(count,SelElem);
	
	// On sélectionne d'abord toutes les lignes
	for(int idxlist=0;idxlist<m_List.GetCount();idxlist++)
	{
		//m_List.SetSel(idxlist,TRUE);		
		m_List.SetItemData(idxlist,1);		
	}
	// Et on enlève les lignes sélectionnées
	for(int idxsel=0;idxsel<count;idxsel++)
	{
		//m_List.SetSel(SelElem[idxsel],FALSE);		
		m_List.SetItemData(SelElem[idxsel],0);		
	}
	m_List.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

// Changement du filtre de régie
void CDlgChoix::OnFiltreRegie() 
{
	// Mise a jour de la liste
	if(m_ComboRegie.GetCurSel()==0)SetListe();
	if(m_ComboRegie.GetCurSel()==1)SetListe("P");
	if(m_ComboRegie.GetCurSel()==2)SetListe("A");
	if(m_ComboRegie.GetCurSel()==3)SetListe("M");
	if(m_ComboRegie.GetCurSel()==4)SetListe("N");
	UpdateData(0);
}


