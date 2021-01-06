// DlgEchelle.cpp : fichier d'impl�mentation
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
// Bo�te de dialogue CDlgEchelle
IMPLEMENT_DYNAMIC(CDlgEchelle, JFCDialog)

CDlgEchelle::CDlgEchelle(CWnd* pParent /*=NULL*/,
						 JATPDocChoixSupport *pDoc)
	: JFCDialog(CDlgEchelle::IDD, pParent)
{
	// R�cup�re pointeur s�lection g�n�rale
	pDocChoixSupport = pDoc;

	// Init Lignes/Colonnes Grid
	m_NbHeaderRow  = 3;
	m_NbRowSupport = 0;
	m_NbHeaderCol  = 1;
	m_NbCol        = 0;

	// Init nombre de ligne info suppl�mentaire
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
// Initialisation fen�tre Echelle
BOOL CDlgEchelle::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre Echelles
	SetupControls();

	// 1 / Informations g�n�rales  (Source - Filtre d'audience - Date tarification etc...)
	UpdateTitreAnalyse();

	// Mise en place des relations objets fen�tre Echelles
	SetWindowRelations();

	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Echelle
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

			// Grille R�sultat Echelle
			{IDC_CS_GRID_ECHELLE,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side

			// Barre rpgression R�sultat Echelle
			{IDC_CS_PROGRESS_RESULTECH, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Echelle
void CDlgEchelle::SetupControls()
{
	// Association objet / ressource
	m_GridEchelle.SubclassDlgItem(IDC_CS_GRID_ECHELLE, this);
	
	// Propri�t�s du cadre Echelle
	m_CadreEchelle.SetTransparent(false);
	m_CadreEchelle.SetSunken(true);

	// Controles en avant plan
	m_CadreEchelle.AddTopWindow(m_TitreEchelle);
	m_CadreEchelle.AddTopWindow(m_GridEchelle); 
	m_CadreEchelle.AddTopWindow(m_ProgressResultEch); 

	// Progress bar cach� par d�faut
	m_ProgressResultEch.ShowWindow(SW_HIDE); 
	m_ProgressResultEch.SetShowText(false);

	// Taille et Font des diff�rents labels
	m_TitreEchelle.SetFontName(CATPColors::GetFontLabelName());
    m_TitreEchelle.SetFontBold(TRUE); 
	m_TitreEchelle.SetFontSize(CATPColors::GetFontLabelSize());
	
	// Initialise grid r�sultat
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
	
	// Enregistrement controle sp�cifique combo box pour les formats
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

	// Init R�f�rence colonne
	InitRefGrid();

	// Envoi les infos g�n�rales au grille de base
	if (m_GridEchelle.OnInitDocument(pDocChoixSupport, this, m_NoColIdSupport) == false)
		AfxMessageBox ("Probl�me association document supports / grille �chelle");

	// Mise en place du grid
	InitGrid();

	// Mise en place grid r�sultat
	if (GrilleResultValid())
		InitGridResultat();

	// Affichage des couleurs
	SetColors();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation des r�f�rences colonnes
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

	// Position colonne p�riodicit�
	m_NoColPeriodicite	= m_NoColFormat + 1;

	// Position colonne tarif
	m_NoColTarif		= m_NoColPeriodicite + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fen�tre Echelle
void CDlgEchelle::SetColors()
{
	// Cadre r�sultat �chelle
	m_CadreEchelle.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD); 
	m_ProgressResultEch.SetBarColour(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));

	// Titre �chelle
	m_TitreEchelle.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreEchelle.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Grille r�sultat
	m_GridEchelle.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));
	m_GridEchelle.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Barre de progression
	m_ProgressResultEch.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS));

	// Les diff�rentes couleurs pour headers ou cellules sp�cials
	UpdateColorSpe();

	// Style des lignes (solid pen)
	m_GridEchelle.GetParam()->SetGridLineStyle(PS_SOLID); 

	// Style s�lection bloc (comme Excel)
	// m_GridEchelle.GetParam()->SetExcelLikeSelectionFrame(TRUE);

}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de la fen�tre Echelles
void CDlgEchelle::OnDestroy()
{
	JFCDialog::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Resize la fen�tre Echelles
void CDlgEchelle::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour de la fen�tre Echelles
void CDlgEchelle::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		// Mise � jour du Grid r�sultat Echelles
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

	// Enleve n� ligne col 
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
		UpdateRowHeader();

		// Delock mode read only
		m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

		// Lignes non resizable et nb lignes headers (3 : Terrains / Cibles / Grandeurs)
		m_GridEchelle.SetFrozenRows(m_NbHeaderRow-1,m_NbHeaderRow-1);
		
		// Aucun header colonne et colonnes bloqu�s (support/p�riodicit�/tarif)
		m_GridEchelle.SetFrozenCols(m_NoColTarif - m_NoColIdSupport ,0);
		m_GridEchelle.HideCols(m_NoColIdSupport-1,m_NoColIdSupport-1); 
		// m_GridEchelle.HideCols(m_NoColFormat-1,m_NoColFormat-1); 

		// Affichage entete Tarif et p�riodicit�
		if (m_GridEchelle.GetRowCount() > 2) 
		{

			// Titre "S�l SUPPORTS"
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
											.SetValue(_T("Tarif �")));

			// Titre "PERIODICITE"
			m_GridEchelle.SetStyleRange(CGXRange(2,m_NoColPeriodicite-1),
										CGXStyle()
											.SetValue(_T("P�riodic.")));

			// Case s�lection / d�selection tot supports
			m_GridEchelle.SetStyleRange(CGXRange(1,m_NoColSelSupport-1),
										CGXStyle()
											.SetValue(_T("T")));
		}

		// Mise � jour affichage moyenne (visu ou pas visu)
		UpdateAfficheMoyenne();

		// Merge des cellules d�finitivement vide
		m_GridEchelle.UpdateData(); 

		m_GridEchelle.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,m_NbHeaderRow-2,m_NoColTarif-1),
		 							CGXStyle()
		 							.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE));



		m_GridEchelle.UpdateWindow(); 

		// On bloque les colonnes qui doivent ne pas �tre modifi�es
		m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}

	// Rend visible le grid, si au moins quelques r�sultats
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

	// R�cup�re objet regroupement
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

		// Style texte libell�s tarif
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


		// Style check-box libell�s supports (attention pas les lignes infos)
		m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,
											 m_NoColSelSupport - 1,
											 // InxLigSupp + NbSupports - 2,
											 InxLigSupp + NbSupports - 1,
											 m_NoColSelSupport - 1),
									CGXStyle()
										.SetControl(GX_IDS_CTRL_CHECKBOXEX));

		
		// Style texte libell�s supports avec init
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
	
		// Style texte libell�s p�riodicit� avec init
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

		// Style texte libell�s tarif
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
				IElem* pSupport   = pRegroupements->GetIElemByID(IdSupport);

				// Indique Id Support dans colonne invisible
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdSupport));

				// on r�cup�re le libell� support
				JString LibSupport = "-";

				// Affichage libell� support
				LibSupport = pSupport->GetLabel().AsJCharPtr();

				// Affichage libell� support
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

				// R�cup�re info formats support (avec espace � droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;
  				pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libell� format du support
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

				// Puis indication support s�lectionn� ou pas
				if (EtatSupport)
				{
					// Valide s�lection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// D�valide la s�lection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incr�mente index ligne
				InxLigSupp++;
			}
		}

		// Balaye toutes les lignes associ�es aux familles
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
				JBool EtatFamille = pDocChoixSupport->m_MapIdSupportsSel.GetItem();

				// Element famille
				IElem* pFamille = pRegroupements->GetIElemByID(IdFamille);

				// Indique Id Famille dans colonne invisible
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColIdSupport-1),
											CGXStyle()
												.SetValue(IdFamille));

				// on r�cup�re le libell� famille
				JString LibFamille;
				LibFamille = pFamille->GetLabel().AsJCharPtr();

				// Affichage libell� famille
				m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSupport-1),
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
				// Affichage libell� format de la famille > indique non d�fini
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

				// Puis indication famille s�lectionn� ou pas
				if (EtatFamille)
				{
					// Valide s�lection support
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
				else
				{
					// D�valide la s�lection famille
					m_GridEchelle.SetStyleRange(CGXRange(InxLigSupp,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}

				// Incr�mente index ligne
				InxLigSupp++;
			}
		}

		// Colonne Id Support invisible
		m_GridEchelle.SetColWidth(m_NoColIdSupport-1,m_NoColIdSupport-1,0);

		// Retaille la colonne textes supports
		m_GridEchelle.SetColWidth(m_NoColSelSupport-1,m_NoColSelSupport-1,20);

		// Retaille la colonne p�riodicit� et tarif
		m_GridEchelle.SetColWidth(m_NoColPeriodicite-1,m_NoColTarif-1,60);

		// Retaille supports via max libell�
		/*
		m_GridEchelle.ResizeColWidthsToFit(CGXRange(m_NbHeaderRow, m_NoColSupport-1, m_GridEchelle.GetRowCount(), m_NoColSupport-1),
 				                               TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);
		*/
		m_GridEchelle.SetColWidth(m_NoColSupport-1,m_NoColSupport-1,100);

		// Mise � jour couleur s�lection supports
		UpdateVisuSupports();
	}
	
	// Mise � jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
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
			// puis les colonnes chiffr�es
			m_GridEchelle.ResizeColWidthsToFit(CGXRange(m_NbHeaderRow,NoCol,m_GridEchelle.GetRowCount(),NoCol),
 				                               TRUE,GX_UPDATENOW | GX_NOSHRINKSIZE);
		}	

	}

	// Mise � jour grille
	m_GridEchelle.UpdateWindow(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour affichage lignes avec infos suppl�mentaires
void CDlgEchelle::UpdateRowInfo()
{
	// Lockage des modifs
    m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	////////////////////////////////////////////////////////////////////
	////////////// A FAIRE .............................................
	////////////////////////////////////////////////////////////////////

	// Mise � jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Couleurs sp�ciales de certaines cellules
void CDlgEchelle::UpdateColorSpe()
{
	// Modif ssi au moins 1 colonne r�sultat
	// Cellules info insertion en fond blanc et couleur caract�re diff�rent
	if (m_GridEchelle.GetColCount() >= m_NoColTarif)
	{
		m_GridEchelle.LockUpdate(true); 

		m_GridEchelle.SetStyleRange(CGXRange(0, m_NoColSelSupport-1, 1, m_NoColTarif-1),
									CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS)));

		// Delock mode read only
		m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

		// Niveau header de couleurs diff�rents (via dlg ordre entete)
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
				// Si data exog�ne -- couleur diff�rente
				// R�cup�re identificateur s�lection grandeur
				JUnt32 IdGrandeur = RecupIdGrandeur(Col,0);

				if (IdGrandeur % 1000 >= 100)
				{
					// Data Exog�ne en + fonc�
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
// Mise � jour s�lection supports
void CDlgEchelle::UpdateEtatSelSupports()
{
	// Autorise modif grid et bloque mise � jour
	//m_GridEchelle.SetReadOnly(false);
	m_GridEchelle.LockUpdate(true); 

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Positionne sur info map id support
		pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);

		// Test si support valide
		if (pDocChoixSupport->m_MapIdSupportsSel.IsValid())
		{
			// Etat s�lection en cours
			CGXStyle Style;
			m_GridEchelle.GetStyleRowCol(NoLig, m_NoColSelSupport-1,Style);
			long ValSel = atoi(Style.GetValue()); 
		
			// Puis indication support s�lectionn� ou pas
			if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			{
				// Valide la s�lection support
				if (ValSel != 1)
				{
					m_GridEchelle.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)1));
				}
			}
			else
			{
				// D�valide la s�lection support
				if (ValSel != 0)
				{
					// D�valide la s�lection support
					m_GridEchelle.SetStyleRange(CGXRange(NoLig,m_NoColSelSupport-1),
												CGXStyle()
													.SetValue((long)0));
				}
			}
		}
	}
	
	// Mise � jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Update visualisation supports
void CDlgEchelle::UpdateVisuSupports()
{
	// Debut et fin marquage support s�lectionn�
	JUnt32 NoColDeb = m_NoColSupport-1;    
	JUnt32 NoColFin = m_GridEchelle.GetColCount();  // avant m_NoColTarif-1

	// Autorise modif grid
	//m_GridEchelle.SetReadOnly(false);

	// Lock mise � jour grid �chelle
	m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup �lmt
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Sp�cial ligne infos (en jaune)
		if (IdSupport == 0)
		{
			// Support non s�lectionn�, on laisse la ligne en blanc
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
				// Puis indication support s�lectionn� ou pas
				if (pDocChoixSupport->m_MapIdSupportsSel.GetItem())
				{
					// Support s�lectionn�, on surligne la ligne
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS)));
				}
				else
				{
					// Support non s�lectionn�, on laisse la ligne en blanc
					m_GridEchelle.SetStyleRange(CGXRange(NoLig, NoColDeb, NoLig, NoColFin),
  												CGXStyle()
													.SetInterior(CATPColors::GetColorWhite()));
				}

				// Diff�rencier les familles (soit couleur, soit font)
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

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage libell� moyenne et ligne moyenne apparente ou non
JVoid CDlgEchelle::UpdateAfficheMoyenne()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Update R�sultat
	m_GridEchelle.UpdateData();

	// Index ligne moyenne
	JUnt32 InxLgMoy= 0;

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if (IdSupport == 0)
		{	
			// R�cup�re index ligne moyenne
			InxLgMoy = NoLig;
		}
		else
		{
			// Attention si support non visible
			JBool EtatVisible = (pDocChoixSupport->GetSupportVisible(IdSupport) && pDocChoixSupport->VisibleSupportMode(IdSupport)); 

			// R�cup�ration validit� support sur terrain(s)/segment(s)
			JBool ValideSupport = true;
			if (pElem->IsTitre() || pElem->IsCouplage())
				pDocChoixSupport->ValideSupportSegments(IdSupport);

			// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
			JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(IdSupport);

			// Remet toutes les autres lignes "visibles" � la m�me hauteur
			if (EtatVisible && ValideSupport && VisibleSupport)
				// m_GridEchelle.SetRowHeight(NoLig+1, NoLig+1 ,16); 
				m_GridEchelle.SetRowHeight(NoLig, NoLig ,16); 
			else
				// m_GridEchelle.SetRowHeight(NoLig+1, NoLig+1 ,0); 
				m_GridEchelle.SetRowHeight(NoLig, NoLig ,0); 
		}
	}

	// V�rifie que la grandeur d�finit pour la moyenne est bien dans la s�lection des grandeurs
	if (InxLgMoy >= m_NbHeaderRow)
	{
		// Ident grandeur associ� � la moyenne
		JInt32 IdGrdMoyenne = pDocChoixSupport->GetIdGrandeurMoyenne(pDocChoixSupport->ModuleEch);

		// D�finition du texte asoci� � la moyenne (= Moyenne + Libell� grandeur tri courant)
		CString LibMoyenne = "";

		// Si pas de grandeur s�lectionn�e, ligne moyenne non visible
		/*
		if (IdGrdMoyenne >= 0 
			&&	pDocChoixSupport->IdGrandeurMoyenneValid(pDocChoixSupport->ModuleEch, IdGrdMoyenne)
			&&  pDocChoixSupport->m_AFFMoyenneResultatEch)
		{
			// Grandeur tri d�fini, on affiche la moyenne
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,16); 
			
			// R�cup libell� grandeur associ�
			LibMoyenne.Format("%s %s","Moyenne : ",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrdMoyenne % 1000)); 
		}
		else
		{
			// Pas de grandeur d�fini, la moyenne disparait
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,0); 
		}
		*/

		if (pDocChoixSupport->m_AFFMoyenneResultatEch)
		{
			// Grandeur tri d�fini, on affiche la moyenne
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,16); 

			// R�cup libell� grandeur associ�
			LibMoyenne.Format("%s","Moyenne"); 

			m_GridEchelle.SetFrozenRows(m_NbHeaderRow, m_NbHeaderRow);

		}
		else
		{
			// Pas de grandeur d�fini, la moyenne disparait
			m_GridEchelle.SetRowHeight(InxLgMoy, InxLgMoy ,0); 

			m_GridEchelle.SetFrozenRows(m_NbHeaderRow-1, m_NbHeaderRow-1);
		}

		// Affichage du texte moyenne r�sultats
		m_GridEchelle.SetValueRange(CGXRange(InxLgMoy,m_NoColSupport-1),LibMoyenne); 

		// Mise � jour des donn�es
		m_GridEchelle.UpdateData();

		// Mise � jour grille �chelle
		// m_GridEchelle.UpdateWindow();

		// Mise � jour affichage
		//m_GridEchelle.LockUpdate(false);
		// m_GridEchelle.Redraw();
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas certains supports
JVoid CDlgEchelle::UpdateAfficheSupports()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Update R�sultat
	m_GridEchelle.UpdateData();

	// Recherche index ligne moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// R�cup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = pDocChoixSupport->GetSupportVisible(IdSupport); 

		// R�cup�ration validit� support sur terrain(s)/segment(s)
        JBool ValideSupport = true;
		if (IdSupport != 0)
		{
			if (pElem->IsTitre() || pElem->IsCouplage())
				pDocChoixSupport->ValideSupportSegments(IdSupport);
		}

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
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
// Initialisation du r�sultat �chelle (VOIR OPTIM)
JBool CDlgEchelle::InitGridResultat()
{
	// Mode attente
	CWaitCursor Cw;

	// Attente mise � jour
	m_GridEchelle.LockUpdate(true);

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultEch KeyResultEch;

	// Information r�sultat archiv�
	CResultEch	ResultEch;

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

	// R�cup Bornes pour barre de d�filement
	int nLower, nUpper;
	m_ProgressResultEch.GetRange( nLower, nUpper);

	// Clef affinit� courante
	KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

	// Calcul indice cl� moteur
	KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

	JDate DateExact;

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Par d�faut c'est un support
		JBool ElmtFamille = false;

		// Avance progress bar
		m_ProgressResultEch.SetPos((nUpper-nLower)* (NoLig-m_NbHeaderRow)/pDocChoixSupport->m_MapIdSupportsSel.GetCount());

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention si c'est une famille
		pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
		if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un �lmt famille
			ElmtFamille = true;
		}
		
		// R�cup�re le format support
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

		// R�cup�re Tarif item selon format
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
				// Tarif bien d�fini
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien d�fini (modif la couleur texte)
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else
			// Tarif non d�fini
			m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

		// Ecriture P�riodicit�
		JString TxtPeriodicite		= "";
		JString TxtAbrPeriodicite	= "";
		if (IdSupport)
			pDocChoixSupport->GetPeriodicite(IdSupport,TxtPeriodicite,TxtAbrPeriodicite);

		// Si c'est une famille, il faut que tous les supports de m�me p�riodcit�, sinon indiquer "multiples"
		CString TxtAbrPer;
		if (ElmtFamille)
		{
			TxtAbrPeriodicite = pDocChoixSupport->GetPeriodiciteFamille(IdSupport);
		}

		// Ajoute espace en fin de p�riodicit� abr�g� (pour pr�sentation tableau � droite)
		TxtAbrPer.Format(" %s",TxtAbrPeriodicite.AsJCharPtr()); 
		m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

		// Balayage des colonnes
		for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++) 
		{
			// Recup Id terrain
			JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

			// Recup Id Cible
			JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

			// Cl� de recherche r�sultat
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Test si �l�ment d�j� calcul� ou �lmt moyenn�
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

					// Positionnement moteur pour calcul �lmts affinit�
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
					// Audience affinit�
					JFlt64 AudienceAffinite = -1;

					// Init Moteur (construction distribution)
					// Voir Jeremy avant on n'�tait pas oblig� de tester VolAffinite
					if (pDocChoixSupport->m_MoteurPresse.BuildDistributions(KeyMoteur,Maille) && VolAffinite >= 0)
					{
						// Cr�ation Distribution via cible affinit�
						JFCDistribution DistriAff = CMoteurPresse::GetDistribution(pDocChoixSupport->m_MoteurPresse,VolAffinite); 
					
						// R�cup�re audience
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
					JFlt64 AudienceLDP		= -1.0;		// uniquement affichage colonne LDP et %affinit�
					JFlt64 AudienceProba	= -1.0;		// sert au calcul du GRP
					// Audience
					JFlt64 Audience			= -1.0;
					JFlt64 Audience1Ins		= -1.0;

					// Test si la cible est autoris�e
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
									if(!pElem->IsCouplage() && !ElmtFamille)
									{
										// Audience devient Audience LDP
										Audience = AudienceLDP;

										// Audience affinit� devient Audience LDP affinit�
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
										JFlt64 VolLect1Ins = VolLecture / pDocChoixSupport->m_NbInsertions;

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
					}

					// Calcul R�sultat Colonne (selon grandeur)
					if (NoLig <= m_GridEchelle.GetRowCount() && IdSupport != 0)
					{
						// Calcul r�sultat colonne
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

						// Ajoute dans matrice r�sultat si nouvelle clef r�sultat
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
				// Elment d�j� calcul�, on prend directement les infos dans map r�sultats
				ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem(); 

				// Calcul R�sultat Colonne (selon grandeur)
				CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible, ResultEch, Tarif);
			}					
		}
	}

	// Affiche r�sultat Familles avec moyenne des supports si cumul faminne non demand�
	if (pDocChoixSupport->m_CumulFamille == false)
		AfficheResultFamille();

	// Affiche la moyenne une fois toutes les lignes calcul�s
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

	// rend invisible les colonnes associ�es au rang r�sultat
	UpdateAllViews(UPDATE_AFFICH_RANG_RESULT);
			
	// Mise � jour des s�lections supports
	UpdateVisuSupports();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// Flag calcul effectu�
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

	// Balaye toutes les lignes, sauf les supports et la moyenne
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Attention � la ligne moyenne
		if (IdSupport == 0)
			continue;

		// Voir si c'est un �lmt famille -- R�cup�rer la liste des composants
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if(pElem->IsLot())
		{
			// R�cup tarif famille
			CString	TxtTarif = m_GridEchelle.GetValueRowCol(NoLig,m_NoColTarif-1); 

			// Vire les espaces
			TxtTarif.Replace(" ",""); 

			// Valeur Tarif
			JUnt32 TarifFamille = atof(TxtTarif);
			// JUnt32 TarifFamille = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColTarif-1));

			// Cr�e liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// R�cup�re la liste des titres
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

					// Information r�sultat famille
					CKeyResultEch KeyResultEchFam;
					CResultEch	ResultEchFam;

					// Cl� r�sultat famille
					KeyResultEchFam.m_IdCibleAff	=  pDocChoixSupport->m_Offre.m_IdItemAffinite;
					KeyResultEchFam.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 
					KeyResultEchFam.m_IdMoteur		= pDocChoixSupport->m_MoteurPresse.GetType(); 
					KeyResultEchFam.m_IdCible		= IdCible; 
					KeyResultEchFam.m_IdTerrain		= IdTerrain;
					KeyResultEchFam.m_IdSupport		= IdSupport; 

					// Test si �l�ment d�j� calcul�
					pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEchFam); 
					if (!pDocChoixSupport->m_MapResultatEch.IsValid())
					{	
						// Nb Composants comptabilis�s pour r�sultat famille
						JUnt32 NbCompoValid = 0;

						// Balaye tous les composants familles
						for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())   
						{
							// R�cup composant famille
							JUnt32 IdCompo = ListSup.GetItem(); 

							// Cl� archivage r�sultat
							CKeyResultEch KeyResultEch;

							// R�sultat composant
							CResultEch	ResultEch;

							// Cl� de recherche r�sultat (idem clef famille, seul id support change)
							KeyResultEch				= KeyResultEchFam;
							KeyResultEch.m_IdSupport	= IdCompo; 

							// Test si �l�ment d�j� calcul� ou �lmt moyenn�
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 

							if (pDocChoixSupport->m_MapResultatEch.IsValid())
							{
								// Elment d�j� calcul�, on prend directement les infos dans map r�sultats
								ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem(); 

								// Attention aux r�sultats non d�finis
								if (ResultEch.m_Audience > 0)
								{
									// Cumul r�sultat colonne pour famille
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

						// Positionne Flag r�sultat partiel
						if (NbCompoValid != NbCompoFamille || NbCompoFamille == 0)
						{
							// Ce sont des r�sultats partiels, manque certains supports
							ResultEchFam.m_ResultPartiel = true;
						}

						//Voir si tous les composants famille ont �t� trouv�
						if (NbCompoValid != 0)
						{
							// Ecriture r�sultat famille
							CString Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible,
														ResultEchFam, TarifFamille);

							// Ajoute dans matrice r�sultat si nouvelle clef r�sultat famille
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEchFam); 
							if (!pDocChoixSupport->m_MapResultatEch.IsValid())
							{
								pDocChoixSupport->m_MapResultatEch.Add(KeyResultEchFam) = ResultEchFam; 
							}
						}
						else
						{
							// R�sultat famille non d�finie car manque prise en compte certains composants
							// non d�fini
							m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),"*");
						}
					}
					else
					{
						// Element famille d�j� calcul�, on prend directement les infos dans map r�sultats
						ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem(); 

						// Calcul R�sultat Colonne (selon grandeur)
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

	// Balayage de toutes les lignes r�sultats
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

				// Calcul R�sultat Moyenne sur chaque colonne grandeur
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

	// Nombre de lignes tarif�es
	JUnt32 NbLigneTarif = 0;

	// Balaye toutes les lignes associ�es aux supports
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// Ne pas tenir compte des lignes "non support"
		if (IdSupport > 0)
		{				
			// Ne pas tenir compte des supports cach�s
			if (!pDocChoixSupport->GetSupportVisible(IdSupport) || !pDocChoixSupport->VisibleSupportMode(IdSupport)) continue;
			{
				// R�cup Tarif Support
				TxtTarif = m_GridEchelle.GetValueRowCol(NoLig,m_NoColTarif-1); 

				// Vire les espaces
				TxtTarif.Replace(" ",""); 

				// Valeur Tarif
				JUnt32 Tarif = atof(TxtTarif);
				if (Tarif > 0)
				{
					// Cumul tarif
					CumulTarif += atof(TxtTarif);

					// Incr�mente nb tarif si tarif valide
					NbLigneTarif++;
				}
			}	
		}
		else
			// R�cup�re index ligne moyenne
			InxLgMoy = NoLig;
	}	

	// Affichage du tarif moyenn�
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
// Affichage r�sultat �chelle selon les grandeurs s�lectionn�es
JBool CDlgEchelle::AfficheResultat()
{
	// Cl� archivage r�sultat
	CKeyResultEch KeyResultEch;

	// R�cup niveau de chacune des grandeurs
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

	// Bloque mise � jour
	m_GridEchelle.LockUpdate(true);

	// Ajoute Ascenseur Vertical et Horizontal
	m_GridEchelle.SetScrollBarMode(SB_BOTH ,gxnAutomatic,TRUE); 
	
	// Scrolling automatique
	m_GridEchelle.SetAutoScroll(TRUE);

	// Initialisation Progress bar et visible
	m_ProgressResultEch.ShowWindow(SW_SHOW); 
	int nLower, nUpper;
	m_ProgressResultEch.GetRange( nLower, nUpper);

	// Clef affinit� courante
	KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

	// Calcul indice cl� moteur
	KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);
		
	// Balaye toutes les lignes associ�es aux supports
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
			// C'est un �lmt famille
			ElmtFamille = true;
		}
	
		// R�cup�re le format support
		JUnt32 IdFormatSupport = 0;
		pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
		if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
			IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

		// R�cup�re Tarif item selon format
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


			// Tarif bien d�fini
			// m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			if (!TarifManuel)
			{
				// Tarif bien d�fini
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorBlack()));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}
			else
			{
				// Tarif manuel bien d�fini (modif la couleur texte)
				m_GridEchelle.SetStyleRange(CGXRange(NoLig, m_NoColTarif-1, NoLig, m_NoColTarif-1),
									CGXStyle()
										.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS)));
				m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),TxtTarif); 
			}

		}
		else if (IdSupport > 0)
			// Tarif non d�fini (sauf pour la moyenne)
			m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColTarif-1),"*"); 

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
		m_GridEchelle.SetValueRange(CGXRange(NoLig,m_NoColPeriodicite-1),TxtAbrPer); 

		/*
		// Init les indications r�sultats complets
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

			// Stockage r�sultat dans matrice
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Recherche R�sultat Colonne (selon grandeur)
			CString Txt;

			// Positionne sur r�sultats
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
			if (pDocChoixSupport->m_MapResultatEch.IsValid())
			{
				// Information r�sultat archiv�
				CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();
				Txt = ResultCellule(IdSupport,NoLig, NoCol, false, IdTerrain, IdCible, 
									ResultEch, Tarif);

			}

			// Calcul moyenne des r�sultats
			else if (KeyResultEch.m_IdSupport == 0)
			{
				// Calcul R�sultat des lignes Moyennes (selon grandeur)
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

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();

	// Puis affichage des rangs
	// AfficheRang();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage rang des r�sultats �chelle selon les grandeurs s�lectionn�es
JVoid CDlgEchelle::AfficheRang()
{
	// Cl� archivage r�sultat
	CKeyResultEch KeyResultEch;

	// R�cup niveau de chacune des grandeurs
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

	// Bloque mise � jour
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
		//                           (ordre d�croissant pour les co�ts)
		// if ((NoCol - m_NbHeaderCol) / 2)

		// Si Colonne rang uniquement
		if ((NoCol - m_NbHeaderCol) % 2 != 0) continue;

		// Recup Id terrain
		JUnt32 IdTerrain = RecupIdTerrain(NoCol,NivTerrain);

		// Recup Id Cible
		JUnt32 IdCible = RecupIdCible(NoCol,NivCible);

		// R�cup�re toutes les valeurs dans une liste
		JMap <JUnt32,JFlt64> MapRang;

				// Clef affinit� courante
		KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

		// Calcul indice cl� segment
		KeyResultEch.m_IdSegment		= pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

		// Calcul indice cl� moteur
		KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

		// R�cup�re map des supports valides sur les segments en cours
		JMap <JUnt32,JUnt32> MapSuppSegTerr;
		pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

		// Balaye toutes les lignes associ�es aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// R�cup�re le format support
			JUnt32 IdFormatSupport = 0;
			pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
			if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
				IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

			// Attention si c'est une famille
			JBool ElmtFamille = false;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// C'est un �lmt famille
				ElmtFamille = true;
			}

			// R�cup�re Tarif item selon format
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

			// Stockage r�sultat dans matrice
			KeyResultEch.m_IdCible			= IdCible; 
			KeyResultEch.m_IdTerrain		= IdTerrain;
			KeyResultEch.m_IdSupport		= IdSupport; 

			// Recherche R�sultat Colonne (selon grandeur)
			JFlt64 ValRes = 0.0;

			// Positionne sur r�sultats
			pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch); 
			if (pDocChoixSupport->m_MapResultatEch.IsValid())
			{
				// Ne pas tenir compte des lignes infos (moyennes, etc...)
				if (IdSupport)
				{
					// Information r�sultat archiv�
					CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();

					// Le r�sultat pour la cellule en valeur num�rique
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
				// Pour les co�ts ordre rang invers�
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
					// R�cup�re valeur courant
					JFlt64 &Valeur = MapRang.GetItem();

					// D�cr�mente indice rang si valeur sup�rieur
					if (ModeRangColonne(NoCol) == 0)
					{
						// Cas des valeurs r�sultats en dehors des co�ts
						if (ValCur < Valeur && Valeur >= 0)	IndiceRang++;
					}
					else
					{
						// Cas des valeurs r�sultats en dehors des co�ts
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

	// Mise � jour des colonnes rangs
	UpdateRangResultat();

	// Mise � jour des s�lections supports
	UpdateVisuSupports();

	// Lock � jour affichage
	m_GridEchelle.LockUpdate(true);

	// Retaille via derni�re modification largeur colonne
	ResizeColonneFixe();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Mise � jour affichage
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Formatage des r�sultats selon grandeur
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
	// Par d�faut Texte
	CGXStyle style;
	m_GridEchelle.SetStyleRange(CGXRange(NoLig,NoCol),
								CGXStyle()
									.SetFormat(GX_FMT_TEXT)
									.SetValueType(GX_VT_STRING));

	// Init les indications r�sultats complets
	if (ResultEch.m_ResultPartiel)
	{	
		// On distingue r�sultat partiel cellule
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

	// Chaine format�e par d�faut
	CString TxtResult = "*";

	// Niveau grandeur appliqu�e
	JUnt32 NivGrandeur = 2;

	// R�cup�re ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

	// R�sultat de la cellule
	JFlt64 ResultCell = -1;

	if (ResultMoyenne == false) 
	{
		// R�cup�re valeur r�sultat cellule
		/*
		ResultCell = ResultValCellule(IdSupport,NoLig,NoCol,Tarif,VolLecture,
		 							  AudienceLDP,AudienceProba, Audience,Audience1Ins,
									  Effectif,NbCas,VolLectAff,AudAffLDP,AudAffProba,AudAff,EffAff);
		*/
		ResultCell = ResultValCellule(IdSupport, NoLig, NoCol, Tarif, ResultEch);
	}
	else
	{
		// R�cup�re valeur r�sultat moyenne via map des r�sultats
		ResultCell = MoyenneResultCellule(NoCol, IdTerrain, IdCible);
	}

	// Formatage des r�sultats
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
					// D�coupage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				////////////////////////////// AUDIENCE / CALCUL NB LECTEURS /////////////////
				case pDocChoixSupport->GrdEch_Audience :
				{
					// D�coupage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////// AUDIENCE (000) / CALCUL NB LECTEURS /////////////////
				case pDocChoixSupport->GrdEch_Audience000 :
				{
					// D�coupage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				////////////////////////////// NOMBRE DE CONTACTS ////////////////////////////
				case pDocChoixSupport->GrdEch_NbContact000 :
				{
					// D�coupage chaine par milliers
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
					// Texte chiffre formatt�
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
					// D�coupage chaine par milliers
					TxtResult.Format("%0.f",ResultCell);
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
				case pDocChoixSupport->GrdEch_PrctAff :
				{
					// Calcul pourcentage affinit�
					TxtResult.Format("%0.1f%%",ResultCell);
				}
				break;

				///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
				case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativit�
				{
					TxtResult.Format("%0.f ",ResultCell);	
					//TxtResult.Replace(".",",");
				}
				break;
				

				///////////////////////////// CALCUL COUT / GRP ///////////////////////////////
				case pDocChoixSupport->GrdEch_CoutGRP : 
				{
					// Le calcul doit �tre fait sur la totalit� des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb d�cimales demand�es
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// D�coupage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////////// CALCUL COUT / % COUV ///////////////////////////////
				case pDocChoixSupport->GrdEch_CoutCouv : 
				{
					// Le calcul doit �tre fait sur la totalit� des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb d�cimales demand�es
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// D�coupage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);			}
				break;

				///////////////////////////// CALCUL COUT/1000 en Effectif ////////////////////////
				case pDocChoixSupport->GrdEch_Cout000Ind : 
				{
					// Le calcul doit �tre fait sur la totalit� des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb d�cimales demand�es
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// D�coupage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				///////////////////////////// CALCUL COUT/1000 CONTACTS ////////////////////////////
				case pDocChoixSupport->GrdEch_Cout000Cts : 
				{
					// Le calcul doit �tre fait sur la totalit� des supports (modif pour famille)
					if (ResultCell >= 0) // && !ResultEch.m_ResultPartiel)
					{
						// Affichage selon nb d�cimales demand�es
						if (pDocChoixSupport->m_NbDecCout == 1)
							TxtResult.Format("%0.1f",ResultCell);
						else if (pDocChoixSupport->m_NbDecCout == 2)
							TxtResult.Format("%0.2f",ResultCell);
						else 
							TxtResult.Format("%0.f",ResultCell);
					}

					// D�coupage chaine par milliers
					pDocChoixSupport->FormatString(TxtResult,3);
				}
				break;

				default:
				break;
			}
		}

		// Affichage des r�sultats datas exog�nes
		else
		{
			// R�cup�re r�sultat format� selon type et nb dec data exo
			TxtResult = GetResultDataExo(IdGrandeur, ResultCell);
		}
	}


	// Renvoi la chaine formatt�e
	m_GridEchelle.SetValueRange(CGXRange(NoLig,NoCol),TxtResult);
	return TxtResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re r�sultat format� selon type et nb dec data exo
CString CDlgEchelle::GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell)
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

//////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des r�sultats selon grandeur en valeur r�elle
//  0  > "LNM/LDP(000)" 					-------------------------------- LNM/LDP (000) (Audience) en milliers
//  1  > "Audience"							-------------------------------- audience * effectif / 100
//  2  > "Audience(000)						-------------------------------- idem en milliers
//  3  > "Cts.[+](000)"						-------------------------------- VolLecture (Contacts en milliers)
//  4  > "GRP"   							-------------------------------- (Audience LNM/LDP * 100.0) / Effectif
//  5  > "GRP[+]							-------------------------------- (VolLecture * 100.0) / Effectif
//  6  > "C% cumul�e"						--------------------------------  audience n insertions
//  7  > "R�p�tition ou reprises en main"	-------------------------------- VolLecture / (Effectif * Audience)
//  8  > "R�p�tition +"						-------------------------------- audience n insertions / ???
//  9  > "Indice Affinit�"					-------------------------------- (GRP / GRPAff) * 100.0 
// 10  > "% Affinit�"						-------------------------------- (AudienceLDP / AudAffLDP) * 100.0 
// 11  > "Cumulativit�"						-------------------------------- (Audience n insertions / Audience � 1 insertion)
// 12  > "Co�t/GRP"							-------------------------------- tarif support / GRP
// 13  > "Co�t/1000 effectif"				-------------------------------- tarif support / audience
// 14  > "Co�t/%Couverture					-------------------------------- tarif support / % Couverture
// 15  > "Co�t/1000 contacts"				-------------------------------- tarif support / Nb Contacts

//  ?? > "Audience / Nb Lect"				-------------------------------- audience * effectif / 100
//
JFlt64 CDlgEchelle::ResultValCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif,
									 CResultEch &ResultEch)									 
{
	// R�sultat cellule
	JFlt64 Resultat = -1.0;

	// R�cup�re ident grandeur
	JUnt32 NivGrandeur  = 2;
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		case pDocChoixSupport->GrdEch_AudienceLDP : 
		{
			// R�cup�re audience
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
			// Calcul R�p�tition Normale
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

			// Calcul GRP Affinit�
			JFlt64 GRPAff	= 0.0;
			if (ResultEch.m_EffectifAffinite > 0)
				GRPAff = (ResultEch.m_AudienceAffiniteProba / ResultEch.m_EffectifAffinite) * 100.0;

			if (GRPAff)
			{
				// Calcul indice affinit�
				Resultat = (GRP / GRPAff)*100.0;
			}
		}
		break;

		//////////////////////////////// CALCUL % AFFINITE ///////////////////////////
		case pDocChoixSupport->GrdEch_PrctAff :
		{
			if (ResultEch.m_AudienceAffinite > 0)
			{
				// Calcul pourcentage affinit�
				// Resultat = (AudienceLDP / AudAffLDP) * 100.0;
				Resultat = (ResultEch.m_Audience / ResultEch.m_AudienceAffinite) * 100.0;
			}
		}
		break;

		///////////////////////////// CALCUL INDICE CUMULATIVITE ////////////////////////////
		case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativit�
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

			// Si GRP d�fini, calcul du co�t
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

	// Traitement des donn�es exog�nes (OJD, .....)
	IdGrandeur = IdGrandeur % 1000;
	if (IdGrandeur >= 100)
	{
		// R�cup�re les informations associ�es aux donn�es exog�nes (OJD etc...)
		const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
		const JList <CString>	& LstData = DataExo.GetListNomDataExo(); 

		// Recherche libell� data
		CString &NomDataExo = DataExo.GetLib(IdGrandeur) ;

		///////////////////////////// Valeurs exog�nes ////////////////////////////
		if (!DataExo.GetValeur(NomDataExo,IdSupport,Resultat))
			Resultat = -1;
	}

	return Resultat;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des r�sultats moyenn�s
JFlt64 CDlgEchelle::MoyenneResultCellule(JUnt32 NoCol, JUnt32 IdTerrain, JUnt32 IdCible)
{
	// Nombre de supports valide pour le calcul
	JUnt32 NbSupportCalc = 0;

	// Cumul Num�rateur
	JFlt64 CumNumerateur	= 0.0;

	// Cumul D�nominateur
	JFlt64 CumDenominateur	= 0.0;

	// R�sultat Moyenn�
	JFlt64 Resultat = 0;

	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// Boucle sut toutes les lignes r�sulats �chelle
	for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Cl� recherche r�sultat du support
		CKeyResultEch KeyResultEch;

		// Clef affinit� courante
		KeyResultEch.m_IdCibleAff =  pDocChoixSupport->m_Offre.m_IdItemAffinite;

		// Calcul indice cl� segment
		KeyResultEch.m_IdSegment = pDocChoixSupport->ClefSegment(pDocChoixSupport->ModuleEch); 

		// Calcul indice cl� moteur
		KeyResultEch.m_IdMoteur  = pDocChoixSupport->m_MoteurPresse.GetType(); 

		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

		// On ne comptabilise pas les lignes non visibles
		if (!pDocChoixSupport->GetSupportVisible(IdSupport) || !pDocChoixSupport->VisibleSupportMode(IdSupport)) continue;
		
		// Cl� de recherche r�sultat
		KeyResultEch.m_IdCible			= IdCible; 
		KeyResultEch.m_IdTerrain		= IdTerrain;
		KeyResultEch.m_IdSupport		= IdSupport; 

		// R�cup r�sultat de la lign courante
		pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
		if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
		{
			// R�cup�re r�sultats pour ce support
			CResultEch	&ResultEch = pDocChoixSupport->m_MapResultatEch.GetItem();

			// R�cup�re le format support
			JUnt32 IdFormatSupport = 0;
			pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(KeyResultEch.m_IdSupport);
			if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
				IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

			// Attention si c'est une famille
			JBool ElmtFamille = false;
			pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdSupport);
			if (pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
			{
				// C'est un �lmt famille
				ElmtFamille = true;
			}

			// R�cup�re Tarif item selon format
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

			// R�cup�re ident grandeur
			JUnt32 NivGrandeur  = 2;
			JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

			// Test selon type de r�sultat (calcul� ou direct type data exog�ne)
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
							CumNumerateur	+= (ResultEch.m_Audience / ResultEch.m_Effectif) * 100.0;
							CumDenominateur ++;
						}
					}
					break;

					////////////////////////////// CALCUL REPRISES EN MAIN ///////////////////////
					case pDocChoixSupport->GrdEch_Repetition : 
					{
						// Calcul R�p�tition Normale
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
						// R�p�tition +
						if (ResultEch.m_Audience >= 0)
						{
							// Cumul des �lmts
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

							// Calcul GRP Affinit�
							JFlt64 GRPAff	= 0.0;
							if (ResultEch.m_EffectifAffinite > 0)
								GRPAff = (ResultEch.m_AudienceAffiniteLDP / ResultEch.m_EffectifAffinite) * 100.0;

							if (GRPAff)
							{
								// Cumul des �lmts
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
							// Cumul des �lmts
							CumNumerateur	+= (ResultEch.m_AudienceLDP / ResultEch.m_AudienceAffiniteLDP) * 100.0;
							CumDenominateur ++;
						}
					}
					break;

					///////////////////////////// CALCUL CUMULATIVITE ////////////////////////////
					case pDocChoixSupport->GrdEch_Cumulativite : // Cumulativit�
					{
						if (ResultEch.m_Audience1Ins > 0 && ResultEch.m_Audience >= 0)
						{
							// Cumul des �lmts
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

							// Si GRP d�fini, calcul du co�t
							if (GRP > 0 && Tarif > 0)
							{
								// Cumul des �lmts
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
								// Cumul des �lmts
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
							// Cumul des �lmts
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
							// Cumul des �lmts
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
				////////////////////  Traitement des valeurs data exog�ne (lecture direct de la valeur dans case)//////////////
				//
				CResultEch ResultEch;
				CString Txt = ResultCellule(IdSupport, NoLig, NoCol, false, IdTerrain, IdCible, ResultEch);

				// Remplace "." par "," pour les tris num�riques
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
					// Cumul des �lmts
					CumNumerateur	+= ValCell;
					CumDenominateur ++;
				}
			}	
		}
	}

	if (CumDenominateur > 0)
		// Renvoi r�sultat moyenn�
		return (CumNumerateur / CumDenominateur);
	else
		// Pas de support cumul�, calcul non d�fini
		return -1.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration s�lection Nb Insertions
void CDlgEchelle::SetNbInsertions(JUnt32 NbInsertions)
{
	// Maj nb insertions
	pDocChoixSupport->m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi s�lection Nb Insertions
JUnt32 CDlgEchelle:: GetNbInsertions()
{
	return pDocChoixSupport->m_NbInsertions;
}	

//////////////////////////////////////////////////////////////////////////////////////////////
// Update grille r�sultat via new sel Terrains / cibles ou format
bool CDlgEchelle::UpdateResultEchelle(JBool CalculResult)
{
	// Test validit� s�lection
	if (pDocChoixSupport == NULL)
		return false;

	// Teste si s�lection valide
	if (!pDocChoixSupport->SelectionValid()) return false;
	
	// D�sactive mise � jour grid �chelle
	m_GridEchelle.LockUpdate(true);

	// Mise � jour nombre de colonnes
	if (!UpdateCol()) return false;

	// Mise � jour nombre et headers rows
	if (!UpdateRowHeader()) return false;

	// Lock mise  � jour
	m_GridEchelle.LockUpdate(true);

	// Calcul des r�sultats pour affichage cellules
	if (!UpdateResultat(CalculResult)) return false;

	// Remettre les s�lections supports
	UpdateVisuSupports();

	// Mise � jour de la moyenne
	UpdateAfficheMoyenne();

	// On remet le tri � jour
	MajTriCourant();

	// Retaille via derni�re modification largeur colonne
	// ResizeColonneFixe();

	// Bloque resize hauteur des lignes
	m_GridEchelle.GetParam()->EnableTrackRowHeight(FALSE);
	
	// Rend visible le grid, si au moins quelques r�sultats
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

	// Active mise � jour grid �chelle
	m_GridEchelle.LockUpdate(false);

	// reddessine le grid �chelle
	m_GridEchelle.Redraw();

	// Retaille supports via max libell�
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

		// Nb colonnes total affect� au grid �chelle
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


	// On bloque les colonnes qui doivent ne pas �tre modifi�es
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

				// Texte associ� au header la cellule
				CString Txt;

				if (Col >= m_NbHeaderCol)
				{
					// Texte associ� au header ligne
					Txt = RecupTexteHeaderRow(Lig,Col);

					// Affichage contenu cellule
					m_GridEchelle.SetValueRange(CGXRange(Lig,Col),Txt); 
				}
			}
		}

		// Couleurs sp�ciales
		UpdateColorSpe();

		// Update Grille modifi�e
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

		// Mise � jour des datas r�sultat grille �chelle
		m_GridEchelle.UpdateWindow(); 
		m_GridEchelle.UpdateData(); 
	}

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(TRUE);

	// m_GridEchelle.Redraw();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration texte header row
CString CDlgEchelle::RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col)
{
	// Init chaine libell�
	CString Txt = "-";

	// Nombre d'�l�mt par strate header
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

	// R�cup�re nb s�lections pour chaque liste
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

			// R�cup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// R�cup�re indice s�lection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// R�cup�re infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// R�cup�re libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}

		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]));
			IndexCible = (IndexCible/2)	% NbCibles;

			// R�cup�re indice s�lection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// R�cup�re ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// R�cup libell� 
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

			// R�cup�re identificateur s�lection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,0);

			// R�cup�re libell� abr�g� si ident grandeur trouv�
			// if ((IdGrandeur >= 0 && IdGrandeur < pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch)) || IdGrandeur >= 1000) 
			if ((IdGrandeur >= 0 && IdGrandeur < 1000) || IdGrandeur >= 1000) 
				// Libell� grandeur abr�g�
				Txt.Format("%s",pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur % 1000)); 
			else
				// non d�fini
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

			// R�cup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// R�cup�re indice s�lection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// R�cup�re infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// R�cup�re libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[2])); // % NbCibles;
			IndexCible = (IndexCible /2) % NbCibles ;
			
			// R�cup�re indice s�lection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// R�cup�re ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// R�cup libell� 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
					Txt = pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 3)
		{
			// Ligne header zone grandeurs
			int IndexGrandeur = (((int)Col - (int)m_NbHeaderCol)/(NbElmtHeader[2])) % NbGrandeurs;

			// R�cup�re identificateur s�lection grandeur
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,1);

			// R�cup�re libell� abr�g� si ident grandeur trouv�
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

			// R�cup la source audience courante
			JUnt32 SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

			// R�cup�re indice s�lection terrain
			pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexTerrain;i++)
				pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Et l'ident terrain
				int TerID = pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 

				// R�cup�re infos terrain 
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				const JTerrain * pt				= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// R�cup�re libelle terrain
				if (pt)
					Txt = pt->m_Libelle.AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 2)
		{
			// Ligne header zone cibles
			int IndexCible = ((int)Col - (int)m_NbHeaderCol); // % NbCibles;
			IndexCible = (IndexCible/2) % NbCibles;

			// R�cup�re indice s�lection cible
			pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
			for (JUnt32 i=0;i<IndexCible;i++)
				pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

			if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
			{
				// R�cup�re ident cible
				int IdCible = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 

				// R�cup libell� 
				pDocChoixSupport->m_MAPpItems.MoveTo(IdCible); 
				if (pDocChoixSupport->m_MAPpItems.IsValid()) 
					Txt = pDocChoixSupport->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
			}
		}
		else if (pDocChoixSupport->m_OrdreEnteteEch[Lig] == 3)
		{
			// Ligne header zone grandeurs
			int IndexGrandeur = ((int)Col - (int)m_NbHeaderCol) % NbGrandeurs;

			// R�cup�re identificateur s�lection grandeur
			//JUnt32 IdGrandeur = RecupOrdoIdGrandeur(IndexGrandeur);
			JUnt32 IdGrandeur = RecupIdGrandeur(Col,2);

			// R�cup�re libell� abr�g� si ident grandeur trouv�
			// if ((IdGrandeur >= 0 && IdGrandeur < pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch)) || IdGrandeur >= 1000) 
			if ((IdGrandeur >= 0 && IdGrandeur < 1000) || IdGrandeur >= 1000) 
				Txt.Format("%s",pDocChoixSupport->GetAbrGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur % 1000)); 
			else
				Txt = "*";
		}
	}
	
	// Renvoi libell� trouv�
	return Txt;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Id Terrain
JUnt32 CDlgEchelle::RecupIdTerrain(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'�l�mt par strate header
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

	// Calcul Index Terrain � afficher
	switch (NivDim)
	{
		case 0 : // Elmt positionn� sur Header Niveau 1
		{
			IndexTerrain = ((int)NoCol - (int)m_NbHeaderCol) / (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = (IndexTerrain/2)	% NbElmtHeader[NivDim];
		}
		break;

		case 1 : // Terrain positionn� sur Header Row 2
		{
			/*
			IndexTerrain = ((int)NoCol - (int)m_NbHeaderCol) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = IndexTerrain / NbElmtHeader[2];
			*/
			IndexTerrain = (((int)NoCol - (int)m_NbHeaderCol)/2) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexTerrain = IndexTerrain / NbElmtHeader[2];
		}
		break;

		case 2 : // Terrain positionn� sur Header Row 3 
		{
			IndexTerrain = (((int)NoCol - (int)m_NbHeaderCol)/2) % NbElmtHeader[NivDim];
		}
		break;
	}

	// R�cup�re indice s�lection terrain
	pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	for (JUnt32 i=0; i<IndexTerrain; i++)
		pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveNext();

	// R�cup�re l'ident terrain
	if (pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// Et l'ident terrain
		return pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Id Cible
JUnt32 CDlgEchelle::RecupIdCible(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'�l�mt par strate header
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
		case 0 : // Cible positionn�e sur Header Niveau 1
		{
			IndexCible = ((int)NoCol - (int)m_NbHeaderCol)/(NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = (IndexCible/2)	% NbElmtHeader[NivDim];
		}
		break;

		case 1 : // Cible positionn�e sur Header Row 2
		{
			/*
			IndexCible = ((int)NoCol - (int)m_NbHeaderCol) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = IndexCible / NbElmtHeader[2];
			*/
			IndexCible = (((int)NoCol - (int)m_NbHeaderCol)/2) % (NbElmtHeader[1] * NbElmtHeader[2]);
			IndexCible = IndexCible / NbElmtHeader[2];
		}
		break;

		case 2 : // Cible positionn�e sur Header Row 3 
		{
			IndexCible = (((int)NoCol - (int)m_NbHeaderCol)/2) % NbElmtHeader[NivDim];
		}
		break;
	}

	// R�cup�re indice s�lection cible
	pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	for (JUnt32 i=0; i<IndexCible; i++)
		pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext();

	// R�cup�re l'ident cible
	if (pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// Et l'ident cible
		return pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey(); 
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Id Grandeur
JUnt32 CDlgEchelle::RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim)
{
	// Nombre d'�l�mt par strate header
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
		case 0 : // Grandeur positionn�e sur Header Niveau 1
		{
			// N� colonne � partir des colonnes fixes header
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

		case 1 : // Grandeur positionn�e sur Header Row 2
		{
			// N� colonne � partir des colonnes fixes header
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

		case 2 : // Grandeur positionn�e sur Header Row 3 
		{
			// N� colonne � partir des colonnes fixes header
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

	// Boucle sur la liste ordonn�e des grandeurs �chelle
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

	// Pas d'�lmt trouv�
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Update des r�sultats au niveau cellule
bool CDlgEchelle::UpdateResultat(JBool CalculResult)
{
	// Debloque modif cellules
	// m_GridEchelle.SetReadOnly(FALSE);
	// Lock mise � jour grid �chelle
	m_GridEchelle.LockUpdate(true);

	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	int RowCount = m_GridEchelle.GetRowCount();
	int ColCount = m_GridEchelle.GetColCount();

	// Init Affichage R�sultats Cellules
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
		m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections supports
bool CDlgEchelle::UpdateSelSupports()
{
	// Nombre de supports nouvelle s�lection
	m_NbRowSupport = pDocChoixSupport->m_MapIdSupportsSel.GetCount();  

	if (m_NbRowSupport > 0)
	{
		// Update Grille
		UpdateRowHeader();

		// Initialise libell�s supports en 1ere colonne
		InitRowSupports();

		// Mise � jour du grid �chelle
		InitGrid();

		// On se repositionne sur 1er elmt de la liste (sinon petit soucis si ascenseur pr�c�demment existant, 
		// et nouvelle liste sans ascenseur !!!
		m_GridEchelle.SetTopRow(m_NbHeaderRow, GX_UPDATENOW); 
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour s�lections format supports
bool CDlgEchelle::UpdateSelFormatSupports()
{
	// Active mise � jour grid �chelle
	m_GridEchelle.LockUpdate(true);

	// Mise � jour des formats sur les supports s�lectionn�s
	MajFormatsSupports();

	// Remettre les s�lections supports
	UpdateVisuSupports();

	// Mise � jour des r�sultat co�ts
	UpdateResultCout();

	// Mise � jour de la moyenne
	UpdateAfficheMoyenne();

	// On remet le tri � jour
	MajTriCourant();

	// Active mise � jour grid �chelle
	m_GridEchelle.LockUpdate(false);

	// reddessine le grid �chelle
	m_GridEchelle.Redraw();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
JVoid CDlgEchelle::MajTriCourant()
{
	// Ident de la grandeur utilis�e pour moyenne
	JUnt32 IdGrdMoyenne = pDocChoixSupport->GetIdGrandeurMoyenne(pDocChoixSupport->ModuleEch);
	if (IdGrdMoyenne >= 0)
	{
		// Recherche no de la colonne � trier de nouveau
		for (JUnt32 NoCol = m_NoColTarif; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
		{
			// Pas de tri sur les colonnes rang
			// if ((NoCol - m_NbHeaderCol) % 2 != 0)
			if ((NoCol - m_NbHeaderCol) % 2 == 0)
			{
				// R�cup�re ident grandeur colonne
				JUnt32 NivGrandeur = 2;

				JUnt32 IdGrandeur = RecupIdGrandeur(NoCol, NivGrandeur);
				IdGrandeur = IdGrandeur % 1000;

				/*
				// Attention pour r�cup�rer l'ident grandeur
				JUnt32 NoColGrandeur = NoCol + 1;
				JUnt32 IdGrandeur = RecupIdGrandeur(NoColGrandeur, NivGrandeur);	
				IdGrandeur = IdGrandeur % 1000;
				*/

				// Si c'est la grandeur s�lectionn� pour le tri et que c'est un co�t
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
// Est-ce une grandeur calcul co�t
JBool CDlgEchelle::GrandeurCout(JUnt32 IdGrandeur)
{
	// A REDEFINIR dans un type �num�ration (A FAIRE !!!)
	if (IdGrandeur == 11 || IdGrandeur == 12 || IdGrandeur == 13 || IdGrandeur == 14)
		return true;
	else
		return false;
}	

//////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats sur les supports s�lectionn�s
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

		// Balaye toutes les lignes associ�es aux supports
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
				// R�cup�re info formats support (avec espace � droite)
				CString ListFormat		= "";
				CString LibFormatNull	= "*";
				long InxSelFormat		= -1;

				if (!ElmtFamille)
					// R�cup�re liste des formats pour le support
  					pDocChoixSupport->GetFormatsSupport(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
				else
					// R�cup�re liste des formats pour la famille
					pDocChoixSupport->GetFormatsFamille(IdSupport, InxSelFormat , ListFormat, pDocChoixSupport->ModuleEch);
						
				// Affichage libell� format du support ou famille
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

				// Incr�mente index ligne
				InxLig++;
			}
		}

		// Mise � jour couleur s�lection supports
		UpdateVisuSupports();
	}
	
	// Mise � jour des modifs
	m_GridEchelle.LockUpdate(false);
	m_GridEchelle.Redraw();	

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
 	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

//////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour les vues
JVoid CDlgEchelle::UpdateAllViews(const JFCUpdate & Message)
{
	
	// R�init grille �chelle
	if (Message == UPDATE_RAZ)
	{
		// Initialise grid r�sultat
	    m_GridEchelle.Initialize();
		GXSetNewGridLineMode(); 
		m_GridEchelle.GetParam()->EnableUndo(FALSE);
		m_GridEchelle.SetMergeCellsMode(gxnMergeDelayEval,false);
		m_GridEchelle.GetParam()->SetNumberedColHeaders(false); 
		m_GridEchelle.GetParam()->SetNumberedRowHeaders(false); 
		m_GridEchelle.GetParam()->SetSmartResize(true);

		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();
	}

	// Mise � jour initial
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise � jour des infos fen�tre �chelle
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();
	}

	// Mise a jour suite � changement Offres
	else if (Message == UPDATE_OFFRE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();

		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Mise � jour de la moyenne
		UpdateAfficheMoyenne();

		// On remet le tri � jour
		MajTriCourant();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}

	// Mise a jour suite � changement Terrains 	
	else if (Message == UPDATE_SEL_TERRAIN)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour du grid �chelle
		InitGrid();
	}

	// Mise a jour suite � changement Cibles
	else if (Message == UPDATE_SEL_CIBLE)
	{
		// Mise � jour du grid �chelle
		InitGrid();
	}

	// Mise a jour suite � changement Segmentss
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// Mise � jour du grid �chelle
		InitGrid();
	}

	// Mise a jour suite � changement Formats
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// Puis on met � jour le map des formats supports dans �chelle
		UpdateFormatSupports();

		// Remet les s�lections format dans le grille
		UpdateSelFormatSupports();
	}

	// Mise a jour suite � changement Format d'un support
	else if (Message == UPDATE_SEL_FORMATSUPPORT)
	{
		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour de la moyenne
		UpdateAfficheMoyenne();

		// On remet le tri � jour
		MajTriCourant();

		// R�affiche les bons formats
		MajFormatsSupports();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}

	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Efface les colonnes innoportunes, on le fait en r�affichant
		UpdateRowHeader();
		UpdateCol();

		// Modifie les infos libell�s supports
		UpdateLibelleSupports();

		// R�affiche le r�sultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;

		// Mise � jour du bandeau informations
		UpdateTitreAnalyse();

		// Mise � jour de la moyenne
		UpdateAfficheMoyenne();
	}

	///////////////////////////// CHANGEMENT OPTIONS MOTEUR ////////////////////////////////
	//
	else if (Message ==UPDATE_OPTIONSMOTEUR)
	{
		// Modifie les infos libell�s supports
		UpdateLibelleSupports();

		// R�affiche le r�sultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;

		// Mise � jour de la moyenne
		UpdateAfficheMoyenne();
	}

	// Mise � jour suite � modification ordre entetes header �chelle
	else if (Message == UPDATE_ORDRE_ENTETE_ECHELLE)
	{
		// Mise � jour headers rows
		UpdateRowHeader();

		// Inhibe affichage des rangs		
		if (pDocChoixSupport->m_OrdreEnteteEch[m_NbHeaderRow-1] != IDGRANDEUR)
		{
			// D�sinhibe affichage rang supports
			pDocChoixSupport->m_AFFRangResultatEch = false;

			// D�sinhibe affichage moyenne r�sultat
			// pDocChoixSupport->m_AFFMoyenneResultatEch = false;
		}
		else
		{
			// D�sinhibe affichage rang supports
			pDocChoixSupport->m_AFFRangResultatEch = true;
		}

		// Mise � jour des r�sultat
		AfficheResultat();
	}

	// Mise � jour suite � modification ordre grandeurs �chelle
	else if (Message == UPDATE_ORDRE_GRANDEUR_ECHELLE)
	{
		// Mise � jour headers rows
		UpdateRowHeader();

		// Mise � jour des r�sultat
		AfficheResultat();
	}

	// Mise � jour suite au changement de date tarifaire
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}

	// Mise � jour suite au changement 
	else if (Message == UPDATE_INSERTIONS)
	{
		// Vire le map courant r�sultat
		pDocChoixSupport->ResetResultat(pDocChoixSupport->ModuleEch);

		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour des ent�tes headers car r�p�tition ou reprise en main)
		UpdateRowHeader();

		// R�affiche le r�sultat
		if (this->IsWindowVisible())
			InitGridResultat();
		else
			pDocChoixSupport->m_ParamEchModif = true;
	}

	else if (Message == UPDATE_AFFICH_RESULT)
	{
		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
        UpdateResultCout();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}

	// Mise � jour s�lection support
	else if (Message == UPDATE_SEL_SUPPORTS)
	{
		// Lock � jour affichage
		m_GridEchelle.LockUpdate(true);

		// Update �tat checkbox support
		UpdateEtatSelSupports();

		// Update visualisation supports
		UpdateVisuSupports();

		// Lock � jour affichage
		m_GridEchelle.LockUpdate(false);

		// Redraw grille
		m_GridEchelle.Redraw();
	}			

	// Mise � jour suite � modif moteur
	else if (Message == UPDATE_FILTREAUDIENCE || Message == UPDATE_CTXLECTURE)
	{
		// Mise � jour Titre Entete Analyse
		UpdateTitreAnalyse();

		// Mise � jour des r�sultat --> d�j� fait dans dialogue g�n�rale
		// InitGridResultat();
	}

	// Mise � jour affichage rang r�sultat
	else if (Message == UPDATE_AFFICH_RANG_RESULT)
	{
		// Mise � jour affichage
		m_GridEchelle.LockUpdate(true);

		// Mise � jour des colonnes rang r�sultat
		UpdateRangResultat();

		// Update visualisation s�lection supports
		UpdateVisuSupports();

		// Retaille les colonnes
		ResizeColonneFixe();

		// Affichage libell� moyenne et ligne moyenne apparente ou non
		UpdateAfficheMoyenne();

		// Mise � jour affichage
		m_GridEchelle.LockUpdate(false);
		m_GridEchelle.Redraw();
	}

	// Mise � jour affichage moyenne selon colonne grandeur s�lection
	else if (Message == UPDATE_AFFICH_MOYENNE)
	{
		// Mise � jour affichage
		m_GridEchelle.LockUpdate(true);

		// Affichage libell� moyenne et ligne moyenne apparente ou non
		UpdateAfficheMoyenne();

		// Mise � jour affichage
		m_GridEchelle.LockUpdate(false);
		m_GridEchelle.Redraw();
	}

	// Mise � jour des taris suite � modification tarif
	else if (Message == UPDATE_TARIFS)
	{
		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Mise � jour des r�sultat co�ts
		UpdateResultCout();

		// Mise � jour r�sultat (pour les ent�tes)
		AfficheResultat();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}

	// Mise a jour visibilit� des supports
	else if (Message == UPDATE_VISIBLE_SUPPORTS)
	{
		// Lock mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// Affichage ou non des supports
		UpdateAfficheSupports();

		// Affichage libell� moyenne et ligne moyenne apparente ou non
		AfficheLigneMoyenne();
		UpdateAfficheMoyenne();

		// Mise � jour tarif moyen
		AfficheTarifMoyenne();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}
	// Relance tri avec mode tri courant
	else if (Message == UPDATE_TRI_COURANT_ECH)
	{
		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(true);

		// On remet le tri � jour
		MajTriCourant();

		// Active mise � jour grid �chelle
		m_GridEchelle.LockUpdate(false);

		// reddessine le grid �chelle
		m_GridEchelle.Redraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des libell�s supports dans �chelle
void CDlgEchelle::UpdateLibelleSupports()
{
	// on teste la validit� du document courant
	if (pDocChoixSupport->IsValid())
	{
		if (m_NbHeaderCol >= m_NoColTarif)
		{
			// Lockage des modifs
			m_GridEchelle.LockUpdate(true);

			// Delock mode read only
			m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

			// R�cup�re objet regroupement
			CRegroupements * pRegroupements = CRegroupements::GetInstance();

			// Index ligne support
			// JUnt32 InxLigSupp = m_NbHeaderRow;

			// Balaye toutes les lignes associ�es aux supports
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

					// on r�cup�re le libell� support
					JString LibSupport = "-";
					if (pSupport)
					{
						// Affichage libell� support
						LibSupport = pSupport->GetLabel().AsJCharPtr();
					}

					// Affichage libell� support
					CString LibSupp = LibSupport.AsJCharPtr();

					// voir info suppl. si quotidien  (pas pour les couplages)
					if (!pSupport->AsCouplage())
					{
						// Attention si c'est une famille, pas d'infos sp�cifiques
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

				// Incr�mente index ligne
				// InxLigSupp++;
			}

			// Mise � jour des modifs
			m_GridEchelle.LockUpdate(false);
			m_GridEchelle.Redraw();	

			// On bloque les colonnes qui doivent ne pas �tre modifi�es
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des formats supports dans �chelle
void CDlgEchelle::UpdateFormatSupports()
{
	// on teste la validit� du document courant
	if (pDocChoixSupport->IsValid())
	{
		// Mise � jour map des formats supports �chelle
		JUnt32 IdFormat = 0;
		pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
		if (pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
			IdFormat = pDocChoixSupport->GetMapIdFormatsSel()->GetKey();

		// Mise � jour des formats par support
		pDocChoixSupport->m_MapIdFormatSpecifEch.Reset();

		// Balaye toutes les lignes associ�es aux supports
		for (JUnt32 NoLig = m_NbHeaderRow; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Ident support
			if (IdSupport > 0)
			{
				// R�cup�re liste des idents formats possibles pour ce support (parmi les formats s�lectionn�s)
				JMap <JUnt32,JUnt32> MapIdFormats;
				pDocChoixSupport->GetListIdFormats(IdSupport,&MapIdFormats);

				// On prendra le format module pour ce support sauf s'il n'est pas dans la liste des formats support
				MapIdFormats.MoveTo(IdFormat);

				if (MapIdFormats.IsValid())
				{
					// Indique information format associ� � ce support
					pDocChoixSupport->m_MapIdFormatSpecifEch.Add(IdSupport) = IdFormat;	
				}
				else
					// Pas d'info format (sera s�lectionn� ult�rieurement via combo box)
					pDocChoixSupport->m_MapIdFormatSpecifEch.Add(IdSupport) = 0;	
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// D�finit si la grille r�sultat est valide
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

	// Pas de grille r�sultat d�finie
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Update Entete Analyse (Nom de la source / terrain et date tarification)
JVoid CDlgEchelle::UpdateTitreAnalyse()
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
		m_TitreEchelle.SetText(Titre); 

		// Type de moteur dans le header au dessus de "Supports" (Niveau 1)
		if (m_GridEchelle.GetRowCount() > 1)
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
			//m_GridEchelle.SetReadOnly(FALSE);
			m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);
			m_GridEchelle.SetStyleRange(CGXRange(0,m_NoColSelSupport-1,0,m_NoColTarif-1),
										CGXStyle()
											.SetValue(TxtMoteur));

			// On bloque les colonnes qui doivent ne pas �tre modifi�es
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}

		// Nombre d'insertions d�sormais dans le header au dessus de "Supports"
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

			// On bloque les colonnes qui doivent ne pas �tre modifi�es
			m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Update R�sultat des Co�ts (suite � modif Format ou Tarif)
JVoid CDlgEchelle::UpdateResultCout()
{
	// Mise � jour fen�tre �chelle
	UpdateResultat(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de lignes header
JUnt32 CDlgEchelle::GetNbHeaderRow()
{
	return m_NbHeaderRow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nombre de colonnes header
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

	// Update Grille modifi�e
	m_GridEchelle.UpdateData(TRUE);
}
*/

void CDlgEchelle::TrierColonne()
{
	// Colonnes � trier
	CRowColArray ArrayCols;
	ROWCOL nCount = m_GridEchelle.GetSelectedCols(ArrayCols);

	// Si aucune colonne s�lection, message de s�lection
	if (!nCount)
	{
		AfxMessageBox("Attention s�lectionner d'abord une colonne pour le tri", MB_ICONINFORMATION);
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
// Mode de tri � appliquer sur une colonne (0 / croissant, 1 /d�croissant)
JUnt32 CDlgEchelle::ModeRangColonne(JUnt32 NoCol)
{
	// Niveau header grandeur
	JUnt32 NivGrandeur = 2;

	// R�cup�re ident grandeur
	JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur) % 1000;	

	// R�sultats en dehors des co�ts en mode croissant
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
// Export des r�sultas �chelle (Global ou par Cibles)
void CDlgEchelle :: ExportResultat(JBool ParCible)
{
	if (!pDocChoixSupport->m_MapIdSupportsSel.GetCount())
	{	
		AfxMessageBox("Pas d'export possible sans s�lection de support",MB_ICONINFORMATION);
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
		// Cr�ation du fichier
		CStdioFile ExportFile;

		// if (!ExportFile.Open(DlgFile.GetFileName( ),CFile::modeCreate | CFile::modeWrite))
		if (!ExportFile.Open(DlgFile.GetPathName(),CFile::modeCreate | CFile::modeWrite))
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
			
		// 2eme bloc : R�cup�ration des r�sultats visibles du grille �chelle
		if (!ParCible)
			// R�cup�ration des r�sultats
			RecupResultEcran(TxtEntete);
		else
			// R�cup�ration des r�sultats par cible
			RecupResultEcranParCible(TxtEntete);

		// Ecriture entete dans fichier export
		ExportFile.WriteString(TxtEntete);


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
// R�cup�ration de l'ent�te pour export �chelle d�taill�e
void CDlgEchelle :: RecupEnteteExport(CString &Entete)
{
	// N� Version
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


	// Les Terrains utilis�s
	Entete += "TERRAINS : \t";

	// R�cup la source audience courante
	SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();  

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
		if (pt)
		{
			Entete += pt->m_Libelle.AsJCharPtr();
			Entete += "\t";
		}
	}
	Entete += "\n";
		
	// ---------------------------------------- Les cibles utilis�es -------------------------------------------
	//
	Entete += "CIBLES : \t";
	
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
	Entete += pDocChoixSupport->GetFormatPrincipal(false);
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
	//
	//// Cr�e la liste des idents contextes s�lectionn�s
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

	// Boucle sur les �lmts segment audience s�lectionn�s pour �chelle
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

	str.Format("%d", pDocChoixSupport->m_NbInsertions);
	Entete += str;
	Entete += "\n";

	// --------------------------------------------- Export Info Pop Globale ---------------------------------------
	// --- Sp�cial REGIE OBS
	CString InfoPopEnsemble = "";
	if (pDocChoixSupport->GetIdSrcAudienceSel())
	{
		// R�cup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// R�cup la source audience courante
		SrcID			= pDocChoixSupport->GetIdSrcAudienceSel();  

		// R�cup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// R�cup la source audience courante
		SrcID	= pDocChoixSupport->GetIdSrcAudienceSel();

		// Uniquement le 1er terrain
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

	// Pr�voir sauts de ligne avant affichage infos tableurs
	Entete += "\n\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration r�sultats �chelle d�taill�e
void CDlgEchelle::RecupGrilleExport(CString &Entete)
{
	// Init chaine r�sultat
	Entete = "";

	// Tableau de lignes et colonnes s�lectionn�es
	CRowColArray Rows,Cols;
	ROWCOL NbCols = m_GridEchelle.GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_GridEchelle.GetSelectedRows(Rows,false,false);
	
	// Vire les n� colonnes avec indice header info g�n�rales
	for (int iCol = Cols.GetCount()-1 ; iCol >= 0; iCol--)
	{
		if (Cols.GetAt(iCol) < m_NoColTarif)
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

			// Tabulation apr�s chaque info
			Entete += "\t";
		}

		// Ecriture header r�sultats colonnes
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
		JUnt32 IdSupport = atoi(m_GridEchelle.GetValueRowCol(NoRow,m_NoColIdSupport-1)); 

		// Ecriture infos g�n�rales supports
		for (int nCol = m_NoColSelSupport-1; nCol < m_NoColTarif; nCol++)
		{	
			if (nCol != m_NoColFormat-1)
				// Libell� support, ou p�riodicit� ou tarif)
				Txt = m_GridEchelle.GetValueRowCol(NoRow,nCol); 
			else
			{
				// R�cup�re le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

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
			Txt = m_GridEchelle.GetValueRowCol(NoRow,NoCol); 
			Entete += Txt +  "\t";
		}

		// Saut apr�s chaque ligne support
		Entete += "\n";
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration des r�sultats visibles du grille �chelle
void CDlgEchelle::RecupResultEcran(CString &Entete)
{
	// Bloc n� 2
	Entete = "BLOC 2\n";

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

	// Balayage de toutes les lignes du grille
	for (JUnt32 NoLig = 0; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
	{
		// Ident support de la ligne
		JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));
		
		// Si support cach� on l'exporte pas
		if (IdSupport > 0 && 
			(pDocChoixSupport->GetSupportVisible(IdSupport) == false ||
				pDocChoixSupport->VisibleSupportMode(IdSupport) == false))
			continue;

		// Pas d'affichage moyenne si non pr�sent pr�sent � l'�cran
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

			// C'est un �lmt famille
			ElmtFamille = true;
		}

		// Balayage de toutes les colonnes
		for (JUnt32 NoCol = m_NoColSelSupport-1; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
		{
			// Pas d'affichage pour les 2 eres lignes jusqu'� colonne tarif inclue
			if (NoLig < m_NbHeaderRow-1 && NoCol < m_NoColTarif)
			{
				// Tabulation pour mettre les header en phase
				Entete +="\t";
			}
			// La colonne format support
			else if (NoCol == (m_NoColFormat-1) && IdSupport != 0)
			{
				// R�cup�re le format support
				JUnt32 IdFormatSupport = 0;
				pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
				if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
					IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

				// libell� format support
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
				// Attention ici d'indiquer les r�sultats partiels (pour certaines familles)
				if (ElmtFamille)
				{
					// Finalise la clef infos famille
					KeyResultEch.m_IdCible			= RecupIdCible(NoCol,NivCible); 
					KeyResultEch.m_IdTerrain		= RecupIdTerrain(NoCol,NivTerrain);
					
					// On se positionne sur r�sultats famille	
					pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
					if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
					{
						// R�cup�re r�sultat pour savoir �tat r�sultat : partiel ou complet
						ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem();
					
						// Ajoute indicateur r�sultat partiel
						if(ResultEchFam.m_ResultPartiel)
						{	
							Entete += "*";
						}
					}
				}

				// Les colonnes qui ne sont pas les rangs r�sultats
				Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
			}
			
			else
			{
				// Les colonnes associ�es aux rangs r�sultats
				JUnt32 NivGrandeur = 2;
				JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

				// Recherche �tat affichage rang r�sultat colonne
				JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 

				// Affichage avec ou sans le rang des colonnes
				if (EtatAffichRang) // (pDocChoixSupport->m_AFFRangResultatEch && EtatAffichRang)
				{
					if (NoLig < m_NbHeaderRow -1)
					{
						// Au dessus du libell� rang, les libell�s des autres headers 
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
					else if (NoLig == m_NbHeaderRow -1)
					{
						// Header niveau 1 > Libell� Rang 
						Entete += "Rang\t";
					}
					else
					{

						// Les colonnes rangs r�sultats
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
				}
			}
		}

		// Saut de ligne apr�s chaque balayage colonnes
		Entete +="\n";
	}

	// 2 saut de ligne en fin de bloc 2
	Entete +="\n\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration des r�sultats visibles du grille �chelle par cible
void CDlgEchelle::RecupResultEcranParCible(CString &Entete)
{
	// Bloc n� 2
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

	// Liste des idents cibles s�lectionn�es
	JList<JUnt32> LstIdCiblesSel;
	for (pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
		 pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid();
		 pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveNext())
	{
		// Stocke ident cible
		LstIdCiblesSel.AddTail() = pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}


	// Boucle cibles � exporter
	for (LstIdCiblesSel.MoveFirst();LstIdCiblesSel.IsValid();LstIdCiblesSel.MoveNext())
	{
		// au moins 1 cible s�lectionn�e
		JUnt32 IdCibleCur = LstIdCiblesSel.GetItem();   

		// Bloc n� 2
		Entete += "BLOC 2\n";
	
		// Balayage de toutes les lignes du grille
		for (JUnt32 NoLig = 0; NoLig <= m_GridEchelle.GetRowCount(); NoLig++)
		{
			// Ident support de la ligne
			JUnt32 IdSupport = (JUnt32) atoi(m_GridEchelle.GetValueRowCol(NoLig, m_NoColIdSupport-1));

			// Si support cach� on l'exporte pas
			if (IdSupport > 0 && 
				(pDocChoixSupport->GetSupportVisible(IdSupport) == false ||
				 pDocChoixSupport->VisibleSupportMode(IdSupport) == false))
				continue;

			// Pas d'affichage moyenne si non pr�sent pr�sent � l'�cran
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

				// C'est un �lmt famille
				ElmtFamille = true;
			}

			// Balayage de toutes les colonnes
			for (JUnt32 NoCol = m_NoColSelSupport-1; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
			{
				// Pas d'affichage pour les 2 eres lignes jusqu'� colonne tarif inclue
				if (NoLig < m_NbHeaderRow-1 && NoCol < m_NoColTarif)
				{
					// Tabulation pour mettre les header en phase
					Entete +="\t";
				}

				// La colonne format support
				else if (NoCol == (m_NoColFormat-1) && IdSupport != 0)
				{
					// R�cup�re le format support
					JUnt32 IdFormatSupport = 0;
					pDocChoixSupport->m_MapIdFormatSpecifEch.MoveTo(IdSupport);
					if (pDocChoixSupport->m_MapIdFormatSpecifEch.IsValid())
						IdFormatSupport = pDocChoixSupport->m_MapIdFormatSpecifEch.GetItem();

					// libell� format support
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
					// Cible correspondant � cette colonne
					JUnt32 IdCible = RecupIdCible(NoCol,InxHeaderCible);

					// On traite cible par cible les r�sultats
					if (IdCible == IdCibleCur)
					{

						// Attention ici d'indiquer les r�sultats partiels (pour certaines familles)
						if (ElmtFamille)
						{
							// Finalise la clef infos famille
							KeyResultEch.m_IdCible			= IdCible; 
							KeyResultEch.m_IdTerrain		= RecupIdTerrain(NoCol,NivTerrain);
							
							// On se positionne sur r�sultats famille	
							pDocChoixSupport->m_MapResultatEch.MoveTo(KeyResultEch);
							if (pDocChoixSupport->m_MapResultatEch.IsValid()) 
							{
								// R�cup�re r�sultat pour savoir �tat r�sultat : partiel ou complet
								ResultEchFam = pDocChoixSupport->m_MapResultatEch.GetItem();
							
								// Ajoute indicateur r�sultat partiel
								if(ResultEchFam.m_ResultPartiel)
								{	
									Entete += "*";
								}
							}
						}

						// Les colonnes qui ne sont pas les rangs r�sultats
						Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
					}
				}
				
				else
				{
					// Cible correspondant � cette colonne
					JUnt32 IdCible = RecupIdCible(NoCol,InxHeaderCible);

					// On traite cible par cible les r�sultats
					if (IdCible == IdCibleCur)
					{
						// Les colonnes associ�es aux rangs r�sultats
						JUnt32 NivGrandeur = 2;
						JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

						// Recherche �tat affichage rang r�sultat colonne
						JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 

						// Affichage avec ou sans le rang des colonnes
						if (EtatAffichRang) // (pDocChoixSupport->m_AFFRangResultatEch && EtatAffichRang)
						{
							if (NoLig < m_NbHeaderRow -1)
							{
								// Au dessus du libell� rang, les libell�s des autres headers 
								Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
							}
							else if (NoLig == m_NbHeaderRow -1)
							{
								// Header niveau 1 > Libell� Rang 
								Entete += "Rang\t";
							}
							else
							{

								// Les colonnes rangs r�sultats
								Entete += m_GridEchelle.GetValueRowCol(NoLig, NoCol) + "\t";
							}
						}
					}
				}
			}

			// Saut de ligne apr�s chaque balayage colonnes
			Entete +="\n";
		}

		// 2 saut de ligne en fin de bloc 2
		Entete +="\n\n\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du bloc infos terrains / cibles
void CDlgEchelle::RecupBlocTerrainCible(CString &Entete)
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

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour affichage des rangs r�sultats
JVoid CDlgEchelle::UpdateRangResultat()
{
	// Delock mode read only
	m_GridEchelle.GetParam()->SetLockReadOnly(FALSE);

	// Bloque les modifs
	m_GridEchelle.LockUpdate(true);

	// Balayage de toutes les colonnes et init largeur colonne rang (selon mode visible/ pas visible)
	for (JUnt32 NoCol = m_NbHeaderCol; NoCol <= m_GridEchelle.GetColCount(); NoCol++)
	{
		// Les colonnes rang sont devant chaque r�sultat
		if ((NoCol - m_NbHeaderCol) % 2 == 0)
		{
			// Id Grandeur de cette colonne
			JUnt32 NivGrandeur = 2;
			JUnt32 IdGrandeur = RecupIdGrandeur(NoCol,NivGrandeur);	

			// Recherche �tat affichage rang r�sultat colonne
			JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 
			if (EtatAffichRang) //  && pDocChoixSupport->m_AFFRangResultatEch)
				// Init largeur rang 
				m_GridEchelle.SetColWidth(NoCol,NoCol,5); 
			else
				// Rend invisible les colonnes rangs
				m_GridEchelle.SetColWidth(NoCol,NoCol,0); 

			// Couleur sp�cifique cellule rang
			m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow,NoCol,m_GridEchelle.GetRowCount(),NoCol),
										CGXStyle()
										.SetInterior(CATPColors::GetColorWhite())
										.SetTextColor(CATPColors::GetColorRed()));
		}
	}

	// Regroupement des lignes rang et r�sultat au niveau header
	m_GridEchelle.SetMergeCellsMode(gxnMergeEvalOnDisplay,false);
	m_GridEchelle.SetStyleRange(CGXRange(m_NbHeaderRow-1,m_NoColFormat-1,m_NbHeaderRow-1,m_GridEchelle.GetColCount()),
								CGXStyle()
									.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE));

	// Mise � jour grille �chelle
	m_GridEchelle.UpdateWindow();

	// On bloque les colonnes qui doivent ne pas �tre modifi�es
	m_GridEchelle.SetColReadOnly(m_NbHeaderRow, m_NoColIdSupport, m_NoColSupport, m_NoColPeriodicite);
}

///////////////////////////////////////////////////////////////////////////////////////
// Clipboard du grille �chelle
bool CDlgEchelle ::Clipboard(JBool ParCible)
{
	// S�lection globale du grid
	POSITION area = m_GridEchelle.GetParam( )->GetRangeList( )->AddTail(new CGXRange);
	m_GridEchelle.SetSelection(area, 0, 0, m_GridEchelle.GetRowCount(), m_GridEchelle.GetColCount());

	// Copie dans clipboard
	bool bRet = m_GridEchelle.Copy(ParCible); 

	// D�selection globale
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


// Sauvegarde r�sultat �chelle fichier format exel
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
