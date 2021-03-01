// ATPProduit.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ATPProduit.h"
#include ".\atpproduit.h"


// Boîte de dialogue CATPProduit

IMPLEMENT_DYNAMIC(CATPProduit, CDialog)
CATPProduit::CATPProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CATPProduit::IDD, pParent)
{
}

CATPProduit::~CATPProduit()
{
}

void CATPProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CR_COMBO_GROUPE, m_ComboGroupe);
	DDX_Control(pDX, IDC_CR_COMBO_ANNONCEUR, m_ComboAnnonceur);
	DDX_Control(pDX, IDC_CR_COMBO_PRODUIT, m_ComboProduit);
}

//////////////////////////////////////////
// Lecture code groupe
JString CATPProduit::GetCodeGroupe() const
{
	// on renvoie le code groupe
	return (m_CodeGroupe);
}

//////////////////////////////////////////
// Lecture code annonceur
JString CATPProduit::GetCodeAnnonceur() const
{
	// on renvoie le code annonceur
	return (m_CodeAnnonceur);
}

//////////////////////////////////////////
// Lecture code produit
JString CATPProduit::GetCodeProduit() const
{
	// on renvoie le code produit
	return (m_CodeProduit);
}

BEGIN_MESSAGE_MAP(CATPProduit, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// Gestionnaires de messages CATPProduit

void CATPProduit::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		JUnt32 i;
		CString Txt;

		// Chargement des groupes
		m_ComboGroupe.ResetContent(); 
		for (i=0;i<10;i++)
		{
			Txt.Format("%s %d","Groupe",i);
			m_ComboGroupe.AddString(Txt); 
		}

		// Chargement des annonceurs
		m_ComboAnnonceur.ResetContent(); 
		for (i=0;i<10;i++)
		{
			Txt.Format("%s %d","Annonceur",i);
			m_ComboAnnonceur.AddString(Txt);
		}

		// Chargement des produits
		m_ComboProduit.ResetContent(); 
		for (i=0;i<10;i++)
		{
			Txt.Format("%s %d","Produit",i);
			m_ComboProduit.AddString(Txt);
		}

	}

}

BOOL CATPProduit::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupControls();

	return TRUE;
}


void CATPProduit::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_CR_FR_ANNONCEUR, this);
	m_ttl0.SubclassDlgItem(IDC_CR_TITLEGROUPE, this);
	m_ttl1.SubclassDlgItem(IDC_CR_TITLEANNONCEUR, this);
	m_ttl2.SubclassDlgItem(IDC_CR_TITLEPRODUIT, this);
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

////
	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl0);
	m_bkg.AddTopWindow(m_ttl1);
	m_bkg.AddTopWindow(m_ttl2);
	m_bkg.AddTopWindow(m_ComboGroupe);
	m_bkg.AddTopWindow(m_ComboAnnonceur);
	m_bkg.AddTopWindow(m_ComboProduit);

////
	//m_ttlFormat.SetFontName(CATPColors::GetFontLabelName());
	//m_ttlFormat.SetFontSize(CATPColors::GetFontLabelSize());
	//m_ttlFormat.SetBorder(false);

	SetColors();
}

void CATPProduit::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	m_ttl0.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_ttl0.SetTextColor(CATPColors::GetColorBlack());
	
	m_ttl1.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_ttl1.SetTextColor(CATPColors::GetColorBlack());
	
	m_ttl2.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_ttl2.SetTextColor(CATPColors::GetColorBlack());
	

	m_ComboGroupe.SetSelectionColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	m_ComboAnnonceur.SetSelectionColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));;
	m_ComboProduit.SetSelectionColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));;

	m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET, 0 );
	
	//m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );

	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET, 0 );
	//m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET), 0 );
	//m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
}


