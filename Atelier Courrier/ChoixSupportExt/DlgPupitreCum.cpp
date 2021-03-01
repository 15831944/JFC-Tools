// DlgPupitreCum.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreCum.h"
#include "JATPDocChoixSupport.h"
#include ".\dlgpupitrecum.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgPupitreCum

IMPLEMENT_DYNAMIC(CDlgPupitreCum, JFCDialog)
CDlgPupitreCum::CDlgPupitreCum(CWnd* pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgCumulativite    *pDlg)
	: JFCDialog(CDlgPupitreCum::IDD, pParent)
{
	// R�cup�re le document
	pDocChoixSupport = pDoc;

	// r�cup�re fen�tre de base
	pDlgCum			 = pDlg;

	// Initialise la grille couleur
	m_GridColorCum.OnInitDocument(pDoc,pDlg); 
}

CDlgPupitreCum::~CDlgPupitreCum()
{
}

void CDlgPupitreCum::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_LABEL_COURBES, m_TitreCourbe);
	DDX_Control(pDX, IDC_CS_STATIC_PUPITRE_CUM, m_CadrePupitreCum);
	DDX_Control(pDX, IDC_CS_LIST_GRANDCUM, m_LstGrandCum);
	DDX_Control(pDX, IDC_CS_LABEL_GRANDCUM, m_LblGrandCum);
	DDX_Control(pDX, IDC_CS_LABEL_SEPCOLOR, m_BtnSeparColor);
	DDX_Control(pDX, IDC_CS_LABEL_GROUPCOLOR, m_BtnGroupColor);
}


BEGIN_MESSAGE_MAP(CDlgPupitreCum, JFCDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_CS_LIST_GRANDCUM, OnLbnSelchangeCsListGrandcum)
	ON_BN_CLICKED(IDC_CS_LABEL_SEPCOLOR, OnBnClickedCsLabelSepcolor)
	ON_BN_CLICKED(IDC_CS_LABEL_GROUPCOLOR, OnBnClickedCsLabelGroupcolor)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation fen�tre Pupitre Cumualtivit�
BOOL CDlgPupitreCum::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre Pupitre Cumulativit�
	SetupControls();

	// Positionne choix mode s�lection couleur
	UpdateModeSelColor();
	
	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Pupitre Cumulativit�
void CDlgPupitreCum::SetupControls()
{
	// Association objet / ressource
	m_GridColorCum.SubclassDlgItem(IDC_CS_GRID_COLOR_CUM, this);

	// Propri�t�s du cadre pupitre Cumulativit�
	m_CadrePupitreCum.SetTransparent(false);
	m_CadrePupitreCum.SetSunken(true);

	// Controles en avant plan
	m_CadrePupitreCum.AddTopWindow(m_LblGrandCum);
	m_CadrePupitreCum.AddTopWindow(m_LstGrandCum);
	m_CadrePupitreCum.AddTopWindow(m_TitreCourbe);
	m_CadrePupitreCum.AddTopWindow(m_GridColorCum);
	m_CadrePupitreCum.AddTopWindow(m_BtnSeparColor);
	m_CadrePupitreCum.AddTopWindow(m_BtnGroupColor);
	
	// Style Label Grandeurs
	m_LblGrandCum.SetFontName(CATPColors::GetFontLabelName());
	m_LblGrandCum.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblGrandCum.SetBorder(false);

	// m_LblGrandCum.SetAppColor(CATPColors::COLORCHOIXSUPPORTS);

	// Style Label Courbe
	m_TitreCourbe.SetFontName(CATPColors::GetFontLabelName());
	m_TitreCourbe.SetFontSize(CATPColors::GetFontLabelSize());
	m_TitreCourbe.SetBorder(false);
	// m_TitreCourbe.SetAppColor(CATPColors::COLORCHOIXSUPPORTS);

	// Initialise Le grille des couleurs
    m_GridColorCum.Initialize();
	GXSetNewGridLineMode(); 
    m_GridColorCum.GetParam()->EnableUndo(FALSE);
	m_GridColorCum.GetParam()->SetNumberedColHeaders(false); 
	m_GridColorCum.GetParam()->SetNumberedRowHeaders(false); 
	m_GridColorCum.GetParam()->SetSmartResize(true);

	// Associe le document au grille
	if (m_GridColorCum.OnInitDocument(pDocChoixSupport,pDlgCum) == false)
		AfxMessageBox ("Probl�me association document supports / grille couleur cumulativit�");

	// Mise en place du grid couleur
	InitGridCouleur();

	// D�finition des couleurs contr�les
	SetColors();

	// Fixe relations  fen�tres et position resize
	SetWindowRelations();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation grille s�lection couleur
void CDlgPupitreCum::InitGridCouleur()
{
	// Nb lignes et colonnes (au total 3 x 4 = 12 couleurs)
	JUnt32 NbRow = pDlgCum->GetNbRowColorCum();
	JUnt32 NbCol = pDlgCum->GetNbColColorCum();

	// Enleve n� ligne col 
	m_GridColorCum.GetParam()->SetNumberedColHeaders(false);
	m_GridColorCum.GetParam()->SetNumberedRowHeaders(false);

	// Init grid avec nb colonnes
    m_GridColorCum.SetColCount(NbCol,GX_UPDATENOW);

	// Init Lignes
	m_GridColorCum.SetRowCount(NbRow,GX_UPDATENOW);

	// Debloque modif cellules
	m_GridColorCum.SetReadOnly(FALSE); 

	// Aucune ligne header
	m_GridColorCum.SetFrozenRows(0,0);

	// Aucune colonne header
	m_GridColorCum.SetFrozenCols(0,0);

	// Type et couleur des cellules
	for (JUnt32 NoRow = 1; NoRow <= NbRow; NoRow++)
		for (JUnt32 NoCol = 1; NoCol <= NbCol; NoCol++)
		{
			// Couleur cellule
			COLORREF ColorCell;
			ColorCell = CATPColors::GetColorGrph((NoRow-1)*(NbRow+1) + NoCol-1);
			m_GridColorCum.SetStyleRange(CGXRange(NoRow,NoCol),
								  		 CGXStyle()
											.SetControl(GX_IDS_CTRL_HEADER)
											.SetInterior(ColorCell));
		}	

	// Quadrillage grille
	m_GridColorCum.GetParam()->GetProperties()->SetDisplayHorzLines(TRUE);
	m_GridColorCum.GetParam()->GetProperties()->SetDisplayVertLines(TRUE);

	// Pas d'ascenceur ni vertical ni horizontal
	m_GridColorCum.SetScrollBarMode(SB_VERT,gxnDisabled,TRUE);
	m_GridColorCum.SetScrollBarMode(SB_HORZ,gxnDisabled,TRUE);

	// Grille couleur toujours visible
	m_GridColorCum.HideRows(0,0,TRUE);
	m_GridColorCum.HideCols(0,0,TRUE);

	// Taille des cellules
	m_GridColorCum.SetColWidth(0,NbCol,33);
	m_GridColorCum.SetRowHeight(0,NbRow,12);

	// Rend visible le grid
	m_GridColorCum.ShowWindow(SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Pupitre Cumulativit�
void CDlgPupitreCum::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre Cumulativit�
			{IDC_CS_STATIC_PUPITRE_CUM, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label des grandeurs s�lectionnables
			{IDC_CS_LABEL_GRANDCUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},		//Bottom side

			// Contr�le liste des grandeurs s�lectionnables
			{IDC_CS_LIST_GRANDCUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label des grandeurs s�lectionnables
			{IDC_CS_LABEL_COURBES,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side
			
			// Contr�le surlignage titre courbe
			{IDC_CS_TIRET_TITRE,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Grille couleur r�sultat
			{IDC_CS_GRID_COLOR_CUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Bouton s�lection couleurs s�par�es
			{IDC_CS_LABEL_SEPCOLOR,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},				//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},			//Top side
										{CResizer::eProportional, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},			//Bottom side

			// Bouton s�lection couleurs group�es
			{IDC_CS_LABEL_GROUPCOLOR,	{CResizer::eFixed, IDC_CS_LABEL_SEPCOLOR, CResizer::eRight},//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},			//Top side
										{CResizer::eProportional, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},			//Bottom side

	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fen�tre Pupitre Echelle
void CDlgPupitreCum::SetColors()
{
	// Cadre pupitre cumulativit�
	m_CadrePupitreCum.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Label grandeur
	m_LblGrandCum.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS, CATPColors::DARK_DEGRAD);
	m_LblGrandCum.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Couleur s�lection dans liste grandeurs
	m_LstGrandCum.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));

	// Contr�le Titre Courbe
	m_TitreCourbe.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreCourbe.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Le grille des couleurs
	m_GridColorCum.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));
	m_GridColorCum.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Bouton activation s�lection s�par�es couleurs
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Bouton activation s�lection group�es couleurs
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

}

/////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgPupitreCum::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
		SetColors();
	}

	//////////////////////////////////////// INITIALISATION GENERALE ////////////////////////////////
	//
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise � jour mode s�lection couleur
		UpdateModeSelColor();
	}

	///////////////////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Mise � jour des grandeurs disponibles
		MajGrandeurs();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Resize dz la fen�tre pupitre cumulativit�
void CDlgPupitreCum::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs (attention aux grandeurs invalides pour certains types de calcul)
//
// exemple : pour moteur full binomial pas de GRP+ ni de reprise
//
bool CDlgPupitreCum::LoadGrandeurs()
{
	// Mise � jour des �lmts grandeurs
	if (m_LstGrandCum.GetCount() <= 0)
	{
		// Attention si s�lection choix support non d�fini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleCum);

		// Impossible de continuer si aucune grandeur s�lectionn�e
		if (m_NbGrandeurs == 0) return false;

		// Chargement des grandeurs d�finies
		CString Txt;
		JUnt32 InxList = 0;
		for (JUnt32 i=0; i<m_NbGrandeurs; i++)
		{
			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, i));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((i == pDocChoixSupport->GrdCum_XGRP)		|| 
					(i == pDocChoixSupport->GrdCum_XRepetition)) continue;

			m_LstGrandCum.AddString(Txt);

			// Ajoute ident grandeur
			// m_LstGrandCum.SetItemData(i,i); 
			m_LstGrandCum.SetItemData(InxList,i); 

			// Incr�mente indice liste r�el
			InxList++;
		}

		// Update affichage de la liste
		m_LstGrandCum.UpdateWindow();
		m_LstGrandCum.Invalidate();
	}

	// Puis on s�lectionne par d�faut la 1ere grandeur
	if (m_LstGrandCum.GetCount() > 0 )
	{
		JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(0);

		// Voir si on l'ajoute au map s�lection
		pDocChoixSupport->m_MapIdGrandCumSel.MoveTo(IdGrandeur);
		if (!pDocChoixSupport->m_MapIdGrandCumSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandCumSel.Add(0) = IdGrandeur;

		// S�lection dans liste
		m_LstGrandCum.SetCurSel(0);
	}

	UpdateSelGrandeurs();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour liste grandeurs (attention aux grandeurs invalides pour certains types de calcul)
//
// exemple : pour moteur full binomial pas de GRP+ ni de reprise
//
void CDlgPupitreCum::MajGrandeurs()
{
	// Reset de la liste
	m_LstGrandCum.ResetContent();

	// Nb grandeurs de la liste � remplir
	m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleCum);

	// Impossible de continuer si aucune grandeur s�lectionn�e
	if (m_NbGrandeurs == 0) return;

	// Chargement des grandeurs d�finies
	CString Txt;
	int IndexList = 0;
	for (JUnt32 i=0; i<m_NbGrandeurs; i++)
	{
		// Ajoute libell� grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, i));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((i == pDocChoixSupport->GrdCum_XGRP)	   ||
				(i == pDocChoixSupport->GrdCum_XRepetition))   // Indice GRP+ et R�pet dans liste grandeur cumulativit� 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
				for (pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
					 pDocChoixSupport->m_MapIdGrandCumSel.IsValid();
					 pDocChoixSupport->m_MapIdGrandCumSel.MoveNext())
				{
					JUnt32 ItemData = pDocChoixSupport->m_MapIdGrandCumSel.GetItem();
					if (ItemData == i)
					{
						pDocChoixSupport->m_MapIdGrandCumSel.Remove();
						break;
					}
				}
				continue;
			}
		}

		m_LstGrandCum.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandCum.SetItemData(IndexList,i); 

		// Si �lmt pr�c�demment s�lectionn� on repositionne la s�lection
		for (pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
			 pDocChoixSupport->m_MapIdGrandCumSel.IsValid();
			 pDocChoixSupport->m_MapIdGrandCumSel.MoveNext())
		{
			JUnt32 ItemData = pDocChoixSupport->m_MapIdGrandCumSel.GetItem();
			
			// Appartient au map s�lection
			if (ItemData == i)
			{
				m_LstGrandCum.SetSel(IndexList,true);
				break;
			}
		}

		// Incr�mente indice liste
		IndexList ++; 
	}

	UpdateSelGrandeurs();
}

////////////////////////////////////////////////////////////////////////
// Mise � jour des s�lections grandeurs
void CDlgPupitreCum::UpdateSelGrandeurs()
{
	// D�selectionne toutes les grandeurs
	for (JUnt32 i= 0; i < m_LstGrandCum.GetCount(); i++)
	{
		m_LstGrandCum.SetSel(i,false);
	}

	// Si au moins 1 grandeur s�lectionn�e
	if (pDocChoixSupport->m_MapIdGrandCumSel.GetCount() > 0)
	{
		// Boucle sur les grandeurs s�lectionn�es
		for (pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
			pDocChoixSupport->m_MapIdGrandCumSel.IsValid();
			pDocChoixSupport->m_MapIdGrandCumSel.MoveNext())
		{
			// Boucle sur les grandeurs existantes
			for (JUnt32 i= 0; i < m_LstGrandCum.GetCount(); i++)
			{
				// Ident grandeur
				JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(i);

				// Mise � jour activation ou d�sactivation bouton group�
				UpdateBtnGroup(IdGrandeur);

				if (IdGrandeur == pDocChoixSupport->m_MapIdGrandCumSel.GetItem())
				{
					// S�lection dans liste
					m_LstGrandCum.SetCurSel(i);
					break;
				}
			}
		}
	}
	else
	{
		// par d�faut on s�lectionne le 1er �l�ment
		m_LstGrandCum.SetCurSel(0);
		pDocChoixSupport->m_MapIdGrandEchSel.MoveTo(0);
		if (!pDocChoixSupport->m_MapIdGrandEchSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = true; 

		// Mise � jour activation ou d�sactivation bouton group�
		UpdateBtnGroup(m_LstGrandCum.GetItemData(0));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Activation ou d�sactivation bouton groupement courbe (seul GRP ou Nb Contact cumulable)
void CDlgPupitreCum::UpdateBtnGroup(JUnt32 IdGrandeurSel)
{
	if (IdGrandeurSel == pDocChoixSupport->GrdCum_GRP  ||
		IdGrandeurSel == pDocChoixSupport->GrdCum_XGRP ||
		IdGrandeurSel == pDocChoixSupport->GrdCum_NbContact000)
	{
		// Rend actif le bouton groupement pour GRP et Nb Contact 
		m_BtnGroupColor.EnableWindow(TRUE); 
	}
	else
		// Sinon le rend invalide
		m_BtnGroupColor.EnableWindow(FALSE); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation de la liste
void CDlgPupitreCum::InitLstGrandeurs()
{
	m_LstGrandCum.ResetContent(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Change grandeur � afficher
void CDlgPupitreCum::OnLbnSelchangeCsListGrandcum()
{
	// Modif s�lection grandeurs
	pDocChoixSupport->m_MapIdGrandCumSel.Reset();
	for (JUnt32 i = 0; i < m_LstGrandCum.GetCount(); i++)
	{
		if (m_LstGrandCum.GetSel(i) > 0)
		{
			// R�cup�re identifiant type grandeur
			JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(i);
			pDocChoixSupport->m_MapIdGrandCumSel.Add(pDocChoixSupport->m_MapIdGrandCumSel.GetCount()) = m_LstGrandCum.GetItemData(i); 

			// Active ou d�sactive le mode cumul group� (seul GRP et Nb Contact permettent le cumul group� > valeurs additives)
			UpdateBtnGroup(IdGrandeur);
			
			break;
		}
	}

	// Mise � jour option mode courbe (group�es ssi Grp,Grp+,Nb Contacts)
	MajModeCourbe();

	// Mise � jour des diff�rentes fen�tres 
	pDlgCum->UpdateResultCum(false);

	// Refait le tri sur cumulativit�
	pDocChoixSupport->UpdateAllViews(UPDATE_TRI_COURANT_CUM);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour du mode courbe
void CDlgPupitreCum::MajModeCourbe()
{
	// Test si mode group�
	if (pDocChoixSupport->m_ModeColorGraphe == 1)
	{
		// Test si grandeur cumulable
		JUnt32 IdGrandeur = 0;
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
			IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

		// Attention si grandeur <> Grp, Grp+, Nb Contacts on revient au mode s�par�
		if	(IdGrandeur != pDocChoixSupport->GrdCum_GRP &&
			 IdGrandeur != pDocChoixSupport->GrdCum_NbContact000 &&
			 IdGrandeur != pDocChoixSupport->GrdCum_XGRP)
		{
			// on reste en courbes par support
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);		

			// Modifie variable s�lection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 0;

			// Mise � jour des diff�rentes fen�tres 
			pDlgCum->UpdateResultCum(false);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection mode couleurs s�par�es
void CDlgPupitreCum::OnBnClickedCsLabelSepcolor()
{
	// Validation bouton
	CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);	

	// Modifie variable s�lection couleurs
	pDocChoixSupport->m_ModeColorGraphe = 0;

	// Mise � jour des diff�rentes fen�tres 
	pDlgCum->UpdateResultCum(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection mode couleurs group�es
void CDlgPupitreCum::OnBnClickedCsLabelGroupcolor()
{
	// Test si grandeur cumulable
	JUnt32 IdGrandeur = 0;
	pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
	if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
		IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		////////////////// Pour GRP, GRP +, Nb Contacts, NbContacts+  //////////////////
		// on passe en mode groupage r�sultats
		case pDocChoixSupport->GrdCum_GRP :
		case pDocChoixSupport->GrdCum_NbContact000 :
		case pDocChoixSupport->GrdCum_XGRP : 
		{
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_GROUPCOLOR);		

			// Modifie variable s�lection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 1;

			// Mise	� jour des diff�rentes fen�tres 
			pDlgCum->UpdateResultCum(false);
		}
		break;

		////////////////// Pour tous les autres calculs //////////////////
		// on reste en courbes par support
		default :
		{
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);		

			// Modifie variable s�lection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 0;

			// Mise � jour des diff�rentes fen�tres 
			pDlgCum->UpdateResultCum(false);
		}
		break;
	}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour mode s�lection couleur
void CDlgPupitreCum::UpdateModeSelColor()
{
	if (pDocChoixSupport->m_IdColorCum != 0)
	{
		if (pDocChoixSupport->m_ModeColorGraphe == 0)
			// Mode s�lection couleurs s�par�es
			OnBnClickedCsLabelSepcolor();
		else
			// Mode s�lection couleurs group�es
			OnBnClickedCsLabelGroupcolor();
	}
}