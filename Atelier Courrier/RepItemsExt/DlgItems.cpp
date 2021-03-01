#include "..\analyseurext\dlganalyseur.cpp"
#include "stdafx.h"
#include ".\dlgitems.h"

#include "ItemMode.h"
#include "ClassMode.h"
#include "QuantileMode.h"
#include "PaletteMode.h"
#include "ConsultationMode.h"
#include "InduiteMode.h"
#include "PatrimoineDlg.h"
#include "SourceDlg.h"
#include "SegmentDlg.h"
#include "PoidsDlg.h"
#include "FormatStr.h"
//#include <AllTypeofNode.h>
#include "ExportDlg.h"
#include "ATPMessages.h"

#include "DlgReplaceLib.h"
#include "DlgFiltre.h"
#include "TerrainManager.h"
#include "ItemsConstruitsAuto.h"


#define WINVER 0x0500
#include <Winuser.h>
#include <Windows.h>


#if !defined(BUILD_FOR_ATELIER_INTERNET)
 #include "ExcelPrinter.h"
#endif

#include <afxadv.h>            // For CSharedFile


#include "JFCConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constantes gestion Split vertical
#define WM_SPLIT2_VERT   (WM_USER + 1)

/// The RepItems About Dialog
/**
* \ingroup RepItems
*
*
* \par requirements
* win98 or later\n
* win2k or later\n
* MFC\n
*
* \version 1.0
* first version
*
* \date 07-09-2003
*
* \author eddie
*
* \par license
* This code is the property of JFC Informatique & Media\n
* Unauthorized copying or use is strictly forbiden.
* 
* \todo 
*
* \bug 
*
*/
class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParentWnd = NULL);

	/// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_RI_ABOUTBOX };
	//}}AFX_DATA

	/// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support
	//}}AFX_VIRTUAL

	/// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRiEffacerSegment();
	afx_msg void OnRiEffacerSas();
	afx_msg void OnRiEffacerTout();
};

CAboutDlg::CAboutDlg(CWnd* pParentWnd) : CDialog(CAboutDlg::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	/// No message handlers

	ON_WM_KEYUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
/// CDlgItems dialog

// Dimension de base de la fenêtre répertoire items par défaut
const CRect CDlgItems::s_rectDefault(100,100, 800, 600);

//  Taille des bordures frame
const JUnt32 BordureJfc = 7;


#if defined(BUILD_FOR_ATELIER_INTERNET)
CDlgItems::CDlgItems(CWnd* pParent) : CDialog(CDlgItems::IDD, pParent)/// @@@ REPITEMS : ATELIER INTERNET
{ Setup(1, pParent); }

CDlgItems::CDlgItems(int lang, CWnd* pParent) : CDialog(CDlgItems::IDD, pParent)/// @@@ REPITEMS : ATELIER INTERNET
{ Setup(lang, pParent); }
#else
CDlgItems::CDlgItems(CWnd* pParent) : CDialog(CDlgItems::IDD, pParent), m_SplitVert(WM_SPLIT2_VERT)
{ Setup(pParent); }
#endif

#if defined(BUILD_FOR_ATELIER_INTERNET)
void CDlgItems::Setup(int lang, CWnd* pParent)
#else
void CDlgItems::Setup(CWnd* pParent)
#endif
{

#if defined(BUILD_FOR_ATELIER_INTERNET)
	DlgItemsMessage::SetCurrentLanguage(lang);
	ItemsMessage::SetCurrentLanguage(lang);
#endif

	// Segment courant
	m_bSegment = 0;

	//	m_pIConst			=	CItemsConstruits::GetInstance();
	m_Id_Source			=	0;
	m_Id_Terrain		=	0;
	m_ContingentMask	=	0xFFFF;
	m_Id_TypePoids		=	1; /// Individus
	m_pCurMode			=	0;

	// Init mode (rep item ou pupitre)
	m_Modes.Reset();

	m_hIcon				=	0;
	m_CATPColorsApp		=	CATPColors::COLORREPITEMS;
	m_FIcones			=	0;
	m_QFilter			=	"";
	m_iModality			=	0;

	CString nString;
	LOADSTRING(nString, IDS_RI_USER_BLOC);

	JLabel Label = nString;
	m_BlocQuestion.SetLabel(Label);
	m_BlocQuestion.SetID(99999999);

	m_pTerrain = 0;

	m_bFirstTime	= TRUE;

	// Show all results by default
	m_ShowResults	= SHOW_ALL;

	// Show mode standard par defaul
	m_ShowMode		= SHOWMODE_STANDARD;
	
	// Ici type de boite par défaut c'est le répertoire des items
	m_TypeRepItem = true;

	// Par défaut non actif 
	m_AjoutSelCompoPalette_Dim1 = false;
	m_AjoutSelCompoPalette_Dim2 = false;

	// Init map mode sélection dim1 et dim2
	m_MapModeSelQuestionDim1.Reset(); 
	m_MapModeSelQuestionDim2.Reset(); 

	// Init Item pour mode genèse
	m_IdItemGenese = 0;

	// Postionnement Mode Selection Ajout Lignes, Colonnes, Pages
	m_SelectionEnBas = true;

	// Récupère la fenêtre parent au niveau du tree rubrique items
	m_Tree_Rubrique_Items.m_pDlgItems		= this;
	m_Tree_Rubrique_Items.m_pDialog			= this;

	// Récupère la fenêtre parent au niveau du tree rubrique palettes
	m_Tree_Rubrique_Palettes.m_pDlgItems	= this;
	m_Tree_Rubrique_Palettes.m_pDialog		= this;
}

CDlgItems::~CDlgItems()
{
	if(m_pTerrain)
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);
}

void CDlgItems::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Contrôle séparateur split vertical
	// @@@ REPITEMS : ATELIER INTERNET
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	DDX_Control(pDX, IDC_CS_SPLIT2_VERT,		m_SplitVert);
#endif

	// DDX_Control(pDX, IDC_RI_COMBO_RUBRIQUES, m_ComboRubriques);
}

BEGIN_MESSAGE_MAP(CDlgItems, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RI_BUT_ACCEPT,					OnAccept)
	ON_BN_CLICKED(IDC_RI_RAD_DIM2_ET,					OnDim_2_Et)
	ON_BN_CLICKED(IDC_RI_RAD_DIM2_NI,					OnDim_2_Ni)
	ON_BN_CLICKED(IDC_RI_RAD_DIM2_XOR,					OnDim_2_Xor)
	ON_BN_CLICKED(IDC_RI_RAD_DIM2_OU,					OnDim_2_Ou)
	ON_BN_CLICKED(IDC_RI_BUT_DIM2_RAZ,					OnDim_2_Raz)
	ON_BN_CLICKED(IDC_RI_BUT_GENESE,					OnGenese)
	ON_BN_CLICKED(IDC_RI_PUSH_DIM1_OK,					OnDim_1_Ok)
	ON_BN_CLICKED(IDC_RI_BUT_DIM1_DOWN,					OnDim_1_Down)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_ET,					OnDim_1_Et)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_XOR,					OnDim_1_Xor)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_NI,					OnDim_1_Ni)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_OU,					OnDim_1_Ou)
	ON_BN_CLICKED(IDC_RI_BUT_DIM1_RAZ,					OnDim_1_Raz)
	ON_BN_CLICKED(IDC_RI_BUT_DIM1_UP,					OnDim_1_Up)
	ON_BN_CLICKED(IDC_RI_BUT_FILTRE,					OnDim_Filtre)
	ON_BN_CLICKED(IDC_RI_RAD_Q_ET,						OnSAS_Et)
	ON_BN_CLICKED(IDC_RI_RAD_Q_OU,						OnSAS_Ou)
	ON_BN_CLICKED(IDC_RI_BUT_QUESTION_TOUS,				OnQuestions_Tous)
	ON_BN_CLICKED(IDC_RI_RAD_S_ET,						OnSegment_Et)
	ON_BN_CLICKED(IDC_RI_RAD_S_OU,						OnSegment_Ou)
	ON_NOTIFY(TCN_SELCHANGE, IDC_RI_TAB_SEGMENT,		OnSelchange_TabSegment)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_LBN_SELCHANGE(IDC_RI_LIST_QUESTIONS,				OnSelchange_ListQuestions)
	ON_LBN_SELCHANGE(IDC_RI_LIST_DIMENSION1,			OnSelchange_ListDimension_1)
	ON_LBN_SELCHANGE(IDC_RI_LIST_IITEMS,				OnSelchange_ListItems)
	ON_LBN_SELCHANGE(IDC_RI_LIST_DIMENSION2,			OnSelchange_ListDimension_2)
	ON_LBN_SELCHANGE(IDC_RI_LIST_SAS,					OnSelchange_ListSas)
	ON_WM_CONTEXTMENU()
	ON_WM_INITMENUPOPUP()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_RI_PAT_CONSULTER,					OnPatrimoine_Consult)
	ON_COMMAND(IDM_RI_SEG_CHANGER,						OnChange_PopSegment)
	ON_COMMAND(IDM_RI_SP_CHOISIR,						OnChange_TypePoids)
	ON_COMMAND(IDM_RI_SOURCE_CHANGER,					OnChange_Source)
	ON_COMMAND(IDM_RI_PALETTE,							OnChange_ModePalette)
	ON_COMMAND(IDM_RI_QUANTILE,							OnChange_ModeQuantile)
	ON_COMMAND(IDM_RI_CLASSE,							OnChange_ModeClasse)
	ON_COMMAND(IDM_RI_ITEM,								OnChange_ModeItem)
	ON_BN_CLICKED(IDC_RI_RAD_QUESTIONS,					OnTree_Questions)
	ON_BN_CLICKED(IDC_RI_RAD_LIST_1,					OnList_1)
	ON_BN_CLICKED(IDC_RI_RAD_LIST_2,					OnList_2)
	ON_BN_CLICKED(IDC_RI_RAD_DIMENSION2,				OnChange_Dim_2_PaletteSel)
	ON_BN_CLICKED(IDC_RI_RAD_DIMENSION1,				OnChange_Dim_1_PaletteSel)
	ON_MESSAGE(WM_USER_DIMENSION_X_DLG_DIMSEL,			OnChange_Dim_x_PaletteSel)
	ON_MESSAGE(WM_USER_DIMENSION_X_DLG_UPDATED,			OnSelChange_ListDimension_x)
	ON_MESSAGE(WM_USER_QUESTIONLISTUPDATED,				OnUpdated_QuestionList)
	ON_MESSAGE(WM_USER_ITEMLISTUPDATED,					OnUpdated_ItemList)
	ON_MESSAGE(WM_USER_VALUE_DLG_DIMSEL,				OnChange_Dim_ValueSel)
	ON_MESSAGE(WM_USER_VALUE_DLG_UPDATED,				OnSelChange_ListValue)
	ON_MESSAGE(WM_USER_VALUE_DLG_QUANTILE,				OnValuesQuantile)
	ON_BN_CLICKED(IDC_RI_CHECK_CALC_SEGMENT,			OnChange_SegmentCalc)
	ON_COMMAND(IDM_RI_PALETTE_1,						OnChange_Palette1)
	ON_COMMAND(IDM_RI_PALETTE_2,						OnChange_Palette2)
	ON_COMMAND(IDM_RI_PALETTE_3,						OnChange_Palette3)
	ON_COMMAND(IDM_RI_PALETTE_4,						OnChange_Palette4)
	ON_COMMAND(IDM_RI_PALETTE_5,						OnChange_Palette5)
	ON_COMMAND(IDM_RI_PALETTE_6,						OnChange_Palette6)
	ON_COMMAND(IDM_RI_PALETTE_7,						OnChange_Palette7)
	ON_COMMAND(IDM_RI_PALETTE_8,						OnChange_Palette8)
	ON_COMMAND(IDM_RI_PALETTE_9,						OnChange_Palette9)
	ON_COMMAND(IDM_RI_PALETTE_10,						OnChange_Palette10)
	ON_COMMAND(IDM_RI_PALETTE_11,						OnChange_Palette11)
	ON_COMMAND(IDM_RI_PALETTE_12,						OnChange_Palette12)
	ON_COMMAND(IDM_RI_PALETTE_13,						OnChange_Palette13)
	ON_COMMAND(IDM_RI_PALETTE_14,						OnChange_Palette14)
	ON_COMMAND(IDM_RI_PALETTE_15,						OnChange_Palette15)
	ON_COMMAND(IDM_RI_ABOUTBOX,							OnAboutbox)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_RI_BUT_RAZSEARCH,					OnRazSearch)
	ON_EN_CHANGE(IDC_RI_EDIT_SEARCH,					OnChanged_EditSearch)
	ON_WM_TIMER()
	ON_COMMAND(IDM_RI_ICONES,							OnIcones)
	ON_COMMAND(IDM_RI_INDUITE,							OnChange_ModeInduite)
	ON_COMMAND(IDM_RI_PAT_IMPORTER,						OnPatrimoine_Import)
	ON_COMMAND(IDM_RI_PAT_EXPORTER,						OnPatrimoine_Export)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	// ON_COMMAND(IDM_RI_QFTOUS,							OnQFilter_Tous)
	// ON_COMMAND(IDM_RI_QFQUESTIONS,						OnQFilter_Questions)
	// ON_COMMAND(IDM_RI_QFMODALITES,						OnQFilter_Modalites)
	ON_COMMAND(IDC_RI_BUT_QFILTER,							OnQFilter_Tous)
	ON_BN_CLICKED(IDC_RI_BUT_QFILTERRAZ,				OnQFilter_RAZ)
	ON_COMMAND(IDM_RI_QFPALETTE,						OnQFilter_Palette)

	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE,				OnUpdate_ModePalette)
	ON_UPDATE_COMMAND_UI(IDM_RI_ITEM,					OnUpdate_ModeItem)
	ON_UPDATE_COMMAND_UI(IDM_RI_CLASSE,					OnUpdate_ModeClasse)
	ON_UPDATE_COMMAND_UI(IDM_RI_QUANTILE,				OnUpdate_ModeQuantile)
	ON_UPDATE_COMMAND_UI(IDM_RI_INDUITE,				OnUpdate_ModeInduite)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_1,				OnUpdate_Palette1)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_2,				OnUpdate_Palette2)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_3,				OnUpdate_Palette3)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_4,				OnUpdate_Palette4)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_5,				OnUpdate_Palette5)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_6,				OnUpdate_Palette6)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_7,				OnUpdate_Palette7)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_8,				OnUpdate_Palette8)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_9,				OnUpdate_Palette9)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_10,				OnUpdate_Palette10)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_11,				OnUpdate_Palette11)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_12,				OnUpdate_Palette12)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_13,				OnUpdate_Palette13)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_14,				OnUpdate_Palette14)
	ON_UPDATE_COMMAND_UI(IDM_RI_PALETTE_15,				OnUpdate_Palette15)

	// ne marche pas ON_UPDATE_COMMAND_UI(IDM_RI_EFFACER_SEGMENT,		OnUpdateEffacerSegment)
	ON_COMMAND(IDM_RI_QUITTER, OnQuitter)
	ON_NOTIFY(TVN_SELCHANGED, IDC_RI_TREE_THEME,		OnTvnSelchangedRiTreeTheme)

	ON_BN_CLICKED(IDC_RI_BTN_DOWN,						OnBnClickedRiBtnDownItem)
	ON_BN_CLICKED(IDC_RI_BTN_UP,						OnBnClickedRiBtnUpItem)
	ON_BN_CLICKED(IDC_RI_BTN_IMPITEM,					OnBnClickedRiBtnImpitem)
	ON_BN_CLICKED(IDC_RI_BTN_EXPORT_ITEM,				OnBnClickedRiBtnExportItem)
	ON_BN_CLICKED(IDC_RI_BTN_SUPP_ITEM,					OnBnClickedRiBtnSuppItem)

	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA,			OnGetListItem)
	ON_COMMAND(IDM_RI_EFFACER_SEGMENT,					OnRiEffacerSegment)
	ON_COMMAND(IDM_RI_EFFACER_TOUT,						OnRiEffacerTout)
	ON_COMMAND(IDM_RI_EFFACER_ITEM,						OnRiEffacerItem)
	ON_COMMAND(IDM_RI_OPE_DIM1_OU,						OnRiOpeDim1Ou)
	ON_COMMAND(IDM_RI_OPE_DIM1_ET,						OnRiOpeDim1Et)
	ON_COMMAND(IDM_RI_OPE_DIM1_EX,						OnRiOpeDim1Ex)
	ON_COMMAND(IDM_RI_OPE_DIM1_NI,						OnRiOpeDim1Ni)
	ON_COMMAND(IDM_RI_OPE_DIM2_OU,						OnRiOpeDim2Ou)
	ON_COMMAND(IDM_RI_OPE_DIM2_ET,						OnRiOpeDim2Et)
	ON_COMMAND(IDM_RI_OPE_DIM2_XOR,						OnRiOpeDim2Xor)
	ON_COMMAND(IDM_RI_OPE_DIM2_NI,						OnRiOpeDim2Ni)
	
	ON_BN_CLICKED(IDCANCEL,								OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_LIGNES,				OnBnClickedRiRadModeLignes)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_COLONNES,				OnBnClickedRiRadModeColonnes)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_PAGES,				OnBnClickedRiRadModePages)
	ON_BN_CLICKED(IDC_RI_BUT_MENU_OPE_DIM1,				OnBnClickedRiButMenuOpeDim1)
	ON_BN_CLICKED(IDC_RI_BUT_MENU_OPE_DIM2,				OnBnClickedRiButMenuOpeDim2)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_RI_BUT_MENU_OPE_QUSEL,			OnBnClickedRiButMenuOpeQusel)
	ON_BN_CLICKED(IDC_RI_BUT_MENU_OPE_SEGMENT,			OnBnClickedRiButMenuOpeSegment)
	ON_COMMAND(IDR_RI_MENU_SUPP_LIGNES,					OnRiMenuSuppLignes)
	ON_COMMAND(IDR_RI_MENU_SUPP_COLONNES,				OnRiMenuSuppColonnes)
	ON_COMMAND(IDR_RI_MENU_SUPP_PAGES,					OnRiMenuSuppPages)
	ON_COMMAND(IDR_RI_MENU_SUPP_TOUT,					OnRiMenuSuppTout)
	ON_BN_CLICKED(IDC_RI_BUT_ITEM_MOY_LIG,				OnBnClickedRiItemMoyenLig)
	ON_BN_CLICKED(IDC_RI_BUT_ITEM_MOY_COL,				OnBnClickedRiItemMoyenCol)
	ON_BN_CLICKED(IDC_RI_BUT_LIGNES_DOWN,				OnBnClickedRiButLignesDown)
	ON_BN_CLICKED(IDC_RI_BUT_LIGNES_UP,					OnBnClickedRiButLignesUp)
	ON_BN_CLICKED(IDC_RI_BUT_COLONNES_UP,				OnBnClickedRiButColonnesUp)
	ON_BN_CLICKED(IDC_RI_BUT_COLONNES_DOWN,				OnBnClickedRiButColonnesDown)
	ON_BN_CLICKED(IDC_RI_BUT_PAGES_UP,					OnBnClickedRiButPagesUp)
	ON_BN_CLICKED(IDC_RI_BUT_PAGES_DOWN,				OnBnClickedRiButPagesDown)
	ON_STN_CLICKED(IDC_RI_LABEL_DIMENSION2,				OnStnClickedRiLabelDimension2)
	ON_WM_ACTIVATE()
	ON_WM_KEYDOWN()
	ON_NOTIFY(LVN_KEYDOWN, IDC_RI_LIST_LIGNES,			OnLvnKeydownRiListLignes)
	ON_NOTIFY(LVN_KEYDOWN, IDC_RI_LIST_COLONNES,		OnLvnKeydownRiListColonnes)
	ON_NOTIFY(LVN_KEYDOWN, IDC_RI_LIST_PAGES,			OnLvnKeydownRiListPages)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_RI_LIST_LIGNES,		OnLvnBegindragRiListLignes)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_RI_LIST_PAGES,			OnLvnBegindragRiListPages)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_RI_LIST_COLONNES,		OnLvnBegindragRiListColonnes)
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_SPLIT2_VERT,							OnSplitVert)
	ON_BN_CLICKED(IDC_RI_BUT_SAS_RAZ,					OnBnClickedRiButSasRaz)
	ON_NOTIFY(NM_CLICK, IDC_RI_LIST_LIGNES,				OnNMClickRiListLignes)
	ON_NOTIFY(NM_CLICK, IDC_RI_LIST_COLONNES,			OnNMClickRiListColonnes)
	ON_NOTIFY(NM_CLICK, IDC_RI_LIST_PAGES,				OnNMClickRiListPages)
	ON_COMMAND(ID_SUPPRIMERTOUTESLESLIGNES,				OnSupprimertoutesleslignes)
	ON_COMMAND(ID_SUPPRIMERTOUTESLESCOLONNES,			OnSupprimertouteslescolonnes)
	ON_COMMAND(ID_SUPPRIMERTOUTESLESPAGES,				OnSupprimertouteslespages)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RI_LIST_LIGNES,		OnLvnItemchangedRiListLignes)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RI_LIST_PAGES,		OnLvnItemchangedRiListPages)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RI_LIST_COLONNES,	OnLvnItemchangedRiListColonnes)
	ON_BN_CLICKED(IDC_RI_BTN_SITES_LIG,					OnBnClickedRiBtnSitesLig)
	ON_BN_CLICKED(IDC_RI_BUT_ALL_LIGNES,				OnBnClickedRiButAllLignes)
	ON_BN_CLICKED(IDC_RI_BUT_ALL_COLONNES,				OnBnClickedRiButAllColonnes)
	ON_BN_CLICKED(IDC_RI_BUT_ALL_PAGES,					OnBnClickedRiButAllPages)
	ON_COMMAND(IDM_RI_EFFACER_TOUT_PALETTE,				OnRiEffacerToutPalette)
	ON_COMMAND(IDM_RI_EFFACER_PALETTE,					OnRiEffacerPalette)
	ON_COMMAND(IDR_RI_MENU_SUPP_ALL_LCP,				OnRiMenuSuppAllLcp)
	ON_BN_CLICKED(IDC_RI_BTN_SITES_COL,					OnBnClickedRiBtnSitesCol)
	ON_BN_CLICKED(IDC_RI_BTN_SITES_PAGE,				OnBnClickedRiBtnSitesPage)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CDlgItems::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		long ValParam = pMsg->wParam; 
		if(ValParam == VK_ESCAPE)
			return TRUE;
	}
	/*
	else if (pMsg->message == WM_MOVE)
	{
		int dbg = 0;
	}
	else if (pMsg->message ==WM_PAINT) 
	{
		int dbg = 1;
	}
	*/

	return CDialog::PreTranslateMessage(pMsg);
}

	
// pour quitter l'application
BOOL CDlgItems::CanClose()
{
	// on peut fermer à tout moment
	return (TRUE);
}

void CDlgItems::Close()
{
	// on ferme la boite
	CDlgItems::OnClose();
}

void CDlgItems::SetPreloadTerrain(int idSource, int idTerrain, int idPoids)
{
	if(m_hWnd== NULL)
	{
		m_Id_Source =		idSource;
		m_Id_Terrain =		idTerrain;
		m_Id_TypePoids = idPoids;
	}
}

/////////////////////////////////////////////////////////////////////////////
/// CDlgItems message handlers
BOOL CDlgItems::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(AfxGetApp())
		m_hIcon = AfxGetApp()->LoadIcon(IDI_RI_ICON);

	// Définition des couleurs
	if (m_ShowMode == SHOWMODE_STANDARD)
		// Cherche la dernière couleur utilisée 
		CATPColors::SetAppColor(CATPColors::COLORREPITEMS, GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_COLOR_REP_ITEM, CATPColors::BOTTLE_GREEN) );
	else
		// Cherche la dernière couleur utilisée 
		CATPColors::SetAppColor(CATPColors::COLORREPITEMS, CATPColors::BLEU_PASTEL);

	// Brushs utilisés
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

#if defined(BUILD_FOR_ATELIER_INTERNET)
	SetWindowText(GETSTRING(IDD_RI_DLG_ITEMS));
#endif

	// Récupère le texte fenêtre dialogue
	this->GetWindowText(m_Title);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// If the data is not initialized then quit
	if(!GetItems())
	{
		this->EndDialog(false);
		return false;
	}

	m_FIcones = GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_OP_ICONS, 0)?true:false;
	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_OP_ICONS, m_FIcones);

	if(m_FIcones)
		GetMenu()->CheckMenuItem(IDM_RI_ICONES,MF_BYCOMMAND|MF_CHECKED);
	else
		GetMenu()->CheckMenuItem(IDM_RI_ICONES,MF_BYCOMMAND|MF_UNCHECKED);

	// Mise à jour du menu
	if (m_ShowMode == SHOWMODE_SIMPLE)
	{	
		// Vire les options Menu en mode menu simple
		GetMenu()->DeleteMenu(4,MF_BYPOSITION);
		GetMenu()->DeleteMenu(3,MF_BYPOSITION);
		GetMenu()->DeleteMenu(2,MF_BYPOSITION);
		GetMenu()->DeleteMenu(1,MF_BYPOSITION);
		GetMenu()->DeleteMenu(0,MF_BYPOSITION);
	}		

	// Initialisation des modes de construction en cours
	m_pCurMode   = 0;
	m_Id_CurMode = 1;

	m_Modes.Reset();
	m_Modes.Add(Mode_Items)		   = PTR_ABSTRACTMODE(new CItemMode(this)); 
	m_Modes.Add(Mode_Classes)	   = PTR_ABSTRACTMODE(new CClassMode(this)); 
	m_Modes.Add(Mode_Quantiles)	   = PTR_ABSTRACTMODE(new CQuantileMode(this)); 
	m_Modes.Add(Mode_Palettes)	   = PTR_ABSTRACTMODE(new CPaletteMode(this));  
	m_Modes.Add(Mode_Consultation)  = PTR_ABSTRACTMODE(new CConsultationMode(this));  
	m_Modes.Add(Mode_QuestInduites) = PTR_ABSTRACTMODE(new CInduiteMode(this));  

	// Définition des contrôles pour Rep Items
	SetupControls();

	if(m_Id_Source==0 && m_Id_Terrain==0)
	{
		m_Id_Source =	GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_SOURCE_ID, 0);
		m_Id_Terrain =	GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_TERRAIN_ID, 0);
	}
	m_pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_Id_Source,m_Id_Terrain);

	if(m_pTerrain)
	{
		m_ContingentMask = 0;
		m_Id_PopSegments.Reset();
		m_PopSegmentFilter.SetSize(0);
		JVector<CSegment*> Segments;
		if(m_pTerrain->GetSegments(Segments))
		{
			JInt32 NbSegments = Segments.GetCount();
			m_Id_PopSegments.SetCount(NbSegments);
			for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1)
			{
				Segments.MoveTo(IdxSegment); m_Id_PopSegments.MoveTo(IdxSegment);
				m_ContingentMask |= Segments.GetItem()->GetContingentMask();
				if(IdxSegment > 0)
					m_PopSegmentFilter |= Segments.GetItem ()->GetPopSegment();
				else
					m_PopSegmentFilter = Segments.GetItem ()->GetPopSegment();
				m_Id_PopSegments.GetItem() = Segments.GetItem()->GetID();
			}
		}	
		if(!m_pTerrain->HasTypePoids(m_Id_TypePoids))
			m_Id_TypePoids = m_pTerrain->GetDefaultTypePoidsID(); 


		// Set the theme tree data
		SetThemeData();

		// Set tree rubriques items
		SetRubriqueItems();

		// Set tree rubriques palettes
		SetRubriquePalettes();
	}

	// Passe au mode courant
	UpdateMode(m_Id_CurMode);
	
	// Sets the current onglet mode (question/item/palette > by default question)
	CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);
	UpdateTheme(IDC_RI_RAD_QUESTIONS);

	// Set the current onglet mode pupitre
	// CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,IDC_RI_RAD_MODE_LIGNES);

	//  Par défaut pas de liste rubriques visible
	m_ComboRubriques.ShowWindow(SW_HIDE);

	// Default radio button setting
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_OU);
	CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_OU);

	// Opérateurs par défaut
	AllOperateurParDefaut();

	m_List_Dimension1.SetRightFocus(TRUE);

	// Default results
	m_Edit_Name      .SetWindowText(""); 
	m_Static_Effectif.SetText("");
	m_Static_NbCas   .SetText("");
	m_Static_Percent .SetText("");
	m_bSegment = 0;
	m_Button_CalcSegment.SetCheck(m_bSegment);

	UpdateData(false);

	//Taille nominale de la boite 730x560
	SetWindowPos(NULL, 0,0, 730, 560, SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOREDRAW );
	
	// Boite Tree Theme visible, boite résultat non visible
	SetVisibleBoiteTreeTheme(true);
	SetVisibleBoiteValidItem(true);
	SetVisibleBoiteResultat(true);
		
	// Boite et composants pupitre non visible
	SetVisibleBoitePupitre(false);
	SetVisibleModePupitre(false);

	// Retaille fenêtre en mode répertoire item - sans barre onglet pupitre
	ResizeInterfaceItem();

	// Sets the current onglet mode (question/item/palette > by default question)
	OnBnClickedRiRadModeLignes();

	// on calcule la configuration du split vertical 
	ConfigSplit();

	// Repositionne via dernière position split vertical
	PosLastSplit();

	// Rend invalide le bouton filtre
	m_Button_Filtre.EnableWindow(FALSE);

	int NbCount = m_List_Items.GetCount();

	if (this->GetShowMode() == SHOWMODE_SIMPLE)
		m_Tree_Rubrique_Items.ExpandRubriqueDivers(); 


	UpdateTheme(IDC_RI_RAD_QUESTIONS);

#if defined(BUILD_FOR_ATELIER_INTERNET)
	if (m_SetCloseAfterOpen)
		EndDialog(0);
#endif

	// ??? Bidouille sinon bug couleur label
	// static int NoColor = CATPColors::GetAppColor(m_CATPColorsApp);
	// ChangeColor(NoColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CDlgItems::GetFocus()
{
	bool Ok = true;
}
*/

void CDlgItems::SetCloseAfterOpen(bool setCloseAfterOpen)
{
	m_SetCloseAfterOpen = setCloseAfterOpen;
}

/////////////////////////////////////////////////////////////////////
// Gestion du déplacement split horizontal fenêtes cumulativité
LRESULT CDlgItems::OnSplitVert(WPARAM WParam, LPARAM LPAram)
{
	// on récupère le déplacement
	LONG Disp = (LONG)WParam;

	// Déplace le split vertical
	DeplaceSplitVert(Disp);

	// Réactive liste résult en mode item
	if (m_Id_CurMode != Mode_Palettes)		
		m_List_Result.ShowWindow(SW_SHOW); 

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Déplacement du split vertical
void CDlgItems::DeplaceSplitVert(LONG Disp)
{
	// Inactive listes
	m_List_Result.ShowWindow(SW_HIDE); 

	// on récupère la position élmts boite questions
	RECT RectBQu_1; m_Box_Question.GetWindowRect(&RectBQu_1);				this->ScreenToClient(&RectBQu_1);
	RECT RectBQu_2; m_List_Questions.GetWindowRect(&RectBQu_2);				this->ScreenToClient(&RectBQu_2);
	RECT RectBQu_3; m_Label_Questions.GetWindowRect(&RectBQu_3);			this->ScreenToClient(&RectBQu_3);
	RECT RectBQu_4; m_Button_Questions_Tous.GetWindowRect(&RectBQu_4);		this->ScreenToClient(&RectBQu_4);

	// on récupère la position élmts boite dimension 1
	RECT RectBDim1_1; m_Box_Dimension1.GetWindowRect(&RectBDim1_1);			this->ScreenToClient(&RectBDim1_1);
	RECT RectBDim1_2; m_Header_Dimension1.GetWindowRect(&RectBDim1_2);		this->ScreenToClient(&RectBDim1_2);
	RECT RectBDim1_3; m_Button_Menu_Ope_Dim1.GetWindowRect(&RectBDim1_3);	this->ScreenToClient(&RectBDim1_3);	
	RECT RectBDim1_4; m_Button_Dim1_RAZ.GetWindowRect(&RectBDim1_4);		this->ScreenToClient(&RectBDim1_4);
	RECT RectBDim1_5bis; m_Button_Filtre.GetWindowRect(&RectBDim1_5bis);	this->ScreenToClient(&RectBDim1_5bis);
	RECT RectBDim1_5; m_Button_Dim1_UP.GetWindowRect(&RectBDim1_5);			this->ScreenToClient(&RectBDim1_5);
	RECT RectBDim1_6; m_Button_Dim1_DOWN.GetWindowRect(&RectBDim1_6);		this->ScreenToClient(&RectBDim1_6);
	RECT RectBDim1_7; m_Button_Dimension1.GetWindowRect(&RectBDim1_7);		this->ScreenToClient(&RectBDim1_7);
	RECT RectBDim1_8; m_Button_Dim1_OK.GetWindowRect(&RectBDim1_8);			this->ScreenToClient(&RectBDim1_8);
	RECT RectBDim1_9; m_Button_Dim1_ET.GetWindowRect(&RectBDim1_9);			this->ScreenToClient(&RectBDim1_9);
	RECT RectBDim1_10;m_Button_Dim1_OU.GetWindowRect(&RectBDim1_10);		this->ScreenToClient(&RectBDim1_10);
	RECT RectBDim1_11;m_Button_Dim1_NI.GetWindowRect(&RectBDim1_11);		this->ScreenToClient(&RectBDim1_11);
	RECT RectBDim1_12;m_Button_Dim1_XOR.GetWindowRect(&RectBDim1_12);		this->ScreenToClient(&RectBDim1_12);
	RECT RectBDim1_13;m_List_Dimension1.GetWindowRect(&RectBDim1_13);		this->ScreenToClient(&RectBDim1_13);

	// on récupère la position élmts boite dimension 2
	RECT RectBDim2_1; m_Box_Dimension2.GetWindowRect(&RectBDim2_1);			this->ScreenToClient(&RectBDim2_1);
	RECT RectBDim2_2; m_Header_Dimension2.GetWindowRect(&RectBDim2_2);		this->ScreenToClient(&RectBDim2_2);
	RECT RectBDim2_3; m_List_Dimension2.GetWindowRect(&RectBDim2_3);		this->ScreenToClient(&RectBDim2_3);
	RECT RectBDim2_4; m_Button_Dimension2.GetWindowRect(&RectBDim2_4);		this->ScreenToClient(&RectBDim2_4);
	RECT RectBDim2_5; m_Button_Dim2_ET.GetWindowRect(&RectBDim2_5);			this->ScreenToClient(&RectBDim2_5);
	RECT RectBDim2_6; m_Button_Dim2_OU.GetWindowRect(&RectBDim2_6);			this->ScreenToClient(&RectBDim2_6);
	RECT RectBDim2_7; m_Button_Dim2_NI.GetWindowRect(&RectBDim2_7);			this->ScreenToClient(&RectBDim2_7);
	RECT RectBDim2_8; m_Button_Dim2_XOR.GetWindowRect(&RectBDim2_8);		this->ScreenToClient(&RectBDim2_8);
	RECT RectBDim2_9; m_Button_Menu_Ope_Dim2.GetWindowRect(&RectBDim2_9);	this->ScreenToClient(&RectBDim2_9);
	RECT RectBDim2_10;m_Button_Dim2_RAZ.GetWindowRect(&RectBDim2_10);		this->ScreenToClient(&RectBDim2_10);

	// on récupère la position élmts boite SAS
	RECT RectBSAS_1; m_Box_SAS.GetWindowRect(&RectBSAS_1);					this->ScreenToClient(&RectBSAS_1);
	RECT RectBSAS_2; m_Label_SAS.GetWindowRect(&RectBSAS_2);				this->ScreenToClient(&RectBSAS_2);
	RECT RectBSAS_3; m_List_SAS.GetWindowRect(&RectBSAS_3);					this->ScreenToClient(&RectBSAS_3);
	RECT RectBSAS_4; m_Button_Menu_Ope_QuSel.GetWindowRect(&RectBSAS_4);	this->ScreenToClient(&RectBSAS_4);
	RECT RectBSAS_5; m_Button_SAS_ET.GetWindowRect(&RectBSAS_5);			this->ScreenToClient(&RectBSAS_5);
	RECT RectBSAS_6; m_Button_SAS_OU.GetWindowRect(&RectBSAS_6);			this->ScreenToClient(&RectBSAS_6);
	RECT RectBSAS_7; m_Button_Mode_Ligne.GetWindowRect(&RectBSAS_7);		this->ScreenToClient(&RectBSAS_7);
	RECT RectBSAS_8; m_Button_Mode_Colonne.GetWindowRect(&RectBSAS_8);		this->ScreenToClient(&RectBSAS_8);
	RECT RectBSAS_9; m_Button_Mode_Page.GetWindowRect(&RectBSAS_9);			this->ScreenToClient(&RectBSAS_9);
	RECT RectBSAS_10; m_Button_SAS_RAZ.GetWindowRect(&RectBSAS_10);			this->ScreenToClient(&RectBSAS_10);

	// on récupère la position élmts boite Segment
	RECT RectBSeg_1; m_Box_Segment.GetWindowRect(&RectBSeg_1);				this->ScreenToClient(&RectBSeg_1);
	RECT RectBSeg_2; m_Tab_Segment.GetWindowRect(&RectBSeg_2);				this->ScreenToClient(&RectBSeg_2);

	// RECT RectBSeg_3; m_Button_CalcSegment.GetWindowRect(&RectBSeg_3);		this->ScreenToClient(&RectBSeg_3);

	// RECT RectBSeg_4; m_Button_Menu_Ope_Segment.GetWindowRect(&RectBSeg_4);	this->ScreenToClient(&RectBSeg_4);
	// RECT RectBSeg_5; m_Button_Segment_ET.GetWindowRect(&RectBSeg_5);		this->ScreenToClient(&RectBSeg_5);
	// RECT RectBSeg_6; m_Button_Segment_OU.GetWindowRect(&RectBSeg_6);		this->ScreenToClient(&RectBSeg_6);

	// on récupère la position élmts boite Affichage Résultats
	RECT RectBResult_1; m_Box_Result.GetWindowRect(&RectBResult_1);				this->ScreenToClient(&RectBResult_1);
	RECT RectBResult_2; m_List_Result.GetWindowRect(&RectBResult_2);			this->ScreenToClient(&RectBResult_2);
	RECT RectBResult_3; m_Label_DefItem.GetWindowRect(&RectBResult_3);			this->ScreenToClient(&RectBResult_3);
	RECT RectBResult_4; m_Label_Effectif.GetWindowRect(&RectBResult_4);			this->ScreenToClient(&RectBResult_4);
	RECT RectBResult_5; m_Static_Effectif.GetWindowRect(&RectBResult_5);		this->ScreenToClient(&RectBResult_5);
	RECT RectBResult_5b; m_Label_Percent.GetWindowRect(&RectBResult_5b);		this->ScreenToClient(&RectBResult_5b);
	RECT RectBResult_6; m_Static_Percent.GetWindowRect(&RectBResult_6);			this->ScreenToClient(&RectBResult_6);
	RECT RectBResult_7; m_Label_NbCas.GetWindowRect(&RectBResult_7);			this->ScreenToClient(&RectBResult_7);
	RECT RectBResult_8; m_Static_NbCas.GetWindowRect(&RectBResult_8);			this->ScreenToClient(&RectBResult_8);
	RECT RectBResult_9; m_Label_TypePoids.GetWindowRect(&RectBResult_9);		this->ScreenToClient(&RectBResult_9);
	RECT RectBResult_10; m_Cadre_Result.GetWindowRect(&RectBResult_10);			this->ScreenToClient(&RectBResult_10);
	RECT RectBResult_12; m_Button_CalcSegment.GetWindowRect(&RectBResult_12);	this->ScreenToClient(&RectBResult_12);
	RECT RectBResult_13; m_Chk_Rubrique_Defaut.GetWindowRect(&RectBResult_13);	this->ScreenToClient(&RectBResult_13);
	RECT RectBResult_14; m_ComboRubriques.GetWindowRect(&RectBResult_14);		this->ScreenToClient(&RectBResult_14);

	// RECT RectBResult_10; m_But_Imprimer_Result

	// on récupére la position de la liste composition palette
	RECT RectBResult_11; m_List_Compo_Palette.GetWindowRect(&RectBResult_11);	this->ScreenToClient(&RectBResult_11);

	// on récupère la position élmts boite Edit Item
	RECT RectBIt_1;  m_Box_Items.GetWindowRect(&RectBIt_1);					this->ScreenToClient(&RectBIt_1);
	RECT RectBIt_2;  m_Button_Suppression.GetWindowRect(&RectBIt_2);		this->ScreenToClient(&RectBIt_2);
	RECT RectBIt_3;  m_Edit_Name.GetWindowRect(&RectBIt_3);					this->ScreenToClient(&RectBIt_3);
	RECT RectBIt_4;  m_Button_Accept.GetWindowRect(&RectBIt_4);				this->ScreenToClient(&RectBIt_4);
	RECT RectBIt_5;  m_Label_Item.GetWindowRect(&RectBIt_5);				this->ScreenToClient(&RectBIt_5);


	// Position split
	// @@@ REPITEMS : ATELIER INTERNET
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);
#endif

	// Limite boite questions et SAS
	const LONG MinLargBoiteSas = 280;

	LONG Larg = RectBQu_1.right - RectBQu_1.left; 
	if (Larg + Disp < MinLargBoiteSas || (Larg + Disp) < 0) 
		Disp = MinLargBoiteSas - Larg;

	// Limite boite résultat
	Larg = RectBResult_1.right - RectBResult_1.left; 
	if (Larg - Disp < MinLargBoiteSas || (Larg - Disp) < 0)  
		Disp = Larg - MinLargBoiteSas;

	// on corrige la position elmts fenetre boite questions
	RectBQu_1.right += Disp;
	RectBQu_2.right += Disp;
	RectBQu_3.right += Disp;
	RectBQu_4.left  += Disp;
	RectBQu_4.right += Disp;

	// on corrige la position elmts fenetre dimension 1
	RectBDim1_1.right += Disp;
	RectBDim1_2.right += Disp;
	RectBDim1_4.left  += Disp;
	RectBDim1_4.right += Disp;
	RectBDim1_5bis.left  += Disp;
	RectBDim1_5bis.right += Disp;
	RectBDim1_5.left  += Disp;
	RectBDim1_5.right += Disp;
	RectBDim1_6.left  += Disp;
	RectBDim1_6.right += Disp;
	RectBDim1_7.right += Disp;
	RectBDim1_13.right += Disp;

	// on corrige la position elmts fenetre dimension 2
	RectBDim2_1.right  += Disp;
	RectBDim2_2.right  += Disp;
	RectBDim2_3.right  += Disp;
	RectBDim2_4.right  += Disp;
	RectBDim2_10.left  += Disp;
	RectBDim2_10.right += Disp;

	// on corrige la position elmts fenetre SAS
	RectBSAS_1.right  += Disp;
	RectBSAS_2.right  += Disp;
	RectBSAS_3.right  += Disp;
	RectBSAS_10.right += Disp;
	RectBSAS_10.left  += Disp;

	// on récupère la position élmts boite Segment
	RectBSeg_1.right += Disp;
    RectBSeg_2.right += Disp;
	// RectBSeg_3.left  += Disp;
	// RectBSeg_3.right += Disp;
		
	// on corrige la position elmts fenêtre affichage résult
	RectBResult_1.left  += Disp;
	RectBResult_2.left  += Disp;
	RectBResult_3.left  += Disp;
	RectBResult_4.left	+= Disp;
	// RectBResult_4.right += Disp;
	// RectBResult_5.left	+= Disp;
	// RectBResult_6.left	+= Disp;
	RectBResult_5b.left	+= Disp;
	RectBResult_7.left	+= Disp;
	//RectBResult_8.left	+= Disp;
	RectBResult_9.left  += Disp;
	RectBResult_10.left	+= Disp;
	// RectBResult_12.left	+= Disp;

	// on corrige position de la liste composition palette
	RectBResult_11.left += Disp;

	// on corrige la position elmts fenêtre Edit Items
	RectBIt_1.left    += Disp;
	RectBIt_2.left    += Disp;
	RectBIt_2.right   += Disp;
	RectBIt_3.left    += Disp;
	
	// La barre split
	// @@@ REPITEMS : ATELIER INTERNET
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	RectSP.left += Disp; RectSP.right += Disp;
#endif

	// on déplace objets boite questions
	m_Box_Question.SetWindowPos(NULL, RectBQu_1.left, RectBQu_1.top, RectBQu_1.right - RectBQu_1.left, RectBQu_1.bottom - RectBQu_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Questions.SetWindowPos(NULL, RectBQu_2.left, RectBQu_2.top, RectBQu_2.right - RectBQu_2.left, RectBQu_2.bottom - RectBQu_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Questions.SetWindowPos(NULL, RectBQu_3.left, RectBQu_3.top, RectBQu_3.right - RectBQu_3.left, RectBQu_3.bottom - RectBQu_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Questions_Tous.SetWindowPos(NULL, RectBQu_4.left, RectBQu_4.top, RectBQu_4.right - RectBQu_4.left, RectBQu_4.bottom - RectBQu_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite dimension 1
	m_Box_Dimension1.SetWindowPos(NULL, RectBDim1_1.left, RectBDim1_1.top, RectBDim1_1.right - RectBDim1_1.left, RectBDim1_1.bottom - RectBDim1_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Header_Dimension1.SetWindowPos(NULL, RectBDim1_2.left, RectBDim1_2.top, RectBDim1_2.right - RectBDim1_2.left, RectBDim1_2.bottom - RectBDim1_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_RAZ.SetWindowPos(NULL, RectBDim1_4.left, RectBDim1_4.top, RectBDim1_4.right - RectBDim1_4.left, RectBDim1_4.bottom - RectBDim1_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Filtre.SetWindowPos(NULL, RectBDim1_5bis.left, RectBDim1_5bis.top, RectBDim1_5bis.right - RectBDim1_5bis.left, RectBDim1_5bis.bottom - RectBDim1_5bis.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_UP.SetWindowPos(NULL, RectBDim1_5.left, RectBDim1_5.top, RectBDim1_5.right - RectBDim1_5.left, RectBDim1_5.bottom - RectBDim1_5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_DOWN.SetWindowPos(NULL, RectBDim1_6.left, RectBDim1_6.top, RectBDim1_6.right - RectBDim1_6.left, RectBDim1_6.bottom - RectBDim1_6.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dimension1.SetWindowPos(NULL, RectBDim1_7.left, RectBDim1_7.top, RectBDim1_7.right - RectBDim1_7.left, RectBDim1_7.bottom - RectBDim1_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Dimension1.SetWindowPos(NULL, RectBDim1_13.left, RectBDim1_13.top, RectBDim1_13.right - RectBDim1_13.left, RectBDim1_13.bottom - RectBDim1_13.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite dimension 2
	m_Box_Dimension2.SetWindowPos(NULL, RectBDim1_1.left, RectBDim1_1.top, RectBDim1_1.right - RectBDim1_1.left, RectBDim1_1.bottom - RectBDim1_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Header_Dimension2.SetWindowPos(NULL, RectBDim2_2.left, RectBDim2_2.top, RectBDim2_2.right - RectBDim2_2.left, RectBDim2_2.bottom - RectBDim2_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Dimension2.SetWindowPos(NULL, RectBDim2_3.left, RectBDim2_3.top, RectBDim2_3.right - RectBDim2_3.left, RectBDim2_3.bottom - RectBDim2_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dimension2.SetWindowPos(NULL, RectBDim2_4.left, RectBDim2_4.top, RectBDim2_4.right - RectBDim2_4.left, RectBDim2_4.bottom - RectBDim2_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim2_RAZ.SetWindowPos(NULL, RectBDim2_10.left, RectBDim2_10.top, RectBDim2_10.right - RectBDim2_10.left, RectBDim2_10.bottom - RectBDim2_10.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite SAS
	m_Box_SAS.SetWindowPos(NULL, RectBSAS_1.left, RectBSAS_1.top, RectBSAS_1.right - RectBSAS_1.left, RectBSAS_1.bottom - RectBSAS_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_SAS.SetWindowPos(NULL, RectBSAS_2.left, RectBSAS_2.top, RectBSAS_2.right - RectBSAS_2.left, RectBSAS_2.bottom - RectBSAS_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_SAS.SetWindowPos(NULL, RectBSAS_3.left, RectBSAS_3.top, RectBSAS_3.right - RectBSAS_3.left, RectBSAS_3.bottom - RectBSAS_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Mode_Ligne.SetWindowPos(NULL, RectBSAS_7.left, RectBSAS_7.top, RectBSAS_7.right - RectBSAS_7.left, RectBSAS_7.bottom - RectBSAS_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Mode_Colonne.SetWindowPos(NULL, RectBSAS_8.left, RectBSAS_8.top, RectBSAS_8.right - RectBSAS_8.left, RectBSAS_8.bottom - RectBSAS_8.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Mode_Page.SetWindowPos(NULL, RectBSAS_9.left, RectBSAS_9.top, RectBSAS_9.right - RectBSAS_9.left, RectBSAS_9.bottom - RectBSAS_9.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_SAS_RAZ.SetWindowPos(NULL, RectBSAS_10.left, RectBSAS_10.top, RectBSAS_10.right - RectBSAS_10.left, RectBSAS_10.bottom - RectBSAS_10.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite segment
	m_Box_Segment.SetWindowPos(NULL, RectBSeg_1.left, RectBSeg_1.top, RectBSeg_1.right - RectBSeg_1.left, RectBSeg_1.bottom - RectBSeg_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Tab_Segment.SetWindowPos(NULL, RectBSeg_2.left, RectBSeg_2.top, RectBSeg_2.right - RectBSeg_2.left, RectBSeg_2.bottom - RectBSeg_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	// m_Button_CalcSegment.SetWindowPos(NULL, RectBSeg_3.left, RectBSeg_3.top, RectBSeg_3.right - RectBSeg_3.left, RectBSeg_3.bottom - RectBSeg_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace boite affichage résult
	m_Box_Result.SetWindowPos(NULL, RectBResult_1.left, RectBResult_1.top, RectBResult_1.right - RectBResult_1.left, RectBResult_1.bottom - RectBResult_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets liste composition palette
	m_List_Compo_Palette.SetWindowPos(NULL, RectBResult_11.left, RectBResult_11.top, RectBResult_11.right - RectBResult_11.left, RectBResult_11.bottom - RectBResult_11.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);	


	// Avant on cache ou décache les controles
	if (m_Id_CurMode != Mode_Palettes)		
	{
		m_List_Result.ShowWindow(TRUE); 
		m_Label_DefItem.ShowWindow(TRUE); 
		m_Label_Effectif.ShowWindow(TRUE); 
		m_Static_Effectif.ShowWindow(TRUE); 
		m_Label_Percent.ShowWindow(TRUE); 
		m_Static_Percent.ShowWindow(TRUE); 
		m_Cadre_Result.ShowWindow(TRUE);
		m_Chk_Rubrique_Defaut.ShowWindow(FALSE);   // TRUE desormais d'office dans rep defaut
		// m_ComboRubriques.ShowWindow(TRUE);
		m_Label_TypePoids.ShowWindow(TRUE); 
		m_But_Imprimer_Result.ShowWindow(TRUE); 
		m_But_Export_Item.ShowWindow(TRUE);
		m_But_Supp_Item.ShowWindow(TRUE);

		// En dehors du mode palette ordre up/down non autorisé
		//m_But_Down_Item.ShowWindow(FALSE); 
		//m_But_Up_Item.ShowWindow(FALSE); 
		m_But_Down_Item.EnableWindow(FALSE); 
		m_But_Up_Item.EnableWindow(FALSE); 
	}
	else
	{
		m_List_Result.ShowWindow(FALSE); 
		m_Label_DefItem.ShowWindow(FALSE); 
		m_Label_Effectif.ShowWindow(FALSE); 
		m_Static_Effectif.ShowWindow(FALSE); 
		m_Label_Percent.ShowWindow(FALSE); 
		m_Cadre_Result.ShowWindow(FALSE);
		m_Static_Percent.ShowWindow(FALSE); 
		m_Chk_Rubrique_Defaut.ShowWindow(FALSE);
		m_ComboRubriques.ShowWindow(FALSE);
		m_Label_TypePoids.ShowWindow(FALSE);
		
		m_But_Imprimer_Result.ShowWindow(FALSE); 
		m_But_Export_Item.ShowWindow(FALSE);
		m_But_Supp_Item.ShowWindow(FALSE);

		// En mode palette ordre up/down autorisé
		m_But_Down_Item.ShowWindow(TRUE); 
		m_But_Up_Item.ShowWindow(TRUE);
		/*
		m_But_Down_Item.EnableWindow(TRUE); 
		m_But_Up_Item.EnableWindow(TRUE); 
		*/
		m_But_Down_Item.EnableWindow(FALSE); 
		m_But_Up_Item.EnableWindow(FALSE); 
	}

	// On déplace objets boite affichage résult (mode items)
	m_List_Result.SetWindowPos(NULL, RectBResult_2.left, RectBResult_2.top, RectBResult_2.right - RectBResult_2.left, RectBResult_2.bottom - RectBResult_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_DefItem.SetWindowPos(NULL, RectBResult_3.left, RectBResult_3.top, RectBResult_3.right - RectBResult_3.left, RectBResult_3.bottom - RectBResult_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Effectif.SetWindowPos(NULL, RectBResult_4.left, RectBResult_4.top, RectBResult_4.right - RectBResult_4.left, RectBResult_4.bottom - RectBResult_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Percent.SetWindowPos(NULL, RectBResult_5b.left, RectBResult_5b.top, RectBResult_5b.right - RectBResult_5b.left, RectBResult_5b.bottom - RectBResult_5b.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_NbCas.SetWindowPos(NULL, RectBResult_7.left, RectBResult_7.top, RectBResult_7.right - RectBResult_7.left, RectBResult_7.bottom - RectBResult_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_TypePoids.SetWindowPos(NULL, RectBResult_9.left, RectBResult_9.top, RectBResult_9.right - RectBResult_9.left, RectBResult_9.bottom - RectBResult_9.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Cadre_Result.SetWindowPos(NULL, RectBResult_10.left, RectBResult_10.top, RectBResult_10.right - RectBResult_10.left, RectBResult_10.bottom - RectBResult_10.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_CalcSegment.SetWindowPos(NULL, RectBResult_12.left, RectBResult_12.top, RectBResult_12.right - RectBResult_12.left, RectBResult_12.bottom - RectBResult_12.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Chk_Rubrique_Defaut.SetWindowPos(NULL, RectBResult_13.left, RectBResult_13.top, RectBResult_13.right - RectBResult_13.left, RectBResult_13.bottom - RectBResult_13.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_ComboRubriques.SetWindowPos(NULL, RectBResult_14.left, RectBResult_14.top, RectBResult_14.right - RectBResult_14.left, RectBResult_14.bottom - RectBResult_14.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	
	// Dimensionnement colonne tableur résultat
	CRect Rect;
	m_List_Result.GetClientRect(&Rect);
	int nColInterval = 26;

	m_List_Result.DeleteColumn(0);
	m_List_Result.DeleteColumn(0);
	m_List_Result.DeleteColumn(0);

	CString labelCol;
	LOADSTRING(labelCol, IDS_RI_COL_ITEMS_COMP);
	m_List_Result.InsertColumn(0, labelCol, LVCFMT_LEFT, Rect.Width()-(4*nColInterval));
	LOADSTRING(labelCol, IDS_RI_COL_NBCAS);
	m_List_Result.InsertColumn(1, labelCol, LVCFMT_RIGHT, nColInterval*2);
	LOADSTRING(labelCol, IDS_RI_COL_EFF);
	m_List_Result.InsertColumn(2, labelCol, LVCFMT_RIGHT, nColInterval*2);
	m_List_Result.EnableToolTips(true);

	// On déplace objets boite edit items
	m_Box_Items.SetWindowPos(NULL, RectBIt_1.left, RectBIt_1.top, RectBIt_1.right - RectBIt_1.left, RectBIt_1.bottom - RectBIt_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Suppression.SetWindowPos(NULL, RectBIt_2.left, RectBIt_2.top, RectBIt_2.right - RectBIt_2.left, RectBIt_2.bottom - RectBIt_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Edit_Name.SetWindowPos(NULL, RectBIt_3.left, RectBIt_3.top, RectBIt_3.right - RectBIt_3.left, RectBIt_3.bottom - RectBIt_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	RectBIt_5.right = RectBIt_3.left - 5;
	RectBIt_5.left = RectBIt_1.left + 5;
	m_Label_Item.SetWindowPos(NULL, RectBIt_5.left, RectBIt_5.top, RectBIt_5.right - RectBIt_5.left, RectBIt_5.bottom - RectBIt_5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	
	// On déplace la barre split vertical 
	// @@@ REPITEMS : ATELIER INTERNET
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	m_SplitVert.SetWindowPos(NULL, RectSP.left, RectSP.top, RectSP.right - RectSP.left, RectSP.bottom - RectSP.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
#endif

	// on calcule la configuration du split
	LONG SPLeft		= RectBQu_1.right - RectBQu_1.left; if (SPLeft < 0) SPLeft = 0;
	LONG SPRight	= RectBResult_1.right - RectBResult_1.left; if (SPRight < 0) SPRight = 0;

	// on reconfigure le split
	// @@@ REPITEMS : ATELIER INTERNET
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	m_SplitVert.SetRange(SPLeft, SPRight);
#endif


	// Positionne la fenêtre des valeurs
	PosFenValeursQuantis();

	//// Récup ItemMode
	//m_Modes.MoveTo(Mode_Items);

	//CAbstractMode * pMode = m_Modes.GetItem().get();

	//CItemMode *pItemMode = static_cast<CItemMode*>(pMode);

	//if (pItemMode)
	//{
	//	if (pItemMode->m_pValuesDlg)
	//	{
	//		if (pItemMode->m_pValuesDlg->IsWindowVisible())
	//		{
	//			// Positionnement de la boite sous le questionnaire
	//			CRect Rect1;
	//			// m_List_Dimension1.GetWindowRect(&Rect1);
	//			m_Box_Dimension1.GetWindowRect(&Rect1);

	//			CRect Rect2;
	//			pItemMode->m_pValuesDlg->GetWindowRect(&Rect2);
	//			Rect2 = Rect1;
	//			pItemMode->m_pValuesDlg->MoveWindow(Rect2,TRUE);

	//			pItemMode->m_pValuesDlg->UpdateSize();


	//		}
	//	}	
	//}

	// Mise à jour de la fenêtre pupitre
	UpdateWindow();
	
	// Si boite dimension 2 visible, on retaille aussi la boite dimension 1
	if (m_Box_Dimension2.IsWindowVisible())
	{
		this->SetDimension2Visible(true); 
	}

}

//////////////////////////////////////////////////////////////////////////////
// Positionnement de la fenêtre valeurs quantitatives si nécessaires
// Bidouille pour la repositionner au même endroit que les listes dimensions
void CDlgItems::PosFenValeursQuantis()
{
	// Récup ItemMode
	m_Modes.MoveTo(Mode_Items);

	if (m_Modes.IsValid())
	{
		CAbstractMode * pMode = m_Modes.GetItem().get();

		CItemMode *pItemMode = static_cast<CItemMode*>(pMode);

		if (pItemMode)
		{
			if (pItemMode->m_pValuesDlg)
			{
				if (this->IsWindowVisible())
				{
					if (pItemMode->m_pValuesDlg->IsWindowVisible())
					{
						// Permet le déplacement de la fenêtre
						pItemMode->m_pValuesDlg->SetBlockFen(false);

						// Positionnement de la boite sous le questionnaire
						CRect Rect1;
						// m_List_Dimension1.GetWindowRect(&Rect1);
						m_Box_Dimension1.GetWindowRect(&Rect1);

						CRect Rect2;
						pItemMode->m_pValuesDlg->GetWindowRect(&Rect2);
						
						// Largeur et hauteur boite des valeurs
						long LgFenValues = abs(Rect2.right - Rect2.left); 
						
						// On recale toujours sur la boire 1ere diension
						Rect2.right  = Rect1.left;
						Rect2.left   = Rect2.right - LgFenValues;
						Rect2.top    = Rect1.top;
						Rect2.bottom = Rect1.bottom;  
						pItemMode->m_pValuesDlg->MoveWindow(Rect2,TRUE);
						pItemMode->m_pValuesDlg->UpdateSize();

						// Remt l'interdiction du déplacement
						pItemMode->m_pValuesDlg->SetBlockFen(true);
					}
				}
			}	
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Test si item quantitatif (surtout pour définir les éléments moyennables)
JBool CDlgItems::ItemQuantitatif(const CItemConstruit * pItem, JInt32 IdSource, JInt32 IdTerrain)
{
	CIBuilder IBuilder;

	// Récup le terrain en cours
	CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(IdSource,IdTerrain);
	if (pTerrain)
	{
		// Récup lé définition de l'item
		pItem->GetItemBuilder(pTerrain,IBuilder);

		// Par défaut ce n'est pas un quantitatif
		JBool Quantitatif = false;

		// Limitation des items moyennables  (sinon c'est le bordel !!!!)
		// -- seulement 1 segment
		// -- pas de combinaison dans 1 segment avec plusieurs items ou questions

		if (IBuilder.GetSegmentCount() > 1) 
			return false;

		/*
		// DECEMBRE 2013
		// Si aucun segemnt détecté on considère l'élmt comme quantitatif
		if (IBuilder.GetSegmentCount() == 0) 
			return true;
		*/

		// Balaye tous les segments di Builder constrcuteur de l'item
		for (int IdxSeg = IBuilder.GetSegmentCount()-1; IdxSeg >=0; IdxSeg--)
		{
			// Pour chaque segment, on vérie si il y a des questions ou des items quantitatifs
			CIBSegment * pIBSegment = IBuilder.GetSegment(IdxSeg);

			// Balaye les elmts du segment
			if(pIBSegment->HasItems())
			{
				// récupère liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				JList <JInt32> LstItems;
				pIBSegment->GetItemsID(LstItems);

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associé
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// Vérifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							// C'est une question, est-elle quantitative
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								return true;
							}
						}
					}
				}

				// récupére liste des items
				JList <JInt32> Items;
				pIBSegment->GetItemsID(Items);

				// Limitation à 1 seul item (sinon c'est encore le bordel)
				if (Items.GetCount() > 1) return false;

				for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				{
					IQuestion const * pQuestion = GetItems()->GetQuestionByID(Items.GetItem());

					IItem const * pItem = GetItems()->GetIItemByID(Items.GetItem());

					CIBItem * pTargetItem = pIBSegment->FindItem(Items.GetItem());

					if (pTargetItem)
					{
						JList<JInt32> Moda;
						pTargetItem->GetValues(Moda);

						// Get the modalities
						for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
						{
							JInt32 ModaItemId = Moda.GetItem();
							const IItem * pModaItem = GetItems()->GetIItemByID(ModaItemId);
							if(pModaItem)
							{
								if (pModaItem->IsItemConstruit())
								{
									const CItemConstruit * pItemConst = pModaItem->AsItemConstruit();

									// Repérage si on est bien dans un item avec un ou des intervalles de valeurs
									CString ValString = pItemConst->GetIBString().AsJCharPtr();
									if (ValString.Find("<")!= -1 || ValString.Find(">") != -1)
									{
										// Test si c'est un item qauntitatif
										if (ItemQuantitatif(pItemConst,IdSource,IdTerrain))
										{
											return true;
										}
										else
										{
											return false;
										}
									}

									// AOUT 2013 (on va au delà et on teste l'item constituant la palette
									else
									{
										// Test si c'est un item quantitatif
										if (ItemQuantitatif(pItemConst,IdSource,IdTerrain))
										{
											return true;
										}
										else
										{
											return false;
										}
									}

								}
							}
						}
					}
					

					if(pQuestion)
					{
						// Vérifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								return true;
							}
						}
					}
					else
					{
						CPseudo * m_pPseudoItem = GetItems()->GetNewPseudo();

						if(m_pPseudoItem && Items.GetItem()==0)
						{
							JList <JInt32> ItemsP;
							m_pPseudoItem->GetItems(ItemsP); 
							for (ItemsP.MoveFirst(); ItemsP.IsValid(); ItemsP.MoveNext())
							{
								JInt32 IdItem = ItemsP.GetItem();

								IItem * pItem = GetItems()->GetIItemByID(IdItem);
								// Est ce un autre item, ou une question
								CPseudo			*  pPseudo			= pItem->AsPseudo(); 
								if (pItem)
								{

									JLabel LabelItem = pItem->GetLabel(); 
									JBool Ok = true;
								}
								else if (pPseudo)
								{
									JList <JInt32> ItemsP2;
									pPseudo->GetItems(ItemsP2); 
								}

								JBool Val = true;
							}
						}
					}
				}
			}

			// Items composés de questions
			else if (pIBSegment->HasQuestions())	
			{
				// récupère liste des questions
				JList <JInt32> LstQuestions;
				pIBSegment->GetQuestionsID(LstQuestions); 

				// Balayage des questions du segment
				for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
				{
					// Id question segment
					JInt32 IdQuestion = LstQuestions.GetItem();

					// Question associé
					IQuestion const * pQuestion = pTerrain->GetQuestionByID(IdQuestion);

					if(pQuestion)
					{
						// Vérifie si c'est une question
						const CQuestion *pQu = pQuestion->AsQuestion();

						// C'est une question, est-elle quantitative
						if (pQu)
						{
							// C'est une question, est-elle quantitative
							if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
							{
								// Attention si la question n'est pas en mode regroupement de valeurs quantitatives
								CIBQuestion *pTargetQuestion = pIBSegment->FindQuestion(IdQuestion);
								CString ValuesStr = "";
								CIBClass * pClass = pTargetQuestion->GetClass();
								CIBNumValue * pNumValue = pTargetQuestion->GetNumValue();

								if (pClass)
									return true;
								else
									return false;
							}
						}
					}
				}
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////////
// Configuration du split vertical
void CDlgItems::ConfigSplit()
{
	// on récupère la position des fenêtres
	RECT RectV1; m_Box_Question.GetWindowRect(&RectV1); this->ScreenToClient(&RectV1);
	RECT RectV2; m_Box_Result.GetWindowRect(&RectV2); this->ScreenToClient(&RectV2);

	// Position gauche, droite
	LONG SPLeft		= RectV1.right - RectV1.left; if (SPLeft < 0) SPLeft = 0;
	LONG SPRight	= RectV2.right - RectV2.left; if (SPLeft < 0) SPLeft = 0;

#if !defined(BUILD_FOR_ATELIER_INTERNET)
	//on reconfigure le split
	//@@@ REPITEMS : ATELIER INTERNET
	m_SplitVert.SetRange(SPLeft, SPRight);
	m_SplitVert.ShowWindow(SW_SHOW);
#endif

}

/////////////////////////////////////////////////////////////////////////////
/// CDlgItems message handlers

void CDlgItems::InitRepItem() 
{
	// Mise à jour gestionnaire des questions
	UpdateTheme(IDC_RI_RAD_QUESTIONS);

	// Bidouille
	// Repositionne le split via dernière sauvegarde
	PosLastSplit();
}

void CDlgItems::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//Taille minimale de la boite 850x425  : Attention ici ça limite la largeur et hauteur de la fenetre
	/*
	if (m_TypeRepItem == true)
	{
		lpMMI->ptMinTrackSize.x = 800; 
		lpMMI->ptMinTrackSize.y = 450; 
	}
	else
	{
		lpMMI->ptMinTrackSize.x = 950; // 1200; Modif Juillet 2013 (taille minimump pour pupitre)
		lpMMI->ptMinTrackSize.y = 450; 
	}
	*/

	if (m_TypeRepItem == true)
	{
		lpMMI->ptMinTrackSize.x = 800; 
		lpMMI->ptMinTrackSize.y = 450; 
	}
	else
	{
		lpMMI->ptMinTrackSize.x = 600; // 1200; Modif Juillet 2013 (taille minimump pour pupitre)
		lpMMI->ptMinTrackSize.y = 450; 
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDlgItems::SetupControls()
{
	CString ToolTip;
	
	// Les boites du répertoires items
	m_Box_Theme.				SubclassDlgItem(IDC_RI_STATIC_THEME,			this); 
	m_Box_Question.				SubclassDlgItem(IDC_RI_STATIC_QUESTION,			this);
	m_Box_Dimension1.			SubclassDlgItem(IDC_RI_STATIC_DIMENSION1,		this);
	m_Box_Dimension2.			SubclassDlgItem(IDC_RI_STATIC_DIMENSION2,		this);
	m_Box_Items.				SubclassDlgItem(IDC_RI_STATIC_ITEMS,			this);
	m_Box_SAS.					SubclassDlgItem(IDC_RI_STATIC_SAS,				this);
	m_Box_Title.				SubclassDlgItem(IDC_RI_STATIC_TITLE,			this);
	m_Box_Source.				SubclassDlgItem(IDC_RI_STATIC_SRCTERRAIN,		this);
	m_Box_Segment.				SubclassDlgItem(IDC_RI_STATIC_SEGMENT,			this);
	m_Box_Result.				SubclassDlgItem(IDC_RI_STATIC_RESULT,			this);
	m_Box_Pupitre.				SubclassDlgItem(IDC_RI_STATIC_PUPITRE,			this);
	m_Box_Onglet_Pupitre.		SubclassDlgItem(IDC_RI_STATIC_ONGLET_PUPITRE,	this);

	m_Tree_Theme.				SubclassDlgItem(IDC_RI_TREE_THEME,				this);
	//m_Tree_Support.			SubclassDlgItem(IDC_RI_TREE_SUPPORTS,			this);
	m_List_Questions.			SubclassDlgItem(IDC_RI_LIST_QUESTIONS,			this);
	m_List_Dimension1.			SubclassDlgItem(IDC_RI_LIST_DIMENSION1,			this);

	m_Tree_Rubrique_Items.		SubclassDlgItem(IDC_RI_TREE_RUBRIC_ITEMS,		this); 
	m_Tree_Rubrique_Palettes.	SubclassDlgItem(IDC_RI_TREE_RUBRIC_PALETTES,	this); 
	m_List_Items.				SubclassDlgItem(IDC_RI_LIST_IITEMS,				this);
	m_List_Dimension2.			SubclassDlgItem(IDC_RI_LIST_DIMENSION2,			this);
	m_List_SAS.					SubclassDlgItem(IDC_RI_LIST_SAS,				this);
	m_Mode_Title.				SubclassDlgItem(IDC_RI_LABEL_TITLE,				this);
	m_Label_Questions.			SubclassDlgItem(IDC_RI_LABEL_QUESTIONS,			this);
	m_Label_Theme.				SubclassDlgItem(IDC_RI_LABEL_THEME,				this);
	m_Label_DefItem.			SubclassDlgItem(IDC_RI_LABEL_DEFITEM,			this);

	// Les composants compo palette
	m_List_Compo_Palette.		SubclassDlgItem(IDC_RI_LIST_COMPO_PALETTE,		this);

	// Les composants de la boite pupitre
	m_List_Row.					SubclassDlgItem(IDC_RI_LIST_LIGNES,				this);
	m_List_Col.					SubclassDlgItem(IDC_RI_LIST_COLONNES,			this);
	m_List_Page.				SubclassDlgItem(IDC_RI_LIST_PAGES,				this);

	m_List_Tri.					SubclassDlgItem(IDC_RI_LIST_TRI,				this);

	// Les libellés entete lignes, colonnes, pages
	m_Label_Lignes.				SubclassDlgItem(IDC_RI_LABEL_LIGNES,			this);
	m_Label_Colonnes.			SubclassDlgItem(IDC_RI_LABEL_COLONNES,			this);
	m_Label_Pages.				SubclassDlgItem(IDC_RI_LABEL_PAGES,				this);

	// Boutons ajout sites (seulement pour SQN-Profiling)
	m_Btn_SitesEnLigne.			SubclassDlgItem(IDC_RI_BTN_SITES_LIG,			this);		
	m_Btn_SitesEnColonne.		SubclassDlgItem(IDC_RI_BTN_SITES_COL,			this);		
	m_Btn_SitesEnPage.			SubclassDlgItem(IDC_RI_BTN_SITES_PAGE,			this);		


	// Bouton pour indiquer les items lignes et colones calculer en moyenne
	m_Button_Item_Moy_Lig.		SubclassDlgItem(IDC_RI_BUT_ITEM_MOY_LIG,	 	this);
	m_Button_Item_Moy_Col.		SubclassDlgItem(IDC_RI_BUT_ITEM_MOY_COL,	 	this);

	// Boutons tris ascendants et descendants sur sélection tris croisés
	m_Button_Ligne_UP.			SubclassDlgItem(IDC_RI_BUT_LIGNES_UP,			this);
	m_Button_Ligne_DOWN.		SubclassDlgItem(IDC_RI_BUT_LIGNES_DOWN,			this);
	m_Button_Colonne_UP.		SubclassDlgItem(IDC_RI_BUT_COLONNES_UP,			this);
	m_Button_Colonne_DOWN.		SubclassDlgItem(IDC_RI_BUT_COLONNES_DOWN,		this);
	m_Button_Page_UP.			SubclassDlgItem(IDC_RI_BUT_PAGES_UP,			this);
	m_Button_Page_DOWN.			SubclassDlgItem(IDC_RI_BUT_PAGES_DOWN,			this);

	// Les boutons tri sur sélection
	m_Btn_All_Lignes.			SubclassDlgItem(IDC_RI_BUT_ALL_LIGNES,			this);
	m_Btn_All_Colonnes.			SubclassDlgItem(IDC_RI_BUT_ALL_COLONNES,		this);
	m_Btn_All_Pages.			SubclassDlgItem(IDC_RI_BUT_ALL_PAGES,			this);

	// Dimensionnement boite compo palette
	CRect rect;
	m_List_Compo_Palette.GetClientRect(&rect);
	m_List_Compo_Palette.		InsertColumn(0, "");
    m_List_Compo_Palette.		SetColumnWidth(0, 2000);
	// m_List_Compo_Palette.EnableToolTips(false);
	m_List_Compo_Palette.EnableToolTips(true);


	// Dimensionnement liste lignes, colonnes, pages
	m_List_Row.GetClientRect(&rect);
	m_List_Row.		InsertColumn(0, "");
	m_List_Row.		SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_List_Row.EnableToolTips(false);

	m_List_Col.		InsertColumn(0, "");
	m_List_Col.		SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_List_Col.EnableToolTips(false);

	m_List_Page.	InsertColumn(0, "");
	m_List_Page.		SetColumnWidth(0, LVSCW_AUTOSIZE);
    // m_List_Page.		SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	// m_List_Page.		SetColumnWidth(0, 2000);
	m_List_Page.EnableToolTips(false);

	// Les contrôles de la boite résultat
	m_List_Result.			SubclassDlgItem(IDC_RI_LIST_ITEMS, this);

	// Dimensionnement colonne tableur résultat
	m_List_Result.GetClientRect(&rect);
	int nColInterval = rect.Width()/12;

	CString labelCol;
	LOADSTRING(labelCol, IDS_RI_COL_ITEMS_COMP);
	m_List_Result.InsertColumn(0, labelCol, LVCFMT_LEFT, rect.Width()-(4*nColInterval));
	LOADSTRING(labelCol, IDS_RI_COL_NBCAS);
	m_List_Result.InsertColumn(1, labelCol, LVCFMT_RIGHT, nColInterval*2);
	LOADSTRING(labelCol, IDS_RI_COL_EFF);
	m_List_Result.InsertColumn(2, labelCol, LVCFMT_RIGHT, nColInterval*2);

	m_List_Result.EnableToolTips(true);

	m_But_Down_Item.		SubclassDlgItem(IDC_RI_BTN_DOWN, this);
	m_But_Up_Item.			SubclassDlgItem(IDC_RI_BTN_UP, this);
	m_But_Imprimer_Result.	SubclassDlgItem(IDC_RI_BTN_IMPITEM, this);
	m_But_Export_Item.		SubclassDlgItem(IDC_RI_BTN_EXPORT_ITEM, this);
	m_But_Supp_Item.        SubclassDlgItem(IDC_RI_BTN_SUPP_ITEM, this);
	m_Header_Dimension2.	SubclassDlgItem(IDC_RI_LABEL_DIMENSION2,		this);
	m_Button_Dimension2.	SubclassDlgItem(IDC_RI_RAD_DIMENSION2,			this);
	m_Header_Dimension1.	SubclassDlgItem(IDC_RI_LABEL_DIMENSION1,		this);
	m_Button_Dimension1.	SubclassDlgItem(IDC_RI_RAD_DIMENSION1,			this);
	m_Button_Dim1_OK.		SubclassDlgItem(IDC_RI_PUSH_DIM1_OK,			this);
	m_Label_TypePoids.		SubclassDlgItem(IDC_RI_LABEL_TYPEPOIDS,			this);
	m_Label_Effectif.		SubclassDlgItem(IDC_RI_LABEL_EFFECTIF,			this);
	m_Label_NbCas.			SubclassDlgItem(IDC_RI_LABEL_NBCAS,				this);
	m_Label_Percent.		SubclassDlgItem(IDC_RI_LABEL_POURCENT,			this);
	m_Label_Source.			SubclassDlgItem(IDC_RI_LABEL_SOURCE,			this);
	m_Label_SAS.			SubclassDlgItem(IDC_RI_LABEL_SAS,				this); 
	m_Label_Item.			SubclassDlgItem(IDC_RI_LABEL_ITEM,				this); 
	m_Edit_Search.			SubclassDlgItem(IDC_RI_EDIT_SEARCH,				this); 
	m_Edit_QFilter.			SubclassDlgItem(IDC_RI_EDIT_QFILTER,			this); 

	m_Label_NbCells.		SubclassDlgItem(IDC_RI_LABEL_NBCELLS,			this);

	m_Cadre_Result.			SubclassDlgItem(IDC_RI_CADRE_RESULT,			this);

	m_Static_Effectif.		SubclassDlgItem(IDC_RI_STATIC_EFF,				this);
	m_Static_Percent.		SubclassDlgItem(IDC_RI_STATIC_PERCENT,			this);
	m_Static_NbCas.			SubclassDlgItem(IDC_RI_STATIC_NBCAS,			this);

	// Le bouton check pour rubrique par défaut 
	m_Chk_Rubrique_Defaut.	SubclassDlgItem(IDC_RI_CHK_RUBRIQUE_DEFAUT,		this);

	// Le liste sélection des rubriques
	m_ComboRubriques.		SubclassDlgItem(IDC_RI_COMBO_RUBRIQUES,			this);

	// Les boutons opérateurs sur le Sas des questions sélectionnées
	m_Button_Menu_Ope_QuSel.SubclassDlgItem(IDC_RI_BUT_MENU_OPE_QUSEL,		this);

	// Bouton déclenchant le menu opération sur le Sas
	m_Button_SAS_RAZ.		SubclassDlgItem(IDC_RI_BUT_SAS_RAZ,					this);

	m_Button_Menu_Ope_QuSel.SetTypeBtnMenu(TRUE,TRUE); 
	m_Button_SAS_ET.		SubclassDlgItem(IDC_RI_RAD_Q_ET,				this);
	m_Button_SAS_OU.		SubclassDlgItem(IDC_RI_RAD_Q_OU,				this);

	// Les boutons opérateurs sur les segments
	m_Button_Menu_Ope_Segment.SubclassDlgItem(IDC_RI_BUT_MENU_OPE_SEGMENT,	this);
	m_Button_Menu_Ope_Segment.SetTypeBtnMenu(TRUE,TRUE); 
	m_Button_Segment_ET.	SubclassDlgItem(IDC_RI_RAD_S_ET,				this);
	m_Button_Segment_OU.	SubclassDlgItem(IDC_RI_RAD_S_OU,				this);
	m_Button_Segment_RAZ.	SubclassDlgItem(IDC_RI_BUT_SEGMENT_RAZ,			this);

	// Les boutons opérateurs panel dimension 1
	m_Button_Dim1_ET.		SubclassDlgItem(IDC_RI_RAD_DIM1_ET,				this);
	m_Button_Dim1_OU.		SubclassDlgItem(IDC_RI_RAD_DIM1_OU,				this);
	m_Button_Dim1_NI.		SubclassDlgItem(IDC_RI_RAD_DIM1_NI,				this);
	m_Button_Dim1_XOR.		SubclassDlgItem(IDC_RI_RAD_DIM1_XOR,			this);
	
	// Bouton déclenchant le menu opération sur dimension 1
	m_Button_Menu_Ope_Dim1.	SubclassDlgItem(IDC_RI_BUT_MENU_OPE_DIM1,		this);
	m_Button_Menu_Ope_Dim1. SetTypeBtnMenu(TRUE,TRUE); 
	
	m_Button_Dim1_RAZ.		SubclassDlgItem(IDC_RI_BUT_DIM1_RAZ,			this);
	m_Button_Filtre.		SubclassDlgItem(IDC_RI_BUT_FILTRE,				this);
	m_Button_Dim1_UP.		SubclassDlgItem(IDC_RI_BUT_DIM1_UP,				this);
	m_Button_Dim1_DOWN.		SubclassDlgItem(IDC_RI_BUT_DIM1_DOWN,			this);

	// Les boutons opérateurs panel dimension 2
	m_Button_Dim2_ET.		SubclassDlgItem(IDC_RI_RAD_DIM2_ET,				this);
	m_Button_Dim2_OU.		SubclassDlgItem(IDC_RI_RAD_DIM2_OU,				this);
	m_Button_Dim2_NI.		SubclassDlgItem(IDC_RI_RAD_DIM2_NI,				this);
	m_Button_Dim2_XOR.		SubclassDlgItem(IDC_RI_RAD_DIM2_XOR,			this);

	// Bouton déclenchant le menu opération sur dimension 2
	m_Button_Menu_Ope_Dim2.	SubclassDlgItem(IDC_RI_BUT_MENU_OPE_DIM2,		this);
	m_Button_Menu_Ope_Dim2.	SetTypeBtnMenu(TRUE,TRUE); 
	
	m_Button_Dim2_RAZ.		SubclassDlgItem(IDC_RI_BUT_DIM2_RAZ,			this);
	m_Button_CalcSegment.	SubclassDlgItem(IDC_RI_CHECK_CALC_SEGMENT,		this);
	m_Button_Accept.		SubclassDlgItem(IDC_RI_BUT_ACCEPT,				this);

	
	// Associe le menu suppression
	if (m_TypeRepItem == true)
	{
		// Les suppressions pour répertoire des items
		m_Button_Suppression.	SubclassDlgItem(IDC_RI_BUT_SUPPRESSION,			this);
		m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPPRESSION, this, FALSE);

		// XSXS MENU

		CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
		m_Button_Suppression.SetWindowText(labelB); 
	}
	else
	{	
		// Les suppressions pour sélection dans pupitre
		m_Button_Suppression.	SubclassDlgItem(IDC_RI_BUT_SUPPRESSION,			this);
		m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_SELPUPITRE, this, FALSE);
		CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
		m_Button_Suppression.SetWindowText(labelB); 
	}
	// Bouton déclenchant le menu opération pour les différentes suppressions
	m_Button_Suppression.SetTypeBtnMenu(TRUE,TRUE); 

	m_Button_Genese.		SubclassDlgItem(IDC_RI_BUT_GENESE,				this);
	m_Button_OK.			SubclassDlgItem(IDOK,							this);
	m_Button_Cancel.		SubclassDlgItem(IDCANCEL,						this);
	
	// Bouton titre en haut
	m_Button_Title.			SubclassDlgItem(IDC_RI_RAD_TITLE,				this);
	m_Button_Title.			SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));

	// 1er bouton visualisation questions
	m_Button_TreeQuestions.	SubclassDlgItem(IDC_RI_RAD_QUESTIONS,			this);
	m_Button_TreeQuestions.	SetTypeOnglet(TRUE,TRUE); 

	// 2eme bouton en bas 1er fenêtre
	m_Button_List1.			SubclassDlgItem(IDC_RI_RAD_LIST_1,				this);
	m_Button_List1.			SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));
	m_Button_List1.			SetTypeOnglet(TRUE,TRUE); 

	// 3eme bouton en bas 1re fenêtre
	m_Button_List2.			SubclassDlgItem(IDC_RI_RAD_LIST_2,				this);
	m_Button_List2.			SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));
	m_Button_List2.			SetTypeOnglet(TRUE,TRUE); 

	// Bouton Transfert sélection ligne
	m_Button_Mode_Ligne.	SubclassDlgItem(IDC_RI_RAD_MODE_LIGNES,				this);	
	m_Button_Mode_Ligne.	SetTypeOnglet(FALSE,TRUE,TRUE);
	m_Button_Mode_Ligne.	SetTypeOnglet(FALSE,FALSE,TRUE);
	m_Button_Mode_Ligne.	SetTypeFleche(FALSE,TRUE);
		
	// Bouton Transfert sélection colonne
	m_Button_Mode_Colonne.	SubclassDlgItem(IDC_RI_RAD_MODE_COLONNES,			this);	
	m_Button_Mode_Colonne.	SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Colonne.	SetTypeFleche(FALSE,TRUE);

	// Bouton Transfert sélection page
	m_Button_Mode_Page.		SubclassDlgItem(IDC_RI_RAD_MODE_PAGES,				this);	
	m_Button_Mode_Page.		SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Page.		SetTypeFleche(FALSE,TRUE);

	m_Button_RAZ_search.	SubclassDlgItem(IDC_RI_BUT_RAZSEARCH,			this);

	m_Button_QFilter.		SubclassDlgItem(IDC_RI_BUT_QFILTER,				this);
	m_Button_QFilterRaz.	SubclassDlgItem(IDC_RI_BUT_QFILTERRAZ,			this);
	m_Tab_Segment.			SubclassDlgItem(IDC_RI_TAB_SEGMENT,				this);
	m_Button_Questions_Tous.SubclassDlgItem(IDC_RI_BUT_QUESTION_TOUS,		this);
	m_Edit_Name.			SubclassDlgItem(IDC_RI_EDIT_NAME,				this);
	

	/*
	m_Button_QFilter.SetMenu(IDR_RI_MENU_QFILTER, this, false);

	LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFQUESTIONS);
	LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFMODALITES);
	LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFTOUS);
	*/

	// ToolTip sur les boutons imprssion et export
	LOADSTRING(ToolTip, IDS_RI_IMP_ITEM); 
	m_But_Imprimer_Result.SetTooltipText(ToolTip) ;

	LOADSTRING(ToolTip, IDS_RI_EXPORT_ITEM); 
	m_But_Export_Item.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_SUPP_ITEM); 
	m_But_Supp_Item.SetTooltipText(ToolTip);

	// ToolTip sur les opérateurs panel segments
	LOADSTRING(ToolTip, IDS_RI_TT_ET_S); 
	m_Button_Segment_ET.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_OU_S); 
	m_Button_Segment_OU.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_BUT_OPE_SEGMENT); 
	m_Button_Menu_Ope_Segment.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_RAZ_SEG); 
	m_Button_Segment_RAZ.SetTooltipText(ToolTip);
	m_Button_Segment_RAZ.SetIcon(IDI_RI_RAZ,	IDI_RI_RAZ); 
	
	// ToolTip sur les opérateurs panel sas des questions sélectionnées
	LOADSTRING(ToolTip, IDS_RI_TT_RAZ_S); 
	m_Button_SAS_RAZ.	SetTooltipText(ToolTip);
	m_Button_SAS_RAZ.   SetIcon(IDI_RI_RAZ,	IDI_RI_RAZ); 

	LOADSTRING(ToolTip, IDS_RI_TT_ET_S); 
	m_Button_SAS_ET.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_OU_S); 
	m_Button_SAS_OU.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_BUT_OPE_SELQUESTION);
	m_Button_Menu_Ope_QuSel.SetTooltipText(ToolTip);
	
	// Tooltip sur les opérateurs panel dimension 1
	LOADSTRING(ToolTip, IDS_RI_TT_ET_M); 
	m_Button_Dim1_ET.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_OU_M); 
	m_Button_Dim1_OU.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_NI_M); 
	m_Button_Dim1_NI.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_XOR_M); 
	m_Button_Dim1_XOR.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_BUT_OPE_DIM1); 
	m_Button_Menu_Ope_Dim1.SetTooltipText(ToolTip);

	// Les autres boutons dimension 1 (recalage up/down, sélection ou déselection all)
	//
	LOADSTRING(ToolTip, IDS_RI_ITEM_FILTRE); 
	m_Button_Filtre.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_ITEM_UP); 
	m_Button_Dim1_UP.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_ITEM_DOWN); 
	m_Button_Dim1_DOWN.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_T_M); 
	m_Button_Dim1_RAZ.SetTooltipText(ToolTip);
	
	// Les infos pour opérateurs sur dimension 2
	LOADSTRING(ToolTip, IDS_RI_TT_ET_E); 
	m_Button_Dim2_ET.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_OU_E); 
	m_Button_Dim2_OU.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_NI_E); 
	m_Button_Dim2_NI.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_XOR_E); 
	m_Button_Dim2_XOR.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_BUT_OPE_DIM1); 
	m_Button_Menu_Ope_Dim2.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_T_E); 
	m_Button_Dim2_RAZ.SetTooltipText(ToolTip);
	LOADSTRING(ToolTip, IDS_RI_TT_SEGMENT); 
	m_Button_CalcSegment.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_SUPPRESSION); 
	m_Button_Suppression.SetTooltipText(ToolTip);
	
	LOADSTRING(ToolTip, IDS_RI_TT_QUESTION); 
	m_Button_TreeQuestions.SetTooltipText(ToolTip);

	// Entete titre mode items ou mode sélection tris croisés
	m_Mode_Title.SetFontName( CATPColors::GetFontLabelName());
	m_Mode_Title.SetFontSize(12);
	m_Mode_Title.SetFontBold(true);
	m_Mode_Title.SetBorder(false);
	
	// Entre Source/Terrain + Type de poids
	m_Label_Source.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Source.SetFontSize(12);
	m_Label_Source.SetFontBold(true);
	m_Label_Source.SetBorder(false);
	// m_Label_Source.SetBorder(true);
	// m_Label_Source.SetBorderColor(CATPColors::GetColorLight(m_CATPColorsApp));

	m_Label_Questions.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Questions.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Questions.SetBorder(false);

	m_Header_Dimension1.SetFontName(CATPColors::GetFontLabelName());
	m_Header_Dimension1.SetFontSize(CATPColors::GetFontLabelSize());
	m_Header_Dimension1.SetBorder(false);

	m_Label_SAS.SetFontName(CATPColors::GetFontLabelName());
	m_Label_SAS.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_SAS.SetBorder(false);

	m_Label_Theme.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Theme.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Theme.SetBorder(false);

	m_Label_DefItem.SetFontName(CATPColors::GetFontLabelName());
	m_Label_DefItem.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_DefItem.SetBorder(false);

	// Paramétrage composants boite résultat
	// rien pour la liste
	// m_ListResult
	//LOADSTRING(ToolTip, IDS_RI_TT_ET_S); 
	// m_ButtonImprimer.SetTooltipText(ToolTip);


	m_Header_Dimension2.SetFontName(CATPColors::GetFontLabelName());
	m_Header_Dimension2.SetFontSize(CATPColors::GetFontLabelSize());
	m_Header_Dimension2.SetBorder(false);

	m_Label_TypePoids.SetFontName(CATPColors::GetFontLabelName());
	m_Label_TypePoids.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_TypePoids.SetBorder(false);

	m_Label_Effectif.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Effectif.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Effectif.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Effectif.SetBorder(false);

	m_Label_NbCas.SetFontName(CATPColors::GetFontStaticName());
	m_Label_NbCas.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_NbCas.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_NbCas.SetBorder(false);

	m_Label_Percent.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Percent.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Percent.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Percent.SetBorder(false);

	m_Static_Effectif.SetFontName(CATPColors::GetFontStaticName());
	m_Static_Effectif.SetFontSize(CATPColors::GetFontStaticSize());
	m_Static_Effectif.SetText(_T(""));

	m_Static_NbCas.SetFontName(CATPColors::GetFontStaticName());
	m_Static_NbCas.SetFontSize(CATPColors::GetFontStaticSize());
	// m_Static_NbCas.SetBorder(true);
	m_Static_NbCas.SetText(_T(""));

	m_Static_Percent.SetFontName(CATPColors::GetFontStaticName());
	m_Static_Percent.SetFontSize(CATPColors::GetFontStaticSize());
	// m_Static_Percent.SetBorder(true);
	m_Static_Percent.SetText(_T(""));

	// m_Button_CalcSegment.SetBitmaps(IDB_RI_SEGMENTS, RGB(192,192,192), IDB_RI_SEGMENT, RGB(192,192,192));
	m_Button_CalcSegment.SetBitmaps(IDB_RI_SEGMENT, RGB(192,192,192), IDB_RI_SEGMENTS, RGB(192,192,192));

	// Font et aspect entete lignes
	m_Label_Lignes.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Lignes.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Lignes.SetBorder(false);

	// Font et aspect entete colonnes
	m_Label_Colonnes.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Colonnes.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Colonnes.SetBorder(false);

	// Font et aspect entete pages
	m_Label_Pages.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Pages.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Pages.SetBorder(false);

	// Font et aspect titre libellé item
	m_Label_Item.SetFontName(CATPColors::GetFontStaticName());
	m_Label_Item.SetFontSize(CATPColors::GetFontStaticSize());
	m_Label_Item.SetBorder(false);

	// Initialisation boite tree items et de ses contrôles
	m_Box_Theme.Reset();
	m_Box_Theme.SetTransparent(false);
	m_Box_Theme.SetSunken(true);
	m_Box_Theme.AddTopWindow(m_Label_Theme);
	m_Box_Theme.AddTopWindow(m_Tree_Theme);
	m_Box_Theme.AddTopWindow(m_Tree_Rubrique_Items);
	m_Box_Theme.AddTopWindow(m_Tree_Rubrique_Palettes);
	m_Box_Theme.AddTopWindow(m_List_Items);
	m_Box_Theme.AddTopWindow(m_Button_TreeQuestions);
	m_Box_Theme.AddTopWindow(m_Button_Title);
	m_Box_Theme.AddTopWindow(m_Button_List1);
	m_Box_Theme.AddTopWindow(m_Button_List2);
	m_Box_Theme.AddTopWindow(m_Button_Genese);
	m_Box_Theme.AddTopWindow(m_Edit_Search);
	m_Box_Theme.AddTopWindow(m_Edit_QFilter);
	m_Box_Theme.AddTopWindow(m_Button_QFilter);
	m_Box_Theme.AddTopWindow(m_Button_QFilterRaz);
	m_Box_Theme.AddTopWindow(m_Button_RAZ_search);

	// Initialisation boite résultat et de ses contrôles
	m_Box_Result.Reset();
	m_Box_Result.SetTransparent(false);
	m_Box_Result.SetSunken(true);
	m_Box_Result.AddTopWindow(m_Label_DefItem);
	m_Box_Result.AddTopWindow(m_List_Result);
	m_Box_Result.AddTopWindow(m_But_Imprimer_Result);

	m_Box_Result.AddTopWindow(m_But_Down_Item);
	m_Box_Result.AddTopWindow(m_But_Up_Item);

	// NEW 2.7
	m_Box_Result.AddTopWindow(m_But_Export_Item);

	m_Box_Result.AddTopWindow(m_But_Supp_Item);
	m_But_Supp_Item.ShowWindow(FALSE);
	m_Box_Result.AddTopWindow(m_Label_TypePoids);
	m_Box_Result.AddTopWindow(m_Button_CalcSegment);
	m_Box_Result.AddTopWindow(m_Cadre_Result);
	m_Box_Result.AddTopWindow(m_Label_Effectif);
	m_Box_Result.AddTopWindow(m_Label_NbCas);
	m_Box_Result.AddTopWindow(m_Label_Percent);
	m_Box_Result.AddTopWindow(m_Static_Effectif);
	m_Box_Result.AddTopWindow(m_Static_Percent);
	m_Box_Result.AddTopWindow(m_Static_NbCas);
	m_Box_Result.AddTopWindow(m_Chk_Rubrique_Defaut); 
	m_Box_Result.AddTopWindow(m_ComboRubriques); 

	// + liste compo palette
	m_Box_Result.AddTopWindow(m_List_Compo_Palette);

	// + controles résultat eff / nb cas /etc....

	// Initialisation boite pupitre
	m_Box_Pupitre.Reset();
	m_Box_Pupitre.SetTransparent(false);
	m_Box_Pupitre.SetSunken(true);

	// Ajout des contôles appartenant à la boite pupitre
	m_Box_Pupitre.AddTopWindow(m_Label_Lignes);
	m_Box_Pupitre.AddTopWindow(m_List_Row);
	m_Box_Pupitre.AddTopWindow(m_Label_Colonnes);
	m_Box_Pupitre.AddTopWindow(m_List_Col);
	m_Box_Pupitre.AddTopWindow(m_Label_Pages);
	m_Box_Pupitre.AddTopWindow(m_List_Page);
	m_Box_Pupitre.AddTopWindow(m_Button_Item_Moy_Lig);
	m_Box_Pupitre.AddTopWindow(m_Button_Item_Moy_Col);
	m_Box_Pupitre.AddTopWindow(m_Button_Ligne_UP);
	m_Box_Pupitre.AddTopWindow(m_Button_Ligne_DOWN);
	m_Box_Pupitre.AddTopWindow(m_Button_Colonne_UP);
	m_Box_Pupitre.AddTopWindow(m_Button_Colonne_DOWN);
	m_Box_Pupitre.AddTopWindow(m_Button_Page_UP);
	m_Box_Pupitre.AddTopWindow(m_Button_Page_DOWN);
	m_Box_Pupitre.AddTopWindow(m_Btn_All_Lignes);
	m_Box_Pupitre.AddTopWindow(m_Btn_All_Colonnes);
	m_Box_Pupitre.AddTopWindow(m_Btn_All_Pages);

	// Ajout des boutons ajout sites pour SQN-Profiling
	m_Box_Pupitre.AddTopWindow(m_Btn_SitesEnLigne);
	m_Box_Pupitre.AddTopWindow(m_Btn_SitesEnColonne);
	m_Box_Pupitre.AddTopWindow(m_Btn_SitesEnPage);

	// Initialisation autre boite
	m_Box_Question.Reset();
	m_Box_Question.SetTransparent(false);
	m_Box_Question.SetSunken(true);
	m_Box_Question.AddTopWindow(m_Button_Questions_Tous);
	m_Box_Question.AddTopWindow(m_Label_Questions);
	m_Box_Question.AddTopWindow(m_List_Questions);

	// Association des objets du panel dimension 1
	m_Box_Dimension1.Reset();
	m_Box_Dimension1.SetTransparent(false);
	m_Box_Dimension1.SetSunken(true);
	m_Box_Dimension1.AddTopWindow(m_Header_Dimension1);
	m_Box_Dimension1.AddTopWindow(m_Button_Dimension1);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_OK);
	m_Box_Dimension1.AddTopWindow(m_List_Dimension1);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_ET);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_OU);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_NI);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_XOR);
	m_Box_Dimension1.AddTopWindow(m_Button_Menu_Ope_Dim1);
	m_Box_Dimension1.AddTopWindow(m_Button_Filtre);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_UP);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_DOWN);
	m_Box_Dimension1.AddTopWindow(m_Button_Dim1_RAZ);

	// Association des objets du panel dimension 2
	m_Box_Dimension2.Reset();
	m_Box_Dimension2.SetTransparent(false);
	m_Box_Dimension2.SetSunken(true);
	m_Box_Dimension2.AddTopWindow(m_Header_Dimension2);
	m_Box_Dimension2.AddTopWindow(m_Button_Dimension2);
	m_Box_Dimension2.AddTopWindow(m_List_Dimension2);
	m_Box_Dimension2.AddTopWindow(m_Button_Dim2_ET);
	m_Box_Dimension2.AddTopWindow(m_Button_Dim2_OU);
	m_Box_Dimension2.AddTopWindow(m_Button_Dim2_NI);
	m_Box_Dimension2.AddTopWindow(m_Button_Dim2_XOR);
	m_Box_Dimension2.AddTopWindow(m_Button_Menu_Ope_Dim2);
	m_Box_Dimension2.AddTopWindow(m_Button_Dim2_RAZ);

	// Association des objets du panel vérifaction / validation
	m_Box_Items.Reset();
	m_Box_Items.SetTransparent(false);
	m_Box_Items.SetSunken(true);
	m_Box_Items.AddTopWindow(m_Button_Suppression);
	m_Box_Items.AddTopWindow(m_Button_Accept);
	m_Box_Items.AddTopWindow(m_Edit_Name);
	m_Box_Items.AddTopWindow(m_Label_Item);

	m_Box_Items.AddTopWindow(m_Button_Mode_Ligne);
	m_Box_Items.AddTopWindow(m_Button_Mode_Colonne);
	m_Box_Items.AddTopWindow(m_Button_Mode_Page);

	// Association des objets du panel SAS
	m_Box_SAS.Reset();
	m_Box_SAS.SetTransparent(false);
	m_Box_SAS.SetSunken(true);
	m_Box_SAS.AddTopWindow(m_Label_SAS);
	m_Box_SAS.AddTopWindow(m_List_SAS);
	m_Box_SAS.AddTopWindow(m_Button_Menu_Ope_QuSel);
	m_Box_SAS.AddTopWindow(m_Button_SAS_ET);
	m_Box_SAS.AddTopWindow(m_Button_SAS_OU);
	m_Box_SAS.AddTopWindow(m_Button_SAS_RAZ);

	// Association des objets du panel titre
	m_Box_Title.Reset();
	m_Box_Title.SetTransparent(false);
	m_Box_Title.SetSunken(true);
	m_Box_Title.AddTopWindow(m_Mode_Title);

	// Association des objets du panel titre source
	m_Box_Source.Reset();
	m_Box_Source.SetTransparent(false);
	m_Box_Source.SetSunken(true);
	m_Box_Source.AddTopWindow(m_Label_Source);

	// Association des objets du panel gestion segment
	m_Box_Segment.Reset();
	m_Box_Segment.SetTransparent(false);
	m_Box_Segment.SetSunken(true);
	m_Box_Segment.AddTopWindow(m_Tab_Segment);
	m_Box_Segment.AddTopWindow(m_Button_Menu_Ope_Segment);
	m_Box_Segment.AddTopWindow(m_Button_Segment_RAZ);

	m_Box_SAS.AddTopWindow(m_Button_Segment_OU);
	m_Box_SAS.AddTopWindow(m_Button_Segment_ET);


	// m_Box_Segment.AddTopWindow(m_Button_CalcSegment);

	m_Button_OK.ShowWindow(SW_SHOW);  
	m_Button_Cancel.ShowWindow(SW_SHOW);
	m_Label_NbCells.ShowWindow(SW_SHOW);

	// Les elmts barre inférieure pupitre
	m_Box_Onglet_Pupitre.Reset();
	m_Box_Onglet_Pupitre.SetTransparent(false);
	m_Box_Onglet_Pupitre.SetSunken(false);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Button_Mode_Ligne);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Button_Mode_Colonne);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Button_Mode_Page);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Button_OK);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Button_Cancel);
	m_Box_Onglet_Pupitre.AddTopWindow(m_Label_NbCells);

	// Les onglets pupitre
	m_Box_Onglet_Pupitre.ShowWindow(SW_SHOW);
	m_Button_Mode_Ligne.ShowWindow(SW_SHOW);
	m_Button_Mode_Colonne.ShowWindow(SW_SHOW);
	m_Button_Mode_Page.ShowWindow(SW_SHOW);

	// Font onglet sélection ligne
	m_Button_Mode_Ligne.SetFontName(CATPColors::GetFontLabelName());
	// m_Button_Mode_Ligne.SetFontSize(10);
	// m_Button_Mode_Ligne.SetFontSize(8);
	m_Button_Mode_Ligne.SetFontSize(CATPColors::GetFontStaticSize());
	m_Button_Mode_Ligne.SetFontBold(false);

	// Font onglet sélection colonne
	m_Button_Mode_Colonne.SetFontName(CATPColors::GetFontLabelName());
	// m_Button_Mode_Colonne.SetFontSize(10);
	m_Button_Mode_Colonne.SetFontSize(CATPColors::GetFontStaticSize());
	m_Button_Mode_Colonne.SetFontBold(false);

	// Font onglet sélection page
	m_Button_Mode_Page.SetFontName(CATPColors::GetFontLabelName());
	// m_Button_Mode_Page.SetFontSize(10);
	m_Button_Mode_Page.SetFontSize(CATPColors::GetFontStaticSize());
	m_Button_Mode_Page.SetFontBold(false);

	// Les boutons Ok et Cancel
	m_Button_OK.ShowWindow(SW_SHOW);
	m_Button_Cancel.ShowWindow(SW_SHOW);

	SetColors();
	SetUpdateOpeLogiques();

	// Set window relations and positions for resizing
	SetWindowRelations();

#if defined(BUILD_FOR_ATELIER_INTERNET)

	SETDLGITEMTEXT(IDC_RI_BUT_RAZSEARCH);
	SETDLGITEMTEXT(IDC_RI_LABEL_POURCENT);
	SETDLGITEMTEXT(IDC_RI_LABEL_POURCENT);
	SETDLGITEMTEXT(IDC_RI_RAD_MODE_LIGNES);
	SETDLGITEMTEXT(IDC_RI_RAD_MODE_COLONNES);
	SETDLGITEMTEXT(IDC_RI_RAD_MODE_PAGES);
	SETDLGITEMTEXT(IDC_RI_LABEL_SAS);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM1_OU);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM1_ET);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM1_XOR);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM1_NI);
	SETDLGITEMTEXT(IDC_RI_RAD_Q_OU);
	SETDLGITEMTEXT(IDC_RI_LABEL_DIMENSION1);
	SETDLGITEMTEXT(IDC_RI_BUT_DIM1_RAZ);
	SETDLGITEMTEXT(IDC_RI_TREE_THEME);
	SETDLGITEMTEXT(IDC_RI_RAD_S_ET);
	SETDLGITEMTEXT(IDC_RI_RAD_S_OU);
	SETDLGITEMTEXT(IDC_RI_BUT_MENU_OPE_SEGMENT);
	SETDLGITEMTEXT(IDC_RI_BUT_MENU_OPE_QUSEL);
	SETDLGITEMTEXT(IDC_RI_RAD_Q_ET);
	SETDLGITEMTEXT(IDC_RI_BUT_MENU_OPE_DIM2);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM2_OU);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM2_ET);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM2_XOR);
	SETDLGITEMTEXT(IDC_RI_RAD_DIM2_NI);
	SETDLGITEMTEXT(IDC_RI_BUT_MENU_OPE_DIM1);
	SETDLGITEMTEXT(IDC_RI_RAD_QUESTIONS);
	SETDLGITEMTEXT(IDC_RI_RAD_LIST_1);
	SETDLGITEMTEXT(IDC_RI_RAD_LIST_2);
	SETDLGITEMTEXT(IDC_RI_TAB_SEGMENT);
	SETDLGITEMTEXT(IDC_RI_BUT_DIM2_RAZ);
	SETDLGITEMTEXT(IDC_RI_BUT_ACCEPT);
	SETDLGITEMTEXT(IDC_RI_BUT_GENESE);
	SETDLGITEMTEXT(IDC_RI_LABEL_TITLE);
	SETDLGITEMTEXT(IDC_RI_LABEL_QUESTIONS);
	SETDLGITEMTEXT(IDC_RI_LABEL_NBCAS);
	SETDLGITEMTEXT(IDC_RI_LABEL_EFFECTIF);
	SETDLGITEMTEXT(IDC_RI_LABEL_DIMENSION2);
	SETDLGITEMTEXT(IDC_RI_CHECK_CALC_SEGMENT);
	SETDLGITEMTEXT(IDC_RI_STATIC_EFF);
	SETDLGITEMTEXT(IDC_RI_STATIC_PERCENT);
	SETDLGITEMTEXT(IDC_RI_STATIC_NBCAS);
	SETDLGITEMTEXT(IDC_RI_LABEL_SOURCE);
	SETDLGITEMTEXT(IDC_RI_BUT_RAZSEARCH);
	SETDLGITEMTEXT(IDC_RI_RAD_DIMENSION1);
	SETDLGITEMTEXT(IDC_RI_RAD_DIMENSION2);
	SETDLGITEMTEXT(IDC_RI_PUSH_DIM1_OK);
	//SETDLGITEMTEXT(IDOK);
	//SETDLGITEMTEXT(IDCANCEL);
	SETDLGITEMTEXT(IDC_RI_LABEL_TYPEPOIDS);
	SETDLGITEMTEXT(IDC_RI_BUT_QUESTION_TOUS);
	SETDLGITEMTEXT(IDC_RI_BUT_QFILTER);
	SETDLGITEMTEXT(IDC_RI_BUT_QFILTERRAZ);
	SETDLGITEMTEXT(IDC_RI_LABEL_THEME);
	SETDLGITEMTEXT(IDC_RI_LABEL_DEFITEM);
	SETDLGITEMTEXT(IDC_RI_BUT_SUPPRESSION);
	SETDLGITEMTEXT(IDC_RI_LABEL_LIGNES);
	SETDLGITEMTEXT(IDC_RI_LABEL_COLONNES);
	SETDLGITEMTEXT(IDC_RI_LABEL_PAGES);
	SETDLGITEMTEXT(IDC_RI_BTN_SITES_PAGE);
	SETDLGITEMTEXT(IDC_RI_BTN_SITES_COL);
	SETDLGITEMTEXT(IDC_RI_BUT_ALL_COLONNES);
	SETDLGITEMTEXT(IDC_RI_BUT_ALL_PAGES);
	SETDLGITEMTEXT(IDC_RI_BTN_SITES_LIG);
	SETDLGITEMTEXT(IDC_RI_BUT_ALL_LIGNES);
	SETDLGITEMTEXT(IDC_RI_CHK_RUBRIQUE_DEFAUT);

#endif

}

void CDlgItems::SetColors()
{
	CString ToolTip;

	m_CATPColorsApp = CATPColors::COLORREPITEMS;

	m_List_Dimension1.		SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Items.			SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Dimension2.		SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_SAS.				SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));

	// La liste des composants palette
	m_List_Compo_Palette.	SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	
	// Les listes sélections pupitre
	m_List_Row.				SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Col.				SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Page.			SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));

	// Au départ c'est la sélection ligne qui est active
	m_List_Row.				SetBkColor(CATPColors::GetColorWhite());
	m_List_Row.				SetBackColor(CATPColors::GetColorWhite());
	m_List_Col.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
	m_List_Col.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
	m_List_Page.			SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
	m_List_Page.			SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
	
	m_List_Questions.		SetSelectionColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Questions.		SetQuestionTextColor(CATPColors::GetColorBlack());
	m_List_Questions.		SetBlocTextColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_List_Questions.		SetQuestionNumTextColor(CATPColors::GetColorBlack());
	m_List_Questions.		SetMarqueColor(CATPColors::GetColorLight(m_CATPColorsApp));	

	// Bouton sélection opérateurs pour SAS des questions sélectionnées
	m_Button_Menu_Ope_QuSel.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Menu_Ope_QuSel.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_QuSel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_QuSel.SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_SAS_ET.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_SAS_ET.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_SAS_ET.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_SAS_OU.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_SAS_OU.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_SAS_OU.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Bouton effacement SAS
	m_Button_SAS_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_SAS_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_SAS_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	
	// Bouton sélection opérateurs pour segments
	m_Button_Menu_Ope_Segment.SetColor(CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Menu_Ope_Segment.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Segment.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Segment.SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);

	// Bouton RAZ Current Segment
	m_Button_Segment_RAZ.SetColor(CButtonST::BTNST_COLOR_BK_IN,			CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Segment_RAZ.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Segment_RAZ.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Segment_RAZ.SetColor(CButtonST::BTNST_COLOR_SEL,			CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Segment_ET.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Segment_ET.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Segment_ET.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Segment_OU.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Segment_OU.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Segment_OU.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	
	// Bouton pour dimension 1
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack(), m_CATPColorsApp);
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_FG_OUT,		RGB(255,255,0), m_CATPColorsApp);
	m_Button_Dimension1.	SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0), m_CATPColorsApp);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack(), m_CATPColorsApp);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_FG_OUT,		RGB(255,255,0), m_CATPColorsApp);
	m_Button_Dim1_OK.		SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0), m_CATPColorsApp);
	m_Button_Dim1_ET.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_ET.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_ET.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_OU.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_OU.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_OU.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_NI.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_NI.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_NI.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Bouton sélection opérateur pour dimension 1
	m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Dim1. SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Dim1_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Filtre.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Filtre.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Filtre.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Filtre.		SetIcon(IDI_ICON8,16,16);

	m_Button_Dim1_UP.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_UP.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_UP.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_UP.		SetBitmaps(IDB_RI_UP,						RGB(192,192,192), m_CATPColorsApp); 
	
	m_Button_Dim1_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim1_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim1_DOWN.		SetBitmaps(IDB_RI_DOWN,						RGB(192,192,192)); 
		
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack(), m_CATPColorsApp);
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_FG_OUT,		RGB(255,255,0), m_CATPColorsApp);
	m_Button_Dimension2.	SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0), m_CATPColorsApp);

	// Bouton sélection opérateur pour dimension 2
	m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Menu_Ope_Dim2. SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Dim2_ET.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_ET.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_ET.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_OU.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_OU.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_OU.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_NI.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_NI.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_NI.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_XOR.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Dim2_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Dim2_RAZ.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_CalcSegment.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_CalcSegment.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_CalcSegment.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Accept.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Accept.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Accept.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Suppression.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Suppression.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Suppression.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Suppression.	SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	

	m_Button_Genese.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp, 0);
	m_Button_Genese.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Genese.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	
	// Couleur des onglets sélection mode questions / items / palettes
	m_Button_TreeQuestions.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_TreeQuestions.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_TreeQuestions.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_List1.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_List1.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_List1.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_List2.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_List2.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_List2.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_OK.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_OK.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_OK.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Cancel.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Cancel.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Cancel.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Couleur des boutons mode pupitre
	m_Button_Questions_Tous.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Questions_Tous.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Questions_Tous.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_RAZ_search.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_RAZ_search.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_RAZ_search.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_QFilter.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_QFilter.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_QFilter.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_QFilterRaz.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_QFilterRaz.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_QFilterRaz.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Couleur de fond des boites répertoire item
	m_Box_Source.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Title.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_SAS.				SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Items.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Dimension2.		SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Dimension1.		SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Question.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Theme.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Segment.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Result.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Pupitre.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);

	m_Static_Percent.		SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Static_NbCas.			SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Static_Effectif.		SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);

	m_Label_NbCas.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Label_Effectif.		SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Label_Percent.		SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);

	m_Label_NbCas.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Effectif.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Percent.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));

	m_Label_Questions.		SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Questions.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Questions.		SetFontBold(true);

	m_Label_SAS.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_SAS.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Label_SAS.			SetFontBold(true);

	m_Label_TypePoids.		SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_TypePoids.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_TypePoids.		SetFontBold(true);

	m_Header_Dimension1.	SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Header_Dimension1.	SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Header_Dimension1.	SetFontBold(true);

	m_Label_Theme.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Theme.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Label_Theme.			SetFontBold(true);

	// Label entete résultat
	m_Label_DefItem.		SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_DefItem.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_DefItem.		SetFontBold(true);

	// Label item entete libellé item
	m_Label_Item.			SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::DARK_DEGRAD);
	m_Label_Item.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Item.			SetFontBold(false);

	// Bouton down composition item
	m_But_Down_Item.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Down_Item.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Down_Item.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Down_Item.		SetBitmaps(IDB_RI_DOWN,RGB(192,192,192)); 

	// Bouton up composition item
	m_But_Up_Item.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Up_Item.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Up_Item.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Up_Item.			SetBitmaps(IDB_RI_UP,RGB(192,192,192)); 

	// Bouton impression résultat
	m_But_Imprimer_Result.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Imprimer_Result.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Imprimer_Result.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Imprimer_Result.	SetBitmaps(IDB_RI_IMP,						RGB(192,192,192)); 
	
	// Bouton export composition item
	m_But_Export_Item.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Export_Item.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Export_Item.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Export_Item.		SetBitmaps(IDB_RI_EXPORT_ITEM,				RGB(192,192,192)); 

	// Bouton suppression composition item
	m_But_Supp_Item.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Supp_Item.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_But_Supp_Item.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_But_Supp_Item.		SetIcon(IDI_RI_RAZ,	IDI_RI_RAZ); 

	m_Header_Dimension2.	SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Header_Dimension2.	SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Header_Dimension2.	SetFontBold(true);

	m_Mode_Title.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Mode_Title.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Mode_Title.			SetFontBold(true);

	m_Button_Title.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp, 0);
	m_Button_Title.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp, 0);
	m_Button_Title.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorSelect(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Title.DrawBorder(FALSE);
	
	m_Label_Source.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp,CATPColors::DARK_DEGRAD);
	m_Label_Source.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_Source.			SetFontBold(true);	

	m_Tab_Segment.			SetColorBk(CATPColors::GetColorMedium(m_CATPColorsApp));

	m_Tree_Theme.			m_CATPColorsApp = m_CATPColorsApp;
	m_Tree_Theme.			SetColors();

	// Couleurs dans boite sélection ligne, colonnes, pages
	m_Label_Lignes.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Lignes.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Label_Lignes.			SetFontBold(true);

	m_Label_Colonnes.		SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Colonnes.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Label_Colonnes.		SetFontBold(true);

	m_Label_Pages.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Pages.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	m_Label_Pages.			SetFontBold(true);

	// Couleur Boton Spéciale Moyennes
	m_Button_Item_Moy_Lig.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Item_Moy_Lig.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Item_Moy_Lig.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Couleur Boton Spéciale Moyennes
	m_Button_Item_Moy_Col.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Item_Moy_Col.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Item_Moy_Col.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Couleurs des boutons tris sélection tris croisés
	// UP & DOWN Lignes
	m_Button_Ligne_UP.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Ligne_UP.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Ligne_UP.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Ligne_UP.		SetBitmaps(IDB_RI_UP,RGB(192,192,192)); 
	m_Button_Ligne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Ligne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Ligne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Ligne_DOWN.	SetBitmaps(IDB_RI_DOWN,RGB(192,192,192));
	
	// UP & DOWN Colonnes
	m_Button_Colonne_UP.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Colonne_UP.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Colonne_UP.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Colonne_UP.	SetBitmaps(IDB_RI_UP,						RGB(192,192,192)); 
	m_Button_Colonne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Colonne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Colonne_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Colonne_DOWN.	SetBitmaps(IDB_RI_DOWN,						RGB(192,192,192));
	
	// UP & DOWN Pages
	m_Button_Page_UP.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Page_UP.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Page_UP.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Page_UP.		SetBitmaps(IDB_RI_UP,						RGB(192,192,192)); 
	m_Button_Page_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Page_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Page_DOWN.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Button_Page_DOWN.		SetBitmaps(IDB_RI_DOWN,						RGB(192,192,192));	
	
	// Les boutons sélection sites
	m_Btn_SitesEnLigne.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnLigne.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnLigne.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Btn_SitesEnColonne.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnColonne.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnColonne.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Btn_SitesEnPage.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnPage.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_SitesEnPage.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Les boutons tris sélection
	m_Btn_All_Lignes.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Lignes.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Lignes.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Btn_All_Colonnes.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Colonnes.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Colonnes.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	m_Btn_All_Pages.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Pages.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Btn_All_Pages.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	// Couleurs des élémts sélection pupitre
	m_Box_Onglet_Pupitre.	SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);

	// Couleur de fond des onglets sélection mode ligne / colonne / page
	m_Button_Mode_Ligne.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Mode_Ligne.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorWhite(), m_CATPColorsApp);
	m_Button_Mode_Ligne.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Mode_Colonne.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Mode_Colonne.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorWhite(), m_CATPColorsApp);
	m_Button_Mode_Colonne.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);

	m_Button_Mode_Page.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
	m_Button_Mode_Page.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorWhite(), m_CATPColorsApp);
	m_Button_Mode_Page.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp);
	

	// Remettre les bonnes couleurs via le mode en cours pour dimension 1 et dimension 2
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Rend invisible et désactive opérateur dim1 et dim2
		SetOpeDimension1(false);
		SetOpeDimension2(false);
	}
	else
	{
		// Remet visible et active les opérateurssur dim1 et dim2
		SetOpeDimension1(true);
		SetOpeDimension2(true);
	}
	
	if(m_pCurMode)
		m_pCurMode->SetColors();

	/*
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));
	*/
}

void CDlgItems::SetUpdateOpeLogiques()
{
	if(m_FIcones)
	{
		CString text;
		text = "";

		// Attention si on n'est pas en mode palette, ni en mode pupitre
		if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
		{
			m_Button_SAS_ET.SetWindowText(text); 
			m_Button_SAS_ET.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET,RGB(192,192,192)); 
			m_Button_SAS_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_SAS_OU.SetWindowText(text); 
			m_Button_SAS_OU.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU,RGB(192,192,192)); 
			m_Button_SAS_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		}
		else
		{
			m_Button_SAS_ET.SetWindowText(text); 
			m_Button_SAS_ET.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
			m_Button_SAS_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_SAS_OU.SetWindowText(text); 
			m_Button_SAS_OU.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
			m_Button_SAS_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		}

		m_Button_Segment_ET.SetWindowText(text); 
		m_Button_Segment_ET.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET,RGB(192,192,192));
		m_Button_Segment_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Segment_OU.SetWindowText(text); 
		m_Button_Segment_OU.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU,RGB(192,192,192)); 
		m_Button_Segment_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  

		m_Button_Dim1_ET.SetWindowText(text); 
		m_Button_Dim1_ET.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET,RGB(192,192,192)); 
		m_Button_Dim1_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_OU.SetWindowText(text); 
		m_Button_Dim1_OU.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU,RGB(192,192,192)); 
		m_Button_Dim1_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_NI.SetWindowText(text); 
		m_Button_Dim1_NI.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI,RGB(192,192,192)); 
		m_Button_Dim1_NI.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_XOR.SetWindowText(text); 
		m_Button_Dim1_XOR.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR,RGB(192,192,192)); 
		m_Button_Dim1_XOR.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  

		m_Button_Dim2_ET.SetWindowText(text); 
		m_Button_Dim2_ET.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET,RGB(192,192,192)); 
		m_Button_Dim2_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim2_OU.SetWindowText(text); 
		m_Button_Dim2_OU.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU,RGB(192,192,192)); 
		m_Button_Dim2_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim2_NI.SetWindowText(text); 
		m_Button_Dim2_NI.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI,RGB(192,192,192)); 
		m_Button_Dim2_NI.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim2_XOR.SetWindowText(text); 
		m_Button_Dim2_XOR.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR,RGB(192,192,192)); 
		m_Button_Dim2_XOR.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  

		// puis bouton sélection opération sur sas questions sélectionnées
		if (m_Button_SAS_ET.GetCheck())
		{
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				// Pour le Sas ET actif
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			}
			else
			{
				// Pour le Sas ET "différent si on n'est en mode palette"
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
			}
		}
		else if (m_Button_SAS_OU.GetCheck())
		{
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				// Pour le Sas OU actif
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			}
			else
			{
				// Pour le Sas OU "différent si on n'est en mode palette"
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
			}	
		}

		m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_QuSel.SetWindowText(text);

		// puis bouton sélection opération sur segments
		if (m_Button_Segment_ET.GetCheck())
			m_Button_Menu_Ope_Segment.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
		else if (m_Button_Segment_OU.GetCheck())
			m_Button_Menu_Ope_Segment.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 

		m_Button_Menu_Ope_Segment.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Segment.SetWindowText(text);

		// puis bouton sélection opération sur dimension1 en cours
		if (m_Button_Dim1_ET.GetCheck())
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
		else if (m_Button_Dim1_OU.GetCheck())
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		else if (m_Button_Dim1_NI.GetCheck())
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
		else if (m_Button_Dim1_XOR.GetCheck())
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));

		m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Dim1.SetWindowText(text);
		
		// puis bouton sélection opération sur dimension2 en cours
		if (m_Button_Dim2_ET.GetCheck())
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
		else if (m_Button_Dim2_OU.GetCheck())
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		else if (m_Button_Dim2_NI.GetCheck())
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
		else if (m_Button_Dim2_XOR.GetCheck())
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));

		m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Dim2.SetWindowText(text);

	}
	else
	{
		CString text;

		// Opérateur ET
		LOADSTRING(text, IDS_RI_ET);
		m_Button_Segment_ET.SetWindowText(text); 
		m_Button_Segment_ET.SetBitmaps(0,0);
		m_Button_Dim1_ET.SetWindowText(text); 
		m_Button_Dim1_ET.SetBitmaps(0,0);
		m_Button_Dim2_ET.SetWindowText(text); 
		m_Button_Dim2_ET.SetBitmaps(0,0);

		// Opérateur OU
		LOADSTRING(text, IDS_RI_OU);
		m_Button_Segment_OU.SetWindowText(text); 
		m_Button_Segment_OU.SetBitmaps(0,0);
		m_Button_Dim1_OU.SetWindowText(text); 
		m_Button_Dim1_OU.SetBitmaps(0,0);
		m_Button_Dim2_OU.SetWindowText(text); 
		m_Button_Dim2_OU.SetBitmaps(0,0);

		// Opérateur NI
		LOADSTRING(text, IDS_RI_NI);
		m_Button_Dim1_NI.SetWindowText(text); 
		m_Button_Dim1_NI.SetBitmaps(0,0);
		m_Button_Dim2_NI.SetWindowText(text); 
		m_Button_Dim2_NI.SetBitmaps(0,0);

		// Opérateur XOR
		LOADSTRING(text, IDS_RI_XOR);
		m_Button_Dim1_XOR.SetWindowText(text); 
		m_Button_Dim1_XOR.SetBitmaps(0,0);
		m_Button_Dim2_XOR.SetWindowText(text); 
		m_Button_Dim2_XOR.SetBitmaps(0,0);

		// Pour le Sas ET " différent si on n'est en mode palette"
		if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
		{
			LOADSTRING(text, IDS_RI_ET);
			m_Button_SAS_ET.SetWindowText(text); 
			m_Button_SAS_ET.SetBitmaps(0,0); 

			LOADSTRING(text, IDS_RI_OU);
			m_Button_SAS_OU.SetWindowText(text); 
			m_Button_SAS_OU.SetBitmaps(0,0); 

			// puis bouton sélection opération sur Sas questions sélectionnées
			if (m_Button_SAS_ET.GetCheck())
				m_Button_SAS_ET.GetWindowText(text);
			else if (m_Button_SAS_OU.GetCheck())
				m_Button_SAS_OU.GetWindowText(text);

			m_Button_Menu_Ope_QuSel.SetBitmaps(0,0);
			m_Button_Menu_Ope_QuSel.SetWindowText(text);
		}
		else
		{
			// en mode palette toujours les icones "&" et "X"
			text = "";
			m_Button_SAS_ET.SetWindowText(text); 
			m_Button_SAS_ET.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
			m_Button_SAS_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_SAS_OU.SetWindowText(text); 
			m_Button_SAS_OU.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
			m_Button_SAS_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  

			// Affecte l'icone correspondante à l'opérateur courant
			if (m_Button_SAS_ET.GetCheck())
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
			else if (m_Button_SAS_OU.GetCheck())
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 

			m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			text = "";
			m_Button_Menu_Ope_QuSel.SetWindowText(text);
		}

		// puis bouton sélection opération sur segments
		if (m_Button_Segment_ET.GetCheck())
			m_Button_Segment_ET.GetWindowText(text);
		else if (m_Button_Segment_OU.GetCheck())
			m_Button_Segment_OU.GetWindowText(text);

		m_Button_Menu_Ope_Segment.SetBitmaps(0,0);
		m_Button_Menu_Ope_Segment.SetWindowText(text);

		// puis bouton sélection opération sur dimension1 en cours
		if (m_Button_Dim1_ET.GetCheck())
			m_Button_Dim1_ET.GetWindowText(text);
		else if (m_Button_Dim1_OU.GetCheck())
			m_Button_Dim1_OU.GetWindowText(text);
		else if (m_Button_Dim1_NI.GetCheck())
			m_Button_Dim1_NI.GetWindowText(text);
		else if (m_Button_Dim1_XOR.GetCheck())
			m_Button_Dim1_XOR.GetWindowText(text);
		
		m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
		m_Button_Menu_Ope_Dim1.SetWindowText(text);

		
		// puis bouton sélection opération sur dimension2 en cours
		if (m_Button_Dim2_ET.GetCheck())
			m_Button_Dim2_ET.GetWindowText(text);
		else if (m_Button_Dim2_OU.GetCheck())
			m_Button_Dim2_OU.GetWindowText(text);
		else if (m_Button_Dim2_NI.GetCheck())
			m_Button_Dim2_NI.GetWindowText(text);
		else if (m_Button_Dim2_XOR.GetCheck())
			m_Button_Dim2_XOR.GetWindowText(text);
		
		m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Dim2.SetWindowText(text);

		// En mode palette (ou pupitre), classe, quantile et quest induite  les opérateurs sur dimension 1 et dimension 2 inactif
		if (m_Id_CurMode == Mode_Palettes  || 
			m_Id_CurMode == Mode_Classes   || 
			m_Id_CurMode == Mode_Quantiles || 
			m_Id_CurMode == Mode_QuestInduites|| 
			!GetTypeRepItem())
		{
			// Rend invisible et désactive opérateur dim1 et dim2
			SetOpeDimension1(false);
			SetOpeDimension2(m_Box_Dimension2.IsWindowVisible(),false);
			// m_pCurMode->SetOpeDimension3(false);
		}
		else
		{
			// Remet visible et active les opérateurssur dim1 et dim2
			SetOpeDimension1(true);
			SetOpeDimension2(m_Box_Dimension2.IsWindowVisible());
		}
	}

	if(m_pCurMode)
		m_pCurMode->SetIcones();
}

void CDlgItems::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_RI_ABOUTBOX)
	{
		CAboutDlg dlgAbout(this);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgItems::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CDlgItems::SetWindowRelations()
{
	// Set all widow relation positions
	static CResizer::CBorderInfo s_bi[] = {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  TITLE BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_TITLE,		{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft},					//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},					//Top side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft},					//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop}},					//Bottom side

	{IDC_RI_LABEL_TITLE,		{CResizer::eFixed,			IDC_RI_STATIC_TITLE, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_TITLE, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_TITLE, CResizer::eRight},
								{CResizer::eFixed,			IDC_RI_STATIC_TITLE, CResizer::eBottom}}, 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  SOURCE & TERRAIN BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_SRCTERRAIN,	{CResizer::eFixed,			IDC_RI_STATIC_TITLE, CResizer::eRight},			//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},						//Top side
								{CResizer::eProportional,	IDC_MAIN, CResizer::eRight},					//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop}},						//Bottom side

	{IDC_RI_LABEL_SOURCE,		{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eRight},
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eBottom}},
    
	{IDC_RI_LABEL_NBCELLS,		{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eRight},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eBottom},	//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eRight},
								{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN, CResizer::eBottom}},


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  THEME BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_THEME,	{CResizer::eFixed,			IDC_RI_STATIC_TITLE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_TITLE,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_TITLE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom}},	//Bottom side

	{IDC_RI_LABEL_THEME,	{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eLeft}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop},  
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop}},

	{IDC_RI_RAD_QUESTIONS,	{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eLeft}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eBottom},  
							{CResizer::eProportional,	IDC_RI_STATIC_THEME, CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eBottom}}, 

	{IDC_RI_RAD_LIST_1,		{CResizer::eFixed,			IDC_RI_RAD_QUESTIONS,	CResizer::eRight}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom},  
							{CResizer::eProportional,	IDC_RI_STATIC_THEME,	CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom}}, 

	{IDC_RI_RAD_LIST_2,		{CResizer::eFixed,			IDC_RI_RAD_LIST_1,		CResizer::eRight}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom},  
							{CResizer::eProportional,	IDC_RI_STATIC_THEME,	CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom}}, 

	{IDC_RI_TREE_THEME,		{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eLeft}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},  
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom}}, 

	{IDC_RI_TREE_RUBRIC_ITEMS,{CResizer::eFixed,		IDC_RI_STATIC_THEME,	CResizer::eLeft}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},  
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}}, 

	{IDC_RI_TREE_RUBRIC_PALETTES,{CResizer::eFixed,		IDC_RI_STATIC_THEME,	CResizer::eLeft}, 
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},  
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eRight},
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}}, 

	{IDC_RI_LIST_IITEMS,	{CResizer::eFixed,			IDC_RI_TREE_THEME,	CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_TREE_THEME,	CResizer::eTop},			//Top side
							{CResizer::eFixed,			IDC_RI_TREE_THEME,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_TREE_THEME,	CResizer::eBottom}},		//Bottom side 

	{IDC_RI_EDIT_SEARCH,	{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop},			//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_THEME, CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop}},			//Bottom side

	{IDC_RI_EDIT_QFILTER,	{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop},			//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_THEME, CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME, CResizer::eTop}},			//Bottom side

	{IDC_RI_BUT_RAZSEARCH,	{CResizer::eFixed,			IDC_RI_EDIT_SEARCH,		CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},		//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_THEME,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_GENESE,		{CResizer::eFixed,			IDC_RI_BUT_RAZSEARCH,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_QFILTERRAZ,	{CResizer::eFixed,			IDC_RI_EDIT_QFILTER,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},		//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_THEME,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_QFILTER,	{CResizer::eFixed,			IDC_RI_BUT_QFILTERRAZ,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eTop}},		//Bottom side


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  QUESTION BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////


	{IDC_RI_STATIC_QUESTION,{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_SRCTERRAIN,	CResizer::eRight},		//Right side
							{CResizer::eProportional,	IDC_MAIN,					CResizer::eBottom}},	//Bottom side

	{IDC_RI_LABEL_QUESTIONS,{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eLeft},					//Left side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop},						//Top side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eRight},
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop}}, 

	{IDC_RI_LABEL_QUESTIONS2,{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eLeft},					//Left side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop},						//Top side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eRight},
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop}}, 

	{IDC_RI_BUT_QUESTION_TOUS,{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eRight},					//Left side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop},						//Top side
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eRight},
							{CResizer::eFixed, IDC_RI_STATIC_QUESTION, CResizer::eTop}}, 

	{IDC_RI_LIST_QUESTIONS,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,		CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LABEL_QUESTIONS ,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,		CResizer::eRight},
							{CResizer::eProportional,	IDC_RI_STATIC_QUESTION,		CResizer::eBottom}}, 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  DIMENSION1 BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_DIMENSION1,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_QUESTION, CResizer::eBottom},		//Top side
								{CResizer::eProportional,	IDC_RI_STATIC_QUESTION,	CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_THEME,	CResizer::eBottom}},	//Bottom side

	{IDC_RI_BUT_DIM1_RAZ,		{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eRight},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop}},		//Bottom side

	{IDC_RI_LABEL_DIMENSION1,	{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_BUT_DIM1_RAZ,	  CResizer::eLeft},	   //Right side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop}},		//Bottom side

	{IDC_RI_RAD_DIMENSION1,		{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_BUT_DIM1_RAZ,	  CResizer::eLeft},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_MENU_OPE_DIM1,	{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eLeft},		//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_DIMENSION1, CResizer::eTop}},		//Bottom side

	{IDC_RI_PUSH_DIM1_OK,		{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1, CResizer::eLeft},				//Left side
								{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1, CResizer::eTop},				//Top side
								{CResizer::eFixed, IDC_RI_BUT_DIM1_RAZ,		 CResizer::eLeft},				//Right side
								{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1, CResizer::eTop}},				//Bottom side

	{IDC_RI_RAD_DIM1_OU,	{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eLeft},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eLeft},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_RAD_DIM1_ET,	{CResizer::eFixed, IDC_RI_RAD_DIM1_OU,			CResizer::eRight},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_RAD_DIM1_OU,			CResizer::eRight},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_RAD_DIM1_XOR,		{CResizer::eFixed, IDC_RI_RAD_DIM1_ET,		CResizer::eRight},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_RAD_DIM1_ET,			CResizer::eRight},				//	Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_RAD_DIM1_NI,	{CResizer::eFixed, IDC_RI_RAD_DIM1_XOR,		    CResizer::eRight},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_RAD_DIM1_XOR,			CResizer::eRight},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_BUT_FILTRE,		{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,    CResizer::eRight},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eRight},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	/* Avant Bouton Filtre
	{IDC_RI_BUT_DIM1_DOWN,	{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,    CResizer::eRight},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eRight},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side
	*/

	{IDC_RI_BUT_DIM1_DOWN,	{CResizer::eFixed, IDC_RI_BUT_FILTRE,			CResizer::eLeft},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_BUT_FILTRE,			CResizer::eLeft},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_BUT_DIM1_UP,	{CResizer::eFixed, IDC_RI_BUT_DIM1_DOWN,	    CResizer::eLeft},				//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_BUT_DIM1_DOWN,		CResizer::eLeft},				//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop}},				//Bottom side

	{IDC_RI_LIST_DIMENSION1,{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eLeft},					//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eTop},				//Top side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eRight},
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION1,	CResizer::eBottom}}, 


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  DIMENSION2 BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_DIMENSION2,{CResizer::eFixed,IDC_RI_STATIC_DIMENSION1,		CResizer::eRight},			//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION1,		CResizer::eTop},			//Top side
							{CResizer::eFixed,	IDC_MAIN,						CResizer::eRight},			//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION1,		CResizer::eBottom}},		//Bottom side

	{IDC_RI_BUT_MENU_OPE_DIM2,{CResizer::eFixed, IDC_RI_STATIC_DIMENSION2,		CResizer::eLeft},			//Left side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},			//Top side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION2,		CResizer::eLeft},			//Right side
							{CResizer::eFixed, IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},			//Bottom side

	{IDC_RI_BUT_DIM2_RAZ,		{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,	CResizer::eRight},			//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},			//Top side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eRight},			//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},			//Bottom side

	{IDC_RI_LABEL_DIMENSION2,	{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,	CResizer::eLeft}, 
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},  
							{CResizer::eFixed,	IDC_RI_BUT_DIM2_RAZ,			CResizer::eLeft},
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}}, 

	{IDC_RI_RAD_DIMENSION2,	{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eLeft}, 
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},  
							{CResizer::eFixed,	IDC_RI_BUT_DIM2_RAZ,			CResizer::eLeft},
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}}, 

	{IDC_RI_RAD_DIM2_OU,		{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,	CResizer::eLeft},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eLeft},	//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},	//Bottom side

	{IDC_RI_RAD_DIM2_ET,		{CResizer::eFixed,	IDC_RI_RAD_DIM2_OU,			CResizer::eRight},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_RAD_DIM2_OU,				CResizer::eRight},	//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},	//Bottom side

	{IDC_RI_RAD_DIM2_XOR,		{CResizer::eFixed,	IDC_RI_RAD_DIM2_ET,			CResizer::eRight},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_RAD_DIM2_ET,				CResizer::eRight},	//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},	//Bottom side

	{IDC_RI_RAD_DIM2_NI,		{CResizer::eFixed,	IDC_RI_RAD_DIM2_XOR,		CResizer::eRight},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_RAD_DIM2_XOR,			CResizer::eRight},	//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop}},	//Bottom side

	{IDC_RI_LIST_DIMENSION2,	{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,	CResizer::eLeft},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eRight},
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eBottom}}, 


///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  ONGLETS PUPITRE
///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Bouton fléche non retaillé
	{IDC_RI_RAD_MODE_LIGNES,{CResizer::eFixed,			IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Right side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom}},	//Bottom side

	{IDC_RI_RAD_MODE_COLONNES,{CResizer::eFixed,		IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Right side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom}},	//Bottom side

	{IDC_RI_RAD_MODE_PAGES,	{CResizer::eFixed,			IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_LIST_QUESTIONS,	CResizer::eLeft},		//Right side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom}},	//Bottom side
	
	{IDC_RI_STATIC_ONGLET_PUPITRE, {CResizer::eFixed,	IDC_MAIN,				CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_RAD_MODE_LIGNES,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_RAD_MODE_LIGNES,	CResizer::eTop}},		//Bottom side

	
	{IDCANCEL,				{CResizer::eFixed,			IDC_MAIN,	CResizer::eRight},					//Left side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom},					//Top side	
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eRight},					//Right side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom}},				//Bottom side

	{IDOK,					{CResizer::eFixed,			IDCANCEL,	CResizer::eRight},					//Left side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom},					//Top side
							{CResizer::eFixed,			IDCANCEL,	CResizer::eRight},					//Right side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom}},				//Bottom side

	/*
	{IDC_RI_LABEL_NBCELLS,	{CResizer::eFixed,			IDOK,		CResizer::eRight},					//Left side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom},					//Top side
							{CResizer::eFixed,			IDOK,		CResizer::eRight},
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom}},
	*/

	/*
	{ID_NBCELLS,			{CResizer::eFixed,			IDOK,		CResizer::eRight},					//Left side
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom},					//Top side
							{CResizer::eFixed,			IDOK,		CResizer::eRight},
							{CResizer::eFixed,			IDC_MAIN,	CResizer::eBottom}},
	*/

						


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  STATIC SEGMENT 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_SEGMENT,	{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION1,		CResizer::eLeft},	//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_ONGLET_PUPITRE,	CResizer::eTop},	//Top side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,		CResizer::eRight},	//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_ONGLET_PUPITRE,	CResizer::eTop}}, //Bottom side

	{IDC_RI_BUT_MENU_OPE_SEGMENT, {CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eBottom},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eLeft}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eBottom}}, //Bottom side

	{IDC_RI_RAD_S_ET,		{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eBottom},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eLeft}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT, CResizer::eBottom}}, //Bottom side

	{IDC_RI_RAD_S_OU,		{CResizer::eFixed,	IDC_RI_RAD_S_ET,		CResizer::eLeft},					//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT,	CResizer::eBottom},					//Top side
							{CResizer::eFixed,	IDC_RI_RAD_S_ET,		CResizer::eLeft},					//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT,	CResizer::eBottom}},				//Bottom side

	{IDC_RI_TAB_SEGMENT,	{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eBottom},			//Top side
							// {CResizer::eFixed,	IDC_RI_RAD_S_OU,		CResizer::eLeft},
							{CResizer::eProportional,	IDC_RI_STATIC_SEGMENT,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eBottom}},		//Bottom side

	{IDC_RI_BUT_SEGMENT_RAZ,{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eRight},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eBottom},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eBottom}},		//Bottom side

	/*
	{IDC_RI_CHECK_CALC_SEGMENT,	{CResizer::eFixed,		IDC_RI_STATIC_SEGMENT,	CResizer::eRight},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eTop},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eTop}},			//Bottom side
	*/


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  SAS BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_SAS,		{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION1,	CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION1,	CResizer::eBottom},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_DIMENSION2,	CResizer::eRight}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SEGMENT,		CResizer::eTop}}, //Bottom side

	{IDC_RI_BUT_MENU_OPE_QUSEL,{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eLeft}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop}}, //Bottom side

	{IDC_RI_RAD_Q_ET,		{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eLeft}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop}}, //Bottom side

	{IDC_RI_RAD_Q_OU,		{CResizer::eFixed,	IDC_RI_RAD_Q_ET,	CResizer::eRight}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},  //Top side
							{CResizer::eFixed,	IDC_RI_RAD_Q_ET,	CResizer::eRight}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop}}, //Bottom side

	{IDC_RI_LABEL_SAS,		{CResizer::eFixed,	IDC_RI_RAD_Q_OU,	CResizer::eRight}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eRight}, //Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop}}, //Bottom side

	{IDC_RI_LIST_SAS,		{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eLeft}, //Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},  //Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eRight},
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eBottom}}, 

	{IDC_RI_BUT_SAS_RAZ,	{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eRight},			//Left side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop},			//Top side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eRight},			//Right side
							{CResizer::eFixed,	IDC_RI_STATIC_SAS,	CResizer::eTop}},			//Bottom side

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  ITEMS BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_ITEMS,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eTop},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_SEGMENT,	CResizer::eBottom}},		//Bottom side

	{IDC_RI_BUT_ACCEPT,		{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eRight},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop}},			//Bottom side

	{IDC_RI_BUT_SUPPRESSION,	{CResizer::eFixed,		IDC_RI_STATIC_ITEMS,	CResizer::eRight},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop}},			//Bottom side
	
	{IDC_RI_EDIT_NAME,		{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eLeft},				//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eTop},				//Top side
							{CResizer::eFixed,			IDC_RI_BUT_ACCEPT,	 CResizer::eLeft},				//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eTop}},				//Bottom side

	{IDC_RI_LABEL_ITEM,		{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eLeft},				//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eTop},				//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eLeft},				//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS, CResizer::eTop}},
//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////// RESULT BOX
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_RESULT,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop}},		//Bottom side

	{IDC_RI_LABEL_DEFITEM,	{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	// Boutons pour ordonner les items sélectionnés
	{IDC_RI_BTN_DOWN,		{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BTN_UP,			{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BTN_IMPITEM,	{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BTN_EXPORT_ITEM,{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BTN_SUPP_ITEM,  {CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop}},		//Bottom side

	{IDC_RI_LIST_ITEMS,		{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_RI_LIST_COMPO_PALETTE,{CResizer::eFixed,		IDC_RI_STATIC_RESULT,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_RI_CADRE_RESULT,	{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom}},		//Bottom side

	{IDC_RI_LABEL_TYPEPOIDS,{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom}},		//Bottom side

	{IDC_RI_CHECK_CALC_SEGMENT,	{CResizer::eFixed,		IDC_RI_STATIC_RESULT,	CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom},			//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},			//Bottom side
	
	{IDC_RI_LABEL_EFFECTIF,	{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom}},		//Bottom side

	{IDC_RI_STATIC_EFF,		{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom},		//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_RESULT, CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom}},		//Bottom side

	{IDC_RI_LABEL_POURCENT,	{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eLeft},			//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT, CResizer::eBottom}},		//Bottom side

	{IDC_RI_STATIC_PERCENT,	{CResizer::eFixed,			IDC_RI_STATIC_EFF,		CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom},		//Top side
							{CResizer::eProportional,	IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_RI_LABEL_NBCAS,	{CResizer::eFixed,			IDC_RI_STATIC_PERCENT,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PERCENT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_RI_STATIC_NBCAS,	{CResizer::eFixed,			IDC_RI_LABEL_NBCAS,		CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side
		
	{IDC_RI_CHK_RUBRIQUE_DEFAUT,{CResizer::eFixed,		IDC_RI_STATIC_RESULT,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eBottom}},	//Bottom side

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////// BOITE PUPITRE
//////////////////////////////////////////////////////////////////////////////////////////////////////
	{IDC_RI_STATIC_PUPITRE,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_QUESTION,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_ITEMS,	CResizer::eTop}},		//Bottom side

	{IDC_RI_LABEL_LIGNES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop}},		//Bottom side	

	{IDC_RI_BUT_ITEM_MOY_LIG,{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_ALL_LIGNES, {CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_LIGNES_DOWN,{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop}},		//Bottom side

	{IDC_RI_BUT_LIGNES_UP,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop}},		//Bottom side
	
	{IDC_RI_LIST_LIGNES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eProportional,	IDC_RI_STATIC_PUPITRE,	CResizer::eBottom}},	//Bottom side	

	{IDC_RI_BTN_SITES_LIG,  {CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eTop}},		//Bottom side	

	{IDC_RI_LABEL_COLONNES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom}},	//Bottom side	

	{IDC_RI_BUT_ITEM_MOY_COL,{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom}},	//Bottom side

	{IDC_RI_BUT_ALL_COLONNES,{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom}},	//Top side

	{IDC_RI_BUT_COLONNES_DOWN,{CResizer::eFixed,		IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom}},	//Top side

	{IDC_RI_BUT_COLONNES_UP,	{CResizer::eFixed,		IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom}},	//Top side

	{IDC_RI_LIST_COLONNES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_LIGNES,		CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eProportional,	IDC_RI_STATIC_PUPITRE,	CResizer::eBottom}},	//Bottom side	

	{IDC_RI_BTN_SITES_COL,  {CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eTop}},	//Bottom side

	{IDC_RI_LABEL_PAGES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom}},	//Bottom side	

	{IDC_RI_BUT_ALL_PAGES,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom}},	//Top side

	{IDC_RI_BUT_PAGES_DOWN,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom}},	//Top side
	
	{IDC_RI_BUT_PAGES_UP,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom}},	//Top side
	
	{IDC_RI_LIST_PAGES,		{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eLeft},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eBottom}},	//Bottom side	

	{IDC_RI_BTN_SITES_PAGE,	{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_PUPITRE,	CResizer::eRight},		//Right side
							{CResizer::eFixed,			IDC_RI_LIST_COLONNES,	CResizer::eBottom}},	//Top side
	
	// Split vertical
	{IDC_CS_SPLIT2_VERT,	{CResizer::eFixed,			IDC_RI_STATIC_QUESTION, CResizer::eRight},		//Left side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eTop},		//Top side
							{CResizer::eFixed,			IDC_RI_STATIC_RESULT,	CResizer::eLeft},		//Right side
							{CResizer::eFixed,			IDC_MAIN,				CResizer::eBottom}},	//Bottom side
	};

	const int nSize = sizeof(s_bi)/sizeof(s_bi[0]);
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgItems::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlgItems::OnAccept() 
{
	UpdateData(true);
	if(!m_Edit_Name.GetWindowTextLength())
	{
		CString str;
		LOADSTRING(str, IDS_RI_ITEM_LABEL);
		MessageBox(str,_T(""),MB_OK | MB_ICONEXCLAMATION);
		m_Edit_Name.SetFocus();		
		return;
	}

	if(m_pCurMode)
	{
		CString Text;
		m_Edit_Name.GetWindowText(Text);
		if(m_pCurMode->OnAccept(Text))
		{
			UpdateData(false);

			GetItems()->Save();

			// OnItem_Raz();
			// Clearing all the current item && palettes sauf tree questions
			m_pCurMode->OnTout_Raz_ItemPalette(); 

			m_Edit_Name.SetWindowText(""); 

			// Mode palette, virer la liste des composants
			if (m_Id_CurMode == Mode_Palettes)
				UpdateListSelCompoPalette();
		}
	}

	// Mettre les opérateurs par défaut
	AllOperateurParDefaut();
}

/// Get the item Label
CString CDlgItems::GetItemLabel()
{
	CString Text;
	m_Edit_Name.GetWindowText(Text);
	return Text;
}

void CDlgItems::OnDim_2_Et() 
{
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_ET);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_2_Et();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
			m_Button_Dim2_ET.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(2,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension2.EnableWindow(TRUE); 

}

void CDlgItems::OnDim_2_Xor() 
{
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_XOR);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_2_Xor();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
			m_Button_Dim2_XOR.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(2,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension2.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_2_Ni() 
{
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_NI);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_2_Ni();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
			m_Button_Dim2_NI.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(2,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension2.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_2_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_OU);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_2_Ou();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
			m_Button_Dim2_OU.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(2,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension2.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_2_Raz() 
{
	if(m_List_Dimension2.ToggleSelection())
		if(m_pCurMode)
			m_pCurMode->OnDim_2_Raz();

	// Update compo mode palette
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Mise à jour de la liste sélection 
		if (m_AjoutSelCompoPalette_Dim2) 
			m_pCurMode->OnSetDimSelection2();
		UpdateListSelCompoPalette();
	}
}

void CDlgItems::OnGenese() 
{
	// Ssi 1 seul élmt de sélectionné dans la liste des items
	if(m_List_Items.GetSelCount() == 1)
	{
	
		if(m_pCurMode)
		{
			///////////////////// Inscrit le nom en bas de l'item sélectionné //////////////////
			JList<JInt32> Values;
			m_List_Items.GetSelectedID(Values);
			Values.MoveFirst();
			if (Values.IsValid())
			{
				IItem *pItem = GetItems()->GetIItemByID(Values.GetItem());
				JLabel LabelItem = pItem->GetLabel(); 
				m_Edit_Name.SetWindowText(LabelItem.AsJCharPtr());

				// Ident item en cours de genèse
				m_IdItemGenese = pItem->GetID(); 
			}

			// Décompose l'item
			if (m_pCurMode->OnGenese())
			{
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);
				
				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					// Affichage questionnaire
					UpdateTheme(IDC_RI_RAD_QUESTIONS);
				
					// Sélection des questions dans tree-items
					m_Tree_Theme.SetQuestionList(ListQuestionSAS);

					// Puis on déselectionne l'élmt item, puisque l'on passe en mode construction
					m_pCurMode->OnDeselect_Items();

					// Mars 06 ???????????????????????????????? Position sélection visible dans liste questions
					int CurSel = m_List_Questions.GetCurSel();
					if (CurSel >= 0)
					{
						m_List_Questions.SetTopIndex(CurSel);	
					}

					// Update the selection list
					m_List_SAS.SetCurSel(0);
					OnSelchange_ListSas();
			
					// Redraw list questions
					m_List_Questions.Invalidate();
				}
				else
				{
					// Rend inactive la fenêtre items
					this->EnableWindow(FALSE) ;

					// Cet item ne peut pas être décomposé
					CString TxtMess;
					LOADSTRING(TxtMess, IDS_RI_MESS_1); 
					AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
					// AfxMessageBox("Item construit à partir d'autres items, pas de genèse possible", MB_ICONEXCLAMATION);
					
					// la réactive
					this->EnableWindow(TRUE) ;

					// Retour à liste des items
					UpdateTheme(IDC_RI_RAD_LIST_1);
					OnSelchange_ListItems();
				}

			}
			else
			{
				// Pas de genèse possible
				// Rend inactive la fenêtre items
				this->EnableWindow(FALSE) ;

				// Cet item ne peut pas être décomposé
				CString TxtMess;
				LOADSTRING(TxtMess, IDS_RI_MESS_1); 
				AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
				//AfxMessageBox("Item construit à partir d'autres items, pas de genèse possible", MB_ICONEXCLAMATION);
				
				// la réactive
				this->EnableWindow(TRUE) ;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Renvoi su genese elmt courant possible
bool CDlgItems::OnGenesePossible() 
{
	// Ssi 1 seul élmt de sélectionné dans la liste des items
	if(m_List_Items.GetSelCount() == 1)
	{
		if(m_pCurMode)
		{
			// Décompose l'item
			if (m_pCurMode->OnGenese())
			{
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}	
	}

	return false;
}

// Bouton Filtrage Modalités
void CDlgItems::OnDim_Filtre() 
{
	// Distinction entre 2 sauvegarde sélection
	// = true; on tient compte uniquement avec les elmts filtés secteur
	// = false; si précedemment filté et sélectionné, puis retire du filtrage on garde la sélection
	bool SelectInFiltrage = true;

	if(m_pCurMode)
	{
		// Récup avant tout la sélection des modalités dimensions 1
		JList<const IModalite *> Values;
		m_List_Dimension1.GetSelectedItems(Values);

		if (SelectInFiltrage == false)
		{
			// Mode sauvegarde sélection m^me si pas dans filtrage
			for (JInt32 IdxModa = 0; IdxModa < m_List_Dimension1.GetCount(); IdxModa++)
			{
				// Récupère la modalité correspondante
				const IModalite* pModalite = (const IModalite*)m_List_Dimension1.GetItemData(IdxModa);

				bool ModaTrouvee = false;
				for (Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
				{
					// Le code de la modalité
					JInt32 Code = Values.GetItem()->GetID();
					if (pModalite->GetID() == Code)
					{
						// On ajoute sélection au map courant de sélection
						m_MapSelModaDimension1.MoveTo(Code);
						if (!m_MapSelModaDimension1.IsValid())
						{
							m_MapSelModaDimension1.Add(Code);
						}
						ModaTrouvee = true;
						break;
					}
				}

				// Dans le cas ou la moda a été précemment sélectionnée, et qu'elle ne l'est plus
				if (!ModaTrouvee)
				{
					// On enleve sélection au map courant de sélection
					JInt32 CodeDelete = pModalite->GetID();
					m_MapSelModaDimension1.MoveTo(CodeDelete);
					if (m_MapSelModaDimension1.IsValid())
					{
						m_MapSelModaDimension1.Remove(CodeDelete);
					}
				}
			}
		}
		else
		{
			m_MapSelModaDimension1.Reset();

			// Ici on ne garde que les modalités sélectonnées via le filtrage secteur
			for (Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
			{
				// Le code de la modalité
				JInt32 Code = Values.GetItem()->GetID();
			
				// On ajoute sélection au map courant de sélection
				m_MapSelModaDimension1.MoveTo(Code);
				if (!m_MapSelModaDimension1.IsValid())
				{
					m_MapSelModaDimension1.Add(Code);
				}
			}
		}
		
		// Lance la boite filtrage (par secteurs pour le moment)
		m_pCurMode->OnDim_Filtre();
 	}
}

void CDlgItems::OnDim_1_Up() 
{
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_1_Up(); 	
		GetItems()->Save();
	}
}

void CDlgItems::OnDim_1_Down() 
{
	if(m_pCurMode)
	{

		m_pCurMode->OnDim_1_Down(); 	
		GetItems()->Save();
	}
}

void CDlgItems::OnDim_1_Ok() 
{
	if(m_pCurMode)
		m_pCurMode->OnDim_1_Ok();


}

void CDlgItems::OnDim_1_Et() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_ET);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_1_Et();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
			m_Button_Dim1_ET.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(1,FALSE);
	
	// Rend actif l'arrière plan
	m_List_Dimension1.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_1_Xor() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_XOR);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_1_Xor();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			CString Txt;
			m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
			m_Button_Dim1_XOR.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(1,FALSE);
	
	// Rend actif l'arrière plan
	m_List_Dimension1.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_1_Ni() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_NI);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_1_Ni();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			CString Txt;
			m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
			m_Button_Dim1_NI.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(1,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension1.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_1_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
	if(m_pCurMode)
	{
		m_pCurMode->OnDim_1_Ou();

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			CString Txt;
			m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
			m_Button_Dim1_OU.GetWindowText(Txt);
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeDim(1,FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension1.EnableWindow(TRUE); 
}

void CDlgItems::OnDim_1_Raz() 
{
	if(m_List_Dimension1.ToggleSelection())
		if(m_pCurMode)
			m_pCurMode->OnDim_1_Raz ();

	// Recup index question courante
	JUnt32 IndexQuCur = LB_ERR;

	// Update compo mode palette
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Attention si quelque chose dans le sas et non sélectionné, SAS Non vide
		// Attention si aucun élmt sélectionné, on revalide un elmt du sas, puis on reviendra sur la question
		if (m_List_SAS.GetCount() > 0)
		{
			// Voir si aucune sélection active sur Sas désormais
			if (m_List_SAS.GetCurSel() == LB_ERR		&& 
				m_List_Dimension1.GetSelCount() <= 0	&& 
				m_List_Dimension2.GetSelCount() <= 0)
			{
				// Revient sur la question en cours
				IndexQuCur = m_List_Questions.GetCurSel();

				// Repositionne élmt du sas
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();
			}
		}

		// Mise à jour de la liste sélection 
		if (m_AjoutSelCompoPalette_Dim1) 
		{
			m_pCurMode->OnSetDimSelection1();
		}

		UpdateListSelCompoPalette();
	}

	// On se remet sur les modalités de la question en cours si nécessaire
	if (IndexQuCur != LB_ERR)
	{
		m_List_Questions.SetCurSel(IndexQuCur);
		OnSelchange_ListQuestions();
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Opération ET sur les éléments du SAS
void CDlgItems::OnSAS_Et() 
{
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);
	if(m_pCurMode)
	{
		m_pCurMode->OnSAS_Et(); 

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur ET en mode icone
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
			}
			m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(0,0);
				m_Button_SAS_ET.GetWindowText(Txt);
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
				m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
				Txt = "";
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
		}
	}

	// Si mode palette, on réaffiche la liste des composants avec nouvel opération
	if (m_Id_CurMode == Mode_Palettes)
		UpdateListSelCompoPalette();

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSelQu(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 
}


///////////////////////////////////////////////////////////////////////////////////
// Opération OU sur les éléments du SAS
void CDlgItems::OnSAS_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_OU);
	if(m_pCurMode)
	{
		m_pCurMode->OnSAS_Ou(); 

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur OU en mode icone
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
			}
			m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
		}
		else
		{
			// Affichage opérateur OU en mode texte
			if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(0,0);
				m_Button_SAS_OU.GetWindowText(Txt);
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
				m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
				Txt = "";
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
		}
	}

	// Si mode palette, on réaffiche la liste des composants avec nouvel opération
	if (m_Id_CurMode == Mode_Palettes)
		UpdateListSelCompoPalette();

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSelQu(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 
}

// Activation opération SAS
void CDlgItems::ActiveOpeSAS(IBloc::OPERATOR Operator)
{
	switch (Operator)
	{
		// Active Opérateur OU sur les éléments du SAS
		case IBloc::Or:
			OnSAS_Ou();
			break;

		// Active Opérateur OU sur les éléments du ET
		case IBloc::And:
			OnSAS_Et();	
			break;

		// Par défaut opérateur ET
		default:
			OnSAS_Et();	
			break;
	}
}

void CDlgItems::OnSegment_Et() 
{
	CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_ET);
	if(m_pCurMode)
	{
		m_pCurMode->OnSegment_Et(); 

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Segment.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Segment.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Segment.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Segment.SetBitmaps(0,0);
			m_Button_Segment_ET.GetWindowText(Txt);
			m_Button_Menu_Ope_Segment.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSegment(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 
}

void CDlgItems::OnSegment_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_OU);
	if(m_pCurMode)
	{
		m_pCurMode->OnSegment_Ou(); 

		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur en mode icone
			m_Button_Menu_Ope_Segment.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			m_Button_Menu_Ope_Segment.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Segment.SetWindowText(Txt); 
		}
		else
		{
			// Affichage en mode texte
			m_Button_Menu_Ope_Segment.SetBitmaps(0,0);
			m_Button_Segment_OU.GetWindowText(Txt);
			m_Button_Menu_Ope_Segment.SetWindowText(Txt); 
		}
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSegment(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 
}

void CDlgItems::OnTree_Questions()
{
	// Affichage questionnaire
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
}

void CDlgItems::OnList_1()
{
	// Affichage liste items (items, classes, quantiles ou quest.induites)
	UpdateTheme(IDC_RI_RAD_LIST_1);
}

void CDlgItems::OnList_2()
{
	// Affichage liste items (items, classes, quantiles ou quest.induites)
	UpdateTheme(IDC_RI_RAD_LIST_2);
}

void CDlgItems::OnUpdate_ModeItem(CCmdUI *pCmdUI)
{
	if (m_Id_CurMode == Mode_Items) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_ModeItem()
{
	if(m_Id_CurMode != Mode_Items)
	{
		// Avant tout, voir si des élmts en construction
		if (Sauvegarde_ElmtEnConstruction()) return;

		// Pas d'élmt mode précédent en construction, on passe au mode item
		OnTout_Raz();
		UpdateMode(Mode_Items);
	}
}

void CDlgItems::OnUpdate_ModeClasse(CCmdUI *pCmdUI)
{
	if (m_Id_CurMode == Mode_Classes) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_ModeClasse()
{
	if(m_Id_CurMode != Mode_Classes)
	{
		// Avant tout, voir si des élmts en construction
		if (Sauvegarde_ElmtEnConstruction()) return;

		// Pas d'élmt mode précédent en construction, on passe au mode classe
		OnTout_Raz();
		UpdateMode(Mode_Classes);
	}
}

void CDlgItems::OnUpdate_ModeQuantile(CCmdUI *pCmdUI)
{
	if (m_Id_CurMode == Mode_Quantiles) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_ModeQuantile()
{
	if(m_Id_CurMode != Mode_Quantiles)
	{
		// Avant tout, voir si des élmts en construction
		if (Sauvegarde_ElmtEnConstruction()) return;

		// Pas d'élmt mode précédent en construction, on passe au mode quantile
		OnTout_Raz();
		UpdateMode(Mode_Quantiles);
	}
}

void CDlgItems::OnUpdate_ModePalette(CCmdUI *pCmdUI)
{
	if (m_Id_CurMode == Mode_Palettes) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Passage en mode Palette
void CDlgItems::OnChange_ModePalette()
{
	// Passe en mode palette
	if(m_Id_CurMode != Mode_Palettes)
	{
		// Avant tout, voir si des élmts en construction
		if (Sauvegarde_ElmtEnConstruction()) return;

		// Pas d'élmt mode précédent en construction, on passe au mode palette
		OnTout_Raz();

		// Défaut sélection opérateur sas OU en mode palette
		OnSAS_Ou();
		UpdateMode(Mode_Palettes);

		// Active le mode sélection compo palette en dim1
		m_AjoutSelCompoPalette_Dim1 = true;

		// Non actif en dimension 2
		m_AjoutSelCompoPalette_Dim2 = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Voir sauvegarde demandée pour élmt item ou palette ou autre en constrcution
JBool CDlgItems::Sauvegarde_ElmtEnConstruction()
{
	if (m_Id_CurMode == Mode_Items || m_Id_CurMode == Mode_Palettes)
	{
		// Si rien dans le Sas, sauvegarde inutile, on peut passer au mode suivant
		if (m_List_SAS.GetCount() == 0)
			return false;

		// demande de sauvegarde ou pas de la sélection précédente en cours
		else if (AfxMessageBox("Une sélection est en cours, voulez-vous la sauvegarder" ,MB_YESNO) == IDYES)
		{
			return true;
		}
	}
	else if (m_Id_CurMode == Mode_QuestInduites || m_Id_CurMode == Mode_Quantiles || m_Id_CurMode == Mode_Classes)
	{
		// Cas du mode classes, quantiles, questions induites
		if (m_List_Dimension1.GetCount() == 0)
			return false;

		else if (m_List_Dimension1.GetSelCount() == 0)
			return false;

		// demande de sauvegarde ou pas de la sélection précédente en cours
		else if (AfxMessageBox("Une sélection est en cours, voulez-vous la sauvegarder" ,MB_YESNO) == IDYES)
		{
			return true;
		}
	}

	// Pour les autres cas, on peut passer au mode suivant
	return false;
}

void CDlgItems::OnUpdate_ModeInduite(CCmdUI *pCmdUI)
{
	if (m_Id_CurMode == Mode_QuestInduites)
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_ModeInduite()
{
	if(m_Id_CurMode != Mode_QuestInduites)
	{	
		// Avant tout, voir si des élmts en construction
		if (Sauvegarde_ElmtEnConstruction()) return;

		OnTout_Raz();
		UpdateMode(Mode_QuestInduites);
	}
}

void CDlgItems::OnChange_Dim_1_PaletteSel()
{
	// Change le mode ajout dimension 1, et mise à jour avec raz des composants palette
	ActiveModeAjoutDim1(true);
}

///////////////////////////////////////////////////////////////////////////
// Change mode activation de la dimension 1 pour constrcution palette
void CDlgItems::ActiveModeAjoutDim1(bool RazCompoPalette)
{
	// Changement sélection palette
	if(m_pCurMode)
	{
		// Mise à jour du mode activation sélection
		if (m_Button_Dimension1.GetCheck())
		{
			// Active le mode ajout sélection courant
			ActiveModeSel(m_Id_CurMode,1,true);
		}
		else
		{
			// Désactive le mode ajout sélection courant
			ActiveModeSel(m_Id_CurMode,1,false);
		}

		/*
		// @@@ Aout 2013 voir si aucune incidence
		if (m_Id_CurMode == Mode_Palettes)
		{
			// On force à activation dimension 1
			ActiveModeSel(m_Id_CurMode,1,true);

		}
		*/
		
		// Attention aussi à la dimension 2 (si non visible)
		// Mise à jour du mode activation sélection
		if (!m_Button_Dimension2.IsWindowVisible())
		{
			// Désactive le mode ajout sélection courant en dimension 2
			ActiveModeSel(m_Id_CurMode,2,false);
		}

		// Modifie le mode sélection palette dimension 1
		m_pCurMode->OnChange_Dim_1_PaletteSel();

		// Seulement si on est en mode palette
		if (m_Id_CurMode == Mode_Palettes)	
		{
			// Sauvegarde nouveau mode sélection dim1
			if(m_List_Questions.GetCurSel() != LB_ERR)
			{
				JInt32 IDs = m_List_Questions.GetSelectedID();
				SaveModeSelectionDim1Dim2(IDs);
			}
			
			// Mise à jour composition palette
			UpdateListSelCompoPalette(RazCompoPalette);
		}
	}
}

void CDlgItems::OnChange_Dim_2_PaletteSel()
{
	if(m_pCurMode)
	{	
		if (m_Id_CurMode != Mode_Palettes)	
		{
			// Changement mode palette en dimension 2
			m_pCurMode->OnChange_Dim_2_PaletteSel();
		}
		else if (m_Id_CurMode == Mode_Palettes)	
		{
			// Mise à jour du mode activation sélection
			if (m_Button_Dimension2.GetCheck())
			{
				// Active le mode ajout sélection courant
				ActiveModeSel(m_Id_CurMode,2,true);
			}
			else
			{
				// Désactive le mode ajout sélection courant
				ActiveModeSel(m_Id_CurMode,2,false);
			}

			// Changement mode palette en dimension 2
			m_pCurMode->OnChange_Dim_2_PaletteSel();

			// Sauvegarde nouveau mode sélection dim2
			if(m_List_Questions.GetCurSel() != LB_ERR)
			{
				JInt32 IDs = m_List_Questions.GetSelectedID();
				SaveModeSelectionDim1Dim2(IDs);
			}

			// Mise à jour de la sélection tris croisés
			UpdateListSelCompoPalette();
		}
	}
}

void CDlgItems::OnSelchange_TabSegment(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_pCurMode)
	{

		// Rend inactif la liste résult
		// m_List_Result.ShowWindow(SW_HIDE);
		m_List_Result.SetRedraw(FALSE); 

		// Changement de segment
		m_pCurMode->OnSelchange_TabSegment(pNMHDR,pResult);

		// Modifie les calculs
		m_pCurMode->OnChange_SegmentCalc(); 

		// Si nouveau segment, on met les opérateurs par défaut
		if (m_List_Dimension1.GetCount() == 0)
			AllOperateurParDefaut();

		// Mise à jour sélection
		UpdateListSelCompoPalette();

		// Mise à jour des opérateurs actifs
		SetUpdateOpeLogiques();

		// Rend actif la liste résult
		// m_List_Result.ShowWindow(SW_SHOW);
		m_List_Result.SetRedraw(TRUE);
	}

	*pResult = 0;
}

// Suppression contenu d'un item
void CDlgItems::OnItem_Raz() 
{
	// Reset the item label text
	m_Edit_Name.SetWindowText("");

	// Sauve position liste question
	JInt32 PosTopIndex = m_List_Questions.GetTopIndex();	

	if(m_pCurMode)
		m_pCurMode->OnItem_Raz();

	// Update compo mode palette
	if (m_Id_CurMode == Mode_Palettes)
		UpdateListSelCompoPalette();

	// Mettre les opérateurs par défaut
	AllOperateurParDefaut();

	// Reposition liste question
	if (PosTopIndex < m_List_Questions.GetCount())  
	{
		m_List_Questions.SetTopIndex(PosTopIndex);	
		m_List_Questions.Invalidate();
	}
}

// Suppression de toute la sélection
void CDlgItems::OnTout_Raz()
{
	// Reset the item label text
	m_Edit_Name.SetWindowText("");

	if(m_pCurMode)
	{
		m_pCurMode->OnTout_Raz();
	}

	if(m_List_Items.GetSelCount()) 
		m_List_Items.ToggleSelection();  

	// Update compo mode palette
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Init map mode sélection dim1 et dim2 pour chaque question
		m_MapModeSelQuestionDim1.Reset(); 
		m_MapModeSelQuestionDim2.Reset(); 

		// Par défaut non actif 
		m_AjoutSelCompoPalette_Dim1 = true;
		m_AjoutSelCompoPalette_Dim2 = false;

		// Mise à jour liste des compositions palette
		UpdateListSelCompoPalette();
	}

	// Mettre les opérateurs par défaut
	AllOperateurParDefaut();

	// Update de toutes les dimensions
	m_pCurMode->UpdateDimensionsList();
}

///////////////////////////////////////////////////////////////
// Positionnement de tous les opérateurs par défaut
void CDlgItems::AllOperateurParDefaut()
{
	// Les opérateurs par défaut en mode questions, items, quantiles etc....
	if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
	{
		// En mode palette ou pupitre
		// Défaut sélection opérateur dim1 et dim2
		OnDim_1_Ou();
		OnDim_2_Ou();

		// Défaut sélection opérateur sas question
		OnSAS_Et();

		// Défaut sélection opérateur segment
		OnSegment_Ou();
	}
	else
	// Les opérateurs par défaut en mode palette
	{
		// Défaut sélection opérateur dim1 et dim2
		OnDim_1_Ou();
		OnDim_2_Ou();

		// Défaut sélection opérateur sas question
		OnSAS_Ou();
	}

	// En mode palette, classe, quantile et quest induite  les opérateurs sur dimension 1 et dimension 2 inactif
	if (m_Id_CurMode == Mode_Palettes  || 
		m_Id_CurMode == Mode_Classes   || 
		m_Id_CurMode == Mode_Quantiles ||
		m_Id_CurMode == Mode_QuestInduites)
	{
		// Rend invisible et désactive opérateur dim1 et dim2
		SetOpeDimension1(false);
		SetOpeDimension2(m_Box_Dimension2.IsWindowVisible());
	}
	else
	{
		// Remet visible et active les opérateurssur dim1 et dim2
		SetOpeDimension1(true);
		SetOpeDimension2(m_Box_Dimension2.IsWindowVisible());
	}

	// Label entete résultat redessiné, sinon bizarrement il ne se repaint pas
	m_Label_DefItem.		SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_DefItem.		SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	m_Label_DefItem.		SetFontBold(true);

	m_Label_DefItem.		Invalidate(); 
	m_Label_DefItem.		RedrawWindow(); 
}

// Suppression élmt du Sas
void CDlgItems::OnSAS_Raz() 
{
	if(m_pCurMode)
	{
		m_pCurMode->OnSAS_Raz();

		// Update compo mode palette
		if (m_Id_CurMode == Mode_Palettes)
			UpdateListSelCompoPalette();

		// Mise à jour des opérateurs
		SetUpdateOpeLogiques();
	}
}

// Suppression élmt segment
void CDlgItems::OnSegment_Raz() 
{
	if(m_pCurMode)
	{
		// Sauve position liste question
		JInt32 PosTopIndex = m_List_Questions.GetTopIndex();	

		// Vire élmts du segment
		m_pCurMode->OnSegment_Raz();

		// Update compo mode palette
		if (m_Id_CurMode == Mode_Palettes)
			UpdateListSelCompoPalette();

		// Mise à jour des opérateurs
		SetUpdateOpeLogiques();

		// Reposition liste question
		if (PosTopIndex < m_List_Questions.GetCount())  
		{
			m_List_Questions.SetTopIndex(PosTopIndex);	
			m_List_Questions.Invalidate();
		}
	}
}

void CDlgItems::OnSize(UINT nType, int cx, int cy) 
{
	// Resize des composants
	CDialog::OnSize(nType, cx, cy);

	m_Resizer.Move();

	// Resize colonne tableau résultat
	if (m_List_Result)
	{
		// Répartition largeur des 3 colonnes (libellé, nb cas, eff(000))
		CRect rect;
		m_List_Result.GetClientRect(&rect);
		int nColInterval = rect.Width()/12;
		m_List_Result.SetColumnWidth(0, rect.Width()-(5*nColInterval));
		m_List_Result.SetColumnWidth(1, 37*nColInterval/16);
		m_List_Result.SetColumnWidth(2, 45*nColInterval/16);
	}
	
	// Positionne la fenêtre des valeurs
	PosFenValeursQuantis();
	
	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnSelchange_ListQuestions() 
{
	// AfxMessageBox("????", MB_ICONERROR);

	// Nouvelle question, on initialise le filtrage
	m_MapFiltrage.Reset();

	// Initialise aussi les modas dimensions 1 sélectionées
	m_MapSelModaDimension1.Reset();


	int CurSel = m_List_Questions.GetCurSel();

	// Si dans liste dimension 1 aucun item de sélectionné, on déselectionne la question dans la liste des questions
	if((m_List_Questions.GetCurSel()		== LB_ERR && 
	   m_List_Dimension1.GetCount()		!= 0 && 
	   m_List_Dimension1.GetSelCount()	== 0) || m_List_Questions.GetCurSel() < 0) 
	{
		// Dévalide la question courante
		m_List_Questions.SetCurSel(LB_ERR);

		// Vire les sélections dans toutes les dimensions
		m_List_Dimension1.ResetContent(); 
		m_List_Dimension2.ResetContent(); 


		// Modif Sept 06 A VOIR !!!!!
		// efface les fenêtres flottantes
		m_pCurMode->EffaceFenDimDlg(true);

		if (m_List_SAS.GetCount() <= 0 && m_List_Questions.GetSelCount() <= 0)
			return;
	}

	// Update de toutes les dimensions
	m_pCurMode->UpdateDimensionsList();

	/* A REVOIR : Pouvoir supprimr uniquement la question sélectionnée, même si desmodalités sont actives
	int ValCurSel = m_List_Questions.GetCurSel();
	if(m_List_Questions.GetCurSel()	== LB_ERR)
	{
		// Vire les sélections dans toutes les dimensions
		m_List_Dimension1.SetCurSel(LB_ERR);
		m_List_Dimension1.ResetContent(); 

		m_List_Dimension2.SetCurSel(LB_ERR);
		m_List_Dimension2.ResetContent(); 

		// Update de toutes les dimensions
		m_pCurMode->UpdateDimensionsList();
	}
	*/

	// Mise à jour des questions container
	if(m_pCurMode)
	{
		// Modif question via le mode courant
		m_pCurMode->OnSelchange_ListQuestions(); 

		// Récupére les infos questions, en autre pour savoir si plusieurs dimensions
		IQuestion * pCurQuestion = 0;
		JInt32 IDs = 0;
		if(m_List_Questions.GetCurSel() != LB_ERR)
		{
			// Passe id item et nb dimensions pour cette question
			pCurQuestion	= (IQuestion *)m_List_Questions.GetItemData(m_List_Questions.GetCurSel());
			IDs				= m_List_Questions.GetSelectedID();
		}

		// Uniquement pour mode palette
		if (m_Id_CurMode == Mode_Palettes)
		{
			// Récupére le paramétrage sélection
			if (IDs)
				LoadModeSelectionDim1Dim2(IDs,pCurQuestion->HasDimensions());

			// Selon les modes on positionne ou pas les boutons sélections
			SetBoutonSelection();

			// Mise à jour liste sélection compo palettes
			// Attention, ne pas mettre à jour la liste des composants palettes si question sélectionnée, et
			// pas encore de dimensions sélectionnées ........... en cours de construction
			if(m_List_Questions.GetCurSel() != LB_ERR &&  m_pCurMode->BuildSelectedItemsPalette() == false)
			{
				// ne fait rien
			}
			else
				// On reconstruit les composants palettes
				UpdateListSelCompoPalette();

			// Replace les opérateurs logiques
			SetUpdateOpeLogiques();

			// En mode palette, on remet le bouton opé dimension 2 visible mais non actif
			if (pCurQuestion && pCurQuestion->HasDimensions() > 1)
				SetOpeDimension2(true,false);
			else
				SetOpeDimension2(false,false);
		}
		else
		{
			// Replace les opérateurs logiques
			SetUpdateOpeLogiques();

			// Si 1 seul dimension, opérateur dimension 2 non visible
			if (!pCurQuestion || (pCurQuestion && pCurQuestion->HasDimensions() == 1))
				SetOpeDimension2(false,false);
		}
	}
}

void CDlgItems::OnSelchange_ListSas()
{
	if(m_pCurMode)
	{
		m_pCurMode->OnSelchange_ListSas(); 
	}

	// Repositionne les sélecteurs dim1 et dim2 pour cet élmt, via la précédente sélection
	IQuestion * pCurQuestion = 0;
	if(m_List_SAS.GetCurSel() != LB_ERR && m_List_Questions.GetCurSel() != LB_ERR)
	{
		// Passe id item et nb dimensions pour cette question
		pCurQuestion = (IQuestion *)m_List_Questions.GetItemData(m_List_Questions.GetCurSel());
		if (pCurQuestion)
		{
			JInt32 IDs = m_List_SAS.GetSelectedID();
			
			// Récupére le paramétrage sélection
			LoadModeSelectionDim1Dim2(IDs,pCurQuestion->HasDimensions());
		}
	}

	// Test bouton Dim1 Checké
	UpdateSelectionOpe();

	// Selon les modes on positionne ou pas les boutons sélections
	SetBoutonSelection();

	// Mise à jour sélection
	UpdateListSelCompoPalette();
}

///////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélection des opérateurs sur dimension 1, 2, 3 etc...
void CDlgItems::UpdateSelectionOpe()
{
	SetUpdateOpeLogiques();
}
void CDlgItems::Selchange_ListDimension_1()
{
	OnSelchange_ListDimension_1(); 
}

//////////////////////////////////////////////////////////////////////////////////
// Choix nouvel élmt via dimension 1
void CDlgItems::OnSelchange_ListDimension_1() 
{
	if(m_pCurMode)
	{
		// Précédente sélection active sur Sas
		int InxPrecSelSas = m_List_SAS.GetCurSel();

		// Change la sélection dimension1
		m_pCurMode->OnSelchange_ListDimension_1(); 

		// Recup index question courante
		JUnt32 IndexQuCur = LB_ERR;
		
		// Attention si quelque chose dans le sas et non sélectionné
		if (m_Id_CurMode == Mode_Palettes)
		{
			// SAS Non vide
			if (m_List_SAS.GetCount() > 0)
			{
				int Nb1 = m_List_Dimension1.GetCount();
				int Nb2 = m_List_Dimension2.GetSelCount();

				// Voir si aucune sélection active sur Sas désormais
				if (m_List_SAS.GetCurSel() == LB_ERR		&& 
					m_List_Dimension1.GetSelCount() <= 0	&& 
					InxPrecSelSas !=  LB_ERR				&&
					m_List_Dimension2.GetSelCount() <= 0)
				{
					// Revient sur la question en cours
					IndexQuCur = m_List_Questions.GetCurSel();

					// Repositionne élmt du sas
					m_List_SAS.SetCurSel(0);
					m_pCurMode->OnSelchange_ListSas();
				}
			}

			// Mise à jour de la liste sélection 
			if (m_AjoutSelCompoPalette_Dim1) 
			{
				m_pCurMode->OnSetDimSelection1();
			}
			UpdateListSelCompoPalette();

			// On repositionne, car dans certain cas n'apparait pas dans liste sélection ????? A REVOIR
			int InxCurSelSas = m_List_SAS.GetCurSel();
			if (InxCurSelSas != LB_ERR)
			{
				m_List_SAS.SetCurSel(InxCurSelSas); 
				OnSelchange_ListSas();
			}
		}
		else
		{
			// SAS Non vide
			if (m_List_SAS.GetCount() > 0)
			{
				// Attention activer le 1er élmt Sas ssi aucun sélectionné et que il y en avait un de sélectionner avant
				if (m_List_SAS.GetCurSel() == LB_ERR && 
					InxPrecSelSas !=  LB_ERR		 &&
					m_List_Dimension2.GetSelCount() <= 0)
				{
					// Revient sur la question en cours
					IndexQuCur = m_List_Questions.GetCurSel();

					// Repositionne élmt du sas
					m_List_SAS.SetCurSel(0);
					m_pCurMode->OnSelchange_ListSas();
				}
			}
		}

		// mise à jour opérateur courant
		SetUpdateOpeLogiques();

		// On se remet sur les modalités de la question en cours si nécessaire
		if (IndexQuCur != LB_ERR)
		{
			m_List_Questions.SetCurSel(IndexQuCur);
			OnSelchange_ListQuestions();
		}
	}
}

void CDlgItems::OnSelchange_ListDimension_2() 
{
	if(m_pCurMode)
	{
		// Précédente sélection active sur Sas
		int InxPrecSelSas = m_List_SAS.GetCurSel();

		// Change la sélection dimension1
		m_pCurMode->OnSelchange_ListDimension_2(); 

		// Attention si quelque chose dans le sas et non sélectionné
		if (m_Id_CurMode == Mode_Palettes)
		{
			// SAS Non vide
			if (m_List_SAS.GetCount() > 0)
			{
				// Attention activer le 1er élmt Sas ssi aucun sélectionné et que il y en avait un de sélectionner avant
				if (m_List_SAS.GetCurSel() == LB_ERR		&& 
					m_List_Dimension1.GetSelCount() <= 0	&& 
					InxPrecSelSas !=  LB_ERR)
				{
					// Active le 1er élmt SAS
					m_List_SAS.SetCurSel(0);
					m_pCurMode->OnSelchange_ListSas();
				}
			}

			// Mise à jour de la liste sélection 
			// m_Button_Dimension2.SetCheck(m_AjoutSelCompoPalette_Dim2);
			if (m_AjoutSelCompoPalette_Dim2) 
				m_pCurMode->OnSetDimSelection2();
			
			UpdateListSelCompoPalette();
		}
		else
		{
			// SAS Non vide
			if (m_List_SAS.GetCount() > 0)
			{
				// Attention activer le 1er élmt Sas ssi aucun sélectionné et que il y en avait un de sélectionner avant
				if (m_List_SAS.GetCurSel() == LB_ERR	&& 
					InxPrecSelSas !=  LB_ERR			&& 
					m_List_Dimension1.GetSelCount() <= 0)
				{
					// Active le 1er élmt SAS
					m_List_SAS.SetCurSel(0);
					m_pCurMode->OnSelchange_ListSas();
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Changement sélection items (cadre sélection à gauche)
void CDlgItems::OnSelchange_ListItems() 
{
	// Sauve index 1er item de la liste
	int TopIndex = m_List_Items.GetTopIndex(); 

	// Si mode en cours (items, palettes ....)
	int NbSelItem = 0;
	if(m_pCurMode)
	{
		// Change la sélection des items courants
		m_pCurMode->OnSelchange_ListItems(); 

		// Nb élmt restant sélectionnés (aussi bien en question, qu'en item ou palette)
		NbSelItem = m_List_Questions.GetCount(); 
	}
	
	// Mise à jour de la liste des items
	m_pCurMode->UpdateListItems(); 
	m_List_Items.RedrawWindow(); 

	// Remet sur la dernière position sélectionnée
	if (TopIndex >= 0)
		m_List_Items.SetTopIndex(TopIndex); 

	// A VOIR / Mise à jour des segments
	// m_pCurMode->UpdateSegments();

	// Si aucun item, on vire tout
	if (NbSelItem <= 0)
	{
		OnItem_Raz();
	}

	// Si mode item et 1 seul élmt de sélectionné, mise à jour id item pour genèse
	if (m_Id_CurMode != 1 || 
		m_List_Items.GetSelCount() != 1 || 
		m_Button_Genese.IsWindowEnabled() == false || 
		m_Button_Genese.IsWindowVisible() == false)
	{
		// Pas d'ident item à prendre en compte pour la genèse
		m_IdItemGenese = 0;
	}

	// Si mode palette, ajustement des modes compo palette si 1er élmt
	if (m_Id_CurMode == Mode_Palettes && m_List_Items.GetSelCount() >= 1)
	{
		// Si 1er élmt item, on active le mode compo palette
		m_AjoutSelCompoPalette_Dim1 = true;
		m_AjoutSelCompoPalette_Dim2 = false;
		SetBoutonSelection();

		// Active le mode ajout sélection courant pour dimension 1
		ActiveModeAjoutDim1(false);
		// OnChange_Dim_1_PaletteSel();

		// Active le mode ajout sélection courant pour dimension 2
		ActiveModeSel(m_Id_CurMode,2,true);
		m_pCurMode->OnChange_Dim_2_PaletteSel();
	}

	// Attention, mettre à jour sélection du SAS à tout moment si non vide et rien dans dimension 1
	if (m_List_SAS.GetCount() > 0 && m_List_Dimension1.GetCount() == 0) 
	{	
		// Si aucun élmt du SAS sélectionné
		int InxCur = m_List_SAS.GetCurSel();
		if (m_List_SAS.GetCurSel() == LB_ERR)
		{
			// On sélectionne au moins le 1er
			m_List_SAS.SetCurSel(0); 
			m_pCurMode->OnSelchange_ListSas();
		}
		else
			m_pCurMode->OnSelchange_ListSas();
	}

	if (m_Id_CurMode == Mode_Palettes && m_AjoutSelCompoPalette_Dim1)
		// Mise à jour de la liste sélection ligne ou colonne ou page
		UpdateListSelCompoPalette(false);

	// mise à jour oérateur courant
	SetUpdateOpeLogiques();
}

////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la list des items
LRESULT CDlgItems::OnUpdated_ItemList(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
	{
		LRESULT Result = m_pCurMode->OnUpdated_ItemList(wParam,lParam); 

		// Mise à jour sélection
		/*
		if (m_Id_CurMode == Mode_Items)
			UpdateListSelItems();
		*/

		 return Result;
	}
	return 0;
}

LRESULT CDlgItems::OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
	{
		LRESULT Result = m_pCurMode->OnUpdated_QuestionList(wParam,lParam); 

		// Mise à jour sélection
		if (m_Id_CurMode == Mode_Palettes)
			UpdateListSelCompoPalette();

		 return Result;
	}
	return 0;

}

LRESULT CDlgItems::OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		return m_pCurMode->OnChange_Dim_x_PaletteSel(wParam,lParam); 
	return 0;
}

LRESULT CDlgItems::OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
	{

		LRESULT Result = m_pCurMode->OnSelChange_ListDimension_x(wParam,lParam); 

		if (m_List_Dimension1.GetCount() <= 0)
		// if (m_pCurMode->NbDimDlg() == 0)
		{
			// Attention si quelque chose dans le sas et non sélectionné
			if (m_Id_CurMode == Mode_Palettes)
			{
				if (m_List_SAS.GetCount() > 0)
				{
					int InxCurSel = m_List_SAS.GetCurSel();

					if (m_List_SAS.GetCurSel() == LB_ERR && m_List_Dimension1.GetCount() <= 0)
					{
						m_List_SAS.SetCurSel(0);
						m_pCurMode->OnSelchange_ListSas();
					}
				}

				// Mise à jour de la liste sélection compo palette 
				UpdateListSelCompoPalette();
			}

			else
			{
				if (m_List_SAS.GetCount() > 0)
				{
					int InxCurSel = m_List_SAS.GetCurSel();

					if (m_List_SAS.GetCurSel() == LB_ERR)
					{
						m_List_SAS.SetCurSel(0);
						m_pCurMode->OnSelchange_ListSas();
					}
				}
			}
		}

		return Result;
	}
	
	return 0;

}

LRESULT CDlgItems::OnChange_Dim_ValueSel(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		m_pCurMode->OnChange_Value_PaletteSel();

	// En mode palette, mise à jour composition palette
	if (m_Id_CurMode == Mode_Palettes)
		UpdateListSelCompoPalette();

	return 0;
}

LRESULT CDlgItems::OnSelChange_ListValue(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		m_pCurMode->OnSelchange_ListValue();

	// En mode palette, mise à jour composition palette
	if (m_Id_CurMode == Mode_Palettes)
		UpdateListSelCompoPalette();

	return 0;
}

LRESULT CDlgItems::OnValuesQuantile(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		m_pCurMode->OnValuesQuantile((LONG)lParam);
	return 0;
}

// Enables or disables windows according to the selected mode
void CDlgItems::UpdateMode(int ID)
{
	GetItems()->Save();

	m_Modes.MoveTo(ID);
	if(!m_Modes.IsValid())
		return;

	CAbstractMode * pMode = m_Modes.GetItem().get();
	m_Id_CurMode = ID;

	// Change les textes opérateurs s'il y a lieu
	SetUpdateOpeLogiques();

	// On se remet par défaut avec une seule dimension
	SetDimension2Visible(false);

	// Ici on détecte changement de mode, afin de réinitialiser
	if(pMode != m_pCurMode)
	{
        if(m_pCurMode)
		{
			m_pCurMode->StopMode();
		}

		/*
		m_Button_QFilter.			SetMenu(IDR_RI_MENU_QFILTER, this, FALSE);

		LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFQUESTIONS);
		LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFMODALITES);
		LOADMENUITEMSTRING(m_Button_QFilter.GetPopupMenu(), IDM_RI_QFTOUS);
		*/

		m_Label_Questions.			ShowWindow(SW_SHOW);
		m_List_Questions.			EnableWindow(TRUE);

		// Activation des objets panel sas questions sélectionnées
		m_Button_Menu_Ope_QuSel.	EnableWindow(TRUE); 
		m_Button_SAS_ET.			EnableWindow(TRUE);
		m_Button_SAS_OU.			EnableWindow(TRUE);
		m_Button_SAS_RAZ.			EnableWindow(TRUE); 

		// Activation des objets panel dimension 1
		m_List_Dimension1.			EnableWindow(TRUE);
		m_Button_Dimension1.		EnableWindow(TRUE);
		m_Button_Dim1_ET.			EnableWindow(TRUE);
		m_Button_Dim1_OU.			EnableWindow(TRUE);
		m_Button_Dim1_NI.			EnableWindow(TRUE);
		m_Button_Dim1_XOR.			EnableWindow(TRUE);
		m_Button_Menu_Ope_Dim1.		EnableWindow(TRUE); 
		m_Button_Filtre.			EnableWindow(TRUE);
		m_Button_Dim1_UP.			EnableWindow(TRUE);
		m_Button_Dim1_DOWN.			EnableWindow(TRUE);
		m_Button_Dim1_RAZ.			EnableWindow(TRUE);

		// Activation des objets panel dimension 2
		m_List_Dimension2.			EnableWindow(TRUE);
		m_Button_Dimension2.		EnableWindow(TRUE);
		m_Button_Dim2_ET.			EnableWindow(TRUE);
		m_Button_Dim2_OU.			EnableWindow(TRUE);
		m_Button_Dim2_NI.			EnableWindow(TRUE);
		m_Button_Dim2_XOR.			EnableWindow(TRUE);
		m_Button_Menu_Ope_Dim2.		EnableWindow(TRUE); 
		m_Button_Dim2_RAZ.			EnableWindow(TRUE);
		m_Tab_Segment.				EnableWindow(TRUE);

		// Activation des objets panel segments
		m_Button_Menu_Ope_Segment.	EnableWindow(TRUE); 
		m_Button_Segment_OU.		EnableWindow(TRUE);
		m_Button_Segment_ET.		EnableWindow(TRUE);
		// m_Button_Segment_RAZ.		EnableWindow(TRUE);

		m_Tree_Theme.				EnableWindow(TRUE);
		m_Tree_Rubrique_Items.		EnableWindow(TRUE);
		m_Tree_Rubrique_Palettes.	EnableWindow(TRUE);
		m_List_Items.				EnableWindow(TRUE);

		m_Button_Title.				EnableWindow(TRUE);
		m_Button_TreeQuestions.		EnableWindow(TRUE);
        m_Button_List1.				EnableWindow(TRUE);
		m_Button_List2.				EnableWindow(TRUE);
		m_Edit_Search.				EnableWindow(TRUE);
		m_Button_RAZ_search.		EnableWindow(TRUE);
		m_Button_Genese.			EnableWindow(TRUE);
		m_Edit_QFilter.				EnableWindow(TRUE);
		m_Button_QFilter.			EnableWindow(TRUE);
		m_Button_QFilterRaz.		EnableWindow(TRUE);
		m_Button_Suppression.		EnableWindow(TRUE);
		m_Button_Accept.			EnableWindow(TRUE);
		m_Button_Genese.			EnableWindow(TRUE);
		m_Cadre_Result.				EnableWindow(TRUE);
		m_Static_Effectif.			EnableWindow(TRUE);
		m_Static_Percent.			EnableWindow(TRUE);
		m_Static_NbCas.				EnableWindow(TRUE);
		m_Edit_Name.				EnableWindow(TRUE);
		
		// Default results
		m_Edit_Name      .SetWindowText(""); 
		m_Static_Effectif.SetText("");
		m_Static_NbCas   .SetText("");
		m_Static_Percent .SetText("");

		// Mise à jour via source et terrain chargée
		const JSRCTables & Sources = JSRCTables::GetInstance ();
		const JSource*  Src = Sources.m_TBLSources .GetItem(m_Id_Source);
		const JTerrain* Ter = Sources.m_TBLTerrains.GetItem(m_Id_Source, m_Id_Terrain);
		if(Src && Ter)
		{
			if(m_pTerrain && m_pTerrain->IsLoaded())  
			{
				// mIse à jour titre terrain et segments utilisés
				UpdateTitreTerrainSeg();
			}
		}

		// Modifie logo 2eme bouton (si mode répertoire item)
		if (GetTypeRepItem() == true)
		{
			m_Cadre_Result.ShowWindow(TRUE);
			m_Chk_Rubrique_Defaut.ShowWindow(FALSE);  // TRUE desormais d'office dans rep defaut
			// m_ComboRubriques.ShowWindow(TRUE);

			// Mode répertoire des items
			// =========================
			// Si item ou palette 
			if (m_Id_CurMode == Mode_Items || m_Id_CurMode == Mode_Palettes)
			{
				m_Button_List1.SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));
				m_Button_List2.SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));

				if (m_Id_CurMode == Mode_Palettes)
					m_Cadre_Result.ShowWindow(FALSE);
				else
				{
					m_Cadre_Result.ShowWindow(TRUE);
					m_Cadre_Result.Invalidate(); 
				}

				// Logo titre apparent
				m_Button_Title.ShowWindow(SW_SHOW); 
				if (m_Id_CurMode == Mode_Items)
				{
					// Titre items
					m_Button_Title.SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));

					// Menu effacement items
					m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPPRESSION, this, FALSE);
					CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
					m_Button_Suppression.SetWindowText(labelB); 
				}
				else
				{
					// Titre palettes
					m_Button_Title.SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));

					// Menu effacement palette
					m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_PALETTE, this, FALSE);
					CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
					m_Button_Suppression.SetWindowText(labelB); 
				}
				LOADMENUITEMSTRING(m_Button_Suppression.GetPopupMenu(), IDM_RI_EFFACER_ITEM);
				LOADMENUITEMSTRING(m_Button_Suppression.GetPopupMenu(), IDM_RI_EFFACER_SEGMENT);
				LOADMENUITEMSTRING(m_Button_Suppression.GetPopupMenu(), IDM_RI_EFFACER_TOUT);

				if (m_Id_CurMode == Mode_Palettes)
				{
					m_Cadre_Result.ShowWindow(FALSE);
					m_Chk_Rubrique_Defaut.ShowWindow(FALSE);
					m_ComboRubriques.ShowWindow(FALSE);
				}
			}

			// Si Classes 
			if (m_Id_CurMode == Mode_Classes)
			{
				m_Button_List1.SetBitmaps(IDB_RI_LOGO_CLASSE, RGB(192,192,192), IDB_RI_LOGO_CLASSE, RGB(192,192,192));

				// Logo Titre classes
				m_Button_Title.ShowWindow(SW_SHOW); 
				m_Button_Title.SetBitmaps(IDB_RI_LOGO_CLASSE, RGB(192,192,192), IDB_RI_LOGO_CLASSE, RGB(192,192,192));

				// Menu effacement palette
				m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_PALETTE, this, FALSE);
				CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
				m_Button_Suppression.SetWindowText(labelB); 
			}

			// si quantiles
			if (m_Id_CurMode == Mode_Quantiles)
			{
				m_Button_List1.SetBitmaps(IDB_RI_LOGO_QUANTILE, RGB(192,192,192), IDB_RI_LOGO_QUANTILE, RGB(192,192,192));

				// Logo Titre quantiles
				m_Button_Title.ShowWindow(SW_SHOW); 
				m_Button_Title.SetBitmaps(IDB_RI_LOGO_QUANTILE, RGB(192,192,192), IDB_RI_LOGO_QUANTILE, RGB(192,192,192));

				// Menu effacement palette
				m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_PALETTE, this, FALSE);
				CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
				m_Button_Suppression.SetWindowText(labelB); 
			}	

			// mode consultation (2eme bouton = logo item, 3eme bouton = logo palette)
			if (m_Id_CurMode == Mode_Consultation)
			{
				m_Button_Title.ShowWindow(SW_HIDE); 
				m_Button_List1.SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));
				m_Button_List2.SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));

				// Menu effacement palette
				m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_PALETTE, this, FALSE);
				CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
				m_Button_Suppression.SetWindowText(labelB); 
			}

			// si question induite
			if (m_Id_CurMode == Mode_QuestInduites)
			{
				m_Button_List1.SetBitmaps(IDB_RI_LOGO_QUESTINDUITE, RGB(192,192,192), IDB_RI_LOGO_QUESTINDUITE, RGB(192,192,192));

				// Logo Titre questions induites
				m_Button_Title.ShowWindow(SW_SHOW); 
				m_Button_Title.SetBitmaps(IDB_RI_LOGO_QUESTINDUITE, RGB(192,192,192), IDB_RI_LOGO_QUESTINDUITE, RGB(192,192,192));

				// Menu effacement palette
				m_Button_Suppression.SetMenu(IDR_RI_MENU_SUPP_PALETTE, this, FALSE);
				CString labelB; LOADSTRING(labelB, IDS_RI_BUT_SUPPRIMER);
				m_Button_Suppression.SetWindowText(labelB); 
			}

			// Mode rep items
			SetVisibleBoiteValidItem(true);

			// Si mode palette, pas de fenêtre résultat, mais fenetre visu composants palettes
			if (m_Id_CurMode == Mode_Palettes)
			{	
				// Fenetre result palette visible
				SetVisibleResultPalette(true);
			}
			else
			{
				// Fenetre result palette non visible
				SetVisibleResultPalette(false);

				// Fenêtre segment non visible ssi mode item actif
				SetVisibleFenSegment(m_Id_CurMode == Mode_Items);
			}
		}

		else
		{
			// Mode Pupitre via Tris Croisés
			// =============================
			// boutons items + palettes
			m_Button_List1.SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));
			m_Button_List2.SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));

			// Pas de logo dans le titre
			m_Button_Title.ShowWindow(SW_HIDE); 

			// Mode rep items
			SetVisibleBoiteValidItem(true);
		}

		UpdateData(false);

		m_pCurMode = pMode ;
		m_bSegment = 0;
		m_Button_CalcSegment.SetCheck(m_bSegment, FALSE);

		// Vide la liste des questions, et passe en mode mono dimension
		InitListQuestion();

		if(m_pCurMode)
			m_pCurMode->StartMode();

		// Positionne le mode courant pour la liste des questions
		m_Tree_Theme.SetModeVisu(m_Id_CurMode); 

		// On modifie aspect bouton, onglet tree-theme actif
		m_Button_TreeQuestions.SetTypeOnglet(TRUE,TRUE,TRUE);
		m_Button_List1.SetTypeOnglet		(TRUE,FALSE,TRUE);
		m_Button_List2.SetTypeOnglet		(TRUE,FALSE,TRUE);
		
		OnQFilter_RAZ();

		// Opérateurs par défaut
		AllOperateurParDefaut();
	}
}

// Initialise la liste des élmts boite questionnaire
void CDlgItems::InitListQuestion()
{
	// Vire tous les composants
	m_List_Questions.ResetContent();
	m_List_Questions.Invalidate();
	
	// Passe en mode mono dimension (seul la boite dimension 1)
	SetDimension2Visible(false);

	// efface les fenêtres flottantes
	m_pCurMode->EffaceFenDimDlg(true);
}

// Update Titre terrains segments
void CDlgItems::UpdateTitreTerrainSeg()
{
	// Table général
	const JSRCTables & SrcTables = JSRCTables::GetInstance(); 

	// Terrain et source
	const JSource*  Src = SrcTables.m_TBLSources .GetItem(m_Id_Source);
	const JTerrain* Ter = SrcTables.m_TBLTerrains.GetItem(m_Id_Source, m_Id_Terrain);

	// Si valides
	if(Src && Ter)
	{
		// Nom source/terrain en cours
		CString txt;
		txt = Src->m_Libelle.AsJCharPtr();
		txt += " ";
		txt += Ter->m_Libelle.AsJCharPtr();

		// + nom segment (si +, seg.multiples)
		if (m_Id_PopSegments.GetCount() > 1)
		{
			CString TxtMultiSeg;
			LOADSTRING(TxtMultiSeg, IDS_RI_MULTISEG); 			
			// Ici indique plusieurs segments
			txt += " ";
			txt += TxtMultiSeg;
			// txt += " (Multi Seg.)";
		}	
		else
		{
			// 1 seul segment sélectionné
			m_Id_PopSegments.MoveFirst();
			if (m_Id_PopSegments.IsValid()) 
			{
				// Id du segment
				const JSegment* Seg = SrcTables.m_TBLSegments.GetItem(m_Id_PopSegments.GetItem());

				// Si ensemble, on ne l'affiche pas
				CString TxtSeg = Seg->m_Libelle.AsJCharPtr();

				CString TxtEnsemble;
				LOADSTRING(TxtEnsemble, IDS_RI_ENSEMBLE); 
				if (TxtSeg != TxtEnsemble)
				// if (TxtSeg != "Ensemble")
				{
					txt += " (";
					txt += Seg->m_Libelle.AsJCharPtr();
					txt += ")";  
				}
			}
		}
		
		// Texte final (Soure/Terrain + Info Segment(s))
		m_Label_Source.SetText(txt);
	}
}

// Displays the selected theme, support window
void CDlgItems::UpdateTheme(int ID)
{
	// Boite Tree Theme visible, boite résultat non visible, boite pupitre non visible
	SetVisibleBoiteTreeTheme(true);
	SetVisibleBoiteValidItem(true);

	// Efface les fenêtres mobiles restantes
	if (m_pCurMode)
		m_pCurMode->EffaceFenDimDlg();

	// Boite et composants pupitre non visible
	SetVisibleBoitePupitre(false);
	SetVisibleModePupitre(false);

	// Boite Compo Palette visible ssi on est en mode palette
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Rend fenêtre visu compo palette visible
		SetVisibleResultPalette(true);
	}
	else
	{
		// Rend fenêtre visu compo palette invisible
		SetVisibleResultPalette(false);
	}

	// Sauvegarde des items en cours
	GetItems()->Save();

	// Check du bouton onglet correspondant
	CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,ID);

	if(m_pCurMode)
	{
		m_pCurMode->UpdateTheme(ID);
	}
	
	// Mise à jour général de toute la fenêtre
	UpdateWindow();

	// Dessine l'onglet actif
	UpdateOngletConstruction(ID);

	if(m_pCurMode)
	{
		m_pCurMode->UpdateListItems();
	}

}

// Dessine l'onglet actif
void CDlgItems::UpdateOngletConstruction(int ID)
{
	// On modifie aspect bouton (type onglet actif)
	m_Button_TreeQuestions.SetTypeOnglet(TRUE,	FALSE,	TRUE);
	m_Button_List1.SetTypeOnglet		(TRUE,	FALSE,	TRUE);
	m_Button_List2.SetTypeOnglet		(TRUE,	FALSE,	TRUE);
		
	// Redessine le dernier actif
	if (ID == IDC_RI_RAD_QUESTIONS)
	{
		// Bouton onglet Tree Questions actif et en 1er plan
		m_Button_TreeQuestions.SetTypeOnglet(TRUE, TRUE, TRUE);
		m_Button_TreeQuestions.BringWindowToTop();
		m_Button_TreeQuestions.RedrawWindow(); 
	}
	else if (ID == IDC_RI_RAD_LIST_1)
	{
		// Bouton onglet Liste item 1 actif et en 1er plan
		m_Button_List1.SetTypeOnglet(TRUE, TRUE, TRUE);
		m_Button_List1.BringWindowToTop();
		m_Button_List1.RedrawWindow(); 
	}
	else if (ID == IDC_RI_RAD_LIST_2)
	{
		// Bouton onglet Liste item 2 actif et en 1er plan
		m_Button_List2.SetTypeOnglet(TRUE, TRUE, TRUE);
		m_Button_List2.BringWindowToTop();
		m_Button_List2.RedrawWindow(); 
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Les menus contextuels du répertoire des items
void CDlgItems::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd->GetDlgCtrlID() == IDC_RI_TREE_RUBRIC_ITEMS)
	{
		// Affichage menu contextuel gestion des rubriques items
		m_Tree_Rubrique_Items.OnContextMenu(pWnd, point); 
	}
	else if (pWnd->GetDlgCtrlID() == IDC_RI_TREE_RUBRIC_PALETTES)
	{
		// Affichage menu contextuel gestion des rubriques palettes
		m_Tree_Rubrique_Palettes.OnContextMenu(pWnd, point); 
	}
	else if(m_pCurMode)
	{
		// Affichage menu contextuel sur les items
		m_pCurMode->OnContextMenu(pWnd, point); 
	}			
}

// Sets up the theme tree
void CDlgItems::SetThemeData()
{
	m_Tree_Theme.SetData(m_pTerrain,GetItems(),m_QFilter,m_iModality);
	m_Tree_Theme.SetEtatData(&m_MapSelTreeTheme);

	// Repositionne l'index thématique via denière sélection
	m_Tree_Theme.TreeViaSauvegarde();
	m_Tree_Theme.UpdateQuestionList(); 


}

// Sets up rubriques items
void CDlgItems::SetRubriqueItems()
{
	// Pour les rubriques items
	m_Tree_Rubrique_Items.SetTypeRubrique(m_Tree_Rubrique_Items.TYPE_RUBRIQUE_ITEM);
    
	// Set tree rubriques items
	m_Tree_Rubrique_Items.LoadRubriquesItems(GetItems(), GetTerrain());
	m_Tree_Rubrique_Items.m_pItemsConstruits	= GetItems(); 
	m_Tree_Rubrique_Items.m_pTerrain			= GetTerrain(); 

	// Init état sélection rubrique
	m_Tree_Rubrique_Items.SetEtatData(&m_MapSelRubriqueItem);

	// Repositionne l'index rubriques items via dernière sélection
	m_Tree_Rubrique_Items.TreeViaSauvegarde();

	// Mise à jour affichage des items
	m_Tree_Rubrique_Items.UpdateItemList(); 

	// Rend rubriques éditables
    m_Tree_Rubrique_Items.EnableEdit(true);
	m_Tree_Rubrique_Items.EnableExpandAll(false);
}

// Sets up rubriques items
void CDlgItems::SetRubriquePalettes()
{
	// Pour les rubriques palettes
	m_Tree_Rubrique_Palettes.SetTypeRubrique(m_Tree_Rubrique_Palettes.TYPE_RUBRIQUE_PALETTE);

	// Set tree rubriques palettes
	m_Tree_Rubrique_Palettes.LoadRubriquesItems(GetItems(), GetTerrain());
	m_Tree_Rubrique_Palettes.m_pItemsConstruits	= GetItems(); 
	m_Tree_Rubrique_Palettes.m_pTerrain			= GetTerrain(); 

	// Init état sélection rubrique
	m_Tree_Rubrique_Palettes.SetEtatData(&m_MapSelRubriqueItem);

	// Repositionne l'index rubriques items via dernière sélection
	m_Tree_Rubrique_Palettes.TreeViaSauvegarde();

	// Mise à jour affichage des palettes
	m_Tree_Rubrique_Palettes.UpdateItemList(); 

	// Rend rubriques éditables
    m_Tree_Rubrique_Palettes.EnableEdit(true);
	m_Tree_Rubrique_Palettes.EnableExpandAll(false);
}

void CDlgItems::OnClose() 
{
	// Sauve dernière position fenêtre
    SavePosFenDlg();

	// Repositionne le split via dernière sauvegarde
	PosLastSplit();
	
	GetItems()->Save();
	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_COLOR_REP_ITEM, CATPColors::GetAppColor(CATPColors::COLORREPITEMS));
	if (m_ShowMode != SHOWMODE_SIMPLE)
	{
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
	}
	
	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////
// Repositionne le split via dernière sauvegarde
void CDlgItems::PosLastSplit()
{
#if !defined(BUILD_FOR_ATELIER_INTERNET)
	// @@@ REPITEMS : ATELIER INTERNET

	// Et comme la fenêtre n'est pas fermée réellement, on repositionne aussitot le spli
	// Recup position barre split
	int NewLeft;
	NewLeft = GETPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_LEFT_SPLIT_VERT, -1);   

	// Position origine split
	RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); 
	this->ScreenToClient(&RectSP);
	if (NewLeft >= 0)
	{
		// Déplacement split
		LONG Depl = NewLeft - RectSP.left;
		RectSP.left		= NewLeft;
		RectSP.right	= RectSP.right + Depl;
		DeplaceSplitVert(Depl);
	}
#endif
}

void CDlgItems::OnPatrimoine_Consult() 
{
	CPatrimoineDlg Dlg(this);
	Dlg.m_CATPColorsApp = m_CATPColorsApp;
	Dlg.DoModal();
}

void CDlgItems::OnChange_PopSegment() 
{
	CSegmentDlg Dlg(this);
	Dlg.m_CATPColorsApp = m_CATPColorsApp;
	Dlg.SetData(m_pTerrain,m_Id_PopSegments);
	int nResponse = Dlg.DoModal();
	if(nResponse == IDOK)
	{
		m_Id_PopSegments.Swap(Dlg.m_Id_PopSegments);
		m_ContingentMask = Dlg.m_ContingentMask;
		m_PopSegmentFilter = Dlg.m_PopulationSeg;
		if(m_pCurMode)
			m_pCurMode->OnChange_PopSegment();
	}

	// Mise à jour Titre Terrain / Segments
	UpdateTitreTerrainSeg();
}

void CDlgItems::OnChange_TypePoids() 
{
	CPoidsDlg  Dlg(this);
	Dlg.m_CATPColorsApp = m_CATPColorsApp;
	Dlg.SetData(m_pTerrain,m_Id_TypePoids);
	int nResponse = Dlg.DoModal();
	if(nResponse == IDOK)
	{
		m_Id_TypePoids = Dlg.m_IdPoids;

		const JSRCTables & Sources = JSRCTables::GetInstance ();
		m_Label_TypePoids.SetText(Sources.m_TBLPoids.GetLabelByID(m_Id_TypePoids));

		if(m_pCurMode)
			m_pCurMode->OnChange_TypePoids();
	}
}

void CDlgItems::OnChange_Source() 
{
	CSourceDlg Dlg(this);
	Dlg.m_CATPColorsApp = m_CATPColorsApp;
	Dlg.m_Id_Source = m_Id_Source;
	Dlg.m_Id_Terrain = m_Id_Terrain;

	int nResponse = Dlg.DoModal();
	if (nResponse == IDOK)
	{
		// Same Source and Terrain Selected
		if(Dlg.m_Id_Source == m_Id_Source && Dlg.m_Id_Terrain == m_Id_Terrain)
			return;

		// Chargement du terrain
		LoadTerrain(Dlg.m_Id_Source, Dlg.m_Id_Terrain);

		// Initialisation de la sélection index thèmatique puisque l'on cahnge de terrains
		m_Tree_Theme.ResetMapTree(); 
	}
}

bool CDlgItems::LoadTerrain(JInt32 SourceId, JInt32 TerrainId)
{
	CWaitCursor Wait;

	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JSource * Src  = Sources.m_TBLSources.GetItem(SourceId);
	const JTerrain * Ter = Sources.m_TBLTerrains.GetItem(SourceId, TerrainId);
	if(!Src || !Ter)
		return false;

	CTerrainManager* pterrainManager = CTerrainManager::GetInstance();
	if(m_pTerrain)
		pterrainManager->ReleaseTerrain(m_pTerrain);

	m_pTerrain = pterrainManager->GetTerrain(SourceId, TerrainId);
	if(m_pTerrain == NULL)
	{
		MessageBox("Erreur pendant le chargement du Terrain");
		return false;
	}


	m_Id_Source = SourceId;
	m_Id_Terrain = TerrainId;

	m_ContingentMask = 0;
	m_Id_PopSegments.Reset();
	m_PopSegmentFilter.SetSize(0);
	JVector<CSegment*> Segments;
	if(m_pTerrain->GetSegments(Segments))
	{
		JInt32 NbSegments = Segments.GetCount();
		m_Id_PopSegments.SetCount(NbSegments);
		for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1)
		{
			Segments.MoveTo(IdxSegment); m_Id_PopSegments.MoveTo(IdxSegment);
			m_ContingentMask |= Segments.GetItem()->GetContingentMask();
			if(IdxSegment > 0)
				m_PopSegmentFilter |= Segments.GetItem ()->GetPopSegment();
			else
				m_PopSegmentFilter = Segments.GetItem ()->GetPopSegment();
			m_Id_PopSegments.GetItem() = Segments.GetItem()->GetID();
		}
	}	
	if(!m_pTerrain->HasTypePoids(m_Id_TypePoids))
		m_Id_TypePoids = m_pTerrain->GetDefaultTypePoidsID(); 

	if(m_pCurMode)
	{
		m_pCurMode->StopMode();
		m_pCurMode=0;
	}


	// Set the theme tree data
	m_iModality = 0;
	m_QFilter.Empty();
	m_Edit_QFilter.SetWindowText(m_QFilter);  
	SetThemeData();

	m_Label_TypePoids.SetText(Sources.m_TBLPoids.GetLabelByID(m_Id_TypePoids));

	// Update Titre Terrain + Segment
	UpdateTitreTerrainSeg();

	// Sets the current
	CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);

	// Passe en mode Questions
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateMode(m_Id_CurMode);

	// Pour l'opérateur sur les elmts question
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);

	// Pour l'opérateur sur les elmts dimension 1
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
	
	// Pour l'opérateur sur les elmts dimension 2
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_OU);

	// Pour l'opérateur sur les segments
	CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_OU);

	// Default results
	m_Edit_Name      .SetWindowText(""); 
	m_Static_Effectif.SetText("");
	m_Static_NbCas   .SetText("");
	m_Static_Percent .SetText("");
	m_bSegment = 0;

	m_Button_CalcSegment.SetCheck(m_bSegment);

	UpdateData(false);

	return true;
}

// Change Couleur
void CDlgItems::ChangeColor(int NoColor)
{
	switch (NoColor)
	{
		case 0: 
			OnChange_Palette1();
			break;
		case 1:
			OnChange_Palette2();
			break;
		case 2:
			OnChange_Palette3();
			break;
		case 3:
			OnChange_Palette4();
			break;
		case 4:
			OnChange_Palette5();
			break;
		case 5:
			OnChange_Palette6();
			break;
		case 6:
			OnChange_Palette7();
			break;
		case 7:
			OnChange_Palette8();
			break;
		case 8:
			OnChange_Palette9();
			break;
		case 9:
			OnChange_Palette10();
			break;
		case 10:
			OnChange_Palette11();
			break;
		case 11:
			OnChange_Palette12();
			break;
		case 12:
			OnChange_Palette13();
			break;
		case 13:
			OnChange_Palette14();
			break;
		case 14:
			OnChange_Palette15();
			break;
		default:
			OnChange_Palette15();
			break;
	}
}

void CDlgItems::OnChange_SegmentCalc()
{
	m_bSegment = m_Button_CalcSegment.GetCheck();
	if(m_pCurMode)
		m_pCurMode->OnChange_SegmentCalc(); 
}

void CDlgItems::OnUpdate_Palette1(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 0) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette1()
{
	CATPColors::SetAppColor(m_CATPColorsApp,0);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette2(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 1) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette2()
{
	CATPColors::SetAppColor(m_CATPColorsApp,1);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
    SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette3(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 2) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette3()
{
	CATPColors::SetAppColor(m_CATPColorsApp,2);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();

	/*
	CATPColors::SetAppColor(m_CATPColorsApp,2);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
	*/
}

void CDlgItems::OnUpdate_Palette4(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 3) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette4()
{
	CATPColors::SetAppColor(m_CATPColorsApp,3);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette5(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 4)
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette5()
{
	CATPColors::SetAppColor(m_CATPColorsApp,4);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();

	RedrawWindow();
}

void CDlgItems::OnUpdate_Palette6(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 5) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette6()
{
	CATPColors::SetAppColor(m_CATPColorsApp,5);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette7(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 6) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette7()
{
	CATPColors::SetAppColor(m_CATPColorsApp,6);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette8(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 7) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette8()
{
	CATPColors::SetAppColor(m_CATPColorsApp,7);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette9(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 8) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette9()
{
	CATPColors::SetAppColor(m_CATPColorsApp,8);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette10(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 9) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette10()
{
	CATPColors::SetAppColor(m_CATPColorsApp,9);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette11(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 10) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette11()
{
	CATPColors::SetAppColor(m_CATPColorsApp,10);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette12(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 11) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette12()
{
	CATPColors::SetAppColor(m_CATPColorsApp,11);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();

}

void CDlgItems::OnUpdate_Palette13(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 12) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette13()
{
	CATPColors::SetAppColor(m_CATPColorsApp,12);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();
}

void CDlgItems::OnUpdate_Palette14(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 13) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette14()
{
	CATPColors::SetAppColor(m_CATPColorsApp,13);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();

}

void CDlgItems::OnUpdate_Palette15(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == 14) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgItems::OnChange_Palette15()
{
	CATPColors::SetAppColor(m_CATPColorsApp,14);

	// Nouvelle Brushs utilisés
	m_BrushColorLight.DeleteObject();
	m_BrushColorLight.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
	m_BrushColorMedium.DeleteObject(); 
	m_BrushColorMedium.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushColorDark.DeleteObject(); 
	m_BrushColorDark.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));

	// Remet les couleurs des objets
	SetColors(); 

	// Redessine le tableau
	Invalidate();
	UpdateWindow();

}


void CDlgItems::OnAboutbox()
{
	CAboutDlg dlgAbout(this);
	dlgAbout.DoModal();
}

void CDlgItems::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors(); 
}

void CDlgItems::OnRazSearch()
{
	m_Edit_Search.SetWindowText("");
	if(m_pCurMode)
		m_pCurMode->UpdateListItems();
	m_Edit_Search.SetFocus();
}

void CDlgItems::OnChanged_EditSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

void CDlgItems::OnTimer(UINT nIDEvent)
{
	KillTimer(IdTimer_Search);
	if(m_pCurMode)
		m_pCurMode->UpdateListItems();
	CDialog::OnTimer(nIDEvent);
}

void CDlgItems::OnIcones()
{
	if(m_FIcones)
		m_FIcones = 0;
	else
		m_FIcones = 1;

	if(m_FIcones)
		GetMenu()->CheckMenuItem(IDM_RI_ICONES,MF_BYCOMMAND|MF_CHECKED);
	else
		GetMenu()->CheckMenuItem(IDM_RI_ICONES,MF_BYCOMMAND|MF_UNCHECKED);

	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_OP_ICONS, m_FIcones);
	SetUpdateOpeLogiques();
}

void CDlgItems::OnPatrimoine_Import()
{
	// Import d'un patrimoine externe
	GetItems()->Import(this);

	// Réactualise distribution des nouveaux items dans les rubriques
	// Tous les nouveaux items seront d'office dans rubrique divers
	m_Tree_Rubrique_Items.ReactualiseItems(GetItems(), GetTerrain());

	// Réactualise les palettes en cours
	m_Tree_Rubrique_Palettes.ReactualiseItems(GetItems(), GetTerrain());
}

void CDlgItems::OnPatrimoine_Export()
{
	CExportDlg Dlg(this);
	Dlg.DoModal();
}

void CDlgItems::OnDestroy()
{
	// Sauve dernière position fenêtre
	SavePosFenDlg();

	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_SOURCE_ID, m_Id_Source);
	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_TERRAIN_ID, m_Id_Terrain);

	CDialog::OnDestroy();
}

void CDlgItems::SavePosFenDlg()
{
	CString strText;
	BOOL bIconic, bMaximized;

	if (!this->IsWindowVisible())
		return;

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	// lit la position courante de la fenêtre et
	// l'état (zoomé/réduit)
	BOOL bRet = GetWindowPlacement(&wndpl);
	if (wndpl.showCmd == SW_SHOWNORMAL)
	{
		bIconic = FALSE;
		bMaximized = FALSE;
	}
	else if (wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
		bIconic = FALSE;
		bMaximized = TRUE;
	} 
	else if (wndpl.showCmd == SW_SHOWMINIMIZED)
	{
		bIconic = TRUE;
		if (wndpl.flags)
			bMaximized = TRUE;
		else
			bMaximized = FALSE;
	}

	// Par sécurité vérifie position et taille fenetre à sauvegarder
	strText.Format("%04d %04d %04d %04d",
		wndpl.rcNormalPosition.left,
		wndpl.rcNormalPosition.top,
		wndpl.rcNormalPosition.right,
		wndpl.rcNormalPosition.bottom);

	// Attention sauvegarde différente si rep items ou pupitre
	if (GetTypeRepItem())
	{
		// Sauve positionnement fenêtre rep items
		WRITEPROFILESTRING(SECTION_OPTIONS_REPITEM, KEY_RECT, strText);

#if !defined(BUILD_FOR_ATELIER_INTERNET)
		// @@@ REPITEMS : ATELIER INTERNET
		if (m_SplitVert.GetSafeHwnd() != NULL)
		{
			// Sauve positionnement split vertical rép items
			RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);
			WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_LEFT_SPLIT_VERT, RectSP.left); 
		}
#endif
	}
	else
	{
		// Sauve positionnement fenêtre pupitre
		WRITEPROFILESTRING(SECTION_OPTIONS_ANALYSEUR, KEY_RECT_PUPITRE, strText);

#if !defined(BUILD_FOR_ATELIER_INTERNET)
		// @@@ REPITEMS : ATELIER INTERNET
		if (m_SplitVert.GetSafeHwnd() != NULL)
		{
			// Sauve positionnement split vertical pupitre
			RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);
			WRITEPROFILEINT(SECTION_OPTIONS_ANALYSEUR, KEY_LEFT_SPLIT_VERT, RectSP.left); 
		}
#endif
	}

	WRITEPROFILEINT(SECTION_OPTIONS_REPITEM, KEY_MAX, bMaximized);
}


void CDlgItems::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
	BOOL bIconic, bMaximized;
	UINT flags;
	WINDOWPLACEMENT wndpl;
	CRect rect;

	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;

		// Attention, positionnement différent rep items et pupitre
		if (GetTypeRepItem())
		{
			// Récup dernière position rep items
			strText = GETPROFILESTRING(SECTION_OPTIONS_REPITEM, KEY_RECT);
		}
		else
		{
			// Récup dernière position pupitre
			strText = GETPROFILESTRING(SECTION_OPTIONS_ANALYSEUR, KEY_RECT_PUPITRE);
		}

        if (!strText.IsEmpty())
		{
            rect.left	= atoi((const char*) strText);
            rect.top	= atoi((const char*) strText + 5);
            rect.right	= atoi((const char*) strText + 10);
            rect.bottom = atoi((const char*) strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_ICON, 0);
        bMaximized = GETPROFILEINTD(SECTION_OPTIONS_REPITEM, KEY_MAX, 0);   
        if (bIconic)
		{
            nStatus = SW_SHOWMINNOACTIVE;
            if (bMaximized)
                flags = WPF_RESTORETOMAXIMIZED;
            else
                flags = WPF_SETMINPOSITION;
        }
        else
		{
            if (bMaximized)
			{
                nStatus = SW_SHOWMAXIMIZED;
                flags = WPF_RESTORETOMAXIMIZED;
            }
            else
			{
                nStatus = SW_NORMAL;
                flags = WPF_SETMINPOSITION;
            }
        }
        wndpl.length = sizeof(WINDOWPLACEMENT);
        wndpl.showCmd = nStatus;
        wndpl.flags = flags;
        wndpl.ptMinPosition = CPoint(0, 0);
        wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
        wndpl.rcNormalPosition = rect;

		// définit la position de la fenêtre 
		// et l'état (zoomé/réduit)
        BOOL bRet = SetWindowPlacement(&wndpl);
    }

	CDialog::OnShowWindow(bShow, nStatus);
}

void CDlgItems::OnKeyUp(int a,int b,int c)
{
	CDialog::OnKeyUp(a,b,c);
}

void CDlgItems::OnMove(int x, int y)
{
	CDialog::OnMove(x,y);

	// Positionne la fenêtre des valeurs
	PosFenValeursQuantis();
}

void CDlgItems::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

// returns true if visualisation mode is active
bool CDlgItems::IsVisualModeActive(void)
{
	return ((m_Id_CurMode == Mode_Items	   || 
			 m_Id_CurMode == Mode_Classes  || 
			 m_Id_CurMode == Mode_Quantiles));
}

// returns true if effective should be visible
bool CDlgItems::IsEffectiveVisible(void)
{
	return ((m_Id_CurMode == Mode_Items || m_Id_CurMode == Mode_Consultation));
}

// returns Value Cur Mode
JInt32 CDlgItems::ValCurMode(void)
{
	return (m_Id_CurMode);
}

// return Titre Appli (Rep Items ou Pupitre)
CString CDlgItems::GetTitleAppli(void)
{
	return m_Title;
}

void CDlgItems::OnQFilter_Tous()
{
	m_Edit_QFilter.GetWindowText(m_QFilter);
	m_iModality = 1;
	SetThemeData();

	// Puis on déploie l'arbre pour visualiser les items recherchés
	HTREEITEM hRootItem = m_Tree_Theme.GetRootItem();
	if (hRootItem != NULL)
	{
		SetCheckItem(hRootItem);
		m_Tree_Theme.ExpandAll();
	}

	// Mise à jour du qestionnaire sélectionné
	m_Tree_Theme.UpdateQuestionList();
}

// Check elmt of the m_Tree_Theme Control
void CDlgItems::SetCheckItem(HTREEITEM hti)
{
	while(hti)
	{
		// Y a t-il une branche enfant
		if (m_Tree_Theme.ItemHasChildren(hti))
		{
			m_Tree_Theme.SetCheck(hti,TRUE);
			SetCheckItem(m_Tree_Theme.GetChildItem(hti));
		}

		m_Tree_Theme.SetCheck(hti,TRUE);
		hti = m_Tree_Theme.GetNextItem(hti,TVGN_NEXT);
	}
}


void CDlgItems::OnQFilter_Questions()
{
	m_Edit_QFilter.GetWindowText(m_QFilter);
	m_iModality = 2;
	SetThemeData();
}

void CDlgItems::OnQFilter_Modalites()
{
	m_Edit_QFilter.GetWindowText(m_QFilter);
	m_iModality = 3;
	SetThemeData();
}

void CDlgItems::OnQFilter_Palette()
{
	m_Edit_QFilter.GetWindowText(m_QFilter);
	m_iModality = 4;
	SetThemeData();
}

void CDlgItems::OnQFilter_RAZ()
{
	m_QFilter.Empty();
	m_Edit_QFilter.SetWindowText(m_QFilter);
	m_iModality = 0;
	SetThemeData();
}

///////////////////////////////////////////////////////////////////////
bool CDlgItems::IsWindowVisuVisible()
{
	// Ne sert plus
	return false;
}

// Impression possible ou pas des résultats
bool CDlgItems::ImpResultPossible()
{
	if (m_Id_CurMode != Mode_Palettes)
	{
		return true;

		// En mode construction item
		// Les valeurs
		JList<JInt32> Values;

		if (m_IdItemGenese == 0)
		{
			m_List_Items.GetSelectedID(Values);
		}
		else
		{
			Values.AddTail(m_IdItemGenese); 
		}

		// Sont-elles valides
		Values.MoveFirst();
		if (!Values.IsValid()) return false; 

		// Au moins 1 composant avec 1 segment
		if (m_List_Result.GetItemCount() > 2)
			return true;
		else
			return false;
	}
	else
	{
		// En mode palette, pas d'impression pour le moment
		//if (m_List_Compo_Palette.GetItemCount() == 0)
		return false;
	}

	return true;
}

// Visibilité Boite Theme
void CDlgItems::SetVisibleBoiteTreeTheme(bool Visible)
{
	if (Visible)
	{	
		// boite thème visible
		m_Box_Theme.ShowWindow(SW_SHOW);

		// Les listes visibles
		m_List_Items.ShowWindow(SW_SHOW);
		
		m_Tree_Theme.ShowWindow(SW_SHOW);

		// Liste rubriques items visible ssi en mode items
		if (m_Id_CurMode == Mode_Items)
		{
			m_Tree_Rubrique_Items.ShowWindow(SW_SHOW); 
			m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE); 
		}
		// Liste rubriques palettes visible ssi en mode palettes
		else if (m_Id_CurMode == Mode_Palettes)
		{
			m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
			m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW); 
		}
		// Sinon aucune liste rubriques visibles
		else
		{
			m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
			m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE); 
		}

		// Activation des boutons de cette boite (pour les tooltips)
		m_Edit_Search.			ShowWindow(SW_SHOW);
		m_Edit_QFilter.			ShowWindow(SW_SHOW);
		m_Button_RAZ_search.	ShowWindow(SW_SHOW);
		m_Button_QFilterRaz.	ShowWindow(SW_SHOW);
		m_Button_Genese.		ShowWindow(SW_SHOW);
		m_Button_QFilter.		ShowWindow(SW_SHOW);

		// Les boutons commandes du bas
		m_Button_TreeQuestions.	ShowWindow(SW_SHOW);
		m_Button_List1.			ShowWindow(SW_SHOW);
		if (m_Id_CurMode == Mode_Palettes)
			m_Button_List2.		ShowWindow(SW_SHOW);

		// En mode questions / items / palettes etc pas d'impression
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);
		m_But_Export_Item.		ShowWindow(SW_HIDE);
		m_But_Supp_Item.		ShowWindow(SW_HIDE);
		
		m_Label_DefItem.		ShowWindow(SW_SHOW);
	}		
	else
	{
		// boite thème invisible
		m_Box_Theme.ShowWindow(SW_HIDE);

		// Les listes visibles
		m_Tree_Theme.ShowWindow(SW_HIDE);
		m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
		m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE); 
		m_List_Items.ShowWindow(SW_HIDE);

		// Désactivation des boutons de cette boite
		m_Edit_Search.			ShowWindow(SW_HIDE);
		m_Edit_QFilter.			ShowWindow(SW_HIDE);
		m_Button_RAZ_search.	ShowWindow(SW_HIDE);
		m_Button_QFilterRaz.	ShowWindow(SW_HIDE);
		m_Button_Genese.		ShowWindow(SW_HIDE);
		m_Button_QFilter.		ShowWindow(SW_HIDE);

		// Les boutons commandes du bas
		// m_Button_TreeQuestions.	ShowWindow(SW_HIDE);
		// m_Button_List1.			ShowWindow(SW_HIDE);
		// m_Button_List2.			ShowWindow(SW_HIDE);

		// En mode résultat (impression et export)
		m_But_Down_Item.		ShowWindow(SW_SHOW);
		m_But_Up_Item.			ShowWindow(SW_SHOW);
		m_But_Imprimer_Result.	ShowWindow(SW_SHOW);
		m_But_Export_Item.		ShowWindow(SW_SHOW);
		m_But_Supp_Item.		ShowWindow(SW_SHOW);
	}
}

// Visibilité Boite Items Validation
void CDlgItems::SetVisibleBoiteValidItem(bool Visible)
{
	if (Visible)
	{
		// Puis on décache la boite Box_Items
		m_Box_Items.			ShowWindow(SW_SHOW);

		// Et ses composants
		m_But_Down_Item.		ShowWindow(SW_SHOW);
		m_But_Up_Item.			ShowWindow(SW_SHOW);
		m_But_Imprimer_Result.	ShowWindow(SW_SHOW);
		m_But_Export_Item.		ShowWindow(SW_SHOW);
		m_But_Supp_Item.		ShowWindow(SW_SHOW);
		m_Button_Accept.		ShowWindow(SW_SHOW);
		// m_Button_Suppression.	ShowWindow(SW_SHOW);
		m_Edit_Name.			ShowWindow(SW_SHOW);
		m_Label_Item.			ShowWindow(SW_SHOW);
		m_Label_TypePoids.		ShowWindow(SW_SHOW);
	}
	else
	{
		// Puis on cache la boite Box_Items
		m_Box_Items.			ShowWindow(SW_HIDE);

		// Et ses composants
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);
		m_But_Export_Item.		ShowWindow(SW_HIDE);
		m_But_Supp_Item.		ShowWindow(SW_HIDE);
		m_Button_Accept.		ShowWindow(SW_HIDE);
		// m_Button_Suppression.	ShowWindow(SW_HIDE);
		m_Edit_Name.			ShowWindow(SW_HIDE);
		m_Label_Item.			ShowWindow(SW_HIDE);
		m_Label_TypePoids.		ShowWindow(SW_HIDE);
	}
}

// Visibilité Boite Résultat
void CDlgItems::SetVisibleBoiteResultat(bool Visible)
{
	if (Visible)
	{	
		// boite résultat visible
		m_Box_Result.			ShowWindow(SW_SHOW);
		
		// Et active
		m_List_Result.			ShowWindow(SW_SHOW);
	
		// Icone impression
		m_But_Down_Item.		ShowWindow(SW_SHOW);
		m_But_Up_Item.			ShowWindow(SW_SHOW);
		m_But_Imprimer_Result.	ShowWindow(SW_SHOW);
		m_But_Export_Item.		ShowWindow(SW_SHOW);
		m_But_Supp_Item.		ShowWindow(SW_SHOW);
		m_Label_DefItem.		ShowWindow(SW_SHOW);
	}		
	else
	{
		// boite résultat non visible
		m_Box_Result.			ShowWindow(SW_HIDE);
		
		// Et non active
		m_List_Result.			ShowWindow(SW_HIDE);

		// Icone impression
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);
		m_But_Export_Item.		ShowWindow(SW_HIDE);
		m_But_Supp_Item.		ShowWindow(SW_HIDE);
	}
}

// Visibilité Boite Visu Palettes Rep Items
void CDlgItems::SetVisibleResultPalette(bool Visible)
{
	if (Visible)
	{	
		// Et boite résultat inactive
		m_List_Result.			ShowWindow(SW_HIDE);

		// Liste compo palette visible
		m_List_Compo_Palette.	ShowWindow(SW_SHOW);
	
		// Entete titre liste "Composition Palette"
		CString text;
		LOADSTRING(text, IDS_RI_LBL_PALETTE_COMPO);
		m_Label_DefItem.SetWindowText(text); 

		// On rend invisible les composants de la fenêtre segment
		SetVisibleFenSegment(false);
	}		
	else
	{
		// Et boite résultat non active
		m_List_Result.			ShowWindow(SW_SHOW);

		// Liste compo palette non visible
		m_List_Compo_Palette.	ShowWindow(SW_HIDE);

		// Entete titre liste "Item en construction"
		CString text;
		LOADSTRING(text, IDS_RI_LBL_ITEM_CONSTRUCTION);
		m_Label_DefItem.SetWindowText(text); 

		// On rend visible les composants de la fenêtre segment (ssi en mode item)
		SetVisibleFenSegment(m_Id_CurMode == Mode_Items);
	}
}

// On rend visible ou invisible les composants de la fenêtre segment
void CDlgItems::SetVisibleFenSegment(bool Visible)
{
	if (Visible)
	{
		// On rend visible les composants de la fenêtre segment
		m_Box_Segment.				ShowWindow(SW_SHOW);
		m_Button_CalcSegment.		ShowWindow(SW_SHOW);
		m_Tab_Segment.				ShowWindow(SW_SHOW);
		m_Button_Menu_Ope_Segment.	ShowWindow(SW_SHOW);
		// m_Button_Segment_RAZ.		ShowWindow(SW_SHOW);

		if (this->GetTypeRepItem())
		{
			// Repositionne la fenetre SAS au dessus de la boite segment
			// Resize la boite Sas en hauteur
			CRect RectBoxSAS;
			m_Box_SAS.GetWindowRect(&RectBoxSAS);
			CRect RectBoxSeg;
			m_Box_Segment.GetWindowRect(&RectBoxSeg);

			// Hauteur de recadrage de la boite SAS
			JUnt32 Recal = RectBoxSAS.bottom - RectBoxSeg.top + 5;  

			// On redéfinit la position bottom de la lboite SAS
			RectBoxSAS.bottom -= Recal; 
			this->ScreenToClient(&RectBoxSAS);
			m_Box_SAS.SetWindowPos(NULL, RectBoxSAS.left,	RectBoxSAS.top,
										 RectBoxSAS.Width(),RectBoxSAS.Height() ,SWP_NOZORDER);

			// Resize la liste Sas en hauteur
			CRect RectListSAS;
			m_List_SAS.GetWindowRect(&RectListSAS);
			RectListSAS.bottom -= Recal;
			this->ScreenToClient(&RectListSAS);
			m_List_SAS.SetWindowPos(NULL, RectListSAS.left,	RectListSAS.top,
										  RectListSAS.Width(),RectListSAS.Height() ,SWP_NOZORDER);


			// Resize le titre Type de poids
			CRect	RectBtnCheckSeg;
			m_Button_CalcSegment.GetWindowRect(&RectBtnCheckSeg);
			CRect   RectLabelPoids;
			m_Label_TypePoids.GetWindowRect(&RectLabelPoids);
			RectLabelPoids.right = RectBtnCheckSeg.left - 5;
			this->ScreenToClient(&RectLabelPoids);
			m_Label_TypePoids.SetWindowPos(NULL, RectLabelPoids.left,	RectLabelPoids.top,
				RectLabelPoids.Width(),RectLabelPoids.Height() ,SWP_NOZORDER);
		}
	}
	else
	{
		// On rend invisible les composants de la fenêtre segment
		m_Box_Segment.				ShowWindow(SW_HIDE);
		m_Button_CalcSegment.		ShowWindow(SW_HIDE);
		m_Tab_Segment.				ShowWindow(SW_HIDE);
		m_Button_Menu_Ope_Segment.	ShowWindow(SW_HIDE);
		m_Button_Segment_OU.		ShowWindow(SW_HIDE);
		m_Button_Segment_ET.		ShowWindow(SW_HIDE);
		// m_Button_Segment_RAZ.		ShowWindow(SW_HIDE);

		if (this->GetTypeRepItem())
		{
			// Repositionne la fenetre SAS à la place de la boite segment
			// Resize la boite Sas en hauteur
			CRect RectBoxSeg;
			m_Box_Segment.GetWindowRect(&RectBoxSeg);
			CRect RectBoxSAS;
			m_Box_SAS.GetWindowRect(&RectBoxSAS);

			// Hauteur de recadrage de la boite SAS
			JUnt32 Recal = RectBoxSeg.bottom - RectBoxSAS.bottom;  
			RectBoxSAS.bottom += Recal; 
			this->ScreenToClient(&RectBoxSAS);
			m_Box_SAS.SetWindowPos(NULL, RectBoxSAS.left,	RectBoxSAS.top,
										 RectBoxSAS.Width(),RectBoxSAS.Height() ,SWP_NOZORDER);



			// Resize la liste Sas en hauteur
			CRect RectListSAS;
			m_List_SAS.GetWindowRect(&RectListSAS);
			RectListSAS.bottom += Recal;
			this->ScreenToClient(&RectListSAS);
			m_List_SAS.SetWindowPos(NULL, RectListSAS.left,	RectListSAS.top,
										  RectListSAS.Width(),RectListSAS.Height() ,SWP_NOZORDER);

			

			// Resize le titre Type de poids
			CRect	RectBtnCheckSeg;
			m_Button_CalcSegment.GetWindowRect(&RectBtnCheckSeg);

			CRect   RectLabelPoids;
			m_Label_TypePoids.GetWindowRect(&RectLabelPoids);
			RectLabelPoids.right = RectBtnCheckSeg.right;
			this->ScreenToClient(&RectLabelPoids);
			m_Label_TypePoids.SetWindowPos(NULL, RectLabelPoids.left,	RectLabelPoids.top,
				RectLabelPoids.Width(),RectLabelPoids.Height() ,SWP_NOZORDER);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste compo palette courante via les sélections mode palette
void CDlgItems::UpdateListSelCompoPalette(bool Raz)
{
	// Recup Liste sélection lignes si on n'efface pas tout
	JList <CString> LstTmp;
	if (!Raz)
	{
		// Récup les composants de la palette en cours, et les stocke en attente
		JInt32 NbElmt = m_List_Compo_Palette.GetItemCount();
		for (int i= 0; i < m_List_Compo_Palette.GetItemCount(); i++)
		{
			LstTmp.AddTail() = m_List_Compo_Palette.GetItemText(i,0);
		}
	}

	// Efface les composants palette de la liste visu
	m_List_Compo_Palette.DeleteAllItems();
	m_List_Compo_Palette.Invalidate();
	m_List_Compo_Palette.RedrawWindow();

	// Uniquement en mode palette
	if (m_pCurMode && m_Id_CurMode == Mode_Palettes)
	{
		if(m_pCurMode->BuildSelectedItemsPalette())
		{
			m_pCurMode->GetSelectedItems(m_ItemsPalette);

			if (m_ItemsPalette.GetCount() > 0)
			{
                // Récupére liste des items sélections
				JList<CItemWrapper *> List;
				GetSelectedItems(List);

				// Add them to the List control and item count
				int i = m_List_Compo_Palette.GetItemCount(); 
				for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
				{
					// Entete ligne
					CString StrEntete = List.GetItem()->GetHeader();

					// Suite ligne
					CString StrLigne = List.GetItem()->GetLabel();

					if (StrLigne != "Error")
					{
						CString Str;

						// Format the string simple
						Str.Format("%s",StrLigne); 

						// Add to the list control
						m_List_Compo_Palette.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());
					}
				}
			}
		}

		else
		{
			// Aucune sélection sur la liste en cours
			// Init Liste sélection compo palette
			m_List_Compo_Palette.DeleteAllItems();

			// On remet les précédents composants, car construction non active mais composants présents !!!
			if (!Raz)
			{
				// A Voir si on récupère la liste précédente
				if (LstTmp.GetCount() > 0)
				{
					int i = m_List_Compo_Palette.GetItemCount(); 
					for (LstTmp.MoveFirst(); LstTmp.IsValid(); LstTmp.MoveNext())
					{
						// Add to the list control
						CString Str = LstTmp.GetItem();
						m_List_Compo_Palette.InsertItem(i,Str);
					}
				}
			}
		}
	}

	// En attente >> Impression ou export possible ou pas (à revoir car dans certains cas item en construction)
	if (ImpResultPossible())
	{
		m_But_Imprimer_Result.	EnableWindow(TRUE);
		m_But_Export_Item.		EnableWindow(TRUE);
		m_But_Supp_Item.		EnableWindow(TRUE);
	}
	else
	{
		m_But_Imprimer_Result.	EnableWindow(FALSE);
		m_But_Export_Item.		EnableWindow(FALSE);
		m_But_Supp_Item.		EnableWindow(FALSE);
	}

	if (m_Id_CurMode == Mode_Palettes)
	{
		/*
		m_But_Down_Item.		EnableWindow(TRUE);
		m_But_Up_Item.			EnableWindow(TRUE);
		*/
		m_But_Down_Item.		EnableWindow(FALSE);
		m_But_Up_Item.			EnableWindow(FALSE);
	}
}

// Visibilité Boite Pupitre
void CDlgItems::SetVisibleBoitePupitre(bool Visible)
{
	if (Visible)
	{	
        // Boite pupitre visible
		m_Box_Pupitre.			ShowWindow(SW_SHOW);

		// Entete titre item
		m_Label_DefItem.		ShowWindow(SW_HIDE);

		// Les boutons tri sélection
		m_Btn_All_Lignes.		ShowWindow(SW_SHOW);

		// pas de boutons tri
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);

		// Pas de bouton imprimer
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);

		// pas de bouton export item
		m_But_Export_Item.		ShowWindow(SW_HIDE);

		// pas de bouton suppression item
		m_But_Supp_Item.		ShowWindow(SW_HIDE);

		// Bouton spéciale sélecteur items pour moyenne
		m_Button_Item_Moy_Lig.	ShowWindow(SW_SHOW);	
		//m_Button_Item_Moy_Col.	ShowWindow(SW_SHOW);	

		// Les boutons up/down non actifs pour le moment
		m_Button_Ligne_UP.		ShowWindow(SW_SHOW);
		m_Button_Ligne_DOWN.	ShowWindow(SW_SHOW);

		// Affichage nb cellules
		m_Label_NbCells.		ShowWindow(SW_SHOW);
	}		
	else
	{
		// boite pupitre non visible
		m_Box_Pupitre.			ShowWindow(SW_HIDE);
		
		// Puis non visible les listes sélections
		m_List_Row.				ShowWindow(SW_HIDE);
		m_Label_Lignes.			ShowWindow(SW_HIDE); 
		m_List_Col.				ShowWindow(SW_HIDE);
		m_Label_Colonnes.		ShowWindow(SW_HIDE); 
		m_List_Page.			ShowWindow(SW_HIDE);
		m_Label_Pages.			ShowWindow(SW_HIDE);

		// Affichage nb cellules
		m_Label_NbCells.		ShowWindow(SW_HIDE);

		// Les boutons pour sites en SQN-Profiling
		m_Btn_SitesEnLigne.		ShowWindow(SW_HIDE);
		m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
		m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);

		// Les boutons tri sélection
		m_Btn_All_Lignes.		ShowWindow(SW_HIDE);
		m_Btn_All_Colonnes.		ShowWindow(SW_HIDE);
		m_Btn_All_Pages.		ShowWindow(SW_HIDE);

		// Décache les boutons spéciaux sélecteur d'items pour moyenne
		m_Button_Item_Moy_Lig.	ShowWindow(SW_HIDE);
		m_Button_Item_Moy_Col.	ShowWindow(SW_HIDE);

		// Décache Les boutons tris
		m_Button_Ligne_UP.		ShowWindow(SW_HIDE);
		m_Button_Ligne_DOWN.	ShowWindow(SW_HIDE);
		m_Button_Colonne_UP.	ShowWindow(SW_HIDE);
		m_Button_Colonne_DOWN.	ShowWindow(SW_HIDE);
		m_Button_Page_UP.		ShowWindow(SW_HIDE);
		m_Button_Page_DOWN.		ShowWindow(SW_HIDE);

		// Ainsi que les entêtes liste sélection
		// A FAIRE
	}
}

// Visibilité Mode Pupitre
void CDlgItems::SetVisibleModePupitre(bool Visible)
{
	if (Visible)
	{
		// Boite Onglet pupitre
		m_Box_Onglet_Pupitre.ShowWindow(SW_SHOW);

		// Les boutons mode pupitre
		m_Button_Mode_Ligne.ShowWindow(SW_SHOW);
		m_Button_Mode_Colonne.ShowWindow(SW_SHOW);
		m_Button_Mode_Page.ShowWindow(SW_SHOW);

		// Pas de bouton logo titre
		m_Button_Title.ShowWindow(SW_HIDE); 

		// Les boutons validation et cancel
		m_Button_OK.ShowWindow(SW_SHOW);			
		m_Button_Cancel.ShowWindow(SW_SHOW);	

		// On la rend invisible ainsi que ses composants
		m_Box_Segment.				ShowWindow(SW_HIDE);
		m_Button_CalcSegment.		ShowWindow(SW_HIDE);
		m_Tab_Segment.				ShowWindow(SW_HIDE);
		m_Button_Menu_Ope_Segment.	ShowWindow(SW_HIDE);
		m_Button_Segment_OU.		ShowWindow(SW_HIDE);
		m_Button_Segment_ET.		ShowWindow(SW_HIDE);
		// m_Button_Segment_RAZ.		ShowWindow(SW_HIDE);

		// Pas d'impression ni d'export en mode pupitre
		m_But_Down_Item.			ShowWindow(SW_HIDE);
		m_But_Up_Item.				ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.		ShowWindow(SW_HIDE);
		m_But_Export_Item.			ShowWindow(SW_HIDE);
		m_But_Supp_Item.			ShowWindow(SW_HIDE);
	}
	else
	{
		// Boite Onglet pupitre
		m_Box_Onglet_Pupitre.ShowWindow(SW_HIDE);

		// Les boutons mode pupitre
		m_Button_Mode_Ligne.ShowWindow(SW_HIDE);
		m_Button_Mode_Colonne.ShowWindow(SW_HIDE);
		m_Button_Mode_Page.ShowWindow(SW_HIDE);

		// Pas de bouton logo titre
		m_Button_Title.ShowWindow(SW_SHOW); 

		// Les boutons validation et cancel
		m_Button_OK.ShowWindow(SW_HIDE);			
		m_Button_Cancel.ShowWindow(SW_HIDE);

		// On la rend visible, ainsi que ses composants
		m_Box_Segment.				ShowWindow(SW_SHOW);
		m_Button_CalcSegment.		ShowWindow(SW_SHOW);
		m_Tab_Segment.				ShowWindow(SW_SHOW);
		m_Button_Menu_Ope_Segment.	ShowWindow(SW_SHOW);
		// m_Button_Segment_RAZ.		ShowWindow(SW_SHOW);
		// m_Button_Segment_OU.		ShowWindow(SW_SHOW);
		// m_Button_Segment_ET.		ShowWindow(SW_SHOW);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//						Interface graphique boite de dialogue Rep Items
void CDlgItems::ResizeInterfaceItem()
{
	// Hauteur boite onglet pupitre
	const int HtBoxPupitre	= 17;
	
	// Coordonnées de la fenêtre
	CRect RectBoxFen;
	this->GetWindowRect(&RectBoxFen);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Boite pupitre se place tout en bas, et non visible
	// Coordonnées de la boite pupitre
	CRect RectBoxOngletPupitre;
	m_Box_Onglet_Pupitre.GetWindowRect(&RectBoxOngletPupitre);
	RectBoxOngletPupitre.top	= RectBoxFen.bottom - 1;
	RectBoxOngletPupitre.bottom = RectBoxOngletPupitre.top;
	this->ScreenToClient(&RectBoxOngletPupitre);
	m_Box_Onglet_Pupitre.SetWindowPos(NULL, RectBoxOngletPupitre.left, RectBoxOngletPupitre.top,
											RectBoxOngletPupitre.Width(),RectBoxOngletPupitre.Height() ,SWP_NOZORDER);
	m_Box_Onglet_Pupitre.GetWindowRect(&RectBoxOngletPupitre);


	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Placement Titre du haut et boite Nb Cellules (si Pupitre)
	//
	// CRect RectBoxNbCell;
	// m_Label_NbCells.GetWindowRect(&RectBoxNbCell);
	CRect RectTot;
	m_Box_Source.GetWindowRect(RectTot);

	CRect RectBoxLabelSrc;
	m_Label_Source.GetWindowRect(RectBoxLabelSrc);
	RectBoxLabelSrc.right = RectTot.right - 5;
	RectBoxLabelSrc.left  = RectTot.left + 5;
	this->ScreenToClient(&RectBoxLabelSrc);
	m_Label_Source.SetWindowPos(NULL, RectBoxLabelSrc.left, RectBoxLabelSrc.top,
									  RectBoxLabelSrc.Width(),RectBoxLabelSrc.Height() ,SWP_NOZORDER);


	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Replacement des composants boite segment
	//
	// Déplace fenêtre segment
	CRect RectBox;
	m_Box_Segment.GetWindowRect(&RectBox);
	int DeplaceH	= RectBox.top; 
	int HtBox		= RectBox.Height();
	RectBox.bottom	= RectBoxOngletPupitre.top - BordureJfc;
	RectBox.top		= RectBox.bottom - HtBox; 
	this->ScreenToClient(&RectBox);
	m_Box_Segment.SetWindowPos(NULL, RectBox.left, RectBox.top,
									 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	m_Box_Segment.GetWindowRect(&RectBox);
	DeplaceH = RectBox.top - DeplaceH;

	// Et ses composants
	// Bouton mode calcul segment
	m_Button_CalcSegment.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_CalcSegment.SetWindowPos(NULL, RectBox.left, RectBox.top,
											RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	// Tabulation segment
	m_Tab_Segment.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Tab_Segment.SetWindowPos(NULL, RectBox.left, RectBox.top,
									 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	
	// Bouton opération sur segment
	m_Button_Menu_Ope_Segment.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Menu_Ope_Segment.SetWindowPos(NULL, RectBox.left, RectBox.top,
												 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	// Bouton opération OU segment
	m_Button_Segment_OU.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Segment_OU.SetWindowPos(NULL, RectBox.left, RectBox.top,
										   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Bouton opération OU segment
	m_Button_Segment_ET.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Segment_ET.SetWindowPos(NULL, RectBox.left, RectBox.top,
										   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	m_Button_Segment_RAZ.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Segment_RAZ.SetWindowPos(NULL, RectBox.left, RectBox.top,
		RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Replacement des composants boite validation
	//
	// Déplacer la boite validation
	m_Box_Items.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Box_Items.SetWindowPos(NULL, RectBox.left, RectBox.top,
			 					   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Et le composant suppression
	m_Button_Suppression.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Suppression.SetWindowPos(NULL, RectBox.left, RectBox.top,
			 								RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Et le composant édition libellé item
	m_Edit_Name.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Edit_Name.SetWindowPos(NULL, RectBox.left, RectBox.top,
			 					   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	
	// Et le composant label libellé item
	m_Label_Item.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Label_Item.SetWindowPos(NULL, RectBox.left, RectBox.top,
		RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	// Et le composant acceptation de l'item
	m_Button_Accept.GetWindowRect(&RectBox);
	RectBox.top		+= DeplaceH;
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_Accept.SetWindowPos(NULL,	RectBox.left, RectBox.top,
			 							RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Replacement des composants fenêtre SAS
	//
	// Agrandit la boite Sas en hauteur
	m_Box_SAS.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Box_SAS.SetWindowPos(NULL, RectBox.left,	RectBox.top,
												RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Agrandit la liste questions SAS
	m_List_SAS.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_List_SAS.SetWindowPos(NULL, RectBox.left,	RectBox.top,
												RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Label SAS inchangé
	// Bouton menu opération questions SAS inchangé
	// Bouton menu option ET inchangé
	// Bouton menu option OU inchangé
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Replacement des composants fenêtre résultat
	//				
	// Agrandit la boite résultat
	m_Box_Result.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Box_Result.SetWindowPos(NULL, RectBox.left, RectBox.top,
									RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);
	
	// Agrandit liste résult
	m_List_Result.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_List_Result.SetWindowPos(NULL, RectBox.left, RectBox.top,
									 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Agrandit liste compo palette dans le même temps
	m_List_Compo_Palette.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_List_Compo_Palette.SetWindowPos(NULL, RectBox.left, RectBox.top,
											RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Replace label type poids
	m_Label_TypePoids.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Label_TypePoids.SetWindowPos(NULL, RectBox.left, RectBox.top,
										 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace cadre résultat
	m_Cadre_Result.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Cadre_Result.SetWindowPos(NULL, RectBox.left, RectBox.top,
									  RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace le label effectif
	m_Label_Effectif.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Label_Effectif.SetWindowPos(NULL, RectBox.left, RectBox.top,
										RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace le label nb cas
	m_Label_NbCas.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Label_NbCas.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					 RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace le label pourcentage
	m_Label_Percent.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Label_Percent.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					    RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace l'info effectif 
	m_Static_Effectif.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Static_Effectif.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					     RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace l'info pourcentage
	m_Static_Percent.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Static_Percent.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					    RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Replace l'info nb cas
	m_Static_NbCas.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Static_NbCas.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					  RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	// Replace l'info check rubrique par défaut
	m_Chk_Rubrique_Defaut.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Chk_Rubrique_Defaut.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	// Replace l'info liste rubriques par défaut
	m_ComboRubriques.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_ComboRubriques.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Replacement des composants fenêtre tree items et liste items
	//				
	// Agrandit la boite listes
	m_Box_Theme.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Box_Theme.SetWindowPos(NULL, RectBox.left, RectBox.top,
								   RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Agrandit liste arbre question
	m_Tree_Theme.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Tree_Theme.SetWindowPos(NULL, RectBox.left, RectBox.top,
								    RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// La liste des rubriques items reste fixe
	/* A VOIR
	m_Tree_Rubrique_Items.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Tree_Rubrique_Items.SetWindowPos(NULL, RectBox.left, RectBox.top,
								    RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);
	*/

	// Agrandit liste items
	m_List_Items.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_List_Items.SetWindowPos(NULL, RectBox.left, RectBox.top,
								    RectBox.Width(),RectBox.Height() ,SWP_NOZORDER);

	// Deplace onglet questions
	m_Button_TreeQuestions.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_TreeQuestions.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 					          RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Deplace onglet list1 (items, ou palettes, ou classes etc....)
	m_Button_List1.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_List1.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 			          RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Deplace onglet list2 (palettes)
	m_Button_List2.GetWindowRect(&RectBox);
	RectBox.bottom	+= DeplaceH;
	RectBox.top		+= DeplaceH;
	this->ScreenToClient(&RectBox);
	m_Button_List2.SetWindowPos(NULL, RectBox.left, RectBox.top,
				 			          RectBox.Width(),RectBox.Height() ,SWP_NOZORDER|SWP_NOSIZE);


	
		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//						Interface graphique boite de dialogue Pupitre
void CDlgItems::ResizeInterfacePupitre(JBool PosSelectionBas)
{
	// Hauteur boite onglet pupitre
	const int HtBoxPupitre	= 17;

	// Coordonnées de la fenêtre
	CRect RectBoxFen;
	this->GetWindowRect(&RectBoxFen);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//						Placement Titre du haut et boite Nb Cellules (si Pupitre)
	//
	CRect RectBoxNbCell;
	m_Label_NbCells.GetWindowRect(&RectBoxNbCell);

	CRect RectTot;
	m_Box_Source.GetWindowRect(RectTot);

	CRect RectBoxLabelSrc;
	m_Label_Source.GetWindowRect(RectBoxLabelSrc);
	RectBoxLabelSrc.right = RectTot.right - RectBoxNbCell.Width() - 10;
	RectBoxLabelSrc.left = RectTot.left + 5;

	this->ScreenToClient(&RectBoxLabelSrc);
	m_Label_Source.SetWindowPos(NULL, RectBoxLabelSrc.left, RectBoxLabelSrc.top,
		RectBoxLabelSrc.Width(),RectBoxLabelSrc.Height() ,SWP_NOZORDER);

	// Boite segment sans hauteur (car lien avec boite SAS) et composants non visible
	CRect RectBoxResult;
	m_Box_Items.GetWindowRect(&RectBoxResult);
	int HtDiffY = RectBoxResult.Height() + BordureJfc; 

	// Repositionne la boite segment en bas de la fenêtre
	CRect RectBoxSeg;
	m_Box_Segment.GetWindowRect(&RectBoxSeg);
	RectBoxSeg.top += HtDiffY;				
	this->ScreenToClient(&RectBoxSeg);
	m_Box_Segment.SetWindowPos(NULL, RectBoxSeg.left,	RectBoxSeg.top,
										RectBoxSeg.Width(),RectBoxSeg.Height() ,SWP_NOZORDER|SWP_NOSIZE);

	// Récupére nouveau coordonnées boite segment
	m_Box_Segment.GetWindowRect(&RectBoxSeg);

	// Resize de la boite SAS
	CRect RectBoxSAS;
	m_Box_SAS.GetWindowRect(&RectBoxSAS);
	RectBoxSAS.bottom = RectBoxSeg.top - BordureJfc;
	this->ScreenToClient(&RectBoxSAS);
	m_Box_SAS.SetWindowPos(NULL, RectBoxSAS.left,	RectBoxSAS.top,
									RectBoxSAS.Width(),RectBoxSAS.Height() ,SWP_NOZORDER|SWP_NOMOVE);

	// 2 modes possibles
	if (!PosSelectionBas)
	{
		// Resize liste élmt SAS
		CRect RectListSAS;
		m_List_SAS.GetWindowRect(&RectListSAS);

		// Alignement bottom sur liste tree them à gauche
		CRect RectTreeTheme;
		m_Tree_Theme.GetWindowRect(&RectTreeTheme);

		// RectListSAS.bottom += HtDiffY;				
		RectListSAS.bottom = RectTreeTheme.bottom; 

		// Largeur des boutons flèches ligne / colonne /page
		CRect RectFlecheMode;
		m_Button_Mode_Ligne.GetWindowRect(&RectFlecheMode);
		RectListSAS.right  -= RectFlecheMode.Width();

		// Retaille
		this->ScreenToClient(&RectListSAS);
		m_List_SAS.SetWindowPos(NULL, RectListSAS.left,	RectListSAS.top,
										RectListSAS.Width(),RectListSAS.Height() ,SWP_NOZORDER|SWP_NOMOVE);
	}
	else
	{
		// Position Box SAS
		CRect RectBoxSAS;
		m_Box_SAS.GetWindowRect(&RectBoxSAS);

		// Resize liste élmt SAS
		CRect RectListSAS;
		m_List_SAS.GetWindowRect(&RectListSAS);

		// Alignement bottom sur liste tree them à gauche
		CRect RectTreeTheme;
		m_Tree_Theme.GetWindowRect(&RectTreeTheme);

		// RectListSAS.bottom += HtDiffY;				
		RectListSAS.bottom = RectTreeTheme.bottom; 

		// Hauteur des boutons flèches ligne / colonne /page
		CRect RectFlecheMode;
		m_Button_Mode_Ligne.GetWindowRect(&RectFlecheMode);
		int HtFleche = RectFlecheMode.Height(); 

		// Retaille largeur flèches
		this->ScreenToClient(&RectListSAS);
		m_List_SAS.SetWindowPos(NULL, RectListSAS.left,	RectListSAS.top,
										RectListSAS.Width(),RectListSAS.Height() ,SWP_NOZORDER|SWP_NOMOVE);

		// Replace les boutons fleches
		CRect RectFlecheLig;
		CRect RectFlecheCol;
		CRect RectFlechePage;
		m_Button_Mode_Ligne.GetWindowRect(&RectFlecheLig);
		m_Button_Mode_Ligne.GetWindowRect(&RectFlecheCol);
		m_Button_Mode_Ligne.GetWindowRect(&RectFlechePage);

		// Largeur Liste SAS
		m_List_SAS.GetWindowRect(&RectListSAS);

		// Place les flèches en hauteur
		RectFlecheLig.top	 = RectListSAS.bottom  + BordureJfc/2 + 1;
		RectFlecheCol.top	 = RectListSAS.bottom  + BordureJfc/2 + 1;
		RectFlechePage.top	 = RectListSAS.bottom  + BordureJfc/2 + 1;
		RectFlecheLig.bottom = RectFlecheLig.top   + HtFleche;
		RectFlecheCol.bottom = RectFlecheCol.top   + HtFleche;
		RectFlechePage.bottom = RectFlechePage.top + HtFleche;

		// Place les flèches en largeur
		JUnt32 LargOrigine = RectFlecheLig.right - RectFlecheLig.left;
		RectFlecheLig.left   =  RectListSAS.left;
		RectFlecheLig.right  =  RectFlecheLig.left  + LargOrigine;
		RectFlecheCol.left   =  RectFlecheLig.right + 2;
		RectFlecheCol.right  =  RectFlecheCol.left  + LargOrigine;
		RectFlechePage.left  =  RectFlecheCol.right + 2;
		RectFlechePage.right  =  RectFlechePage.left  + LargOrigine;
        
		// Retaille les boutons de sélection lignes, colonnes, pages
		this->ScreenToClient(&RectFlecheLig);
		m_Button_Mode_Ligne.SetWindowPos(NULL, RectFlecheLig.left, RectFlecheLig.top,
											   RectFlecheLig.Width(),RectFlecheLig.Height() ,SWP_NOZORDER);
		this->ScreenToClient(&RectFlecheCol);
		m_Button_Mode_Colonne.SetWindowPos(NULL, RectFlecheCol.left, RectFlecheCol.top,
											     RectFlecheCol.Width(),RectFlecheCol.Height() ,SWP_NOZORDER);
		this->ScreenToClient(&RectFlechePage);
		m_Button_Mode_Page.SetWindowPos(NULL, RectFlechePage.left, RectFlechePage.top,
											  RectFlechePage.Width(),RectFlechePage.Height() ,SWP_NOZORDER);
	}

	// Réactive les relations position objets
	SetWindowRelations();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Affichage item
void CDlgItems::AfficherItem()
{
	// Visualisation pour items, palettes (voir classe, quantiles
	if(m_Id_CurMode  == Mode_Items	   || 
		m_Id_CurMode == Mode_Palettes  || 
		m_Id_CurMode == Mode_Classes   || 
		m_Id_CurMode == Mode_Quantiles)
	{
		
		CRect Rect1,RectNew;
		m_Box_Theme.GetWindowRect(&Rect1);
		RectNew.left	= Rect1.left-7;
		RectNew.right	= Rect1.right+7;
		RectNew.top		= Rect1.top-7;
		RectNew.bottom	= Rect1.bottom+6;

		// Cas VerifierItem bouton
		if(m_pCurMode)
			m_pCurMode->DoVisualisation();
	}
}

// Remplacement chaines caractères sur élmt modalités
void CDlgItems::RemplacerStrItem(JList <JInt32> &ListIDs)
{
	// Mise en place fenêtre dialogue remplacement
	CDlgReplaceLib DlgReplace(this);
	DlgReplace.m_CATPColorsApp = this->m_CATPColorsApp;

	// Passe la liste des idents
	DlgReplace.m_pListIDs = &ListIDs; 

	if(IDOK == DlgReplace.DoModal())
	{
		// Remplacement des chaines
		for (ListIDs.MoveFirst(); ListIDs.IsValid(); ListIDs.MoveNext())
		{
			// Récupère index item
			JInt32 IDItem = ListIDs.GetItem();

			// Récupère item
			IItem * pItem = GetItems()->GetIItemByID(IDItem);
			if(pItem && pItem->CanModify())
			{
				// Récupération du libellé item
				const JLabel & LibItem = pItem->GetLabel();
				
				// Remplacement chaine
				CString StrLibItem;
				StrLibItem = LibItem.AsJCharPtr(); 
				int Pos = StrLibItem.Find(DlgReplace.m_EditBoxRemplace);
				if (StrLibItem.Find(DlgReplace.m_EditBoxRemplace) >= 0)
				{
					// Chaine trouvée on la remplace
					StrLibItem.Replace(DlgReplace.m_EditBoxRemplace,DlgReplace.m_EditBoxRemplacePar); 

					// Nouveau label
					JLabel NewLabel;
					NewLabel = StrLibItem;

					// Récupération du libellé item
					pItem->SetLabel(NewLabel);
				}
				
				// Test
				int Toto = 0;
			}
		}
	}
}

// All questions selected for the palette
void CDlgItems::OnQuestions_Tous()
{
	if(m_pCurMode)
		m_pCurMode->OnQuestions_Tous(); 
}

void CDlgItems::OnQuitter()
{
	this->SendMessage(WM_CLOSE);
}

void CDlgItems::OnTvnSelchangedRiTreeTheme(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	*pResult = 0;
}

// Get CurMode
CAbstractMode	* CDlgItems::GetCurMode()
{
	return m_pCurMode;
}

///////////////////////////////////////////////////////////////////////////////////////
// Bouton impression structure de l'item courant
void CDlgItems::OnBnClickedRiBtnImpitem()
{
	if (m_Id_CurMode != Mode_Palettes)
	{
		JList<JInt32> Values;

		// A VOIR
		if (m_IdItemGenese == 0 && m_Id_CurMode == Mode_Items)
		{
			// Récupère le 1er item sélectionné
			m_List_Items.GetSelectedID(Values);

			// Export item en construction
			CString LabelConstruction;
			m_Edit_Name.GetWindowText(LabelConstruction);
			if (Values.GetCount() == 0 || LabelConstruction == "")
			{
				OnExportItemEnConstruction(Values,true);
				return;
			}
		}

		// Repositionne data liste en construction
		JList<CVisuItems> Item;
		SetData(Item);

		if (m_IdItemGenese == 0)
		{
			// Récupère le 1er item sélectionné
			m_List_Items.GetSelectedID(Values);
		}
		else
		{
			// Ajoute ident item de la genèse
			Values.AddTail() = m_IdItemGenese;
		}

		// On se positionne sur le 1er item
		Values.MoveFirst();
		if (!Values.IsValid()) return; 
		
		// Liste des items correspondants
		JList<JInt32> ItemValues;

		// Récupère instance item construit
		JInt32 IdItem = Values.GetItem();
		IItem *pItem = GetItems()->GetIItemByID(Values.GetItem());
		
		if(pItem && pItem->IsItemConstruit())
			ItemValues.AddTail() = Values.GetItem();
		else
		{
			JList<JInt32> Items;
			if(pItem->IsPalette())
				pItem->AsPalette()->GetItems(Items);
			else if(pItem->IsCritereModifie())
				pItem->AsCritereModifie()->GetItems(Items);
			else if(pItem->IsCritereQuantile())
				pItem->AsCritereQuantile()->GetItems(Items);
			else if(pItem->IsQuestionInduite())
				pItem->AsQuestionInduite()->GetItems(Items);
			
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				ItemValues.AddTail() = Items.GetItem();
		}

		// Export descriptif item, avec impression
		OnExportItem(ItemValues, true);
	}
	else
	{
		// Export liste composants palette
		OnExportPalette();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Export composition item
void CDlgItems::OnBnClickedRiBtnExportItem()
{
	if (m_Id_CurMode != Mode_Palettes)
	{
		JList<JInt32> Values;
		
		// A VOIR
		if (m_IdItemGenese == 0 && m_Id_CurMode == Mode_Items)
		{
			// Récupère le 1er item sélectionné
			m_List_Items.GetSelectedID(Values);

			// Export item en construction
			CString LabelConstruction;
			m_Edit_Name.GetWindowText(LabelConstruction);
			if (Values.GetCount() == 0 || LabelConstruction == "")
			{
				OnExportItemEnConstruction(Values,false);
				return;
			}
		}

		// Repositionne data liste en construction
		JList<CVisuItems> Item;
		SetData(Item);

		if (m_IdItemGenese == 0)
		{
			// Récupère le 1er item sélectionné
			m_List_Items.GetSelectedID(Values);
		}
		else
		{
			// Ajoute ident item de la genèse
			Values.AddTail() = m_IdItemGenese;
		}

		// On se positionne sur le 1er item
		Values.MoveFirst();
		if (!Values.IsValid()) return; 
		
		// Liste des items correspondants
		JList<JInt32> ItemValues;

		// Récupère instance item construit
		JInt32 IdItem = Values.GetItem();
		IItem *pItem = GetItems()->GetIItemByID(Values.GetItem());
		
		if(pItem && pItem->IsItemConstruit())
			ItemValues.AddTail() = Values.GetItem();
		else
		{
			JList<JInt32> Items;
			if(pItem->IsPalette())
				pItem->AsPalette()->GetItems(Items);
			else if(pItem->IsCritereModifie())
				pItem->AsCritereModifie()->GetItems(Items);
			else if(pItem->IsCritereQuantile())
				pItem->AsCritereQuantile()->GetItems(Items);
			else if(pItem->IsQuestionInduite())
				pItem->AsQuestionInduite()->GetItems(Items);
			
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				ItemValues.AddTail() = Items.GetItem();
		}

		// Export descriptif item, uniquement dans clipboard
		OnExportItem(ItemValues, false);
	}
	else
	{
		// Export liste composants palette
		OnExportPalette();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Suppression composition item
void CDlgItems::OnBnClickedRiBtnSuppItem()
{
	if (m_Id_CurMode != Mode_Palettes)
	{
		OnRiEffacerItem();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Decalage item vers le bas de composition item
void CDlgItems::OnBnClickedRiBtnDownItem()
{
	if (m_Id_CurMode == Mode_Palettes)
	{
		int NbItemsPalette = m_List_Compo_Palette.GetItemCount();
		
		// On reclasse uniquement si 1 seul element sélectioné
		if (m_List_Compo_Palette.GetSelectedCount() == 1)
		{

			int IdxSelect = m_List_Compo_Palette.GetSelectionMark();
			if (IdxSelect < (m_List_Compo_Palette.GetItemCount()-1))
			{
				JList <CString> LstTmp;
				JList<CItemWrapper *> ListItemWrapper;
				JList<CItemWrapper *> ListSelectItemWrapper;

				// Récup les composants de la palette en cours, et les stocke en attente
				JInt32 NbElmt = m_List_Compo_Palette.GetItemCount();
				CString TxtDecal = "";
				CItemWrapper *pItemWrapperDecal = 0;
				for (int i= 0; i < m_List_Compo_Palette.GetItemCount(); i++)
				{
					CString Text = m_List_Compo_Palette.GetItemText(i,0);

					if (i != IdxSelect)
					{
						LstTmp.AddTail() = m_List_Compo_Palette.GetItemText(i,0);
						ListItemWrapper.AddTail() = (CItemWrapper*)m_List_Compo_Palette.GetItemData(i);
					}

					if (i == IdxSelect)
					{
						TxtDecal = m_List_Compo_Palette.GetItemText(i,0);
						pItemWrapperDecal = (CItemWrapper*)m_List_Compo_Palette.GetItemData(i);
					}
					else
					{
						if (TxtDecal != "")
						{
							LstTmp.AddTail() = TxtDecal;
							ListItemWrapper.AddTail() = pItemWrapperDecal;
							TxtDecal = "";
						}
					}
				}

				// Sauve en attelnte la liste des wrapper selected
				

				// Vide tous les elmts compo palette
				m_List_Compo_Palette.DeleteAllItems();

				// On remet dans l'ordre
				int i = 0;
				
				for (LstTmp.MoveFirst(),ListItemWrapper.MoveFirst(); LstTmp.IsValid(),ListItemWrapper.IsValid(); LstTmp.MoveNext(), ListItemWrapper.MoveNext())
				{
					// Add to the list control
					CString Str = LstTmp.GetItem();
					// m_List_Compo_Palette.InsertItem(i,Str);
					m_List_Compo_Palette.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)ListItemWrapper.GetItem());

					CItemWrapper *pTestWrap = ListItemWrapper.GetItem();

					i++;
				}
		
				// On remet les bonnes sélections dans l'ordre
				ListItemWrapper.MoveFirst();
				m_pCurMode->SetSelectedItems(ListItemWrapper);

				// Remet la sélection
				m_List_Compo_Palette.SetSelectionMark(IdxSelect+1);
				m_List_Compo_Palette.SetItemState(IdxSelect+1 ,LVIS_SELECTED, LVIS_SELECTED);

				m_List_Compo_Palette.Invalidate();
				m_List_Compo_Palette.RedrawWindow();

				// Redessine la liste
				m_List_Compo_Palette.RedrawItems(0, m_List_Compo_Palette.GetItemCount()-1);

				if (m_List_Compo_Palette.GetSelectedCount() == 1)
				{

					int IdxSelect = m_List_Compo_Palette.GetSelectionMark();
					bool Sel =true;
				}

				//////////////////////////// A VOIR ///////////////////////////////
				/*
				// Get the Modalies to reorder
				JList<JInt32> Values;
				// Get the selected ID in the list box
				m_Tree_Rubrique_Palettes.m_pDlgItems->m_List_Dimension1.GetSelectedID(Values);
				if(Values.GetCount() != 1)
					return;
				Values.MoveFirst();

				JList<JInt32> Ids;
				JVector<const IModalite *> vMods;

				CAbstractMode * pMode = m_Modes.GetItem().get();

				CItemMode *pItemMode = static_cast<CItemMode*>(pMode);

				if (pItemMode)
				{
					if (pItemMode->m_pValuesDlg)
					{
						pItemMode->m_pSelectedQuestion->GetIModalites(vMods,0,0,m_Tree_Rubrique_Palettes.m_pDlgItems->GetContingentMask());

						for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
						{
							JInt32 Val = vMods.GetItem()->GetID();
							Ids.AddTail() = vMods.GetItem()->GetID(); 
						}

						// Reorder the Ids
						if(pItemMode->OrderValues(false,Ids, Values.GetItem()))
						{
						}
					}
				}
				*/
				//////////////////////////////////////////////////////////////////////


			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Decalage item vers le haut de composition item
void CDlgItems::OnBnClickedRiBtnUpItem()
{
	if (m_Id_CurMode == Mode_Palettes)
	{
		// Nombre d'items de la palette en cours de construction
		int NbItemsPalette = m_List_Compo_Palette.GetItemCount();

		// On reclasse uniquement si 1 seul element sélectioné
		if (m_List_Compo_Palette.GetSelectedCount() == 1)
		{
			int IdxSelect = m_List_Compo_Palette.GetSelectionMark();
			if (IdxSelect >0)
			{
				JList <CString> LstTmp;

				// Récup les composants de la palette en cours, et les stocke en attente
				JInt32 NbElmt = m_List_Compo_Palette.GetItemCount();
				CString TxtDecal = "";
				for (int i= m_List_Compo_Palette.GetItemCount()-1; i>=0; i--)
				{
					CString Text = m_List_Compo_Palette.GetItemText(i,0);

					if (i != IdxSelect)
						LstTmp.AddTail() = m_List_Compo_Palette.GetItemText(i,0);

					if (i == IdxSelect)
						TxtDecal = m_List_Compo_Palette.GetItemText(i,0);
					else
					{
						if (TxtDecal != "")
						{
							LstTmp.AddTail() = TxtDecal;
							TxtDecal = "";
						}
					}
				}

				// Vide tous les elmts compo palette
				m_List_Compo_Palette.DeleteAllItems();

				// On remet dans l'ordre
				int i = 0;
				for (LstTmp.MoveLast(); LstTmp.IsValid(); LstTmp.MovePrev())
				{
					// Add to the list control
					CString Str = LstTmp.GetItem();
					m_List_Compo_Palette.InsertItem(i,Str);
					i++;
				}

				// Remet la sélection
				m_List_Compo_Palette.SetSelectionMark(IdxSelect-1);
				m_List_Compo_Palette.SetItemState(IdxSelect-1 ,LVIS_SELECTED, LVIS_SELECTED);

				m_List_Compo_Palette.Invalidate();
				m_List_Compo_Palette.RedrawWindow();

				// Redessine la liste
				m_List_Compo_Palette.RedrawItems(0, m_List_Compo_Palette.GetItemCount()-1);

				if (m_List_Compo_Palette.GetSelectedCount() == 1)
				{
					int IdxSelect = m_List_Compo_Palette.GetSelectionMark();
					bool Sel =true;
				}



			}
		}
	}
}

// Export de l'entete création d'un item
void CDlgItems::ExportEntete(JList<JInt32> &Values, CString &TxtMacro, bool ItemEnConstruction)
{
	// Init texte 
	TxtMacro = "";
	CString Text;

	if (ItemEnConstruction)
	{
		// Titre de la macro
		// Text.Format("%s\n","EXPORT ITEM EN CONSTRUCTION");
		Text.Format("%s\n","EXPORT ITEM");
		TxtMacro += Text;
		
		// Source
		CString Tmp;
		m_Label_Source.GetWindowText(Tmp);
		Text.Format("%s\t%s\n","Source : ", Tmp);
		TxtMacro += Text;

		// Poids
		const JSRCTables & srcTables = JSRCTables::GetInstance(); 

		// All segments
		const JVector<JInt32> & Segments= GetSegments();
		for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
		{
			const JSegment* Seg = srcTables.m_TBLSegments.GetItem(Segments.GetItem());
			Text.Format("%s\t%s\n","Segment de population : ",Seg->m_Libelle.AsJCharPtr()); 
			TxtMacro += Text;
		}
		
		// Type de Poids
		JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(GetTypePoids());
		Text.Format("%s\t%s\n","Type de poids : ",pLibelle);
		TxtMacro += Text;		

		// Nom composant
		Text.Format("%s\t%s\n","Libellé de l'item : ","item en construction...");
		TxtMacro += Text;

		// Calcul pour la cible
		CCalcItemParams Params;
		Params.SetTerrain(GetTerrain());
		Params.SetEchantillonID(1);
		Params.SetUniversID(GetTerrain()->GetDefaultUnivers());
		Params.SetTypePoidsID(GetTypePoids());
		Params.SetPopulationSegment(GetPopulationSegment());
		Params.SetSegmentsIDs(GetSegments());

		/*
		// Attention au flag Calcul Segment ou Calcul Global
		m_bSegment = m_Button_CalcSegment.GetCheck();
		if (m_bSegment == 1)
		{
			// Arrêt du calcul demandé 
			if (AfxMessageBox("Attention le temps de calcul est plus long en multi-insertions, désirez-vous quand même effectuer le calcul ?",MB_YESNO|MB_ICONQUESTION) == IDNO)
				return;

			// On repasse en mode calcul global
			m_Button_CalcSegment.SetCheck(BST_UNCHECKED);
			OnChange_SegmentCalc();
		}
		*/

		// Effectif Cible
		if(GetShowResults()&SHOW_EFFECTIVE)
		{
			CString TxtEff;
			m_Static_Effectif.GetWindowText(TxtEff);
			Text.Format("%s\t%s\n","Effectif : ",TxtEff);
		}
		else
			Text = "\n";
		TxtMacro += Text;

		// Pourcentage cibleXML percent tag
		if(GetShowResults()&SHOW_PERCENT)
		{
			CString TxtPrct;
			m_Static_Percent.GetWindowText(TxtPrct);
			Text.Format("%s\t%s\n","Pourcentage : ",TxtPrct);
		}
		else
			Text = "\n";
		TxtMacro += Text;
		
		// Nb Cas Cible
		if(GetShowResults()&SHOW_NBCAS)
		{
			CString TxtNbCas;
			m_Static_NbCas.GetWindowText(TxtNbCas);
			Text.Format("%s\t%s\n","Nombre de cas : ",TxtNbCas);
		}
		else
			Text = "\n";
		TxtMacro += Text;
	}
	else
	{
		// Titre de la macro
		Text.Format("%s\n","EXPORT ITEM");
		TxtMacro += Text;

		for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
		{
			// Récupère l'item
			CItemConstruit * pItem = GetItems()->GetItemConstruit(Values.GetItem());

			// Item non valide
			if(!pItem || !pItem->IsValid(GetTerrain()))
				continue;

			// Source
			CString Tmp;
			m_Label_Source.GetWindowText(Tmp);
			Text.Format("%s\t%s\n","Source : ", Tmp);
			TxtMacro += Text;

			// Poids
			const JSRCTables & srcTables = JSRCTables::GetInstance(); 

			// All segments
			const JVector<JInt32> & Segments= GetSegments();
			for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
			{
				const JSegment* Seg = srcTables.m_TBLSegments.GetItem(Segments.GetItem());
				Text.Format("%s\t%s\n","Segment de population : ",Seg->m_Libelle.AsJCharPtr()); 
				TxtMacro += Text;
			}
			
			// Type de Poids
			JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(GetTypePoids());
			Text.Format("%s\t%s\n","Type de poids : ",pLibelle);
			TxtMacro += Text;		

			// Nom composant
			Text.Format("%s\t%s\n","Libellé de l'item : ",pItem->GetLabel().AsJCharPtr());
			TxtMacro += Text;
			
			// Calcul pour la cible
			CCalcItemParams Params;
			Params.SetTerrain(GetTerrain());
			Params.SetEchantillonID(1);
			Params.SetUniversID(GetTerrain()->GetDefaultUnivers());
			Params.SetTypePoidsID(GetTypePoids());
			Params.SetPopulationSegment(GetPopulationSegment());
			Params.SetSegmentsIDs(GetSegments());
			pItem->CalculateEffectif(&Params); 
			
			// Effectif Cible
			if(GetShowResults()&SHOW_EFFECTIVE)
				Text.Format("%s\t%0.f\n","Effectif : ",Params.GetEffectif());
			else
				Text = "\n";
			TxtMacro += Text;

			// Pourcentage cibleXML percent tag
			if(GetShowResults()&SHOW_PERCENT)
				Text.Format("%s\t%0.2f%%\n","Pourcentage : ",(Params.GetEffectif()/Params.GetEffectifTot())*100.0);
			else
				Text = "\n";
			TxtMacro += Text;
			
			// Nb Cas Cible
			if(GetShowResults()&SHOW_NBCAS)
				Text.Format("%s\t%d\n","Nombre de cas : ",Params.GetNbCas());
			else
				Text = "\n";
			TxtMacro += Text;
		}
	}

	// Saut de paragraphe
	TxtMacro += "\n";
}

// Export des composants de  l'item
void CDlgItems::ExportItem(CString &TxtMacro)
{
	// Récupération des élmts constituant l'item sous forme de texte
	TxtMacro += GetCurMode()->GetVisualisation();  

	// Puis on remet la visualisation normale
	GetCurMode()->DoVisualisation();  
}

// Export des composants de  la palette
void CDlgItems::ExportPalette(CString &TxtMacro)
{
	// Boucle sur les élmts de la liste palette
	for (int i=0; i<m_List_Compo_Palette.GetItemCount(); i++) 
	{
		CString TxtLine;
		TxtLine.Format("%s\n",m_List_Compo_Palette.GetItemText(i,0));
		TxtMacro += TxtLine;  
	}
}

void CDlgItems::SetData(JList<CVisuItems> &Items)
{
	m_Items.Swap(Items);
	m_List_Result.SetItemCount(m_Items.GetCount()); 
}

/////////////////////////////////// EXPORT DANS UN FICHIER ITEM CONSTRUIT ////////////////////////////////////
//
// Export Construction Item Echelle dans un fichier
void CDlgItems::OnExportItem(JList<JInt32> &Values, bool Impression)
{

#if !defined(BUILD_FOR_ATELIER_INTERNET)

	// Texte final descriptif item
	CString DescriptifItem = "";

	// Export de l'entete info
	ExportEntete(Values,DescriptifItem);

	// Export des composants items
	ExportItem(DescriptifItem);

	// A VOIR si on le laisse !!!
	// Puis on inscrit les informations dans un fichier texte
	CStdioFile ExportFile;
	CFileException Error = 0;
	CString path = GETPROFILESTRING(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);
	path += "\\TempTarget.txt";
	if(!ExportFile.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeText, &Error))
		return;
	ExportFile.WriteString(DescriptifItem);
	ExportFile.Close();

	// Ecriture dans clipboard pour prévisu et impression
	if (Clipboard(DescriptifItem) && Impression == true)
	{
		CExcelPrinter excelPrinter(CATPColors::COLORREPITEMS, this);
		CString path = GETPROFILESTRING(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);

		// Compléter le MacroPresse pour affichage item
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
	}

#endif

}

/////////////////////////////////// EXPORT DANS UN FICHIER ITEM EN CONSTRUCTION ////////////////////////////////////
//
// Export Item en cours de construction !!!!!
void CDlgItems::OnExportItemEnConstruction(JList<JInt32> &Values, bool Impression)
{
	// Il faut au moins 2 élmt dans la liste résultat (sinon rien à imprimer)
	int NbLignes = m_List_Result.GetItemCount() - 2;
	if (NbLignes > 0)
	{
		// Attention au flag Calcul Segment ou Calcul Global
		m_bSegment = m_Button_CalcSegment.GetCheck();
		if (m_bSegment == 1)
		{
			// Arrêt du calcul demandé 
			CString TxtMess;
			LOADSTRING(TxtMess, IDS_RI_MESS_2); 
			if (AfxMessageBox(TxtMess,MB_YESNO|MB_ICONQUESTION) == IDNO)
				return;
			/*
			if (AfxMessageBox("Actuellement vous êtes en mode Calcul Segment (effectif, nb cas).\nPour imprimer l'item en cours de construction, le programme reviendra automatiquement en mode Effectif Global, désirez-vous quand même continuer l'impression ?",MB_YESNO|MB_ICONQUESTION) == IDNO)
				return;
			*/

			// On repasse en mode calcul global
			m_Button_CalcSegment.SetCheck(BST_UNCHECKED);

			// On recalcule l'effectif en brut et en %, ainsi que le nombre de cas
			OnChange_SegmentCalc();
		}

		// Texte final descriptif item
		CString DescriptifItem = "";

		// Export de l'entete info
		ExportEntete(Values,DescriptifItem,true);

		// Export des composants items
		ExportItem(DescriptifItem);

		// A VOIR si on le laisse !!!
		// Puis on inscrit les informations dans un fichier texte
		CStdioFile ExportFile;
		CFileException Error = 0;
		CString path = GETPROFILESTRINGD(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);
		path += "\\TempTarget.txt";
		if(!ExportFile.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeText, &Error))
			return;
		ExportFile.WriteString(DescriptifItem);
		ExportFile.Close();

		// Ecriture dans clipboard pour prévisu et impression
		if (Clipboard(DescriptifItem))
		{
#if !defined(BUILD_FOR_ATELIER_INTERNET)
			// @@@ REPITEMS : ATELIER INTERNET
			if (Impression)
			{
				CExcelPrinter excelPrinter(CATPColors::COLORREPITEMS, this);
				CString path = GETPROFILESTRING(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);

				// Compléter le MacroPresse pour affichage item
				path += "\\MacroPresse.xla";
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			}
#endif
		}
	}
}

/////////////////////////////////// EXPORT DANS UN FICHIER LA PALETTE EN COURS ////////////////////////////////////
//
// Export Construction Palette en cours fichier
void CDlgItems::OnExportPalette()
{
	// Texte final descriptif item
	CString DescriptifItem = "";

	// Export de l'entete info
	// ExportEntete(Values,DescriptifItem);

	// Export des composants de la palette
	ExportPalette(DescriptifItem);

	// Puis on inscrit les informations dans un fichier texte
	CStdioFile ExportFile;
	CFileException Error = 0;
	CString path = GETPROFILESTRINGD(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);
	path += "\\TempTarget.txt";
	if(!ExportFile.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeText, &Error))
		return;
	ExportFile.WriteString(DescriptifItem);
	ExportFile.Close();

	// Ecriture dans clipboard pour prévisu et impression
	if (Clipboard(DescriptifItem))
	{
#if !defined(BUILD_FOR_ATELIER_INTERNET)
		// @@@ REPITEMS : ATELIER INTERNET
		CExcelPrinter excelPrinter(CATPColors::COLORREPITEMS, this);
		CString path = GETPROFILESTRING(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);

		// Compléter le MacroPresse pour affichage item
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
#endif
	}
}

// Passe le descriptif item dans le clipboard
bool CDlgItems::Clipboard(CString DescriptifItem)
{
	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= DescriptifItem;
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// Test opération Clipboard
	VERIFY(OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	// Lance la copie clipboard
	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}
	return true;
}

LRESULT CDlgItems::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	// En attente >> Impression possible ou pas (à revoir car dans certains cas item en construction)
	if (ImpResultPossible())
	{
		m_But_Imprimer_Result.	EnableWindow(TRUE);
		m_But_Export_Item.		EnableWindow(TRUE);
		m_But_Supp_Item.		EnableWindow(TRUE);
	}
	else
	{
		m_But_Imprimer_Result.	EnableWindow(FALSE);
		m_But_Export_Item.		EnableWindow(FALSE);
		m_But_Supp_Item.		EnableWindow(FALSE);
	}

	if (m_Id_CurMode == Mode_Palettes)
	{
		/*
		m_But_Down_Item.		EnableWindow(TRUE);
		m_But_Up_Item.			EnableWindow(TRUE);
		*/
		m_But_Down_Item.		EnableWindow(FALSE);
		m_But_Up_Item.			EnableWindow(FALSE);
	}

	CString Fmt = _T("### ### ### ### ###");
    //wParam is a handler to the list
    //Make sure message comes from list box
    ASSERT( (HWND)wParam == m_List_Result.GetSafeHwnd() );

    //lParam is a pointer to the data that 
    //is needed for the element
	CQuickList::CListItemData * pdata = 
        (CQuickList::CListItemData *) lParam;

    //Get which item and subitem that is asked for.
    int item = pdata->GetItem();
    int subItem = pdata->GetSubItem();

	m_Items.MoveTo(item);
	if(m_Items.IsValid())
	{
		CVisuItems & Item = m_Items.GetItem();

		pdata->m_allowEdit = false;
		pdata->m_noSelection = true;

		BYTE red	= 0;
		BYTE green	= 0;
		BYTE blue	= 0;

		switch(Item.m_Level)
		{
			case 0:
			{
				COLORREF clr = CATPColors::GetColorWhite();
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 1:
			{
				COLORREF clr = CATPColors::GetColorLight(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 2:
			{
				pdata->m_textStyle.m_bold = true;
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorSelect(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 3:
			{
				pdata->m_textStyle.m_bold = true;
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorDark(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
            break;

			case 4:
			{
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorMedium(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			default:
			{
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorMedium(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
				red		+= (Item.m_Level-4)*15 ;
				green	+= (Item.m_Level-4)*15; 
				blue	+= (Item.m_Level-4)*15; 
			}
			break;
		}

		switch(subItem)
		{
			case 1:
			{
				if(Item.m_NbCas >= 0)
				{
					if (m_ShowResults & SHOW_NBCAS)
						pdata->m_text.Format("%d",Item.m_NbCas);
					else
						pdata->m_text.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_text);  
					pdata->m_text+= _T("  ");
					if (m_ShowResults & SHOW_NBCAS)
						pdata->m_tooltip.Format("%d",Item.m_NbCas);
					else
						pdata->m_tooltip.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_tooltip);  
				}
			}
			break;

			case 2:
			{
				if(Item.m_NbCas >= 0)
				{

					if (m_ShowResults & SHOW_EFFECTIVE)
						pdata->m_text.Format("%0.0f",Item.m_Effective/1000.0);
					else
						pdata->m_text.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_text);  
					pdata->m_text+= _T("  ");
					if (m_ShowResults & SHOW_EFFECTIVE)
						pdata->m_tooltip.Format("%0.0f",Item.m_Effective);
					else
						pdata->m_tooltip.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_tooltip);  
					pdata->m_progressBar.m_edge		= 0;
					pdata->m_progressBar.m_maxvalue	= 100;
					pdata->m_progressBar.m_value	= (int)((Item.m_Effective/Item.m_EffectiveTotal)*100.0);
					pdata->m_progressBar.m_fillTextColor	= CATPColors::GetColorBlack();
					pdata->m_progressBar.m_fillColor		= RGB(255,255,0);//CATPColors::GetColorSelect(CATPColors::COLORREPITEMS);//CATPColors::GetColorRed();
				}
			}
			break;

			default:
			{
				pdata->m_text = Item.m_Label;
				pdata->m_tooltip = Item.m_Label;
			}
			break;
		}
		pdata->m_colors.m_backColor = RGB(red,green,blue);
	}
	else
	{
		pdata->m_allowEdit = false;
		pdata->m_noSelection = true;
		pdata->m_text = _T("Empty");
	}
    return 0;
}

// Effacer contenu d'un segment
void CDlgItems::OnRiEffacerSegment()
{
	// A FAIRE Suppression sélection segment en cours
	OnSegment_Raz();
}

// Effacer contenu d'un item
void CDlgItems::OnRiEffacerItem()
{
	OnItem_Raz();
}

// Effacer contenu d'une palette
void CDlgItems::OnRiEffacerPalette()
{
	OnItem_Raz();
}

// Effacer tout
void CDlgItems::OnRiEffacerTout()
{
	// efface sélection en cours
	OnTout_Raz();

	// efface sélection courant index thématique
	m_Tree_Theme.ResetMapTree();

	// Reset tree index thematique
	SetThemeData();
}

// Effacer tout
void CDlgItems::OnRiEffacerToutPalette()
{
	// efface sélection en cours
	OnTout_Raz();

	// efface sélection courant index thématique
	m_Tree_Theme.ResetMapTree();

	// Reset tree index thematique
	SetThemeData();
}

// Gestion menu contextuel opérateurs logiques pour dimension 1
//
void CDlgItems::OnRiOpeDim1Ou()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_OU);  
	m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 

	// On se met en mode opérateur OU
	if(m_pCurMode)
		m_pCurMode->OnDim_1_Ou();
}

void CDlgItems::OnRiOpeDim1Et()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_ET);  
	m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 

	// On se met en mode opérateur ET
	if(m_pCurMode)
		m_pCurMode->OnDim_1_Et();
}

void CDlgItems::OnRiOpeDim1Ex()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_XOR);  
	m_Button_Menu_Ope_Dim1.SetWindowText(Txt);

	// On se met en mode opérateur XOR
	if(m_pCurMode)
		m_pCurMode->OnDim_1_Xor();
}

void CDlgItems::OnRiOpeDim1Ni()
{
	// Modifie texte opépateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_NI);  
	m_Button_Menu_Ope_Dim1.SetWindowText(Txt); 

	// On se met en mode opérateur NI
	if(m_pCurMode)
		m_pCurMode->OnDim_1_Ni();
}

// Gestion menu contextuel opérateurs logiques pour dimension 2
//
void CDlgItems::OnRiOpeDim2Ou()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_OU);  
	m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 

	// On se met en mode opérateur OU
	if(m_pCurMode)
		m_pCurMode->OnDim_2_Ou();
}

void CDlgItems::OnRiOpeDim2Et()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_ET);  
	m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 

	// On se met en mode opérateur ET
	if(m_pCurMode)
		m_pCurMode->OnDim_2_Et();
}

void CDlgItems::OnRiOpeDim2Xor()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_XOR);  
	m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 

	// On se met en mode opérateur XOR
	if(m_pCurMode)
		m_pCurMode->OnDim_2_Xor();
}

void CDlgItems::OnRiOpeDim2Ni()
{
	// Modifie texte opérateur courant
	CString Txt;
	LOADSTRING(Txt, IDS_RI_NI);  
	m_Button_Menu_Ope_Dim2.SetWindowText(Txt); 

	// On se met en mode opérateur NI
	if(m_pCurMode)
		m_pCurMode->OnDim_2_Ni();
}

void CDlgItems::OnBnClickedCancel()
{
	// efface les fenêtres flottantes
	m_pCurMode->EffaceFenDimDlg(true);

	// Et on se barre
	OnCancel();
}

// Pseudo menu déroulant sas questions sélectionnées avec les icones opérateurs (textes ou icônes)
void CDlgItems::OnBnClickedRiButMenuOpeQusel()
{
	// Rend visible pour la sélection des opérateurs
	SetVisibleOpeSelQu(TRUE);

	// Rend inactif l'arrière plan dimension 1
	m_List_SAS.EnableWindow(FALSE); 
}

// Pseudo menu déroulant segement avec les icones opérateurs (textes ou icônes)
void CDlgItems::OnBnClickedRiButMenuOpeSegment()
{
	// Rend visible pour la sélection des opérateurs
	SetVisibleOpeSegment(TRUE);
}

// Pseudo menu déroulant dimension 1 avec les icones opérateurs (textes ou icônes)
void CDlgItems::OnBnClickedRiButMenuOpeDim1()
{
	// Actif uniquement dans les autres modes que palette
	if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
	{
		// Rend visible pour la sélection des opérateurs
		SetVisibleOpeDim(1,TRUE);

		// Rend inactif l'arrière plan dimension 1
		m_List_Dimension1.EnableWindow(FALSE); 
	}
}

// Rend visible ou pas selon la question la boite dimension 2
void CDlgItems::SetDimension2Visible(bool Visible)
{
	// Largeur séparateur entre box interface item
	JUnt32 LargSepar = 6;

	// Ratio Fixe Fen Dim1 via Total (Fen Dim1 + Fen Dim2) 
	JFlt64 RatioFen1 = 0.5;
	
	if (Visible == false)
	{
		// Controles Box dimension 2 devient invisible
		SetCtrlDimension2(false);

		// Pos et dim  dimension 1
		CRect RectBoxDim1;
		m_Box_Dimension1.GetWindowRect(&RectBoxDim1); 

		// Pos et dim  Questions
		CRect RectBoxQuest;
		m_Box_Question.GetWindowRect(&RectBoxQuest); 
		RectBoxDim1.right = RectBoxQuest.right; 

		// Retaille box dimension 1
		m_Box_Dimension1.SetWindowPos(NULL,RectBoxDim1.left,RectBoxDim1.top,RectBoxDim1.Width(),RectBoxDim1.Height() ,SWP_NOZORDER | SWP_NOMOVE);

		// Retaille list
		CRect RectList;
		m_List_Dimension1.GetWindowRect(&RectList);
		m_List_Dimension1.SetWindowPos(NULL,RectList.left,RectList.top,RectBoxQuest.Width()- 13,RectList.Height() ,SWP_NOZORDER | SWP_NOMOVE);

		// Retaille header label et les 2 boutons cachés derrière
		CRect RectLabel;
		CRect RectBtnRAZ;
		CRect RectMenu;

		m_Button_Menu_Ope_Dim1.GetWindowRect(&RectMenu);

		CRect RectBtnDown;
		CRect RectBtnFiltre;
		m_Button_Filtre.GetWindowRect(&RectBtnFiltre);
		// m_Button_Dim1_DOWN.GetWindowRect(&RectBtnDown);
		m_Header_Dimension1.GetWindowRect(&RectLabel);

		// Tous les modes sauf mode palette
		if (GetTypeRepItem())
		{
			// Bouton opérateur présent
			RectLabel.left  = RectMenu.left + RectMenu.Width() + LargSepar/2;
			m_Button_Menu_Ope_Dim1.ShowWindow(SW_SHOW);
		}
		else
		{
			// Bouton opérateur absent
			RectLabel.left  = RectMenu.left;
			m_Button_Menu_Ope_Dim1.ShowWindow(SW_HIDE);
		}
		// RectLabel.right = RectBtnDown.left - LargSepar/2; 
		RectLabel.right = RectBtnFiltre.left - LargSepar/2; 
		this->ScreenToClient(&RectLabel);
		
		// Placement des boutons dimension 1
		m_Header_Dimension1.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dim1_OK.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dimension1.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);

		// Repositionne bouton raz
		m_Button_Dim1_RAZ.GetWindowRect(&RectBtnRAZ);
		JUnt32 LargBtn = RectBtnRAZ.Width();
		RectBtnRAZ.left = RectBoxQuest.left + RectBoxQuest.Width() - LargBtn - LargSepar;
		RectBtnRAZ.right = RectBtnRAZ.left + LargBtn;
		this->ScreenToClient(&RectBtnRAZ);
		m_Button_Dim1_RAZ.SetWindowPos(NULL,RectBtnRAZ.left,RectBtnRAZ.top,RectBtnRAZ.Width(),RectBtnRAZ.Height() ,SWP_NOZORDER | SWP_NOSIZE);
		

		// Repositionne bouton up
		CRect RectBtn;
		m_Button_Dim1_UP.GetWindowRect(&RectBtn);
		LargBtn = RectBtn.Width();
		RectBtn.left = RectBoxQuest.left + RectBoxQuest.Width() - 2 * LargBtn - (3 * LargSepar)/2;
		RectBtn.right = RectBtn.left + LargBtn;
		this->ScreenToClient(&RectBtn);
		m_Button_Dim1_UP.SetWindowPos(NULL,RectBtn.left,RectBtn.top,RectBtn.Width(),RectBtn.Height() ,SWP_NOZORDER | SWP_NOSIZE);
		
		// Repositionne bouton down
		m_Button_Dim1_DOWN.GetWindowRect(&RectBtn);
		LargBtn = RectBtn.Width();
		RectBtn.left = RectBoxQuest.left + RectBoxQuest.Width() - 3 * LargBtn - 2*LargSepar;
		RectBtn.right = RectBtn.left + LargBtn;
		this->ScreenToClient(&RectBtn);
		m_Button_Dim1_DOWN.SetWindowPos(NULL,RectBtn.left,RectBtn.top,RectBtn.Width(),RectBtn.Height() ,SWP_NOZORDER | SWP_NOSIZE);

		// TODO Bouton Filtre
		m_Button_Filtre.GetWindowRect(&RectBtn);
		LargBtn = RectBtn.Width();
		RectBtn.left = RectBoxQuest.left + RectBoxQuest.Width() - 4 * LargBtn - (5 * LargSepar)/2;
		RectBtn.right = RectBtn.left + LargBtn;
		this->ScreenToClient(&RectBtn);
		m_Button_Filtre.SetWindowPos(NULL,RectBtn.left,RectBtn.top,RectBtn.Width(),RectBtn.Height() ,SWP_NOZORDER | SWP_NOSIZE);

	}
	else
	{
		// Box dimension 2 redevient visible
		SetCtrlDimension2(true);

		///////////////////////////////// Mise en place boite dimension 1 ///////////////////////////////
		// Pos et dim box dimension 1
		CRect RectBoxDim1;
		m_Box_Dimension1.GetWindowRect(&RectBoxDim1);

		// Pos et dim  Questions
		CRect RectBoxQuest;
		m_Box_Question.GetWindowRect(&RectBoxQuest); 

		// Nouvelle dimension boite dimension 1
		RectBoxDim1.right = RectBoxDim1.left + RectBoxQuest.Width() * RatioFen1;
		
		// En coordonnées écran
		this->ScreenToClient(&RectBoxDim1);
		m_Box_Dimension1.SetWindowPos(NULL,RectBoxDim1.left, RectBoxDim1.top, RectBoxDim1.Width(), RectBoxDim1.Height() ,SWP_NOZORDER  | SWP_NOMOVE);

		///////////////////////////////// Mise en place de la liste dimension 1 ///////////////////////////////
		m_Box_Dimension1.GetWindowRect(&RectBoxDim1);
		CRect RectListBox1;
		m_List_Dimension1.GetWindowRect(&RectListBox1);
		RectListBox1.right = RectBoxDim1.right - LargSepar;
		this->ScreenToClient(&RectListBox1);
		m_List_Dimension1.SetWindowPos(NULL,RectListBox1.left, RectListBox1.top, RectListBox1.Width(), RectListBox1.Height() ,SWP_NOZORDER  | SWP_NOMOVE);

		///////////////////////////////// Mise en place du bouton tous ou RAZ dimension 1 //////////////
		CRect RectBtnRAZ;
		m_Button_Dim1_RAZ.GetWindowRect(&RectBtnRAZ);
		m_List_Dimension1.GetWindowRect(&RectListBox1);
		int LargBtn = RectBtnRAZ.Width();
		RectBtnRAZ.right = RectListBox1.right;
		RectBtnRAZ.left  = RectBtnRAZ.right - LargBtn; 
		this->ScreenToClient(&RectBtnRAZ);
		m_Button_Dim1_RAZ.SetWindowPos(NULL,RectBtnRAZ.left, RectBtnRAZ.top, RectBtnRAZ.Width(), RectBtnRAZ.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place du bouton filtrage modalité //////////////
		CRect RectBtnFiltre;
		m_Button_Filtre.GetWindowRect(&RectBtnFiltre);
		int LargBtnFiltre = RectBtnFiltre.Width();
		RectBtnFiltre.right = RectListBox1.right - LargBtn - LargSepar/2;
		RectBtnFiltre.left  = RectBtnFiltre.right - LargBtnFiltre; 
		this->ScreenToClient(&RectBtnFiltre);
		m_Button_Filtre.SetWindowPos(NULL,RectBtnFiltre.left, RectBtnFiltre.top, RectBtnFiltre.Width(), RectBtnFiltre.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place des entetes dimension 1 ///////////////////////////////
		CRect RectLabel;
		m_Header_Dimension1.GetWindowRect(&RectLabel);
		m_Button_Filtre.GetWindowRect(&RectBtnFiltre);
		CRect RectMenu;
		m_Button_Menu_Ope_Dim1.GetWindowRect(&RectMenu);

		if (GetTypeRepItem())
		{
			// Bouton opérateur présent
			RectLabel.left  = RectMenu.left + RectMenu.Width() + LargSepar/2;
			m_Button_Menu_Ope_Dim1.ShowWindow(SW_SHOW);
		}
		else
		{
			// Bouton opérateur absent
			RectLabel.left  = RectMenu.left;
			m_Button_Menu_Ope_Dim1.ShowWindow(SW_HIDE);
		}
		RectLabel.right = RectBtnFiltre.left - LargSepar/2;  
		this->ScreenToClient(&RectLabel);

		// Placement des boutons dimension 1
		m_Header_Dimension1.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dim1_OK.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dimension1.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place boite dimension 2 ///////////////////////////////
		// Pos et dim box dimension 1 
		CRect RectBoxDim2;
		m_Box_Dimension1.GetWindowRect(&RectBoxDim2);

		// Pos et dim  Questions
		m_Box_Question.GetWindowRect(&RectBoxQuest); 

		// Nouvelle dimension boite dimension 1
		RectBoxDim2.left  = RectBoxDim1.left + RectBoxDim1.Width() + LargSepar/2;
		RectBoxDim2.right = RectBoxQuest.right; 
		
		// En coordonnées écran
		this->ScreenToClient(&RectBoxDim2);
		m_Box_Dimension2.SetWindowPos(NULL,RectBoxDim2.left, RectBoxDim2.top, RectBoxDim2.Width(), RectBoxDim2.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place de la liste dimension 2 ///////////////////////////////
		m_Box_Dimension2.GetWindowRect(&RectBoxDim2);
		CRect RectListBox2;
		m_List_Dimension2.GetWindowRect(&RectListBox2);
		RectListBox2.right = RectBoxDim2.right - LargSepar;
		RectListBox2.left  = RectBoxDim2.left + LargSepar;
		this->ScreenToClient(&RectListBox2);
		m_List_Dimension2.SetWindowPos(NULL,RectListBox2.left, RectListBox2.top, RectListBox2.Width(), RectListBox2.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place menu dimension 2 ///////////////////////////////
		m_Button_Menu_Ope_Dim2.GetWindowRect(&RectMenu);
		this->ScreenToClient(&RectMenu);
		m_Button_Menu_Ope_Dim2.SetWindowPos(NULL,RectListBox2.left, RectMenu.top, RectMenu.Width(), RectMenu.Height() ,SWP_NOZORDER|SWP_NOSIZE);

		m_Button_Dim2_OU.GetWindowRect(&RectMenu);
		this->ScreenToClient(&RectMenu);
		m_Button_Dim2_OU.SetWindowPos(NULL,RectListBox2.left, RectMenu.top, RectMenu.Width(), RectMenu.Height() ,SWP_NOZORDER|SWP_NOSIZE);

		m_Button_Dim2_ET.GetWindowRect(&RectMenu);
		this->ScreenToClient(&RectMenu);
		m_Button_Dim2_ET.SetWindowPos(NULL,RectListBox2.left, RectMenu.top, RectMenu.Width(), RectMenu.Height() ,SWP_NOZORDER|SWP_NOSIZE);

		m_Button_Dim2_NI.GetWindowRect(&RectMenu);
		this->ScreenToClient(&RectMenu);
		m_Button_Dim2_NI.SetWindowPos(NULL,RectListBox2.left, RectMenu.top, RectMenu.Width(), RectMenu.Height() ,SWP_NOZORDER|SWP_NOSIZE);

		m_Button_Dim2_XOR.GetWindowRect(&RectMenu);
		this->ScreenToClient(&RectMenu);
		m_Button_Dim2_XOR.SetWindowPos(NULL,RectListBox2.left, RectMenu.top, RectMenu.Width(), RectMenu.Height() ,SWP_NOZORDER|SWP_NOSIZE);

		///////////////////////////////// Mise en place du bouton tous ou RAZ dimension 2 //////////////
		m_Button_Dim2_RAZ.GetWindowRect(&RectBtnRAZ);
		m_List_Dimension2.GetWindowRect(&RectListBox2);
		LargBtn = RectBtnRAZ.Width();
		RectBtnRAZ.right = RectListBox2.right;
		RectBtnRAZ.left  = RectBtnRAZ.right - LargBtn; 
		this->ScreenToClient(&RectBtnRAZ);
		m_Button_Dim2_RAZ.SetWindowPos(NULL,RectBtnRAZ.left, RectBtnRAZ.top, RectBtnRAZ.Width(), RectBtnRAZ.Height() ,SWP_NOZORDER);

		///////////////////////////////// Mise en place des entêtes dimension 2 ////////////////////////
		m_Header_Dimension2.GetWindowRect(&RectLabel);
		m_Button_Dim2_RAZ.GetWindowRect(&RectBtnRAZ);
		m_Button_Menu_Ope_Dim2.GetWindowRect(&RectMenu);
		if (GetTypeRepItem())
		{
			// Bouton opérateur présent
			RectLabel.left  = RectMenu.left + RectMenu.Width() + LargSepar/2;
			m_Button_Menu_Ope_Dim2.ShowWindow(SW_SHOW);
		}
		else
		{
			// Bouton opérateur absent
			RectLabel.left  = RectMenu.left;
			m_Button_Menu_Ope_Dim2.ShowWindow(SW_HIDE);
		}
		RectLabel.right = RectBtnRAZ.left - LargSepar/2;  
		this->ScreenToClient(&RectLabel);
		m_Header_Dimension2.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dimension2.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);

		if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
		{
			m_Header_Dimension2.ShowWindow(SW_SHOW);
			m_Button_Dimension2.ShowWindow(SW_HIDE);
		}
		else
		{
			m_Header_Dimension2.ShowWindow(SW_HIDE);
			m_Button_Dimension2.ShowWindow(SW_SHOW);
		}
	}

	// Layout entre controls
	SetWindowRelations();
}

// Pseudo menu déroulant dimension 2 avec les icones opérateurs (textes ou icônes)
void CDlgItems::OnBnClickedRiButMenuOpeDim2()
{
	// Actif uniquement dans les autres modes que palette
	if (m_Id_CurMode != Mode_Palettes && GetTypeRepItem())
	{
		// Rend visible pour la sélection des opérateurs
		SetVisibleOpeDim(2,TRUE);

		// Rend inactif l'arrière plan dimension 2
		m_List_Dimension2.EnableWindow(FALSE); 
	}
}

// Test si au moins 1 menu déroulant encore actif
bool CDlgItems::OpeActif()
{
	// Y a t-il un opérateur sur dimension 1 actif
	if (m_Button_Dim1_ET.IsWindowVisible()  || 
		m_Button_Dim1_OU.IsWindowVisible()  ||
		m_Button_Dim1_XOR.IsWindowVisible() ||
		m_Button_Dim1_NI.IsWindowVisible())		return true;

	// Y a t-il un opérateur sur dimension 1 actif
	if (m_Button_Dim2_ET.IsWindowVisible()  || 
		m_Button_Dim2_OU.IsWindowVisible()  ||
		m_Button_Dim2_XOR.IsWindowVisible() ||
		m_Button_Dim2_NI.IsWindowVisible())		return true;

	// Y a t-il un opérateur sur le sas questions actif
	if (m_Button_SAS_ET.IsWindowVisible()   || 
		m_Button_SAS_OU.IsWindowVisible())		return true;

	// Y a t-il un opérateur sur le segment actif
	if (m_Button_Segment_ET.IsWindowVisible()   ||
		m_Button_Segment_OU.IsWindowVisible())  return true;

	return false;
}

// Rend visible pour la sélection des opérateurs
void CDlgItems::SetVisibleOpeDim(JUnt32 NoDim, bool Visible)
{
	if (NoDim == 1)
	{
		// Concernant les opérateurs sur la dimension 1
		m_Button_Dim1_ET.ShowWindow(Visible);
		if (Visible==false) m_Button_Dim1_ET.ActivateTopParent(); 
		m_Button_Dim1_OU.ShowWindow(Visible);
		if (Visible==false) m_Button_Dim1_OU.ActivateTopParent(); 
		m_Button_Dim1_XOR.ShowWindow(Visible);
		if (Visible==false) m_Button_Dim1_XOR.ActivateTopParent(); 
		m_Button_Dim1_NI.ShowWindow(Visible);
		if (Visible==false) m_Button_Dim1_NI.ActivateTopParent(); 
	}
	else if (NoDim == 2)
	{
		// Concernant les opérateurs sur la dimension 2
		m_Button_Dim2_ET.ShowWindow(Visible);
		m_Button_Dim2_OU.ShowWindow(Visible);
		m_Button_Dim2_NI.ShowWindow(Visible);
		m_Button_Dim2_XOR.ShowWindow(Visible);
	}
}

// Rend visible pour la sélection des opérateurs sas questions
void CDlgItems::SetVisibleOpeSelQu(bool Visible)
{
	// Concernant les opérateurs sur le sas questions
	m_Button_SAS_ET.ShowWindow(Visible);
	m_Button_SAS_OU.ShowWindow(Visible);
}

// Rend visible pour la sélection des opérateurs segments
void CDlgItems::SetVisibleOpeSegment(bool Visible)
{
	// Concernant les opérateurs sur le segment
	m_Button_Segment_ET.ShowWindow(Visible);
	m_Button_Segment_OU.ShowWindow(Visible);

	m_Button_Segment_ET.BringWindowToTop();
	m_Button_Segment_OU.BringWindowToTop();
}

/////////////////////////////////////////////////////////////
// Gestion déplacement souris dur la fenêtre de dialogue
void CDlgItems::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	// Test tsi au moins 1 opérateur actif
	if (OpeActif())
	{
		// Rend invisible tous les menus opérateurs
		SetVisibleOpeDim(1,false);
		SetVisibleOpeDim(2,false);
		SetVisibleOpeSelQu(false);
		SetVisibleOpeSegment(false);

		// La liste Sas redevient accessible
		m_List_SAS.EnableWindow(TRUE); 

		// La lise Dimension 1 et dimension 2 aussi
		m_List_Dimension1.EnableWindow(TRUE); 
		m_List_Dimension2.EnableWindow(TRUE);

		// Les élmts segments aussi
		m_Tab_Segment.EnableWindow(TRUE); 
	}
}

// Rend visible pour la sélection des opérateurs
void CDlgItems::SetCtrlDimension2(bool Visible)
{
	// Si non visible, on redimension la fenêtre dimension 1
	if (Visible == false)
	{
		// Composants boite dimension 2 non visibles
		m_Box_Dimension2.		ShowWindow(SW_HIDE);
		m_List_Dimension2.		ShowWindow(SW_HIDE);

		m_Header_Dimension2.	ShowWindow(SW_HIDE);
		m_Button_Dimension2.	ShowWindow(SW_HIDE);

		m_Button_Dim2_RAZ.		ShowWindow(SW_HIDE);
		m_Button_Menu_Ope_Dim2.	ShowWindow(SW_HIDE);
		m_Button_Dim2_OU.		ShowWindow(SW_HIDE);
		m_Button_Dim2_ET.		ShowWindow(SW_HIDE);
		m_Button_Dim2_NI.		ShowWindow(SW_HIDE);
		m_Button_Dim2_XOR.		ShowWindow(SW_HIDE);

		// Remet les boutons tris visible (apparaissent uniquement si 1 seul dimension)
		m_Button_Filtre.		ShowWindow(SW_SHOW);
		m_Button_Dim1_UP.		ShowWindow(SW_SHOW);
		m_Button_Dim1_DOWN.		ShowWindow(SW_SHOW);
	}
	else
	{
		// Composants boite dimension 2 visibles
		m_Box_Dimension2.		ShowWindow(SW_SHOW);
		m_List_Dimension2.		ShowWindow(SW_SHOW);

		if (m_Id_CurMode != 1)
		{
			// En mode autre que items
			m_Button_Dimension2.	ShowWindow(SW_SHOW);
            m_Header_Dimension2.	ShowWindow(SW_HIDE);
		}
		else
		{
			// en mode item
			m_Button_Dimension2.	ShowWindow(SW_HIDE);
            m_Header_Dimension2.	ShowWindow(SW_SHOW);
		}

		m_Button_Dim2_RAZ.		ShowWindow(SW_SHOW);
		m_Button_Menu_Ope_Dim2.	ShowWindow(SW_SHOW);

		// Remet les boutons tris visible (apparaissent uniquement si 1 seul dimension)
		// m_Button_Filtre.		ShowWindow(SW_HIDE);
		m_Button_Dim1_UP.		ShowWindow(SW_HIDE);
		m_Button_Dim1_DOWN.		ShowWindow(SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Rend visible ou non les opérateurs sur dimension 1
void CDlgItems::SetOpeDimension1(bool Visible, bool Actif)
{
	if (Visible)
	{
		// Activation en mode autre que palette
		m_Button_Menu_Ope_Dim1.ShowWindow(SW_SHOW); 

		// puis bouton sélection opération sur dimension1 en cours
		if(m_FIcones)
		{
			// puis bouton sélection opération sur dimension1 en cours
			if (m_Button_Dim1_ET.GetCheck())
				m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			else if (m_Button_Dim1_OU.GetCheck())
				m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			else if (m_Button_Dim1_NI.GetCheck())
				m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
			else if (m_Button_Dim1_XOR.GetCheck())
				m_Button_Menu_Ope_Dim1.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));

			m_Button_Menu_Ope_Dim1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim1.SetWindowText("");
		}
		else
		{
			CString Txt;
			if (m_Button_Dim1_ET.GetCheck())
				m_Button_Dim1_ET.GetWindowText(Txt);
			else if (m_Button_Dim1_OU.GetCheck())
				m_Button_Dim1_OU.GetWindowText(Txt);
			else if (m_Button_Dim1_NI.GetCheck())
				m_Button_Dim1_NI.GetWindowText(Txt);
			else if (m_Button_Dim1_XOR.GetCheck())
				m_Button_Dim1_XOR.GetWindowText(Txt);
			
			m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
			m_Button_Menu_Ope_Dim1.SetWindowText(Txt);
		}

		// Remet les back color d'origine
		m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
		m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	}
	else
	{			
		// Désactivation en mode palette
		// m_Button_Menu_Ope_Dim1.ShowWindow(SW_HIDE); 

		// Efface le texte
		m_Button_Menu_Ope_Dim1.SetBitmaps(0,0);
		m_Button_Menu_Ope_Dim1.SetWindowText(""); 

		// Remettre Fond Back Color inactif
		m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp,0);
		m_Button_Menu_Ope_Dim1.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp,0);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Rend visible ou non les opérateurs sur dimension 1
void CDlgItems::SetOpeDimension2(bool Visible, bool Actif)
{
	if (Visible && Actif)
	{
		// Activation en mode autre que palette
		m_Button_Menu_Ope_Dim2.ShowWindow(SW_SHOW); 

		// puis bouton sélection opération sur dimension1 en cours
		if(m_FIcones)
		{
			// puis bouton sélection opération sur dimension2 en cours
			if (m_Button_Dim2_ET.GetCheck())
				m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			else if (m_Button_Dim2_OU.GetCheck())
				m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			else if (m_Button_Dim2_NI.GetCheck())
				m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
			else if (m_Button_Dim2_XOR.GetCheck())
				m_Button_Menu_Ope_Dim2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));
			
			m_Button_Menu_Ope_Dim2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Dim2.SetWindowText("");
		}
		else
		{
			CString Txt;
			if (m_Button_Dim2_ET.GetCheck())
				m_Button_Dim2_ET.GetWindowText(Txt);
			else if (m_Button_Dim2_OU.GetCheck())
				m_Button_Dim2_OU.GetWindowText(Txt);
			else if (m_Button_Dim2_NI.GetCheck())
				m_Button_Dim2_NI.GetWindowText(Txt);
			else if (m_Button_Dim2_XOR.GetCheck())
				m_Button_Dim2_XOR.GetWindowText(Txt);
			
			m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
			m_Button_Menu_Ope_Dim2.SetWindowText(Txt);
		}

		// Remet les back color d'origine
		m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp,0);
		m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp), m_CATPColorsApp);
	}
	else
	{			
		// Activation en mode autre que palette
		if (Visible == false || !GetTypeRepItem())
			m_Button_Menu_Ope_Dim2.ShowWindow(SW_HIDE); 
		else
			m_Button_Menu_Ope_Dim2.ShowWindow(SW_SHOW); 

		// Efface le texte
		m_Button_Menu_Ope_Dim2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Dim2.SetWindowText(""); 

		// Remettre Fond Back Color inactif
		m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp,0);
		m_Button_Menu_Ope_Dim2.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp), m_CATPColorsApp,0);
	}	
}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode lignes pour les tris croisés
void CDlgItems::OnBnClickedRiRadModeLignes()
{
	// Active le bouton sélections ligne
	m_Button_Mode_Ligne.SetTypeOnglet(FALSE,TRUE,TRUE); 
	m_Button_Mode_Colonne.SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Page.SetTypeOnglet(FALSE,FALSE,TRUE); 

	// Valide le bouton
	CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,IDC_RI_RAD_MODE_LIGNES);
}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode colonnes pour les tris croisés
void CDlgItems::OnBnClickedRiRadModeColonnes()
{
	// Active le bouton sélections colonne
	m_Button_Mode_Ligne.SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Colonne.SetTypeOnglet(FALSE,TRUE,TRUE); 
	m_Button_Mode_Page.SetTypeOnglet(FALSE,FALSE,TRUE); 

	// Valide le bouton
	CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,IDC_RI_RAD_MODE_COLONNES);
}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode pages pour les tris croisés
void CDlgItems::OnBnClickedRiRadModePages()
{
	// Active le bouton sélections page
	m_Button_Mode_Ligne.SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Colonne.SetTypeOnglet(FALSE,FALSE,TRUE); 
	m_Button_Mode_Page.SetTypeOnglet(FALSE,TRUE,TRUE); 

	// Valide le bouton
	CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,IDC_RI_RAD_MODE_PAGES);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression lignes sélectionnées
void CDlgItems::OnRiMenuSuppLignes()
{
	// A FAIRE dans DlgTableau
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression colonnes sélectionnées
void CDlgItems::OnRiMenuSuppColonnes()
{
	// A FAIRE dans DlgTableau
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression lignes pages
void CDlgItems::OnRiMenuSuppPages()
{
	// A FAIRE dans DlgTableau
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression de toutes les sélections
void CDlgItems::OnRiMenuSuppTout()
{
	// A FAIRE dans DlgTableau
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lance le sélecteur d'items quantitatifs à moyenner en ligne
void CDlgItems::OnBnClickedRiItemMoyenLig()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lance le sélecteur d'items quantitatifs à moyenner en colonne
void CDlgItems::OnBnClickedRiItemMoyenCol()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection lignes
void CDlgItems::OnBnClickedRiButLignesDown()
{
	// rien
}

// Tri ascendant sélection lignes
void CDlgItems::OnBnClickedRiButLignesUp()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri ascendant sélection colonnes
void CDlgItems::OnBnClickedRiButColonnesDown()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection colonnes
void CDlgItems::OnBnClickedRiButColonnesUp()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri ascendant sélection lignes
void CDlgItems::OnBnClickedRiButPagesDown()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection pages
void CDlgItems::OnBnClickedRiButPagesUp()
{
	// rien
}

//////////////////////////////////////////////////////////////////////////////////////////
// Selon les modes on positionne ou pas les boutons sélections
void CDlgItems::SetBoutonSelection()
{
	// Pour dimension 1
	m_Button_Dimension1.SetCheck(m_AjoutSelCompoPalette_Dim1);
			
	// Pour dimension 2
	m_Button_Dimension2.SetCheck(m_AjoutSelCompoPalette_Dim2);

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Renvoi état sélection dimension 1 ou 2
bool CDlgItems::GetSelDimActive(JUnt32 Dim)
{
	/// En mode répertoire des items
	if (m_TypeRepItem)
	{	
		// Dimension 1
		if (Dim == 1)
			return m_AjoutSelCompoPalette_Dim1;
		// Dimension 2
		else if (Dim == 2)
			return m_AjoutSelCompoPalette_Dim2;
	}
	else
	{
		// En mode pupitre
		return GetSelDimPupitreActive(Dim);
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Renvoi état sélection dimension 1 ou 2
bool CDlgItems::GetSelDimPupitreActive(JUnt32 Dim)
{
	// rien ici
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde des modes des sélections pour dimension 1 et dimension 2
void CDlgItems::SaveModeSelectionDim1Dim2(JInt32 IdItem)
{
	// Pour le mode sélection dimension 1
	m_MapModeSelQuestionDim1.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim1.IsValid())
	{
		// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
		JBool &Actif = m_MapModeSelQuestionDim1.GetItem();
		Actif = m_AjoutSelCompoPalette_Dim1;
	}
	else
	{
		// Nouvel item, sauve son mode de sélection en dimension 1
		m_MapModeSelQuestionDim1.Add(IdItem) = m_AjoutSelCompoPalette_Dim1;  
	}

	// Pour le mode sélection dimension 2
	m_MapModeSelQuestionDim2.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim2.IsValid())
	{
		// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
		JBool &Actif = m_MapModeSelQuestionDim2.GetItem();
		Actif = m_AjoutSelCompoPalette_Dim2;
	}
	else
	{
		// Nouvel item, sauve son mode de sélection en dimension 1
		m_MapModeSelQuestionDim2.Add(IdItem) = m_AjoutSelCompoPalette_Dim2;  
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Récupére modes des sélections pour dimension 1 et dimension 2
void CDlgItems::LoadModeSelectionDim1Dim2(JInt32 IdItem, JUnt32 NbDimension)
{
	// Pour le mode sélection dimension 1
	m_MapModeSelQuestionDim1.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim1.IsValid())
	{
		// Récup à jour item déjà sélectionné pour le mode sélection dimension 1
		m_AjoutSelCompoPalette_Dim1 = m_MapModeSelQuestionDim1.GetItem();
	}
	else
	{
		// Par défaut toujours valide quelquesoit le nb dimensions question
		m_AjoutSelCompoPalette_Dim1 = true;
		// sauve son mode de sélection en dimension 2
		m_MapModeSelQuestionDim1.Add(IdItem) = m_AjoutSelCompoPalette_Dim1;  
	}

	// Pour le mode sélection dimension 2
	m_MapModeSelQuestionDim2.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim2.IsValid())
	{
		// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
		m_AjoutSelCompoPalette_Dim2 = m_MapModeSelQuestionDim2.GetItem();
	}
	else
	{
		// Par défaut toujours non sélectionné pour la  dimension 2
		m_AjoutSelCompoPalette_Dim2 = false;
		// sauve son mode de sélection en dimension 2
		m_MapModeSelQuestionDim2.Add(IdItem) = m_AjoutSelCompoPalette_Dim2;  
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Activation ou Désactivation du mode sélection courante pour compo palette
void CDlgItems::ActiveModeSel(JUnt32 CurModeSel, JUnt32 Dimension, bool Actif)
{
	if (Dimension == 1)
	{
		// Update du mode ajout ligne via dimension 1
		m_AjoutSelCompoPalette_Dim1	= Actif;
	}
	else if (Dimension == 2)
	{
		// Update du mode ajout ligne via dimension 2
		m_AjoutSelCompoPalette_Dim2	= Actif;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Renvoi si la question est en mode activation (pour palette essentiellement)
bool CDlgItems::QuestionSelActive(JInt32 IdItem)
{
	// Active sur dimension 1 ?
	m_MapModeSelQuestionDim1.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim1.IsValid())
	{
		// @@@ Aout 2013
		if (m_MapModeSelQuestionDim1.GetItem() == true || m_Id_CurMode == Mode_Palettes)
			return true;
	}

	// Active sur dimension 2 ?
	m_MapModeSelQuestionDim2.MoveTo(IdItem);
	if (m_MapModeSelQuestionDim2.IsValid())
	{
		if (m_MapModeSelQuestionDim2.GetItem() == true)
			return true;
	}
	// sinon pas active
	return false;
}

// Rien à faite sur cet objet
void CDlgItems::OnStnClickedRiLabelDimension2()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	int ValRep = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Validation du mode (surtout pour mode classe)
void CDlgItems::ValideMode()
{
	// En mode classe uniquement
	if (m_Id_CurMode == Mode_Classes)
		OnAccept();
	return;

	// .......................................... à voir
	// En mode classe uniquement
	if (m_Id_CurMode == Mode_Classes)
	{
        //OnAccept();
		UpdateData(true);
		if(!m_Edit_Name.GetWindowTextLength())
		{
			CString str;
			LOADSTRING(str, IDS_RI_ITEM_LABEL);
			MessageBox(str,_T(""),MB_OK | MB_ICONEXCLAMATION);
			m_Edit_Name.SetFocus();		
			return;
		}

		if(m_pCurMode)
		{
			CString Text;
			m_Edit_Name.GetWindowText(Text);
			if(m_pCurMode->OnAccept(Text))
			{
				UpdateData(false);
				GetItems()->Save();

				// Clearing all the current item && palettes sauf tree questions
				// m_pCurMode->OnTout_Raz_ItemPalette(); 

				m_Edit_Name.SetWindowText(""); 

				// Mode palette, virer la liste des composants
				if (m_Id_CurMode == Mode_Palettes)
					UpdateListSelCompoPalette();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Gestion KeyDown des lignes sélectionnées
void CDlgItems::OnLvnKeydownRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien (implémenter dans CDlgTableau)
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
}

void CDlgItems::OnLvnKeydownRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien (implémenter dans CDlgTableau)
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
}

void CDlgItems::OnLvnKeydownRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien (implémenter dans CDlgTableau)
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste lignes
void CDlgItems::OnLvnBegindragRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien à faire
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste colonnes
void CDlgItems::OnLvnBegindragRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien à faire
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste page
void CDlgItems::OnLvnBegindragRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien à faire
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Fin execution DragDrop
void CDlgItems::OnLButtonUp(UINT nFlags, CPoint point)
{
	// rien à faire
	CDialog::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////////
// Suppression des élmts sélectionnés en cours d'ajout dans le SAS
void CDlgItems::OnBnClickedRiButSasRaz()
{
	OnItem_Raz();
}


void CDlgItems::OnNMClickRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien ici
	*pResult = 0;
}

void CDlgItems::OnNMClickRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{	
	// rien ici	
	*pResult = 0;
}

void CDlgItems::OnNMClickRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien ici
	*pResult = 0;
}

void CDlgItems::OnSupprimertoutesleslignes()
{
	// rien ici
}

void CDlgItems::OnSupprimertouteslescolonnes()
{
	// rien ici
}

void CDlgItems::OnSupprimertouteslespages()
{
	// rien ici
}

void CDlgItems::OnLvnItemchangedRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien ici
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}


void CDlgItems::OnLvnItemchangedRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien ici
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CDlgItems::OnLvnItemchangedRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// rien ici
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}

void CDlgItems::OnBnClickedRiBtnSitesLig()
{
	// rien ici
}

void CDlgItems::OnBnClickedRiButAllLignes()
{
	// rien ici
}

void CDlgItems::OnBnClickedRiButAllColonnes()
{
	// rien ici
}

void CDlgItems::OnBnClickedRiButAllPages()
{
	// rien ici
}

void CDlgItems::OnRiMenuSuppAllLcp()
{
	// rien ici
}


void CDlgItems::OnBnClickedRiBtnSitesCol()
{
	// rien ici
}

void CDlgItems::OnBnClickedRiBtnSitesPage()
{
	// rien ici
}

//////////////////////////////////////////////////////////////////////////////////////
// Visualisation ou pas d'un item (via sélection dans rubriques items)
bool CDlgItems::ShowItem(JInt32 IdItem)
{
	return m_Tree_Rubrique_Items.ShowItem(IdItem);
}

//////////////////////////////////////////////////////////////////////////////////////
// Visualisation ou pas d'une palette (via sélection dans rubriques palettes)
bool CDlgItems::ShowPalette(JInt32 IdItem)
{
	return m_Tree_Rubrique_Palettes.ShowItem(IdItem);
}

/////////////////////////////////////////////////////////////////////////////////////
// Selon mode on viualise en entier ou pas la liste mode items (sinon tree rubrique apparent)
JVoid CDlgItems::ResizeListItems(bool RubriqueVisible)
{
	// Coordonneés liste items et tree rubriques
	CRect RectBoxListItem;
	CRect RectBoxTreeRubrique;
	m_List_Items.GetWindowRect(&RectBoxListItem);
	m_Tree_Rubrique_Items.GetWindowRect(&RectBoxTreeRubrique);
		
	if (RubriqueVisible)
	{
		// Rubriques non visible, on adopte la hauteur objet liste items maxi
		RectBoxListItem.top	= RectBoxTreeRubrique.bottom + BordureJfc;
		this->ScreenToClient(&RectBoxListItem);
		m_List_Items.SetWindowPos(NULL, RectBoxListItem.left, RectBoxListItem.top,
										RectBoxListItem.Width(),RectBoxListItem.Height() ,SWP_NOZORDER);
	}
	else
	{
		// Rubriques visible, on diminue la hauteur objet liste items
		RectBoxListItem.top	= RectBoxTreeRubrique.top;
		this->ScreenToClient(&RectBoxListItem);
		m_List_Items.SetWindowPos(NULL, RectBoxListItem.left, RectBoxListItem.top,
										RectBoxListItem.Width(),RectBoxListItem.Height() ,SWP_NOZORDER);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Couleur de fond des boutons classiques type CButton
HBRUSH CDlgItems::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Couleur de fond de la case à cocher rubrique
	if (pWnd->GetDlgCtrlID() == IDC_RI_CHK_RUBRIQUE_DEFAUT)
	{
		// on modifie le pinceau et la couleur de fond du texte
		pDC->SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp));

		pDC->SetBkMode(TRANSPARENT);

		// Puis on renvoie nouvelle couleur associé au fond bouton
		return m_BrushColorMedium;
	}

	// Couleur de fond de la case à cocher rubrique
	if (pWnd->GetDlgCtrlID() == IDC_RI_BUT_FILTRE || pWnd->GetDlgCtrlID() == IDC_RI_BUT_DIM1_UP || pWnd->GetDlgCtrlID() == IDC_RI_BUT_DIM1_DOWN)
	{
		// on modifie le pinceau et la couleur de fond du texte
		pDC->SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp));

		pDC->SetBkMode(TRANSPARENT);

		// Puis on renvoie nouvelle couleur associé au fond bouton
		return m_BrushColorMedium;
	}


	// Return a different brush if the default is not desired
    return hbrush;

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi la rubrique item par défaut
HTREEITEM CDlgItems::GetRubriqueItemDefaut()
{
	/* Désormais on met les items d'office dans rubrique par défaut
	if (m_Chk_Rubrique_Defaut.GetCheck())
	{
		return m_Tree_Rubrique_Items.GetItemByDefault();
	}
	else
		return NULL;
	*/

	// D'office dans rubrique par défaut
	if (m_Tree_Rubrique_Items)
		return m_Tree_Rubrique_Items.GetItemByDefault();
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi la rubrique palette par défaut
HTREEITEM CDlgItems::GetRubriquePaletteDefaut()
{
	/* Désormais on met les items palettes d'office dans rubrique par défaut
	if (m_Chk_Rubrique_Defaut.GetCheck())
	{
		return m_Tree_Rubrique_Palettes.GetItemByDefault();
	}
	else
		return NULL;
	*/
	// Aout 2013
	if (m_Tree_Rubrique_Palettes)
		return m_Tree_Rubrique_Palettes.GetItemByDefault();
	else
		return NULL;

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Ajout d'un item dans une rubrique via sélection dans boite de dialogue rubriques en cours
void CDlgItems::AddItemRubrique(JInt32 IdItem, JLabel LabelItem)
{

	// !!!!!!!!!!!! Pas gérer pour le moment	
	
	// atention en mode Mode_Items ou Mode_Palettes
	if (m_Id_CurMode != Mode_Items && m_Id_CurMode != Mode_Palettes)
		return;
	else
	{
		// Afficher boite de dialogue de toutes les rubriques valides
		if (m_Id_CurMode == Mode_Items)
			 // m_ComboRubriques.ShowWindow(SW_SHOW); 
			 m_ComboRubriques.ShowWindow(SW_HIDE); 
		else if (m_Id_CurMode != Mode_Palettes)
			m_ComboRubriques.ShowWindow(SW_HIDE); 

	}

}
