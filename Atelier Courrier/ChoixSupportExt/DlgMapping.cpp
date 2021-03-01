// DlgMapping.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgMapping.h"
#include "ExcelPrinter.h"

// Voir pour copie BMP
#include <afxctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SPLIT_HORZ   (WM_USER + 1)

// Boîte de dialogue CDlgMapping
IMPLEMENT_DYNAMIC(CDlgMapping, JFCDialog)
CDlgMapping::CDlgMapping(CWnd* pParent,  /*=NULL*/
						 JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgMapping::IDD, pParent), m_SplitHorz(WM_SPLIT_HORZ)
{
	// Récupère pointeur sélection générale
	pDocChoixSupport = pDoc;

	// Passe le document pour la création mapping
	m_BagMining.SetDocument(pDoc,this); 

	// Init Lignes/Colonnes Grid
	m_NbHeaderRow  = 3;
	m_NbRowSupport = 0;
	m_NbHeaderCol  = 1;
	m_NbCol        = 0;
}

CDlgMapping::~CDlgMapping()
{
}

void CDlgMapping::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_TITRE_MAPPING, m_TitreMapping);
	DDX_Control(pDX, IDC_CS_PROGRESS_RESULTMAP, m_ProgressResultMap);
	DDX_Control(pDX, IDC_CS_CADRE_GRIDMAP, m_CadreGridMapping);
	DDX_Control(pDX, IDC_CS_MAPPING, m_Mapping);
	DDX_Control(pDX, IDC_CS_CADRE_MAPPING, m_CadreMapping);
	DDX_Control(pDX, IDC_CS_SPLIT_HORZ, m_SplitHorz);
}


BEGIN_MESSAGE_MAP(CDlgMapping, JFCDialog)
	//{{AFX_MSG_MAP(CDlgMapping)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SPLIT_HORZ, OnSplitHorz)
END_MESSAGE_MAP()


BOOL CDlgMapping::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// Initialisation des différents contrôles 
	SetupControls();

	// 1 / Titre Source - Filtre d'audience
	UpdateTitreAnalyse();

	// Mise en place des realtions objets fenêtre Mapping
	SetWindowRelations();

	// on initialise les modes de déplacement 
	this->SetMode(&m_Mapping, MGER_SIZE, MGER_SIZE);

	// on récupère la position des fenêtres
	RECT RectL1; m_CadreMapping.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGridMapping.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);

	// on calcule la configuration du split
	LONG SPUp = RectL1.bottom - RectL1.top; if (SPUp < 0) SPUp = 0;
	LONG SPDn = RectL2.bottom - RectL2.top; if (SPDn < 0) SPDn = 0;

	// on reconfigure le split
	m_SplitHorz.SetRange(SPDn, SPUp);

	return TRUE;  // return TRUE unless you set the focus to a control

}

/////////////////////////////////////////////////////////////////////
// Gestion du déplacement split horizontal mapping
LRESULT CDlgMapping::OnSplitHorz(WPARAM WParam, LPARAM LPAram)
{
	// on récupère le déplacement
	LONG Disp = (LONG)WParam;

	// on récupère la position des fenêtres
	RECT RectL1; m_CadreMapping.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGridMapping.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);
	RECT RectL3; m_Mapping.GetWindowRect(&RectL3); this->ScreenToClient(&RectL3);
	RECT RectL4; m_GridMapping.GetWindowRect(&RectL4); this->ScreenToClient(&RectL4);
	RECT RectSP; m_SplitHorz.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);

	// Limite aux déplacements pour éviter des abérations graphiques
	// Hauteur minimale graph mapping
	if (RectL1.bottom + Disp < 100) 
		Disp = 100 - RectL1.bottom;

	// Hauteur maximale graph mapping
	if (RectL1.bottom + Disp > (RectL2.bottom - 100))  
		Disp = (RectL2.bottom - 100)  - RectL1.bottom;

	// on corrige la position des fenêtres
	RectL1.bottom += Disp;
	RectL3.bottom += Disp; 
	RectSP.bottom += Disp; RectSP.top += Disp;
	RectL2.top    += Disp;
	RectL4.top    += Disp;

	// on déplace les fenêtres
	m_CadreMapping.SetWindowPos(NULL, RectL1.left, RectL1.top, RectL1.right - RectL1.left, RectL1.bottom - RectL1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Mapping.SetWindowPos(NULL, RectL3.left, RectL3.top, RectL3.right - RectL3.left, RectL3.bottom - RectL3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER); 
	m_CadreGridMapping.SetWindowPos(NULL, RectL2.left, RectL2.top, RectL2.right - RectL2.left, RectL2.bottom - RectL2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GridMapping.SetWindowPos(NULL, RectL4.left, RectL4.top, RectL4.right - RectL4.left, RectL4.bottom - RectL4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

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
// Mise en place des relations entre objets de la fenêtre Mapping
void CDlgMapping::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Cadre englobant le graphe mapping
			{IDC_CS_CADRE_MAPPING, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eProportional, IDC_MAIN, CResizer::eBottom}},				//Bottom side

			// Titre Mapping
			{IDC_CS_TITRE_MAPPING, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
						  		   {CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eTop}},						//Bottom side

			{IDC_CS_MAPPING,	   {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eTop},						//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eFixed, IDC_CS_CADRE_MAPPING, CResizer::eBottom}},		//Bottom side

			// Cadre englobant le grid résultat mapping
			{IDC_CS_CADRE_GRIDMAP, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
								   {CResizer::eFixed, IDC_CS_CADRE_MAPPING, CResizer::eBottom},			//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					//Bottom side

			// Grid résultat mapping
			{IDC_CS_GRID_MAPPING,  {CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eLeft},			//Left side
								   {CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eTop},			//Top side
								   {CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eRight},			//Right side
								   {CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eBottom}},		//Bottom side

			// Split horizontal
			{IDC_CS_SPLIT_HORZ,    {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
								   {CResizer::eFixed, IDC_CS_CADRE_MAPPING, CResizer::eBottom},			//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eTop}},			//Bottom side

			// Barre de progression
			{IDC_CS_PROGRESS_RESULTMAP,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},					//Left side
										{CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},					//Right side
										{CResizer::eFixed, IDC_CS_CADRE_GRIDMAP, CResizer::eBottom}},	//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contrôles fenêtre Mapping
void CDlgMapping::SetupControls()
{
	// Propriétés du cadre Mapping
	m_CadreGridMapping.SetTransparent(false);  // A REMETTRE true ?????
	m_CadreGridMapping.SetSunken(true);

	// Propriétés du cadre Grille Mapping
	m_CadreMapping.SetTransparent(false);
	m_CadreMapping.SetSunken(true);
 
	// Les éléments du cadre Mapping
	m_CadreMapping.AddTopWindow(m_TitreMapping);
	m_CadreMapping.AddTopWindow(m_Mapping);

	// Les éléments du cadre résultat mapping
	m_CadreGridMapping.AddTopWindow(m_GridMapping); 
	m_CadreGridMapping.AddTopWindow(m_ProgressResultMap);
	
	// Progress bar non visible au départ
	m_ProgressResultMap.ShowWindow(SW_HIDE); 
	m_ProgressResultMap.SetShowText(false);

	// Taille et Font des différents labels
	m_TitreMapping.SetFontName(CATPColors::GetFontLabelName());
	m_TitreMapping.SetFontBold(TRUE); 
	m_TitreMapping.SetFontSize(CATPColors::GetFontLabelSize());

	// A FAIRE
	// Initialise objet Mapping
	// m_Mapping.
	
	// Initialise grid résultat mapping
	m_GridMapping.SubclassDlgItem(IDC_CS_GRID_MAPPING, this);
	m_GridMapping.EnableMouseWheel(); 
	m_GridMapping.Initialize(); 

	GXSetNewGridLineMode(); 
    m_GridMapping.GetParam()->EnableUndo(FALSE);
	m_GridMapping.SetMergeCellsMode(gxnMergeDelayEval,false);  //avant sans false
	m_GridMapping.GetParam()->SetNumberedColHeaders(false); 
	m_GridMapping.GetParam()->EnableMoveCols(FALSE);
	m_GridMapping.GetParam()->SetMinTrackColWidth(40);
	m_GridMapping.GetParam()->SetNumberedRowHeaders(false); 
	m_GridMapping.GetParam()->EnableMoveRows(FALSE);
	m_GridMapping.GetParam()->SetMinTrackRowHeight(16);
	m_GridMapping.GetParam()->SetSmartResize(true);	

	m_GridMapping.GetParam()->SetLockReadOnly(true); 

	// en + 
	m_GridMapping.GetParam()->GetProperties()->SetMarkColHeader(FALSE);

	// Enregistrement controle spécifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(&m_GridMapping);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridMapping.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);

	// Init Référence colonne
	InitRefGrid();

	// Associe le document au grille
	if (m_GridMapping.OnInitDocument(pDocChoixSupport, this, m_NoColIdSupport) == false)
		AfxMessageBox ("Problème association document supports / grille résultat mapping");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid résultat
	if (GrilleResultValid())
		InitGridResultat();

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des références colonnes
void CDlgMapping::InitRefGrid()
{
	// Position colonne id supports
	m_NoColIdSupport	= 1;

	// Position colonne supports
	m_NoColSelSupport	= m_NoColIdSupport + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne formats
	m_NoColFormat		= m_NoColSupport + 1;

	// Position colonne périodicité
	m_NoColPeriodicite	= m_NoColFormat + 1;

	// Position colonne tarif
	m_NoColTarif		= m_NoColPeriodicite + 1;
}

// Définition des couleurs interface fenêtre Mapping
void CDlgMapping::SetColors()
{
	// Coloriage du cadre englobant
	m_CadreMapping.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_CadreGridMapping.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultMap.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Titre Cumulativité
	m_TitreMapping.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreMapping.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Grille résultat
	m_GridMapping.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridMapping.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Progress bar
	m_ProgressResultMap.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)); 

	// Style des lignes (solid pen)
	m_GridMapping.GetParam()->SetGridLineStyle(PS_SOLID); 

}

// Destruction de la fenêtre Mapping
void CDlgMapping::OnDestroy()
{
	JFCDialog::OnDestroy();
}

// Retaille de la fenêtre Mapping
void CDlgMapping::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

void CDlgMapping::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		// Mise à jour du Mapping
		// AfxMessageBox("A FAIRE / MAPPING",MB_ICONINFORMATION);	
	}
}

void CDlgMapping::OnCancel()
{
	JFCDialog::OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid résultat mapping
void CDlgMapping::InitGrid()
{
	m_GridMapping.LockUpdate(true);

	// Enleve n° ligne col 
	m_GridMapping.GetParam()->SetNumberedColHeaders(false);
	m_GridMapping.GetParam()->SetNumberedRowHeaders(false);

	// Position colonne id supports
	m_NoColIdSupport	= 1;

	// Position colonne supports
	m_NoColSelSupport	= m_NoColIdSupport + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne formats
	m_NoColFormat		= m_NoColSupport + 1;

	// Position colonne périodicité
	m_NoColPeriodicite	= m_NoColFormat + 1;

	// Position colonne tarif
	m_NoColTarif		= m_NoColPeriodicite + 1;

	// Init colonnes
	UpdateCol();

	// Initialisation si au moins 1 support
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{
		// Init Lignes
		UpdateRow();

		// Delock mode read only
		m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

		// Lignes non resizable et nb lignes headers (3 : Terrains / Cibles / Grandeurs)
		m_GridMapping.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

		// Aucun header colonne et colonnes bloqués (support/périodicité/tarif)
		m_GridMapping.SetFrozenCols(m_NoColTarif - m_NoColIdSupport ,0);

		// Affichage entete Tarif et périodicité
		if (m_GridMapping.GetRowCount() > 0) 
		{

			// Titre "Sél SUPPORTS"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("X")));

			// Titre "SUPPORTS"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColSupport-1),
										CGXStyle()
											.SetValue(_T("Supports")));

			// Titre "FORMATS"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColFormat-1),
										CGXStyle()
											.SetValue(_T("Formats")));

			// Titre "TARIF"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColTarif-1),
										CGXStyle()
											.SetValue(_T("Tarif €")));

			// Titre "PERIODICITE"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColPeriodicite-1),
										CGXStyle()
											.SetValue(_T("Périodic.")));

			// Case sélection / déselection tot supports
			m_GridMapping.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("T")));
		}

		// Merge des cellules définitivement vide
		m_GridMapping.UpdateData(); 

		m_GridMapping.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,m_NbHeaderRow-1,m_GridMapping.GetColCount()),
									CGXStyle()
									.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

		m_GridMapping.UpdateWindow(); 

		// On bloque les colonnes qui doivent ne pas être modifiées
		m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Rend visible le grid, si au moins quelques résultats
	if (m_GridMapping.GetColCount() >= m_NoColTarif)
	{
		m_GridMapping.ShowWindow(SW_SHOW);
		m_CadreGridMapping.SetTransparent(true);
	}
	else
	{
		m_GridMapping.ShowWindow(SW_HIDE);
		m_CadreGridMapping.SetTransparent(false);
	}

	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Colonnes
bool CDlgMapping::UpdateCol()
{
	// Debloque modif cellules
	// m_GridMapping.SetReadOnly(FALSE); 

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Colonnes
	m_NbHeaderCol = m_NoColTarif - m_NoColIdSupport + 1;

	// Nb Colonnes
	if (pDocChoixSupport != NULL && pDocChoixSupport->m_MapIdSupportsSel.GetCount() > 0)
	{
		// Uniquement les 2 colonnes correspondant aux infos abscisse et ordonnée mapping
		m_NbCol = 2;
	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Init grid avec nb colonnes
    if (!m_GridMapping.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Lignes
bool CDlgMapping::UpdateRow()
{
	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Lignes  (ligne info support + état nb insertions)
	m_NbHeaderRow = 3;

	// Nombre de lignes
    if (!m_GridMapping.SetRowCount(m_NbHeaderRow + m_NbRowSupport-1,GX_UPDATENOW))
		return false;

	// Lignes headers 
	m_GridMapping.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Remplissage des entetes header rows
	if (m_NbCol >= 1)
	{
		for (int Lig = 0; Lig < m_NbHeaderRow ; Lig++)
		{
			// Pour chaque colonne
			JUnt32 NbCol = m_GridMapping.GetColCount();

			for (int Col = 0; Col <= m_GridMapping.GetColCount(); Col++) 
			{
				// Centrage et style texte header
				m_GridMapping.SetStyleRange(CGXRange(Lig,Col),
											CGXStyle()
												.SetHorizontalAlignment(DT_CENTER)
												.SetVerticalAlignment(DT_VCENTER)
												.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL))
												.SetFont(CGXFont()
												.SetSize(pDocChoixSupport->m_SizeFontHeader)
												.SetBold(pDocChoixSupport->m_BoldFontHeader)));

				// Texte associé au header la cellule
				CString Txt;

				if (Col >= m_NbHeaderCol)
				{
					// Texte associé au header ligne
					Txt = RecupTexteHeaderRow(Lig,Col);

					// Affichage contenu cellule
					m_GridMapping.SetValueRange(CGXRange(Lig,Col),Txt); 
				}
			}
		}

		// Couleurs spéciales
		UpdateColorSpe();

		// Update Grille modifiée
		m_GridMapping.UpdateData(TRUE);

		// Regroupement des lignes header row
		m_GridMapping.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
		m_GridMapping.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridMapping.GetColCount()),
									CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

		// Mise à jour des datas résultat grille échelle
		m_GridMapping.UpdateWindow(); 
		m_GridMapping.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridMapping.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs spéciales de ceratines cellules
void CDlgMapping::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne résultat
	// Cellules info insertion en fond blanc et couleur caractère différent
	if (m_GridMapping.GetColCount() >= m_NoColTarif)
	{
		m_GridMapping.LockUpdate(true); 

		m_GridMapping.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 1, m_NoColTarif-1),
									CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Delock mode read only
		m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

		// Niveau header de couleurs différents (via dlg ordre entete)
		// Niveau 1
		m_GridMapping.SetStyleRange(CGXRange(0, m_NoColTarif, 0, m_GridMapping.GetColCount()),
									CGXStyle()
										.SetInterior(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
		// Niveau 2
		m_GridMapping.SetStyleRange(CGXRange(1, m_NoColTarif, 1, m_GridMapping.GetColCount()),
									CGXStyle()
										.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));


		// en +++++++
		for (JUnt32 Col = m_NbHeaderCol; Col <= m_GridMapping.GetColCount(); Col++) 
		{
			// Si data exogène -- couleur différente
			// Récupère identificateur sélection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col);

			if (IdGrandeur >= 100)
			{
				// Data Exogène en + foncé
				m_GridMapping.SetStyleRange(CGXRange(0, Col, m_NbHeaderRow-1, Col),
											CGXStyle()
											.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));
			}
			else
			{
				// Les autres data en light
				m_GridMapping.SetStyleRange(CGXRange(0, Col, m_NbHeaderRow-1, Col),
											CGXStyle()
											.SetTextColor(CATPColors::GetColorBlack()));
			}
		// m_GridMapping.UpdateWindow(); 
		}

		// A Voir

		// Delock mode read only
		m_GridMapping.GetParam()->SetLockReadOnly(TRUE);
		m_GridMapping.LockUpdate(false); 
		m_GridMapping.Redraw();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération texte header row
CString CDlgMapping::RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col)
{
	// Init chaine libellé
	CString Txt = "-";

	// Afficher titre des colonnes abscisses et ordonnées du mapping
	if (Col == m_NoColTarif)
	{
		// Colonne consacrée à l'affichage de la grandeur en abscisse
		Txt.LoadString(IDS_CS_ABSCISSE_ND);
		// Txt = "Abscisse n.d.";
		if (pDocChoixSupport->m_IdGrandMapSelAbs >= 0)
			Txt = pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleMap, pDocChoixSupport->m_IdGrandMapSelAbs);
	}
	else if (Col == m_NoColTarif + 1)
	{
		// Colonne consacrée à l'affichage de la grandeur en ordonnée
		Txt.LoadString(IDS_CS_ORDONNEE_ND);
		// Txt = "Ordonnée n.d.";
		if (pDocChoixSupport->m_IdGrandMapSelOrd >= 0)
			Txt = pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleMap, pDocChoixSupport->m_IdGrandMapSelOrd);
	}	
	// Renvoi libellé trouvé
	return Txt;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération Id Grandeur
JUnt32 CDlgMapping::RecupIdGrandeur(JUnt32 NoCol)
{
	// Abscisse ou ordonnée du mapping
	if (NoCol == m_NoColTarif)
		// Colonne consacrée à l'affichage de la grandeur en abscisse
		return pDocChoixSupport->m_IdGrandMapSelAbs;

	else if (NoCol == m_NoColTarif + 1)
	{
		// Colonne consacrée à l'affichage de la grandeur en ordonnée
		return pDocChoixSupport->m_IdGrandMapSelOrd;
	}	
	else
		// Colonne grandeur non définie
		return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des résultats selon grandeur
//  0  > "LNM/LDP(000)" 					-------------------------------- LNM/LDP (000) (Audience) en milliers
//  1  > "Audience"							-------------------------------- audience * effectif / 100
//  2  > "Audience(000)						-------------------------------- idem en milliers
//  3  > "Cts.[+](000)"						-------------------------------- VolLecture (Contacts en milliers)
//  4  > "GRP"   							-------------------------------- (Audience LNM/LDP * 100.0) / Effectif
//  5  > "GRP[+]							-------------------------------- (VolLecture * 100.0) / Effectif
//  6  > "C% cumulée"						--------------------------------  audience n insertions
//  7  > "Répétition ou reprises en main"	-------------------------------- VolLecture / (Effectif * Audience)
//  8  > "Indice Affinité"					-------------------------------- (GRP / GRPAff) * 100.0 
//  9  > "% Affinité"						-------------------------------- (AudienceLDP / AudAffLDP) * 100.0 
// 10  > "Cumulativité"						-------------------------------- (Audience n insertions / Audience à 1 insertion)
// 11  > "Coût/GRP"							-------------------------------- tarif support / GRP
// 12  > "Coût/1000 effectif"				-------------------------------- tarif support / audience
// 13  > "Coût/%Couverture					-------------------------------- tarif support / % Couverture
// 14  > "Coût/1000 contacts"				-------------------------------- tarif support / Nb Contacts

//  ?? > "Audience / Nb Lect"				-------------------------------- audience * effectif / 100
//
/*
CString CDlgMapping::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol,  JFlt64 Tarif,      JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
																   JFlt64 Effectif,   JFlt64 NbCas,
																   JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff)
{
	// Par défaut Texte
	CGXStyle style;
	m_GridMapping.SetStyleRange(CGXRange(NoLig,NoCol),
								CGXStyle()
									.SetFormat(GX_FMT_TEXT)
									.SetValueType(GX_VT_STRING));

	// Recup Resultat
	return RecupResultTxt(IdSupport, NoCol,Tarif,VolLecture,
		                  AudienceLDP,AudienceProba,Audience,Audience1Ins,Effectif,
				          NbCas,VolLectAff,AudAffLDP,AudAffProba,AudAff,EffAff);	
	
}
*/
CString CDlgMapping::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol,
								   JFlt64 Tarif, CResultMap &ResultMap)
{
	// Par défaut Texte
	CGXStyle style;
	m_GridMapping.SetStyleRange(CGXRange(NoLig,NoCol),
								CGXStyle()
									.SetFormat(GX_FMT_TEXT)
									.SetValueType(GX_VT_STRING));

	// Indique les résultats partiels
	if (ResultMap.m_ResultPartiel)
	{	
		// On distingue résultat partiel cellule
		m_GridMapping.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorRed())
										.SetFont(CGXFont()
										.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));
	}
	else
	{
		// On distingue cellule resultat complet
		m_GridMapping.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack())
										.SetFont(CGXFont()
										.SetItalic(pDocChoixSupport->m_ItalicResultComplet)));
	}

	// Recup Resultat
	return RecupResultTxt(IdSupport, NoCol, Tarif, ResultMap);

}

CString CDlgMapping::RecupResultTxt(JUnt32 IdSupport,JUnt32 NoCol, JFlt64 Tarif, CResultMap &ResultMap)
{
	// Chaine formatée par défaut
	CString TxtResult = "*";

	// Pour le moment Mapping avec 1 seule insertion
	JFlt64 NbInsertions = 1.0;

	// Récupère ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol);	

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
		case pDocChoixSupport->GrdMap_AudienceLDP : 
		{
			if (ResultMap.m_AudienceLDP >= 0)
			{
				// Récupére audience
				// TxtResult.Format("%d",(JUnt32)((AudienceLDP + 500.0)/1000.0));
				TxtResult.Format("%0.f",ResultMap.m_AudienceLDP / 1000.0);

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdMap_Audience :
		{
			if (ResultMap.m_Audience >= 0)
			{
				// TxtResult.Format("%d",(JUnt32)Audience);
				TxtResult.Format("%0.f",ResultMap.m_Audience);

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdMap_Audience000 :
		{
			if (ResultMap.m_Audience >= 0)
			{
				//TxtResult.Format("%d",(JUnt32)((Audience + 500.0)/1000.0));
				TxtResult.Format("%0.f",ResultMap.m_Audience / 1000.0);

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdMap_NbContact000 :
		{
			if (ResultMap.m_VolLecture >= 0)
			{
				TxtResult.Format("%0.f",ResultMap.m_VolLecture / 1000.0);

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		/////////////////////////////////// CALCUL GRP ///////////////////////////////////
		case pDocChoixSupport->GrdMap_GRP : 
		{
			if (ResultMap.m_Effectif > 0 && ResultMap.m_AudienceProba >= 0)
			{
				// JFlt64 GRP = (100.0 * AudienceLDP * pDocChoixSupport->m_NbInsertions) / Effectif;
				// 1 seul insertion pour le moment !!!
				JFlt64 GRP = (100.0 * ResultMap.m_AudienceProba * NbInsertions) / ResultMap.m_Effectif;
				TxtResult.Format("%0.1f ",GRP);
			}
		}
		break;

		/////////////////////////////////// CALCUL GRP ACTIF //////////////////////////////
		case pDocChoixSupport->GrdMap_XGRP : 
		{
			if (ResultMap.m_Effectif      >  0 && 
				ResultMap.m_VolLecture	  >= 0 && 
				ResultMap.m_AudienceProba >= 0)
			{
				JFlt64 xGRP = (ResultMap.m_VolLecture * 100.0) / ResultMap.m_Effectif;
				TxtResult.Format("%0.1f ",xGRP);
			}
		}
		break;

		/////////////////////////////////// CALCUL COUVERTURE ////////////////////////////
		case pDocChoixSupport->GrdMap_CouvCum : 
		{
			if (ResultMap.m_Effectif > 0 && ResultMap.m_Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultMap.m_Audience / ResultMap.m_Effectif) * 100.0;
				TxtResult.Format("%0.1f%%",Couv);
			}
		}
		break;

		////////////////////////////// CALCUL REPETITION //////////////////////////////
		case pDocChoixSupport->GrdMap_Repetition : 
		{
			if (ResultMap.m_Audience > 0)
			{
				JFlt64 Repet = (ResultMap.m_AudienceProba * NbInsertions) / ResultMap.m_Audience;
				TxtResult.Format("%0.2f ",Repet);
			}
		}
		break;

		////////////////////////////// REPETITION + ///////////////////////////////////
		case pDocChoixSupport->GrdMap_XRepetition : 
		{
			if (ResultMap.m_Audience > 0)
			{
				JFlt64 Repet = ResultMap.m_VolLecture / ResultMap.m_Audience;
				TxtResult.Format("%0.2f ",Repet);
			}
		}
		break;

		///////////////////////////// CALCUL INDICE AFFINITE ////////////////////////
		case pDocChoixSupport->GrdMap_IndAffinite :
		{
			// Calcul GRP
			JFlt64 GRP		= 0.0;
			if (ResultMap.m_Effectif > 0)
				GRP = (ResultMap.m_AudienceProba / ResultMap.m_Effectif) * 100.0;

			// Calcul GRP Affinité
			JFlt64 GRPAff	= 0.0;
			if (ResultMap.m_EffectifAffinite > 0)
				GRPAff = (ResultMap.m_AudienceAffiniteProba / ResultMap.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				// Calcul indice affinité
				double IndAff = (GRP / GRPAff)* 100.0;
				TxtResult.Format("%0.f",IndAff);

				// Découpage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
		case pDocChoixSupport->GrdMap_PrctAff :
		{
			if (ResultMap.m_AudienceAffinite > 0)
			{

				// Calcul pourcentage affinité
				JFlt64 PrctAff = (ResultMap.m_AudienceLDP / ResultMap.m_AudienceAffiniteLDP)* 100;
				TxtResult.Format("%0.1f%%",PrctAff);
			}
		}
		break;

		///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
		case pDocChoixSupport->GrdMap_Cumulativite : 
		{
			if (ResultMap.m_Audience1Ins > 0)
			{
				TxtResult.Format("%.2f ", ResultMap.m_Audience / ResultMap.m_Audience1Ins);	
			}
		}
		break;
		

		///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
		case pDocChoixSupport->GrdMap_CoutGRP : 
		{
			// Calcul GRP
			JFlt64 GRP	= 0.0;
			if (ResultMap.m_Effectif > 0)
				GRP = (ResultMap.m_AudienceProba / ResultMap.m_Effectif) * 100.0;

			// Si GRP défini, calcul du coût
			if (GRP > 0 && Tarif > 0) // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif / GRP;

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
		break;

		///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
		case pDocChoixSupport->GrdMap_CoutCouv : 
		{
			if (ResultMap.m_Effectif > 0 && ResultMap.m_Audience >= 0) // && !ResultMap.m_ResultPartiel)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultMap.m_Audience / ResultMap.m_Effectif) * 100.0;

				if (Couv > 0 && Tarif > 0)
				{
					JFlt64 Cout  = Tarif / Couv;
					
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
		}
		break;

		///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Ind : 
		{
			if ((ResultMap.m_Audience / 1000) > 0 && Tarif > 0)  // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif /(ResultMap.m_Audience / 1000.0);
				
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
		break;

		///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Cts : 
		{
			// Calcul Cout
			if (ResultMap.m_VolLecture > 0 && Tarif > 0)  // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif / (ResultMap.m_VolLecture / 1000.0);

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
		break;


		default:
		break;
	}

	// Traitement des données exogènes (OJD, .....)
	if (IdGrandeur >= 100)
	{
		// Récupère les informations associées aux données exogènes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libellé data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exogènes ////////////////////////////
		JFlt64 Resultat = -1;
		if (DataExo.GetValeur(NomDataExo,IdSupport,Resultat))
		{
			// Récupère résultat formaté selon type et nb dec data exo
			TxtResult = GetResultDataExo(IdGrandeur, Resultat);
		}
	}
	
	// Renvoi la chaine formatée
	// m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
	return TxtResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupère résultat formaté selon type et nb dec data exo
CString CDlgMapping::GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell)
{
	CString TxtResult;

	// Récupère les informations associées aux données exogènes (OJD etc...)
	const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
	const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

	// Recherche libellé data
	CString &NomDataExo = DataExo.GetLib(IdGrandeur);
			
	// Type et nb decimales
	JUnt32 TypeData;
	JUnt32 NbDec;
	if (DataExo.GetTypeValeur(NomDataExo,TypeData,NbDec))
	{
		if (NbDec == 1)
			TxtResult.Format("%0.1f",ResultCell);
		else if (NbDec == 2)
			TxtResult.Format("%0.2f",ResultCell);
		else if (NbDec == 3)
			TxtResult.Format("%0.3f",ResultCell);
		else
			// Par défaut en entier et sans décimale
			TxtResult.Format("%0.f",ResultCell);
	}
	else
		// Par défaut en entier et sans décimale
		TxtResult.Format("%0.f",ResultCell);

	// Découpage chaine par milliers
	pDocChoixSupport->FormatString(TxtResult,3);

	return TxtResult;
}

double CDlgMapping::RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  
								JFlt64 Tarif, CResultMap &ResultMap)
{
	// Init valeur résultat
	double Result = -1.0;

	// Pour le moment 1 seule insertion dans Mapping
	JFlt64 NbInsertions = 1.0;

	// Récupère ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol);	

	// Recup cible sélectionnée pour mapping
	JUnt32 IdCible = 0;
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible sélectionnée
		IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
		case pDocChoixSupport->GrdMap_AudienceLDP : 
		{
			if (ResultMap.m_AudienceLDP >= 0)
				// Récupére audience
				Result = ResultMap.m_AudienceLDP / 1000.0;
		}
		break;

		////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdMap_Audience :
		{
			if (ResultMap.m_Audience >= 0)
				Result = ResultMap.m_Audience;
		}
		break;

		///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdMap_Audience000 :
		{
			if (ResultMap.m_Audience >= 0)
				Result = ResultMap.m_Audience / 1000.0;
		}
		break;

		////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdMap_NbContact000 :
		{
			if (ResultMap.m_VolLecture >= 0)
				Result = ResultMap.m_VolLecture /1000.0;
		}
		break;

		/////////////////////////////////// CALCUL GRP ///////////////////////////////////
		case pDocChoixSupport->GrdMap_GRP : 
		{
			if (ResultMap.m_Effectif > 0)
				// 1 seule insertion pour le moment
				Result = (100.0 *  ResultMap.m_AudienceProba * NbInsertions) / ResultMap.m_Effectif;
		}
		break;

		/////////////////////////////////// CALCUL GRP ACTIF //////////////////////////////
		case pDocChoixSupport->GrdMap_XGRP : 
		{
			if (ResultMap.m_Effectif > 0 && ResultMap.m_VolLecture >= 0)
				Result = (ResultMap.m_VolLecture * 100.0) / ResultMap.m_Effectif;
		}
		break;

		/////////////////////////////////// CALCUL COUVERTURE ////////////////////////////
		case pDocChoixSupport->GrdMap_CouvCum : 
		{
			if (ResultMap.m_Effectif >= 0 && ResultMap.m_Audience >= 0)
				// Calcul couverture
				Result = (ResultMap.m_Audience / ResultMap.m_Effectif) * 100.0;
		}
		break;

		////////////////////////////// CALCUL REPRISES EN MAIN ///////////////////////
		case pDocChoixSupport->GrdMap_Repetition : 
		{
			if (ResultMap.m_Audience > 0)
				Result = (int(((ResultMap.m_AudienceProba * NbInsertions) / ResultMap.m_Audience + 0.005) * 100.0)) / 100.0;
		}
		break;

		////////////////////////////// CALCUL REPETITION + ///////////////////////
		case pDocChoixSupport->GrdMap_XRepetition : 
		{
			if (ResultMap.m_Audience > 0)
				Result = ResultMap.m_VolLecture / ResultMap.m_Audience;
		}
		break;

		///////////////////////////// CALCUL INDICE AFFINITE ////////////////////////
		case pDocChoixSupport->GrdMap_IndAffinite :
		{
			// Calcul GRP
			JFlt64 GRP		= 0.0;
			if (ResultMap.m_Effectif > 0)
				GRP = (ResultMap.m_AudienceProba / ResultMap.m_Effectif) * 100.0;

			// Calcul GRP Affinité
			JFlt64 GRPAff	= 0.0;
			if (ResultMap.m_EffectifAffinite > 0)
				GRPAff = (ResultMap.m_AudienceAffiniteProba / ResultMap.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				Result = 100.0;
				if (pDocChoixSupport->m_Offre.m_IdItemAffinite != IdCible)
					// Calcul indice affinité
					Result = (GRP / GRPAff)*100.0;
			}
		}
		break;

		//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
		case pDocChoixSupport->GrdMap_PrctAff :
		{
			if (ResultMap.m_AudienceAffiniteLDP > 0)
			{
				Result = 100.0;
				if (pDocChoixSupport->m_Offre.m_IdItemAffinite != IdCible)
				{
					// Calcul pourcentage affinité
					Result = (ResultMap.m_AudienceLDP / ResultMap.m_AudienceAffiniteLDP) * 100.0;
				}
			}
		}
		break;

		///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
		case pDocChoixSupport->GrdMap_Cumulativite :
		{
			if (ResultMap.m_Audience1Ins > 0)
				Result = ResultMap.m_Audience / ResultMap.m_Audience1Ins;	
		}
		break;
		

		///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
		case pDocChoixSupport->GrdMap_CoutGRP : 
		{
			// Calcul GRP
			JFlt64 GRP	= 0.0;
			if (ResultMap.m_Effectif > 0)
				GRP = (ResultMap.m_AudienceProba / ResultMap.m_Effectif) * 100.0;

			// Si GRP défini, calcul du coût
			if (GRP > 0 && Tarif > 0)
				Result = Tarif / GRP;
		}
		break;

		///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
		case pDocChoixSupport->GrdMap_CoutCouv : 
		{
			if (ResultMap.m_Effectif > 0 && ResultMap.m_Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultMap.m_Audience / ResultMap.m_Effectif) * 100.0;

				if (Couv > 0 && Tarif > 0)
					Result = Tarif / Couv;
			}
		}
		break;

		///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Ind : 
		{
			if ((ResultMap.m_Audience / 1000) > 0 && Tarif > 0)
				Result  = Tarif /(ResultMap.m_Audience/1000.0);
		}
		break;

		///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Cts : 
		{
			// Calcul Cout
			if (ResultMap.m_VolLecture > 0 && Tarif > 0)
				Result = Tarif / (ResultMap.m_VolLecture/1000.0);
		}
		break;

		default:
		break;
	}

	// Traitement des données exogènes (OJD, .....)
	if (IdGrandeur >= 100)
	{
		// Récupère les informations associées aux données exogènes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libellé data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exogènes ////////////////////////////
		JFlt64 Resultat = -1;
		if (!DataExo.GetValeur(NomDataExo,IdSupport,Result))
		{
			// Récupère résultat formaté selon type et nb dec data exo
			Result = -1;
		}
	}
	
	// Renvoi le résultat trouvé
	return Result;

}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du résultat mapping (selon grandeurs abscisse et ordonnée sélectionnées)
JBool CDlgMapping::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridMapping.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultMap KeyResultMap;

	// Information résultat archivé
	CResultMap	ResultMap;

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
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridMapping.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridMapping.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultMap.ShowWindow(SW_SHOW); 
	m_ProgressResultMap.SetShowText(false); 
	int nLower, nUpper;
	m_ProgressResultMap.GetRange( nLower, nUpper);

	// Clef affinité courante
	KeyResultMap.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice clé segment
	KeyResultMap.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 

	// Calcul indice clé moteur
	KeyResultMap.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Récupére map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);


	// Balayage ligne support
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Par défaut c'est un support
		JBool ElmtFamille = false;

		// Avance progress bar
		m_ProgressResultMap.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention si c'est une famille
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un élmt famille
			ElmtFamille = true;
		}

		// Récupère le format support ligne sélectionné pour le mapping
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();
		else
			IdFormatSupport = IdFormat;

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			JUnt32 Duree;
			JDate DateExact;

			// Récupération du tarif
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

			// Si c'est une famille, calculer le tarif correspondant
			if (ElmtFamille)
			{
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
					                                      pDocChoixSupport->m_DateTarifaire, DateExact,
														  Duree, MapSuppSegTerr, TarifManuel);
			}

			// Tarif *= pDocChoixSupport->m_NbInsertions;
			// 1 seule insertion pour le moment
			Tarif *= 1;
		}

		// Ecriture du tarif
		if (Tarif > 0)
		{
			// Recup tarif format chaine avec espace milliers
			CString TxtTarif;
			TxtTarif.Format("%d",Tarif);
			pDocChoixSupport->FormatString(TxtTarif,3);

			// Tarif bien défini
			// m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 

			if (!TarifManuel)
			{
				// Tarif bien défini
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien défini (modif la couleur texte)
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else
			// Tarif non défini
			m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture Périodicité
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);
		
		// Si c'est une famille, il faut que tous les supports de même périodcité, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de périodicité abrégé (pour présentation tableau à droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 
	
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
		KeyResultMap.m_IdCible			= IdCible; 
		KeyResultMap.m_IdTerrain		= IdTerrain;
		KeyResultMap.m_IdSupport		= IdSupport; 

		// Test si la cible est autorisée
		if (pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain) == true)
		{
			// 1 seul insertion pour le moment
			// JUnt32 NbInsertions = pDocChoixSupport->m_NbInsertions;
			JUnt32 NbInsertions = 1;

			
			// Attention si déjà calculé dans échelle, pas la peine de recommencer
			// OPTIM 2.7
			CKeyResultEch KeyResultEch;
			KeyResultEch.m_IdCibleAff	= KeyResultMap.m_IdCibleAff;
			KeyResultEch.m_IdSegment	= KeyResultMap.m_IdSegment;
			KeyResultEch.m_IdMoteur		= KeyResultMap.m_IdMoteur;
			KeyResultEch.m_IdCible		= KeyResultMap.m_IdCible; 
			KeyResultEch.m_IdTerrain	= KeyResultMap.m_IdTerrain;
			KeyResultEch.m_IdSupport	= KeyResultMap.m_IdSupport;
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 

			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);

			if (!pDocChoixSupport->m_MapResultatEch.IsValid())
			{

				// Test si élément déjà calculé
				pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
				if (!pDocChoixSupport->m_MapResultatMap.IsValid())
				{
					// Pas de calcul pour le moment si famille avec calcul moyenne
					if (!ElmtFamille || pDocChoixSupport->m_CumulFamille == true)
					{
						// Balayage des colonnes
						for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
						{
							// Construction de la maille avec n insertions
							JFCMaille Maille;
							Maille.Reset(); 
							KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,pDocChoixSupport->m_Offre.m_IdItemAffinite);
							pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur,Maille,IdSupport,NbInsertions); 

							// Positionnement moteur pour calcul élmts affinité
							JFlt64 EffectifAffinite			= -1.0;
							JFlt64 VolAffinite				= -1.0;
							JFlt64 AudienceAffiniteLDP		= -1.0;
							JFlt64 AudienceAffiniteProba	= -1.0;
							JInt32 PtEtal					= -1.0;
							EffectifAffinite = pDocChoixSupport->m_MoteurPresse.GetEffectif(KeyMoteur);

							// Calcul Audience
							if (pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupport, VolAffinite, AudienceAffiniteLDP,AudienceAffiniteProba,PtEtal, true))
							{
								// Tenir compte du nombre d'insertions
								VolAffinite *= NbInsertions;
								AudienceAffiniteLDP *= NbInsertions;
							}

							// Audience affinité
							JFlt64 AudienceAffinite = -1;

							// Init Moteur (construction distribution)
							// Voir Jeremy avant on n'étatit pas obligé de tester VolAffinite
							if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille) && VolAffinite >= 0)
							{
								// Création Distribution via cible affinité
								JFCDistribution DistriAff = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolAffinite); 

								// Récupère audience
								AudienceAffinite = DistriAff.Item(1).AsJFlt64();  
							}

							// Mise en place des infos offres
							KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,IdCible);

							// Effectif et nb cas
							JFlt64 Effectif;
							JInt32 NbCas;
							JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

							// Calcul audience et volume lecture
							JFlt64 VolLecture		= -0.0;
							JFlt64 AudienceLDP		= -0.0;
							JFlt64 AudienceProba	= -1.0;

							// Audience
							JFlt64 Audience		= -1.0;
							JFlt64 Audience1Ins = -1.0;

							// Calcul Audience
							if (pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur,IdSupport,VolLecture,AudienceLDP,AudienceProba,PtEtal,true))
							{
								if (VolLecture >= 0)
								{
									// Tenir compte du nombre d'insertions
									VolLecture *= NbInsertions;
									// AudienceLDP *= pDocChoixSupport->m_NbInsertions;

									// Init Moteur (construction distribution)
									if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille))
									{	
										// Création Distribution
										JFCDistribution Distri = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLecture); 

										// Récupère audience
										Audience = Distri.Item(1).AsJFlt64();  
									}

									// Force Audience LDP si 1 seule insertion
									if (pDocChoixSupport->m_NbInsertions == 1)
									{
										CRegroupements * pReg = CRegroupements::GetInstance();
										// Récup id support
										IElem *pElem = pReg->GetIElemByID(IdSupport);

										// Test si c'est un couplage
										if(!pElem->IsCouplage())
										{
											// Audience devient Audience LDP
											Audience = AudienceLDP;

											// Audience affinité devient Audience LDP Affinité
											AudienceAffinite = AudienceAffiniteLDP;
										}
									}

									// Pour cumulativité (construction Maille et Calcul avec 1 insertions)
									if (pDocChoixSupport->m_NbInsertions > 1)
									{
										// Construction de la maille avec 1 insertions
										JFCMaille Maille1Ins;
										Maille1Ins.Reset(); 
										pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maille1Ins,IdSupport,1); 				

										// Init Moteur (construction distribution)
										if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille1Ins))
										{	
											// Volume lecture 1 insertion
											// JFlt64 VolLect1Ins = VolLecture / pDocChoixSupport->m_NbInsertions;
											JFlt64 VolLect1Ins = VolLecture / NbInsertions;

											// Création Distribution
											JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

											// Récupère audience
											Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  
										}
									}
									else
										// même audience, cas d'une seule insertion par support
										Audience1Ins = Audience;
								}
							}

							// Calcul Résultat Colonne (selon grandeur)
							ResultMap.m_AudienceLDP			= AudienceLDP;
							ResultMap.m_AudienceProba		= AudienceProba;
							ResultMap.m_Audience			= Audience; 
							ResultMap.m_Audience1Ins		= Audience1Ins;
							ResultMap.m_AudienceAffinite	= AudienceAffinite;
							ResultMap.m_AudienceAffiniteLDP = AudienceAffiniteLDP;
							ResultMap.m_AudienceAffiniteProba = AudienceAffiniteProba;
							ResultMap.m_Effectif			= Effectif;
							ResultMap.m_EffectifAffinite	= EffectifAffinite;
							ResultMap.m_NbCas				= NbCas;
							ResultMap.m_VolAffinite			= VolAffinite;
							ResultMap.m_VolLecture			= VolLecture;

							CString Txt = ResultCellule(IdSupport, NoLig, NoCol, 
														Tarif, ResultMap);

							// Ecriture du résultat
							m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);

							// Stockage résultat dans matrice
							pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
							if (!pDocChoixSupport->m_MapResultatMap.IsValid())
							{
								pDocChoixSupport->m_MapResultatMap.Add(KeyResultMap) = ResultMap; 
							}
						}
					}
				}

				else
				{
					// Elment déjà calculé, on prend directement les infos dans map résultats
					// Recherche les résultats dans le map
					ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem(); 

					// Balayage des colonnes abscisse et ordonnée
					for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
					{
						// Calcul Résultat Colonne (selon grandeur)
						CString Txt = ResultCellule(IdSupport, NoLig, NoCol,
													Tarif, ResultMap);

						// Ecriture du résultat
						m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);
					}
				}
			}
			else
			{
				// OPTIM 2.7 on récupère les résultats déjà calculées dans Echelle
				CResultEch	ResultEch;
				ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem(); 

				ResultMap.m_AudienceLDP				= ResultEch.m_AudienceLDP;
				ResultMap.m_AudienceProba			= ResultEch.m_AudienceProba;
				ResultMap.m_Audience				= ResultEch.m_Audience;
				ResultMap.m_Audience1Ins			= ResultEch.m_Audience1Ins;
				ResultMap.m_AudienceAffinite		= ResultEch.m_AudienceAffinite;
				ResultMap.m_AudienceAffiniteLDP		= ResultEch.m_AudienceAffiniteLDP;
				ResultMap.m_AudienceAffiniteProba	= ResultEch.m_AudienceAffiniteProba;
				ResultMap.m_Effectif				= ResultEch.m_Effectif;
				ResultMap.m_EffectifAffinite		= ResultEch.m_EffectifAffinite;
				ResultMap.m_NbCas					= ResultEch.m_NbCas;
				ResultMap.m_VolAffinite				= ResultEch.m_VolAffinite;
				ResultMap.m_VolLecture				= ResultEch.m_VolLecture;

				// Stockage résultat dans matrice
				pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
				if (!pDocChoixSupport->m_MapResultatMap.IsValid())
				{
					pDocChoixSupport->m_MapResultatMap.Add(KeyResultMap) = ResultMap; 
				}

				// Balayage des colonnes abscisse et ordonnée
				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
				{
					// Calcul Résultat Colonne (selon grandeur)
					CString Txt = ResultCellule(IdSupport, NoLig, NoCol,
						Tarif, ResultMap);

					// Ecriture du résultat
					m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);
				}
			}
		}
		else
		{
			// Cible non autorisee
			JBool CibleNonAutorisee = true;
		}
	}

	// Affiche résultat Familles avec moyenne des supports si cumul faminne non demandé
	if (pDocChoixSupport->m_CumulFamille == false)
		AfficheResultFamille();

	// Progress bar invisible
	m_ProgressResultMap.ShowWindow(SW_HIDE); 

	if (m_NbRowSupport > 0)
	{
		// Bloque modification cellule
		m_GridMapping.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
											m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
											m_GridMapping.GetColCount()-1),
									CGXStyle()
											.SetEnabled(false));

		// Retaille largeur colonnes supports/formats/périodicités
		m_GridMapping.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
													m_GridMapping.GetRowCount(),m_NoColPeriodicite-1),
										TRUE,GX_UPDATENOW);
		
		// Retaille hauteur header lignes (pour titres supports)
		m_GridMapping.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
													0,m_GridMapping.GetColCount()),
											TRUE,GX_UPDATENOW);
	}

	// Mise à jour des sélections supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise à jour du grid
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	// Flag calcul effectué
	pDocChoixSupport->m_ParamMapModif = false;
	// pDocChoixSupport->m_ParamMapModif = true;   // OPTIM 2.7

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage Résultat Familles
JVoid CDlgMapping::AfficheResultFamille()
{
	/* ici on calcul m^me si non visible, car point mapping nécessaire
	// Mode Famille non actif, pas de calcul
	if (pDocChoixSupport->m_AFFModeFamilles == FALSE)
		return;
	*/
	
	// Récup niveau de chacune des grandeurs
	JUnt32 NivTerrain, NivCible, NivGrandeur;
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
			NivTerrain  = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
			NivCible    = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
			NivGrandeur = i;
	}

	// Recup 1er terrain sélectionné
	JUnt32 IdTerrain;
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain sélectionné
		IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return;

	// Recup 1ere cible sélectionnée
	JUnt32 IdCible;
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible sélectionnée
		IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return;

	// Balaye toutes les lignes, sauf les supports et la moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention à la ligne moyenne
		if (IdSupport == 0)
			continue;

		// Voir si c'est un élmt famille -- Récupérer la liste des composants
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if(pElem->IsLot())
		{
			// Crée liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// Récup tarif famille
				CString	TxtTarif = m_GridMapping.GetValueRowCol(NoLig,m_NoColTarif-1); 

				// Vire les espaces
				TxtTarif.Replace(" ",""); 

				// Valeur Tarif
				JUnt32 TarifFamille = atof(TxtTarif);
				// JUnt32 TarifFamille = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColTarif-1));

				// Récupère la liste des titres
				pLot->GetFils(ListSup);

				// Nombre de composants de la famille
				JUnt32 NbCompoFamille = ListSup.GetCount(); 

				// Balayage des colonnes
				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
				{
					// Information résultat famille
					CKeyResultMap KeyResultMapFam;
					CResultMap	ResultMapFam;

					// Clé résultat famille
					KeyResultMapFam.m_IdCibleAff	=  pDocChoixSupport->m_Offre.m_IdItemAffinite;
					KeyResultMapFam.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 
					KeyResultMapFam.m_IdMoteur		= pDocChoixSupport->m_MoteurPresse.GetType(); 
					KeyResultMapFam.m_IdCible		= IdCible; 
					KeyResultMapFam.m_IdTerrain		= IdTerrain;
					KeyResultMapFam.m_IdSupport		= IdSupport; 

					// Test si élément déjà calculé
					pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMapFam); 
					if (!pDocChoixSupport->m_MapResultatMap.IsValid())
					{	
						// Nb Composants comptabilisés pour résultat famille
						JUnt32 NbCompoValid = 0;

						// Balaye tous les composants familles
						for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())   
						{
							// Récup composant famille
							JUnt32 IdCompo = ListSup.GetItem(); 

							// Clé archivage résultat
							CKeyResultMap KeyResultMap;

							// Résultat composant
							CResultMap	ResultMap;

							// Clé de recherche résultat (idem clef famille, seul id support change)
							KeyResultMap				= KeyResultMapFam;
							KeyResultMap.m_IdSupport	= IdCompo; 

							// Test si élément déjà calculé ou élmt moyenné
							pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 

							if (pDocChoixSupport->m_MapResultatMap.IsValid())
							{
								// Elment déjà calculé, on prend directement les infos dans map résultats
								ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem(); 

								// Attention aux résultats non définis
								if (ResultMap.m_Audience > 0)
								{
									// Cumul résultat colonne pour famille
									ResultMapFam.m_Audience					+= ResultMap.m_Audience; 
									ResultMapFam.m_Audience1Ins				+= ResultMap.m_Audience1Ins;
									ResultMapFam.m_AudienceAffinite			+= ResultMap.m_AudienceAffinite;
									ResultMapFam.m_AudienceAffiniteLDP		+= ResultMap.m_AudienceAffiniteLDP;
									ResultMapFam.m_AudienceAffiniteProba	+= ResultMap.m_AudienceAffiniteProba;
									ResultMapFam.m_AudienceLDP				+= ResultMap.m_AudienceLDP;
									ResultMapFam.m_AudienceProba			+= ResultMap.m_AudienceProba;
									ResultMapFam.m_Effectif					+= ResultMap.m_Effectif;
									ResultMapFam.m_EffectifAffinite			+= ResultMap.m_EffectifAffinite;
									ResultMapFam.m_NbCas					+= ResultMap.m_NbCas;
									ResultMapFam.m_VolAffinite				+= ResultMap.m_VolAffinite;
									ResultMapFam.m_VolLecture				+= ResultMap.m_VolLecture;

									// 1 composant de + pris en compte
									NbCompoValid++;
								}
							}
						}

						// Si au moins 1 composant valide, on fait la moyenne
						if (NbCompoValid != 0)
						{
							// Attention si calcul famille avec moyenne
							ResultMapFam.m_Audience					/= NbCompoValid; 
							ResultMapFam.m_Audience1Ins				/= NbCompoValid;
							ResultMapFam.m_AudienceAffinite			/= NbCompoValid;
							ResultMapFam.m_AudienceAffiniteLDP		/= NbCompoValid;
							ResultMapFam.m_AudienceAffiniteProba	/= NbCompoValid;
							ResultMapFam.m_AudienceLDP				/= NbCompoValid;
							ResultMapFam.m_AudienceProba			/= NbCompoValid;
							ResultMapFam.m_Effectif					/= NbCompoValid;
							ResultMapFam.m_EffectifAffinite			/= NbCompoValid;
							ResultMapFam.m_NbCas					/= NbCompoValid;
							ResultMapFam.m_VolAffinite				/= NbCompoValid;
							ResultMapFam.m_VolLecture				/= NbCompoValid;
						}

						// Positionne Flag résultat partiel
						if (NbCompoValid != NbCompoFamille || NbCompoFamille == 0)
						{
							// Ce sont des résultats partiels, manque certains supports
							ResultMapFam.m_ResultPartiel = true;
						}

						//Voir si tous les composants famille ont été trouvé
						// if (NbCompoValid == NbCompoFamille && NbCompoFamille != 0)
						if (NbCompoValid != 0)
						{
							// Ecriture résultat famille
							CString Txt = ResultCellule(IdSupport,NoLig, NoCol, TarifFamille, ResultMapFam);

							// Ajoute dans matrice résultat si nouvelle clef résultat famille
							pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMapFam); 
							if (!pDocChoixSupport->m_MapResultatMap.IsValid())
							{
								pDocChoixSupport->m_MapResultatMap.Add(KeyResultMapFam) = ResultMapFam; 
							}

							/*
							// Distinguer les résultats famille non complets (calcul avec seulement
							// les composants valides)
							if (ResultMapFam.m_ResultPartiel)
							{
								// On distingue la ligne compléte pour cette étude "semi-calculée"
								m_GridMapping.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
															CGXStyle()
															.SetHorizontalAlignment(DT_RIGHT)
															.SetVerticalAlignment(DT_BOTTOM)
															.SetTextColor(CATPColors::GetColorRed())
															.SetFont(CGXFont()
																.SetSize(pDocChoixSupport->m_SizeFontCell)
																.SetBold(pDocChoixSupport->m_BoldFontCell)
																.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));

							}
							*/
						}
						else
						{
							// Résultat famille non définie car manque prise en compte certains composants
							// non défini
							m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),"*");
						}
					}
					else
					{
						// Element famille déjà calculé, on prend directement les infos dans map résultats
						ResultMapFam = pDocChoixSupport->m_MapResultatMap.GetItem(); 

						// Calcul Résultat Colonne (selon grandeur)
						CString Txt = ResultCellule(IdSupport,NoLig, NoCol, TarifFamille, ResultMapFam);

						/*
						// Attention aux résultats partiels, manque certains supports
						if (ResultMapFam.m_ResultPartiel)
						{
							// On distingue la ligne compléte pour cette étude "semi-calculée"
							m_GridMapping.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
														CGXStyle()
														.SetHorizontalAlignment(DT_RIGHT)
														.SetVerticalAlignment(DT_BOTTOM)
														.SetTextColor(CATPColors::GetColorRed())
														.SetFont(CGXFont()
															.SetSize(pDocChoixSupport->m_SizeFontCell)
															.SetBold(pDocChoixSupport->m_BoldFontCell)
															.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));
						}
						*/
					}					
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage résultat mapping (selon grandeurs abscisse et ordonnée sélectionnées)
JBool CDlgMapping::AfficheResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridMapping.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultMap KeyResultMap;

	// Information résultat archivé
	CResultMap	ResultMap;

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
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridMapping.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridMapping.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultMap.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultMap.GetRange( nLower, nUpper);

	// Clef affinité courante
	KeyResultMap.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice clé segment
	KeyResultMap.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 

	// Calcul indice clé moteur
	KeyResultMap.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Récupére map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);
	
	// Balayage ligne support
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultMap.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));



		// Attention si c'est une famille
		JBool ElmtFamille = false;
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un élmt famille
			ElmtFamille = true;
		}

		// Récupère le format support ligne sélectionné pour le mapping
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();
		else
			IdFormatSupport = IdFormat;

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			JUnt32 Duree;
			JDate DateExact;

			// if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
			// 	Tarif = 0;
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

			// Si c'est une famille, calculer le tarif correspondant
			if (ElmtFamille)
			{
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
														  pDocChoixSupport->m_DateTarifaire, DateExact,
														  Duree, MapSuppSegTerr, TarifManuel);
			}

			// Tarif *= pDocChoixSupport->m_NbInsertions;
			// 1 seule insertion pour le moment
			Tarif *= 1;
		}

		// Ecriture du tarif
		if (Tarif > 0)
		{
			// Recup tarif format chaine avec espace milliers
			CString TxtTarif;
			TxtTarif.Format("%d",Tarif);
			pDocChoixSupport->FormatString(TxtTarif,3);

			// Tarif bien défini
			// m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			if (!TarifManuel)
			{
				// Tarif bien défini
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien défini (modif la couleur texte)
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
		}
		else
			// Tarif non défini
			m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture Périodicité
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);
		
		// Si c'est une famille, il faut que tous les supports de même périodcité, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de périodicité abrégé (pour présentation tableau à droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

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

		// Définit clef résultat
		KeyResultMap.m_IdSupport = IdSupport;
		KeyResultMap.m_IdTerrain = IdTerrain;
		KeyResultMap.m_IdCible	 = IdCible;

		/*
		// Init les indications résultats complets
		m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NbHeaderCol, NoLig, m_GridMapping.GetColCount()),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS))
										.SetFont(CGXFont()
											.SetItalic(pDocChoixSupport->m_ItalicResultComplet)));
		*/

		// Boucle sur les colonnes supports
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
		{
			// A FAIRE 
			// Recherche Résultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur résultats
			pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
			if (pDocChoixSupport->m_MapResultatMap.IsValid())
			{
				// Information résultat archivé
				CResultMap	&ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem();

				// Le résultat pour la cellule
				Txt = ResultCellule(IdSupport, NoLig, NoCol,
									Tarif, ResultMap); 
				// Ecriture du résultat
				m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);

				/*
				// Attention aux résultats partiels, manque certains supports
				if (ResultMap.m_ResultPartiel)
				{
					// On distingue la ligne compléte pour cette étude "semi-calculée"
					m_GridMapping.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
												CGXStyle()
													.SetTextColor(CATPColors::GetColorRed())
													.SetFont(CGXFont()
														.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));
				}
				*/
			}
			else
			{
				// Pas de résultat défini
				Txt = "*";
				m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}

		}
	}

	// Progress bar invisible
	m_ProgressResultMap.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridMapping.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                 m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										 m_GridMapping.GetColCount()-1),
									 CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/périodicité
	m_GridMapping.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                    m_GridMapping.GetRowCount(),m_NoColPeriodicite-1),
                                       TRUE,GX_UPDATENOW);
	
	// Retaille hauteur header lignes (pour titres supports)
	m_GridMapping.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
			                                     0,m_GridMapping.GetColCount()),
		                                TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise à jour valide
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	return true; 
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélections supports
bool CDlgMapping::UpdateSelSupports()
{
	// Nombre de supports nouvelle sélection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRow();

		// Initialise libellés supports en 1ere colonne
		InitRowSupports();
		
		// Initialise largeur colonne
		InitSizeColonne();

		// Mise à jour du grid
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur précédemment existant, 
		// et nouvelle liste sans ascenseur !!!
		m_GridMapping.SetTopRow(m_NbHeaderRow, GX_UPDATENOW); 
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Init lignes supports
void CDlgMapping::InitRowSupports()
{
	// Lockage des modifs
    m_GridMapping.LockUpdate(true);

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Récupère objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Nb lignes et nb colonnes du grid
	int NbCol = m_GridMapping.GetColCount();
	int NbRow = m_GridMapping.GetRowCount();

	// Nombre de supports
	JUnt32 NbSupports = pDocChoixSupport->m_MapIdSupportsSel.GetCount();

	if (m_NbHeaderCol >= m_NoColTarif && NbSupports > 0)
	{
		// Style check-box libellés supports
		m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColSelSupport - 1,
											 InxLigSupp + NbSupports - 1,
											 m_NoColSelSupport - 1),
									CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

		// Style texte libellés supports
		m_GridMapping.SetStyleRange(CGXRange(InxLigSupp -1 ,
			                                 m_NoColSupport - 1,
			                                 InxLigSupp + NbSupports - 1,
											 m_NoColSupport - 1),
									CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_VCENTER)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontHeader)
										.SetBold(pDocChoixSupport->m_BoldFontHeader)));
	
		// Style texte libellés périodicité
		m_GridMapping.SetStyleRange(CGXRange(InxLigSupp-1,
											 m_NoColPeriodicite-1,
											 InxLigSupp + NbSupports - 1,
											 m_NoColPeriodicite-1),
									CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_VCENTER)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)
										.SetItalic(FALSE)));

		// Style texte libellés tarif
		m_GridMapping.SetStyleRange(CGXRange(InxLigSupp-1,
											 m_NoColTarif-1,
											 InxLigSupp + NbSupports - 1,
											 m_NoColTarif-1),
									CGXStyle()
										.SetHorizontalAlignment(DT_RIGHT)
										.SetVerticalAlignment(DT_VCENTER)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

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
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdSupport));

				// on récupére le libellé support
				JString LibSupport = "-";
				if (pSupport)
				{
					// Affichage libellé support
					LibSupport = pSupport->GetLabel().AsJCharPtr();
				}

				// Affichage libellé support
				CString LibSupp = LibSupport.AsJCharPtr();
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibSupp));

				// Récupère info formats support
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;
  				pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleMap);
						
				// Affichage libellé format du support
				if (InxSelFormat >= 0)
					// Au moins 1 format possible
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_RIGHT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetControl(IDS_CS_CTRL_INDEXDROPDOWNLIST)
													.SetValue(InxSelFormat)
													.SetChoiceList(ListFormat),
													gxOverride,0,GX_SMART);
													// gxApplyNew);
													// gxExclude,0,GX_SMART);
				else
				{
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_RIGHT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetValue("*"),
													gxOverride,0,GX_SMART);
													// gxApplyNew);
													// gxExclude,0,GX_SMART);
				}

				// Puis indication support sélectionné ou pas
				if (EtatSupport)
				{
					// Valide sélection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// Dévalide la sélection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incrémente index ligne
				InxLigSupp++;
			}
		}

		// Balaye toutes les familles associées aux supports
		for (pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveFirst();
			 pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid();
			 pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveNext())
		{
			// Ident famille
			JUnt32 IdFamille = pDocChoixSupport->m_Offre.m_MapSelFamilles.GetKey();
			pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdFamille);

			// Vérifie qu'il est valide dans le map sélection
			if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
			{
				// Etat Support
				JBool EtatSupport = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

				// Element famille
				IElem* pFamille = pRegroupements->GetIElemByID(IdFamille);

				// Indique Id Support dans colonne invisible
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdFamille));

				// on récupére le libellé famille
				JString LibFamille = "-";

				// Affichage libellé famille
				LibFamille = pFamille->GetLabel().AsJCharPtr();
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibFamille.AsJCharPtr()));
				
				// Format famille non défini (avec espace à droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;

				// Récupère liste des formats pour la famille
				pDocChoixSupport->GetFormatsFamille(IdFamille, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libellé format du support
				if (InxSelFormat >= 0)
					// Au moins 1 format possible
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_RIGHT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetControl(IDS_CS_CTRL_INDEXDROPDOWNLIST)
													.SetValue(InxSelFormat)
													.SetChoiceList(ListFormat),
													gxOverride,0,GX_SMART);
				else
				{
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_RIGHT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetValue(LibFormatNull),
													gxOverride,0,GX_SMART);
				}
	
				/*
				// Affichage libellé format de la famille > indique non défini
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
											CGXStyle()
												.SetHorizontalAlignment(DT_LEFT)
												.SetVerticalAlignment(DT_VCENTER)
												.SetFont(CGXFont()
													.SetSize(pDocChoixSupport->m_SizeFontHeader)
													.SetBold(pDocChoixSupport->m_BoldFontHeader))
												.SetValue(LibFormatNull),
												gxOverride,0,GX_SMART);
				*/	

				// Puis indication famille sélectionné ou pas
				if (EtatSupport)
				{
					// Valide sélection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
											CGXStyle()
												.SetValue((long)1));
				}
				else
				{
					// Dévalide la sélection famille
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incrémente index ligne
				InxLigSupp++;
			}
		}

		// Mise à jour couleur sélection supports
		UpdateVisuSupports();
	}
	
	// Mise à jour des modifs
	m_GridMapping.LockUpdate(false);
	m_GridMapping.UpdateWindow();
	m_GridMapping.Redraw();	

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Init Taille des colonnes
void CDlgMapping::InitSizeColonne()
{
	// Lockage des modifs
    m_GridMapping.LockUpdate(true);

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Colonne Id Support invisible
	m_GridMapping.SetColWidth(m_NoColIdSupport-1,m_NoColIdSupport-1,0);

	// Retaille la colonne textes supports
	m_GridMapping.SetColWidth(m_NoColSelSupport-1,m_NoColSelSupport-1,20);

	// Retaille la colonne périodicité et tarif
	m_GridMapping.SetColWidth(m_NoColPeriodicite-1,m_NoColTarif-1,60);

	// Retaille la colonne résultat abscisse et ordonnée
	m_GridMapping.SetColWidth(m_NoColTarif,m_NoColTarif+1,80,0,GX_UPDATENOW);

	// Mise à jour des modifs
	m_GridMapping.LockUpdate(false);
	m_GridMapping.UpdateWindow();
	m_GridMapping.Redraw();	

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgMapping::UpdateVisuSupports()
{
	// Debut et fin marquage support sélectionné
	JUnt32 NoColDeb = m_NoColSupport-1;    
	JUnt32 NoColFin = m_GridMapping.GetColCount();  // avant m_NoColTarif-1

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup élmt
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Puis indication support sélectionné ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Support sélectionné, on surligne la ligne
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  											CGXStyle()
												.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
			}
			else
			{
				// Support non sélectionné, on laisse la ligne en blanc
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  											CGXStyle()
												.SetInterior(CATPColors::GetColorWhite()));
			}

			// Différencier les familles (soit couleur, soit font)
			if (pElem->IsLot())
			{
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
											CGXStyle()
												.SetInterior(CATPColors::GetColorBeige()));
			}
		}
	}

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélection supports
void CDlgMapping::UpdateEtatSelSupports()
{
	// Autorise modif grid et bloque mise à jour
	//m_GridMapping.SetReadOnly(false);
	m_GridMapping.LockUpdate(true); 

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat sélection en cours
			CGXStyle Style;
			m_GridMapping.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 

			// Puis indication support sélectionné ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la sélection support
				if (ValSel != 1)
				{
					m_GridMapping.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
			}
			else
			{
				// Dévalide la sélection support
				if (ValSel != 0)
				{
					m_GridMapping.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}
			}
		}

		// Le support est-il visible
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Vérifie élment valide via le mode sélectionnée (mode Supports et/ou mode Familles)
		JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		// Récupération validité support sur terrain(s)/segment(s)
		JBool ValideSupport = true;
		if (pElem)
		{
			if (pElem->IsTitre() || pElem->IsCouplage())
					pDocChoixSupport->ValideSupportSegments(IdSupport);
		}
		
		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false || VisibleSupport == false)
		{	
			// Cache la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,0); 
		}
		else
			// Rend visible la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,16);
	}
	
	// Mise à jour affichage
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération sélection Nb Insertions
void CDlgMapping::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi sélection Nb Insertions
JUnt32 CDlgMapping::GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Update résultat mapping
bool CDlgMapping::UpdateResultMapping(JBool CalculResult)
{
	// Test validité sélection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si sélection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// Désactive mise à jour grid dup
	m_GridMapping.LockUpdate(true);

	// Mise à jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise à jour nombre et headers rows
	if (!UpdateRow()) return false;

	// Calcul des résultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Remettre les sélections supports et/ou famille, et réafficher proprement les colonnes
	UpdateVisuSupports();
	UpdateEtatSelSupports();
	AfficheResultat();

	// Bloque resize hauteur des lignes
	m_GridMapping.GetParam()->EnableTrackRowHeight(FALSE);

	// Active mise à jour grid dup
	m_GridMapping.LockUpdate(false);

	// reddessine le grid dup
	m_GridMapping.Redraw();

	// Rend visible le grid, si au moins quelques résultats
	if (m_GridMapping.GetColCount() >= m_NoColPeriodicite)
	{
		m_GridMapping.ShowWindow(SW_SHOW);
		m_CadreGridMapping.SetTransparent(true);
	}
	else
	{
		m_GridMapping.ShowWindow(SW_HIDE);
		m_CadreGridMapping.SetTransparent(false);
	}

	// Update affichage infos analyse
	UpdateTitreAnalyse();

	// Test si les supports ont changés
	if (SupportModif())
	{
		// Mise à jour du mapping avec modfication des supports
		MajMapping(true);
	}
	else
	{
		// Mise à jour du mapping en gardant les mêmes supports
		MajMapping(false);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Les supports du mapping ont-ils été modifiés
JBool CDlgMapping::SupportModif()
{
	// Test déjà si pas le même nombre de supports
	if (m_BagMining.GetCount() != pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{
		return true;
	}
	else
	{
		// Balaye les elmts ident mapping
		for (JUnt32 i = 0; i < m_BagMining.GetCount(); i++)
		{
			// Récup ident support mapping
			JUnt32 IdSuppMapping = m_BagMining.GetIdent(i);

			// Test si présent dans étude en cours
			pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSuppMapping);

			// Si non présent, on redessinera le mapping
			if (!pDocChoixSupport->m_MapIdSupportsSel.IsValid())
				return true;
		}
	}

	// Mêmes supports
	return false;
}	

//////////////////////////////////////////////////////////////////////////////////////////////
// Update des résultats au niveau grille cellule (ne touche pas aux headers)
bool CDlgMapping::UpdateResultat(JBool CalculResult)
{
	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Init Affichage Résultats Cellules
	if (m_GridMapping.GetColCount() > 0 && m_GridMapping.GetRowCount() > 0)
	{
		m_GridMapping.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridMapping.GetRowCount(),m_GridMapping.GetColCount()),
									CGXStyle()
										.SetHorizontalAlignment(DT_RIGHT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

		if (GrilleResultValid())
		{
			if (CalculResult && pDocChoixSupport->m_ParamMapModif)
			{
				// Mise à jour des résultats cellules
				if (!InitGridResultat()) 
					return false;
			}
			else
			{
				// Affichage des résultats cellules
				if (!AfficheResultat()) 
					return false;
			}
		}		

		// On bloque les colonnes qui doivent ne pas être modifiées
		m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélections format supports
bool CDlgMapping::UpdateSelFormatSupports()
{
	// Initialise libellés supports en 1ere colonne
	InitRowSupports();

	// Mise à jour du mapping
	MajMapping(true);

	return true;
}

//////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CDlgMapping::UpdateAllViews(const JFCUpdate & Message)
{
	// Réinit grille mapping et graphe mapping
	if (Message == UPDATE_RAZ)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialise grid résultat
	    m_GridMapping.Initialize();
		GXSetNewGridLineMode(); 
		m_GridMapping.GetParam()->EnableUndo(FALSE);
		m_GridMapping.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridMapping.GetParam()->SetNumberedColHeaders(false); 
		m_GridMapping.GetParam()->SetNumberedRowHeaders(false); 
		m_GridMapping.GetParam()->SetSmartResize(true);

		// Mise à jour des infos fenêtre mapping
		UpdateTitreAnalyse();

		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour du graphe
		MajMapping(true);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mise à jour couleur Interface Choix Supports
	else if(Message == UPDATE_COLOR)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mise à jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		//Init Contrôles
		// SetupControls();

		// Mise à jour des infos fenêtre échelle
		UpdateTitreAnalyse();

		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour des nouveaux résultats (grid + graphe mapping)
		// @@@@@ Pourquoi le faire ici, car ça prend du temps quand on est dans échelle
		// il faut trouver un moyen de le faire dans le
		InitGridResultat();

		/*
		// Mise à jour du mapping
		MajMapping();
		*/

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/

	}

	// Mise a jour suite à changement Offres
	else if (Message == UPDATE_OFFRE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour du mapping
		MajMapping(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise à jour format si nouveaux supports
		MajFormat();

		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour des nouveaux résultats (grid + graphe mapping)
		// ????????????????????? A VOIR si on l'enleve
		// InitGridResultat();

		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		// ?????????????????????????? A VOIR Si on l'enleve UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
	}

	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// A FAIRE
		// Mise à jour du mapping
		MajMapping(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Mise à jour du mapping
		MajMapping(false);
		
		// Update état sélection supports
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise à jour du mapping
		MajMapping(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour du mapping
		MajMapping(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise à jour résultats coût
		if (this->IsWindowVisible())
			UpdateResultat(true);
		else
			pDocChoixSupport->m_ParamMapModif = true;

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise a jour suite à changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour du mapping
		MajMapping(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise à jour résultats coût
		if (this->IsWindowVisible() == TRUE)
			UpdateResultat(true);
		else
			pDocChoixSupport->m_ParamMapModif = true;

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	/* Pour le moment 1 seule insertion ==> invariable
	// Mise à jour suite au changement 
	else if (Message == UPDATE_INSERTIONS)
	{
		// Vire le map courant résultat
		pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleMap);

		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour des entêtes headers car répétition ou reprise en main)
		UpdateRow();

		// Mise à jour des résultat (uniquement si fenêtre mapping visible)
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamMapModif = true;

		// Mise à jour du mapping
		MajMapping(true);
	}
	*/

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Mise à jour résultats coût
		UpdateResultat(false);

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise à jour suite à modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse(); 

		// Mise à jour des résultat --> déjà fait dans dialogue générale
		// InitGridResultat();
	}

	// Mise à jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise à jour grid mapping
		m_GridMapping.LockUpdate(true);

		// Mise à jour des résultat coûts
		UpdateResultCout();

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		// Mise à jour du graphe
		MajMapping(true);

		// Active mise à jour grid mapping
		m_GridMapping.LockUpdate(false);

		// reddessine le grid mapping
		m_GridMapping.Redraw();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise à jour des taris suite à modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise à jour grid mapping
		m_GridMapping.LockUpdate(true);

		// Mise à jour des résultat coûts
		UpdateResultCout();

		// Mise à jour du graphe
		MajMapping(true);

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		/*
		// Mise à jour du grid mapping
		InitGrid();

		// Mise à jour des nouveaux résultats (grid + graphe mapping)
		// @@@@@ Pourquoi le faire ici, car ça prend du temps quand on est dans échelle
		// il faut trouver un moyen de le faire dans le
		InitGridResultat();

		
		// A VOIR /////////////////////////////////////////////////////////////
		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(false);

		// mise à jour du mapping
		MajMapping(false);

		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		///////////////////////////////////////////////////////////////////////

		// Active mise à jour grid mapping
		m_GridMapping.LockUpdate(false);
		*/

		// reddessine le grid mapping
		m_GridMapping.Redraw();
	}

	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise à jour grid mapping
		m_GridMapping.LockUpdate(true);

		MajFormat();

		// Affichage ou non des supports dans grille mapping
		UpdateAfficheSupports();

		// Active mise à jour grid mapping
		m_GridMapping.LockUpdate(false);

		// redessine le grid mapping
		m_GridMapping.Redraw();

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(false);
		// UpdateResultMapping(true);		// OPTIM 2.7

		// mise à jour du mapping
		MajMapping(false);

		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise à jour des différentes fenêtres 
		UpdateResultMapping(true);

		// Mise à jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// on redessine le mapping
		// m_Mapping.RedrawWindow();
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports non définis
JVoid CDlgMapping::MajFormat()
{
	// Flag si maj effective
	JBool MajOk = false;

	// Identificateur format courant
	JUnt32 IdFormat = 0;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    

	// Boucle sur tous les supports, on ne traitera que les supports non défini au niveau format
	for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// Ident support
		JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

		// Voir si support non défini au niveau format
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (!pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
		{
			// Récupération des idents libellés format possible pour un support
			JMap <JUnt32,JUnt32> MapIdFormats;
			pDocChoixSupport->GetListIdFormats(IdSupport, &MapIdFormats);

			// Si le format courant appartient aux formats possibles supports, 
			// on le met comme format sélectionné pour les nouveaux supports
			MapIdFormats.MoveTo(IdFormat);
			if (MapIdFormats.IsValid())
			{
				// Indique information format associé à ce support
				pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;		
			}
			else
			{
				// Pas de format pour le moment associé à ce support
				pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = 0;		
			}

			// Mise à jour format pour les nouveaux supports
			MajOk = true;
		}
	}

	// Init infos rows supports
	if (MajOk == true) 
	{	
		// Mise à jour des supports sélectionnés
		UpdateSelSupports();
		
		// Initialisation lignes support
		InitRowSupports();
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgMapping::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Update Résultat
	m_GridMapping.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Récupère état visibilité support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Récupération validité support sur terrain(s)/segment(s)
		JBool ValideSupport = true;
		if (pElem)
		{
			if (pElem->IsTitre() || pElem->IsCouplage())
					pDocChoixSupport->ValideSupportSegments(IdSupport);
		}

		// Vérifie élment valide via le mode sélectionnée (mode Supports et/ou mode Familles)
		JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false || VisibleSupport == false)
		{	
			// Cache la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,0); 

			// Mise à jour graphe mapping
			m_BagMining.SetShowItemById(IdSupport,false); 
		}
		else
		{
			// Rend visible la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,16); 

			// Mise à jour graphe mapping
			m_BagMining.SetShowItemById(IdSupport,true); 
		}
	}

	// Mise à jour du mapping
	m_Mapping.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour du document Mapping (avec réinit ou pas des positions)
void CDlgMapping::MajMapping(JBool InitPos)
{
	// Attention, si réinitialisation, on n'oublies pas de virer les infos courbes
	if (InitPos)
	{
		// Initialise  toutes les infos mapping
		m_Mapping.Reset();
		
		// on init par default les courbes
		for (long IndCourbe = 0; IndCourbe < m_BagCurve.GetNbCourbeMax(); IndCourbe++)
			m_BagCurve.ResetCourbe(IndCourbe);
	}

	// Update points mapping
	if (m_BagMining.UpdatePoints(InitPos))
	{
		// on met à jour le mapping
		m_Mapping.SetItems    (&m_BagMining   );

		// Récup infos courbes
		m_Mapping.SetCurve    (&m_BagCurve    );

		//	m_Mapping.SetHierarchy(&m_BagHierarchy);

		// repositionne état affichage
		if (m_BagMining.GetLibAfficher())
		{
			m_BagMining.SetShowAllLibelle(true); 
		}
		else
		{
			m_BagMining.SetShowAllLibelle(false); 
		}

		// on redessine le mapping
		m_Mapping.UpdateWindow();
	}

	// on redessine le mapping
	m_Mapping.RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour la sélection
void CDlgMapping::UpdateSelection()
{
	// on redessine le mapping
	m_Mapping.InvalidateRect(NULL, FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports dans mapping
void CDlgMapping::UpdateFormatSupports()
{
	// on teste la validité du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise à jour map des formats supports mapping
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		pDocChoixSupport->m_MapIdFormatSpecifMap.Reset();
		for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
				pDocChoixSupport->m_MapIdSupportsSel.IsValid();
				pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
		{
			// Ident support
			JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

			// Indique information format associé à ce support
			pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;	
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Définit si la grille résultat est valide
bool CDlgMapping::GrilleResultValid()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount()  			> 0		&&
		pDocChoixSupport->m_NbInsertions							> 0		&& 
		pDocChoixSupport->m_DateTarifaire.IsValid())
	{
		// La grille est valide
		return true;
	}

	// Pas de grille résultat définie
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Update Entete Analyse (Nom de la source / terrain et date tarification)
void CDlgMapping::UpdateTitreAnalyse()
{
	CString Titre = "Analyse non définie";
	if (pDocChoixSupport)
	{
		// On vérifie qu'une source terrain a été choisi, et auusi une date tarification
		if (pDocChoixSupport!=0 && pDocChoixSupport->IsValid())    
		{
			// Ajoute Info Filtre d'audience
			JSRCPresse* m_srcPresse;
			m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

			// Récup Id Filtre audience du module courant
			JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

			// Récupère label 
			Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
		
			// Ajoute libellé de la source
			JUnt32 SrcID = 0;
			if (pDocChoixSupport->GetIdSrcAudienceSel())
			{
				// Récup Table
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();

				// Récup la source audience courante
				SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

				// Récup le nouveau source
				const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

				// Recuperer dans table source
				// Titre += " - ";
				// Titre += pt->m_Libelle.AsJCharPtr();
			}
			else
			{
				// Titre += " - Aucune source";
			}

			// Ajoute Info Date Tarification
			if (pDocChoixSupport->m_DateTarifaire.IsValid())
			{
				CString TxtDate;
				JInt32 Day, Month, Year;
				pDocChoixSupport->m_DateTarifaire.GetDate(Day,Month,Year); 
				TxtDate.Format("Date tarifaire : %02d/%02d/%04d", Day,Month,Year);
				Titre += " - ";
				Titre += TxtDate;
			}
			else
				Titre += " - Aucune date - ";

			// Ajoute info Cible >> PLUS BESOIN DANS ECHELLE CAR MULTI-CIBLES
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
					Titre += pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
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

		}

		// Ecrit le titre analyse non définie
		m_TitreMapping.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridMapping.GetRowCount() > 1)
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
			m_GridMapping.GetParam()->SetLockReadOnly(FALSE);
			m_GridMapping.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtMoteur));

			// On bloque les colonnes qui doivent ne pas être modifiées
			m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}

		// Nombre d'insertions désormais dans le header au dessus de "Supports"
		if (m_GridMapping.GetRowCount() > 1)
		{
			// Texte inf onb insertions
			CString TxtNbInsertion;
			// TxtNbInsertion.Format("%s : %d","Nombre d'insertions",pDocChoixSupport->m_NbInsertions);
			// Nombre d'insertion forcée à 1 pour le moment
			TxtNbInsertion.Format("%s : %d","Nombre d'insertions",1);

			// On l'affiche dans la grille
			//m_GridMapping.SetReadOnly(FALSE);
			m_GridMapping.GetParam()->SetLockReadOnly(FALSE);
			m_GridMapping.SetStyleRange(CGXRange(1,m_NoColSelSupport,1,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtNbInsertion));

			// On bloque les colonnes qui doivent ne pas être modifiées
			m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Update Résultat des Coûts (suite à modif Format ou Tarif)
JVoid CDlgMapping::UpdateResultCout()
{
	// Mise à jour fenêtre échelle
	UpdateResultat(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Affiche barre de calcul
void CDlgMapping::AffichBarreCalcul()
{
	// A FAIRE
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de lignes header
JUnt32 CDlgMapping::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de colonnes header
JUnt32 CDlgMapping::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export des résultats Mapping
void CDlgMapping :: ExportResultat()
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
		
		// Export résultat Mapping
		TxtEntete = "BLOC 2\n";
		ExportFile.WriteString(TxtEntete);

		// Copie des infos tableurs dans fichier export en 2 passes
		// m_GridMapping.CopyTextToFile( ExportFile, CGXRange(m_NbHeaderRow-1, m_NoColSelSupport-1, m_GridMapping.GetRowCount(), m_GridMapping.GetColCount()));
		for (int lig = m_NbHeaderRow-1; lig < m_GridMapping.GetRowCount(); lig++)
		{
			for (int col = m_NoColSelSupport-1; col < m_GridMapping.GetColCount(); col++)
			{
				CString Txt = m_GridMapping.GetValueRowCol(lig, col) + "\t";
				ExportFile.WriteString(Txt);
			}
			// Saut de ligne
			TxtEntete = "\n";
			ExportFile.WriteString(TxtEntete);
		}		

		// Bloc n° 3 : Infos Terrain + Cible + Effectif + Nb Cas
		// ------------------------------------- Les effectifs des cibles utilisées -------------------------------------------
		//
		// Récupération du bloc infos terrains / cibles
		RecupBlocTerrainCible(TxtEntete);

		// Ecriture entete dans fichier export
		ExportFile.WriteString(TxtEntete);

		// Fin Export
		ExportFile.Close();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export du graphique Mapping
void CDlgMapping :: ExportGraphique()
{
	// Demande emplacement du fichier export
	CFileDialog DlgFile(FALSE,".emf",0,
						OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Text Files (*.emf)|*.emf|All Files (*.*)|*.*||",this);

	if (DlgFile.DoModal( ) == IDCANCEL)
		return;

	// Test si au moins 1 support sélectionné pour le mapping
	if(!m_Mapping.ExportItems(480,DlgFile.GetPathName()))
		AfxMessageBox("Erreur lors de l'export");	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération de l'entête pour export Mapping
void CDlgMapping :: RecupEnteteExport(CString &Entete)
{
	// N° Version
	Entete  = "No de version :\t";
	Entete += "1.0\n";

	// Nom du plateau
	Entete += "NOM PLATEAU :\t";
	Entete += pDocChoixSupport->m_KEYPlateau.GetLabel() + "\n";

	// Init entete
	Entete += "EXPORT MAPPING \n\n";

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

	//if (NbCtx)
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
	for (JUnt32 NoLig = 0; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

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

	// Boucle sur les élmts segment audience sélectionnés pour mapping
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

	// --------------------------------------------- Export Nb Insertions ------------------------------------------
	//
	// on exporte le titre
	Entete += "Nb insertions : \t";

	// str.Format("%d", pDocChoixSupport->m_NbInsertions);
	// 1 seule insertion pour le moment
	str.Format("%d", 1);
	Entete += str;
	Entete += "\n";

	// Prévoir sauts de ligne avant affichage infos tableurs
	Entete += "\n\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération résultats mapping détaillée
void CDlgMapping ::RecupGrilleExport(CString &Entete)
{
	// Init chaine résultat
	Entete = "";

	// Récup niveau de chacune des grandeurs
	JUnt32 NivTerrain, NivCible, NivGrandeur;
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
			NivTerrain  = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
			NivCible    = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
			NivGrandeur = i;
	}

	// Tableau de lignes et colonnes sélectionnées
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridMapping.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridMapping.GetSelectedRows(Rows,false,false);

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
	for (int nRow = 2; nRow < m_NbHeaderRow ; nRow++)
	{
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
		{	
			if (nRow == m_NbHeaderRow-1)
			{
				// Header info support
				Txt = m_GridMapping.GetValueRowCol(nRow,nCol); 
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
				Txt = m_GridMapping.GetValueRowCol(nRow,NoCol); 
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
		JUnt32 IdSupport = atoi(m_GridMapping.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Attention si support non visible
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Vérifie élment valide via le mode sélectionnée (mode Supports et/ou mode Familles)
		JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		// Affiche uniquement les supports visibles
		if (EtatVisible == true && VisibleSupport == true && IdSupport != 0)
		{
			// Attention si c'est une famille
			JBool ElmtFamille = false;
			CResultMap		ResultMapFam;
			CKeyResultMap	KeyResultMap;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// Reconstitue la clef infos famille
				KeyResultMap.m_IdCibleAff		= pDocChoixSupport->m_Offre.m_IdItemAffinite;
				KeyResultMap.m_IdMoteur			= pDocChoixSupport->m_MoteurPresse.GetType();
				KeyResultMap.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch);
				KeyResultMap.m_IdSupport		= IdSupport; 

				// Recup 1er terrain sélectionné
				JUnt32 IdTerrain;
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
				{
					// au moins 1 terrain sélectionné
					KeyResultMap.m_IdTerrain  = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
				}
				else
					continue;

				// Recup 1ere cible sélectionnée
				JUnt32 IdCible;
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
				{
					// au moins 1 cible sélectionnée
					KeyResultMap.m_IdCible		 = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
				}
				else
					continue;

				// C'est un élmt famille
				ElmtFamille = true;
			}

			// Ecriture infos générales supports
			for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
			{	
				Txt = "";
				if (nCol != m_NoColFormat-1)
				{	
					// Libellé support, ou périodicité )
					Txt += m_GridMapping.GetValueRowCol(NoRow,nCol); 

					// Info famille en +
					if ((nCol == m_NoColSelSupport-1) && ElmtFamille)
						Txt += "F";
				}
				else
				{
					// Récupère le format support
					JUnt32 IdFormatSupport = 0;
					pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
					if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
						IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();

					// libellé format support
					Txt = pDocChoixSupport->GetFormat(IdFormatSupport,true);
				}

				Entete += Txt +  "\t";
			}

			// Ecriture résultats colonnes
			for (int nCol = 0; nCol < Cols.GetCount(); nCol++)
			{
				// No de la colonne résultat
				int NoCol = Cols.GetAt(nCol);

				// Attention ici d'indiquer les résultats partiels (pour certaines familles)
				Txt = "";
				if (NoCol >= m_NoColTarif && ElmtFamille)
				{
					// On se positionne sur résultats famille	
					pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap);
					if (pDocChoixSupport->m_MapResultatMap.IsValid()) 
					{
						// Récupére résultat pour savoir état résultat : partiel ou complet
						ResultMapFam = pDocChoixSupport->m_MapResultatMap.GetItem();
					
						// Ajoute indicateur résultat partiel
						if(ResultMapFam.m_ResultPartiel)
						{	
							Txt = "*";
						}
					}
				}

				Txt += m_GridMapping.GetValueRowCol(NoRow,NoCol); 
				Entete += Txt +  "\t";
			}

			// Saut après chaque ligne support
			Entete += "\n";
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère n° colonne abscisse mapping
JUnt32 CDlgMapping::RecupNoColAbscisseMap()
{
	// Résultat abscisse juste après colonne tarif
	return m_NoColTarif;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère n° colonne ordonnée mapping	
JUnt32 CDlgMapping::RecupNoColOrdonneeMap()
{
	// Résultat ordonnée juste après colonne abscisse
	return m_NoColTarif+1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération du bloc infos terrains / cibles
void CDlgMapping ::RecupBlocTerrainCible(CString &Entete)
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
// Clipboard du grille mapping
bool CDlgMapping::ClipboardResultat()
{
	// Sélection globale du grid
	POSITION area = m_GridMapping.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridMapping.SetSelection(area, 0, 0, m_GridMapping.GetRowCount(), m_GridMapping.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridMapping.Copy(); 

	// Déselection globale
	m_GridMapping.SetSelection(NULL, 0, 0, 0, 0);
	
	return bRet;
}

// Print Resultat Mapping
bool CDlgMapping::PrintExcel()
{
	if(ClipboardResultat())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}

// Open Resultat Mapping
bool CDlgMapping::OpenExcel()
{
	if(ClipboardResultat())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open( path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}



bool CDlgMapping ::SaveAsExcel(CString filePath)
{
	if(ClipboardResultat())
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.SaveAsFile(filePath, path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du graphique mapping
void CDlgMapping::ClipboardGraphique()
{
	// Sélection globale du grid
	POSITION area = m_GridMapping.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridMapping.SetSelection(area, 0, 0, m_GridMapping.GetRowCount(), m_GridMapping.GetColCount());

	// Copie dans clipboard
	m_GridMapping.Copy(); 

	// Déselection globale
	m_GridMapping.SetSelection(NULL, 0, 0, 0, 0);

	// Test si au moins 1 support sélectionné pour le mapping
	if(!m_Mapping.ExportItems(480))
		AfxMessageBox("Erreur lors de l'export");	

}

///////////////////////////////////////////////////////////////////////////////////////
// Récupération du bitmap mapping
CBitmap * CDlgMapping::GetBitmapMapping()
{
	// Image du mapping
	CBitmap *pBmp = CopyWindowToBitmap(&m_Mapping,true);


	return pBmp;
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du bitmap mapping
void CDlgMapping::ClipboardBitmap()
{
	// HWND hwndNextViewer = SetClipboardViewer(m_Mapping.m_hWnd); 

	// ------------- Copie Bmp .... Marche 
	// Image du mapping
	CBitmap *pBmp = CopyWindowToBitmap(&m_Mapping,true);

	// Copy dans clipboard
	CopyBitmapToClipBoard(pBmp);

	pBmp->DeleteObject();
	delete pBmp;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


CBitmap * CDlgMapping::CopyScreenToBitmap(LPRECT lpRect)
{
    CDC     SrcDC, MemDC;
    int     nX, nY, nX2, nY2;      
    int     nWidth, nHeight;       
    int     xScrn, yScrn;          
    
    CBitmap *   pOldBitmap ,*pBitmap;
    
    if (IsRectEmpty(lpRect)) return NULL;      
    
    SrcDC.CreateDC("DISPLAY", NULL, NULL, NULL);        
    MemDC.CreateCompatibleDC(&SrcDC);
    
    nX = lpRect->left;     
    nY = lpRect->top;     
    nX2 = lpRect->right;     
    nY2 = lpRect->bottom;
    
    xScrn = SrcDC.GetDeviceCaps(HORZRES);     
    yScrn = SrcDC.GetDeviceCaps(VERTRES);      
    
    if (nX < 0) nX = 0;        
    if (nY < 0) nY = 0;     
    if (nX2 > xScrn) nX2 = xScrn;      
    if (nY2 > yScrn) nY2 = yScrn;      
    nWidth = nX2 - nX;     
    nHeight = nY2 - nY;      
    
    pBitmap=new CBitmap;
    pBitmap->CreateCompatibleBitmap(&SrcDC,nWidth, nHeight);      
    
    pOldBitmap =MemDC.SelectObject (pBitmap);          
    MemDC.BitBlt(0, 0, nWidth, nHeight, &SrcDC, nX, nY, SRCCOPY);     
    
    pBitmap = MemDC.SelectObject( pOldBitmap);       
    MemDC.DeleteDC();     
    SrcDC.DeleteDC();      
    
    return (pBitmap); 
}

// CBitmap * CDlgMapping::CopyWindowToBitmap(CWnd* pWnd ,bool bWindowClient)
// CBitmap * CDlgMapping::CopyWindowToBitmap(CGXGridWnd* pWnd ,bool bWindowClient)
CBitmap * CDlgMapping::CopyWindowToBitmap(JFControl* pWnd ,bool bWindowClient)
{
    CBitmap *pBitmap=NULL;
    if(!pWnd->m_hWnd) return NULL;
    
    RECT    rectWnd; 
    pWnd->GetWindowRect(&rectWnd);
    if(!bWindowClient) return CopyScreenToBitmap(&rectWnd);
    
    RECT rectClient;             
    POINT pt1, pt2;             
    pWnd->GetClientRect(&rectClient);
    pt1.x = rectClient.left;             
    pt1.y = rectClient.top;             
    pt2.x = rectClient.right;             
    pt2.y = rectClient.bottom;             
    
    pWnd->ClientToScreen(&pt1);             
    pWnd->ClientToScreen(&pt2);             
    rectClient.left = pt1.x;             
    rectClient.top = pt1.y;             
    rectClient.right = pt2.x;             
    rectClient.bottom = pt2.y;  
    
    return CopyScreenToBitmap(&rectClient);   
}
void CDlgMapping ::PrintBmp(CBitmap *pBmp)
{
    // mise a jour le 15/01/2005 :ajuste le bitmap sur la totalité de la feuille et edite en mode paysage. 
    CDC             dc;
    CPrintDialog    printDlg(FALSE);
    
    // selection de l'imprimante.
    if (printDlg.DoModal() == IDCANCEL)   return;
    
    DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(printDlg.GetDevMode());
    
    // set orientation to landscape
    pDevMode->dmOrientation=DMORIENT_LANDSCAPE;
    ::GlobalUnlock(printDlg.GetDevMode());
    
    HDC hdc =printDlg.CreatePrinterDC();
    
    //dc.Attach(printDlg.GetPrinterDC()); sinon le mode paysage ne fonctionne pas
    
    dc.Attach(hdc);
    dc.m_bPrinting = TRUE; // dc d'impression.
    
    // titre du document = titre application
    CString strTitle;   
    strTitle.LoadString(AFX_IDS_APP_TITLE);
    
    DOCINFO di;
    ::ZeroMemory (&di, sizeof (DOCINFO));
    
    di.cbSize = sizeof (DOCINFO);   
    di.lpszDocName = strTitle;
    
    // debut d'impression
    if(dc.StartDoc( &di ))
    {     
        // debut page
        dc.StartPage();
        
        // surface d'impression
        CRect rectDraw;
        rectDraw.SetRect(0, 0,
            dc.GetDeviceCaps(HORZRES),
            dc.GetDeviceCaps(VERTRES));
        
        // infos bitmap
        BITMAP bmpInfo;
        pBmp->GetBitmap(&bmpInfo);
        
        // creation d'un objet CPictureHolder pour avoir un dib.
        CPictureHolder picture;
        picture.CreateFromBitmap(pBmp);
        
        // rectangle d'impression      
        int ncoefy=(rectDraw.Height()/bmpInfo.bmHeight);
        
        // centrer l'image sur la feuille
        int nX = rectDraw.left + (rectDraw.Width() - (bmpInfo.bmWidth*ncoefy)) / 2;
        int nY = rectDraw.top + (rectDraw.Height() - (bmpInfo.bmHeight*ncoefy)) / 2;
        
        CRect rect;      
        rect.SetRect(CPoint(nX,nY),CPoint(nX+(bmpInfo.bmWidth*ncoefy),nY+(bmpInfo.bmHeight*ncoefy)));
        
        // affichage final
        picture.Render(&dc,rect,rect);
        
        dc.EndPage(); // fin de page
        dc.EndDoc();  // fin du document
    }   
    else dc.AbortDoc(); // erreur d'impression
    dc.Detach();        // liberation dc d'impression.
    DeleteObject(hdc); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Copie image dans le clipboard
bool CDlgMapping::CopyBitmapToClipBoard(CBitmap *pBitmap) 
{ 
    bool bret=(SetClipboardData(CF_BITMAP, pBitmap->m_hObject)!=NULL); 
    return bret; 
} 


/*
// contexte d'utilisation : dans une boite de dialogue ou view etc..
CBitmap *pBmp=CopyWindowToBitmap(this,true);
PrintBmp(pBmp);    
pBmp->DeleteObject();
delete pBmp;
*/
