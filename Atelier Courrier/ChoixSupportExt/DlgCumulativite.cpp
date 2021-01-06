//
// Fichier: DlgCumulativite.cpp.cpp
// Date:    02/04/2004
// Gestion Interface Cumulativit� dans choix Supports
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

// Bo�te de dialogue CDlgCumulativite
IMPLEMENT_DYNAMIC(CDlgCumulativite, JFCDialog)

CDlgCumulativite::CDlgCumulativite(CWnd* pParent, /*=NULL*/
								   JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgCumulativite::IDD, pParent), m_SplitHorz(WM_SPLIT2_HORZ)
{
	// R�cup�re pointeur s�lection g�n�rale
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

	// Envoi document � interface graphique
	m_IGraphCourbeCum.SetDocument(pDocChoixSupport);

	// On initilaise le pas graduation de la courbe cumulativit� � 7 jours
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


// Initialisation des diff�rents contr�les fen�tre Cumulativit�
BOOL CDlgCumulativite::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre cumulativit�
	SetupControls();

	// Informations g�n�rales en ent�te
	UpdateTitreAnalyse();

	// Mise � jour couleur graphe courante
	UpdateVisuColorGraphe();

	// Mise en place des relations objets fen�tre Cumulativit�
	SetWindowRelations();

	// on r�cup�re la position des fen�tres
	RECT RectL1; m_CadreCumulativite.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGrapheCum.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);

	// on calcule la configuration du split
	LONG SPUp = RectL1.bottom - RectL1.top; if (SPUp < 0) SPUp = 0;
	LONG SPDn = RectL2.bottom - RectL2.top; if (SPDn < 0) SPDn = 0;

	// on reconfigure le split
	m_SplitHorz.SetRange(SPDn, SPUp);

	// Init �tat calcul
	m_StopCalcul = false;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Gestion du d�placement split horizontal fen�tes cumulativit�
LRESULT CDlgCumulativite::OnSplitHorz(WPARAM WParam, LPARAM LPAram)
{
	// on r�cup�re le d�placement
	LONG Disp = (LONG)WParam;

	// on r�cup�re la position des fen�tres
	RECT RectL1; m_CadreCumulativite.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGrapheCum.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);
	RECT RectL3; m_GridCumulativite.GetWindowRect(&RectL3); this->ScreenToClient(&RectL3);
	RECT RectL4; m_GraphCum.GetWindowRect(&RectL4); this->ScreenToClient(&RectL4);
	RECT RectL5; m_ProgressResultCum.GetWindowRect(&RectL5);this->ScreenToClient(&RectL5); 
	RECT RectSP; m_SplitHorz.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);

	// Limite aux d�placements pour �viter des ab�rations graphiques
	// Hauteur minimale grid cumulativit�
	if (RectL1.bottom + Disp < 100) 
		Disp = 100 - RectL1.bottom;

	// Hauteur maximale grid cumulativit�
	if (RectL1.bottom + Disp > (RectL2.bottom - 100))  
		Disp = (RectL2.bottom - 100)  - RectL1.bottom;

	// on corrige la position des fen�tres (fen�tre grid r�sultat)
	RectL1.bottom += Disp;
	RectL3.bottom += Disp; 
	
	// on corrige la position des fen�tres (le graph cumulativit�)
	RectL2.top    += Disp;
	RectL4.top    += Disp;

	// la barre de progression
	RectL5.bottom += Disp;
	RectL5.top    += Disp;

	// La barre split
	RectSP.bottom += Disp; RectSP.top += Disp;

	// on d�place les fen�tres
	m_CadreCumulativite.SetWindowPos(NULL, RectL1.left, RectL1.top, RectL1.right - RectL1.left, RectL1.bottom - RectL1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GridCumulativite.SetWindowPos(NULL, RectL3.left, RectL3.top, RectL3.right - RectL3.left, RectL3.bottom - RectL3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER); 
	m_CadreGrapheCum.SetWindowPos(NULL, RectL2.left, RectL2.top, RectL2.right - RectL2.left, RectL2.bottom - RectL2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GraphCum.SetWindowPos(NULL, RectL4.left, RectL4.top, RectL4.right - RectL4.left, RectL4.bottom - RectL4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_ProgressResultCum.SetWindowPos(NULL, RectL5.left, RectL5.top, RectL5.right - RectL5.left, RectL5.bottom - RectL5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On d�place la barre split
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
// Mise en place des relations entre objets de la fen�tre Cumulativit�
void CDlgCumulativite::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Cadre englobant les �lmts r�sultats cumulativit�
			{IDC_CS_CADRE_CUMULATIVITE, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}},			//Bottom side

			// Titre Cumulativit� (Source + Source Audience)
			{IDC_CS_TITRE_CUMULATIVITE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
						  				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},						//Bottom side

			// Grille R�sultat Cumulativit�
			{IDC_CS_GRID_CUMULATIVITE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom}},	//Bottom side

			// Barre de progression
			{IDC_CS_PROGRESS_RESULTCUM,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
										{CResizer::eFixed, IDC_CS_CADRE_CUMULATIVITE, CResizer::eBottom}},	//Bottom side

			// Cadre englobant les �lmts r�sultats cumulativit�
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
// Set up de tous les contr�les fen�tre Cumulativit�
void CDlgCumulativite::SetupControls()
{
	// Association objet / ressource
	m_GridCumulativite.SubclassDlgItem(IDC_CS_GRID_CUMULATIVITE, this);

	// Propri�t�s du cadre Cumulativite
	m_CadreCumulativite.SetTransparent(false);
	m_CadreCumulativite.SetSunken(true);

	// Propri�t�s du cadre graphe Cumulativite
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

	// Progress bar non visible au d�part
	m_ProgressResultCum.ShowWindow(SW_HIDE); 
	m_ProgressResultCum.SetShowText(false);
 
	// Taille et Font des diff�rents labels
	m_TitreCumulativite.SetFontName(CATPColors::GetFontLabelName());
	m_TitreCumulativite.SetFontBold(TRUE); 
	m_TitreCumulativite.SetFontSize(CATPColors::GetFontLabelSize());
	
	
	// Initialise grid r�sultat cumulativit�
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

	// Enregistrement controle sp�cifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(&m_GridCumulativite);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridCumulativite.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Init R�f�rence colonne
	InitRefGrid();

	// Associe le document au grille
	if (m_GridCumulativite.OnInitDocument(pDocChoixSupport,this, m_NoColIdSupport) == false)
		AfxMessageBox ("Probl�me association document supports / grille cumulativit�");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid r�sultat
	if (GrilleResultValid())
	{
		if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		{
			// En mode moteur PROBA >> Nouvelle optim calcul cumul
			InitGridResultat_OPTIM();
		}
		else
		{
			// Autre mode moteur >> Calcul comme avant malgr� l'incoh�rence calcul en Betabinomial (voir Sylvain)
			InitGridResultat();
		}
	}

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des r�f�rences colonnes
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

	// Position colonne p�riodicit�
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColColorGraphe + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid
void CDlgCumulativite::InitGrid()
{
	// Enleve n� ligne col 
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

	// Position colonne p�riodicit�
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColColorGraphe + 1;

	// Limite maximum insertions selon p�riodicit�
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

		// Aucun header colonne et colonnes bloqu�s (support/tarif/p�riodicit�)
		m_GridCumulativite.SetFrozenCols(m_NoColPeriodicite - m_NoColIdSupport + 1,0);

		// Affichage entete Tarif et p�riodicit�
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
									 			.SetValue(_T("P�riodic.")));
		}

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Merge des cellules en header niveau 1
	m_GridCumulativite.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_GridCumulativite.GetColCount()),
					 		         CGXStyle()
										.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

	// Rend visible le grid, si au moins quelques r�sultats
	int NbCol = m_GridCumulativite.GetColCount();
	int NbRow = m_GridCumulativite.GetRowCount();
	if (m_GridCumulativite.GetColCount() >= m_NoColPeriodicite)
		m_GridCumulativite.ShowWindow(SW_SHOW);
	else
		m_GridCumulativite.ShowWindow(SW_HIDE);
}

// D�finition des couleurs interface fen�tre Cumulativit�
void CDlgCumulativite::SetColors()
{
	// Coloriage du cadre englobant
	m_CadreCumulativite.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultCum.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Titre Cumulativit�
	m_TitreCumulativite.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreCumulativite.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Grille r�sultat
	m_GridCumulativite.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridCumulativite.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Progress bar
	m_ProgressResultCum.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage du cadre englobant
	m_CadreGrapheCum.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::LIGHT_DEGRAD); 

	// Les diff�rentes couleurs pour headers ou cellules sp�cials
	UpdateColorSpe();

	// Style des lignes (solid pen)
	m_GridCumulativite.GetParam()->SetGridLineStyle(PS_SOLID); 

	// Style s�lection bloc (comme Excel)
	// m_GridCumulativite.GetParam()->SetExcelLikeSelectionFrame(TRUE);
}



// Destruction de la fen�tre Cumulativit�
void CDlgCumulativite::OnDestroy()
{
	JFCDialog::OnDestroy();
}

// Retaille de la fen�tre Cumulativit�
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

		// Graphe cumulativit�
		m_GraphCum.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	}
}

void CDlgCumulativite::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		// Mise � jour du Grid r�sultat et graphe Cumulativit�
		// AfxMessageBox("A FAIRE / CUMULATIVITE",MB_ICONINFORMATION);	
		
	}
}

void CDlgCumulativite::OnCancel()
{
	JFCDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections format supports
bool CDlgCumulativite::UpdateSelFormatSupports()
{
	// Initialise libell�s supports en 1ere colonne
	InitRowSupports();

	return true;
}

//////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgCumulativite::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	// R�initialisation g�n�rale
	else if (Message == UPDATE_RAZ)
	{
		// Initialise grid r�sultat
	    m_GridCumulativite.Initialize();
		GXSetNewGridLineMode(); 
		m_GridCumulativite.GetParam()->EnableUndo(FALSE);
		m_GridCumulativite.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridCumulativite.GetParam()->SetNumberedColHeaders(false); 
		m_GridCumulativite.GetParam()->SetNumberedRowHeaders(false); 
		m_GridCumulativite.GetParam()->SetSmartResize(true);

		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();

		// Fermeture du graphe
		UpdateGraphe();

	}

	//////////////////////////////////////////////////////////////////////////
	// Mise � jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();
	}

	else if (Message == UPDATE_OFFRE)
	{
		// Mise � jour du graphe
		UpdateGraphe();
	}

	// Mise � jour s�lection support
	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Initialisation du grid		
		InitGrid();
	}

	// Mise � jour s�lection segment
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Initialisation du grid		
		InitGrid();
	}

	// Mise � jour s�lection support
	else if (Message == UPDATE_SEL_COLOR_GRAPHSUPP)
	{
		// Update couleur graphe supports
		UpdateColorGraphSupports();

	}
	
	// Mise a jour suite � changement Format
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Puis on met � jour le map des formats supports dans cumulativit�
		UpdateFormatSupports();

		// Remet les s�lections format dans le grille
		UpdateSelFormatSupports();

		// Mise � jour des r�sultat co�ts (uniquement si fen�tre cumul visible)
		if (this->IsWindowVisible())
			UpdateResultCout();
		else
			pDocChoixSupport->m_ParamCumModif = true;
	}

	// Mise a jour suite � changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Mise � jour couleur s�lection supports
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
		// Efface les colonnes innoportunes, on le fait en r�affichant
		UpdateRow();
		UpdateCol();

		// R�affiche le r�sultat
		if (this->EnableWindow() == TRUE)
		{
			// Test si param�tre modifi�
			if (pDocChoixSupport->m_ParamCumModif)
			{
				// Calcul cumulativit�
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					InitGridResultat_OPTIM();
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgr� l'incoh�rence calcul en Betabinomial (voir Sylvain)
					InitGridResultat();
				}
			}
		}
		else
			pDocChoixSupport->m_ParamCumModif = true;

		// Mise � jour du bandeau informations
		UpdateTitreAnalyse();
	}

	// Mise � jour s�lection support
	else if (Message == UPDATE_SEL_COLOR)
	{
		// Update couleur graphe courante
		UpdateVisuColorGraphe();
	}

	// Mise � jour suite � modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();
	}

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Mise � jour des r�sultat co�ts
		UpdateResultat(false);
	}

	// Mise � jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise � jour grid cumulativit�
		m_GridCumulativite.LockUpdate(true);

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Active mise � jour grid cumulativit�
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativit�
		m_GridCumulativite.Redraw();
	}

	// Mise � jour des taris suite � modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise � jour grid mapping
		m_GridCumulativite.LockUpdate(true);

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Mise � jour du graphe
		UpdateGraphe();

		// Active mise � jour grid cumulativite
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativit�
		m_GridCumulativite.Redraw();
	}
	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise � jour grid Cumulativite
		m_GridCumulativite.LockUpdate(true);

		// Affichage ou non des supports dans grille Cumulativite
		UpdateAfficheSupports();

		// Active mise � jour grid Cumulativite
		m_GridCumulativite.LockUpdate(false);

		// redessine le grid Cumulativite
		m_GridCumulativite.Redraw();

		// redessine le graphe cumulativit�
		// m_GraphCum.RedrawWindow();
		UpdateGraphe();

	}
	// Mise � jour suite au changement nb insertions
	else if (Message == UPDATE_INSERTIONS_CUM)
	{
		// Vire le map courant r�sultat  ( A VOIR si on recalcul)
		// pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleCum);

		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Indique calcul � finaliser
		pDocChoixSupport->m_ParamCumModif = true;

		// R�affiche le r�sultat
		if (this->IsWindowVisible())
		{
			// Mise � jour du grid cumulativit�
			InitGrid();

			// Test si param�tre modifi�
			if (pDocChoixSupport->m_ParamCumModif)
			{
				// Calcul cumulativit�
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					InitGridResultat_OPTIM();
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgr� l'incoh�rence calcul en Betabinomial (voir Sylvain)
					InitGridResultat();
				}
			}

			// Init Affichage R�sultats Cellules
			// Suite � Optim 2.7

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
		// Active mise � jour grid cumulativit�
		m_GridCumulativite.LockUpdate(true);

		// On remet le tri � jour
		MajTriCourant();

		// Active mise � jour grid cumulativit�
		m_GridCumulativite.LockUpdate(false);

		// reddessine le grid cumulativit�
		m_GridCumulativite.Redraw();
	}
}	

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour du tri courant
JVoid CDlgCumulativite::MajTriCourant()
{
	// Retri avec les m�mes param�tres de tri
	m_GridCumulativite.SortColonneCourante(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgCumulativite::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Update R�sultat
	m_GridCumulativite.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// R�cup�ration validit� support sur terrain(s)/segment(s)
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

	// Mise � jour courbe cumulativit�
	m_GraphCum.RedrawWindow(); 
	m_GraphCum.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats supports dans cumulativit�
void CDlgCumulativite::UpdateFormatSupports()
{
	// on teste la validit� du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise � jour map des formats supports cumulativit�
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		// Mise � jour des formats par support
		pDocChoixSupport->m_MapIdFormatSpecifCum.Reset();

		// Boucle sur tous les supports
		for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
			 pDocChoixSupport->m_MapIdSupportsSel.IsValid();
			 pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
		{
			// Ident support
			JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

			// R�cup�re liste des idents formats possibles pour ce support (parmi les formats s�lectionn�s)
			JMap <JUnt32,JUnt32> MapIdFormats;
			pDocChoixSupport->GetListIdFormats(IdSupport,&MapIdFormats);

			// On prendra le format module pour ce support sauf s'il n'est pas dans la liste des formats support
			MapIdFormats.MoveTo(IdFormat);

			if (MapIdFormats.IsValid())
			{
				// Indique information format associ� � ce support
				pDocChoixSupport->m_MapIdFormatSpecifCum.Add(IdSupport) = IdFormat;	
			}
			else
				// Pas d'info format (sera s�lectionn� ult�rieurement via combo box)
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

	// Nb Header Colonnes (Support + P�riodicit�)
	m_NbHeaderCol = m_NoColPeriodicite - m_NoColIdSupport + 1;

	// Maintenant on tient compte directement du choix nb insertions dans menu
	// Nb Colonnes
	if (pDocChoixSupport != NULL && pDocChoixSupport->m_MapIdSupportsSel.GetCount() > 0)
	{
		// calcul max nb col, selon p�riodicit� des supports s�lectionn�s
		m_MaxColIns = 1;

		m_MaxColIns = pDocChoixSupport->m_NbInsertionsCum;

		// Nb col = 1 + Max colonnes insertions (1ere colonne = Max Asypmtote)
		m_NbCol = 1 + m_MaxColIns;
	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Bidouille pour diminuer le nombre de colonnes
	// On va de toutes mani�res jusqu'� 12
	/*
	if (m_NbCol > MaxInsMensuel && m_NbCol <=  MaxInsBiMensuel)
		// 1 colonne suppl�mentaire pour les mensuels
		// m_NbCol = 13;
		m_NbCol = MaxInsGeneral + 1;
	else if (m_NbCol > MaxInsBiMensuel && m_NbCol <= MaxInsHebdo)
		// 1 colonne suppl�mentaire pour les bi-mensuel
		// m_NbCol = 14;
		m_NbCol = MaxInsGeneral + 2;
	else if (m_NbCol > MaxInsHebdo && m_NbCol < MaxInsBiHebdo)
		// 1 colonne suppl�mentaire pour les hebdomadaires
		// m_NbCol = 15;
		m_NbCol = MaxInsGeneral + 3;
	else if (m_NbCol >= MaxInsBiHebdo && m_NbCol <= MaxInsQuotidien)
		// 1 colonne suppl�mentaire pour les bi-hebdo
		// m_NbCol = 16;
		m_NbCol = MaxInsGeneral + 4;
	else if (m_NbCol > MaxInsQuotidien)
		// 1 colonne supll�mentaire pour les quotidiens
		// m_NbCol = 17;
		m_NbCol = MaxInsGeneral + 5;
	*/

	if (m_NbCol > MaxInsMensuel && m_NbCol <=  MaxInsBiMensuel)
		// 1 colonne suppl�mentaire pour les mensuels
		m_NbCol = 13;
	else if (m_NbCol > MaxInsBiMensuel && m_NbCol <= MaxInsHebdo)
		// 1 colonne suppl�mentaire pour les bi-mensuel
		m_NbCol = 14;
	else if (m_NbCol > MaxInsHebdo && m_NbCol < MaxInsBiHebdo)
		// 1 colonne suppl�mentaire pour les hebdomadaires
		m_NbCol = 15;
	else if (m_NbCol >= MaxInsBiHebdo && m_NbCol <= MaxInsQuotidien)
		// 1 colonne suppl�mentaire pour les bi-hebdo
		m_NbCol = 16;
	else if (m_NbCol > MaxInsQuotidien)
		// 1 colonne supll�mentaire pour les quotidiens
		m_NbCol = 17;

	// Init grid avec nb colonnes
    if (!m_GridCumulativite.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
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
		// Ligne header niveau 0 r�serv� � la grandeur s�lectionn�e
		// Centrage et style texte header
		m_GridCumulativite.SetStyleRange(CGXRange(0, 0, 0, m_GridCumulativite.GetColCount()),
										 CGXStyle()
											.SetHorizontalAlignment(DT_CENTER)
											.SetVerticalAlignment(DT_VCENTER)
											.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL))
											.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontCell)
														.SetBold(pDocChoixSupport->m_BoldFontCell)));


		// Texte associ� au header ligne
		CString Txt = "";
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		{
			JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem();
			Txt = pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, IdGrandeur);
		}

		// Affichage Grandeur
		m_GridCumulativite.SetValueRange(CGXRange(0, m_NoColPeriodicite, 0, m_GridCumulativite.GetColCount()),Txt); 

		// Affichage des headers infos insertions selon p�riodicit�
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

				// Texte associ� au header la cellule
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

		// Couleurs sp�ciales
		UpdateColorSpe();

		// Regroupement des lignes header row
		m_GridCumulativite.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
		m_GridCumulativite.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridCumulativite.GetColCount()),
									     CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));

		// Update Grille modifi�e
		m_GridCumulativite.UpdateData(TRUE);

		// Regroupement cellule
		/*
		m_GridCumulativite.UpdateData(); 
		m_GridCumulativite.SetMergeCellsMode(gxnMergeEvalOnDisplay);
		m_GridCumulativite.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridCumulativite.GetColCount()),
							 			 CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));
		*/

		// Mise � jour
		m_GridCumulativite.UpdateWindow(); 
		m_GridCumulativite.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update des r�sultats au niveau cellule
bool CDlgCumulativite::UpdateResultat(JBool CalculResult)
{
	if (m_GridCumulativite.GetColCount() > 0 && m_GridCumulativite.GetRowCount() > 0)
	{
		// Delock mode read only
		m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

		// Init Affichage R�sultats Cellules
		int NbCol = m_GridCumulativite.GetColCount();
		m_GridCumulativite.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridCumulativite.GetRowCount(),m_GridCumulativite.GetColCount()),
								CGXStyle()
									.SetHorizontalAlignment(DT_RIGHT)
									.SetVerticalAlignment(DT_BOTTOM)
									.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

		// Test si grille r�sultat valide
		if (GrilleResultValid())
		{
			// Si calcul demand� et modif param�tre
			if (CalculResult && pDocChoixSupport->m_ParamCumModif)
			{
				if (pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				{
					// En mode moteur PROBA >> Nouvelle optim calcul cumul
					if (!InitGridResultat_OPTIM()) return false;
				}
				else
				{
					// Autre mode moteur >> Calcul comme avant malgr� l'incoh�rence calcul en Betabinomial (voir Sylvain)
					if (!InitGridResultat()) return false;
				}
			}
			else
			{
				// Affichage des r�sultats cellules
				if (!AfficheResultat()) 
					return false;
			}
		}		

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lection supports
void CDlgCumulativite::UpdateEtatSelSupports()
{
	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat s�lection en cours
			CGXStyle Style;
			m_GridCumulativite.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 

			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la s�lection support
				if (ValSel != 1)
				{
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
												     CGXStyle()
														.SetValue((long)1));
				}
			}
			else
			{
				// D�valide la s�lection support
				if (ValSel != 0)
				{
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
												     CGXStyle()
														.SetValue((long)0));
				}
			}
		}
	}	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgCumulativite::UpdateVisuSupports()
{
	// Attente mise � jour
	m_GridCumulativite.LockUpdate(true);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Debut et fin marquage s�lection
	JUnt32 NoDebCol = m_NoColSupport-1;
	JUnt32 NoFinCol = m_GridCumulativite.GetColCount();

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Support s�lectionn�, on surligne la ligne
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig, NoDebCol, NoLig, NoFinCol),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
			}
			else
			{
				// Support non s�lectionn�, on laisse la ligne en blanc
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig, NoDebCol, NoLig, NoFinCol),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
			}
		}
	}

	// Mise � jour couleur supports pour graphe
	// Balaye toutes les lignes associ�es aux supports
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
			// R�cup�re la couleur
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

	// Mise � jour graphe cumulativit�
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
			// R�cup�re �tat s�lection support
			bool EtatSel = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

			// Maximum insertions possible sur ce support
			JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n� de colonne
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
					// Couleur sp�ciale pour indiquer max cumulativit�
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
														.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
				else
				{
					// Seuil maxi non atteint
					if (EtatSel)
						// Indique couleur cumulativit� progressive avec support s�lectionn�
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetEnabled(false)
															.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
					else
						// Indique couleur cumulativit� progressive avec support non s�lectionn�
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetEnabled(false)
															.SetInterior(CATPColors::GetColorWhite()));

				}

				// Affichage des r�sultats suivants directement si limite insertion atteinte
				if (NbInsertions >= MaxInsSupport && NoCol != m_NbHeaderCol )
				{
					// Indique en couleur limite atteinte pour calcul cumulativit�
					m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,NoCol),
														CGXStyle()
														.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
				}
			}
		}
	}
	
	// Voir pour les supports cach�s
	UpdateAfficheSupports();

	// Attente mise � jour
	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// On bloque les colonnes qui ne doivent pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update couleur graphe associ� aux supports
void CDlgCumulativite::UpdateColorGraphSupports()
{
	// Attente mise � jour
	m_GridCumulativite.LockUpdate(true);

	// Delock mode read only
	m_GridCumulativite.GetParam()->SetLockReadOnly(FALSE);

	// Mise � jour couleur supports pour graphe
	// Balaye toutes les lignes associ�es aux supports
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
			// R�cup�re la couleur
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


	// Mise � jour graphe cumulativit�
	UpdateGraphe();
	
	// Attente mise � jour
	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update couleur courante pour graphe cumulativit�
void CDlgCumulativite::UpdateVisuColorGraphe()
{
	// Bloque modif grid
	m_GridCumulativite.LockUpdate(true); 

	// Nombre de lignes
    JUnt32 NoLig = 0;
	if (m_GridCumulativite.GetRowCount()) NoLig = m_NbHeaderRow - 1;

	if (pDocChoixSupport->m_IdColorCum > 0)
	{
		// Information couleur courante en entete colonne s�lection
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

	// Mise � jour trac� du graphe
	m_GraphCum.SetItems(&m_IGraphCourbeCum,TRUE); 

}

////////////////////////////////////////////////////////////////////////////////////////////
// Maximum insertions sur les supports trait�s
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
// Initialisation du r�sultat cumulativit�
JBool CDlgCumulativite::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridCumulativite.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultCum KeyResultCum;

	// Information r�sultat archiv�
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

	// Calcul indice cl� segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice cl� moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Max Insertions sur tous les supports
	// JUnt32 MaxInsAllSupp = MaxInsertionsAllSupports();
	JUnt32 MaxInsAllSupp = pDocChoixSupport->m_NbInsertionsCum;

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Test si on continue la suite des calculs cumulativit�
		if (m_StopCalcul)
			break;
		
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re le format support s�lectionn� pour la cumulativit�
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();
	
		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// R�cup�re Tarif item selon format
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

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace � gauche pour pr�sentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Ins�re dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain s�lectionn�
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain s�lectionn�
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible s�lectionn�e
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible s�lectionn�e
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// Cl� de recherche r�sultat
		KeyResultCum.m_IdCible			= IdCible; 
		KeyResultCum.m_IdTerrain		= IdTerrain;
		KeyResultCum.m_IdSupport		= IdSupport; 
		KeyResultCum.m_NbInsert         = 1;

		// Test si �l�ment d�j� calcul�
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
		if (!pDocChoixSupport->m_MapResultatCum.IsValid())
		{
			// ----------------------------------- Calcul � 1 insertion ------------------------------
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

			// Flag Audience D�fini
			JBool AudienceDefini = false;
			
			// Test si la cible est autoris�e
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
					// Cr�ation Distribution
					JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

					// R�cup�re audience
					Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  

					// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
					// et qui pourtant renvoie une distribution de fr�quence = 0
					if (AudProba1Ins < 0) Audience1Ins = AudProba1Ins;
				}
			}

			// Balayage des colonnes insertions
			// Attention 13 > correspond �  24 insertions	(pour les bi-mensuels)
			//           14 > ............  53 ..........	(pour les hebdomadaires)
			//           15 > ............ 105 ..........	(pour les bi-hebdomadaires)
			//           16 > ............ 365 ..........	(pour les quotidiens)
			JUnt32 NbCol = m_GridCumulativite.GetColCount();

			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n� de colonne
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

				// Calcul si nb insertions ne d�passe pas le maximum autoris�
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
						// On fabrique la maille � n insertions
						JFCMaille Maillen;
						pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maillen,IdSupport,NbInsertions);  // false; 

						// Construction de la distribution sur ces n insertions
						// A REMETTRE // A OPTIMISER = TRES LONG !!!!!!!!!!!!!!!!!!!!!
						pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maillen);
						//.................................................

						// Direct on prend la distri � n insertions
						JFCDistribution Distri = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLecture); 

						// R�cup�re audience
						Audience = Distri.Item(1).AsJFlt64();  

						// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
						// et qui pourtant renvoie une distribution de fr�quence = 0
						if (AudProba1Ins < 0) Audience = AudProba1Ins;
					}

					// R�cup id support
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

					// Calcul R�sultat Colonne (selon grandeur)
					ResultCum.m_Audience			= Audience; 
					ResultCum.m_AudProba1Ins		= AudProba1Ins;
					if(!pElem->IsCouplage())
						// Cas de titre simple, audience 1 insertion = audience LDP
						ResultCum.m_Aud1Ins			= AudProba1Ins;
					else
						// Cas de couplage, on prend audience 1 insertion (calcul� � partir de Distri(1)
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
					// Stockage r�sultat dans matrice
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
						// Cas de couplage, on prend audience 1 insertion (calcul� � partir de Distri(1)
						ResultCum.m_Aud1Ins		= Audience1Ins;
					
					ResultCum.m_Effectif			= Effectif;
					ResultCum.m_NbCas				= NbCas;
					ResultCum.m_VolLecture			= VolLecture;
					ResultCum.m_PtEtalonnage		= PtEtal;
					*/

					// Ajoute nouveau r�sultats
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
						// Couleur sp�ciale pour indiquer max cumulativit�
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
						// Indique couleur cumulativit� progressive
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
														CGXStyle()
															.SetInterior(CATPColors::GetColorWhite()));

					// Affichage des r�sultats suivants directement si limite insertion atteinte
					if (NbInsertions == MaxInsSupport && NoCol != m_NbHeaderCol )
					{
						// Ecriture dans toutes les cellules suivantes (m�me support)
						for (int i = NoCol + 1; i <= m_GridCumulativite.GetColCount(); i++) 
						{
							// M�me valeur qu'avec max insertion
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

						// Indique en couleur limite atteinte pour calcul cumulativit�
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
			// Attention 13 > correspond �  24 insertions
			//           14 > ............  53 ..........
			//           15 > ............ 105 ..........
			//           16 > ............ 365 ..........
			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n� de colonne
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

				// Calcul si nb insertions ne d�passe pas le maximum autoris�
				if (NbInsertions <= MaxInsSupport)
				{
					// R�cup�re r�sultat
					KeyResultCum.m_NbInsert         = NbInsertions;
					pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

					if (pDocChoixSupport->m_MapResultatCum.IsValid())
					{
						ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 

						// Calcul R�sultat Colonne (selon grandeur)
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
							// Couleur sp�ciale pour indiquer max cumulativit�
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
															CGXStyle()
																.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
						else
							// Indique couleur cumulativit� progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));

						// Affichage des r�sultats suivants directement si limite insertion atteinte
						if (NbInsertions == MaxInsSupport && NoCol != m_NbHeaderCol )
						{
							// Ecriture dans toutes les cellules suivantes (m�me support)
							for (int i = NoCol + 1; i <= m_GridCumulativite.GetColCount(); i++) 
							{
								// M�me valeur qu'avec max insertion
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

							// Indique en couleur limite atteinte pour calcul cumulativit�
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

	// Retaille largeur colonnes supports/format/p�riodicit�
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

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Flag calcul effectu�
	pDocChoixSupport->m_ParamCumModif = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du r�sultat cumulativit�
JBool CDlgCumulativite::InitGridResultat_OPTIM_OLD()
{
	// Remette code avant 24 Decembre 2005
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// V�rifie si tous les calculs effectu�es
JBool CDlgCumulativite::AuMoins1ResultatNonCalcule(CKeyResultCum KeyResultCum, JUnt32 MaxInsSupport)
{
	// Resultat non calcul�
	bool ResultatNonCalcule = false; 

	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
	{
		// Nombre d'insertions via n� de colonne
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

		// Test si �l�ment d�j� calcul�
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

		// Validation des r�sultats pr�c�dents
		if (!pDocChoixSupport->m_MapResultatCum.IsValid())
		{
			ResultatNonCalcule = true;
			break;
		}
	}
	
	return ResultatNonCalcule;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du r�sultat cumulativit�
JBool CDlgCumulativite::InitGridResultat_OPTIM()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridCumulativite.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultCum KeyResultCum;
	CKeyResultCum KeyResultCumMaxIns;

	// Information r�sultat archiv�
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

	// Calcul indice cl� segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice cl� moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	JUnt32 NbCol = m_GridCumulativite.GetColCount();

	// Max Insertions sur tous les supports (en tenant compte aussi du choix menu)
	JUnt32 MaxInsAllSupp = MaxInsertionsAllSupports();

	if (pDocChoixSupport->m_NbInsertionsCum > MaxInsAllSupp)
		MaxInsAllSupp = pDocChoixSupport->m_NbInsertionsCum;

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Test si on continue la suite des calculs cumulativit�
		if (m_StopCalcul)
			break;
		
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re le format support s�lectionn� pour la cumulativit�
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();
	
		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// R�cup�re Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);	
		}

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace � gauche pour pr�sentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Ins�re dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain s�lectionn�
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain s�lectionn�
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible s�lectionn�e
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible s�lectionn�e
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// Cl� de recherche r�sultat (la 1ere correspondant � 1 insertion)
		KeyResultCum.m_IdCible			= IdCible; 
		KeyResultCum.m_IdTerrain		= IdTerrain;
		KeyResultCum.m_IdSupport		= IdSupport; 
		
		// Test si au moins 1 r�sultt non calcul�
		bool AllNonValid = false;
		AllNonValid =AuMoins1ResultatNonCalcule(KeyResultCum, MaxInsSupport);

		// Puis � 1 insertion
		KeyResultCum.m_NbInsert         = 1;

		// La derni�re correpondant au max insertion via menu insertion
		KeyResultCumMaxIns				= KeyResultCum; 
		KeyResultCumMaxIns.m_NbInsert   = pDocChoixSupport->m_NbInsertionsCum;

		// Test si �l�ment d�j� calcul�
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

		// Validation des r�sultats pr�c�dents
		bool Valid1Ins = pDocChoixSupport->m_MapResultatCum.IsValid();

		// Test si �l�ment d�j� calcul�
		pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCumMaxIns); 

		// Validation des r�sultats pr�c�dents
		bool ValidMaxIns = pDocChoixSupport->m_MapResultatCum.IsValid();

		if (!Valid1Ins || !ValidMaxIns || AllNonValid)
		{
			// ----------------------------------- Calcul � 1 insertion ------------------------------
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

			// Flag Audience D�fini
			JBool AudienceDefini = false;

			// Test si la cible est autoris�e
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
					// Cr�ation Distribution
					if (VolLect1Ins >= 0)
					{
						JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

						// R�cup�re audience
						Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  
					}

					// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
					// et qui pourtant renvoie une distribution de fr�quence = 0
					if (AudProba1Ins < 0) Audience1Ins = AudProba1Ins;
				}

			}

			// Construction de la distribution sur ces n insertions
			// pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur,Maille1Ins,MaxInsAllSupp);
			// OPTIM 2.7
			// pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur, Maille1Ins, MaxInsAllSupp);
			pDocChoixSupport->m_MoteurPresse.BuildTabDistributions(KeyMoteur, Maille1Ins, MaxInsSupport);

			// Calcul distribution en prenant � chaque fois 1 insertion, puis 2 insertions, puis 3 insertions etc... 
			JArray <JFCDistribution> TDistri_NIns;
			pDocChoixSupport->m_MoteurPresse.GetTabDistribution(pDocChoixSupport->m_MoteurPresse,MaxInsAllSupp, TDistri_NIns, MaxInsSupport); 
			// pDocChoixSupport->m_MoteurPresse.GetTabDistribution(pDocChoixSupport->m_MoteurPresse,MaxInsSupport, TDistri_NIns); 

			// R�cup�re audience
			Audience = TDistri_NIns.Item(1).Item(1).AsJFlt64();  

			// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
			// et qui pourtant renvoie une distribution de fr�quence = 0
			if (AudProba1Ins < 0) Audience = AudProba1Ins;

			// Balayage des colonnes insertions
			// Attention 13 > correspond �  24 insertions	(pour les bi-mensuels)
			//           14 > ............  53 ..........	(pour les hebdomadaires)
			//           15 > ............ 105 ..........	(pour les bi-hebdomadaires)
			//           16 > ............ 365 ..........	(pour les quotidiens)
			JUnt32 NbCol = m_GridCumulativite.GetColCount();

			// R�cup id support
			CRegroupements * pReg = CRegroupements::GetInstance();
			IElem *pElem = pReg->GetIElemByID(IdSupport);

			if (pElem)
			{
				JUnt32 NbColEff = m_GridCumulativite.GetColCount();

				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
				{
					// Nombre d'insertions via n� de colonne
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

					// Appel r�sultat avec nb insertions (dans le cas o� d�j� calcul�)
					KeyResultCum.m_NbInsert         = NbInsertions;
					
					// Test si �l�ment d�j� calcul�
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
								// R�cup�re audience 1 constact et + pour nb insertions
								Audience = TDistri_NIns.Item(NbInsertions-1).Item(1).AsJFlt64();  

								// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
								// et qui pourtant renvoie une distribution de fr�quence = 0
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
							// Stockage r�sultat dans matrice
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
								// Cas de couplage, on prend audience 1 insertion (calcul� � partir de Distri(1)
								ResultCum.m_Aud1Ins		= Audience1Ins;
							
							ResultCum.m_Effectif			= Effectif;
							ResultCum.m_NbCas				= NbCas;
							ResultCum.m_VolLecture			= VolLecture;
							ResultCum.m_PtEtalonnage		= PtEtal;
						}
						else
						{
							// Si quotidien on calculera tous les pas de 365 jours � 22 jours avec pas de 10
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
									// R�cup�re audience 1 constact et + pour nb insertions
									Audience = TDistri_NIns.Item(NbInsertionCur-1).Item(1).AsJFlt64();  

									// *** Bidouille en attendant de r�soudre le probl�me des LNMs non d�finis
									// et qui pourtant renvoie une distribution de fr�quence = 0
									if (AudProba1Ins < 0) Audience = AudProba1Ins;
								}

								// R�cup id support

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
								// Stockage r�sultat dans matrice
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
									// Cas de couplage, on prend audience 1 insertion (calcul� � partir de Distri(1)
									ResultCum.m_Aud1Ins		= Audience1Ins;
								
								ResultCum.m_Effectif			= Effectif;
								ResultCum.m_NbCas				= NbCas;
								ResultCum.m_VolLecture			= VolLecture;
								ResultCum.m_PtEtalonnage		= PtEtal;

								// Ajoute nouveau r�sultats
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
						// R�cup�ration R�sultat d�j� existant
						ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 
					}

					// Calcul R�sultat Colonne (selon grandeur)
					CString Txt;
					if(!pElem->IsCouplage())
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);
					else
						Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);

					// Ajoute nouveau r�sultats
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
						// Couleur sp�ciale pour indiquer max cumulativit�
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
					{
						if (NbInsertions < MaxInsSupport)
						{
							// Indique couleur cumulativit� progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));
						}
						else
						{
							// Indique en couleur limite atteinte pour calcul cumulativit�
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
			// Attention 13 > correspond �  24 insertions
			//           14 > ............  53 ..........
			//           15 > ............ 105 ..........
			//           16 > ............ 365 ..........
			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
			{
				// Nombre d'insertions via n� de colonne
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

				// R�cup�re r�sultat
				KeyResultCum.m_NbInsert         = NbInsertions;
				pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 

				if (pDocChoixSupport->m_MapResultatCum.IsValid())
				{
					// R�cup�ration R�sultat
					ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem(); 

					// Calcul R�sultat Colonne (selon grandeur)
					CString Txt;
					Txt = ResultCellule(NoLig, NoCol, Tarif, NbInsertions, ResultCum);

					// Sauvegarde valeur maximal
					CResultCum	ResultCumMaxInsert;
					if (NbInsertions == MaxInsSupport)
						ResultCumMaxInsert = ResultCum;

					// Indication maximum atteint
					if (NoCol == m_NbHeaderCol)
						// Couleur sp�ciale pour indiquer max cumulativit�
						m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,m_GridCumulativite.GetColCount()-1),
														CGXStyle()
															.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
					else
					{
						if (NbInsertions < MaxInsSupport)
						{
							// Indique couleur cumulativit� progressive
							m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
															CGXStyle()
																.SetInterior(CATPColors::GetColorWhite()));
						}
						else
						{
							// Indique en couleur limite atteinte pour calcul cumulativit�
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
				// R�cup�ration R�sultat d�j� existant
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

	// Retaille largeur colonnes supports/format/p�riodicit�
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

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Flag calcul effectu�
	pDocChoixSupport->m_ParamCumModif = false;

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Affichage r�sultat cumulativit�
JBool CDlgCumulativite::AfficheResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridCumulativite.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultCum KeyResultCum;

	// Information r�sultat archiv�
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

	// Calcul indice cl� segment
	KeyResultCum.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleCum); 

	// Calcul indice cl� moteur
	KeyResultCum.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 
	
	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridCumulativite.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultCum.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridCumulativite.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re le format support s�lectionn� pour la cumulativit�
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();

		// Maximum insertions possible sur ce support
		JUnt32 MaxInsSupport = pDocChoixSupport->GetMaxInsertions(IdSupport);

		// R�cup�re Tarif item selon format
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

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute espace � gauche pour pr�sentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Insere dans grille
		m_GridCumulativite.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Recup 1er terrain s�lectionn�
		JUnt32 IdTerrain;
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
		{
			// au moins 1 terrain s�lectionn�
			IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
		}
		else
			return false;

		// Recup 1ere cible s�lectionn�e
		JUnt32 IdCible;
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
		{
			// au moins 1 cible s�lectionn�e
			IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
		}
		else
			return false;

		// A FAIRE R�cup�ration des segments population
		// @@@ SEGPOP

		// Balayage des colonnes insertions
		// Attention 13 > correspond �  24 insertions
		//           14 > ............  53 ..........
		//           15 > ............ 105 ..........
		//           16 > ............ 365 ..........
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridCumulativite.GetColCount(); NoCol++) 
		{
			// Nombre d'insertions via n� de colonne
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

			// Calcul si nb insertions ne d�passe pas le maximum autoris�

			// Recherche R�sultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur r�sultats
			KeyResultCum.m_IdSupport = IdSupport;
			KeyResultCum.m_IdTerrain = IdTerrain;
			KeyResultCum.m_IdCible   = IdCible;
			KeyResultCum.m_NbInsert  = NbInsertions;

			CResultCum	ResultCumMaxInsert;
			pDocChoixSupport->m_MapResultatCum.MoveTo(KeyResultCum); 
			if (pDocChoixSupport->m_MapResultatCum.IsValid())
			{
				// Information r�sultat archiv�
				CResultCum	&ResultCum = pDocChoixSupport->m_MapResultatCum.GetItem();

				// Le r�sultat pour la cellule
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
				// Couleur sp�ciale pour indiquer max cumulativit� et au del�
				m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
													CGXStyle()
													.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));
			else
				// Indique couleur cumulativit� progressive
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

	// Retaille largeur colonnes supports/p�riodicit�
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

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridCumulativite.LockUpdate(false);
	m_GridCumulativite.Redraw();

	// Update visualisation supports
	UpdateVisuSupports();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// R�sultat cellule selon grandeur analyse demand�e
// "Audience"								------------ Audience (Nb lecteurs)
// "Audience (000)"							------------ Audience (Nb lecteurs en milliers)
// "Nb Contacts (000)"						------------ Nombre de contacts en milliers
// "GRP [+]"								------------ GRP Actif	
// "Couverture cumul�e"						------------ Couverture cumul�e en %
// "R�p�tition"								------------ R�p�tition
// "R�p�tition"								------------ R�p�tition
// "R�p�tition[+]"							------------ R�p�tition[+}
// "Cumulativit�"							------------ Cumulativit�
// "Co�t/%Couverture"						------------ Co�t en % de couveture
//
CString CDlgCumulativite::ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JInt32 NbInsertions, CResultCum &ResultCum)
{

	// Par d�faut Texte
	CGXStyle style;
	m_GridCumulativite.SetStyleRange(CGXRange(NoLig,NoCol),
								     CGXStyle()
									   .SetFormat(GX_FMT_TEXT)
									   .SetValueType(GX_VT_STRING));
	
	// Chaine formatt�e par d�faut
	CString TxtResult = "*";

	// Chaine stop calcul cumul
	CString TxtStopResult = "-";

	// R�cup�re index grandeur
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

			// D�coupage chaine par milliers
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

			// D�coupage chaine par milliers
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

				// D�coupage chaine par milliers
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
				// Calcul r�p�tition
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
				// Calcul r�p�tition +
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

					// Affichage selon nb d�cimales demand�es
					if (pDocChoixSupport->m_NbDecCout == 1)
						TxtResult.Format("%0.1f",Cout);
					else if (pDocChoixSupport->m_NbDecCout == 2)
						TxtResult.Format("%0.2f",Cout);
					else 
						TxtResult.Format("%0.f",Cout);

					// D�coupage chaine par milliers
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
	
	// Renvoi la chaine format�e
	return TxtResult;
}

////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections supports
bool CDlgCumulativite::UpdateSelSupports()
{
	// Nombre de supports nouvelle s�lection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRow();

		// Initialise libell�s supports en 1ere colonne
		InitRowSupports();

		// Mise � jour du grid
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur pr�c�demment existant, 
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

	// R�cup�re objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Style check-box libell�s supports
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1,InxLigSupp + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColSelSupport-1),
									 CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

	// Style texte libell�s supports
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
	// Style texte libell�s formats
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColFormat-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColFormat-1),
									 CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));
	*/
	
	// Style texte libell�s p�riodicit�
	m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp-1,m_NoColPeriodicite-1,InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,m_NoColPeriodicite-1),
									 CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)
										.SetItalic(FALSE)));

	// Balaye toutes les lignes associ�es aux supports
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// Ident support
		JUnt32 IdSupport  = pDocChoixSupport->m_Offre.m_TBLIdSupports.GetItem();
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// V�rifie qu'il est valide dans le map s�lection
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


			// on r�cup�re le libell� support
			CString LibSupport = "-";
			if (pSupport)
				// Affichage libell� support
				LibSupport = pSupport->GetLabel().AsJCharPtr();

			// Affichage libell� support
			m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibSupport));

			// R�cup�re info formats support
			CString ListFormat		= "";
			CString LibFormatNull	= "*";
			long InxSelFormat		= -1;
			pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleCum);
				
			// Puis indication support s�lectionn� ou pas
			if (EtatSupport)
			{
				// Valide s�lection support
				m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
			}
			else
			{
				// D�valide la s�lection support
				m_GridCumulativite.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
			}

			// Incr�mente index ligne
			InxLigSupp++;
		}	
	}

	// Balaye toutes les lignes associ�es aux familles
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// A FAIRE - FAMILLES
	}

	// Colonne Id Support invisible
	m_GridCumulativite.SetColWidth(m_NoColIdSupport-1, m_NoColIdSupport-1,0);

	// Retaille la colonne s�lection supports
	m_GridCumulativite.SetColWidth(m_NoColSelSupport-1, m_NoColSelSupport-1,20);

	// Retaille la colonne couleur graphe
	m_GridCumulativite.SetColWidth(m_NoColColorGraphe-1, m_NoColColorGraphe-1,16);

	// Hauteur des lignes
	JUnt32 NbRow = m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount();
	// m_GridCumulativite.SetRowHeight(0,m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,12,0,GX_SMART);

	// Mise � jour couleur s�lection supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridCumulativite.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs sp�ciales de certaines cellules
void CDlgCumulativite::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne r�sultat
	// Cellules info insertion en fond blanc et couleur caract�re diff�rent
	if (m_GridCumulativite.GetColCount() >= m_NoColPeriodicite)
	{
		m_GridCumulativite.LockUpdate(true); 

		// Cellules info moteur en fond blanc et couleur caract�re diff�rent
		m_GridCumulativite.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 0, m_NoColPeriodicite-1),
											CGXStyle()
											.SetInterior(CATPColors::GetColorWhite())
											.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Niveau 0 / Header info grandeur de couleurs diff�rente
		m_GridCumulativite.SetStyleRange(CGXRange(0, m_NoColPeriodicite, 0, m_GridCumulativite.GetColCount()),
											CGXStyle()
											.SetInterior(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));

		m_GridCumulativite.LockUpdate(false); 
		m_GridCumulativite.Redraw(); 
	}		
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Update grille r�sultat via new sel Terrains / cibles ou format
bool CDlgCumulativite::UpdateResultCum(JBool CalculResult)

{
	// Test validit� s�lection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si s�lection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// D�sactive mise � jour grid cum
	m_GridCumulativite.LockUpdate(true);

	// Mise � jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise � jour nombre et headers rows
	if (!UpdateRow()) return false;

	// Update affichage infos analyse
	UpdateTitreAnalyse();

	// Calcul des r�sultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Bloque resize hauteur des lignes
	m_GridCumulativite.GetParam()->EnableTrackRowHeight(FALSE);

	// Active mise � jour grid cum
	m_GridCumulativite.LockUpdate(false);

	// redessine le grid cum
	m_GridCumulativite.Redraw();

	// Rend visible le grid, si au moins quelques r�sultats
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
	CString Titre = "Analyse non d�finie";

	// Test si analyse d�finie
	if (pDocChoixSupport)
	{
		// Init Texte Info
		Titre = "";

		// Ajoute Info Filtre d'audience
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

		// R�cup Id Filtre audience du module courant
		JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

		// R�cup�re label 
		Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
		
		// On v�rifie qu'une source terrain a �t� choisi, et auusi une date tarification
		JUnt32 SrcID = 0;
		if (pDocChoixSupport!=0 && pDocChoixSupport->IsValid())    
		{
			// Ajoute libell� de la source
			if (pDocChoixSupport->GetIdSrcAudienceSel())
			{

				// Ajoute Info Filtre d'audience
				JSRCPresse* m_srcPresse;
				m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

				// R�cup Id Filtre audience du module courant
				JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

				// R�cup�re label 
				// Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);

				// R�cup Table
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();

				// R�cup la source audience courante
				SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

				// R�cup le nouveau source
				const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

				// Recuperer dans table source
				// Titre += " - ";
				// Titre += pt->m_Libelle.AsJCharPtr();

				// Ajoute Info Terrain
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
				{
					// R�cup�re ident terrain
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
				// R�cup�re ident cible
				JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

				// R�cup libell� 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
				{
					// Le libell� cible
					Titre += pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();

					// ...... son effectif
					// R�cup Table des cibles
					// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

					// R�cup cible correspondant � l'indice
					// IItem * pItem = pItemConst->GetItemConstruit(IdCible);
					IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

					// Si cible trouv�, on retourne son libell�
					if(pItem)
					{
						// R�cup�re effectif cible courante
						JUnt32 EffectifCible = 0;
					
						// on cr�e la cl� des effectifs
						CKeyMoteur keyMoteur;
						
						// R�cup terrain s�lectionn�
						JUnt32 IdTerrain = 0;
						pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
						if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
							// au moins 1 terrain s�lectionn�
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

			// Ajoute R�f�rence Affinit�
			// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
			// IItem * pItem = pItemConst->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);
			IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

			Titre += " - R�f.Aff : ";
			if(pItem)
				// Ajoute et s�lection d'office du seul indice affinit� (pour le moment)
				Titre += pItem->GetLabel().AsJCharPtr();
			else
				Titre += "Aucune";


			/*
			// R�cup id cible
			JUnt32 Id = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
			if(Id)
			{
				// R�cup Table des cibles
				CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

				// R�cup cible correspondant � l'indice
				IItem * pItem = pItemConst->GetItemConstruit(Id);

				// Si cible trouv�, on retourne son libell�
				if(pItem)
				{
					// Recuperer libell� cible en cours
					Text += " - ";
					Text += pItem->GetLabel().AsJCharPtr();

					// R�cup�re effectif cible courante
					JFlt64 EffectifCible = 0.0;
					
					// on cr�e la cl� des effectifs
					CKeyMoteur keyMoteur;
					// on initialise la cl�
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
				// Indx Grandeur Analyse S�lectionn�e
				JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

				// Ajoute au titre globale
				Titre += pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, IdGrandeur);
			}
			else
				// Aucune analyse en cours
				Titre += "Aucune analyse";
			*/

		}

		// Ecrit le titre analyse non d�finie
		m_TitreCumulativite.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridCumulativite.GetRowCount() > 1)
		{
			// Nom du moteur utilis�
			CString TxtMoteur;
			switch (pDocChoixSupport->m_MoteurPresse.GetType())
			{
				case CMoteurPresse::ABAQUE:
					// M�thode macro-probabiliste (JFC)
					TxtMoteur.LoadString(IDS_CS_MOTABAQUE);
					break;
				case CMoteurPresse::ABAQUE_ETAL:
					// M�thode macro-probabiliste �talonn�e (JFC)
					TxtMoteur.LoadString(IDS_CS_MOTABAQUEETAL);
					break;
				case CMoteurPresse::PROBA:
					// M�thode probabilit�s individuelles (idem SMP-PRESSE)
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
// D�finit si la grille r�sultat est valide
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

	// Pas de grille r�sultat d�finie
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
void CDlgCumulativite::UpdateResultCout()
{
	// Mise � jour fen�tre cumulativit�
	UpdateResultat(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de lignes header
JUnt32 CDlgCumulativite::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de colonnes header
JUnt32 CDlgCumulativite::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re indice couleur pour courbe cumulativit�
JUnt32 CDlgCumulativite::GetIdColorCourbe(JUnt32 NoRow, JUnt32 NoCol)
{
	return ((NoRow - 1)*(m_NbLigColor + 1) + NoCol - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re le nombre de courbe cumulation en cours
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
	// Cr�ation du graphe et r�cup�ration liaison document
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
// Export des r�sultats Cumulation d'audience
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
		// Cr�ation du fichier
		CStdioFile ExportFile;
		
		// if (!ExportFile.Open(DlgFile.GetFileName( ),CFile::modeCreate | CFile::modeWrite))
		if (!ExportFile.Open(DlgFile.GetPathName( ),CFile::modeCreate | CFile::modeWrite))
		{
			// Impossible d'ouvrir ce fichier
			CString TxtErr;
			TxtErr.Format("%s %s %s","Fichier ",ExportFile.GetFileName(),"ne peut pas �tre ouvert");
			AfxMessageBox(TxtErr,MB_ICONEXCLAMATION);
			return;
		}

		// R�cup�ration de l'ent�te globale
		CString TxtEntete;
		RecupEnteteExport(TxtEntete);

		// Ecriture entete dans fichier export
		ExportFile.WriteString(TxtEntete);

		// 2eme bloc : R�sultats
		TxtEntete = "BLOC 2\t";

		// Type de r�sultat (grandeur s�lectionn�e)
		CString TxtResultat = "";
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		{
			// Une grandeur a bien �t� s�lectionn�e
			TxtResultat = pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum,pDocChoixSupport->m_MapIdGrandCumSel.GetItem());  
		}
		TxtEntete += TxtResultat + "\n";
		ExportFile.WriteString(TxtEntete);
		
		// Les r�sultats du grille (ATTENTION OD OD OA avec CStdioFile)
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

		// Bloc n� 3 : Infos Terrain + Cible + Effectif + Nb Cas
		// ------------------------------------- Les effectifs des cibles utilis�es -------------------------------------------
		//
		// R�cup�ration du bloc infos terrains / cibles
		RecupBlocTerrainCible(TxtEntete);

		// Ecriture dans fichier export
		ExportFile.WriteString(TxtEntete);

		// Bloc n� 4 : D�tail groupes
		// Si on est en mode group�, export contenu des groupes
		if (pDocChoixSupport->m_ModeColorGraphe == 1)
		{
			// Saut de ligne
			TxtEntete = "\n";
			ExportFile.WriteString(TxtEntete);

			// R�cup�ration des infos groupe
			RecupContenuGroupe(TxtEntete);

			// Ecriture dans fichier export
			ExportFile.WriteString(TxtEntete);
		}
		
		// Fin Export
		ExportFile.Close();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'ent�te pour export cumulativit�
void CDlgCumulativite :: RecupEnteteExport(CString &Entete)
{
	// N� Version
	Entete  = "No de version :\t";
	Entete += "1.0\n";

		// Nom du plateau
	Entete += "NOM PLATEAU :\t";
	Entete += pDocChoixSupport->m_KEYPlateau.GetLabel() + "\n";

	// Init entete avec type de r�sultat
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
		// R�cup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// R�cup la source audience courante
		SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

		// R�cup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// Recuperer dans table source
		Entete += pt->m_Libelle.AsJCharPtr();
	}
	else
		// Pas de source d'audience
		Entete += "Aucune source";

	Entete += "\n";


	// Le Terrain utilis�
	Entete += "TERRAIN : \t";

	// R�cup la source audience courante
	SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

	// ---------------------------------------- S�lection du terrain -------------------------------------------
	//
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// Et l'ident terrain
		int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

		// R�cup�re infos terrain 
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();
		const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

		// R�cup�re libelle terrain
		if (pt)
		{
			Entete += pt->m_Libelle.AsJCharPtr();
			Entete += "\t";
		}
	}
	Entete += "\n";
		
	// ---------------------------------------- La cible utilis�e -------------------------------------------
	//
	Entete += "CIBLE : \t";

	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// R�cup�re ident cible
		JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

		// R�cup libell� 
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
	
	// ----------------------------------------- La r�f�rence affinit� -----------------------------------------
	//
	Entete += "REF. AFFINITE : \t";
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
	// IItem * pItem = pItemConst->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(pDocChoixSupport->m_Offre.m_IdItemAffinite);

	if(pItem)
		// Ajoute et s�lection d'office du seul indice affinit� (pour le moment)
		Entete += pItem->GetLabel().AsJCharPtr();
	else
		Entete += "Aucune r�f�rence affinit�";
	Entete += "\n";

	// -------------------------------------------- Le format utilis� ------------------------------------------------
	Entete += "FORMAT : \t";
	JUnt32 IdFormat;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
	{
		// R�cup�re l'ident du format courant
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
			// libell� long format
			Entete += pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat); 
		else
			Entete += "non d�fini";
	}
	else
		Entete += "non d�fini";

	Entete += "\n";

	// Ajoute Info Filtre d'audience
	Entete += "FILTRE AUDIENCE : \t";
	JSRCPresse* m_srcPresse;
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

	// R�cup Id Filtre audience du module courant
	JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

	// R�cup�re label 
	Entete += m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
	Entete += "\n";
		
	// Le contexte de lecture
	Entete += "CONTEXTE DE LECTURE \n";
	//CKeyTerrain KeyTerrain;
	//KeyTerrain.m_IDSource  = SrcID;
	//pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	//KeyTerrain.m_IDTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	//int NbCtx = pDocChoixSupport->m_MoteurPresse.GetNbCtxLecture(KeyTerrain); 

	//// Cr�e la liste des idents contextes s�lectionn�s
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
	//			// Aucun contexte s�lectionn�
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
	//	// Pas de contexte de lecture pour certaines enqu�tes
		// Entete += "Aucune contexte : \tAucun contexte\n";
	//}

	// Mode calcul des familles
	Entete += "CALCUL DES FAMILLES : \t ";
	if (pDocChoixSupport->m_CumulFamille)
		Entete += "R�sultats cumul�s \n";
	else	
		Entete += "R�sultats moyenn�s \n";

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
		Entete += "non d�finie";

	Entete += "\n";

	// ----------------------------------------- Export Type de poids ----------------------------------------------
	//
	Entete += "TYPE POIDS :\t";
	JSRCTables* srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Libelle type de poids
	if (pDocChoixSupport->m_Offre.m_IdTypePoids > 0)
		Entete += srcTables->m_TBLPoids.GetLabelByID(pDocChoixSupport->m_Offre.m_IdTypePoids);
	else
		// type de poids non d�fini
		Entete += "non d�fini";

	Entete += "\n";

	// ---------------------------------------- Export Type de moteur calcul ---------------------------------------
	//
	// on exporte le titre
	Entete += "METHODE DE CALCUL DES COUVERTURES : \t";

	CString str;
	switch (pDocChoixSupport->m_MoteurPresse.GetType())
	{
		case CMoteurPresse::ABAQUE:
			// M�thode macro-probabiliste (JFC)
			str = "Mod�lisation macro-probabiliste brute";
			break;
		case CMoteurPresse::ABAQUE_ETAL:
			// M�thode macro-probabiliste �talonn�e (JFC)
			str = "Modelisation macro-probabiliste";
			break;
		case CMoteurPresse::PROBA:
			// M�thode probabilit�s individuelles (idem SMP-PRESSE)
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
			Entete += " (R�sultats cumul�s pour les familles) \t";
		else	
			Entete += " (R�sultats moyenn�s pour les familles) \t";
	}
	*/

	Entete += "\n";

	// ---------------------------------------- Export Param�trage LDP / LNM / LDP 30j -----------------------------
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

	// LDP � 30 jours ou � 60 jours pour les bimestriels
	if (pDocChoixSupport->m_bLDP30J)
		Entete += "LDP � 30 jours pour les bimestriels";
	else
		Entete += "LDP � 60 jours pour les bimestriels";
	Entete += "\n";

	// --------------------------- Libelle segment de ciblage ----------------------------- 
	Entete += "SEGMENT:\t";

	// Boucle sur les �lmts segment audience s�lectionn�s pour cumulativit�
	JBool PremSeg = true;
	for (pDocChoixSupport->GetMapIdSegmentsAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdSegmentsAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdSegmentsAudienceSel()->MoveNext())
	{
		// R�cup�re ident segment
		JUnt32 IdSeg	= pDocChoixSupport->GetMapIdSegmentsAudienceSel()->GetKey();

		// Segment de ciblage
		const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(IdSeg);

		// Ajout Libell� Segment
		if (!PremSeg) Entete += " , ";
		Entete += Seg->m_Libelle.AsJCharPtr(); 
		PremSeg = false;
	}	
	Entete += "\n";

	// --------------------------------- Info mode graphique -----------------------------
	//
	Entete += "GRAPHIQUE\t";
	if (pDocChoixSupport->m_ModeColorGraphe == 0)
		// Mode courbe supports s�par�s
		Entete += "S�par�\n";
	else
		// Mode regroupement support par couleur
		Entete += "Group�\n";

	// Pr�voir sauts de ligne avant affichage infos tableurs
	Entete += "\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration r�sultats cumulation d�taill�e
void CDlgCumulativite ::RecupGrilleExport(CString &Entete)
{
	// Init chaine r�sultat
	Entete = "";

	// Tableau de lignes et colonnes s�lectionn�es
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridCumulativite.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridCumulativite.GetSelectedRows(Rows,false,false);
	
	// Vire les n� colonnes avec indice header info g�n�rales
	for (int iCol = Cols.GetCount()-1 ; iCol >= 0; iCol--)
	{
		if (Cols.GetAt(iCol) < m_NoColPeriodicite)
			Cols.RemoveAt(iCol); 
	}

	// Vire les n� lignes avec indice header
	for (int iRow = Rows.GetCount()-1 ; iRow >= 0; iRow--)
	{
		if (Rows.GetAt(iRow) < m_NbHeaderRow)
			Rows.RemoveAt(iRow); 
	}

	// R�cup�ration de la partie header pure  (row & col)
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

			// Tabulation apr�s chaque info
			Entete += "\t";
		}

		// Ecriture header r�sultats colonnes
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

		// Saut apr�s chaque ligne header grille
		Entete += "\n";
	}

	// Balayage des lignes s�lectionn�es
	int x = Rows.GetCount();
	for (int nRow = 0; nRow < Rows.GetCount(); nRow++)
	{
		// No de la ligne support
		int NoRow = Rows.GetAt(nRow);

		// R�cup�re ident support
		JUnt32 IdSupport = atoi(m_GridCumulativite.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Ecriture infos g�n�rales supports
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
		{	
			// Libell� support, ou p�riodicit� )
			Txt = m_GridCumulativite.GetValueRowCol(NoRow,nCol); 

			/*
			if (nCol != m_NoColFormat-1)
				// Libell� support, ou p�riodicit� )
				Txt = m_GridCumulativite.GetValueRowCol(NoRow,nCol); 
			else
			{
				// R�cup�re le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifCum.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifCum.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifCum.GetItem();

				// libell� format support
				Txt = pDocChoixSupport->GetFormat(IdFormatSupport,true);
			}
			*/
			Entete += Txt +  "\t";
		}
		
		// Ecriture r�sultats colonnes
		for (int nCol = 0; nCol < Cols.GetCount(); nCol++)
		{
			// No de la colonne r�sultat
			int NoCol = Cols.GetAt(nCol);
			Txt = m_GridCumulativite.GetValueRowCol(NoRow,NoCol); 
			Entete += Txt +  "\t";
		}

		// Saut apr�s chaque ligne support
		Entete += "\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Export du graphique cumulativit�
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
		AfxMessageBox("Erreur lors de l'export fichier graphique cumulativit�");	
	}

	/*
	// Test si au moins 1 support s�lectionn� pour le mapping
	if(!ExportItems(480,DlgFile.GetPathName()))
		AfxMessageBox("Erreur lors de l'export graphique cumulativit�");	
	*/
}	


BOOL CDlgCumulativite::ExportItems(long VSize, const TCHAR* pFilename) 
{
	/////////////////////////////////////////////////////////////////////////////
	// la fonction pour exporter le dessin vers un MetaFile
	/////////////////////////////////////////////////////////////////////////////
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on v�rifie la validit� des �l�ments
	// if(m_GraphCum == NULL) return FALSE;

	return TRUE;

	/*
	// on r�cup�re le contexte de p�riph�rique
	CDC* pDC = this->GetDC();
	if(pDC == NULL) return FALSE;

	CMetaFileDC MetaDC; // cr�ation du MetaFile
	if(!MetaDC.CreateEnhanced(pDC, pFilename, NULL, NULL)) return FALSE;

	// r�cup�ration des attributs d'origine
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
	
	// Dessin liaison courbe et libell� �quation
	this->DrawTraitEquation(MetaDC, HSize, VSize);

	// Les points du mapping
	this->DrawLosanges(MetaDC, HSize, VSize);

	// Dessin lien point et libell�
	this->DrawTraitLib(MetaDC, HSize, VSize);

	// Libell�s point du mapping
	this->DrawLibelles(MetaDC, HSize, VSize);

	// Libelles des equations
	this->DrawLibEquation(MetaDC, HSize, VSize);
		
	// lib�ration des outils
	this->OnEndDrawing(MetaDC); 

	MetaDC.ReleaseAttribDC();

	// fermeture du fichier, on garde un handle pour la copie �ventuelle dans le Presse-Papiers
	HENHMETAFILE hMetaFile = MetaDC.CloseEnhanced();
	if(hMetaFile == NULL) return FALSE;

	// si un nom de fichier a �t� sp�cifi� on termine ici avec succ�s
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
		// envoie les donn�es du MetaFile dans le presse-papiers
		if(SetClipboardData(CF_ENHMETAFILE, hMetaFile) == NULL)
		{
			CloseClipboard();
			return FALSE;
		}
		CloseClipboard();
	}
	else
	{
		// on supprime le m�tafile
		if (FALSE == ::DeleteEnhMetaFile(hMetaFile)) return (FALSE);
	}
	return TRUE;
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du bloc infos terrains / cibles
void CDlgCumulativite ::RecupBlocTerrainCible(CString &Entete)
{
	// Bloc n� 3
    Entete = "\nBLOC 3\n";
    Entete += "TERRAIN \t CIBLE\t EFFECTIF\t NB CAS\n";

	// R�cup Table
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	// R�cup la source audience courante
	JUnt32	SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

	// ------------------------------------------ S�lection des terrains ---------------------------------------------
	//
	for (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext())
	{
		// Et l'ident terrain
		int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

		// R�cup�re infos terrain 
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();
		const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

		// R�cup�re libelle terrain
		CString LibTerrain = "";
		if (pt)
			LibTerrain= pt->m_Libelle.AsJCharPtr();
	
		// Les cibles associ�es
		for (pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			 pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid();
			 pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext())
		{
			// R�cup�re ident cible
			JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

			// R�cup libell� 
			pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
			
			if (pDocChoixSupport->m_MAPpItems.IsValid()) 
			{
				Entete += LibTerrain + "\t" + pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
			else
				Entete += LibTerrain + "\t" + "Aucune cible";

			Entete += "\t";

			// D�finition clef moteur
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
// R�cup�ration des infos groupe
void CDlgCumulativite :: RecupContenuGroupe(CString &Entete)
{
	// S�parateur item
	CString SepItem = "\t";

	// Texte format
	CString Txt;

	// Bloc n� 4
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

		// Balaye toutes les lignes associ�es aux supports
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
				// Incr�mente nb supports pour cette courbe
				NbSupp++;

				// R�cup�re le libell� support
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
			Txt.Format("%s %d %s%d %s","Courbe n�",InxCourbe,SepItem,NbSupp,TxtCourbe);
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
// Clipboard du grille cumulativit�
bool CDlgCumulativite ::Clipboard()
{
	// S�lection globale du grid
	POSITION area = m_GridCumulativite.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridCumulativite.SetSelection(area, 0, 0, m_GridCumulativite.GetRowCount(), m_GridCumulativite.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridCumulativite.Copy(); 

	// D�selection globale
	m_GridCumulativite.SetSelection(NULL, 0, 0, 0, 0);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du graphique cumulativit�
bool CDlgCumulativite ::ClipboardGraphique()
{
	// Export du graphique sous format clipboard
	if (!m_GraphCum.ExportGraph(NULL)) 
	{
		AfxMessageBox("Erreur lors de l'export clipboard graphique cumulativit�");	
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

		// Arr�te tous les calculs
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
// R�cup�ration s�lection Nb Insertions
void CDlgCumulativite::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertionsCum = NbInsertions;

}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi s�lection Nb Insertions
JUnt32 CDlgCumulativite:: GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertionsCum;
}
