// FormulaireGroupe.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireGroupe.h"
#include "FormulaireEDI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaireGroupe dialog


CFormulaireGroupe::CFormulaireGroupe(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireGroupe::IDD, pParent)
{

	// Par défaut ce n'est pas un annonceur
	m_Annonceur = false;

	// Mode ajout par défaut
	m_Mode = 0;


	//}}AFX_DATA_INIT
}


void CFormulaireGroupe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireGroupe)
	DDX_Control(pDX, IDC_STATIC_ANNEECOUR, m_AnneeCur);
	DDX_Control(pDX, IDC_CADRE7, m_Cadre7);
	DDX_Control(pDX, IDC_STATIC_REF4, m_StaticRef4);
	DDX_Control(pDX, IDC_EDIT4_4, m_ChampsDateFG);
	DDX_Control(pDX, IDC_EDIT4_3, m_ChampsCodeFG);
	DDX_Control(pDX, IDC_EDIT4_2, m_ChampsAliasFG);
	DDX_Control(pDX, IDC_EDIT4_1, m_ChampsNomRepFG);
	//}}AFX_DATA_MAP
}

// Recupére les valeurs controles >> variables m_Str
void CFormulaireGroupe::MajValChamps()
{
	m_ChampsNomRepFG.SetWindowText(m_StrNomRepFG);
	m_ChampsAliasFG.SetWindowText(m_StrAliasFG);
	m_ChampsCodeFG.SetWindowText(m_StrCodeFG);
	m_ChampsDateFG.SetWindowText(m_StrDateFG.Left(4));
	m_DateParDefaut = m_StrDateFG;	
}

// Renvoi les champs existants
void CFormulaireGroupe::RenvoiValChamps()
{
	m_ChampsNomRepFG.GetWindowText(m_StrNomRepFG);
	m_ChampsAliasFG.GetWindowText(m_StrAliasFG);
	m_ChampsCodeFG.GetWindowText(m_StrCodeFG);
	m_ChampsDateFG.GetWindowText(m_StrDateFG);
}

BEGIN_MESSAGE_MAP(CFormulaireGroupe, CDialog)
	//{{AFX_MSG_MAP(CFormulaireGroupe)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_INFOEDI, OnInfoedi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireGroupe message handlers

BOOL CFormulaireGroupe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Réecriture entete fenetre
	SetWindowText(m_TitreFenFG);

	// Infos EDI invalide au départ
	m_OkInfoEDI = false;

	// Recup les nouveaux libellés
	m_AnneeCur.SetWindowText("/" + theApp.m_AnneeNomencl);
	MajValChamps();
	m_ChampsAliasFG.SetFocus();

	// Change en Bold la police Titre 
	// Définition des fonts
	CClientDC dc (this);
	int nHeight = -(dc.GetDeviceCaps(LOGPIXELSY)*8/72);

	m_FontTimeBold8.CreateFont(nHeight,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,
		                       OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
							   DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"MS Sans Serif");
	
	m_StaticRef4.SetFont(&m_FontTimeBold8, TRUE);

	return false;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE

}

void CFormulaireGroupe::OnOK() 
{
	CString MessErr ="";
	CString ChDate;
	int DateOk = false;
	COleDateTime DateTime;

	// Renvoi les nouveaux libellés saisis
	RenvoiValChamps();

	// Test si Annee Valide (format imposé jjmm)
	if (m_StrDateFG.GetLength() == 4)
	{
		if (m_DateParDefaut.GetLength() <= 4)
			m_DateParDefaut = m_DateParDefaut  + theApp.m_AnneeNomencl;

		ChDate = m_StrDateFG.Left(2)+ "/" + m_StrDateFG.Mid(2,2) + "/" +  m_DateParDefaut.Mid(4,4);
		DateTime.Format("%d%m%Y");
		DateOk = DateTime.ParseDateTime(ChDate,VAR_DATEVALUEONLY);
	}

	if (DateOk == false)
	{
		// Date non valide
		MessErr = MessErr + "Champs date non valide\n";
	}
	
	else if (m_StrNomRepFG != "" && m_StrAliasFG != "" && m_StrCodeFG != "")
	// Si tout est Ok on valide
	{
		CDialog::OnOK();
	}
	else if (m_StrAliasFG == "")
	{
		MessErr = MessErr + "Champs nom vide\n";
	}
	else
	{
		// si un des autres champs est vide >> remplit par défaut
		if (m_StrNomRepFG == "")
		{
			m_StrNomRepFG = m_StrAliasFG;
		}
		if (m_StrCodeFG == "")
		{
			m_StrCodeFG = m_StrAliasFG;
		}
		// date par défaut 1er janvier année cur
		if (m_StrDateFG == "")
		{
			m_StrDateFG = m_DateParDefaut;
		}
		CDialog::OnOK();
	}
	
	// Affichage message d'erreur
	if (MessErr != "")	AfxMessageBox(MessErr);

}

void CFormulaireGroupe::OnPaint() 
{
	// Coloriage du fond de ce formulaire
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_OrangeFormulaire);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	*/

	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

void CFormulaireGroupe::OnInfoedi() 
{
	// Appel Formulaire Infos EDI
	CFormulaireEDI FormulaireEDI;

	FormulaireEDI.m_TitreFenEDI = "Informations EDI / Annonceur";

	// Nom de l'annonceur en cours
    m_ChampsAliasFG.GetWindowText(m_NomTiers);
	FormulaireEDI.m_NomTiers			= m_NomTiers;
	
	// Quel mode fonctionnel (ajout ou modif)
	FormulaireEDI.m_Mode = m_Mode;

	if (FormulaireEDI.m_NomTiers != "")
	{
		// Recup Info EDI existantes
		m_TypeTiers							= "BY";         // Annonceur ici
		FormulaireEDI.m_TypeTiers			= m_TypeTiers;		
		FormulaireEDI.m_CodeTiers			= m_CodeTiers;
		m_TypeCodeTiers						= "107";		// toujours 107
		FormulaireEDI.m_TypeCodeTiers		= m_TypeCodeTiers;
		FormulaireEDI.m_AdresseTiers1		= m_AdresseTiers1;	
		FormulaireEDI.m_AdresseTiers2		= m_AdresseTiers2;	
		FormulaireEDI.m_CodePostalTiers		= m_CodePostalTiers;	
		FormulaireEDI.m_VilleTiers			= m_VilleTiers;		
		FormulaireEDI.m_CodePaysTiers		= m_CodePaysTiers;
		
		// passe les infos contact au formulaire (pour annonceur 1 seul contact)
		FormulaireEDI.m_InfoContact.m_NomContactTiers		= m_NomContactTiers;	
		FormulaireEDI.m_InfoContact.m_PrenomContactTiers	= m_PrenomContactTiers;
		FormulaireEDI.m_InfoContact.m_AdrMailContactTiers	= m_AdrMailContactTiers;
		FormulaireEDI.m_TabInfoContact.RemoveAll();
		FormulaireEDI.m_TabInfoContact.Add(FormulaireEDI.m_InfoContact);

		FormulaireEDI.m_NoTelTiers			= m_NoTelTiers;		
		FormulaireEDI.m_NoFaxTiers			= m_NoFaxTiers;
		
		if (FormulaireEDI.DoModal() == IDOK)
		{
			// Récupération des infos EDI modifiés
			m_TypeTiers				= FormulaireEDI.m_TypeTiers;		
			m_CodeTiers				= FormulaireEDI.m_CodeTiers;			
			m_TypeCodeTiers			= FormulaireEDI.m_TypeCodeTiers;		
			m_NomTiers				= FormulaireEDI.m_NomTiers;				
			m_AdresseTiers1			= FormulaireEDI.m_AdresseTiers1;	
			m_AdresseTiers2			= FormulaireEDI.m_AdresseTiers2;	
			m_CodePostalTiers		= FormulaireEDI.m_CodePostalTiers;	
			m_VilleTiers			= FormulaireEDI.m_VilleTiers;		
			m_CodePaysTiers			= FormulaireEDI.m_CodePaysTiers;
			
			m_NomContactTiers		= FormulaireEDI.m_TabInfoContact[0].m_NomContactTiers;	
			m_PrenomContactTiers	= FormulaireEDI.m_TabInfoContact[0].m_PrenomContactTiers;
			m_AdrMailContactTiers	= FormulaireEDI.m_TabInfoContact[0].m_AdrMailContactTiers;

			m_NoTelTiers			= FormulaireEDI.m_NoTelTiers;		
			m_NoFaxTiers			= FormulaireEDI.m_NoFaxTiers;
			
			// Récup validité des infos EDI
			m_OkInfoEDI = FormulaireEDI.m_OkInfoEDI;
		}
	}

	else
	{
		// Avant d'aller aux infos EDi, il faut entrer le nom de l'annonceur
		AfxMessageBox("Avant d'entrer dans les informations Tiers EDI, il faut que vous donniez un nom à cet annonceur !!!",MB_ICONINFORMATION);
	}	
}
