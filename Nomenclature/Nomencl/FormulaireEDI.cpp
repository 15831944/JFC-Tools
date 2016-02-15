// FormulaireEDI.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireEDI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaireEDI dialog


CFormulaireEDI::CFormulaireEDI(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireEDI::IDD, pParent)
{
	// à priori au départ pas d'infos EDI saisies
	m_OkInfoEDI =false;

	// Interlocuteur par défaut (tjrs le 1er)
	m_IndexContact = -1;

	// par défaut mode Ajout (=0)
	m_Mode = 0;

	//{{AFX_DATA_INIT(CFormulaireEDI)
	//}}AFX_DATA_INIT
}

// les constantes EDI
// Longueur maxi des libellés longs
const int CFormulaireEDI::m_MaxCarLongLib			= 35;
const int CFormulaireEDI::m_MaxCarSuperLongLib		= 100;

// Et les autres longueurs libellés
const int CFormulaireEDI::m_MaxCarTypeCode			= 3;
const int CFormulaireEDI::m_MaxCarCodePays			= 3;
const int CFormulaireEDI::m_MaxCarTelFax			= 20;
const int CFormulaireEDI::m_MaxCarCodePostal		= 9;

void CFormulaireEDI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireEDI)
	DDX_Control(pDX, IDC_ADD_CONTACT, m_AddContact);
	DDX_Control(pDX, IDC_CADRECONTACT, m_CadreContact);
	DDX_Control(pDX, IDC_SUIV_CONTACT, m_SuivContact);
	DDX_Control(pDX, IDC_PREC_CONTACT, m_PrecContact);
	DDX_Control(pDX, IDC_SUPP_CONTACT, m_SuppContact);
	DDX_Control(pDX, IDC_EDIT_NOFAX, m_ChampsNoFax);
	DDX_Control(pDX, IDC_EDIT_NOTEL, m_ChampsNoTel);
	DDX_Control(pDX, IDC_EDIT_VILLE, m_ChampsVille);
	DDX_Control(pDX, IDC_EDIT_PRENOMCONTACT, m_ChampsPrenomContact);
	DDX_Control(pDX, IDC_EDIT_NOMCONTACT, m_ChampsNomContact);
	DDX_Control(pDX, IDC_EDIT_CODEPOSTAL, m_ChampsCodePostal);
	DDX_Control(pDX, IDC_EDIT_ADRMAILCONTACT, m_ChampsAdrMailContact);
	DDX_Control(pDX, IDC_EDIT_ADR2, m_ChampsAdr2);
	DDX_Control(pDX, IDC_EDIT_ADR1, m_ChampsAdr1);
	DDX_Control(pDX, IDC_COMBO_PAYS, m_ComboPays);
	DDX_Control(pDX, IDC_OBLIG5, m_Oblig5);
	DDX_Control(pDX, IDC_OBLIG4, m_Oblig4);
	DDX_Control(pDX, IDC_OBLIG3, m_Oblig3);
	DDX_Control(pDX, IDC_OBLIG2, m_Oblig2);
	DDX_Control(pDX, IDC_OBLIG1, m_Oblig1);
	DDX_Control(pDX, IDC_EDIT_TYPECODE, m_ChampsTypeCode);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_ChampsType);
	DDX_Control(pDX, IDC_EDIT_NOM, m_ChampsNom);
	DDX_Control(pDX, IDC_EDIT_CODE, m_ChampsCode);
	DDX_Control(pDX, IDC_EDIT_CODE_GLN, m_ChampsCodeGLN);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affiche les valeurs de champs sur les controles dialogues
void CFormulaireEDI ::MajValChamps()
{
	CString StrVide = "";

	// Les infos type EDI
	AfficheValeurChamps(m_ChampsType,m_TypeTiers);

	// Position du séparateur &, entre code siret et code GLN
	int PosSep = m_CodeTiers.Find("&",0);

	// Récupère le code Siret à gauche du séparateur & s'il existe, sinon on prend tout
	if (PosSep >=0)
		AfficheValeurChamps(m_ChampsCode,m_CodeTiers.Left(PosSep));
	else
		AfficheValeurChamps(m_ChampsCode,m_CodeTiers);

	// Récupère le code GLN à droite du séparateur & s'il existe, sinon init à vide
	if (PosSep >= 0)
		AfficheValeurChamps(m_ChampsCodeGLN,m_CodeTiers.Mid(PosSep+1));
	else
		AfficheValeurChamps(m_ChampsCodeGLN,StrVide);

	AfficheValeurChamps(m_ChampsTypeCode,m_TypeCodeTiers);

	// Les infos générales 
	AfficheValeurChamps(m_ChampsNom,m_NomTiers);
	AfficheValeurChamps(m_ChampsAdr1,m_AdresseTiers1);
	AfficheValeurChamps(m_ChampsAdr2,m_AdresseTiers2);
	AfficheValeurChamps(m_ChampsCodePostal,m_CodePostalTiers);
	AfficheValeurChamps(m_ChampsVille,m_VilleTiers);
	
	// positionne choix du pays
	CString NomPays;
	NomPays = StrPays(m_CodePaysTiers);
	m_ComboPays.SelectString(0,NomPays);
	
	// Les champs Tél / Fax
	AfficheValeurChamps(m_ChampsNoTel,m_NoTelTiers);
	AfficheValeurChamps(m_ChampsNoFax,m_NoFaxTiers);
	
	// Les infos interlocuteurs
	if (m_NbContact > 0)
	{
		// on prend le 1er contact pour affichage
		m_IndexContact = 0;
		AfficheValeurChamps(m_ChampsNomContact,m_TabInfoContact[m_IndexContact].m_NomContactTiers);
		AfficheValeurChamps(m_ChampsPrenomContact,m_TabInfoContact[m_IndexContact].m_PrenomContactTiers);
		AfficheValeurChamps(m_ChampsAdrMailContact,m_TabInfoContact[m_IndexContact].m_AdrMailContactTiers);
	}
	else
	{
		// ici un seul contact (pour annonceur)
		m_IndexContact = 0;
		AfficheValeurChamps(m_ChampsNomContact,m_InfoContact.m_NomContactTiers);
		AfficheValeurChamps(m_ChampsPrenomContact,m_InfoContact.m_PrenomContactTiers);
		AfficheValeurChamps(m_ChampsAdrMailContact,m_InfoContact.m_AdrMailContactTiers);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////
// Recupére les valeurs controles >> variables m_Str
void CFormulaireEDI::RenvoiValChamps()
{
	CString StrVide = "";
	CString StrSiret;
	CString StrGLN;

	// Les infos type EDI
	m_ChampsType.GetWindowText(m_TypeTiers);

	// Récupère le code Siret à gauche du séparateur & s'il existe, sinon on prend tout
	m_ChampsCode.GetWindowText(StrSiret);
	
	// Récupère le code GLN à droite du séparateur & s'il existe, sinon init à vide
	m_ChampsCodeGLN.GetWindowText(StrGLN);

	// Reconstitue Siret + GLN avec séparateur
	if (StrGLN != "")
		m_CodeTiers = StrSiret + "&" + StrGLN;
	else if (StrSiret != "")
		m_CodeTiers = StrSiret;
	else
		m_CodeTiers = "";
	
	m_ChampsTypeCode.GetWindowText(m_TypeCodeTiers);

	// Les infos générales 
	m_ChampsNom.GetWindowText(m_NomTiers);
	m_ChampsAdr1.GetWindowText(m_AdresseTiers1);
	m_ChampsAdr2.GetWindowText(m_AdresseTiers2);
	m_ChampsCodePostal.GetWindowText(m_CodePostalTiers);
	m_ChampsVille.GetWindowText(m_VilleTiers);

	// Récupère abréviation pays
	m_CodePaysTiers = AbrPays();

	// Les infos Tel et Fax
	m_ChampsNoTel.GetWindowText(m_NoTelTiers);
	m_ChampsNoFax.GetWindowText(m_NoFaxTiers);
	
	// Les infos interlocuteurs
	// ATTENTION CAS DES MULTI-INTERLOCUTEURS >> Renvoie tableau (pour Régie et Centrale)
	if (m_TypeTiers != "BY")
	{
		// Récupère dernier élmt en cours
		SInfoContact InfoContact;

		// Il faut que les 3 champs soient remplies
		m_ChampsNomContact.GetWindowText(InfoContact.m_NomContactTiers);
		m_ChampsPrenomContact.GetWindowText(InfoContact.m_PrenomContactTiers);
		m_ChampsAdrMailContact.GetWindowText(InfoContact.m_AdrMailContactTiers); 

		// Si saisie correct on ajoute au tableau des contacts
		if (ChampsContactOk(InfoContact) == true )
		{
			// si rien dans tableau on l'ajoute
			if (m_IndexContact == -1 || m_IndexContact >= m_TabInfoContact.GetSize())
				// Ajout nouveau contact
				m_TabInfoContact.Add(InfoContact);
			else
				// Modifie contenu tableau
				m_TabInfoContact[m_IndexContact] = InfoContact;

		}

	}

	else
	{
		// renvoie uniquement l'élément 0
		m_TabInfoContact.RemoveAll();
		m_ChampsNomContact.GetWindowText(m_InfoContact.m_NomContactTiers);
		m_ChampsPrenomContact.GetWindowText(m_InfoContact.m_PrenomContactTiers);
		m_ChampsAdrMailContact.GetWindowText(m_InfoContact.m_AdrMailContactTiers);
		m_TabInfoContact.Add(m_InfoContact);
	}

	// Mise à jour nombre de contact total
	m_NbContact = m_TabInfoContact.GetSize();

}

///////////////////////////////////////////////////////////////////////////////////////////
// Teste la validité des informations
bool CFormulaireEDI::InformationEDIValid()
{
	if (theApp.m_NiveauEDI < 2)
	{
		// Ici on ne teste pas les champs comme obligatoire, uniquement taille et conformité de chaque champs
		return (ChampsConforme());
	}
	else
	{
		// Ici en plus on vérifié la saisie des champs obligatoires
		if (ChampsConforme() == true)
			return ChampsObligatoireOk();

		else
			return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Validité de chaque champs
bool CFormulaireEDI::ChampsConforme()
{
	CString MessErr = "";
	CString Texte;

	// Test champs code  (doit être uniquement alphanum, eventuellement espace)
	m_ChampsCode.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,false) == false)	MessErr += "Erreur champs Code\n";

	// Test champ code GLN
	m_ChampsCodeGLN.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,false) == false)	MessErr += "Erreur champs Code GLN\n";

	// Test champs adresse (espace et ponctuation possible)
	m_ChampsAdr1.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,true) == false)	MessErr += "Erreur champs adresse 1\n";
	m_ChampsAdr2.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,true) == false)	MessErr += "Erreur champs adresse 2\n";

	// Test champs code postal (uniquement numérique)
	m_ChampsCodePostal.GetWindowText(Texte);
	if (AlphaNumText(Texte,true,false,false) == false)	MessErr += "Erreur champs code postal\n";
	
	// Test champs ville
	m_ChampsCodePostal.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,true) == false)	MessErr += "Erreur champs ville\n";

	// Test Telephone (numérique, espace)
	m_ChampsNoTel.GetWindowText(Texte);
	if (AlphaNumText(Texte,true,true,false) == false)	MessErr += "Erreur champs n° téléphone\n";

	// Test Fax (numérique, espace)
	m_ChampsNoFax.GetWindowText(Texte);
	if (AlphaNumText(Texte,true,true,false) == false)	MessErr += "Erreur champs n° fax\n";


	// Test Champs Contact
	m_ChampsNomContact.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,false) == false)	MessErr += "Erreur champs nom contact\n";

	m_ChampsPrenomContact.GetWindowText(Texte);
	if (AlphaNumText(Texte,false,true,false) == false)	MessErr += "Erreur champs prénom contact\n";

	m_ChampsAdrMailContact.GetWindowText(Texte);
	if (AdrMailValid(Texte) == false)					MessErr += "Erreur champs adresse mail\n";
	
	if (MessErr == "")
		return true;
	else
	{
		MessErr = "Attention certaines infos sont invalides !!!! \n" + MessErr;
		// Afficher message erreur si champs non conforme
		AfxMessageBox(MessErr,MB_ICONEXCLAMATION);
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Test des champs obligatoires
bool CFormulaireEDI::ChampsObligatoireOk()
{
	if (theApp.m_NiveauEDI < 2)
		// pas obligatoire pour ce type d'accés EDI
		return true;
	else
	{
		// accés EDI champs obligatoires valides
		if (m_TabInfoContact.GetSize() == 0)
		{
			AfxMessageBox("Remplissez obligatoirement les champs obligatoires (*)",MB_ICONEXCLAMATION);
			return false;
		}	
		else if ((m_CodeTiers			== "") ||
			(m_NomTiers				== "") ||
			(m_TabInfoContact[0].m_NomContactTiers		== "") ||
			(m_TabInfoContact[0].m_PrenomContactTiers	== "") ||
			(m_TabInfoContact[0].m_AdrMailContactTiers	== ""))
		{
			AfxMessageBox("Remplissez obligatoirement les champs obligatoires (*)",MB_ICONEXCLAMATION);
			return false;
		}
		else
			return true;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////
// Test Champs Adresse Mail
bool CFormulaireEDI::AdrMailValid(CString &AdrMail)
{

	// Si aucun caractère, adresse mail tout simplement non rempli
	CString Texte;
	Texte = AdrMail;
	Texte.TrimLeft();
	Texte.TrimRight();

	if (Texte != "")
	{
		// Test si caractère @ existant (obligatoire pour adresse email) et en position >=1
		int PosAt = Texte.Find("@",0);
		if (PosAt < 1 ) return false;

		// Test au moins 3 caractères après
		CString AfterAt = Texte.Mid(PosAt+1);
		if (AfterAt.GetLength() < 3) return false;
			
		// Puis test si au moins caractère "."
		int PosPoint = AfterAt.Find(".",0);
		if (PosPoint < 1 || PosPoint == AfterAt.GetLength()-1) return false;

		// De plus le dernier caractère de cette partie ne doit pas être un "." ni une ponctuation (quel BOULET !!!!!)
		if ((AfterAt.Right(1) == ".")  ||
			(AfterAt.Right(1) == ",")  ||
			(AfterAt.Right(1) == "-")  ||
			(AfterAt.Right(1) == "'")  ||
			(AfterAt.Right(1) == "&")  ||
			(AfterAt.Right(1) == """") ||
			(AfterAt.Right(1) == "#")  ||
			(AfterAt.Right(1) == "{")  ||
			(AfterAt.Right(1) == "}")  ||
			(AfterAt.Right(1) == "|")  ||
			(AfterAt.Right(1) == "\\") ||
			(AfterAt.Right(1) == "(")  ||
			(AfterAt.Right(1) == ")")  ||
			(AfterAt.Right(1) == "_")  ||
			(AfterAt.Right(1) == "%")  ||
			(AfterAt.Right(1) == ":")  ||
			(AfterAt.Right(1) == ";")  ||
			(AfterAt.Right(1) == "!")  ||
			(AfterAt.Right(1) == "?"))
			return false;

		// Tout va bien, good adresse email !!! en tout cas correct syntaxiquement parlant
		return true;
	}
	return true;

}

///////////////////////////////////////////////////////////////////////////////////////////
// Routines test validité texte
bool CFormulaireEDI::AlphaNumText(CString &Texte,bool OnlyNum,bool AcceptSpace,bool AcceptPonctuation)
{

	for (int i=0;i<Texte.GetLength();i++)
	{
		// Test si caratère numérique 0 --> 9
		if ((Texte[i] >= '0') && (Texte[i] <= '9'))
			continue;

		// Test si caratère alpha A --> Z
		if ((Texte[i] >= 'A') && (Texte[i] <= 'Z') && OnlyNum == false)
			continue;

		// Test si caratère alpha a --> z
		if ((Texte[i] >= 'a') && (Texte[i] <= 'z') && OnlyNum == false)
			continue;

		// les caractères accentués spécifiques		
		if (((Texte[i] == 'é') || (Texte[i] == 'è') || 
			(Texte[i] == 'ç') || (Texte[i] == 'à') ||
			(Texte[i] == 'ù') || (Texte[i] == 'ë') || 
			(Texte[i] == 'ê') || (Texte[i] == 'â') ||
			(Texte[i] == 'ö') || (Texte[i] == 'ô')) && OnlyNum == false)
			continue;

		// Test si espace
		if ((AcceptSpace == true) && (Texte[i] == ' '))
			continue;

		// Test si ponctuation
		if ((AcceptPonctuation == true) &&  (OnlyNum == false) &&
		   ((Texte[i] == '.')  ||
		    (Texte[i] == ',')  ||
			(Texte[i] == '-')  ||
			(Texte[i] == ';')  ||
			(Texte[i] == '\\') ||
			(Texte[i] == '/')))
			continue;


		return false;
	}		
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Limitation taille texte de chaque champs de saisie
void CFormulaireEDI::InitialiseMaxChamps()
{
	// Limite codage tiers EDI
	m_ChampsType.SetLimitText(2);
	m_ChampsCode.SetLimitText(m_MaxCarLongLib);
	m_ChampsCodeGLN.SetLimitText(m_MaxCarLongLib);
	m_ChampsTypeCode.SetLimitText(m_MaxCarTypeCode);

	// Limite saisie générale
	m_ChampsNom.SetLimitText(m_MaxCarLongLib);
	m_ChampsAdr1.SetLimitText(m_MaxCarLongLib);
	m_ChampsAdr2.SetLimitText(m_MaxCarLongLib);
	m_ChampsCodePostal.SetLimitText(m_MaxCarCodePostal);
	m_ChampsVille.SetLimitText(m_MaxCarLongLib);
	m_ChampsNoTel.SetLimitText(m_MaxCarTelFax);
	m_ChampsNoFax.SetLimitText(m_MaxCarTelFax);

	// Limite saisie info contact
	m_ChampsNomContact.SetLimitText(m_MaxCarLongLib);
	m_ChampsPrenomContact.SetLimitText(m_MaxCarLongLib);
	m_ChampsAdrMailContact.SetLimitText(m_MaxCarSuperLongLib);

}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage valeur champs dans contrôle
void CFormulaireEDI::AfficheValeurChamps(CEdit &EditText, CString &Texte)
{
	if (Texte == "xxx")
		// indique champs non saisi	
		EditText.SetWindowText("");
	else
		// champs déjà saisi
		EditText.SetWindowText(Texte);
}


BEGIN_MESSAGE_MAP(CFormulaireEDI, CDialog)
	//{{AFX_MSG_MAP(CFormulaireEDI)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SUPP_CONTACT, OnSuppContact)
	ON_BN_CLICKED(IDC_PREC_CONTACT, OnPrecContact)
	ON_BN_CLICKED(IDC_SUIV_CONTACT, OnSuivContact)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_ADD_CONTACT, OnAddContact)
	ON_WM_VKEYTOITEM()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFormulaireEDI::OnBnClickedOk)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////
// CFormulaireEDI message handlers
BOOL CFormulaireEDI::OnInitDialog() 
{
	
	CDialog::OnInitDialog();

	// Réecriture entete fenetre
	SetWindowText(m_TitreFenEDI + " / " + m_NomTiers);

	// Initialise longueur des champs de saisie
	InitialiseMaxChamps();

	// Chargement des pays traités
	if (ChargePays() == false)
	{
		AfxMessageBox("Fichier pays non valide !!!",MB_ICONEXCLAMATION);
	}

	// Recup les nouveaux libellés
	MajValChamps();
	m_ChampsCode.SetFocus();
	m_ChampsCodeGLN.SetFocus();

	// Selon type de Tiers, on dévalide certains champs
	if (m_TypeTiers == "BY")
	{		
		m_ChampsNom.EnableWindow(FALSE);
		m_SuppContact.ShowWindow(FALSE);
		m_SuivContact.ShowWindow(FALSE);
		m_PrecContact.ShowWindow(FALSE);
		m_AddContact.ShowWindow(FALSE);
	}

	// Affichage N° Interlocuteur
	m_CadreContact.SetWindowText("Interlocuteur n° 1");

	// En mode modification le champs nom tiers EDI est bloqué
	if (m_Mode == 1)
		m_ChampsNom.EnableWindow(FALSE);

	// Selon type d'utilisateur Niveau EDI, on indique les champs obligatoires
	if (theApp.m_NiveauEDI < 2)
	{
		m_Oblig1.ShowWindow(SW_HIDE);
		m_Oblig2.ShowWindow(SW_HIDE);
		m_Oblig3.ShowWindow(SW_HIDE);
		m_Oblig4.ShowWindow(SW_HIDE);
		m_Oblig5.ShowWindow(SW_HIDE);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////////////////
// Validation du formulaire
void CFormulaireEDI::OnOK() 
{
	// Renvoi les nouveaux libellés saisis
	RenvoiValChamps();

	// Avant tout on vérifie la validité de tous les champs
	// On regarde également si on rend obligatoire certains champs
	if (InformationEDIValid() == true)
	{
		// Ok infos EDI valide
		m_OkInfoEDI = true;
		CDialog::OnOK();
	}
	else
	{
		m_OkInfoEDI = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Couleur fond formulaire
void CFormulaireEDI::OnPaint() 
{
	
	// Coloriage du fond de ce formulaire
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

	/*
	// Coloriage des boutons Prec <> Suiv
	CPaintDC dc2(&m_PrecContact); 
	GetClientRect(&rect);
	dc2.FillRect(rect,&fond);	

	CPaintDC dc3(&m_SuivContact); 
	GetClientRect(&rect);
	dc3.FillRect(rect,&fond);	
	*/

}

///////////////////////////////////////////////////////////////////////////////////////////
// Chargement fichier pays
bool CFormulaireEDI::ChargePays()
{
	CString FicPays;
	CString Ligne;
	CPWFile File;
	int NbPays;
	int PosSep;
	int i;
	SInfoPays InfoPays;
	
	// Nom du fichier pays
	FicPays = theApp.m_PathNomencl + "AbrPays.tbl"; 
	
	if (File.OpenFile(FicPays))
	{
		// Ligne commentaire
		File.ReadString(&Ligne);

		// Ligne info nb pays
		File.ReadString(&Ligne);
		NbPays = atoi(Ligne.Mid(1));

		// On charge tous les pays dans le tableau 
		TInfoPays.RemoveAll();
		for (i=0;i<NbPays;i++)
		{
			// Lecture ligne nom pays,abr pays
			File.ReadString(&Ligne);

			PosSep = Ligne.Find(",",0);
			if (PosSep != 0)
			{
				InfoPays.StrPays = Ligne.Left(PosSep);
				InfoPays.AbrPays = Ligne.Mid(PosSep+1);
				TInfoPays.Add(InfoPays);	
			}
			else
				return false;

		}

		// 
		File.CloseFile();	
	}
	
	else
	{
		// pas de fichier pays , on met uniquement la france par défaut
		TInfoPays.RemoveAll();
		InfoPays.StrPays = "FRANCE";
		InfoPays.AbrPays = "FRA";
		TInfoPays.Add(InfoPays);	

		// Annonce fichier pays manquant
		AfxMessageBox("Fichier Pays <AbrPays.tbl> manquant",MB_ICONEXCLAMATION);
	}

	// Chargement du combo-box pays
	m_ComboPays.ResetContent();
	for (i=0;i<TInfoPays.GetSize();i++)
	{
		m_ComboPays.AddString(TInfoPays[i].StrPays); 
	}

	// par défaut on mettra le 1er pays de la liste
	m_ComboPays.SelectString(0,TInfoPays[0].StrPays);

	return true;


}

///////////////////////////////////////////////////////////////////////////////////////////
// Récupère Nom pays sélectionner via AbrPays
CString CFormulaireEDI::StrPays(CString &AbrPays)
{
	// Recherche abréviation pays dans tab des pays chargés au départ
	for (int i=0;i<TInfoPays.GetSize();i++)
	{
		if (TInfoPays[i].AbrPays == AbrPays)
			return TInfoPays[i].StrPays;
	}		

	// ici problème, pays non trouvé
	return "";
}

///////////////////////////////////////////////////////////////////////////////////////////
// Récupère abréviation pays
CString CFormulaireEDI::AbrPays()
{
	CString SelPays;
	int SelIndex;

	SelIndex = m_ComboPays.GetCurSel();
	m_ComboPays.GetLBText(SelIndex,SelPays);

	for (int i=0;i<TInfoPays.GetSize();i++)
	{
		if (TInfoPays[i].StrPays == SelPays)
			return TInfoPays[i].AbrPays;
	}		

	// ici problème, pays non trouvé
	return "xxx";
}

///////////////////////////////////////////////////////////////////////////////////////////
// Ajout d'un contact Utilisateur
void CFormulaireEDI::OnAddContact() 
{
	
	SInfoContact InfoContact;

	// Il faut que les 3 champs soient remplies
    m_ChampsNomContact.GetWindowText(InfoContact.m_NomContactTiers);
	m_ChampsPrenomContact.GetWindowText(InfoContact.m_PrenomContactTiers);
	m_ChampsAdrMailContact.GetWindowText(InfoContact.m_AdrMailContactTiers); 
	
	// Si saisie correct on ajoute au tableau des contacts
	if (ChampsContactOk(InfoContact) == true )
	{
		m_TabInfoContact.Add(InfoContact);
		m_NbContact = m_TabInfoContact.GetSize();
	}

	// Maj Info N° Interlocuteur
	MajNoInterlocuteur();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Suppression d'un contact (pour Régie ou Centrale Achat)
void CFormulaireEDI::OnSuppContact() 
{
	// Suppression de l'index courant contact utilisateur
	if (m_NbContact != 0 && m_IndexContact >=0 && m_IndexContact < m_TabInfoContact.GetSize())
	{
		// enlève élément tab
		m_TabInfoContact.RemoveAt(m_IndexContact,1);
		m_NbContact--; 
		// passe à index 0 ou index juste avant
		if (m_IndexContact > 0) m_IndexContact--;

		// efface contenu des champs contacts
		m_ChampsNomContact.SetWindowText(""); 
		m_ChampsPrenomContact.SetWindowText(""); 
		m_ChampsAdrMailContact.SetWindowText(""); 

		// s'il existe un autre contact, on l'affiche
		if (m_NbContact > 0)
		{
			// on prend le contact nvlle index 
			m_ChampsNomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_NomContactTiers); 
			m_ChampsPrenomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_PrenomContactTiers); 
			m_ChampsAdrMailContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_AdrMailContactTiers); 
		}

	}

	// Focus sur champs nom interlocuteur courant
	m_ChampsNomContact.SetFocus(); 

	// Maj Info N° Interlocuteur
	MajNoInterlocuteur();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Afficher contact précédent (pour Régie et Centrale Achat)
void CFormulaireEDI::OnPrecContact() 
{
	// passe au contact utilisateur précédent
	if (m_IndexContact > 0)
	{
		
		// Si saisie correct on ajoute au tableau des contacts
		SInfoContact InfoContact;
		m_ChampsNomContact.GetWindowText(InfoContact.m_NomContactTiers);
		m_ChampsPrenomContact.GetWindowText(InfoContact.m_PrenomContactTiers);
		m_ChampsAdrMailContact.GetWindowText(InfoContact.m_AdrMailContactTiers); 
		if (ChampsContactOk(InfoContact) == true )
		{
			// si rien dans tableau on l'ajoute
			if (m_IndexContact >= m_TabInfoContact.GetSize())
			{
				// Ajout nouveau contact
				m_TabInfoContact.Add(InfoContact);
				m_NbContact = m_TabInfoContact.GetSize();		
			}
			else
				// Modifie contenu tableau
				m_TabInfoContact[m_IndexContact] = InfoContact;

			
		}
		else
		{
			if (InfoContact.m_NomContactTiers != "" || InfoContact.m_PrenomContactTiers != "" || InfoContact.m_AdrMailContactTiers != "")
				AfxMessageBox("Erreur saisie utilisateur",MB_ICONEXCLAMATION);
		}

		// affiche utilisateur index précèdent
		m_IndexContact--;
		m_ChampsNomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_NomContactTiers); 
		m_ChampsPrenomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_PrenomContactTiers); 
		m_ChampsAdrMailContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_AdrMailContactTiers); 
	}

	// Maj Info N° Interlocuteur
	MajNoInterlocuteur();

	// Focus sur champs nom interlocuteur courant
	m_ChampsNomContact.SetFocus(); 
}

///////////////////////////////////////////////////////////////////////////////////////////
// Afficher contact suivant (pour Régie et Centrale Achat)
void CFormulaireEDI::OnSuivContact() 
{
	// passe au contact utilisateur suivant
	if (m_IndexContact < m_TabInfoContact.GetSize()-1)
	{
		// affiche utilisateur index suivant
		m_IndexContact++;
		m_ChampsNomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_NomContactTiers); 
		m_ChampsPrenomContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_PrenomContactTiers); 
		m_ChampsAdrMailContact.SetWindowText(m_TabInfoContact[m_IndexContact].m_AdrMailContactTiers); 
	}
	else
	{
		// Ajoute élmt courant
		SInfoContact InfoContact;

		// Il faut que les 3 champs soient remplies
		m_ChampsNomContact.GetWindowText(InfoContact.m_NomContactTiers);
		m_ChampsPrenomContact.GetWindowText(InfoContact.m_PrenomContactTiers);
		m_ChampsAdrMailContact.GetWindowText(InfoContact.m_AdrMailContactTiers); 

		// Si saisie correct on ajoute au tableau des contacts
		if (ChampsContactOk(InfoContact) == true )
		{
			// si rien dans tableau on l'ajoute
			if (m_IndexContact == -1 || m_IndexContact >= m_TabInfoContact.GetSize())
			{
				// Ajout nouveau contact
				m_TabInfoContact.Add(InfoContact);
				if (m_IndexContact == -1) m_IndexContact++;
			}
			else
				// Modifie contenu tableau
				m_TabInfoContact[m_IndexContact] = InfoContact;

			m_NbContact = m_TabInfoContact.GetSize();
		
			// Affiche champs vierge nouveau contact
			m_IndexContact++;
			m_ChampsNomContact.SetWindowText("");
			m_ChampsPrenomContact.SetWindowText("");
			m_ChampsAdrMailContact.SetWindowText("");
		}
		else
			if (InfoContact.m_NomContactTiers != "" || InfoContact.m_PrenomContactTiers != "" || InfoContact.m_AdrMailContactTiers != "")
				AfxMessageBox("Erreur saisie utilisateur",MB_ICONEXCLAMATION);
	}

	// Maj Info N° Interlocuteur
	MajNoInterlocuteur();

	// Focus sur champs nom interlocuteur courant
	m_ChampsNomContact.SetFocus(); 
}

///////////////////////////////////////////////////////////////////////////////////////////
// Maj Info N° Interlocuteur
void CFormulaireEDI::MajNoInterlocuteur()
{
	if (m_IndexContact >=0)
	{
		CString NoInterloc;
		NoInterloc.Format("%s%d","Interlocuteur n° ",m_IndexContact+1);
		m_CadreContact.SetWindowText(NoInterloc);
	}
}	

///////////////////////////////////////////////////////////////////////////////////////////
// Gestion touches clavier
void CFormulaireEDI::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	if ((nChar != 27) && (nChar != 13))

		// Traitement touche clavier
		CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	else
		// On reste dans la boite de dialogue
		return;
}


///////////////////////////////////////////////////////////////////////////////////////////
// Test champs contact utilisateur
bool CFormulaireEDI::ChampsContactOk(SInfoContact &InfoContact)
{
	if ((InfoContact.m_NomContactTiers     != "") &&
	    (InfoContact.m_PrenomContactTiers  != "") &&
	    (AdrMailValid(InfoContact.m_AdrMailContactTiers) == true))
		return true;
	else
		return false;
}



void CFormulaireEDI::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
