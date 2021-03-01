// DlgDuplication.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgDuplication.h"
#include "ExcelPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// Bo�te de dialogue CDlgDuplication

IMPLEMENT_DYNAMIC(CDlgDuplication, JFCDialog)
CDlgDuplication::CDlgDuplication(CWnd* pParent, /*=NULL*/
								 JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgDuplication::IDD, pParent)
{
	// R�cup�re pointeur s�lection g�n�rale
	pDocChoixSupport = pDoc;

	// Init Lignes/Colonnes Grid
	m_NbHeaderRow  = 2;
	m_NbRowSupport = 0;
	m_NbHeaderCol  = 1;
	m_NbCol        = 0;
}

CDlgDuplication::~CDlgDuplication()
{
}

void CDlgDuplication::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_CADRE_DUPLICATION, m_CadreDuplication);
	DDX_Control(pDX, IDC_CS_TITRE_DUPLICATION, m_TitreDuplication);
	DDX_Control(pDX, IDC_CS_PROGRESS_RESULTDUP, m_ProgressResultDup);
}


BEGIN_MESSAGE_MAP(CDlgDuplication, JFCDialog)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// Initialisation des diff�rents contr�les fen�tre Duplication
BOOL CDlgDuplication::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre duplication
	SetupControls();

	// Titre Source - Filtre d'audience etc....
	UpdateTitreAnalyse();

	// Mise en place des realtions objets fen�tre duplication
	SetWindowRelations();

	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Duplication
void CDlgDuplication::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
			// Cadre englobant les �lmts r�sultats duplication
			{IDC_CS_CADRE_DUPLICATION,  {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side

			// Titre Duplication (Source + Source Audience)
			{IDC_CS_TITRE_DUPLICATION,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
						  				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},			//Bottom side

			// Grille R�sultat Duplication
			{IDC_CS_GRID_DUPLICATION,  {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
									   {CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
									   {CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
									   {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side

			// Barre de progression
			{IDC_CS_PROGRESS_RESULTDUP,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Duplication
void CDlgDuplication::SetupControls()
{
	// Association objet / ressource
	m_GridDuplication.SubclassDlgItem(IDC_CS_GRID_DUPLICATION, this);

	// Propri�t�s du cadre Duplication
	m_CadreDuplication.SetTransparent(false);
	m_CadreDuplication.SetSunken(true);

	// Controles en avant plan
	m_CadreDuplication.AddTopWindow(m_TitreDuplication);
	m_CadreDuplication.AddTopWindow(m_GridDuplication); 
	m_CadreDuplication.AddTopWindow(m_ProgressResultDup);

	// Progress bar non visible au d�part
	m_ProgressResultDup.ShowWindow(SW_HIDE); 
 
	// Taille et Font des diff�rents labels
	m_TitreDuplication.SetFontName(CATPColors::GetFontLabelName());
    //m_TitreDuplication.SetFontSize(CATPColors::GetFontBigLabelSize());
	m_TitreDuplication.SetFontBold(TRUE); 
	m_TitreDuplication.SetFontSize(CATPColors::GetFontLabelSize());

	// Initialise grid r�sultat duplication
	m_GridDuplication.EnableMouseWheel(); 
	m_GridDuplication.Initialize(); 
	GXSetNewGridLineMode(); 
    m_GridDuplication.GetParam()->EnableUndo(FALSE);
	m_GridDuplication.SetMergeCellsMode(gxnMergeDelayEval);
	m_GridDuplication.GetParam()->SetNumberedColHeaders(false); 
	m_GridDuplication.GetParam()->EnableMoveCols(FALSE);
	m_GridDuplication.GetParam()->SetMinTrackColWidth(16);
	m_GridDuplication.GetParam()->SetNumberedRowHeaders(false); 
	m_GridDuplication.GetParam()->EnableMoveRows(FALSE);
	m_GridDuplication.GetParam()->SetMinTrackRowHeight(16);

	m_GridDuplication.GetParam()->SetSmartResize(true);

	// Enregistrement controle sp�cifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(&m_GridDuplication);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridDuplication.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Init R�f�rence colonne
	InitRefGrid();

	// Associe le document au grille
	if (m_GridDuplication.OnInitDocument(pDocChoixSupport, this, m_NoColIdSupport) == false)
		AfxMessageBox ("Probl�me association document supports / grille duplication");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid r�sultat
	if (GrilleResultValid())
		InitGridResultat();

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des r�f�rences colonnes
void CDlgDuplication::InitRefGrid()
{
	// Position colonne invisible id support
	m_NoColIdSupport	= 1;

	// Position colonne sel supports
	m_NoColSelSupport	= m_NoColIdSupport  + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne formats
	// m_NoColFormat		= m_NoColSupport + 1;

	// Position colonne p�riodicit�
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColSupport + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid
void CDlgDuplication::InitGrid()
{
	// Enleve n� ligne col 
	m_GridDuplication.GetParam()->SetNumberedColHeaders(false);
	m_GridDuplication.GetParam()->SetNumberedRowHeaders(false);

	// Position colonne invisible id support
	m_NoColIdSupport	= 1;

	// Position colonne sel supports
	m_NoColSelSupport	= m_NoColIdSupport  + 1;

	// Position colonne supports
	m_NoColSupport		= m_NoColSelSupport + 1;

	// Position colonne formats
	// m_NoColFormat		= m_NoColSupport + 1;

	// Position colonne p�riodicit�
	// m_NoColPeriodicite	= m_NoColFormat + 1;
	m_NoColPeriodicite	= m_NoColSupport + 1;

	// Init colonnes
	UpdateCol();

	// Initialisation si au moins 1 support
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{
		// Init Lignes
		UpdateRow();

		// Delock mode read only
		m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

		// Lignes non resizable et nb lignes headers
		m_GridDuplication.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

		// Aucun header colonne et colonnes bloqu�s (support / p�riodicit�)
		m_GridDuplication.SetFrozenCols(m_NoColPeriodicite - m_NoColIdSupport,0);

		// Affichage entete Tarif et p�riodicit�
		if (m_GridDuplication.GetRowCount() > 0) 
		{
			// Titre "SEL"
			m_GridDuplication.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
									  		 CGXStyle()
												.SetValue(_T("X")));
			// Titre "SUPPORTS"
			m_GridDuplication.SetStyleRange(CGXRange(1,m_NoColSupport-1),
									  		 CGXStyle()
												.SetValue(_T("Supports")));

			/*
			// Titre "FORMATS"
			m_GridDuplication.SetStyleRange(CGXRange(1,m_NoColFormat-1),
									  		 CGXStyle()
												.SetValue(_T("Formats")));
			*/

			// Titre "PERIODICITE"
			m_GridDuplication.SetStyleRange(CGXRange(1,m_NoColPeriodicite-1),
											 CGXStyle()
									 			.SetValue(_T("P�riodic.")));
		}

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Merge des cellules en header niveau 1
	m_GridDuplication.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_GridDuplication.GetColCount()),
					 		        CGXStyle()
										.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));

	// Rend visible le grid, si au moins quelques r�sultats
	int NbCol = m_GridDuplication.GetColCount();
	int NbRow = m_GridDuplication.GetRowCount();

	if (m_GridDuplication.GetColCount() >= m_NoColPeriodicite)
		m_GridDuplication.ShowWindow(SW_SHOW);
	else
		m_GridDuplication.ShowWindow(SW_HIDE);
	
}

// D�finition des couleurs interface fen�tre Duplication
void CDlgDuplication::SetColors()
{
	// Coloriage du cadre englobant
	m_CadreDuplication.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultDup.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)); 

	// Coloriage Titre Duplication
	m_TitreDuplication.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreDuplication.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Coloriage Grille r�sultat
	m_GridDuplication.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridDuplication.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Les diff�rentes couleurs pour headers ou cellules sp�cials
	UpdateColorSpe();

	// Style des lignes (solid pen)
	m_GridDuplication.GetParam()->SetGridLineStyle(PS_SOLID);

	// Style s�lection bloc (comme Excel)
	// m_GridDuplication.GetParam()->SetExcelLikeSelectionFrame(TRUE);
}

// Destruction de la fen�tre Duplication
void CDlgDuplication::OnDestroy()
{
	JFCDialog::OnDestroy();
}

// Retaille de la fen�tre Duplication
void CDlgDuplication::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

void CDlgDuplication::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		// Mise � jour du Grid r�sultat et graphe Duplication
		// AfxMessageBox("A FAIRE / DUPLICATION",MB_ICONINFORMATION);	
	}
}

void CDlgDuplication::OnCancel()
{
	JFCDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections supports
bool CDlgDuplication::UpdateSelSupports()
{
	// Nombre de supports nouvelle s�lection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille en colonne et en ligne
		UpdateCol();
		UpdateRow();

		// Initialise libell�s supports en 1ere colonne
		InitRowSupports();

		// Mise � jour du grid 
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur pr�c�demment existant, 
		// et nouvelle liste sans ascenseur !!!
		m_GridDuplication.SetTopRow(m_NbHeaderRow, GX_UPDATENOW); 
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Init lignes supports
void CDlgDuplication::InitRowSupports()
{
	// Nombre de supports
	JUnt32 NbSupports = pDocChoixSupport->m_MapIdSupportsSel.GetCount();

	if (NbSupports > 0)
	{

		// Delock mode read only
		m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

		// R�cup�re objet regroupement
		CRegroupements * pRegroupements = CRegroupements::GetInstance();

		// Index ligne support
		JUnt32 InxLigSupp = m_NbHeaderRow;

		// Style check-box libell�s supports
		m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp,
												m_NoColSelSupport-1,
												InxLigSupp + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
												m_NoColSelSupport-1),
										CGXStyle()
											.SetControl(GX_IDS_CTRL_CHECKBOXEX));

		// Style texte libell�s supports / formats / p�riodicit�
		m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp-1,
												m_NoColSupport-1,
												InxLigSupp+pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
												m_NoColPeriodicite-1),
										CGXStyle()
											.SetHorizontalAlignment(DT_LEFT)
											.SetVerticalAlignment(DT_BOTTOM)
											.SetFont(CGXFont()
											.SetSize(pDocChoixSupport->m_SizeFontCell)
											.SetBold(pDocChoixSupport->m_BoldFontCell)));

		// Alignement Vertical pour Titres Supports en colonnes
		m_GridDuplication.SetStyleRange(CGXRange(0,m_NoColPeriodicite-1,0,m_GridDuplication.GetColCount()),
										CGXStyle()
											.SetHorizontalAlignment(DT_CENTER)
											.SetVerticalAlignment(DT_TOP)
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
				m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
												CGXStyle()
													.SetValue(IdSupport));

				// on r�cup�re le libell� support
				CString LibSupport = "-";
				if (pSupport)
					// Affichage libell� support
					LibSupport = pSupport->GetLabel().AsJCharPtr();

				// Affichage libell� support
				m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
												CGXStyle()
													.SetValue(LibSupport));

				// R�cup�re info formats support
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;
				pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleDup);

				// Puis indication support s�lectionn� ou pas
				if (EtatSupport)
				{
					// Valide s�lection support
					m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
													CGXStyle()
														.SetValue((long)1));
				}
				else
				{
					// D�valide la s�lection support
					m_GridDuplication.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
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
		m_GridDuplication.SetColWidth(m_NoColIdSupport-1, m_NoColIdSupport-1,0);

		// Retaille la colonne textes supports
		m_GridDuplication.SetColWidth(m_NoColSelSupport-1,m_NoColSelSupport-1,20);

		/*
		// Retaille 
		if (m_GridDuplication.ResizeColWidthsToFit(CGXRange(0, m_NoColSupport-1, m_GridDuplication.GetRowCount(), m_NoColPeriodicite-1),
 														TRUE,GX_UPDATENOW | GX_INVALIDATE | GX_NOSHRINKSIZE))
		{
			int NbR = m_GridDuplication.GetRowCount();
			int Stop = 0;
		}
		*/

		// Delock mode read only
		m_GridDuplication.GetParam()->SetLockReadOnly(TRUE);
			
		// Hauteur des lignes
		JUnt32 NbRow = m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount();
		//m_GridDuplication.SetRowHeight(0,m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,20,0,GX_SMART);

		// Mise � jour couleur s�lection supports
		UpdateVisuSupports();

		// Retaille 
		m_GridDuplication.ResizeColWidthsToFit(CGXRange(0, m_NoColSupport-1, m_GridDuplication.GetRowCount(), m_NoColPeriodicite-1),
 														TRUE,GX_UPDATENOW | GX_INVALIDATE | GX_NOSHRINKSIZE);

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs sp�ciales de certaines cellules
void CDlgDuplication::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne r�sultat
	// Cellules info insertion en fond blanc et couleur caract�re diff�rent
	if (m_GridDuplication.GetColCount() >= m_NoColPeriodicite)
	{
		m_GridDuplication.LockUpdate(true); 

		// Niveau 0 / entete nom moteur (fond blanc et couleur caract�re diff�rent)
		m_GridDuplication.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 0, m_NoColPeriodicite-1),
										CGXStyle()
											.SetInterior(CATPColors::GetColorWhite())
											.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Niveau 0 / Au dessus des libell�s supports en colonne
		m_GridDuplication.SetStyleRange(CGXRange(0, m_NoColPeriodicite, 0, m_GridDuplication.GetColCount()),
											CGXStyle()
											.SetInterior(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));

		m_GridDuplication.LockUpdate(false); 
		m_GridDuplication.Redraw(); 
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Update r�sultat duplication
bool CDlgDuplication::UpdateResultDup(JBool CalculResult)
{
	// Test validit� s�lection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si s�lection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// D�sactive mise � jour grid dup
	m_GridDuplication.LockUpdate(true);

	// Mise � jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise � jour nombre et headers rows
	if (!UpdateRow()) return false;

	// Calcul des r�sultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Bloque resize hauteur des lignes
	m_GridDuplication.GetParam()->EnableTrackRowHeight(FALSE);

	// Active mise � jour grid dup
	m_GridDuplication.LockUpdate(false);

	// reddessine le grid dup
	m_GridDuplication.Redraw();

	// Rend visible le grid, si au moins quelques r�sultats
	if (m_GridDuplication.GetColCount() >= m_NoColPeriodicite)
		m_GridDuplication.ShowWindow(SW_SHOW);
	else
		m_GridDuplication.ShowWindow(SW_HIDE);

	// Update affichage infos analyse
	UpdateTitreAnalyse();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Update Entete Analyse (Nom de la source / terrain et date tarification)
void CDlgDuplication::UpdateTitreAnalyse()
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
				JUnt32 SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

				// R�cup le nouveau source
				const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

				// Recuperer dans table source
				//Titre += " - ";
				//Titre += pt->m_Libelle.AsJCharPtr();

				// Ajoute Info Terrain
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
				if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
				{
					// R�cup�re ident terrain
					JUnt32 TerID				= pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey();
					const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);
	
					// Recuperer dans table terrain
					//Titre += " ";
					//Titre += pt->m_Libelle.AsJCharPtr();
				}
			}
			else
			{
				//Titre += " - Aucune source";
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
					Titre += pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
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

			// Ajoute info type d'analyse en cours
			Titre += " - ";
			pDocChoixSupport->m_MapIdGrandDupSel.MoveFirst();
			if (pDocChoixSupport->m_MapIdGrandDupSel.IsValid())
			{
				// Indx Grandeur Analyse S�lectionn�e
				JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandDupSel.GetItem(); 

				// Ajoute au titre globale
				Titre += pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleDup, IdGrandeur);
			}
			else
				// Aucune analyse en cours
				Titre += "Aucune analyse";
		}

		// Ecrit le titre analyse non d�finie
		m_TitreDuplication.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridDuplication.GetRowCount() > 1)
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
			m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);
			m_GridDuplication.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColPeriodicite-1),
										    CGXStyle()
												.SetValue(TxtMoteur));
			m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
JVoid CDlgDuplication::UpdateResultCout()
{
	// Mise � jour fen�tre �chelle
	UpdateResultat(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections format supports
bool CDlgDuplication::UpdateSelFormatSupports()
{
	// Initialise libell�s supports en 1ere colonne
	InitRowSupports();

	return true;
}

//////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgDuplication::UpdateAllViews(const JFCUpdate & Message)
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
	    m_GridDuplication.Initialize();
		GXSetNewGridLineMode(); 
		m_GridDuplication.GetParam()->EnableUndo(FALSE);
		m_GridDuplication.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridDuplication.GetParam()->SetNumberedColHeaders(false); 
		m_GridDuplication.GetParam()->SetNumberedRowHeaders(false); 
		m_GridDuplication.GetParam()->SetSmartResize(true);

		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();
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
			if (pDocChoixSupport->m_ParamDupModif)
			{
				// Calcul duplication
				InitGridResultat();
			}
		}
		else
			pDocChoixSupport->m_ParamDupModif = true;

		// Mise � jour du bandeau informations
		UpdateTitreAnalyse();
	}

	// Mise � jour s�lection support
	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();
	}

	// Mise a jour suite � changement Cibles
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Puis on met � jour le map des formats supports dans duplication
		UpdateFormatSupports();
	}

	// Mise a jour suite � changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Mise � jour r�sultats co�t
		UpdateResultat(true);
	}

	// Mise � jour s�lection segment
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise � jour r�sultats
		UpdateResultat(true);
	}

	// Mise � jour suite � modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour des r�sultat --> d�j� fait dans dialogue g�n�rale
		// InitGridResultat();
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

		// Lock mise � jour grid duplication
		m_GridDuplication.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Active mise � jour grid duplication
		m_GridDuplication.LockUpdate(false);

		// reddessine le grid duplication
		m_GridDuplication.Redraw();
	}

	// Mise � jour des taris suite � modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise � jour grid duplication
		m_GridDuplication.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Active mise � jour grid duplication
		m_GridDuplication.LockUpdate(false);

		// reddessine le grid duplication
		m_GridDuplication.Redraw();
	}
	// Mise � jour visibilit� de chacun des supports
	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise � jour grid Duplication
		m_GridDuplication.LockUpdate(true);

		// Affichage ou non des supports dans grille Duplication
		UpdateAfficheSupports();

		// Active mise � jour grid Duplication
		m_GridDuplication.LockUpdate(false);

		// redessine le grid Duplication
		m_GridDuplication.Redraw();
	}
	// Relance tri avec mode tri courant
	else if (Message == UPDATE_TRI_COURANT_DUP)
	{
		// Active mise � jour grid duplication
		m_GridDuplication.LockUpdate(true);

		// On remet le tri � jour
		MajTriCourant();

		// Active mise � jour grid duplication
		m_GridDuplication.LockUpdate(false);

		// reddessine le grid duplication
		m_GridDuplication.Redraw();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour du tri courant
JVoid CDlgDuplication::MajTriCourant()
{
	// Retri avec les m�mes param�tres de tri
	m_GridDuplication.SortColonneCourante(false);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgDuplication::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Update R�sultat
	m_GridDuplication.UpdateData();

	/*
	// Rend visible la colonne support sym�trique � la ligne
	m_GridDuplication.ResizeColWidthsToFit(CGXRange(0, m_NbHeaderCol, m_GridDuplication.GetRowCount(),m_GridDuplication.GetColCount()),
 											  	   TRUE,GX_UPDATENOW | GX_INVALIDATE | GX_NOSHRINKSIZE);
	*/

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Libell� du support - pour recalage avec colonnes - � cause du tri
		CString LibLigne = m_GridDuplication.GetValueRowCol(NoLig, m_NoColSupport-1);

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Recherche No Colonne associ�e par libell� (�a suffit pour retrouver le support associ�)
		JUnt32 NoCol = m_NbHeaderCol;
		for (; NoCol <m_GridDuplication.GetColCount(); NoCol++)
		{
			// Recherche m�me libell�
			CString LibColonne = m_GridDuplication.GetValueRowCol(m_NbHeaderRow-1, NoCol);

			// On a trouv� la colonne associ�e
			if (LibColonne == LibLigne)	break;
		}

		// R�cup�ration validit� support sur terrain(s)/segment(s)
		JBool ValideSupport = pDocChoixSupport->ValideSupportSegments(IdSupport);

		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false)
		{	
			// Cache la ligne support
			m_GridDuplication.SetRowHeight(NoLig, NoLig ,0); 

			// Cache la colonne support sym�trique � la ligne
			m_GridDuplication.SetColWidth(NoCol, NoCol, 0); 
		}
		else
		{
			// Rend visible la ligne support 
			m_GridDuplication.SetRowHeight(NoLig, NoLig ,16); 

			// Rend visible la colonne support sym�trique � la ligne
			m_GridDuplication.SetColWidth(NoCol, NoCol, 70); 
			m_GridDuplication.ResizeColWidthsToFit(CGXRange(0, NoCol, m_GridDuplication.GetRowCount(), NoCol),
 											  	   TRUE,GX_UPDATENOW | GX_INVALIDATE | GX_NOSHRINKSIZE);
		}
	}

	// Redessine 
	m_GridDuplication.RedrawWindow(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats supports dans duplication
void CDlgDuplication::UpdateFormatSupports()
{
	// on teste la validit� du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise � jour map des formats supports duplication
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		pDocChoixSupport->m_MapIdFormatSpecifDup.Reset();
		for (pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
				pDocChoixSupport->m_MapIdSupportsSel.IsValid();
				pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
		{
			// Ident support
			JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();

			// Indique information format associ� � ce support
			pDocChoixSupport->m_MapIdFormatSpecifDup.Add(IdSupport) = IdFormat;	
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Colonnes
bool CDlgDuplication::UpdateCol()
{
	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Colonnes (Sel Support + Support + Format + P�riodicit�)
	m_NbHeaderCol = m_NoColPeriodicite - m_NoColIdSupport + 1;

	// Nb Colonnes
	if (pDocChoixSupport != NULL && pDocChoixSupport->m_MapIdSupportsSel.GetCount() > 0)
	{
		// Nb colonnes = nombre de supports en cours
		m_NbCol = pDocChoixSupport->m_MapIdSupportsSel.GetCount();
	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Init grid avec nb colonnes
    if (!m_GridDuplication.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Lignes
bool CDlgDuplication::UpdateRow()
{
	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// R�cup�re objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Nb Header Lignes (1 au total)
	m_NbHeaderRow = 2;

	// Nombre de lignes
    if (!m_GridDuplication.SetRowCount(m_NbHeaderRow + m_NbRowSupport-1,GX_UPDATENOW))
		return false;

	// Lignes headers 
	m_GridDuplication.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Remplissage des entetes header rows
	if (m_NbCol >= 1)
	{
		// Ligne header niveau 0 r�serv� � la grandeur s�lectionn�e
		// Centrage et style texte header
		m_GridDuplication.SetStyleRange(CGXRange(0, 0, 0, m_GridDuplication.GetColCount()),
										 CGXStyle()
											.SetHorizontalAlignment(DT_CENTER)
											.SetVerticalAlignment(DT_VCENTER)
											.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL))
											.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontCell)
														.SetBold(pDocChoixSupport->m_BoldFontCell)));

		// Affichage des headers ligne / colonne (en l'occurence les supports)
		for (JUnt32 NoLig = 0; NoLig < m_NbHeaderRow ; NoLig++)
		{
			// Pour chaque colonne
			JUnt32 NbCol = m_GridDuplication.GetColCount();

			// pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
			pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();  

			for (JUnt32 NoCol = 0; NoCol < m_NbHeaderCol + pDocChoixSupport->m_MapIdSupportsSel.GetCount(); NoCol++) 
			{
				// Centrage et style texte header
				m_GridDuplication.SetStyleRange(CGXRange(NoLig,NoCol),
												 CGXStyle()
													.SetHorizontalAlignment(DT_CENTER)
													// .SetVerticalAlignment(DT_BOTTOM)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
													.SetSize(pDocChoixSupport->m_SizeFontHeader)
													.SetBold(pDocChoixSupport->m_BoldFontHeader))
												);

				// Texte associ� au header la cellule
				CString Txt;
				if (NoCol >= m_NbHeaderCol)
				{
					// Init Libell� support
					CString LibSupport = "-";

					// Ident support
					if (pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid()) 
					{
						JUnt32 IdSupport = pDocChoixSupport->m_Offre.m_TBLIdSupports.GetItem();

						// V�rifie validit� du support
						pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
						if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())  
						{
							// Element support
							IElem* pSupport = pRegroupements->GetIElemByID(IdSupport);

							// on r�cup�re le libell� support
							
							if (pSupport)
								// Affichage libell� support
								LibSupport = pSupport->GetLabel().AsJCharPtr();
						}
					}

					// Affichage contenu cellule = libelle support
					m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),LibSupport); 

					// Passe au support suivant (via table offre)
					// pDocChoixSupport->m_MapIdSupportsSel.MoveNext();
					pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext();
				}
			}
		}

		// Couleyurs sp�ciales
		UpdateColorSpe();

		// Regroupement des lignes header row
		m_GridDuplication.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
		m_GridDuplication.SetStyleRange(CGXRange(0,0,m_NbHeaderRow-1,m_GridDuplication.GetColCount()),
									    CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));


		// Update Grille modifi�e
		m_GridDuplication.UpdateData(TRUE);

		// Mise � jour
		m_GridDuplication.UpdateWindow(); 
		m_GridDuplication.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Redessinne la grille
	m_GridDuplication.Redraw();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update des r�sultats au niveau cellule
bool CDlgDuplication::UpdateResultat(JBool CalculResult)
{
	if (m_GridDuplication.GetColCount() > 0 && m_GridDuplication.GetRowCount() > 0)
	{
		// Delock mode read only
		m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

		// Init Affichage R�sultats Cellules
		m_GridDuplication.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridDuplication.GetRowCount(),m_GridDuplication.GetColCount()),
								CGXStyle()
									.SetHorizontalAlignment(DT_RIGHT)
									.SetVerticalAlignment(DT_BOTTOM)
									.SetFont(CGXFont()
									.SetSize(pDocChoixSupport->m_SizeFontCell)
									.SetBold(pDocChoixSupport->m_BoldFontCell)));

		if (GrilleResultValid())
		{
			if (CalculResult && pDocChoixSupport->m_ParamDupModif)
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
		m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Redessine les sp�cifit�s graphiques cellule (en gras, en clair etc...)
	UpdateVisuSupports();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lection supports
void CDlgDuplication::UpdateEtatSelSupports()
{
	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat s�lection en cours
			CGXStyle Style;
			m_GridDuplication.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 


			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la s�lection support
				if (ValSel != 1)
				{
					m_GridDuplication.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
													CGXStyle()
														.SetValue((long)1));
				}
			}
			else
			{
				// D�valide la s�lection support
				if (ValSel != 0)
				{
					m_GridDuplication.SetStyleRange(CGXRange(NoLig, m_NoColSelSupport-1),
													CGXStyle()
														.SetValue((long)0));
				}
			}
		}
	}	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgDuplication::UpdateVisuSupports()
{
	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Support s�lectionn�, on surligne la ligne
				m_GridDuplication.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1,NoLig,m_GridDuplication.GetColCount()),
						 					    CGXStyle()
													.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
			}	
			else
			{
				// Support non s�lectionn�, on laisse la ligne en blanc
				m_GridDuplication.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1,NoLig,m_GridDuplication.GetColCount()),
					 						    CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
			}
		}		
	}

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupportRow = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Init NoColonne
		JUnt32 NoCol = m_NbHeaderCol;

		// Balaye tous les supports
		for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
			 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
			 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
		{
			// Ident support de la colonne
			JUnt32 IdSupportCol = pDocChoixSupport->m_Offre.m_TBLIdSupports.GetItem(); 

			if (IdSupportRow == IdSupportCol)
			{
				// Elmt de la diagonale surlign� en couleur fonc�
				m_GridDuplication.SetStyleRange(CGXRange(NoLig,NoCol,NoLig,NoCol),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS))
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(true)));
			}

			// Incr�ment n� de colonne
			NoCol++;
		}

		// Balaye toutes les familles
		for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
			 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
			 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
		{
			// A FAIRE - FAMILLES
		}
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du r�sultat duplication
JBool CDlgDuplication::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridDuplication.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultDup KeyResultDup;

	// Information r�sultat archiv�
	CResultDup	ResultDup;

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridDuplication.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridDuplication.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultDup.ShowWindow(SW_SHOW); 
	m_ProgressResultDup.SetShowText(false); 
	int nLower, nUpper;
	m_ProgressResultDup.GetRange( nLower, nUpper);

	// Identificateur format courant
	JUnt32 IdFormat;
	pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    
	else
		return false;

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

	// Positionnement moteur
	KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(), IdTerrain, IdCible);

	// Calcul indice cl� segment
	KeyResultDup.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleDup); 

	// Calcul indice cl� moteur
	KeyResultDup.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Cr�e la clef audience 
	JMTFR03KEYAudience KeyAudience;
	KeyAudience.m_IdSource	= pDocChoixSupport->GetIdSrcAudienceSel(); 
	KeyAudience.m_IdTerrain = IdTerrain;
	KeyAudience.m_IdCible	= IdCible; 
	KeyAudience.m_IdTitre	= 1;

	// R�cup�ration des diff�rentes tables et options moteur
	const JMTFR03TBLTerrains	 & TblTerrains		= pDocChoixSupport->m_MoteurPresse.GetTblTerrains();
	const JMTFR03TBLCibles		 & TblCibles		= pDocChoixSupport->m_MoteurPresse.GetTblCibles();
	const JMTFR03TBLModulations	 & TblModulations   = pDocChoixSupport->m_MoteurPresse.GetTblModulations();
	const JMTFR03Options		 & Options			= pDocChoixSupport->m_MoteurPresse.GetOptions();

	// Map stockage des calculs audience interm�diaires (pour �viter recalcul des audiences supports)
	JMap <JUnt32, JFlt64> MapAudLDPSupport;
	MapAudLDPSupport.Reset(); 

	// Calcul en mode sym�trie ou pas  (attention pas pour H% et V%)
	JBool CalculSymetrique = false;
	pDocChoixSupport->m_MapIdGrandDupSel.MoveFirst();
	if (pDocChoixSupport->m_MapIdGrandDupSel.IsValid())
	{
		JUnt32 IdGrandeur = pDocChoixSupport->m_MapIdGrandDupSel.GetItem();
		if (IdGrandeur != 3 && IdGrandeur != 4)   // ni H% , ni V%
		{
			// Calcul sym�trique
			CalculSymetrique = true;
		}
	}

	// Balayage ligne support
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultDup.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupportRow = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re le format support ligne s�lectionn� pour la cumulativit�
		JUnt32 IdFormatSupportRow = 0;
		pDocChoixSupport->m_MapIdFormatSpecifDup.MoveTo(IdSupportRow);
		if (pDocChoixSupport->m_MapIdFormatSpecifDup.IsValid())
			IdFormatSupportRow = pDocChoixSupport->m_MapIdFormatSpecifDup.GetItem();

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupportRow,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute un espace � gauche pour pr�sentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		// Affichage p�riodicit� support
		m_GridDuplication.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

		// Balayage des colonnes supports
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridDuplication.GetColCount(); NoCol++) 
		{
			// Calcul si NoRow >= NoCol (apr�s r�sult identique)

			// On ne calcul que la partie diagonal basse (si calcul <> H% et V%, sinon on recalcule tout  
			if (((NoLig - m_NbHeaderRow) >= (NoCol - m_NbHeaderCol) && CalculSymetrique == true) || CalculSymetrique == false)
			{
				// Ident support en colonne
				JUnt32 IdSupportCol = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(m_NbHeaderRow + NoCol - m_NbHeaderCol, m_NoColIdSupport-1));

				// Calcul R�sultat Colonne (selon grandeur)
				CString Txt;
				JFlt64 AudienceDup	= -1.0;
				JFlt64 Penetration	= -1.0;
				JFlt64 CentrageH	= -1.0;
				JFlt64 CentrageV	= -1.0;
				JFlt64 CoeffYule	= -1.0;
				
				// Key r�sultat
				KeyResultDup.m_IdCible			= IdCible; 
				KeyResultDup.m_IdTerrain		= IdTerrain;
				KeyResultDup.m_IdSupportRow		= IdSupportRow; 
				KeyResultDup.m_IdSupportCol	    = IdSupportCol;

				// Test si la cible est autoris�e
				// Pointe sur la matrice r�sultats existants
				pDocChoixSupport->m_MapResultatDup.MoveTo(KeyResultDup); 
				if (pDocChoixSupport->m_MapResultatDup.IsValid())
				{
					// Information r�sultat archiv�
					CResultDup	&ResultDup = pDocChoixSupport->m_MapResultatDup.GetItem();

					// Le r�sultat pour la cellule
					Txt = ResultCellule(NoLig,NoCol,KeyResultDup,ResultDup);
					m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
				}
				else
				{
					/////////////////// Calcul R�sultat Plan avec les 2 supports /////////
					
					// Effectif et nb cas
					JFlt64 Effectif;
					JInt32 NbCas;
					JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur, Effectif, NbCas); 

					// Calcul des audiences
					JFlt64	AudLDPSuppRow	= -1.0;
					JFlt64	AudLDPSuppCol	= -1.0;
					JFlt64	AudProbaSuppRow	= -1.0;
					JFlt64	AudProbaSuppCol	= -1.0;
					JFlt64	VolLectSuppRow	= -1.0;
					JFlt64	VolLectSuppCol	= -1.0;
					JInt32	PtEtal			= -1.0;		

					// V�rifie que calcul bien d�fini sur chacun des �l�ments
					// Calcul audience ligne si inexistante
					MapAudLDPSupport.MoveTo(IdSupportRow);
					if (!MapAudLDPSupport.IsValid())
					{
						JBool OkRow = pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupportRow, VolLectSuppRow, AudLDPSuppRow, AudProbaSuppRow, PtEtal, true);
						if (OkRow && pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain))
							MapAudLDPSupport.Add(IdSupportRow) = AudLDPSuppRow; 
						else
							MapAudLDPSupport.Add(IdSupportRow) = -1.0;
					}
					else
						// R�cup�re r�sultat archiv�
						AudLDPSuppRow = MapAudLDPSupport.GetItem(); 
					
					// Calcul audience colonne si inexistante
					MapAudLDPSupport.MoveTo(IdSupportCol);
					if (!MapAudLDPSupport.IsValid())
					{
						JBool OkCol = pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupportCol, VolLectSuppCol, AudLDPSuppCol, AudProbaSuppCol, PtEtal, true);
						if (OkCol && pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain))
							MapAudLDPSupport.Add(IdSupportCol) = AudLDPSuppCol; 
						else
							MapAudLDPSupport.Add(IdSupportCol) = -1.0;
					}
					else
						// R�cup�re r�sultat archiv�
						AudLDPSuppCol = MapAudLDPSupport.GetItem(); 
					
					// En + , v�rifie qu'il y a bien pour chacun des audiences					
					// if	((OkRow && AudLDPSuppRow >= 0) && (OkCol  && AudLDPSuppCol >= 0))
					if	(AudLDPSuppRow >= 0 && AudLDPSuppCol >= 0)
					{
						// Couplage associ�
						JMTFR03Couplage Couplage2Supp;

						// Init Audience du couplage
						JFlt64	AudienceCouplage = AudLDPSuppRow;

						// Pour la diagonale, on fait l'impasse des calculs complexes
						if ((NoLig - m_NbHeaderRow) != (NoCol - m_NbHeaderCol))
						{
							// R�cup�re les �lmts supports ligne et colonne
							const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
							IElem* pElemRow = CRegroupements::GetInstance()->GetIElemByID(KeyResultDup.m_IdSupportRow);
							IElem* pElemCol = CRegroupements::GetInstance()->GetIElemByID(KeyResultDup.m_IdSupportCol);

							// Liste global des id audience supports
							JMap<JUnt32, JUnt32> MapIdAudienceSup;

							// Recup id audience support ligne
							JUnt32 IdAudienceRow;
							JList<IElem *> ListSupRow;
							if (pElemRow->IsCouplage()) 
							{
								// R�cup�re composants du couplage
								pElemRow->AsCouplage()->GetFils(ListSupRow);

								//pour chaque support
								for(ListSupRow.MoveFirst() ; ListSupRow.IsValid(); ListSupRow.MoveNext())
								{
									IElem * pIElem = ListSupRow.GetItem();
									if(pIElem)
									{
										// Ident audience
										IdAudienceRow = pIElem->AsITitre()->GetIDAudience();

										// Ajout direct id audience support
										MapIdAudienceSup.Add(IdAudienceRow);
									}
								}
							}
							else
							{
								// Ident audience
								IdAudienceRow = pElemRow->AsITitre()->GetIDAudience();

								// Ajout direct id audience support
								MapIdAudienceSup.Add(IdAudienceRow);
							}

							// Recup id audience support colonne
							JUnt32 IdAudienceCol;
							JList<IElem *> ListSupCol;
							if (pElemCol->IsCouplage()) 
							{		
								// R�cup�re composants du couplage
								pElemCol->AsCouplage()->GetFils(ListSupCol);

								//pour chaque support
								for(ListSupCol.MoveFirst() ; ListSupCol.IsValid(); ListSupCol.MoveNext())
								{
									IElem * pIElem = ListSupCol.GetItem();
									if(pIElem)
									{
										// Ident audience
										IdAudienceCol = pIElem->AsITitre()->GetIDAudience();

										// Test si id audience support non d�j� pr�sent
										MapIdAudienceSup.MoveTo(IdAudienceCol);
										if (!MapIdAudienceSup.IsValid()) 
											MapIdAudienceSup.Add(IdAudienceCol);
									}
								}
							}
							else
							{
								// Ident audience
								IdAudienceCol = pElemCol->AsITitre()->GetIDAudience();

								// Test si id audience support non d�j� pr�sent
								MapIdAudienceSup.MoveTo(IdAudienceCol);
								if (!MapIdAudienceSup.IsValid()) 
									MapIdAudienceSup.Add(IdAudienceCol);
							}
								
							// Couplage ssi les au moins 2 supports composants
							if (MapIdAudienceSup.GetCount() > 1)
							{
								// Ajout id audience dans couplage
								for (MapIdAudienceSup.MoveFirst(); MapIdAudienceSup.IsValid(); MapIdAudienceSup.MoveNext())
								{
									Couplage2Supp.Add(MapIdAudienceSup.GetKey());
								}

								// Cr�er la table d'audience
								JMTFR03TBLAudiences TBLAudiences;

								// Ici Ajout dans table d'audience (TRES LONG !!!!!!!!!!)
								TBLAudiences.Add(KeyAudience, TblTerrains, TblModulations, TblCibles, Options, Couplage2Supp,0,false); 

								// On se positionne sur l'�lement info audience
								TBLAudiences.MoveTo(KeyAudience);

								// Test si �lmt valide
								// Version 2.7.1 Alain :: Correction comme pour Calcul CMoteurPresse::CalcAudienceCouplage (08/12/2010)
								if (TBLAudiences.IsValid())
								{
									// R�cup�re audience du couplage  (attention v�rifier audience r�f�rence
									const JMTFR03DATAudience &DATAudience = TBLAudiences.GetItem();
									if (DATAudience.m_AudienceRef.AsJFlt64() != 0)
										AudienceCouplage	= DATAudience.m_AudienceRef.AsJFlt64();
									else
										AudienceCouplage	= DATAudience.m_AudienceProba.AsJFlt64();
								}
							}
						}

						// Volume de contact totale
						JFlt64 VolTotLect = VolLectSuppRow;
						if (IdSupportRow != IdSupportCol)
							VolTotLect += VolLectSuppCol;

						// Stockage r�sultat
						CResultDup ResultDup;
						ResultDup.AudienceDup		= (AudLDPSuppRow + AudLDPSuppCol) - AudienceCouplage;

						// Cas limite o� r�sultat tr�s proche de 0
						if (ResultDup.AudienceDup <= 0)
						{
							ResultDup.AudienceDup = 0.0;
							ResultDup.Penetration = 0.0;
							ResultDup.CentrageH   = 0.0;
							ResultDup.CentrageV   = 0.0;
							ResultDup.CoeffYule   = -1;
						}
						else
						{	// P�n�tration
							ResultDup.Penetration	= (ResultDup.AudienceDup / Effectif)  * 100.0;

							// Calcul centrage H% et V%
							ResultDup.CentrageH		= CalculCentrage(ResultDup.AudienceDup, AudLDPSuppRow);
							ResultDup.CentrageV		= CalculCentrage(ResultDup.AudienceDup, AudLDPSuppCol);

							// Coefficient Yule
							ResultDup.CoeffYule		= CalculCoeffYule(ResultDup.AudienceDup,
																	AudLDPSuppRow,AudLDPSuppCol,
																	Effectif);
						}

						// Sauvegarde r�sultat
						pDocChoixSupport->m_MapResultatDup.MoveTo(KeyResultDup); 
						if (!pDocChoixSupport->m_MapResultatDup.IsValid())
						{
							pDocChoixSupport->m_MapResultatDup.Add(KeyResultDup) = ResultDup; 
						}

						// Ecriture r�sultat dans grid duplication
						Txt = ResultCellule(NoLig,NoCol,KeyResultDup, ResultDup);
						m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
						
						// Stocke r�sultat de l'autre partie de la diagonale
						KeyResultDup.m_IdCible			= IdCible; 
						KeyResultDup.m_IdTerrain		= IdTerrain;

						if (CalculSymetrique)
						{
							// Stockage de la partie sym�trique
							KeyResultDup.m_IdSupportRow		= IdSupportCol; 
							KeyResultDup.m_IdSupportCol	    = IdSupportRow;
						}
						else
						{
							// Si grandeur H% et V%, on stocke bien da l'autre c�t� de la diagonale
							KeyResultDup.m_IdSupportRow		= IdSupportRow; 
							KeyResultDup.m_IdSupportCol	    = IdSupportCol;
						}

						// Test si nouveau r�sultat
						pDocChoixSupport->m_MapResultatDup.MoveTo(KeyResultDup); 
						if (!pDocChoixSupport->m_MapResultatDup.IsValid())
						{
							// Calcul r�sultat centrage
							ResultDup.CentrageH = CalculCentrage(ResultDup.AudienceDup, AudLDPSuppCol);
							ResultDup.CentrageV = CalculCentrage(ResultDup.AudienceDup, AudLDPSuppRow);
							pDocChoixSupport->m_MapResultatDup.Add(KeyResultDup) = ResultDup; 
						}

						// Ecriture r�sultat invers� ligne/colonne via la diagonale grid duplication
						Txt = ResultCellule(NoLig,NoCol,KeyResultDup, ResultDup);
					}

					else
					{
						// Calcul non d�fini
						ResultDup.AudienceDup	= -1.0;
						ResultDup.Penetration	= -1.0;
						ResultDup.CentrageH		= -1.0;
						ResultDup.CentrageV		= -1.0;
						ResultDup.CoeffYule		= -1.0;
						
						// Sauvegarde r�sultat
						pDocChoixSupport->m_MapResultatDup.MoveTo(KeyResultDup); 
						if (!pDocChoixSupport->m_MapResultatDup.IsValid())
						{
							pDocChoixSupport->m_MapResultatDup.Add(KeyResultDup) = ResultDup; 
						}

						// Ecriture r�sultat dans grid duplication
						Txt = ResultCellule(NoLig,NoCol,KeyResultDup, ResultDup);
						m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
					}
				}

				// Recopie valeur sym�trique si calcul diff�rent de H% et V%
				if (CalculSymetrique)
				{
					// Ligne Sym�trique (non recalcul�e)
					JUnt32 NoLigSym = (NoCol - m_NbHeaderCol) + m_NbHeaderRow;

					// Colonne Sym�trique (non recalcul�e)
					JUnt32 NoColSym = (NoLig - m_NbHeaderRow) + m_NbHeaderCol;

					// Affiche r�sultat sym�trique (si grandeur <> H% et V%)
					m_GridDuplication.SetValueRange(CGXRange(NoLigSym, NoColSym),Txt); 
				}
			}
		}
	}
	
	// Progress bar invisible
	m_ProgressResultDup.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridDuplication.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                      m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										      m_GridDuplication.GetColCount()-1),
								    CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/formats/p�riodicit�s
	m_GridDuplication.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                        m_GridDuplication.GetRowCount(),m_NoColPeriodicite-1),
		                                   TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);
	
	// Retaille hauteur header lignes (pour titres supports)
	m_GridDuplication.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
			                                         0,m_GridDuplication.GetColCount()),
		                                    TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour du grid
	m_GridDuplication.LockUpdate(false);
	m_GridDuplication.Redraw();

	// Flag calcul effectu�
	pDocChoixSupport->m_ParamDupModif = false;

	// Test largeur colonne
	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridDuplication.GetColCount(); NoCol++)
	{
		int ColWidth = m_GridDuplication.GetColWidth(NoCol);
		int Stop = 1;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Calcul Centrage entre 2 titres
JFlt64 CDlgDuplication::CalculCentrage(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCible)
{
	if (AudienceCible > 0)
	{
		// Audience dupliqu� sur la cible courante
		return (AudienceDupliquee / AudienceCible) * 100.0;
	}	
	else 
		return -1.0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Calcul Coefficient de Yule
JFlt64 CDlgDuplication::CalculCoeffYule(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCibleRow, JFlt64 &AudienceCibleCol, JFlt64 &EffectifGlobal)
{

	// Individus appartenant au support en ligne mais pas au support en colonne
	JFlt64 NbIndivRowPure = AudienceCibleRow - AudienceDupliquee;		// c dans la formule

	// Individus appartenant au support en ligne mais pas au support en colonne
	JFlt64 NbIndivColPure = AudienceCibleCol - AudienceDupliquee;		// t dans la formule

	// Individus communs
	JFlt64 NbIndivCommun = AudienceDupliquee;							// r dans la formule

	// Individus ext�rieurs au 2 cibles
	JFlt64 NbIndivExterne = EffectifGlobal - (NbIndivRowPure + NbIndivColPure + NbIndivCommun);	// v dans la formule

	// Coefficient de Yule =  100 * (1 - ((1 + (rv - ct)/ (rv + ct))) / 2)) manque 1 parenthese
	if (NbIndivCommun * NbIndivExterne > 0)
	{
		return (100 * (1 - ((1 + (NbIndivCommun * NbIndivExterne - NbIndivRowPure * NbIndivColPure) / 
								 ((NbIndivCommun * NbIndivExterne) + (NbIndivRowPure * NbIndivColPure))) / 2)));
	}

	// Les autres cas
	return -1.0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage r�sultat duplication
JBool CDlgDuplication::AfficheResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridDuplication.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultDup KeyResultDup;

	// Information r�sultat archiv�
	CResultDup	ResultDup;

	// Delock mode read only
	m_GridDuplication.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridDuplication.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridDuplication.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultDup.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultDup.GetRange( nLower, nUpper);

	// Calcul indice cl� segment
	KeyResultDup.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleDup); 

	// Calcul indice cl� moteur
	KeyResultDup.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Init Indice colonne
	JUnt32 NoCol = m_NbHeaderCol;

	// Nombre de colonnes globales
	JUnt32 NbColTot = m_GridDuplication.GetColCount();

	// Tableau Id Suppor Colonne
	JArray <JUnt32> TIdColonne;
	TIdColonne.Reset(); 
	TIdColonne.SetCount(m_GridDuplication.GetColCount()); 

	// Sauve Id Support Colonne
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// Sauvegarde Id Colonne
		// TIdColonne.Item(NoCol-1) = pDocChoixSupport->m_MapIdSupportsSel.GetKey();
		TIdColonne.Item(NoCol-1) = pDocChoixSupport->m_Offre.m_TBLIdSupports.GetItem();

		// Passe � la colone suivante
		NoCol++;
	}

	// Sauve Id Famille Colonne
	for (pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveFirst();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.IsValid();
		 pDocChoixSupport->m_Offre.m_TBLIdSupports.MoveNext())
	{
		// A FAIRE - FAMILLES
	}	
	
	// Balayage ligne support
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultDup.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupportRow = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup�re le format support ligne s�lectionn� pour la cumulativit�
		JUnt32 IdFormatSupportRow = 0;
		pDocChoixSupport->m_MapIdFormatSpecifDup.MoveTo(IdSupportRow);
		if (pDocChoixSupport->m_MapIdFormatSpecifDup.IsValid())
			IdFormatSupportRow = pDocChoixSupport->m_MapIdFormatSpecifDup.GetItem();

		// Ecriture P�riodicit�
		JString TxtPeriodicite,TxtAbrPeriodicite;
		pDocChoixSupport->GetPeriodicite(IdSupportRow,TxtPeriodicite,TxtAbrPeriodicite);

		// Ajoute un espace � gauche pour pr�sentation grille
		CString TxtFormatAbrPeriod;
		TxtFormatAbrPeriod.Format(" %s", TxtAbrPeriodicite.AsJCharPtr());
		TxtAbrPeriodicite = TxtFormatAbrPeriod;

		m_GridDuplication.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPeriodicite.AsJCharPtr()); 

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

		// Boucle sur les colonnes supports
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridDuplication.GetColCount(); NoCol++) 
		{
			// Ident Support en colonne
			// JUnt32 IdSupportCol = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(m_NbHeaderRow + NoCol - m_NbHeaderCol, m_NoColIdSupport-1));
			JUnt32 IdSupportCol = TIdColonne.Item(NoCol-1);

			// A FAIRE 
			// Recherche R�sultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur r�sultats
			KeyResultDup.m_IdSupportRow = IdSupportRow;
			KeyResultDup.m_IdSupportCol = IdSupportCol;
			KeyResultDup.m_IdTerrain	= IdTerrain;
			KeyResultDup.m_IdCible		= IdCible;

			// V�rifie que le r�sultat existe
			pDocChoixSupport->m_MapResultatDup.MoveTo(KeyResultDup); 
			if (pDocChoixSupport->m_MapResultatDup.IsValid())
			{
				// Information r�sultat archiv�
				CResultDup	&ResultDup = pDocChoixSupport->m_MapResultatDup.GetItem();

				// Le r�sultat pour la cellule
				Txt = ResultCellule(NoLig,NoCol,KeyResultDup,ResultDup);
				m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}
			else
			{
				// R�sultats non d�termin�s
				Txt = "*";
				m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}
		}
	}

	// Progress bar invisible
	m_ProgressResultDup.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridDuplication.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                      m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										      m_GridDuplication.GetColCount()-1),
									 CGXStyle()
											.SetEnabled(false));

	// Retaille largeur colonnes supports/p�riodicit�
	m_GridDuplication.ResizeColWidthsToFit(CGXRange(0,m_NoColSupport-1,
			                                        m_GridDuplication.GetRowCount(),m_NoColPeriodicite-1),
		                                    TRUE,GX_UPDATENOW);
	
	// Retaille hauteur header lignes (pour titres supports)
	m_GridDuplication.ResizeRowHeightsToFit(CGXRange(0,m_NoColSupport-1,
			                                         0,m_GridDuplication.GetColCount()),
		                                    TRUE,GX_UPDATENOW);

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridDuplication.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour valide
	m_GridDuplication.LockUpdate(false);
	m_GridDuplication.Redraw();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// R�sultat cellule pour toutes les grandeurs duplication
CString CDlgDuplication::ResultCellule(JUnt32 NoLig, JUnt32 NoCol,
									   CKeyResultDup KeyResultDup, 
									   CResultDup    ResultDup)
{
	// Par d�faut Texte
	CGXStyle style;
	m_GridDuplication.SetStyleRange(CGXRange(NoLig,NoCol),
								     CGXStyle()
									   .SetFormat(GX_FMT_TEXT)
									   .SetValueType(GX_VT_STRING));
	
	// Chaine formatt�e par d�faut
	CString TxtResult = "*";

	// R�cup�re index grandeur
	JUnt32 IdGrandeur = 0;
	pDocChoixSupport->m_MapIdGrandDupSel.MoveFirst();
	if (pDocChoixSupport->m_MapIdGrandDupSel.IsValid())
		IdGrandeur = pDocChoixSupport->m_MapIdGrandDupSel.GetItem(); 

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		///////////////////////////// CALCUL AUDIENCE DUPLIQUEE LDP/LNM /////////////////////////////
		case pDocChoixSupport->GrdDup_Audience_LDPLNM: 
		{
			// Test si audience dupliqu�e LDP/LNM d�fini
			if (ResultDup.AudienceDup >= 0)
			{
				// Formattage du r�sultat
				TxtResult.Format("%0.f",ResultDup.AudienceDup);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		///////////////////// CALCUL AUDIENCE DUPLIQUEE LDP/LNM (000) //////////////////////////
		case pDocChoixSupport->GrdDup_Audience_LDPLNM_000: 
		{
			// Test si audience dupliqu�e LDP/LNM d�fini
			if (ResultDup.AudienceDup >= 0)
			{
				// Formatage du r�sultat
				TxtResult.Format("%0.f",ResultDup.AudienceDup/1000.0);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		///////////////////////////// CALCUL PENETRATION DUPLIQUEE /////////////////////////////
		case pDocChoixSupport->GrdDup_Penetration : 
		{
			// Test si p�n�tration d�finie
			if (ResultDup.Penetration >= 0)
			{
				// Formatage le r�sultat
				TxtResult.Format("%0.2f",ResultDup.Penetration);
	
				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);

				// Ajout suffixe %
				TxtResult += "%";
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		///////////////////////////// CALCUL CENTRAGE DUPLIQUEE HORZ /////////////////////////////
		case pDocChoixSupport->GrdDup_CentrageH : 
		{
			// Test si coefficient centrage d�fini
			if (ResultDup.CentrageH >= 0)
			{
				// Formatage le r�sultat
				TxtResult.Format("%0.2f",ResultDup.CentrageH);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);

				// Ajout suffixe %
				TxtResult += "%";
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		///////////////////////////// CALCUL CENTRAGE DUPLIQUEE VERT /////////////////////////////
		case pDocChoixSupport->GrdDup_CentrageV : 
		{
			// Test si coefficient centrage d�fini
			if (ResultDup.CentrageV >= 0)
			{
				// Formatage le r�sultat
				TxtResult.Format("%0.2f",ResultDup.CentrageV);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);

				// Ajout suffixe %
				TxtResult += "%";
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		///////////////////////////// CALCUL COEFFICIENT YULE DUPLIQUEE /////////////////////////////
		case pDocChoixSupport->GrdDup_CoeffYule : 
		{
			// Test si coefficient indice de proximit� d�fini
			if (ResultDup.CoeffYule >= 0)
			{
				// Formatage le r�sultat
				TxtResult.Format("%0.f",ResultDup.CoeffYule);

				// D�coupage chaine par milliers
				pDocChoixSupport->FormatString(TxtResult,3);
			}

			// Inscrit la valeur trouv�e
			m_GridDuplication.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
		}
		break;

		default:
		break;
	}
	
	// Renvoi la chaine format�e
	return TxtResult;
}

//////////////////////////////////////////////////////////////////////////////////////
// D�finit si la grille r�sultat est valide
bool CDlgDuplication::GrilleResultValid()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount() > 0 )
	{
		// La grille est valide car au moins 1 support
		return true;
	}

	// Pas de grille r�sultat d�finie
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de lignes header
JUnt32 CDlgDuplication::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de colonnes header
JUnt32 CDlgDuplication::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Affiche barre de calcul
void CDlgDuplication::AffichBarreCalcul()
{
	// A FAIRE
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export des r�sultats Duplication
void CDlgDuplication :: ExportResultat()
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
		
		// 2eme bloc : R�sultats
		TxtEntete = "BLOC 2\t";

		// Type de r�sultat (grandeur s�lectionn�e)
		CString TxtResultat = "";
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandDupSel.IsValid())
		{
			// Une grandeur a bien �t� s�lectionn�e
			TxtResultat = pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleDup,pDocChoixSupport->m_MapIdGrandDupSel.GetItem());  
		}
		TxtEntete += TxtResultat + "\n";
		ExportFile.WriteString(TxtEntete);

		// Ex Copie avant de passer En StdioFile 
		// Copie des infos tableurs dans fichier export en 2 passes
		// m_GridDuplication.CopyTextToFile( ExportFile, CGXRange(m_NbHeaderRow-1, m_NoColSelSupport-1, m_NbHeaderRow-1, m_GridDuplication.GetColCount()));
		// Copie des infos tableurs dans fichier export
		// Les r�sultats du grille (ATTENTION OD OD OA avec CStdioFile)
		// m_GridDuplication.CopyTextToFile( ExportFile,CGXRange(m_NbHeaderRow, m_NoColSelSupport-1, m_GridDuplication.GetRowCount(), m_GridDuplication.GetColCount()));
		for (int lig = m_NbHeaderRow-1; lig <= m_GridDuplication.GetRowCount(); lig++)
		{
			for (int col = m_NoColSelSupport-1; col <= m_GridDuplication.GetColCount(); col++)
			{
				CString Txt = m_GridDuplication.GetValueRowCol(lig, col) + "\t";
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'ent�te pour export duplication
void CDlgDuplication :: RecupEnteteExport(CString &Entete)
{
	// N� Version
	Entete  = "No de version :\t";
	Entete += "1.0\n";

	// Nom du plateau
	Entete += "NOM PLATEAU :\t";
	Entete += pDocChoixSupport->m_KEYPlateau.GetLabel() + "\n";

	// Init entete
	Entete += "EXPORT DUPLICATION \n\n";

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
	for (JUnt32 NoLig = 0; NoLig <= m_GridDuplication.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridDuplication.GetValueRowCol(NoLig, m_NoColIdSupport-1));

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

	// Boucle sur les �lmts segment audience s�lectionn�s pour duplication
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

	// Pr�voir sauts de ligne avant affichage infos tableurs
	Entete += "\n\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration r�sultats duplication d�taill�e
void CDlgDuplication ::RecupGrilleExport(CString &Entete)
{
	// Init chaine r�sultat
	Entete = "";

	// Tableau de lignes et colonnes s�lectionn�es
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridDuplication.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridDuplication.GetSelectedRows(Rows,false,false);
	
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
				Txt = m_GridDuplication.GetValueRowCol(nRow,nCol); 
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
				Txt = m_GridDuplication.GetValueRowCol(nRow,NoCol); 
				Entete += Txt;
				Entete += "\t";
			}
		}

		// Saut apr�s chaque ligne header grille
		Entete += "\n";
	}

	// Balayage des lignes s�lectionn�es
	for (int nRow = 0; nRow < Rows.GetCount(); nRow++)
	{
		// No de la ligne support
		int NoRow = Rows.GetAt(nRow);

		// R�cup�re ident support
		JUnt32 IdSupport = atoi(m_GridDuplication.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Ecriture infos g�n�rales supports
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColPeriodicite; nCol++)
		{	
			// Libell� support, ou p�riodicit� ou tarif)
			Txt = m_GridDuplication.GetValueRowCol(NoRow,nCol); 
			/*
			if (nCol != m_NoColFormat-1)
				// Libell� support, ou p�riodicit� ou tarif)
				Txt = m_GridDuplication.GetValueRowCol(NoRow,nCol); 
			else
			{
				// R�cup�re le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifDup.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifDup.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifDup.GetItem();

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
			Txt = m_GridDuplication.GetValueRowCol(NoRow,NoCol); 
			Entete += Txt +  "\t";
		}

		// Saut apr�s chaque ligne support
		Entete += "\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du bloc infos terrains / cibles
void CDlgDuplication::RecupBlocTerrainCible(CString &Entete)
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
// Clipboard du grille duplication
bool CDlgDuplication::Clipboard()
{
	// S�lection globale du grid
	POSITION area = m_GridDuplication.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridDuplication.SetSelection(area, 0, 0, m_GridDuplication.GetRowCount(), m_GridDuplication.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridDuplication.Copy(); 

	// D�selection globale
	m_GridDuplication.SetSelection(NULL, 0, 0, 0, 0);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////
// Impression Excel
bool CDlgDuplication ::PrintExcel()
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

///////////////////////////////////////////////////////////////////////////////////////
// Impression Excel
bool CDlgDuplication ::OpenExcel()
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

///////////////////////////////////////////////////////////////////////////////////////
// Sauve en format fichier excel
bool CDlgDuplication ::SaveAsExcel(CString filePath)
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
