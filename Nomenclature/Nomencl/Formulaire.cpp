// Formulaire.cpp : implementation file
//

#include "stdafx.h"
#include "Nomenclature.h"
#include "Formulaire.h"
#include "FormulaireEDI.h"
#include "Noeud.h"
//#include "NomenclatureView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaire dialog


CFormulaire::CFormulaire(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaire::IDD, pParent)
{
	// Par défaut on est en mode ajout
	m_Mode = 0;

	//{{AFX_DATA_INIT(CFormulaire)
	//}}AFX_DATA_INIT
}


void CFormulaire::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaire)
	DDX_Control(pDX, IDC_INFOEDI, m_InfoEDI);
	DDX_Control(pDX, IDC_STATIC2_ANNEECOUR, m_AnneeCur);
	DDX_Control(pDX, IDC_EDIT2_4, m_ChampsDateFP);
	DDX_Control(pDX, IDC_COMBOSECTEUR2, m_ComboSecteur);
	DDX_Control(pDX, IDC_COMBOFAMILLE2, m_ComboFamille);
	DDX_Control(pDX, IDC_STATIC_REF, m_StaticRef);
	DDX_Control(pDX, IDC_CADRE4, m_Cadre4);
	DDX_Control(pDX, IDC_EDIT2_3, m_ChampsCodeFP);
	DDX_Control(pDX, IDC_EDIT2_2, m_ChampsAliasFP);
	DDX_Control(pDX, IDC_EDIT2_1, m_ChampsNomRepFP);
	DDX_Control(pDX, IDC_COMBOMARQUE2, m_ComboMarque);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaire, CDialog)
	//{{AFX_MSG_MAP(CFormulaire)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBOFAMILLE2, OnSelchangeCombofamille2)
	ON_BN_CLICKED(IDC_INFOEDI, OnInfoedi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaire message handlers


void CFormulaire::OnOK() 
{
	CString MessErr;
	bool DateOk = true;
	CString ChDate = "";
	COleDateTime DateTime;

	// Si info EDI Obligatoire, on teste s'ils sont valides (pour annonceur uniquement)
	if (theApp.m_NiveauEDI == 2 && m_OkInfoEDI == false && m_TypeAnnonceur == true)
	{
		AfxMessageBox ("Vous devez saisir les infos EDI avant de sauver un Annonceur",MB_ICONEXCLAMATION);
		return;
	}
	
	// Renvoi les valeurs des champs
	RenvoiValChamps();

	if (m_AnnonceurSansProduit == true)
	{
		// Test si Annee Valide (format imposé jjmm)
		if (m_StrDateFP.GetLength() == 4)
		{
			if (m_DateParDefaut.GetLength() <= 4)
				m_DateParDefaut = m_DateParDefaut + theApp.m_AnneeNomencl;
			ChDate = m_StrDateFP.Left(2)+ "/" + m_StrDateFP.Mid(2,2) + "/" +  m_DateParDefaut.Mid(4,4);
			DateTime.Format("%d%m%Y");
			DateOk = DateTime.ParseDateTime(ChDate,VAR_DATEVALUEONLY);
		}

	}

	// date par défaut 1er janvier année cur
	if (m_StrDateFP == "")
	{
		m_StrDateFP = m_DateParDefaut;
	}
	
	if (DateOk == false)
		// Date non valide
		MessErr = MessErr + "Champs date non valide\n";

	else if (m_StrNomRepFP != "" && m_StrAliasFP != "" && m_StrCodeFP != "" && m_StrMarqueFP != "" && m_StrFamilleFP != "" && m_StrSecteurFP != "")
	{
		CDialog::OnOK();
	}
	else if (m_StrAliasFP == "" || m_StrFamilleFP == "" || m_StrSecteurFP == "")
	{
		if (m_StrAliasFP == "")
			MessErr = MessErr + "Champs nom vide\n";
		/*
		/Marque non saisie possible
		 if (m_StrMarqueFP == "")
			MessErr = MessErr + "Champs marque produit vide\n";
		*/

		if (m_StrFamilleFP == "")
			MessErr = MessErr + "Champs famille produit vide\n";

		if (m_StrSecteurFP == "")
			MessErr = MessErr + "Champs famille/secteur produit vide\n";
	}

	else
	{
		// si un des autres champs est vide >> remplit par défaut
		if (m_StrNomRepFP == "")
		{
			m_StrNomRepFP = m_StrAliasFP;
		}
		if (m_StrCodeFP == "")
		{
			m_StrCodeFP = m_StrAliasFP;
		}
		CDialog::OnOK();
	}


	if (MessErr != "")
		AfxMessageBox(MessErr);

}

BOOL CFormulaire::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Infos EDI visible ou non (visible seulement pour les annonceurs)
	if (theApp.m_NiveauEDI == 0 || m_TypeAnnonceur == false)
		m_InfoEDI.ShowWindow(SW_HIDE);

	// Infos EDI invalide au départ
	m_OkInfoEDI = false;

	// Réecriture entete fenetre
	SetWindowText(m_TitreFenFP);

	// Recup les nouveaux libellés
	m_AnneeCur.SetWindowText("/" + theApp.m_AnneeNomencl);
	MajValChamps();
	m_ChampsAliasFP.SetFocus();

	// Change en Bold la police Titre 
	// Définition des fonts
	CClientDC dc (this);
	int nHeight = -(dc.GetDeviceCaps(LOGPIXELSY)*8/72);

	m_FontTimeBold8.CreateFont(nHeight,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,
		                       OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
							   DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"MS Sans Serif");
	
	m_StaticRef.SetFont(&m_FontTimeBold8, TRUE);


	return false;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

// Affiche les valeurs de champs sur les controles dialogues
void CFormulaire ::MajValChamps()
{
	int i;

	// les champs libelles
	m_ChampsNomRepFP.SetWindowText(m_StrNomRepFP);
	m_ChampsAliasFP.SetWindowText(m_StrAliasFP);
	m_ChampsCodeFP.SetWindowText(m_StrCodeFP);

	if (m_AnnonceurIsole == true)
	{
		m_ChampsDateFP.SetWindowText(m_StrDateFP.Left(4));
		m_DateParDefaut = m_StrDateFP;
	}
	else
	{
		m_StrDateFP = "";
		m_ChampsDateFP.SetWindowText(m_StrDateFP);
	}
	
	// maj combo box avec liste des marques disponibles
	m_AnnonceurSansProduit = false;
	if (m_TabMarque.GetSize() == 0)
	{
		// Ici pas de marque à séléctionner / forcément un annonceur sans produit
		m_ComboMarque.EnableWindow(FALSE);
		m_AnnonceurSansProduit =true;
	}
	else
	{
		m_ComboMarque.ResetContent();
		for (i= 0;i<m_TabMarque.GetSize();i++)
			m_ComboMarque.AddString(m_TabMarque[i]);

		m_ComboMarque.SelectString(-1,m_StrMarqueFP);
	}

	// Si Annonceur sans produit rend EditBox Date Exercice active
	if (m_AnnonceurSansProduit == true && m_AnnonceurIsole == true)
	{
		m_DateParDefaut = m_StrDateFP;
		m_ChampsDateFP.EnableWindow(TRUE);
	}
	else
		m_ChampsDateFP.EnableWindow(FALSE);


	// maj combo box avec liste des familles disponibles
	m_ComboFamille.ResetContent();
	for (i= 0;i<m_TabFamille.GetSize();i++)
		m_ComboFamille.AddString(m_TabFamille[i]);

	m_ComboFamille.SelectString(-1,m_StrFamilleFP);

	// rien dans la liste combo secteurs au début
	RemplirComboSecteur(m_StrFamilleFP);
	m_ComboSecteur.SelectString(-1,m_StrSecteurFP);


}

// Recupére les valeurs controles >> variables m_Str
void CFormulaire::RenvoiValChamps()
{
	// Recup les libelles
	m_ChampsNomRepFP.GetWindowText(m_StrNomRepFP);
	m_ChampsAliasFP.GetWindowText(m_StrAliasFP);
	m_ChampsCodeFP.GetWindowText(m_StrCodeFP);

	// Recup la marque saisie dans combo
	m_ComboMarque.GetWindowText(m_StrMarqueFP);

	// Recup la famille saisie dans combo
	m_ComboFamille.GetWindowText(m_StrFamilleFP);

	// Recup le secteur saisie dans combo
	m_ComboSecteur.GetWindowText(m_StrSecteurFP);

	if (m_AnnonceurSansProduit == true && m_AnnonceurIsole == true)
		m_ChampsDateFP.GetWindowText(m_StrDateFP);

}


// Remplissage liste secteurs via Famille courante (pour les produits)
void CFormulaire::RemplirComboSecteur(CString Famille)
{

	m_ComboSecteur.ResetContent();

	if (Famille != "" && m_pNoeudFamille->m_pNoeud.GetSize() > 0 )
	{
		for (int i=0;i<m_pNoeudFamille->m_pNoeud.GetSize();i++)
		{
			if (m_pNoeudFamille->m_pNoeud[i]->m_Alias == Famille)
			{
				if (m_pNoeudFamille->m_pNoeud[i]->m_pNoeud.GetSize() > 0 )
				{
					for (int j=0;j<m_pNoeudFamille->m_pNoeud[i]->m_pNoeud.GetSize();j++)
					{
						m_ComboSecteur.AddString(m_pNoeudFamille->m_pNoeud[i]->m_pNoeud[j]->m_Alias);					
					}
				}
				break;
			}
		}
	}
}

void CFormulaire::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
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

void CFormulaire::OnSelchangeCombofamille2() 
{
	CString NomFamilleCur;

	// Sélection d'une famille , mise à jour de la liste des secteurs associés
	m_ComboFamille.GetWindowText(NomFamilleCur);
	RemplirComboSecteur(NomFamilleCur);
	if (m_ComboSecteur.GetCount() == 1)
	{
		// Sélection d'office du secteur unique
		m_ComboSecteur.SetCurSel(0);
	}
}


void CFormulaire::OnInfoedi() 
{
	// Appel Formulaire Infos EDI
	CFormulaireEDI FormulaireEDI;

	FormulaireEDI.m_TitreFenEDI = "Informations EDI / Annonceur";

	// Nom de l'annonceur en cours
    m_ChampsAliasFP.GetWindowText(m_NomTiers);
	FormulaireEDI.m_NomTiers			= m_NomTiers;
	
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

		// Recup les infos contact annonceur
		FormulaireEDI.m_TabInfoContact.RemoveAll();
		FormulaireEDI.m_InfoContact.m_NomContactTiers		= m_NomContactTiers;
		FormulaireEDI.m_InfoContact.m_PrenomContactTiers	= m_PrenomContactTiers;
		FormulaireEDI.m_InfoContact.m_AdrMailContactTiers	= m_AdrMailContactTiers;
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
