// DlgMapping.cpp : fichier d'impl�mentation
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

// Bo�te de dialogue CDlgMapping
IMPLEMENT_DYNAMIC(CDlgMapping, JFCDialog)
CDlgMapping::CDlgMapping(CWnd* pParent,  /*=NULL*/
						 JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgMapping::IDD, pParent), m_SplitHorz(WM_SPLIT_HORZ)
{
	// R�cup�re pointeur s�lection g�n�rale
	pDocChoixSupport = pDoc;

	// Passe le document pour la cr�ation mapping
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

	// Initialisation des diff�rents contr�les 
	SetupControls();

	// 1 / Titre Source - Filtre d'audience
	UpdateTitreAnalyse();

	// Mise en place des realtions objets fen�tre Mapping
	SetWindowRelations();

	// on initialise les modes de d�placement 
	this->SetMode(&m_Mapping, MGER_SIZE, MGER_SIZE);

	// on r�cup�re la position des fen�tres
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
// Gestion du d�placement split horizontal mapping
LRESULT CDlgMapping::OnSplitHorz(WPARAM WParam, LPARAM LPAram)
{
	// on r�cup�re le d�placement
	LONG Disp = (LONG)WParam;

	// on r�cup�re la position des fen�tres
	RECT RectL1; m_CadreMapping.GetWindowRect(&RectL1); this->ScreenToClient(&RectL1);
	RECT RectL2; m_CadreGridMapping.GetWindowRect(&RectL2); this->ScreenToClient(&RectL2);
	RECT RectL3; m_Mapping.GetWindowRect(&RectL3); this->ScreenToClient(&RectL3);
	RECT RectL4; m_GridMapping.GetWindowRect(&RectL4); this->ScreenToClient(&RectL4);
	RECT RectSP; m_SplitHorz.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);

	// Limite aux d�placements pour �viter des ab�rations graphiques
	// Hauteur minimale graph mapping
	if (RectL1.bottom + Disp < 100) 
		Disp = 100 - RectL1.bottom;

	// Hauteur maximale graph mapping
	if (RectL1.bottom + Disp > (RectL2.bottom - 100))  
		Disp = (RectL2.bottom - 100)  - RectL1.bottom;

	// on corrige la position des fen�tres
	RectL1.bottom += Disp;
	RectL3.bottom += Disp; 
	RectSP.bottom += Disp; RectSP.top += Disp;
	RectL2.top    += Disp;
	RectL4.top    += Disp;

	// on d�place les fen�tres
	m_CadreMapping.SetWindowPos(NULL, RectL1.left, RectL1.top, RectL1.right - RectL1.left, RectL1.bottom - RectL1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Mapping.SetWindowPos(NULL, RectL3.left, RectL3.top, RectL3.right - RectL3.left, RectL3.bottom - RectL3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER); 
	m_CadreGridMapping.SetWindowPos(NULL, RectL2.left, RectL2.top, RectL2.right - RectL2.left, RectL2.bottom - RectL2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_GridMapping.SetWindowPos(NULL, RectL4.left, RectL4.top, RectL4.right - RectL4.left, RectL4.bottom - RectL4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

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
// Mise en place des relations entre objets de la fen�tre Mapping
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

			// Cadre englobant le grid r�sultat mapping
			{IDC_CS_CADRE_GRIDMAP, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},						//Left side
								   {CResizer::eFixed, IDC_CS_CADRE_MAPPING, CResizer::eBottom},			//Top side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},						//Right side
								   {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},					//Bottom side

			// Grid r�sultat mapping
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
// Set up de tous les contr�les fen�tre Mapping
void CDlgMapping::SetupControls()
{
	// Propri�t�s du cadre Mapping
	m_CadreGridMapping.SetTransparent(false);  // A REMETTRE true ?????
	m_CadreGridMapping.SetSunken(true);

	// Propri�t�s du cadre Grille Mapping
	m_CadreMapping.SetTransparent(false);
	m_CadreMapping.SetSunken(true);
 
	// Les �l�ments du cadre Mapping
	m_CadreMapping.AddTopWindow(m_TitreMapping);
	m_CadreMapping.AddTopWindow(m_Mapping);

	// Les �l�ments du cadre r�sultat mapping
	m_CadreGridMapping.AddTopWindow(m_GridMapping); 
	m_CadreGridMapping.AddTopWindow(m_ProgressResultMap);
	
	// Progress bar non visible au d�part
	m_ProgressResultMap.ShowWindow(SW_HIDE); 
	m_ProgressResultMap.SetShowText(false);

	// Taille et Font des diff�rents labels
	m_TitreMapping.SetFontName(CATPColors::GetFontLabelName());
	m_TitreMapping.SetFontBold(TRUE); 
	m_TitreMapping.SetFontSize(CATPColors::GetFontLabelSize());

	// A FAIRE
	// Initialise objet Mapping
	// m_Mapping.
	
	// Initialise grid r�sultat mapping
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

	// Enregistrement controle sp�cifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(&m_GridMapping);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridMapping.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);

	// Init R�f�rence colonne
	InitRefGrid();

	// Associe le document au grille
	if (m_GridMapping.OnInitDocument(pDocChoixSupport, this, m_NoColIdSupport) == false)
		AfxMessageBox ("Probl�me association document supports / grille r�sultat mapping");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid r�sultat
	if (GrilleResultValid())
		InitGridResultat();

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des r�f�rences colonnes
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

	// Position colonne p�riodicit�
	m_NoColPeriodicite	= m_NoColFormat + 1;

	// Position colonne tarif
	m_NoColTarif		= m_NoColPeriodicite + 1;
}

// D�finition des couleurs interface fen�tre Mapping
void CDlgMapping::SetColors()
{
	// Coloriage du cadre englobant
	m_CadreMapping.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_CadreGridMapping.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultMap.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Titre Cumulativit�
	m_TitreMapping.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreMapping.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Grille r�sultat
	m_GridMapping.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridMapping.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Progress bar
	m_ProgressResultMap.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)); 

	// Style des lignes (solid pen)
	m_GridMapping.GetParam()->SetGridLineStyle(PS_SOLID); 

}

// Destruction de la fen�tre Mapping
void CDlgMapping::OnDestroy()
{
	JFCDialog::OnDestroy();
}

// Retaille de la fen�tre Mapping
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
		// Mise � jour du Mapping
		// AfxMessageBox("A FAIRE / MAPPING",MB_ICONINFORMATION);	
	}
}

void CDlgMapping::OnCancel()
{
	JFCDialog::OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid r�sultat mapping
void CDlgMapping::InitGrid()
{
	m_GridMapping.LockUpdate(true);

	// Enleve n� ligne col 
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

	// Position colonne p�riodicit�
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

		// Aucun header colonne et colonnes bloqu�s (support/p�riodicit�/tarif)
		m_GridMapping.SetFrozenCols(m_NoColTarif - m_NoColIdSupport ,0);

		// Affichage entete Tarif et p�riodicit�
		if (m_GridMapping.GetRowCount() > 0) 
		{

			// Titre "S�l SUPPORTS"
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
											.SetValue(_T("Tarif �")));

			// Titre "PERIODICITE"
			m_GridMapping.SetStyleRange(CGXRange(2,m_NoColPeriodicite-1),
										CGXStyle()
											.SetValue(_T("P�riodic.")));

			// Case s�lection / d�selection tot supports
			m_GridMapping.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("T")));
		}

		// Merge des cellules d�finitivement vide
		m_GridMapping.UpdateData(); 

		m_GridMapping.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,m_NbHeaderRow-1,m_GridMapping.GetColCount()),
									CGXStyle()
									.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

		m_GridMapping.UpdateWindow(); 

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Rend visible le grid, si au moins quelques r�sultats
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
		// Uniquement les 2 colonnes correspondant aux infos abscisse et ordonn�e mapping
		m_NbCol = 2;
	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Init grid avec nb colonnes
    if (!m_GridMapping.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Lignes
bool CDlgMapping::UpdateRow()
{
	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Lignes  (ligne info support + �tat nb insertions)
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

				// Texte associ� au header la cellule
				CString Txt;

				if (Col >= m_NbHeaderCol)
				{
					// Texte associ� au header ligne
					Txt = RecupTexteHeaderRow(Lig,Col);

					// Affichage contenu cellule
					m_GridMapping.SetValueRange(CGXRange(Lig,Col),Txt); 
				}
			}
		}

		// Couleurs sp�ciales
		UpdateColorSpe();

		// Update Grille modifi�e
		m_GridMapping.UpdateData(TRUE);

		// Regroupement des lignes header row
		m_GridMapping.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
		m_GridMapping.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,m_GridMapping.GetColCount()),
									CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

		// Mise � jour des datas r�sultat grille �chelle
		m_GridMapping.UpdateWindow(); 
		m_GridMapping.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	m_GridMapping.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs sp�ciales de ceratines cellules
void CDlgMapping::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne r�sultat
	// Cellules info insertion en fond blanc et couleur caract�re diff�rent
	if (m_GridMapping.GetColCount() >= m_NoColTarif)
	{
		m_GridMapping.LockUpdate(true); 

		m_GridMapping.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 1, m_NoColTarif-1),
									CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Delock mode read only
		m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

		// Niveau header de couleurs diff�rents (via dlg ordre entete)
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
			// Si data exog�ne -- couleur diff�rente
			// R�cup�re identificateur s�lection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col);

			if (IdGrandeur >= 100)
			{
				// Data Exog�ne en + fonc�
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
// R�cup�ration texte header row
CString CDlgMapping::RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col)
{
	// Init chaine libell�
	CString Txt = "-";

	// Afficher titre des colonnes abscisses et ordonn�es du mapping
	if (Col == m_NoColTarif)
	{
		// Colonne consacr�e � l'affichage de la grandeur en abscisse
		Txt.LoadString(IDS_CS_ABSCISSE_ND);
		// Txt = "Abscisse n.d.";
		if (pDocChoixSupport->m_IdGrandMapSelAbs >= 0)
			Txt = pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleMap, pDocChoixSupport->m_IdGrandMapSelAbs);
	}
	else if (Col == m_NoColTarif + 1)
	{
		// Colonne consacr�e � l'affichage de la grandeur en ordonn�e
		Txt.LoadString(IDS_CS_ORDONNEE_ND);
		// Txt = "Ordonn�e n.d.";
		if (pDocChoixSupport->m_IdGrandMapSelOrd >= 0)
			Txt = pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleMap, pDocChoixSupport->m_IdGrandMapSelOrd);
	}	
	// Renvoi libell� trouv�
	return Txt;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Id Grandeur
JUnt32 CDlgMapping::RecupIdGrandeur(JUnt32 NoCol)
{
	// Abscisse ou ordonn�e du mapping
	if (NoCol == m_NoColTarif)
		// Colonne consacr�e � l'affichage de la grandeur en abscisse
		return pDocChoixSupport->m_IdGrandMapSelAbs;

	else if (NoCol == m_NoColTarif + 1)
	{
		// Colonne consacr�e � l'affichage de la grandeur en ordonn�e
		return pDocChoixSupport->m_IdGrandMapSelOrd;
	}	
	else
		// Colonne grandeur non d�finie
		return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des r�sultats selon grandeur
//  0  > "LNM/LDP(000)" 					-------------------------------- LNM/LDP (000) (Audience) en milliers
//  1  > "Audience"							-------------------------------- audience * effectif / 100
//  2  > "Audience(000)						-------------------------------- idem en milliers
//  3  > "Cts.[+](000)"						-------------------------------- VolLecture (Contacts en milliers)
//  4  > "GRP"   							-------------------------------- (Audience LNM/LDP * 100.0) / Effectif
//  5  > "GRP[+]							-------------------------------- (VolLecture * 100.0) / Effectif
//  6  > "C% cumul�e"						--------------------------------  audience n insertions
//  7  > "R�p�tition ou reprises en main"	-------------------------------- VolLecture / (Effectif * Audience)
//  8  > "Indice Affinit�"					-------------------------------- (GRP / GRPAff) * 100.0 
//  9  > "% Affinit�"						-------------------------------- (AudienceLDP / AudAffLDP) * 100.0 
// 10  > "Cumulativit�"						-------------------------------- (Audience n insertions / Audience � 1 insertion)
// 11  > "Co�t/GRP"							-------------------------------- tarif support / GRP
// 12  > "Co�t/1000 effectif"				-------------------------------- tarif support / audience
// 13  > "Co�t/%Couverture					-------------------------------- tarif support / % Couverture
// 14  > "Co�t/1000 contacts"				-------------------------------- tarif support / Nb Contacts

//  ?? > "Audience / Nb Lect"				-------------------------------- audience * effectif / 100
//
/*
CString CDlgMapping::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol,  JFlt64 Tarif,      JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
																   JFlt64 Effectif,   JFlt64 NbCas,
																   JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff)
{
	// Par d�faut Texte
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
	// Par d�faut Texte
	CGXStyle style;
	m_GridMapping.SetStyleRange(CGXRange(NoLig,NoCol),
								CGXStyle()
									.SetFormat(GX_FMT_TEXT)
									.SetValueType(GX_VT_STRING));

	// Indique les r�sultats partiels
	if (ResultMap.m_ResultPartiel)
	{	
		// On distingue r�sultat partiel cellule
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
	// Chaine format�e par d�faut
	CString TxtResult = "*";

	// Pour le moment Mapping avec 1 seule insertion
	JFlt64 NbInsertions = 1.0;

	// R�cup�re ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol);	

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
		case pDocChoixSupport->GrdMap_AudienceLDP : 
		{
			if (ResultMap.m_AudienceLDP >= 0)
			{
				// R�cup�re audience
				// TxtResult.Format("%d",(JUnt32)((AudienceLDP + 500.0)/1000.0));
				TxtResult.Format("%0.f",ResultMap.m_AudienceLDP / 1000.0);

				// D�coupage chaine par milliers
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

				// D�coupage chaine par milliers
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

				// D�coupage chaine par milliers
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

				// D�coupage chaine par milliers
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

			// Calcul GRP Affinit�
			JFlt64 GRPAff	= 0.0;
			if (ResultMap.m_EffectifAffinite > 0)
				GRPAff = (ResultMap.m_AudienceAffiniteProba / ResultMap.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				// Calcul indice affinit�
				double IndAff = (GRP / GRPAff)* 100.0;
				TxtResult.Format("%0.f",IndAff);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}
		}
		break;

		//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
		case pDocChoixSupport->GrdMap_PrctAff :
		{
			if (ResultMap.m_AudienceAffinite > 0)
			{

				// Calcul pourcentage affinit�
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

			// Si GRP d�fini, calcul du co�t
			if (GRP > 0 && Tarif > 0) // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif / GRP;

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
		}
		break;

		///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Ind : 
		{
			if ((ResultMap.m_Audience / 1000) > 0 && Tarif > 0)  // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif /(ResultMap.m_Audience / 1000.0);
				
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
		break;

		///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdMap_Cout000Cts : 
		{
			// Calcul Cout
			if (ResultMap.m_VolLecture > 0 && Tarif > 0)  // && !ResultMap.m_ResultPartiel)
			{
				JFlt64 Cout  = Tarif / (ResultMap.m_VolLecture / 1000.0);

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
		break;


		default:
		break;
	}

	// Traitement des donn�es exog�nes (OJD, .....)
	if (IdGrandeur >= 100)
	{
		// R�cup�re les informations associ�es aux donn�es exog�nes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libell� data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exog�nes ////////////////////////////
		JFlt64 Resultat = -1;
		if (DataExo.GetValeur(NomDataExo,IdSupport,Resultat))
		{
			// R�cup�re r�sultat format� selon type et nb dec data exo
			TxtResult = GetResultDataExo(IdGrandeur, Resultat);
		}
	}
	
	// Renvoi la chaine format�e
	// m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
	return TxtResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re r�sultat format� selon type et nb dec data exo
CString CDlgMapping::GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell)
{
	CString TxtResult;

	// R�cup�re les informations associ�es aux donn�es exog�nes (OJD etc...)
	const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
	const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

	// Recherche libell� data
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
			// Par d�faut en entier et sans d�cimale
			TxtResult.Format("%0.f",ResultCell);
	}
	else
		// Par d�faut en entier et sans d�cimale
		TxtResult.Format("%0.f",ResultCell);

	// D�coupage chaine par milliers
	pDocChoixSupport->FormatString(TxtResult,3);

	return TxtResult;
}

double CDlgMapping::RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  
								JFlt64 Tarif, CResultMap &ResultMap)
{
	// Init valeur r�sultat
	double Result = -1.0;

	// Pour le moment 1 seule insertion dans Mapping
	JFlt64 NbInsertions = 1.0;

	// R�cup�re ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol);	

	// Recup cible s�lectionn�e pour mapping
	JUnt32 IdCible = 0;
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
		case pDocChoixSupport->GrdMap_AudienceLDP : 
		{
			if (ResultMap.m_AudienceLDP >= 0)
				// R�cup�re audience
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

			// Calcul GRP Affinit�
			JFlt64 GRPAff	= 0.0;
			if (ResultMap.m_EffectifAffinite > 0)
				GRPAff = (ResultMap.m_AudienceAffiniteProba / ResultMap.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				Result = 100.0;
				if (pDocChoixSupport->m_Offre.m_IdItemAffinite != IdCible)
					// Calcul indice affinit�
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
					// Calcul pourcentage affinit�
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

			// Si GRP d�fini, calcul du co�t
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

	// Traitement des donn�es exog�nes (OJD, .....)
	if (IdGrandeur >= 100)
	{
		// R�cup�re les informations associ�es aux donn�es exog�nes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libell� data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exog�nes ////////////////////////////
		JFlt64 Resultat = -1;
		if (!DataExo.GetValeur(NomDataExo,IdSupport,Result))
		{
			// R�cup�re r�sultat format� selon type et nb dec data exo
			Result = -1;
		}
	}
	
	// Renvoi le r�sultat trouv�
	return Result;

}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du r�sultat mapping (selon grandeurs abscisse et ordonn�e s�lectionn�es)
JBool CDlgMapping::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridMapping.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultMap KeyResultMap;

	// Information r�sultat archiv�
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

	// Clef affinit� courante
	KeyResultMap.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResultMap.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 

	// Calcul indice cl� moteur
	KeyResultMap.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);


	// Balayage ligne support
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Par d�faut c'est un support
		JBool ElmtFamille = false;

		// Avance progress bar
		m_ProgressResultMap.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention si c'est une famille
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un �lmt famille
			ElmtFamille = true;
		}

		// R�cup�re le format support ligne s�lectionn� pour le mapping
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();
		else
			IdFormatSupport = IdFormat;

		// R�cup�re Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			JUnt32 Duree;
			JDate DateExact;

			// R�cup�ration du tarif
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

			// Tarif bien d�fini
			// m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 

			if (!TarifManuel)
			{
				// Tarif bien d�fini
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien d�fini (modif la couleur texte)
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else
			// Tarif non d�fini
			m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);
		
		// Si c'est une famille, il faut que tous les supports de m�me p�riodcit�, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de p�riodicit� abr�g� (pour pr�sentation tableau � droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 
	
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
		KeyResultMap.m_IdCible			= IdCible; 
		KeyResultMap.m_IdTerrain		= IdTerrain;
		KeyResultMap.m_IdSupport		= IdSupport; 

		// Test si la cible est autoris�e
		if (pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain) == true)
		{
			// 1 seul insertion pour le moment
			// JUnt32 NbInsertions = pDocChoixSupport->m_NbInsertions;
			JUnt32 NbInsertions = 1;

			
			// Attention si d�j� calcul� dans �chelle, pas la peine de recommencer
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

				// Test si �l�ment d�j� calcul�
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

							// Positionnement moteur pour calcul �lmts affinit�
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

							// Audience affinit�
							JFlt64 AudienceAffinite = -1;

							// Init Moteur (construction distribution)
							// Voir Jeremy avant on n'�tatit pas oblig� de tester VolAffinite
							if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille) && VolAffinite >= 0)
							{
								// Cr�ation Distribution via cible affinit�
								JFCDistribution DistriAff = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolAffinite); 

								// R�cup�re audience
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
										// Cr�ation Distribution
										JFCDistribution Distri = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLecture); 

										// R�cup�re audience
										Audience = Distri.Item(1).AsJFlt64();  
									}

									// Force Audience LDP si 1 seule insertion
									if (pDocChoixSupport->m_NbInsertions == 1)
									{
										CRegroupements * pReg = CRegroupements::GetInstance();
										// R�cup id support
										IElem *pElem = pReg->GetIElemByID(IdSupport);

										// Test si c'est un couplage
										if(!pElem->IsCouplage())
										{
											// Audience devient Audience LDP
											Audience = AudienceLDP;

											// Audience affinit� devient Audience LDP Affinit�
											AudienceAffinite = AudienceAffiniteLDP;
										}
									}

									// Pour cumulativit� (construction Maille et Calcul avec 1 insertions)
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

											// Cr�ation Distribution
											JFCDistribution Distri1Ins = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolLect1Ins); 

											// R�cup�re audience
											Audience1Ins = Distri1Ins.Item(1).AsJFlt64();  
										}
									}
									else
										// m�me audience, cas d'une seule insertion par support
										Audience1Ins = Audience;
								}
							}

							// Calcul R�sultat Colonne (selon grandeur)
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

							// Ecriture du r�sultat
							m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);

							// Stockage r�sultat dans matrice
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
					// Elment d�j� calcul�, on prend directement les infos dans map r�sultats
					// Recherche les r�sultats dans le map
					ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem(); 

					// Balayage des colonnes abscisse et ordonn�e
					for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
					{
						// Calcul R�sultat Colonne (selon grandeur)
						CString Txt = ResultCellule(IdSupport, NoLig, NoCol,
													Tarif, ResultMap);

						// Ecriture du r�sultat
						m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);
					}
				}
			}
			else
			{
				// OPTIM 2.7 on r�cup�re les r�sultats d�j� calcul�es dans Echelle
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

				// Stockage r�sultat dans matrice
				pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
				if (!pDocChoixSupport->m_MapResultatMap.IsValid())
				{
					pDocChoixSupport->m_MapResultatMap.Add(KeyResultMap) = ResultMap; 
				}

				// Balayage des colonnes abscisse et ordonn�e
				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
				{
					// Calcul R�sultat Colonne (selon grandeur)
					CString Txt = ResultCellule(IdSupport, NoLig, NoCol,
						Tarif, ResultMap);

					// Ecriture du r�sultat
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

	// Affiche r�sultat Familles avec moyenne des supports si cumul faminne non demand�
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

		// Retaille largeur colonnes supports/formats/p�riodicit�s
		m_GridMapping.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
													m_GridMapping.GetRowCount(),m_NoColPeriodicite-1),
										TRUE,GX_UPDATENOW);
		
		// Retaille hauteur header lignes (pour titres supports)
		m_GridMapping.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
													0,m_GridMapping.GetColCount()),
											TRUE,GX_UPDATENOW);
	}

	// Mise � jour des s�lections supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour du grid
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	// Flag calcul effectu�
	pDocChoixSupport->m_ParamMapModif = false;
	// pDocChoixSupport->m_ParamMapModif = true;   // OPTIM 2.7

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage R�sultat Familles
JVoid CDlgMapping::AfficheResultFamille()
{
	/* ici on calcul m^me si non visible, car point mapping n�cessaire
	// Mode Famille non actif, pas de calcul
	if (pDocChoixSupport->m_AFFModeFamilles == FALSE)
		return;
	*/
	
	// R�cup niveau de chacune des grandeurs
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

	// Recup 1er terrain s�lectionn�
	JUnt32 IdTerrain;
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		IdTerrain = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return;

	// Recup 1ere cible s�lectionn�e
	JUnt32 IdCible;
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return;

	// Balaye toutes les lignes, sauf les supports et la moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention � la ligne moyenne
		if (IdSupport == 0)
			continue;

		// Voir si c'est un �lmt famille -- R�cup�rer la liste des composants
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if(pElem->IsLot())
		{
			// Cr�e liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// R�cup tarif famille
				CString	TxtTarif = m_GridMapping.GetValueRowCol(NoLig,m_NoColTarif-1); 

				// Vire les espaces
				TxtTarif.Replace(" ",""); 

				// Valeur Tarif
				JUnt32 TarifFamille = atof(TxtTarif);
				// JUnt32 TarifFamille = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColTarif-1));

				// R�cup�re la liste des titres
				pLot->GetFils(ListSup);

				// Nombre de composants de la famille
				JUnt32 NbCompoFamille = ListSup.GetCount(); 

				// Balayage des colonnes
				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridMapping.GetColCount(); NoCol++) 
				{
					// Information r�sultat famille
					CKeyResultMap KeyResultMapFam;
					CResultMap	ResultMapFam;

					// Cl� r�sultat famille
					KeyResultMapFam.m_IdCibleAff	=  pDocChoixSupport->m_Offre.m_IdItemAffinite;
					KeyResultMapFam.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 
					KeyResultMapFam.m_IdMoteur		= pDocChoixSupport->m_MoteurPresse.GetType(); 
					KeyResultMapFam.m_IdCible		= IdCible; 
					KeyResultMapFam.m_IdTerrain		= IdTerrain;
					KeyResultMapFam.m_IdSupport		= IdSupport; 

					// Test si �l�ment d�j� calcul�
					pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMapFam); 
					if (!pDocChoixSupport->m_MapResultatMap.IsValid())
					{	
						// Nb Composants comptabilis�s pour r�sultat famille
						JUnt32 NbCompoValid = 0;

						// Balaye tous les composants familles
						for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())   
						{
							// R�cup composant famille
							JUnt32 IdCompo = ListSup.GetItem(); 

							// Cl� archivage r�sultat
							CKeyResultMap KeyResultMap;

							// R�sultat composant
							CResultMap	ResultMap;

							// Cl� de recherche r�sultat (idem clef famille, seul id support change)
							KeyResultMap				= KeyResultMapFam;
							KeyResultMap.m_IdSupport	= IdCompo; 

							// Test si �l�ment d�j� calcul� ou �lmt moyenn�
							pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 

							if (pDocChoixSupport->m_MapResultatMap.IsValid())
							{
								// Elment d�j� calcul�, on prend directement les infos dans map r�sultats
								ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem(); 

								// Attention aux r�sultats non d�finis
								if (ResultMap.m_Audience > 0)
								{
									// Cumul r�sultat colonne pour famille
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

						// Positionne Flag r�sultat partiel
						if (NbCompoValid != NbCompoFamille || NbCompoFamille == 0)
						{
							// Ce sont des r�sultats partiels, manque certains supports
							ResultMapFam.m_ResultPartiel = true;
						}

						//Voir si tous les composants famille ont �t� trouv�
						// if (NbCompoValid == NbCompoFamille && NbCompoFamille != 0)
						if (NbCompoValid != 0)
						{
							// Ecriture r�sultat famille
							CString Txt = ResultCellule(IdSupport,NoLig, NoCol, TarifFamille, ResultMapFam);

							// Ajoute dans matrice r�sultat si nouvelle clef r�sultat famille
							pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMapFam); 
							if (!pDocChoixSupport->m_MapResultatMap.IsValid())
							{
								pDocChoixSupport->m_MapResultatMap.Add(KeyResultMapFam) = ResultMapFam; 
							}

							/*
							// Distinguer les r�sultats famille non complets (calcul avec seulement
							// les composants valides)
							if (ResultMapFam.m_ResultPartiel)
							{
								// On distingue la ligne compl�te pour cette �tude "semi-calcul�e"
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
							// R�sultat famille non d�finie car manque prise en compte certains composants
							// non d�fini
							m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),"*");
						}
					}
					else
					{
						// Element famille d�j� calcul�, on prend directement les infos dans map r�sultats
						ResultMapFam = pDocChoixSupport->m_MapResultatMap.GetItem(); 

						// Calcul R�sultat Colonne (selon grandeur)
						CString Txt = ResultCellule(IdSupport,NoLig, NoCol, TarifFamille, ResultMapFam);

						/*
						// Attention aux r�sultats partiels, manque certains supports
						if (ResultMapFam.m_ResultPartiel)
						{
							// On distingue la ligne compl�te pour cette �tude "semi-calcul�e"
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
// Affichage r�sultat mapping (selon grandeurs abscisse et ordonn�e s�lectionn�es)
JBool CDlgMapping::AfficheResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridMapping.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultMap KeyResultMap;

	// Information r�sultat archiv�
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

	// Clef affinit� courante
	KeyResultMap.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResultMap.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleMap); 

	// Calcul indice cl� moteur
	KeyResultMap.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// R�cup�re map des supports valides sur les segments en cours
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
			// C'est un �lmt famille
			ElmtFamille = true;
		}

		// R�cup�re le format support ligne s�lectionn� pour le mapping
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();
		else
			IdFormatSupport = IdFormat;

		// R�cup�re Tarif item selon format
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

			// Tarif bien d�fini
			// m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			if (!TarifManuel)
			{
				// Tarif bien d�fini
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien d�fini (modif la couleur texte)
				m_GridMapping.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
		}
		else
			// Tarif non d�fini
			m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);
		
		// Si c'est une famille, il faut que tous les supports de m�me p�riodcit�, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de p�riodicit� abr�g� (pour pr�sentation tableau � droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridMapping.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

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

		// D�finit clef r�sultat
		KeyResultMap.m_IdSupport = IdSupport;
		KeyResultMap.m_IdTerrain = IdTerrain;
		KeyResultMap.m_IdCible	 = IdCible;

		/*
		// Init les indications r�sultats complets
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
			// Recherche R�sultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur r�sultats
			pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap); 
			if (pDocChoixSupport->m_MapResultatMap.IsValid())
			{
				// Information r�sultat archiv�
				CResultMap	&ResultMap = pDocChoixSupport->m_MapResultatMap.GetItem();

				// Le r�sultat pour la cellule
				Txt = ResultCellule(IdSupport, NoLig, NoCol,
									Tarif, ResultMap); 
				// Ecriture du r�sultat
				m_GridMapping.SetValueRange(CGXRange(NoLig,NoCol),Txt);

				/*
				// Attention aux r�sultats partiels, manque certains supports
				if (ResultMap.m_ResultPartiel)
				{
					// On distingue la ligne compl�te pour cette �tude "semi-calcul�e"
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
				// Pas de r�sultat d�fini
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

	// Retaille largeur colonnes supports/p�riodicit�
	m_GridMapping.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                    m_GridMapping.GetRowCount(),m_NoColPeriodicite-1),
                                       TRUE,GX_UPDATENOW);
	
	// Retaille hauteur header lignes (pour titres supports)
	m_GridMapping.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
			                                     0,m_GridMapping.GetColCount()),
		                                TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour valide
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	return true; 
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections supports
bool CDlgMapping::UpdateSelSupports()
{
	// Nombre de supports nouvelle s�lection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRow();

		// Initialise libell�s supports en 1ere colonne
		InitRowSupports();
		
		// Initialise largeur colonne
		InitSizeColonne();

		// Mise � jour du grid
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur pr�c�demment existant, 
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

	// R�cup�re objet regroupement
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
		// Style check-box libell�s supports
		m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColSelSupport - 1,
											 InxLigSupp + NbSupports - 1,
											 m_NoColSelSupport - 1),
									CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

		// Style texte libell�s supports
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
	
		// Style texte libell�s p�riodicit�
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

		// Style texte libell�s tarif
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
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdSupport));

				// on r�cup�re le libell� support
				JString LibSupport = "-";
				if (pSupport)
				{
					// Affichage libell� support
					LibSupport = pSupport->GetLabel().AsJCharPtr();
				}

				// Affichage libell� support
				CString LibSupp = LibSupport.AsJCharPtr();
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibSupp));

				// R�cup�re info formats support
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;
  				pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleMap);
						
				// Affichage libell� format du support
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

				// Puis indication support s�lectionn� ou pas
				if (EtatSupport)
				{
					// Valide s�lection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// D�valide la s�lection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incr�mente index ligne
				InxLigSupp++;
			}
		}

		// Balaye toutes les familles associ�es aux supports
		for (pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveFirst();
			 pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid();
			 pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveNext())
		{
			// Ident famille
			JUnt32 IdFamille = pDocChoixSupport->m_Offre.m_MapSelFamilles.GetKey();
			pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdFamille);

			// V�rifie qu'il est valide dans le map s�lection
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

				// on r�cup�re le libell� famille
				JString LibFamille = "-";

				// Affichage libell� famille
				LibFamille = pFamille->GetLabel().AsJCharPtr();
				m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibFamille.AsJCharPtr()));
				
				// Format famille non d�fini (avec espace � droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;

				// R�cup�re liste des formats pour la famille
				pDocChoixSupport->GetFormatsFamille(IdFamille, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libell� format du support
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
				// Affichage libell� format de la famille > indique non d�fini
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

				// Puis indication famille s�lectionn� ou pas
				if (EtatSupport)
				{
					// Valide s�lection support
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
											CGXStyle()
												.SetValue((long)1));
				}
				else
				{
					// D�valide la s�lection famille
					m_GridMapping.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incr�mente index ligne
				InxLigSupp++;
			}
		}

		// Mise � jour couleur s�lection supports
		UpdateVisuSupports();
	}
	
	// Mise � jour des modifs
	m_GridMapping.LockUpdate(false);
	m_GridMapping.UpdateWindow();
	m_GridMapping.Redraw();	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
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

	// Retaille la colonne p�riodicit� et tarif
	m_GridMapping.SetColWidth(m_NoColPeriodicite-1,m_NoColTarif-1,60);

	// Retaille la colonne r�sultat abscisse et ordonn�e
	m_GridMapping.SetColWidth(m_NoColTarif,m_NoColTarif+1,80,0,GX_UPDATENOW);

	// Mise � jour des modifs
	m_GridMapping.LockUpdate(false);
	m_GridMapping.UpdateWindow();
	m_GridMapping.Redraw();	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgMapping::UpdateVisuSupports()
{
	// Debut et fin marquage support s�lectionn�
	JUnt32 NoColDeb = m_NoColSupport-1;    
	JUnt32 NoColFin = m_GridMapping.GetColCount();  // avant m_NoColTarif-1

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup �lmt
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Support s�lectionn�, on surligne la ligne
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  											CGXStyle()
												.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
			}
			else
			{
				// Support non s�lectionn�, on laisse la ligne en blanc
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  											CGXStyle()
												.SetInterior(CATPColors::GetColorWhite()));
			}

			// Diff�rencier les familles (soit couleur, soit font)
			if (pElem->IsLot())
			{
				m_GridMapping.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
											CGXStyle()
												.SetInterior(CATPColors::GetColorBeige()));
			}
		}
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lection supports
void CDlgMapping::UpdateEtatSelSupports()
{
	// Autorise modif grid et bloque mise � jour
	//m_GridMapping.SetReadOnly(false);
	m_GridMapping.LockUpdate(true); 

	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat s�lection en cours
			CGXStyle Style;
			m_GridMapping.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 

			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la s�lection support
				if (ValSel != 1)
				{
					m_GridMapping.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
			}
			else
			{
				// D�valide la s�lection support
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

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
		JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		// R�cup�ration validit� support sur terrain(s)/segment(s)
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
	
	// Mise � jour affichage
	m_GridMapping.LockUpdate(false);
	m_GridMapping.Redraw();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration s�lection Nb Insertions
void CDlgMapping::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi s�lection Nb Insertions
JUnt32 CDlgMapping::GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Update r�sultat mapping
bool CDlgMapping::UpdateResultMapping(JBool CalculResult)
{
	// Test validit� s�lection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si s�lection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// D�sactive mise � jour grid dup
	m_GridMapping.LockUpdate(true);

	// Mise � jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise � jour nombre et headers rows
	if (!UpdateRow()) return false;

	// Calcul des r�sultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Remettre les s�lections supports et/ou famille, et r�afficher proprement les colonnes
	UpdateVisuSupports();
	UpdateEtatSelSupports();
	AfficheResultat();

	// Bloque resize hauteur des lignes
	m_GridMapping.GetParam()->EnableTrackRowHeight(FALSE);

	// Active mise � jour grid dup
	m_GridMapping.LockUpdate(false);

	// reddessine le grid dup
	m_GridMapping.Redraw();

	// Rend visible le grid, si au moins quelques r�sultats
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

	// Test si les supports ont chang�s
	if (SupportModif())
	{
		// Mise � jour du mapping avec modfication des supports
		MajMapping(true);
	}
	else
	{
		// Mise � jour du mapping en gardant les m�mes supports
		MajMapping(false);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Les supports du mapping ont-ils �t� modifi�s
JBool CDlgMapping::SupportModif()
{
	// Test d�j� si pas le m�me nombre de supports
	if (m_BagMining.GetCount() != pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{
		return true;
	}
	else
	{
		// Balaye les elmts ident mapping
		for (JUnt32 i = 0; i < m_BagMining.GetCount(); i++)
		{
			// R�cup ident support mapping
			JUnt32 IdSuppMapping = m_BagMining.GetIdent(i);

			// Test si pr�sent dans �tude en cours
			pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSuppMapping);

			// Si non pr�sent, on redessinera le mapping
			if (!pDocChoixSupport->m_MapIdSupportsSel.IsValid())
				return true;
		}
	}

	// M�mes supports
	return false;
}	

//////////////////////////////////////////////////////////////////////////////////////////////
// Update des r�sultats au niveau grille cellule (ne touche pas aux headers)
bool CDlgMapping::UpdateResultat(JBool CalculResult)
{
	// Delock mode read only
	m_GridMapping.GetParam()->SetLockReadOnly(FALSE);

	// Init Affichage R�sultats Cellules
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
				// Mise � jour des r�sultats cellules
				if (!InitGridResultat()) 
					return false;
			}
			else
			{
				// Affichage des r�sultats cellules
				if (!AfficheResultat()) 
					return false;
			}
		}		

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections format supports
bool CDlgMapping::UpdateSelFormatSupports()
{
	// Initialise libell�s supports en 1ere colonne
	InitRowSupports();

	// Mise � jour du mapping
	MajMapping(true);

	return true;
}

//////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgMapping::UpdateAllViews(const JFCUpdate & Message)
{
	// R�init grille mapping et graphe mapping
	if (Message == UPDATE_RAZ)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialise grid r�sultat
	    m_GridMapping.Initialize();
		GXSetNewGridLineMode(); 
		m_GridMapping.GetParam()->EnableUndo(FALSE);
		m_GridMapping.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridMapping.GetParam()->SetNumberedColHeaders(false); 
		m_GridMapping.GetParam()->SetNumberedRowHeaders(false); 
		m_GridMapping.GetParam()->SetSmartResize(true);

		// Mise � jour des infos fen�tre mapping
		UpdateTitreAnalyse();

		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour du graphe
		MajMapping(true);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	else if(Message == UPDATE_COLOR)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mise � jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		//Init Contr�les
		// SetupControls();

		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour des nouveaux r�sultats (grid + graphe mapping)
		// @@@@@ Pourquoi le faire ici, car �a prend du temps quand on est dans �chelle
		// il faut trouver un moyen de le faire dans le
		InitGridResultat();

		/*
		// Mise � jour du mapping
		MajMapping();
		*/

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/

	}

	// Mise a jour suite � changement Offres
	else if (Message == UPDATE_OFFRE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour du mapping
		MajMapping(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise � jour format si nouveaux supports
		MajFormat();

		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour des nouveaux r�sultats (grid + graphe mapping)
		// ????????????????????? A VOIR si on l'enleve
		// InitGridResultat();

		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		// ?????????????????????????? A VOIR Si on l'enleve UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
	}

	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// A FAIRE
		// Mise � jour du mapping
		MajMapping(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Mise � jour du mapping
		MajMapping(false);
		
		// Update �tat s�lection supports
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise � jour du mapping
		MajMapping(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour du mapping
		MajMapping(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise � jour r�sultats co�t
		if (this->IsWindowVisible())
			UpdateResultat(true);
		else
			pDocChoixSupport->m_ParamMapModif = true;

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise a jour suite � changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour du mapping
		MajMapping(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Mise � jour r�sultats co�t
		if (this->IsWindowVisible() == TRUE)
			UpdateResultat(true);
		else
			pDocChoixSupport->m_ParamMapModif = true;

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	/* Pour le moment 1 seule insertion ==> invariable
	// Mise � jour suite au changement 
	else if (Message == UPDATE_INSERTIONS)
	{
		// Vire le map courant r�sultat
		pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleMap);

		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour des ent�tes headers car r�p�tition ou reprise en main)
		UpdateRow();

		// Mise � jour des r�sultat (uniquement si fen�tre mapping visible)
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamMapModif = true;

		// Mise � jour du mapping
		MajMapping(true);
	}
	*/

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Mise � jour r�sultats co�t
		UpdateResultat(false);

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise � jour suite � modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse(); 

		// Mise � jour des r�sultat --> d�j� fait dans dialogue g�n�rale
		// InitGridResultat();
	}

	// Mise � jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise � jour grid mapping
		m_GridMapping.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Mise � jour du graphe
		MajMapping(true);

		// Active mise � jour grid mapping
		m_GridMapping.LockUpdate(false);

		// reddessine le grid mapping
		m_GridMapping.Redraw();

		/*
		/////////////////////////////////////////
		// A VOIR --- en attente simplification
		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		/////////////////////////////////////////
		*/
	}

	// Mise � jour des taris suite � modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise � jour grid mapping
		m_GridMapping.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour du graphe
		MajMapping(true);

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		/*
		// Mise � jour du grid mapping
		InitGrid();

		// Mise � jour des nouveaux r�sultats (grid + graphe mapping)
		// @@@@@ Pourquoi le faire ici, car �a prend du temps quand on est dans �chelle
		// il faut trouver un moyen de le faire dans le
		InitGridResultat();

		
		// A VOIR /////////////////////////////////////////////////////////////
		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(false);

		// mise � jour du mapping
		MajMapping(false);

		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);
		///////////////////////////////////////////////////////////////////////

		// Active mise � jour grid mapping
		m_GridMapping.LockUpdate(false);
		*/

		// reddessine le grid mapping
		m_GridMapping.Redraw();
	}

	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise � jour grid mapping
		m_GridMapping.LockUpdate(true);

		MajFormat();

		// Affichage ou non des supports dans grille mapping
		UpdateAfficheSupports();

		// Active mise � jour grid mapping
		m_GridMapping.LockUpdate(false);

		// redessine le grid mapping
		m_GridMapping.Redraw();

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(false);
		// UpdateResultMapping(true);		// OPTIM 2.7

		// mise � jour du mapping
		MajMapping(false);

		// on redessine le mapping
		m_Mapping.RedrawWindow();

		// Mise � jour des diff�rentes fen�tres 
		UpdateResultMapping(true);

		// Mise � jour du graphe
		m_Mapping.SetItems(&m_BagMining,true);

		// on redessine le mapping
		// m_Mapping.RedrawWindow();
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats supports non d�finis
JVoid CDlgMapping::MajFormat()
{
	// Flag si maj effective
	JBool MajOk = false;

	// Identificateur format courant
	JUnt32 IdFormat = 0;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    

	// Boucle sur tous les supports, on ne traitera que les supports non d�fini au niveau format
	for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// Ident support
		JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

		// Voir si support non d�fini au niveau format
		pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
		if (!pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
		{
			// R�cup�ration des idents libell�s format possible pour un support
			JMap <JUnt32,JUnt32> MapIdFormats;
			pDocChoixSupport->GetListIdFormats(IdSupport, &MapIdFormats);

			// Si le format courant appartient aux formats possibles supports, 
			// on le met comme format s�lectionn� pour les nouveaux supports
			MapIdFormats.MoveTo(IdFormat);
			if (MapIdFormats.IsValid())
			{
				// Indique information format associ� � ce support
				pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;		
			}
			else
			{
				// Pas de format pour le moment associ� � ce support
				pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = 0;		
			}

			// Mise � jour format pour les nouveaux supports
			MajOk = true;
		}
	}

	// Init infos rows supports
	if (MajOk == true) 
	{	
		// Mise � jour des supports s�lectionn�s
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

	// Update R�sultat
	m_GridMapping.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridMapping.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridMapping.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// R�cup�ration validit� support sur terrain(s)/segment(s)
		JBool ValideSupport = true;
		if (pElem)
		{
			if (pElem->IsTitre() || pElem->IsCouplage())
					pDocChoixSupport->ValideSupportSegments(IdSupport);
		}

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
		JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false || VisibleSupport == false)
		{	
			// Cache la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,0); 

			// Mise � jour graphe mapping
			m_BagMining.SetShowItemById(IdSupport,false); 
		}
		else
		{
			// Rend visible la ligne support
			m_GridMapping.SetRowHeight(NoLig, NoLig ,16); 

			// Mise � jour graphe mapping
			m_BagMining.SetShowItemById(IdSupport,true); 
		}
	}

	// Mise � jour du mapping
	m_Mapping.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour du document Mapping (avec r�init ou pas des positions)
void CDlgMapping::MajMapping(JBool InitPos)
{
	// Attention, si r�initialisation, on n'oublies pas de virer les infos courbes
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
		// on met � jour le mapping
		m_Mapping.SetItems    (&m_BagMining   );

		// R�cup infos courbes
		m_Mapping.SetCurve    (&m_BagCurve    );

		//	m_Mapping.SetHierarchy(&m_BagHierarchy);

		// repositionne �tat affichage
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
// la fonction pour mettre � jour la s�lection
void CDlgMapping::UpdateSelection()
{
	// on redessine le mapping
	m_Mapping.InvalidateRect(NULL, FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats supports dans mapping
void CDlgMapping::UpdateFormatSupports()
{
	// on teste la validit� du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise � jour map des formats supports mapping
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

			// Indique information format associ� � ce support
			pDocChoixSupport->m_MapIdFormatSpecifMap.Add(IdSupport) = IdFormat;	
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// D�finit si la grille r�sultat est valide
bool CDlgMapping::GrilleResultValid()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount()  			> 0		&&
		pDocChoixSupport->m_NbInsertions							> 0		&& 
		pDocChoixSupport->m_DateTarifaire.IsValid())
	{
		// La grille est valide
		return true;
	}

	// Pas de grille r�sultat d�finie
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Update Entete Analyse (Nom de la source / terrain et date tarification)
void CDlgMapping::UpdateTitreAnalyse()
{
	CString Titre = "Analyse non d�finie";
	if (pDocChoixSupport)
	{
		// On v�rifie qu'une source terrain a �t� choisi, et auusi une date tarification
		if (pDocChoixSupport!=0 && pDocChoixSupport->IsValid())    
		{
			// Ajoute Info Filtre d'audience
			JSRCPresse* m_srcPresse;
			m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

			// R�cup Id Filtre audience du module courant
			JUnt32 IdFiltreAudienceSel = pDocChoixSupport->GetIdFiltreAudienceSel(); 

			// R�cup�re label 
			Titre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(IdFiltreAudienceSel);
		
			// Ajoute libell� de la source
			JUnt32 SrcID = 0;
			if (pDocChoixSupport->GetIdSrcAudienceSel())
			{
				// R�cup Table
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();

				// R�cup la source audience courante
				SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

				// R�cup le nouveau source
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
				// R�cup�re ident cible
				JUnt32 IdCible	= pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();

				// R�cup libell� 
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

		}

		// Ecrit le titre analyse non d�finie
		m_TitreMapping.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridMapping.GetRowCount() > 1)
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
			m_GridMapping.GetParam()->SetLockReadOnly(FALSE);
			m_GridMapping.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtMoteur));

			// On bloque les colonnes qui doivent ne pas �tre modifi�es
			m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}

		// Nombre d'insertions d�sormais dans le header au dessus de "Supports"
		if (m_GridMapping.GetRowCount() > 1)
		{
			// Texte inf onb insertions
			CString TxtNbInsertion;
			// TxtNbInsertion.Format("%s : %d","Nombre d'insertions",pDocChoixSupport->m_NbInsertions);
			// Nombre d'insertion forc�e � 1 pour le moment
			TxtNbInsertion.Format("%s : %d","Nombre d'insertions",1);

			// On l'affiche dans la grille
			//m_GridMapping.SetReadOnly(FALSE);
			m_GridMapping.GetParam()->SetLockReadOnly(FALSE);
			m_GridMapping.SetStyleRange(CGXRange(1,m_NoColSelSupport,1,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtNbInsertion));

			// On bloque les colonnes qui doivent ne pas �tre modifi�es
			m_GridMapping.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
JVoid CDlgMapping::UpdateResultCout()
{
	// Mise � jour fen�tre �chelle
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
// R�cup�re nombre de lignes header
JUnt32 CDlgMapping::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de colonnes header
JUnt32 CDlgMapping::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export des r�sultats Mapping
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
		// Cr�ation du fichier
		CStdioFile ExportFile;
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
		
		// Export r�sultat Mapping
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

		// Bloc n� 3 : Infos Terrain + Cible + Effectif + Nb Cas
		// ------------------------------------- Les effectifs des cibles utilis�es -------------------------------------------
		//
		// R�cup�ration du bloc infos terrains / cibles
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

	// Test si au moins 1 support s�lectionn� pour le mapping
	if(!m_Mapping.ExportItems(480,DlgFile.GetPathName()))
		AfxMessageBox("Erreur lors de l'export");	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'ent�te pour export Mapping
void CDlgMapping :: RecupEnteteExport(CString &Entete)
{
	// N� Version
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

	// Boucle sur les �lmts segment audience s�lectionn�s pour mapping
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

	// --------------------------------------------- Export Nb Insertions ------------------------------------------
	//
	// on exporte le titre
	Entete += "Nb insertions : \t";

	// str.Format("%d", pDocChoixSupport->m_NbInsertions);
	// 1 seule insertion pour le moment
	str.Format("%d", 1);
	Entete += str;
	Entete += "\n";

	// Pr�voir sauts de ligne avant affichage infos tableurs
	Entete += "\n\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration r�sultats mapping d�taill�e
void CDlgMapping ::RecupGrilleExport(CString &Entete)
{
	// Init chaine r�sultat
	Entete = "";

	// R�cup niveau de chacune des grandeurs
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

	// Tableau de lignes et colonnes s�lectionn�es
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridMapping.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridMapping.GetSelectedRows(Rows,false,false);

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
				Txt = m_GridMapping.GetValueRowCol(nRow,NoCol); 
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
		JUnt32 IdSupport = atoi(m_GridMapping.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Attention si support non visible
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
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

				// Recup 1er terrain s�lectionn�
				JUnt32 IdTerrain;
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
				{
					// au moins 1 terrain s�lectionn�
					KeyResultMap.m_IdTerrain  = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
				}
				else
					continue;

				// Recup 1ere cible s�lectionn�e
				JUnt32 IdCible;
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
				{
					// au moins 1 cible s�lectionn�e
					KeyResultMap.m_IdCible		 = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
				}
				else
					continue;

				// C'est un �lmt famille
				ElmtFamille = true;
			}

			// Ecriture infos g�n�rales supports
			for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
			{	
				Txt = "";
				if (nCol != m_NoColFormat-1)
				{	
					// Libell� support, ou p�riodicit� )
					Txt += m_GridMapping.GetValueRowCol(NoRow,nCol); 

					// Info famille en +
					if ((nCol == m_NoColSelSupport-1) && ElmtFamille)
						Txt += "F";
				}
				else
				{
					// R�cup�re le format support
					JUnt32 IdFormatSupport = 0;
					pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(IdSupport);
					if (pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
						IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();

					// libell� format support
					Txt = pDocChoixSupport->GetFormat(IdFormatSupport,true);
				}

				Entete += Txt +  "\t";
			}

			// Ecriture r�sultats colonnes
			for (int nCol = 0; nCol < Cols.GetCount(); nCol++)
			{
				// No de la colonne r�sultat
				int NoCol = Cols.GetAt(nCol);

				// Attention ici d'indiquer les r�sultats partiels (pour certaines familles)
				Txt = "";
				if (NoCol >= m_NoColTarif && ElmtFamille)
				{
					// On se positionne sur r�sultats famille	
					pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResultMap);
					if (pDocChoixSupport->m_MapResultatMap.IsValid()) 
					{
						// R�cup�re r�sultat pour savoir �tat r�sultat : partiel ou complet
						ResultMapFam = pDocChoixSupport->m_MapResultatMap.GetItem();
					
						// Ajoute indicateur r�sultat partiel
						if(ResultMapFam.m_ResultPartiel)
						{	
							Txt = "*";
						}
					}
				}

				Txt += m_GridMapping.GetValueRowCol(NoRow,NoCol); 
				Entete += Txt +  "\t";
			}

			// Saut apr�s chaque ligne support
			Entete += "\n";
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re n� colonne abscisse mapping
JUnt32 CDlgMapping::RecupNoColAbscisseMap()
{
	// R�sultat abscisse juste apr�s colonne tarif
	return m_NoColTarif;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re n� colonne ordonn�e mapping	
JUnt32 CDlgMapping::RecupNoColOrdonneeMap()
{
	// R�sultat ordonn�e juste apr�s colonne abscisse
	return m_NoColTarif+1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du bloc infos terrains / cibles
void CDlgMapping ::RecupBlocTerrainCible(CString &Entete)
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
// Clipboard du grille mapping
bool CDlgMapping::ClipboardResultat()
{
	// S�lection globale du grid
	POSITION area = m_GridMapping.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridMapping.SetSelection(area, 0, 0, m_GridMapping.GetRowCount(), m_GridMapping.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridMapping.Copy(); 

	// D�selection globale
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
	// S�lection globale du grid
	POSITION area = m_GridMapping.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridMapping.SetSelection(area, 0, 0, m_GridMapping.GetRowCount(), m_GridMapping.GetColCount());

	// Copie dans clipboard
	m_GridMapping.Copy(); 

	// D�selection globale
	m_GridMapping.SetSelection(NULL, 0, 0, 0, 0);

	// Test si au moins 1 support s�lectionn� pour le mapping
	if(!m_Mapping.ExportItems(480))
		AfxMessageBox("Erreur lors de l'export");	

}

///////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du bitmap mapping
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
    // mise a jour le 15/01/2005 :ajuste le bitmap sur la totalit� de la feuille et edite en mode paysage. 
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
