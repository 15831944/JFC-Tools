// DlgSelectionProduit.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgSelectionProduit.h"
#include "DlgChoix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionProduit dialog


CDlgSelectionProduit::CDlgSelectionProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectionProduit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectionProduit)
	//}}AFX_DATA_INIT
}


void CDlgSelectionProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectionProduit)
	DDX_Control(pDX, IDC_CADRE11, m_Cadre11);
	DDX_Control(pDX, IDC_CADRE10, m_Cadre10);
	DDX_Control(pDX, IDC_LISTSELECT, m_ListSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectionProduit, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectionProduit)
	ON_BN_CLICKED(IDC_AJOUT, OnAjout)
	ON_BN_CLICKED(IDC_SUPP, OnSupp)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionProduit message handlers

void CDlgSelectionProduit::OnOK() 
{
	

	if(m_Code.GetSize()==0)	
	{
		// la sélection est invalide il faut sélectionner au moins 1 produit
		CString message;
		message.Format("La sélection est invalide: Veuillez sélectionner au moins un produit");
		MessageBox(message);
		return;
	}
	EndDialog(IDOK);
}

void CDlgSelectionProduit::OnCancel() 
{
	EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////////////////
// Ajout de produits pour évaluation produits films
void CDlgSelectionProduit::OnAjout() 
{
	CDlgChoix dlg;

	// On ajoute des produits
	dlg.SetType(0); 

	// Donne à la boite que les libellés qui ne font pas partie de la sélection courante 
	dlg.SetLibelleAjoute(m_Libelle); 
	if(dlg.DoModal()!=IDOK)return;

	// ajout des libellés sélectionnés
	m_Libelle.Append(dlg.GetLibelleAjoute()); 
	UpdateListFromLibelle();
	
}


//////////////////////////////////////////////////////////////////////////////////////
// Suppression de produits pour évaluation produits films
void CDlgSelectionProduit::OnSupp() 
{
	// récupération éléments produit d'une sélection multiple
	int selcount = m_ListSelect.GetSelCount(); 
	int * indexList = new int[selcount];
	m_ListSelect.GetSelItems(selcount,indexList);

	// suppression des chaines de la liste
	for(int idxlist=selcount-1;idxlist>=0;idxlist--)
	{
		m_ListSelect.DeleteString(indexList[idxlist]);
	}

	// Mise à jour des libellés
	UpdateLibelleFromList();

	// Destruction de la liste provisoire d'indexs sélectionnés pr
	delete indexList;		
	
}

//////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste
void CDlgSelectionProduit::UpdateListFromLibelle()
{
	m_ListSelect.ResetContent(); 

	// On convertit les libellés sélectionnés en code
	m_Code.RemoveAll(); 

	for(int idxlib=0;idxlib<m_Libelle.GetSize();idxlib++)
	{
		CString code,libelle;

		// Ceci permet de récupérer seulement le libellé 
		int pos = m_Libelle[idxlib].Find("\t");

		if(pos>=0)
		{
			libelle = m_Libelle[idxlib].Left(pos); 

			// ligne d'un complexe déjà ajouté
			if(libelle=="P")
				libelle = m_Libelle[idxlib].Right(m_Libelle[idxlib].GetLength()-(pos+1)); 
		}
		else libelle = m_Libelle[idxlib];

		code = theApp.m_TableCinema.FindCodeFromLibelle(libelle);
		m_Code.Add(code);

		// Type + libellé
		CString ligne;
		ligne = m_Code[idxlib][0];
		ligne+="\t";
		ligne+= libelle;
		m_ListSelect.AddString(ligne);
	}
}

void CDlgSelectionProduit::UpdateLibelleFromList()
{
	m_Libelle.RemoveAll(); 

	// On convertit les libellés sélectionnés en code
	m_Code.RemoveAll(); 

	for(int idxlib=0;idxlib<m_ListSelect.GetCount();idxlib++)
	{
		CString libelle;
		m_ListSelect.GetText(idxlib,libelle);
		m_Libelle.Add(libelle);
		int pos = libelle.Find('\t');
		if(pos>=0)
		{	// Ce libellé est un libellé complexe
			libelle = libelle.Right(libelle.GetLength()-(pos+1)); 
		}
		CString code = theApp.m_TableCinema.FindCodeFromLibelle(libelle);
		m_Code.Add(code);

	}
}

BOOL CDlgSelectionProduit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ListSelect.SetTextAlign(DT_CENTER);
	m_ListSelect.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);

	// On récupère les libellés à partir des codes
	for(int idcode=0;idcode<m_Code.GetSize();idcode++)
	{
		CString libelle;
		libelle = theApp.m_TableCinema.FindLibelleFromCode(m_Code[idcode]);
		m_Libelle.Add(libelle);
	}

	// On ajoute les libellés à la liste 
	UpdateListFromLibelle();
	UpdateData(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectionProduit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Définition de la couleur de fond
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	// Remplissage couleur
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

// Ajout d'un code produit au tableau des codes sélectionnés
void CDlgSelectionProduit::AddCode(CString code)
{
	m_Code.Add(code); 
}

// Récupère code produit via sélection en cours
CString CDlgSelectionProduit::GetCode(int position)
{
	CString code =  m_Code[position];
	return code;
}

// Nombre de code produits sélectionnés
int CDlgSelectionProduit::GetNbCode()
{
	return m_Code.GetSize();
}
