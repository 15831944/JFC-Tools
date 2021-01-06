// DlgChoixSupport.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgChoixSupport.h"
#include "DlgPlateau.h"
#include "ATPVersionCS.h"
#include "DialogFormat.h"
#include "DialogChoixOffre.h"
#include "JMFCStream.h"
#include "DlgDateTarif.h"
#include "DlgChoixSupport.h"
#include "DlgOrdreHeader.h"
#include "DlgOrdreGrandEch.h"
#include "DlgRangResult.h"
#include "JATPDocument.h"
#include "ATPDocCSSerializer.h"
#include ".\dlgchoixsupport.h"
#include "Offre2MainDialog.h"
#include <afxadv.h>            // For CSharedFile


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CRect CDlgChoixSupport::s_rectDefault(390, 130, 1070, 730);  // statique

// Position ss-menu nb insertions
const JUnt32 PosSsMenu_NbIns = 5;

// Boîte de dialogue CDlgChoixSupport
IMPLEMENT_DYNAMIC(CDlgChoixSupport, JFCDialog)
CDlgChoixSupport::CDlgChoixSupport(CWnd* pParent /*=NULL*/)
	: JFCDialog(CDlgChoixSupport::IDD, pParent)
	/*
	,
	m_BtnSupports(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnEchelle(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnMapping(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnCumulativite(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnDuplication(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnFamilles(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnSupports(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnFiltreAudience(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnFicheSupport(CATPColors::COLORCHOIXSUPPORTS),
	m_BtnCreuset(CATPColors::COLORCHOIXSUPPORTS)
	*/
{

	m_hIcon = AfxGetApp()->LoadIcon(IDI_CS_ICON);

	// Init pointeur pages
	m_pActivePage			= NULL;
	m_pPageEchelle			= NULL;
	m_pPageMapping			= NULL;
	m_pPageCumulativite		= NULL;
	m_pPageDuplication		= NULL;

	// Init pointeur pages pupitres
	m_pActivePupitre		= NULL;
	m_pPupitreEch			= NULL;
	m_pPupitreMap			= NULL;
	m_pPupitreCum			= NULL;
	m_pPupitreDup			= NULL;

	// on ajoute fen dialogue choix support pour update dialogues
	m_DocChoixSupport.AddDialog(this);

	// Initialise le module courant
	m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleEch);

	m_bFirstTime = TRUE;

	// Init Nb segment au total sur le ou les terrains
	m_DocChoixSupport.m_NbTotSegDispo = 1;

	// Identifie la couleur application
	m_CATPColorsApp		=	CATPColors::COLORCHOIXSUPPORTS;

}

CDlgChoixSupport::~CDlgChoixSupport()
{
}

void CDlgChoixSupport::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);

	// Contrôles sélection terrains
	DDX_Control(pDX, IDC_CS_LABEL_TERRAINS,		m_LblTerrains);
	DDX_Control(pDX, IDC_CS_LIST_TERRAINS,		m_LstTerrains);

	// Contrôles sélection cibles
	DDX_Control(pDX, IDC_CS_LABEL_CIBLES,		m_LblCibles);
	DDX_Control(pDX, IDC_CS_LIST_CIBLES,		m_LstCibles);

	// Contrôles sélection seg. population
	DDX_Control(pDX, IDC_CS_LABEL_SEGPOP,		m_LblSegPop);
	DDX_Control(pDX, IDC_CS_LIST_SEGPOP,		m_LstSegPop);

	// Contrôles sélection réf affinité
	DDX_Control(pDX, IDC_CS_LABEL_REFAFF,		m_LblRefAff);
	DDX_Control(pDX, IDC_CS_LIST_REFAFF,		m_LstRefAff);

	// Contrôles sélection formats
	DDX_Control(pDX, IDC_CS_LABEL_FORMATS,		m_LblFormats);
	DDX_Control(pDX, IDC_CS_LIST_FORMATS,		m_LstFormats);

	DDX_Control(pDX, IDC_CS_CADRE_SEL1,			m_CadreSelection);
	DDX_Control(pDX, IDC_CS_CADRE_RESULT,		m_CadreResult);
	DDX_Control(pDX, IDC_CS_CADRE_SEL2,			m_CadreSelSpe);
	DDX_Control(pDX, IDC_CS_CADRE_BTNFIXE,		m_CadreBtnPupitre);
	DDX_Control(pDX, IDC_CS_CADRE_ONGLET,		m_CadreOnglet);
	DDX_Control(pDX, IDC_CS_LABEL_FILTRE_AUDIENCE, m_BtnFiltreAudience);
	DDX_Control(pDX, IDC_CS_LABEL_NIVEAU,		m_LblNiveau);
	DDX_Control(pDX, IDC_CS_LABEL_FAMILLES,		m_BtnFamilles);
	DDX_Control(pDX, IDC_CS_LABEL_SUPPORTS,		m_BtnSupports);
}


BEGIN_MESSAGE_MAP(CDlgChoixSupport, JFCDialog)
	ON_COMMAND(IDM_CS_PLATEAU_NOUVEAU,			OnCsPlateauNouveau)
	ON_COMMAND(IDM_CS_PLATEAU_OUVRIR,			OnCsPlateauOuvrir)
	ON_COMMAND(IDM_CS_PLATEAU_ENREGISTRER,		OnCsPlateauEnregistrer)
	ON_COMMAND(IDM_CS_PLATEAU_ENREGISTRERSOUS,	OnCsPlateauEnregistrersous)
	ON_COMMAND(IDM_CS_PLATEAU_FERMER,			OnCsPlateauFermer)
	ON_COMMAND(IDM_CS_PLATEAU_QUITTER,			OnCsPlateauQuitter)
	ON_COMMAND(IDM_CS_SOURCES,					OnCsSources)
	//ON_COMMAND(IDM_CS_SUPPORTS,					OnCsSupports)
	ON_COMMAND(IDM_CS_CIBLES,					OnCsCibles)
	ON_COMMAND(IDM_CS_FORMATS,					OnCsFormats)
	ON_COMMAND(IDM_CS_PERIODETARIF,				OnCsPeriodeTarif)
	ON_UPDATE_COMMAND_UI(IDM_CS_PLATEAU_OUVRIR, OnUpdateCsPlateauOuvrir)
	ON_UPDATE_COMMAND_UI(IDM_CS_PLATEAU_ENREGISTRER,		OnUpdateCsPlateauEnregistrer)
	ON_UPDATE_COMMAND_UI(IDM_CS_PLATEAU_ENREGISTRERSOUS,	OnUpdateCsPlateauEnregistrersous)
	ON_UPDATE_COMMAND_UI(IDM_CS_PLATEAU_FERMER,				OnUpdateCsPlateauFermer)
	ON_COMMAND(IDM_CS_PALETTE_1,				OnCsPalette1)
	ON_COMMAND(IDM_CS_PALETTE_2,				OnCsPalette2)
	ON_COMMAND(IDM_CS_PALETTE_3,				OnCsPalette3)
	ON_COMMAND(IDM_CS_PALETTE_4,				OnCsPalette4)
	ON_COMMAND(IDM_CS_PALETTE_5,				OnCsPalette5)
	ON_COMMAND(IDM_CS_PALETTE_6,				OnCsPalette6)
	ON_COMMAND(IDM_CS_PALETTE_7,				OnCsPalette7)
	ON_COMMAND(IDM_CS_PALETTE_8,				OnCsPalette8)
	ON_COMMAND(IDM_CS_PALETTE_9,				OnCsPalette9)
	ON_COMMAND(IDM_CS_PALETTE_10,				OnCsPalette10)
	ON_COMMAND(IDM_CS_PALETTE_11,				OnCsPalette11)
	ON_COMMAND(IDM_CS_PALETTE_12,				OnCsPalette12)
	ON_COMMAND(IDM_CS_PALETTE_13,				OnCsPalette13)
	ON_COMMAND(IDM_CS_PALETTE_14,				OnCsPalette14)
	ON_COMMAND(IDM_CS_PALETTE_15,				OnCsPalette15)
	ON_BN_CLICKED(IDC_CS_ECHELLES,				OnBnClickedCsEchelles)
	ON_BN_CLICKED(IDC_CS_MAPPING,				OnBnClickedCsMapping)
	ON_BN_CLICKED(IDC_CS_CUMULATIVITE,			OnBnClickedCsCumulativite)
	ON_BN_CLICKED(IDC_CS_DUPLICATION,			OnBnClickedCsDuplication)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(IDM_CS_1_INSERTION,					OnCs1Insertion)
	ON_COMMAND(IDM_CS_2_INSERTIONS,					OnCs2Insertions)
	ON_COMMAND(IDM_CS_3_INSERTIONS,					OnCs3Insertions)
	ON_COMMAND(IDM_CS_4_INSERTIONS,					OnCs4Insertions)
	ON_COMMAND(IDM_CS_5_INSERTIONS,					OnCs5Insertions)
	ON_COMMAND(IDM_CS_6_INSERTIONS,					OnCs6Insertions)
	ON_COMMAND(IDM_CS_7_INSERTIONS,					OnCs7Insertions)
	ON_COMMAND(IDM_CS_8_INSERTIONS,					OnCs8Insertions)
	ON_COMMAND(IDM_CS_9_INSERTIONS,					OnCs9Insertions)
	ON_COMMAND(IDM_CS_10_INSERTIONS,				OnCs10Insertions)
	ON_COMMAND(IDM_CS_12_INSERTIONS,				OnCs12Insertions)
	ON_COMMAND(IDM_CS_24_INSERTIONS,				OnCs24Insertions)
	ON_COMMAND(IDM_CS_53_INSERTIONS,				OnCs53Insertions)
	ON_COMMAND(IDM_CS_105_INSERTIONS,				OnCs105Insertions)
	ON_COMMAND(IDM_CS_365_INSERTIONS,				OnCs365Insertions)
	ON_COMMAND(IDM_CS_MAX_INSERTIONS,				OnCsMaxInsertions)
	ON_LBN_SELCHANGE(IDC_CS_LIST_TERRAINS,			OnLbnSelchangeCsListTerrains)
	ON_LBN_SELCHANGE(IDC_CS_LIST_CIBLES,			OnLbnSelchangeCsListCibles)
	ON_LBN_SELCHANGE(IDC_CS_LIST_FORMATS,			OnLbnSelchangeCsListFormats)
	ON_COMMAND(IDM_CS_PLATEAU_IMPORTETUDE,			OnCsPlateauImportEtude)
	ON_COMMAND(IDM_CS_ABOUTBOX,						OnCsVersion)
	ON_COMMAND(IDM_CS_OPT_REPRISEENMAIN,			OnCsOptRepriseenmain)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPT_REPRISEENMAIN,	OnUpdateCsOptRepriseenmain)
	ON_COMMAND(IDM_CS_OPT_LNMMAGAZINE,				OnCsOptLnmmagazine)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPT_LNMMAGAZINE,	OnUpdateCsOptLnmmagazine)
	ON_COMMAND(IDM_CS_OPT_LDPQUOTIDIEN,				OnCsOptLdpquotidien)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPT_LDPQUOTIDIEN,	OnUpdateCsOptLdpquotidien)
	ON_COMMAND(IDM_CS_MODEL,						OnCsModel)
	ON_UPDATE_COMMAND_UI(IDM_CS_MODEL,				OnUpdateCsModel)
	ON_COMMAND(IDM_CS_MODELETAL,					OnCsModeletal)
	ON_UPDATE_COMMAND_UI(IDM_CS_MODELETAL,			OnUpdateCsModeletal)
	ON_COMMAND(IDM_CS_PROBABILIT,					OnCsProbabilit)
	ON_UPDATE_COMMAND_UI(IDM_CS_PROBABILIT,			OnUpdateCsProbabilit)
	ON_COMMAND(IDM_CS_BASETARIFPLAQ,				OnCsBasetarifplaq)
	ON_UPDATE_COMMAND_UI(IDM_CS_BASETARIFPLAQ,		OnUpdateCsBasetarifplaq)
	ON_COMMAND(IDM_CS_BASETARIFNEGO,				OnCsBasetarifnego)
	ON_UPDATE_COMMAND_UI(IDM_CS_BASETARIFNEGO,		OnUpdateCsBasetarifnego)
	// ON_COMMAND(IDM_CS_FILTRE_AUDIENCE,				OnCsFiltreAudience)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(IDM_CS_FILTRE_AUDIENCE,	OnUpdateCsFiltreAudience)
	ON_UPDATE_COMMAND_UI(IDM_CS_SOURCES,			OnUpdateCsSources)
	ON_UPDATE_COMMAND_UI(IDM_CS_SUPPORTS,			OnUpdateCsSupports)
	ON_UPDATE_COMMAND_UI(IDM_CS_CIBLES,				OnUpdateCsCibles)
	ON_UPDATE_COMMAND_UI(IDM_CS_FORMATS,			OnUpdateCsFormats)
	ON_UPDATE_COMMAND_UI(IDM_CS_PERIODETARIF,		OnUpdateCsPeriodetarif)
	ON_COMMAND(IDM_CS_AFFOPTMOTEUR,				    OnCsOptionMoteur)
	ON_UPDATE_COMMAND_UI(IDM_CS_AFFOPTMOTEUR,		OnUpdateCsOptionMoteur)
	ON_COMMAND(IDM_CS_ORDRE_ENTETES,				OnCsOrdreEntetes)
	ON_UPDATE_COMMAND_UI(IDM_CS_ORDRE_ENTETES,		OnUpdateCsOrdreEntetes)
	ON_UPDATE_COMMAND_UI(IDM_CS_MAX_INSERTIONS,		OnUpdateCsMaxInsertions)
	ON_BN_CLICKED(IDC_CS_LABEL_FILTRE_AUDIENCE,		OnBnClickedCsFiltreAudience)
	ON_WM_MENUSELECT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CS_LABEL_CREUSET,				OnBnClickedCsCreuset)
	ON_COMMAND(IDM_CS_OPTIONS_LDPBIMESTR,			OnOptionsLDP30j)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPTIONS_LDPBIMESTR, OnUpdateOptionsLDP30j)
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(IDM_CS_ORDRE_GRANDECH, OnCsOrdreGrandech)
	ON_UPDATE_COMMAND_UI(IDM_CS_ORDRE_GRANDECH, OnUpdateCsOrdreGrandech)
	ON_UPDATE_COMMAND_UI(IDM_CS_1_INSERTION, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_2_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_3_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_4_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_5_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_6_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_7_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_8_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_9_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_10_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_12_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_24_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_53_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_105_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_365_INSERTIONS, OnUpdateCsNInsertion)
	ON_UPDATE_COMMAND_UI(IDM_CS_MAX_INSERTIONS, OnUpdateCsNInsertion)
	ON_BN_CLICKED(IDC_CS_LABEL_FAMILLES, OnBnClickedCsLabelFamilles)
	ON_BN_CLICKED(IDC_CS_LABEL_SUPPORTS, OnBnClickedCsLabelSupports)
	ON_COMMAND(ID_CS_NBD_AUCUNEDECIMALE, OnCsAucunedecimale)
	ON_COMMAND(ID_CS_NBD_1DECIMALE, OnCs1decimale)
	ON_COMMAND(ID_CS_NBD_2DECIMALE, OnCs2decimale)
	ON_UPDATE_COMMAND_UI(ID_CS_NBD_AUCUNEDECIMALE, OnUpdateNbddecimale)
	ON_COMMAND(IDM_CS_RANG_COL_VISIBLE, OnCsRangColVisible)
	ON_UPDATE_COMMAND_UI(IDM_CS_RANG_COL_VISIBLE, OnUpdateCsRangColVisible)
	ON_COMMAND(IDM_CS_MOYENNE_VISIBLE, OnCsMoyenneVisible)
	ON_UPDATE_COMMAND_UI(IDM_CS_MOYENNE_VISIBLE, OnUpdateCsMoyenneVisible)
	ON_COMMAND(ID_PARAM_AFFICHAGEDESRANGSPARR, OnCsAffichageRangResult)
	
	
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_ECHELLE, OnCsExporterPressepapierEch)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_ECHELLE, OnUpdateCsExporterPressepapierEch)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_MAPPING, OnCsExporterPressepapierMap)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_MAPPING, OnUpdateCsExporterPressepapierMap)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_CUMULATIVITE, OnCsExporterPressepapierCum)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_CUMULATIVITE, OnUpdateCsExporterPressepapierCum)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_DUPLICATION, OnCsExporterPressepapierDup)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_DUPLICATION, OnUpdateCsExporterPressepapierDup)
	
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_ECHELLE, OnCsExporterFichierEch)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_ECHELLE, OnUpdateCsExporterFichierEch)
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_MAPPING, OnCsExporterFichierMap)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_MAPPING, OnUpdateCsExporterFichierMap)
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_CUMULATIVITE, OnCsExporterFichierCum)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_CUMULATIVITE, OnUpdateCsExporterFichierCum)
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_DUPLICATION, OnCsExporterFichierDup)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_DUPLICATION, OnUpdateCsExporterFichierDup)
	
	ON_COMMAND(IDM_CS_EXPORTER_EXCEL_ECHELLE, OnCsExporterExcelEch)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_EXCEL_ECHELLE, OnUpdateCsExporterExcelEch)
	ON_COMMAND(IDM_CS_EXPORTER_EXCEL_MAPPING, OnCsExporterExcelMap)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_EXCEL_MAPPING, OnUpdateCsExporterExcelMap)
	ON_COMMAND(IDM_CS_EXPORTER_EXCEL_CUMULATIVITE, OnCsExporterExcelCum)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_EXCEL_CUMULATIVITE, OnUpdateCsExporterExcelCum)
	ON_COMMAND(IDM_CS_EXPORTER_EXCEL_DUPLICATION, OnCsExporterExcelDup)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_EXCEL_DUPLICATION, OnUpdateCsExporterExcelDup)

	ON_COMMAND(IDM_CS_PRINT_ECHELLE, OnCsPrintEchelle)
	ON_UPDATE_COMMAND_UI(IDM_CS_PRINT_ECHELLE, OnUpdateCsPrintEchelle)
	ON_COMMAND(IDM_CS_PRINT_MAPPING, OnCsPrintMapping)
	ON_UPDATE_COMMAND_UI(IDM_CS_PRINT_MAPPING, OnUpdateCsPrintMapping)
	ON_COMMAND(IDM_CS_PRINT_CUMULATIVITE, OnCsPrintCumulation)
	ON_UPDATE_COMMAND_UI(IDM_CS_PRINT_CUMULATIVITE, OnUpdateCsPrintCumulation)
	ON_COMMAND(IDM_CS_PRINT_DUPLICATION, OnCsPrintDuplication)
	ON_UPDATE_COMMAND_UI(IDM_CS_PRINT_DUPLICATION, OnUpdateCsPrintDuplication)
	
	ON_UPDATE_COMMAND_UI(ID_PARAM_AFFICHAGEDESRANGSPARR, OnUpdateAffichageRangResult)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_GRAPHMAPPING, OnCsExporterPressepapierGraphmapping)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_GRAPHMAPPING, OnUpdateCsExporterPressepapierGraphmapping)
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_GRAPHMAPPING, OnCsExporterFichierGraphmapping)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_GRAPHMAPPING, OnUpdateCsExporterFichierGraphmapping)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_GRAPHCUMUL, OnCsExporterPressepapierGraphcumul)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_GRAPHCUMUL, OnUpdateCsExporterPressepapierGraphcumul)

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCLBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_SIZING()
	ON_LBN_SELCHANGE(IDC_CS_LIST_SEGPOP, OnLbnSelchangeCsListSegPop)

	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_ECHELLE_PARCIBLE, OnCsExporterFichierEchParcible)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_ECHELLE_PARCIBLE, OnUpdateCsExporterFichierEchParcible)
	ON_COMMAND(IDM_CS_EXPORTER_PRESSEPAPIER_ECHELLE_PARCIBLE, OnCsExporterPressepapierEchParcible)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_PRESSEPAPIER_ECHELLE_PARCIBLE, OnUpdateCsExporterPressepapierEchParcible)
	ON_COMMAND(IDM_CS_PRINT_ECHELLE_PARCIBLE, OnCsPrintEchParcible)
	ON_UPDATE_COMMAND_UI(IDM_CS_PRINT_ECHELLE_PARCIBLE, OnUpdateCsPrintEchParcible)
	ON_COMMAND(IDM_CS_EXPORTER_FICHIER_GRAPHCUMUL, OnCsExporterFichierGraphcumul)
	ON_UPDATE_COMMAND_UI(IDM_CS_EXPORTER_FICHIER_GRAPHCUMUL, OnUpdateCsExporterFichierGraphcumul)
	// ON_COMMAND(IDM_CS_OPT_CUMUL_FAMILLE, OnCsOptCumulFamille)
	// ON_UPDATE_COMMAND_UI(IDM_CS_OPT_CUMUL_FAMILLE, OnUpdateCsOptCumulFamille)
	ON_COMMAND(IDM_CS_OPT_FAM_CUMUL, OnCsOptFamCumul)
	ON_COMMAND(IDM_CS_OPT_FAM_MOYENNE, OnCsOptFamMoyenne)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPT_FAM_CUMUL, OnUpdateCsOptFamCumul)
	ON_UPDATE_COMMAND_UI(IDM_CS_OPT_FAM_MOYENNE, OnUpdateCsOptFamMoyenne)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_1, OnUpdateCsPalette1)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_2, OnUpdateCsPalette2)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_3, OnUpdateCsPalette3)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_4, OnUpdateCsPalette4)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_5, OnUpdateCsPalette5)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_6, OnUpdateCsPalette6)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_7, OnUpdateCsPalette7)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_8, OnUpdateCsPalette8)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_9, OnUpdateCsPalette9)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_10, OnUpdateCsPalette10)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_11, OnUpdateCsPalette11)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_12, OnUpdateCsPalette12)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_13, OnUpdateCsPalette13)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_14, OnUpdateCsPalette14)
	ON_UPDATE_COMMAND_UI(IDM_CS_PALETTE_15, OnUpdateCsPalette15)
	END_MESSAGE_MAP()

BOOL CDlgChoixSupport::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// Définir l'icône de cette boîte de dialogue. L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	// Application couleur interface 
	CATPColors::SetAppColor(CATPColors::COLORCHOIXSUPPORTS, AfxGetApp()->GetProfileInt("Options\\ChoixSupport", "ColorChoixSupport", CATPColors::BLEU_PASTEL) );

	// Association boutons
	SetupControls();

	// NO_FULL_REPAINT_ON_RESIZE 

	//Création de la page Echelles et de son pupitre
	m_pPageEchelle = new CDlgEchelle(this,&m_DocChoixSupport);
	if(m_pPageEchelle->Create(IDD_CS_DLG_ECHELLES, this) == FALSE)
		return FALSE;
	m_pPupitreEch  = new CDlgPupitreEch(this,&m_DocChoixSupport,m_pPageEchelle);
	if(m_pPupitreEch->Create(IDD_CS_DLG_PUPITRE_ECH, this) == FALSE)
		return FALSE;

	//Création de la page Mapping et de son pupitre
	m_pPageMapping = new CDlgMapping(this,&m_DocChoixSupport);
	if(m_pPageMapping->Create(IDD_CS_DLG_MAPPING, this) == FALSE)
		return FALSE;
	m_pPupitreMap  = new CDlgPupitreMap(this,&m_DocChoixSupport,m_pPageMapping);
	if(m_pPupitreMap->Create(IDD_CS_DLG_PUPITRE_MAP, this) == FALSE)
		return FALSE;

	//Création de la page Cumulativité et de son pupitre
	m_pPageCumulativite = new CDlgCumulativite(this,&m_DocChoixSupport);
	if(m_pPageCumulativite->Create(IDD_CS_DLG_CUMULATIVITE, this) == FALSE)
		return FALSE;
	m_pPupitreCum  = new CDlgPupitreCum(this,&m_DocChoixSupport,m_pPageCumulativite);
	if(m_pPupitreCum->Create(IDD_CS_DLG_PUPITRE_CUM, this) == FALSE)
		return FALSE;

	//Création de la page Duplication et de son pupitre
	m_pPageDuplication = new CDlgDuplication(this,&m_DocChoixSupport);
	if(m_pPageDuplication->Create(IDD_CS_DLG_DUPLICATION, this) == FALSE)
		return FALSE;
	m_pPupitreDup  = new CDlgPupitreDup(this,&m_DocChoixSupport,m_pPageDuplication);
	if(m_pPupitreDup->Create(IDD_CS_DLG_PUPITRE_DUP, this) == FALSE)
		return FALSE;

	// on ajoute fens dialogues cadres pupitres
	m_DocChoixSupport.AddDialog(m_pPupitreEch);
	m_DocChoixSupport.AddDialog(m_pPupitreMap);
	m_DocChoixSupport.AddDialog(m_pPupitreCum);
	m_DocChoixSupport.AddDialog(m_pPupitreDup);

	// on ajoute fens dialogues cadres principaux
	m_DocChoixSupport.AddDialog(m_pPageEchelle);
	m_DocChoixSupport.AddDialog(m_pPageMapping);
	m_DocChoixSupport.AddDialog(m_pPageCumulativite);
	m_DocChoixSupport.AddDialog(m_pPageDuplication);

	// Création boite de dialogue filtre d'audience
	if (m_DlgFiltresAudience.Create(CDialogFiltres::IDD, this) == FALSE)
		return FALSE;

	// Envoi document à la boite filtre d'audience
	m_DlgFiltresAudience.SetDocument(&m_DocChoixSupport);
	m_DlgFiltresAudience.SetColors(CATPColors::COLORCHOIXSUPPORTS);


	if (m_DlgChoixMoteur.Create(CDlgChoixMoteur::IDD, this) == FALSE)
		return (-1);
	m_DlgChoixMoteur.SetDocument(&m_DocChoixSupport);
	m_DlgChoixMoteur.SetColors(CATPColors::COLORCHOIXSUPPORTS);

	// Ne pas valider les boutons mode affichage si aucune étude en cours
	m_BtnFamilles.EnableWindow(FALSE); 
	m_BtnSupports.EnableWindow(FALSE); 

	// Ne pas valider les boutons Filtre et Contexte si aucune étude en cours
	m_BtnFiltreAudience.EnableWindow(FALSE);
	
	// Fenêtre échelle par défaut
	OnBnClickedCsEchelles();

	//Taille nominale de la boite 650x570
	SetWindowPos(NULL, 0,0, 680, 600, SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOREDRAW);

	// Mise en route timer
	FlagResize = false;
	SetTimer(IdTimer, 2000, NULL);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//							GESTION MENU DIALOGUE CHOIX SUPPORTS														
///////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// Création nouveau plateau
void CDlgChoixSupport::OnCsPlateauNouveau()
{
	// on crée un nouveau document
	JATPDocChoixSupport ATPDocChoixSupport;

	// on teste la validité du document courant
	if (m_DocChoixSupport.IsValid())
	{
		// Sauvegarde ou non du document en cours...........
		SavePlateauViaQuestion();
	}

	// Via module offre, on charge les paramétres courants "Source"
	JString NomAppli = "ChoixSupport";
	ATPDocChoixSupport.m_Offre.LoadParamSource(NomAppli); 

	// Puis on charge les options moteur en cours
	LoadOptionMoteur(ATPDocChoixSupport);

	// on lance la boite d'offre sans l'onglet 'Période'
	COffre2MainDialog Dlg(CATPColors::COLORCHOIXSUPPORTS, this, false);
	Dlg.SetTitle("Création de l'offre pour l'Analyseur");

	// Setup de la boite offre source
	Dlg.SetStartupMode(COffre2MainDialog::eodoTerrainsSupports);

	// On passe l'offre et les terrains
	Dlg.SetOffre(ATPDocChoixSupport.m_Offre);

	// on affiche la boîte de dialogue
	if (IDOK == Dlg.DoModal())
	{
		//if(Dlg.IsModified()) TODO : aurelien : gestion modification ou pas de l'offre

		// on récupère la nouvelle offre sélectionnée par l'utilisateur
		Dlg.GetOffre(ATPDocChoixSupport.m_Offre);

		// on vérifie la validité du nouveau document
		if (ATPDocChoixSupport.IsValid())
		{
			// on prépare le contenu du document
			if (FALSE != ATPDocChoixSupport.PrepareContents())
			{
				// On récupère les derniers paramétrages
				ATPDocChoixSupport.RecupParamDef();

				// on permute les documents
				m_DocChoixSupport.Swap(ATPDocChoixSupport);

				// Garde ordre affichage résultat 
				JList <JUnt32> LstOrdoGrandeurEch;
				LstOrdoGrandeurEch.Reset();
				for (m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveFirst();
					m_DocChoixSupport.m_LstOrdoGrandeurEch.IsValid();
					m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveNext())
				{
					LstOrdoGrandeurEch.AddTail() =  m_DocChoixSupport.m_LstOrdoGrandeurEch.GetItem();
				}

				m_pPupitreEch->InitLstGrandeurs();
				m_pPupitreEch->LoadGrandeurs();

				// Repositionne les flag params calcul
				ParamCalculModif();

				// on positionne l'indicateur de modification
				m_DocChoixSupport.SetModifiedFlag(TRUE);

				// TODO : changer ICI
				// TODO : changer ICI
				// TODO : changer ICI
				// en attendant de refaire la fonction RestreincreOffre
				CDialogChoixOffre oldDlg(this);
				oldDlg.SetOffre(m_DocChoixSupport.m_Offre);
				oldDlg.GetOffreRestreinte(m_DocChoixSupport.m_Offre); 

		//// Reconstruit la table des items interdits
		//m_DocChoixSupport.m_Offre.ConstructionItemsInterdits();



				// On récupère ordre affichage résultat
				m_DocChoixSupport.m_LstOrdoGrandeurEch.Reset();
				for (LstOrdoGrandeurEch.MoveFirst();
					LstOrdoGrandeurEch.IsValid();
					LstOrdoGrandeurEch.MoveNext())
				{
					m_DocChoixSupport.m_LstOrdoGrandeurEch.AddTail() =  LstOrdoGrandeurEch.GetItem();
				}

				// on met à jour les vues du document
				m_DocChoixSupport.UpdateAllViews(UPDATE_INITIAL);

				// Mise à jour titre fenêtre
				MajTitreFenetreCS();

				// Via module offre, on charge les paramétres courants "Source"
				JString NomAppli = "ChoixSupport";
				m_DocChoixSupport.m_Offre.SaveParamSource(NomAppli); 
			}
			else
			{
				// on affiche le message
				HWND hHwnd = NULL;
				CWnd *phParent = this->GetParent(); 
				if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
				::MessageBox(hHwnd, "L'initialisation du plateau." ,"NOUVEAU PLATEAU SUPPORTS", MB_ICONERROR | MB_OK);
			}
		}
		else
		{
			// on affiche le message
			HWND hHwnd = NULL;
			CWnd *phParent = this->GetParent(); 
			if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
			::MessageBox(hHwnd, "Le plateau que vous voulez créer n'est pas valide." ,"NOUVEAU PLATEAU SUPPORTS", MB_ICONERROR | MB_OK);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des dernieres options moteurs
void CDlgChoixSupport::LoadOptionMoteur(JATPDocChoixSupport &ATPDocChoixSupport)
{
	// Les options calcul
	if (m_DocChoixSupport.IsValid())
	{
		// Les options calculs courantes
		ATPDocChoixSupport.m_bRepriseEnMain	= m_DocChoixSupport.m_bRepriseEnMain; 
		ATPDocChoixSupport.m_bLNMMag			= m_DocChoixSupport.m_bLNMMag;
		ATPDocChoixSupport.m_bLDPQtd			= m_DocChoixSupport.m_bLDPQtd; 		
		ATPDocChoixSupport.m_bLDP30J			= m_DocChoixSupport.m_bLDP30J; 
		ATPDocChoixSupport.m_CumulFamille		= m_DocChoixSupport.m_CumulFamille; 

		// et le type de moteur en cours
		int TypeMoteur		= m_DocChoixSupport.m_MoteurPresse.GetType(); 
		ATPDocChoixSupport.m_MoteurPresse.SetType((CMoteurPresse::TYPEMOTEUR)TypeMoteur);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Création d'un nouveau plateau
void CDlgChoixSupport::CreerNvPlateau()
{
	// on teste la validité du document courant
	if (m_DocChoixSupport.IsValid())
	{
		// Sauvegarde ou non du document en cours...........
		SavePlateauViaQuestion();
	}
	
	// Garde ordre affichage résultat 
	JList <JUnt32> LstOrdoGrandeurEch;
	LstOrdoGrandeurEch.Reset();
	for (m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveFirst();
 		 m_DocChoixSupport.m_LstOrdoGrandeurEch.IsValid();
		 m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveNext())
	{
		LstOrdoGrandeurEch.AddTail() =  m_DocChoixSupport.m_LstOrdoGrandeurEch.GetItem();
	}

	m_pPupitreEch->InitLstGrandeurs();
	m_pPupitreEch->LoadGrandeurs();

	// Repositionne les flag params calcul
	ParamCalculModif();

	// on positionne l'indicateur de modification
	m_DocChoixSupport.SetModifiedFlag(TRUE);

	// On récupère ordre affichage résultat
	m_DocChoixSupport.m_LstOrdoGrandeurEch.Reset();
	for (LstOrdoGrandeurEch.MoveFirst();
 		 LstOrdoGrandeurEch.IsValid();
		 LstOrdoGrandeurEch.MoveNext())
	{
		m_DocChoixSupport.m_LstOrdoGrandeurEch.AddTail() =  LstOrdoGrandeurEch.GetItem();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Ouverture d'un plateau
void CDlgChoixSupport::OnCsPlateauOuvrir()
{
	// Sauvegarde ou non du document en cours...........
	SavePlateauViaQuestion();

	// Chargement d'un plateau
	OnLoadPlateau("", true);

	// Mise à jour titre fenêtre
	MajTitreFenetreCS();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Enregistrer un plateau
void CDlgChoixSupport::OnCsPlateauEnregistrer()
{
	// on teste la validité du document choix supports courant
	if (m_DocChoixSupport.IsValid())
	{
		// on sauvegarde le document courant
		if (FALSE != this->OnSaveDocument(m_DocChoixSupport, false))
		{
			// on annule l'indicateur de modification
			m_DocChoixSupport.SetModifiedFlag(FALSE);
		}
		else
			// Mise à jour titre fenêtre
			MajTitreFenetreCS();
	}
	else
	{
		// on affiche le message
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); 
		if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Le plateau que vous voulez enregistrer n'est pas valide." ,"ENREGISTRER PLATEAU", MB_ICONERROR | MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Enregistrer un plateau sous un autre nom
void CDlgChoixSupport::OnCsPlateauEnregistrersous()
{
	// on teste la validité du document choix supports courant
	if (m_DocChoixSupport.IsValid())
	{
		// on sauvegarde le document courant
		if (FALSE != this->OnSaveDocument(m_DocChoixSupport, true))
		{
			// on annule l'indicateur de modification
			m_DocChoixSupport.SetModifiedFlag(FALSE);
		}
	}
	else
	{
		// on affiche le message
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); 
		if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Le plateau que vous voulez enregistrer n'est pas valide." ,"ENREGISTRER PLATEAU SOUS AUTRE NOM", MB_ICONERROR | MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Fermeture du plateau courant
void CDlgChoixSupport::OnCsPlateauFermer()
{
	// on teste la validité du document choix supports courant
	if (m_DocChoixSupport.IsValid())
	{
		// on crée un nouveau document choix supports
		JATPDocChoixSupport ATPDocChoixSupport;
		ATPDocChoixSupport.PrepareContents();

		// Sauvegarde ou non du document en cours...........
		SavePlateauViaQuestion();
		
		// on permute les documents
		m_DocChoixSupport.Swap(ATPDocChoixSupport);

		// on annule l'indicateur de modification
		m_DocChoixSupport.SetModifiedFlag(FALSE);

		// on réinitialise tout et invalide la date document
		JDate DateInit;
		m_DocChoixSupport.m_DateTarifaire = DateInit; 
		m_DocChoixSupport.UpdateAllViews(UPDATE_RAZ);

		// Mise à jour titre fenêtre
		MajTitreFenetreCS();
	}

	// Pas de page active
	// m_pActivePage = NULL; 
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde du plateau courant si bon vouloir utilisateur
void CDlgChoixSupport::SavePlateauViaQuestion()
{
	// on teste l'indicateur de modification du document courant 
	if (FALSE != m_DocChoixSupport.IsModified())
	{
		// on affiche le message
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); 
		if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		//INT Ret = ::MessageBox(hHwnd, "Voulez-vous enregistrer les modifications apportées au plateau courant ?" ,"SAUVEGARDE PLATEAU COURANT", MB_ICONQUESTION | MB_YESNO);
		INT Ret = this->MessageBox("Voulez-vous enregistrer les modifications apportées au plateau courant ?" ,"ANALYSE DE L'OFFRE", MB_ICONQUESTION | MB_YESNO);

		// on teste la réponse de l'utilisateur
		if (Ret == IDYES)
		{
			// on sauvegarde le document choix support courant
			if (FALSE == this->OnSaveDocument(m_DocChoixSupport, false)) return;

			// on annule l'indicateur de modification
			m_DocChoixSupport.SetModifiedFlag(FALSE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Quitter Plateau Choix Supports
void CDlgChoixSupport::OnCsPlateauQuitter()
{
	if (this->CanClose()) 
	{
		// on appelle le gestionnaire de base	
		this->OnClose();
		this->ShowWindow(SW_HIDE); 
	}
	
}

// A REVOIR..............................
// 
// JATPDocCreuset a metre en public y compris les KEYEtude, KeyPlateau	
//
///////////////////////////////////////////////////////////////////////////////////////////////
// Import Plateau d'une étude existante
void CDlgChoixSupport::OnCsPlateauImportEtude()
{
	// Attention prévoir renregistrement plateau courant s'il y a lieu
	// AfxMessageBox("IMPORT PLATEAU D UNE ETUDE",MB_ICONINFORMATION);

	// on initialise l'indicateur
	BOOL Ret = FALSE;
	JMFCStream* stream = NULL;

	// Préparation filtre fichiers plateaux
	CString szFilters;
	szFilters.LoadString(IDS_CS_FILTER_ATP);
	DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
	CFileDialog fileDlg(TRUE, "atp", 0, flags, szFilters, this);

	// Affichage de la liste des plateaux existants
	CString path;
	if( fileDlg.DoModal() == IDOK )
	{
		// Récupère le chemin du plateau creuset
		path = fileDlg.GetPathName();

		try
		{
			// Récupère les informations offre du document
			CFile file(path, CFile::modeRead);
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);

			// L'offre à récupérer
			JATPOffre Offre;

			// Liste des supports sélectionnés
			JList <JUnt32> LstSelSupp; 

			// Récup offre + plateau + date début étude
			CATPDocCSSerializer DocCSSerializer;
			if (DocCSSerializer.RecvOffrePlateau(*stream,Offre, LstSelSupp, m_DocChoixSupport.m_DateTarifaire)) 
			{
				// On créer une nouvelle étude
				CreerNvPlateau();

				// On y insére l'offre de creuset
				m_DocChoixSupport.m_Offre.Swap(Offre);   

				// Dévalide tous les supports de l'offre
				for (m_DocChoixSupport.m_Offre.m_TBLIdSupports.MoveFirst();
					 m_DocChoixSupport.m_Offre.m_TBLIdSupports.IsValid();
					 m_DocChoixSupport.m_Offre.m_TBLIdSupports.MoveNext())
				{
					// Id support à initialiser
					JUnt32 IdSupport = m_DocChoixSupport.m_Offre.m_TBLIdSupports.GetItem();

					// Mise à jour du document
					m_DocChoixSupport.SetSelSupport(IdSupport,false);
				}

				// Tous les supports visibles d'office
				m_DocChoixSupport.SetAllSupportVisible(true); 

				// On positionne les supports sélectionnés
				for (LstSelSupp.MoveFirst(); LstSelSupp.IsValid(); LstSelSupp.MoveNext())   
				{
					// Support sélectionné
					JUnt32 IdSupport = LstSelSupp.GetItem();

					// Mise à jour du document
					m_DocChoixSupport.SetSelSupport(IdSupport,true);
				}

				// on récupère le map des couleurs affectées aux supports pour les graphes
				for (m_DocChoixSupport.m_MapIdColorSupports.MoveFirst();
					 m_DocChoixSupport.m_MapIdColorSupports.IsValid();
					 m_DocChoixSupport.m_MapIdColorSupports.MoveNext())
				{
					// Init pas de couleur associée
                    JUnt32				&IdColor = m_DocChoixSupport.m_MapIdColorSupports.GetItem(); 
					IdColor = 0;
				}

				// Remet le mode couleur normal
				 m_DocChoixSupport.m_ModeColorGraphe = 0;

				// Vérifie date existante
				/*
				COleDateTime DateCur = COleDateTime::GetCurrentTime();
				JDate DateTarJour;
				DateTarJour.SetDate(DateCur.GetDay(),DateCur.GetMonth(),DateCur.GetYear());  

				if (!m_DocChoixSupport.m_DateTarifaire.IsValid())
				{
					m_DocChoixSupport.m_DateTarifaire = DateTarJour;
				}
				*/

				// on met à jour les vues du document
				m_DocChoixSupport.UpdateAllViews(UPDATE_INITIAL);

				// Mise à jour titre fenêtre
				MajTitreFenetreCS();
			}
			else
			{
				// Problème offre (certainement car ancienne version sauvegarde)
				AfxMessageBox("IMPORT PLATEAU ETUDE IMPOSSIBLE CAR NON GERE SUR ANCIENNE VERSION ETUDE",MB_ICONINFORMATION);
			}
		}
		catch(...)
		{
		}
	}

	else
		// Annulation chargement
		return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour charger/sauver les documents choix support
BOOL CDlgChoixSupport::OnLoadPlateau(CString path, bool bDialog)
{
	JATPDocChoixSupport ATPDocChoixSupport;
	// on initialise l'indicateur validation plateau
	BOOL Ret = FALSE;
	JMFCStream* stream		= NULL;

	if (bDialog)
	{
		// Préparation filtre fichiers plateaux
		CString szFilters;
		szFilters.LoadString(IDS_CS_FILTER);
		DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
		CFileDialog fileDlg(TRUE, "ofr", 0, flags, szFilters, this);

		// Affichage de la liste des plateaux existants
		// JATPDocChoixSupport ATPDocChoixSupport;
		if( fileDlg.DoModal() == IDOK )
			path = fileDlg.GetPathName();
		else
			return FALSE;
	}
	else
		UpdateWindow();

	// On se positionne sur le répertoire User
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	//SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	try
	{

		///////////////////////////// GROSSE VERRUE (du à code 282 de AEPM 2006-2007 en mode StdioFile ////////////////////////////
		//
		// par défaut on prend pour les versions > 7 nouveau mode lecture via CFile (et non StdioFile)

		// Lecture N° de version
		JUnt32 NoVersion = 0;
		CFile filetmp(path, CFile::modeRead);
		
		// Ouvre le fichier en mode sérialize
		CArchive ar(&filetmp, CArchive::load);
		stream = JMFCStream::Create(ar);

		// Récup n° de version
		ATPDocChoixSupport.RecvNoVersion(*stream, NoVersion);

		// on ferme avant de le réouvrir
		/*
		if (stream)
			delete stream;
		*/

		// Ferme l'archive
		ar.Close();

		// Ferme le fichier
		filetmp.Close(); 
		
		// Selon le N° de version on ouvre en CFile ou StdioFile (pour récupérer les anciens fichiers)
		JMFCStream* stream = NULL;
		if (NoVersion <= 7)
		{
			// Ouverture en mode StdioFile (Récup anciens fichiers)
			CStdioFile file(path, CFile::modeRead);

			// Le reste reste identique
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
			ATPDocChoixSupport.m_KEYPlateau.SetPath(path); 
		}
		else
		{
			CFile file(path, CFile::modeRead);

			// Le reste reste identique
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
			ATPDocChoixSupport.m_KEYPlateau.SetPath(path); 
		}
	}

	catch(...)
	{
		AfxMessageBox("Problème récupération étude",MB_ICONINFORMATION);
		if (stream)	delete stream;
		return FALSE;
	}

	if (stream)
		delete stream;

	if (ATPDocChoixSupport.IsValid())
	{
		// on prépare le contenu du document choix support
		if (FALSE != ATPDocChoixSupport.PrepareContents())
		{
			// on permute les documents
			m_DocChoixSupport.Swap(ATPDocChoixSupport);

			// Repositionne les flag params calcul
			ParamCalculModif();

			// on met à jour les vues du document
			AfficheBarreCalcul();
			m_DocChoixSupport.UpdateAllViews(UPDATE_LOADPLATEAU);

			// Récupère les infos plateau
			m_DocChoixSupport.UpdateAllViews(UPDATE_KEY_PLATEAU); 

			Ret = TRUE;
		}
		else
		{
			// on affiche le message
			::AfxMessageBox("L'initialisation du plateau a échoué.", MB_ICONERROR | MB_OK);
		}
	}
	else
	{
		// on affiche le message
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); 
		if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Le plateau que vous voulez ouvrir n'est pas valide." ,"OUVERTURE PLATEAU SUPPORTS", MB_ICONERROR | MB_OK);
	}

	return (Ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////////			
// Affichage barre calcul pour l'application en cours
void CDlgChoixSupport:: AfficheBarreCalcul()
{
	// Si page appli en cours, on appel affichage de sa barr de défilement
	if (m_pActivePage)
	{
		// Barre calcul dans Echelle
		if (m_pActivePage == m_pPageEchelle)
			m_pPageEchelle->AffichBarreCalcul();

		// Barre calcul dans Mapping
		else if (m_pActivePage == m_pPageMapping)
			m_pPageMapping->AffichBarreCalcul();

		// Barre calcul dans Cumulativité
		else if (m_pActivePage == m_pPageCumulativite)
			m_pPageCumulativite->AffichBarreCalcul();

		// Barre calcul dans Duplication
		else if (m_pActivePage == m_pPageDuplication)
			m_pPageDuplication->AffichBarreCalcul();
	}
}	

// Choix de la Source et des Terrains
void CDlgChoixSupport::OnCsSources()
{
	// On vérifie que l'on a bien sélectionné une date de tarification
	if (!m_DocChoixSupport.m_DateTarifaire.IsValid())
	{
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Il faut d'abord choisir une date de tarification" ,"SELECTION SOURCE / TERRAINS ", MB_ICONEXCLAMATION);
		return;
	}

	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude;
	JDate FinEtude;

	// Sauvegarde ancien type de poids
	JUnt32 ExIdTypePoids = m_DocChoixSupport.m_Offre.m_IdTypePoids; 

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre courante", m_CATPColorsApp, 
		m_DocChoixSupport.m_Offre, COffre2MainDialog::eodoTerrainsSupports,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this, false))
	{
		// Redessine fenêtres en cours
		m_pActivePage->UpdateWindow();
		this->UpdateWindow(); 

		// Si éléments modifiés, on récupère les nouvelles infos
		if(bOffreModified)
		{
			// Repositionne les flag params calcul
			ParamCalculModif();

			// Le document a changé
			m_DocChoixSupport.SetModifiedFlag(TRUE); 
		}

		// TODO : changer ICI
		// TODO : changer ICI
		// TODO : changer ICI
		// en attendant de refaire la fonction RestreincreOffre
		CDialogChoixOffre oldDlg(this);
		oldDlg.SetOffre(m_DocChoixSupport.m_Offre);
		oldDlg.GetOffreRestreinte(m_DocChoixSupport.m_Offre); 

		// Reconstruit la table des items interdits
		m_DocChoixSupport.m_Offre.ConstructionItemsInterdits();

		if (m_DocChoixSupport.m_Offre.m_IdTypePoids == ExIdTypePoids) 
		{
			// Mise à jour globale sans changement de type de poids
			m_DocChoixSupport.UpdateAllViews(UPDATE_OFFRE);
		}
		else
		{
			// Mise à jour globale avec changement de type de poids (comme initialisation général)
			m_DocChoixSupport.UpdateAllViews(UPDATE_INITIAL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate option menu Sources
void CDlgChoixSupport::OnUpdateCsSources(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid()) 
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Appel de la boite Offres pour choix Supports
void CDlgChoixSupport::OnCsSupports()
{
	///////////////////////////////////////////////
	// On vérifie que l'on a bien sélectionné une période d'action
	if (!m_DocChoixSupport.m_DateTarifaire.IsValid()) 
	{
		// AfxMessageBox("Il faut d'abord choisir une période d'action",MB_ICONEXCLAMATION);
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Il faut d'abord choisir une date tarifaire" ,"SELECTION SUPPORTS", MB_ICONEXCLAMATION);
		return;
	}

	// La boite offre
	CDialogChoixOffre Dlg(this);

	// Setup de la boite offre supports
	Dlg.SetStartupMode(CDialogChoixOffre::SUPPORT, CATPColors::COLORCHOIXSUPPORTS);

	// On passe l'offre et les terrains
	Dlg.SetOffre(m_DocChoixSupport.m_Offre);

	// Affichage de la fenêtre offre
	if(Dlg.DoModal()== IDOK)
	{
		// Redessine fenêtres en cours
		m_pActivePage->UpdateWindow();
		this->UpdateWindow(); 

		// Si éléments modifiés, on récupère les nouvelles infos
		if(Dlg.IsModified())
		{
			// Repositionne les flag params calcul
			ParamCalculModif();

			// Le document a changé
			m_DocChoixSupport.SetModifiedFlag(TRUE); 
		}

		// Récupère l'offre
		//Dlg.GetOffre(m_DocChoixSupport.m_Offre);
		Dlg.GetOffreRestreinte(m_DocChoixSupport.m_Offre);

		// Reconstruit la table des items interdits
		m_DocChoixSupport.m_Offre.ConstructionItemsInterdits();

		// Mise à jour de l'offre dans toutes les vues
		m_DocChoixSupport.UpdateAllViews(UPDATE_OFFRE);

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate option menu Supports
void CDlgChoixSupport::OnUpdateCsSupports(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid()) 
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);

}

////////////////////////////////////////////////////////////////////////////////////////////////
// Appel de la boite Offres pour choix Cibles
void CDlgChoixSupport::OnCsCibles()
{
	// On vérifie que l'on a bien sélectionné une date de tarification
	if (!m_DocChoixSupport.m_DateTarifaire.IsValid())
	{
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Il faut d'abord choisir une date de tarification" ,"SELECTION SOURCE / TERRAINS ", MB_ICONEXCLAMATION);
		return;
	}

	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude;
	JDate FinEtude;

	// Sauvegarde ancien type de poids
	JUnt32 ExIdTypePoids = m_DocChoixSupport.m_Offre.m_IdTypePoids; 

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre courante", m_CATPColorsApp, 
		m_DocChoixSupport.m_Offre, COffre2MainDialog::eodoCibles,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this, false))
	{
		// Redessine fenêtres en cours
		m_pActivePage->UpdateWindow();
		this->UpdateWindow(); 

		// Si éléments modifiés, on récupère les nouvelles infos
		if(bOffreModified)
		{
			// Repositionne les flag params calcul
			ParamCalculModif();

			// Le document a changé
			m_DocChoixSupport.SetModifiedFlag(TRUE); 
		}

		// TODO : changer ICI
		// TODO : changer ICI
		// TODO : changer ICI
		// en attendant de refaire la fonction RestreincreOffre
		CDialogChoixOffre oldDlg(this);
		oldDlg.SetOffre(m_DocChoixSupport.m_Offre);
		oldDlg.GetOffreRestreinte(m_DocChoixSupport.m_Offre); 

		// Reconstruit la table des items interdits
		m_DocChoixSupport.m_Offre.ConstructionItemsInterdits();

		if (m_DocChoixSupport.m_Offre.m_IdTypePoids == ExIdTypePoids) 
		{
			// Mise à jour globale sans changement de type de poids
			m_DocChoixSupport.UpdateAllViews(UPDATE_OFFRE);
		}
		else
		{
			// Mise à jour globale avec changement de type de poids (comme initialisation général)
			m_DocChoixSupport.UpdateAllViews(UPDATE_INITIAL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate option menu Cibles
void CDlgChoixSupport::OnUpdateCsCibles(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid()) 
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);}

///////////////////////////////////////////////////////////////////////////////////////////////
// Appel de la boite Offres pour choix des Formats
void CDlgChoixSupport::OnCsFormats()
{
	// On vérifie que l'on a bien sélectionné une date de tarification
	if (!m_DocChoixSupport.m_DateTarifaire.IsValid())
	{
		HWND hHwnd = NULL;
		CWnd *phParent = this->GetParent(); if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
		::MessageBox(hHwnd, "Il faut d'abord choisir une date de tarification" ,"SELECTION SOURCE / TERRAINS ", MB_ICONEXCLAMATION);
		return;
	}

	bool bDateChanged, bOffreModified;

	// on recopie les dates de l'etude
	JDate DebEtude;
	JDate FinEtude;

	// Sauvegarde ancien type de poids
	JUnt32 ExIdTypePoids = m_DocChoixSupport.m_Offre.m_IdTypePoids; 

	// on ouvre le dialogue de modification de l'offre
	if (OpenOffreDialog("Modification de l'offre courante", m_CATPColorsApp, 
		m_DocChoixSupport.m_Offre, COffre2MainDialog::eodoFormats,
		DebEtude, FinEtude, bDateChanged, bOffreModified, true, this, false))
	{
		// Redessine fenêtres en cours
		m_pActivePage->UpdateWindow();
		this->UpdateWindow(); 

		// Si éléments modifiés, on récupère les nouvelles infos
		if(bOffreModified)
		{
			// Repositionne les flag params calcul
			ParamCalculModif();

			// Le document a changé
			m_DocChoixSupport.SetModifiedFlag(TRUE); 
		}

		// TODO : changer ICI, redevelopper la fonction RestreincreOffre
		// TODO : changer ICI, redevelopper la fonction RestreincreOffre
		// TODO : changer ICI, redevelopper la fonction RestreincreOffre
		// en attendant de refaire la fonction RestreincreOffre
		CDialogChoixOffre oldDlg(this);
		oldDlg.SetOffre(m_DocChoixSupport.m_Offre);
		oldDlg.GetOffreRestreinte(m_DocChoixSupport.m_Offre); 

		// Reconstruit la table des items interdits
		m_DocChoixSupport.m_Offre.ConstructionItemsInterdits();

		if (m_DocChoixSupport.m_Offre.m_IdTypePoids == ExIdTypePoids) 
		{
			// Mise à jour globale sans changement de type de poids
			m_DocChoixSupport.UpdateAllViews(UPDATE_OFFRE);
		}
		else
		{
			// Mise à jour globale avec changement de type de poids (comme initialisation général)
			m_DocChoixSupport.UpdateAllViews(UPDATE_INITIAL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate option menu Formats
void CDlgChoixSupport::OnUpdateCsFormats(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid()) 
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//  Choix de la période tarifaire
void CDlgChoixSupport::OnCsPeriodeTarif()
{
	// voir si document valide
	if (m_DocChoixSupport.IsValid())
	{
		// La boite choix date tarifaire
		CDlgDateTarif Dlg(this);
		Dlg.SetColors(CATPColors::COLORCHOIXSUPPORTS);

		// Initialisation date
		if (m_DocChoixSupport.m_DateTarifaire.IsValid()) 
			Dlg.SetDateTar(m_DocChoixSupport.m_DateTarifaire);

		// Ouverture Sélection Boite de dialogue Date Tarifaire
		if( Dlg.DoModal())
		{
			// Récuperation nouvelle date tarifaire
			JDate NewDateTar; 
			NewDateTar = Dlg.GetDateTar(); 

			// Test si la date a changée
			if (NewDateTar != m_DocChoixSupport.m_DateTarifaire)
			{
				// Sauve nouvelle date
				m_DocChoixSupport.m_DateTarifaire		 = NewDateTar;
				m_DocChoixSupport.m_KEYPlateau.SetDateTar(NewDateTar); 

				// Le document a changé
				m_DocChoixSupport.SetModifiedFlag(TRUE); 

				// Update résultats dépendant de la nouvelle date tarifaire
				m_DocChoixSupport.UpdateAllViews(UPDATE_DATE_TARIFAIRE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
void CDlgChoixSupport::OnUpdateCsPeriodetarif(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid()) 
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);

}


//////////////////////////////////////////////////////////////////////////////////////////////
//						UPDATE SOUS MENU CHOIX SUPPORTS
//
void CDlgChoixSupport::OnUpdateCsPlateauOuvrir(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable(TRUE);
}

void CDlgChoixSupport::OnUpdateCsPlateauEnregistrer(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_DocChoixSupport.IsValid()) ? TRUE : FALSE);
}

void CDlgChoixSupport::OnUpdateCsPlateauEnregistrersous(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_DocChoixSupport.IsValid()) ? TRUE : FALSE);
}

void CDlgChoixSupport::OnUpdateCsPlateauFermer(CCmdUI *pCmdUI)
{
	// on modifie l'état du menu
	pCmdUI->Enable((m_DocChoixSupport.IsValid()) ? TRUE : FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//							GESTION DES DIFFERENTES PAGES
//
// Subclass de tous les elements de la fenêtre
void CDlgChoixSupport::SetupControls()
{
	// Les boutons pages
	m_BtnEchelle.SubclassDlgItem(IDC_CS_ECHELLES, this);
	m_BtnMapping.SubclassDlgItem(IDC_CS_MAPPING, this);
	m_BtnCumulativite.SubclassDlgItem(IDC_CS_CUMULATIVITE, this);
	m_BtnDuplication.SubclassDlgItem(IDC_CS_DUPLICATION, this);

	// Bouton passerelle creuset
	m_BtnCreuset.SubclassDlgItem(IDC_CS_LABEL_CREUSET, this);

	// Style Label Terrain
	m_LblTerrains.SetFontName(CATPColors::GetFontLabelName());
	m_LblTerrains.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblTerrains.SetFontBold(true); 
	m_LblTerrains.SetBorder(false);

	// Style Label Cibles
	m_LblCibles.SetFontName(CATPColors::GetFontLabelName());
	m_LblCibles.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblCibles.SetBorder(false);

	// Style Label Segment Pop.
	m_LblSegPop.SetFontName(CATPColors::GetFontLabelName());
	m_LblSegPop.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblSegPop.SetBorder(false);

	// Style Label Ref Affinité
	m_LblRefAff.SetFontName(CATPColors::GetFontLabelName());
	m_LblRefAff.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblRefAff.SetBorder(false);

	// Style Label Format
	m_LblFormats.SetFontName(CATPColors::GetFontLabelName());
	m_LblFormats.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblFormats.SetBorder(false);

	// Style Label Niveau Affichage
	m_LblNiveau.SetFontName(CATPColors::GetFontLabelName());
	m_LblNiveau.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblNiveau.SetBorder(false);

	// Définition des couleurs contrôles
	SetColors();

	// Fixe relations  fenêtres et position resize
	SetWindowRelations();

	// Mise à jour des sélecteurs
	UpdateSelecteur();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Positionnement de la page active sur main page
void CDlgChoixSupport::UpdateDlgPos(BOOL b_show/*=FALSE*/) 
{
	// Récupération position et coordonnées fenêtre de base Choix Supports 
	CRect rect;
	GetDlgItem(IDC_CS_CADRE_RESULT)->GetWindowRect(&rect);
	POINT pt1, pt2;

	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;

	// Positionnement de la fenêtre active
	if (m_pActivePage)
			m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER);

	// Rend visible la fenêtre active
	if (b_show)
	{
		m_pActivePupitre->ShowWindow(SW_SHOW);
		m_pActivePage->ShowWindow(SW_SHOW);
	}

	// Récupération coordonnées zone pupitre
	GetDlgItem(IDC_CS_CADRE_SEL2)->GetWindowRect(&rect);
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;

	// Positionnement de la fenêtre pupitre active
	if (m_pActivePupitre)
	{
		m_pActivePupitre->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER );
		m_pActivePupitre->RedrawWindow(); 
	}

	// Chargement liste grandeurs pour résultats en colonne échelle
	m_pPupitreEch->LoadGrandeurs(); 

	// Chargement liste grandeurs pour résultats en colonne mapping
    m_pPupitreMap->LoadGrandeurs(); 

	// Chargement liste grandeurs pour résultats cumulativité
	m_pPupitreCum->LoadGrandeurs(); 

	// Chargement liste grandeurs pour résultats duplication
	// m_pPupitreDup->LoadGrandeurs(); 
	
}


// Définition des couleurs pour chaque objet
void CDlgChoixSupport::SetColors()
{
	// Eléments sélection Terrains
	m_LblTerrains.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblTerrains.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblTerrains.SetFontBold(true);
	
	// Eléments sélection Cibles
	m_LblCibles.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblCibles.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblCibles.SetFontBold(true);

	// Eléments sélection Seg Population
	m_LblSegPop.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblSegPop.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblSegPop.SetFontBold(true);

	// Eléments sélection Ref Affinité
	m_LblRefAff.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblRefAff.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblRefAff.SetFontBold(true);

	// Elément sélection Formats
	m_LblFormats.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblFormats.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblFormats.SetFontBold(true);

	// Elément sélection Niveau
	m_LblNiveau.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblNiveau.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));
	m_LblNiveau.SetFontBold(true);

	// Elément mode affichage familles
	m_BtnFamilles.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnFamilles.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );
	m_BtnFamilles.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );

	// Elément mode affichage supports
	m_BtnSupports.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS );
	m_BtnSupports.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnSupports.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );

	// Elément sélection Affichage Boite de dialogue Filtre d'audience
	m_BtnFiltreAudience.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnFiltreAudience.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnFiltreAudience.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);


	// Elément sélection Affichage Info Supports
	/* Reporter en option de menu pop pup support
	m_BtnFicheSupport.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) );
	m_BtnFicheSupport.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) );
	m_BtnFicheSupport.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) );
	*/

	// Elément passerelle Creuset
	/* En attente
	m_BtnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) );
	m_BtnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) );
	m_BtnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) );
	*/

	// Bouton activation Echelle
	m_BtnEchelle.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );
	m_BtnEchelle.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );
	m_BtnEchelle.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Bouton activation Mapping
	m_BtnMapping.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );
	m_BtnMapping.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnMapping.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Bouton activation Cumulativité
	m_BtnCumulativite.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnCumulativite.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );
	m_BtnCumulativite.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS );

	// Bouton activation Duplication
	m_BtnDuplication.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnDuplication.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnDuplication.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Controle cadres
	m_CadreSelection.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);
	m_CadreBtnPupitre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);
	m_CadreOnglet.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);

	// Couleur sélection dans listes  terrains / cibles / réf affinité / grandeurs
	m_LstTerrains.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	m_LstCibles.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	
	// Couleur sélection segments population
	m_LstSegPop.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	m_LstRefAff.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	m_LstFormats.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Détermine les relations entre objets
void CDlgChoixSupport::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

		// Cadre1 de base
		{IDC_CS_CADRE_RESULT,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						// Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Cadre Onglet Hrizontal englobant les sélections Echelles/Mapping/Cum/Dup
		{IDC_CS_CADRE_ONGLET,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Bouton sélection page échelle
		{IDC_CS_ECHELLES,			{CResizer::eFixed, IDC_CS_CADRE_RESULT, CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eProportional, IDC_CS_CADRE_RESULT, CResizer::eRight},	// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Bouton sélection page mapping
		{IDC_CS_MAPPING,			{CResizer::eFixed, IDC_CS_ECHELLES, CResizer::eRight},				// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eProportional, IDC_CS_CADRE_RESULT, CResizer::eRight},	// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Bouton sélection page cumulativité
		{IDC_CS_CUMULATIVITE,		{CResizer::eFixed, IDC_CS_MAPPING, CResizer::eRight},				// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eProportional, IDC_CS_CADRE_RESULT, CResizer::eRight},	// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Bouton sélection page duplication
		{IDC_CS_DUPLICATION,		{CResizer::eFixed, IDC_CS_CUMULATIVITE, CResizer::eRight},			// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eProportional, IDC_CS_CADRE_RESULT, CResizer::eRight},	// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side


		// Cadre englobant les sélections Terrains /Cibles / Formats
		{IDC_CS_CADRE_SEL1,			{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						// Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},						// Bottom side

		// Label et liste Terrains
		{IDC_CS_LABEL_TERRAINS,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eTop},				// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eTop}},				// Bottom side

		{IDC_CS_LIST_TERRAINS,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eTop},				// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eTop}},				// Bottom side

		// Label et Liste Cibles
		{IDC_CS_LABEL_CIBLES,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LIST_TERRAINS, CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LIST_TERRAINS, CResizer::eBottom}},		// Bottom side

		{IDC_CS_LIST_CIBLES,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LABEL_CIBLES,  CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_CIBLES,  CResizer::eBottom}},		// Bottom side

		// Label et Liste Seg. Population
		{IDC_CS_LABEL_SEGPOP,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LIST_CIBLES,   CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LIST_CIBLES,   CResizer::eBottom}},		// Bottom side

		{IDC_CS_LIST_SEGPOP,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LABEL_SEGPOP,  CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_SEGPOP,  CResizer::eBottom}},		// Bottom side

		// Label et Liste Reference affinité
		{IDC_CS_LABEL_REFAFF,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LIST_SEGPOP,   CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LIST_SEGPOP,   CResizer::eBottom}},		// Bottom side

		{IDC_CS_LIST_REFAFF,		{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eLeft},			// Left side
									{CResizer::eFixed, IDC_CS_LABEL_REFAFF,  CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1,    CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_REFAFF,  CResizer::eBottom}},		// Bottom side

		// Label et Liste Formats
		{IDC_CS_LABEL_FORMATS,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_LIST_CIBLES, CResizer::eBottom},			// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LIST_CIBLES, CResizer::eBottom}},			// Bottom side

		{IDC_CS_LIST_FORMATS,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_LABEL_FORMATS, CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_FORMATS, CResizer::eBottom}},		// Bottom side

		// LAbel et boutons mode affichage
		{IDC_CS_LABEL_NIVEAU,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_LIST_FORMATS, CResizer::eBottom},			// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LIST_FORMATS, CResizer::eBottom}},		// Bottom side

		{IDC_CS_LABEL_FAMILLES,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_LABEL_NIVEAU, CResizer::eBottom},			// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_NIVEAU, CResizer::eBottom}},		// Bottom side

		{IDC_CS_LABEL_SUPPORTS,		{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eLeft},				// Left side
									{CResizer::eFixed, IDC_CS_LABEL_FAMILLES, CResizer::eBottom},		// Top side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eRight},			// Right side
									{CResizer::eFixed, IDC_CS_LABEL_FAMILLES, CResizer::eBottom}},		// Bottom side

		// Cadre 2eme zone objets différents par page
		{IDC_CS_CADRE_SEL2,			{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Left side
									{CResizer::eFixed, IDC_CS_CADRE_SEL1, CResizer::eBottom},			// Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Cadre fixe pour les boutons pupitre
		{IDC_CS_CADRE_BTNFIXE,		{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Left side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},					// Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						// Right side
									{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					// Bottom side

		// Bouton sélection filtre d'audience
		{IDC_CS_LABEL_FILTRE_AUDIENCE,	{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom}},			//Bottom side

		// Bouton sélection contexte de lecture
		{IDC_CS_LABEL_CONTEXTE_LECT,	{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom}},			//Bottom side

		/* Remplacer par une option menu pop pup sur supports
		// Bouton sélection affichage détaillé support
		{IDC_CS_LABEL_FICHESUPPORT,	{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Left side
									{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom},			//Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
									{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eBottom}},			//Bottom side
		*/

		/* En attente
		// Bouton sélection passerelle Creuset
		{IDC_CS_LABEL_CREUSET,		{CResizer::eFixed, IDC_CS_CADRE_SEL2, CResizer::eLeft},				//Left side
									{CResizer::eFixed, IDC_CS_LABEL_FICHESUPPORT, CResizer::eBottom},	//Top side
									{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
									{CResizer::eFixed, IDC_CS_LABEL_FICHESUPPORT, CResizer::eBottom}},	//Bottom side
		*/
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resize.Init(m_hWnd, NULL, s_bi, nSize);

}



//////////////////////////////////////////////////////////////////////////////////////// 
// Gestion Ss-menu couleurs interface
////////////////////////////////////////////////////////////////////////////////////////////

void CDlgChoixSupport::OnCsPalette1()
{
	// Interface couleur Vert Bouteille
	InitCouleurCs(0);
}

void CDlgChoixSupport::OnCsPalette2()
{
	// Interface couleur Emeraude
	InitCouleurCs(1);
}

void CDlgChoixSupport::OnCsPalette3()
{
	// Interface couleur Bleu Pastel
	InitCouleurCs(2);
}

void CDlgChoixSupport::OnCsPalette4()
{
	// Interface couleur Ocean
	InitCouleurCs(3);
}

void CDlgChoixSupport::OnCsPalette5()
{
	// Interface couleur Lilas
	InitCouleurCs(4);
}

void CDlgChoixSupport::OnCsPalette6()
{
	// Interface couleur Sahara
	InitCouleurCs(5);
}

void CDlgChoixSupport::OnCsPalette7()
{
	// Interface couleur Terre de Sienne
	InitCouleurCs(6);
}

void CDlgChoixSupport::OnCsPalette8()
{
	// Interface couleur Rubis
	InitCouleurCs(7);
}

void CDlgChoixSupport::OnCsPalette9()
{
	// Interface couleur Gris souris
	InitCouleurCs(8);
}

void CDlgChoixSupport::OnCsPalette10()
{
	// Interface couleur Rose
	InitCouleurCs(9);
}

void CDlgChoixSupport::OnCsPalette11()
{
	// Interface couleur Rose kaki
	InitCouleurCs(10);
}

void CDlgChoixSupport::OnCsPalette12()
{
	// Interface couleur Rose gris
	InitCouleurCs(11);
}

void CDlgChoixSupport::OnCsPalette13()
{
	// Interface couleur Sable
	InitCouleurCs(12);
}

void CDlgChoixSupport::OnCsPalette14()
{
	// Interface couleur CafeChocolat
	InitCouleurCs(13);
}

void CDlgChoixSupport::OnCsPalette15()
{
	// Interface couleur CafeChocolat
	InitCouleurCs(14);
}


/////////////////////////////////////////////////////////////////////////////////////
// Redéfinition des couleurs interface Choix Support
void CDlgChoixSupport::InitCouleurCs(JUnt32 IndiceCouleur)
{
	// Définition de la nouvelle couleur
	CATPColors::SetAppColor(CATPColors::COLORCHOIXSUPPORTS, IndiceCouleur);
	SetColors();

	// Pour les boites dialogues externes
	m_DocChoixSupport.UpdateAllViews(UPDATE_COLOR);  

	// Définit la couleur pour toutes les autres pages
	m_pPageEchelle->SetColors();
	m_pPageMapping->SetColors();
	m_pPageCumulativite->SetColors();
	m_pPageDuplication->SetColors();

	// Définit la couleur pour toutes les autres pupitres pages
	m_pPupitreEch->SetColors();
	m_pPupitreMap->SetColors();
	m_pPupitreCum->SetColors();
	m_pPupitreDup->SetColors();

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	if (m_DocChoixSupport.GetNbSupports() > 0)
	{
		// Ruse pour remettre les couleurs de sélection du grid
		UpdateResultat(false);
		m_DocChoixSupport.UpdateAllViews(UPDATE_SEL_SUPPORTS); 
	}

	Invalidate();
}


////////////////////////////////// GESTION PAGES CHOIX SUPPORT ////////////////////////////
// Affichage de la page Echelles
void CDlgChoixSupport::OnBnClickedCsEchelles()
{
	// Nouvel index module courant
	m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleEch);  
	
	// Rend inactif les autres boutons onglets
	m_BtnMapping.EnableWindow(FALSE);
	m_BtnCumulativite.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(FALSE);

	// Validation bouton Echelle
	CheckRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION, IDC_CS_ECHELLES);
	
	// Activation de la page échelle et de son pupitre
	if (m_pActivePage && m_pActivePupitre)
	{
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);
	}
	m_pActivePage    = m_pPageEchelle;
	m_pActivePupitre = m_pPupitreEch;
	/*
	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);
	*/

	// Mise en place des objets échelles
	UpdateDlgPos(TRUE);	

	// Mise à jour fenetre fille
	if (m_DocChoixSupport.m_ParamEchModif == true && m_DocChoixSupport.GetNbSupports() > 0) 
	{
		// Mise à jour des segments courant et positionnement moteur
		m_DocChoixSupport.UpdateOffreMoteur();

		// Attention mise à jour des formats pour les nouveaux supports
		UpdateModuleFormatSupports(0,m_DocChoixSupport.ModuleEch);
		m_pPageEchelle->UpdateSelFormatSupports();

		// Filtrage via les segments du module
		// m_DocChoixSupport.FiltreSupportsSegments(); 		
		m_DocChoixSupport.SetSupportsSegmentsVisible();

		// Visu Supports
		m_pPageEchelle->UpdateVisuSupports(); 

		// Mise à jour fenêtre échelle
		m_pPageEchelle->UpdateResultEchelle(true);
	}

	if (m_BtnEchelle.IsWindowEnabled())
	{
		// Activation des listes en multi-sélection
		m_LstTerrains.ModifyStyle(NULL, LBS_EXTENDEDSEL | WS_VSCROLL);
		m_LstTerrains.RecreateListBox();
		m_LstTerrains.ShowScrollBar(SB_VERT,TRUE);
		m_LstCibles.ModifyStyle(NULL, LBS_EXTENDEDSEL | WS_VSCROLL);     
		m_LstCibles.RecreateListBox();
		m_LstCibles.ShowScrollBar(SB_VERT,TRUE);
				
		// Mise à jour titre fenêtre principale
		MajTitreFenetreCS();

		// En attente de mise à jour
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);

		// Valide les menus indispensables
		MajSelFenetreCS();

		// Mise à jour des résultats associés (si au moins 1 support analysé)
		if (m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) 
			UpdateResultat(m_DocChoixSupport.m_ParamEchModif);
		m_DocChoixSupport.m_ParamEchModif = false;

		// Valide les menus indispensables (pour échelle de 1 * 10 insertions)
		OnValideSsMenuNbInsertion(10);
	}

	// Remet tous les boutons actifs
	m_BtnMapping.EnableWindow(TRUE);
	m_BtnCumulativite.EnableWindow(TRUE);

	// Pour le moment Duplication non active
	//m_BtnDuplication.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(TRUE);

	// Mise à jour boutons sélections "Familles/Supports"
	UpdateBtnModeSelSupports();

	// Remet la liste des formats accessibles
	m_LstFormats.EnableWindow(TRUE); 

	// En attente de mise à jour
	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);
}

// Affichage de la page Mapping
void CDlgChoixSupport::OnBnClickedCsMapping()
{
	// Nouvel index module courant
	m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleMap);  

	// Rend inactif les autres boutons onglets
	m_BtnEchelle.EnableWindow(FALSE); 
	m_BtnCumulativite.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(FALSE);
	
	// Validation bouton Mapping
	CheckRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION, IDC_CS_MAPPING);
	
	// Activation de la page Mapping et de son pupitre
	if (m_pActivePage && m_pActivePupitre)
	{
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);
	}
	m_pActivePage		= m_pPageMapping;
	m_pActivePupitre	= m_pPupitreMap;
	/*
	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);
	*/
	
	// Mise en place des objets Mapping
	UpdateDlgPos(TRUE);
	
	// Mise à jour fenetre fille
	if (m_DocChoixSupport.m_ParamMapModif == true) 
	{
		// Mise à jour des segments courant et positionnement moteur
		m_DocChoixSupport.UpdateOffreMoteur();

		// Attention mise à jour des formats pour les nouveaux supports
		UpdateModuleFormatSupports(0,m_DocChoixSupport.ModuleMap);
		m_pPageMapping->UpdateSelFormatSupports();

		// Filtrage via les segments du module
		// m_DocChoixSupport.FiltreSupportsSegments(); 		
		m_DocChoixSupport.SetSupportsSegmentsVisible();

		// Visu Supports
		m_pPageMapping->UpdateVisuSupports(); 

		// Mise à jour fenêtre mapping
		m_pPageMapping->UpdateResultMapping(true);
	}

	// Activation des listes en mono sélection
	m_LstTerrains.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);
	m_LstTerrains.RecreateListBox();
	m_LstTerrains.ShowScrollBar(SB_VERT,TRUE);
	m_LstCibles.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);     
	m_LstCibles.RecreateListBox();
	m_LstCibles.ShowScrollBar(SB_VERT,TRUE);

	// Mise à jour titre fenêtre principale
	MajTitreFenetreCS();

	// Valide les menus indispensables
	m_pActivePupitre->ShowWindow(SW_HIDE);
	m_pActivePage->ShowWindow(SW_HIDE);
	MajSelFenetreCS();
	
	// Mise à jour des résultats associés (si au moins 1 support)
	if (m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) 
		UpdateResultat(m_DocChoixSupport.m_ParamMapModif);

	// Init modif pour module mapping
	m_DocChoixSupport.m_ParamMapModif = false;

	// Dévalide les autres
	OnInvalideSsMenuNbInsertion();

	// Remet tous les boutons actifs
	m_BtnEchelle.EnableWindow(TRUE); 
	m_BtnCumulativite.EnableWindow(TRUE);

	// Pour le moment Duplication non active
	m_BtnDuplication.EnableWindow(TRUE);

	// Mise à jour boutons sélections "Familles/Supports"
	UpdateBtnModeSelSupports();

	// Remet la liste des formats accessibles
	m_LstFormats.EnableWindow(TRUE); 

	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);
}

// Affichage de la page Cumulativité
void CDlgChoixSupport::OnBnClickedCsCumulativite()
{
	// Nouvel index module courant
	m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleCum);  

	// Rend inactif les autres boutons onglets
	m_BtnEchelle.EnableWindow(FALSE); 
	m_BtnMapping.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(FALSE);

	// Validation bouton Cumulativité
	CheckRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION, IDC_CS_CUMULATIVITE);

	// Activation de la page Cumulativité et de son pupitre
	if (m_pActivePage && m_pActivePupitre)
	{
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);
	}

	// Visu page cumulativité
	m_pActivePage		= m_pPageCumulativite;
	m_pActivePupitre	= m_pPupitreCum;
	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);

	// Mise en place des objets Cumulativité
	UpdateDlgPos(TRUE);

	// Mise à jour fenetre fille
	if (m_DocChoixSupport.m_ParamCumModif == true) 
	{
		// Attention cumulativité ne prendra que le moteur ABAQUE_ETAL
		if (m_DocChoixSupport.m_MoteurPresse.GetType() !=  m_DocChoixSupport.m_MoteurPresse.PROBA)
		{
			// Repositionne le type moteur
			m_DocChoixSupport.m_MoteurPresse.SetType(m_DocChoixSupport.m_MoteurPresse.PROBA);

			// Prévient tous les autres modules d'un changement de type moteur
			m_DocChoixSupport.m_ParamEchModif = true;
			m_DocChoixSupport.m_ParamMapModif = true;
			m_DocChoixSupport.m_ParamDupModif = true;

			// Vire les maps résultats (cache résultat)
			// En fait A FAIRE il faudrait un moteur indépendant ou bien sauvé le type moteur dans le cache résultat 
			// de chaque module
			/* A VOIR 
			m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleEch);
			m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleMap);
			*/

			// Appel mise à jour type moteur
			m_DocChoixSupport.UpdateAllViews(UPDATE_TYPEMOTEUR);
		}
		
		// Mise à jour des segments courant et positionnement moteur
		m_DocChoixSupport.UpdateOffreMoteur();

		// Attention mise à jour des formats pour les nouveaux supports
		UpdateModuleFormatSupports(0,m_DocChoixSupport.ModuleCum);
		m_pPageCumulativite->UpdateSelFormatSupports();

		// Filtrage via les segments du module
		// m_DocChoixSupport.FiltreSupportsSegments(); 	
		m_DocChoixSupport.SetSupportsSegmentsVisible();

		// Visu Supports
		m_pPageCumulativite->UpdateVisuSupports(); 

		// Mise à jour fenêtre cumulativité
		m_pPageCumulativite->UpdateResultCum(true);
	}

	// Activation des listes en mono sélection
	m_LstTerrains.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);
	m_LstTerrains.RecreateListBox();
	m_LstTerrains.ShowScrollBar(SB_VERT,TRUE);
	m_LstCibles.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);     
	m_LstCibles.RecreateListBox();
	m_LstCibles.ShowScrollBar(SB_VERT,TRUE);

	// Mise à jour titre fenêtre principale
	MajTitreFenetreCS();
	
	// Mise à jour des résultats associés (si au moins 1 support)
	if (m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) 
		UpdateResultat(m_DocChoixSupport.m_ParamCumModif);

	// Init modif pour module cumulativité
	m_DocChoixSupport.m_ParamCumModif = false;

	// Valide les menus indispensables
	MajSelFenetreCS();

	// Dévalide les autres
	// OnInvalideSsMenuNbInsertion();
	OnValideSsMenuNbInsertion(1000);

	// Remet tous les boutons actifs
	m_BtnEchelle.EnableWindow(TRUE); 
	m_BtnMapping.EnableWindow(TRUE);

	// Pour le moment Duplication non active
	//m_BtnDuplication.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(TRUE);

	// Mise à jour boutons sélections "Familles/Supports"
	UpdateBtnModeSelSupports();

	// Liste des formats innaccessibles
	m_LstFormats.EnableWindow(FALSE); 
}

// Affichage de la page Duplication
void CDlgChoixSupport::OnBnClickedCsDuplication()
{
	// Nouvel index module courant
	m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleDup);  
	
	// Rend inactif les autres boutons onglets
	m_BtnEchelle.EnableWindow(FALSE); 
	m_BtnMapping.EnableWindow(FALSE);
	m_BtnDuplication.EnableWindow(FALSE);
	
	// Validation bouton Duplication
	CheckRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION, IDC_CS_DUPLICATION);
	
	// Activation de la page Duplication et de son pupitre
	if (m_pActivePage && m_pActivePupitre)
	{
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);
	}

	// Visu page duplication
	m_pActivePage		= m_pPageDuplication;
	m_pActivePupitre	= m_pPupitreDup;
	m_pActivePupitre->ShowWindow(SW_SHOW);
	m_pActivePage->ShowWindow(SW_SHOW);

	// Mise en place des objets Cumulativité
	UpdateDlgPos(TRUE);

	// Mise à jour fenetre fille
	if (m_DocChoixSupport.m_ParamDupModif == true) 
	{
		// Attention duplication ne prendra que le moteur ABAQUE_ETAL
		// En fait A FAIRE il faudrait un moteur indépendant ou bien sauvé le type moteur dans le cache résultat 
		// de chaque module
		if (m_DocChoixSupport.m_MoteurPresse.GetType() !=  m_DocChoixSupport.m_MoteurPresse.PROBA)
		{
			// Repositionne le type moteur
			m_DocChoixSupport.m_MoteurPresse.SetType(m_DocChoixSupport.m_MoteurPresse.PROBA);

			// Prévient tous les autres modules d'un changement de type moteur
			m_DocChoixSupport.m_ParamEchModif = true;
			m_DocChoixSupport.m_ParamMapModif = true;
			m_DocChoixSupport.m_ParamCumModif = true;

			// Vire les maps résultats (cache résultat)
			/* A VOIR
			m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleEch);
			m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleMap);
			*/

			// Appel mise à jour type moteur
			m_DocChoixSupport.UpdateAllViews(UPDATE_TYPEMOTEUR);
		}

		// Mise à jour des segments courant et positionnement moteur
		m_DocChoixSupport.UpdateOffreMoteur();

		// Attention mise à jour des formats pour les nouveaux supports
		UpdateModuleFormatSupports(0,m_DocChoixSupport.ModuleDup);
		m_pPageDuplication->UpdateSelFormatSupports();

		// Filtrage via les segments du module
		// m_DocChoixSupport.FiltreSupportsSegments(); 	
		m_DocChoixSupport.SetSupportsSegmentsVisible();

		// Visu Supports
		m_pPageDuplication->UpdateVisuSupports(); 

		// Mise à jour fenêtre duplication
		m_pPageDuplication->UpdateResultDup(true);
	}


	// Rend inactif les autres boutons onglets
	m_BtnEchelle.EnableWindow(FALSE); 
	m_BtnMapping.EnableWindow(FALSE);
	m_BtnCumulativite.EnableWindow(FALSE);

	// Nouvel index module courant
	// m_DocChoixSupport.SetModuleCourant(m_DocChoixSupport.ModuleDup);  

	// Validation bouton Duplication
	CheckRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION, IDC_CS_DUPLICATION);

	// Activation de la page Duplication et de son pupitre
	if (m_pActivePage && m_pActivePupitre)
	{
		m_pActivePupitre->ShowWindow(SW_HIDE);
		m_pActivePage->ShowWindow(SW_HIDE);
	}
	m_pActivePage		= m_pPageDuplication;
	m_pActivePupitre	= m_pPupitreDup;

	// Mise en place des objets Duplication
	UpdateDlgPos(TRUE);

	// Activation des listes en mono sélection
	m_LstTerrains.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);
	m_LstTerrains.RecreateListBox();
	m_LstTerrains.ShowScrollBar(SB_VERT,TRUE);
	m_LstCibles.ModifyStyle(LBS_EXTENDEDSEL, WS_VSCROLL);     
	m_LstCibles.RecreateListBox();
	m_LstCibles.ShowScrollBar(SB_VERT,TRUE);

	// Mise à jour titre fenêtre Duplication
	MajTitreFenetreCS();

	// Valide les menus indispensables
	MajSelFenetreCS();

	// A VOIR   @@@@@@@
	// Mise à jour des résultats associés (si au moins 1 support)
	if (m_DocChoixSupport.m_MapIdSupportsSel.GetCount() && m_DocChoixSupport.m_ParamDupModif) 
		UpdateResultat(m_DocChoixSupport.m_ParamDupModif);

	// Init modif pour module duplication
	m_DocChoixSupport.m_ParamDupModif = false;

	// Dévalide les autres
	OnInvalideSsMenuNbInsertion();

	// Remet tous les boutons actifs
	m_BtnEchelle.EnableWindow(TRUE); 
	m_BtnMapping.EnableWindow(TRUE);
	m_BtnCumulativite.EnableWindow(TRUE);

	// Mise à jour boutons sélections "Familles/Supports"
	UpdateBtnModeSelSupports();

	// Liste des formats innaccessibles
	m_LstFormats.EnableWindow(FALSE); 
}

// Resize les différents contrôles de la fenêtre
void CDlgChoixSupport::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);

	// Resize les éléments
	m_Resize.Move();

	// Mise à jour des fenêtres associées
	if (m_pActivePage && m_pActivePupitre)
		UpdateDlgPos(TRUE);
}

// Taille minimale de la fenêtre resizée
void CDlgChoixSupport::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 600;
	lpMMI->ptMinTrackSize.y = 550;
	JFCDialog::OnGetMinMaxInfo(lpMMI);
}

// Fermeture boite de dialogue Choix Supports
void CDlgChoixSupport::OnClose()
{
	// on teste la validité du document choix supports courant
	if(this->CanClose())
	{
		// on teste la validité du document choix supports courant
		if (m_DocChoixSupport.IsValid())
		{
			// Sauvegarde ou non du document en cours...........
			SavePlateauViaQuestion();
		}

		// Efface la boite filtre audience si visible
		m_DocChoixSupport.m_AFFFiltreAudience  = false; 

		m_DlgFiltresAudience.Update();

		JFCDialog::OnClose();
	}
}



// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône. Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.
void CDlgChoixSupport::OnPaint()
{
	if (IsIconic())
	{
		// contexte de périphérique pour la peinture
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'icône dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'icône
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		JFCDialog::OnPaint();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection fenêtre pour l'ordre des entetes grille résultat
void CDlgChoixSupport::OnCsOrdreEntetes()
{
	// voir si document valide
	if (m_DocChoixSupport.IsValid())
	{
		// Création objet fenêtre dialog ordre headers
		CDlgOrdreHeader DlgOrdre(this);

		// Initialisation ordre
		DlgOrdre.m_OrdreEntete.SetSize(m_DocChoixSupport.m_OrdreEnteteEch.GetCount());
		for (JUnt32 i = 0; i < m_DocChoixSupport.m_OrdreEnteteEch.GetCount(); i++)
			DlgOrdre.m_OrdreEntete[i] = m_DocChoixSupport.m_OrdreEnteteEch[i];

		// Ouverture Sélection Boite de dialogue
		if(DlgOrdre.DoModal() == IDOK)
		{
			// Test si ordre entete a changé
			JBool OrdreChange = false;
			for (JUnt32 i = 0; i < m_DocChoixSupport.m_OrdreEnteteEch.GetCount(); i++)
			{
				if (m_DocChoixSupport.m_OrdreEnteteEch[i] != DlgOrdre.m_OrdreEntete[i])
					OrdreChange = true;
			}

			// Ordre a changé
			if (OrdreChange)
			{
				// Récupération et affichage nouvelle grille
				for (JUnt32 i = 0; i < m_DocChoixSupport.m_OrdreEnteteEch.GetCount(); i++)
				{
					m_DocChoixSupport.m_OrdreEnteteEch[i] = DlgOrdre.m_OrdreEntete[i];
				}

				// Le document a changé
				m_DocChoixSupport.SetModifiedFlag(TRUE); 

				// Permutation des entetes sur la grille échelle
				m_DocChoixSupport.UpdateAllViews(UPDATE_ORDRE_ENTETE_ECHELLE);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Sélection fenêtre pour l'ordre des entetes grille résultat
void CDlgChoixSupport::OnUpdateCsOrdreEntetes(CCmdUI *pCmdUI)
{
	// Par défaut inactif
	pCmdUI->Enable(FALSE);

	// Voir si fenetre active
	if (m_pActivePage && m_pPageEchelle)
	{
		// Valide si échelle active et au moins 2 entetes headers
		if (m_pActivePage == m_pPageEchelle && m_DocChoixSupport.GetNbSupports() && m_pPageEchelle->GetNbHeaderRow() >= 2)
		{
			pCmdUI->Enable(TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Va dans la boite de dialogue tri des grandeurs échelle à afficher
void CDlgChoixSupport::OnCsOrdreGrandech()
{
	// Création objet fenêtre dialog tri grandeurs échelle
	CDlgOrdreGrandEch DlgTri(this,&m_DocChoixSupport);

	// Initialisation ordre ave liste en cours
	DlgTri.m_LstGrandeurs.Reset();

	JUnt32 NbElmt = m_DocChoixSupport.m_LstOrdoGrandeurEch.GetCount();

	// Ajout des libellés grandeurs disponibles
	for (m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveFirst();
		 m_DocChoixSupport.m_LstOrdoGrandeurEch.IsValid();
		 m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveNext())
	{
		// Attention aux grandeurs spécifiques GRP+ et Répétition+
		DlgTri.m_LstGrandeurs.AddTail() = m_DocChoixSupport.m_LstOrdoGrandeurEch.GetItem(); 
	}
	
	// Ouverture Sélection Boite de dialogue 
	if(DlgTri.DoModal() == IDOK)
	{
		// Test si ordre entete a changé
		JBool OrdreChange = false;

		if (DlgTri.m_LstGrandeurs.GetCount() != m_DocChoixSupport.m_LstOrdoGrandeurEch.GetCount())
			OrdreChange = true;
		else
		{
			// Vérifie sur chacune des listes
			m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveFirst();
			for (DlgTri.m_LstGrandeurs.MoveFirst(); 
				 DlgTri.m_LstGrandeurs.IsValid();
				 DlgTri.m_LstGrandeurs.MoveNext())
			{
				if (DlgTri.m_LstGrandeurs.GetItem() != m_DocChoixSupport.m_LstOrdoGrandeurEch.GetItem())
				{
					OrdreChange = true;
					break;
				}

				// Passe à l'élmt suivant de la liste d'origine
				m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveNext();
			}
		}

		if (OrdreChange)
		{
			// Récupére nouvelle liste triée
			m_DocChoixSupport.m_LstOrdoGrandeurEch.Reset();
			for (DlgTri.m_LstGrandeurs.MoveFirst();
				 DlgTri.m_LstGrandeurs.IsValid();
				 DlgTri.m_LstGrandeurs.MoveNext())
			{
				JUnt32 IdGrandeur = DlgTri.m_LstGrandeurs.GetItem();

				// Pas de GRP + ni de reprise en moteur proba
				if (m_DocChoixSupport.m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				{
					if ((IdGrandeur == m_DocChoixSupport.GrdEch_XGRP)			|| 
						(IdGrandeur == m_DocChoixSupport.GrdEch_XRepetition)) 
					{
						continue;
					}
				}

				// Ajoute la grandeur à la liste des grandeurs disponibles	
				m_DocChoixSupport.m_LstOrdoGrandeurEch.AddTail() = DlgTri.m_LstGrandeurs.GetItem();  
			}

			if (m_DocChoixSupport.m_MoteurPresse.GetType() !=  CMoteurPresse::PROBA)
			{
				bool GRPPlusExist	= false;
				bool RepetPlusExist = false;
				for (DlgTri.m_LstGrandeurs.MoveFirst();
					 DlgTri.m_LstGrandeurs.IsValid();
					 DlgTri.m_LstGrandeurs.MoveNext())
				{
					JUnt32 IdGrandeur = DlgTri.m_LstGrandeurs.GetItem();
					if (IdGrandeur == m_DocChoixSupport.GrdEch_XGRP)
						GRPPlusExist = true;
					if (IdGrandeur == m_DocChoixSupport.GrdEch_XRepetition)
						RepetPlusExist = true;
				}
				if (!GRPPlusExist)
				{
					// Ajoute la grandeur à la liste des grandeurs disponibles	
					m_DocChoixSupport.m_LstOrdoGrandeurEch.AddTail() = m_DocChoixSupport.GrdEch_XGRP; 
				}
				if (!RepetPlusExist)
				{
					// Ajoute la grandeur à la liste des grandeurs disponibles	
					m_DocChoixSupport.m_LstOrdoGrandeurEch.AddTail() = m_DocChoixSupport.GrdEch_XRepetition; 
				}
			}
			
			// Le document a changé
			m_DocChoixSupport.SetModifiedFlag(TRUE); 

			// Permutation des entetes sur la grille échelle
			m_DocChoixSupport.UpdateAllViews(UPDATE_ORDRE_GRANDEUR_ECHELLE);

		}

		// Mise à jour des rangs affichés
		m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RANG_RESULT);

		///////////////////// Sauvegarde dernière liste dans canevas ordre grandeurs par défaut ///////////////////
		//
		// Définition du fichier de sauvegarde (type canevas)
		CString path = AfxGetApp()->GetProfileString("Data", "UserPath", 0);
		CStdioFile FileDefaultCanevas;
		CString StrFile = path + "\\CSEch000.par";

		// On le recré à chaque modif
		FileDefaultCanevas.Open(StrFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

		// Ecrit chapitre param "ordre grandeurs pour échelle"
    	CString Txt;
		Txt.Format("%s\n","// Ordre grandeurs échelle");
		FileDefaultCanevas.WriteString(Txt);

		// Info nb grandeurs
		Txt.Format("%d\n",m_DocChoixSupport.m_LstOrdoGrandeurEch.GetCount());
		FileDefaultCanevas.WriteString(Txt);

		// Balaye nouvel ordre des grandeurs
		for (m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveFirst();
			 m_DocChoixSupport.m_LstOrdoGrandeurEch.IsValid();
			 m_DocChoixSupport.m_LstOrdoGrandeurEch.MoveNext())
		{
			// Récup item
			JUnt32 IdGrandeur = m_DocChoixSupport.m_LstOrdoGrandeurEch.GetItem(); 

			// Format chaine id grandeur
			Txt.Format("%d\n",IdGrandeur);
			FileDefaultCanevas.WriteString(Txt);
		}

		// Fermeture fichier canevas échelle par défaut
		FileDefaultCanevas.Close(); 
	}
}


			

////////////////////////////////////////////////////////////////////////////////////////////////
// Update option tri grandeurs échelle
void CDlgChoixSupport::OnUpdateCsOrdreGrandech(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.GetNbSupports())? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection du nombre d'insertions pour les résultats Echelles
void CDlgChoixSupport::MajNbInsertions(JUnt32 NbInsertions)
{

	/* Pas de message, le temps n'est pas si long
	// Afficher message de prévention si + d'une insertion (en attendant optimistation)
	if (NbInsertions > 1)
	{
		// Arrêt du calcul demandé 
		if (AfxMessageBox("Attention le temps de calcul est plus long en multi-insertions, désirez-vous quand même effectuer le calcul ?",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return;
	}
	*/

	// n insertion sur chaque support (dans échelle et mapping)
	if(m_pActivePage == m_pPageEchelle)
	{
		m_pPageEchelle->SetNbInsertions(NbInsertions);

		// Pour le moment 1 seule insertion pour mapping
		// m_pPageMapping->SetNbInsertions(NbInsertions);

		// Le document a changé	pour échelle et mapping
		m_DocChoixSupport.SetModifiedFlag(TRUE); 

		// Update résultat suite à la modif paramètre nb insertions
		m_DocChoixSupport.UpdateAllViews(UPDATE_INSERTIONS);

		// Update Texte Grandeur pour reprise en main / Répétition
		UpdateTxtGrandeur();
	}

	// n insertion sur chaque support (dans échelle et mapping)
	if(m_pActivePage == m_pPageCumulativite)
	{
		// Sauve la nouvelle sélection nb insertions pour cumulativité
		m_pPageCumulativite->SetNbInsertions(NbInsertions);

		// Le document a changé	pour cumulativité
		m_DocChoixSupport.SetModifiedFlag(TRUE); 

		// Update résultat suite à la modif paramètre nb insertions
		m_DocChoixSupport.UpdateAllViews(UPDATE_INSERTIONS_CUM);
	}

	
}

// Validation ss-menu insertion
void CDlgChoixSupport::OnValideSsMenuNbInsertion(JUnt32 NbMaxInsertion)
{
	// Récupère le sous menu parent Option Calcul
	JUnt32 NbItem = GetMenu()->GetMenuItemCount();
	CMenu *pSsMenuOption = GetMenu()->GetSubMenu(POS_MENU_OPTIONS); 

	// Nombre d'élmt du ss menu option calcul
	NbItem = pSsMenuOption->GetMenuItemCount(); 

	// Récupère élmt ss-menu nb insertions
	CMenu*pSsMenuInsertion = pSsMenuOption->GetSubMenu(PosSsMenu_NbIns);

	if (pSsMenuInsertion == NULL) return;

	NbItem = pSsMenuInsertion->GetMenuItemCount(); 

	pSsMenuOption->EnableMenuItem(PosSsMenu_NbIns, MF_BYPOSITION | MF_ENABLED); 

	// Les 10 premières options toujours valides 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_1_INSERTION,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_2_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_3_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_4_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_5_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_6_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_7_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_8_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_9_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 
	pSsMenuInsertion->EnableMenuItem(IDM_CS_10_INSERTIONS,MF_BYCOMMAND | MF_ENABLED); 

	
	if (NbMaxInsertion <= 10)
	{
		// En mode échelle les autre options non valides
		pSsMenuInsertion->EnableMenuItem(IDM_CS_12_INSERTIONS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		pSsMenuInsertion->EnableMenuItem(IDM_CS_24_INSERTIONS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		pSsMenuInsertion->EnableMenuItem(IDM_CS_53_INSERTIONS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		pSsMenuInsertion->EnableMenuItem(IDM_CS_105_INSERTIONS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		pSsMenuInsertion->EnableMenuItem(IDM_CS_365_INSERTIONS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
	}
	else
	{
		// Toutes les options insertions disponibles
		pSsMenuInsertion->EnableMenuItem(IDM_CS_12_INSERTIONS,MF_BYCOMMAND|MF_ENABLED); 
		pSsMenuInsertion->EnableMenuItem(IDM_CS_24_INSERTIONS,MF_BYCOMMAND|MF_ENABLED); 
		pSsMenuInsertion->EnableMenuItem(IDM_CS_53_INSERTIONS,MF_BYCOMMAND|MF_ENABLED); 
		pSsMenuInsertion->EnableMenuItem(IDM_CS_105_INSERTIONS,MF_BYCOMMAND|MF_ENABLED); 
		pSsMenuInsertion->EnableMenuItem(IDM_CS_365_INSERTIONS,MF_BYCOMMAND|MF_ENABLED); 
	}
}

// Dévalidation ss-menu insertion
void CDlgChoixSupport::OnInvalideSsMenuNbInsertion()
{
	// Récupère le sous menu parent
	JUnt32 NbItem = GetMenu()->GetMenuItemCount();
	CMenu *pSsMenuOption = GetMenu()->GetSubMenu(POS_MENU_OPTIONS); 

	// Rend invalide l'option Nb Insertions
	pSsMenuOption->EnableMenuItem(PosSsMenu_NbIns, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); 
}

void CDlgChoixSupport::OnCs1Insertion()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(1);
}

void CDlgChoixSupport::OnCs2Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(2);
}

void CDlgChoixSupport::OnCs3Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(3);
}

void CDlgChoixSupport::OnCs4Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(4);
}

void CDlgChoixSupport::OnCs5Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(5);
}


void CDlgChoixSupport::OnCs6Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(6);
}


void CDlgChoixSupport::OnCs7Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(7);
}

void CDlgChoixSupport::OnCs8Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(8);
}

void CDlgChoixSupport::OnCs9Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(9);
}

void CDlgChoixSupport::OnCs10Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(10);
}

void CDlgChoixSupport::OnCs12Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(12);
}

void CDlgChoixSupport::OnCs24Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(24);
}

void CDlgChoixSupport::OnCs53Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(53);
}

void CDlgChoixSupport::OnCs105Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(105);
}

void CDlgChoixSupport::OnCs365Insertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(365);
}

void CDlgChoixSupport::OnUpdateCsMaxInsertions(CCmdUI *pCmdUI)
{
	// Pour le moment pas de maximum insertion
	pCmdUI->Enable(FALSE);
}

void CDlgChoixSupport::OnCsMaxInsertions()
{
	// Mise à jour Check Nb Insertions
	MajNbInsertions(1000);
}

///////////////////////////////////////////////////////////////////////////////////////////////
//									MODIFICATION DES SELECTIONS	
///////////////////////////////////////////////////////////////////////////////////////////////
//
// Modification sélection terrains
void CDlgChoixSupport::OnLbnSelchangeCsListTerrains()
{
	// Si 1 seul sélection en cours et mode multi-sélection , on la laisse
	if (m_LstTerrains.GetSelCount() == 0 && m_pActivePage == m_pPageEchelle)
	{
		// Récupère dernière sélection
		JUnt32 IndexTerrain;
		m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveFirst();

		if (m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->IsValid())
		{
			IndexTerrain = m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->GetKey();
			for (JUnt32 i=0; i< m_LstTerrains.GetCount(); i++)
			{
				// Affichage sélection
				if (m_LstTerrains.GetItemData(i) == IndexTerrain)
				{
					m_LstTerrains.SetSel(i,TRUE);
					break;
				}
			}
		}
	}
	else
	{
		// Modif sélection terrains
		m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->Reset();

		if (m_pActivePage == m_pPageCumulativite)
		{
			// Mono-sélection
			JUnt32 IndexSel = m_LstTerrains.GetCurSel() ;
			m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->Add(m_LstTerrains.GetItemData(IndexSel)); 
		}
		else
		{
			// Multi-Sélection
			for (JUnt32 i = 0; i < m_LstTerrains.GetCount(); i++)
			{
				// Multi-sélections
				if (m_LstTerrains.GetSel(i) > 0)
					m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->Add(m_LstTerrains.GetItemData(i)); 
			}
		}

		// Avertissement si cible non autorisé sur tous les terrains sélectionnées
		JMap <JUnt32, JUnt32> * pMapCibleSel = m_DocChoixSupport.GetMapIdCiblesAudienceSel();
		AvertissementCible(pMapCibleSel);

		// Mise à jour des différentes fenêtres associés
		UpdateFenetreFille();
	}

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

}

///////////////////////////////////////////////////////////////////////////////////////////////
// Modification sélection cibles
void CDlgChoixSupport::OnLbnSelchangeCsListCibles()
{
	int Inx = m_LstCibles.GetCurSel();

	// Si 1 seul sélection en cours et mode multi-sélection , on la laisse
	if (m_LstCibles.GetSelCount() == 0 && m_pActivePage == m_pPageEchelle)
	{
		// Récupère dernière sélection existante
		JUnt32 IndexCible;
		m_DocChoixSupport.GetMapIdCiblesAudienceSel()->MoveFirst();
		
		if (m_DocChoixSupport.GetMapIdCiblesAudienceSel()->IsValid())
		{
			IndexCible = m_DocChoixSupport.GetMapIdCiblesAudienceSel()->GetKey();
			for (JUnt32 i=0; i< m_LstCibles.GetCount(); i++)
			{
				// Affichage sélection
				if (m_LstCibles.GetItemData(i) == IndexCible)
				{
					m_LstCibles.SetSel(i,TRUE);
					break;
				}
			}
		}
	}
	else
	{
		// Modif sélection cibles
		m_DocChoixSupport.GetMapIdCiblesAudienceSel()->Reset();

		// Ident cible nouvellement sélectionnée
		JUnt32 IdCible = m_LstCibles.GetItemData(m_LstCibles.GetCurSel());

		if (m_pActivePage == m_pPageCumulativite)
		{
			// Mono-sélection
			JUnt32 IndexSel = m_LstCibles.GetCurSel() ;
			m_DocChoixSupport.GetMapIdCiblesAudienceSel()->Add(m_LstCibles.GetItemData(IndexSel)); 
		}
		else
		{
			// Multi-Sélection
			for (JUnt32 i = 0; i < m_LstCibles.GetCount(); i++)
			{
				// Multi-sélections
				if (m_LstCibles.GetSel(i) > 0)
					m_DocChoixSupport.GetMapIdCiblesAudienceSel()->Add(m_LstCibles.GetItemData(i)); 
			}
		}

		// Avertissement si cible non autorisé sur tous les terrains sélectionnées
		JMap <JUnt32, JUnt32> * pMapCibleSel = m_DocChoixSupport.GetMapIdCiblesAudienceSel();
		AvertissementCible(pMapCibleSel);

		// Mise à jour des différentes fenêtres associés
		UpdateFenetreFille();
	}

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

}

///////////////////////////////////////////////////////////////////////////////////////////////
// Avertissement si cible non représentative (Nb cas non atteint via seuil rédhibitoire)
void CDlgChoixSupport::AvertissementCible(JMap <JUnt32, JUnt32> * pMapCibleSel)
{
	// Avertissement si cible non autorisé sur tous les terrains sélectionnées
	JBool AuMoins1TerrainNonAutorise = false; 
	CString TxtAvertissement;

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Récup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_DocChoixSupport.m_Offre.m_IdSourceAudience);

	// Chaines de tous les terrains avc avertissement
	CString TxtAvertissementTerrain = "";

	// 1 seule cible à tester
	if (pMapCibleSel->GetCount() == 1)
	{
		// Récup ident cible
		pMapCibleSel->MoveFirst();
		if (pMapCibleSel->IsValid())
		{
			// 1 seule cible sélectionné
			JUnt32 IdCible = pMapCibleSel->GetKey();

			// Boucle sur les terrains sélectionnés
			for (m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveFirst(); 
					m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->IsValid();
					m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveNext())
			{
				// Récup ident terrain
				const JUnt32 IdTerrain = m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->GetKey();

				if (m_DocChoixSupport.m_Offre.CibleAutorisee(IdCible, IdTerrain) == false)  
				{
					// Ce terrain n'est pas autorisé pour cette cible
					AuMoins1TerrainNonAutorise = true;

					// Récup infos terrain
					const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_DocChoixSupport.m_Offre.m_IdSourceAudience, IdTerrain);
					CString TxtTerrain;
					TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

					// Ajoute au message le terrain
					TxtAvertissementTerrain += TxtTerrain;
				}
			}

			// Affiche message si cible non autorisée 
			if (AuMoins1TerrainNonAutorise)
			{
				/*
				// CString TxtCible;
				// m_LstCibles.GetText(m_LstCibles.GetCurSel(), TxtCible);
				// TxtAvertissement.Format("%s %s %s\n%s","Cible", TxtCible, "invalide sur terrains", TxtAvertissementTerrain);
				// Récup cible correspondant à l'indice
				*/

				// IItem * pItem = CItemsConstruits::GetInstance()->GetItemConstruit(IdCible);
				IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

				TxtAvertissement.Format("%s %s %s\n\n%s","Le nombre de cas de la cible", pItem->GetLabel().AsJCharPtr(), "est trop faible sur le(s) terrain(s) ci-dessous : ", TxtAvertissementTerrain);
				MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
			}
		}
	}
	else
	{	
		TxtAvertissement = "";

		// Plusieurs cibles sélectionnées
		for (pMapCibleSel->MoveFirst(); pMapCibleSel->IsValid(); pMapCibleSel->MoveNext())
		{
			// Récup la cible
			JUnt32 IdCible = pMapCibleSel->GetKey();

			// Boucle sur les terrains sélectionnés
			for (m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveFirst(); 
					m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->IsValid();
					m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveNext())
			{
				// Récup ident terrain
				const JUnt32 IdTerrain = m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->GetKey();

				if (m_DocChoixSupport.m_Offre.CibleAutorisee(IdCible, IdTerrain) == false)  
				{
					// Ce terrain n'est pas autorisé pour cette cible
					AuMoins1TerrainNonAutorise = true;

					// Récup infos terrain
					const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_DocChoixSupport.m_Offre.m_IdSourceAudience, IdTerrain);
					CString TxtTerrain;
					TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

					// Ajoute au message le terrain
					TxtAvertissementTerrain.Append(TxtTerrain);
				}
			}

			// Affiche message si cible non autorisée 
			if (AuMoins1TerrainNonAutorise)
			{
				// Création du texte d'avertissement
				CString TxtTerrains;
				// IItem * pItem = CItemsConstruits::GetInstance()->GetItemConstruit(IdCible);
				IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

				TxtAvertissement.Format("%s %s %s\n\n%s","Le nombre de cas de la cible", pItem->GetLabel().AsJCharPtr(), "est trop faible sur le(s) terrain(s) ci-dessous : ", TxtAvertissementTerrain);

				TxtAvertissement.Append(TxtTerrains); 
			}
		}

		if (TxtAvertissement != "")
			MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Modification sélection segment d'audience pour tous les modules
void CDlgChoixSupport::OnLbnSelchangeCsListSegPop()
{
	if (m_LstSegPop.GetCount() > 1)
	{
		for (JUnt32 InxModule = m_DocChoixSupport.ModuleEch; 
					InxModule <= m_DocChoixSupport.ModuleDup; 
					InxModule++)
		{
			// Reset du map sélection
			m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->Reset(); 

			// Si 1 seul sélection en cours et mode multi-sélection , on la laisse
			if (m_LstSegPop.GetSelCount() == 0)
			{
				// Récupère dernière sélection
				JUnt32 IndexSegPop;
				m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->MoveFirst();

				if (m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->IsValid())
				{
					IndexSegPop = m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->GetKey();
					for (JUnt32 i=0; i< m_LstSegPop.GetCount(); i++)
					{
						// Affichage sélection
						if (m_LstSegPop.GetItemData(i) == IndexSegPop)
						{
							m_LstSegPop.SetSel(i,TRUE);
							break;
						}
					}
				}
			}
			else
			{
				// Init Map sélection segments population pour le module courant
				m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->Reset();

				// Multi-Sélection
				for (JUnt32 i = 0; i < m_LstSegPop.GetCount(); i++)
				{
					// Item Data
					JUnt32 IdSeg = m_LstSegPop.GetItemData(i);

					// Multi-sélections
					if (m_LstSegPop.GetSel(i) > 0)
						m_DocChoixSupport.GetMapIdSegmentsAudienceSel(InxModule)->Add(m_LstSegPop.GetItemData(i)); 
				}

				// Recalcul avec nouveaux segments
				ParamCalculModif();
			}
		}

		// Mise à jour sélections segments
		m_DocChoixSupport.UpdateAllViews(UPDATE_SEL_SEGMENT);

		// Filtre visibilité des supports via nouveau(x) segment(s)
		m_DocChoixSupport.FiltreSupportsSegments();

		// On remet le tri à jour ?????
		// Dans échelles
		m_DocChoixSupport.UpdateAllViews(UPDATE_TRI_COURANT_ECH);

		// Dans Cumulation
		m_DocChoixSupport.UpdateAllViews(UPDATE_TRI_COURANT_CUM);

		// DAns Duplication
		m_DocChoixSupport.UpdateAllViews(UPDATE_TRI_COURANT_DUP);

		// Le document a changé
		m_DocChoixSupport.SetModifiedFlag(TRUE); 
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Modification sélection format (1 seul format à la fois)
void CDlgChoixSupport::OnLbnSelchangeCsListFormats()
{
	// Récup dernier format sélectionné
	JUnt32 IdFormatCur;
	if (m_DocChoixSupport.GetMapIdFormatsSel()->IsValid()) 
		IdFormatCur = m_DocChoixSupport.GetMapIdFormatsSel()->GetKey();
	else
	{
		m_DocChoixSupport.GetMapIdFormatsSel()->MoveFirst(); 
		IdFormatCur = m_DocChoixSupport.GetMapIdFormatsSel()->GetKey();
	}

	// Modif sélection format
	m_DocChoixSupport.GetMapIdFormatsSel()->Reset();
	JUnt32 IdFormat;
	for (JUnt32 i = 0; i < m_LstFormats.GetCount(); i++)
	{
		if (m_LstFormats.GetSel(i) > 0)
		{
			// 1 seul format à la fois
			IdFormat = m_LstFormats.GetItemData(i);
			m_DocChoixSupport.GetMapIdFormatsSel()->Add(IdFormat); 
			break;
		}
	}

	// Puis on met à jour le map des formats supports pour le module courant (mode remplacement)
	UpdateFormatSupports(IdFormat, true);

	// Change les résultats si nouveau format
	m_DocChoixSupport.GetMapIdFormatsSel()->MoveFirst();
	if (m_DocChoixSupport.GetMapIdFormatsSel()->IsValid())
	{
		if (m_DocChoixSupport.GetMapIdFormatsSel()->GetKey() != IdFormatCur)
		{
			// Mise à jour des résultats associés (sauf pour duplication)
			if (m_pActivePage != m_pPageDuplication)
				UpdateResultat(false);
		}
	}

	// Mise à jour des différentes fenêtres associés
	if (m_pActivePage ==  m_pPageEchelle)
	{
		// Mise à jour format par défaut pour échelle
		m_pPageEchelle->UpdateSelFormatSupports();
	}

	else if (m_pActivePage ==  m_pPageMapping) 	
		// Mise à jour format par défaut pour mapping
		m_pPageMapping->UpdateSelFormatSupports();

	else if (m_pActivePage ==  m_pPageCumulativite)
		// Mise à jour format par défaut pour cumulativité
		m_pPageCumulativite->UpdateSelFormatSupports();

	else if (m_pActivePage ==  m_pPageDuplication)	
		// Mise à jour format par défaut pour duplication
		m_pPageDuplication->UpdateSelFormatSupports();
		
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports dans chacun des modules
void CDlgChoixSupport::UpdateFormatSupports(JUnt32 IdFormat, bool ReplacePrecFormat)
{

	// Récupére l'ident du format courant
	m_DocChoixSupport.GetMapIdFormatsSel()->MoveFirst(); 
	if (m_DocChoixSupport.GetMapIdFormatsSel()->IsValid())
	{
		// Format courant 
		IdFormat		= m_DocChoixSupport.GetMapIdFormatsSel()->GetKey();

		// on teste la validité du document courant
		if (m_DocChoixSupport.IsValid())
		{
			if (ReplacePrecFormat)
			{
				// Remplacement dans module Echelle
				// On remplace les précédents formats sélectionnés par le format courant
				m_DocChoixSupport.m_MapIdFormatSpecifEch.Reset();
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Indique information format associé à ce support
					m_DocChoixSupport.m_MapIdFormatSpecifEch.Add(IdSupport) = IdFormat;	
				}

				// Remplacement dans module Cumulativité
				// On remplace les précédents formats sélectionnés par le format courant
				m_DocChoixSupport.m_MapIdFormatSpecifCum.Reset();
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
					m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
					m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Indique information format associé à ce support
					m_DocChoixSupport.m_MapIdFormatSpecifCum.Add(IdSupport) = IdFormat;	
				}

				// Remplacement dans module Duplication
				// On remplace les précédents formats sélectionnés par le format courant
				m_DocChoixSupport.m_MapIdFormatSpecifDup.Reset();
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Indique information format associé à ce support
					m_DocChoixSupport.m_MapIdFormatSpecifDup.Add(IdSupport) = IdFormat;	
				}

				// Remplacement dans module Mapping
				// On remplace les précédents formats sélectionnés par le format courant
				m_DocChoixSupport.m_MapIdFormatSpecifMap.Reset();
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Indique information format associé à ce support
					m_DocChoixSupport.m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;	
				}
			}
			
			else
			// Mise à jour des formats via nouveau formats sélectionnés (sauf les formats déjà validés par support)
			{
				// Mise à jour map des formats supports échelle
				if (m_pActivePage == m_pPageEchelle)
				{
					// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
					for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
					{
						// Ident support
						JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

						// Voir si support non défini au niveau format
						m_DocChoixSupport.m_MapIdFormatSpecifEch.MoveTo(IdSupport);
						if (!m_DocChoixSupport.m_MapIdFormatSpecifEch.IsValid())
						{
							// Récupération des idents libellés format possible pour un support
							// JMap <JUnt32,JUnt32> *pLstIdFormats;
							JMap <JUnt32,JUnt32> MapIdFormats;
							m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

							// Si le format courant appartient aux formats possibles supports, 
							// on le met comme format sélectionné pour les nouveaux supports
							MapIdFormats.MoveTo(IdFormat);
							if (MapIdFormats.IsValid())
							{
								// Indique information format associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifEch.Add(IdSupport) = IdFormat;		
							}
							else
							{
								// Pas de format pour le moment associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifEch.Add(IdSupport) = 0;		
							}
						}
					}
				}
	
				// Mise à jour map des formats supports mapping
				else if (m_pActivePage == m_pPageMapping) 	
				{
					// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
					for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
					{
						// Ident support
						JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

						// Voir si support non défini au niveau format
						m_DocChoixSupport.m_MapIdFormatSpecifMap.MoveTo(IdSupport);
						if (!m_DocChoixSupport.m_MapIdFormatSpecifMap.IsValid())
						{
							// Récupération des idents libellés format possible pour un support
							JMap <JUnt32,JUnt32> MapIdFormats;
							m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

							// Si le format courant appartient aux formats possibles supports, 
							// on le met comme format sélectionné pour les nouveaux supports
							MapIdFormats.MoveTo(IdFormat);
							if (MapIdFormats.IsValid())
							{
								// Indique information format associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;		
							}
							else
							{
								// Pas de format pour le moment associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifMap.Add(IdSupport) = 0;		
							}
							
						}
					}
				}
		
				// Mise à jour map des formats supports cumulativité
				else if (m_pActivePage == m_pPageCumulativite)
				{
					// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
					for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
					{
						// Ident support
						JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

						// Voir si support non défini au niveau format
						m_DocChoixSupport.m_MapIdFormatSpecifCum.MoveTo(IdSupport);
						if (!m_DocChoixSupport.m_MapIdFormatSpecifCum.IsValid())
						{
							// Récupération des idents libellés format possible pour un support
							JMap <JUnt32,JUnt32> MapIdFormats;
							m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

							// Si le format courant appartient aux formats possibles supports, 
							// on le met comme format sélectionné pour les nouveaux supports
							MapIdFormats.MoveTo(IdFormat);
							if (MapIdFormats.IsValid())
							{
								// Indique information format associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifCum.Add(IdSupport) = IdFormat;		
							}
							else
							{
								// Pas de format pour le moment associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifCum.Add(IdSupport) = 0;		
							}
						}
					}
				}

				// Mise à jour map des formats supports duplication
				else if (m_pActivePage == m_pPageDuplication)	
				{
					// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
					for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
					{
						// Ident support
						JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

						// Voir si support non défini au niveau format
						m_DocChoixSupport.m_MapIdFormatSpecifDup.MoveTo(IdSupport);
						if (!m_DocChoixSupport.m_MapIdFormatSpecifDup.IsValid())
						{
							// Récupération des idents libellés format possible pour un support
							JMap <JUnt32,JUnt32> MapIdFormats;
							m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

							// Si le format courant appartient aux formats possibles supports, 
							// on le met comme format sélectionné pour les nouveaux supports
							MapIdFormats.MoveTo(IdFormat);
							if (MapIdFormats.IsValid())
							{
								// Indique information format associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifDup.Add(IdSupport) = IdFormat;		
							}
							else
							{
								// Pas de format pour le moment associé à ce support
								m_DocChoixSupport.m_MapIdFormatSpecifDup.Add(IdSupport) = 0;		
							}
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports dans chacun des modules
void CDlgChoixSupport::UpdateModuleFormatSupports(JUnt32 IdFormat, JInt32 TypeModule)
{

	// Récupére l'ident du format courant
	m_DocChoixSupport.GetMapIdFormatsSel()->MoveFirst(); 
	if (m_DocChoixSupport.GetMapIdFormatsSel()->IsValid())
	{
		// Format courant 
		IdFormat		= m_DocChoixSupport.GetMapIdFormatsSel()->GetKey();

		// on teste la validité du document courant
		if (m_DocChoixSupport.IsValid())
		{
			// Mise à jour map des formats supports échelle
			if (TypeModule == m_DocChoixSupport.ModuleEch)
			{
				// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
			 		 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
					 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Voir si support non défini au niveau format
					m_DocChoixSupport.m_MapIdFormatSpecifEch.MoveTo(IdSupport);
					if (!m_DocChoixSupport.m_MapIdFormatSpecifEch.IsValid())
					{
						// Récupération des idents libellés format possible pour un support
						// JMap <JUnt32,JUnt32> *pLstIdFormats;
						JMap <JUnt32,JUnt32> MapIdFormats;
						m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

						// Si le format courant appartient aux formats possibles supports, 
						// on le met comme format sélectionné pour les nouveaux supports
						MapIdFormats.MoveTo(IdFormat);
						if (MapIdFormats.IsValid())
						{
							// Indique information format associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifEch.Add(IdSupport) = IdFormat;		
						}
						else
						{
							// Pas de format pour le moment associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifEch.Add(IdSupport) = 0;		
						}
					}
				}
			}

			// Mise à jour map des formats supports mapping
			else if (TypeModule == m_DocChoixSupport.ModuleMap) 	
			{
				// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
						m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
						m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Voir si support non défini au niveau format
					m_DocChoixSupport.m_MapIdFormatSpecifMap.MoveTo(IdSupport);
					if (!m_DocChoixSupport.m_MapIdFormatSpecifMap.IsValid())
					{
						// Récupération des idents libellés format possible pour un support
						JMap <JUnt32,JUnt32> MapIdFormats;
						m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

						// Si le format courant appartient aux formats possibles supports, 
						// on le met comme format sélectionné pour les nouveaux supports
						MapIdFormats.MoveTo(IdFormat);
						if (MapIdFormats.IsValid())
						{
							// Indique information format associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;		
						}
						else
						{
							// Pas de format pour le moment associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifMap.Add(IdSupport) = 0;		
						}
						
					}
				}
			}
	
			// Mise à jour map des formats supports cumulativité
			else if (TypeModule == m_DocChoixSupport.ModuleCum)
			{
				// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
					 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
					 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Voir si support non défini au niveau format
					m_DocChoixSupport.m_MapIdFormatSpecifCum.MoveTo(IdSupport);
					if (!m_DocChoixSupport.m_MapIdFormatSpecifCum.IsValid())
					{
						// Récupération des idents libellés format possible pour un support
						JMap <JUnt32,JUnt32> MapIdFormats;
						m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

						// Si le format courant appartient aux formats possibles supports, 
						// on le met comme format sélectionné pour les nouveaux supports
						MapIdFormats.MoveTo(IdFormat);
						if (MapIdFormats.IsValid())
						{
							// Indique information format associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifCum.Add(IdSupport) = IdFormat;		
						}
						else
						{
							// Pas de format pour le moment associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifCum.Add(IdSupport) = 0;		
						}
					}
				}
			}

			// Mise à jour map des formats supports duplication
			else if (TypeModule == m_DocChoixSupport.ModuleDup)	
			{
				// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
				for (m_DocChoixSupport.m_MapIdSupportsSel.MoveFirst();
					 m_DocChoixSupport.m_MapIdSupportsSel.IsValid();
					 m_DocChoixSupport.m_MapIdSupportsSel.MoveNext())
				{
					// Ident support
					JUnt32 IdSupport = m_DocChoixSupport.m_MapIdSupportsSel.GetKey();

					// Voir si support non défini au niveau format
					m_DocChoixSupport.m_MapIdFormatSpecifDup.MoveTo(IdSupport);
					if (!m_DocChoixSupport.m_MapIdFormatSpecifDup.IsValid())
					{
						// Récupération des idents libellés format possible pour un support
						JMap <JUnt32,JUnt32> MapIdFormats;
						m_DocChoixSupport.GetListIdFormats(IdSupport, &MapIdFormats);

						// Si le format courant appartient aux formats possibles supports, 
						// on le met comme format sélectionné pour les nouveaux supports
						MapIdFormats.MoveTo(IdFormat);
						if (MapIdFormats.IsValid())
						{
							// Indique information format associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifDup.Add(IdSupport) = IdFormat;		
						}
						else
						{
							// Pas de format pour le moment associé à ce support
							m_DocChoixSupport.m_MapIdFormatSpecifDup.Add(IdSupport) = 0;		
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Affichage fenêtre information version
void CDlgChoixSupport::OnCsVersion()
{
	// on affiche la boîte version choix support
	CATPVersionCS Dlg(this); 
	Dlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Teste si fermeture plateau possible 
JBool CDlgChoixSupport::CanClose() 
{
	// on teste la validité du document courant
	if (m_DocChoixSupport.IsValid())
	{
		// on teste l'indicateur de modification du document courant
		if (FALSE != m_DocChoixSupport.IsModified())
		{
			// on affiche le message
			HWND hHwnd = NULL;
			CWnd *phParent = this->GetParent(); 
			if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
			// INT Ret = ::MessageBox(hHwnd, "Voulez-vous enregistrer les modifications apportées au plateau courant ?" ,"SAUVEGARDE PLATEAU COURANT", MB_ICONQUESTION | MB_YESNOCANCEL);
			INT Ret = this->MessageBox("Voulez-vous enregistrer les modifications apportées au plateau courant ?" ,"ANALYSE DE L'OFFRE", MB_ICONQUESTION | MB_YESNOCANCEL);

			// on teste la réponse de l'utilisateur
			if (Ret == IDYES)
			{
				// on sauvegarde le document choix supports courant
				if (FALSE == this->OnSaveDocument(m_DocChoixSupport, false))
					return false;

				// on annule l'indicateur de modification
				m_DocChoixSupport.SetModifiedFlag(FALSE);
			}

			/*
			else
			{
				if (Ret != IDNO) // on quitte
					return false;
			}
			*/
			if (Ret == IDCANCEL) // on quitte
				return false;
		}
	}


	// on crée un nouveau document choix support
	JATPDocChoixSupport ATPDocChoixSupport;
	ATPDocChoixSupport.PrepareContents();

	// on permute les documents
	m_DocChoixSupport.Swap(ATPDocChoixSupport);

	// on annule l'indicateur de modification
	m_DocChoixSupport.SetModifiedFlag(FALSE);

	// on réinitialise tout
	JDate DateInit;
	m_DocChoixSupport.m_DateTarifaire = DateInit;
	m_DocChoixSupport.UpdateAllViews(UPDATE_RAZ);

	// Mise à jour titre fenêtre
	//MajTitreFenetreCS();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde document Choix Supports
BOOL CDlgChoixSupport::OnSaveDocument(JATPDocChoixSupport & ATPDocChoixSupport, JBool bRename)
{
	// on initialise l'indicateur
	BOOL Ret = FALSE;

	CString path;
	if (bRename || ATPDocChoixSupport.m_KEYPlateau.GetPath() == "")
	{
		// Filtre pour sauvegarde document choix support (extension ofr)
		CString szFilters;
		szFilters.Format(IDS_CS_FILTER);
		DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN |OFN_OVERWRITEPROMPT ;

		// Affichage boite de dialogue sauvegarde fichier
		CFileDialog fileDlg(FALSE, "ofr", 0, flags, szFilters, this);
		if( fileDlg.DoModal() == IDOK )
		{
			// Récupère path sauvegarde fichier plateaux
			path = fileDlg.GetPathName();

			// Sauve si enregistrement
			if (!bRename)
				ATPDocChoixSupport.m_KEYPlateau.SetPath(path);
		}
		else
			return FALSE;
	}
	else
		// Récupère path sauvegarde fichier plateaux
		path = ATPDocChoixSupport.m_KEYPlateau.GetPath();


	// Sauvegarde dossier plateau
	JMFCStream* stream = NULL;
	try
	{
		// Avant bug avec 282 CStdioFile file(path, CFile::modeCreate |CFile::modeWrite);
		CFile file(path, CFile::modeCreate |CFile::modeWrite);
		CArchive ar(&file,CArchive::store);
		stream = JMFCStream::Create(ar);
		ATPDocChoixSupport.Send(*stream);
		stream->Flush();
	}
	catch(...)
	{
	}
	if (stream)
		delete stream;

	// Sauve nouveau path sauvegarde dossier plateau
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));

	if (!bRename)
	{
		// Mise à jour des vues
		ATPDocChoixSupport.SetModifiedFlag(FALSE);
		ATPDocChoixSupport.UpdateAllViews(UPDATE_KEY_PLATEAU); 
	}
	return (Ret);
}

////////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CDlgChoixSupport::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Réinitialise les élmts Interface Choix Supports
	if(Message == UPDATE_RAZ)
	{
		// Charge grandeurs car document réinitialisé
		m_pPupitreEch->InitLstGrandeurs();
		m_pPupitreEch->LoadGrandeurs();

		m_pPupitreMap->InitLstGrandeurs();
		m_pPupitreMap->LoadGrandeurs();

		m_pPupitreCum->InitLstGrandeurs();
		m_pPupitreCum->LoadGrandeurs();

		m_pPupitreDup->InitLstGrandeurs();
		m_pPupitreDup->LoadGrandeurs();

		// Mise à jour des sélections terrains / cibles / réf affinité / formats
		UpdateSelTerrains();
		UpdateSelCibles();
		UpdateSelSegPop();
		UpdateSelRefAff();
		UpdateSelFormats();

		// Mise sélection supports dans toutes les fenêtres associés
		UpdateSelSupports();

		// Mise à jour des différentes fenêtres associées
		UpdateFenetreFille();

		// Ne pas valider les boutons mode affichage Familles et Supports si aucune étude en cours
		m_BtnFamilles.EnableWindow(FALSE);
		m_BtnSupports.EnableWindow(FALSE);

		// Ne pas valider les boutons Filtre et Contexte si aucune étude en cours
		m_BtnFiltreAudience.EnableWindow(FALSE);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Mise à jour couleur Interface Choix Supports
	else if(Message == UPDATE_COLOR)
	{
		// Zone principale pour chaque fenêtre choix supports
		m_pPageEchelle->SetColors();
		m_pPageMapping->SetColors();
		m_pPageCumulativite->SetColors();
		m_pPageDuplication->SetColors();

		// Les zones sélections pupitres spécifiques à chaque page
		m_pPupitreEch->SetColors();
		m_pPupitreMap->SetColors();
		m_pPupitreCum->SetColors();
		m_pPupitreDup->SetColors();

		// Boite de dialogue filtre d'audience
		m_DlgFiltresAudience.SetColors(CATPColors::COLORCHOIXSUPPORTS);

	}

	//////////////////////////////////////////////////////////////////////////
	// Mise à jour des résultats suite modif Offre 
	else if (Message == UPDATE_OFFRE || Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU )
	{
		if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
		{
			// Charge grandeurs car document réinitialisé
			m_pPupitreEch->InitLstGrandeurs();
			m_pPupitreEch->LoadGrandeurs();

			m_pPupitreMap->InitLstGrandeurs();
			m_pPupitreMap->LoadGrandeurs();

			m_pPupitreCum->InitLstGrandeurs();
			m_pPupitreCum->LoadGrandeurs();

			m_pPupitreDup->InitLstGrandeurs();
			m_pPupitreDup->LoadGrandeurs();

			// Reset des résultats
			m_DocChoixSupport.ResetAllResultat();
		}

		// A VOIR Octobre 2006 pas nécessaire (prend beaucoup de temps) Calcul nombre des segments totals dispo
		// m_DocChoixSupport.m_NbTotSegDispo	= GetNbTotSegmentTerrains();

		// A REVOIR
		// Reset des résultats (car il faudra par la suite tester l'affinité // mettre dans clef résult)
		// 
		// Reset résultats échelle et mapping pour calcul indice affinité
		/*
		m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleEch);
		m_DocChoixSupport.ResetResultat(m_DocChoixSupport.ModuleMap);
		*/
        		
		// Maj des sélections terrains / cibles / seg pop / ref.aff./ formats
		UpdateSelTerrains();
		UpdateSelCibles();
		UpdateSelSegPop();
		UpdateSelRefAff();
		UpdateSelFormats();

		// Mise à jour du format associé à chaque support
		if (Message == UPDATE_INITIAL)
			UpdateFormatSupports(0,true);
		else
			UpdateFormatSupports(0,false);

		// Mise à jour sélections supports
		UpdateSelSupports();

		// Mise à jour boutons sélections "Familles/Supports"
		UpdateBtnModeSelSupports();

		// Rend visible tous les supports
		// ??? laisse état courant des supports
		//m_DocChoixSupport.SetAllSupportVisible(true);

		// Mise à jour des différentes fenêtres associés
		UpdateFenetreFille();

		// Validation des boutons mode affichage
		/* A REMETTRE
		m_BtnFamilles.EnableWindow(FALSE); 
		m_BtnSupports.EnableWindow(FALSE); 
		*/

		// ?????????????????????????????  VOIR SI ON VIRE  ALAIN Mise à jour des grilles supports
		// m_DocChoixSupport.UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

		// Validation des boutons Filtre / Contexte Lecture
		m_BtnFiltreAudience.EnableWindow(TRUE);	
		//m_BtnContexteLecture.EnableWindow(TRUE);
	}

	//////////////////////////////////////////////////////////////////////////
	// Mise à jour des résultats suite modif Formats
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Mise à jour des sélections formats
		UpdateSelFormats();

		// Mise à jour des résultats
		UpdateResultat(false);
	}

	//////////////////////////////////////////////////////////////////////////
	// Mise à jour des résultats suite modif Cible
	else if (Message == UPDATE_SEL_CIBLE)
	{
		// Mise à jour des sélections cibles
		UpdateSelCibles();

		// Mise à jour des résultats
		UpdateResultat(true);
	}

	//////////////////////////////////////////////////////////////////////////
	// Mise à jour des résultats suite modif Segment Population
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise à jour des sélections segments population
		UpdateSelSegPop();

		// Filtrage supports via segment(s)
		m_DocChoixSupport.FiltreSupportsSegments();

		// Mise à jour des résultats
		UpdateResultat(true);

	}

	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// on teste les exceptions
		try
		{
			// Vire le map courant résultat
			/* A VOIR
			m_DocChoixSupport.ResetAllResultat();
			*/

			// Positionne flags modif calcul pour tous les modules
			ParamCalculModif();

			// Recalcul avec nouveau type moteur
			UpdateResultat(true);
			m_DlgChoixMoteur.Update();
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}


	///////////////////////////// CHANGEMENT OPTIONS MOTEUR ///////////////////////////
	//
	else if (Message == UPDATE_OPTIONSMOTEUR)
	{
		// on teste les exceptions
		try
		{
			// Vire le map courant résultat
			m_DocChoixSupport.ResetAllResultat();

			// Positionnement nouvelles options moteur
			// Attention pour certains supports il se peut que l'on doit automatiquement
			// passer dans un autre mode LDP ou LNM
			m_DocChoixSupport.m_MoteurPresse.SetOptions(m_DocChoixSupport.m_Offre, 
														m_DocChoixSupport.m_bLDPQtd, 
														m_DocChoixSupport.m_bLNMMag,
														m_DocChoixSupport.m_bLDP30J,
														m_DocChoixSupport.m_bRepriseEnMain);

			// Positionne flags modif calcul pour tous les modules
			ParamCalculModif();

			// Recalcul avec nouvelles options moteur
			UpdateResultat(true);
			m_DlgChoixMoteur.Update();
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_FILTREAUDIENCE)
	{
		// on teste les exceptions
		try
		{
			// Vire le map courant résultat
			m_DocChoixSupport.ResetAllResultat();

			// Positionnement nouvelles options moteur
			m_DocChoixSupport.m_MoteurPresse.SetFiltreAudience(m_DocChoixSupport.m_Offre, m_DocChoixSupport.GetIdFiltreAudienceSel());

			// Positionne flags modif calcul pour tous les modules
			ParamCalculModif();

			// Recalcul avec nouvelles options moteur
			UpdateResultat(true);

			// Mise à jour info sélecteurs
			UpdateSelecteur();
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	else if (Message == UPDATE_CTXLECTURE)
	{
		// on teste les exceptions
		try
		{
			// Vire le map courant résultat
			m_DocChoixSupport.ResetAllResultat();

			// Positionne flags modif calcul pour tous les modules
			ParamCalculModif();

			// Recalcul avec nouvelles options moteur
			UpdateResultat(true);

			// Mise à jour info sélecteurs
			UpdateSelecteur();
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	/////////////////////////////////////////////////  MAJ PANELS ////////////////////////////////////////////
	// 
	else if (Message == UPDATE_AFF_PANEL)
	{
		// on teste les exceptions
		try
		{
			// Mise à jour d'office boite dialogue Filtre
			m_DlgFiltresAudience.Update();

		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	// Refresh les listes sélection
	m_LstTerrains.Invalidate(); 
	m_LstCibles.Invalidate(); 
	m_LstSegPop.Invalidate();
	m_LstFormats.Invalidate(); 
}

////////////////////////////////////////////////////////////////////////////
// Mise à jour boutons sélections "Familles/Supports"
void CDlgChoixSupport::UpdateBtnModeSelSupports()
{
	// Bouton Famille
	m_BtnFamilles.EnableWindow(false); 
	if (m_DocChoixSupport.m_Offre.m_MapSelFamilles.GetCount() > 0)
	{
		// Ssi fenetre echelle ou mapping
		if (m_pActivePage == m_pPageEchelle || m_pActivePage == m_pPageMapping)
			m_BtnFamilles.EnableWindow(true); 
	}

	// Bouton Supports
	m_BtnSupports.EnableWindow(false);
	if (m_DocChoixSupport.m_Offre.m_TBLIdSupports.GetCount() > 0)  
	{
		// Ssi fenetre echelle ou mapping
		if (m_pActivePage == m_pPageEchelle || m_pActivePage == m_pPageMapping)
			m_BtnSupports.EnableWindow(true); 
	}
}

// Nombre de segments maxi pour la source sélectionné
JUnt32 CDlgChoixSupport::GetNbTotSegmentTerrains()
{
	// Init Nb tot segments
	JUnt32 NbTotSeg = 0;

	// Récupère liste globale des terrains de cette source
	const JSRCTables & Sources = JSRCTables::GetInstance();

	// Liste des terrains
	JList <JUnt32> ListeTerrains;
	ListeTerrains.Reset();
	
	// Récupére id source en cours
	JUnt32 IdSource = m_DocChoixSupport.GetIdSrcAudienceSel();

	// Balaye tous les terrains
	for(Sources.m_TBLTerrains.MoveFirst(); Sources.m_TBLTerrains.IsValid(); Sources.m_TBLTerrains.MoveNext())
	{
		const JTerrain &Item = Sources.m_TBLTerrains.GetItem();
		if(Item.m_IdSource  == IdSource)
		{
			// Ajoute à la liste des id terrains
			ListeTerrains.AddTail() = Item.m_IdTerrain;  
		}
	}

	// Init liste des segments
	JMap<JUnt32,JUnt32> MapSegments;
	MapSegments.Reset();

	// Balaye tous les terrains
	for (ListeTerrains.MoveFirst(); ListeTerrains.IsValid(); ListeTerrains.MoveNext())
	{
		// Récup id terrain
		JUnt32 &IdTerrain = ListeTerrains.GetItem(); 

		//on remplit avec les segments des terrains
		CTerrain * pterrain = NULL;
		CKeyTerrain Key(IdSource, IdTerrain);
		pterrain = CTerrainManager::GetInstance()->GetTerrain(Key);
		
		if (pterrain != NULL)
		{
			JVector<CSegment*> vect_segment;
			if (pterrain->GetSegments(vect_segment))
			{
				// Balaye liste des segments
				for (vect_segment.MoveFirst(); vect_segment.IsValid(); vect_segment.MoveNext())
				{
					// Récup id segment
					JUnt32 IdSeg = vect_segment.GetItem()->GetID();

					// Voir si nouveau
					MapSegments.MoveTo(IdSeg);
					if (!MapSegments.IsValid())
					{
						// On l'ajoute à la liste des segments dispos
						MapSegments.Add(IdSeg); 
					}
				}
			}
		}
	}

	// Récup nb tot segments
	NbTotSeg = 	MapSegments.GetCount();

	// Retourne le nombre de segments au total sur la source
	return (NbTotSeg); 
}

//////////////////////////////////////////////////////////////////////////////////////////
// Positionne flags modif calcul pour tous les modules
void CDlgChoixSupport::ParamCalculModif()
{
	// Modification calcul pour échelles
	m_DocChoixSupport.m_ParamEchModif = true;

	// Modification calcul pour mapping
	m_DocChoixSupport.m_ParamMapModif = true;

	// Modification calcul pour cumulativité
	m_DocChoixSupport.m_ParamCumModif = true;

	// Modification calcul pour duplication
	m_DocChoixSupport.m_ParamDupModif = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections terrains selon module 
void CDlgChoixSupport::UpdateSelTerrains()
{
	// Mise à jour entête source
	JUnt32 SrcID =  m_DocChoixSupport.GetIdSrcAudienceSel();
	if (SrcID)
	{
		// Récup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// Récup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// Ecriture du label
		m_LblTerrains.SetFontBold(true);
		m_LblTerrains.SetText(pt->m_Libelle.AsJCharPtr());

		// Tonton a décidé de remettre Terrains
		// m_LblTerrains.SetText("Terrains");
	}
	else
	{
		// Aucune source de détecté, label "terrains" en standard
		m_LblTerrains.SetFontBold(true);
		m_LblTerrains.SetText("Terrains");
	}

	// IndexModule : correspond à (0:Echelle, 1:Mapping, 2:Cumul, 3:Dupli)
	// Initialise liste terrains
	m_LstTerrains.ResetContent();

	// Déselectione tout selon mode type sélection (multi ou mono)
	if (m_pActivePage == m_pPageCumulativite || 
		m_pActivePage == m_pPageMapping ||
		m_pActivePage == m_pPageDuplication)
		m_LstTerrains.SetCurSel(-1);
	else
		m_LstTerrains.SetSel(-1,FALSE);

	// L'index du module courant
	JUnt32 InxModule = m_DocChoixSupport.GetModuleCourant();
	if (InxModule >= m_DocChoixSupport.ModuleEch &&	InxModule <= m_DocChoixSupport.ModuleDup)
	{	
		// Charge la liste des terrains
		for (m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
			 m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.IsValid();
			 m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			// Récup Table
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// Récup la source audiences courante sur ce module
			JUnt32 SrcID = m_DocChoixSupport.GetIdSrcAudienceSel();  

			// Si source valide
			if (SrcID)
			{
				// Récupére id terrain et le terrain
				JUnt32 TerID = m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.GetItem();
				const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// Ajoute libellé dans liste		 
				CString Txt = pt->m_Libelle.AsJCharPtr();
				int Index = m_LstTerrains.AddString(pt->m_Libelle.AsJCharPtr()); 

				// Ajoute info ident terrain
				m_LstTerrains.SetItemData(Index,TerID); 
			}
		}

		// Mise à jour de la liste
		m_LstTerrains.UpdateWindow();
		m_LstTerrains.Invalidate();

		// Valide les sélections
		for (m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
			 m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.IsValid();
			 m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			// Récup Table
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// Récup la source audiences courante sur ce module
			JUnt32 SrcID = m_DocChoixSupport.GetIdSrcAudienceSel();  

			// Si source valide
			if (SrcID)
			{
				// Récupére id terrain et le terrain
				JUnt32 TerID = m_DocChoixSupport.m_Offre.m_TBLIdTerrainsAudience.GetItem();

				// Recherche index libellé dans liste		 
				int Index = 0;
				for (JUnt32 j=0; j<m_LstTerrains.GetCount(); j++)
				{
					if (m_LstTerrains.GetItemData(j) == TerID)
						Index = j;
				}

				// Récupère le map id terrains sélectionné pour ce module
				JMap <JUnt32,JUnt32> *pMapIdTerrainsSel = m_DocChoixSupport.GetMapIdTerrainsAudienceSel(); 

				JUnt32 NbElmt = pMapIdTerrainsSel->GetCount(); 
				if (pMapIdTerrainsSel)
				{
					// Mono-sélection
					if (m_pActivePage == m_pPageCumulativite || 
						m_pActivePage == m_pPageMapping ||
						m_pActivePage == m_pPageDuplication)
					{
						// Vérifie dans le map sélection terrain
						pMapIdTerrainsSel->MoveTo(TerID);
						if (pMapIdTerrainsSel->IsValid ())
						{
							// s'il existe on indique la sélection dans la liste
							int nret  = m_LstTerrains.SetCurSel(Index);
							m_LstTerrains.UpdateWindow();
						}
					}
					else
					// Multi-sélections
					{
						// Vérifie dans le map sélection terrain
						pMapIdTerrainsSel->MoveTo(TerID);
						if (pMapIdTerrainsSel->IsValid ())
						{
							// s'il existe on indique la sélection dans la liste
							m_LstTerrains.SetSel(Index); 
						}
					}
				}
			}			
		}
	}	
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections cibles
void CDlgChoixSupport::UpdateSelCibles()
{
	// IndexModule : correspond à (0:Echelle, 1:Mapping, 2:Cumul, 3:Dupli))
	// Initialise liste cibles
	m_LstCibles.ResetContent();

	// Désélectione tout selon mode type sélection (multi ou mono)
	if (m_pActivePage == m_pPageCumulativite)
		m_LstCibles.SetCurSel(-1);
	else
		m_LstCibles.SetSel(-1,FALSE);

	// Charge la liste des cibles
	for (m_DocChoixSupport.m_MAPpItems.MoveFirst();
		 m_DocChoixSupport.m_MAPpItems.IsValid();
		 m_DocChoixSupport.m_MAPpItems.MoveNext())
	{

		// Récup id cible
		IItem * item = m_DocChoixSupport.m_MAPpItems.GetItem();
		JUnt32 IdCible =  item->GetID();
		IdCible =  m_DocChoixSupport.m_MAPpItems.GetItem()->GetID();

		// Récup libellé 
		JString LibCible =  m_DocChoixSupport.m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();

		// Ajout info cible non exploitable
		CString TxtAvertissement = "";

		// Index de la nouvelle cible
		JUnt32 Index;

		// Voir si la cible est autorisée sur tous les terrains
		if (AuMoins1TerrainNonAutorise(IdCible, TxtAvertissement))
		{
			// Indique par (*) la non validité de la cible sur certains terrains
			CString TxtCible;
			TxtCible.Format("%s - %s","(*)", LibCible.AsJCharPtr());

			// Insére libellé cible
			Index = m_LstCibles.AddString(TxtCible); 
		}
		else
		{
			// Insére libellé cible
			Index = m_LstCibles.AddString(LibCible.AsJCharPtr()); 
		}

		// Ajoute libellé dans liste		 
		// JUnt32 Index = m_LstCibles.AddString(LibCible.AsJCharPtr()); 

		// Ajoute info ident terrain
		m_LstCibles.SetItemData(Index,IdCible); 
	}

	// Mise à jour de la liste
	m_LstCibles.UpdateWindow();
	m_LstCibles.Invalidate();

	// Les sélections en cours
	for (m_DocChoixSupport.m_MAPpItems.MoveFirst();
		 m_DocChoixSupport.m_MAPpItems.IsValid();
		 m_DocChoixSupport.m_MAPpItems.MoveNext())
	{
		// L'index du module courant
		JUnt32 InxModule = m_DocChoixSupport.GetModuleCourant();

		// Récupère le map id cibles sélectionnées pour chacun des modules
		if (InxModule >= m_DocChoixSupport.ModuleEch &&
			InxModule <= m_DocChoixSupport.ModuleDup)
		{
			// Récupère le map cibles audience du module courant
			JMap <JUnt32,JUnt32> *pMapIdCiblesSel = m_DocChoixSupport.GetMapIdCiblesAudienceSel();

			if (pMapIdCiblesSel)
			{
				// Récup id cible
				IItem * item = m_DocChoixSupport.m_MAPpItems.GetItem();
				JUnt32 IdCible =  item->GetID();
				IdCible =  m_DocChoixSupport.m_MAPpItems.GetItem()->GetID();

				// Recherche index libellé dans liste		 
				int Index = 0;
				for (JUnt32 j=0; j<m_LstCibles.GetCount(); j++)
					if (m_LstCibles.GetItemData(j) == IdCible)
						Index = j;

				// Mono-sélection
				if (m_pActivePage == m_pPageCumulativite || 
					m_pActivePage == m_pPageMapping ||
					m_pActivePage == m_pPageDuplication)
				{
					// Vérifie dans le map sélection cible
					pMapIdCiblesSel->MoveTo(IdCible);
					if (pMapIdCiblesSel->IsValid ())
					{
						// s'il existe on indique la sélection dans la liste
						int nret  = m_LstCibles.SetCurSel(Index);
						m_LstCibles.UpdateWindow();
					}
				}
				else
				// Multi-sélections
				{
					// Vérifie dans le map sélection cible
					pMapIdCiblesSel->MoveTo(IdCible);
					if (pMapIdCiblesSel->IsValid ())
					{
						// s'il existe on indique la sélection dans la liste
						m_LstCibles.SetSel(Index); 
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Requête pour savoir si cible valide sur tous les terrains sélectionnés
JBool CDlgChoixSupport::AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement)
{
	// Initialise la balise test
	JBool AuMoins1TerrainNonAutorise = false;

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Récup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_DocChoixSupport.m_Offre.m_IdSourceAudience);

	// Chaines de tous les terrains avc avertissement
	TxtAvertissement = "";

	// Boucle sur les terrains sélectionnés
	for (m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveFirst(); 
		 m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->IsValid();
		 m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveNext())
	{
		// Récup ident terrain
		const JUnt32 IdTerrain = m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->GetKey();

		// Test sur les cibles de l'offre
		if (m_DocChoixSupport.m_Offre.CibleAutorisee(IdCible, IdTerrain) == false)  
		{
			// Ce terrain n'est pas autorisé pour cette cible
			AuMoins1TerrainNonAutorise = true;

			// Récup infos terrain
			const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_DocChoixSupport.m_Offre.m_IdSourceAudience, IdTerrain);
			CString TxtTerrain;
			TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

			// Ajoute au message le terrain
			TxtAvertissement += TxtTerrain;
		}
	}

	// Renvoi si au moins un terrain non autorisé
	return AuMoins1TerrainNonAutorise;
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections population de référence
void CDlgChoixSupport::UpdateSelSegPop()
{
	// Initialise liste segment de population
	m_LstSegPop.ResetContent();

	// Crée la liste des idents contextes sélectionnés
	JSRCTables* m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Balaye tous les segments sélectionnés dans l'offre 
	for (m_DocChoixSupport.m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
		 m_DocChoixSupport.m_Offre.m_TBLIdSegmentsAudience.IsValid();
		 m_DocChoixSupport.m_Offre.m_TBLIdSegmentsAudience.MoveNext())
	{
		// Id Segment
		JUnt32 IdSeg;
		IdSeg = m_DocChoixSupport.m_Offre.m_TBLIdSegmentsAudience.GetItem();

		// Segment
		const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(IdSeg);

		// Ajoute et sélection d'office tous les segments
		m_LstSegPop.AddString(Seg->m_Libelle.AsJCharPtr());
		m_LstSegPop.SetSel(m_LstSegPop.GetCount()-1, false);

		// Sauvegarde Index Segment
		m_LstSegPop.SetItemData(m_LstSegPop.GetCount()-1, IdSeg);

		// L'index du module courant
		JUnt32 InxModule = m_DocChoixSupport.GetModuleCourant();

		// Récupère le map id cibles sélectionnées pour chacun des modules
		if (InxModule >= m_DocChoixSupport.ModuleEch &&
			InxModule <= m_DocChoixSupport.ModuleDup)
		{
			// Récupère le map id formats sélectionnées pour ce module
			JMap <JUnt32,JUnt32> *pMapIdSegmentsSel = m_DocChoixSupport.GetMapIdSegmentsAudienceSel(); 
			if (pMapIdSegmentsSel)
			{
				// Vérifie dans le map sélection segments
				pMapIdSegmentsSel->MoveTo(IdSeg);
				if (pMapIdSegmentsSel->IsValid ())
				{
					// s'il existe on indique la sélection dans l
					JUnt32 IndexSel = m_LstSegPop.GetCount()-1;
					m_LstSegPop.SetSel(m_LstSegPop.GetCount()-1,true); 
					m_LstSegPop.SetCurSel(IndexSel); 
				}
				/*
				else
					// Non sélectionné
					m_LstSegPop.SetSel(m_LstSegPop.GetCount()-1,false); 
				*/
			}
		}
	}
	m_LstSegPop.RedrawWindow(); 
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections référence affinité
void CDlgChoixSupport::UpdateSelRefAff()
{
	// Initialise liste réf affinité
	m_LstRefAff.ResetContent();

	// Récup item correspondant à l'indice
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
	// IItem * pItem = pItemConst->GetIItemByID(m_DocChoixSupport.m_Offre.m_IdItemAffinite);
	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_DocChoixSupport.m_Offre.m_IdItemAffinite);

	if(pItem)
	{
		// Ajoute et sélection d'office du seul indice affinité (pour le moment)
		m_LstRefAff.AddString(pItem->GetLabel().AsJCharPtr());
		m_LstRefAff.SetCurSel(0);
	}
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections formats (sur tous les modules)
void CDlgChoixSupport::UpdateSelFormats()
{
	// Initialise liste formats
	m_LstFormats.ResetContent();

	// Nb de formats disponible suite à l'offre
	JUnt32 NbFormatDispo = m_DocChoixSupport.m_Offre.m_TBLIdFormats.GetCount(); 

	// Charge la liste des formats
	for (m_DocChoixSupport.m_Offre.m_TBLIdFormats.MoveFirst();
		 m_DocChoixSupport.m_Offre.m_TBLIdFormats.IsValid();
		 m_DocChoixSupport.m_Offre.m_TBLIdFormats.MoveNext())
	{
		// Récup id formats
		JUnt32 IdFormat   =  m_DocChoixSupport.m_Offre.m_TBLIdFormats.GetItem();

		// Récup libellés 
		JString LblCompletFormat = "*";
		JString LblAbregeFormat  = "*";

		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
		{
			// libellé long format
			LblCompletFormat = pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat); 

			// puis le libellé abrégé format
			LblAbregeFormat = pSrcPresse->m_TBLFormats.GetLabelCourtByID(IdFormat); 
		}

		// Ajoute libellé dans liste		 
		m_LstFormats.AddString(LblCompletFormat.AsJCharPtr()); 

		// Ajoute info ident format
		m_LstFormats.SetItemData(m_LstFormats.GetCount()-1,IdFormat); 

		// L'index du module courant
		JUnt32 InxModule = m_DocChoixSupport.GetModuleCourant();

		// Récupère le map id cibles sélectionnées pour chacun des modules
		if (InxModule >= m_DocChoixSupport.ModuleEch &&
			InxModule <= m_DocChoixSupport.ModuleDup)
		{
			// Récupère le map id formats sélectionnées pour ce module
			JMap <JUnt32,JUnt32> *pMapIdFormatsSel = m_DocChoixSupport.GetMapIdFormatsSel(); 
			if (pMapIdFormatsSel)
			{
				// Vérifie dans le map sélection formats
				pMapIdFormatsSel->MoveTo(IdFormat);
				if (pMapIdFormatsSel->IsValid ())
				{
					// s'il existe on indique la sélection dans l
					JUnt32 IndexSel = m_LstFormats.GetCount()-1;
					m_LstFormats.SetSel(m_LstFormats.GetCount()-1,true); 
					m_LstFormats.SetCurSel(IndexSel); 
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Mise sélection supports dans toutes les fenêtres associés
bool CDlgChoixSupport::UpdateSelSupports()
{
	// Mise à jour sélections supports dans échelle
	if (!m_pPageEchelle->UpdateSelSupports()) return false;

	// Mise à jour sélections supports dans mapping
	if (!m_pPageMapping->UpdateSelSupports()) return false;

	// Mise à jour sélections supports dans cumulativité
	if (!m_pPageCumulativite->UpdateSelSupports()) return false;

	// Mise à jour sélections supports dans duplication
	if (!m_pPageDuplication->UpdateSelSupports()) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des indications grandeurs (pour les reprises en main)
void CDlgChoixSupport::UpdateTxtGrandeur()
{
	// Mise à jour des élmts grandeurs (s'ils existent)
	if (m_pPupitreEch->m_LstGrandeurs.GetCount() > 0)
	{
		// Attention si sélection choix support non défini
		if (!m_DocChoixSupport.IsValid()) return ;

		// Nombre de grandeurs utilisables pour les calculs
		JUnt32 NbGrandeurs = m_DocChoixSupport.GetNbGrandeurs(m_DocChoixSupport.ModuleEch);

		// Impossible de continuer si aucune grandeur sélectionnée
		if (NbGrandeurs == 0) return;

		// Chargement des grandeurs définies
		CArray <JUnt32, bool> ArrayEtatSel;
		ArrayEtatSel.SetSize(NbGrandeurs); 
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Etat sélection
			bool Etat = m_pPupitreEch->m_LstGrandeurs.GetSel(i);

			// Stocke état
			ArrayEtatSel.SetAt(i,Etat);
		}
		
		// Reinit la liste
		m_pPupitreEch->m_LstGrandeurs.ResetContent();

		// Recharge les données grandeurs
		m_pPupitreEch->LoadGrandeurs(); 

		// repositionne les états en cours
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Ancien état
			bool ExEtat = ArrayEtatSel.GetAt(i);

			// Remet Etat Sélection
			m_pPupitreEch->m_LstGrandeurs.SetSel(i,ExEtat);
		}
	}

	// Mise à jour des élmts grandeurs an abscisse mapping (s'ils existent)
	if (m_pPupitreMap->m_LstGrandAbs.GetCount() > 0)
	{
		// Attention si sélection choix support non défini
		if (!m_DocChoixSupport.IsValid()) return ;

		// Nombre de grandeurs utilisables pour les calculs
		JUnt32 NbGrandeurs = m_DocChoixSupport.GetNbGrandeurs(m_DocChoixSupport.ModuleMap);

		// Impossible de continuer si aucune grandeur sélectionnée
		if (NbGrandeurs == 0) return;

		// Chargement des grandeurs définies
		CArray <JUnt32, bool> ArrayEtatSel;
		ArrayEtatSel.SetSize(NbGrandeurs); 
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Etat sélection
			bool Etat = m_pPupitreMap->m_LstGrandAbs.GetSel(i);

			// Stocke état
			ArrayEtatSel.SetAt(i,Etat);
		}
		
		// Reinit la liste
		m_pPupitreMap->m_LstGrandAbs.ResetContent();

		// Recharge les données grandeurs
		m_pPupitreMap->LoadGrandeursAbs(); 

		// repositionne les états en cours
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Ancien état
			bool ExEtat = ArrayEtatSel.GetAt(i);

			// Remet Etat Sélection
			m_pPupitreMap->m_LstGrandAbs.SetSel(i,ExEtat);
		}
	}

	// Mise à jour des élmts grandeurs en ordonnée mapping (s'ils existent)
	if (m_pPupitreMap->m_LstGrandOrd.GetCount() > 0)
	{
		// Attention si sélection choix support non défini
		if (!m_DocChoixSupport.IsValid()) return ;

		// Nombre de grandeurs utilisables pour les calculs
		JUnt32 NbGrandeurs = m_DocChoixSupport.GetNbGrandeurs(m_DocChoixSupport.ModuleMap);

		// Impossible de continuer si aucune grandeur sélectionnée
		if (NbGrandeurs == 0) return;

		// Chargement des grandeurs définies
		CArray <JUnt32, bool> ArrayEtatSel;
		ArrayEtatSel.SetSize(NbGrandeurs); 
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Etat sélection
			bool Etat = m_pPupitreMap->m_LstGrandOrd.GetSel(i);

			// Stocke état
			ArrayEtatSel.SetAt(i,Etat);
		}
		
		// Reinit la liste
		m_pPupitreMap->m_LstGrandOrd.ResetContent();

		// Recharge les données grandeurs
		m_pPupitreMap->LoadGrandeursOrd(); 

		// repositionne les états en cours
		for (JUnt32 i=0; i<NbGrandeurs; i++)
		{
			// Ancien état
			bool ExEtat = (bool)ArrayEtatSel.GetAt(i);

			// Remet Etat Sélection
			m_pPupitreMap->m_LstGrandOrd.SetSel(i,ExEtat);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des indications grandeurs (pour les reprises en main)
void CDlgChoixSupport::UpdateTxtHeader()
{
}


////////////////////////////////////////////////////////////////////////
// Mise à jour des fenêtres associés
bool CDlgChoixSupport::UpdateFenetreFille()
{
	// Paramètres modifiés
	ParamCalculModif();

	if (m_pActivePage == m_pPageEchelle)
	{	
		// Mise à jour fenêtre échelle
		if (!m_pPageEchelle->UpdateResultEchelle(true)) return false;
	}
	else if (m_pActivePage == m_pPageMapping)
	{
		// Mise à jour fenêtre mapping
		if (!m_pPageMapping->UpdateResultMapping(true)) return false;
	}
	else if (m_pActivePage == m_pPageCumulativite)
	{
		// Mise à jour fenêtre cumulativité
		if (!m_pPageCumulativite->UpdateResultCum(true)) return false;
	}
	else if (m_pActivePage == m_pPageDuplication)
	{
		// Mise à jour fenêtre duplication
		if (!m_pPageDuplication->UpdateResultDup(true)) return false;
	}
    
	// Mise à jour des sélecteurs
	UpdateSelecteur();

	return true;
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des résultats
bool CDlgChoixSupport::UpdateResultat(JBool CalculResult)
{
	if (m_pActivePage == m_pPageEchelle)
	{
		// Mise à jour résultat échelle
		if (!m_pPageEchelle->UpdateResultat(CalculResult)) return false;

		// Calcul effectué pour échelles
		m_DocChoixSupport.m_ParamEchModif = false;
	}
	else if (m_pActivePage == m_pPageMapping)
	{
		// Mise à jour résultat mapping
		if (!m_pPageMapping->UpdateResultMapping(CalculResult)) return false;

		// Calcul effectué pour mapping
		m_DocChoixSupport.m_ParamMapModif = false;
	}
	else if (m_pActivePage == m_pPageCumulativite)
	{
		// Mise à jour résultat cumulativité
		if (!m_pPageCumulativite->UpdateResultat(CalculResult)) return false;

		// Mise à jour du graphe associé
		m_pPageCumulativite->UpdateGraphe();

		// Calcul effectué pour cumulativité
		m_DocChoixSupport.m_ParamCumModif = false;
	}
	else if (m_pActivePage == m_pPageDuplication)
	{
		// Mise à jour résultat duplication
		if (!m_pPageDuplication->UpdateResultat(CalculResult)) return false;

		// Calcul effectué pour duplication
		m_DocChoixSupport.m_ParamDupModif = false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Reprises en main
void CDlgChoixSupport::OnCsOptRepriseenmain()
{
	// Repositionne option reprise en main
	m_DocChoixSupport.m_bRepriseEnMain = !m_DocChoixSupport.m_bRepriseEnMain;

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Reprises en main
void CDlgChoixSupport::OnUpdateCsOptRepriseenmain(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option reprise en main
		m_DocChoixSupport.m_bRepriseEnMain ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);

}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option LNM Magazine
void CDlgChoixSupport::OnCsOptLnmmagazine()
{
	// Repositionne option LNM Magazine
	m_DocChoixSupport.m_bLNMMag = !m_DocChoixSupport.m_bLNMMag;

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate LNM Magazine
void CDlgChoixSupport::OnUpdateCsOptLnmmagazine(CCmdUI *pCmdUI)
{
	// Non valide pour le moment
	pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option LDP Quotidien
void CDlgChoixSupport::OnCsOptLdpquotidien()
{
	// Repositionne option LDP Quotidien
	m_DocChoixSupport.m_bLDPQtd = !m_DocChoixSupport.m_bLDPQtd;

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate LDP Quotidien
void CDlgChoixSupport::OnUpdateCsOptLdpquotidien(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option LDP Quotidien
		m_DocChoixSupport.m_bLDPQtd ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										    Gestion mode de calcul des familles			
//
void CDlgChoixSupport::OnCsOptFamCumul()
{
	// Repositionne option Cumul Famille
	m_DocChoixSupport.m_CumulFamille = true;

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  	
}

void CDlgChoixSupport::OnUpdateCsOptFamCumul(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option cumul famille ou pas
		m_DocChoixSupport.m_CumulFamille ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CDlgChoixSupport::OnCsOptFamMoyenne()
{
	// Repositionne option Cumul Famille
	m_DocChoixSupport.m_CumulFamille = false;

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  	
}

void CDlgChoixSupport::OnUpdateCsOptFamMoyenne(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option cumul famille ou pas
		m_DocChoixSupport.m_CumulFamille ? pCmdUI->SetCheck(FALSE) : pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Modélisation macro-probabiliste &brute / Moteur Abaque
void CDlgChoixSupport::OnCsModel()
{
	// Repositionne Option Moteur abaque
	m_DocChoixSupport.m_MoteurPresse.SetType(CMoteurPresse::ABAQUE);

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Appel mise à jour type moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_TYPEMOTEUR);
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Modélisation macro-probabiliste &brute
void CDlgChoixSupport::OnUpdateCsModel(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option type moteur abaque
		(m_DocChoixSupport.m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Modélisation macro-probabiliste é&talonnée / Moteur Abaque étalonné
void CDlgChoixSupport::OnCsModeletal()
{
	// Repositionne Option Moteur abaque étalonné
	m_DocChoixSupport.m_MoteurPresse.SetType(CMoteurPresse::ABAQUE_ETAL);
	
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Appel mise à jour type moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_TYPEMOTEUR);
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Option Modélisation macro-probabiliste étalonnée
void CDlgChoixSupport::OnUpdateCsModeletal(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé (attention valide uniquement pour échelle et mapping)
		(m_DocChoixSupport.m_MoteurPresse.IsReady() &&
			(m_pActivePage == m_pPageEchelle || m_pActivePage == m_pPageMapping)) 
			    ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		if(m_DocChoixSupport.m_MoteurPresse.IsReady())
		{
			CKeyTerrain keyTerrain;
			m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->MoveFirst();

			if (m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->IsValid())
			{
				//m_DocChoixSupport.m_MapIdTerrainsAudienceSel.MoveFirst();
				keyTerrain.Set(m_DocChoixSupport.GetIdSrcAudienceSel(), m_DocChoixSupport.GetMapIdTerrainsAudienceSel()->GetKey());

				(m_DocChoixSupport.m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
			}

			// Voir si étude permet ce type de moteur
			bool Can = m_DocChoixSupport.m_MoteurPresse.CanAbaque(keyTerrain);

			if(! AfxGetApp()->GetProfileInt("Moteur", "VersionJFC", 0) &&
				(m_pActivePage == m_pPageEchelle || m_pActivePage == m_pPageMapping))
					pCmdUI->Enable(m_DocChoixSupport.m_MoteurPresse.CanAbaque(keyTerrain));

		}
		// Positionne option type moteur abaque étalonné
		//(m_DocChoixSupport.m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Probabilités individuelles / Moteur Proba
void CDlgChoixSupport::OnCsProbabilit()
{
	// Repositionne Option Moteur Probabilisation
	m_DocChoixSupport.m_MoteurPresse.SetType(CMoteurPresse::PROBA);

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Appel mise à jour type moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_TYPEMOTEUR);

}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Option Probabilités individuelles
void CDlgChoixSupport::OnUpdateCsProbabilit(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady()  ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option type moteur probabilisation
		(m_DocChoixSupport.m_MoteurPresse.GetType() == CMoteurPresse::PROBA) ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);

}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Tarif Plaquette
void CDlgChoixSupport::OnCsBasetarifplaq()
{
	AfxMessageBox("A FAIRE / Option Tarif Plaquette");

	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Option Tarif Plaquette
void CDlgChoixSupport::OnUpdateCsBasetarifplaq(CCmdUI *pCmdUI)
{
	// A FAIRE
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification Option Tarif Négocié
void CDlgChoixSupport::OnCsBasetarifnego()
{
	AfxMessageBox("A FAIRE / Option Tarif Négocié");
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 
}	

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Option Tarif Négocié
void CDlgChoixSupport::OnUpdateCsBasetarifnego(CCmdUI *pCmdUI)
{
}

///////////////////////////////////////////////////////////////////////////////////////////
// OnUpdate Option Tarif Négocié
void CDlgChoixSupport::OnUpdateCsFiltreAudience(CCmdUI *pCmdUI)
{
	if (m_pActivePage && m_DocChoixSupport.IsValid())
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////
void CDlgChoixSupport::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	//	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

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

///////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections fenêtre active
void CDlgChoixSupport::MajSelFenetreCS()
{
	// Mise à jour sélection terrains
	UpdateSelTerrains();

	// Mise à jour sélection cibles
	UpdateSelCibles();

	// Mise à jour sélection segment de population
	UpdateSelSegPop();

	// Mise à jour sélection réf affinité
	UpdateSelRefAff();

	// Mise à jour sélection format pour module actif
	UpdateSelFormats();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour entete fenêtre active
void CDlgChoixSupport::MajTitreFenetreCS()
{
	// Init titre fenêtre
	CString TxtTitreFen = "Analyse offre ";

	if (m_pActivePage == m_pPageEchelle)
		// Mise à jour titre fenêtre en mode Echelles
		TxtTitreFen +=  "- Echelles - ";

	else if (m_pActivePage == m_pPageMapping)
		// Mise à jour titre fenêtre en mode Mapping
		TxtTitreFen +=  "- Mapping - ";

	else if (m_pActivePage == m_pPageCumulativite)
		// Mise à jour titre fenêtre en mode Cumulativité
		TxtTitreFen +=  "- Cumulativité - ";

	else if (m_pActivePage == m_pPageDuplication)
		// Mise à jour titre fenêtre en mode Duplication
		TxtTitreFen +=  "- Duplication - ";	

	// Ajoute nom de l'étude
	TxtTitreFen += m_DocChoixSupport.m_KEYPlateau.GetLabel();	

	// Affichage sur la barre titre fenêtre
	SetWindowText(TxtTitreFen);
}


///////////////////////////////////////////////////////////////////////////////////////////
void CDlgChoixSupport::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);

	//Menu choix moteur et reprise en main
	if(nItemID == 2)
	{

		CMenu *pMenuChoixOffre;
		pMenuChoixOffre = this->GetMenu();
		if (pMenuChoixOffre == 0) return;

		// Récup le menu hypothèses
		CMenu *pMenu;
		pMenu = pMenuChoixOffre->GetSubMenu(nItemID);
		ASSERT(pMenu);

		// Récup ss menu choix moteur
		CMenu *pMenuChoixMoteur;
		pMenuChoixMoteur = pMenu->GetSubMenu(2);
		ASSERT(pMenuChoixMoteur);

		// A FAIRE / Test icon menu
		/*
		CBitmap m_Bitmap;
		LONG Taille = GetSystemMetrics(SM_CXMENUCHECK);   // ->GetMenuCheckMarkDimensions();
		if(m_Bitmap.LoadBitmap(IDB_CS_DOWN))
		{
			if (pMenuChoixMoteur->SetMenuItemBitmaps(IDM_CS_OPT_LNMMAGAZINE, MF_BYCOMMAND,&m_Bitmap, &m_Bitmap))
				AfxMessageBox("OK");
			else
				AfxMessageBox("PAS OK");
		}
		*/
		
		//Il faut creer la clé registre Moteur/VersionJFC == 1 pour acceder totalement à ces menu
		if(! AfxGetApp()->GetProfileInt("Moteur", "VersionJFC", 0) )
		{
			pMenuChoixMoteur->DeleteMenu(IDM_CS_MODEL, MF_BYCOMMAND);
			pMenu->DeleteMenu(IDM_CS_OPT_REPRISEENMAIN, MF_BYCOMMAND);
		}
	}

}

// Sauvegarde paramètres en sortie de Choix Supports
void CDlgChoixSupport::OnDestroy()
{
	// Sauvegarde derniière page active
	AfxGetApp()->WriteProfileInt("Options\\ChoixSupport","ActivePageChoixSupport", GetCheckedRadioButton(IDC_CS_ECHELLES, IDC_CS_DUPLICATION));

	// Sauvegarde dernière couleur interface sélectionnée
	AfxGetApp()->WriteProfileInt("Options\\ChoixSupport","ColorChoixSupport",CATPColors::GetAppColor(CATPColors::COLORCHOIXSUPPORTS));

	// Sauvegarde nb décimales affichage résultat
	// Nb décimales coût
	// Sauvegarde dernière couleur interface sélectionnée
//	AfxGetApp()->WriteProfileInt("Options","NbDecimaleCout",m_DocChoixSupport.m_NbDecCout);

	// Destruction fenêtre Echelles
	if(m_pPageEchelle)
	{
		m_pPageEchelle->DestroyWindow();
		delete m_pPageEchelle;
	}

	// Destruction fenêtre Pupitre Echelles
	if(m_pPupitreEch)
	{
		m_pPupitreEch->DestroyWindow();
		delete m_pPupitreEch;
	}

	// Destruction fenêtre Mapping
	if(m_pPageMapping)
	{
		m_pPageMapping->DestroyWindow();
		delete m_pPageMapping;
	}

	// Destruction fenêtre Pupitre Mapping
	if(m_pPupitreMap)
	{
		m_pPupitreMap->DestroyWindow();
		delete m_pPupitreMap;
	}

	// Destruction fenêtre Cumulativité
	if(m_pPageCumulativite)
	{
		m_pPageCumulativite->DestroyWindow();
		delete m_pPageCumulativite;
	}

	// Destruction fenêtre Pupitre Cumulativité
	if(m_pPupitreCum)
	{
		m_pPupitreCum->DestroyWindow();
		delete m_pPupitreCum;
	}

	// Destruction fenêtre Duplication
	if(m_pPageDuplication)
	{
		m_pPageDuplication->DestroyWindow();
		delete m_pPageDuplication;
	}

	// Destruction fenêtre Pupitre Mapping
	if(m_pPupitreDup)
	{
		m_pPupitreDup->DestroyWindow();
		delete m_pPupitreDup;
	}
	
	
	CString strText;
    BOOL bIconic, bMaximized;

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
 
	strText.Format("%04d %04d %04d %04d",
                   wndpl.rcNormalPosition.left,
                   wndpl.rcNormalPosition.top,
                   wndpl.rcNormalPosition.right,
                   wndpl.rcNormalPosition.bottom);
 
	AfxGetApp()->WriteProfileString("Options\\ChoixSupport", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\ChoixSupport", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\ChoixSupport", "Max", bMaximized);

	JFCDialog::OnDestroy();
}

void CDlgChoixSupport::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\ChoixSupport", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left = atoi((const char*) strText);
            rect.top = atoi((const char*) strText + 5);
            rect.right = atoi((const char*) strText + 10);
            rect.bottom = atoi((const char*) strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\ChoixSupport", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\ChoixSupport", "Max", 0);   
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

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection mode affichage Familles
void CDlgChoixSupport::OnBnClickedCsLabelFamilles()
{
	// Update Flag affichage mode familles
	m_DocChoixSupport.m_AFFModeFamilles = ! m_DocChoixSupport.m_AFFModeFamilles;
		
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Mise à jour des formats
	UpdateAllViews(UPDATE_SEL_FORMAT);

	// Mise à jour visibilité support
	m_DocChoixSupport.UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Mise à jour des différentes fenêtres associés
	UpdateFenetreFille();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection mode affichage Supports
void CDlgChoixSupport::OnBnClickedCsLabelSupports()
{
	// Update Flag affichage mode familles
	m_DocChoixSupport.m_AFFModeSupports = ! m_DocChoixSupport.m_AFFModeSupports;
		
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Mise à jour des formats
	UpdateAllViews(UPDATE_SEL_FORMAT);

	// Mise à jour visibilité support
	m_DocChoixSupport.UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Mise à jour des différentes fenêtres associés
	UpdateFenetreFille();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection boite dialogue filtre d'audience
void CDlgChoixSupport::OnBnClickedCsFiltreAudience()
{
	// Ouverture boite de dialogue filtre d'audience
	m_DocChoixSupport.m_AFFFiltreAudience = ! m_DocChoixSupport.m_AFFFiltreAudience;
	m_DlgFiltresAudience.Update();
	
	// Le document a changé
	m_DocChoixSupport.SetModifiedFlag(TRUE); 

	// Mise à jour des différentes fenêtres associés
	UpdateFenetreFille();
}



///////////////////////////////////////////////////////////////////////////////////////////
/* Remplacer par option menu poppup sur supports
// Affichage Informations Supports
void CDlgChoixSupport::OnBnClickedCsLabelFichesupport()
{
	AfxMessageBox("Affichage Informations Supports : Module en cours de développement");
}
*/

///////////////////////////////////////////////////////////////////////////////////////////
// Passerelle pour afficher Creuset via le plateau en cours
void CDlgChoixSupport::OnBnClickedCsCreuset()
{
	AfxMessageBox("A FAIRE / Passerelle Creuset");
}


///////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des sélecteurs (Filtre audience, Contexte lecture etc ...)
void CDlgChoixSupport::UpdateSelecteur()
{
	// Mise à jour des sélections mode affichage
	m_BtnFamilles.SetCheck(m_DocChoixSupport.m_AFFModeFamilles);
	m_BtnSupports.SetCheck(m_DocChoixSupport.m_AFFModeSupports);

	// Mise à jour du sélecteur filtre d'audience
	m_BtnFiltreAudience.SetCheck(m_DocChoixSupport.m_AFFFiltreAudience);

	// On se positionne sur l'élmt filtre d'audience pour le module courant
	JUnt32 IdFiltre = m_DocChoixSupport.GetIdFiltreAudienceSel();
	if(IdFiltre != 1)
		m_BtnFiltreAudience.SetBitmaps(IDB_CS_LEDVERT, RGB(0,128,128));
	else
		m_BtnFiltreAudience.SetBitmaps(IDB_CS_LEDNEUTRE, RGB(0,128,128));

	// Bouton passage au Creuset
	m_BtnCreuset.SetIcon(IDI_CS_FLECHE);

}

void CDlgChoixSupport::OnOptionsLDP30j()
{
	// Repositionne option LDP 30 jours
	m_DocChoixSupport.m_bLDP30J = !m_DocChoixSupport.m_bLDP30J;

	// Appel mise à jour options moteur
	m_DocChoixSupport.UpdateAllViews(UPDATE_OPTIONSMOTEUR);  
}

void CDlgChoixSupport::OnUpdateOptionsLDP30j(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);

	/* En attente !!!
	if (m_pActivePage && m_DocChoixSupport.IsValid())
	{
		// Valide si moteur déjà initialisé
		m_DocChoixSupport.m_MoteurPresse.IsReady() ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);

		// Positionne option LDP 30 jours
		m_DocChoixSupport.m_bLDP30J ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
	*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Changement des couleurs d'environnement Choix Supports
void CDlgChoixSupport::OnSysColorChange()
{
	JFCDialog::OnSysColorChange();
	SetColors();

	/*
	m_pPageEchelle->SetColors();
	m_pPageMapping->SetColors();
	m_pPageCumulativite->SetColors();
	m_pPageDuplication->SetColors();

	// Définit la couleur pour toutes les autres pupitres pages
	m_pPupitreEch->SetColors();
	m_pPupitreMap->SetColors();
	m_pPupitreCum->SetColors();
	m_pPupitreDup->SetColors();
	*/
	m_DocChoixSupport.UpdateAllViews(UPDATE_COLOR);  
}

void CDlgChoixSupport::OnCsOptionMoteur()
{
	m_DocChoixSupport.m_AFFOptionsMoteur = ! m_DocChoixSupport.m_AFFOptionsMoteur;
	m_DlgChoixMoteur.Update();
}

void CDlgChoixSupport::OnUpdateCsOptionMoteur(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_DocChoixSupport.m_MoteurPresse.IsReady()) ? TRUE : FALSE);
	m_DocChoixSupport.m_AFFOptionsMoteur ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsNInsertion(CCmdUI *pCmdUI)
{
	// Dévalide toutes les sélections
	GetMenu()->CheckMenuItem(IDM_CS_1_INSERTION,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_2_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_3_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_4_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_5_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_6_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_7_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_8_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_9_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_10_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_12_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_24_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_53_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_105_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_365_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_CS_MAX_INSERTIONS,MF_BYCOMMAND|MF_UNCHECKED);

	if(m_DocChoixSupport.GetNbSupports() > 0 && ((m_pActivePage == m_pPageEchelle) || (m_pActivePage == m_pPageCumulativite)))
	{
		// Valide le sélection d'insertion
		pCmdUI->Enable(TRUE);
	}
	else
		// Menu insertion non valide
		 pCmdUI->Enable(FALSE);

	JUnt32 NbInsSelect = 1;
	if (m_pActivePage == m_pPageEchelle)
		NbInsSelect = m_pPageEchelle->GetNbInsertions();
	else
		NbInsSelect = m_pPageCumulativite->GetNbInsertions();

	// Mise à jour du ss-menu sélectionné
	//switch (m_pPageEchelle->GetNbInsertions())
	switch (NbInsSelect)
	{
		case 1    :
			GetMenu()->CheckMenuItem(IDM_CS_1_INSERTION,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 2    :
			GetMenu()->CheckMenuItem(IDM_CS_2_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 3    :
			GetMenu()->CheckMenuItem(IDM_CS_3_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 4    :
			GetMenu()->CheckMenuItem(IDM_CS_4_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 5    :
			GetMenu()->CheckMenuItem(IDM_CS_5_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 6    :
			GetMenu()->CheckMenuItem(IDM_CS_6_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 7    :
			GetMenu()->CheckMenuItem(IDM_CS_7_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 8    :
			GetMenu()->CheckMenuItem(IDM_CS_8_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 9    :
			GetMenu()->CheckMenuItem(IDM_CS_9_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 10   :
			GetMenu()->CheckMenuItem(IDM_CS_10_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		// Nb insertion mensuel / an
		case 12   :
			GetMenu()->CheckMenuItem(IDM_CS_12_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		// Nb insertion bi-mensuel / an
		case 24   :
			GetMenu()->CheckMenuItem(IDM_CS_24_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		// Nb insertion hebdomadaire / an
		case 53   :
			GetMenu()->CheckMenuItem(IDM_CS_53_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		// Nb insertion bi-hebdomadaire / an
		case 105   :
			GetMenu()->CheckMenuItem(IDM_CS_105_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		// Nb insertion quotidien / an				
		case 365   :
			GetMenu()->CheckMenuItem(IDM_CS_365_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 1000 :
			GetMenu()->CheckMenuItem(IDM_CS_MAX_INSERTIONS,MF_BYCOMMAND|MF_CHECKED);
			break;
		default :
			GetMenu()->CheckMenuItem(IDM_CS_1_INSERTION,MF_BYCOMMAND|MF_CHECKED);
			break;
	}	
}


///////////////////////////////////////////////////////////////////////////////////
// Mode 0 décimale pour l'affichage des coûts
void CDlgChoixSupport::OnCsAucunedecimale()
{
	// Sauve nouve nb décimale pour coût
	m_DocChoixSupport.m_NbDecCout = 0;

	// Update résultat suite à la modif paramètre nb insertions
	m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RESULT);
}

///////////////////////////////////////////////////////////////////////////////////
// Mode 1 décimale pour l'affichage des coûts
void CDlgChoixSupport::OnCs1decimale()
{
	// Sauve nouve nb décimale pour coût
	m_DocChoixSupport.m_NbDecCout = 1;

	// Update résultat suite à la modif paramètre nb insertions
	m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RESULT);
}

///////////////////////////////////////////////////////////////////////////////////
// Mode 2 décimale pour l'affichage des coûts
void CDlgChoixSupport::OnCs2decimale()
{
	// Sauve nouve nb décimale pour coût
	m_DocChoixSupport.m_NbDecCout = 2;

	// Update résultat suite à la modif paramètre nb insertions
	m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RESULT);
}

///////////////////////////////////////////////////////////////////////////////////
// Update affichage mode décimale pour les coûts coûts
void CDlgChoixSupport::OnUpdateNbddecimale(CCmdUI *pCmdUI)
{
	UINT nEnable = (m_DocChoixSupport.GetNbSupports() > 0)? MF_ENABLED : MF_DISABLED;

	// on active/desctive les menus de choix de decimales en fonction du nbr de supports
	GetMenu()->EnableMenuItem(ID_CS_NBD_AUCUNEDECIMALE, MF_BYCOMMAND | nEnable);
	GetMenu()->EnableMenuItem(ID_CS_NBD_1DECIMALE, MF_BYCOMMAND | nEnable);
	GetMenu()->EnableMenuItem(ID_CS_NBD_2DECIMALE, MF_BYCOMMAND | nEnable);

	// Dévalide toutes les sélections
	GetMenu()->CheckMenuItem(ID_CS_NBD_AUCUNEDECIMALE,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_CS_NBD_1DECIMALE,MF_BYCOMMAND|MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_CS_NBD_2DECIMALE,MF_BYCOMMAND|MF_UNCHECKED);
	
	// Mise à jour du ss-menu sélectionné
	switch (m_DocChoixSupport.m_NbDecCout)
	{
		case 0    :
			GetMenu()->CheckMenuItem(ID_CS_NBD_AUCUNEDECIMALE,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 1    :
			GetMenu()->CheckMenuItem(ID_CS_NBD_1DECIMALE,MF_BYCOMMAND|MF_CHECKED);
			break;
		case 2    :
			GetMenu()->CheckMenuItem(ID_CS_NBD_2DECIMALE,MF_BYCOMMAND|MF_CHECKED);
			break;
		default :
			GetMenu()->CheckMenuItem(ID_CS_NBD_AUCUNEDECIMALE,MF_BYCOMMAND|MF_CHECKED);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Visibilité des colonnes rang résultats
void CDlgChoixSupport::OnCsRangColVisible()
{
	// Mise à jour Flag Affichage Rang Résultat
	m_DocChoixSupport.m_AFFRangResultatEch = ! m_DocChoixSupport.m_AFFRangResultatEch;

	// Update affichage colonne
	m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RANG_RESULT);
}

/////////////////////////////////////////////////////////////////////////////////
// Mise ss Menu Visibilité Rang résultats
void CDlgChoixSupport::OnUpdateCsRangColVisible(CCmdUI *pCmdUI)
{
	// Mise à jour mode affichage rangs
	( m_DocChoixSupport.m_AFFRangResultatEch && m_DocChoixSupport.GetNbSupports() > 0) ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);

	// Active ou désactive affichage des rangs	
	((m_pActivePage == m_pPageEchelle) 
		&& (m_DocChoixSupport.m_OrdreEnteteEch[m_pPageEchelle->GetNbHeaderRow()-1] == m_pPageEchelle->IDGRANDEUR)
		&& (m_DocChoixSupport.GetNbSupports() > 0)) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////////
// Mise ss Menu Visibilité Moyenne Résultat sur une colonne grandeur
void CDlgChoixSupport::OnCsMoyenneVisible()
{
	// Mise à jour Flag Affichage Moyenne Résultat
	m_DocChoixSupport.m_AFFMoyenneResultatEch = ! m_DocChoixSupport.m_AFFMoyenneResultatEch;

	// Update affichage colonne
	m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_MOYENNE);
}

void CDlgChoixSupport::OnUpdateCsMoyenneVisible(CCmdUI *pCmdUI)
{
	// Mise à jour mode affichage moyenne
	(m_DocChoixSupport.m_AFFMoyenneResultatEch) ? pCmdUI->SetCheck() : pCmdUI->SetCheck(FALSE);

	// Active ou désactive affichage de la moyenne (si module échlle et grandeur en header position 0)
	((m_pActivePage == m_pPageEchelle) 
		&& (m_DocChoixSupport.GetNbSupports() > 0)) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////
// Sélection des rangs à afficher
void CDlgChoixSupport::OnCsAffichageRangResult()
{
	// Création objet fenêtre dialog rangs à afficher
	CDlgRangResult DlgRang(this,&m_DocChoixSupport);

	// Ouverture Sélection Boite de dialogue 
	if(DlgRang.DoModal() == IDOK)
	{
		// Mise à jour des rangs affichés
		m_DocChoixSupport.UpdateAllViews(UPDATE_AFFICH_RANG_RESULT);
	}
}

///////////////////////////////// EXPORTS DANS LE PRESSE PAPIER ////////////////////////////////
//
// Export Echelle dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierEch()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageEchelle->Clipboard(false);
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierEch(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Echelle Par Cible dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierEchParcible()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageEchelle->Clipboard(true);
}

// Validation ou pas Export Echelle Par Cible dans le presse papier
void CDlgChoixSupport::OnUpdateCsExporterPressepapierEchParcible(CCmdUI *pCmdUI)
{
	// A remettre une fois la macro refaite
	pCmdUI->Enable(FALSE);
	// (m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


// Impression des résultats échelle
void CDlgChoixSupport::OnCsPrintEchelle()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageEchelle->PrintExcel(false);
}

// Autoristaion impression des résultats échelle
void CDlgChoixSupport::OnUpdateCsPrintEchelle(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Impression des résultats échelle par cibles
void CDlgChoixSupport::OnCsPrintEchParcible()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageEchelle->PrintExcel(true);
}

// Autorisation impression des résultats échelle par cibles
void CDlgChoixSupport::OnUpdateCsPrintEchParcible(CCmdUI *pCmdUI)
{
	// A remettre une fois la macro refaite
	// pCmdUI->Enable(FALSE);
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


// Export Resultat Mapping dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierMap()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageMapping->ClipboardResultat();
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierMap(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Resultat Mapping dans le presse papier
void CDlgChoixSupport::OnCsPrintMapping()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageMapping->PrintExcel();
}

void CDlgChoixSupport::OnUpdateCsPrintMapping(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Graphique Mapping dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierGraphmapping()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageMapping->ClipboardGraphique();
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierGraphmapping(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


// Export Cumulativité dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierCum()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Sélectionne tout le tableau
	// m_pPageCumulativite->m_GridCumulativite.SetSelection(); 

	// Lance le clipboard
	m_pPageCumulativite->Clipboard();
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierCum(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}



////////////////////////////////////////////////////////////////////////
//
void CDlgChoixSupport::OnCsExporterPressepapierGraphcumul()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Lance le clipboard
	m_pPageCumulativite->ClipboardGraphique();
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierGraphcumul(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_pPageCumulativite->GetNbCourbeCum() > 0) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Cumulativité dans le presse papier
void CDlgChoixSupport::OnCsPrintCumulation()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Sélectionne tout le tableau
	// m_pPageCumulativite->m_GridCumulativite.SetSelection(); 

	// Lance le clipboard
	m_pPageCumulativite->PrintExcel();
}

void CDlgChoixSupport::OnUpdateCsPrintCumulation(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Duplication dans le presse papier
void CDlgChoixSupport::OnCsExporterPressepapierDup()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Sélectionne tout le tableau
	// m_pPageDuplication->m_GridDuplication.SetSelection(); 

	// Lance le clipboard
	m_pPageDuplication->Clipboard();
}

void CDlgChoixSupport::OnUpdateCsExporterPressepapierDup(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageDuplication && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);	
}

// Export Duplication dans le presse papier
void CDlgChoixSupport::OnCsPrintDuplication()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}
	
	// Sélectionne tout le tableau
	// m_pPageDuplication->m_GridDuplication.SetSelection(); 

	// Lance le clipboard
	m_pPageDuplication->PrintExcel();
}

void CDlgChoixSupport::OnUpdateCsPrintDuplication(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageDuplication && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);	
}

/////////////////////////////////// EXPORT EXCEL (OUVRIR) ////////////////////////////////////
//
// export excel (ouverture dans Excel)
void CDlgChoixSupport::OnCsExporterExcelEch()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}

	// lance la macro en ouvrant Excel
	m_pPageEchelle->OpenExcel(false);
}

void CDlgChoixSupport::OnUpdateCsExporterExcelEch(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CDlgChoixSupport::OnCsExporterExcelMap()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}

	// lance la macro en ouvrant Excel
	m_pPageMapping->OpenExcel();
}

void CDlgChoixSupport::OnUpdateCsExporterExcelMap(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CDlgChoixSupport::OnCsExporterExcelCum()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}

	// lance la macro en ouvrant Excel
	m_pPageCumulativite->OpenExcel();
}

void CDlgChoixSupport::OnUpdateCsExporterExcelCum(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CDlgChoixSupport::OnCsExporterExcelDup()
{
	if (!m_DocChoixSupport.m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}

	// lance la macro en ouvrant Excel
	m_pPageDuplication->OpenExcel();
}

void CDlgChoixSupport::OnUpdateCsExporterExcelDup(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageDuplication && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);	
}

/////////////////////////////////// EXPORT DANS UN FICHIER ////////////////////////////////////
//
// Export Echelle dans un fichier
void CDlgChoixSupport::OnCsExporterFichierEch()
{
	m_pPageEchelle->ExportResultat(false); 
}

// Autorisation ou pas de l'export échelle par cible dans fichier
void CDlgChoixSupport::OnUpdateCsExporterFichierEch(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Echelle Par Cible dans un fichier
void CDlgChoixSupport::OnCsExporterFichierEchParcible()
{
	m_pPageEchelle->ExportResultat(true); 
}

// Autorisation ou pas de l'export échelle par cible dans fichier
void CDlgChoixSupport::OnUpdateCsExporterFichierEchParcible(CCmdUI *pCmdUI)
{
	// A remettre une fois la macro refaite
	pCmdUI->Enable(FALSE);
	// (m_pActivePage == m_pPageEchelle && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Resultat Mapping dans un fichier
void CDlgChoixSupport::OnCsExporterFichierMap()
{
	// Export du tableau
	m_pPageMapping->ExportResultat(); 
}

void CDlgChoixSupport::OnUpdateCsExporterFichierMap(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Resultat Mapping dans un fichier
void CDlgChoixSupport::OnCsExporterFichierGraphmapping()
{
	// Export du graphique
	m_pPageMapping->ExportGraphique(); 
}

void CDlgChoixSupport::OnUpdateCsExporterFichierGraphmapping(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageMapping && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Export Cumulativité dans un fichier
void CDlgChoixSupport::OnCsExporterFichierCum()
{
	m_pPageCumulativite->ExportResultat(); 
}

void CDlgChoixSupport::OnUpdateCsExporterFichierCum(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


////////////////////////////////////////////////////////////////////////////////
// Export fichier du graphique cumulation
void CDlgChoixSupport::OnCsExporterFichierGraphcumul()
{
	// Export du graphique
	m_pPageCumulativite->ExportGraphique(); 
}

////////////////////////////////////////////////////////////////////////////////
// Update export fichier du graphique cumulation
void CDlgChoixSupport::OnUpdateCsExporterFichierGraphcumul(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageCumulativite && m_pPageCumulativite->GetNbCourbeCum()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


// Export Duplication dans un fichier
void CDlgChoixSupport::OnCsExporterFichierDup()
{
	m_pPageDuplication->ExportResultat(); 
}

void CDlgChoixSupport::OnUpdateCsExporterFichierDup(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageDuplication && m_DocChoixSupport.m_MapIdSupportsSel.GetCount()) ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CDlgChoixSupport::OnUpdateAffichageRangResult(CCmdUI *pCmdUI)
{
	(m_pActivePage == m_pPageEchelle && m_DocChoixSupport.GetNbSupports())? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

// Traitement Timer
void CDlgChoixSupport::OnTimer(UINT nIDEvent)
{
	FlagResize = !FlagResize;
	CDialog::OnTimer(nIDEvent);
}

void CDlgChoixSupport::OnSizing(UINT fwSide, LPRECT pRect)
{
	JFCDialog::OnSizing(fwSide, pRect);
}

BOOL CDlgChoixSupport::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		long ValParam = pMsg->wParam; 
		if(ValParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identication de la couleur en cours
/*  Rappel des couleurs dispos
		BOTTLE_GREEN		=	0,
		EMERAUDE			=	1,
		BLEU_PASTEL			=	2,
		OCEAN				=   3,
		LILAS				=   4,
		SAHARA				=	5,
		TERRE_SIENNE		=   6,
		RUBIS				=   7,
		GREY_MOUSE			=	8,
		ROSE				=	9,
		ROSE_KAKI			=  10,
		ROSE_GRIS			=  11,
*/
void CDlgChoixSupport::OnUpdateCsPalette1(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::BOTTLE_GREEN) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}


void CDlgChoixSupport::OnUpdateCsPalette2(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::EMERAUDE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette3(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::BLEU_PASTEL) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette4(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::OCEAN) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette5(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::LILAS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette6(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::SAHARA) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette7(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::TERRE_SIENNE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette8(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::RUBIS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette9(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::GREY_MOUSE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette10(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette11(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE_KAKI) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette12(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::ROSE_GRIS) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}
void CDlgChoixSupport::OnUpdateCsPalette13(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::SABLE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette14(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::CAFE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgChoixSupport::OnUpdateCsPalette15(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(m_CATPColorsApp) == CATPColors::NEWBLUE) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}
