// FormulaireUtilTaux.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireUtilTaux.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaireUtilTaux dialog


CFormulaireUtilTaux::CFormulaireUtilTaux(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireUtilTaux::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaireUtilTaux)
	m_MotDePasseUtilTaux = _T("");
	m_NomUtilTaux = _T("");
	//}}AFX_DATA_INIT
}


void CFormulaireUtilTaux::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireUtilTaux)
	DDX_Control(pDX, IDC_MP_UTILTAUX, m_EditMPUtilTaux);
	DDX_Control(pDX, IDC_NOM_UTILTAUX, m_EditUtilTaux);
	DDX_Control(pDX, IDC_SUPPUTILTAUX, m_SuppUtilTaux);
	DDX_Control(pDX, IDC_AJOUTUTILTAUX, m_AjoutUtilTaux);
	DDX_Control(pDX, IDC_CADRE10, m_Cadre10);
	DDX_Text(pDX, IDC_MP_UTILTAUX, m_MotDePasseUtilTaux);
	DDX_Text(pDX, IDC_NOM_UTILTAUX, m_NomUtilTaux);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireUtilTaux, CDialog)
	//{{AFX_MSG_MAP(CFormulaireUtilTaux)
	ON_BN_CLICKED(IDC_AJOUTUTILTAUX, OnAjoututiltaux)
	ON_BN_CLICKED(IDC_SUPPUTILTAUX, OnSupputiltaux)
	ON_EN_CHANGE(IDC_MP_UTILTAUX, OnChangeMpUtiltaux)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireUtilTaux message handlers

BOOL CFormulaireUtilTaux::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_MajUtilTaux == true)
	{
		// Ici affichage boite de gestion utilisateur taux FO
		m_AjoutUtilTaux.EnableWindow(TRUE);
		m_SuppUtilTaux.EnableWindow(TRUE);
	}
	else
	{
		// Ici affichage demande nom et mot de passe uniquement
		m_AjoutUtilTaux.EnableWindow(FALSE);
		m_SuppUtilTaux.EnableWindow(FALSE);
	}

	// Focus sur saisie nom utilisateur
	m_EditUtilTaux.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormulaireUtilTaux::OnOK() 
{
	CString CodeMP;
	
	CDialog::OnOK();

	// Codage du mot de passe saisie
	CodeMP = CodageMP(m_MotDePasseUtilTaux);
	m_MotDePasseUtilTaux = CodeMP;

}

bool CFormulaireUtilTaux::AjouteUtilisateur(CString Nom,CString MotDePasse)
{
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString ExMotDePasse;
	
	CString KeyUtil;
	CString StrUtil;
	CString CodeMP;
	CString Str;
	
	int NoUtil;
	int NbUtilisateur;

	// Si utilisateur non présent, on le rajoute
	if (UtilisateurDejaPresent(Nom,ExMotDePasse,NoUtil) == false)
	{
		// Lecture nombre d'utilisateurs déjà présent
		GetPrivateProfileString ("Utilisateur", "NbUtil","0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
		NbUtilisateur = atoi(Buffer);

		// Ecriture nombre total utilisateurs
		Str.Format("%d",NbUtilisateur+1);
		WritePrivateProfileString ("Utilisateur", "NbUtil",Str, theApp.m_PathNomencl + "UtilTFO.ini"); 

		// Fabrique info utilisateur (nom + mot de passe)
		CodeMP = CodageMP(MotDePasse);
		// Test décodage
		//CodeMP = DecodageMP(CodeMP);

		// Ecriture infos nouvel utilisateur
		KeyUtil.Format("%s%d","Util",NbUtilisateur+1);
		StrUtil = Nom + "," + CodeMP;
		WritePrivateProfileString ("Utilisateur", KeyUtil,StrUtil, theApp.m_PathNomencl + "UtilTFO.ini"); 

		return true;
	}
	else
		return false;
	

}

bool CFormulaireUtilTaux::SupprimeUtilisateur(CString Nom,CString MotDePasse)
{
	CHAR Buffer[255];
	CString ExMotDePasse;
	CString CodeMP;
	CString Str;
	CString KeyUtil;
	CString StrUtil;
	int NoUtil;
	int NbUtilisateur;

	// Si utilisateur présent, on peut le supprimer / après vérif mot de passe
	if (UtilisateurDejaPresent(Nom,ExMotDePasse,NoUtil) == true)
	{
		// Lecture nombre d'utilisateurs déjà présent
		GetPrivateProfileString ("Utilisateur", "NbUtil","0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
		NbUtilisateur = atoi(Buffer);

		// Test si même mot de passe
		// Codage du mot de passe saisie
		CodeMP = CodageMP(MotDePasse);
		if (CodeMP.Compare(ExMotDePasse) == 0)
		{
			// Initialisation ligne utilisateur
			Str.Format("%s%d","Util",NoUtil);
			WritePrivateProfileString ("Utilisateur", Str,"", theApp.m_PathNomencl + "UtilTFO.ini"); 

			// Maj nb utilisateurs
			Str.Format("%s%d","NbUtil",NbUtilisateur-1);
			WritePrivateProfileString ("Utilisateur", "NbUtil",Str, theApp.m_PathNomencl + "UtilTFO.ini"); 

			return true;	
		}
	}

	return false;
}

// Test si utilisateur présent dans liste des utilisateurs possibles
bool CFormulaireUtilTaux::UtilisateurDejaPresent(CString Nom,CString &ExMotDePasse,int &NoUtilisateur)
{
	CString NomUtil;
	CString Str;
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString BufTmp;
	int NbUtilisateur; 
	int PosSep;
	int i;

	// Lecture nombre d'utilisateurs déjà présent
	GetPrivateProfileString ("Utilisateur", "NbUtil","0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
	NbUtilisateur = atoi(Buffer);
	
	// Aucun utilisateur présent
	if (NbUtilisateur == 0) return false; 

	// Test sur tous les utilisateurs
	for (i=1;i<=NbUtilisateur;i++)
	{
		Str.Format("%s%d","Util",i);
		GetPrivateProfileString ("Utilisateur", Str,"0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
		BufTmp = Buffer;
		PosSep = BufTmp.Find(",",0);
		if (PosSep != 0)
		{
			NomUtil = BufTmp.Left(PosSep);
			ExMotDePasse = BufTmp.Mid(PosSep+1);
			NoUtilisateur = i;
		}
		if (NomUtil.Compare(Nom) == 0) return true;
	}

	return false;
}


// Ajout d'un nouvel utilisateur mise à jour Taux fin d'ordre
void CFormulaireUtilTaux::OnAjoututiltaux() 
{
	UpdateData(true);

	if (m_MotDePasseUtilTaux.GetLength()> 6)
	{
		AfxMessageBox("Mot de passe trop long (maxi 6 caractères) !!!");
		m_MotDePasseUtilTaux = "";
		m_EditMPUtilTaux.SetFocus();
	}
	else if (m_MotDePasseUtilTaux.GetLength() == 0)
	{
		AfxMessageBox("Entrer votre mot de passe");
		m_MotDePasseUtilTaux = "";
		m_EditMPUtilTaux.SetFocus();
	}
	else if (m_NomUtilTaux.GetLength() == 0)
	{
		AfxMessageBox("Entrer votre nom utilistateur");
		m_NomUtilTaux = "";
		m_MotDePasseUtilTaux = "";
		m_EditUtilTaux.SetFocus();
	}
	else
	{
		AjouteUtilisateur(m_NomUtilTaux,m_MotDePasseUtilTaux);
		m_NomUtilTaux = "";
		m_MotDePasseUtilTaux = "";
		m_EditUtilTaux.SetFocus();
	}

	UpdateData(false);


}

// Suppression d'un utilisateur
void CFormulaireUtilTaux::OnSupputiltaux() 
{
	UpdateData(true);
	if (SupprimeUtilisateur(m_NomUtilTaux,m_MotDePasseUtilTaux) == false)
	{
		// Impossible de le supprimer nom ou mot de passe incorrect
		AfxMessageBox("Impossible de supprimer cet utilisateur\n-Nom inexistant\n-Mot de passe incorrect"); 
	}

	// Init boite de saisie
	m_NomUtilTaux ="";
	m_MotDePasseUtilTaux ="";
	m_EditUtilTaux.SetFocus();
	UpdateData(false);

}

void CFormulaireUtilTaux::OnChangeMpUtiltaux() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CFormulaireUtilTaux::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	CRect rect;

	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_VertEmeraude);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	*/
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
