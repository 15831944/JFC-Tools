// DlgExportMediavision.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgExportMediavision.h"
#include "Campagne.h"

extern CCinemaApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportMediavision dialog


CDlgExportMediavision::CDlgExportMediavision(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportMediavision::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportMediavision)
	m_DateDebut = _T("");
	m_DateFin = _T("");
	m_SsTitre = _T("");
	m_Titre = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExportMediavision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportMediavision)
	DDX_Control(pDX, IDC_LISTSELCIBLES, m_LstSelCiblesExportM);
	DDX_Control(pDX, IDC_LISTCIBLES, m_LstCiblesExportM);
	DDX_Control(pDX, IDC_CADRE7, m_CadreCibles);
	DDX_Control(pDX, IDC_CADRE, m_CadreTitres);
	DDX_Text(pDX, IDC_DATEDEBUT, m_DateDebut);
	DDX_Text(pDX, IDC_DATEFIN, m_DateFin);
	DDX_Text(pDX, IDC_NOMSSTITRE, m_SsTitre);
	DDX_Text(pDX, IDC_NOMTITRE, m_Titre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportMediavision, CDialog)
	//{{AFX_MSG_MAP(CDlgExportMediavision)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LISTCIBLES, OnDblclkListcibles)
	ON_LBN_DBLCLK(IDC_LISTSELCIBLES, OnDblclkListselcibles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportMediavision message handlers
void CDlgExportMediavision::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

/////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue export M�diavision
BOOL CDlgExportMediavision::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Init des 2 listes cible
	m_LstCiblesExportM.ResetContent(); 
	m_LstSelCiblesExportM.ResetContent(); 

	// Init s�lection des cibles
	m_TSelCibleExportM.RemoveAll();

	// Chargement des cibles
	if (ChargeLstCible())
	{

		// Debut et fin date campagne
		CString TxtDateDeb,TxtDateFin;
		m_DateDebut = m_pCampagne->m_DateDebut.Format("%d-%m-%Y");
		COleDateTime DateFin;
		DateFin = m_pCampagne->m_DateDebut + COleDateTimeSpan(m_pCampagne->m_NbSemaine*7-1,0,0,0);
		m_DateFin = DateFin.Format("%d-%m-%Y");

		// Init edit titre et ss-titre
		m_Titre = "";
		m_SsTitre = "";

		// Mise � jour des controles
		UpdateData(0);

		return TRUE;	// return TRUE unless you set the focus to a control
						// EXCEPTION: OCX Property Pages should return FALSE
	}

	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// Chargement des cibles disponibles
BOOL CDlgExportMediavision::ChargeLstCible()
{
	if (theApp.m_Cible.m_LibCible.GetSize() > 0)
	{
		// Au moins & cible disponible
		for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
		{
			// Ajoute � la liste
			m_LstCiblesExportM.AddString(theApp.m_Cible.m_LibCible[c]);
			
			// Conserve ident cible
			m_LstCiblesExportM.SetItemData(c,c); 
		}
		return TRUE;
	}

	else
		// Aucune cible disponible
		return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////
// Validation de la s�lection pour l'export
void CDlgExportMediavision::OnOK() 
{
	// Mise � jour des valeurs
	UpdateData(1);

	// Init s�lection des cibles
	m_TSelCibleExportM.RemoveAll();

	// Remplissage tableau des cibles s�lectionn�es
	for (int InxCib = 0; InxCib < m_LstSelCiblesExportM.GetCount(); InxCib++)
	{
		// Index dans tableau des ident
		int Index = m_LstSelCiblesExportM.GetItemData(InxCib);

		// Ajout Ident Cible au tableau des cibles s�lectionn�es
		m_TSelCibleExportM.Add(Index);
	}
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////
// S�lection d'une cible
void CDlgExportMediavision::OnDblclkListcibles() 
{
	// R�cup�re le texte de la cible s�lectionn�e
	CString TxtCible;
	m_LstCiblesExportM.GetText(m_LstCiblesExportM.GetCurSel(),TxtCible);

	// Transfert cible s�lectionn�e dans liste cibles � exporter
	m_LstSelCiblesExportM.AddString(TxtCible);  

	// Conserve l'ident cible
	int InxCur		= m_LstSelCiblesExportM.GetCount()-1;
	int InxCurBase  = m_LstCiblesExportM.GetCurSel();

	int ItemData	= m_LstCiblesExportM.GetItemData(InxCurBase);
	// int ItemData = -1;


//////////////////////////////
	int size=theApp.m_Cible.m_NumCible.GetSize();  
	for(int cible=0;cible<size;cible++)
	{
		if(theApp.m_Cible.m_LibCible[cible]==TxtCible)
		{
			ItemData = cible; // theApp.m_Cible.m_NumCible[cible];
		}
	}
//////////////////////////////


	m_LstSelCiblesExportM.SetItemData(InxCur,ItemData); 

	// Puis on enl�ve la cible de la liste de base
	m_LstCiblesExportM.DeleteString(m_LstCiblesExportM.GetCurSel()); 
	
}


/////////////////////////////////////////////////////////////////////////////////
// D�s�lection d'une cible
void CDlgExportMediavision::OnDblclkListselcibles() 
{
	
	// R�cup�re le texte de la cible � d�selectionn�e
	CString TxtCible;
	m_LstSelCiblesExportM.GetText(m_LstSelCiblesExportM.GetCurSel(),TxtCible);

	// Remet la cible dans liste cibles disponibles
	m_LstCiblesExportM.AddString(TxtCible);  

	// Conserve l'ident cible
	int InxSel		= m_LstSelCiblesExportM.GetCurSel();
	int ItemData	= m_LstSelCiblesExportM.GetItemData(InxSel);
	m_LstCiblesExportM.SetItemData(m_LstCiblesExportM.GetCount()-1,ItemData); 

	// Puis on enl�ve la cible de la liste de base
	m_LstSelCiblesExportM.DeleteString(m_LstSelCiblesExportM.GetCurSel()); 	
}
