// DlgParams.cpp : implementation file
//

#include "stdafx.h"
#include "DlgParams.h"
#include "AnalyseurDoc.h"
#include "AnalyseurEnums.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UNIVERS_STYLES		( TSTYLE_UNIVERSEX |  TSTYLE_NICHEX |  TSTYLE_CONTRENICHEX |  TSTYLE_ITEMMOYENX |  TSTYLE_EXCLUSIVE |  TSTYLE_TERRAINCOL |  STYLEBASE100UNIVERS |  TSTYLE_GRANDEURCOL)
#define NICHE_STYLES		( TSTYLE_NICHEX |  TSTYLE_ITEMMOYENX |  TSTYLE_EXCLUSIVE |  TSTYLE_TERRAINCOL |  TSTYLE_GRANDEURCOL)

// CDlgParams dialog(
IMPLEMENT_DYNAMIC(CDlgParams, CDialog)
CDlgParams::CDlgParams(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParams::IDD, pParent)
{
	m_TableStyle = (TSTYLE_TERRAINCOL | TSTYLE_UNIVERSEX);
	m_bCible = false;
	m_pDoc = 0;
}

CDlgParams::~CDlgParams()
{
}

void	CDlgParams::SetParams(CAnalyseurDoc *pDoc)
{	
	m_pDoc			= pDoc;
	m_bCible		= (m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES);  
    m_TableStyle	= m_pDoc->Get_TableStyle();
}

void CDlgParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgParams, CDialog)
	ON_BN_CLICKED(IDC_AN_BASE_UNIVERS,			OnBnClickedAnBaseUnivers)
	ON_BN_CLICKED(IDC_AN_BASE_NICHE,			OnBnClickedAnBaseNiche)
	ON_BN_CLICKED(IDC_AN_OPTION_NICHE,			OnBnClickedAnOptionNicheL)
	ON_BN_CLICKED(IDC_AN_OPTION_CONTRENICHE,	OnBnClickedAnOptionContrenicheL)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMMOYEN,		OnBnClickedAnOptionItemmoyenL)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMS,			OnBnClickedAnOptionItemsL)
	ON_BN_CLICKED(IDC_AN_OPTION_NICHE2,			OnBnClickedAnOptionNicheC)
	ON_BN_CLICKED(IDC_AN_OPTION_CONTRENICHE2,	OnBnClickedAnOptionContrenicheC)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMMOYEN2,		OnBnClickedAnOptionItemmoyenC)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMS2,			OnBnClickedAnOptionItemsC)
	ON_BN_CLICKED(IDC_AN_OPTION_NICHE3,			OnBnClickedAnOptionNicheP)
	ON_BN_CLICKED(IDC_AN_OPTION_CONTRENICHE3,	OnBnClickedAnOptionContrenicheP)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMMOYEN3,		OnBnClickedAnOptionItemmoyenP)
	ON_BN_CLICKED(IDC_AN_OPTION_ITEMS3,			OnBnClickedAnOptionItemsP)
	ON_BN_CLICKED(IDC_AN_GRANDEUR_LIGNE,		OnBnClickedAnGrandeurLigne)
	ON_BN_CLICKED(IDC_AN_GRANDEUR_COLONNE,		OnBnClickedAnGrandeurColonne)
	ON_BN_CLICKED(IDC_AN_TERRAIN_LIGNE,			OnBnClickedAnTerrainLigne)
	ON_BN_CLICKED(IDC_AN_TERRAIN_COLONNE,		OnBnClickedAnTerrainColonne)
	ON_BN_CLICKED(IDC_AN_SURLIGNAGE_PAGE,		OnBnClickedAnSurlignagePage)
	ON_BN_CLICKED(IDC_AN_SURLIGNAGE_GLOBAL,		OnBnClickedAnSurlignageGlobal)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDlgParams::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupControls();
	SetControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParams::SetupControls()
{
	m_But_OK							.SubclassDlgItem(IDOK,this);
	m_But_CANCEL						.SubclassDlgItem(IDCANCEL,this);
	
	m_Box1								.SubclassDlgItem(IDC_AN_STATIC_BASE100,this);
	m_Label_Base100						.SubclassDlgItem(IDC_AN_STATIC_TTLBASE100,this);
	m_Radio_Base_Univers				.SubclassDlgItem(IDC_AN_BASE_UNIVERS ,this);
	m_Radio_Base_Niche					.SubclassDlgItem(IDC_AN_BASE_NICHE ,this);


	m_Box2								.SubclassDlgItem(IDC_AN_STATIC_OPTIONS,this);
	m_Label_OptionsL					.SubclassDlgItem(IDC_AN_STATIC_TTLOPTIONS ,this);
	m_Check_Option_NicheL				.SubclassDlgItem(IDC_AN_OPTION_NICHE ,this);
	m_Check_Option_ContreNicheL			.SubclassDlgItem(IDC_AN_OPTION_CONTRENICHE ,this);
	m_Check_Option_ItemMoyenL			.SubclassDlgItem(IDC_AN_OPTION_ITEMMOYEN ,this);
	m_Check_Option_ItemsL				.SubclassDlgItem(IDC_AN_OPTION_ITEMS ,this);
	m_Label_OptionsC					.SubclassDlgItem(IDC_AN_STATIC_TTLOPTIONS2 ,this);
	m_Check_Option_NicheC				.SubclassDlgItem(IDC_AN_OPTION_NICHE2 ,this);
	m_Check_Option_ContreNicheC			.SubclassDlgItem(IDC_AN_OPTION_CONTRENICHE2 ,this);
	m_Check_Option_ItemMoyenC			.SubclassDlgItem(IDC_AN_OPTION_ITEMMOYEN2 ,this);
	m_Check_Option_ItemsC				.SubclassDlgItem(IDC_AN_OPTION_ITEMS2 ,this);
	m_Label_OptionsP					.SubclassDlgItem(IDC_AN_STATIC_TTLOPTIONS3 ,this);
	m_Check_Option_NicheP				.SubclassDlgItem(IDC_AN_OPTION_NICHE3 ,this);
	m_Check_Option_ContreNicheP			.SubclassDlgItem(IDC_AN_OPTION_CONTRENICHE3 ,this);
	m_Check_Option_ItemMoyenP			.SubclassDlgItem(IDC_AN_OPTION_ITEMMOYEN3 ,this);
	m_Check_Option_ItemsP				.SubclassDlgItem(IDC_AN_OPTION_ITEMS3 ,this);

	m_Label_Grandeurs					.SubclassDlgItem(IDC_AN_STATIC_TTLGRANDEURS ,this);
	m_Radio_Grandeurs_Ligne				.SubclassDlgItem(IDC_AN_GRANDEUR_LIGNE ,this);
	m_Radio_Grandeurs_Colonne			.SubclassDlgItem(IDC_AN_GRANDEUR_COLONNE ,this);

	m_Label_Terrains					.SubclassDlgItem(IDC_AN_STATIC_TTLTERRAINS ,this);
	m_Radio_Terrains_Ligne				.SubclassDlgItem(IDC_AN_TERRAIN_LIGNE ,this);
	m_Radio_Terrains_Colonne			.SubclassDlgItem(IDC_AN_TERRAIN_COLONNE ,this);

	m_Label_Surlignage					.SubclassDlgItem(IDC_AN_STATIC_TTLSURLIGNAGE ,this);
	m_Radio_Surlignage_Page				.SubclassDlgItem(IDC_AN_SURLIGNAGE_PAGE  ,this);
	m_Radio_Surlignage_Global			.SubclassDlgItem(IDC_AN_SURLIGNAGE_GLOBAL ,this);

	CString Txt;

	if(m_bCible)
		Txt.LoadString(IDS_AN_LABEL_CIBLES);
	else
		Txt.LoadString(IDS_AN_LABEL_UNIVERS);
	m_Radio_Base_Univers				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Base_Univers				.SetTextAlignement(DT_LEFT);
	m_Radio_Base_Univers				.DrawBorder(false); 
	m_Radio_Base_Univers				.SetWindowText(Txt); 

	if(m_bCible)
		Txt.LoadString(IDS_AN_NICHECIBLE);
	else
		Txt.LoadString(IDS_AN_NICHE);
	m_Radio_Base_Niche					.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Base_Niche					.SetTextAlignement(DT_LEFT);
	m_Radio_Base_Niche					.DrawBorder(false);
	m_Radio_Base_Niche					.SetWindowText(Txt); 

	m_Check_Option_NicheL				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_NicheL				.SetTextAlignement(DT_LEFT);
	m_Check_Option_NicheL				.DrawBorder(false); 
	m_Check_Option_NicheC				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_NicheC				.SetTextAlignement(DT_LEFT);
	m_Check_Option_NicheC				.DrawBorder(false); 
	m_Check_Option_NicheP				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_NicheP				.SetTextAlignement(DT_LEFT);
	m_Check_Option_NicheP				.DrawBorder(false); 
	m_Check_Option_ContreNicheL			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ContreNicheL			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ContreNicheL			.DrawBorder(false); 
	m_Check_Option_ContreNicheC			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ContreNicheC			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ContreNicheC			.DrawBorder(false); 
	m_Check_Option_ContreNicheP			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ContreNicheP			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ContreNicheP			.DrawBorder(false); 
	m_Check_Option_ItemMoyenL			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemMoyenL			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemMoyenL			.DrawBorder(false); 
	m_Check_Option_ItemMoyenC			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemMoyenC			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemMoyenC			.DrawBorder(false); 
	m_Check_Option_ItemMoyenP			.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemMoyenP			.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemMoyenP			.DrawBorder(false); 
	m_Check_Option_ItemsL				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemsL				.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemsL				.DrawBorder(false); 
	m_Check_Option_ItemsC				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemsC				.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemsC				.DrawBorder(false); 
	m_Check_Option_ItemsP				.SetIcon(IDI_AN_CHECKON,IDI_AN_CHECKOFF);  
	m_Check_Option_ItemsP				.SetTextAlignement(DT_LEFT);
	m_Check_Option_ItemsP				.DrawBorder(false); 

	m_Radio_Grandeurs_Ligne				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Grandeurs_Ligne				.SetTextAlignement(DT_LEFT);
	m_Radio_Grandeurs_Ligne				.DrawBorder(false); 
	m_Radio_Grandeurs_Colonne			.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Grandeurs_Colonne			.SetTextAlignement(DT_LEFT);
	m_Radio_Grandeurs_Colonne			.DrawBorder(false); 

	m_Radio_Terrains_Ligne				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Terrains_Ligne				.SetTextAlignement(DT_LEFT);
	m_Radio_Terrains_Ligne				.DrawBorder(false); 
	m_Radio_Terrains_Colonne			.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Terrains_Colonne			.SetTextAlignement(DT_LEFT);
	m_Radio_Terrains_Colonne			.DrawBorder(false); 

	m_Radio_Surlignage_Page				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Surlignage_Page				.SetTextAlignement(DT_LEFT);
	m_Radio_Surlignage_Page				.DrawBorder(false); 
	m_Radio_Surlignage_Global			.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Radio_Surlignage_Global			.SetTextAlignement(DT_LEFT);
	m_Radio_Surlignage_Global			.DrawBorder(false); 

	m_Label_OptionsL.SetFontName(CATPColors::GetFontStaticName());
	m_Label_OptionsL.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_OptionsL.SetBorder(false);

	m_Label_OptionsC.SetFontName(CATPColors::GetFontStaticName());
	m_Label_OptionsC.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_OptionsC.SetBorder(false);

	m_Label_OptionsP.SetFontName(CATPColors::GetFontStaticName());
	m_Label_OptionsP.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_OptionsP.SetBorder(false);

	m_Label_Grandeurs.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Grandeurs.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Grandeurs.SetBorder(false);

	m_Label_Terrains.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Terrains.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Terrains.SetBorder(false);

	m_Label_Surlignage.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Surlignage.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Surlignage.SetBorder(false);

	m_Box1.Reset();
	m_Box1.SetTransparent(false);
	m_Box1.SetSunken(true);
	m_Box1.AddTopWindow(m_Label_Base100);
	m_Box1.AddTopWindow(m_Radio_Base_Univers);
	m_Box1.AddTopWindow(m_Radio_Base_Niche);

	m_Box2.Reset();
	m_Box2.SetTransparent(false);
	m_Box2.SetSunken(true);
	m_Box2.AddTopWindow(m_Label_OptionsL);
	m_Box2.AddTopWindow(m_Check_Option_NicheL);
	m_Box2.AddTopWindow(m_Check_Option_ContreNicheL);
	m_Box2.AddTopWindow(m_Check_Option_ItemMoyenL);
	m_Box2.AddTopWindow(m_Check_Option_ItemsL);
	m_Box2.AddTopWindow(m_Label_OptionsC);
	m_Box2.AddTopWindow(m_Check_Option_NicheC);
	m_Box2.AddTopWindow(m_Check_Option_ContreNicheC);
	m_Box2.AddTopWindow(m_Check_Option_ItemMoyenC);
	m_Box2.AddTopWindow(m_Check_Option_ItemsC);
	m_Box2.AddTopWindow(m_Label_OptionsP);
	m_Box2.AddTopWindow(m_Check_Option_NicheP);
	m_Box2.AddTopWindow(m_Check_Option_ContreNicheP);
	m_Box2.AddTopWindow(m_Check_Option_ItemMoyenP);
	m_Box2.AddTopWindow(m_Check_Option_ItemsP);
	m_Box2.AddTopWindow(m_Label_Grandeurs);
	m_Box2.AddTopWindow(m_Radio_Grandeurs_Ligne);
	m_Box2.AddTopWindow(m_Radio_Grandeurs_Colonne);
	m_Box2.AddTopWindow(m_Label_Terrains);
	m_Box2.AddTopWindow(m_Radio_Terrains_Ligne);
	m_Box2.AddTopWindow(m_Radio_Terrains_Colonne);
	m_Box2.AddTopWindow(m_Label_Surlignage);
	m_Box2.AddTopWindow(m_Radio_Surlignage_Page);
	m_Box2.AddTopWindow(m_Radio_Surlignage_Global);

	SetColors();
}

void CDlgParams::SetColors()
{
	m_Box1.							SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::MEDIUM_DEGRAD);
	m_Box2.							SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::MEDIUM_DEGRAD);

	m_Radio_Base_Univers			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Radio_Base_Univers			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Radio_Base_Univers			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Radio_Base_Niche				.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Radio_Base_Niche				.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Radio_Base_Niche				.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);

	m_Check_Option_NicheL			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);					
	m_Check_Option_NicheL			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_NicheL			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_ContreNicheL		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Check_Option_ContreNicheL		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ContreNicheL		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenL		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemMoyenL		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenL		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsL			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemsL			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsL			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	

	m_Check_Option_NicheC			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);					
	m_Check_Option_NicheC			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_NicheC			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_ContreNicheC		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Check_Option_ContreNicheC		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ContreNicheC		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenC		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemMoyenC		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenC		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsC			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemsC			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsC			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	

	m_Check_Option_NicheP			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);					
	m_Check_Option_NicheP			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_NicheP			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);					
	m_Check_Option_ContreNicheP		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Check_Option_ContreNicheP		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ContreNicheP		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenP		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemMoyenP		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemMoyenP		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsP			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);	
	m_Check_Option_ItemsP			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	
	m_Check_Option_ItemsP			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);	

	m_Radio_Grandeurs_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Radio_Grandeurs_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Grandeurs_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Grandeurs_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Radio_Grandeurs_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Grandeurs_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		

	m_Radio_Terrains_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Radio_Terrains_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Terrains_Ligne			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Terrains_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Radio_Terrains_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Terrains_Colonne		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		

	m_Radio_Surlignage_Page			.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);			
	m_Radio_Surlignage_Page			.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);			
	m_Radio_Surlignage_Page			.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);			
	m_Radio_Surlignage_Global		.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Radio_Surlignage_Global		.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Radio_Surlignage_Global		.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		

	m_Label_Base100					.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Base100					.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_OptionsL				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_OptionsL				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_OptionsC				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_OptionsC				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_OptionsP				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_OptionsP				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Grandeurs				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Grandeurs				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Terrains				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Terrains				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Surlignage				.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Surlignage				.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
}

void CDlgParams::SetControls()
{
	int IdOn = 0;

	if((m_TableStyle & TSTYLE_UNIVERSEX))
	{
		IdOn = IDC_AN_BASE_UNIVERS;
		m_Check_Option_NicheL.		EnableWindow(true); 
		m_Check_Option_ContreNicheL.EnableWindow(true); 
		m_Check_Option_NicheC.		EnableWindow(true); 
		m_Check_Option_ContreNicheC.EnableWindow(true); 
		m_Check_Option_NicheP.		EnableWindow(true); 
		m_Check_Option_ContreNicheP.EnableWindow(true); 
	}
	else
	{
		IdOn = IDC_AN_BASE_NICHE;
		m_Check_Option_NicheL.		EnableWindow(false); 
		m_Check_Option_ContreNicheL.EnableWindow(false); 
		m_Check_Option_NicheC.		EnableWindow(false); 
		m_Check_Option_ContreNicheC.EnableWindow(false); 
		m_Check_Option_NicheP.		EnableWindow(false); 
		m_Check_Option_ContreNicheP.EnableWindow(false); 
	}
	CheckRadioButton(IDC_AN_BASE_UNIVERS,IDC_AN_BASE_NICHE,IdOn);

	if((m_TableStyle & TSTYLE_NICHER))
		m_Check_Option_NicheL.SetCheck(BST_CHECKED);
	else
		m_Check_Option_NicheL.SetCheck(BST_UNCHECKED);

	if((m_TableStyle & TSTYLE_CONTRENICHER))
		m_Check_Option_ContreNicheL.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ContreNicheL.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMMOYENR))
		m_Check_Option_ItemMoyenL.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemMoyenL.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMSR))
		m_Check_Option_ItemsL.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemsL.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_NICHEC))
		m_Check_Option_NicheC.SetCheck(BST_CHECKED);
	else
		m_Check_Option_NicheC.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_CONTRENICHEC))
		m_Check_Option_ContreNicheC.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ContreNicheC.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMMOYENC))
		m_Check_Option_ItemMoyenC.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemMoyenC.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMSC))
		m_Check_Option_ItemsC.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemsC.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_NICHEP))
		m_Check_Option_NicheP.SetCheck(BST_CHECKED);
	else
		m_Check_Option_NicheP.SetCheck(BST_UNCHECKED);

	if((m_TableStyle & TSTYLE_CONTRENICHEP))
		m_Check_Option_ContreNicheP.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ContreNicheP.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMMOYENP))
		m_Check_Option_ItemMoyenP.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemMoyenP.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_ITEMSP))
		m_Check_Option_ItemsP.SetCheck(BST_CHECKED);
	else
		m_Check_Option_ItemsP.SetCheck(BST_UNCHECKED);

	if((m_TableStyle& TSTYLE_GRANDEURCOL))
		CheckRadioButton(IDC_AN_GRANDEUR_LIGNE ,IDC_AN_GRANDEUR_COLONNE ,IDC_AN_GRANDEUR_COLONNE);
	else
		CheckRadioButton(IDC_AN_GRANDEUR_LIGNE ,IDC_AN_GRANDEUR_COLONNE ,IDC_AN_GRANDEUR_LIGNE);

	if((m_TableStyle& TSTYLE_TERRAINCOL))
		CheckRadioButton(IDC_AN_TERRAIN_LIGNE ,IDC_AN_TERRAIN_COLONNE ,IDC_AN_TERRAIN_COLONNE);
	else
		CheckRadioButton(IDC_AN_TERRAIN_LIGNE ,IDC_AN_TERRAIN_COLONNE ,IDC_AN_TERRAIN_LIGNE);

	// Par défaut maintenant c'est la globalité du tableau
	m_TableStyle &= ~ TSTYLE_ORDONNERPAGE;
	if((m_TableStyle& TSTYLE_ORDONNERPAGE))
		CheckRadioButton(IDC_AN_SURLIGNAGE_PAGE,IDC_AN_SURLIGNAGE_GLOBAL ,IDC_AN_SURLIGNAGE_PAGE);
	else
		CheckRadioButton(IDC_AN_SURLIGNAGE_PAGE,IDC_AN_SURLIGNAGE_GLOBAL ,IDC_AN_SURLIGNAGE_GLOBAL);
}

// CDlgParams message handlers

void CDlgParams::OnBnClickedAnBaseUnivers()
{
	if((m_TableStyle& TSTYLE_UNIVERSEX) == 0)
	{
		// Remove the niche
		m_TableStyle &= ~TSTYLE_NICHEX; 
		// Set the base and univers styles
		m_TableStyle |=  TSTYLE_UNIVERSEX;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnBaseNiche()
{
	if((m_TableStyle& TSTYLE_UNIVERSEX))
	{
		// Remove the base and univers styles
		m_TableStyle &= ~( TSTYLE_UNIVERSEX | TSTYLE_CONTRENICHEX);
		// Set the niche style
		m_TableStyle |=  TSTYLE_NICHEX;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionNicheL()
{
	if(m_Check_Option_NicheL.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_NICHER;
	else
		m_TableStyle &= ~( TSTYLE_NICHER);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionContrenicheL()
{
	if(m_Check_Option_ContreNicheL.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_CONTRENICHER;
	else
		m_TableStyle &= ~( TSTYLE_CONTRENICHER);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemmoyenL()
{
	if(m_Check_Option_ItemMoyenL.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMMOYENR;
	else
		m_TableStyle &= ~(TSTYLE_ITEMMOYENR);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemsL()
{
	if(m_Check_Option_ItemsL.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMSR;
	else
		m_TableStyle &= ~(TSTYLE_ITEMSR);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionNicheC()
{
	if(m_Check_Option_NicheC.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_NICHEC;
	else
		m_TableStyle &= ~(TSTYLE_NICHEC);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionContrenicheC()
{
	if(m_Check_Option_ContreNicheC.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_CONTRENICHEC;
	else
		m_TableStyle &= ~( TSTYLE_CONTRENICHEC);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemmoyenC()
{
	if(m_Check_Option_ItemMoyenC.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMMOYENC;
	else
		m_TableStyle &= ~( TSTYLE_ITEMMOYENC);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemsC()
{
	if(m_Check_Option_ItemsC.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMSC;
	else
		m_TableStyle &= ~( TSTYLE_ITEMSC);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionNicheP()
{
	if(m_Check_Option_NicheP.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_NICHEP;
	else
		m_TableStyle &= ~( TSTYLE_NICHEP);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionContrenicheP()
{
	if(m_Check_Option_ContreNicheP.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_CONTRENICHEP;
	else
		m_TableStyle &= ~( TSTYLE_CONTRENICHEP);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemmoyenP()
{
	if(m_Check_Option_ItemMoyenP.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMMOYENP;
	else
		m_TableStyle &= ~( TSTYLE_ITEMMOYENP);
	SetControls();
}

void CDlgParams::OnBnClickedAnOptionItemsP()
{
	if(m_Check_Option_ItemsP.GetCheck() == BST_CHECKED)
		m_TableStyle |=  TSTYLE_ITEMSP;
	else
		m_TableStyle &= ~(TSTYLE_ITEMSP);
	SetControls();
}

void CDlgParams::OnBnClickedAnGrandeurLigne()
{
	if((m_TableStyle& TSTYLE_GRANDEURCOL))
	{
		m_TableStyle &= ~ TSTYLE_GRANDEURCOL;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnGrandeurColonne()
{
	if((m_TableStyle& TSTYLE_GRANDEURCOL) == 0)
	{
		m_TableStyle |=  TSTYLE_GRANDEURCOL;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnTerrainLigne()
{
	if((m_TableStyle& TSTYLE_TERRAINCOL))
	{
		m_TableStyle &= ~ TSTYLE_TERRAINCOL;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnTerrainColonne()
{
	if((m_TableStyle& TSTYLE_TERRAINCOL) == 0)
	{
		m_TableStyle |=  TSTYLE_TERRAINCOL;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnSurlignagePage()
{
	if((m_TableStyle& TSTYLE_ORDONNERPAGE) == 0)
	{
		m_TableStyle |=  TSTYLE_ORDONNERPAGE;
	}
	SetControls();
}

void CDlgParams::OnBnClickedAnSurlignageGlobal()
{
	if((m_TableStyle& TSTYLE_ORDONNERPAGE))
	{
		m_TableStyle &= ~ TSTYLE_ORDONNERPAGE;
	}
	SetControls();
}

void CDlgParams::OnBnClickedOk()
{
    m_pDoc->Set_TableStyle(m_TableStyle);
	OnOK();
}
