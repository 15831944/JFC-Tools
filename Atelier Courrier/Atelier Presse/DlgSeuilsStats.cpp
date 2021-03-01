// DlgSeuilsStats.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DlgSeuilsStats.h"


// Boîte de dialogue CDlgSeuilsStats

IMPLEMENT_DYNAMIC(CDlgSeuilsStats, CDialog)
CDlgSeuilsStats::CDlgSeuilsStats(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSeuilsStats::IDD, pParent)
{
	// Les seuils calcul moteur
	m_Avertissement = AfxGetApp()->GetProfileInt("Moteur", "Seuil_Avertissement", 200);
	m_Limite = AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100);

	// Les seuils marge d'erreur
	m_SeuilMargeErreur_Util		= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Util", 30);
	m_SeuilMargeErreur_Appli	= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Appli", 75);

	m_appColor = CATPColors::COLORCREUSET;
}

CDlgSeuilsStats::~CDlgSeuilsStats()
{
}

void CDlgSeuilsStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FD_SEUILA, m_Avertissement);
	DDX_Text(pDX, IDC_FD_SEUILL, m_Limite);
	DDX_Text(pDX, IDC_FD_SEUIL_MARGERREUR_UTIL, m_SeuilMargeErreur_Util);
	DDX_Text(pDX, IDC_FD_SEUIL_MARGERREUR_APPLI, m_SeuilMargeErreur_Appli);
	// DDX_Control(pDX, IDC_FD_SEUILA, m_EditSeuilA);
	// DDX_Control(pDX, IDC_FD_SEUIL_MARGERREUR_UTIL, m_EditMargeErreur);
	// DDX_Control(pDX, IDC_PRCT_MARGE_UTIL, m_PrctMargeUtil);
	// DDX_Control(pDX, IDC_PRCT_MARGE_APPLI, m_PrctMargeAppli);
}


BEGIN_MESSAGE_MAP(CDlgSeuilsStats, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgSeuilsStats
BOOL CDlgSeuilsStats::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	SetupControls();

	return TRUE;
}

void CDlgSeuilsStats::OnOK() 
{
	UpdateData();
	if(m_Limite < m_Avertissement && m_SeuilMargeErreur_Util > 0 && m_SeuilMargeErreur_Util <= m_SeuilMargeErreur_Appli 
		&& m_SeuilMargeErreur_Appli <= MargeErreurMax)
	{
		// Seuil moteur
		AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Avertissement", m_Avertissement);
		AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Limite", m_Limite);

		// Seuil marge erreur
		AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Marge_Erreur_Util", m_SeuilMargeErreur_Util);
		AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Marge_Erreur_Appli", m_SeuilMargeErreur_Appli);

		// on appelle le gestionnaire de base
		this->CDialog::OnOK();
	}
}

void CDlgSeuilsStats::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_FD_BK, this);
	m_bkgMargeErreur.SubclassDlgItem(IDC_FD_BK_MARGE_ERREUR, this);
	m_ttl.SubclassDlgItem(IDC_FD_TTL, this);
	m_ttlMargeErreur.SubclassDlgItem(IDC_FD_TTL_MARGE_ERREUR, this);
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	// Lable seuil calcul moteur
	m_Lbl1.SubclassDlgItem(IDC_FD_TXT, this);
	m_Lbl2.SubclassDlgItem(IDC_FD_TXT2, this);

	// Labels marge erreur
	m_Lbl_MargeErrUtil.SubclassDlgItem(IDC_FD_TXT3, this);
	m_Lbl_MargeErrAppli.SubclassDlgItem(IDC_FD_TXT4, this);

	m_PrctMargeUtil.SubclassDlgItem(IDC_PRCT_MARGE_UTIL, this);
	m_PrctMargeAppli.SubclassDlgItem(IDC_PRCT_MARGE_APPLI, this);

	m_EditSeuilA.SubclassDlgItem(IDC_FD_SEUILA, this);
	m_EditMargeErreur.SubclassDlgItem(IDC_FD_SEUIL_MARGERREUR_UTIL, this);

	////
	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(GetDlgItem(IDC_FD_SEUILA)->GetSafeHwnd());
	m_bkg.AddTopWindow(GetDlgItem(IDC_FD_SEUILL)->GetSafeHwnd());

	m_bkgMargeErreur.SetTransparent(false);
	m_bkgMargeErreur.SetSunken(true);
	m_bkgMargeErreur.AddTopWindow(m_ttlMargeErreur);
	m_bkgMargeErreur.AddTopWindow(GetDlgItem(IDC_FD_SEUIL_MARGERREUR_UTIL)->GetSafeHwnd());
	m_bkgMargeErreur.AddTopWindow(GetDlgItem(IDC_FD_SEUIL_MARGERREUR_APPLI)->GetSafeHwnd());
	m_bkgMargeErreur.AddTopWindow(GetDlgItem(IDC_PRCT_MARGE_UTIL)->GetSafeHwnd());
	m_bkgMargeErreur.AddTopWindow(GetDlgItem(IDC_PRCT_MARGE_APPLI)->GetSafeHwnd());
	
	// Ajout fenêtre label seuil moteur
	m_bkg.AddTopWindow(m_Lbl1);
	m_bkg.AddTopWindow(m_Lbl2);

	// Ajout fenêtre label seuil marge d'erreur
	m_bkgMargeErreur.AddTopWindow(m_Lbl_MargeErrUtil);
	m_bkgMargeErreur.AddTopWindow(m_Lbl_MargeErrAppli);

	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

	m_ttlMargeErreur.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlMargeErreur.SetBorder(false);

	
	SetColors();
}

void CDlgSeuilsStats::SetColors(CATPColors::APPCOLOR app)
{
	m_appColor = app;
}

void CDlgSeuilsStats::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	m_bkgMargeErreur.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	
	m_ttl.SetBkColor(CATPColors::GetColorDark(m_appColor),m_appColor,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));
	
	m_ttlMargeErreur.SetBkColor(CATPColors::GetColorDark(m_appColor),m_appColor,CATPColors::DARK_DEGRAD);
	m_ttlMargeErreur.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));

	// Labels seuil calcul moteur
	m_Lbl1.SetBkColor(CATPColors::GetColorMedium(m_appColor),m_appColor,CATPColors::MEDIUM_DEGRAD);
	m_Lbl1.SetTextColor(CATPColors::GetColorBlack());
	
	m_Lbl2.SetBkColor(CATPColors::GetColorMedium(m_appColor),m_appColor,CATPColors::MEDIUM_DEGRAD);
	m_Lbl2.SetTextColor(CATPColors::GetColorBlack());
	
	m_EditSeuilA.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_EditSeuilA.SetTextColor(CATPColors::GetColorRed());
	
	// Label seuil marge d'erreur
	m_Lbl_MargeErrUtil.SetBkColor(CATPColors::GetColorMedium(m_appColor),m_appColor,CATPColors::MEDIUM_DEGRAD);
	m_Lbl_MargeErrUtil.SetTextColor(CATPColors::GetColorBlack());
	m_Lbl_MargeErrAppli.SetBkColor(CATPColors::GetColorMedium(m_appColor),m_appColor,CATPColors::MEDIUM_DEGRAD);
	m_Lbl_MargeErrAppli.SetTextColor(CATPColors::GetColorBlack());
	m_EditMargeErreur.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_EditMargeErreur.SetTextColor(CATPColors::GetColorRed());

	m_PrctMargeUtil.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	m_PrctMargeAppli.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	
	m_bkgMargeErreur.AddTopWindow(m_Lbl_MargeErrUtil);
	m_bkgMargeErreur.AddTopWindow(m_Lbl_MargeErrAppli);

	m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

}