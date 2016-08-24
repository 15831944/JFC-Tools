// DlgExportScreenvision.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgExportScreenvision.h"
#include "Campagne.h"

extern CCinemaApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportScreenvision dialog


CDlgExportScreenvision::CDlgExportScreenvision(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportScreenvision::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportScreenvision)
		// NOTE: the ClassWizard will add member initialization here
	m_DateDebut = _T("");
	m_DateFin = _T("");
	m_SsTitre = _T("");
	m_Titre = _T("");
	// m_CiblePerfSem = _T("AAAAAA");
	//}}AFX_DATA_INIT
}


void CDlgExportScreenvision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportScreenvision)
	DDX_Control(pDX, IDC_CIBLE_PERF_SEM, m_CtrlCiblePerfSem);
	DDX_Control(pDX, IDC_LISTSELCIBLES, m_LstSelCiblesExportSV);
	DDX_Control(pDX, IDC_LISTCIBLES, m_LstCiblesExportSV);
	DDX_Control(pDX, IDC_CADRE7, m_CadreCibles);
	DDX_Control(pDX, IDC_CADRE, m_CadreTitres);
	DDX_Text(pDX, IDC_DATEDEBUT, m_DateDebut);
	DDX_Text(pDX, IDC_DATEFIN, m_DateFin);
	DDX_Text(pDX, IDC_NOMSSTITRE, m_SsTitre);
	DDX_Text(pDX, IDC_NOMTITRE, m_Titre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportScreenvision, CDialog)
	//{{AFX_MSG_MAP(CDlgExportScreenvision)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LISTCIBLES, OnDblclkListcibles)
	ON_LBN_DBLCLK(IDC_LISTSELCIBLES, OnDblclkListselcibles)
	ON_LBN_SELCHANGE(IDC_LISTSELCIBLES, OnSelchangeListselcibles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportScreenvision message handlers
void CDlgExportScreenvision::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

/////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue export Screenvision
BOOL CDlgExportScreenvision::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Init des 2 listes cible
	m_LstCiblesExportSV.ResetContent(); 
	m_LstSelCiblesExportSV.ResetContent(); 

	// Init sélection des cibles
	m_TSelCibleExportSV.RemoveAll();

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

		// Init Index cible performances par semaines
		m_IndexCiblePerfSem = -1;

		// Mise à jour des controles
		UpdateData(0);

		return TRUE;	// return TRUE unless you set the focus to a control
						// EXCEPTION: OCX Property Pages should return FALSE
	}

	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
// Chargement des cibles disponibles
BOOL CDlgExportScreenvision::ChargeLstCible()
{
	if (theApp.m_Cible.m_LibCible.GetSize() > 0)
	{
		// Au moins & cible disponible
		for(int c=0;c<theApp.m_Cible.m_LibCible.GetSize();c++)
		{
			// Ajoute à la liste
			CString LibCible = theApp.m_Cible.m_LibCible[c];
			m_LstCiblesExportSV.AddString(theApp.m_Cible.m_LibCible[c]);
			
			// Conserve ident cible
			m_LstCiblesExportSV.SetItemData(c,c); 
		}
		return TRUE;
	}

	else
		// Aucune cible disponible
		return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////
// Validation de la sélection pour l'export
void CDlgExportScreenvision::OnOK() 
{
	// Mise à jour des valeurs
	UpdateData(1);

	// m_Titre
	// m_SsTitre

	// Init sélection des cibles
	m_TSelCibleExportSV.RemoveAll();

	// Text de la cible pour perf semaine
	CString TxtCibPerf;
	m_CtrlCiblePerfSem.GetWindowText(TxtCibPerf);

	// Remplissage tableau des cibles sélectionnées
	for (int InxCib = 0; InxCib < m_LstSelCiblesExportSV.GetCount(); InxCib++)
	{
		// Index dans tableau des ident
		int Index = m_LstSelCiblesExportSV.GetItemData(InxCib);

		// Texte associé
		CString Txt;
		Txt = theApp.m_Cible.m_LibCible[Index];

		// Ajout Ident Cible au tableau des cibles sélectionnées
		m_TSelCibleExportSV.Add(Index);

		if (TxtCibPerf == Txt)
		{
			m_IndexCiblePerfSem = Index;
		}
	}
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////
// Sélection d'une cible
void CDlgExportScreenvision::OnDblclkListcibles() 
{
	// Récupère le texte de la cible sélectionnée
	CString TxtCible;
	m_LstCiblesExportSV.GetText(m_LstCiblesExportSV.GetCurSel(),TxtCible);

	// Transfert cible sélectionnée dans liste cibles à exporter
	m_LstSelCiblesExportSV.AddString(TxtCible);  

	// Conserve l'ident cible
	int InxCur		= m_LstSelCiblesExportSV.GetCount()-1;
	int InxCurBase  = m_LstCiblesExportSV.GetCurSel();

	int ItemData	= m_LstCiblesExportSV.GetItemData(InxCurBase);
	// int ItemData = -1;


	//////////////////////////////
	int size=theApp.m_Cible.m_NumCible.GetSize();  
	for(int cible=0;cible<size;cible++)
	{
		if(theApp.m_Cible.m_LibCible[cible]==TxtCible)
		{
			ItemData = cible; 
		}
	}
	//////////////////////////////


	m_LstSelCiblesExportSV.SetItemData(InxCur,ItemData); 

	// Puis on enlève la cible de la liste de base
	m_LstCiblesExportSV.DeleteString(m_LstCiblesExportSV.GetCurSel()); 


	// Update cible pour perf semaine -- si rien de sélectionner on prend la 1ere
	CString TxtCibPerf;
	m_CtrlCiblePerfSem.GetWindowText(TxtCibPerf);
	if (TxtCibPerf == "" && m_LstSelCiblesExportSV.GetCount() > 0)
	{
		m_LstSelCiblesExportSV.GetText(m_LstSelCiblesExportSV.GetSel(0),TxtCibPerf);
		m_CtrlCiblePerfSem.SetWindowText(TxtCibPerf);
		
	}
	else if (m_LstSelCiblesExportSV.GetCount() == 0)
	{
		m_CtrlCiblePerfSem.SetWindowText("");
	}
	
}


/////////////////////////////////////////////////////////////////////////////////
// Désélection d'une cible
void CDlgExportScreenvision::OnDblclkListselcibles() 
{
	
	// Récupère le texte de la cible à déselectionnée
	CString TxtCible;
	m_LstSelCiblesExportSV.GetText(m_LstSelCiblesExportSV.GetCurSel(),TxtCible);

	// Remet la cible dans liste cibles disponibles
	m_LstCiblesExportSV.AddString(TxtCible);  

	// Conserve l'ident cible
	int InxSel		= m_LstSelCiblesExportSV.GetCurSel();
	int ItemData	= m_LstSelCiblesExportSV.GetItemData(InxSel);
	m_LstCiblesExportSV.SetItemData(m_LstCiblesExportSV.GetCount()-1,ItemData); 

	
	// Puis on enlève la cible de la liste de base
	CString CurDeleteCibles = m_LstSelCiblesExportSV.GetCurSel();
	m_LstSelCiblesExportSV.DeleteString(m_LstSelCiblesExportSV.GetCurSel()); 
	
	// Update cible pour perf semaine -- si rien de sélectionner on prend la 1ere
	CString TxtCibPerf;
	m_CtrlCiblePerfSem.GetWindowText(TxtCibPerf);
	if (TxtCibPerf == "" && m_LstSelCiblesExportSV.GetCount() > 0)
	{
		m_LstSelCiblesExportSV.GetText(m_LstSelCiblesExportSV.GetSel(0),TxtCibPerf);
		m_CtrlCiblePerfSem.SetWindowText(TxtCibPerf);
	}
	else if (TxtCibPerf == TxtCible && m_LstSelCiblesExportSV.GetCount() > 0)
	{
		m_LstSelCiblesExportSV.GetText(m_LstSelCiblesExportSV.GetSel(0),TxtCibPerf);
		m_CtrlCiblePerfSem.SetWindowText(TxtCibPerf);
	}		
	else if (m_LstSelCiblesExportSV.GetCount() == 0)
	{
		m_CtrlCiblePerfSem.SetWindowText("");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgExportScreenvision message handlers

/////////////////////////////////////////////////////////////////////////////////
// Sélection d'une cible pour les performances semaines
void CDlgExportScreenvision::OnSelchangeListselcibles() 
{
	CString TxtCibPerf;
	m_LstSelCiblesExportSV.GetText(m_LstSelCiblesExportSV.GetCurSel(),TxtCibPerf);
	m_CtrlCiblePerfSem.SetWindowText(TxtCibPerf);
}
