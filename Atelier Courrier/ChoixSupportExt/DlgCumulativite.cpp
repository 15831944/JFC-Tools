//
// Fichier: DlgCumulativite.cpp.cpp
// Date:    02/04/2004
// Gestion Interface Cumulativité dans choix Supports
//
#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgCumulativite.h"
#include "ATPGraphCrbCum.h"
#include "JFCCourbe.h"
#include "ExcelPrinter.h"
#include ".\dlgcumulativite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Constantes gestion Split horizontal et vertical
#define WM_SPLIT2_HORZ   (WM_USER + 1)
#define WM_SPLIT2_VERT   (WM_SPLIT2_HORZ + 1)

// Boîte de dialogue CDlgCumulativite
IMPLEMENT_DYNAMIC(CDlgCumulativite, JFCDialog)

CDlgCumulativite::CDlgCumulativite(CWnd* pParent, /*=NULL*/
								   JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgCumulativite::IDD, pParent), m_SplitHorz(WM_SPLIT2_HORZ)
{
	// Récupère pointeur sélection générale
	pDocChoixSupport = pDoc;

	// Init Lignes/Colonnes Grid
	m_NbHeaderRow  = 2;
	m_NbRowSupport = 0;
	m_NbHeaderCol  = 1;
	m_NbCol        = 0;

	// Nombre de couleur courbe en ligne
	m_NbLigColor = 3;

	// Nombre de couleur courbe en colonne
	m_NbColColor = 4;

	// Envoi document à interface graphique
	m_IGraphCourbeCum.SetDocument(pDocChoixSupport);

	// On initilaise le pas graduation de la courbe cumulativité à 7 jours
	LONG PasGraduationX = 7;
	m_GraphCum.SetPasGraduationEnX(PasGraduationX); 
}

CDlgCumulativite::~CDlgCumulativite()
{
}

void CDlgCumulativite::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_CADRE_CUMULATIVITE, m_CadreCumulativite);
	DDX_Control(pDX, IDC_CS_TITRE_CUMULATIVITE, m_TitreCumulativite);
	DDX_Control(pDX, IDC_CS_PROGRESS_RESULTCUM, m_ProgressResultCum);
	DDX_Control(pDX, IDC_CS_CADRE_GRAPHCUM,		m_CadreGrapheCum);
	DDX_Control(pDX, IDC_CS_GRAPHCUM,			m_GraphCum);
	DDX_Control(pDX, IDC_CS_SCROLLGRAPH,		m_ScrollGraph);
	DDX_Control(pDX, IDC_CS_SPLIT2_HORZ,		m_SplitHorz);
}


BEGIN_MESSAGE_MAP(CDlgCumulativite, JFCDialog)
	//{{AFX_MSG_MAP(CDlgCumulativite)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPLIT2_HORZ, OnSplitHorz)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// Initialisation des différents contrôles fenêtre Cumulativité
BOOL CDlgCumulativite::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fenêtre cumulativité
	SetupControls();

	// Informations générales en entête
	UpdateTitreAnalyse();

	// Mise à jour couleur graphe courante
	UpdateVisuColorGraphe();

	// Mise en place des relations objets fenêtre Cumulativité
	SetWindowRelations();

	// on récupère la position des fenêtres
	RECT RectL1; m_CadreCumulativite.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGrapheCum.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);

	// on calcule la configuration du split
	LONG SPUp = RectL1.bottom - RectL1.top; if (SPUp < 0) SPUp = 0;
	LONG SPDn = RectL2.bottom - RectL2.top; if (SPDn < 0) SPDn = 0;

	// on reconfigure le split
	m_SplitHorz.SetRange(SPDn, SPUp);

	// Init état calcul
	m_StopCalcul = false;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Gestion du déplacement split horizontal fenêtes cumulativité
LRESULT CDlgCumulativite::OnSplitHorz(WPARAM WParam, LPARAM LPAram)
{
	// on récupère le déplacement
	LONG Disp = (LONG)WParam;

	// on récupère la position des fenêtres
	RECT RectL1; m_CadreCumulativite.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGrapheCum.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);
	RECT RectL3; m_GridCumulativite.GetWindowRect(&RectL3); this->ScreenToClient(&RectL3);
	RECT RectL4; m_GraphCum.GetWindowRect(&RectL4); this->ScreenToClient(&RectL4);
	RECT RectL5; m_ProgressResultCum.GetWindowRect(&RectL5);this->ScreenToClient(&RectL5); 
	RECT RectSP; m_SplitHorz.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);

	// Limite aux déplacements pour éviter des abérations graphiques
	// Hauteur minimale grid cumulativité
	if (RectL1.bottom + Disp < 100) 
		Disp = 100 - RectL1.bottom;

	// Hauteur maximale grid cumulativité
	if (RectL1.bottom + Disp > (RectL2.bottom - 100))  
		Disp = (RectL2.bottom - 100)  - RectL1.bottom;

	// on corrige la position des fenêtres (fenêtre grid résultat)
	RectL1.bottom += Disp;
	RectL3.bottom += Disp; 
	
	// on corrige la position des fenêtres (le graph cumulativité)
	RectL2.top    += Disp;
	RectL4.top    += Disp;

	// la barre de progression
	RectL5.bottom += Disp;
	RectL5.top    += Disp;

	// La barre split
	RectSP.bottom += Disp; RectSP.top += Disp;

	// on déplace les fenêtres
	m_CadreCumulativite.SetWindowPos(NULL, RectL1.left, RectL1.top, RectL1.right - RectL1.left, RectL1.bottom - RectL1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GridCumulativite.SetWindowPos(NULL, RectL3.left, RectL3.top, RectL3.right - RectL3.left, RectL3.bottom - RectL3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER); 
	m_CadreGrapheCum.SetWindowPos(NULL, RectL2.left, RectL2.top, RectL2.right - RectL2.left, RectL2.bottom - RectL2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GraphCum.SetWindowPos(NULL, RectL4.left, RectL4.top, RectL4.right - RectL4.left, RectL4.bottom - RectL4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_ProgressResultCum.SetWindowPos(NULL, RectL5.left, RectL5.top, RectL5.right - RectL5.left, RectL5.bottom - RectL5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace la barre split
	m_SplitHorz.SetWindowPos(NULL, RectSP.left, RectSP.top, RectSP.right - RectSP.left, RectSP.bottom - RectSP.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	
	// on calcule la configuration du split
	LONG SPUp = RectL1.bottom - RectL1.top; if (SPUp < 0) SPUp = 0;
	LONG SPDn = RectL2.bottom - RectL2.top; if (SPDn < 0) SPDn = 0;

	// on reconfigure le split
	m_SplitHorz.SetRange(SPDn, SPUp);

	/*
	m_Resizer.Move();
	*/

	UpdateWindow();
	
	// on quitte
	return (0);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fenêtre Cumulativité
void CDlgCumulativite::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Cadre englobant les élmts résultats cumulativité
			{IDC_CS_CADRE_CUMULATIVITE, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}},			//Bottom side

			// Titre Cumulativité (Source + Source Audience)
			{IDC_CS_TITRE_CUMULATIVITE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
						  				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},						//Bottom side

			// Grille Résultat Cumulativité
			{IDC_CS_GRID_CUMULATIVITE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom}},	//Bottom side

			// Barre de progression
			{IDC_CS_PROGRESS_RESULTCUM,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom}},	//Bottom side

			// Cadre englobant les élmts résultats cumulativité
			{IDC_CS_CADRE_GRAPHCUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					//Bottom side

			{IDC_CS_GRAPHCUM,			{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eBottom}},		//Bottom side

			{IDC_CS_SCROLLGRAPH,		{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_CS_GRAPHCUM, CResizer::eBottom},				//Top side
										{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_CS_GRAPHCUM, CResizer::eBottom}},			//Bottom side

			// Split horizontal
			{IDC_CS_SPLIT2_HORZ,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_GRAPHCUM, CResizer::eTop}},			//Bottom side

	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contrôles fenêtre Cumulativité
void CDlgCumulativite::SetupControls()
{
	// Association objet / ressource
	m_GridCumulativite.SubclassDlgItem(IDC_CS_GRID_CUMULATIVITE, this);

	// Propriétés du cadre Cumulativite
	m_CadreCumulativite.SetTransparent(false);
	m_CadreCumulativite.SetSunken(true);

	// Propriétés du cadre graphe Cumulativite
	m_CadreGrapheCum.SetTransparent(false);
	m_CadreGrapheCum.SetSunken(true);

	// Controles en avant plan
	m_CadreCumulativite.AddTopWindow(m_TitreCumulativite);
	m_CadreCumulativite.AddTopWindow(m_GridCumulativite); 
	m_CadreCumulativite.AddTopWindow(m_ProgressResultCum);
	m_CadreGrapheCum.AddTopWindow(m_GraphCum); 
	m_CadreGrapheCum.AddTopWindow(m_ScrollGraph); 

	// Lien du scroll horizontal avec le graphe
	m_GraphCum.SetHorzScroll(&m_ScrollGraph,TRUE); 	
	this->SetScrollBar(&m_GraphCum,&m_ScrollGraph,NULL); 

	m_CadreGrapheCum.ShowWindow(SW_SHOW); 

	// Init graphe (attachement avec interface graphique)
	m_GraphCum.SetItems(&m_IGraphCourbeCum,TRUE); 

	// Progress bar non visible au départ
	m_ProgressResultCum.ShowWindow(SW_HIDE); 
	m_ProgressResultCum.SetShowText(false);
 
	// Taille et Font des différents labels
	m_TitreCumulativite.SetFontName(CATPColors::GetFontLabelName());
	m_TitreCumulativite.SetFontBold(TRUE); 
	m_TitreCumulativite.SetFontSize(CATPColors::GetFontLabelSize());
	
	
	// Initialise grid résultat cumulativité
	m_GridCumulativite.EnableMouseWheel(); 
	m_GridCumulativite.Initialize(); 
	GXSetNewGridLineMode(); 
    m_GridCumulativite.GetParam()->EnableUndo(FALSE);
	m_GridCumulativite.SetMergeCellsMode(gxnMergeDelayEval);
	m_GridCumulativite.GetParam()->SetNumberedColHeaders(false); 
	m_GridCumulativite.GetParam()->EnableMoveCols(FALSE);
	m_GridCumulativite.GetParam()->SetMinTrackColWidth(16);
	m_GridCumulativite.GetParam()->SetNumberedRowHeaders(false); 
	m_GridCumulativite.GetParam()->EnableMoveRows(FALSE);
	m_GridCumulativite.GetParam()->SetMinTrackRowHeight(16);
	m_GridCumulativite.GetParam()->SetSmartResize(true);

	// en + 
	m_GridCumulativite.GetParam()->GetProperties()->SetMarkColHeader(FALSE);

	// Enregistrement controle spécifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(&m_GridCumulativite);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridCumulativite.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Init Référence colonne
	InitRefGrid();

	// Associe le document au grille
	if (m_GridCumulativite.OnInitDocument(pDocChoixSupport,this, m_NoColIdSupport) == false)
		AfxMessageBox ("Problème association document supports / grille cumulativité");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid résultat
	if (GrilleResultValid())
	{
		if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		{
			// En mode moteur PROBA >> Nouvelle optim calcul cumul
			InitGridResultat_OPTIM();
		}
		else
		{
			// Autre mode moteur >> Calcul comme avant malgré l'incohérence calcul en Betabinomial (voir Sylvain)
			InitGridResultat();
		}
	}

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des références colonnes
void CDlgCumulativite::InitRefGrid()
{
	// Position colonne invisible id support
	m_NoColIdSupport	= 1;

	// Position colonne sel supports
	m_NoColSelSupport	= m_NoColIdSupport  + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne couleur graphe
	m_NoColColorGraphe	= m_NoColSupport + 1;

	// Position colonne formats
	// m_NoColFormat		= m_NoColColorGraphe + 1;

	// Position colonne périodicité
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColColorGraphe + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid
void CDlgCumulativite::InitGrid()
{
	// Enleve n° ligne col 
	m_GridCumulativite.GetParam()->SetNumberedColHeaders(false);
	m_GridCumulativite.GetParam()->SetNumberedRowHeaders(false);

	// Position colonne invisible id support
	m_NoColIdSupport	= 1;

	// Position colonne sel supports
	m_NoColSelSupport	= m_NoColIdSupport  + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne couleur graphe
	m_NoColColorGraphe	= m_NoColSupport + 1;

	// Position colonne formats
	// m_NoColFormat		= m_NoColColorGraphe + 1;

	// Position colonne périodicité
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColColorGraphe + 1;

	// Limite maximum insertions selon périodicité
	MaxInsGeneral		= 6;

	MaxInsMensuel		= 12;
	MaxInsBiMensuel		= 24;
	MaxInsHebdo			= 53;
	MaxInsBiHebdo		= 105;
	MaxInsQuotidien		= 365;

	// Init colonnes
	UpdateCol();

	// Initialisation si au moins 1 support
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{
		// Init Lignes
		UpdateRow();

		// Delock mode read only
		m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

		// Lignes non resizable et nb lignes headers (3 : Terrains / Cibles / Grandeurs)
		m_GridCumulativite.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

		// Aucun header colonne et colonnes bloqués (support/tarif/périodicité)
		m_GridCumulativite.SetFrozenCols(m_NoColPeriodicite - m_NoColIdSupport + 1,0);

		// Affichage entete Tarif et périodicité
		if (m_GridCumulativite.GetRowCount() > 0) 
		{
			// Titre "SEL"
			m_GridCumulativite.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
									  		 CGXStyle()
												.SetValue(_T("X")));
			// Titre "SUPPORTS"
			m_GridCumulativite.SetStyleRange(CGXRange(1,m_NoColSupport-1),
									  		 CGXStyle()
												.SetValue(_T("Supports")));
			// Titre "COULEUR GRAPHE"
			m_GridCumulativite.SetStyleRange(CGXRange(1,m_NoColColorGraphe-1),
									  		 CGXStyle()
											 .SetValue(_T("G")));

			/*
			// Titre "FORMATS"
			m_GridCumulativite.SetStyleRange(CGXRange(1,m_NoColFormat-1),
									  		 CGXStyle()
												.SetValue(_T("Formats")));
			*/

			// Titre "PERIODICITE"
			m_GridCumulativite.SetStyleRange(CGXRange(1,m_NoColPeriodicite-1),
											 CGXStyle()
									 			.SetValue(_T("Périodic.")));
		}

		// On bloque les colonnes qui doivent ne pas être modifiées
		m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Merge des cellules en header niveau 1
	m_GridCumulativite.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_GridCumulativite.GetColCount()),
					 		         CGXStyle()
										.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

	// Rend visible le grid, si au moins quelques résultats
	int NbCol = m_GridCumulativite.GetColCount();
	int NbRow = m_GridCumulativite.GetRowCount();
	if (m_GridCumulativite.GetColCount() >= m_NoColPeriodicite)
		m_GridCumulativite.ShowWindow(SW_SHOW);
	else
		m_GridCumulativite.ShowWindow(SW_HIDE);
}

// Définition des couleurs interface fenêtre Cumulativité
void CDlgCumulativite::SetColors()
{
	// Coloriage du cadre englobant
	m_CadreCumulativite.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultCum.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Titre Cumulativité
	m_TitreCumulativite.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreCumulativite.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Grille résultat
	m_GridCumulativite.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridCumulativite.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Progress bar
	m_ProgressResultCum.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage du cadre englobant
	m_CadreGrapheCum.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::LIGHT_DEGRAD); 

	// Les différentes couleurs pour headers ou cellules spécials
	UpdateColorSpe();

	// Style des lignes (solid pen)
	m_GridCumulativite.GetParam()->SetGridLineStyle(PS_SOLID); 

	// Style sélection bloc (comme Excel)
	// m_GridCumulativite.GetParam()->SetExcelLikeSelectionFrame(TRUE);
}



// Destruction de la fenêtre Cumulativité
void CDlgCumulativite::OnDestroy()
{
	JFCDialog::OnDestroy();
}

// Retaille de la fenêtre Cumulativité
void CDlgCumulativite::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();

	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_CS_GRAPHCUM);
	if (wnd)
	{
		wnd->GetWindowRect(&rect);
		ScreenToClient(rect);

		// Graphe cumulativité
		m_GraphCum.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	}
}

void CDlgCumulativite::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		// Mise à jour du Grid résultat et graphe Cumulativité
		// AfxMessageBox("A FAIRE / CUMULATIVITE",MB_ICONINFORMATION);	
		
	}
}

void CDlgCumulativite::OnCancel()
{
	JFCDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélections format supports
bool CDlgCumulativite::UpdateSelFormatSupports()
{
	// Initialise libellés supports en 1ere colonne
	InitRowSupports();

	return true;
}

//////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CDlgCumulativite::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise à jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	// Réinitialisation générale
	else if (Message == UPDATE_RAZ)
	{
		// Initialise grid résultat
	    m_GridCumulativite.Initialize();
		GXSetNewGridLineMode(); 
		m_GridCumulativite.GetParam()->EnableUndo(FALSE);
		m_GridCumulativite.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridCumulativite.GetParam()->SetNumberedColHeaders(false); 
		m_GridCumulativite.GetParam()->SetNumberedRowHeaders(false); 
		m_GridCumulativite.GetParam()->SetSmartResize(true);

		// Mise à jour des infos fenêtre échelle
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();

		// Fermeture du graphe
		UpdateGraphe();

	}

	//////////////////////////////////////////////////////////////////////////
	// Mise à jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise à jour des infos fenêtre échelle
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();
	}

	else if (Message == UPDATE_OFFRE)
	{
		// Mise à jour du graphe
		UpdateGraphe();
	}

	// Mise à jour sélection support
	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Initialisation du grid		
		InitGrid();
	}

	// Mise à jour sélection segment
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Initialisation du grid		
		InitGrid();
	}

	// Mise à jour sélection support
	else if (Message == UPDATE_SEL_COLOR_GRAPHSUPP)
	{
		// Update couleur graphe supports
		UpdateColorGraphSupports();

	}
	
	// Mise a jour suite à changement Format
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Puis on met à jour le map des formats supports dans cumulativité
		UpdateFormatSupports();

		// Remet les sélections format dans le grille
		UpdateSelFormatSupports();

		// Mise à jour des résultat coûts (uniquement si fenêtre cumul visible)
		if (this->IsWindowVisible())
			UpdateResultCout();
		else
			pDocChoixSupport->m_ParamCumModif = true;
	}

	// Mise a jour suite à changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Mise à jour couleur sélection supports
		UpdateVisuSupports();

		if (this->IsWindowVisible())
			UpdateResultCout();
		else
			pDocChoixSupport->m_ParamCumModif = true;
	}

	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Efface les colonnes innoportunes, on le fait en réaffichant
		UpdateRow();
		UpdateCol();

		// Réaffiche le résultat
		if (this->EnableWindow() == TRUE)
		{
			// Test si paramètre modifié
			if (pDocChoixSupport->m_ParamCumModif)
			{
				// Calcul cumulativité
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					InitGridResultat_OPTIM();
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgré l'incohérence calcul en Betabinomial (voir Sylvain)
					InitGridResultat();
				}
			}
		}
		else
			pDocChoixSupport->m_ParamCumModif = true;

		// Mise à jour du bandeau informations
		UpdateTitreAnalyse();
	}

	// Mise à jour sélection support
	else if (Message == UPDATE_SEL_COLOR)
	{
		// Update couleur graphe courante
		UpdateVisuColorGraphe();
	}

	// Mise à jour suite à modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();
	}

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Mise à jour des résultat coûts
		UpdateResultat(false);
	}

	// Mise à jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise à jour grid cumulativité
		m_GridCumulativite.LockUpdate(true);

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		// Active mise à jour grid cumulativité
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativité
		m_GridCumulativite.Redraw();
	}

	// Mise à jour des taris suite à modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise à jour grid mapping
		m_GridCumulativite.LockUpdate(true);

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		// Mise à jour du graphe
		UpdateGraphe();

		// Active mise à jour grid cumulativite
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativité
		m_GridCumulativite.Redraw();
	}
	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise à jour grid Cumulativite
		m_GridCumulativite.LockUpdate(true);

		// Affichage ou non des supports dans grille Cumulativite
		UpdateAfficheSupports();

		// Active mise à jour grid Cumulativite
		m_GridCumulativite.LockUpdate(false);

		// redessine le grid Cumulativite
		m_GridCumulativite.Redraw();

		// redessine le graphe cumulativité
		// m_GraphCum.RedrawWindow();
		UpdateGraphe();

	}
	// Mise à jour suite au changement nb insertions
	else if (Message == UPDATE_INSERTIONS_CUM)
	{
		// Vire le map courant résultat  ( A VOIR si on recalcul)
		// pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleCum);

		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Indique calcul à finaliser
		pDocChoixSupport->m_ParamCumModif = true;

		// Réaffiche le résultat
		if (this->IsWindowVisible())
		{
			// Mise à jour du grid cumulativité
			InitGrid();

			// Test si paramètre modifié
			if (pDocChoixSupport->m_ParamCumModif)
			{
				// Calcul cumulativité
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					InitGridResultat_OPTIM();
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgré l'incohérence calcul en Betabinomial (voir Sylvain)
					InitGridResultat();
				}
			}

			// Init Affichage Résultats Cellules
			// Suite à Optim 2.7

			// Delock mode read only
			m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);
			int NbCol = m_GridCumulativite.GetColCount();
			m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridCumulativite.GetRowCount(),m_GridCumulativite.GetColCount()),
				CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)
				.SetVerticalAlignment(DT_BOTTOM)
				.SetFont(CGXFont()
				.SetSize(pDocChoixSupport->m_SizeFontCell)
				.SetBold(pDocChoixSupport->m_BoldFontCell)));

			m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
		else
			pDocChoixSupport->m_ParamCumModif = true;
	}
	// Relance tri avec mode tri courant
	else if (Message == UPDATE_TRI_COURANT_CUM)
	{
		// Active mise à jour grid cumulativité
		m_GridCumulativite.LockUpdate(true);

		// On remet le tri à jour
		MajTriCourant();

		// Active mise à jour grid cumulativité
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativité
		m_GridCumulativite.Redraw();
	}
}	

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour du tri courant
JVoid CDlgCumulativite::MajTriCourant()
{
	// Retri avec les mêmes paramétres de tri
	m_GridCumulativite.SortColonneCourante(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgCumulativite::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Update Résultat
	m_GridCumulativite.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récupère état visibilité support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Récupération validité support sur terrain(s)/segment(s)
		JBool ValideSupport = pDocChoixSupport->ValideSupportSegments(IdSupport);

		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false)
		{	
			// Cache la ligne support
			m_GridCumulativite.SetRowHeight(NoLig, NoLig ,0); 

		}
		else
		{
			// Rend visible la ligne support
			m_GridCumulativite.SetRowHeight(NoLig, NoLig ,16); 
		}
	}

	// Recalcul les courbes cumuls
	m_IGraphCourbeCum.Update(); 

	// Mise à jour courbe cumulativité
	m_GraphCum.RedrawWindow(); 
	m_GraphCum.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports dans cumulativité
void CDlgCumulativite::UpdateFormatSupports()
{
	// on teste la validité du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise à jour map des formats supports cumulativité
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		// Mise à jour des formats par support
		pDocChoixSupport->m_MapIdFormatSpecifCum.Reset();

		// Boucle sur tous les supports
		for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
			 pDocChoixSupport->m_MapIdSupportsSel.IsValid();
			 pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
		{
			// Ident support
			JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

			// Récupére liste des idents formats possibles pour ce support (parmi les formats sélectionnés)
			JMap <JUnt32,JUnt32> MapIdFormats;
			pDocChoixSupport->GetListIdFormats(IdSupport,&MapIdFormats);

			// On prendra le format module pour ce support sauf s'il n'est pas dans la liste des formats support
			MapIdFormats.MoveTo(IdFormat);

			if (MapIdFormats.IsValid())
			{
				// Indique information format associé à ce support
				pDocChoixSupport->m_MapIdFormatSpecifCum.Add(IdSupport) = IdFormat;	
			}
			else
				// Pas d'info format (sera sélectionné ultérieurement via combo box)
				pDocChoixSupport->m_MapIdFormatSpecifCum.Add(IdSupport) = 0;	

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Colonnes
bool CDlgCumulativite::UpdateCol()
{
	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Colonnes (Support + Périodicité)
	m_NbHeaderCol = m_NoColPeriodicite - m_NoColIdSupport + 1;

	// Maintenant on tient compte directement du choix nb insertions dans menu
	// Nb Colonnes
	if (pDocChoixSupport != NULL && pDocChoixSupport->m_MapIdSupportsSel.GetCount() > 0)
	{
		// calcul max nb col, selon périodicité des supports sélectionnés
		m_MaxColIns = 1;

		m_MaxColIns = pDocChoixSupport->m_NbInsertionsCum;

		// Nb col = 1 + Max colonnes insertions (1ere colonne = Max Asypmtote)
		m_NbCol = 1 + m_MaxColIns;
	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Bidouille pour diminuer le nombre de colonnes
	// On va de toutes manières jusqu'à 12
	/*
	if (m_NbCol > MaxInsMensuel && m_NbCol <=  MaxInsBiMensuel)
		// 1 colonne supplémentaire pour les mensuels
		// m_NbCol = 13;
		m_NbCol = MaxInsGeneral + 1;
	else if (m_NbCol > MaxInsBiMensuel && m_NbCol <= MaxInsHebdo)
		// 1 colonne supplémentaire pour les bi-mensuel
		// m_NbCol = 14;
		m_NbCol = MaxInsGeneral + 2;
	else if (m_NbCol > MaxInsHebdo && m_NbCol < MaxInsBiHebdo)
		// 1 colonne supplémentaire pour les hebdomadaires
		// m_NbCol = 15;
		m_NbCol = MaxInsGeneral + 3;
	else if (m_NbCol >= MaxInsBiHebdo && m_NbCol <= MaxInsQuotidien)
		// 1 colonne supplémentaire pour les bi-hebdo
		// m_NbCol = 16;
		m_NbCol = MaxInsGeneral + 4;
	else if (m_NbCol > MaxInsQuotidien)
		// 1 colonne supllémentaire pour les quotidiens
		// m_NbCol = 17;
		m_NbCol = MaxInsGeneral + 5;
	*/

	if (m_NbCol > MaxInsMensuel && m_NbCol <=  MaxInsBiMensuel)
		// 1 colonne supplémentaire pour les mensuels
		m_NbCol = 13;
	else if (m_NbCol > MaxInsBiMensuel && m_NbCol <= MaxInsHebdo)
		// 1 colonne supplémentaire pour les bi-mensuel
		m_NbCol = 14;
	else if (m_NbCol > MaxInsHebdo && m_NbCol < MaxInsBiHebdo)
		// 1 colonne supplémentaire pour les hebdomadaires
		m_NbCol = 15;
	else if (m_NbCol >= MaxInsBiHebdo && m_NbCol <= MaxInsQuotidien)
		// 1 colonne supplémentaire pour les bi-hebdo
		m_NbCol = 16;
	else if (m_NbCol > MaxInsQuotidien)
		// 1 colonne supllémentaire pour les quotidiens
		m_NbCol = 17;

	// Init grid avec nb colonnes
    if (!m_GridCumulativite.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Lignes
bool CDlgCumulativite::UpdateRow()
{
	// Nb Header Lignes (1 au total)
	m_NbHeaderRow = 2;

	// Nombre de lignes
    if (!m_GridCumulativite.SetRowCount(m_NbHeaderRow + m_NbRowSupport-1,GX_UPDATENOW))
		return false;

	// Lignes headers 
	m_GridCumulativite.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Remplissage des entetes header rows
	if (m_NbCol >= 1)
	{
		// Ligne header niveau 0 réservé à la grandeur sélectionnée
		// Centrage et style texte header
		m_GridCumulativite.SetStyleRange(CGXRange(0, 0, 0, m_GridCumulativite.GetColCount()),
										 CGXStyle()
											.SetHorizontalAlignment(DT_CENTER)
											.SetVerticalAlignment(DT_VCENTER)
											.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL))
											.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontCell)
														.SetBold(pDocChoixSupport->m_BoldFontCell)));


		// Texte associé au header ligne
		CString Txt = "";
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		{
			JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem();
			Txt = pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, IdGrandeur);
		}

		// Affichage Grandeur
		m_GridCumulativite.SetValueRange(CGXRange(0, m_NoColPeriodicite, 0, m_GridCumulativite.GetColCount()),Txt); 

		// Affichage des headers infos insertions selon périodicité
		for (int NoLig = 1; NoLig < m_NbHeaderRow ; NoLig++)
		{
			// Pour chaque colonne
			JUnt32 NbCol = m_GridCumulativite.GetColCount();

			for (int NoCol = 0; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Centrage et style texte header
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
												 CGXStyle()
													.SetHorizontalAlignment(DT_CENTER)
													.SetVerticalAlignment(DT_BOTTOM)
													.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL))
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontCell)
														.SetBold(pDocChoixSupport->m_BoldFontCell)));

				// Texte associé au header la cellule
				CString Txt;

				if (NoCol >= m_NbHeaderCol)
				{
					if (NoCol == m_NbHeaderCol)
					{
						// Entete header Asymptote ou Max 
						Txt.LoadString(IDS_CS_A_UN_AN); 
						// Txt = "A un an";
					}
					else
					{
						if (NoCol - m_NbHeaderCol > 1)
						{
							if (NoCol - m_NbHeaderCol == 13)
								// Les colonnes Nb insertions pour plafond bi-mensuel
								Txt.Format("%d %s",MaxInsBiMensuel,"insert.");
							else if (NoCol - m_NbHeaderCol == 14)
								// Les colonnes Nb insertions pour plafond hebdomadaires
								Txt.Format("%d %s",MaxInsHebdo,"insert.");
							else if (NoCol - m_NbHeaderCol == 15)
								// Les colonnes Nb insertions pour plafond bi-hebdomadaires
								Txt.Format("%d %s",MaxInsBiHebdo,"insert.");
							else if (NoCol - m_NbHeaderCol == 16)
								// Les colonnes Nb insertions pour plafond quotidiens
								Txt.Format("%d %s",MaxInsQuotidien,"insert.");
							else
								// Les autres colonnes
								Txt.Format("%d %s",NoCol - m_NbHeaderCol,"inserts");
						}	
						else
							// La colonnes 1 insertion
							Txt.Format("%d %s",NoCol - m_NbHeaderCol,"insert");
					}

					// Affichage contenu cellule
					m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
				}

			}
		}

		// Couleurs spéciales
		UpdateColorSpe();

		// Regroupement des lignes header row
		m_GridCumulativite.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
		m_GridCumulativite.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridCumulativite.GetColCount()),
									     CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));

		// Update Grille modifiée
		m_GridCumulativite.UpdateData(TRUE);

		// Regroupement cellule
		/*
		m_GridCumulativite.UpdateData(); 
		m_GridCumulativite.SetMergeCellsMode(gxnMergeEvalOnDisplay);
		m_GridCumulativite.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridCumulativite.GetColCount()),
							 			 CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));
		*/

		// Mise à jour
		m_GridCumulativite.UpdateWindow(); 
		m_GridCumulativite.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update des résultats au niveau cellule
bool CDlgCumulativite::UpdateResultat(JBool CalculResult)
{
	if (m_GridCumulativite.GetColCount() > 0 && m_GridCumulativite.GetRowCount() > 0)
	{
		// Delock mode read only
		m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

		// Init Affichage Résultats Cellules
		int NbCol = m_GridCumulativite.GetColCount();
		m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridCumulativite.GetRowCount(),m_GridCumulativite.GetColCount()),
								CGXStyle()
									.SetHorizontalAlignment(DT_RIGHT)
									.SetVerticalAlignment(DT_BOTTOM)
									.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

		// Test si grille résultat valide
		if (GrilleResultValid())
		{
			// Si calcul demandé et modif paramétre
			if (CalculResult && pDocChoixSupport->m_ParamCumModif)
			{
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					if (!InitGridResultat_OPTIM()) return false;
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgré l'incohérence calcul en Betabinomial (voir Sylvain)
					if (!InitGridResultat()) return false;
				}
			}
			else
			{
				// Affichage des résultats cellules
				if (!AfficheResultat()) 
					return false;
			}
		}		

		// On bloque les colonnes qui doivent ne pas être modifiées
		m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélection supports
void CDlgCumulativite::UpdateEtatSelSupports()
{
	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat sélection en cours
			CGXStyle Style;
			m_GridCumulativite.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 

			// Puis indication support sélectionné ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la sélection support
				if (ValSel != 1)
				{
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
												     CGXStyle()
														.SetValue((long)1));
				}
			}
			else
			{
				// Dévalide la sélection support
				if (ValSel != 0)
				{
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
												     CGXStyle()
														.SetValue((long)0));
				}
			}
		}
	}	

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgCumulativite::UpdateVisuSupports()
{
	// Attente mise à jour
	m_GridCumulativite.LockUpdate(true);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Debut et fin marquage sélection
	JUnt32 NoDebCol = m_NoColSupport-1;
	JUnt32 NoFinCol = m_GridCumulativite.GetColCount();

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Puis indication support sélectionné ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Support sélectionné, on surligne la ligne
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig, NoDebCol, NoLig, NoFinCol),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
			}
			else
			{
				// Support non sélectionné, on laisse la ligne en blanc
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig, NoDebCol, NoLig, NoFinCol),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
			}
		}
	}

	// Mise à jour couleur supports pour graphe
	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdColorSupports.MoveTo(IdSupport);

		JUnt32 IndexColor = 0;
		if (pDocChoixSupport->m_MapIdColorSupports.IsValid()) 
		{
			// Voir si support appartient au graphe et indiquer sa couleur
			IndexColor = pDocChoixSupport->m_MapIdColorSupports.GetItem();
		}

		if (IndexColor > 0)
		{
			// Récupère la couleur
			m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1, NoLig, m_NoColColorGraphe-1),
  											CGXStyle()
												.SetValue((long)IndexColor)
												.SetEnabled(false)
												.SetInterior(CATPColors::GetColorGrph(IndexColor-1))
												.SetTextColor(CATPColors::GetColorGrph(IndexColor-1)));
		}
		else
			m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1, NoLig, m_NoColColorGraphe-1),
  											CGXStyle()
											.SetValue("")
											.SetEnabled(false)
											.SetInterior(CATPColors::GetColorWhite()));
	}

	// Mise à jour graphe cumulativité
	UpdateGraphe();
	
	////////////////////////////// Remet les indicateurs maximum atteint ////////////////////////////////
	//	
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne dans le map
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Récupére état sélection support
			bool EtatSel = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

			// Maximum insertions possible sur ce support
			JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n° de colonne
				JUnt32 NbInsertions;
				NbInsertions = (NoCol - m_NbHeaderCol);

				switch (NoCol - m_NbHeaderCol)
				{
					case 0   :
						// Asymptote atteinte
						NbInsertions = MaxInsSupport;
						break;
					case 13  :
						// Pour les bi-mensuels
						NbInsertions = 24;
						break;
					case 14  :
						// Pour les hebdomadaires
						NbInsertions = 53;
						break;
					case 15  :
						// Pour les bi-hebdomadaires
						NbInsertions = 105;
						break;
					case 16 :
						// Pour les quotidiens
						NbInsertions = 365;
						break;
				}

				// Indication maximum atteint
				if (NoCol == m_NbHeaderCol && EtatSel == false)
					// Couleur spéciale pour indiquer max cumulativité
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
														.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
				else
				{
					// Seuil maxi non atteint
					if (EtatSel)
						// Indique couleur cumulativité progressive avec support sélectionné
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetEnabled(false)
															.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
					else
						// Indique couleur cumulativité progressive avec support non sélectionné
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetEnabled(false)
															.SetInterior(CATPColors::GetColorWhite()));

				}

				// Affichage des résultats suivants directement si limite insertion atteinte
				if (NbInsertions >= MaxInsSupport && NoCol != m_NbHeaderCol )
				{
					// Indique en couleur limite atteinte pour calcul cumulativité
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,NoCol),
														CGXStyle()
														.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
				}
			}
		}
	}
	
	// Voir pour les supports cachés
	UpdateAfficheSupports();

	// Attente mise à jour
	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// On bloque les colonnes qui ne doivent pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update couleur graphe associé aux supports
void CDlgCumulativite::UpdateColorGraphSupports()
{
	// Attente mise à jour
	m_GridCumulativite.LockUpdate(true);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Mise à jour couleur supports pour graphe
	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdColorSupports.MoveTo(IdSupport);

		JUnt32 IndexColor = 0;
		if (pDocChoixSupport->m_MapIdColorSupports.IsValid()) 
		{
			// Voir si support appartient au graphe et indiquer sa couleur
			IndexColor = pDocChoixSupport->m_MapIdColorSupports.GetItem();
		}

		if (IndexColor > 0)
		{
			// Récupère la couleur
			m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1, NoLig, m_NoColColorGraphe-1),
  											CGXStyle()
												.SetValue((long)IndexColor)
												.SetEnabled(false)
												.SetTextColor(CATPColors::GetColorGrph(IndexColor-1))
												.SetInterior(CATPColors::GetColorGrph(IndexColor-1)));
		}
		else
			m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1, NoLig, m_NoColColorGraphe-1),
  											CGXStyle()
											.SetValue("")
											.SetEnabled(false)
											.SetInterior(CATPColors::GetColorWhite()));
	}


	// Mise à jour graphe cumulativité
	UpdateGraphe();
	
	// Attente mise à jour
	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update couleur courante pour graphe cumulativité
void CDlgCumulativite::UpdateVisuColorGraphe()
{
	// Bloque modif grid
	m_GridCumulativite.LockUpdate(true); 

	// Nombre de lignes
    JUnt32 NoLig = 0;
	if (m_GridCumulativite.GetRowCount()) NoLig = m_NbHeaderRow - 1;

	if (pDocChoixSupport->m_IdColorCum > 0)
	{
		// Information couleur courante en entete colonne sélection
		m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1,NoLig ,m_NoColColorGraphe-1),
										 CGXStyle()
											.SetEnabled(false)
											.SetTextColor(CATPColors::GetColorGrph(pDocChoixSupport->m_IdColorCum-1))
											.SetInterior(CATPColors::GetColorGrph(pDocChoixSupport->m_IdColorCum-1)));
	}
	else
	{
		// Remet la couleur d'origine / standard header
		m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColColorGraphe-1,NoLig, m_NoColColorGraphe-1),
										 CGXStyle()
											.SetEnabled(false)
									        .SetInterior(CATPColors::GetColorWhite()));
	}

	// Redessine le grid
	m_GridCumulativite.LockUpdate(false); 
	m_GridCumulativite.Redraw(GX_UPDATENOW); 
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update du graphe
void CDlgCumulativite::UpdateGraphe()
{
	// Update interface graphique
	m_IGraphCourbeCum.Update();

	// Mise à jour tracé du graphe
	m_GraphCum.SetItems(&m_IGraphCourbeCum,TRUE); 

}

////////////////////////////////////////////////////////////////////////////////////////////
// Maximum insertions sur les supports traités
JUnt32 CDlgCumulativite::MaxInsertionsAllSupports()
{
	// Max insertions
	JUnt32 MaxIns = 0;

	// Boucle sur tous les supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// Pris en compte max insertions
		if (MaxInsSupport > MaxIns)
			MaxIns = MaxInsSupport;
	}

	return MaxIns;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du résultat cumulativité
JBool CDlgCumulativite::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridCumulativite.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultCum KeyResultCum;

	// Information résultat archivé
	CResultCum	ResultCum;

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Identificateur format courant
	JUnt32 IdFormat;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    
	else
		return false;

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridCumulativite.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridCumulativite.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultCum.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultCum.GetRange( nLower, nUpper);

	// Init Calcul
	m_StopCalcul = false;

	// Calcul indice clé segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice clé moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Max Insertions sur tous les supports
	// JUnt32 MaxInsAllSupp = MaxInsertionsAllSupports();
	JUnt32 MaxInsAllSupp = pDocChoixSupport->m_NbInsertionsCum;

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Test si on continue la suite des calculs cumulativité
		if (m_StopCalcul)
			break;
		
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récupère le format support sélectionné pour la cumulativité
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();
	
		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			JUnt32 Duree;
			JDate DateExact;

			/*
			if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
				Tarif = 0;
			*/
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);	
		}

		// Ecriture Périodicité
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace à gauche pour présentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Insére dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain sélectionné
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain sélectionné
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible sélectionnée
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible sélectionnée
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// Clé de recherche résultat
		KeyResultCum.m_IdCible			= IdCible; 
		KeyResultCum.m_IdTerrain		= IdTerrain;
		KeyResultCum.m_IdSupport		= IdSupport; 
		KeyResultCum.m_NbInsert         = 1;

		// Test si élément déjà calculé
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
		if (!pDocChoixSupport->m_MapResultatCum.IsValid())
		{
			// ----------------------------------- Calcul à 1 insertion ------------------------------
			// Mise en place des infos offres
			KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,IdCible);

			// Construction de la maille avec n insertions
			JFCMaille Maille1Ins;
			pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maille1Ins,IdSupport,1); 

			// Calcul audience et volume lecture 1 insertion
			JFlt64 VolLect1Ins		= -1.0;
			JFlt64 AudProba1Ins		= -1.0;
			JFlt64 sertarien		= -1.0;
			JInt32 PtEtal			= -1.0;

			// Audience
			JFlt64 Audience			= -1.0;

			// Flag Audience Défini
			JBool AudienceDefini = false;
			
			// Test si la cible est autorisée
			if (pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain) == true)
			{
				AudienceDefini = pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupport, VolLect1Ins, sertarien, AudProba1Ins, PtEtal, true);
			}
			else
			{
				JBool NonOk = false;
			}

			// Init Moteur (construction distribution)
			JFlt64 Audience1Ins = -1;

			if (AudienceDefini)
			{
				if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille1Ins))
				{	
					// Création Distribution
					JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

					// Récupère audience
					Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  

					// *** Bidouille en attendant de résoudre le problème des LNMs non définis
					// et qui pourtant renvoie une distribution de fréquence = 0
					if (AudProba1Ins < 0) Audience1Ins = AudProba1Ins;
				}
			}

			// Balayage des colonnes insertions
			// Attention 13 > correspond à  24 insertions	(pour les bi-mensuels)
			//           14 > ............  53 ..........	(pour les hebdomadaires)
			//           15 > ............ 105 ..........	(pour les bi-hebdomadaires)
			//           16 > ............ 365 ..........	(pour les quotidiens)
			JUnt32 NbCol = m_GridCumulativite.GetColCount();

			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n° de colonne
				JUnt32 NbInsertions;
				NbInsertions = (NoCol - m_NbHeaderCol);
				switch (NoCol - m_NbHeaderCol)
				{
					case 0   :
						// Asymptote atteinte
						NbInsertions = MaxInsSupport;
						break;
					case 13  :
						// Pour les bi-mensuels
						NbInsertions = 24;
						break;
					case 14  :
						// Pour les hebdomadaires
						NbInsertions = 53;
						break;
					case 15  :
						// Pour les bi-hebdomadaires
						NbInsertions = 105;
						break;
					case 16 :
						// Pour les quotidiens
						NbInsertions = 365;
						break;
				}

				// Calcul si nb insertions ne dépasse pas le maximum autorisé
				if (MaxInsSupport > MaxInsAllSupp)
					MaxInsSupport = MaxInsAllSupp;

				if (NbInsertions <= MaxInsSupport)
				{
					// Calcul audience et volume lecture
					JFlt64 VolLecture	  = VolLect1Ins * NbInsertions ;
					JFlt64 AudienceProba  = AudProba1Ins * NbInsertions ;

					// Audience
					JFlt64 Audience			= -1.0;

					if (AudienceDefini)
					{
						// On fabrique la maille à n insertions
						JFCMaille Maillen;
						pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maillen,IdSupport,NbInsertions);  // false; 

						// Construction de la distribution sur ces n insertions
						// A REMETTRE // A OPTIMISER = TRES LONG !!!!!!!!!!!!!!!!!!!!!
						pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maillen);
						//.................................................

						// Direct on prend la distri à n insertions
						JFCDistribution Distri = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLecture); 

						// Récupère audience
						Audience = Distri.Item(1).AsJFlt64();  

						// *** Bidouille en attendant de résoudre le problème des LNMs non définis
						// et qui pourtant renvoie une distribution de fréquence = 0
						if (AudProba1Ins < 0) Audience = AudProba1Ins;
					}

					// Récup id support
					CRegroupements * pReg = CRegroupements::GetInstance();
					IElem *pElem = pReg->GetIElemByID(IdSupport);

					// Force Audience LDP si 1 seule insertion
					if (NbInsertions == 1)
					{
						// Test si c'est un couplage
						if(!pElem->IsCouplage())
							// Audience devient Audience LDP
							Audience = AudienceProba;    // AudienceProba = en fait c'est audience proba 1 insertion
					}

					// Effectif et nb cas
					JFlt64 Effectif;
					JInt32 NbCas;
					JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

					// Calcul Résultat Colonne (selon grandeur)
					ResultCum.m_Audience			= Audience; 
					ResultCum.m_AudProba1Ins		= AudProba1Ins;
					if(!pElem->IsCouplage())
						// Cas de titre simple, audience 1 insertion = audience LDP
						ResultCum.m_Aud1Ins			= AudProba1Ins;
					else
						// Cas de couplage, on prend audience 1 insertion (calculé à partir de Distri(1)
						ResultCum.m_Aud1Ins		= Audience1Ins;
					ResultCum.m_Effectif			= Effectif;
					ResultCum.m_NbCas				= NbCas;
					ResultCum.m_VolLecture			= VolLecture;
					ResultCum.m_PtEtalonnage		= PtEtal;

					CString Txt;
					if(!pElem->IsCouplage())
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);
					else
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);

					/*
					CString Txt;
					if(!pElem->IsCouplage())
						Txt = ResultCellule(NoLig, NoCol, Tarif, VolLecture, Audience, AudProba1Ins, AudProba1Ins, Effectif, NbCas, NbInsertions);
					else
						Txt = ResultCellule(NoLig, NoCol, Tarif, VolLecture, Audience, AudProba1Ins, Audience1Ins, Effectif, NbCas, NbInsertions);

					/////////////////////////////////////////
					// Stockage résultat dans matrice
					KeyResultCum.m_IdCible			= IdCible; 
					KeyResultCum.m_IdTerrain		= IdTerrain;
					KeyResultCum.m_IdSupport		= IdSupport; 
					KeyResultCum.m_NbInsert		    = NbInsertions;

					ResultCum.m_Audience			= Audience; 
					ResultCum.m_AudProba1Ins		= AudProba1Ins;
					if(!pElem->IsCouplage())
						// Cas de titre simple, audience 1 insertion = audience LDP
						ResultCum.m_Aud1Ins			= AudProba1Ins;
					else
						// Cas de couplage, on prend audience 1 insertion (calculé à partir de Distri(1)
						ResultCum.m_Aud1Ins		= Audience1Ins;
					
					ResultCum.m_Effectif			= Effectif;
					ResultCum.m_NbCas				= NbCas;
					ResultCum.m_VolLecture			= VolLecture;
					ResultCum.m_PtEtalonnage		= PtEtal;
					*/

					// Ajoute nouveau résultats
					pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
					if (!pDocChoixSupport->m_MapResultatCum.IsValid())
					{
						pDocChoixSupport->m_MapResultatCum.Add(KeyResultCum) = ResultCum; 
					}

					// Sauvegarde valeur maximal
					CResultCum	ResultCumMaxInsert;
					if (NbInsertions == MaxInsSupport)
						ResultCumMaxInsert = ResultCum;

					// Indication maximum atteint
					if (NoCol == m_NbHeaderCol)
						// Couleur spéciale pour indiquer max cumulativité
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
						// Indique couleur cumulativité progressive
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetInterior(CATPColors::GetColorWhite()));

					// Affichage des résultats suivants directement si limite insertion atteinte
					if (NbInsertions == MaxInsSupport && NoCol != m_NbHeaderCol )
					{
						// Ecriture dans toutes les cellules suivantes (même support)
						for (int i = NoCol + 1; i <= m_GridCumulativite.GetColCount(); i++) 
						{
							// Même valeur qu'avec max insertion
							Txt = ResultCellule(NoLig, i, Tarif, NbInsertions, ResultCumMaxInsert);
							/*
							Txt = ResultCellule(NoLig, i, Tarif, 
												ResultCumMaxInsert.m_VolLecture, 
												ResultCumMaxInsert.m_Audience, 
												ResultCumMaxInsert.m_AudProba1Ins, 
												ResultCumMaxInsert.m_Aud1Ins,
												ResultCumMaxInsert.m_Effectif,
												ResultCumMaxInsert.m_NbCas,
												NbInsertions);
							*/
						}

						// Indique en couleur limite atteinte pour calcul cumulativité
						if (NoCol+1 <= m_GridCumulativite.GetColCount())
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol+1,NoLig,m_GridCumulativite.GetColCount()),
															CGXStyle()
																.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					}
				}
			}
		}

		else
		{
			// Balayage des colonnes insertions
			// Attention 13 > correspond à  24 insertions
			//           14 > ............  53 ..........
			//           15 > ............ 105 ..........
			//           16 > ............ 365 ..........
			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n° de colonne
				JUnt32 NbInsertions;
				NbInsertions = (NoCol - m_NbHeaderCol);
				switch (NoCol - m_NbHeaderCol)
				{
					case 0   :
						// Asymptote atteinte
						NbInsertions = MaxInsSupport;
						break;
					case 13  :
						// Pour les bi-mensuels
						NbInsertions = 24;
						break;
					case 14  :
						// Pour les hebdomadaires
						NbInsertions = 53;
						break;
					case 15  :
						// Pour les bi-hebdomadaires
						NbInsertions = 105;
						break;
					case 16 :
						// Pour les quotidiens
						NbInsertions = 365;
						break;
				}

				// Calcul si nb insertions ne dépasse pas le maximum autorisé
				if (NbInsertions <= MaxInsSupport)
				{
					// Récupère résultat
					KeyResultCum.m_NbInsert         = NbInsertions;
					pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

					if (pDocChoixSupport->m_MapResultatCum.IsValid())
					{
						ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 

						// Calcul Résultat Colonne (selon grandeur)
						CString Txt;
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);
						
						/*
						CString Txt;
						Txt = ResultCellule(NoLig, NoCol, Tarif, 
											ResultCum.m_VolLecture,
											ResultCum.m_Audience,
											ResultCum.m_AudProba1Ins,
											ResultCum.m_Aud1Ins,
											ResultCum.m_Effectif,
											ResultCum.m_NbCas,
											NbInsertions);
						*/

						// Sauvegarde valeur maximal
						CResultCum	ResultCumMaxInsert;
						if (NbInsertions == MaxInsSupport)
							ResultCumMaxInsert = ResultCum;

						// Indication maximum atteint
						if (NoCol == m_NbHeaderCol)
							// Couleur spéciale pour indiquer max cumulativité
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
															CGXStyle()
																.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
						else
							// Indique couleur cumulativité progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));

						// Affichage des résultats suivants directement si limite insertion atteinte
						if (NbInsertions == MaxInsSupport && NoCol != m_NbHeaderCol )
						{
							// Ecriture dans toutes les cellules suivantes (même support)
							for (int i = NoCol + 1; i <= m_GridCumulativite.GetColCount(); i++) 
							{
								// Même valeur qu'avec max insertion
								Txt = ResultCellule(NoLig, i, Tarif, NbInsertions, ResultCumMaxInsert);
								/*
								Txt = ResultCellule(NoLig, i, Tarif, 
													ResultCumMaxInsert.m_VolLecture, 
													ResultCumMaxInsert.m_Audience, 
													ResultCumMaxInsert.m_AudProba1Ins, 
													ResultCumMaxInsert.m_Aud1Ins, 
													ResultCumMaxInsert.m_Effectif,
													ResultCumMaxInsert.m_NbCas,
													NbInsertions);
								*/
							}

							// Indique en couleur limite atteinte pour calcul cumulativité
							if (NoCol+1 <= m_GridCumulativite.GetColCount())
								m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol+1,NoLig,m_GridCumulativite.GetColCount()),
																CGXStyle()
																	.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
						}
					}
				}
			}
		}
	}

	// Progress bar invisible
	m_ProgressResultCum.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                      m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										      m_GridCumulativite.GetColCount()-1),
									 CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/format/périodicité
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColSupport-1),
		                                    TRUE,GX_UPDATENOW);

	/*
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColFormat-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);
	*/
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColPeriodicite-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Flag calcul effectué
	pDocChoixSupport->m_ParamCumModif = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du résultat cumulativité
JBool CDlgCumulativite::InitGridResultat_OPTIM_OLD()
{
	// Remette code avant 24 Decembre 2005
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Vérifie si tous les calculs effectuées
JBool CDlgCumulativite::AuMoins1ResultatNonCalcule(CKeyResultCum KeyResultCum, JUnt32 MaxInsSupport)
{
	// Resultat non calculé
	bool ResultatNonCalcule = false; 

	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
	{
		// Nombre d'insertions via n° de colonne
		JUnt32 NbInsertions;
		NbInsertions = (NoCol - m_NbHeaderCol);
		switch (NoCol - m_NbHeaderCol)
		{
			case 0   :
				// Asymptote atteinte
				NbInsertions = MaxInsSupport;
				break;
			case 13  :
				// Pour les bi-mensuels
				NbInsertions = 24;
				break;
			case 14  :
				// Pour les hebdomadaires
				NbInsertions = 53;
				break;
			case 15  :
				// Pour les bi-hebdomadaires
				NbInsertions = 105;
				break;

			case 16 :
				// Pour les quotidiens
				NbInsertions = 365;
				break;
		}

		KeyResultCum.m_NbInsert   = NbInsertions;

		// Test si élément déjà calculé
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

		// Validation des résultats précédents
		if (!pDocChoixSupport->m_MapResultatCum.IsValid())
		{
			ResultatNonCalcule = true;
			break;
		}
	}
	
	return ResultatNonCalcule;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du résultat cumulativité
JBool CDlgCumulativite::InitGridResultat_OPTIM()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridCumulativite.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultCum KeyResultCum;
	CKeyResultCum KeyResultCumMaxIns;

	// Information résultat archivé
	CResultCum	ResultCum;

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Identificateur format courant
	JUnt32 IdFormat;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    
	else
		return false;

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridCumulativite.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridCumulativite.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultCum.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultCum.GetRange( nLower, nUpper);

	// Init Calcul
	m_StopCalcul = false;

	// Calcul indice clé segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice clé moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	JUnt32 NbCol = m_GridCumulativite.GetColCount();

	// Max Insertions sur tous les supports (en tenant compte aussi du choix menu)
	JUnt32 MaxInsAllSupp = MaxInsertionsAllSupports();

	if (pDocChoixSupport->m_NbInsertionsCum > MaxInsAllSupp)
		MaxInsAllSupp = pDocChoixSupport->m_NbInsertionsCum;

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Test si on continue la suite des calculs cumulativité
		if (m_StopCalcul)
			break;
		
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récupère le format support sélectionné pour la cumulativité
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();
	
		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);	
		}

		// Ecriture Périodicité
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace à gauche pour présentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Insére dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain sélectionné
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain sélectionné
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible sélectionnée
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible sélectionnée
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// Clé de recherche résultat (la 1ere correspondant à 1 insertion)
		KeyResultCum.m_IdCible			= IdCible; 
		KeyResultCum.m_IdTerrain		= IdTerrain;
		KeyResultCum.m_IdSupport		= IdSupport; 
		
		// Test si au moins 1 résultt non calculé
		bool AllNonValid = false;
		AllNonValid =AuMoins1ResultatNonCalcule(KeyResultCum, MaxInsSupport);

		// Puis à 1 insertion
		KeyResultCum.m_NbInsert         = 1;

		// La dernière correpondant au max insertion via menu insertion
		KeyResultCumMaxIns				= KeyResultCum; 
		KeyResultCumMaxIns.m_NbInsert   = pDocChoixSupport->m_NbInsertionsCum;

		// Test si élément déjà calculé
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

		// Validation des résultats précédents
		bool Valid1Ins = pDocChoixSupport->m_MapResultatCum.IsValid();

		// Test si élément déjà calculé
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCumMaxIns); 

		// Validation des résultats précédents
		bool ValidMaxIns = pDocChoixSupport->m_MapResultatCum.IsValid();

		if (!Valid1Ins || !ValidMaxIns || AllNonValid)
		{
			// ----------------------------------- Calcul à 1 insertion ------------------------------
			// Mise en place des infos offres
			KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,IdCible);

			// Construction de la maille avec 1 insertions
			JFCMaille Maille1Ins;
			pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maille1Ins,IdSupport,1); 

			// Calcul audience et volume lecture 1 insertion
			JFlt64 VolLect1Ins		= -1.0;
			JFlt64 AudProba1Ins		= -1.0;
			JFlt64 sertarien		= -1.0;
			JInt32 PtEtal			= -1.0;

			// Audience
			JFlt64 Audience			= -1.0;

			// Flag Audience Défini
			JBool AudienceDefini = false;

			// Test si la cible est autorisée
			if (pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain) == true)
			{
				AudienceDefini = pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupport, VolLect1Ins, sertarien, AudProba1Ins, PtEtal, true);
			}
			else
			{
				JBool NonOk = true;
			}

			// Init Moteur (construction distribution)
			JFlt64 Audience1Ins = -1;

			if (AudienceDefini)
			{
				if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille1Ins))
				{	
					// Création Distribution
					if (VolLect1Ins >= 0)
					{
						JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

						// Récupère audience
						Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  
					}

					// *** Bidouille en attendant de résoudre le problème des LNMs non définis
					// et qui pourtant renvoie une distribution de fréquence = 0
					if (AudProba1Ins < 0) Audience1Ins = AudProba1Ins;
				}

			}

			// Construction de la distribution sur ces n insertions
			// pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur,Maille1Ins,MaxInsAllSupp);
			// OPTIM 2.7
			// pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur, Maille1Ins, MaxInsAllSupp);
			pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur, Maille1Ins, MaxInsSupport);

			// Calcul distribution en prenant à chaque fois 1 insertion, puis 2 insertions, puis 3 insertions etc... 
			JArray <JFCDistribution> TDistri_NIns;
			pDocChoixSupport->m_MoteurPresse.GetTabDistribution(pDocChoixSupport->m_MoteurPresse,MaxInsAllSupp, TDistri_NIns, MaxInsSupport); 
			// pDocChoixSupport->m_MoteurPresse.GetTabDistribution(pDocChoixSupport->m_MoteurPresse,MaxInsSupport, TDistri_NIns); 

			// Récupère audience
			Audience = TDistri_NIns.Item(1).Item(1).AsJFlt64();  

			// *** Bidouille en attendant de résoudre le problème des LNMs non définis
			// et qui pourtant renvoie une distribution de fréquence = 0
			if (AudProba1Ins < 0) Audience = AudProba1Ins;

			// Balayage des colonnes insertions
			// Attention 13 > correspond à  24 insertions	(pour les bi-mensuels)
			//           14 > ............  53 ..........	(pour les hebdomadaires)
			//           15 > ............ 105 ..........	(pour les bi-hebdomadaires)
			//           16 > ............ 365 ..........	(pour les quotidiens)
			JUnt32 NbCol = m_GridCumulativite.GetColCount();

			// Récup id support
			CRegroupements * pReg = CRegroupements::GetInstance();
			IElem *pElem = pReg->GetIElemByID(IdSupport);

			if (pElem)
			{
				JUnt32 NbColEff = m_GridCumulativite.GetColCount();

				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
				{
					// Nombre d'insertions via n° de colonne
					JUnt32 NbInsertions;
					NbInsertions = (NoCol - m_NbHeaderCol);
					switch (NoCol - m_NbHeaderCol)
					{
						case 0   :
							// Asymptote atteinte
							NbInsertions = MaxInsSupport;
							break;
						case 13  :
							// Pour les bi-mensuels
							NbInsertions = 24;
							break;
						case 14  :
							// Pour les hebdomadaires
							NbInsertions = 53;
							break;
						case 15  :
							// Pour les bi-hebdomadaires
							NbInsertions = 105;
							break;

						case 16 :
							// Pour les quotidiens
							NbInsertions = 365;
							break;
					}

					// Appel résultat avec nb insertions (dans le cas où déjà calculé)
					KeyResultCum.m_NbInsert         = NbInsertions;
					
					// Test si élément déjà calculé
					pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
					if (!pDocChoixSupport->m_MapResultatCum.IsValid())
					{
						// Sauf les quotidiens
						if (NbInsertions != 366)
						{
							// Calcul audience et volume lecture
							JFlt64 VolLecture	  = VolLect1Ins * NbInsertions ;
							JFlt64 AudienceProba  = AudProba1Ins * NbInsertions ;

							// Audience
							JFlt64 Audience			= -1.0;

							if (AudienceDefini)
							{
								// Récupère audience 1 constact et + pour nb insertions
								Audience = TDistri_NIns.Item(NbInsertions-1).Item(1).AsJFlt64();  

								// *** Bidouille en attendant de résoudre le problème des LNMs non définis
								// et qui pourtant renvoie une distribution de fréquence = 0
								if (AudProba1Ins < 0) Audience = AudProba1Ins;
							}

							// Force Audience LDP si 1 seule insertion
							if (NbInsertions == 1)
							{
								// Test si c'est un couplage
								if(!pElem->IsCouplage())
									// Audience devient Audience LDP
									Audience = AudienceProba;    // AudienceProba = en fait c'est audience proba 1 insertion
							}

							// Effectif et nb cas
							JFlt64 Effectif;
							JInt32 NbCas;
							JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

							/////////////////////////////////////////
							// Stockage résultat dans matrice
							KeyResultCum.m_IdCible			= IdCible; 
							KeyResultCum.m_IdTerrain		= IdTerrain;
							KeyResultCum.m_IdSupport		= IdSupport; 
							KeyResultCum.m_NbInsert		    = NbInsertions;
							ResultCum.m_Audience			= Audience; 
							ResultCum.m_AudProba1Ins		= AudProba1Ins;

							if(!pElem->IsCouplage())
								// Cas de titre simple, audience 1 insertion = audience LDP
								ResultCum.m_Aud1Ins			= AudProba1Ins;
							else
								// Cas de couplage, on prend audience 1 insertion (calculé à partir de Distri(1)
								ResultCum.m_Aud1Ins		= Audience1Ins;
							
							ResultCum.m_Effectif			= Effectif;
							ResultCum.m_NbCas				= NbCas;
							ResultCum.m_VolLecture			= VolLecture;
							ResultCum.m_PtEtalonnage		= PtEtal;
						}
						else
						{
							// Si quotidien on calculera tous les pas de 365 jours à 22 jours avec pas de 10
							JUnt32 NbInsertionCur = NbInsertions;
							for (JUnt32 NbInsertionCur = NbInsertions; NbInsertionCur > 12; NbInsertionCur -= 10)
							{
								// Calcul audience et volume lecture
								JFlt64 VolLecture	  = VolLect1Ins * NbInsertionCur ;
								JFlt64 AudienceProba  = AudProba1Ins * NbInsertionCur ;

								// Audience
								JFlt64 Audience			= -1.0;

								if (AudienceDefini)
								{
									// Récupère audience 1 constact et + pour nb insertions
									Audience = TDistri_NIns.Item(NbInsertionCur-1).Item(1).AsJFlt64();  

									// *** Bidouille en attendant de résoudre le problème des LNMs non définis
									// et qui pourtant renvoie une distribution de fréquence = 0
									if (AudProba1Ins < 0) Audience = AudProba1Ins;
								}

								// Récup id support

								// Force Audience LDP si 1 seule insertion
								if (NbInsertionCur == 1)
								{
									// Test si c'est un couplage
									if(!pElem->IsCouplage())
										// Audience devient Audience LDP
										Audience = AudienceProba;    // AudienceProba = en fait c'est audience proba 1 insertion
								}

								// Effectif et nb cas
								JFlt64 Effectif;
								JInt32 NbCas;
								JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

								/////////////////////////////////////////
								// Stockage résultat dans matrice
								KeyResultCum.m_IdCible			= IdCible; 
								KeyResultCum.m_IdTerrain		= IdTerrain;
								KeyResultCum.m_IdSupport		= IdSupport; 
								KeyResultCum.m_NbInsert		    = NbInsertionCur;
								ResultCum.m_Audience			= Audience; 
								ResultCum.m_AudProba1Ins		= AudProba1Ins;

								if(!pElem->IsCouplage())
									// Cas de titre simple, audience 1 insertion = audience LDP
									ResultCum.m_Aud1Ins			= AudProba1Ins;
								else
									// Cas de couplage, on prend audience 1 insertion (calculé à partir de Distri(1)
									ResultCum.m_Aud1Ins		= Audience1Ins;
								
								ResultCum.m_Effectif			= Effectif;
								ResultCum.m_NbCas				= NbCas;
								ResultCum.m_VolLecture			= VolLecture;
								ResultCum.m_PtEtalonnage		= PtEtal;

								// Ajoute nouveau résultats
								pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
								if (!pDocChoixSupport->m_MapResultatCum.IsValid())
								{
									pDocChoixSupport->m_MapResultatCum.Add(KeyResultCum) = ResultCum; 
								}
							}
						}
					}
					else
					{
						// Récupération Résultat déjà existant
						ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 
					}

					// Calcul Résultat Colonne (selon grandeur)
					CString Txt;
					if(!pElem->IsCouplage())
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);
					else
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);

					// Ajoute nouveau résultats
					pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
					if (!pDocChoixSupport->m_MapResultatCum.IsValid())
					{
						pDocChoixSupport->m_MapResultatCum.Add(KeyResultCum) = ResultCum; 
					}

					// Sauvegarde valeur maximal
					CResultCum	ResultCumMaxInsert;
					if (NbInsertions == MaxInsSupport)
						ResultCumMaxInsert = ResultCum;

					// Indication maximum atteint
					if (NoCol == m_NbHeaderCol)
						// Couleur spéciale pour indiquer max cumulativité
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
					{
						if (NbInsertions < MaxInsSupport)
						{
							// Indique couleur cumulativité progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));
						}
						else
						{
							// Indique en couleur limite atteinte pour calcul cumulativité
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()),
																CGXStyle()
 																.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));

							if (NbInsertions > MaxInsSupport)
							{
								// Stop calcul

							}
						}
					}
				}
			}
		}

		else
		{
			// Balayage des colonnes insertions
			// Attention 13 > correspond à  24 insertions
			//           14 > ............  53 ..........
			//           15 > ............ 105 ..........
			//           16 > ............ 365 ..........
			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n° de colonne
				JUnt32 NbInsertions;
				NbInsertions = (NoCol - m_NbHeaderCol);
				switch (NoCol - m_NbHeaderCol)
				{
					case 0   :
						// Asymptote atteinte
						NbInsertions = MaxInsSupport;
						break;
					case 13  :
						// Pour les bi-mensuels
						NbInsertions = 24;
						break;
					case 14  :
						// Pour les hebdomadaires
						NbInsertions = 53;
						break;
					case 15  :
						// Pour les bi-hebdomadaires
						NbInsertions = 105;
						break;
					case 16 :
						// Pour les quotidiens
						NbInsertions = 365;
						break;
				}

				// Récupère résultat
				KeyResultCum.m_NbInsert         = NbInsertions;
				pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

				if (pDocChoixSupport->m_MapResultatCum.IsValid())
				{
					// Récupération Résultat
					ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 

					// Calcul Résultat Colonne (selon grandeur)
					CString Txt;
					Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);

					// Sauvegarde valeur maximal
					CResultCum	ResultCumMaxInsert;
					if (NbInsertions == MaxInsSupport)
						ResultCumMaxInsert = ResultCum;

					// Indication maximum atteint
					if (NoCol == m_NbHeaderCol)
						// Couleur spéciale pour indiquer max cumulativité
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
					{
						if (NbInsertions < MaxInsSupport)
						{
							// Indique couleur cumulativité progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));
						}
						else
						{
							// Indique en couleur limite atteinte pour calcul cumulativité
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()),
																CGXStyle()
 																.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
						}
					}
				}
				else
				{
					// A calculer
				}
			}
		}

		/*
		else
		{
			// Cible non autorisee
			pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
			if (!pDocChoixSupport->m_MapResultatCum.IsValid())
			{
				ResultCum.m_Aud1Ins		 = -1.0;
				ResultCum.m_AudProba1Ins = -1.0;
				ResultCum.m_Effectif	 = -1.0;

				pDocChoixSupport->m_MapResultatCum.Add(KeyResultCum) = ResultCum; 

				JFlt64 VolLect1Ins		= -1.0;
				JFlt64 AudProba1Ins		= -1.0;
				JFlt64 sertarien		= -1.0;
				JInt32 PtEtal			= -1.0;
			}
			else
			{
				// Récupération Résultat déjà existant
				ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 
			}

			JBool CibleNonAutorisee = true;
		}
		*/
	}

	// Progress bar invisible
	m_ProgressResultCum.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                      m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										      m_GridCumulativite.GetColCount()-1),
									 CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/format/périodicité
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColSupport-1),
		                                    TRUE,GX_UPDATENOW);

	/*
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColFormat-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);
	*/
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColPeriodicite-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Flag calcul effectué
	pDocChoixSupport->m_ParamCumModif = false;

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Affichage résultat cumulativité
JBool CDlgCumulativite::AfficheResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridCumulativite.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultCum KeyResultCum;

	// Information résultat archivé
	CResultCum	ResultCum;

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridCumulativite.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridCumulativite.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultCum.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultCum.GetRange( nLower, nUpper);

	// Calcul indice clé segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice clé moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 
	
	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récupère le format support sélectionné pour la cumulativité
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();

		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			/*
			if(!pTarifs->GetTarifByVersion(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,Tarif))
				Tarif = 0;
			*/
			JUnt32 Duree;
			JDate DateExact;
			/*
			if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
				Tarif = 0;
			*/
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);	
		}

		// Ecriture Périodicité
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace à gauche pour présentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Insere dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain sélectionné
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain sélectionné
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible sélectionnée
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible sélectionnée
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// A FAIRE Récupération des segments population
		// @@@ SEGPOP

		// Balayage des colonnes insertions
		// Attention 13 > correspond à  24 insertions
		//           14 > ............  53 ..........
		//           15 > ............ 105 ..........
		//           16 > ............ 365 ..........
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
		{
			// Nombre d'insertions via n° de colonne
			JUnt32 NbInsertions;
			NbInsertions = (NoCol - m_NbHeaderCol);
			switch (NoCol - m_NbHeaderCol)
			{
				case 0   :
					// Asymptote atteinte
					NbInsertions = MaxInsSupport;
					break;
				case 13  :
					// Pour les bi-mensuels
					NbInsertions = 24;
					break;
				case 14  :
					// Pour les hebdomadaires
					NbInsertions = 53;
					break;
				case 15  :
					// Pour les bi-hebdomadaires
					NbInsertions = 105;
					break;
				case 16 :
					// Pour les quotidiens
					NbInsertions = 365;
					break;
			}

			// Calcul si nb insertions ne dépasse pas le maximum autorisé

			// Recherche Résultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur résultats
			KeyResultCum.m_IdSupport = IdSupport;
			KeyResultCum.m_IdTerrain = IdTerrain;
			KeyResultCum.m_IdCible   = IdCible;
			KeyResultCum.m_NbInsert  = NbInsertions;

			CResultCum	ResultCumMaxInsert;
			pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
			if (pDocChoixSupport->m_MapResultatCum.IsValid())
			{
				// Information résultat archivé
				CResultCum	&ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem();

				// Le résultat pour la cellule
				Txt = ResultCellule(NoLig, NoCol, Tarif, KeyResultCum.m_NbInsert, ResultCum);

				// Sauvegarde valeur maximal
				if (NbInsertions == MaxInsSupport)
					ResultCumMaxInsert = ResultCum;
			}
			else
			{
				Txt = "*";
				m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}

			// Indication maximum atteint
			if (NbInsertions >= MaxInsSupport || NbInsertions == 3)
				// Couleur spéciale pour indiquer max cumulativité et au delà
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
													CGXStyle()
													.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
			else
				// Indique couleur cumulativité progressive
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
													CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
		}
	}

	// Progress bar invisible
	m_ProgressResultCum.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                      m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										      m_GridCumulativite.GetColCount()-1),
									 CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/périodicité
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColSupport-1),
		                                    TRUE,GX_UPDATENOW);

	/*
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColFormat-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);
	*/
	m_GridCumulativite.ResizeColWidthsToFit(CGXRange(0,m_NoColPeriodicite-1,
			                                         m_GridCumulativite.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Update visualisation supports
	UpdateVisuSupports();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Résultat cellule selon grandeur analyse demandée
// "Audience"								------------ Audience (Nb lecteurs)
// "Audience (000)"							------------ Audience (Nb lecteurs en milliers)
// "Nb Contacts (000)"						------------ Nombre de contacts en milliers
// "GRP [+]"								------------ GRP Actif	
// "Couverture cumulée"						------------ Couverture cumulée en %
// "Répétition"								------------ Répétition
// "Répétition"								------------ Répétition
// "Répétition[+]"							------------ Répétition[+}
// "Cumulativité"							------------ Cumulativité
// "Coût/%Couverture"						------------ Coût en % de couveture
//
CString CDlgCumulativite::ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JInt32 NbInsertions, CResultCum &ResultCum)
{

	// Par défaut Texte
	CGXStyle style;
	m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
								     CGXStyle()
									   .SetFormat(GX_FMT_TEXT)
									   .SetValueType(GX_VT_STRING));
	
	// Chaine formattée par défaut
	CString TxtResult = "*";

	// Chaine stop calcul cumul
	CString TxtStopResult = "-";

	// Récupère index grandeur
	JUnt32 IdGrandeur = 0;
	pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE NB LECTEURS /////////////////////////////
		case pDocChoixSupport->GrdCum_Audience : 
		{
			if (ResultCum.m_Audience >= 0)
				TxtResult.Format("%0.f",ResultCum.m_Audience);

			// Découpage chaine par milliers
			pDocChoixSupport->FormatString(TxtResult,3);

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		//////////////////////////// CALCUL AUDIENCE NB LECTEURS (000) /////////////////////////
		case pDocChoixSupport->GrdCum_Audience000 : 
		{
			if (ResultCum.m_Audience >= 0)
				TxtResult.Format("%d",(JUnt32)((ResultCum.m_Audience + 500.0)/1000.0));

			// Découpage chaine par milliers
			pDocChoixSupport->FormatString(TxtResult,3);

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		/////////////////////////////////// CALCUL NB CONTACT (000) ////////////////////////////
		case pDocChoixSupport->GrdCum_NbContact000 : 
		{
			if (ResultCum.m_VolLecture >= 0) 
			{
				// Avec arrondi
				TxtResult.Format("%d",(JUnt32)((ResultCum.m_VolLecture + 500.0)/1000.0));

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		////////////////////////////////////// CALCUL GRP ///////////////////////////////////////////
		case pDocChoixSupport->GrdCum_GRP :
		{
			if (ResultCum.m_Effectif > 0 && ResultCum.m_AudProba1Ins >=0)
			{
				// Calcul du GRP si au moins effectif valide
				JFlt64 GRP = (100.0 * ResultCum.m_AudProba1Ins * NbInsertions) / ResultCum.m_Effectif;

				TxtResult.Format("%0.1f ",GRP);
				//TxtResult.Replace(".",",");
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		////////////////////////////////////// CALCUL X-GRP ///////////////////////////////////////////
		case pDocChoixSupport->GrdCum_XGRP :
		{
			// Test si calcul possible
			if (ResultCum.m_Effectif > 0 && ResultCum.m_VolLecture >= 0)
			{
				// Calcul du GRP[+]
				JFlt64 xGRP = (ResultCum.m_VolLecture * 100.0) / ResultCum.m_Effectif;

				TxtResult.Format("%0.1f ",xGRP);
				//TxtResult.Replace(".",",");
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		//////////////////////////////// CALCUL COUVERTURE CUMULEE ///////////////////////////
		case pDocChoixSupport->GrdCum_CouvCum : 
		{
			if (ResultCum.m_Effectif > 0 && ResultCum.m_Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultCum.m_Audience / ResultCum.m_Effectif) * 100.0;
				TxtResult.Format("%0.1f%%",Couv);
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		//////////////////////////////// CALCUL REPETITION ///////////////////////////////////
		case pDocChoixSupport->GrdCum_Repetition :
		{
			// Il faut au moins de l'audience
			if (ResultCum.m_Audience > 0)
			{
				// Calcul répétition
				JFlt64 Repet = (ResultCum.m_AudProba1Ins * NbInsertions) / ResultCum.m_Audience;
				TxtResult.Format("%0.2f ",Repet);
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		//////////////////////////////// CALCUL REPETITION + ///////////////////////////////////
		case pDocChoixSupport->GrdCum_XRepetition :
		{
			// Il faut au moins de l'audience
			if (ResultCum.m_Audience > 0)
			{
				// Calcul répétition +
				JFlt64 Repet = ResultCum.m_VolLecture / ResultCum.m_Audience;
				TxtResult.Format("%0.2f ",Repet);
				//TxtResult.Replace(".",",");
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		/////////////////////////////////// CALCUL CUMULATIVITE ///////////////////////////////////
		case pDocChoixSupport->GrdCum_Cumulativite : 
		{
			if (ResultCum.m_Aud1Ins > 0)
			{
				// TxtResult.Format("%0.1f ",ResultCum.m_Audience / ResultCum.m_Aud1Ins);
				TxtResult.Format("%0.f ",(ResultCum.m_Audience / ResultCum.m_Aud1Ins)*100);
				//TxtResult.Replace(".",",");
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;
		
		////////////////////////////// CALCUL COUT COUVERTURE /////////////////////////////////
		case pDocChoixSupport->GrdCum_CoutCouv : 
		{
			if (ResultCum.m_Effectif > 0 && ResultCum.m_Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultCum.m_Audience / ResultCum.m_Effectif) * 100.0;

				// Calcul Cout Point Couverture
				if (Couv > 0)
				{
					// Calcul en entier
					JFlt64 Cout = (Tarif * (JFlt64)NbInsertions / Couv);

					// Affichage selon nb décimales demandées
					if (pDocChoixSupport->m_NbDecCout == 1)
						TxtResult.Format("%0.1f",Cout);
					else if (pDocChoixSupport->m_NbDecCout == 2)
						TxtResult.Format("%0.2f",Cout);
					else 
						TxtResult.Format("%0.f",Cout);

					// Découpage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);

				}
			}

			// Optim 2.7
			if (ResultCum.m_Audience == 0) TxtResult = TxtStopResult;

			m_GridCumulativite.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		default:
		break;
	}
	
	// Renvoi la chaine formatée
	return TxtResult;
}

////////////////////////////////////////////////////////////////////////
// Mise à jour sélections supports
bool CDlgCumulativite::UpdateSelSupports()
{
	// Nombre de supports nouvelle sélection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRow();

		// Initialise libellés supports en 1ere colonne
		InitRowSupports();

		// Mise à jour du grid
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur précédemment existant, 
		// et nouvelle liste sans ascenseur !!!
		m_GridCumulativite.SetTopRow(m_NbHeaderRow, GX_UPDATENOW); 
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Init lignes supports
void CDlgCumulativite::InitRowSupports()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount()== 0) return;

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Récupère objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Style check-box libellés supports
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1,InxLigSupp + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColSelSupport-1),
									 CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

	// Style texte libellés supports
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColSupport-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColSupport-1),
									 CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

	// Style texte couleur graphe
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColColorGraphe-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColColorGraphe-1),
									 CGXStyle()
										.SetEnabled(false)
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

	/*
	// Style texte libellés formats
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColFormat-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColFormat-1),
									 CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));
	*/
	
	// Style texte libellés périodicité
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColPeriodicite-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColPeriodicite-1),
									 CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)
										.SetItalic(FALSE)));

	// Balaye toutes les lignes associées aux supports
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// Ident support
		JUnt32 IdSupport  = pDocChoixSupport->m_Offre.m_TBLIdSupports.GetItem();
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Vérifie qu'il est valide dans le map sélection
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat Support
			JBool EtatSupport = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

			// Element support
			IElem* pSupport = pRegroupements->GetIElemByID(IdSupport);

			// Indique Id Support dans colonne invisible
 			m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdSupport));


			// on récupére le libellé support
			CString LibSupport = "-";
			if (pSupport)
				// Affichage libellé support
				LibSupport = pSupport->GetLabel().AsJCharPtr();

			// Affichage libellé support
			m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibSupport));

			// Récupère info formats support
			CString ListFormat		= "";
			CString LibFormatNull	= "*";
			long InxSelFormat		= -1;
			pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleCum);
				
			// Puis indication support sélectionné ou pas
			if (EtatSupport)
			{
				// Valide sélection support
				m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
			}
			else
			{
				// Dévalide la sélection support
				m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
			}

			// Incrémente index ligne
			InxLigSupp++;
		}	
	}

	// Balaye toutes les lignes associées aux familles
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// A FAIRE - FAMILLES
	}

	// Colonne Id Support invisible
	m_GridCumulativite.SetColWidth(m_NoColIdSupport-1, m_NoColIdSupport-1,0);

	// Retaille la colonne sélection supports
	m_GridCumulativite.SetColWidth(m_NoColSelSupport-1, m_NoColSelSupport-1,20);

	// Retaille la colonne couleur graphe
	m_GridCumulativite.SetColWidth(m_NoColColorGraphe-1, m_NoColColorGraphe-1,16);

	// Hauteur des lignes
	JUnt32 NbRow = m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount();
	// m_GridCumulativite.SetRowHeight(0,m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,12,0,GX_SMART);

	// Mise à jour couleur sélection supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs spéciales de certaines cellules
void CDlgCumulativite::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne résultat
	// Cellules info insertion en fond blanc et couleur caractère différent
	if (m_GridCumulativite.GetColCount() >= m_NoColPeriodicite)
	{
		m_GridCumulativite.LockUpdate(true); 

		// Cellules info moteur en fond blanc et couleur caractère différent
		m_GridCumulativite.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 0, m_NoColPeriodicite-1),
											CGXStyle()
											.SetInterior(CATPColors::GetColorWhite())
											.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Niveau 0 / Header info grandeur de couleurs différente
		m_GridCumulativite.SetStyleRange(CGXRange(0, m_NoColPeriodicite, 0, m_GridCumulativite.GetColCount()),
											CGXStyle()
											.SetInterior(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));

		m_GridCumulativite.LockUpdate(false); 
		m_GridCumulativite.Redraw(); 
	}		
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Update grille résultat via new sel Terrains / cibles ou format
bool CDlgCumulativite::UpdateResultCum(JBool CalculResult)

{
	// Test validité sélection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si sélection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// Désactive mise à jour grid cum
	m_GridCumulativite.LockUpdate(true);

	// Mise à jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise à jour nombre et headers rows
	if (!UpdateRow()) return false;

	// Update affichage infos analyse
	UpdateTitreAnalyse();

	// Calcul des résultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Bloque resize hauteur des lignes
	m_GridCumulativite.GetParam()->EnableTrackRowHeight(FALSE);

	// Active mise à jour grid cum
	m_GridCumulativite.LockUpdate(false);

	// redessine le grid cum
	m_GridCumulativite.Redraw();

	// Rend visible le grid, si au moins quelques résultats
	if (m_GridCumulativite.GetColCount() >= m_NoColPeriodicite)
		m_GridCumulativite.ShowWindow(SW_SHOW);
	else
		m_GridCumulativite.ShowWindow(SW_HIDE);

	
	// Update visualisation supports
	UpdateVisuSupports();

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////
// Update Entete Analyse (Nom de la source / terrain et date tarification)
void CDlgCumulativite::UpdateTitreAnalyse()
{
	// Si aucune analyse en ours
	CString Titre = "Analyse non définie";

	// Test si analyse définie
	if (pDocChoixSupport)
	{
		// Init Texte Info
		Titre = "";

		// Ajoute Info Filtre d'audience
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

		// Récup Id Filtre audience du module courant
		JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

		// Récupère label 
		Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
		
		// On vérifie qu'une source terrain a été choisi, et auusi une date tarification
		JUnt32 SrcID = 0;
		if (pDocChoixSupport!=0 && pDocChoixSupport->IsValid())    
		{
			// Ajoute libellé de la source
			if (pDocChoixSupport->GetIdSrcAudienceSel())
			{

				// Ajoute Info Filtre d'audience
				JSRCPresse* m_srcPresse;
				m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

				// Récup Id Filtre audience du module courant
				JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

				// Récupère label 
				// Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);

				// Récup Table
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();

				// Récup la source audience courante
				SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

				// Récup le nouveau source
				const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

				// Recuperer dans table source
				// Titre += " - ";
				// Titre += pt->m_Libelle.AsJCharPtr();

				// Ajoute Info Terrain
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
				{
					// Récupére ident terrain
					JUnt32 TerID				= pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey();
					const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);
	
					// Recuperer dans table terrain
					// Titre += " ";
					// Titre += pt->m_Libelle.AsJCharPtr();
				}
			}
			else
			{
				// Titre += " - Aucune source";
			}

			// Ajoute info Cible
			Titre += " - ";
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid ())
			{
				// Récupére ident cible
				JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

				// Récup libellé 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
				{
					// Le libellé cible
					Titre += pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();

					// ...... son effectif
					// Récup Table des cibles
					// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

					// Récup cible correspondant à l'indice
					// IItem * pItem = pItemConst->GetItemConstruit(IdCible);
					IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

					// Si cible trouvé, on retourne son libellé
					if(pItem)
					{
						// Récupère effectif cible courante
						JUnt32 EffectifCible = 0;
					
						// on crée la clé des effectifs
						CKeyMoteur keyMoteur;
						
						// Récup terrain sélectionné
						JUnt32 IdTerrain = 0;
						pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
						if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
							// au moins 1 terrain sélectionné
							IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

						// lance le moteur
						keyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(), 
										IdTerrain,IdCible);
						EffectifCible = (JUnt32)(pDocChoixSupport->m_MoteurPresse.GetEffectif(keyMoteur)+0.5);

						// Ajoute l'info effectif cible
						CString TxtEff;
						TxtEff.Format("%d",EffectifCible);
						pDocChoixSupport->FormatString(TxtEff,3);
						Titre += " (" + TxtEff + ")";
					}
				}
				else
					Titre += "Aucune cible";
			}
			else
				Titre += "Aucune cible";

			// Ajoute Référence Affinité
			// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
			// IItem * pItem = pItemConst->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);
			IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

			Titre += " - Réf.Aff : ";
			if(pItem)
				// Ajoute et sélection d'office du seul indice affinité (pour le moment)
				Titre += pItem->GetLabel().AsJCharPtr();
			else
				Titre += "Aucune";


			/*
			// Récup id cible
			JUnt32 Id = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
			if(Id)
			{
				// Récup Table des cibles
				CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

				// Récup cible correspondant à l'indice
				IItem * pItem = pItemConst->GetItemConstruit(Id);

				// Si cible trouvé, on retourne son libellé
				if(pItem)
				{
					// Recuperer libellé cible en cours
					Text += " - ";
					Text += pItem->GetLabel().AsJCharPtr();

					// Récupère effectif cible courante
					JFlt64 EffectifCible = 0.0;
					
					// on crée la clé des effectifs
					CKeyMoteur keyMoteur;
					// on initialise la clé
					// A FAIRE JEREMY
					m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
					if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
						keyMoteur.Set(m_pATPDocCreuset->GetIdSrcAudienceSel(), m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
					// on initialise l'effectif
	
					EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

					// si effectif <> 0 ajoute info effectif
					if (EffectifCible > 0)
					{
						CString TxtEffectif;
						Text += " (";

						TxtEffectif.Format("%.0f",EffectifCible); 
						CString Fmt = _T("### ### ### ### ###");
						CFormatStr::FormatNum(Fmt, TxtEffectif);
						Text += TxtEffectif;

			*/

			/*
			// Ajoute info type d'analyse en cours
			Titre += " - ";
			pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
			if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
			{
				// Indx Grandeur Analyse Sélectionnée
				JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

				// Ajoute au titre globale
				Titre += pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, IdGrandeur);
			}
			else
				// Aucune analyse en cours
				Titre += "Aucune analyse";
			*/

		}

		// Ecrit le titre analyse non définie
		m_TitreCumulativite.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridCumulativite.GetRowCount() > 1)
		{
			// Nom du moteur utilisé
			CString TxtMoteur;
			switch (pDocChoixSupport->m_MoteurPresse.GetType())
			{
				case CMoteurPresse::ABAQUE:
					// Méthode macro-probabiliste (JFC)
					TxtMoteur.LoadString(IDS_CS_MOTABAQUE);
					break;
				case CMoteurPresse::ABAQUE_ETAL:
					// Méthode macro-probabiliste étalonnée (JFC)
					TxtMoteur.LoadString(IDS_CS_MOTABAQUEETAL);
					break;
				case CMoteurPresse::PROBA:
					// Méthode probabilités individuelles (idem SMP-PRESSE)
					TxtMoteur.LoadString(IDS_CS_MOTPROBA);
				break;
			}

			// On l'affiche dans la grille
			m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);
			m_GridCumulativite.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColPeriodicite-1),
										     CGXStyle()
											.SetValue(TxtMoteur));
			m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Définit si la grille résultat est valide
bool CDlgCumulativite::GrilleResultValid()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount()  			> 0  &&
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount() > 0  &&
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount()   > 0  &&
		pDocChoixSupport->GetMapIdSegmentsAudienceSel()->GetCount() > 0)
	{
		// La grille est valide
		return true;
	}

	// Pas de grille résultat définie
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Update Résultat des Coûts (suite à modif Format ou Tarif)
void CDlgCumulativite::UpdateResultCout()
{
	// Mise à jour fenêtre cumulativité
	UpdateResultat(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de lignes header
JUnt32 CDlgCumulativite::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de colonnes header
JUnt32 CDlgCumulativite::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére indice couleur pour courbe cumulativité
JUnt32 CDlgCumulativite::GetIdColorCourbe(JUnt32 NoRow, JUnt32 NoCol)
{
	return ((NoRow - 1)*(m_NbLigColor + 1) + NoCol - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère le nombre de courbe cumulation en cours
JUnt32 CDlgCumulativite::GetNbCourbeCum()
{
	return m_IGraphCourbeCum.GetCourbeCount();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Nb lignes pour couleur courbe cum
JUnt32 CDlgCumulativite::GetNbRowColorCum()
{
	return m_NbLigColor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Nb colonnes pour couleur courbe cum
JUnt32 CDlgCumulativite::GetNbColColorCum()
{
	return m_NbColColor;
}

int CDlgCumulativite::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (JFCDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	/* 
	// Création du graphe et récupération liaison document
	if (m_GrapheCum.Create(NULL, WS_VISIBLE|WS_CHILD    , CRect(0, 0, CATPGraphCum::GetCXMin(), CATPGraphCum::GetCYMin()), this, 0) == FALSE) return (-1);
	m_GrapheCum.SetDocument(pDocChoixSupport,this);
	*/

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Affiche barre de calcul
void CDlgCumulativite::AffichBarreCalcul()
{
	// Initialisation Progress bar et visible
	m_ProgressResultCum.ShowWindow(SW_SHOW); 

	// Avance progress bar
	m_ProgressResultCum.SetPos(2);
	m_ProgressResultCum.RedrawWindow(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export des résultats Cumulation d'audience
void CDlgCumulativite :: ExportResultat()
{
	// Demande emplacement du fichier export
	CFileDialog DlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (DlgFile.DoModal( ) == IDCANCEL)
		return;

	if(DlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		SaveAsExcel(DlgFile.GetPathName());
	}
	else
	{
		// Création du fichier
		CStdioFile ExportFile;
		
		// if (!ExportFile.Open(DlgFile.GetFileName( ),CFile::modeCreate | CFile::modeWrite))
		if (!ExportFile.Open(DlgFile.GetPathName( ),CFile::modeCreate | CFile::modeWrite))
		{
			// Impossible d'ouvrir ce fichier
			CString TxtErr;
			TxtErr.Format("%s %s %s","Fichier ",ExportFile.GetFileName(),"ne peut pas être ouvert");
			AfxMessageBox(TxtErr,MB_ICONEXCLAMATION);
			return;
		}

		// Récupération de l'entête globale
		CString TxtEntete;
		RecupEnteteExport(TxtEntete);

		// Ecriture entete dans fichier export
		ExportFile.WriteString(TxtEntete);

		// 2eme bloc : Résultats
		TxtEntete = "BLOC 2\t";

		// Type de résultat (grandeur sélectionnée)
		CString TxtResultat = "";
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		{
			// Une grandeur a bien été sélectionnée
			TxtResultat = pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum,pDocChoixSupport->m_MapIdGrandCumSel.GetItem());  
		}
		TxtEntete += TxtResultat + "\n";
		ExportFile.WriteString(TxtEntete);
		
		// Les résultats du grille (ATTENTION OD OD OA avec CStdioFile)
		// m_GridCumulativite.CopyTextToFile(ExportFile, CGXRange(m_NbHeaderRow-1, m_NoColSelSupport-1, m_GridCumulativite.GetRowCount(), m_GridCumulativite.GetColCount()));
		for (int lig = m_NbHeaderRow-1; lig < m_GridCumulativite.GetRowCount(); lig++)
		{
			for (int col = m_NoColSelSupport-1; col <= m_GridCumulativite.GetColCount(); col++)
			{
				CString Txt = m_GridCumulativite.GetValueRowCol(lig, col) + "\t";
				ExportFile.WriteString(Txt);
			}
			// Saut de ligne
			TxtEntete = "\n";
			ExportFile.WriteString(TxtEntete);
		}
		
		/*
		CFile *pFileTmp;
		pFileTmp = &ExportFile;
		m_GridCumulativite.CopyTextToFile(*pFileTmp, CGXRange(m_NbHeaderRow-1, m_NoColSelSupport-1, m_GridCumulativite.GetRowCount(), m_GridCumulativite.GetColCount()));
		*/

		// Bloc n° 3 : Infos Terrain + Cible + Effectif + Nb Cas
		// ------------------------------------- Les effectifs des cibles utilisées -------------------------------------------
		//
		// Récupération du bloc infos terrains / cibles
		RecupBlocTerrainCible(TxtEntete);

		// Ecriture dans fichier export
		ExportFile.WriteString(TxtEntete);

		// Bloc n° 4 : Détail groupes
		// Si on est en mode groupé, export contenu des groupes
		if (pDocChoixSupport->m_ModeColorGraphe == 1)
		{
			// Saut de ligne
			TxtEntete = "\n";
			ExportFile.WriteString(TxtEntete);

			// Récupération des infos groupe
			RecupContenuGroupe(TxtEntete);

			// Ecriture dans fichier export
			ExportFile.WriteString(TxtEntete);
		}
		
		// Fin Export
		ExportFile.Close();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération de l'entête pour export cumulativité
void CDlgCumulativite :: RecupEnteteExport(CString &Entete)
{
	// N° Version
	Entete  = "No de version :\t";
	Entete += "1.0\n";

		// Nom du plateau
	Entete += "NOM PLATEAU :\t";
	Entete += pDocChoixSupport->m_KEYPlateau.GetLabel() + "\n";

	// Init entete avec type de résultat
	Entete += "EXPORT CUMULATION\n\n";
	
	// 1er Bloc : Entete
	Entete += "BLOC 1\n";
	
	// ------------------------------------------- Date de l'export ------------------------------------------------
	Entete += "DATE : \t";
	COleDateTime DateCur = COleDateTime::GetCurrentTime();
	CString TxtDate;
	TxtDate.Format("%02d/%02d/%04d - %02dh%02d",DateCur.GetDay(),DateCur.GetMonth(),DateCur.GetYear(),DateCur.GetHour(),DateCur.GetMinute());
	Entete += TxtDate;
	Entete += "\n";

	// --------------------------------------------- La source d'audience --------------------------------------------
	//
	Entete += "SOURCE AUDIENCE : \t";
	JUnt32 SrcID;
	if (pDocChoixSupport->GetIdSrcAudienceSel())
	{
		// Récup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// Récup la source audience courante
		SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

		// Récup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// Recuperer dans table source
		Entete += pt->m_Libelle.AsJCharPtr();
	}
	else
		// Pas de source d'audience
		Entete += "Aucune source";

	Entete += "\n";


	// Le Terrain utilisé
	Entete += "TERRAIN : \t";

	// Récup la source audience courante
	SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

	// ---------------------------------------- Sélection du terrain -------------------------------------------
	//
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// Et l'ident terrain
		int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

		// Récupère infos terrain 
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();
		const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

		// Récupére libelle terrain
		if (pt)
		{
			Entete += pt->m_Libelle.AsJCharPtr();
			Entete += "\t";
		}
	}
	Entete += "\n";
		
	// ---------------------------------------- La cible utilisée -------------------------------------------
	//
	Entete += "CIBLE : \t";

	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// Récupére ident cible
		JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

		// Récup libellé 
		pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
		
		if (pDocChoixSupport->m_MAPpItems.IsValid()) 
		{
			Entete += pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
		}
		else
			Entete += "Aucune cible";

		Entete += "\t";
	}	
	Entete += "\n";
	
	// ----------------------------------------- La référence affinité -----------------------------------------
	//
	Entete += "REF. AFFINITE : \t";
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
	// IItem * pItem = pItemConst->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

	if(pItem)
		// Ajoute et sélection d'office du seul indice affinité (pour le moment)
		Entete += pItem->GetLabel().AsJCharPtr();
	else
		Entete += "Aucune référence affinité";
	Entete += "\n";

	// -------------------------------------------- Le format utilisé ------------------------------------------------
	Entete += "FORMAT : \t";
	JUnt32 IdFormat;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
	{
		// Récupére l'ident du format courant
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
			// libellé long format
			Entete += pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat); 
		else
			Entete += "non défini";
	}
	else
		Entete += "non défini";

	Entete += "\n";

	// Ajoute Info Filtre d'audience
	Entete += "FILTRE AUDIENCE : \t";
	JSRCPresse* m_srcPresse;
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

	// Récup Id Filtre audience du module courant
	JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

	// Récupère label 
	Entete += m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
	Entete += "\n";
		
	// Le contexte de lecture
	Entete += "CONTEXTE DE LECTURE \n";
	//CKeyTerrain KeyTerrain;
	//KeyTerrain.m_IDSource  = SrcID;
	//pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	//KeyTerrain.m_IDTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	//int NbCtx = pDocChoixSupport->m_MoteurPresse.GetNbCtxLecture(KeyTerrain); 

	//// Crée la liste des idents contextes sélectionnés
	JSRCTables* m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	//if (NbCtx > 0)
	//{
	//	JList <JInt32> LstCtx;
	//	for(int i= 0; i<NbCtx; i++)
	//	{
	//		pDocChoixSupport->m_MoteurPresse.GetSelCtxLecture(KeyTerrain,i,LstCtx);

	//		// Nom du contexte
	//		if (pDocChoixSupport->m_MoteurPresse.GetIDCtxLecture(KeyTerrain,i) == 11) continue;
	//		Entete += m_srcTables->m_TBLDimensions.GetLabelByID(pDocChoixSupport->m_MoteurPresse.GetIDCtxLecture(KeyTerrain,i)); 
	//		Entete += " : \t";

	//		if (LstCtx.GetCount() == 0)
	//			// Aucun contexte sélectionné
	//			Entete += "Aucun contexte";
	//		else
	//		{
	//			// Pour chaque contexte
	//			for (LstCtx.MoveFirst(); LstCtx.IsValid(); LstCtx.MoveNext())
	//			{
	//				JInt32 IdCtx = LstCtx.GetItem();
	//				Entete += m_srcTables->m_TBLDimensions.GetLabelByID(IdCtx);
	//				Entete += "\t";
	//			}
	//		}
	//		Entete += "\n";
	//	}
	//}
	//else
	//{
	//	// Pas de contexte de lecture pour certaines enquêtes
		// Entete += "Aucune contexte : \tAucun contexte\n";
	//}

	// Mode calcul des familles
	Entete += "CALCUL DES FAMILLES : \t ";
	if (pDocChoixSupport->m_CumulFamille)
		Entete += "Résultats cumulés \n";
	else	
		Entete += "Résultats moyennés \n";

	// ---------------------------------------- La date de tarification --------------------------------------------
	//
	Entete += "DATE DE TARIFICATION : \t";

	if (pDocChoixSupport->m_DateTarifaire.IsValid())
	{
		CString TxtDate;
		JInt32 Day, Month, Year;
		pDocChoixSupport->m_DateTarifaire.GetDate(Day,Month,Year); 
		TxtDate.Format("%02d/%02d/%04d",Day,Month,Year);
		Entete += TxtDate;
	}
	else
		Entete += "non définie";

	Entete += "\n";

	// ----------------------------------------- Export Type de poids ----------------------------------------------
	//
	Entete += "TYPE POIDS :\t";
	JSRCTables* srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Libelle type de poids
	if (pDocChoixSupport->m_Offre.m_IdTypePoids > 0)
		Entete += srcTables->m_TBLPoids.GetLabelByID(pDocChoixSupport->m_Offre.m_IdTypePoids);
	else
		// type de poids non défini
		Entete += "non défini";

	Entete += "\n";

	// ---------------------------------------- Export Type de moteur calcul ---------------------------------------
	//
	// on exporte le titre
	Entete += "METHODE DE CALCUL DES COUVERTURES : \t";

	CString str;
	switch (pDocChoixSupport->m_MoteurPresse.GetType())
	{
		case CMoteurPresse::ABAQUE:
			// Méthode macro-probabiliste (JFC)
			str = "Modélisation macro-probabiliste brute";
			break;
		case CMoteurPresse::ABAQUE_ETAL:
			// Méthode macro-probabiliste étalonnée (JFC)
			str = "Modelisation macro-probabiliste";
			break;
		case CMoteurPresse::PROBA:
			// Méthode probabilités individuelles (idem SMP-PRESSE)
			str = "Algorithme full-binomial";
			break;
	}
	Entete += str;

	/*
	// Test si au moins 1 famille
	JBool AuMoins1Famille = false;
	for (JUnt32 NoLig = 0; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Tes tsi c'est une famille
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			AuMoins1Famille = true;
			break;
		}
	}

	// Attention mode de calcul famille
	if (AuMoins1Famille && pDocChoixSupport->m_AFFModeFamilles)
	{
		if (pDocChoixSupport->m_CumulFamille)
			Entete += " (Résultats cumulés pour les familles) \t";
		else	
			Entete += " (Résultats moyennés pour les familles) \t";
	}
	*/

	Entete += "\n";

	// ---------------------------------------- Export Paramétrage LDP / LNM / LDP 30j -----------------------------
	//
	// LDP ou LNM Magazines
	if (pDocChoixSupport->m_bLNMMag)
		Entete += "LNM pour les magazines";
	else
		Entete += "LDP pour les magazines";
	Entete += "\n";

	// LDP ou LNM Quotidiens
	if (pDocChoixSupport->m_bLDPQtd)
		Entete += "LDP pour les quotidiens";
	else
		Entete += "LNM pour les quotidiens";
	Entete += "\n";

	// LDP à 30 jours ou à 60 jours pour les bimestriels
	if (pDocChoixSupport->m_bLDP30J)
		Entete += "LDP à 30 jours pour les bimestriels";
	else
		Entete += "LDP à 60 jours pour les bimestriels";
	Entete += "\n";

	// --------------------------- Libelle segment de ciblage ----------------------------- 
	Entete += "SEGMENT:\t";

	// Boucle sur les élmts segment audience sélectionnés pour cumulativité
	JBool PremSeg = true;
	for (pDocChoixSupport->GetMapIdSegmentsAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdSegmentsAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdSegmentsAudienceSel()->MoveNext())
	{
		// Récupére ident segment
		JUnt32 IdSeg	= pDocChoixSupport->GetMapIdSegmentsAudienceSel()->GetKey();

		// Segment de ciblage
		const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(IdSeg);

		// Ajout Libellé Segment
		if (!PremSeg) Entete += " , ";
		Entete += Seg->m_Libelle.AsJCharPtr(); 
		PremSeg = false;
	}	
	Entete += "\n";

	// --------------------------------- Info mode graphique -----------------------------
	//
	Entete += "GRAPHIQUE\t";
	if (pDocChoixSupport->m_ModeColorGraphe == 0)
		// Mode courbe supports séparés
		Entete += "Séparé\n";
	else
		// Mode regroupement support par couleur
		Entete += "Groupé\n";

	// Prévoir sauts de ligne avant affichage infos tableurs
	Entete += "\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération résultats cumulation détaillée
void CDlgCumulativite ::RecupGrilleExport(CString &Entete)
{
	// Init chaine résultat
	Entete = "";

	// Tableau de lignes et colonnes sélectionnées
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridCumulativite.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridCumulativite.GetSelectedRows(Rows,false,false);
	
	// Vire les n° colonnes avec indice header info générales
	for (int iCol = Cols.GetCount()-1 ; iCol >= 0; iCol--)
	{
		if (Cols.GetAt(iCol) < m_NoColPeriodicite)
			Cols.RemoveAt(iCol); 
	}

	// Vire les n° lignes avec indice header
	for (int iRow = Rows.GetCount()-1 ; iRow >= 0; iRow--)
	{
		if (Rows.GetAt(iRow) < m_NbHeaderRow)
			Rows.RemoveAt(iRow); 
	}

	// Récupération de la partie header pure  (row & col)
	CString Txt;
	for (int nRow = 1; nRow < m_NbHeaderRow ; nRow++)
	{
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
		{	
			if (nRow == m_NbHeaderRow-1)
			{
				// Header info support
				Txt = m_GridCumulativite.GetValueRowCol(nRow,nCol); 
				Entete += Txt;
			}

			// Tabulation après chaque info
			Entete += "\t";
		}

		// Ecriture header résultats colonnes
		for (int nCol = 0; nCol < Cols.GetCount(); nCol++)
		{
			// No de la colonne
			int NoCol = Cols.GetAt(nCol);

			if (NoCol >= m_NoColPeriodicite)
			{
				Txt = m_GridCumulativite.GetValueRowCol(nRow,NoCol); 
				Entete += Txt;
				Entete += "\t";
			}
		}

		// Saut après chaque ligne header grille
		Entete += "\n";
	}

	// Balayage des lignes sélectionnées
	int x = Rows.GetCount();
	for (int nRow = 0; nRow < Rows.GetCount(); nRow++)
	{
		// No de la ligne support
		int NoRow = Rows.GetAt(nRow);

		// Récupère ident support
		JUnt32 IdSupport = atoi(m_GridCumulativite.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Ecriture infos générales supports
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
		{	
			// Libellé support, ou périodicité )
			Txt = m_GridCumulativite.GetValueRowCol(NoRow,nCol); 

			/*
			if (nCol != m_NoColFormat-1)
				// Libellé support, ou périodicité )
				Txt = m_GridCumulativite.GetValueRowCol(NoRow,nCol); 
			else
			{
				// Récupère le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();

				// libellé format support
				Txt = pDocChoixSupport->GetFormat(IdFormatSupport,true);
			}
			*/
			Entete += Txt +  "\t";
		}
		
		// Ecriture résultats colonnes
		for (int nCol = 0; nCol < Cols.GetCount(); nCol++)
		{
			// No de la colonne résultat
			int NoCol = Cols.GetAt(nCol);
			Txt = m_GridCumulativite.GetValueRowCol(NoRow,NoCol); 
			Entete += Txt +  "\t";
		}

		// Saut après chaque ligne support
		Entete += "\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Export du graphique cumulativité
void CDlgCumulativite :: ExportGraphique()
{
	// Demande emplacement du fichier export
	CFileDialog DlgFile(FALSE,".emf",0,
						OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Text Files (*.emf)|*.emf|All Files (*.*)|*.*||",this);

	if (DlgFile.DoModal( ) == IDCANCEL)
		return;

	// Export du graphique sous format fichier
	if (!m_GraphCum.ExportGraph(DlgFile.GetPathName())) 
	{
		AfxMessageBox("Erreur lors de l'export fichier graphique cumulativité");	
	}

	/*
	// Test si au moins 1 support sélectionné pour le mapping
	if(!ExportItems(480,DlgFile.GetPathName()))
		AfxMessageBox("Erreur lors de l'export graphique cumulativité");	
	*/
}	


BOOL CDlgCumulativite::ExportItems(long VSize, const TCHAR* pFilename) 
{
	/////////////////////////////////////////////////////////////////////////////
	// la fonction pour exporter le dessin vers un MetaFile
	/////////////////////////////////////////////////////////////////////////////
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on vérifie la validité des éléments
	// if(m_GraphCum == NULL) return FALSE;

	return TRUE;

	/*
	// on récupère le contexte de périphérique
	CDC* pDC = this->GetDC();
	if(pDC == NULL) return FALSE;

	CMetaFileDC MetaDC; // création du MetaFile
	if(!MetaDC.CreateEnhanced(pDC, pFilename, NULL, NULL)) return FALSE;

	// récupération des attributs d'origine
	MetaDC.SetAttribDC(pDC->m_hAttribDC);

	// initialisation des outils de dessin
	if(!this->OnBeginDrawing(MetaDC))
	{
		MetaDC.ReleaseAttribDC();
		MetaDC.CloseEnhanced();
		return FALSE;
	}

	double HRes = (double)MetaDC.GetDeviceCaps(HORZSIZE) * (double)MetaDC.GetDeviceCaps(VERTRES);
	double VRes = (double)MetaDC.GetDeviceCaps(VERTSIZE) * (double)MetaDC.GetDeviceCaps(HORZRES);
	double Ratio = VRes / HRes;
	long HSize = (long)(((double)VSize * Ratio) + .5);
	
	// Dessin des axes
	this->DrawAxes(MetaDC, HSize, VSize, true);

	// Dessin des courbes
	this->DrawCurves(MetaDC, HSize, VSize);
	
	// Dessin liaison courbe et libellé équation
	this->DrawTraitEquation(MetaDC, HSize, VSize);

	// Les points du mapping
	this->DrawLosanges(MetaDC, HSize, VSize);

	// Dessin lien point et libellé
	this->DrawTraitLib(MetaDC, HSize, VSize);

	// Libellés point du mapping
	this->DrawLibelles(MetaDC, HSize, VSize);

	// Libelles des equations
	this->DrawLibEquation(MetaDC, HSize, VSize);
		
	// libération des outils
	this->OnEndDrawing(MetaDC); 

	MetaDC.ReleaseAttribDC();

	// fermeture du fichier, on garde un handle pour la copie éventuelle dans le Presse-Papiers
	HENHMETAFILE hMetaFile = MetaDC.CloseEnhanced();
	if(hMetaFile == NULL) return FALSE;

	// si un nom de fichier a été spécifié on termine ici avec succès
	if(pFilename == NULL)
	{
		// ouverture du presse-papiers
		if(!OpenClipboard()) return FALSE;

		// vidange du presse-papiers
		if(!EmptyClipboard())
		{
			CloseClipboard();
			return FALSE;
		}
		// envoie les données du MetaFile dans le presse-papiers
		if(SetClipboardData(CF_ENHMETAFILE, hMetaFile) == NULL)
		{
			CloseClipboard();
			return FALSE;
		}
		CloseClipboard();
	}
	else
	{
		// on supprime le métafile
		if (FALSE == ::DeleteEnhMetaFile(hMetaFile)) return (FALSE);
	}
	return TRUE;
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération du bloc infos terrains / cibles
void CDlgCumulativite ::RecupBlocTerrainCible(CString &Entete)
{
	// Bloc n° 3
    Entete = "\nBLOC 3\n";
    Entete += "TERRAIN \t CIBLE\t EFFECTIF\t NB CAS\n";

	// Récup Table
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	// Récup la source audience courante
	JUnt32	SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

	// ------------------------------------------ Sélection des terrains ---------------------------------------------
	//
	for (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext())
	{
		// Et l'ident terrain
		int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

		// Récupère infos terrain 
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();
		const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

		// Récupére libelle terrain
		CString LibTerrain = "";
		if (pt)
			LibTerrain= pt->m_Libelle.AsJCharPtr();
	
		// Les cibles associées
		for (pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			 pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid();
			 pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext())
		{
			// Récupére ident cible
			JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

			// Récup libellé 
			pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
			
			if (pDocChoixSupport->m_MAPpItems.IsValid()) 
			{
				Entete += LibTerrain + "\t" + pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
			else
				Entete += LibTerrain + "\t" + "Aucune cible";

			Entete += "\t";

			// Définition clef moteur
			CKeyMoteur KeyMoteur;
			KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),TerID,IdCible);

			// Calcul Effectif de la cible et nombre de cas
			JFlt64 Effectif;
			JInt32 NbCas;
			CString Txt;
			JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

			// Ajout info effectif et nb cas 
			Txt.Format("%0.f\t%d\n",Effectif,NbCas);
			Entete += Txt;
		}	
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Récupération des infos groupe
void CDlgCumulativite :: RecupContenuGroupe(CString &Entete)
{
	// Séparateur item
	CString SepItem = "\t";

	// Texte format
	CString Txt;

	// Bloc n° 4
    Entete = "BLOC 4\n";

	// Nb max courbes
	JUnt32 NbMaxCourbe = m_NbLigColor * m_NbColColor;
    
	// Balaye toutes les courbes possibles
	for (JUnt32 InxCourbe = 1; InxCourbe <= NbMaxCourbe; InxCourbe++)
	{
		// Flag 1er elmt courbe en cours
		JUnt32 NbSupp = 0;

		// Info courbe
		CString TxtCourbe;


		// Tableau des cumuls
        CArray <JFlt64,JFlt64> TCumul;
		TCumul.SetSize(m_GridCumulativite.GetColCount() - m_NoColPeriodicite + 1);

		int Taille = TCumul.GetCount(); 

		// Balaye toutes les lignes associées aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Positionne sur info map id support
			pDocChoixSupport->m_MapIdColorSupports.MoveTo(IdSupport);

			JUnt32 IndexColor = 0;
			if (pDocChoixSupport->m_MapIdColorSupports.IsValid()) 
			{
				// Voir si support appartient au graphe et indiquer sa couleur
				IndexColor = pDocChoixSupport->m_MapIdColorSupports.GetItem();
			}

			if (IndexColor == InxCourbe)
			{
				// Incrémente nb supports pour cette courbe
				NbSupp++;

				// Récupére le libellé support
				Txt.Format("%s%s",SepItem,m_GridCumulativite.GetValueRowCol(NoLig,m_NoColSupport-1)); 
				TxtCourbe += Txt;

				// Cumul des colonnes
				for (JUnt32 NoCol = m_NoColPeriodicite; NoCol <= m_GridCumulativite.GetColCount(); NoCol++)
				{
					// Ajoute valeur colonne
					Txt = m_GridCumulativite.GetValueRowCol(NoLig,NoCol);
					// Txt.Replace(",",".");
					TCumul[NoCol-m_NoColPeriodicite] += atof(Txt);
				}
			}
		}

		// Si 1er support pour cette courbe
		if (NbSupp > 0)
		{
			// Titre de la courbe
			Txt.Format("%s %d %s%d %s","Courbe n°",InxCourbe,SepItem,NbSupp,TxtCourbe);
			Entete += Txt;

			// Valeur des courbes
			for (JUnt32 NoCol = m_NoColPeriodicite; NoCol <= m_GridCumulativite.GetColCount(); NoCol++)
			{
				Txt.Format("%s%0.2f",SepItem, TCumul[NoCol-m_NoColPeriodicite]);
				Entete += Txt;
			}
			Entete += "\n";
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du grille cumulativité
bool CDlgCumulativite ::Clipboard()
{
	// Sélection globale du grid
	POSITION area = m_GridCumulativite.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridCumulativite.SetSelection(area, 0, 0, m_GridCumulativite.GetRowCount(), m_GridCumulativite.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridCumulativite.Copy(); 

	// Déselection globale
	m_GridCumulativite.SetSelection(NULL, 0, 0, 0, 0);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du graphique cumulativité
bool CDlgCumulativite ::ClipboardGraphique()
{
	// Export du graphique sous format clipboard
	if (!m_GraphCum.ExportGraph(NULL)) 
	{
		AfxMessageBox("Erreur lors de l'export clipboard graphique cumulativité");	
		return false;
	}
	else
		return true;
	
}

bool CDlgCumulativite ::PrintExcel()
{
	if(Clipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
		return true;

	}
	return false;
}

bool CDlgCumulativite ::OpenExcel()
{
	if(Clipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open( path, "DefinirTypeExportPresse");
		return true;

	}
	return false;
}

bool CDlgCumulativite ::SaveAsExcel(CString filePath)
{
	if(Clipboard())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.SaveAsFile(filePath, path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////
// Gestion des messages 
BOOL CDlgCumulativite::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		long ValParam = pMsg->wParam; 

		// Arrête tous les calculs
		m_StopCalcul = true;
		// return true;

		return JFCDialog::PreTranslateMessage(pMsg);
	}

	return JFCDialog::PreTranslateMessage(pMsg);
}

void CDlgCumulativite::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_ESCAPE:	
			m_StopCalcul = true;
			break;
	}
	JFCDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération sélection Nb Insertions
void CDlgCumulativite::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertionsCum = NbInsertions;

}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi sélection Nb Insertions
JUnt32 CDlgCumulativite:: GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertionsCum;
}
