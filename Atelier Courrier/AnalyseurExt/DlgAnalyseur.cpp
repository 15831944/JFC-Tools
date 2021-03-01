#include "..\repitemsext\dlgitems.cpp"
// DlgAnalyseur.cpp : implementation file
//

#include "stdafx.h"
#include ".\dlganalyseur.h"
#include "DlgParams.h"
#include "CibleSelectionDlg.h"
#include "SrcUniqueDlg.h"
#include "ATPMessages.h"
#include "CrossTable.h"
#include "ExcelPrinter.h"
#include "DlgTableau.h"
#include "DlgParamsSurlignage.h"
#include "AnalyseurDoc.h"
#include "DlgItemsMoyenne.h"

#include "JFCApp.h"

class CKeyRowCol; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParentWnd);

/// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_AN_ABOUTBOX };
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
	//{{AFX_MSG_MAP(CAboutDlg)
		/// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgAnalyseur dialog

IMPLEMENT_DYNAMIC(CDlgAnalyseur, CDialog)

const CRect CDlgAnalyseur::s_rectDefault(155,135,885,605);  // statique

///////////////////////////////////////////////////////////////////////////////////////////
// Construction
CDlgAnalyseur::CDlgAnalyseur(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyseur::IDD, pParent)
{
	m_pCrossTable		=	0;
	m_pDoc				=	0;//new CAnalyseurDoc();
	m_bFirstTime 		= 	TRUE;

	// Par défaut le mode base 100 est inactif
	m_ModeBase100Actif	=	FALSE;

	// Par défaut le mode exclusif n'est pas actif
	m_ModeExclusif		=	FALSE;

	// Par défaut le mode duplicant n'est pas actif
	m_ModeDuplicant		=	FALSE;

	// Par défaut le mode duplicant n'est pas actif
	m_ModeCurseurTri	=	FALSE;

	// Pointeur boite sélection
	m_pDlgTableau = new CDlgTableau(this);

	// Init sélection dans pupitre
	m_pDlgTableau->InitSelGeneral(); 

	// Init paramètres par défaut surlignage
	m_Default_OpeSurlignage.Param_Color		= RGB(255, 255, 0);
	m_Default_OpeSurlignage.Param_Grandeur	= RTYPE_EFFECTIVE_000;
	m_Default_OpeSurlignage.Param_Result	= RSTYLE_RESULT;
	m_Default_OpeSurlignage.TypeParam		= CAnalyseurDoc::TYPE_SUP;
	m_Default_OpeSurlignage.Valeur1			= 0.0;
	m_Default_OpeSurlignage.Valeur2			= 0.0;

}

///////////////////////////////////////////////////////////////////////////////////////////
// Desctructeur
CDlgAnalyseur::~CDlgAnalyseur()
{
	// Destruction tableau croisé
	if(m_pCrossTable)
		delete m_pCrossTable;

	// Destruction du document associé
	if(m_pDoc)
		delete m_pDoc;

	/* pas nécessaire
	// Destruction du dialogue sélection
	if(m_pDlgTableau)
		delete m_pDlgTableau;
	*/

}

void CDlgAnalyseur::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAnalyseur, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDM_AN_ABOUTBOX,												OnAboutbox)
	ON_WM_SIZE()
	ON_WM_CLOSE()

	// Menu séelction couleurs application Tris Croisés
	ON_COMMAND(IDM_AN_PALETTE_1,											OnPalette1)
	ON_COMMAND(IDM_AN_PALETTE_2,											OnPalette2)
	ON_COMMAND(IDM_AN_PALETTE_3,											OnPalette3)
	ON_COMMAND(IDM_AN_PALETTE_4,											OnPalette4)
	ON_COMMAND(IDM_AN_PALETTE_5,											OnPalette5)
	ON_COMMAND(IDM_AN_PALETTE_6,											OnPalette6)
	ON_COMMAND(IDM_AN_PALETTE_7,											OnPalette7)
	ON_COMMAND(IDM_AN_PALETTE_8,											OnPalette8)
	ON_COMMAND(IDM_AN_PALETTE_9,											OnPalette9)
	ON_COMMAND(IDM_AN_PALETTE_10,											OnPalette10)
	ON_COMMAND(IDM_AN_PALETTE_11,											OnPalette11)
	ON_COMMAND(IDM_AN_PALETTE_12,											OnPalette12)
	ON_COMMAND(IDM_AN_PALETTE_13,											OnPalette13)
	ON_COMMAND(IDM_AN_PALETTE_14,											OnPalette14)
	ON_COMMAND(IDM_AN_PALETTE_15,											OnPalette15)

	ON_BN_CLICKED(IDC_AN_BUT_VECTORIELLE ,									OnBnClickedButVectorielle)
	ON_LBN_SELCHANGE(IDC_AN_LIST_GRANDEURS,									OnLbnSelchangeAnListGrandeurs)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(IDM_AN_SOURCE_CHOISIR_TERRAINS,								OnChangerTerrains)
	ON_LBN_SELCHANGE(IDC_AN_LIST_TERRAINS,									OnLbnSelchangeAnListTerrains)
	ON_COMMAND(IDM_AN_TABLEAU_TRANSPOSER,									OnTableauTransposer)
	ON_COMMAND(IDM_AN_TABLEAU_MODIFIER,										OnBnClickedButVectorielle)
	ON_LBN_SELCHANGE(IDC_AN_LIST_CIBLES,									OnLbnSelchangeAnListCibles)
	ON_COMMAND(IDM_AN_PARAMETRES_DEFINIR_STRUCTURE_TABLEAU,					OnDefinirStructureTableau)

	ON_LBN_SELCHANGE(IDC_AN_LIST_VAR_MOY,									OnLbnSelchangeAnListVarMoy)
	
	// Base 100 passe dans le menu
	// ON_BN_CLICKED(IDC_AN_BUT_BASE100,									OnBnClickedAnButBase100)

	ON_BN_CLICKED(IDC_AN_BUT_ORDONNER,										OnBnClickedAnButOrdonner)
	ON_BN_CLICKED(IDC_AN_BUT_PARAMSURLIGNAGE,								OnBnClickedAnButParamSurlignage)


	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_AN_PARAMETRES_INTERPRETER_UNIVERS_COMME_CIBLE,			OnInterpreterUniversCommeCible)
	ON_UPDATE_COMMAND_UI(IDM_AN_PARAMETRES_INTERPRETER_UNIVERS_COMME_CIBLE, OnUpdateInterpreterUniversCommeCible)
	ON_COMMAND(ID_FILE_NEW,													OnFileNew)
	ON_COMMAND(ID_FILE_OPEN,												OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE,												OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS,												OnFileSaveAs)
	ON_COMMAND(ID_FILE_CLOSE,												OnFileClose)
	ON_COMMAND(IDM_AN_TABLEAU_QUITTER,										OnTableauQuitter)
	ON_COMMAND(IDM_AN_TABLEAU_APPLIQUER_CANEVAS,							OnTableauAppliquerCanevas)
	ON_COMMAND(IDM_AN_TABLEAU_ENREGISTRER_CANEVAS,							OnTableauEnregistrerCanevas)

	ON_BN_CLICKED(IDC_AN_BUT_GRANDEUR,										OnBnClickedAnButGrandeur)
	ON_BN_CLICKED(IDC_AN_BUT_VERT,											OnBnClickedAnButVert)
	ON_BN_CLICKED(IDC_AN_BUT_HORIZ,											OnBnClickedAnButHoriz)
	ON_BN_CLICKED(IDC_AN_BUT_GLOBAL,										OnBnClickedAnButGlobal)
	ON_BN_CLICKED(IDC_AN_BUT_GRANDEUR2,										OnBnClickedAnButGrandeur2)
	ON_BN_CLICKED(IDC_AN_BUT_VERT2,											OnBnClickedAnButVert2)
	ON_BN_CLICKED(IDC_AN_BUT_HORIZ2,										OnBnClickedAnButHoriz2)
	ON_BN_CLICKED(IDC_AN_BUT_GLOBAL2,										OnBnClickedAnButGlobal2)
	ON_BN_CLICKED(IDC_AN_BUT_GRANDEUR3,										OnBnClickedAnButGrandeur3)
	ON_BN_CLICKED(IDC_AN_BUT_VERT3,											OnBnClickedAnButVert3)
	ON_BN_CLICKED(IDC_AN_BUT_HORIZ3,										OnBnClickedAnButHoriz3)
	ON_BN_CLICKED(IDC_AN_BUT_GLOBAL3,										OnBnClickedAnButGlobal3)

	ON_BN_CLICKED(IDC_AN_BUT_MOYENNE_TABLEAU,								OnBnClickedAnButMoyenneTableau)
	ON_BN_CLICKED(IDC_AN_BUT_RAZ_VAR_MOY,									OnBnClickedAnButRazVarMoy)

	ON_BN_CLICKED(IDC_AN_BUT_TERRAIN_UP,									OnBnClickedAnButUpTerrain)
	ON_BN_CLICKED(IDC_AN_BUT_TERRAIN_DOWN,									OnBnClickedAnButDownTerrain)

	ON_BN_CLICKED(IDC_AN_BUT_EXCLU,											OnBnClickedAnButExclu)
	ON_BN_CLICKED(IDC_AN_BUT_DUPLI,											OnBnClickedAnButDupli)
	ON_MESSAGE(ATPMSG_SEUILS_CHANGED,										OnSeuilsChanged)
	ON_COMMAND(IDM_AN_TABLEAU_EXPORTER_PRESSEPAPIER,						OnTableauExporterPressepapier)
	ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_EXPORTER_PRESSEPAPIER,				OnUpdateTableauExporterPressepapier)
	ON_COMMAND(IDM_AN_TABLEAU_EXPORTER_FICHIER,								OnTableauExporterFichier)
	ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_EXPORTER_FICHIER,					OnUpdateTableauExporterFichier)
	// ON_COMMAND(IDM_AN_TABLEAU_EXPORTER_EXCEL,								OnTableauExporterExcel)
	// ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_EXPORTER_EXCEL,						OnUpdateTableauExporterExcel)
	ON_COMMAND(IDM_AN_TABLEAU_EXPORTER_EXCEL_AVECDECOUP,					OnTableauExporterExcel_AvecDecoup)
	ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_EXPORTER_EXCEL_AVECDECOUP,			OnUpdateTableauExporterExcel_AvecDecoup)
	ON_COMMAND(IDM_AN_TABLEAU_EXPORTER_EXCEL_SANSDECOUP,					OnTableauExporterExcel_SansDecoup)
	ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_EXPORTER_EXCEL_SANSDECOUP,			OnUpdateTableauExporterExcel_SansDecoup)
	ON_COMMAND(IDM_AN_TABLEAU_IMPRIMER,										OnTableauImprimer)
	ON_UPDATE_COMMAND_UI(IDM_AN_TABLEAU_IMPRIMER,							OnUpdateTableauExporterFichier)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_1,									OnUpdatePalette1)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_2,									OnUpdatePalette2)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_3,									OnUpdatePalette3)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_4,									OnUpdatePalette4)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_5,									OnUpdatePalette5)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_6,									OnUpdatePalette6)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_7,									OnUpdatePalette7)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_8,									OnUpdatePalette8)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_9,									OnUpdatePalette9)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_10,									OnUpdatePalette10)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_11,									OnUpdatePalette11)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_12,									OnUpdatePalette12)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_13,									OnUpdatePalette13)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_14,									OnUpdatePalette14)
	ON_UPDATE_COMMAND_UI(IDM_AN_PALETTE_15,									OnUpdatePalette15)

	ON_UPDATE_COMMAND_UI(IDM_AN_SOURCE_CHOISIR_UNIVERS,						OnUpdateChoisirUnivers)
	ON_COMMAND(IDM_AN_SOURCE_CHOISIR_UNIVERS,								OnChoisirUnivers)
	ON_MESSAGE(WM_BTNST_LBUTTONDOWN, OnBnLClickedAnButGlobal)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_AN_PARAMETRES_PRISE_EN_COMPTE_MARGERREUR,				OnAnParametresPriseEnCompteMargerreur)
	ON_UPDATE_COMMAND_UI(IDM_AN_PARAMETRES_PRISE_EN_COMPTE_MARGERREUR,		OnUpdateAnParametresPriseEnCompteMargerreur)

	ON_COMMAND(IDM_AN_PARAMETRES_FORCE_QUANTI,								OnAnParametresForceQuanti)
	ON_UPDATE_COMMAND_UI(IDM_AN_PARAMETRES_FORCE_QUANTI,				OnUpdateAnParametresForceQuanti)

	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_AN_PARAM_EXCLUSIFS,										OnAnParamExclusifs)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_EXCLUSIFS,								OnUpdateAnParamExclusifs)
	ON_COMMAND(ID_AN_PARAM_DUPLICANTS,										OnAnParamDuplicants)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_DUPLICANTS,							OnUpdateAnParamDuplicants)
	ON_COMMAND(ID_AN_PARAM_DUPLICANTS,										OnAnParamBase100)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_BASE100,								OnUpdateAnParamBase100)
	ON_CBN_SELCHANGE(IDC_COMBO_TRI_GRANDEURS,								OnCbnSelchangeComboTriGrandeurs)
	ON_CBN_SELCHANGE(IDC_COMBO_TRI_RESULTATS,								OnCbnSelchangeComboTriResultats)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPETRI,										OnCbnSelchangeComboTypetri)
	ON_CBN_SELCHANGE(IDC_COMBO_MODETRI,										OnCbnSelchangeComboModetri)
	ON_COMMAND(IDM_AN_DISPO_LIGNES,											OnAnDispoLignes)
	ON_UPDATE_COMMAND_UI(IDM_AN_DISPO_LIGNES,								OnUpdateAnDispoLignes)
	ON_COMMAND(IDM_AN_DISPO_2PAR2,											OnAnDispo2par2)
	ON_UPDATE_COMMAND_UI(IDM_AN_DISPO_2PAR2,								OnUpdateAnDispo2par2)
	ON_COMMAND(IDM_AN_DISPO_COLONNES,										OnAnDispoColonnes)
	ON_UPDATE_COMMAND_UI(IDM_AN_DISPO_COLONNES,								OnUpdateAnDispoColonnes)
	ON_COMMAND(ID_AN_PARAM_BASE100,											OnAnParamBase100)
	ON_BN_CLICKED(IDC_AN_BUT_PREC_SURLIGN,									OnBnClickedAnButPrecSurlign)
	ON_BN_CLICKED(IDC_AN_BUT_RAZ_SURLIGN,									OnBnClickedAnButRazSurlign)
	ON_COMMAND(IDM_AN_OUTILS_LINEARISATION,									OnAnOutilsLinearisation)
	ON_COMMAND(IDM_AN_OUTILS_SURLIGNAGE,									OnAnOutilsSurlignage)
	ON_UPDATE_COMMAND_UI(IDM_AN_OUTILS_LINEARISATION,						OnUpdateAnOutilsLinearisation)
	ON_UPDATE_COMMAND_UI(IDM_AN_OUTILS_SURLIGNAGE,							OnUpdateAnOutilsSurlignage)

	ON_COMMAND(ID_AN_PARAM_MOYENNE_TABLEAU,									OnAnParamMoyenneTableau)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_MOYENNE_TABLEAU,						OnUpdateAnParamMoyenneTableau)
	ON_COMMAND(ID_AN_PARAM_MOYENNE_LIGNE,									OnAnParamMoyenneLigne)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_MOYENNE_LIGNE,							OnUpdateAnParamMoyenneLigne)
	ON_COMMAND(ID_AN_PARAM_MOYENNE_COLONNE,									OnAnParamMoyenneColonne)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_MOYENNE_COLONNE,						OnUpdateAnParamMoyenneColonne)

	ON_COMMAND(ID_AN_PARAM_MOYENNE_0_INCLUS,								OnAnParamMoyenne0Inclus)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_MOYENNE_0_INCLUS,						OnUpdateAnParamMoyenne0Inclus)
	ON_COMMAND(ID_AN_PARAM_MOYENNE_0_EXCLUS,								OnAnParamMoyenne0Exclus)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_MOYENNE_0_EXCLUS,						OnUpdateAnParamMoyenne0Exclus)

	// Commande pour sélectionner l'indice global courant
	ON_COMMAND(IDM_AN_GLOBAL_AFFINITE,										OnAnParamIndiceAffinite)
	ON_UPDATE_COMMAND_UI(IDM_AN_GLOBAL_AFFINITE,							OnUpdateAnParamIndiceAffinite)
	ON_COMMAND(IDM_AN_GLOBAL_STRUCTURE,										OnAnParamIndiceStructure)
	ON_UPDATE_COMMAND_UI(IDM_AN_GLOBAL_STRUCTURE,							OnUpdateAnParamIndiceStructure)
	ON_COMMAND(IDM_AN_GLOBAL_PROXIMITY,										OnAnParamIndiceProximite)
	ON_UPDATE_COMMAND_UI(IDM_AN_GLOBAL_PROXIMITY,							OnUpdateAnParamIndiceProximite)

	// Mode affichage des grandeurs moyenne et effectif
	ON_COMMAND(ID_AN_PARAM_AFFICH_EFFECTIF_MILLIERS,						OnAnParamAffichEffMilliers)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_AFFICH_EFFECTIF_MILLIERS,				OnUpdateAnParamAffichEffMilliers)

	ON_COMMAND(ID_AN_PARAM_AFFICH_EFFECTIF_GLOBAL,							OnAnParamAffichEffGlobal)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_AFFICH_EFFECTIF_GLOBAL,				OnUpdateAnParamAffichEffGlobal)

	ON_COMMAND(ID_AN_PARAM_AFFICH_MOYENNE_2DEC,								OnAnParamAffichMoy2Dec)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_AFFICH_MOYENNE_2DEC,					OnUpdateAnParamAffichMoy2Dec)

	ON_COMMAND(ID_AN_PARAM_AFFICH_MOYENNE_SANSDEC,							OnAnParamAffichMoySansDec)
	ON_UPDATE_COMMAND_UI(ID_AN_PARAM_AFFICH_MOYENNE_SANSDEC,				OnUpdateAnParamAffichMoySansDec)
	
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// pour quitter l'application

BOOL CDlgAnalyseur::CanClose()
{
	if (m_pDoc != 0 && m_pDoc->IsDirty())
	{
		CString txt_1, txt_2;
		txt_1.LoadString(IDS_AN_SAVETABLEAU);
		txt_2.LoadString(IDS_AN_APPNAME);
		INT Ret = this->MessageBox(txt_1, txt_2, MB_YESNOCANCEL | MB_ICONQUESTION);
		if      (Ret == IDYES) { this->OnFileSave(); if (m_pDoc->IsDirty()) return (FALSE); }
		else if (Ret != IDNO)  { return (FALSE); }
	}


	// on peut fermer sans rien perdre
	return (TRUE);
}

void CDlgAnalyseur::Close()
{
	if (m_pDoc != 0)
	{
		// Init document, avec mise à jour des composants mode moyenne
		m_pDoc->Reset(); 
		UpdateElmtVarMoy();

		CWaitCursor Cw;
		m_pCrossTable->CalculateItems();
		UpdateSurlignage();
		SetTitre();
	}

	// on ferme la boite
	CDlgAnalyseur::OnClose();
}

void CDlgAnalyseur::OnAboutbox()
{
	CAboutDlg dlgAbout(this);
	dlgAbout.DoModal();
}

void CDlgAnalyseur::OnClose() 
{
	// On ferme le dossier tris croisés courant
	if (OkFileClose())
	{
		//OnFileClose();
		m_pDoc->Set_ClickMode(CMODE_TRI);

		// m_But_Base100   .SetCheck(0);

		m_But_Ordonner.SetCheck(0);
		m_DlgSurlignage.ShowWindow(SW_HIDE);
		m_But_Vectorielle.EnableWindow(TRUE); 

		// Par défaut tous les modes base 100, exclusif, duplicant inactifs
		m_ModeBase100Actif	=	FALSE;
		m_ModeExclusif		=	FALSE;
		m_ModeDuplicant		=	FALSE;	
		m_ModeCurseurTri	=   FALSE;

		// Sauve état couleur application Tris Croises
		AfxGetApp()->WriteProfileInt("Options\\Analyseur","ColorAnalyseur",CATPColors::GetAppColor(CATPColors::COLORANALYSEUR));

		// Sauve mode calcul marge erreur
		AfxGetApp()->WriteProfileInt("Options\\Analyseur","MargeErreur",m_pDoc->CalculMargeErreur());

		// Fermeture pupitre su ouvert
		if (m_pDlgTableau->CanClose())
		{
			if (m_pDlgTableau->m_hWnd != NULL)
				m_pDlgTableau->Close(); 
		}

		CDialog::OnClose();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Fermeture des tris croises O/N
JBool CDlgAnalyseur::OkFileClose()
{
	if(m_pDoc->IsDirty())
	{
		CString txt;
		txt.LoadString(IDS_AN_SAVETABLEAU);
		int Result = MessageBox(txt,0,MB_YESNOCANCEL | MB_ICONQUESTION);
		if(Result == IDYES)
			OnFileSave();
		else if(Result == IDCANCEL)
			return false;
	}

	// Init document, avec mise à jour des composants mode moyenne
	m_pDoc->Reset(); 
	UpdateElmtVarMoy();

	// Init sélection dans pupitre
	m_pDlgTableau->InitSelGeneral(); 

	// Puis on va sélectionner les nouveaux éléments dans pupitre
	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();

	SetTitre();
	
	return true;
}

void CDlgAnalyseur::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_AN_ABOUTBOX)
	{
		CAboutDlg dlgAbout(this);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgAnalyseur::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlgAnalyseur::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors(); 
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgAnalyseur::OnPaint() 
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

void CDlgAnalyseur::OnSize(UINT nType, int cx, int cy) 
{
	// Redimensionne les objets de la fenêtre analayseur
	CDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();

	// Retaille fenêtre surlignage si existante
	RetailleFenOrdonnancement();

	// Mise à jour de la fenêtre
	Invalidate();
	UpdateWindow();

	/*
	if (m_CrossTable)
	{
		OnLbnSelchangeAnListCibles();
	}
	*/

	/*
	// PROBLEME ??????? SI SEULEMENT GRID VIDE NE SE REDESSINE PAS CORRECTEMENT ???????
	if (m_CrossTable)
	{
		this->InvalidateRect(NULL, FALSE);
		// AfxMessageBox("RedrawGrid");
		m_pCrossTable->RedrawGrid();
		this->OnPaint();
		m_CrossTable.UpdateAllRowsCols();
	}
	*/
	

	/*
	if (m_CrossTable)
	{
		m_pCrossTable->CalculateItems(true);
		m_pCrossTable->RedrawGrid();
		this->OnPaint();


		m_List_Grandeurs.SetFocus();

		m_List_Grandeurs.SetActiveWindow();

		OnLbnSelchangeAnListGrandeurs();

		OnTableauTransposer();
		Invalidate(TRUE);

		OnTableauTransposer();
		Invalidate(TRUE);
	}
	*/
}

void CDlgAnalyseur::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//Taille minimale de la boite 730x520
	lpMMI->ptMinTrackSize.x = 730;
	lpMMI->ptMinTrackSize.y = 528;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

// CDlgAnalyseur message handlers

BOOL CDlgAnalyseur::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	if(AfxGetApp())
		m_hIcon = AfxGetApp()->LoadIcon(IDI_AN_ICON);

	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR, AfxGetApp()->GetProfileInt("Options\\Analyseur", "ColorAnalyseur", CATPColors::TERRE_SIENNE));

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetupControls();

	m_DlgSurlignage.SetButtons(&m_But_Ordonner, m_ModeBase100Actif);

	m_pDoc	=	new CAnalyseurDoc(m_ProgCtrl, *this, m_DlgSurlignage);

	// Récupération mode affichage effectif (par défaut en milliers)
	int TypeAffichSelEffectif = AfxGetApp()->GetProfileInt("Options\\Analyseur", "ModeAffichEffectif", m_pDoc->MODE_EFFECTIF_000);
	m_pDoc->SetTypeAffichSelEffectif(TypeAffichSelEffectif);

	// Récupération mode affichage moyenne (par défaut en décimale)
	int TypeAffichSelMoyenne = AfxGetApp()->GetProfileInt("Options\\Analyseur", "ModeAffichMoyenne", m_pDoc->MODE_AVERAGEQUANTUM_P00);
	m_pDoc->SetTypeAffichSelMoyenne(TypeAffichSelMoyenne);

	SetupGrandeurs();

	SetGrandeurs();

	// Init sélection des tris
	SetupSelTri();

	// Init liste des variables moyennables
	m_List_VarMoy.ResetContent();

	// Init Type et Mode tri
	if (m_pDoc)
	{
		m_pDoc->Set_TypeTri(m_pDoc->TYPE_BASE);
		m_pDoc->Set_ModeTri(m_pDoc->MODE_LIGNE); 
	}

	// Initialize the grid. For CWnd based grids this call is
    // essential.
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(& m_CrossTable);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_AN_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys = FALSE;
	pWnd->m_nIndexValue = 0;
	pWnd->m_bDispChoice = TRUE;

	m_CrossTable.RegisterControl(IDS_AN_CTRL_INDEXDROPDOWNLIST, pWnd);
	m_CrossTable.LockUpdate(false);
	SetupCrossTable();

	m_pCrossTable->SetupGrid(&m_CrossTable, m_pDoc, m_ModeBase100Actif, &m_But_Ordonner);

	m_pDoc->Set_ClickMode(CMODE_TRI);

	m_But_Ordonner.SetCheck(0);

	LoadProfile();
	UpdateListCibles();

	CheckItems(); // ?? A VOIR OPTIM 2.7

	CWaitCursor Cw;

	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();

	SetTitre();

	// Récup mode marge erreur
	m_pDoc->SetModeMargeErreur(AfxGetApp()->GetProfileInt("Options\\Analyseur", "MargeErreur", true));

	// Récupération du mode d'affichage résultats (ligne / colonne / 2 x 2) // par défaut on sera en mode ligne
	int ModeAffichResult = AfxGetApp()->GetProfileInt("Options\\Analyseur", "ModeAffichResult", m_pDoc->AFFRESULT_LIGNE );
	m_pDoc->SetModeAffichResult(ModeAffichResult);

	// Redisposition des sélections grandeurs
	DispoBtnGrandeurs();

	// Update validité du mode moyenne tableau
	UpdateElmtVarMoy();

	return 0;
}

void CDlgAnalyseur::SetupCrossTable()
{
	m_pCrossTable = new CCrossTable();
}

void CDlgAnalyseur::SetupGrandeursBase()
{
	CString Str;

	// Avant on sauve les 2 sélections qui éventuellement changent
	JBool EffectifSel = false;
	JBool MoyenneSel  = false;
	if (m_List_Grandeurs.GetCount())
	{
		// Boucle sur toutes les grandeurs existantes
		for (JInt32 InxLst = 0; InxLst <= m_List_Grandeurs.GetCount(); InxLst++)
		{
			// De quel garndeur il s'agit ????
			JInt32 ItemData = m_List_Grandeurs.GetItemData(InxLst);

			// Si Effectif sélectionné
			if ((ItemData == RTYPE_EFFECTIVE_000 || ItemData == RTYPE_EFFECTIVE_CIBLE) && m_List_Grandeurs.GetSel(InxLst))
				EffectifSel = true;

			// Si Moyenne sélectionnée
			if ((ItemData == RTYPE_AVERAGEQUANTUM_P00 || ItemData == RTYPE_AVERAGEQUANTUM) && m_List_Grandeurs.GetSel(InxLst))
				MoyenneSel = true;
		}
	}

	// Reset après sélection
	m_List_Grandeurs.ResetContent();

	JInt32 InxLstCur;

	// Sélecteur effectif (par défaut c'est l'effectif en milliers)
	if (m_pDoc->GetTypeAffichSelEffectif() == CAnalyseurDoc::MODE_EFFECTIF_000)
	{
		// Effectif en milliers
		m_pDoc->GetGrandeurString(Str,RTYPE_EFFECTIVE_000);
		InxLstCur = m_List_Grandeurs.AddString(Str);
		m_List_Grandeurs.SetItemData(InxLstCur, RTYPE_EFFECTIVE_000);
	}
	else
	{
		// Effectif
		m_pDoc->GetGrandeurString(Str,RTYPE_EFFECTIVE_CIBLE);
		InxLstCur = m_List_Grandeurs.AddString(Str);
		m_List_Grandeurs.SetItemData(InxLstCur, RTYPE_EFFECTIVE_CIBLE);
	}

	// Mis à jour sélection ou pas de la grandeur effectif
	m_List_Grandeurs.SetSel(InxLstCur, EffectifSel);


	// Nombre de cas
	m_pDoc->GetGrandeurString(Str,RTYPE_NBCAS);
	m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_NBCAS);

	// Quantité agrégé
	m_pDoc->GetGrandeurString(Str,RTYPE_QUANTITY_000);
	m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_QUANTITY_000);

	// Sélecteur moyenne (par défaut c'est la moyenne avec 2 décimales
	if (m_pDoc->GetTypeAffichSelMoyenne() == CAnalyseurDoc::MODE_AVERAGEQUANTUM_P00)
	{
		// Moyenne avec 2 décimales
		m_pDoc->GetGrandeurString(Str,RTYPE_AVERAGEQUANTUM_P00);
		InxLstCur = m_List_Grandeurs.AddString(Str);
		m_List_Grandeurs.SetItemData(InxLstCur, RTYPE_AVERAGEQUANTUM_P00);
	}
	else
	{
		// Moyenne
		m_pDoc->GetGrandeurString(Str,RTYPE_AVERAGEQUANTUM);
		InxLstCur = m_List_Grandeurs.AddString(Str);
		m_List_Grandeurs.SetItemData(InxLstCur, RTYPE_AVERAGEQUANTUM);
	}

	// Mis à jour sélection ou pas de la grandeur moyenne
	m_List_Grandeurs.SetSel(InxLstCur, MoyenneSel);

}

void CDlgAnalyseur::SetupGrandeurs(JBool Reset)
{
	// Reset avant remplissage
	if (Reset)
		m_List_Grandeurs.ResetContent();

	// Setup the Grandeurs list box
	this->SetupGrandeursBase();
	CString Str;
	if(m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
	{
		m_pDoc->GetGrandeurString(Str,RTYPE_PENETRATION);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_PENETRATION);
		m_pDoc->GetGrandeurString(Str,RTYPE_CENTRAGE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_CENTRAGE);
		m_pDoc->GetGrandeurString(Str,RTYPE_CONCOMITENCE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_CONCOMITENCE);
		m_pDoc->GetGrandeurString(Str,RTYPE_COEFFYULE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_COEFFYULE);
	}
	m_List_Grandeurs.Invalidate(TRUE);
	m_List_Grandeurs.UpdateWindow(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des sélections pour le tri dans le document afin de le récupérer via la boite de dialogue paramétres tri
void CDlgAnalyseur::SetupSelTri()
{
	// Sélection de la grandeur pour le tri
	m_pDoc->m_MapGrandeursDispos.Reset();
	CString Txt;
	for (JInt32 InxLst = 0; InxLst < m_List_Grandeurs.GetCount(); InxLst++)
	{
		// Ajoute uniquement les grandeurs sélectionnées
		if (m_List_Grandeurs.GetSel(InxLst))
		{
			// Info texte grandeur
			m_List_Grandeurs.GetText(InxLst, Txt); 

			// Info ident grandeur
			JInt32 ItemData = m_List_Grandeurs.GetItemData(InxLst);

			// Ajoute au map des grandeurs dispos pour le tri (avec itemdata coeff 10000 sinon ordre différent)
			m_pDoc->m_MapGrandeursDispos.Add(ItemData + (InxLst * 10000)) = Txt; 
		}
	}

	// Sélection du type de résultat
	m_pDoc->m_MapResultCellDispo.Reset();

	// Ajoute type résultat Grandeur si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
	{
		m_pDoc->m_MapResultCellDispo.Add(RSTYLE_RESULT) = "Grandeur" ;
	}

	// Ajoute type résultat Horizontal si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
	{
		m_pDoc->m_MapResultCellDispo.Add(RSTYLE_HORIZONTAL) = "H%";
	}

	// Ajoute type résultat Vertical si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0)
	{
		m_pDoc->m_MapResultCellDispo.Add(RSTYLE_VERTICAL) = "V%";
	}

	// Ajoute type résultat Global si actif
	CString LibResult = "";
	if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
	{
		// Mode calcul global standard
		if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
		{
			LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
			m_pDoc->m_MapResultCellDispo.Add(RSTYLE_GBL_CORRELATION) = LibResult;
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
		{
			LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
			m_pDoc->m_MapResultCellDispo.Add(RSTYLE_GBL_PROXIMITY) = LibResult;
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
		{
			LibResult.LoadString(IDS_AN_ABR_ZSCORE);
			m_pDoc->m_MapResultCellDispo.Add(RSTYLE_GBL_ZSCORE) = LibResult;
		}
		else
		{
			LibResult.LoadString(IDS_AN_ABR_GLOBAL);
			m_pDoc->m_MapResultCellDispo.Add(RSTYLE_GLOBAL) = LibResult;
		}

	}
	else
	{
		// Mise à jour texte dernier bouton indice
		if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
		{
			LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
		{
			LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
		{
			LibResult.LoadString(IDS_AN_ABR_ZSCORE);
		}
		else
		{
			LibResult.LoadString(IDS_AN_ABR_GLOBAL);
		}
	}

	// Sélection du type de tri (de base, croissant, décroissant / TYPE_BASE = 0, TYPE_DECROISSANT, TYPE_CROISSANT)
	// puis on positionne le type de tri courant
	if (m_Combo_TriType.GetCount() > 0)
	{
		for (JInt32 InxCmd = 0; InxCmd < m_Combo_TriType.GetCount(); InxCmd++)	
		{
			// Récup item data de l'élmt
			JInt32 ItemData = m_Combo_TriType.GetItemData(InxCmd);

			// Si c'est celui courant, on le sélectionne
			if (ItemData == m_pDoc->Get_TypeTri()) // m_TypeTri)
			{
				m_Combo_TriType.SetCurSel(InxCmd); 
				break;
			}
		}
	}
}

/* Avant avel les combo box directement sur la fenetre analyseur
///////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des sélections pour le tri
void CDlgAnalyseur::SetupSelTri()
{

	// Attention désrmais il nous faut définir ces conditions dans le document
	// afin de le récupérer via la boite de dialogue paramétres tri

	// Sélection de la grandeur pour le tri
	m_Combo_TriGrandeurs.ResetContent();
	CString Txt;
	for (JInt32 InxLst = 0; InxLst < m_List_Grandeurs.GetCount(); InxLst++)
	{
		// Ajoute uniquement les grandeurs sélectionnées
		if (m_List_Grandeurs.GetSel(InxLst))
		{
			// Info texte grandeur
			m_List_Grandeurs.GetText(InxLst, Txt); 
			JInt32 InxCmd = m_Combo_TriGrandeurs.AddString(Txt);

			// Info ident grandeur
			JInt32 ItemData = m_List_Grandeurs.GetItemData(InxLst);
			m_Combo_TriGrandeurs.SetItemData(InxCmd,ItemData); 
		}
	}

	// puis on postionne la grandeur tri sélection en cours
	if (m_Combo_TriGrandeurs.GetCount() > 0)
	{
		for (JInt32 InxCmd = 0; InxCmd < m_Combo_TriGrandeurs.GetCount(); InxCmd++)	
		{
			// Récup item data de l'élmt
			JInt32 ItemData = m_Combo_TriGrandeurs.GetItemData(InxCmd);

			// Si c'est celui courant, on le sélectionne
			if (ItemData == m_pDoc->Get_GrandeurTri()) // m_IdTriGrandeurs)
			{
				m_Combo_TriGrandeurs.SetCurSel(InxCmd); 
				break;
			}
		}

		// Si aucune sélection, on se positionne sur le 1er elmt de la combo box
		JInt32 Value = m_Combo_TriGrandeurs.GetCurSel();

		if (m_Combo_TriGrandeurs.GetCurSel() < 0)
		{
			m_Combo_TriGrandeurs.SetCurSel(0);
			JInt32 ItemData = m_Combo_TriGrandeurs.GetItemData(0);
			// m_IdTriGrandeurs = ItemData;
			RESULTTYPE ResultType = (RESULTTYPE) ItemData;
			m_pDoc->Set_GrandeurTri(ResultType); 
		}
	}

	// Sélection du type de résultat
	m_Combo_TriResultats.ResetContent();
	m_pDoc->m_MapGrandeursDispos.Reset(); 
	JInt32 InxCmd;
	
	// Ajoute type résultat Grandeur si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
	{
		InxCmd = m_Combo_TriResultats.AddString("Grandeur");
		m_Combo_TriResultats.SetItemData(InxCmd, RSTYLE_RESULT);
		m_pDoc->m_MapGrandeursDispos.Add("Grandeur", RSTYLE_RESULT);
	}

	// Ajoute type résultat Horizontal si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
	{
		InxCmd = m_Combo_TriResultats.AddString("H%");
		m_Combo_TriResultats.SetItemData(InxCmd, RSTYLE_HORIZONTAL);
		m_pDoc->m_MapGrandeursDispos.Add("H%", RSTYLE_HORIZONTAL);
	}

	// Ajoute type résultat Vertical si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0)
	{
		InxCmd = m_Combo_TriResultats.AddString("V%");
		m_Combo_TriResultats.SetItemData(InxCmd, RSTYLE_VERTICAL);
		m_pDoc->m_MapGrandeursDispos.Add("V%", RSTYLE_VERTICAL);
	}

	// Ajoute type résultat Vertical si actif
	if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
	{
		InxCmd = m_Combo_TriResultats.AddString("Global");
		m_Combo_TriResultats.SetItemData(InxCmd, RSTYLE_GLOBAL);
		m_pDoc->m_MapGrandeursDispos.Add("Global%", RSTYLE_GLOBAL);
	}

	// puis on positionne le type de résultat courant
	if (m_Combo_TriResultats.GetCount() > 0)
	{
		for (JInt32 InxCmd = 0; InxCmd < m_Combo_TriResultats.GetCount(); InxCmd++)	
		{
			// Récup item data de l'élmt
			JInt32 ItemData = (RESULTSTYLE) m_Combo_TriResultats.GetItemData(InxCmd);

			// Si c'est celui courant, on le sélectionne
			if (ItemData == m_pDoc->Get_ResultTri())
			{
				m_Combo_TriResultats.SetCurSel(InxCmd); 
				break;
			}
		}

		// Si aucune sélection, on se positionne sur le 1er elmt de la combo box
		if (m_Combo_TriResultats.GetCurSel() < 0)
		{
			m_Combo_TriResultats.SetCurSel(0);
			JInt32 ItemData = m_Combo_TriResultats.GetItemData(0);
			RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
			m_pDoc->Set_ResultTri(ResultStyle);
		}
	}
	
	// Sélection du type de tri (de base, croissant, décroissant / TYPE_BASE = 0, TYPE_DECROISSANT, TYPE_CROISSANT)
	m_Combo_TriType.ResetContent();
	InxCmd = m_Combo_TriType.AddString("Ordre origine"); 
	m_Combo_TriType.SetItemData(InxCmd, m_pDoc->TYPE_BASE);
	InxCmd = m_Combo_TriType.AddString("Ordre décroissant"); 
	m_Combo_TriType.SetItemData(InxCmd, m_pDoc->TYPE_DECROISSANT);
	InxCmd = m_Combo_TriType.AddString("Ordre croissant"); 
	m_Combo_TriType.SetItemData(InxCmd, m_pDoc->TYPE_CROISSANT);

	// puis on positionne le type de tri courant
	if (m_Combo_TriType.GetCount() > 0)
	{
		for (JInt32 InxCmd = 0; InxCmd < m_Combo_TriType.GetCount(); InxCmd++)	
		{
			// Récup item data de l'élmt
			JInt32 ItemData = m_Combo_TriType.GetItemData(InxCmd);

			// Si c'est celui courant, on le sélectionne
			if (ItemData == m_pDoc->Get_TypeTri()) // m_TypeTri)
			{
				m_Combo_TriType.SetCurSel(InxCmd); 
				break;
			}
		}
	}
		
	// Sélection mode tri (sur toutes les lignes, par blocs de critères / MODE_LIGNE = 0, MODE_BLOC) 
	m_Combo_TriMode.ResetContent();		
	InxCmd = m_Combo_TriMode.AddString("Sur toutes les lignes"); 
	m_Combo_TriMode.SetItemData(InxCmd, m_pDoc->MODE_LIGNE);
	InxCmd = m_Combo_TriMode.AddString("Par bloc de critères"); 
	m_Combo_TriMode.SetItemData(InxCmd, m_pDoc->MODE_BLOC);

	// puis on positionne le mode de tri courant
	if (m_Combo_TriMode.GetCount() > 0)
	{
		for (JInt32 InxCmd = 0; InxCmd < m_Combo_TriMode.GetCount(); InxCmd++)	
		{
			// Récup item data de l'élmt
			JInt32 ItemData = m_Combo_TriMode.GetItemData(InxCmd);

			// Si c'est celui courant, on le sélectionne
			if (ItemData == m_pDoc->Get_ModeTri())  //m_ModeTri)
			{
				m_Combo_TriMode.SetCurSel(InxCmd); 
				break;
			}
		}
	}
}
*/

void CDlgAnalyseur::SetupControls()
{
	m_DlgSurlignage.		Create(CDlgSurlignage::IDD,this);
	m_DlgSurlignage.		ShowWindow(SW_HIDE);

	// Positionne la fenêtre surlignage en haut de la fenêtre principale
	m_DlgSurlignage.		BringWindowToTop();

	m_Box_Grid.				SubclassDlgItem(IDC_AN_BOX_GRID ,				this);
	m_Grid_Title.			SubclassDlgItem(IDC_AN_GRID_TITLE ,				this);

	m_Box_Config.			SubclassDlgItem(IDC_AN_BOX_CONFIG ,				this);
	m_Label_Terrains.		SubclassDlgItem(IDC_AN_LABEL_TERRAINS ,			this);
	m_Label_Terrains.		SetBorder(TRUE);
	m_List_Terrains.		SubclassDlgItem(IDC_AN_LIST_TERRAINS ,			this);

	m_List_TmpTerrains.		SubclassDlgItem(IDC_AN_LIST_TERRAINS2 ,			this);

	m_But_Up_Terrain.		SubclassDlgItem(IDC_AN_BUT_TERRAIN_UP ,			this);
	m_But_Down_Terrain.		SubclassDlgItem(IDC_AN_BUT_TERRAIN_DOWN ,		this);

	m_Label_Cibles.			SubclassDlgItem(IDC_AN_LABEL_CIBLES ,			this);
	m_List_Cibles.			SubclassDlgItem(IDC_AN_LIST_CIBLES ,			this);
	m_Label_Grandeurs.		SubclassDlgItem(IDC_AN_LABEL_GRANDEURS ,		this);
	m_List_Grandeurs.		SubclassDlgItem(IDC_AN_LIST_GRANDEURS ,			this);
	m_But_Vectorielle.		SubclassDlgItem(IDC_AN_BUT_VECTORIELLE ,		this);
	m_CrossTable.			SubclassDlgItem(IDC_AN_TABLE,					this);
	m_ProgCtrl.				SubclassDlgItem(IDC_AN_PROGRESS,				this);
	m_Label_Marquage.		SubclassDlgItem(IDC_AN_LABEL_MARQUAGE ,			this);

	//...............................  Elmts variables moyennables
	// Liste des elmts moyennables
	m_Label_VarMoy.			SubclassDlgItem(IDC_AN_LABEL_VAR_MOY ,			this);

	// Bouton Sélection Items à moyenne en mode tableau
	m_Btn_Moyenne_Tableau.	SubclassDlgItem(IDC_AN_BUT_MOYENNE_TABLEAU ,	this);

	// Liste des élmts moyennables
	m_List_VarMoy.			SubclassDlgItem(IDC_AN_LIST_VAR_MOY,			this);	

	// Bouton suppression des variables moyennables
	m_Btn_Raz_VarMoy.		SubclassDlgItem(IDC_AN_BUT_RAZ_VAR_MOY, 			this);

	// Boutons Surlignage et Ordonnancement
	m_Btn_PrecSurlign.		SubclassDlgItem(IDC_AN_BUT_PREC_SURLIGN,		this);
	m_But_Surlignage.		SubclassDlgItem(IDC_AN_BUT_PARAMSURLIGNAGE ,	this);
	m_Btn_Raz_AllSurlign.	SubclassDlgItem(IDC_AN_BUT_RAZ_SURLIGN,			this);
	m_But_Ordonner.			SubclassDlgItem(IDC_AN_BUT_ORDONNER ,			this);

	m_Label_Result.			SubclassDlgItem(IDC_AN_LABEL_RESULT ,			this);

	// Boutons sélection grandeur mode 2*2 
	m_But_Grandeur.			SubclassDlgItem(IDC_AN_BUT_GRANDEUR ,			this);
	m_But_Horiz.			SubclassDlgItem(IDC_AN_BUT_HORIZ	,			this);
	m_But_Vert.				SubclassDlgItem(IDC_AN_BUT_VERT		,			this);
	m_But_Global.			SubclassDlgItem(IDC_AN_BUT_GLOBAL	,			this);

	// Boutons sélection grandeur mode ligne
	m_But_Grandeur2.		SubclassDlgItem(IDC_AN_BUT_GRANDEUR2,			this);
	m_But_Horiz2.			SubclassDlgItem(IDC_AN_BUT_HORIZ2	,			this);
	m_But_Vert2.			SubclassDlgItem(IDC_AN_BUT_VERT2	,			this);
	m_But_Global2.			SubclassDlgItem(IDC_AN_BUT_GLOBAL2	,			this);

	// Boutons sélection grandeur mode colonne
	m_But_Grandeur3.		SubclassDlgItem(IDC_AN_BUT_GRANDEUR3,			this);
	m_But_Horiz3.			SubclassDlgItem(IDC_AN_BUT_HORIZ3	,			this);
	m_But_Vert3.			SubclassDlgItem(IDC_AN_BUT_VERT3	,			this);
	m_But_Global3.			SubclassDlgItem(IDC_AN_BUT_GLOBAL3	,			this);

	m_Combo_TriGrandeurs.   SubclassDlgItem(IDC_COMBO_TRI_GRANDEURS ,		this);
	m_Combo_TriResultats.   SubclassDlgItem(IDC_COMBO_TRI_RESULTATS ,		this); 
	m_Combo_TriType.		SubclassDlgItem(IDC_COMBO_TYPETRI ,				this);
	m_Combo_TriMode.		SubclassDlgItem(IDC_COMBO_MODETRI ,				this);
	m_Label_Tri.			SubclassDlgItem(IDC_AN_LABEL_TRI ,				this);

	m_But_Exclusifs.		SubclassDlgItem(IDC_AN_BUT_EXCLU ,				this);
	m_But_Dupliquants.		SubclassDlgItem(IDC_AN_BUT_DUPLI ,				this);

//	m_But_Global.SetMenu(IDR_AN_MENU_GLOBAL, *this, true);

    // done in OnInitialUpdate.
	m_CrossTable.EnableIntelliMouse();
    m_CrossTable.Initialize();
	m_CrossTable.LockUpdate(true); 

    m_CrossTable.GetParam()->EnableUndo(false);
	m_CrossTable.GetParam()->SetSmartResize(true);
	m_CrossTable.GetParam()->SetNewGridLineMode(TRUE);

	m_CrossTable.GetParam()->EnableSelection(GX_SELROW|GX_SELCOL|GX_SELTABLE|GX_SELCELL);

	m_CrossTable.m_nClipboardFlags = (GX_DNDTEXT|GX_DNDCOLHEADER|GX_DNDROWHEADER);

	m_CrossTable.GetParam()->SetNumberedColHeaders(false); 

	m_CrossTable.GetParam()->GetProperties()->SetMarkColHeader(FALSE);

	m_CrossTable.GetParam()->EnableMoveCols(FALSE);
	m_CrossTable.GetParam()->SetMinTrackColWidth(40);
	m_CrossTable.GetParam()->EnableTrackColWidth(GX_TRACK_INDIVIDUAL | GX_TRACK_SMOOTHHEADER | GX_TRACK_NOPRESSEDHEADER | GX_TRACK_NOMARKTRACKLINE | GX_TRACK_EXTHITTEST);

	m_CrossTable.GetParam()->SetNumberedRowHeaders(false); 
	m_CrossTable.GetParam()->GetProperties()->SetMarkRowHeader(FALSE); 
	m_CrossTable.GetParam()->EnableMoveRows(FALSE);
	m_CrossTable.GetParam()->SetMinTrackRowHeight(16);
	m_CrossTable.GetParam()->EnableTrackRowHeight(GX_TRACK_INDIVIDUAL | GX_TRACK_SMOOTHHEADER | GX_TRACK_NOPRESSEDHEADER | GX_TRACK_NOMARKTRACKLINE | GX_TRACK_EXTHITTEST);

	// A voir pour Tooltips
	m_CrossTable.EnableToolTips(true); 

	/*
	// Ajoute Ascenseur Vertical et Horizontal
	m_CrossTable.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	m_CrossTable.UpdateScrollbars(TRUE,FALSE);
		
	// Scrolling automatique
	m_CrossTable.SetAutoScroll(TRUE);
	*/

	m_Grid_Title.SetFontName(CATPColors::GetFontLabelName());
	//m_Grid_Title.SetFontSize(CATPColors::GetFontBigLabelSize());
	m_Grid_Title.SetFontBold(TRUE);
	m_Grid_Title.SetFontSize(CATPColors::GetFontLabelSize());
	m_Grid_Title.SetBorder(false);

	m_Label_Terrains.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Terrains.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Terrains.SetFontBold(true);
	m_Label_Terrains.SetBorder(false);

	m_Label_Cibles.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Cibles.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Cibles.SetFontBold(true);
	m_Label_Cibles.SetBorder(false);
	
	m_Label_Result.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Result.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Result.SetFontBold(true);
	m_Label_Result.SetBorder(false);

	m_Label_Marquage.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Marquage.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Marquage.SetFontBold(true);
	m_Label_Marquage.SetBorder(false);

	m_Label_Grandeurs.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Grandeurs.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Grandeurs.SetFontBold(true);
	m_Label_Grandeurs.SetBorder(false);

	m_Label_VarMoy.SetFontName(CATPColors::GetFontLabelName());
	m_Label_VarMoy.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_VarMoy.SetFontBold(true);
	m_Label_VarMoy.SetBorder(false);

	m_Label_Tri.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Tri.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Tri.SetFontBold(true);
	m_Label_Tri.SetBorder(false);

	/*
	m_Label_Tableau.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Tableau.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Tableau.SetBorder(false);
	*/

	m_ProgCtrl.SetRange32(0,0); 
	m_ProgCtrl.SetPos(0);


	m_Box_Grid.SetTransparent(false);
	m_Box_Grid.SetSunken(true);
	m_Box_Grid.AddTopWindow(m_Grid_Title);
	m_Box_Grid.AddTopWindow(m_Box_Strates);
	m_Box_Grid.AddTopWindow(m_CrossTable);
	m_Box_Grid.AddTopWindow(m_ProgCtrl); 
	
	m_Box_Config.SetTransparent(false);
	m_Box_Config.SetSunken(true);
	m_Box_Config.AddTopWindow(m_Label_Terrains );
	m_Box_Config.AddTopWindow(m_List_Terrains );
	m_Box_Config.AddTopWindow(m_List_TmpTerrains );

	m_Box_Config.AddTopWindow(m_But_Up_Terrain);
	m_Box_Config.AddTopWindow(m_But_Down_Terrain);

	m_Box_Config.AddTopWindow(m_Label_Cibles );
	m_Box_Config.AddTopWindow(m_List_Cibles );
	m_Box_Config.AddTopWindow(m_Label_Grandeurs );
	m_Box_Config.AddTopWindow(m_List_Grandeurs  );
	m_Box_Config.AddTopWindow(m_Label_Result );

	m_Box_Config.AddTopWindow(m_But_Grandeur );
	m_Box_Config.AddTopWindow(m_But_Vert );
	m_Box_Config.AddTopWindow(m_But_Horiz );
	m_Box_Config.AddTopWindow(m_But_Global );
	m_Box_Config.AddTopWindow(m_But_Grandeur2 );
	m_Box_Config.AddTopWindow(m_But_Vert2 );
	m_Box_Config.AddTopWindow(m_But_Horiz2 );
	m_Box_Config.AddTopWindow(m_But_Global2 );
	m_Box_Config.AddTopWindow(m_But_Grandeur3 );
	m_Box_Config.AddTopWindow(m_But_Vert3 );
	m_Box_Config.AddTopWindow(m_But_Horiz3 );
	m_Box_Config.AddTopWindow(m_But_Global3 );

	m_Box_Config.AddTopWindow(m_But_Exclusifs );
	m_Box_Config.AddTopWindow(m_But_Dupliquants );
	m_Box_Config.AddTopWindow(m_Label_Marquage );

	m_Box_Config.AddTopWindow(m_Label_VarMoy);
	m_Box_Config.AddTopWindow(m_Btn_Moyenne_Tableau);
	m_Box_Config.AddTopWindow(m_List_VarMoy);
	m_Box_Config.AddTopWindow(m_Btn_Raz_VarMoy);

	m_Box_Config.AddTopWindow(m_Btn_PrecSurlign);
	m_Box_Config.AddTopWindow(m_But_Surlignage);
	m_Box_Config.AddTopWindow(m_Btn_Raz_AllSurlign);
	m_Box_Config.AddTopWindow(m_But_Ordonner);

	// m_Box_Config.AddTopWindow(m_But_Vectorielle );
	m_Box_Grid.AddTopWindow(m_But_Vectorielle );

	m_Box_Config.AddTopWindow(m_Label_Tri);
	m_Box_Config.AddTopWindow(m_Combo_TriGrandeurs);    
	m_Box_Config.AddTopWindow(m_Combo_TriResultats);    
	m_Box_Config.AddTopWindow(m_Combo_TriType);			
	m_Box_Config.AddTopWindow(m_Combo_TriMode);			

	// Set the windows colors
	SetColors();

	// Set window relations and positions for resizing
	SetWindowRelations();
}

void CDlgAnalyseur::SetColors()
{
	// Template system de base
	const CXTemplate & systTemplate = CXTemplateManager::GetInstance()->GetTemplate(CXTemplateManager::GetSystemTemplate());

	// Définition template pour listes elmts standards
	m_TemplateStandard = CXTemplateManager::GetInstance()->CreateTemplate(systTemplate.textcolor_, 
																		  systTemplate.backcolor_,
																		  systTemplate.seltextcolor_,
																		  CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),
																		  false,false,false); 
	

	// Définition template pour listes elmts quantitatifs
	m_TemplateQuanti   = CXTemplateManager::GetInstance()->CreateTemplate(systTemplate.textcolor_, 
																		  systTemplate.backcolor_,
																		  systTemplate.seltextcolor_,
																		  CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),
																		  false,true,false); 


	// Font et border des boites
	m_Box_Grid.				SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR), CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box_Config.			SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_ProgCtrl.				SetBkColor(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

	// Les Boutons Up/Down Terrains
	m_But_Up_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Up_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Up_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Up_Terrain.		SetTooltipText("Remonte le terrain dans la liste");
	m_But_Up_Terrain.		SetBitmaps(IDB_RI_UP,RGB(192,192,192)); 

	m_But_Down_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Down_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Down_Terrain.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Down_Terrain.		SetTooltipText("Redescend le terrain dans la liste");
	m_But_Down_Terrain.		SetBitmaps(IDB_RI_DOWN,RGB(192,192,192)); 

	m_List_Terrains.		SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));
	m_List_Grandeurs.		SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));
	
	for (JUnt32 InxLst = 0; InxLst < m_List_Cibles.GetCount(); InxLst++)
	{
		m_List_Cibles.SetTextColor(InxLst, CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR), true);
	}

	// Couleur des élmts pour var. m_List_Cibles
	m_Btn_Moyenne_Tableau.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR ,0);
	m_Btn_Moyenne_Tableau.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Moyenne_Tableau.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Moyenne_Tableau.	SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR); 
	m_Btn_Moyenne_Tableau.	SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR);	

	// Couleur de sélection pour la liste des variables moyennables
	m_List_VarMoy.			ChangeItemRangeTemplate(m_TemplateQuanti);

	// Coloration bouton annulation sélection variables moyennables en mode tableau
	m_Btn_Raz_VarMoy.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Btn_Raz_VarMoy.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Raz_VarMoy.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Raz_VarMoy.		SetTooltipText("Suppression sélection des variables moyennables");
	m_Btn_Raz_VarMoy.		SetIcon(IDI_AN_RAZ,	IDI_AN_RAZ); 

	// Les boutons sélection résultats
	m_But_Grandeur.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Grandeur.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR); 
	m_But_Grandeur.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR);	

	
	m_But_Grandeur2.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Grandeur2.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur2.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur2.		SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR); 
	m_But_Grandeur2.		SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR);	
	
	m_But_Grandeur3.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Grandeur3.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur3.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Grandeur3.		SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR); 
	m_But_Grandeur3.		SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult1(),CATPColors::COLORANALYSEUR);	

	m_But_Vert.				SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Vert.				SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert.				SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert.				SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR); 
	m_But_Vert.				SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR);	

	m_But_Vert2.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Vert2.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert2.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert2.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR); 
	m_But_Vert2.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR);	

	m_But_Vert3.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Vert3.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert3.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vert3.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR); 
	m_But_Vert3.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult2(),CATPColors::COLORANALYSEUR);	

	m_But_Horiz.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Horiz.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR); 
	m_But_Horiz.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR);	

	m_But_Horiz2.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Horiz2.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz2.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz2.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR); 
	m_But_Horiz2.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR);	

	m_But_Horiz3.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Horiz3.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz3.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Horiz3.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR); 
	m_But_Horiz3.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult3(),CATPColors::COLORANALYSEUR);	

	m_But_Global.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Global.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR); 
	m_But_Global.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR);	

	m_But_Global2.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Global2.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global2.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global2.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR); 
	m_But_Global2.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR);	

	m_But_Global3.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Global3.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global3.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Global3.			SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR); 
	m_But_Global3.			SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	CATPColors::GetColorResult4(),CATPColors::COLORANALYSEUR);	
	m_But_Exclusifs.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Exclusifs.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Exclusifs.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Dupliquants.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Dupliquants.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Dupliquants.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);

	// Coloration bouton retour arrière surlignage
	m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_PrecSurlign.		SetTooltipText("Suppression de la dernière opération surlignage");
	m_Btn_PrecSurlign.		SetIcon(IDI_AN_UNDO,	IDI_AN_UNDO); 

	// Coloration bouton paramétrage surlignage
	m_But_Surlignage.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);  // CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),0);
	m_But_Surlignage.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Surlignage.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Surlignage.		SetTooltipText("Définir les paramètres de surlignage");

	// Coloration bouton retour arrière surlignage
	m_Btn_Raz_AllSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_Btn_Raz_AllSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Raz_AllSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_Btn_Raz_AllSurlign.	SetTooltipText("Suppression de la totalité du surlignage");
	m_Btn_Raz_AllSurlign.   SetIcon(IDI_AN_RAZ,	IDI_AN_RAZ); 
	
	// Coloration bouton ordonnancement
	m_But_Ordonner.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Ordonner.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Ordonner.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Ordonner.			SetTooltipText("Sélection de la colonne à mettre en surlignage");

	m_But_Vectorielle.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Vectorielle.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vectorielle.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Vectorielle.		SetBitmaps(IDB_AN_PUPITRE, RGB(192,192,192)); 
	m_But_Vectorielle.		SetTooltipText("Sélection des items lignes / colonnes / pages"); 

	m_Label_Cibles.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Result.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Marquage.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Grandeurs.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);

	m_Label_Terrains.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Grid_Title.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_VarMoy.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);
	m_Label_Tri.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);

	m_Label_Cibles.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Result.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_Label_Marquage.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Grandeurs.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Terrains.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Grid_Title.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_VarMoy.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 
	m_Label_Tri.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 

	m_DlgSurlignage.		SetColors();

	m_CrossTable.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
	m_CrossTable.Invalidate();
	m_CrossTable.Redraw();

	// Redessine les boites conteneurs
	m_Box_Grid.RedrawWindow();	
	m_Box_Config.RedrawWindow();
}

void CDlgAnalyseur::SetWindowRelations()
{
	// Set all widow relation positions
	static CResizer::CBorderInfo s_bi[] = {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  CONFIG BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_AN_BOX_CONFIG,			{CResizer::eFixed,			IDC_MAIN, CResizer::eRight},					//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},						//Top side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eRight},					//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eBottom}},					//Bottom side
	/*
	{IDC_AN_BUT_MATRICIELLE,	{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side
	*/

	/*
	{IDC_AN_BUT_VECTORIELLE,	{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side
	*/

	/*
	{IDC_AN_LABEL_TABLEAU,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side
	*/

	{IDC_AN_BUT_PREC_SURLIGN ,  {CResizer::eFixed,		    IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_BUT_PARAMSURLIGNAGE , {CResizer::eFixed,		IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_BUT_RAZ_SURLIGN ,   {CResizer::eFixed,		    IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_BUT_ORDONNER ,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_BUT_BASE100,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_LABEL_MARQUAGE,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	
	// Disposition des boutons mode affcihage																
	{IDC_AN_BUT_EXCLU,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side


	{IDC_AN_BUT_DUPLI,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_LABEL_TERRAINS,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop}},			//Bottom side

	{IDC_AN_BUT_TERRAIN_UP,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop}},			//Bottom side

	{IDC_AN_BUT_TERRAIN_DOWN,	{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eTop}},			//Bottom side

	{IDC_AN_LIST_TERRAINS,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_TERRAINS, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eProportional,	IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_LABEL_CIBLES,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_LIST_TERRAINS, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_LIST_TERRAINS, CResizer::eBottom}},		//Bottom side

	{IDC_AN_LIST_CIBLES,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_CIBLES, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eProportional,	IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_AN_LABEL_GRANDEURS,	{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_LIST_CIBLES, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_LIST_CIBLES, CResizer::eBottom}},		//Bottom side

	{IDC_AN_LIST_GRANDEURS,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_GRANDEURS, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eProportional,	IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side


	{IDC_AN_LABEL_RESULT,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LIST_GRANDEURS,  CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LIST_GRANDEURS,  CResizer::eBottom}},	//Bottom side

    {IDC_AN_BUT_GRANDEUR,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_VERT,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_HORIZ,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_GLOBAL,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_GRANDEUR2,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_VERT2,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_HORIZ2,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_GLOBAL2,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_GRANDEUR3,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_VERT3,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_HORIZ3,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_GLOBAL3,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_RESULT,	CResizer::eBottom}},	//Bottom side

	// Les controles pour les variables moyennables
	{IDC_AN_LABEL_VAR_MOY,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_BUT_GLOBAL3,		CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_BUT_GLOBAL3,		CResizer::eBottom}},	//Bottom side

	{IDC_AN_BUT_MOYENNE_TABLEAU,{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_VAR_MOY,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_VAR_MOY,	CResizer::eBottom}},	//Bottom side

	{IDC_AN_LIST_VAR_MOY,		{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_VAR_MOY,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								// {CResizer::eProportional,	IDC_AN_BOX_CONFIG,		CResizer::eBottom}},	//Bottom side
								{CResizer::eFixed,			IDC_AN_LABEL_MARQUAGE,	CResizer::eTop}},		//Bottom side	

								

	{IDC_AN_BUT_RAZ_VAR_MOY,	{CResizer::eFixed,		    IDC_AN_BOX_CONFIG,		CResizer::eLeft},		//Left side
								{CResizer::eFixed,			IDC_AN_LABEL_VAR_MOY,	CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG,		CResizer::eRight},		//Right side
								{CResizer::eFixed,			IDC_AN_LABEL_VAR_MOY,	CResizer::eBottom}},	//Bottom side

	// Disposition des éléments pour sélection tri courant
	{IDC_AN_LABEL_TRI,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_COMBO_TRI_GRANDEURS,   {CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_COMBO_TRI_RESULTATS,   {CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side

	{IDC_COMBO_TYPETRI,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side	

	{IDC_COMBO_MODETRI,			{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Left side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eBottom}},			//Bottom side
	

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  GRID BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////
	{IDC_AN_BOX_GRID,			{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft},						//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},						//Top side
								{CResizer::eFixed,			IDC_AN_BOX_CONFIG, CResizer::eLeft},			//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eBottom}},					//Bottom side

	{IDC_AN_GRID_TITLE ,		{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eLeft},				//Left side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eRight},				//Right side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eTop}},				//Bottom side

	{IDC_AN_BUT_VECTORIELLE,	{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eLeft},				//Left side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eLeft},				//Right side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eTop}},				//Bottom side


	{IDC_AN_PROGRESS ,			{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eLeft},				//Left side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eBottom},			//Top side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eRight},				//Right side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eBottom}},			//Bottom side

	{IDC_AN_TABLE ,				{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eLeft},				//Left side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_AN_BOX_GRID, CResizer::eRight},				//Right side
								{CResizer::eFixed,			IDC_AN_PROGRESS, CResizer::eTop}},				//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

void CDlgAnalyseur::OnUpdatePalette1(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 0) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette1()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,0);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette2(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 1) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette2()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,1);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette3(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 2) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette3()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,2);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette4(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 3) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette4()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,3);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette5(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 4) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette5()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,4);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette6(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 5) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette6()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,5);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette7(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 6) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette7()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,6);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette8(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 7) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette8()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,7);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette9(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 8) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette9()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,8);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette10(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 9) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette10()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,9);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette11(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 10) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette11()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,10);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette12(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 11) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette12()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,11);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette13(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 12) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette13()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,12);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette14(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 13) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette14()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,13);
	SetColors(); 
}

void CDlgAnalyseur::OnUpdatePalette15(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORANALYSEUR) == 14) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CDlgAnalyseur::OnPalette15()
{
	CATPColors::SetAppColor(CATPColors::COLORANALYSEUR,14);

	SetColors(); 
}


// Affichage fenêtre présentoit tri croisé
void CDlgAnalyseur::OnBnClickedButVectorielle()
{
	// Aller dans pupitre

	GoPupitre();


}

//////////////////////////////////////////////////////////////////////////////////////////////
// Redirection vers la construction Tris Croises = Pupitre
void CDlgAnalyseur::GoPupitre()
{
	// positionne les élmts déjà sélectionné
	m_pDlgTableau->SetData(m_pDoc);

	// Invalide le bouton Affichage Pupitre
	m_But_Vectorielle.EnableWindow(FALSE); 

	// Ponteur sur terrain temoin en cours
	CTerrain *pTerrainTemoin = m_pDoc->Get_TerrainTemoin();

	if (pTerrainTemoin)
	{
		// Les segments en cours
		const JList<UINT> & SegmentsId = m_pDoc->Get_SegmentsId();
		
		// L'identificateur vecteur poids
		m_pDlgTableau->SetParams(m_pDoc->Get_TypePoidsId(), SegmentsId);

		// Lance le sélecteur en mode ligne
		m_pDlgTableau->SetMode(CPaletteMode::Line); 

		// Ouverture Répertoire des items en mode Pupitre
		if(m_pDlgTableau->DoModal() == IDOK)
		{
			// Sauve positionnement fenêtre pupitre
			// WRITEPROFILESTRING(SECTION_OPTIONS_ANALYSEUR, KEY_RECT_PUPITRE, strText);

			// Récupération des élmts sélectionnés pour tris croisés (lignes + colonnes + pages)
			InvalidateRect(NULL, FALSE);
			UpdateWindow();
			m_pDlgTableau->GetData(m_pDoc);
			if((m_pDoc->Get_ResultStyle()&(RSTYLE_RESULT | RSTYLE_GLOBAL | RSTYLE_HORIZONTAL | RSTYLE_VERTICAL)) == 0)
			{
				// Force grandeur effectif
				m_pDoc->Set_ResultStyle(RSTYLE_RESULT);
				SetResultControls();
			}

			CWaitCursor Cw;
			if(m_pCrossTable)
			{
				m_pCrossTable->CalculateItems();
				UpdateSurlignage();
			}

			// Puis on positionne le source et terrain de référence
			CTerrain * pTerrainRef = m_pDoc->Get_TerrainTemoin();
			JUnt32 IdSourceRef	= pTerrainRef->GetSourceID();
			JUnt32 IdTerrainRef = pTerrainRef->GetTerrainID();

			m_pDoc->Set_SourceIdRef(IdSourceRef);
			m_pDoc->Set_TerrainIdRef(IdTerrainRef);
			
			// Provisoire, sinon rien n'apparait si seulement grandeur moyenne sélectionnée
			// On recalcule si uniquement moyenne de sélectionner  / provisoire
			if (m_List_Grandeurs.GetSelCount() == 1)
			{
				JInt32 ItemData = m_List_Grandeurs.GetItemData(m_List_Grandeurs.GetCurSel());
				if (ItemData ==	RTYPE_AVERAGEQUANTUM_P00 || ItemData == RTYPE_AVERAGEQUANTUM || ItemData == RTYPE_QUANTITY_000)
				{
					m_pDoc->Set_Calculation(CALC_ROWS | CALC_COLS);
					m_pCrossTable->CalculateItems();
				}
			}
		}
	}
	else
	{
		// Pas de terrain actif, signaler à l'utilisateur d'en sélectionner au moins 1
		AfxMessageBox("Pas de terrain en cours, veuillez en sélectionner un !!!");
	}

	// Remet le bouton actif
	m_But_Vectorielle.EnableWindow(TRUE); 

	// Redessine la partie droite box config
	m_Box_Grid.RedrawWindow();
	m_Box_Config.RedrawWindow();

	// Redessine la fenêtre
	this->RedrawWindow();
}

void CDlgAnalyseur::SetGrandeurs()
{
	CString Str;
	// Remove all selected Grandeurs
	int NbSel = m_List_Grandeurs.GetSelCount();
	int  *pSel = new int[NbSel];
	m_List_Grandeurs.GetSelItems(NbSel,pSel);

	// Init Flag au moins 1 grandeur moyennable (pour le moment on teste uniquement que les grandeurs sont validées)
	m_pDoc->m_ElmtGrandeurMoyennableSel = false;

	for(int i = 0; i < NbSel ; ++i)
		m_List_Grandeurs.SetSel(pSel[NbSel],false); 
	delete [] pSel;
	
	// Set selected Grandeurs
	const JList< RESULTTYPE > & Grandeurs = m_pDoc->Get_Grandeurs();
	for(Grandeurs.MoveFirst(); Grandeurs.IsValid(); Grandeurs.MoveNext())
	{
		int index = 2;

		switch(Grandeurs.GetItem())
		{
			case RTYPE_EFFECTIVE_000:
				m_pDoc->GetGrandeurString(Str,RTYPE_EFFECTIVE_000);
				index = -1;
				break;
			case RTYPE_EFFECTIVE_CIBLE:
				m_pDoc->GetGrandeurString(Str,RTYPE_EFFECTIVE_CIBLE);
				index = 0;
				break;
			case RTYPE_NBCAS:
				m_pDoc->GetGrandeurString(Str,RTYPE_NBCAS);
				index = 1;
				break;
			case RTYPE_QUANTITY_000:
				m_pDoc->GetGrandeurString(Str,RTYPE_QUANTITY_000);
				m_pDoc->m_ElmtGrandeurMoyennableSel = true;
				break;
			case RTYPE_AVERAGEQUANTUM:
				m_pDoc->GetGrandeurString(Str,RTYPE_AVERAGEQUANTUM);
				m_pDoc->m_ElmtGrandeurMoyennableSel = true;
				break;
			case RTYPE_AVERAGEQUANTUM_P00:
				m_pDoc->GetGrandeurString(Str,RTYPE_AVERAGEQUANTUM_P00);
				m_pDoc->m_ElmtGrandeurMoyennableSel = true;
				break;
			case RTYPE_PENETRATION:
				m_pDoc->GetGrandeurString(Str,RTYPE_PENETRATION);
				break;
			case RTYPE_CENTRAGE:
				m_pDoc->GetGrandeurString(Str,RTYPE_CENTRAGE);
				break;
			case RTYPE_CONCOMITENCE:
				m_pDoc->GetGrandeurString(Str,RTYPE_CONCOMITENCE);
				break;
			case RTYPE_COEFFYULE:
				m_pDoc->GetGrandeurString(Str,RTYPE_COEFFYULE);
				break;
			case RTYPE_ZSCORE:
				m_pDoc->GetGrandeurString(Str,RTYPE_ZSCORE);
				break;
		}
		index = m_List_Grandeurs.FindString(index,Str);
		if(index != LB_ERR)
			m_List_Grandeurs.SetSel(index,true);
	}
	GetGrandeurs();
}

void CDlgAnalyseur::GetGrandeurs()
{
	if(m_List_Grandeurs.GetSelCount()==0)
		m_List_Grandeurs.SetSel(0,true); 
	// Get selected grandeurs
	int NbSel = m_List_Grandeurs.GetSelCount();
	int  *pSel = new int[NbSel];
	m_List_Grandeurs.GetSelItems(NbSel,pSel);   

	// Remove all granduers from list
	JList<RESULTTYPE>	Grandeurs;

	// Set the selected grandeurs
	for(int i = 0; i < NbSel ; ++i)
	{
		Grandeurs.AddTail() = (RESULTTYPE)m_List_Grandeurs.GetItemData(pSel[i]);
	}


	delete [] pSel;

	m_pDoc->Set_Grandeurs(Grandeurs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recup info si au moins 1 grandeur moyennable de sélectionner
void CDlgAnalyseur::UpdateElmtMoyennable()
{
	// OPTIM 2.7
	m_pDoc->m_ElmtGrandeurMoyennableSel = false;

	const JList< RESULTTYPE > & Grandeurs = m_pDoc->Get_Grandeurs();

	// Set the selected grandeurs
	for(Grandeurs.MoveFirst(); Grandeurs.IsValid(); Grandeurs.MoveNext())
	{
		// OPTIM 2.7
		if (Grandeurs.GetItem() == RTYPE_QUANTITY_000 || Grandeurs.GetItem() == RTYPE_AVERAGEQUANTUM || Grandeurs.GetItem() == RTYPE_AVERAGEQUANTUM_P00)
		{
			m_pDoc->m_ElmtGrandeurMoyennableSel = true;
			return;
		}
	}
}

void CDlgAnalyseur::OnLbnSelchangeAnListGrandeurs()
{
	CWaitCursor Cw;

	// Liste des types de grandeurs utilisées
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	// Nombre de grandeurs avant nouvelle sélection
	int ExNbGrandeurs = CalcTypes.GetCount();

	// Nombre de grandeurs nouvellement sélectionnées
	int NewNbGrandeurs = m_List_Grandeurs.GetSelCount();

	// Recup position ascenseur horizontal et vertical
	ROWCOL LeftCol = m_CrossTable.GetLeftCol();
	ROWCOL TopRow = m_CrossTable.GetTopRow();

	// Get the selected grandeurs
	GetGrandeurs();

	// Remet en place les sélecteurs pour le tri
	SetupSelTri();

	// Update the table to show the grandeurs
	if(m_pCrossTable)
	{
		// A REMETTRE OPTIM 2.7
		m_CrossTable.LockUpdate(true);

		// Recalcul les valeurs des cellules et on les affiche
		m_pCrossTable->CalculateItems(false);

		// Mise à jour surlignagne
		// UpdateSurlignage();
	}

	// Remet l'information titre
	SetTitre();

	// Mise à jour Map des cellules à surligner
	UpdateSurlignage();

	// Remet Row & Col Scroll Initiale
	GoToInitialPosCroll3(TopRow, LeftCol, ExNbGrandeurs, NewNbGrandeurs);
	m_CrossTable.SetLeftCol(LeftCol);
	m_CrossTable.SetTopRow(TopRow);

	// Redessine après recalage ligne colonne via posotion scrolls
	m_pCrossTable->RedrawGrid();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Modification de la variable moyennable en mode tableau
void CDlgAnalyseur::OnLbnSelchangeAnListVarMoy()
{
	CWaitCursor Cw;

	// Idem changement de grandeurs
	// OnLbnSelchangeAnListGrandeurs();

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Get the selected grandeurs
	GetGrandeurs();

	// Remet en place les sélecteurs pour le tri
	SetupSelTri();
	///////////////////////////////////////////////////////////////////////////////////////////////

	// Mise à jour variable moyennable
	if (m_List_VarMoy.GetCurSel() >= 0)
	{
		// écupère l'item wrapper correspondant
		m_pDoc->m_pItemWrapperMoy = (CItemWrapper *)m_List_VarMoy.GetItemData(m_List_VarMoy.GetCurSel());
	}
	else
		// Pas d'item wrapper moyenne
		m_pDoc->m_pItemWrapperMoy = 0;

	// Sauve les variables moyennables dans document
	LIST_ITEM_PUPITRE AllVarMoy;
	for (JUnt32 InxLst = 0; InxLst < m_List_VarMoy.GetCount(); InxLst++)
	{
		// Ajoute ce nouveau wrapper à la liste globale
		AllVarMoy.AddTail() = (CItemWrapper *)m_List_VarMoy.GetItemData(InxLst);
	}

	// Récupére liste des variables moyennables au niveau du document
	m_pDoc->SetAllVariablesMoyennables(AllVarMoy);

	// Remet la cible à calculer
	int Sel = m_List_Cibles.GetCurSel();
	if(Sel != LB_ERR)
	{
		m_pDoc->Set_SelectedCible(0);
		m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
	}
	else 
		m_pDoc->Set_SelectedCible(0);

	// Update résultat tableau
	SetTitre();
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();

	// Elmts mode moyenne tableau visibles
	UpdateElmtVarMoy();
}

void CDlgAnalyseur::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
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

void CDlgAnalyseur::OnChangerTerrains()
{
	bool bChanged = false;

	// On doit garder la source et terrain de référence
	// La source avant le changement -- Attention avec la Source et Terrain Référence
    CTerrain *pTerrainTemoin = m_pDoc->Get_TerrainTemoin();

	JUnt32 IdSourcePrec = 0;
	if (pTerrainTemoin)
		IdSourcePrec = pTerrainTemoin->GetSourceID(); 

	CSrcUniqueDlg Dlg(this);
	Dlg.SetData(m_pDoc->Get_SourceId(), m_pDoc->Get_TerrainId(), m_pDoc->Get_SegmentsId(), m_pDoc->Get_TypePoidsId());

	if(Dlg.DoModal()==IDOK)
	{
		JUnt32					SourceId;
		JList<UINT>				TerrainId;
		JUnt32					TypePoidsId;
		JList<UINT>				SegmentsId;
		Dlg.GetData(SourceId,TerrainId,SegmentsId,TypePoidsId);


		// A VOIR AVEC SYLVAIN
		// ????????? Ici pour le moment on interdit le changement de source
		bChanged = LoadTerrains(SourceId,TerrainId,SegmentsId,TypePoidsId);

		// Init Elmt Moyennes mode tableau
		if (IdSourcePrec != SourceId)
			m_List_VarMoy.ResetContent();

		// Update btn moyenne
		UpdateElmtVarMoy();

		SetSelectedUniverses(bChanged);
		CheckItems();
		CWaitCursor Cw;

		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		SetTitre();

		// Initialise sauvegarde sélection et déploiement index thématique associé à nvlle sélection terrain
		m_pDoc->ResetEtatTreeTheme(bChanged);

		// Initialise les elmts à moyenner
		m_pDoc->ResetEtatTreeQuanti(bChanged);
	}
}

void CDlgAnalyseur::OnUpdateChoisirUnivers(CCmdUI *pCmdUI)
{
	if(m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(TRUE);
}

void CDlgAnalyseur::OnChoisirUnivers()
{
	CCibleSelectionDlg Dlg(this);

	Dlg.SetData(m_pDoc);  
	if(Dlg.DoModal() == IDOK)
	{
		UpdateListCibles();
	}
}

bool CDlgAnalyseur::LoadTerrains(const JUnt32 SourceId, const JList<UINT> & TerrainIds, const JList<UINT> & Segments, const JUnt32 TypePoidsId)
{
	CWaitCursor Wait;

	bool bChanged = m_pDoc->LoadTerrains(SourceId, TerrainIds, Segments, TypePoidsId);
	JMap<UINT, PTR_TERRAINDATA>	& TerrainData = m_pDoc->Get_ListTerrainData();

	m_List_Terrains.ResetContent(); 
	const JList<JInt32> & SelList = m_pDoc->Get_SelectedTerrains();

	// On boucle sur la liste des terrains (via la liste d'origine, même ordre que l'offre)
	JUnt32 NbTerrain = TerrainIds.GetCount(); 
	for(TerrainIds.MoveFirst(); TerrainIds.IsValid(); TerrainIds.MoveNext())
	{
		// Key Terrain
		JUnt32 IdTerrain = TerrainIds.GetItem(); 

		// On se place sur info terrains (Id + Label)
		TerrainData.MoveTo(IdTerrain);

		// Voir si sélectionné
		bool bb = false;
		for(SelList.MoveFirst(); SelList.IsValid() ; SelList.MoveNext())
		{
			if(TerrainData.GetItem()->m_TerrainId == SelList.GetItem())
			{
				// Terrain bien sélectionné
				bb = true;
				break;
			}
		}

		// Ajoute terrains
		if (TerrainData.IsValid())
		{
			int index = m_List_Terrains.AddString(TerrainData.GetItem()->GetTerrainLabel());
			m_List_Terrains.SetItemData(index, TerrainData.GetItem()->m_TerrainId);
			m_List_Terrains.SetSel(index,bb);   
		}
	}

	/*
	for(TerrainData.MoveFirst(); TerrainData.IsValid(); TerrainData.MoveNext())
	{
		// Key Terrain
		JUnt32 IdTerrain = TerrainData.GetKey(); 
		bool bb = false;
		for(SelList.MoveFirst(); SelList.IsValid() ; SelList.MoveNext())
		{
			if(TerrainData.GetItem()->m_TerrainId == SelList.GetItem())
			{
				bb = true;
				break;
			}
		}
		int index = m_List_Terrains.AddString(TerrainData.GetItem()->GetTerrainLabel());
		m_List_Terrains.SetItemData(index, TerrainData.GetItem()->m_TerrainId);
		m_List_Terrains.SetSel(index,bb);   
	}
	*/

	return bChanged;
}

void CDlgAnalyseur::CheckItems()
{
	JList<const CItemConstruit *> Temp;
	const JList<const CItemConstruit *> & ListCibles = m_pDoc->Get_ListCibles();
	const JMap<UINT, PTR_TERRAINDATA> & pTerrainData  = m_pDoc->Get_ListTerrainData(); 

	for(ListCibles.MoveFirst(); ListCibles.IsValid(); ListCibles.MoveNext())
	{
		const CItemConstruit *pItem = ListCibles.GetItem();
		bool bValid = true;
		for(pTerrainData.MoveFirst(); pTerrainData.IsValid(); pTerrainData.MoveNext())
		{
			if(!(pItem->IsValid(pTerrainData.GetItem()->m_pTerrain)))
				bValid = false;
		}
		if(bValid)
			Temp.AddTail() = pItem;
	}
	m_pDoc->Set_ListCibles(Temp);
	UpdateListCibles();
}

void CDlgAnalyseur::SetSelectedUniverses(bool bChanged)
{
	JList<CCalcItemParams *> Universes;

	if(m_List_Terrains.GetCount() > 0 && m_List_Terrains.GetSelCount() == 0)
		m_List_Terrains.SetSel(0,1);  

	int NbItems = m_List_Terrains.GetSelCount();
	int * pInt = new int[NbItems];
	m_List_Terrains.GetSelItems(NbItems,pInt);

	const JMap<UINT, PTR_TERRAINDATA> & pTerrainData  = m_pDoc->Get_ListTerrainData(); 
	JList<JInt32> SelList;

	for(int x=0; x < NbItems; x++)
	{
		pTerrainData.MoveTo(m_List_Terrains.GetItemData(pInt[x]));
		if(pTerrainData.IsValid())
			SelList.AddTail() = pTerrainData.GetItem()->m_TerrainId;
	}
	delete [] pInt;

	m_pDoc->Set_SelectedTerrains(SelList, bChanged);
}

void CDlgAnalyseur::OnLbnSelchangeAnListTerrains()
{
	CWaitCursor Cw;
	// Calculate the Universe
	SetSelectedUniverses(false);
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
	SetTitre();
}

void CDlgAnalyseur::OnTableauTransposer()
{
	// Wait
	CWaitCursor Cw;

	// Transposition Lignes <> Colonnes
	m_pDoc->TransposeRowCol();

	// Attention si on est en mode Moyenne Ligne ou Moyenne Colonne
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
	{
		// On transpose en mode moyenne Colonne
		m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;
	}
	else if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
	{
		// On transpose en mode moyenne Ligne
		m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;
	}

	// On remet les paramètres de calcul en forçant le recalcul sur lignes et colonnes (car depuis moyenne calcul change, vecteur quanti uniquement en ligne)
	m_pDoc->SetCalculation();
	m_pDoc->Add_Calculation(CALC_ROWS);
	m_pDoc->Add_Calculation(CALC_COLS);
	m_pDoc->Add_Calculation(CALC_UNIVERSE);

	// Idem pour les palettes ligne, colonne
	m_pDlgTableau->Transpose(CPaletteMode::Line,CPaletteMode::Column);

	m_pCrossTable->CalculateItems();

	UpdateSurlignage();
	SetTitre();
}

void CDlgAnalyseur::UpdateListCibles()
{
	m_List_Cibles.ResetContent();

	bool bEnsemblefound = false;
	JList<const CItemConstruit *> List;
	const JList<const CItemConstruit *> & ListCibles = m_pDoc->Get_ListCibles();
	for(ListCibles.MoveFirst(); ListCibles.IsValid(); ListCibles.MoveNext())
	{
		List.AddTail() = ListCibles.GetItem();
		if(ListCibles.GetItem() == m_pDoc->Get_ItemEnsemble())
			bEnsemblefound = true;
	}

	if(!bEnsemblefound)
	{
		List.AddHead() = m_pDoc->Get_ItemEnsemble();
		m_pDoc->Set_ListCibles(List);
	}

	// Update liste des cibles univers
	for(ListCibles.MoveFirst(); ListCibles.IsValid(); ListCibles.MoveNext())
	{
		const CItemConstruit * pItem = ListCibles.GetItem();

		// int Pos = m_List_Cibles.AddString(pItem->GetLabel().AsJCharPtr());
		int Pos = 0;

		// On affiche en italique si l'item est quantitatif  (pas actif pour le moment)
		if (ItemQuantitatif(pItem, m_pDoc->Get_TerrainTemoin()))
			// Pos =  m_List_Cibles.AddLine(pItem->GetLabel().AsJCharPtr(),m_TemplateQuanti);
			Pos =  m_List_Cibles.AddLine(pItem->GetLabel().AsJCharPtr(),m_TemplateStandard);
		else
			Pos =  m_List_Cibles.AddLine(pItem->GetLabel().AsJCharPtr(),m_TemplateStandard);

		m_List_Cibles.SetItemData(Pos,(DWORD_PTR)pItem);
		if(m_pDoc->Get_SelectedCible() == pItem)
			m_List_Cibles.SetCurSel(Pos);
	}

	if(m_List_Cibles.GetCurSel() == LB_ERR)
		m_List_Cibles.SetCurSel(0);

	OnLbnSelchangeAnListCibles();
}

void CDlgAnalyseur::OnLbnSelchangeAnListCibles()
{
	int Sel = m_List_Cibles.GetCurSel();
	if(Sel != LB_ERR)
	{
		m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
	}
	else m_pDoc->Set_SelectedCible(0);

	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); // A OPTIM 2.7 !!!!!!

	UpdateSurlignage();
	SetTitre();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage du titre global (Exclusif/Duplicant + Cibles/Univers + Paramètres de tri)
void CDlgAnalyseur::SetTitre()
{
	CString txt,txt2;

	SetResultControls();

	if(m_pDoc->IsTableStyleSet(TSTYLE_EXCLUSIVE))
	{
		txt.LoadString(IDS_AN_ITEMEXCLUSIVE);
		txt.AppendFormat(_T(" - "));
	}
	else if(m_pDoc->IsTableStyleSet(TSTYLE_DUPLIQUE))
	{
		txt.LoadString(IDS_AN_ITEMDUPLIQUANT);
		txt.AppendFormat(_T(" - "));
	}

	// Mais dans le titre
	const JMap<UINT, PTR_TERRAINDATA> & pTerrainData  = m_pDoc->Get_ListTerrainData(); 
	pTerrainData.MoveFirst();
	if(pTerrainData.IsValid())
	{
		m_Label_Terrains.SetText(pTerrainData .GetItem()->GetSourceLabel()); 
	}

	// La cible
	if(m_pDoc->Get_SelectedCible())
	{
		if (m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
			// Mode Cible
			txt.Append("Cible : ");
		else
		{
			// Mode Univers
			txt2.LoadString(IDS_AN_LABEL_UNIVERS);
			txt.Append(txt2);
			txt.Append(" : ");
		}	

		// Affichage de la cible
		txt.Append(m_pDoc->Get_SelectedCible()->GetLabel().AsJCharPtr()); 
	}

	// Les Segments
	const JList<UINT>	&	SegmentsId	= m_pDoc->Get_SegmentsId();

	// + nom segment (si +, seg.multiples)
	if(SegmentsId.GetCount()!=0)
	{
		if (SegmentsId.GetCount() > 1)
			// Ici indique plusieurs segments
			txt.Append(" - Multi Segments");
		else
		{
			// Source Table
			const JSRCTables & Sources = JSRCTables::GetInstance ();

			// 1 seul segment sélectionné
			SegmentsId.MoveFirst();

			if (SegmentsId.IsValid())
			{
				// Id Segment
				JUnt32 IdSeg = SegmentsId.GetItem();

				// Affiche ssi segment différent de ensemble
				if (IdSeg != 1)
				{
					const JSegment* Seg = Sources.m_TBLSegments.GetItem(SegmentsId.GetItem());
					txt += " - Segment : ";
					txt += Seg->m_Libelle.AsJCharPtr();
				}
			}
		}
	}

	// Affichage des infos tri si des paramètres de tri sont positionnés
	if (m_pDoc->Get_TypeTri() != m_pDoc->TYPE_BASE)
	{
		// Ajoute paramétrages de tri en cours
		txt.Append(" - Tri "); 
			
		// Récup type de tri (croisant, décroissant, origine)
		txt.Append(m_pDoc->Get_LibTypeTri().MakeLower()); 
		txt.Append(" ");

		// Récup mode de tri (sur toutes les lignes ou par bloc de critères)
		txt.Append(m_pDoc->Get_LibModeTri().MakeLower()); 
		txt.Append(" sur < ");

		// Récup sur quel type de résultat (grandeur, H%, V%, Global...)
		CString LibResult;
		LibResult.Append(m_pDoc->Get_LibResultTri());

		// Global ???
		RESULTSTYLE ResStyle = m_pDoc->Get_ResultTri();

		if (m_pDoc->Get_ResultTri() & RSTYLE_GBL_STRUCTURE)
		{
			LibResult.LoadString(IDS_AN_ABR_GLOBAL);
		}
		else if (m_pDoc->Get_ResultTri() & RSTYLE_GBL_CORRELATION)
		{
			// Mode calcul global standard
			LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
		}
		else if(m_pDoc->Get_ResultTri() & RSTYLE_GBL_PROXIMITY)
		{
			LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
		}
		else if(m_pDoc->Get_ResultTri() & RSTYLE_GBL_ZSCORE)
		{
			LibResult.LoadString(IDS_AN_ABR_ZSCORE);
		}

		txt.Append(LibResult); 
		txt.Append(", ");

		// Récup sur quelle grandeur le tri est esffectué (effectif, nb cas etc...)
		txt.Append(m_pDoc->Get_LibGrandeurTri().MakeLower());
		txt.Append(" >");
	}

	// Ajoute l'information concernant le mode moyenne si valeur moyenne sélectionnée
	if (m_pDoc->GrandeurMoyenneActive())
	{
		if(m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
		{
			txt.Append(" (Mode Moyenne Lignes : ");
		}
		else if(m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
		{
			txt.Append(" (Mode Moyenne Colonnes : ");
		}
		else
		{
			// En mode tableau on ajoute le libellé de la variable moyennable
			txt.Append(" (Mode Moyenne Tableau : ");

			/* Voir si on ajoute le nom de la variable moyennable, car déjà sélectionnée dans la liste)
			JInt32 InxSel = m_List_VarMoy.GetCurSel();
			CString StrTxt = "";
			if (InxSel >= 0)
			{
				m_List_VarMoy.GetText(InxSel, StrTxt);
				txt.Append(" / ");
				txt.Append(StrTxt);
			}
			txt.Append(")");
			*/
		}

		// Ajoute information concernant le mode de calcul (avec ou sans les 0)
		CString StrTxt = "";
		if (m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_INCLUS)
			StrTxt = "0 inclus";
		else
			StrTxt = "0 exclus";

		txt.Append(StrTxt);
		txt.Append(")");
	}

	// Affiche texte de l'entête
	m_Grid_Title.SetText(txt);

	if(m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
	{
		txt.LoadString(IDS_AN_LABEL_CIBLES);
		txt2.LoadString(IDS_AN_MENU_CHOISIR_CIBLES);
	}
	else
	{
		txt.LoadString(IDS_AN_LABEL_UNIVERS);
		txt2.LoadString(IDS_AN_MENU_CHOISIR_UNIVERS);
	}

	m_Label_Cibles.SetText(txt); 

	CMenu * pMenu = GetMenu();
	if(pMenu)
	{
		pMenu = pMenu->GetSubMenu(1);
		if(pMenu)
			pMenu->ModifyMenu(IDM_AN_SOURCE_CHOISIR_UNIVERS,MF_BYCOMMAND|MF_STRING,IDM_AN_SOURCE_CHOISIR_UNIVERS,txt2);
//		DrawMenuBar();
	}
	txt.LoadString(IDS_AN_APPNAME);
	txt2.LoadString(IDS_AN_NONAME); 
	if(m_pDoc->GetTitle().IsEmpty())
		txt.AppendFormat(" - %s",txt2);
	else
		txt.AppendFormat(" - %s",m_pDoc->GetTitle());
	SetWindowText(txt);

	m_Grid_Title.Invalidate();
	m_Grid_Title.RedrawWindow();

	// Invalidate(TRUE);
	// UpdateWindow();
}

void CDlgAnalyseur::OnDefinirStructureTableau()
{
	CDlgParams Dlg(this);
	Dlg.SetParams(m_pDoc);
	if(Dlg.DoModal()==IDOK)
	{
		CWaitCursor Cw;
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();
		SetTitre();
	}
}

/* Commande passée dans le Menu (on garde pour le moment si retour en arrière)
void CDlgAnalyseur::OnBnClickedAnButBase100()
{
	if(m_pDoc->Get_ClickMode() != CMODE_BASE100)
	{
		m_pDoc->Set_ClickMode(CMODE_BASE100);

		// Mode base 100 actif
        m_But_Base100   .SetCheck(1);
		m_ModeBase100Actif = TRUE;
		
		m_But_Surlignage.SetCheck(0);
	}
	else
	{
		m_pDoc->Set_ClickMode(CMODE_TRI);

		// Mode base 100 inactif
        m_But_Base100   .SetCheck(0);
		m_ModeBase100Actif = FALSE;

		m_But_Surlignage.SetCheck(0);
	}
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage fenêtre de paramétrage pour le surlignage direct sur le tableau croisé
void CDlgAnalyseur::OnBnClickedAnButParamSurlignage()
{
	// Instance fenetre parametres surlignage
	CDlgParamsSurlignage DlgParamsSurlign(this);

	// Récup liste des grandeurs et résultats dispos
	JMap  <JUnt32, CString>			MapGrandeurs;		// les grandeurs sélectionnées dans analyse de l'offre
	JMap  <JUnt32, CString>			MapResultats;		// les résultats cellules valides
	MapGrandeurs.Reset();
	MapResultats.Reset();

	// Liste des grandeurs disponibles
	for (JInt32 InxLst = 0; InxLst < m_List_Grandeurs.GetCount(); InxLst++)
	{
		// Libellé grandeur
		CString Txt;

		// Info texte grandeur
		m_List_Grandeurs.GetText(InxLst, Txt); 

		// Info ident grandeur
		JInt32 ItemData = m_List_Grandeurs.GetItemData(InxLst);

		// Ajoute au map des grandeurs dispos pour le tri (avec itemdata coeff 10000 sinon ordre différent)
		MapGrandeurs.Add(ItemData + (InxLst * 10000)) = Txt; 
	}

	// Ajoute type résultat Grandeur si actif
	MapResultats.Add(RSTYLE_RESULT) = "Grandeur" ;

	// Ajoute type résultat Horizontal si actif
	MapResultats.Add(RSTYLE_HORIZONTAL) = "H%";

	// Ajoute type résultat Vertical si actif
	MapResultats.Add(RSTYLE_VERTICAL) = "V%";
	
	// Ajoute type résultat Global si actif
	CString LibResult = "";

	// Mode calcul global standard
	LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
	MapResultats.Add(RSTYLE_GBL_CORRELATION) = LibResult;

	LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
	MapResultats.Add(RSTYLE_GBL_PROXIMITY) = LibResult;
	
	LibResult.LoadString(IDS_AN_ABR_ZSCORE);
	MapResultats.Add(RSTYLE_GBL_ZSCORE) = LibResult;

	LibResult.LoadString(IDS_AN_ABR_GLOBAL);
	MapResultats.Add(RSTYLE_GLOBAL) = LibResult;

	// Récup les paramètres par défaut
	m_pDoc->SetParamDefSurlignage(&m_Default_OpeSurlignage);

	// Envoi le document courant à la fenêtre
	DlgParamsSurlign.SetParams(m_pDoc, &MapGrandeurs, &MapResultats);

	// Affichage de la fenêtre
	if(DlgParamsSurlign.DoModal() == IDOK)
	{
		/*
		if (DlgParamsSurlign.GetRazSurlign() == true)
		{
			// Init map des cellules à surligner
			m_pDoc->m_MapCellTotSurlign.Reset();

			// Init liste des opérations surlignage 
			m_pDoc->m_Lst_Ope_Surlignages.Reset(); 

			// Mise à jour Map des cellules à surligner
			UpdateSurlignage();

		}
		else
		*/
		
		// Récupère la dernière opération surlignage à effectuer
		const COpeSurlignage OpeSurlignage = DlgParamsSurlign.GetParam();

		// Vire l'anciene opération surlignage
		// A REVOIR ???????????????????
		for (m_pDoc->m_Lst_Ope_Surlignages.MoveFirst(); m_pDoc->m_Lst_Ope_Surlignages.IsValid(); m_pDoc->m_Lst_Ope_Surlignages.MoveNext())
		{
			const COpeSurlignage & OpeSurlignCur = m_pDoc->m_Lst_Ope_Surlignages.GetItem();
			if (OpeSurlignCur.Param_Grandeur	== OpeSurlignage.Param_Grandeur &&
				OpeSurlignCur.Param_Result		== OpeSurlignage.Param_Result &&
				OpeSurlignCur.TypeParam			== OpeSurlignage.TypeParam &&
				OpeSurlignCur.Valeur1			== OpeSurlignage.Valeur1 &&
				OpeSurlignCur.Valeur2			== OpeSurlignage.Valeur2) 
			{
				m_pDoc->m_Lst_Ope_Surlignages.Remove();
				break;
			}
		}

		// On l'ajoute à la liste des opérations surlignage
		m_pDoc->m_Lst_Ope_Surlignages.AddTail() = OpeSurlignage; 

		// Mise à jour Map des cellules à surligner
		UpdateSurlignage();
	}

	/*
	// Pas de surlignage supplémntaite à effection (Cancel)
	else
	{
		// Init liste des opérations surlignage
		m_pDoc->m_Lst_Ope_Surlignages.Reset();

		// Mise à jour Map des cellules à surligner
		UpdateSurlignage();
	}
	*/

	// Dévalide le btn surlignage
	m_But_Surlignage.SetCheck(0);
}	

////////////////////////////////////////////////////////////////////////////////////////////
// Retour en arrière surlignage
void  CDlgAnalyseur::OnBnClickedAnButPrecSurlign()
{
	// Si au moins 1 opération surlignage
	if (m_pDoc->m_Lst_Ope_Surlignages.GetCount() > 0)
	{
		// On vire la dernière
		m_pDoc->m_Lst_Ope_Surlignages.MoveLast(); 
		m_pDoc->m_Lst_Ope_Surlignages.Remove(-1);

		// Puis on réaffiche
		UpdateSurlignage();

		if (m_pDoc->m_Lst_Ope_Surlignages.GetCount() == 0)
		{
			m_CrossTable.UpdateAllRowsCols();
			m_CrossTable.Redraw();

			m_CrossTable.LockUpdate(false);
			m_CrossTable.Invalidate(); 
		}
		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour du surlignage via les opérations de surlignage
void CDlgAnalyseur::UpdateSurlignage()
{
	// Puis on réaffiche
	m_pDoc->UpdateSurlignage(m_pCrossTable, &m_CrossTable);

	// Update couleur bouton surlignage en cours
	UpdateColorBtnPrecSurlign();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update couleur bouton surlignage précédent
void CDlgAnalyseur::UpdateColorBtnPrecSurlign()
{
	// Mise à jour couleur du bouton
	if (m_pDoc->m_Lst_Ope_Surlignages.GetCount()== 0)
	{
		// Coloration bouton retour arrière surlignage
		m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
		m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
		m_Btn_PrecSurlign.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	}
	else
	{
		// Récup couleur surlignage
		m_pDoc->m_Lst_Ope_Surlignages.MoveLast(); 
		const COpeSurlignage & OpeSurlignage = m_pDoc->m_Lst_Ope_Surlignages.GetItem();	

		// Coloration du bouton
		m_Btn_PrecSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		OpeSurlignage.Param_Color,CATPColors::COLORANALYSEUR, 0);
		m_Btn_PrecSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		OpeSurlignage.Param_Color,CATPColors::COLORANALYSEUR);
		m_Btn_PrecSurlign.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	OpeSurlignage.Param_Color,CATPColors::COLORANALYSEUR);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Annulation de toutes les commandes surlignage
void CDlgAnalyseur::OnBnClickedAnButRazSurlign()
{
	// Reset de la liste des opératuers surlignage
	m_pDoc->m_Lst_Ope_Surlignages.Reset();

	// Puis on réaffiche
	UpdateSurlignage();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Ranking ou ordonnancement (linéarisation) de la grille tableau croisé
void CDlgAnalyseur::OnBnClickedAnButOrdonner()
{
	// Retaillage fenetre surlignage si elle existe
	RetailleFenOrdonnancement();
	
	if(m_pDoc->Get_ClickMode() != CMODE_ORDONNER)
	{
		m_pDoc->Set_ClickMode(CMODE_ORDONNER);
		m_ModeBase100Actif = FALSE;
		m_But_Ordonner.SetCheck(1);
	}
	else
	{
		m_pDoc->Set_ClickMode(CMODE_TRI);
		m_ModeBase100Actif = FALSE;
		m_But_Ordonner.SetCheck(0);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Retaillage de la fenêtre ordonnancement ou ranking
void CDlgAnalyseur::RetailleFenOrdonnancement()
{
	if (m_DlgSurlignage)
	{
		// Dimension objet grille / tableur
		CRect RectGrille;
		m_CrossTable.GetWindowRect(&RectGrille); 

		// Repositionne la fenêtre surlignage en bas de la fenêtre analyseur
		CRect Rect1,Rect2;
		this->GetWindowRect(&Rect1);
		m_DlgSurlignage.GetWindowRect(&Rect2);
		Rect2.right  = RectGrille.right;
		Rect2.left   = Rect1.left + 5;  
		Rect2.top    = Rect1.bottom - Rect2.Height() - 5; 
		Rect2.bottom = Rect1.bottom - 5; 
		m_DlgSurlignage.MoveWindow(Rect2,TRUE);
	}			
}

void CDlgAnalyseur::OnDestroy()
{
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
 
	AfxGetApp()->WriteProfileString("Options\\Analyseur", "Rect", strText);
    AfxGetApp()->WriteProfileInt("Options\\Analyseur", "Max", bMaximized);

	SaveProfile();

	m_DlgSurlignage.SetButtons(0, 0);

	CDialog::OnDestroy();
}

void CDlgAnalyseur::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\Analyseur", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left	=	atoi((const char*) strText);
            rect.top	=	atoi((const char*) strText + 5);
            rect.right	=	atoi((const char*) strText + 10);
            rect.bottom	=	atoi((const char*) strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\Analyseur", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\Analyseur", "Max", 0);   
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

void CDlgAnalyseur::OnInterpreterUniversCommeCible()
{
	UINT ResultStyle = m_pDoc->Get_ResultStyle();

	if(ResultStyle & RSTYLE_CIBLES)
		ResultStyle&= ~RSTYLE_CIBLES;
	else
		ResultStyle |= RSTYLE_CIBLES;

	m_pDoc->Set_ResultStyle(ResultStyle);

	SetupGrandeurs();
	SetGrandeurs();

	// Si le bouton Global Sélectionner, il faut le déselectionner
	if(ResultStyle & RSTYLE_CIBLES)
	{
		m_But_Global.SetCheck(0);
		m_But_Global2.SetCheck(0);
		m_But_Global3.SetCheck(0);
		OnBnClickedAnButGlobal();
	}

	CWaitCursor Cw;
	if(m_pCrossTable)
	{	
		m_pCrossTable->CalculateItems();
		UpdateSurlignage();
	}

	SetTitre();
}

void CDlgAnalyseur::OnUpdateInterpreterUniversCommeCible(CCmdUI *pCmdUI)
{
	if(m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CDlgAnalyseur::OnFileNew()
{
	// ATTENTION : cette fonction est la copie conforme de la fonction CDlgProfiling::OnFileNew
	//             il faut reporter les modifications (en attendant mieux)
	if(m_pDoc->IsDirty())
	{
		CString txt;
		txt.LoadString(IDS_AN_SAVETABLEAU);
		int Result = MessageBox(txt,0,MB_YESNOCANCEL | MB_ICONQUESTION);
		if(Result == IDYES)
			OnFileSave();
		else if(Result == IDCANCEL)
			return;
	}

	// Init document, avec mise à jour des composants mode moyenne
	m_pDoc->Reset(); 
	UpdateElmtVarMoy();

	m_pCrossTable->SetupGrid(&m_CrossTable, m_pDoc, m_ModeBase100Actif, &m_But_Ordonner);

	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
	SetTitre();

	// Init liste des elmts moyennables
	m_List_VarMoy.ResetContent();

	// Init sélection dans pupitre
	m_pDlgTableau->InitSelGeneral(); 

	// Puis on va sélectionner les nouveaux éléments dans pupitre
	m_But_Vectorielle.EnableWindow(TRUE); 

    // OnBnClickedButVectorielle();
	GoPupitre();

}

void CDlgAnalyseur::OnFileOpen()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		TRUE,
		_T(".anl"),
		NULL,
		OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Fichiers Analyseur (*.anl)|*.anl|Tous les fichiers (*.*)|*.*||"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	OpenFile(dlgFile.GetPathName( ));

}

void CDlgAnalyseur::OpenFile(CString path)
{
	CStdioFile textFile;

	if (!textFile.Open(path, CFile::typeBinary | CFile::modeRead))
	{
		CString txt;
		txt.Format("Fichier %s invalide!", path);
		MessageBox(txt);
		return;
	}

	CAnalyseurDoc *pDoc = new CAnalyseurDoc(m_ProgCtrl, *this, m_DlgSurlignage);

	// Chargement du document Tris Croisées 
	// + (si version > 6) à la suite chargement construction pupitre associé
	if(!pDoc->LoadDoc(textFile))
	{
		CString txt;
		txt.Format("Erreur pendant le chargement du fichier %s", path);
		MessageBox(txt);
		return;
	}

	// Fermeture du fichier
	// textFile.Close();

	if(m_pDoc)
		delete m_pDoc;

	m_pDoc = pDoc;

	// Init sélection dans pupitre
	m_pDlgTableau->InitSelGeneral(); 

	// TEST RECUP PALETTES LIGNE + COLONNE + PAGE
	m_But_Vectorielle.EnableWindow(TRUE); 
	const int NoVersion = pDoc->GetNoVersion();
	if (pDoc->GetNoVersion() > 6)
	{
		m_pDlgTableau->LoadPalettes(textFile);
	}

	// Fermeture du fichier
	textFile.Close();
	
	// Init liste des variables moyennables
	m_List_VarMoy.ResetContent();

	// Si mode tableau, on update la liste des variables moyennables avec celle sauvegardée
	LIST_ITEM_PUPITRE & LstVarMoys = m_pDoc->GetAllVariablesMoyennables();

	if (LstVarMoys.GetCount() > 0)
	{
		// Mise à jour de la liste des variables
		for (LstVarMoys.MoveFirst(); LstVarMoys.IsValid(); LstVarMoys.MoveNext())
		{
			// Récup Item Wrapper
			CItemWrapper * pItemWrapper = LstVarMoys.GetItem();

			// Label de l'item
			CString LblItem = pItemWrapper->GetLabel();

			// Vire le séparateur Label <> Header
			LblItem.Replace("@", "-");

			// Ajoute à la liste
			// JInt32 InxLst = m_List_VarMoy.AddString(LblItem);
			// m_List_VarMoy.SetItemData(InxLst, (LPARAM)pItemWrapper);
			JInt32 InxLst = m_List_VarMoy.AddLine(LblItem, m_TemplateQuanti);
			m_List_VarMoy.SetItemData(InxLst, (LPARAM)pItemWrapper);

			// Récup index liste elmt sélectionné
			if (pItemWrapper == m_pDoc->m_pItemWrapperMoy)
			{
				m_List_VarMoy.SetCurSel(InxLst);
				m_List_VarMoy.RedrawWindow();
			}
		}
	}

	// Update validité du mode moyenne tableau
	UpdateElmtVarMoy();

	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)
	{
		// Redessine la liste des variables moyennables
		m_List_VarMoy.RedrawWindow();
	}

	// Mise en place élmt du Grid suivi du Calcul
	MiseEnPlaceGridTC();	

	// Affichage du titre avec sélection tris
	SetTitre();

}

// Mise en place élmt du Grid suivi du Calcul
void CDlgAnalyseur::MiseEnPlaceGridTC(JBool NewCalcul)
{
	// On bloque pour le moment le rafraichissement du tableur
	m_CrossTable.LockUpdate(true);	

	m_pCrossTable->SetupGrid(&m_CrossTable, m_pDoc, m_ModeBase100Actif, &m_But_Ordonner);
		
	// Initialisation des grandeurs à afficher
	SetupGrandeurs();

	// Les grandeurs utilisées
	SetGrandeurs();

	// Update elmt tris
	SetupSelTri();

	// Init paramètres d'affichage
	m_pDoc->Set_ClickMode(CMODE_TRI);

	m_But_Ordonner.SetCheck(0);

	// Par défaut le mode exclusif et mode duplicant n'est pas actif
	m_ModeExclusif		=	FALSE;
	m_ModeDuplicant		=	FALSE;	

	// Chargement des terrains
	LoadTerrains(m_pDoc->Get_SourceId(),m_pDoc->Get_TerrainId(),m_pDoc->Get_SegmentsId(),m_pDoc->Get_TypePoidsId());
	UpdateListCibles();

	// Véerification globale des items

	/*
	// OPTIM 2.7 A VOIR
	// OPTIM 2.7 attention normalement déjà fait dans UpdateListCibles
	CheckItems();    
	
	// Calcul
	CWaitCursor Cw;

	m_pCrossTable->CalculateItems();   // ??? bizarre si on le vire ça marche quand même
	UpdateSurlignage();
	// FIN OPTIM 2.7 attention normalement déjà fait dans UpdateListCibles
	*/

	// Mise en place titre
	SetTitre();

	// On bloque pour le moment le rafraichissement du tableur
	m_CrossTable.LockUpdate(false);	
}


void CDlgAnalyseur::OnFileSave()
{
	if(m_pDoc && m_pDoc->IsDirty())
	{
		CStdioFile textFile;

		// pop-up file-open dlg to ask for location
		CFileDialog dlgFile(
			FALSE,
			_T(".anl"),
			NULL,
			OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("Fichiers Analyseur (*.anl)|*.anl|Tous les fichiers (*.*)|*.*||"),this);

		// Récupération chemin sauvegarde fichier client tris croisés
		if(m_pDoc->GetFilePath().IsEmpty())
		{
			if (dlgFile.DoModal( ) == IDCANCEL)
				return ;

			if (!textFile.Open(dlgFile.GetPathName( ),
				CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
			{
				CString txt;
				txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
				MessageBox(txt);
				return ;
			}
		}
		else
		{
			if (!textFile.Open(m_pDoc->GetFilePath(),
				CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
			{
				CString txt;
				txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
				MessageBox(txt);
				return ;
			}
		}
		if(m_pDoc->SaveDoc(textFile))
		{
			// Sauvegarde du dossier Tris Croisés
			textFile.Flush();

			// A FAIRE - Ajout de la partie Pupitre Palettes Ligne/Colonne/Page 
			// A partir de la version 7 du Doc
			// Test sauvegarde construction des palettes (lignes, colonnes, pages)
			// TEST SAUVEGARDE PALETTES LIGNE + COLONNE + PAGE
			if (m_pDlgTableau)
			{
				// Ajoute infos palettes pupitre en fin de fichier
				m_pDlgTableau->SavePalettes(textFile);
			}

			// Fermeture du fichier
			textFile.Close();

		}
		else
		{
			textFile.Flush();
			textFile.Close();
			DeleteFile(textFile.GetFilePath());  
		}
	}
	SetTitre();
}

void CDlgAnalyseur::OnFileSaveAs()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".anl"),
		NULL,
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichiers Analyseur (*.anl)|*.anl|Tous les fichiers (*.*)|*.*||"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return ;

	CStdioFile textFile;

	if (!textFile.Open(dlgFile.GetPathName( ),
		CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
	{
		CString txt;
		txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
		MessageBox(txt);
		return ;
	}
	if(m_pDoc->SaveDoc(textFile))
	{
		textFile.Flush();
		textFile.Close();
	}
	else
	{
		textFile.Flush();
		textFile.Close();
		DeleteFile(textFile.GetFilePath());  
	}
	SetTitre();
}

void CDlgAnalyseur::OnFileClose()
{
	if(m_pDoc->IsDirty())
	{
		CString txt;
		txt.LoadString(IDS_AN_SAVETABLEAU);
		int Result = MessageBox(txt,0,MB_YESNOCANCEL | MB_ICONQUESTION);
		if(Result == IDYES)
			OnFileSave();
		else if(Result == IDCANCEL)
			return;
	}

	// Init document, avec mise à jour des composants mode moyenne
	m_pDoc->Reset(); 
	UpdateElmtVarMoy();

	// Init sélection dans pupitre
	m_pDlgTableau->InitSelGeneral(); 

	// Puis on va sélectionner les nouveaux éléments dans pupitre
	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
	SetTitre();
}

void CDlgAnalyseur::OnTableauQuitter()
{
	if(m_pDoc->IsDirty())
	{
		CString txt;
		txt.LoadString(IDS_AN_SAVETABLEAU);
		switch(MessageBox(txt,0,MB_YESNOCANCEL | MB_ICONQUESTION))
		{
			case IDYES:	OnFileSave();
			case IDNO:	break;

			case IDCANCEL:
			default: return;
		}
	}
	OnClose();
	ShowWindow(SW_HIDE); 
}

void CDlgAnalyseur::OnTableauAppliquerCanevas()
{
	CFileDialog dlgFile(
		TRUE,
		_T(".anp"),
		NULL,
		OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Fichiers Canevas (*.anp)|*.anp|Tous les fichiers (*.*)|*.*||"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return ;

	CStdioFile textFile;

	if (!textFile.Open(dlgFile.GetPathName( ), CFile::typeBinary | CFile::modeRead))
	{
		CString txt;
		txt.Format("Fichier %s invalide!", dlgFile.GetPathName());
		MessageBox(txt);
		return;
	}

	// Resd the version number
	UINT Data = 0;
	textFile.Read(&Data,sizeof(Data));
	if(Data == 1)
	{
		// Read the result style
		textFile.Read(&Data,sizeof(Data));
		m_pDoc->Set_ResultStyle(Data);
		// Read the table style
		textFile.Read(&Data,sizeof(Data));
		m_pDoc->Set_TableStyle(Data);
	}
	textFile.Close();

	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
	SetTitre();
}

void CDlgAnalyseur::OnTableauEnregistrerCanevas()
{
	CFileDialog dlgFile(
		FALSE,
		_T(".anp"),
		NULL,
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichiers Canevas (*.anp)|*.anp|Tous les fichiers (*.*)|*.*||"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return ;

	CStdioFile textFile;

	if (!textFile.Open(dlgFile.GetPathName( ),
		CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
	{
		CString txt;
		txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
		MessageBox(txt);
		return ;
	}

	// Write the version number
	UINT Data = 1;
	textFile.Write(&Data,sizeof(Data));
	// Write the result style
	Data = m_pDoc->Get_ResultStyle();
	textFile.Write(&Data,sizeof(Data));
	// Write the table style
	Data = m_pDoc->Get_TableStyle();
	textFile.Write(&Data,sizeof(Data));
	textFile.Flush();
	textFile.Close();
}


void CDlgAnalyseur::LoadProfile()
{
	CString					Str;
	JUnt32					SourceId = 1;
	JList<UINT>				TerrainId;
	JUnt32					TypePoidsId = 1;
	JList<UINT>				SegmentsId;
	int x=0;

	SourceId =		AfxGetApp()->GetProfileInt("Options\\Analyseur", "SourceId",0);

	int Count = AfxGetApp()->GetProfileInt("Options\\Analyseur", "NbTerrainId",0);
	TerrainId.Reset();
	for(x=0; x < Count; ++x)
	{
		Str.Format(_T("TerrainId%d"),x);
	    TerrainId.AddTail() = AfxGetApp()->GetProfileInt("Options\\Analyseur", Str,0);
	}

	Count =AfxGetApp()->GetProfileInt("Options\\Analyseur","NbSegmentsId",0);
	SegmentsId.Reset();
	for(x=0; x < Count; ++x)
	{
		Str.Format(_T("SegmentsId%d"),x);
	    SegmentsId.AddTail() = AfxGetApp()->GetProfileInt("Options\\Analyseur", Str,0);
	}

	// Récupération du type de poids
	TypePoidsId	=	AfxGetApp()->GetProfileInt("Options\\Analyseur","TypePoidsId",0);
	
	if(SourceId != 0 && TerrainId.GetCount() != 0 && SegmentsId.GetCount() != 0 && TypePoidsId != 0)
		LoadTerrains(SourceId,TerrainId,SegmentsId,TypePoidsId);
	else
		LoadTerrains(m_pDoc->Get_SourceId(),m_pDoc->Get_TerrainId(),m_pDoc->Get_SegmentsId(),m_pDoc->Get_TypePoidsId());

	SetSelectedUniverses(true);

	
}

void CDlgAnalyseur::SaveProfile()
{
	CString					Str;
	JUnt32					SourceId	= m_pDoc->Get_SourceId();
	const JList<UINT>	&	TerrainId	= m_pDoc->Get_TerrainId();
	JUnt32					TypePoidsId	= m_pDoc->Get_TypePoidsId();
	const JList<UINT>	&	SegmentsId	= m_pDoc->Get_SegmentsId();

    AfxGetApp()->WriteProfileInt("Options\\Analyseur", "SourceId",SourceId);

	AfxGetApp()->WriteProfileInt("Options\\Analyseur", "NbTerrainId",TerrainId.GetCount());
	int x=0;
	Str = "";
	for(TerrainId.MoveFirst() ; TerrainId.IsValid(); TerrainId.MoveNext(), ++x)
	{
		Str.Format(_T("TerrainId%d"),x);
	    AfxGetApp()->WriteProfileInt("Options\\Analyseur", Str,TerrainId.GetItem());
	}

    AfxGetApp()->WriteProfileInt("Options\\Analyseur", "NbSegmentsId",SegmentsId.GetCount());
	x=0;
	Str = "";
	for(SegmentsId.MoveFirst() ; SegmentsId.IsValid(); SegmentsId.MoveNext(), ++x)
	{
		Str.Format(_T("SegmentsId%d"),x);
	    AfxGetApp()->WriteProfileInt("Options\\Analyseur", Str,SegmentsId.GetItem());
	}

	// Sauvegarde du type de poids
    AfxGetApp()->WriteProfileInt("Options\\Analyseur", "TypePoidsId",TypePoidsId);

}

void CDlgAnalyseur::OnBnClickedAnButGrandeur()
{
	JBool ChkBtnGrandeur = false;

	// Recup position ascenseur horizontal et vertical
	ROWCOL LeftCol = m_CrossTable.GetLeftCol();
	ROWCOL TopRow = m_CrossTable.GetTopRow();

	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if(m_But_Grandeur.GetCheck() != 0) 		ChkBtnGrandeur = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if(m_But_Grandeur2.GetCheck() != 0)		ChkBtnGrandeur = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if(m_But_Grandeur3.GetCheck() != 0)		ChkBtnGrandeur = true;
	}

	/*
	if(m_But_Grandeur.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_RESULT);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_RESULT);
	*/
	if(ChkBtnGrandeur)
		m_pDoc->Add_ResultStyle(RSTYLE_RESULT);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_RESULT);

	// Remet en place les sélecteurs pour le tri
	SetupSelTri();

	SetTitre();
	m_pCrossTable->CalculateItems(false); 

	// Remet Row & Col Scroll Initiale
	GoToInitialPosCroll(TopRow, LeftCol, RSTYLE_RESULT, ChkBtnGrandeur);
	m_CrossTable.SetLeftCol(LeftCol);
	m_CrossTable.SetTopRow(TopRow);

	// Redessine après recalage ligne colonne via posotion scrolls
	m_pCrossTable->RedrawGrid();

	UpdateSurlignage();

	// Redessine la boite config
	m_Box_Config.Invalidate();
	m_Box_Config.RedrawWindow();
}

void CDlgAnalyseur::OnBnClickedAnButGrandeur2()
{
	OnBnClickedAnButGrandeur();
}

void CDlgAnalyseur::OnBnClickedAnButGrandeur3()
{
	OnBnClickedAnButGrandeur();
}


void CDlgAnalyseur::OnBnClickedAnButVert()
{
	JBool ChkBtnVert = false;

	// Recup position ascenseur horizontal et vertical
	ROWCOL LeftCol = m_CrossTable.GetLeftCol();
	ROWCOL TopRow = m_CrossTable.GetTopRow();

	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if(m_But_Vert.GetCheck() != 0) 		ChkBtnVert = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if(m_But_Vert2.GetCheck() != 0) 		ChkBtnVert = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if(m_But_Vert3.GetCheck() != 0) 		ChkBtnVert = true;
	}

	if(ChkBtnVert != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_VERT_STRUCTURE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_VERT_STRUCTURE);

	/*
	if(m_But_Vert.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_VERT_STRUCTURE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_VERT_STRUCTURE);
	*/

	// Remet en place les sélecteurs pour le tri
	SetupSelTri();

	SetTitre();
	m_pCrossTable->CalculateItems(false); 

	// Remet Row & Col Scroll Initiale
	GoToInitialPosCroll(TopRow, LeftCol, RSTYLE_VERT_STRUCTURE, ChkBtnVert);
	m_CrossTable.SetLeftCol(LeftCol);
	m_CrossTable.SetTopRow(TopRow);

	// Redessine après recalage ligne colonne via posotion scrolls
	m_pCrossTable->RedrawGrid();

	UpdateSurlignage();

	// Redessine la boite config
	m_Box_Config.Invalidate();
	m_Box_Config.RedrawWindow();
}

void CDlgAnalyseur::OnBnClickedAnButVert2()
{
	OnBnClickedAnButVert();
}

void CDlgAnalyseur::OnBnClickedAnButVert3()
{
	OnBnClickedAnButVert();
}

void CDlgAnalyseur::OnBnClickedAnButHoriz()
{
	ClickedAnButHoriz();
}

void CDlgAnalyseur::ClickedAnButHoriz()
{
	JBool ChkBtnHorz = false;
	
	// Recup position ascenseur horizontal et vertical
	ROWCOL LeftCol = m_CrossTable.GetLeftCol();
	ROWCOL TopRow = m_CrossTable.GetTopRow();

	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if(m_But_Horiz.GetCheck() != 0) ChkBtnHorz = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if(m_But_Horiz2.GetCheck() != 0) ChkBtnHorz  = true;
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if(m_But_Horiz3.GetCheck() != 0) ChkBtnHorz  = true;
	}

	if(ChkBtnHorz != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_HORZ_STRUCTURE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_HORZ_STRUCTURE);

	/*
	if(m_But_Horiz.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_HORZ_STRUCTURE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_HORZ_STRUCTURE);
	*/

	// Remet en place les sélecteurs pour le tri
	SetupSelTri();

	SetTitre();
	
	// A REMETTRE OPTIM 2.7
	m_CrossTable.LockUpdate(true);

	// Calcul
	m_pCrossTable->CalculateItems(false);

	UpdateSurlignage();

	// Remet Row & Col Scroll Initiale
	GoToInitialPosCroll(TopRow, LeftCol, RSTYLE_HORZ_STRUCTURE, ChkBtnHorz);
	m_CrossTable.SetLeftCol(LeftCol);
	m_CrossTable.SetTopRow(TopRow);

	// Redessine après recalage ligne colonne via posotion scrolls
	m_pCrossTable->RedrawGrid();

	// A REMETTRE OPTIM 2.7
	m_CrossTable.LockUpdate(false);

	
	// Redessine la boite config
	m_Box_Config.Invalidate();
	m_Box_Config.RedrawWindow();

}

void CDlgAnalyseur::OnBnClickedAnButHoriz2()
{
	ClickedAnButHoriz();
}


void CDlgAnalyseur::OnBnClickedAnButHoriz3()
{
	ClickedAnButHoriz();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Repositionnement des ascenseurs horizontals et verticals après changement dans liste des grandeurs
void CDlgAnalyseur::GoToInitialPosCroll3(ROWCOL & TopRow, ROWCOL & LeftCol, int ExNbGrandeurs, int NewNbGrandeurs)
{
	// Si nombre de grandeurs sélectionnées inchangées, rien à faire
	if (ExNbGrandeurs == NewNbGrandeurs || NewNbGrandeurs == 0) return;

	// Nombre de colonnes et rows à afficher par cellule, avant et apres la nouvelle sélection
	int NbColTotal1 = 0;
	int NbRowTotal1 = 0;

	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)
		{
			NbColTotal1++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbColTotal1++; 
		}

		// Nombre au départ de lignes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)
		{
			NbRowTotal1++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbRowTotal1++; 
		}
	}		
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		// Toujours 1 seule colonne
		NbColTotal1 = 1;

		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbRowTotal1++;
	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		// Toujours 1 seule ligne
		NbRowTotal1 = 1;

		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbColTotal1++; 
	}

	if((m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL))
	{
		int NbEnteteColFixe = 2;

		// Grandeurs en mode colonne
		// Si changement de nombre de colonnes par résultat
		if (ExNbGrandeurs != NewNbGrandeurs)
		{
			// Le nombre de colonnes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
			JUnt32 NbItemCols = m_pDoc->GetColItems().GetCount();

			if (ExNbGrandeurs > 0)
			{
				JUnt32 NbBlocs = (LeftCol - NbEnteteColFixe)/ (ExNbGrandeurs * NbColTotal1);

				if (NewNbGrandeurs > ExNbGrandeurs)
				{
					LeftCol += NbBlocs * (NewNbGrandeurs - ExNbGrandeurs) * NbColTotal1;
				}
				else
				{
					LeftCol -= NbBlocs * (ExNbGrandeurs - NewNbGrandeurs) * NbColTotal1;
				}
			}

			// Attention aussi à la place occupée par la ligne info grandeurs (1 ligne si + de 1 grandeur)
			if (ExNbGrandeurs == 1 && NewNbGrandeurs >= 2)
				TopRow += 1;
			else if (ExNbGrandeurs >= 2 && NewNbGrandeurs == 1)			
				TopRow -= 1;
		}
	}
	else
	{	
		int NbEnteteRowFixe = 2;

		// Grandeurs en mode ligne
		// Si changement de nombre de lignes par résultat
		if (ExNbGrandeurs != NewNbGrandeurs)
		{
			// Le nombre de lignes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
			JUnt32 NbItemRows = m_pDoc->GetRowItems().GetCount();

			if (ExNbGrandeurs > 0 && NbRowTotal1 > 0)
			{
				int Diff = (NewNbGrandeurs - ExNbGrandeurs);

				JUnt32 NbBlocs = (TopRow - NbEnteteRowFixe) / (ExNbGrandeurs * NbRowTotal1);

				if (NewNbGrandeurs > ExNbGrandeurs)
				{
					TopRow += NbBlocs * (NewNbGrandeurs - ExNbGrandeurs) * NbRowTotal1;
				}
				else
				{
					TopRow -= NbBlocs * (ExNbGrandeurs - NewNbGrandeurs) * NbRowTotal1;
				}
			}

			// Attention aussi à la place occupée par la colonne info grandeurs (1 colonne si + de 1 grandeur)
			if (ExNbGrandeurs == 1 && NewNbGrandeurs >= 2)
				LeftCol += 1;
			else if (ExNbGrandeurs >= 2 && NewNbGrandeurs == 1)			
				LeftCol -= 1;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Repositionnement des ascenseurs horizontals et verticals après changement mode resultat cellule
void CDlgAnalyseur::GoToInitialPosCroll2(ROWCOL & TopRow, ROWCOL & LeftCol, int ExModeAffich, int NewModeAffich)
{
	// Nombre de colonnes et rows à afficher par cellule, avant et apres la nouvelle sélection
	int NbColTotal1 = 0;
	int NbColTotal2 = 0;
	int NbRowTotal1 = 0;
	int NbRowTotal2 = 0;

	// Mode affichage 2 x 2
	if (ExModeAffich == m_pDoc->AFFRESULT_2X2)
	{
		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)
		{
			NbColTotal1++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbColTotal1++; 
		}

		// Nombre au départ de lignes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)
		{
			NbRowTotal1++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbRowTotal1++; 
		}
	}		
	else if (ExModeAffich == m_pDoc->AFFRESULT_LIGNE)
	{
		// Toujours 1 seule colonne
		NbColTotal1 = 1;

		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbRowTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbRowTotal1++;

	}
	else if (ExModeAffich == m_pDoc->AFFRESULT_COLONNE)
	{
		// Toujours 1 seule ligne
		NbRowTotal1 = 1;

		// Nombre au départ de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbColTotal1++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbColTotal1++; 
	}

	// Calcul nombre final lignes et colonnes avec nouveau mode affichage
	// Mode affichage 2 x 2
	if (NewModeAffich == m_pDoc->AFFRESULT_2X2)
	{
		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)
		{
			NbColTotal2++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbColTotal2++; 
		}

		// Nombre final de lignes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)
		{
			NbRowTotal2++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbRowTotal2++; 
		}
	}		
	else if (NewModeAffich == m_pDoc->AFFRESULT_LIGNE)
	{
		// Toujours 1 seule colonne
		NbColTotal2 = 1;

		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbRowTotal2++;

	}
	else if (NewModeAffich == m_pDoc->AFFRESULT_COLONNE)
	{
		// Toujours 1 seule ligne
		NbRowTotal2 = 1;

		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbColTotal2++; 
	}

	// Si changement de nombre de lignes par résultat
	if (NbRowTotal1 != NbRowTotal2)
	{
		// Le nombre de lignes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
		JUnt32 NbItemRows = m_pDoc->GetRowItems().GetCount();

		if (NbRowTotal1 > 0)
		{
			JUnt32 NbBlocs = (TopRow -2- NbRowTotal1) / NbRowTotal1 + 1;

			if (NbRowTotal2 > NbRowTotal1)
				TopRow += NbBlocs * (NbRowTotal2 - NbRowTotal1);
			else
				TopRow -= NbBlocs * (NbRowTotal1 - NbRowTotal2);
		}
	}

	// Si changement de nombre de colonnes par résultat
	if (NbColTotal1 != NbColTotal2)
	{
		// Le nombre de colonnes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
		JUnt32 NbItemCols = m_pDoc->GetColItems().GetCount();

		if (NbColTotal1 > 0)
		{
			JUnt32 NbBlocs = (LeftCol-2-NbColTotal1) / NbColTotal1 + 1;

			if (NbColTotal2 > NbColTotal1)
			{
				LeftCol += NbBlocs * (NbColTotal2 - NbColTotal1);
			}
			else
			{
				LeftCol -= NbBlocs * (NbColTotal1 - NbColTotal2);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Repositionnement des ascenseurs horizontals et verticals après ajout resultat
void CDlgAnalyseur::GoToInitialPosCroll(ROWCOL & TopRow, ROWCOL & LeftCol, UINT StyleStructure, JBool ChkBtn)
{
	// Nombte de colonnes et rows à afficher par cellule, avant et apres la nouvelle sélection
	int NbColTotal1 = 0;
	int NbColTotal2 = 0;
	int NbRowTotal1 = 0;
	int NbRowTotal2 = 0;

	// Mode affichage 2 x 2
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)
		{
			NbColTotal2++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbColTotal2++; 
		}

		// Nombre final de lignes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)
		{
			NbRowTotal2++; 
		}
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0 || (m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		{
			NbRowTotal2++; 
		}

		NbRowTotal1 = NbRowTotal2;
		NbColTotal1 = NbColTotal2;

		// Option RSTYLE_RESULT nouvellement Clické
		if (StyleStructure == RSTYLE_RESULT)
		{
			if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) == 0)
			{
				if (ChkBtn)
					NbRowTotal1--; 
				else
					NbRowTotal1++; 
			}

			if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) == 0)
			{
				if (ChkBtn)
					NbColTotal1--; 
				else
					NbColTotal1++; 
			}
		}
		
		// Option RSTYLE_VERT_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_VERT_STRUCTURE)
		{
			if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) == 0)
			{
				if (ChkBtn)
					NbRowTotal1--; 
				else
					NbRowTotal1++; 
			}

			if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) == 0)
			{
				if (ChkBtn)
					NbColTotal1--; 
				else
					NbColTotal1++; 
			}
		}

		// Option RSTYLE_HORZ_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_HORZ_STRUCTURE)
		{
			if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) == 0)
			{
				if (ChkBtn)
					NbRowTotal1--; 
				else
					NbRowTotal1++; 
			}

			if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) == 0)
			{
				if (ChkBtn)
					NbColTotal1--; 
				else
					NbColTotal1++; 
			}
		}

		// Option RSTYLE_GLOBAL nouvellement Clické
		if (StyleStructure == RSTYLE_GLOBAL)
		{
			if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) == 0)
			{
				if (ChkBtn)
					NbRowTotal1--; 
				else
					NbRowTotal1++; 
			}

			if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) == 0)
			{
				if (ChkBtn)
					NbColTotal1--; 
				else
					NbColTotal1++; 
			}
		}

	}
	// Mode affichage en colonne
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		// Toujours 1 ligne par bloc résultat
		NbRowTotal1 = 1;
		NbRowTotal2 = 1;

		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbColTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbColTotal2++; 

		NbColTotal1 = NbColTotal2;

		// Option RSTYLE_RESULT nouvellement Clické
		if (StyleStructure == RSTYLE_RESULT)
		{
			if (ChkBtn)
				NbColTotal1--; 
			else
				NbColTotal1++; 
		}

		// Option RSTYLE_VERT_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_VERT_STRUCTURE)
		{
			if (ChkBtn)
				NbColTotal1--; 
			else
				NbColTotal1++; 
		}

		// Option RSTYLE_HORZ_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_HORZ_STRUCTURE)
		{
			if (ChkBtn)
				NbColTotal1--; 
			else
				NbColTotal1++; 
		}

		// Option RSTYLE_GLOBAL nouvellement Clické
		if (StyleStructure == RSTYLE_GLOBAL)
		{
			if (ChkBtn)
				NbColTotal1--; 
			else
				NbColTotal1++; 
		}
	}

	// Mode affichage en ligne 
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		// Toujours 1 colonne
		NbColTotal1 = 1;
		NbColTotal2 = 1;

		// Nombre final de colonnes
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)			NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERT_STRUCTURE) > 0)	NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_STRUCTURE) > 0)	NbRowTotal2++; 
		if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)			NbRowTotal2++;

		NbRowTotal1 = NbRowTotal2;

		// Option RSTYLE_RESULT nouvellement Clické
		if (StyleStructure == RSTYLE_RESULT)
		{
			if (ChkBtn)
				NbRowTotal1--; 
			else
				NbRowTotal1++; 
		}

		// Option RSTYLE_VERT_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_VERT_STRUCTURE)
		{
			if (ChkBtn)
				NbRowTotal1--; 
			else
				NbRowTotal1++; 
		}

		// Option RSTYLE_HORZ_STRUCTURE nouvellement Clické
		if (StyleStructure == RSTYLE_HORZ_STRUCTURE)
		{
			if (ChkBtn)
				NbRowTotal1--; 
			else
				NbRowTotal1++; 
		}

		// Option RSTYLE_GLOBAL nouvellement Clické
		if (StyleStructure == RSTYLE_GLOBAL)
		{
			if (ChkBtn)
				NbRowTotal1--; 
			else
				NbRowTotal1++; 
		}
	}

	// Si changement de nombre de lignes par résultat
	if (NbRowTotal1 != NbRowTotal2)
	{
		// Le nombre de lignes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
		JUnt32 NbItemRows = m_pDoc->GetRowItems().GetCount();

		if (NbRowTotal1 > 0)
		{
			JUnt32 NbBlocs = (TopRow -2- NbRowTotal1) / NbRowTotal1 + 1;

			if (NbRowTotal2 > NbRowTotal1)
				TopRow += NbBlocs;
			else
				TopRow -= NbBlocs;
		}
	}

	// Si changement de nombre de colonnes par résultat
	if (NbColTotal1 != NbColTotal2)
	{
		// Le nombre de colonnes par résultat cellule a changé, on doit donc recalé le scroll horizontal en conséquence
		JUnt32 NbItemCols = m_pDoc->GetColItems().GetCount();

		if (NbColTotal1 > 0)
		{
			JUnt32 NbBlocs = (LeftCol-2-NbColTotal1) / NbColTotal1 + 1;

			if (NbColTotal2 > NbColTotal1)
			{
				LeftCol += NbBlocs;
			}
			else
			{
				LeftCol -= NbBlocs;
			}
		}
	}
}

// Gestion par défaut du bouton global mode 2 x 2
void CDlgAnalyseur::OnBnClickedAnButGlobal()
{
	ClickedAnButGlobal();
}

// Gestion par défaut du bouton global mode ligne
void CDlgAnalyseur::OnBnClickedAnButGlobal2()
{
	ClickedAnButGlobal();
}

// Gestion par défaut du bouton global mode colonne
void CDlgAnalyseur::OnBnClickedAnButGlobal3()
{
	ClickedAnButGlobal();
}

// Gestion Bouton Global
void CDlgAnalyseur::ClickedAnButGlobal()
{
	// Recup position ascenseur horizontal et vertical
	ROWCOL LeftCol = m_CrossTable.GetLeftCol();
	ROWCOL TopRow = m_CrossTable.GetTopRow();

	// Menu Popup 4éme indicateur (en bas à droite d'une cellule)
	switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
	{
		case RSTYLE_GBL_STRUCTURE :
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_STRUCTURE);
		break;
		case RSTYLE_GBL_CORRELATION:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_CORRELATION);
		break;
		case RSTYLE_GBL_PROXIMITY:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_PROXIMITY);
		break;
		case RSTYLE_GBL_ZSCORE:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);
		break;
	}

	// Teste les boutons globaux et récupère coordonnées bouton global
	JBool ChkBtnGlobal = false;
	CRect Rect;
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if(m_But_Global.GetCheck() != 0) 	ChkBtnGlobal = true;
		m_But_Global.GetWindowRect(&Rect);
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if(m_But_Global2.GetCheck() != 0) 	ChkBtnGlobal  = true;
		m_But_Global2.GetWindowRect(&Rect);
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if(m_But_Global3.GetCheck() != 0) 	ChkBtnGlobal  = true;
		m_But_Global3.GetWindowRect(&Rect);
	}

	if (ChkBtnGlobal)
	{
		// Récupére l'indice courant sélectionné dans menu paramètre
		// m_pDoc->Add_ResultStyle(RSTYLE_GBL_CORRELATION);
		if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_CORRELATION ||
			m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_PROXIMITY ||
			m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_STRUCTURE ||
			m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_ZSCORE)
		{
			m_pDoc->Add_ResultStyle(m_pDoc->m_ModeIndiceGlobal);
		}
	}

	/*
	if (ChkBtnGlobal)
	// if(m_But_Global.GetCheck() != 0)
	{
		// Show the menu for the button selection
		// CRect Rect;
		//m_But_Global.GetWindowRect(&Rect);

		CMenu Menu;
		Menu.LoadMenu(IDR_AN_MENU_GLOBAL);
		CMenu* global_menu = Menu.GetSubMenu(0);

		// Vire les options non nécessaires (selon Advision ou Profiling)
		SuppOptionButGlobal(global_menu);
		
		// Selon paramètre option univers
		if(m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES)
		{
			// Ici Univers considéré comme cible
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_PROXIMITY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_AFFINITE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_ZSCORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		else
		{
			// Ici Univers non considéré comme cible
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_PROXIMITY, MF_BYCOMMAND | MF_ENABLED);
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_AFFINITE, MF_BYCOMMAND | MF_ENABLED);
			global_menu->EnableMenuItem(IDM_AN_GLOBAL_ZSCORE, MF_BYCOMMAND | MF_ENABLED);
		}

		switch(global_menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,Rect.left,Rect.top,this))
		{
			case IDM_AN_GLOBAL_STRUCTURE:
				m_pDoc->Add_ResultStyle(RSTYLE_GBL_STRUCTURE);
			break;

			case IDM_AN_GLOBAL_AFFINITE:
				m_pDoc->Add_ResultStyle(RSTYLE_GBL_CORRELATION);
			break;

			case IDM_AN_GLOBAL_PROXIMITY:
				m_pDoc->Add_ResultStyle(RSTYLE_GBL_PROXIMITY);
			break;

			case IDM_AN_GLOBAL_ZSCORE:
				m_pDoc->Add_ResultStyle(RSTYLE_GBL_ZSCORE);
			break;

			default:
			{
				m_But_Global.SetCheck(0); 
				m_But_Global2.SetCheck(0); 
				m_But_Global3.SetCheck(0); 
				return;
			}
			break;
		}
	}
	*/

	// Récupére valeur sélectionnée
	GetGrandeurs();
	if (m_But_Ordonner.GetCheck() == 1)
	{
		// Initialisation si surlignage en cours
		m_pDoc->ResetSurlignage();
	}

	// Attention si surlignage effectué précédemment sur case global, et que option dévalidé
	// Alors on désaffiche les résultats dans surlignage
	// if (m_But_Global.GetCheck() == 0 && m_pDoc->Get_SurlignageStyle() == RSTYLE_GLOBAL)
	if (ChkBtnGlobal == false && m_pDoc->Get_SurlignageStyle() == RSTYLE_GLOBAL)
	{
		m_pDoc->Set_SurlignageStyle(RSTYLE_NOSTYLE);
	}
	
	// Remet en place les sélecteurs pour le tri
	SetupSelTri();

	// Mise en forme des résultats
	SetTitre();
	m_pCrossTable->CalculateItems(false);

	// Remet Row & Col Scroll Initiale
	GoToInitialPosCroll(TopRow, LeftCol, RSTYLE_GLOBAL, ChkBtnGlobal);
	m_CrossTable.SetLeftCol(LeftCol);
	m_CrossTable.SetTopRow(TopRow);

	// Redessine après recalage ligne colonne via posotion scrolls
	m_pCrossTable->RedrawGrid();

	UpdateSurlignage();

	// Mise à jour titre surlignage si fenêtre surlignage ouverte
	m_DlgSurlignage.UpdateTitre();

	// Redessine la boite config
	m_Box_Config.Invalidate();
	m_Box_Config.RedrawWindow();

}

// Gestion menu contextuel en cliquant avec bouton droit
LRESULT CDlgAnalyseur::OnBnLClickedAnButGlobal(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// Gestion Bouton Global (click droit pour affichage menu contextuel sélection grandeur)
void CDlgAnalyseur::RClickedAnButGlobal()
{
	// Menu Popup 4éme indicateur (en bas à droite d'une cellule)
	switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
	{
		case RSTYLE_GBL_STRUCTURE :
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_STRUCTURE);
		break;
		case RSTYLE_GBL_CORRELATION:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_CORRELATION);
		break;
		case RSTYLE_GBL_PROXIMITY:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_PROXIMITY);
		break;
		case RSTYLE_GBL_ZSCORE:
			m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);
		break;
	}

	
	// Show the menu for the button selection
	// Récupère coordonnées bouton global
	JBool ChkBtnGlobal = false;
	CRect Rect;
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if(m_But_Global.GetCheck() != 0) 	ChkBtnGlobal = true;
		m_But_Global.GetWindowRect(&Rect);
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if(m_But_Global2.GetCheck() != 0) 	ChkBtnGlobal = true;
		m_But_Global2.GetWindowRect(&Rect);
	}
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if(m_But_Global3.GetCheck() != 0) 	ChkBtnGlobal = true;
		m_But_Global3.GetWindowRect(&Rect);
	}
	// CRect Rect;
	// m_But_Global.GetWindowRect(&Rect);

	/*
	CMenu Menu;
	Menu.LoadMenu(IDR_AN_MENU_GLOBAL);
	CMenu* global_menu = Menu.GetSubMenu(0);

	// Vire les options non nécessaires (selon Advision ou Profiling)
	SuppOptionButGlobal(global_menu);
	
	// Selon paramètre option univers
	if(m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES)
	{
		// Ici Univers considéré comme cible
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_PROXIMITY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_AFFINITE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_ZSCORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		// Ici Univers non considéré comme cible
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_PROXIMITY, MF_BYCOMMAND | MF_ENABLED);
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_AFFINITE, MF_BYCOMMAND | MF_ENABLED);
		global_menu->EnableMenuItem(IDM_AN_GLOBAL_ZSCORE, MF_BYCOMMAND | MF_ENABLED);
	}

	switch(global_menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,Rect.left,Rect.top,this))
	{
		case IDM_AN_GLOBAL_STRUCTURE:
			m_pDoc->Add_ResultStyle(RSTYLE_GBL_STRUCTURE);
		break;

		case IDM_AN_GLOBAL_AFFINITE:
			m_pDoc->Add_ResultStyle(RSTYLE_GBL_CORRELATION);
		break;

		case IDM_AN_GLOBAL_PROXIMITY:
			m_pDoc->Add_ResultStyle(RSTYLE_GBL_PROXIMITY);
		break;

		case IDM_AN_GLOBAL_ZSCORE:
			m_pDoc->Add_ResultStyle(RSTYLE_GBL_ZSCORE);
		break;

		default:
		{
			m_But_Global.SetCheck(0); 
			m_But_Global2.SetCheck(0); 
			m_But_Global3.SetCheck(0); 
			return;
		}
		break;
	}
	*/

	// Récupére valeur sélectionnée
	GetGrandeurs();
	if (m_But_Ordonner.GetCheck() == 1)
	{
		// Initialisation si surlignage en cours
		m_pDoc->ResetSurlignage();
	}

	// Attention si surlignage effectué précédemment sur case global, et que option dévalidé
	// Alors on désaffiche les résultats dans surlignage
	// if (m_But_Global.GetCheck() == 0 && m_pDoc->Get_SurlignageStyle() == RSTYLE_GLOBAL)
	if (ChkBtnGlobal == false && m_pDoc->Get_SurlignageStyle() == RSTYLE_GLOBAL)
	{
		m_pDoc->Set_SurlignageStyle(RSTYLE_NOSTYLE);
	}
	
	// Mise en forme des résultats
	SetTitre();
	m_pCrossTable->CalculateItems();
	UpdateSurlignage();
}

// Gestion Bouton Global
void CDlgAnalyseur::LClickedAnButGlobal()
{
	int Toto = 1;
}

// Suppression des options calcul non nécessaires
void CDlgAnalyseur::SuppOptionButGlobal(CMenu *pMenu)
{
	// par défaut on supprime le ZScore
	pMenu->RemoveMenu(IDM_AN_GLOBAL_ZSCORE,MF_BYCOMMAND); 
}

void CDlgAnalyseur::OnBnClickedAnButExclu()
{
	if(m_But_Exclusifs.GetCheck() != 0)
	{
		if(m_pDoc->Get_TableStyle()&TSTYLE_DUPLIQUE)
		{
			// Vire le mode dupliquant
			m_pDoc->Remove_TableStyle(TSTYLE_DUPLIQUE);
			m_ModeDuplicant = FALSE;
		}

		// Mode Exclusif
		m_pDoc->Add_TableStyle(TSTYLE_EXCLUSIVE);
		m_pDoc->Set_ClickMode(CMODE_BASE100);
		m_ModeExclusif = TRUE;

	}
	else
	{
		m_pDoc->Remove_TableStyle(TSTYLE_EXCLUSIVE);
		m_ModeExclusif = FALSE;
	}

	SetTitre();
	m_pCrossTable->CalculateItems(); 

	UpdateSurlignage();
}

void CDlgAnalyseur::OnBnClickedAnButDupli()
{
	if(m_But_Dupliquants.GetCheck() != 0)
	{
		if(m_pDoc->Get_TableStyle()&TSTYLE_EXCLUSIVE)
		{
			// Vire le mode exclusif
			m_pDoc->Remove_TableStyle(TSTYLE_EXCLUSIVE);
			m_ModeExclusif = FALSE;
		}
		m_pDoc->Add_TableStyle(TSTYLE_DUPLIQUE);

		// Mode duplicant actif
		m_ModeDuplicant = TRUE;
	}
	else
	{
		m_pDoc->Remove_TableStyle(TSTYLE_DUPLIQUE);

		// Mode duplicant inactif
		m_ModeDuplicant = FALSE;
	}

	SetTitre();
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
}

void CDlgAnalyseur::SetResultControls()
{
	long ValResult = m_pDoc->Get_ResultStyle();

	if(m_pDoc->Get_ResultStyle()&RSTYLE_RESULT)
	{
		m_But_Grandeur.SetCheck(1); 
		m_But_Grandeur2.SetCheck(1); 
		m_But_Grandeur3.SetCheck(1); 
	}
	else
	{
		m_But_Grandeur.SetCheck(0);
		m_But_Grandeur2.SetCheck(0);
		m_But_Grandeur3.SetCheck(0);
	}

	if(m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL)
	{
		m_But_Horiz.SetCheck(1); 
		m_But_Horiz2.SetCheck(1); 
		m_But_Horiz3.SetCheck(1); 
	}
	else
	{
		m_But_Horiz.SetCheck(0);
		m_But_Horiz2.SetCheck(0);
		m_But_Horiz3.SetCheck(0);
	}

	if(m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL)
	{
		m_But_Vert.SetCheck(1); 
		m_But_Vert2.SetCheck(1);
		m_But_Vert3.SetCheck(1);
	}
	else
	{
		m_But_Vert.SetCheck(0);
		m_But_Vert2.SetCheck(0);
		m_But_Vert3.SetCheck(0);
	}

	// Affichage tooltip sur les boutons grandeurs
	//
	m_But_Grandeur.SetTooltipText("Grandeur");
	m_But_Grandeur2.SetTooltipText("Grandeur"); 
	m_But_Grandeur3.SetTooltipText("Grandeur"); 

	// Tooltip info grandeur % en horizontal
	m_But_Horiz.SetTooltipText("% en horizontal");
	m_But_Horiz2.SetTooltipText("% en horizontal");
	m_But_Horiz3.SetTooltipText("% en horizontal");

	// Tooltip info grandeur % en vertical
	m_But_Vert.SetTooltipText("% en vertical");
	m_But_Vert2.SetTooltipText("% en vertical");
	m_But_Vert3.SetTooltipText("% en vertical");

	// Mise à jour texte bouton indice
	if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_CORRELATION)
	{
		// Indice d'affinité
		/*
		m_But_Global.SetTooltipText(IDS_AN_INDAFFINITE);
		m_But_Global2.SetTooltipText(IDS_AN_INDAFFINITE);
		m_But_Global3.SetTooltipText(IDS_AN_INDAFFINITE);
		*/
		// Aout 2013
		m_But_Global.SetTooltipText(IDS_AN_CONCOMITENCE);
		m_But_Global2.SetTooltipText(IDS_AN_CONCOMITENCE);
		m_But_Global3.SetTooltipText(IDS_AN_CONCOMITENCE);
	}
	else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_PROXIMITY)
	{
		// Indice de proximité
		m_But_Global.SetTooltipText(IDS_AN_PROXIMITY);
		m_But_Global2.SetTooltipText(IDS_AN_PROXIMITY);
		m_But_Global3.SetTooltipText(IDS_AN_PROXIMITY);
	}
	else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_ZSCORE)
	{
		// ZScore
		m_But_Global.SetTooltipText(IDS_AN_ABR_ZSCORE);
		m_But_Global2.SetTooltipText(IDS_AN_ABR_ZSCORE);
		m_But_Global3.SetTooltipText(IDS_AN_ABR_ZSCORE);
	}
	else if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_STRUCTURE)
	{
		// Global
		m_But_Global.SetTooltipText(IDS_AN_INDGLOBAL);
		m_But_Global2.SetTooltipText(IDS_AN_INDGLOBAL);
		m_But_Global3.SetTooltipText(IDS_AN_INDGLOBAL);
	}
	
	CString GblStr;
	GblStr.LoadString(IDS_AN_ABR_CORRELATION);
	if(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
	{
		m_But_Global.SetCheck(1);
		m_But_Global2.SetCheck(1);
		m_But_Global3.SetCheck(1);

		switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			case RSTYLE_GBL_STRUCTURE:
				GblStr.LoadString(IDS_AN_ABR_GLOBAL);
				break;
			case RSTYLE_GBL_CORRELATION:
//				GblStr.LoadString(IDS_AN_ABR_CORRELATION);
				// Aout 2013
				GblStr.LoadString(IDS_AN_ABR_CONCOMITENCE);

			break;
			case RSTYLE_GBL_PROXIMITY:
				GblStr.LoadString(IDS_AN_ABR_PROXIMITY);
			break;
			case RSTYLE_GBL_ZSCORE:
				GblStr.LoadString(IDS_AN_ABR_ZSCORE);
			break;
		}
	}
	else
	{
		m_But_Global.SetCheck(0);
		m_But_Global2.SetCheck(0);
		m_But_Global3.SetCheck(0);

		// Mise à jour texte bouton indice
		if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_CORRELATION)
		{
			// Aout 2013
			// GblStr.LoadString(IDS_AN_ABR_CORRELATION)
			GblStr.LoadString(IDS_AN_ABR_CONCOMITENCE);;
		}
		else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_PROXIMITY)
		{
			GblStr.LoadString(IDS_AN_ABR_PROXIMITY);
		}
		else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_ZSCORE)
		{
			GblStr.LoadString(IDS_AN_ABR_ZSCORE);
		}
		else if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_STRUCTURE)
		{
			GblStr.LoadString(IDS_AN_ABR_GLOBAL);
		}
	}

	m_But_Global.SetWindowText(GblStr); 
	m_But_Global2.SetWindowText(GblStr);
	m_But_Global3.SetWindowText(GblStr);

	if(m_pDoc->Get_TableStyle()&TSTYLE_EXCLUSIVE)
		m_But_Exclusifs.SetCheck(1); 
	else
		m_But_Exclusifs.SetCheck(0);

	if(m_pDoc->Get_TableStyle()&TSTYLE_DUPLIQUE)
		m_But_Dupliquants.SetCheck(1); 
	else
		m_But_Dupliquants.SetCheck(0);

}

LRESULT CDlgAnalyseur::OnSeuilsChanged(WPARAM wParam, LPARAM lParam)
{
	m_pDoc->Set_Seuils();

	// Remet les paramètres de calcul
	m_pDoc->SetCalculation();

	// Relance les calculs
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Export résultat dans un fichier
void CDlgAnalyseur::OnTableauExporterPressepapier()
{
	// Appel au clipboard tableur
	m_pCrossTable->CopyAllToClipboard();	
}

void CDlgAnalyseur::OnUpdateTableauExporterPressepapier(CCmdUI *pCmdUI)
{
	// Validation copie presse papier si au moins 1 cellule "active"
	if (m_CrossTable.GetRowCount() > 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

//////////////////////////////////////////////////////////////////////////////////
// Export résultat dans un fichier
void CDlgAnalyseur::OnTableauExporterFichier()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		// Appel au clipboard tableur
		if(m_pCrossTable->CopyAllToClipboard())
		{
			CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");
		}
	}
	else
	{
		CWaitCursor wait;

		CStdioFile txtFile;

		try
		{
			txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			m_pCrossTable->Export(txtFile);
		}
		catch (CFileException *e)
		{
			CString txt;
			txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
			MessageBox(txt);
			return;
		}

		txtFile.Close();
	}
}

void CDlgAnalyseur::OnUpdateTableauExporterFichier(CCmdUI *pCmdUI)
{
	// Validation copie résultat fichier
	if (m_CrossTable.GetRowCount() > 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CDlgAnalyseur::OnTableauImprimer()
{
	// Appel au clipboard tableur
	if(m_pCrossTable->CopyAllToClipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
	}
}

/*
void CDlgAnalyseur::OnUpdateTableauExporterExcel(CCmdUI *pCmdUI)
{
	// Validation copie résultat fichier
	if (m_CrossTable.GetRowCount() > 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CDlgAnalyseur::OnTableauExporterExcel()
{
	// Appel au clipboard tableur
	if(m_pCrossTable->CopyAllToClipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open(path, "DefinirTypeExportPresse");
	}
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXPORT EXCEL VIA MACRO AVEC DECOUPAGE DES PAGES
void CDlgAnalyseur::OnUpdateTableauExporterExcel_AvecDecoup(CCmdUI *pCmdUI)
{
	// Validation copie résultat fichier
	if (m_CrossTable.GetRowCount() > 1 && m_pDoc->m_ModeDecoupage == m_pDoc->AffichModeDecoupage::MODE_AVEC_DECOUPAGE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CDlgAnalyseur::OnTableauExporterExcel_AvecDecoup()
{
	// Signal export sans découpage
	m_pDoc->m_ModeDecoupage = m_pDoc->MODE_AVEC_DECOUPAGE;

	// Appel au clipboard tableur
	if(m_pCrossTable->CopyAllToClipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open(path, "DefinirTypeExportPresse");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXPORT EXCEL VIA MACRO SANS DECOUPAGE DES PAGES (A LA QUEUE LEU LEU !!!!)
void CDlgAnalyseur::OnUpdateTableauExporterExcel_SansDecoup(CCmdUI *pCmdUI)
{
	// Validation copie résultat fichier
	if (m_CrossTable.GetRowCount() > 1 && m_pDoc->m_ModeDecoupage == m_pDoc->AffichModeDecoupage::MODE_SANS_DECOUPAGE)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CDlgAnalyseur::OnTableauExporterExcel_SansDecoup()
{
	// Signal export sans découpage
	m_pDoc->m_ModeDecoupage = m_pDoc->MODE_SANS_DECOUPAGE;

	// Appel au clipboard tableur
	if(m_pCrossTable->CopyAllToClipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open(path, "DefinirTypeExportPresse");
	}
}

///////////////////////////////////////////////////////////////
// Gestion ou pas de la marge d'erreur
void CDlgAnalyseur::OnAnParametresPriseEnCompteMargerreur()
{
	// Inverse choix calcul avec ou sans marge d'erreur
	bool AffichMargeErreur = m_pDoc->CalculMargeErreur();
	AffichMargeErreur = !AffichMargeErreur;
	m_pDoc->SetModeMargeErreur(AffichMargeErreur);	

	// Recalcul
	CWaitCursor Cw;
	if(m_pCrossTable)
	{
		m_pCrossTable->CalculateItems();
		UpdateSurlignage();
	}
	SetTitre();
}

void CDlgAnalyseur::OnUpdateAnParametresPriseEnCompteMargerreur(CCmdUI *pCmdUI)
{
	// Positionne option 
	if (m_pDoc->CalculMargeErreur())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}	

///////////////////////////////////////////////////////////////
// Force au calcul quantitatif
void CDlgAnalyseur::OnAnParametresForceQuanti()
{
	bool ForceQuantiCalcul = m_pDoc->GetForceQuantiCalcul();
	m_pDoc->SetForceQuantiCalcul(!ForceQuantiCalcul);

	// Recalcul
	CWaitCursor Cw;
	if(m_pCrossTable)
	{
		m_pCrossTable->CalculateItems();
		UpdateSurlignage();
	}
	SetTitre();
}

void CDlgAnalyseur::OnUpdateAnParametresForceQuanti(CCmdUI *pCmdUI)
{
	// Positionne option 
	if (m_pDoc->GetForceQuantiCalcul())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}	

void CDlgAnalyseur::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		// Suppression sélections
		case VK_DELETE:	
		{
			AfxMessageBox("Suppression", MB_ICONEXCLAMATION);
			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////
// Gestion des messages 
BOOL CDlgAnalyseur::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE)
		{
			// Suppression des lignes ou colonnes sélectionnés dans tableau
			SupprimeSelection();
		}
		/*
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		*/
		return TRUE;
	}
	else if(pMsg->message != WM_TIMER)
	{
		int dbg = 1;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

// Suppression sélection (lignes et/ou colonnes)
void CDlgAnalyseur::SupprimeSelection()
{
	// Récup début 1er item ligne ou colonne
	int FirstColItem = m_pCrossTable->FirstIndexItemCol()+1;
	int FirstRowItem = m_pCrossTable->FirstIndexItemRow()+1;

	// Attention aux grandeurs placées en sous niveau
	if (m_List_Grandeurs.GetSelCount() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
			FirstRowItem++;
		else
			FirstColItem++;
	}

	// Tab Index lignes
	CRowColArray ArrayRows;
	ROWCOL nRowCount = m_CrossTable.GetSelectedRows(ArrayRows);

	// Tab Index colonnes
	CRowColArray ArrayCols;
	ROWCOL nColCount = m_CrossTable.GetSelectedCols(ArrayCols);
	
	// Suppression lignes ou colonnes
	JBool SuppRows  = false;
	JBool SuppCols = false;

	if (nColCount > 0)
		if (ArrayCols[0] == 0)	SuppRows  = true;

	if (nRowCount > 0)
		if (ArrayRows[0] == 0)	SuppCols = true;

	// Debut et fin index réel suppression lignes
	JUnt32 DebRowDel = 0;
	JUnt32 FinRowDel = 0;
	if (SuppRows && nRowCount > 0)
	{
		// Suppression des entetes lignes (celles avec les lignes items)
		if (ArrayRows[0] > FirstRowItem)
		{
			DebRowDel = ArrayRows[0] - FirstRowItem;
			FinRowDel = ArrayRows[nRowCount-1] - FirstRowItem;
		}
	}
	// Attention si on n'est pas sur les bornes autorisées suppression de ligne
	if (DebRowDel == 0 || FinRowDel == 0)
		SuppRows = false;

	// Debut et fin index réel suppression colonnes
	JUnt32 DebColDel = 0;
	JUnt32 FinColDel = 0;
	if (SuppCols && nColCount > 0)
	{
		// Suppression des entetes colonnes (celles avec les lignes items)
		if (ArrayCols[0] > FirstColItem)
		{
			DebColDel = ArrayCols[0] - FirstColItem;
			FinColDel = ArrayCols[nColCount-1] - FirstColItem;
		}
	}
	// Attention si on n'est pas sur les bornes autorisées suppression de colonne
	if (DebColDel == 0 || FinColDel == 0)
		SuppCols = false;

	// Flag Bouton Grandeur Horz, Vert et Global
	JBool BtnGrandeur	= false;
	JBool BtnHorz		= false;
	JBool BtnVert		= false;
	JBool BtnGlobal		= false;

	// Boutons mode colonne visibles ou invisibles
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		if (m_But_Grandeur.GetCheck()	== 1)	BtnGrandeur = true;
		if (m_But_Horiz.GetCheck()		== 1)	BtnHorz		= true;
		if (m_But_Vert.GetCheck()		== 1)	BtnVert		= true;
		if (m_But_Global.GetCheck()		== 1)	BtnGlobal	= true;
	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		if (m_But_Grandeur2.GetCheck()	== 1)	BtnGrandeur = true;
		if (m_But_Horiz2.GetCheck()		== 1)	BtnHorz		= true;
		if (m_But_Vert2.GetCheck()		== 1)	BtnVert		= true;
		if (m_But_Global2.GetCheck()	== 1)	BtnGlobal	= true;
	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		if (m_But_Grandeur3.GetCheck()	== 1)	BtnGrandeur = true;
		if (m_But_Horiz3.GetCheck()		== 1)	BtnHorz		= true;
		if (m_But_Vert3.GetCheck()		== 1)	BtnVert		= true;
		if (m_But_Global3.GetCheck()	== 1)	BtnGlobal	= true;
	}

	// On autorise la suppression uniquement en mode simple calcul
	// (1 seul terrain, 1 seule cible, 1 seule grandeur de sélectionnée
	if ( (SuppCols && 
				   m_List_Terrains.GetSelCount()	== 1 && 
				   BtnHorz							== false && 
				   BtnGlobal						== false &&
				  (!(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) || (m_List_Grandeurs.GetSelCount() == 1 && (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)))
				   )  ||
		 (SuppRows && 
		           BtnGrandeur						== true && 
				   BtnVert							== false && 
				   BtnGlobal						== false &&
 			      ((m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) || (m_List_Grandeurs.GetSelCount() == 1) && !(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)))
		 )	
	{
		// Test si lignes ou colonnes à supprimer
		if (SuppCols || SuppRows)
		{
			// Récup structure all items
			LIST_ITEM_PUPITRE & AllItems = m_pDoc->GetAllItems();

			// Tempo Totalité des sélections
			LIST_ITEM_PUPITRE			Temp_Original;

			// Vire les lignes sélectionnées
			CONST_LIST_ITEM_PUPITRE RowItems;
			m_pDoc->GetRowItems(RowItems);

			// Temp Row
			CONST_LIST_ITEM_PUPITRE		Temp_Row;

			// On se place sur le 1er élmt à supprimer
			for (AllItems.MoveFirst(); AllItems.IsValid(); AllItems.MoveNext())
			{
				// Récup item ligne dans liste générale
				CItemWrapper * pItemWrapperAll = AllItems.GetItem();

				// balaye les lignes
				JUnt32 InxRow = 1;
				for (RowItems.MoveFirst(); RowItems.IsValid(); RowItems.MoveNext())
				{
					if ((InxRow < DebRowDel || InxRow > FinRowDel && SuppRows) || !SuppRows)
					{
						// Récup item
						const CItemWrapper * pItemWrapperRow = RowItems.GetItem();
						if (pItemWrapperRow == pItemWrapperAll)
						{
							// Row à conserver
							Temp_Original.AddTail() = pItemWrapperAll; 
							Temp_Row.AddTail()		= pItemWrapperRow; 
							break;
						}						
					}			
					InxRow++;
				}
			}

			// Remplace avec nouveau tableau rows
			RowItems.Swap(Temp_Row);
			int RowDataType = m_pDoc->GetRowDataType();
			m_pDoc->SetRowItems(RowItems, RowDataType);

			// Vire les colonnes sélectionnées
			CONST_LIST_ITEM_PUPITRE ColItems;
			m_pDoc->GetColItems(ColItems);

			// Tempo sélection Colonnes
			CONST_LIST_ITEM_PUPITRE		Temp_Col;

			// On se place sur le 1er élmt à supprimer
			for (AllItems.MoveFirst(); AllItems.IsValid(); AllItems.MoveNext())
			{
				// Récup item ligne dans liste générale
				CItemWrapper * pItemWrapperAll = AllItems.GetItem();

				// balaye les colonnes
				JUnt32 InxCol = 1;
				for (ColItems.MoveFirst(); ColItems.IsValid(); ColItems.MoveNext())
				{
					if ((InxCol < DebColDel || InxCol > FinColDel && SuppCols) || !SuppCols)
					{
						// Récup item colonne
						const CItemWrapper * pItemWrapperCol = ColItems.GetItem();
						if (pItemWrapperCol == pItemWrapperAll)
						{
							// Row à conserver
							Temp_Original.AddTail() = pItemWrapperAll; 
							Temp_Col.AddTail()		= pItemWrapperCol; 
							break;
						}						
					}			
					InxCol++;
				}
			}
			// Remplace avec nouveau tableau cols
			ColItems.Swap(Temp_Col);
			int ColDataType = m_pDoc->GetColDataType();
			m_pDoc->SetColItems(ColItems, ColDataType);
			
			// Remplace avec nouveau tableau générale
			AllItems.Swap(Temp_Original); 
			
			// Redessine et recalcul le tableur
			m_pDoc->Set_Calculation(CALC_ROWS | CALC_COLS);
			MiseEnPlaceGridTC(false);	
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Redessine le curseur (en mode page 100, exclusif ou duplicant
BOOL CDlgAnalyseur::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_ModeBase100Actif)
	{
		// On est en mode page 100
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_BASE100);
	}
	else if (m_ModeExclusif)
	{
		// On est dans le mode exclusif
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_EXCL);
	}
	else if (m_ModeDuplicant)
	{
		// On set dans le mode duplicant
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_DUP);
	}
	else if (m_ModeCurseurTri)
	{
		// On set dans le mode duplicant
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_TRI);
	}
	else
		// Remet le curseur d'origine sur le grid stingray
		m_CrossTable.SetGridCursor(0);

	// Sinon on prend le curseur standard
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
// Set cursor grille
void CDlgAnalyseur::SetGridCursor()
{
	if (m_ModeBase100Actif)
	{
		// On est en mode page 100
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_BASE100);
	}
	else if (m_ModeExclusif)
	{
		// On est dans le mode exclusif
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_EXCL);
	}
	else if (m_ModeDuplicant)
	{
		// On set dans le mode duplicant
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_DUP);
	}
	else if (m_ModeCurseurTri)
	{
		// On set dans le mode duplicant
		// Et modifie cursor du Grid (StingRay)
		m_CrossTable.SetGridCursor(IDC_AN_TRI);
	}
	else
		// Remet le curseur d'origine sur le grid stingray
		m_CrossTable.SetGridCursor(0);
}

// Positionne curseur tri
void CDlgAnalyseur::SetCursorTri(JBool Tri)
{
	m_ModeCurseurTri = Tri;
}

///////////////////////////////////////// Option EXCLUSIFS //////////////////////////////////////////
//
void CDlgAnalyseur::OnAnParamExclusifs()
{
	// Valide ou dévalide l'option
	m_ModeExclusif = !m_ModeExclusif;

	if (m_ModeExclusif) 
	{
		if(m_pDoc->Get_TableStyle()&TSTYLE_DUPLIQUE)
		{
			// Vire le mode dupliquant
			m_pDoc->Remove_TableStyle(TSTYLE_DUPLIQUE);
			m_ModeDuplicant = FALSE;
		}

		// Mode Exclusif
		m_pDoc->Add_TableStyle(TSTYLE_EXCLUSIVE);
		m_pDoc->Set_ClickMode(CMODE_BASE100);
        
		// Change le curseur
		m_CrossTable.SetGridCursor(IDC_AN_EXCL);
	}
	else
	{
		// Vire l'option exclusif
		m_pDoc->Remove_TableStyle(TSTYLE_EXCLUSIVE);
	}

	SetTitre();
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();

	// Si aucun mode actif, on remet le curseur de base
	if (!m_ModeExclusif && !m_ModeDuplicant)
		m_CrossTable.SetGridCursor(0);


}

void CDlgAnalyseur::OnUpdateAnParamExclusifs(CCmdUI *pCmdUI)
{
	if (m_ModeExclusif)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

///////////////////////////////////////// Option DUPLICANTS //////////////////////////////////////////
//
void CDlgAnalyseur::OnAnParamDuplicants()
{
	// Permut l'option
	m_ModeDuplicant = !m_ModeDuplicant;
	
	// Relance les calculs
	if(m_ModeDuplicant)
	{
		if(m_pDoc->Get_TableStyle()&TSTYLE_EXCLUSIVE)
		{
			// Vire le mode exclusif
			m_pDoc->Remove_TableStyle(TSTYLE_EXCLUSIVE);
			m_ModeExclusif = FALSE;
		}
		m_pDoc->Add_TableStyle(TSTYLE_DUPLIQUE);

		// Change le curseur
		m_CrossTable.SetGridCursor(IDC_AN_DUP);
	}
	else
	{
		// Vire l'option duplicant
		m_pDoc->Remove_TableStyle(TSTYLE_DUPLIQUE);
	}

	SetTitre();
	m_pCrossTable->CalculateItems(); 
	UpdateSurlignage();

	// Si aucun mode actif, on remet le curseur de base
	if (!m_ModeExclusif && !m_ModeDuplicant)
		m_CrossTable.SetGridCursor(0);
}

void CDlgAnalyseur::OnUpdateAnParamDuplicants(CCmdUI *pCmdUI)
{
	if (m_ModeDuplicant)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);	
}

///////////////////////////////////////// Option BASE 100 //////////////////////////////////////////
//
void CDlgAnalyseur::OnAnParamBase100()
{
	// Permut l'option
	m_ModeBase100Actif = !m_ModeBase100Actif;

	// Active ou désactive la base 100
	if (m_ModeBase100Actif)
		m_pDoc->Set_ClickMode(CMODE_BASE100);
	else
		m_pDoc->Set_ClickMode(CMODE_TRI);

	// Pas de surlignage
	m_But_Ordonner.SetCheck(0);

	/*
	if(m_ModeBase100Actif)
	{
		// Mode base 100 actif
        m_But_Base100   .SetCheck(1);
		m_ModeBase100Actif = TRUE;
		
		m_But_Surlignage.SetCheck(0);
	}
	else
	{
		m_pDoc->Set_ClickMode(CMODE_TRI);

		// Mode base 100 inactif
        m_But_Base100   .SetCheck(0);
		m_ModeBase100Actif = FALSE;

		m_But_Surlignage.SetCheck(0);
	}
	*/
}

void CDlgAnalyseur::OnUpdateAnParamBase100(CCmdUI *pCmdUI)
{
	if (m_ModeBase100Actif)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection de certains critères de sélection tri
//
// // Sélection de la grandeur pour le tri
void CDlgAnalyseur::OnCbnSelchangeComboTriGrandeurs()
{
	// Récup l'index sélection grandeur
	JInt32 InxSel = m_Combo_TriGrandeurs.GetCurSel();		

	if (InxSel >= 0)
	{
		// Récup itemdata associé
		JInt32 ItemData = m_Combo_TriGrandeurs.GetItemData(InxSel);

		// Sauvegarde le nouveau choix
		RESULTTYPE ResultType = (RESULTTYPE) ItemData;
		m_pDoc->Set_GrandeurTri(ResultType); 
	}
}

// Sélection du type de résultat
void CDlgAnalyseur::OnCbnSelchangeComboTriResultats()
{
	// Récup l'index sélection résultats
	JInt32 InxSel = m_Combo_TriResultats.GetCurSel();		

	if (InxSel >= 0)
	{
		// Récup itemdata associé
		JInt32 ItemData = m_Combo_TriResultats.GetItemData(InxSel);

		// Sauvegarde le nouveau choix
		RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData;
		m_pDoc->Set_ResultTri(ResultStyle); 
	}		
}

// Sélection du type de tri (croissant, décroissant, retour origine)
void CDlgAnalyseur::OnCbnSelchangeComboTypetri()
{
	// Récup l'index sélection type de tri
	JInt32 InxSel = m_Combo_TriType.GetCurSel();		

	if (InxSel >= 0)
	{
		// Récup itemdata associé
		JInt32 ItemData = m_Combo_TriType.GetItemData(InxSel);

		// Sauvegarde le nouveau choix
		BYTE Type = (BYTE) ItemData;
		m_pDoc->Set_TypeTri(Type); 
	}		
}

// Sélection mode tri (sur toutes les lignes, par blocs de critères)
void CDlgAnalyseur::OnCbnSelchangeComboModetri()
{
	// Récup l'index sélection mode de tri
	JInt32 InxSel = m_Combo_TriMode.GetCurSel();		

	if (InxSel >= 0)
	{
		// Récup itemdata associé
		JInt32 ItemData = m_Combo_TriMode.GetItemData(InxSel);

		// Sauvegarde le nouveau choix
		BYTE Mode = (BYTE) ItemData;
		m_pDoc->Set_ModeTri(Mode); 
	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Disposition des résultats en lignes (ordre Result > H% > V% > Divers grandeurs)
void CDlgAnalyseur::OnAnDispoLignes()
{
	if (m_pDoc->GetModeAffichResult() != m_pDoc->AFFRESULT_LIGNE)
	{
		// Recup position ascenseur horizontal et vertical
		ROWCOL LeftCol = m_CrossTable.GetLeftCol();
		ROWCOL TopRow = m_CrossTable.GetTopRow();

		// Ex Mode Affich Result
		int ExModeAffich = m_pDoc->GetModeAffichResult();

		// On se positionne en mode affichage ligne
		m_pDoc->SetModeAffichResult(m_pDoc->AFFRESULT_LIGNE);

		// On recalcule
		m_pDoc->Set_Calculation(CALC_ROWS | CALC_COLS);
		m_pCrossTable->CalculateItems(false);

		// On refait le surlignage couleur
		UpdateSurlignage();

		// Remet Row & Col Scroll Initiale
		GoToInitialPosCroll2(TopRow, LeftCol, ExModeAffich, m_pDoc->AFFRESULT_LIGNE);
		m_CrossTable.SetLeftCol(LeftCol);
		m_CrossTable.SetTopRow(TopRow);

		m_pCrossTable->RedrawGrid();

		// Sauvegarde du mode d'affichage résultats
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichResult", m_pDoc->AFFRESULT_LIGNE);

		// Redisposition des sélections grandeurs
		DispoBtnGrandeurs();
	}
}

void CDlgAnalyseur::OnUpdateAnDispoLignes(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Disposition des résultats en colonnes (ordre Result > H% > V% > Divers grandeurs)
void CDlgAnalyseur::OnAnDispoColonnes()
{
	if (m_pDoc->GetModeAffichResult() != m_pDoc->AFFRESULT_COLONNE)
	{
		// Recup position ascenseur horizontal et vertical
		ROWCOL LeftCol = m_CrossTable.GetLeftCol();
		ROWCOL TopRow = m_CrossTable.GetTopRow();

		// Ex Mode Affich Result
		int ExModeAffich = m_pDoc->GetModeAffichResult();

		// On se positionne en mode affichage colonne
		m_pDoc->SetModeAffichResult(m_pDoc->AFFRESULT_COLONNE);

		// On recalcule
		m_pDoc->Set_Calculation(CALC_ROWS | CALC_COLS);
		m_pCrossTable->CalculateItems(false);

		// On refait le surlignage couleur
		UpdateSurlignage();

		// Remet Row & Col Scroll Initiale
		GoToInitialPosCroll2(TopRow, LeftCol, ExModeAffich, m_pDoc->AFFRESULT_COLONNE);
		m_CrossTable.SetLeftCol(LeftCol);
		m_CrossTable.SetTopRow(TopRow);

		m_pCrossTable->RedrawGrid();

		// Sauvegarde du mode d'affichage résultats
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichResult", m_pDoc->AFFRESULT_COLONNE);

		// Redisposition des sélections grandeurs
		DispoBtnGrandeurs();
	}	
}


void CDlgAnalyseur::OnUpdateAnDispoColonnes(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Disposition des résultats en 2 x 2 
//							 en dessous   | Result		|   H%		|
//                                        |   V%		| Grandeur  |  
void CDlgAnalyseur::OnAnDispo2par2()
{
	if (m_pDoc->GetModeAffichResult() != m_pDoc->AFFRESULT_2X2)
	{
		// Recup position ascenseur horizontal et vertical
		ROWCOL LeftCol = m_CrossTable.GetLeftCol();
		ROWCOL TopRow = m_CrossTable.GetTopRow();

		// Ex Mode Affich Result
		int ExModeAffich = m_pDoc->GetModeAffichResult();

		// On se positionne en mode affichage 2 x 2
		m_pDoc->SetModeAffichResult(m_pDoc->AFFRESULT_2X2);

		// On recalcule
		m_pDoc->Set_Calculation(CALC_ROWS | CALC_COLS);
		m_pCrossTable->CalculateItems(false);

		// On refait le surlignage couleur
		UpdateSurlignage();

		// Remet Row & Col Scroll Initiale
		GoToInitialPosCroll2(TopRow, LeftCol, ExModeAffich, m_pDoc->AFFRESULT_2X2);
		m_CrossTable.SetLeftCol(LeftCol);
		m_CrossTable.SetTopRow(TopRow);

		m_pCrossTable->RedrawGrid();

		// Sauvegarde du mode d'affichage résultats
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichResult", m_pDoc->AFFRESULT_2X2);
		
		// Redisposition des sélections grandeurs
		DispoBtnGrandeurs();
	}
}

void CDlgAnalyseur::OnUpdateAnDispo2par2(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Redispositin des boutons sélection grandeur (pour respecter la disposition sur la grille)
void CDlgAnalyseur::DispoBtnGrandeurs()
{
	// Boutons mode 2 X 2 visibles ou invisibles
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		m_But_Grandeur.	ShowWindow(SW_SHOW);
		m_But_Horiz.	ShowWindow(SW_SHOW);
		m_But_Vert.		ShowWindow(SW_SHOW);
		m_But_Global.	ShowWindow(SW_SHOW);
	}
	else
	{
		m_But_Grandeur.	ShowWindow(SW_HIDE);
		m_But_Horiz.	ShowWindow(SW_HIDE);
		m_But_Vert.		ShowWindow(SW_HIDE);
		m_But_Global.	ShowWindow(SW_HIDE);
	}

	// Boutons mode ligne visibles ou invisibles
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		m_But_Grandeur2.ShowWindow(SW_SHOW);
		m_But_Horiz2.	ShowWindow(SW_SHOW);
		m_But_Vert2.	ShowWindow(SW_SHOW);
		m_But_Global2.	ShowWindow(SW_SHOW);
	}
	else
	{
		m_But_Grandeur2.ShowWindow(SW_HIDE);
		m_But_Horiz2.	ShowWindow(SW_HIDE);
		m_But_Vert2.	ShowWindow(SW_HIDE);
		m_But_Global2.	ShowWindow(SW_HIDE);
	}

	// Boutons mode colonne visibles ou invisibles
	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		m_But_Grandeur3.ShowWindow(SW_SHOW);
		m_But_Horiz3.	ShowWindow(SW_SHOW);
		m_But_Vert3.	ShowWindow(SW_SHOW);
		m_But_Global3.	ShowWindow(SW_SHOW);
	}
	else
	{
		m_But_Grandeur3.ShowWindow(SW_HIDE);
		m_But_Horiz3.	ShowWindow(SW_HIDE);
		m_But_Vert3.	ShowWindow(SW_HIDE);
		m_But_Global3.	ShowWindow(SW_HIDE);
	}

	// Refresh des boutons
	m_But_Grandeur	.Invalidate(); 
	m_But_Grandeur2	.Invalidate(); 
	m_But_Grandeur3	.Invalidate(); 

	m_But_Horiz		.Invalidate(); 
	m_But_Horiz2	.Invalidate(); 
	m_But_Horiz3	.Invalidate(); 

	m_But_Vert		.Invalidate(); 
	m_But_Vert2		.Invalidate(); 
	m_But_Vert3		.Invalidate(); 

	m_But_Global	.Invalidate(); 
	m_But_Global2	.Invalidate(); 
	m_But_Global3	.Invalidate(); 

	// Rafraichit le pupitre config
	m_Box_Config.Invalidate(); 
	m_Box_Config.RedrawWindow(); 
	
}


//////////////////////////////////////// Menu Outils /////////////////////////////////////////
//
// Option Ranking ou classement (linéarisation) de la grille tableau croisé
void CDlgAnalyseur::OnAnOutilsLinearisation()
{
	// Retaillage fenetre surlignage si elle existe
	RetailleFenOrdonnancement();
	
	if(m_pDoc->Get_ClickMode() != CMODE_ORDONNER)
	{
		m_pDoc->Set_ClickMode(CMODE_ORDONNER);
		m_ModeBase100Actif = FALSE;
		m_But_Ordonner.SetCheck(1);
	}
	else
	{
		m_pDoc->Set_ClickMode(CMODE_TRI);
		m_ModeBase100Actif = FALSE;
		m_But_Ordonner.SetCheck(0);
	}
}

void CDlgAnalyseur::OnUpdateAnOutilsLinearisation(CCmdUI *pCmdUI)
{
	// Validation copie presse papier si au moins 1 cellule "active"
	if (m_CrossTable.GetRowCount() > 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);

	// Check = true, si fenêtre linéarisation ouverte
	if (m_pDoc->Get_ClickMode() == CMODE_ORDONNER)
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// Option surlignage du tris croisés
void CDlgAnalyseur::OnAnOutilsSurlignage()
{
	OnBnClickedAnButParamSurlignage();
}

void CDlgAnalyseur::OnUpdateAnOutilsSurlignage(CCmdUI *pCmdUI)
{
	// Validation copie presse papier si au moins 1 cellule "active"
	if (m_CrossTable.GetRowCount() > 1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode moyenne sur l'item univers
void CDlgAnalyseur::OnAnParamMoyenneTableau()
{
	if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU)
	{
		// Passe moyenne en mode calcul sur univers numérisé
		m_pDoc->m_ModeMoyenne = m_pDoc->MOY_TABLEAU;

		// Remet la cible à calculer
		int Sel = m_List_Cibles.GetCurSel();
		if(Sel != LB_ERR)
		{
			m_pDoc->Set_SelectedCible(0);
			m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
		}
		else 
			m_pDoc->Set_SelectedCible(0);

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Elmts mode moyenne tableau visibles
		UpdateElmtVarMoy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update Mode moyenne sur l'item univers
void CDlgAnalyseur::OnUpdateAnParamMoyenneTableau(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

// Get selected cible
int CDlgAnalyseur::GetIndexSelCible()
{
	int Sel = m_List_Cibles.GetCurSel();
	return Sel;
}

const CItemConstruit * CDlgAnalyseur:: GetItemCible(int Sel)
{
	return(const CItemConstruit *)m_List_Cibles.GetItemData(Sel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode moyenne sur les items lignes
void CDlgAnalyseur::OnAnParamMoyenneLigne()
{
	if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_LIGNE)
	{
		// Passe moyenne en mode calcul moyenne sur les items numérisés en ligne
		m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;

		// Remet la cible à calculer
		int Sel = m_List_Cibles.GetCurSel();
		if(Sel != LB_ERR)
		{
			m_pDoc->Set_SelectedCible(0);
			m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
		}
		else 
			m_pDoc->Set_SelectedCible(0);


		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Elmts mode moyenne tableau non visibles
		UpdateElmtVarMoy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update Mode moyenne sur les items lignes
void CDlgAnalyseur::OnUpdateAnParamMoyenneLigne(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode moyenne sur les items colonnes
void CDlgAnalyseur::OnAnParamMoyenneColonne()
{
	if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_COLONNE)
	{
		// Passe moyenne en mode calcul moyenne sur les items numérisés en colonne
		m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;

		// Remet la cible à calculer
		int Sel = m_List_Cibles.GetCurSel();
		if(Sel != LB_ERR)
		{
			m_pDoc->Set_SelectedCible(0);
			m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
		}
		else 
			m_pDoc->Set_SelectedCible(0);


		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Elmts mode moyenne tableau non visibles
		UpdateElmtVarMoy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update Mode moyenne sur les items colonnes
void CDlgAnalyseur::OnUpdateAnParamMoyenneColonne(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Update des éléments concernant les variables moyennables en mode tableau
void CDlgAnalyseur::UpdateElmtVarMoy()
{
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)
	{
		// Mode tableau, on rend visible tous les élmts
		if (m_pDoc->Get_TerrainTemoin() != NULL)
			m_Btn_Moyenne_Tableau.EnableWindow(true);
		else
			m_Btn_Moyenne_Tableau.EnableWindow(false);

		m_Btn_Moyenne_Tableau.ShowWindow(SW_SHOW);
		m_List_VarMoy.ShowWindow(SW_SHOW);
		m_Label_VarMoy.ShowWindow(SW_SHOW);
		m_Btn_Raz_VarMoy.EnableWindow(true);
		m_Btn_Raz_VarMoy.ShowWindow(SW_SHOW);
	}
	else
	{
		// Mode tableau, on les cache
		m_Btn_Moyenne_Tableau.EnableWindow(false);
		m_Btn_Moyenne_Tableau.ShowWindow(SW_HIDE);
		m_List_VarMoy.ShowWindow(SW_HIDE);
		m_Label_VarMoy.ShowWindow(SW_HIDE);
		m_Btn_Raz_VarMoy.EnableWindow(false);
		m_Btn_Raz_VarMoy.ShowWindow(SW_HIDE);
		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode calcul de la moyenne (en tenant compte des 0)
void CDlgAnalyseur::OnAnParamMoyenne0Inclus()
{
	if (m_pDoc->m_ModeCalculMoyenne != m_pDoc->CALC_MOY_0_INCLUS)
	{
		// Passe moyenne en mode calcul avec les zéros inclus
		m_pDoc->m_ModeCalculMoyenne = m_pDoc->CALC_MOY_0_INCLUS;

		// Remet la cible à calculer
		int Sel = m_List_Cibles.GetCurSel();
		if(Sel != LB_ERR)
		{
			m_pDoc->Set_SelectedCible(0);
			m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
		}
		else 
			m_pDoc->Set_SelectedCible(0);

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();

		// Remet en place les sélecteurs pour le tri
		SetupSelTri();

		// Relance le calcul
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Elmts mode moyenne tableau visibles
		UpdateElmtVarMoy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update calcul de la moyenne (en tenant compte des 0)
void CDlgAnalyseur::OnUpdateAnParamMoyenne0Inclus(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_INCLUS)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode calcul de la moyenne (en ne tenant pas compte des 0)
void CDlgAnalyseur::OnAnParamMoyenne0Exclus()
{
	if (m_pDoc->m_ModeCalculMoyenne != m_pDoc->CALC_MOY_0_EXCLUS)
	{
		// Passe moyenne en mode calcul avec les zéros inclus
		m_pDoc->m_ModeCalculMoyenne = m_pDoc->CALC_MOY_0_EXCLUS;

		// Remet la cible à calculer
		int Sel = m_List_Cibles.GetCurSel();
		if(Sel != LB_ERR)
		{
			m_pDoc->Set_SelectedCible(0);
			m_pDoc->Set_SelectedCible((const CItemConstruit *)m_List_Cibles.GetItemData(Sel));
		}
		else 
			m_pDoc->Set_SelectedCible(0);

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();

		// Remet en place les sélecteurs pour le tri
		SetupSelTri();

		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Elmts mode moyenne tableau visibles
		UpdateElmtVarMoy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update calcul de la moyenne (en ne tenant pas compte des 0)
void CDlgAnalyseur::OnUpdateAnParamMoyenne0Exclus(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection des indices de calcul
//
// Sélection Indice de corrélation
void CDlgAnalyseur::OnAnParamIndiceAffinite()
{
	m_pDoc->m_ModeIndiceGlobal = RSTYLE_GBL_CORRELATION;
	ClickedAnButGlobal();
}

void CDlgAnalyseur::OnUpdateAnParamIndiceAffinite(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_CORRELATION)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

// Sélection Indice de structure
void CDlgAnalyseur::OnAnParamIndiceStructure()
{
	m_pDoc->m_ModeIndiceGlobal = RSTYLE_GBL_STRUCTURE;
	ClickedAnButGlobal();
}

void CDlgAnalyseur::OnUpdateAnParamIndiceStructure(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_STRUCTURE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

// Sélection Indice de proximité
void CDlgAnalyseur::OnAnParamIndiceProximite()
{
	m_pDoc->m_ModeIndiceGlobal = RSTYLE_GBL_PROXIMITY;
	ClickedAnButGlobal();
}

void CDlgAnalyseur::OnUpdateAnParamIndiceProximite(CCmdUI *pCmdUI)
{
	if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_PROXIMITY)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode affichage résultats effectif
void CDlgAnalyseur::OnAnParamAffichEffMilliers()
{
	if (m_pDoc->GetTypeAffichSelEffectif() != CAnalyseurDoc::MODE_EFFECTIF_000)
	{
		// Passe affichage effectif en mode milliers
		m_pDoc->SetTypeAffichSelEffectif(CAnalyseurDoc::MODE_EFFECTIF_000);

		// Remettre à jour la liste des grandeurs sélectionnables
		SetupGrandeursBase();

		// Get the selected grandeurs
		GetGrandeurs();
		
		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Sauvegarde du mode d'affichage effectif
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichEffectif", m_pDoc->MODE_EFFECTIF_000);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update affichage résultats effectif
void CDlgAnalyseur::OnUpdateAnParamAffichEffMilliers(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetTypeAffichSelEffectif() == CAnalyseurDoc::MODE_EFFECTIF_000)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode affichage résultats avec tous les chiffres
void CDlgAnalyseur::OnAnParamAffichEffGlobal()
{
	if (m_pDoc->GetTypeAffichSelEffectif() != CAnalyseurDoc::MODE_EFFECTIF)
	{
		// Passe affichage effectif en mode global
		m_pDoc->SetTypeAffichSelEffectif(CAnalyseurDoc::MODE_EFFECTIF);

		// Remettre à jour la liste des grandeurs sélectionnables
		SetupGrandeursBase();

		// Get the selected grandeurs
		GetGrandeurs();

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Sauvegarde du mode d'affichage effectif
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichEffectif", m_pDoc->MODE_EFFECTIF);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update affichage résultats effectif avec tous les chiffres
void CDlgAnalyseur::OnUpdateAnParamAffichEffGlobal(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetTypeAffichSelEffectif() == CAnalyseurDoc::MODE_EFFECTIF)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode affichage résultats moyenne avec 2 décimales
void CDlgAnalyseur::OnAnParamAffichMoy2Dec()
{
	if (m_pDoc->GetTypeAffichSelMoyenne() != CAnalyseurDoc::MODE_AVERAGEQUANTUM_P00)
	{
		// Passe affichage moyenne avec 2 décimales
		m_pDoc->SetTypeAffichSelMoyenne(CAnalyseurDoc::MODE_AVERAGEQUANTUM_P00);

		// Remettre à jour la liste des grandeurs sélectionnables
		SetupGrandeursBase();

		// Get the selected grandeurs
		GetGrandeurs();

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Sauvegarde du mode d'affichage moyenne
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichMoyenne", m_pDoc->MODE_AVERAGEQUANTUM_P00);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update affichage résultats moyenne avec 2 décimales
void CDlgAnalyseur::OnUpdateAnParamAffichMoy2Dec(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetTypeAffichSelMoyenne() == CAnalyseurDoc::MODE_AVERAGEQUANTUM_P00)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mode affichage résultats moyenne sans décimales
void CDlgAnalyseur::OnAnParamAffichMoySansDec()
{
	if (m_pDoc->GetTypeAffichSelMoyenne() != CAnalyseurDoc::MODE_AVERAGEQUANTUM)
	{
		// Passe affichage moyenne avec 2 décimales
		m_pDoc->SetTypeAffichSelMoyenne(CAnalyseurDoc::MODE_AVERAGEQUANTUM);

		// Remettre à jour la liste des grandeurs sélectionnables
		SetupGrandeursBase();

		// Get the selected grandeurs
		GetGrandeurs();

		// Update résultat tableau
		CWaitCursor Cw;
		SetTitre();
		m_pCrossTable->CalculateItems(); 
		UpdateSurlignage();

		// Sauvegarde du mode d'affichage moyenne
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "ModeAffichMoyenne", m_pDoc->MODE_AVERAGEQUANTUM);		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Update affichage résultats moyenne sans décimales
void CDlgAnalyseur::OnUpdateAnParamAffichMoySansDec(CCmdUI *pCmdUI)
{
	if (m_pDoc->GetTypeAffichSelMoyenne() == CAnalyseurDoc::MODE_AVERAGEQUANTUM)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

	// Ne pas afficher en mode SQN_Profiling
	if (m_pDlgTableau->GetShowSite() == SHOW_SITES)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage fenêtre de Sélection des items à moyenner pour le mode tableau
void CDlgAnalyseur::OnBnClickedAnButMoyenneTableau()
{
	// Prépare les listes sélection de retour
	JList <JUnt32> LstItemMoyAuto;
	JList <JUnt32> LstItemMoyUtil;

	// Mise en place fenêtre dialogue sélection items à moyenner
	CDlgItemsMoyenne DlgItemsMoyenne(this, &LstItemMoyAuto, &LstItemMoyUtil);
	DlgItemsMoyenne.m_CATPColorsApp = CATPColors::COLORANALYSEUR;
	DlgItemsMoyenne.m_Tree_Quantis.SetEtatData(&m_pDoc->m_MapSelTreeQuanti);

	// Affiche la fenêtre de sélection des items à moyenner
	if(IDOK == DlgItemsMoyenne.DoModal())
	{
		// Mise à jour de la liste des variables moyennables crées via les items quantis automatiques
		CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);

		// On se met en mode ajout
		// m_List_VarMoy.ResetContent();
		m_List_VarMoy.SetRedraw(FALSE);
		for (LstItemMoyAuto.MoveFirst(); LstItemMoyAuto.IsValid(); LstItemMoyAuto.MoveNext())
		{
			// Chargement de l'item construit via son ident
			JUnt32 IdItem  = LstItemMoyAuto.GetItem();
			CItemConstruit * pItemConstruit = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItem);

			// Label de l'item
			CString LblItem = pItemConstruit->GetLabel().AsJCharPtr();

			// Vire le séparateur Label <> Header
			LblItem.Replace("@", "-");

			// Créer l'item wrapper associé
			CItemWrapper *pIW = new CItemWrapper();
			pIW->SetItemConstruit(pItemConstruit);

			// Ajoute à la liste
			// m_List_VarMoy.SetItemData(m_List_VarMoy.AddString(LblItem), (LPARAM)pIW);				// item date en négatif pour distinguer les items autos
			m_List_VarMoy.SetItemData(m_List_VarMoy.AddLine(LblItem, m_TemplateQuanti), (LPARAM)pIW);	// item date en négatif pour distinguer les items autos

		}
		// revient au mode items construits utilisateur par defaut
		CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);

		// Update la liste des variables moyennables
		m_List_VarMoy.SetRedraw(TRUE);

		// Mise à jour de la liste des variables moyennables crées via les items quantis utilisateurs
		for (LstItemMoyUtil.MoveFirst(); LstItemMoyUtil.IsValid(); LstItemMoyUtil.MoveNext())
		{
			// Chargement de l'item construit via son ident
			JUnt32 IdItem  = LstItemMoyUtil.GetItem();
			CItemConstruit * pItemConstruit = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItem);

			// Label de l'item
			CString LblItem = pItemConstruit->GetLabel().AsJCharPtr();

			// Créer l'item wrapper associé
			CItemWrapper *pIW = new CItemWrapper();
			pIW->SetItemConstruit(pItemConstruit);

			// Ajoute à la liste
			// m_List_VarMoy.SetItemData(m_List_VarMoy.AddString(LblItem), (LPARAM)pIW);				// item date en négatif pour distinguer les items autos
			m_List_VarMoy.SetItemData(m_List_VarMoy.AddLine(LblItem, m_TemplateQuanti), (LPARAM)pIW);	// item date en négatif pour distinguer les items autos
		}

		// Puis on sélectionne le 1er élmt
		if (m_List_VarMoy.GetCount() && m_List_VarMoy.GetCurSel() < 0)
		{
			// Nouvelle sélection
			m_List_VarMoy.SetCurSel(0);
		}

		// Puis on sélectionne d'office la grandeur valeur moyenne
		for (JUnt32 InxLst = 0; m_List_Grandeurs.GetCount(); InxLst++)
		{
			JInt32 ItemData = m_List_Grandeurs.GetItemData(InxLst);
			if (ItemData ==	RTYPE_AVERAGEQUANTUM_P00 || ItemData == RTYPE_AVERAGEQUANTUM)
			{
				m_List_Grandeurs.SetSel(InxLst, TRUE);
				break;
			}
		}
		m_List_Grandeurs.RedrawWindow();

		// Puis on refait les calculs
		OnLbnSelchangeAnListVarMoy();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Remonte le terrain sélectionné dans la liste
void CDlgAnalyseur::OnBnClickedAnButUpTerrain()
{
	CString CurTxt;

	// Liste intermédiaire
	m_List_TmpTerrains.ResetContent();

	// Nb Sel Terrains et nb tot terrains
	int NbSel		= m_List_Terrains.GetSelCount();
	int NbTerrains	= m_List_Terrains.GetCount();

	// On vérifie qu'il y a au moins 2 terrains, et qu'un seul est sélectionné
	if (NbTerrains > 1 && NbSel == 1)
	{
		// Récupère position du terrain sélectionné
		int IndexSel = m_List_Terrains.GetCurSel();
		
		if (IndexSel > 0)
		{
			int TmpItemData = 0;
			CString TmpTxt = "";

			// Construit liste temporaire avec élmt remonté
			int iTmp = 0;
			for (int i= 0; i < NbTerrains; i++)
			{
				if (i == IndexSel - 1)
				{
					TmpItemData = m_List_Terrains.GetItemData(i);
					m_List_Terrains.GetText(i, TmpTxt);
				}
				else
				{
					m_List_Terrains.GetText(i, CurTxt);
					m_List_TmpTerrains.AddString(CurTxt);
					m_List_TmpTerrains.SetItemData(iTmp, m_List_Terrains.GetItemData(i));

					if (TmpItemData != 0)
					{
						m_List_TmpTerrains.AddString(TmpTxt);
						m_List_TmpTerrains.SetItemData(iTmp+1, TmpItemData);
					}

					iTmp++;
				}
			}

			// Puis on reconstruit la liste à afficher
			m_List_Terrains.ResetContent();
			for (int i = 0; i < m_List_TmpTerrains.GetCount(); i++)
			{
				m_List_TmpTerrains.GetText(i, CurTxt);
				m_List_Terrains.AddString(CurTxt);
				m_List_Terrains.SetItemData(i, m_List_TmpTerrains.GetItemData(i)); 
				if (i == IndexSel-1)
				{
					// On remet la sélection d'origine
					m_List_Terrains.SetSel(i, true);
				}
			}

			// Puis on reinitialise les univers
			SetSelectedUniverses(true);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Redescend le terrain sélectionné dans la liste
void CDlgAnalyseur::OnBnClickedAnButDownTerrain()
{
	CString CurTxt;

	// Liste intermédiaire
	m_List_TmpTerrains.ResetContent();

	// Nb Sel Terrains et nb tot terrains
	int NbSel		= m_List_Terrains.GetSelCount();
	int NbTerrains	= m_List_Terrains.GetCount();

	// On vérifie qu'il y a au moins 2 terrains, et qu'un seul est sélectionné
	if (NbTerrains > 1 && NbSel == 1)
	{
		// Récupère position du terrain sélectionné
		int IndexSel = m_List_Terrains.GetCurSel();

		if (IndexSel <  NbTerrains - 1)
		{
			int TmpItemData = 0;
			CString TmpTxt = "";

			// Construit liste temporaire avec élmt redescendu
			int iTmp = 0;
			for (int i= NbTerrains - 1; i >= 0; i--)
			{
				if (i == IndexSel + 1)
				{
					TmpItemData = m_List_Terrains.GetItemData(i);
					m_List_Terrains.GetText(i, TmpTxt);
				}
				else
				{
					m_List_Terrains.GetText(i, CurTxt);
					m_List_TmpTerrains.AddString(CurTxt);
					m_List_TmpTerrains.SetItemData(iTmp, m_List_Terrains.GetItemData(i));
					iTmp++;

					if (TmpItemData != 0)
					{
						m_List_TmpTerrains.AddString(TmpTxt);
						m_List_TmpTerrains.SetItemData(iTmp, TmpItemData);
					}
				}
			}

			// Puis on reconstruit la liste à afficher
			m_List_Terrains.ResetContent();
			iTmp = 0;
			for (int i = m_List_TmpTerrains.GetCount() -1; i >= 0; i--)
			{
				m_List_TmpTerrains.GetText(i, CurTxt);
				m_List_Terrains.AddString(CurTxt);

				m_List_Terrains.SetItemData(iTmp, m_List_TmpTerrains.GetItemData(i)); 
				if (iTmp == IndexSel+1)
				{
					// On remet la sélection d'origine
					m_List_Terrains.SetSel(iTmp, true);
				}
				iTmp++;
			}

			// Puis on reinitialise les univers
			SetSelectedUniverses(true);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Suppression de toute la sélection des variables moyennables en mode tableau
void CDlgAnalyseur::OnBnClickedAnButRazVarMoy()
{
	// ON se met en mode ajout
	m_List_VarMoy.ResetContent();

	// Si grandeur moyenne sélectionnée on vire la sélection
	if(m_List_Grandeurs.GetSelCount()!=0)
	{
		// Get selected grandeurs
		int NbSel = m_List_Grandeurs.GetSelCount();
		int  *pSel = new int[NbSel];
		m_List_Grandeurs.GetSelItems(NbSel,pSel);   
	
		// Délectionne si valeur moyenne sélectionnée
		for(int i = 0; i < NbSel ; ++i)
		{
			JInt32 ItemData = m_List_Grandeurs.GetItemData(pSel[i]);
			if (ItemData ==	RTYPE_QUANTITY_000 || ItemData ==	RTYPE_AVERAGEQUANTUM_P00 || ItemData == RTYPE_AVERAGEQUANTUM)
			{
				m_List_Grandeurs.SetSel(pSel[i],FALSE);
			}
		}
	}		

	// Recalcul
	OnLbnSelchangeAnListVarMoy();
}

//////////////////////////////////////////////////////////////////////////////
// Test si item quantitatif (surtout pour définir les éléments moyennables)
JBool CDlgAnalyseur::ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain)
{
	CIBuilder IBuilder;

	// Test le terrain en cours
	if (pTerrain)
	{
		// Récup lé définition de l'item
		pItem->GetItemBuilder(pTerrain,IBuilder);

		// Par défaut ce n'est pas un quantitatif
		JBool Quantitatif = false;

		// Limitation des items moyennables  (sinon c'est le bordel !!!!)
		// -- seulement 1 segment
		// -- pas de combinaison dans 1 segment avec plusieurs items ou questions
		if (IBuilder.GetSegmentCount() > 1) return false;

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
					IQuestion const * pQuestion = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetQuestionByID(Items.GetItem());

					IItem const * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(Items.GetItem());

					CIBItem * pTargetItem = pIBSegment->FindItem(Items.GetItem());

					if (pTargetItem)
					{
						JList<JInt32> Moda;
						pTargetItem->GetValues(Moda);

						// Get the modalities
						for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
						{
							JInt32 ModaItemId = Moda.GetItem();
							const IItem * pModaItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(ModaItemId);
							if(pModaItem)
							{
								if (pModaItem->IsItemConstruit())
								{
									const CItemConstruit * pItemConst = pModaItem->AsItemConstruit();

									// Repérage si on est bien dans un item avec un ou des intervalles de valeurs
									CString ValString = pItemConst->GetIBString().AsJCharPtr();


									// @@@@@@
									// @@@@@ Faire le nécessaire poour les elmts d'une palette


									if (ValString.Find("<")!= -1 || ValString.Find(">") != -1)
									{
										// Test si c'est un item qauntitatif
										if (ItemQuantitatif(pItemConst,pTerrain))
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
										if (ItemQuantitatif(pItemConst,pTerrain))
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
						CPseudo * m_pPseudoItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNewPseudo();

						if(m_pPseudoItem && Items.GetItem()==0)
						{
							JList <JInt32> ItemsP;
							m_pPseudoItem->GetItems(ItemsP); 
							for (ItemsP.MoveFirst(); ItemsP.IsValid(); ItemsP.MoveNext())
							{
								JInt32 IdItem = ItemsP.GetItem();

								IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItem);
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