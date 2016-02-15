// FormulaireStandard.cpp : implementation file
//

#include "stdafx.h"
#include "Nomenclature.h"
#include "FormulaireStandard.h"
#include "FormulaireEDI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaireStandard dialog


CFormulaireStandard::CFormulaireStandard(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireStandard::IDD, pParent)
{
	// Par défaut on est en mode jout
	m_Mode = 0;

	//{{AFX_DATA_INIT(CFormulaireStandard)
	//}}AFX_DATA_INIT
}


void CFormulaireStandard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireStandard)
	DDX_Control(pDX, IDC_INFOEDI, m_InfoEDI);
	DDX_Control(pDX, IDC_STATIC_REF2, m_StaticRef2);
	DDX_Control(pDX, IDC_CADRE5, m_Cadre5);
	DDX_Control(pDX, IDC_EDIT1_3, m_ChampsCodeFStd);
	DDX_Control(pDX, IDC_EDIT1_2, m_ChampsAliasFStd);
	DDX_Control(pDX, IDC_EDIT1_1, m_ChampsNomRepFStd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireStandard, CDialog)
	//{{AFX_MSG_MAP(CFormulaireStandard)
	ON_WM_PAINT()
	ON_COMMAND(ID_INFOS_APROPOSDENOMENCLATURE, OnInfosAproposdenomenclature)
	ON_BN_CLICKED(IDC_INFOEDI, OnInfoedi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireStandard message handlers


BOOL CFormulaireStandard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Infos EDI visible ou non (visible seulement pour les annonceurs)
	if (theApp.m_NiveauEDI == 0 || m_TypeAnnonceur == false)
		m_InfoEDI.ShowWindow(SW_HIDE);

	// Infos EDI invalide au départ
	m_OkInfoEDI = false;

	// Réecriture entete fenetre
	SetWindowText(m_TitreFenStd);

	// Recup les nouveaux libellés
	MajValChamps();
	m_ChampsAliasFStd.SetFocus();

	// Change en Bold la police Titre 
	// Définition des fonts
	CClientDC dc (this);
	int nHeight = -(dc.GetDeviceCaps(LOGPIXELSY)*8/72);

	m_FontTimeBold8.CreateFont(nHeight,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,
		                       OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
							   DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"MS Sans Serif");
	
	m_StaticRef2.SetFont(&m_FontTimeBold8, TRUE);

	return false;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE

}

// Affiche les valeurs de champs sur les controles dialogues
void CFormulaireStandard ::MajValChamps()
{
	m_ChampsNomRepFStd.SetWindowText(m_StrNomRepFStd);
	m_ChampsAliasFStd.SetWindowText(m_StrAliasFStd);
	m_ChampsCodeFStd.SetWindowText(m_StrCodeFStd);
}

// Recupére les valeurs controles >> variables m_Str
void CFormulaireStandard::RenvoiValChamps()
{
	m_ChampsNomRepFStd.GetWindowText(m_StrNomRepFStd);
	m_ChampsAliasFStd.GetWindowText(m_StrAliasFStd);
	m_ChampsCodeFStd.GetWindowText(m_StrCodeFStd);
}


void CFormulaireStandard::OnOK() 
{

	CString MessErr;

	// Si info EDI Obligatoire, on teste s'ils sont valides (pour annonceur uniquement)
	if (theApp.m_NiveauEDI == 2 && m_OkInfoEDI == false && m_TypeAnnonceur == true)
	{
		AfxMessageBox ("Vous devez saisir les infos EDI avant de sauver un Annonceur",MB_ICONEXCLAMATION);
		return;
	}


	// Renvoi les nouveaux libellés saisis
	RenvoiValChamps();
	
	/*
	if (m_StrNomRepFStd != "" && m_StrAliasFStd != "" && m_StrCodeFStd != "")
		CDialog::OnOK();
	else
	{
		if (m_StrNomRepFStd == "")
			MessErr = MessErr + "Champs répertoire vide\n" ;
		if (m_StrAliasFStd == "")
			MessErr = MessErr + "Champs nom vide\n";
		if (m_StrCodeFStd == "")
			MessErr = MessErr + "Champs code vide\n";

		if (MessErr != "")
			AfxMessageBox(MessErr + "\n" + "Vous devez saisir tous ces champs !!!!");
	}
	*/

	if (m_StrNomRepFStd != "" && m_StrAliasFStd != "" && m_StrCodeFStd != "")
	{
		CDialog::OnOK();
	}
	else if (m_StrAliasFStd == "")
	{
		MessErr = MessErr + "Champs nom vide\n";
	}
	else
	{
		// si un des autres champs est vide >> remplit par défaut
		if (m_StrNomRepFStd == "")
		{
			m_StrNomRepFStd = m_StrAliasFStd;
		}
		if (m_StrCodeFStd == "")
		{
			m_StrCodeFStd = m_StrAliasFStd;
		}
		CDialog::OnOK();
	}

}

void CFormulaireStandard::OnPaint() 
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

void CFormulaireStandard::OnInfosAproposdenomenclature() 
{
	// Affichage message pour superviseur Nomenclature
	
	
}

int CFormulaireStandard::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

void CFormulaireStandard::OnInfoedi() 
{
	// Appel Formulaire Infos EDI
	CFormulaireEDI FormulaireEDI;

	FormulaireEDI.m_TitreFenEDI = "Informations EDI / Annonceur";

	// Nom de l'annonceur en cours
    m_ChampsAliasFStd.GetWindowText(m_NomTiers);
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
