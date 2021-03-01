// DlgEchelle.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgEchelle.h"
#include "KeyMoteur.h"
#include "DlgEchelle.h"
#include "ExcelPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// Boîte de dialogue CDlgEchelle
IMPLEMENT_DYNAMIC(CDlgEchelle, JFCDialog)

CDlgEchelle::CDlgEchelle(CWnd* pParent /*=NULL*/,
						 JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgEchelle::IDD, pParent)
{
	// Récupère pointeur sélection générale
	pDocChoixSupport = pDoc;

	// Init Lignes/Colonnes Grid
	m_NbHeaderRow  = 3;
	m_NbRowSupport = 0;
	m_NbHeaderCol  = 1;
	m_NbCol        = 0;

	// Init nombre de ligne info supplémentaire
	m_NbRowInfo    = 1;
}

CDlgEchelle::~CDlgEchelle()
{
}

void CDlgEchelle::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_CADRE_ECH, m_CadreEchelle);
	DDX_Control(pDX, IDC_CS_TITRE_ECHELLE, m_TitreEchelle);
	DDX_Control(pDX, IDC_CS_PROGRESS_RESULTECH, m_ProgressResultEch);
}


BEGIN_MESSAGE_MAP(CDlgEchelle, JFCDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation fenêtre Echelle
BOOL CDlgEchelle::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fenêtre Echelles
	SetupControls();

	// 1 / Informations générales  (Source - Filtre d'audience - Date tarification etc...)
	UpdateTitreAnalyse();

	// Mise en place des relations objets fenêtre Echelles
	SetWindowRelations();

	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fenêtre Echelle
void CDlgEchelle::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
			// Cadre Echelle
			{IDC_CS_CADRE_ECH,			{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side

			// Titre Echelle (Source + Source Audience)
			{IDC_CS_TITRE_ECHELLE,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},			//Bottom side

			// Grille Résultat Echelle
			{IDC_CS_GRID_ECHELLE,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side

			// Barre rpgression Résultat Echelle
			{IDC_CS_PROGRESS_RESULTECH, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contrôles fenêtre Echelle
void CDlgEchelle::SetupControls()
{
	// Association objet / ressource
	m_GridEchelle.SubclassDlgItem(IDC_CS_GRID_ECHELLE, this);
	
	// Propriétés du cadre Echelle
	m_CadreEchelle.SetTransparent(false);
	m_CadreEchelle.SetSunken(true);

	// Controles en avant plan
	m_CadreEchelle.AddTopWindow(m_TitreEchelle);
	m_CadreEchelle.AddTopWindow(m_GridEchelle); 
	m_CadreEchelle.AddTopWindow(m_ProgressResultEch); 

	// Progress bar caché par défaut
	m_ProgressResultEch.ShowWindow(SW_HIDE); 
	m_ProgressResultEch.SetShowText(false);

	// Taille et Font des différents labels
	m_TitreEchelle.SetFontName(CATPColors::GetFontLabelName());
    m_TitreEchelle.SetFontBold(TRUE); 
	m_TitreEchelle.SetFontSize(CATPColors::GetFontLabelSize());
	
	// Initialise grid résultat
	m_GridEchelle.EnableMouseWheel(); 

	m_GridEchelle.Initialize();
	GXSetNewGridLineMode(); 

	m_GridEchelle.GetParam()->EnableUndo(FALSE);
	m_GridEchelle.SetMergeCellsMode(gxnMergeDelayEval,false);
	m_GridEchelle.GetParam()->SetNumberedColHeaders(false); 
	m_GridEchelle.GetParam()->EnableMoveCols(FALSE);
	m_GridEchelle.GetParam()->SetMinTrackColWidth(16);
	m_GridEchelle.GetParam()->SetNumberedRowHeaders(false); 
	m_GridEchelle.GetParam()->EnableMoveRows(FALSE);
	m_GridEchelle.GetParam()->SetMinTrackRowHeight(16);
	m_GridEchelle.GetParam()->SetSmartResize(true);
	
	m_GridEchelle.GetParam()->SetLockReadOnly(true); 

	// en + 
	m_GridEchelle.GetParam()->GetProperties()->SetMarkColHeader(FALSE);
	
	// Enregistrement controle spécifique combo box pour les formats
	CGXComboBoxWnd* pWnd = new CGXComboBoxWnd(& m_GridEchelle);
	pWnd->Create(WS_VSCROLL | CBS_DROPDOWNLIST, IDS_CS_CTRL_INDEXDROPDOWNLIST);
	pWnd->m_bFillWithChoiceList = TRUE;
	pWnd->m_bWantArrowKeys		= FALSE;
	pWnd->m_nIndexValue			= 0;
	pWnd->m_bDispChoice			= TRUE;
	m_GridEchelle.RegisterControl(IDS_CS_CTRL_INDEXDROPDOWNLIST, pWnd);
	
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Init Référence colonne
	InitRefGrid();

	// Envoi les infos générales au grille de base
	if (m_GridEchelle.OnInitDocument(pDocChoixSupport, this, m_NoColIdSupport) == false)
		AfxMessageBox ("Problème association document supports / grille échelle");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid résultat
	if (GrilleResultValid())
		InitGridResultat();

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des références colonnes
void CDlgEchelle::InitRefGrid()
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

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fenêtre Echelle
void CDlgEchelle::SetColors()
{
	// Cadre résultat échelle
	m_CadreEchelle.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultEch.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Titre échelle
	m_TitreEchelle.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreEchelle.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Grille résultat
	m_GridEchelle.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridEchelle.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Barre de progression
	m_ProgressResultEch.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));

	// Les différentes couleurs pour headers ou cellules spécials
	UpdateColorSpe();

	// Style des lignes (solid pen)
	m_GridEchelle.GetParam()->SetGridLineStyle(PS_SOLID); 

	// Style sélection bloc (comme Excel)
	// m_GridEchelle.GetParam()->SetExcelLikeSelectionFrame(TRUE);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de la fenêtre Echelles
void CDlgEchelle::OnDestroy()
{
	JFCDialog::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Resize la fenêtre Echelles
void CDlgEchelle::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la fenêtre Echelles
void CDlgEchelle::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		// Mise à jour du Grid résultat Echelles
		// AfxMessageBox("A FAIRE / GRID RESULTAT",MB_ICONINFORMATION);	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void CDlgEchelle::OnCancel()
{
	JFCDialog::OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du grid
void CDlgEchelle::InitGrid()
{
	m_GridEchelle.LockUpdate(true);

	// Enleve n° ligne col 
	m_GridEchelle.GetParam()->SetNumberedColHeaders(false);
	m_GridEchelle.GetParam()->SetNumberedRowHeaders(false);

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
		UpdateRowHeader();

		// Delock mode read only
		m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

		// Lignes non resizable et nb lignes headers (3 : Terrains / Cibles / Grandeurs)
		m_GridEchelle.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);
		
		// Aucun header colonne et colonnes bloqués (support/périodicité/tarif)
		m_GridEchelle.SetFrozenCols(m_NoColTarif - m_NoColIdSupport ,0);
		m_GridEchelle.HideCols(m_NoColIdSupport-1,m_NoColIdSupport-1); 
		// m_GridEchelle.HideCols(m_NoColFormat-1,m_NoColFormat-1); 

		// Affichage entete Tarif et périodicité
		if (m_GridEchelle.GetRowCount() > 2) 
		{

			// Titre "Sél SUPPORTS"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("X")));

			// Titre "SUPPORTS"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColSupport-1),
										CGXStyle()
											.SetValue(_T("Supports")));

			// Titre "FORMATS"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColFormat-1),
										CGXStyle()
											.SetValue(_T("Formats")));

			// Titre "TARIF"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColTarif-1),
										CGXStyle()
											.SetValue(_T("Tarif €")));

			// Titre "PERIODICITE"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColPeriodicite-1),
										CGXStyle()
											.SetValue(_T("Périodic.")));

			// Case sélection / déselection tot supports
			m_GridEchelle.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("T")));
		}

		// Mise à jour affichage moyenne (visu ou pas visu)
		UpdateAfficheMoyenne();

		// Merge des cellules définitivement vide
		m_GridEchelle.UpdateData(); 

		m_GridEchelle.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,m_NbHeaderRow-2,m_NoColTarif-1),
		 							CGXStyle()
		 							.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));



		m_GridEchelle.UpdateWindow(); 

		// On bloque les colonnes qui doivent ne pas être modifiées
		m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Rend visible le grid, si au moins quelques résultats
	if (m_GridEchelle.GetColCount() >= m_NoColTarif)
	{
		m_GridEchelle.ShowWindow(SW_SHOW);
	}
	else
		m_GridEchelle.ShowWindow(SW_HIDE);

	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	
}

////////////////////////////////////////////////////////////////////////////////////////////
// Init lignes supports
void CDlgEchelle::InitRowSupports()
{
	// Lockage des modifs
    m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Récupère objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Index ligne support
	JUnt32 InxLigSupp = m_NbHeaderRow;

	// Nb lignes et nb colonnes du grid
	int NbCol = m_GridEchelle.GetColCount();
	int NbRow = m_GridEchelle.GetRowCount();

	// Nombre de supports (supports + ligne d'infos)
	JUnt32 NbSupports = pDocChoixSupport->m_MapIdSupportsSel.GetCount(); //  + m_NbRowInfo;

	if (m_NbHeaderCol >= m_NoColTarif && NbSupports > 0)
	{
		// Indique Id Support = 0 pour lignes infos
		/*
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp + NbSupports - 1,m_NoColIdSupport - 1),
									CGXStyle()
											.SetValue(JUnt32(0)));
		*/
		// Pas de check-box pour les lignes infos)
		/*
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp + NbSupports - 1,
		m_NoColSelSupport - 1,
		InxLigSupp + NbSupports - 1,
		m_NoColSelSupport - 1),
		CGXStyle()
		.SetValue("")
		.SetControl(GX_IDS_CTRL_STATIC));
		*/
		/*
		// Pas de format pour ligne infos supports
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp + NbSupports - 1,m_NoColFormat - 1),
			CGXStyle()
			.SetValue(""));
		*/

		// Indique Id Support = 0 pour lignes infos
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp ,m_NoColIdSupport - 1),
								    CGXStyle()
											.SetValue(JUnt32(0)));

		// Pas de check-box pour les lignes infos)
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColSelSupport - 1,
											 InxLigSupp,
											 m_NoColSelSupport - 1),
									CGXStyle()
											.SetValue("")
											.SetControl(GX_IDS_CTRL_STATIC));

		// Pas de format pour ligne infos supports
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat - 1),
									CGXStyle()
											.SetValue(""));

		// Style texte libellés tarif
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColTarif-1,
											 InxLigSupp,
											 m_NoColTarif-1),
									CGXStyle()
										.SetHorizontalAlignment(DT_RIGHT)
										.SetVerticalAlignment(DT_VCENTER)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));


		// Passe aux supports
		InxLigSupp++;


		// Style check-box libellés supports (attention pas les lignes infos)
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColSelSupport - 1,
											 // InxLigSupp + NbSupports - 2,
											 InxLigSupp + NbSupports - 1,
											 m_NoColSelSupport - 1),
									CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

		
		// Style texte libellés supports avec init
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
			                                 m_NoColSupport - 1,
			                                 InxLigSupp + NbSupports - 1,
											 m_NoColSupport - 1),
									CGXStyle()
										.SetHorizontalAlignment(DT_LEFT)
										.SetVerticalAlignment(DT_VCENTER)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontHeader)
										.SetBold(pDocChoixSupport->m_BoldFontHeader)));
	
		// Style texte libellés périodicité avec init
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
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
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
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
				IElem* pSupport   = pRegroupements->GetIElemByID(IdSupport);

				// Indique Id Support dans colonne invisible
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdSupport));

				// on récupére le libellé support
				JString LibSupport = "-";

				// Affichage libellé support
				LibSupport = pSupport->GetLabel().AsJCharPtr();

				// Affichage libellé support
				CString LibSupp = LibSupport.AsJCharPtr();

				// voir info suppl. si quotidien
				// if (!pSupport->AsCouplage() && !pSupport->AsLot()) 
				if (!pSupport->AsCouplage()) 
				{
					if(pSupport->AsITitre())
					{
						if(pSupport->AsITitre()->GetPeriodicite() == 1)
							pDocChoixSupport->m_bLDPQtd ? LibSupp += " (LDP)" : LibSupp += " (LNM)";
					}
				}
				
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
											CGXStyle()
												.SetValue(LibSupp));

				// Récupère info formats support (avec espace à droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;
  				pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libellé format du support
				if (InxSelFormat >= 0)
					// Au moins 1 format possible
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
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
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetValue(LibFormatNull),
													gxOverride,0,GX_SMART);
													// gxApplyNew);
													// gxExclude,0,GX_SMART);
				}

				// Puis indication support sélectionné ou pas
				if (EtatSupport)
				{
					// Valide sélection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// Dévalide la sélection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incrémente index ligne
				InxLigSupp++;
			}
		}

		// Balaye toutes les lignes associées aux familles
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
				JBool EtatFamille = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

				// Element famille
				IElem* pFamille = pRegroupements->GetIElemByID(IdFamille);

				// Indique Id Famille dans colonne invisible
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdFamille));

				// on récupére le libellé famille
				JString LibFamille;
				LibFamille = pFamille->GetLabel().AsJCharPtr();

				// Affichage libellé famille
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
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
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
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
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetValue(LibFormatNull),
													gxOverride,0,GX_SMART);
													// gxApplyNew);
													// gxExclude,0,GX_SMART);
				}
				
				/*
				// Affichage libellé format de la famille > indique non défini
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColFormat-1),
											CGXStyle()
												.SetHorizontalAlignment(DT_LEFT)
												.SetVerticalAlignment(DT_VCENTER)
												.SetFont(CGXFont()
													.SetSize(pDocChoixSupport->m_SizeFontHeader)
													.SetBold(pDocChoixSupport->m_BoldFontHeader))
												.SetValue(LibFormatNull),
												gxOverride,0,GX_SMART);
												// gxApplyNew);
												// gxExclude,0,GX_SMART);
				*/

				// Puis indication famille sélectionné ou pas
				if (EtatFamille)
				{
					// Valide sélection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// Dévalide la sélection famille
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incrémente index ligne
				InxLigSupp++;
			}
		}

		// Colonne Id Support invisible
		m_GridEchelle.SetColWidth(m_NoColIdSupport-1,m_NoColIdSupport-1,0);

		// Retaille la colonne textes supports
		m_GridEchelle.SetColWidth(m_NoColSelSupport-1,m_NoColSelSupport-1,20);

		// Retaille la colonne périodicité et tarif
		m_GridEchelle.SetColWidth(m_NoColPeriodicite-1,m_NoColTarif-1,60);

		// Retaille supports via max libellé
		/*
		m_GridEchelle.ResizeColWidthsToFit(CGXRange(m_NbHeaderRow, m_NoColSupport-1, m_GridEchelle.GetRowCount(), m_NoColSupport-1),
 				                               TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);
		*/
		m_GridEchelle.SetColWidth(m_NoColSupport-1,m_NoColSupport-1,100);

		// Mise à jour couleur sélection supports
		UpdateVisuSupports();
	}
	
	// Mise à jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas être modifiées
 	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resize des colonnes
void CDlgEchelle::ResizeColonneFixe()
{
	// Retaille	cellules sauf si largeur = 0 (pour colonne rang invisible)
	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
	{
		if (m_GridEchelle.GetColWidth(NoCol) != 0) 
		{
			// puis les colonnes chiffrées
			m_GridEchelle.ResizeColWidthsToFit(CGXRange(m_NbHeaderRow,NoCol,m_GridEchelle.GetRowCount(),NoCol),
 				                               TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);
		}	

	}

	// Mise à jour grille
	m_GridEchelle.UpdateWindow(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour affichage lignes avec infos supplémentaires
void CDlgEchelle::UpdateRowInfo()
{
	// Lockage des modifs
    m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	////////////////////////////////////////////////////////////////////
	////////////// A FAIRE .............................................
	////////////////////////////////////////////////////////////////////

	// Mise à jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs spéciales de certaines cellules
void CDlgEchelle::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne résultat
	// Cellules info insertion en fond blanc et couleur caractère différent
	if (m_GridEchelle.GetColCount() >= m_NoColTarif)
	{
		m_GridEchelle.LockUpdate(true); 

		m_GridEchelle.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 1, m_NoColTarif-1),
									CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Delock mode read only
		m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

		// Niveau header de couleurs différents (via dlg ordre entete)
		// Niveau 1
		m_GridEchelle.SetStyleRange(CGXRange(0, m_NoColTarif, 0, m_GridEchelle.GetColCount()),
									CGXStyle()
										.SetInterior(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
		// Niveau 2
		m_GridEchelle.SetStyleRange(CGXRange(1, m_NoColTarif, 1, m_GridEchelle.GetColCount()),
									CGXStyle()
										.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
		// Niveau 3
		m_GridEchelle.SetStyleRange(CGXRange(2, m_NoColTarif, 2, m_GridEchelle.GetColCount()),
									CGXStyle()
										.SetInterior(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS)));

		//////////////////////////////////  A FAIRE //////////
		for (JUnt32 Lig = 0; Lig < m_NbHeaderRow ; Lig++)
		{
			for (JUnt32 Col = m_NbHeaderCol; Col <= m_GridEchelle.GetColCount(); Col++) 
			{
				// Si data exogène -- couleur différente
				// Récupère identificateur sélection grandeur
				JUnt32 IdGrandeur = RecupIdGrandeur(Col,0);

				if (IdGrandeur % 1000 >= 100)
				{
					// Data Exogène en + foncé
					m_GridEchelle.SetStyleRange(CGXRange(Lig, Col, Lig, Col),
												CGXStyle()
												.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));
				}
				else
				{
					// Les autres data en light
					m_GridEchelle.SetStyleRange(CGXRange(Lig, Col, Lig, Col),
												CGXStyle()
												.SetTextColor(CATPColors::GetColorBlack()));
				}

				m_GridEchelle.UpdateWindow(); 
			}
		}

		//////////////////////////////////////////////////////


		// Delock mode read only
		m_GridEchelle.GetParam()->SetLockReadOnly(TRUE);

		/*
		m_GridEchelle.LockUpdate(false); 
		m_GridEchelle.Redraw();
		*/



	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélection supports
void CDlgEchelle::UpdateEtatSelSupports()
{
	// Autorise modif grid et bloque mise à jour
	//m_GridEchelle.SetReadOnly(false);
	m_GridEchelle.LockUpdate(true); 

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat sélection en cours
			CGXStyle Style;
			m_GridEchelle.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 
		
			// Puis indication support sélectionné ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la sélection support
				if (ValSel != 1)
				{
					m_GridEchelle.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
			}
			else
			{
				// Dévalide la sélection support
				if (ValSel != 0)
				{
					// Dévalide la sélection support
					m_GridEchelle.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}
			}
		}
	}
	
	// Mise à jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgEchelle::UpdateVisuSupports()
{
	// Debut et fin marquage support sélectionné
	JUnt32 NoColDeb = m_NoColSupport-1;    
	JUnt32 NoColFin = m_GridEchelle.GetColCount();  // avant m_NoColTarif-1

	// Autorise modif grid
	//m_GridEchelle.SetReadOnly(false);

	// Lock mise à jour grid échelle
	m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup élmt
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Spécial ligne infos (en jaune)
		if (IdSupport == 0)
		{
			// Support non sélectionné, on laisse la ligne en blanc
			m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  										CGXStyle()
										.SetInterior(CATPColors::GetColorMoyenne(CATPColors::COLORCHOIXSUPPORTS)));
		}
		else
		{
			// Positionne sur info map id support
			pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

			if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
			{
				// Puis indication support sélectionné ou pas
				if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
				{
					// Support sélectionné, on surligne la ligne
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
				}
				else
				{
					// Support non sélectionné, on laisse la ligne en blanc
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
				}

				// Différencier les familles (soit couleur, soit font)
				if (pElem->IsLot())
				{
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorBeige()));
					/* voir en + en italique 
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
                                                	.SetInterior(CATPColors::GetColorBeige())
													.SetFont(CGXFont()
														.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));
					*/
				}
			}
		}
	}

	// Retaillage des colonnes fixes
	// ResizeColonneFixe();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage libellé moyenne et ligne moyenne apparente ou non
JVoid CDlgEchelle::UpdateAfficheMoyenne()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Update Résultat
	m_GridEchelle.UpdateData();

	// Index ligne moyenne
	JUnt32 InxLgMoy= 0;

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if (IdSupport == 0)
		{	
			// Récupére index ligne moyenne
			InxLgMoy = NoLig;
		}
		else
		{
			// Attention si support non visible
			JBool EtatVisible = (pDocChoixSupport->GetSupportVisible(IdSupport) && pDocChoixSupport->VisibleSupportMode(IdSupport)); 

			// Récupération validité support sur terrain(s)/segment(s)
			JBool ValideSupport = true;
			if (pElem->IsTitre() || pElem->IsCouplage())
				pDocChoixSupport->ValideSupportSegments(IdSupport);

			// Vérifie élment valide via le mode sélectionnée (mode Supports et/ou mode Familles)
			JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

			// Remet toutes les autres lignes "visibles" à la même hauteur
			if (EtatVisible && ValideSupport && VisibleSupport)
				// m_GridEchelle.SetRowHeight(NoLig+1, NoLig+1 ,16); 
				m_GridEchelle.SetRowHeight(NoLig, NoLig ,16); 
			else
				// m_GridEchelle.SetRowHeight(NoLig+1, NoLig+1 ,0); 
				m_GridEchelle.SetRowHeight(NoLig, NoLig ,0); 
		}
	}

	// Vérifie que la grandeur définit pour la moyenne est bien dans la sélection des grandeurs
	if (InxLgMoy >= m_NbHeaderRow)
	{
		// Ident grandeur associé à la moyenne
		JInt32 IdGrdMoyenne = pDocChoixSupport->GetIdGrandeurMoyenne(pDocChoixSupport->ModuleEch);

		// Définition du texte asocié à la moyenne (= Moyenne + Libellé grandeur tri courant)
		CString LibMoyenne = "";

		// Si pas de grandeur sélectionnée, ligne moyenne non visible
		/*
		if (IdGrdMoyenne >= 0 
			&&	pDocChoixSupport->IdGrandeurMoyenneValid(pDocChoixSupport->ModuleEch, IdGrdMoyenne)
			&&  pDocChoixSupport->m_AFFMoyenneResultatEch)
		{
			// Grandeur tri défini, on affiche la moyenne
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,16); 
			
			// Récup libellé grandeur associé
			LibMoyenne.Format("%s %s","Moyenne : ",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrdMoyenne % 1000)); 
		}
		else
		{
			// Pas de grandeur défini, la moyenne disparait
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,0); 
		}
		*/

		if (pDocChoixSupport->m_AFFMoyenneResultatEch)
		{
			// Grandeur tri défini, on affiche la moyenne
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,16); 

			// Récup libellé grandeur associé
			LibMoyenne.Format("%s","Moyenne"); 

			m_GridEchelle.SetFrozenRows(m_NbHeaderRow, m_NbHeaderRow);

		}
		else
		{
			// Pas de grandeur défini, la moyenne disparait
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,0); 

			m_GridEchelle.SetFrozenRows(m_NbHeaderRow-1, m_NbHeaderRow-1);
		}

		// Affichage du texte moyenne résultats
		m_GridEchelle.SetValueRange(CGXRange(InxLgMoy,m_NoColSupport-1),LibMoyenne); 

		// Mise à jour des données
		m_GridEchelle.UpdateData();

		// Mise à jour grille échelle
		// m_GridEchelle.UpdateWindow();

		// Mise à jour affichage
		//m_GridEchelle.LockUpdate(false);
		// m_GridEchelle.Redraw();
	}

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgEchelle::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Update Résultat
	m_GridEchelle.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Récup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Récupère état visibilité support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// Récupération validité support sur terrain(s)/segment(s)
        JBool ValideSupport = true;
		if (IdSupport != 0)
		{
			if (pElem->IsTitre() || pElem->IsCouplage())
				pDocChoixSupport->ValideSupportSegments(IdSupport);
		}

		// Vérifie élment valide via le mode sélectionnée (mode Supports et/ou mode Familles)
        JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

		if (EtatVisible == false || IdSupport == 0 || ValideSupport == false || VisibleSupport == false)
		{	
			// Cache la ligne support
			m_GridEchelle.SetRowHeight(NoLig, NoLig ,0); 
		}
		else
			// Rend visible la ligne support
			m_GridEchelle.SetRowHeight(NoLig, NoLig ,16); 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation du résultat échelle (VOIR OPTIM)
JBool CDlgEchelle::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise à jour
	m_GridEchelle.LockUpdate(true);

	// Clé de recherche résultat
	CKeyMoteur KeyMoteur;

	// Clé archivage résultat
	CKeyResultEch KeyResultEch;

	// Information résultat archivé
	CResultEch	ResultEch;

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
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridEchelle.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridEchelle.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultEch.ShowWindow(SW_SHOW); 

	// Récup Bornes pour barre de défilement
	int nLower, nUpper;
	m_ProgressResultEch.GetRange( nLower, nUpper);

	// Clef affinité courante
	KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice clé segment
	KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

	// Calcul indice clé moteur
	KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Récupére map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

	JDate DateExact;

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Par défaut c'est un support
		JBool ElmtFamille = false;

		// Avance progress bar
		m_ProgressResultEch.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention si c'est une famille
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un élmt famille
			ElmtFamille = true;
		}
		
		// Récupère le format support
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			// Tarif du support
			JUnt32 Duree;
			// JDate DateExact;

			/*
			if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
				Tarif = 0;
			*/
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

			// Si c'est une famille, calculer le tarif correspondant
			if (ElmtFamille)
			{
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
					                                      pDocChoixSupport->m_DateTarifaire, DateExact,
														  Duree, MapSuppSegTerr, TarifManuel);
			}

			// On n'oublie pas de tenir compte du nombre d'insertions
			Tarif *= pDocChoixSupport->m_NbInsertions;
		}

		// Ecriture du tarif
		if (Tarif > 0)
		{
			// Recup tarif format chaine avec espace milliers
			CString TxtTarif;
			TxtTarif.Format("%d",Tarif);
			pDocChoixSupport->FormatString(TxtTarif,3);

			if (!TarifManuel)
			{
				// Tarif bien défini
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien défini (modif la couleur texte)
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else
			// Tarif non défini
			m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture Périodicité
		JString TxtPeriodicite		= "";
		JString TxtAbrPeriodicite	= "";
		if (IdSupport)
			pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Si c'est une famille, il faut que tous les supports de même périodcité, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de périodicité abrégé (pour présentation tableau à droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

		// Balayage des colonnes
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
		{
			// Recup Id terrain
			JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

			// Recup Id Cible
			JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

			// Clé de recherche résultat
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Test si élément déjà calculé ou élmt moyenné
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
			if (ElmtFamille || !pDocChoixSupport->m_MapResultatEch.IsValid() || KeyResultEch.m_IdSupport == 0)
			{
				// Pas de calcul pour le moment si famille avec calcul moyenne
				if (!ElmtFamille || pDocChoixSupport->m_CumulFamille == true)
				{
					// Positionnement du moteur
					KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,pDocChoixSupport->m_Offre.m_IdItemAffinite);

					// Construction de la maille avec n insertions
					JFCMaille Maille;
					Maille.Reset(); 
					pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maille,IdSupport,pDocChoixSupport->m_NbInsertions); 
					// pDocChoixSupport->m_MoteurPresse.SetMailleTemporelle(KeyMoteur, Maille,IdSupport,pDocChoixSupport->m_NbInsertions,DateExact); 

					// Positionnement moteur pour calcul élmts affinité
					JFlt64 EffectifAffinite			= -1.0;
					JFlt64 VolAffinite				= -1.0;
					JFlt64 AudienceAffiniteLDP		= -1.0;
					JFlt64 AudienceAffiniteProba	= -1.0;
					JInt32 PtEtal					= -1.0;
					EffectifAffinite = pDocChoixSupport->m_MoteurPresse.GetEffectif(KeyMoteur);

					// Calcul Auience
					if (pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur, IdSupport, VolAffinite, AudienceAffiniteLDP,AudienceAffiniteProba,PtEtal, true))
					{
						// Tenir compte du nombre d'insertions
						VolAffinite *= pDocChoixSupport->m_NbInsertions;
						// AudienceAffiniteLDP *= pDocChoixSupport->m_NbInsertions;
					}
					// Audience affinité
					JFlt64 AudienceAffinite = -1;

					// Init Moteur (construction distribution)
					// Voir Jeremy avant on n'était pas obligé de tester VolAffinite
					if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille) && VolAffinite >= 0)
					{
						// Création Distribution via cible affinité
						JFCDistribution DistriAff = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolAffinite); 
					
						// Récupère audience
						AudienceAffinite = DistriAff.Item(1).AsJFlt64();  
					}

					// Mise en place des infos offres
					KeyMoteur.Set(pDocChoixSupport->GetIdSrcAudienceSel(),IdTerrain,IdCible);

					// Refaire Maille avec cible
					Maille.Reset(); 

					pDocChoixSupport->m_MoteurPresse.SetMailleEchelle(KeyMoteur, Maille,IdSupport,pDocChoixSupport->m_NbInsertions); 

					// Effectif et nb cas
					JFlt64 Effectif;
					JInt32 NbCas;
					JBool OkCalc = pDocChoixSupport->m_MoteurPresse.GetEffCas(KeyMoteur,Effectif,NbCas); 

					// Calcul audience et volume lecture
					JFlt64 VolLecture		= -1.0;
					JFlt64 AudienceLDP		= -1.0;		// uniquement affichage colonne LDP et %affinité
					JFlt64 AudienceProba	= -1.0;		// sert au calcul du GRP
					// Audience
					JFlt64 Audience			= -1.0;
					JFlt64 Audience1Ins		= -1.0;

					// Test si la cible est autorisée
					if (pDocChoixSupport->m_Offre.CibleAutorisee(IdCible, IdTerrain) == true)
					{
						if (pDocChoixSupport->m_MoteurPresse.CalcAudience(KeyMoteur,IdSupport,VolLecture,AudienceLDP,AudienceProba,PtEtal,true))
						{
							if (VolLecture >= 0)
							{
								// Tenir compte du nombre d'insertions
								VolLecture *= pDocChoixSupport->m_NbInsertions;

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
									if(!pElem->IsCouplage() && !ElmtFamille)
									{
										// Audience devient Audience LDP
										Audience = AudienceLDP;

										// Audience affinité devient Audience LDP affinité
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
										JFlt64 VolLect1Ins = VolLecture / pDocChoixSupport->m_NbInsertions;

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
					}

					// Calcul Résultat Colonne (selon grandeur)
					if (NoLig <= m_GridEchelle.GetRowCount() && IdSupport != 0)
					{
						// Calcul résultat colonne
						ResultEch.m_Audience			= Audience; 
						ResultEch.m_Audience1Ins		= Audience1Ins;
						ResultEch.m_AudienceAffinite	= AudienceAffinite;
						ResultEch.m_AudienceAffiniteLDP = AudienceAffiniteLDP;
						ResultEch.m_AudienceAffiniteProba = AudienceAffiniteProba;
						ResultEch.m_AudienceLDP			= AudienceLDP;
						ResultEch.m_AudienceProba		= AudienceProba;
						ResultEch.m_Effectif			= Effectif;
						ResultEch.m_EffectifAffinite	= EffectifAffinite;
						ResultEch.m_NbCas				= NbCas;
						ResultEch.m_VolAffinite			= VolAffinite;
						ResultEch.m_VolLecture			= VolLecture;
						CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible,
													ResultEch, Tarif);

						// Ajoute dans matrice résultat si nouvelle clef résultat
						pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
						if (!pDocChoixSupport->m_MapResultatEch.IsValid())
						{
							pDocChoixSupport->m_MapResultatEch.Add(KeyResultEch) = ResultEch; 
						}
					}
				}
			}
			else
			{
				// Elment déjà calculé, on prend directement les infos dans map résultats
				ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem(); 

				// Calcul Résultat Colonne (selon grandeur)
				CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible, ResultEch, Tarif);
			}					
		}
	}

	// Affiche résultat Familles avec moyenne des supports si cumul faminne non demandé
	if (pDocChoixSupport->m_CumulFamille == false)
		AfficheResultFamille();

	// Affiche la moyenne une fois toutes les lignes calculés
	AfficheLigneMoyenne();
	
	// Tarif Moyenne
	AfficheTarifMoyenne();

	// Barre de progression invisible
	m_ProgressResultEch.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                 m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										 m_GridEchelle.GetColCount()-1),
										 CGXStyle()
											.SetEnabled(false));

	// Puis affichage des rangs
	AfficheRang();

	// rend invisible les colonnes associées au rang résultat
	UpdateAllViews(UPDATE_AFFICH_RANG_RESULT);
			
	// Mise à jour des sélections supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise à jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// Flag calcul effectué
	pDocChoixSupport->m_ParamEchModif = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage Resultat Famille
JVoid CDlgEchelle::AfficheResultFamille()
{
	// Mode Famille non actif, pas de calcul
	if (pDocChoixSupport->m_AFFModeFamilles == FALSE)
		return;

	// Bloque les modifs
	m_GridEchelle.LockUpdate(true); 

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

	// Balaye toutes les lignes, sauf les supports et la moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention à la ligne moyenne
		if (IdSupport == 0)
			continue;

		// Voir si c'est un élmt famille -- Récupérer la liste des composants
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if(pElem->IsLot())
		{
			// Récup tarif famille
			CString	TxtTarif = m_GridEchelle.GetValueRowCol(NoLig,m_NoColTarif-1); 

			// Vire les espaces
			TxtTarif.Replace(" ",""); 

			// Valeur Tarif
			JUnt32 TarifFamille = atof(TxtTarif);
			// JUnt32 TarifFamille = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColTarif-1));

			// Crée liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// Récupère la liste des titres
				pLot->GetFils(ListSup);

				// Nombre de coposants de la famille
				JUnt32 NbCompoFamille = ListSup.GetCount(); 

				// Balayage des colonnes
				for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
				{
					// Recup Id terrain
					JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

					// Recup Id Cible
					JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

					// Information résultat famille
					CKeyResultEch KeyResultEchFam;
					CResultEch	ResultEchFam;

					// Clé résultat famille
					KeyResultEchFam.m_IdCibleAff	=  pDocChoixSupport->m_Offre.m_IdItemAffinite;
					KeyResultEchFam.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 
					KeyResultEchFam.m_IdMoteur		= pDocChoixSupport->m_MoteurPresse.GetType(); 
					KeyResultEchFam.m_IdCible		= IdCible; 
					KeyResultEchFam.m_IdTerrain		= IdTerrain;
					KeyResultEchFam.m_IdSupport		= IdSupport; 

					// Test si élément déjà calculé
					pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEchFam); 
					if (!pDocChoixSupport->m_MapResultatEch.IsValid())
					{	
						// Nb Composants comptabilisés pour résultat famille
						JUnt32 NbCompoValid = 0;

						// Balaye tous les composants familles
						for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())   
						{
							// Récup composant famille
							JUnt32 IdCompo = ListSup.GetItem(); 

							// Clé archivage résultat
							CKeyResultEch KeyResultEch;

							// Résultat composant
							CResultEch	ResultEch;

							// Clé de recherche résultat (idem clef famille, seul id support change)
							KeyResultEch				= KeyResultEchFam;
							KeyResultEch.m_IdSupport	= IdCompo; 

							// Test si élément déjà calculé ou élmt moyenné
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 

							if (pDocChoixSupport->m_MapResultatEch.IsValid())
							{
								// Elment déjà calculé, on prend directement les infos dans map résultats
								ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem(); 

								// Attention aux résultats non définis
								if (ResultEch.m_Audience > 0)
								{
									// Cumul résultat colonne pour famille
									ResultEchFam.m_Audience					+= ResultEch.m_Audience; 
									ResultEchFam.m_Audience1Ins				+= ResultEch.m_Audience1Ins;
									ResultEchFam.m_AudienceAffinite			+= ResultEch.m_AudienceAffinite;
									ResultEchFam.m_AudienceAffiniteLDP		+= ResultEch.m_AudienceAffiniteLDP;
									ResultEchFam.m_AudienceAffiniteProba	+= ResultEch.m_AudienceAffiniteProba;
									ResultEchFam.m_AudienceLDP				+= ResultEch.m_AudienceLDP;
									ResultEchFam.m_AudienceProba			+= ResultEch.m_AudienceProba;
									ResultEchFam.m_Effectif					+= ResultEch.m_Effectif;
									ResultEchFam.m_EffectifAffinite			+= ResultEch.m_EffectifAffinite;
									ResultEchFam.m_NbCas					+= ResultEch.m_NbCas;
									ResultEchFam.m_VolAffinite				+= ResultEch.m_VolAffinite;
									ResultEchFam.m_VolLecture				+= ResultEch.m_VolLecture;

									// 1 composant de + pris en compte
									NbCompoValid++;
								}
							}
						}

						// Si au moins 1 composant valide, on fait la moyenne
						if (NbCompoValid != 0)
						{
							// Attention si calcul famille avec moyenne
							ResultEchFam.m_Audience					/= NbCompoValid; 
							ResultEchFam.m_Audience1Ins				/= NbCompoValid;
							ResultEchFam.m_AudienceAffinite			/= NbCompoValid;
							ResultEchFam.m_AudienceAffiniteLDP		/= NbCompoValid;
							ResultEchFam.m_AudienceAffiniteProba	/= NbCompoValid;
							ResultEchFam.m_AudienceLDP				/= NbCompoValid;
							ResultEchFam.m_AudienceProba			/= NbCompoValid;
							ResultEchFam.m_Effectif					/= NbCompoValid;
							ResultEchFam.m_EffectifAffinite			/= NbCompoValid;
							ResultEchFam.m_NbCas					/= NbCompoValid;
							ResultEchFam.m_VolAffinite				/= NbCompoValid;
							ResultEchFam.m_VolLecture				/= NbCompoValid;
						}

						// Positionne Flag résultat partiel
						if (NbCompoValid != NbCompoFamille || NbCompoFamille == 0)
						{
							// Ce sont des résultats partiels, manque certains supports
							ResultEchFam.m_ResultPartiel = true;
						}

						//Voir si tous les composants famille ont été trouvé
						if (NbCompoValid != 0)
						{
							// Ecriture résultat famille
							CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible,
														ResultEchFam, TarifFamille);

							// Ajoute dans matrice résultat si nouvelle clef résultat famille
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEchFam); 
							if (!pDocChoixSupport->m_MapResultatEch.IsValid())
							{
								pDocChoixSupport->m_MapResultatEch.Add(KeyResultEchFam) = ResultEchFam; 
							}
						}
						else
						{
							// Résultat famille non définie car manque prise en compte certains composants
							// non défini
							m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),"*");
						}
					}
					else
					{
						// Element famille déjà calculé, on prend directement les infos dans map résultats
						ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem(); 

						// Calcul Résultat Colonne (selon grandeur)
						CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible, ResultEchFam, TarifFamille);
					}					
				}
			}
		}
	}

	// Redessine la grille	
	m_GridEchelle.Redraw(); 
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage Moyenne des supports (visibles bien sur)
JVoid CDlgEchelle::AfficheLigneMoyenne()
{
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

	// Balayage de toutes les lignes résultats
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Calcul moyenne ssi Idsupport = Id Support Moyen (=0)
		if (IdSupport == 0)
		{
			// Balayage des colonnes
			for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
			{
				// Recup Id terrainResultCellule
				JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

				// Recup Id Cible
				JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

				// Calcul Résultat Moyenne sur chaque colonne grandeur
				CString Txt;
				CResultEch ResultEch;
				Txt = ResultCellule(IdSupport,NoLig, NoCol, true,IdTerrain, IdCible, ResultEch);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage Moyenne des tarifs supports
JVoid CDlgEchelle::AfficheTarifMoyenne()
{
	// Init Cumul Tarif
	JFlt64 CumulTarif = 0.0;

	// Index Ligne Moyenne
	JUnt32 InxLgMoy = 0;

	// Valeur tarif
	CString TxtTarif = "";

	// Nombre de lignes tarifées
	JUnt32 NbLigneTarif = 0;

	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Ne pas tenir compte des lignes "non support"
		if (IdSupport > 0)
		{				
			// Ne pas tenir compte des supports cachés
			if (!pDocChoixSupport->GetSupportVisible(IdSupport) || !pDocChoixSupport->VisibleSupportMode(IdSupport)) continue;
			{
				// Récup Tarif Support
				TxtTarif = m_GridEchelle.GetValueRowCol(NoLig,m_NoColTarif-1); 

				// Vire les espaces
				TxtTarif.Replace(" ",""); 

				// Valeur Tarif
				JUnt32 Tarif = atof(TxtTarif);
				if (Tarif > 0)
				{
					// Cumul tarif
					CumulTarif += atof(TxtTarif);

					// Incrémente nb tarif si tarif valide
					NbLigneTarif++;
				}
			}	
		}
		else
			// Récupére index ligne moyenne
			InxLgMoy = NoLig;
	}	

	// Affichage du tarif moyenné
	TxtTarif = "*";
	if (CumulTarif >0 && NbLigneTarif > 0)
	{
		TxtTarif.Format("%0.f",CumulTarif/ NbLigneTarif);
		pDocChoixSupport->FormatString(TxtTarif,3);
	}

	// Ecriture nouvelle moyenne tarif
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);
	m_GridEchelle.SetValueRange(CGXRange(InxLgMoy,m_NoColTarif-1),TxtTarif); 
	m_GridEchelle.GetParam()->SetLockReadOnly(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage résultat échelle selon les grandeurs sélectionnées
JBool CDlgEchelle::AfficheResultat()
{
	// Clé archivage résultat
	CKeyResultEch KeyResultEch;

	// Récup niveau de chacune des grandeurs
	JUnt32 NivTerrain, NivCible, NivGrandeur;
	NivTerrain  = 0;
	NivCible    = 1;
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
			NivTerrain  = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
			NivCible    = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
			NivGrandeur = i;
	}

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Bloque mise à jour
	m_GridEchelle.LockUpdate(true);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridEchelle.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridEchelle.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultEch.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultEch.GetRange( nLower, nUpper);

	// Clef affinité courante
	KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice clé segment
	KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

	// Calcul indice clé moteur
	KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// Récupére map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);
		
	// Balaye toutes les lignes associées aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Avance progress bar
		m_ProgressResultEch.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());
	
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention si c'est une famille
		JBool ElmtFamille = false;
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un élmt famille
			ElmtFamille = true;
		}
	
		// Récupère le format support
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

		// Récupère Tarif item selon format
		JUnt32 Tarif;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			// Tarif du support
			JUnt32 Duree;
			JDate DateExact;

			/*
			if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
					Tarif = 0;
			*/
			Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

			// Si c'est une famille, calculer le tarif correspondant
			if (ElmtFamille)
			{
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
					 					                  pDocChoixSupport->m_DateTarifaire, DateExact,
														  Duree, MapSuppSegTerr, TarifManuel);
			}
		}

		

		// Multiplie par nb insertions
		Tarif *= pDocChoixSupport->m_NbInsertions; 

		// Ecriture du tarif
		if (Tarif > 0)
		{
			// Recup tarif format chaine avec espace milliers
			CString TxtTarif;
			TxtTarif.Format("%d",Tarif);
			pDocChoixSupport->FormatString(TxtTarif,3);


			// Tarif bien défini
			// m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			if (!TarifManuel)
			{
				// Tarif bien défini
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien défini (modif la couleur texte)
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else if (IdSupport > 0)
			// Tarif non défini (sauf pour la moyenne)
			m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

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
		m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

		/*
		// Init les indications résultats complets
		m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NbHeaderCol, NoLig, m_GridEchelle.GetColCount()),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS))
										.SetFont(CGXFont()
											.SetItalic(pDocChoixSupport->m_ItalicResultComplet)));
		*/

		// Balayage des colonnes
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
		{
			// Recup Id terrain
			JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

			// Recup Id Cible
			JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

			// Stockage résultat dans matrice
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Recherche Résultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur résultats
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
			if (pDocChoixSupport->m_MapResultatEch.IsValid())
			{
				// Information résultat archivé
				CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();
				Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible, 
									ResultEch, Tarif);

			}

			// Calcul moyenne des résultats
			else if (KeyResultEch.m_IdSupport == 0)
			{
				// Calcul Résultat des lignes Moyennes (selon grandeur)
				CResultEch ResultEch;
				Txt = ResultCellule(IdSupport,NoLig, NoCol, true, IdTerrain, IdCible, ResultEch);
			}
			else
			{
				Txt = "*";
				m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}
		}
	}

	// Tarif Moyenne
	AfficheTarifMoyenne();

	// Barre de progression invisible
	m_ProgressResultEch.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                 m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										 m_GridEchelle.GetColCount()-1),
										 CGXStyle()
											.SetEnabled(false));

	// Puis affichage des rangs
	AfficheRang();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise à jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// Puis affichage des rangs
	// AfficheRang();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage rang des résultats échelle selon les grandeurs sélectionnées
JVoid CDlgEchelle::AfficheRang()
{
	// Clé archivage résultat
	CKeyResultEch KeyResultEch;

	// Récup niveau de chacune des grandeurs
	JUnt32 NivTerrain, NivCible, NivGrandeur;
	NivTerrain  = 0;
	NivCible    = 1;
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
			NivTerrain  = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
			NivCible    = i;
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
			NivGrandeur = i;
	}

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Bloque mise à jour
	m_GridEchelle.LockUpdate(true);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridEchelle.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridEchelle.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultEch.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultEch.GetRange( nLower, nUpper);

	// Balayage des colonnes rang
	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
	{
		// Attention dans quel ordre (ordre croissant pour Audience, GRP etc...)
		//                           (ordre décroissant pour les coûts)
		// if ((NoCol - m_NbHeaderCol) / 2)

		// Si Colonne rang uniquement
		if ((NoCol - m_NbHeaderCol) % 2 != 0) continue;

		// Recup Id terrain
		JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

		// Recup Id Cible
		JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

		// Récupère toutes les valeurs dans une liste
		JMap <JUnt32,JFlt64> MapRang;

				// Clef affinité courante
		KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

		// Calcul indice clé segment
		KeyResultEch.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

		// Calcul indice clé moteur
		KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

		// Récupére map des supports valides sur les segments en cours
		JMap <JUnt32,JUnt32> MapSuppSegTerr;
		pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

		// Balaye toutes les lignes associées aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Récupère le format support
			JUnt32 IdFormatSupport = 0;
			pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
			if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
				IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

			// Attention si c'est une famille
			JBool ElmtFamille = false;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// C'est un élmt famille
				ElmtFamille = true;
			}

			// Récupère Tarif item selon format
			JUnt32 Tarif;
			if(pTarifs)
			{
				JUnt32 Duree;
				JDate DateExact;

				/*
				if(!pTarifs->GetTarifByDate(IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
						Tarif = 0;
				*/
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

				// Si c'est une famille, calculer le tarif correspondant
				if (ElmtFamille)
				{
					JBool TarifManuel = false;
					Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
															pDocChoixSupport->m_DateTarifaire, DateExact,
															Duree, MapSuppSegTerr, TarifManuel);
				}

				// Multiplie par nb insertions
				Tarif *= pDocChoixSupport->m_NbInsertions;
			}

			// Stockage résultat dans matrice
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Recherche Résultat Colonne (selon grandeur)
			JFlt64 ValRes = 0.0;

			// Positionne sur résultats
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
			if (pDocChoixSupport->m_MapResultatEch.IsValid())
			{
				// Ne pas tenir compte des lignes infos (moyennes, etc...)
				if (IdSupport)
				{
					// Information résultat archivé
					CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();

					// Le résultat pour la cellule en valeur numérique
					ValRes = ResultValCellule(IdSupport, NoLig, NoCol+1,Tarif,ResultEch);
				}
			}
			
			// Ajoute au map rang (si non existant)
			MapRang.MoveTo(IdSupport);
			if (!MapRang.IsValid()) 
				MapRang.Add(IdSupport) = ValRes; 
		}

		// Calcul Valeur indice
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Init indice rang
			JUnt32 IndiceRang = 1;
			if (ModeRangColonne(NoCol) == 1)
				// Pour les coûts ordre rang inversé
				IndiceRang = 0;

			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Ne pas tenir compte des lignes infos (moyennes, etc...)
			if (IdSupport)
			{
				// Valeur en cours
				MapRang.MoveTo(IdSupport);
				JFlt64 &ValCur = MapRang.GetItem();

				// Analyse toutes les lignes (bourrin !!!!)
				for (MapRang.MoveFirst(); MapRang.IsValid(); MapRang.MoveNext())
				{
					// Récupère valeur courant
					JFlt64 &Valeur = MapRang.GetItem();

					// Décrémente indice rang si valeur supérieur
					if (ModeRangColonne(NoCol) == 0)
					{
						// Cas des valeurs résultats en dehors des coûts
						if (ValCur < Valeur && Valeur >= 0)	IndiceRang++;
					}
					else
					{
						// Cas des valeurs résultats en dehors des coûts
						if (ValCur > Valeur && Valeur >= 0)	IndiceRang++;
					}
				}

				// Affiche le rang de la ligne sur la colonne en cours
				CString Txt = "*";
				if (ValCur >= 0)
				{
					Txt.Format("%d",IndiceRang); 
				}
				m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}

			else
			{
				// Ne rien afficher pour les lignes infos (type moyenne....)
				CString Txt = "";
				m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),Txt); 
			}
		}
	}

	// Barre de progression invisible
	m_ProgressResultEch.ShowWindow(SW_HIDE); 

	// Bloque modification cellule
	m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,
		                                 m_NbHeaderRow + pDocChoixSupport->m_MapIdSupportsSel.GetCount()-1,
										 m_GridEchelle.GetColCount()-1),
										 CGXStyle()
											.SetEnabled(false));

	// Mise à jour des colonnes rangs
	UpdateRangResultat();

	// Mise à jour des sélections supports
	UpdateVisuSupports();

	// Lock à jour affichage
	m_GridEchelle.LockUpdate(true);

	// Retaille via dernière modification largeur colonne
	ResizeColonneFixe();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise à jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Formatage des résultats selon grandeur
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
/*
CString CDlgEchelle::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JBool ResultMoyenne, JUnt32 IdTerrain, JUnt32 IdCible, 
								   JFlt64 Tarif, JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
								   JFlt64 Effectif, JFlt64 NbCas, 
								   JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba,   
								   JFlt64 AudAff, JFlt64 EffAff)
*/
CString CDlgEchelle::ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JBool ResultMoyenne, JUnt32 IdTerrain, JUnt32 IdCible, 
								   CResultEch	&ResultEch, JFlt64 Tarif)
{
	// Par défaut Texte
	CGXStyle style;
	m_GridEchelle.SetStyleRange(CGXRange(NoLig,NoCol),
								CGXStyle()
									.SetFormat(GX_FMT_TEXT)
									.SetValueType(GX_VT_STRING));

	// Init les indications résultats complets
	if (ResultEch.m_ResultPartiel)
	{	
		// On distingue résultat partiel cellule
		m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorRed())
										.SetFont(CGXFont()
										.SetItalic(pDocChoixSupport->m_ItalicResultIncomplet)));
	}
	else
	{
		// On distingue cellule resultat complet
		m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoCol, NoLig, NoCol),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack())
										.SetFont(CGXFont()
										.SetItalic(pDocChoixSupport->m_ItalicResultComplet)));
	}

	// Chaine formatée par défaut
	CString TxtResult = "*";

	// Niveau grandeur appliquée
	JUnt32 NivGrandeur = 2;

	// Récupère ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

	// Résultat de la cellule
	JFlt64 ResultCell = -1;

	if (ResultMoyenne == false) 
	{
		// Récupère valeur résultat cellule
		/*
		ResultCell = ResultValCellule(IdSupport,NoLig,NoCol,Tarif,VolLecture,
		 							  AudienceLDP,AudienceProba, Audience,Audience1Ins,
									  Effectif,NbCas,VolLectAff,AudAffLDP,AudAffProba,AudAff,EffAff);
		*/
		ResultCell = ResultValCellule(IdSupport, NoLig, NoCol, Tarif, ResultEch);
	}
	else
	{
		// Récupère valeur résultat moyenne via map des résultats
		ResultCell = MoyenneResultCellule(NoCol, IdTerrain, IdCible);
	}

	// Formatage des résultats
	if (IdGrandeur >= 1000)
	{
		TxtResult = "R";
	}
	else if (ResultCell >= 0)
	{
		if (IdGrandeur < 100)
		{
			// Calcul selon grandeurs
			switch(IdGrandeur)
			{
				///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
				case pDocChoixSupport->GrdEch_AudienceLDP : 
				{
					// Découpage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
				case pDocChoixSupport->GrdEch_Audience :
				{
					// Découpage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
				case pDocChoixSupport->GrdEch_Audience000 :
				{
					// Découpage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
				case pDocChoixSupport->GrdEch_NbContact000 :
				{
					// Découpage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				/////////////////////////////////// CALCUL GRP ///////////////////////////////////
				case pDocChoixSupport->GrdEch_GRP : 
				{
					TxtResult.Format("%0.1f ",ResultCell);
					//TxtResult.Replace(".",",");
				}
				break;

				/////////////////////////////////// CALCUL GRP ACTIF //////////////////////////////
				case pDocChoixSupport->GrdEch_XGRP : 
				{
					TxtResult.Format("%0.1f ",ResultCell);
					//TxtResult.Replace(".",",");
				}
				break;

				/////////////////////////////////// CALCUL COUVERTURE ////////////////////////////
				case pDocChoixSupport->GrdEch_CouvCum : 
				{
					// Texte chiffre formatté
					TxtResult.Format("%0.1f%%",ResultCell);
				}
				break;

				////////////////////////////// CALCUL REPRISES EN MAIN ///////////////////////
				case pDocChoixSupport->GrdEch_Repetition : 
				{
					TxtResult.Format("%0.2f ",ResultCell);
					//TxtResult.Replace(".",",");
				}
				break;

				////////////////////////////// CALCUL REPETITION + ///////////////////////////
				case pDocChoixSupport->GrdEch_XRepetition : 
				{
					TxtResult.Format("%0.2f ",ResultCell);
					//TxtResult.Replace(".",",");
				}
				break;

				///////////////////////////// CALCUL INDICE AFFINITE ////////////////////////
				case pDocChoixSupport->GrdEch_IndAffinite :
				{
					// Découpage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
				case pDocChoixSupport->GrdEch_PrctAff :
				{
					// Calcul pourcentage affinité
					TxtResult.Format("%0.1f%%",ResultCell);
				}
				break;

				///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
				case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativité
				{
					TxtResult.Format("%0.f ",ResultCell);	
					//TxtResult.Replace(".",",");
				}
				break;
				

				///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
				case pDocChoixSupport->GrdEch_CoutGRP : 
				{
					// Le calcul doit être fait sur la totalité des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb décimales demandées
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// Découpage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
				case pDocChoixSupport->GrdEch_CoutCouv : 
				{
					// Le calcul doit être fait sur la totalité des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb décimales demandées
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// Découpage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);			}
				break;

				///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
				case pDocChoixSupport->GrdEch_Cout000Ind : 
				{
					// Le calcul doit être fait sur la totalité des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb décimales demandées
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// Découpage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
				case pDocChoixSupport->GrdEch_Cout000Cts : 
				{
					// Le calcul doit être fait sur la totalité des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb décimales demandées
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// Découpage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				default:
				break;
			}
		}

		// Affichage des résultats datas exogènes
		else
		{
			// Récupère résultat formaté selon type et nb dec data exo
			TxtResult = GetResultDataExo(IdGrandeur, ResultCell);
		}
	}


	// Renvoi la chaine formattée
	m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
	return TxtResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupère résultat formaté selon type et nb dec data exo
CString CDlgEchelle::GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell)
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

//////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des résultats selon grandeur en valeur réelle
//  0  > "LNM/LDP(000)" 					-------------------------------- LNM/LDP (000) (Audience) en milliers
//  1  > "Audience"							-------------------------------- audience * effectif / 100
//  2  > "Audience(000)						-------------------------------- idem en milliers
//  3  > "Cts.[+](000)"						-------------------------------- VolLecture (Contacts en milliers)
//  4  > "GRP"   							-------------------------------- (Audience LNM/LDP * 100.0) / Effectif
//  5  > "GRP[+]							-------------------------------- (VolLecture * 100.0) / Effectif
//  6  > "C% cumulée"						--------------------------------  audience n insertions
//  7  > "Répétition ou reprises en main"	-------------------------------- VolLecture / (Effectif * Audience)
//  8  > "Répétition +"						-------------------------------- audience n insertions / ???
//  9  > "Indice Affinité"					-------------------------------- (GRP / GRPAff) * 100.0 
// 10  > "% Affinité"						-------------------------------- (AudienceLDP / AudAffLDP) * 100.0 
// 11  > "Cumulativité"						-------------------------------- (Audience n insertions / Audience à 1 insertion)
// 12  > "Coût/GRP"							-------------------------------- tarif support / GRP
// 13  > "Coût/1000 effectif"				-------------------------------- tarif support / audience
// 14  > "Coût/%Couverture					-------------------------------- tarif support / % Couverture
// 15  > "Coût/1000 contacts"				-------------------------------- tarif support / Nb Contacts

//  ?? > "Audience / Nb Lect"				-------------------------------- audience * effectif / 100
//
JFlt64 CDlgEchelle::ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif,
									 CResultEch &ResultEch)									 
{
	// Résultat cellule
	JFlt64 Resultat = -1.0;

	// Récupère ident grandeur
	JUnt32 NivGrandeur  = 2;
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		case pDocChoixSupport->GrdEch_AudienceLDP : 
		{
			// Récupére audience
			Resultat = ResultEch.m_AudienceLDP / 1000;
		}
		break;

		////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdEch_Audience :
		{
			Resultat = ResultEch.m_Audience;
		}
		break;

		///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
		case pDocChoixSupport->GrdEch_Audience000 :
		{
			Resultat = ResultEch.m_Audience /1000.0;
		}
		break;

		////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdEch_NbContact000 :
		{
			Resultat = ResultEch.m_VolLecture / 1000.0;
		}
		break;

		/////////////////////////////////// CALCUL GRP ///////////////////////////////////
		case pDocChoixSupport->GrdEch_GRP : 
		{
			if (ResultEch.m_Effectif > 0 && ResultEch.m_AudienceProba >= 0)
				Resultat = (100.0 * ResultEch.m_AudienceProba * pDocChoixSupport->m_NbInsertions) / ResultEch.m_Effectif;
		}
		break;

		/////////////////////////////////// CALCUL GRP ACTIF //////////////////////////////
		case pDocChoixSupport->GrdEch_XGRP : 
		{
			if (ResultEch.m_Effectif > 0 && ResultEch.m_VolLecture >= 0)
			{
				Resultat = (ResultEch.m_VolLecture * 100.0) / ResultEch.m_Effectif;
			}
		}
		break;

		/////////////////////////////////// CALCUL COUVERTURE ////////////////////////////
		case pDocChoixSupport->GrdEch_CouvCum : 
		{
			if (ResultEch.m_Effectif > 0 && ResultEch.m_Audience >= 0)
			{
				Resultat = (ResultEch.m_Audience / ResultEch.m_Effectif) * 100.0;
			}
		}
		break;

		////////////////////////////// CALCUL REPRISES EN MAIN ///////////////////////
		case pDocChoixSupport->GrdEch_Repetition : 
		{
			// Calcul Répétition Normale
			if (ResultEch.m_Audience > 0)
				Resultat = (ResultEch.m_AudienceProba * pDocChoixSupport->m_NbInsertions) / ResultEch.m_Audience;
		}
		break;

		////////////////////////////// CALCUL REPETITION + ///////////////////////////
		case pDocChoixSupport->GrdEch_XRepetition : 
		{
			if (ResultEch.m_Audience > 0)
			{
				Resultat = ResultEch.m_VolLecture / ResultEch.m_Audience;
			}
		}
		break;

		///////////////////////////// CALCUL INDICE AFFINITE ////////////////////////
		case pDocChoixSupport->GrdEch_IndAffinite :
		{
			// Calcul GRP
			JFlt64 GRP		= 0.0;
			if (ResultEch.m_Effectif > 0)
				GRP = (ResultEch.m_AudienceProba / ResultEch.m_Effectif) * 100.0;

			// Calcul GRP Affinité
			JFlt64 GRPAff	= 0.0;
			if (ResultEch.m_EffectifAffinite > 0)
				GRPAff = (ResultEch.m_AudienceAffiniteProba / ResultEch.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				// Calcul indice affinité
				Resultat = (GRP / GRPAff)*100.0;
			}
		}
		break;

		//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
		case pDocChoixSupport->GrdEch_PrctAff :
		{
			if (ResultEch.m_AudienceAffinite > 0)
			{
				// Calcul pourcentage affinité
				// Resultat = (AudienceLDP / AudAffLDP) * 100.0;
				Resultat = (ResultEch.m_Audience / ResultEch.m_AudienceAffinite) * 100.0;
			}
		}
		break;

		///////////////////////////// CALCUL INDICE CUMULATIVITE ////////////////////////////
		case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativité
		{
			if (ResultEch.m_Audience1Ins > 0)
			{
				Resultat =  ResultEch.m_Audience / ResultEch.m_Audience1Ins * 100.0;	
			}
		}
		break;
		

		///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
		case pDocChoixSupport->GrdEch_CoutGRP : 
		{
			// Calcul GRP
			JFlt64 GRP	= 0.0;
			if (ResultEch.m_Effectif > 0)
				GRP = (ResultEch.m_AudienceProba * pDocChoixSupport->m_NbInsertions / ResultEch.m_Effectif) * 100.0;

			// Si GRP défini, calcul du coût
			if (GRP > 0 && Tarif > 0)
			{
				Resultat = (Tarif) / GRP;
			}
		}
		break;

		///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
		case pDocChoixSupport->GrdEch_CoutCouv : 
		{
			if (ResultEch.m_Effectif > 0 && ResultEch.m_Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (ResultEch.m_Audience / ResultEch.m_Effectif) * 100.0;

				if (Couv > 0 && Tarif > 0)
				{
					Resultat  = Tarif / Couv;
				}
			}
		}
		break;

		///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
		case pDocChoixSupport->GrdEch_Cout000Ind : 
		{
			if ((ResultEch.m_Audience / 1000) > 0 && Tarif > 0)
			{
				Resultat  = Tarif / (ResultEch.m_Audience/1000.0);
			}
		}
		break;

		///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
		case pDocChoixSupport->GrdEch_Cout000Cts : 
		{
			// Calcul Cout
			if (ResultEch.m_VolLecture > 0 && Tarif > 0)
			{
				Resultat = Tarif / (ResultEch.m_VolLecture /1000.0);
			}
		}
		break;

		default:
		break;
	}

	// Traitement des données exogènes (OJD, .....)
	IdGrandeur = IdGrandeur % 1000;
	if (IdGrandeur >= 100)
	{
		// Récupère les informations associées aux données exogènes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libellé data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exogènes ////////////////////////////
		if (!DataExo.GetValeur(NomDataExo,IdSupport,Resultat))
			Resultat = -1;
	}

	return Resultat;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des résultats moyennés
JFlt64 CDlgEchelle::MoyenneResultCellule(JUnt32 NoCol, JUnt32 IdTerrain, JUnt32 IdCible)
{
	// Nombre de supports valide pour le calcul
	JUnt32 NbSupportCalc = 0;

	// Cumul Numérateur
	JFlt64 CumNumerateur	= 0.0;

	// Cumul Dénominateur
	JFlt64 CumDenominateur	= 0.0;

	// Résultat Moyenné
	JFlt64 Resultat = 0;

	// Récupére map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// Boucle sut toutes les lignes résulats échelle
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Clé recherche résultat du support
		CKeyResultEch KeyResultEch;

		// Clef affinité courante
		KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

		// Calcul indice clé segment
		KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

		// Calcul indice clé moteur
		KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// On ne comptabilise pas les lignes non visibles
		if (!pDocChoixSupport->GetSupportVisible(IdSupport) || !pDocChoixSupport->VisibleSupportMode(IdSupport)) continue;
		
		// Clé de recherche résultat
		KeyResultEch.m_IdCible			= IdCible; 
		KeyResultEch.m_IdTerrain		= IdTerrain;
		KeyResultEch.m_IdSupport		= IdSupport; 

		// Récup résultat de la lign courante
		pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
		if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
		{
			// Récupère résultats pour ce support
			CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();

			// Récupère le format support
			JUnt32 IdFormatSupport = 0;
			pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(KeyResultEch.m_IdSupport);
			if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
				IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

			// Attention si c'est une famille
			JBool ElmtFamille = false;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// C'est un élmt famille
				ElmtFamille = true;
			}

			// Récupère Tarif item selon format
			CTarifs * pTarifs	= CTarifs::GetInstance();
			JUnt32 Tarif;
			if(pTarifs)
			{
				JUnt32 Duree;
				JDate DateExact;

				/*
				if(!pTarifs->GetTarifByDate(KeyResultEch.m_IdSupport, IdFormatSupport, pDocChoixSupport->m_DateTarifaire,pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
						Tarif = 0;
				*/
				JBool TarifManuel = false;
				Tarif = pDocChoixSupport->GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);

				// Si c'est une famille, calculer le tarif correspondant
				if (ElmtFamille)
				{
					Tarif = pDocChoixSupport->GetTarifFamille(IdSupport, IdFormatSupport, 
															pDocChoixSupport->m_DateTarifaire, DateExact,
															Duree, MapSuppSegTerr, TarifManuel);
				}

			}

			// Tarif par nb insertions
			Tarif *= pDocChoixSupport->m_NbInsertions;

			// Récupère ident grandeur
			JUnt32 NivGrandeur  = 2;
			JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

			// Test selon type de résultat (calculé ou direct type data exogène)
			if (IdGrandeur < 100)
			{
				// Calcul selon grandeurs
				switch(IdGrandeur)
				{
					///////////////////////////// CALCUL AUDIENCE LDP/LNM /////////////////////////////
					case pDocChoixSupport->GrdEch_AudienceLDP : 
					{
						if (ResultEch.m_AudienceLDP >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= ResultEch.m_AudienceLDP / 1000;		// (ResultEch.m_AudienceLDP + 500)/1000;
							CumDenominateur ++;
						}
					}
					break;

					////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
					case pDocChoixSupport->GrdEch_Audience :
					{
						if (ResultEch.m_Audience >= 0)
						{							
							// Cumul des élmts
							CumNumerateur	+= ResultEch.m_Audience;
							CumDenominateur ++;
						}
					}
					break;

					///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
					case pDocChoixSupport->GrdEch_Audience000 :
					{
						if (ResultEch.m_Audience >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= ResultEch.m_Audience / 1000; // (ResultEch.m_Audience + 500)/1000;
							CumDenominateur ++;
						}
					}
					break;

					////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
					case pDocChoixSupport->GrdEch_NbContact000 :
					{
						if (ResultEch.m_VolLecture >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= ResultEch.m_VolLecture / 1000;    //(ResultEch.m_VolLecture + 500)/1000;
							CumDenominateur ++;
						}
					}
					break;

					/////////////////////////////////// CALCUL GRP ///////////////////////////////////
					case pDocChoixSupport->GrdEch_GRP : 
					{
						if (ResultEch.m_Effectif >= 0 && ResultEch.m_AudienceLDP >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= (100.0 * ResultEch.m_AudienceLDP * pDocChoixSupport->m_NbInsertions) / ResultEch.m_Effectif;
							CumDenominateur ++;
						}
					}
					break;

					/////////////////////////////////// CALCUL GRP ACTIF //////////////////////////////
					case pDocChoixSupport->GrdEch_XGRP : 
					{
						if (ResultEch.m_Effectif > 0 && ResultEch.m_VolLecture >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= (ResultEch.m_VolLecture * 100.0) / ResultEch.m_Effectif;
							CumDenominateur ++;
						}
					}
					break;

					/////////////////////////////////// CALCUL COUVERTURE ////////////////////////////
					case pDocChoixSupport->GrdEch_CouvCum : 
					{
						if (ResultEch.m_Effectif > 0 && ResultEch.m_Audience >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= (ResultEch.m_Audience / ResultEch.m_Effectif) * 100.0;
							CumDenominateur ++;
						}
					}
					break;

					////////////////////////////// CALCUL REPRISES EN MAIN ///////////////////////
					case pDocChoixSupport->GrdEch_Repetition : 
					{
						// Calcul Répétition Normale
						if (ResultEch.m_Audience > 0)
						{
							CumNumerateur += (ResultEch.m_AudienceProba * pDocChoixSupport->m_NbInsertions) / ResultEch.m_Audience;
							CumDenominateur ++;
						}
					}
					break;

					////////////////////////////// REPETITION + //////////////////////////////
					case pDocChoixSupport->GrdEch_XRepetition : 
					{
						// Répétition +
						if (ResultEch.m_Audience >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= ResultEch.m_VolLecture / ResultEch.m_Audience;
							CumDenominateur ++;
						}
					}
					break;

					///////////////////////////// CALCUL INDICE AFFINITE ////////////////////////
					case pDocChoixSupport->GrdEch_IndAffinite :
					{
						if (ResultEch.m_AudienceLDP >= 0)
						{
							// Calcul GRP
							JFlt64 GRP		= 0.0;
							if (ResultEch.m_Effectif > 0)
								GRP = (ResultEch.m_AudienceLDP / ResultEch.m_Effectif) * 100.0;

							// Calcul GRP Affinité
							JFlt64 GRPAff	= 0.0;
							if (ResultEch.m_EffectifAffinite > 0)
								GRPAff = (ResultEch.m_AudienceAffiniteLDP / ResultEch.m_EffectifAffinite) * 100.0;

							if (GRPAff)
							{
								// Cumul des élmts
								CumNumerateur	+= (GRP / GRPAff) * 100.0;
								CumDenominateur ++;
							}
						}
					}
					break;

					//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
					case pDocChoixSupport->GrdEch_PrctAff :
					{
						if (ResultEch.m_AudienceAffiniteLDP > 0 && ResultEch.m_AudienceLDP >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= (ResultEch.m_AudienceLDP / ResultEch.m_AudienceAffiniteLDP) * 100.0;
							CumDenominateur ++;
						}
					}
					break;

					///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
					case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativité
					{
						if (ResultEch.m_Audience1Ins > 0 && ResultEch.m_Audience >= 0)
						{
							// Cumul des élmts
							CumNumerateur	+= (ResultEch.m_Audience / ResultEch.m_Audience1Ins) * 100.0;	
							CumDenominateur ++;
						}
					}
					break;
				
					///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
					case pDocChoixSupport->GrdEch_CoutGRP : 
					{
						if (ResultEch.m_AudienceLDP >= 0)
						{
							// Calcul GRP
							JFlt64 GRP	= 0.0;
							if (ResultEch.m_Effectif > 0)
								GRP = (ResultEch.m_AudienceLDP * pDocChoixSupport->m_NbInsertions / ResultEch.m_Effectif) * 100.0;

							// Si GRP défini, calcul du coût
							if (GRP > 0 && Tarif > 0)
							{
								// Cumul des élmts
								CumNumerateur	+= Tarif;
								CumDenominateur += GRP;
							}
						}
					}
					break;

					///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
					case pDocChoixSupport->GrdEch_CoutCouv : 
					{
						if (ResultEch.m_Effectif >= 0 && ResultEch.m_Audience >= 0)
						{
							// Calcul couverture
							JFlt64 Couv = (ResultEch.m_Audience / ResultEch.m_Effectif) * 100.0;

							if (Couv > 0 && Tarif > 0)
							{
								// Cumul des élmts
								CumNumerateur	+= Tarif ;
								CumDenominateur += Couv;
							}
						}
					}
					break;

					///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
					case pDocChoixSupport->GrdEch_Cout000Ind : 
					{
						if ((ResultEch.m_Audience/1000) > 0 && Tarif > 0)
						{
							// Cumul des élmts
							CumNumerateur	+= Tarif;
							CumDenominateur += ResultEch.m_Audience /1000;
						}
					}
					break;

					///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
					case pDocChoixSupport->GrdEch_Cout000Cts : 
					{
						// Calcul Cout
						if (ResultEch.m_VolLecture > 0 && Tarif > 0)
						{
							// Cumul des élmts
							CumNumerateur	+= Tarif;
							CumDenominateur += ResultEch.m_VolLecture /1000;
						}
					}
					break;

					///////////////////////////// AUCUN CALCUL DEFINI ///////////////////// 
					default:
					break;
				}		
			}
			else if (IdGrandeur < 1000)
			{
				////////////////////  Traitement des valeurs data exogène (lecture direct de la valeur dans case)//////////////
				//
				CResultEch ResultEch;
				CString Txt = ResultCellule(IdSupport, NoLig, NoCol, false, IdTerrain, IdCible, ResultEch);

				// Remplace "." par "," pour les tris numériques
				// Txt.Replace(".",","); 
				CString Dec;
				Dec.AppendChar((TCHAR)CFormatStr::GetDecimalPoint());
				Txt.Replace(".",Dec);

				// Et on vire les espaces
				Txt.Replace(" ",""); 

				// Valeur cellule
				JFlt64 ValCell = atof(Txt);

				if (Txt != "*")
				{
					// Cumul des élmts
					CumNumerateur	+= ValCell;
					CumDenominateur ++;
				}
			}	
		}
	}

	if (CumDenominateur > 0)
		// Renvoi résultat moyenné
		return (CumNumerateur / CumDenominateur);
	else
		// Pas de support cumulé, calcul non défini
		return -1.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération sélection Nb Insertions
void CDlgEchelle::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi sélection Nb Insertions
JUnt32 CDlgEchelle:: GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertions;
}	

//////////////////////////////////////////////////////////////////////////////////////////////
// Update grille résultat via new sel Terrains / cibles ou format
bool CDlgEchelle::UpdateResultEchelle(JBool CalculResult)
{
	// Test validité sélection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si sélection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// Désactive mise à jour grid échelle
	m_GridEchelle.LockUpdate(true);

	// Mise à jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise à jour nombre et headers rows
	if (!UpdateRowHeader()) return false;

	// Lock mise  à jour
	m_GridEchelle.LockUpdate(true);

	// Calcul des résultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Remettre les sélections supports
	UpdateVisuSupports();

	// Mise à jour de la moyenne
	UpdateAfficheMoyenne();

	// On remet le tri à jour
	MajTriCourant();

	// Retaille via dernière modification largeur colonne
	// ResizeColonneFixe();

	// Bloque resize hauteur des lignes
	m_GridEchelle.GetParam()->EnableTrackRowHeight(FALSE);
	
	// Rend visible le grid, si au moins quelques résultats
	if (m_GridEchelle.GetColCount() >= m_NoColTarif)
	{
		m_GridEchelle.ShowWindow(SW_SHOW);


	}
	else
		m_GridEchelle.ShowWindow(SW_HIDE);

	// Update affichage infos analyse
	UpdateTitreAnalyse();

	// Retaillage des colonnes fixes
	ResizeColonneFixe();

	// Active mise à jour grid échelle
	m_GridEchelle.LockUpdate(false);

	// reddessine le grid échelle
	m_GridEchelle.Redraw();

	// Retaille supports via max libellé
	m_GridEchelle.ResizeColWidthsToFit(CGXRange(m_NbHeaderRow, m_NoColSupport-1, m_GridEchelle.GetRowCount(), m_NoColSupport-1),
 				                       TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Colonnes
bool CDlgEchelle::UpdateCol()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Colonnes
	m_NbHeaderCol = m_NoColTarif - m_NoColIdSupport + 1;

	// Nb Colonnes
	if (pDocChoixSupport != NULL)
	{
		// Nb col = Nb Terrains x Nb Cibles x Nb Grandeurs
		JUnt32 NbColTer = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
		JUnt32 NbColCib = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount();
		JUnt32 NbColGr  = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();

		// Nb colonnes total affecté au grid échelle
		m_NbCol = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount() * 
		          pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount()   *  
				  pDocChoixSupport->m_MapIdGrandEchSel.GetCount();

		// Init tableau dimension
		// A REVOIR
		m_NbDim.SetSize(3);
		m_NbDim.SetAt(0,pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount());
		m_NbDim.SetAt(1,pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount());
		m_NbDim.SetAt(2,pDocChoixSupport->m_MapIdGrandEchSel.GetCount());

		// Doublement des colonnes pour les infos rang
		m_NbCol *= 2;

	}
	else
		// Aucune colonne valide
		m_NbCol = 0;

	// Init grid avec nb colonnes
    if (!m_GridEchelle.SetColCount(m_NbHeaderCol + m_NbCol - 1,GX_UPDATENOW))
		return false;


	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update Lignes
bool CDlgEchelle::UpdateRowHeader()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Nb Header Lignes
	m_NbHeaderRow = 0;
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount() > 0)
		m_NbHeaderRow++;
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount() > 0)
		m_NbHeaderRow++;
	if (pDocChoixSupport->m_MapIdGrandEchSel.GetCount() > 0)
		m_NbHeaderRow++;

	// D'office les 3 dimensions headers rows (soit terrains, cibles, grandeurs)
	if (m_NbHeaderRow != 3) 
		m_NbHeaderRow = 1;

	// Nombre de lignes
    if (!m_GridEchelle.SetRowCount(m_NbHeaderRow + m_NbRowSupport + m_NbRowInfo - 1,GX_UPDATENOW))
		return false;

	// Lignes headers (3 : Terrains / Cibles / Grandeurs)
	m_GridEchelle.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Remplissage des entetes header rows
	if (m_NbCol >= 1)
	{
		for (JUnt32 Lig = 0; Lig < m_NbHeaderRow ; Lig++)
		{
			// Pour chaque colonne
			JUnt32 NbCol = m_GridEchelle.GetColCount();

			for (JUnt32 Col = 0; Col <= m_GridEchelle.GetColCount(); Col++) 
			{
				// Centrage et style texte header
				m_GridEchelle.SetStyleRange(CGXRange(Lig,Col),
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
					m_GridEchelle.SetValueRange(CGXRange(Lig,Col),Txt); 
				}
			}
		}

		// Couleurs spéciales
		UpdateColorSpe();

		// Update Grille modifiée
		m_GridEchelle.UpdateData(TRUE);

		// Calcul pas fragmentation header
		CArray <int,int> TNbPas; 
		TNbPas.SetSize(m_NbHeaderRow); 
		switch (m_NbHeaderRow)
		{
			case 3 :
			{
				TNbPas.SetAt(0,pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount() * 
							pDocChoixSupport->m_MapIdGrandEchSel.GetCount());
				TNbPas.SetAt(1,pDocChoixSupport->m_MapIdGrandEchSel.GetCount());
				TNbPas.SetAt(2,1);
			}				
			break;

			default :
			{
			}
		}

		// Regroupement des lignes header row
		JUnt32 NbCol = m_GridEchelle.GetColCount();
		m_GridEchelle.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);

		// Style des headers rows
		m_GridEchelle.SetStyleRange(CGXRange(0,1,m_NbHeaderRow-1,NbCol),
									CGXStyle()
											.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));

		//m_GridEchelle.RedrawRowCol(CGXRange(0,1,m_NbHeaderRow-1,m_GridEchelle.GetColCount()),GX_UPDATENOW); 

		// Mise à jour des datas résultat grille échelle
		m_GridEchelle.UpdateWindow(); 
		m_GridEchelle.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(TRUE);

	// m_GridEchelle.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération texte header row
CString CDlgEchelle::RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col)
{
	// Init chaine libellé
	CString Txt = "-";

	// Nombre d'élémt par strate header
	CArray <JUnt32> NbElmtHeader;
	NbElmtHeader.SetSize(pDocChoixSupport->m_OrdreEnteteEch.GetCount());
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre = pDocChoixSupport->m_OrdreEnteteEch[i];

		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
		{
			// Elmt Terrains
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
		{
			// Elmt Cibles
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
		{
			// Elmt Grandeurs
			NbElmtHeader[i] = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();
		}
	}

	// Récupère nb sélections pour chaque liste
	int NbTerrains  = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
	int NbCibles    = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount(); 
	int NbGrandeurs = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();

	// Pas la peine de continuer
	if (NbTerrains == 0 || NbCibles == 0 || NbGrandeurs == 0)
		return "";

	//////////////////////////////////// Entete header ligne niveau 1 /////////////////////////
	//
	if (Lig == 0)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 1)
		{
			// Ligne header zone terrains
			// int IndexTerrain = (((int)Col - (int)m_NbHeaderCol)/(NbCibles * NbGrandeurs));
			int IndexTerrain = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]));
			IndexTerrain = (IndexTerrain/2)	% NbTerrains;

			// Récup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// Récupère indice sélection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// Récupère infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// Récupére libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}

		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]));
			IndexCible = (IndexCible/2)	% NbCibles;

			// Récupère indice sélection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// Récupére ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// Récup libellé 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
					Txt = pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
		}

		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 3)
		{
			// Ligne header zone grandeurs
			int IndexGrandeur = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]));
			IndexGrandeur = IndexGrandeur % NbGrandeurs;

			// Récupère identificateur sélection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,0);

			// Récupère libellé abrégé si ident grandeur trouvé
			// if ((IdGrandeur >= 0 && IdGrandeur < pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch)) || IdGrandeur >= 1000) 
			if ((IdGrandeur >= 0 && IdGrandeur < 1000) || IdGrandeur >= 1000) 
				// Libellé grandeur abrégé
				Txt.Format("%s",pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur % 1000)); 
			else
				// non défini
				Txt = "*";
		}
	}

	//////////////////////////////////// Entete header ligne niveau 2 /////////////////////////
	//
	else if (Lig == 1)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 1)
		{
			// Ligne header zone terrains
			int IndexTerrain = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[2])); //% NbTerrains;
			IndexTerrain = (IndexTerrain / 2) % NbTerrains;

			// Récup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// Récupère indice sélection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// Récupère infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// Récupére libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[2])); // % NbCibles;
			IndexCible = (IndexCible /2) % NbCibles ;
			
			// Récupère indice sélection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// Récupére ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// Récup libellé 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
					Txt = pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 3)
		{
			// Ligne header zone grandeurs
			int IndexGrandeur = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[2])) % NbGrandeurs;

			// Récupère identificateur sélection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,1);

			// Récupère libellé abrégé si ident grandeur trouvé
			// if ((IdGrandeur >= 0 && IdGrandeur < pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch)) || IdGrandeur >= 1000) 
			if ((IdGrandeur >= 0 && IdGrandeur < 1000) || IdGrandeur >= 1000) 
				Txt.Format("%s",pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur % 1000)); 
			else
				Txt = "*";
		}
	}

	//////////////////////////////////// Entete header ligne niveau 2 /////////////////////////
	//
	else if (Lig == 2)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 1)
		{
			// Ligne header zone terrains
			int IndexTerrain = ((int)Col - (int)m_NbHeaderCol); // % NbTerrains;
			IndexTerrain = (IndexTerrain / 2) % NbTerrains;

			// Récup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// Récupère indice sélection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// Récupère infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// Récupére libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = ((int)Col - (int)m_NbHeaderCol); // % NbCibles;
			IndexCible = (IndexCible/2) % NbCibles;

			// Récupère indice sélection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// Récupére ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// Récup libellé 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
					Txt = pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 3)
		{
			// Ligne header zone grandeurs
			int IndexGrandeur = ((int)Col - (int)m_NbHeaderCol) % NbGrandeurs;

			// Récupère identificateur sélection grandeur
			//JUnt32 IdGrandeur = RecupOrdoIdGrandeur(IndexGrandeur);
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,2);

			// Récupère libellé abrégé si ident grandeur trouvé
			// if ((IdGrandeur >= 0 && IdGrandeur < pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch)) || IdGrandeur >= 1000) 
			if ((IdGrandeur >= 0 && IdGrandeur < 1000) || IdGrandeur >= 1000) 
				Txt.Format("%s",pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur % 1000)); 
			else
				Txt = "*";
		}
	}
	
	// Renvoi libellé trouvé
	return Txt;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération Id Terrain
JUnt32 CDlgEchelle::RecupIdTerrain(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'élémt par strate header
	CArray <JUnt32> NbElmtHeader;
	NbElmtHeader.SetSize(pDocChoixSupport->m_OrdreEnteteEch.GetCount());
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
		{
			// Elmt Terrains
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
		{
			// Elmt Cibles
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
		{
			// Elmt Grandeurs
			NbElmtHeader[i] = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();
		}
	}

	// Niveau header terrain
	for (JUnt32 i = 0; pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
		{
			NivDim = i;
			break;
		}
	}

	// Init Index Terrain
	int IndexTerrain;

	// Calcul Index Terrain à afficher
	switch (NivDim)
	{
		case 0 : // Elmt positionné sur Header Niveau 1
		{
			IndexTerrain = ((int)NoCol - (int)m_NbHeaderCol) / (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = (IndexTerrain/2)	% NbElmtHeader[NivDim];
		}
		break;

		case 1 : // Terrain positionné sur Header Row 2
		{
			/*
			IndexTerrain = ((int)NoCol - (int)m_NbHeaderCol) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = IndexTerrain / NbElmtHeader[2];
			*/
			IndexTerrain = (((int)NoCol - (int)m_NbHeaderCol)/2) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = IndexTerrain / NbElmtHeader[2];
		}
		break;

		case 2 : // Terrain positionné sur Header Row 3 
		{
			IndexTerrain = (((int)NoCol - (int)m_NbHeaderCol)/2) % NbElmtHeader[NivDim];
		}
		break;
	}

	// Récupère indice sélection terrain
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	for (JUnt32 i=0; i<IndexTerrain; i++)
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

	// Récupére l'ident terrain
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// Et l'ident terrain
		return pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération Id Cible
JUnt32 CDlgEchelle::RecupIdCible(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'élémt par strate header
	CArray <JUnt32> NbElmtHeader;
	NbElmtHeader.SetSize(pDocChoixSupport->m_OrdreEnteteEch.GetCount());
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 1)
		{
			// Elmt Terrains
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
		{
			// Elmt Cibles
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == 3)
		{
			// Elmt Grandeurs
			NbElmtHeader[i] = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();
		}
	}

	for (JUnt32 i = 0; pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == 2)
		{
			NivDim = i;
			break;
		}
	}

	int IndexCible;

	switch (NivDim)
	{
		case 0 : // Cible positionnée sur Header Niveau 1
		{
			IndexCible = ((int)NoCol - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = (IndexCible/2)	% NbElmtHeader[NivDim];
		}
		break;

		case 1 : // Cible positionnée sur Header Row 2
		{
			/*
			IndexCible = ((int)NoCol - (int)m_NbHeaderCol) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = IndexCible / NbElmtHeader[2];
			*/
			IndexCible = (((int)NoCol - (int)m_NbHeaderCol)/2) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = IndexCible / NbElmtHeader[2];
		}
		break;

		case 2 : // Cible positionnée sur Header Row 3 
		{
			IndexCible = (((int)NoCol - (int)m_NbHeaderCol)/2) % NbElmtHeader[NivDim];
		}
		break;
	}

	// Récupère indice sélection cible
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	for (JUnt32 i=0; i<IndexCible; i++)
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

	// Récupére l'ident cible
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// Et l'ident cible
		return pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération Id Grandeur
JUnt32 CDlgEchelle::RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'élémt par strate header
	CArray <JUnt32> NbElmtHeader;
	NbElmtHeader.SetSize(pDocChoixSupport->m_OrdreEnteteEch.GetCount());
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == IDTERRAIN)
		{
			// Elmt Terrains
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == IDCIBLE)
		{
			// Elmt Cibles
			NbElmtHeader[i] = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount();
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[i] == IDGRANDEUR)
		{
			// Elmt Grandeurs
			NbElmtHeader[i] = pDocChoixSupport->m_MapIdGrandEchSel.GetCount();
		}
	}

	for (JUnt32 i = 0; pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == IDGRANDEUR)
		{
			NivDim = i;
			break;
		}
	}

	int IndexGrandeur;

	switch (NivDim)
	{
		case 0 : // Grandeur positionnée sur Header Niveau 1
		{
			// N° colonne à partir des colonnes fixes header
			JUnt32 NoColGrandeur = (int)NoCol - (int)m_NbHeaderCol;

			// Index grandeur
			IndexGrandeur = (NoColGrandeur/2) / (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexGrandeur = IndexGrandeur % NbElmtHeader[NivDim];
			
			/* Avant
			IndexGrandeur = ((int)NoCol - (int)m_NbHeaderCol) / (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexGrandeur = IndexGrandeur % NbElmtHeader[NivDim];
			*/
		}
		break;

		case 1 : // Grandeur positionnée sur Header Row 2
		{
			// N° colonne à partir des colonnes fixes header
			JUnt32 NoColGrandeur = (int)NoCol - (int)m_NbHeaderCol;

			// Index Grandeur
			IndexGrandeur = (NoColGrandeur/2) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexGrandeur = IndexGrandeur / NbElmtHeader[2];

			/* avant
			IndexGrandeur = ((int)NoCol - (int)m_NbHeaderCol) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexGrandeur = IndexGrandeur / NbElmtHeader[2];
			*/
		}
		break;

		case 2 : // Grandeur positionnée sur Header Row 3 
		{
			// N° colonne à partir des colonnes fixes header
			JUnt32 NoColGrandeur = (int)NoCol - (int)m_NbHeaderCol;

			if (NoColGrandeur % 2 == 0)
			{
				// Index rang
				IndexGrandeur = ((NoColGrandeur/2) % NbElmtHeader[NivDim]) + 1000;
			}
			else
			{
				// Index grandeur
				IndexGrandeur = (NoColGrandeur/2) % NbElmtHeader[NivDim];
			}
		}
		break;
	}

	// Boucle sur la liste ordonnée des grandeurs échelle
	JUnt32 InxGrSel = 0;
	for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 Id = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

		for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
			 pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
			 pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
		{
			JUnt32 ItemSel = pDocChoixSupport->m_MapIdGrandEchSel.GetItem();

			if (pDocChoixSupport->m_MapIdGrandEchSel.GetItem() == Id)
			{
				if (InxGrSel == IndexGrandeur)
					// Et l'ident Grandeur
					return pDocChoixSupport->m_MapIdGrandEchSel.GetItem(); 
				else if (InxGrSel == IndexGrandeur - 1000)
					// Et l'ident Rang Grandeur
					return pDocChoixSupport->m_MapIdGrandEchSel.GetItem() + 1000; 
				else
					InxGrSel++;
			}
		}
	}

	// Pas d'élmt trouvé
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update des résultats au niveau cellule
bool CDlgEchelle::UpdateResultat(JBool CalculResult)
{
	// Debloque modif cellules
	// m_GridEchelle.SetReadOnly(FALSE);
	// Lock mise à jour grid échelle
	m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	int RowCount = m_GridEchelle.GetRowCount();
	int ColCount = m_GridEchelle.GetColCount();

	// Init Affichage Résultats Cellules
	if (m_GridEchelle.GetColCount() > 0 && m_GridEchelle.GetRowCount() > 0)
	{
		m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,m_NbHeaderCol,m_GridEchelle.GetRowCount(),m_GridEchelle.GetColCount()),
									CGXStyle()
										.SetHorizontalAlignment(DT_RIGHT)
										.SetVerticalAlignment(DT_BOTTOM)
										.SetFont(CGXFont()
										.SetSize(pDocChoixSupport->m_SizeFontCell)
										.SetBold(pDocChoixSupport->m_BoldFontCell)));

		if (GrilleResultValid())
		{
			if (CalculResult && pDocChoixSupport->m_ParamEchModif)
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
		m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélections supports
bool CDlgEchelle::UpdateSelSupports()
{
	// Nombre de supports nouvelle sélection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRowHeader();

		// Initialise libellés supports en 1ere colonne
		InitRowSupports();

		// Mise à jour du grid échelle
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur précédemment existant, 
		// et nouvelle liste sans ascenseur !!!
		m_GridEchelle.SetTopRow(m_NbHeaderRow, GX_UPDATENOW); 
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour sélections format supports
bool CDlgEchelle::UpdateSelFormatSupports()
{
	// Active mise à jour grid échelle
	m_GridEchelle.LockUpdate(true);

	// Mise à jour des formats sur les supports sélectionnés
	MajFormatsSupports();

	// Remettre les sélections supports
	UpdateVisuSupports();

	// Mise à jour des résultat coûts
	UpdateResultCout();

	// Mise à jour de la moyenne
	UpdateAfficheMoyenne();

	// On remet le tri à jour
	MajTriCourant();

	// Active mise à jour grid échelle
	m_GridEchelle.LockUpdate(false);

	// reddessine le grid échelle
	m_GridEchelle.Redraw();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
JVoid CDlgEchelle::MajTriCourant()
{
	// Ident de la grandeur utilisée pour moyenne
	JUnt32 IdGrdMoyenne = pDocChoixSupport->GetIdGrandeurMoyenne(pDocChoixSupport->ModuleEch);
	if (IdGrdMoyenne >= 0)
	{
		// Recherche no de la colonne à trier de nouveau
		for (JUnt32 NoCol = m_NoColTarif; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
		{
			// Pas de tri sur les colonnes rang
			// if ((NoCol - m_NbHeaderCol) % 2 != 0)
			if ((NoCol - m_NbHeaderCol) % 2 == 0)
			{
				// Récupère ident grandeur colonne
				JUnt32 NivGrandeur = 2;

				JUnt32 IdGrandeur = RecupIdGrandeur(NoCol, NivGrandeur);
				IdGrandeur = IdGrandeur % 1000;

				/*
				// Attention pour récupérer l'ident grandeur
				JUnt32 NoColGrandeur = NoCol + 1;
				JUnt32 IdGrandeur = RecupIdGrandeur(NoColGrandeur, NivGrandeur);	
				IdGrandeur = IdGrandeur % 1000;
				*/

				// Si c'est la grandeur sélectionné pour le tri et que c'est un coût
				if (IdGrandeur == IdGrdMoyenne) // && GrandeurCout(IdGrandeur))
				{
					// On refait le tri
					m_GridEchelle.SortColonne(NoCol, false);
					break;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Est-ce une grandeur calcul coût
JBool CDlgEchelle::GrandeurCout(JUnt32 IdGrandeur)
{
	// A REDEFINIR dans un type énumération (A FAIRE !!!)
	if (IdGrandeur == 11 || IdGrandeur == 12 || IdGrandeur == 13 || IdGrandeur == 14)
		return true;
	else
		return false;
}	

//////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats sur les supports sélectionnés
JVoid CDlgEchelle::MajFormatsSupports()
{
	// Lockage des modifs
    m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Maj si au moins 1 support dans l'analyse
	if (m_NbHeaderCol >= m_NoColTarif)
	{
		// Index ligne
		JUnt32 InxLig = m_NbHeaderRow;

		// Balaye toutes les lignes associées aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Est ce une famille ???
			JBool ElmtFamille = false;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
				ElmtFamille = true;

			// Attention, pas de format pour les lignes infos
			if (IdSupport > 0)
			{
				// Récupère info formats support (avec espace à droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;

				if (!ElmtFamille)
					// Récupère liste des formats pour le support
  					pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
				else
					// Récupère liste des formats pour la famille
					pDocChoixSupport->GetFormatsFamille(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libellé format du support ou famille
				if (InxSelFormat >= 0)
					// Au moins 1 format possible
					m_GridEchelle.SetStyleRange(CGXRange(InxLig,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
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
					// PAs de format disponible pour ce support
					m_GridEchelle.SetStyleRange(CGXRange(InxLig,m_NoColFormat-1),
												CGXStyle()
													.SetHorizontalAlignment(DT_LEFT)
													.SetVerticalAlignment(DT_VCENTER)
													.SetFont(CGXFont()
														.SetSize(pDocChoixSupport->m_SizeFontHeader)
														.SetBold(pDocChoixSupport->m_BoldFontHeader))
													.SetValue(LibFormatNull),
													gxOverride,0,GX_SMART);
				}

				// Incrémente index ligne
				InxLig++;
			}
		}

		// Mise à jour couleur sélection supports
		UpdateVisuSupports();
	}
	
	// Mise à jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas être modifiées
 	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

//////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour les vues
JVoid CDlgEchelle::UpdateAllViews(const JFCUpdate & Message)
{
	
	// Réinit grille échelle
	if (Message == UPDATE_RAZ)
	{
		// Initialise grid résultat
	    m_GridEchelle.Initialize();
		GXSetNewGridLineMode(); 
		m_GridEchelle.GetParam()->EnableUndo(FALSE);
		m_GridEchelle.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridEchelle.GetParam()->SetNumberedColHeaders(false); 
		m_GridEchelle.GetParam()->SetNumberedRowHeaders(false); 
		m_GridEchelle.GetParam()->SetSmartResize(true);

		// Mise à jour des infos fenêtre échelle
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();
	}

	// Mise à jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise à jour des infos fenêtre échelle
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();
	}

	// Mise a jour suite à changement Offres
	else if (Message == UPDATE_OFFRE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();

		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Mise à jour de la moyenne
		UpdateAfficheMoyenne();

		// On remet le tri à jour
		MajTriCourant();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}

	// Mise a jour suite à changement Terrains 	
	else if (Message == UPDATE_SEL_TERRAIN)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour du grid échelle
		InitGrid();
	}

	// Mise a jour suite à changement Cibles
	else if (Message == UPDATE_SEL_CIBLE)
	{
		// Mise à jour du grid échelle
		InitGrid();
	}

	// Mise a jour suite à changement Segmentss
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise à jour du grid échelle
		InitGrid();
	}

	// Mise a jour suite à changement Formats
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Puis on met à jour le map des formats supports dans échelle
		UpdateFormatSupports();

		// Remet les sélections format dans le grille
		UpdateSelFormatSupports();
	}

	// Mise a jour suite à changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Mise à jour des résultat coûts
		UpdateResultCout();

		// Mise à jour de la moyenne
		UpdateAfficheMoyenne();

		// On remet le tri à jour
		MajTriCourant();

		// Réaffiche les bons formats
		MajFormatsSupports();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}

	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Efface les colonnes innoportunes, on le fait en réaffichant
		UpdateRowHeader();
		UpdateCol();

		// Modifie les infos libellés supports
		UpdateLibelleSupports();

		// Réaffiche le résultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;

		// Mise à jour du bandeau informations
		UpdateTitreAnalyse();

		// Mise à jour de la moyenne
		UpdateAfficheMoyenne();
	}

	///////////////////////////// CHANGEMENT OPTIONS MOTEUR ////////////////////////////////
	//
	else if (Message ==UPDATE_OPTIONSMOTEUR)
	{
		// Modifie les infos libellés supports
		UpdateLibelleSupports();

		// Réaffiche le résultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;

		// Mise à jour de la moyenne
		UpdateAfficheMoyenne();
	}

	// Mise à jour suite à modification ordre entetes header échelle
	else if (Message == UPDATE_ORDRE_ENTETE_ECHELLE)
	{
		// Mise à jour headers rows
		UpdateRowHeader();

		// Inhibe affichage des rangs		
		if (pDocChoixSupport->m_OrdreEnteteEch[m_NbHeaderRow-1] != IDGRANDEUR)
		{
			// Désinhibe affichage rang supports
			pDocChoixSupport->m_AFFRangResultatEch = false;

			// Désinhibe affichage moyenne résultat
			// pDocChoixSupport->m_AFFMoyenneResultatEch = false;
		}
		else
		{
			// Désinhibe affichage rang supports
			pDocChoixSupport->m_AFFRangResultatEch = true;
		}

		// Mise à jour des résultat
		AfficheResultat();
	}

	// Mise à jour suite à modification ordre grandeurs échelle
	else if (Message == UPDATE_ORDRE_GRANDEUR_ECHELLE)
	{
		// Mise à jour headers rows
		UpdateRowHeader();

		// Mise à jour des résultat
		AfficheResultat();
	}

	// Mise à jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Mise à jour des résultat coûts
		UpdateResultCout();

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}

	// Mise à jour suite au changement 
	else if (Message == UPDATE_INSERTIONS)
	{
		// Vire le map courant résultat
		pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleEch);

		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour des entêtes headers car répétition ou reprise en main)
		UpdateRowHeader();

		// Réaffiche le résultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;
	}

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Mise à jour des résultat coûts
        UpdateResultCout();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}

	// Mise à jour sélection support
	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Lock à jour affichage
		m_GridEchelle.LockUpdate(true);

		// Update état checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Lock à jour affichage
		m_GridEchelle.LockUpdate(false);

		// Redraw grille
		m_GridEchelle.Redraw();
	}			

	// Mise à jour suite à modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise à jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise à jour des résultat --> déjà fait dans dialogue générale
		// InitGridResultat();
	}

	// Mise à jour affichage rang résultat
	else if (Message == UPDATE_AFFICH_RANG_RESULT)
	{
		// Mise à jour affichage
		m_GridEchelle.LockUpdate(true);

		// Mise à jour des colonnes rang résultat
		UpdateRangResultat();

		// Update visualisation sélection supports
		UpdateVisuSupports();

		// Retaille les colonnes
		ResizeColonneFixe();

		// Affichage libellé moyenne et ligne moyenne apparente ou non
		UpdateAfficheMoyenne();

		// Mise à jour affichage
		m_GridEchelle.LockUpdate(false);
		m_GridEchelle.Redraw();
	}

	// Mise à jour affichage moyenne selon colonne grandeur sélection
	else if (Message == UPDATE_AFFICH_MOYENNE)
	{
		// Mise à jour affichage
		m_GridEchelle.LockUpdate(true);

		// Affichage libellé moyenne et ligne moyenne apparente ou non
		UpdateAfficheMoyenne();

		// Mise à jour affichage
		m_GridEchelle.LockUpdate(false);
		m_GridEchelle.Redraw();
	}

	// Mise à jour des taris suite à modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Mise à jour des résultat coûts
		UpdateResultCout();

		// Mise à jour résultat (pour les entêtes)
		AfficheResultat();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}

	// Mise a jour visibilité des supports
	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// Affichage ou non des supports
		UpdateAfficheSupports();

		// Affichage libellé moyenne et ligne moyenne apparente ou non
		AfficheLigneMoyenne();
		UpdateAfficheMoyenne();

		// Mise à jour tarif moyen
		AfficheTarifMoyenne();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}
	// Relance tri avec mode tri courant
	else if (Message == UPDATE_TRI_COURANT_ECH)
	{
		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(true);

		// On remet le tri à jour
		MajTriCourant();

		// Active mise à jour grid échelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid échelle
		m_GridEchelle.Redraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des libellés supports dans échelle
void CDlgEchelle::UpdateLibelleSupports()
{
	// on teste la validité du document courant
	if (pDocChoixSupport->IsValid())
	{
		if (m_NbHeaderCol >= m_NoColTarif)
		{
			// Lockage des modifs
			m_GridEchelle.LockUpdate(true);

			// Delock mode read only
			m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

			// Récupère objet regroupement
			CRegroupements * pRegroupements = CRegroupements::GetInstance();

			// Index ligne support
			// JUnt32 InxLigSupp = m_NbHeaderRow;

			// Balaye toutes les lignes associées aux supports
			for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
			{
				// Ident support de la ligne
				JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

				// Ident support
				// JUnt32 IdSupport = pDocChoixSupport->m_MapIdSupportsSel.GetKey();
				if (IdSupport > 0)
				{
					// Element support
					IElem* pSupport = pRegroupements->GetIElemByID(IdSupport);

					// on récupére le libellé support
					JString LibSupport = "-";
					if (pSupport)
					{
						// Affichage libellé support
						LibSupport = pSupport->GetLabel().AsJCharPtr();
					}

					// Affichage libellé support
					CString LibSupp = LibSupport.AsJCharPtr();

					// voir info suppl. si quotidien  (pas pour les couplages)
					if (!pSupport->AsCouplage())
					{
						// Attention si c'est une famille, pas d'infos spécifiques
						CResultEch ResultEchFam;
						CKeyResultEch KeyResultEch;
						pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
						if (!pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
						{
							if (pSupport->AsITitre())
							{
								if (pSupport->AsITitre()->GetPeriodicite() == 1)
								{
									pDocChoixSupport->m_bLDPQtd ? LibSupp += " (LDP)" : LibSupp += " (LNM)";
								}
							}
						}
					}
					
					m_GridEchelle.SetStyleRange(CGXRange(NoLig,m_NoColSupport-1),
												CGXStyle()
													.SetValue(LibSupp));
				}

				// Incrémente index ligne
				// InxLigSupp++;
			}

			// Mise à jour des modifs
			m_GridEchelle.LockUpdate(false);
			m_GridEchelle.Redraw();	

			// On bloque les colonnes qui doivent ne pas être modifiées
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des formats supports dans échelle
void CDlgEchelle::UpdateFormatSupports()
{
	// on teste la validité du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise à jour map des formats supports échelle
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		// Mise à jour des formats par support
		pDocChoixSupport->m_MapIdFormatSpecifEch.Reset();

		// Balaye toutes les lignes associées aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Ident support
			if (IdSupport > 0)
			{
				// Récupére liste des idents formats possibles pour ce support (parmi les formats sélectionnés)
				JMap <JUnt32,JUnt32> MapIdFormats;
				pDocChoixSupport->GetListIdFormats(IdSupport,&MapIdFormats);

				// On prendra le format module pour ce support sauf s'il n'est pas dans la liste des formats support
				MapIdFormats.MoveTo(IdFormat);

				if (MapIdFormats.IsValid())
				{
					// Indique information format associé à ce support
					pDocChoixSupport->m_MapIdFormatSpecifEch.Add(IdSupport) = IdFormat;	
				}
				else
					// Pas d'info format (sera sélectionné ultérieurement via combo box)
					pDocChoixSupport->m_MapIdFormatSpecifEch.Add(IdSupport) = 0;	
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Définit si la grille résultat est valide
bool CDlgEchelle::GrilleResultValid()
{
	if (pDocChoixSupport->m_MapIdSupportsSel.GetCount()  			> 0		&&
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetCount() > 0		&&
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetCount()   > 0		&&
		pDocChoixSupport->m_MapIdGrandEchSel.GetCount()				> 0		&&	 
		pDocChoixSupport->GetMapIdFormatsSel()->GetCount()			== 1	&&
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
JVoid CDlgEchelle::UpdateTitreAnalyse()
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
				//Titre += " - ";
				//Titre += pt->m_Libelle.AsJCharPtr();
			}
			else
			{
				//Titre += " - Aucune source";
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

			/*
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
			*/

			// Ajoute Info Nb Insertions  >> REPORTER EN ENTETE HEADER
			/*
			if (pDocChoixSupport->m_NbInsertions)
			{
				CString TxtIns;
				JUnt32 NbInsertions = pDocChoixSupport->m_NbInsertions;
				if (NbInsertions > 1)
				{
					// Cas analyse avec 1 insertion sur chaque support
					TxtIns.Format("%d %s",pDocChoixSupport->m_NbInsertions,"insertions");
				}
				else
				{
					// Cas analyse avec plus d'une insertion sur chaque support
					TxtIns.Format("%d %s",pDocChoixSupport->m_NbInsertions,"insertion");
				}

				Titre += " - " + TxtIns;
			}
			*/

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
		m_TitreEchelle.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridEchelle.GetRowCount() > 1)
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
			//m_GridEchelle.SetReadOnly(FALSE);
			m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);
			m_GridEchelle.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtMoteur));

			// On bloque les colonnes qui doivent ne pas être modifiées
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}

		// Nombre d'insertions désormais dans le header au dessus de "Supports"
		if (m_GridEchelle.GetRowCount() > 1)
		{
			// Texte inf onb insertions
			CString TxtNbInsertion;
			TxtNbInsertion.Format("%s : %d","Nombre d'insertions",pDocChoixSupport->m_NbInsertions);

			// On l'affiche dans la grille
			//m_GridEchelle.SetReadOnly(FALSE);
			m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);
			m_GridEchelle.SetStyleRange(CGXRange(1,m_NoColSelSupport,1,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtNbInsertion));

			// On bloque les colonnes qui doivent ne pas être modifiées
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Update Résultat des Coûts (suite à modif Format ou Tarif)
JVoid CDlgEchelle::UpdateResultCout()
{
	// Mise à jour fenêtre échelle
	UpdateResultat(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de lignes header
JUnt32 CDlgEchelle::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére nombre de colonnes header
JUnt32 CDlgEchelle::GetNbHeaderCol()
{
	return m_NbHeaderCol;
}


/*---------------------- SCROLL BAR
// Update Grid
m_GridEchelle.UpdateProperties(GX_UPDATENOW,TRUE);

// Ajoute Ascenseur Vertical et Horizontal
m_GridEchelle.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 

// Scrolling automatique
m_GridEchelle.SetAutoScroll(TRUE);
*/

/*---------------------- MERGE DES CELLULES 
// Boucle sur les headers
for (Lig = m_NbHeaderRow-1; Lig >=0;  Lig--)
{
	// Regroupement Header Row Index Lig
	for (int Col = 1; Col <= m_GridEchelle.GetColCount() ; Col += TNbPas.GetAt(Lig))
		m_GridEchelle.SetCoveredCellsRowCol(Lig, Col, Lig, Col + TNbPas.GetAt(Lig)-1 ,GX_UPDATENOW, gxRedo);

	// Update Grille modifiée
	m_GridEchelle.UpdateData(TRUE);
}
*/

void CDlgEchelle::TrierColonne()
{
	// Colonnes à trier
	CRowColArray ArrayCols;
	ROWCOL nCount = m_GridEchelle.GetSelectedCols(ArrayCols);

	// Si aucune colonne sélection, message de sélection
	if (!nCount)
	{
		AfxMessageBox("Attention sélectionner d'abord une colonne pour le tri", MB_ICONINFORMATION);
		return;
	}			
	
	// Specify the sort-information
	CGXSortInfoArray sortInfo;
	sortInfo.SetSize(ArrayCols.GetCount());					// keys only (you can also have more keys)
	for (ROWCOL n = 0; n < nCount; n++)
	{
		sortInfo[0].nRC = ArrayCols[n]; 					// column nCol is the key
		sortInfo[0].sortType = CGXSortInfo::autodetect;		// the grid will determine if the key is a date, numeric or
															// alphanumeric value
		sortInfo[0].sortOrder = CGXSortInfo::ascending;		// sort ascending
	}

	// Call SortRows to let the grid do the sorting
	m_GridEchelle.SortRows(CGXRange().SetTable(), sortInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Mode de tri à appliquer sur une colonne (0 / croissant, 1 /décroissant)
JUnt32 CDlgEchelle::ModeRangColonne(JUnt32 NoCol)
{
	// Niveau header grandeur
	JUnt32 NivGrandeur = 2;

	// Récupère ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur) % 1000;	

	// Résultats en dehors des coûts en mode croissant
	if (IdGrandeur < 11 || IdGrandeur >= 1000 )
		return 0;
	else
		return 1;
}
	

////////////////////////////////////////////////////////////////////////////////////////////////
// Affiche barre de calcul
void CDlgEchelle::AffichBarreCalcul()
{
	// Initialisation Progress bar et visible
	m_ProgressResultEch.ShowWindow(SW_SHOW); 

	// Avance progress bar
	m_ProgressResultEch.SetPos(2);
	m_ProgressResultEch.RedrawWindow(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Export des résultas échelle (Global ou par Cibles)
void CDlgEchelle :: ExportResultat(JBool ParCible)
{
	if (!pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans sélection de support",MB_ICONINFORMATION);
		return;
	}

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
		SaveAsExcel(DlgFile.GetPathName(),ParCible);
	}
	else
	{
		// Création du fichier
		CStdioFile ExportFile;

		// if (!ExportFile.Open(DlgFile.GetFileName( ),CFile::modeCreate | CFile::modeWrite))
		if (!ExportFile.Open(DlgFile.GetPathName(),CFile::modeCreate | CFile::modeWrite))
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
			
		// 2eme bloc : Récupération des résultats visibles du grille échelle
		if (!ParCible)
			// Récupération des résultats
			RecupResultEcran(TxtEntete);
		else
			// Récupération des résultats par cible
			RecupResultEcranParCible(TxtEntete);

		// Ecriture entete dans fichier export
		ExportFile.WriteString(TxtEntete);


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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération de l'entête pour export échelle détaillée
void CDlgEchelle :: RecupEnteteExport(CString &Entete)
{
	// N° Version
	Entete  = "No de version :\t";
	Entete += "1.0\n";

	// Nom du plateau
	Entete += "NOM PLATEAU :\t";
	Entete += pDocChoixSupport->m_KEYPlateau.GetLabel() + "\n";

	// Init entete
	Entete += "EXPORT ECHELLE DETAILLE \n\n";

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


	// Les Terrains utilisés
	Entete += "TERRAINS : \t";

	// Récup la source audience courante
	SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

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
		if (pt)
		{
			Entete += pt->m_Libelle.AsJCharPtr();
			Entete += "\t";
		}
	}
	Entete += "\n";
		
	// ---------------------------------------- Les cibles utilisées -------------------------------------------
	//
	Entete += "CIBLES : \t";
	
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
	Entete += pDocChoixSupport->GetFormatPrincipal(false);
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
	//
	//// Crée la liste des idents contextes sélectionnés
	JSRCTables* m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	//if (NbCtx > 0)
	//{
	//	JList <JInt32> LstCtx;
	//	for(int i= 0; i<NbCtx; i++)
	//	{
	//		pDocChoixSupport->m_MoteurPresse.GetSelCtxLecture(KeyTerrain,i,LstCtx);

	//		// Nom du contexte (attention si Id Contexte = 11 >> voir Jem ????)
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
	for (JUnt32 NoLig = 0; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

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

	// Boucle sur les élmts segment audience sélectionnés pour échelle
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

	str.Format("%d", pDocChoixSupport->m_NbInsertions);
	Entete += str;
	Entete += "\n";

	// --------------------------------------------- Export Info Pop Globale ---------------------------------------
	// --- Spécial REGIE OBS
	CString InfoPopEnsemble = "";
	if (pDocChoixSupport->GetIdSrcAudienceSel())
	{
		// Récup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// Récup la source audience courante
		SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

		// Récup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// Récup la source audience courante
		SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();

		// Uniquement le 1er terrain
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
			if (pt)
			{
				// Calcul effectif ensemble
				CKeyMoteur keyMoteur;
				keyMoteur.Set(SrcID, TerID, 1000000);
				CString TxtEff = "";
				double Effectif = pDocChoixSupport->m_MoteurPresse.GetEffectif(keyMoteur);
				TxtEff.Format("%.0f",Effectif);
				InfoPopEnsemble += "CIBLEREF : \t";
				InfoPopEnsemble += "Ensemble \t";
				InfoPopEnsemble += TxtEff + "\t"; 
			}
			break;
		}
	}

	if (InfoPopEnsemble != "")
	{
		Entete += InfoPopEnsemble;
		Entete += "\n";
	}	

	// Prévoir sauts de ligne avant affichage infos tableurs
	Entete += "\n\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération résultats échelle détaillée
void CDlgEchelle::RecupGrilleExport(CString &Entete)
{
	// Init chaine résultat
	Entete = "";

	// Tableau de lignes et colonnes sélectionnées
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridEchelle.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridEchelle.GetSelectedRows(Rows,false,false);
	
	// Vire les n° colonnes avec indice header info générales
	for (int iCol = Cols.GetCount()-1 ; iCol >= 0; iCol--)
	{
		if (Cols.GetAt(iCol) < m_NoColTarif)
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
	for (int nRow = 0; nRow < m_NbHeaderRow ; nRow++)
	{
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColTarif; nCol++)
		{	
			if (nRow == m_NbHeaderRow-1)
			{
				// Header info support
				Txt = m_GridEchelle.GetValueRowCol(nRow,nCol); 
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

			if (NoCol >= m_NoColTarif)
			{
				Txt = m_GridEchelle.GetValueRowCol(nRow,NoCol); 
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
		JUnt32 IdSupport = atoi(m_GridEchelle.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Ecriture infos générales supports
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColTarif; nCol++)
		{	
			if (nCol != m_NoColFormat-1)
				// Libellé support, ou périodicité ou tarif)
				Txt = m_GridEchelle.GetValueRowCol(NoRow,nCol); 
			else
			{
				// Récupère le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

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
			Txt = m_GridEchelle.GetValueRowCol(NoRow,NoCol); 
			Entete += Txt +  "\t";
		}

		// Saut après chaque ligne support
		Entete += "\n";
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération des résultats visibles du grille échelle
void CDlgEchelle::RecupResultEcran(CString &Entete)
{
	// Bloc n° 2
	Entete = "BLOC 2\n";

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

	// Balayage de toutes les lignes du grille
	for (JUnt32 NoLig = 0; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));
		
		// Si support caché on l'exporte pas
		if (IdSupport > 0 && 
			(pDocChoixSupport->GetSupportVisible(IdSupport) == false ||
				pDocChoixSupport->VisibleSupportMode(IdSupport) == false))
			continue;

		// Pas d'affichage moyenne si non présent présent à l'écran
		if (NoLig >= m_NbHeaderRow && !pDocChoixSupport->m_AFFMoyenneResultatEch && IdSupport == 0)
			continue;

		// Attention si c'est une famille
		JBool ElmtFamille = false;
		CResultEch ResultEchFam;
		CKeyResultEch KeyResultEch;
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// Reconstitue la clef infos famille
			KeyResultEch.m_IdCibleAff		= pDocChoixSupport->m_Offre.m_IdItemAffinite;
			KeyResultEch.m_IdMoteur			= pDocChoixSupport->m_MoteurPresse.GetType();
			KeyResultEch.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch);
			KeyResultEch.m_IdSupport		= IdSupport; 

			// C'est un élmt famille
			ElmtFamille = true;
		}

		// Balayage de toutes les colonnes
		for (JUnt32 NoCol = m_NoColSelSupport-1; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
		{
			// Pas d'affichage pour les 2 eres lignes jusqu'à colonne tarif inclue
			if (NoLig < m_NbHeaderRow-1 && NoCol < m_NoColTarif)
			{
				// Tabulation pour mettre les header en phase
				Entete +="\t";
			}
			// La colonne format support
			else if (NoCol == (m_NoColFormat-1) && IdSupport != 0)
			{
				// Récupère le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

				// libellé format support
				Entete += pDocChoixSupport->GetFormat(IdFormatSupport,true) + "\t";
			}
			// Les colonnes avant la colonne tarif
			else if (NoCol < m_NoColTarif)
			{
				// Ident support de la ligne
				Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol);
				
				// Info famille en +
				if ((NoCol == m_NoColSelSupport-1) && ElmtFamille)
					Entete += "F";

				// enfin tabulation
				Entete += "\t";
			}
			else if ((NoCol - m_NbHeaderCol) % 2 != 0)
			{
				// Attention ici d'indiquer les résultats partiels (pour certaines familles)
				if (ElmtFamille)
				{
					// Finalise la clef infos famille
					KeyResultEch.m_IdCible			= RecupIdCible(NoCol,NivCible); 
					KeyResultEch.m_IdTerrain		= RecupIdTerrain(NoCol,NivTerrain);
					
					// On se positionne sur résultats famille	
					pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
					if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
					{
						// Récupére résultat pour savoir état résultat : partiel ou complet
						ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem();
					
						// Ajoute indicateur résultat partiel
						if(ResultEchFam.m_ResultPartiel)
						{	
							Entete += "*";
						}
					}
				}

				// Les colonnes qui ne sont pas les rangs résultats
				Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
			}
			
			else
			{
				// Les colonnes associées aux rangs résultats
				JUnt32 NivGrandeur = 2;
				JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

				// Recherche état affichage rang résultat colonne
				JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 

				// Affichage avec ou sans le rang des colonnes
				if (EtatAffichRang) // (pDocChoixSupport->m_AFFRangResultatEch && EtatAffichRang)
				{
					if (NoLig < m_NbHeaderRow -1)
					{
						// Au dessus du libellé rang, les libellés des autres headers 
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
					else if (NoLig == m_NbHeaderRow -1)
					{
						// Header niveau 1 > Libellé Rang 
						Entete += "Rang\t";
					}
					else
					{

						// Les colonnes rangs résultats
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
				}
			}
		}

		// Saut de ligne après chaque balayage colonnes
		Entete +="\n";
	}

	// 2 saut de ligne en fin de bloc 2
	Entete +="\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération des résultats visibles du grille échelle par cible
void CDlgEchelle::RecupResultEcranParCible(CString &Entete)
{
	// Bloc n° 2
	Entete = "";

	// Index Header Cible
	JUnt32 InxHeaderCible = pDocChoixSupport->m_OrdreEnteteEch.GetCount()-1;
	for (JUnt32 i = 0; i < pDocChoixSupport->m_OrdreEnteteEch.GetCount(); i++)
	{
		if (pDocChoixSupport->m_OrdreEnteteEch[i] == IDCIBLE)
		{
			InxHeaderCible = i;
			break;
		}
	}

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

	// Liste des idents cibles sélectionnées
	JList<JUnt32> LstIdCiblesSel;
	for (pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext())
	{
		// Stocke ident cible
		LstIdCiblesSel.AddTail() = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}


	// Boucle cibles à exporter
	for (LstIdCiblesSel.MoveFirst();LstIdCiblesSel.IsValid();LstIdCiblesSel.MoveNext())
	{
		// au moins 1 cible sélectionnée
		JUnt32 IdCibleCur = LstIdCiblesSel.GetItem();   

		// Bloc n° 2
		Entete += "BLOC 2\n";
	
		// Balayage de toutes les lignes du grille
		for (JUnt32 NoLig = 0; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Si support caché on l'exporte pas
			if (IdSupport > 0 && 
				(pDocChoixSupport->GetSupportVisible(IdSupport) == false ||
				 pDocChoixSupport->VisibleSupportMode(IdSupport) == false))
				continue;

			// Pas d'affichage moyenne si non présent présent à l'écran
			if (NoLig >= m_NbHeaderRow && !pDocChoixSupport->m_AFFMoyenneResultatEch && IdSupport == 0)
				continue;

			// Attention si c'est une famille
			JBool ElmtFamille = false;
			CResultEch ResultEchFam;
			CKeyResultEch KeyResultEch;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// Reconstitue la clef infos famille
				KeyResultEch.m_IdCibleAff		= pDocChoixSupport->m_Offre.m_IdItemAffinite;
				KeyResultEch.m_IdMoteur			= pDocChoixSupport->m_MoteurPresse.GetType();
				KeyResultEch.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch);
				KeyResultEch.m_IdSupport		= IdSupport; 

				// C'est un élmt famille
				ElmtFamille = true;
			}

			// Balayage de toutes les colonnes
			for (JUnt32 NoCol = m_NoColSelSupport-1; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
			{
				// Pas d'affichage pour les 2 eres lignes jusqu'à colonne tarif inclue
				if (NoLig < m_NbHeaderRow-1 && NoCol < m_NoColTarif)
				{
					// Tabulation pour mettre les header en phase
					Entete +="\t";
				}

				// La colonne format support
				else if (NoCol == (m_NoColFormat-1) && IdSupport != 0)
				{
					// Récupère le format support
					JUnt32 IdFormatSupport = 0;
					pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
					if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
						IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

					// libellé format support
					Entete += pDocChoixSupport->GetFormat(IdFormatSupport,true) + "\t";
				}
				// Les colonnes avant la colonne tarif
				else if (NoCol < m_NoColTarif)
				{
					// Ident support de la ligne
					Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol);

					// Info famille en +
					if ((NoCol == m_NoColSelSupport-1) && ElmtFamille)
						Entete += "F";

					// enfin tabulation
					Entete += "\t";
				}
				else if ((NoCol - m_NbHeaderCol) % 2 != 0)
				{
					// Cible correspondant à cette colonne
					JUnt32 IdCible = RecupIdCible(NoCol,InxHeaderCible);

					// On traite cible par cible les résultats
					if (IdCible == IdCibleCur)
					{

						// Attention ici d'indiquer les résultats partiels (pour certaines familles)
						if (ElmtFamille)
						{
							// Finalise la clef infos famille
							KeyResultEch.m_IdCible			= IdCible; 
							KeyResultEch.m_IdTerrain		= RecupIdTerrain(NoCol,NivTerrain);
							
							// On se positionne sur résultats famille	
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
							if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
							{
								// Récupére résultat pour savoir état résultat : partiel ou complet
								ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem();
							
								// Ajoute indicateur résultat partiel
								if(ResultEchFam.m_ResultPartiel)
								{	
									Entete += "*";
								}
							}
						}

						// Les colonnes qui ne sont pas les rangs résultats
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
				}
				
				else
				{
					// Cible correspondant à cette colonne
					JUnt32 IdCible = RecupIdCible(NoCol,InxHeaderCible);

					// On traite cible par cible les résultats
					if (IdCible == IdCibleCur)
					{
						// Les colonnes associées aux rangs résultats
						JUnt32 NivGrandeur = 2;
						JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

						// Recherche état affichage rang résultat colonne
						JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 

						// Affichage avec ou sans le rang des colonnes
						if (EtatAffichRang) // (pDocChoixSupport->m_AFFRangResultatEch && EtatAffichRang)
						{
							if (NoLig < m_NbHeaderRow -1)
							{
								// Au dessus du libellé rang, les libellés des autres headers 
								Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
							}
							else if (NoLig == m_NbHeaderRow -1)
							{
								// Header niveau 1 > Libellé Rang 
								Entete += "Rang\t";
							}
							else
							{

								// Les colonnes rangs résultats
								Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
							}
						}
					}
				}
			}

			// Saut de ligne après chaque balayage colonnes
			Entete +="\n";
		}

		// 2 saut de ligne en fin de bloc 2
		Entete +="\n\n\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération du bloc infos terrains / cibles
void CDlgEchelle::RecupBlocTerrainCible(CString &Entete)
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

////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour affichage des rangs résultats
JVoid CDlgEchelle::UpdateRangResultat()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Bloque les modifs
	m_GridEchelle.LockUpdate(true);

	// Balayage de toutes les colonnes et init largeur colonne rang (selon mode visible/ pas visible)
	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
	{
		// Les colonnes rang sont devant chaque résultat
		if ((NoCol - m_NbHeaderCol) % 2 == 0)
		{
			// Id Grandeur de cette colonne
			JUnt32 NivGrandeur = 2;
			JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

			// Recherche état affichage rang résultat colonne
			JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 
			if (EtatAffichRang) //  && pDocChoixSupport->m_AFFRangResultatEch)
				// Init largeur rang 
				m_GridEchelle.SetColWidth(NoCol,NoCol,5); 
			else
				// Rend invisible les colonnes rangs
				m_GridEchelle.SetColWidth(NoCol,NoCol,0); 

			// Couleur spécifique cellule rang
			m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,NoCol,m_GridEchelle.GetRowCount(),NoCol),
										CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorRed()));
		}
	}

	// Regroupement des lignes rang et résultat au niveau header
	m_GridEchelle.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
	m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow-1,m_NoColFormat-1,m_NbHeaderRow-1,m_GridEchelle.GetColCount()),
								CGXStyle()
									.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));

	// Mise à jour grille échelle
	m_GridEchelle.UpdateWindow();

	// On bloque les colonnes qui doivent ne pas être modifiées
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du grille échelle
bool CDlgEchelle ::Clipboard(JBool ParCible)
{
	// Sélection globale du grid
	POSITION area = m_GridEchelle.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridEchelle.SetSelection(area, 0, 0, m_GridEchelle.GetRowCount(), m_GridEchelle.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridEchelle.Copy(ParCible); 

	// Déselection globale
	m_GridEchelle.SetSelection(NULL, 0, 0, 0, 0);

	return bRet;
}

// Lancement Macro Excel Impression Echelle
bool CDlgEchelle ::PrintExcel(JBool ParCible)
{
	if(Clipboard(ParCible))
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}

bool  CDlgEchelle ::OpenExcel(JBool ParCible)
{
	if(Clipboard(ParCible))
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.Open( path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}


// Sauvegarde résultat échelle fichier format exel
bool CDlgEchelle ::SaveAsExcel(CString filePath, JBool ParCible)
{
	if(Clipboard(ParCible))
	{
		CExcelPrinter excelPrinter(CATPColors::COLORCHOIXSUPPORTS, this);
		CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		path += "\\MacroPresse.xla";
		excelPrinter.SaveAsFile(filePath, path, "DefinirTypeExportPresse");
		return true;
	}
	return false;
}
