// DlgPupitreCum.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreCum.h"
#include "JATPDocChoixSupport.h"
#include ".\dlgpupitrecum.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Boîte de dialogue CDlgPupitreCum

IMPLEMENT_DYNAMIC(CDlgPupitreCum, JFCDialog)
CDlgPupitreCum::CDlgPupitreCum(CWnd* pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgCumulativite    *pDlg)
	: JFCDialog(CDlgPupitreCum::IDD, pParent)
{
	// Récupère le document
	pDocChoixSupport = pDoc;

	// récupère fenêtre de base
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
// Initialisation fenêtre Pupitre Cumualtivité
BOOL CDlgPupitreCum::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fenêtre Pupitre Cumulativité
	SetupControls();

	// Positionne choix mode sélection couleur
	UpdateModeSelColor();
	
	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contrôles fenêtre Pupitre Cumulativité
void CDlgPupitreCum::SetupControls()
{
	// Association objet / ressource
	m_GridColorCum.SubclassDlgItem(IDC_CS_GRID_COLOR_CUM, this);

	// Propriétés du cadre pupitre Cumulativité
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
		AfxMessageBox ("Problème association document supports / grille couleur cumulativité");

	// Mise en place du grid couleur
	InitGridCouleur();

	// Définition des couleurs contrôles
	SetColors();

	// Fixe relations  fenêtres et position resize
	SetWindowRelations();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation grille sélection couleur
void CDlgPupitreCum::InitGridCouleur()
{
	// Nb lignes et colonnes (au total 3 x 4 = 12 couleurs)
	JUnt32 NbRow = pDlgCum->GetNbRowColorCum();
	JUnt32 NbCol = pDlgCum->GetNbColColorCum();

	// Enleve n° ligne col 
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
// Mise en place des relations entre objets de la fenêtre Pupitre Cumulativité
void CDlgPupitreCum::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre Cumulativité
			{IDC_CS_STATIC_PUPITRE_CUM, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contrôle label des grandeurs sélectionnables
			{IDC_CS_LABEL_GRANDCUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},		//Bottom side

			// Contrôle liste des grandeurs sélectionnables
			{IDC_CS_LIST_GRANDCUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contrôle label des grandeurs sélectionnables
			{IDC_CS_LABEL_COURBES,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side
			
			// Contrôle surlignage titre courbe
			{IDC_CS_TIRET_TITRE,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Grille couleur résultat
			{IDC_CS_GRID_COLOR_CUM,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Bouton sélection couleurs séparées
			{IDC_CS_LABEL_SEPCOLOR,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},				//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},			//Top side
										{CResizer::eProportional, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},			//Bottom side

			// Bouton sélection couleurs groupées
			{IDC_CS_LABEL_GROUPCOLOR,	{CResizer::eFixed, IDC_CS_LABEL_SEPCOLOR, CResizer::eRight},//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom},			//Top side
										{CResizer::eProportional, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},			//Bottom side

	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fenêtre Pupitre Echelle
void CDlgPupitreCum::SetColors()
{
	// Cadre pupitre cumulativité
	m_CadrePupitreCum.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Label grandeur
	m_LblGrandCum.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS, CATPColors::DARK_DEGRAD);
	m_LblGrandCum.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Couleur sélection dans liste grandeurs
	m_LstGrandCum.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));

	// Contrôle Titre Courbe
	m_TitreCourbe.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_TitreCourbe.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Le grille des couleurs
	m_GridColorCum.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));
	m_GridColorCum.GetParam()->GetProperties()->SetColor(GX_COLOR_FIXEDLINES, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// Bouton activation sélection séparées couleurs
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnSeparColor.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Bouton activation sélection groupées couleurs
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnGroupColor.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

}

/////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CDlgPupitreCum::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise à jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
		SetColors();
	}

	//////////////////////////////////////// INITIALISATION GENERALE ////////////////////////////////
	//
	else if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
	{
		// Mise à jour mode sélection couleur
		UpdateModeSelColor();
	}

	///////////////////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Mise à jour des grandeurs disponibles
		MajGrandeurs();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Resize dz la fenêtre pupitre cumulativité
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
	// Mise à jour des élmts grandeurs
	if (m_LstGrandCum.GetCount() <= 0)
	{
		// Attention si sélection choix support non défini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleCum);

		// Impossible de continuer si aucune grandeur sélectionnée
		if (m_NbGrandeurs == 0) return false;

		// Chargement des grandeurs définies
		CString Txt;
		JUnt32 InxList = 0;
		for (JUnt32 i=0; i<m_NbGrandeurs; i++)
		{
			// Ajoute libellé grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, i));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((i == pDocChoixSupport->GrdCum_XGRP)		|| 
					(i == pDocChoixSupport->GrdCum_XRepetition)) continue;

			m_LstGrandCum.AddString(Txt);

			// Ajoute ident grandeur
			// m_LstGrandCum.SetItemData(i,i); 
			m_LstGrandCum.SetItemData(InxList,i); 

			// Incrémente indice liste réel
			InxList++;
		}

		// Update affichage de la liste
		m_LstGrandCum.UpdateWindow();
		m_LstGrandCum.Invalidate();
	}

	// Puis on sélectionne par défaut la 1ere grandeur
	if (m_LstGrandCum.GetCount() > 0 )
	{
		JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(0);

		// Voir si on l'ajoute au map sélection
		pDocChoixSupport->m_MapIdGrandCumSel.MoveTo(IdGrandeur);
		if (!pDocChoixSupport->m_MapIdGrandCumSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandCumSel.Add(0) = IdGrandeur;

		// Sélection dans liste
		m_LstGrandCum.SetCurSel(0);
	}

	UpdateSelGrandeurs();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour liste grandeurs (attention aux grandeurs invalides pour certains types de calcul)
//
// exemple : pour moteur full binomial pas de GRP+ ni de reprise
//
void CDlgPupitreCum::MajGrandeurs()
{
	// Reset de la liste
	m_LstGrandCum.ResetContent();

	// Nb grandeurs de la liste à remplir
	m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleCum);

	// Impossible de continuer si aucune grandeur sélectionnée
	if (m_NbGrandeurs == 0) return;

	// Chargement des grandeurs définies
	CString Txt;
	int IndexList = 0;
	for (JUnt32 i=0; i<m_NbGrandeurs; i++)
	{
		// Ajoute libellé grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleCum, i));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((i == pDocChoixSupport->GrdCum_XGRP)	   ||
				(i == pDocChoixSupport->GrdCum_XRepetition))   // Indice GRP+ et Répet dans liste grandeur cumulativité 
			{
				// Regarde si grandeur non précédemment sélectionné
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

		// Si élmt précédemment sélectionné on repositionne la sélection
		for (pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
			 pDocChoixSupport->m_MapIdGrandCumSel.IsValid();
			 pDocChoixSupport->m_MapIdGrandCumSel.MoveNext())
		{
			JUnt32 ItemData = pDocChoixSupport->m_MapIdGrandCumSel.GetItem();
			
			// Appartient au map sélection
			if (ItemData == i)
			{
				m_LstGrandCum.SetSel(IndexList,true);
				break;
			}
		}

		// Incrémente indice liste
		IndexList ++; 
	}

	UpdateSelGrandeurs();
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections grandeurs
void CDlgPupitreCum::UpdateSelGrandeurs()
{
	// Déselectionne toutes les grandeurs
	for (JUnt32 i= 0; i < m_LstGrandCum.GetCount(); i++)
	{
		m_LstGrandCum.SetSel(i,false);
	}

	// Si au moins 1 grandeur sélectionnée
	if (pDocChoixSupport->m_MapIdGrandCumSel.GetCount() > 0)
	{
		// Boucle sur les grandeurs sélectionnées
		for (pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
			pDocChoixSupport->m_MapIdGrandCumSel.IsValid();
			pDocChoixSupport->m_MapIdGrandCumSel.MoveNext())
		{
			// Boucle sur les grandeurs existantes
			for (JUnt32 i= 0; i < m_LstGrandCum.GetCount(); i++)
			{
				// Ident grandeur
				JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(i);

				// Mise à jour activation ou désactivation bouton groupé
				UpdateBtnGroup(IdGrandeur);

				if (IdGrandeur == pDocChoixSupport->m_MapIdGrandCumSel.GetItem())
				{
					// Sélection dans liste
					m_LstGrandCum.SetCurSel(i);
					break;
				}
			}
		}
	}
	else
	{
		// par défaut on sélectionne le 1er élément
		m_LstGrandCum.SetCurSel(0);
		pDocChoixSupport->m_MapIdGrandEchSel.MoveTo(0);
		if (!pDocChoixSupport->m_MapIdGrandEchSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = true; 

		// Mise à jour activation ou désactivation bouton groupé
		UpdateBtnGroup(m_LstGrandCum.GetItemData(0));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Activation ou désactivation bouton groupement courbe (seul GRP ou Nb Contact cumulable)
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
// Change grandeur à afficher
void CDlgPupitreCum::OnLbnSelchangeCsListGrandcum()
{
	// Modif sélection grandeurs
	pDocChoixSupport->m_MapIdGrandCumSel.Reset();
	for (JUnt32 i = 0; i < m_LstGrandCum.GetCount(); i++)
	{
		if (m_LstGrandCum.GetSel(i) > 0)
		{
			// Récupère identifiant type grandeur
			JUnt32 IdGrandeur = m_LstGrandCum.GetItemData(i);
			pDocChoixSupport->m_MapIdGrandCumSel.Add(pDocChoixSupport->m_MapIdGrandCumSel.GetCount()) = m_LstGrandCum.GetItemData(i); 

			// Active ou désactive le mode cumul groupé (seul GRP et Nb Contact permettent le cumul groupé > valeurs additives)
			UpdateBtnGroup(IdGrandeur);
			
			break;
		}
	}

	// Mise à jour option mode courbe (groupées ssi Grp,Grp+,Nb Contacts)
	MajModeCourbe();

	// Mise à jour des différentes fenêtres 
	pDlgCum->UpdateResultCum(false);

	// Refait le tri sur cumulativité
	pDocChoixSupport->UpdateAllViews(UPDATE_TRI_COURANT_CUM);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour du mode courbe
void CDlgPupitreCum::MajModeCourbe()
{
	// Test si mode groupé
	if (pDocChoixSupport->m_ModeColorGraphe == 1)
	{
		// Test si grandeur cumulable
		JUnt32 IdGrandeur = 0;
		pDocChoixSupport->m_MapIdGrandCumSel.MoveFirst();
		if (pDocChoixSupport->m_MapIdGrandCumSel.IsValid())
			IdGrandeur = pDocChoixSupport->m_MapIdGrandCumSel.GetItem(); 

		// Attention si grandeur <> Grp, Grp+, Nb Contacts on revient au mode séparé
		if	(IdGrandeur != pDocChoixSupport->GrdCum_GRP &&
			 IdGrandeur != pDocChoixSupport->GrdCum_NbContact000 &&
			 IdGrandeur != pDocChoixSupport->GrdCum_XGRP)
		{
			// on reste en courbes par support
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);		

			// Modifie variable sélection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 0;

			// Mise à jour des différentes fenêtres 
			pDlgCum->UpdateResultCum(false);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection mode couleurs séparées
void CDlgPupitreCum::OnBnClickedCsLabelSepcolor()
{
	// Validation bouton
	CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);	

	// Modifie variable sélection couleurs
	pDocChoixSupport->m_ModeColorGraphe = 0;

	// Mise à jour des différentes fenêtres 
	pDlgCum->UpdateResultCum(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sélection mode couleurs groupées
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
		// on passe en mode groupage résultats
		case pDocChoixSupport->GrdCum_GRP :
		case pDocChoixSupport->GrdCum_NbContact000 :
		case pDocChoixSupport->GrdCum_XGRP : 
		{
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_GROUPCOLOR);		

			// Modifie variable sélection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 1;

			// Mise	à jour des différentes fenêtres 
			pDlgCum->UpdateResultCum(false);
		}
		break;

		////////////////// Pour tous les autres calculs //////////////////
		// on reste en courbes par support
		default :
		{
			// Validation bouton
			CheckRadioButton(IDC_CS_LABEL_SEPCOLOR, IDC_CS_LABEL_GROUPCOLOR, IDC_CS_LABEL_SEPCOLOR);		

			// Modifie variable sélection couleurs
			pDocChoixSupport->m_ModeColorGraphe = 0;

			// Mise à jour des différentes fenêtres 
			pDlgCum->UpdateResultCum(false);
		}
		break;
	}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour mode sélection couleur
void CDlgPupitreCum::UpdateModeSelColor()
{
	if (pDocChoixSupport->m_IdColorCum != 0)
	{
		if (pDocChoixSupport->m_ModeColorGraphe == 0)
			// Mode sélection couleurs séparées
			OnBnClickedCsLabelSepcolor();
		else
			// Mode sélection couleurs groupées
			OnBnClickedCsLabelGroupcolor();
	}
}