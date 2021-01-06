// DlgTableau.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTableau.h"
#include "PaletteMode.h"
#include "LineMode.h"
#include "ColumnMode.h"
#include "PageMode.h"
#include "TerrainManager.h"
#include "AnalyseurDoc.h";
#include "JMFCStream.h"
#include "SiteSelectionDlg.h"
#include "DlgItemsMoyenne.h"
#include "Label.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgTableau dialog
BEGIN_MESSAGE_MAP(CDlgTableau, CDlgItems)
	ON_BN_CLICKED(IDOK,							OnOk)
	ON_BN_CLICKED(IDCANCEL,						OnCancel)
	ON_BN_CLICKED(IDC_RI_BUT_GENESE,			OnGenese)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_LIGNES,		OnBnClickedRiRadModeLignes)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_COLONNES,		OnBnClickedRiRadModeColonnes)
	ON_BN_CLICKED(IDC_RI_RAD_MODE_PAGES,		OnBnClickedRiRadModePages)
	ON_BN_CLICKED(IDC_RI_BUT_ITEM_MOY_LIG,		OnBnClickedRiItemMoyenLig)
	ON_BN_CLICKED(IDC_RI_BUT_ITEM_MOY_COL,		OnBnClickedRiItemMoyenCol)
	ON_COMMAND(IDR_RI_MENU_SUPP_LIGNES,			OnRiMenuSuppLignes)
	ON_COMMAND(IDR_RI_MENU_SUPP_COLONNES,		OnRiMenuSuppColonnes)
	ON_COMMAND(IDR_RI_MENU_SUPP_PAGES,			OnRiMenuSuppPages)
	ON_COMMAND(IDR_RI_MENU_SUPP_TOUT,			OnRiMenuSuppTout)
	ON_LBN_SELCHANGE(IDC_RI_LIST_IITEMS,		OnSelchange_ListItems)
	ON_BN_CLICKED(IDC_RI_RAD_DIMENSION1,		OnChange_Dim_1_PaletteSel)
	ON_BN_CLICKED(IDC_RI_RAD_DIMENSION1,		OnChange_Dim_2_PaletteSel)
	ON_LBN_SELCHANGE(IDC_RI_LIST_DIMENSION1,	OnSelchange_ListDimension_1)
	ON_LBN_SELCHANGE(IDC_RI_LIST_DIMENSION2,	OnSelchange_ListDimension_2)
	ON_LBN_SELCHANGE(IDC_RI_LIST_QUESTIONS,		OnSelchange_ListQuestions)
	ON_LBN_SELCHANGE(IDC_RI_LIST_SAS,			OnSelchange_ListSas)
	ON_MESSAGE(WM_USER_QUESTIONLISTUPDATED,		OnUpdated_QuestionList)
	ON_BN_CLICKED(IDC_RI_RAD_Q_ET,				OnSAS_Et)
	ON_BN_CLICKED(IDC_RI_RAD_Q_OU,				OnSAS_Ou)
	ON_BN_CLICKED(IDC_RI_BUT_ACCEPT,			OnAccept)
	ON_BN_CLICKED(IDC_RI_BUT_QUESTION_TOUS,		OnQuestions_Tous)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
// CDlgTableau::CDlgTableau(CTerrain *pTerrain, CWnd* pParent /*=NULL*/)	: CDlgItems(pParent)
CDlgTableau::CDlgTableau(CWnd* pParent /*=NULL*/)	: CDlgItems(pParent)
{
	m_CATPColorsApp = CATPColors::COLORANALYSEUR; 

	// Pour la partie pupitre
	m_pCurModeLigne		= 0;
	m_pCurModeColonne	= 0;
	m_pCurModePage		= 0;


	// Mode courant par defaut = mode ligne
	m_Id_CurMode		=	CPaletteMode::Line;

	// Ici type de boite = pupitre
	m_TypeRepItem = false;

	// Elmt de Construction (tel que c(était dans DlgVectorielle)
	m_pTerrain		= 0;
	m_bItemsChanged	=	false;
	m_bRowsChanged	=	false;
	m_bColsChanged	=	false;
	m_bPagesChanged =	false;
	m_bRowsSort		=	false;
	m_bColsSort		=	false;
	m_bPagesSort	=	false;
	m_DType_Row		=	DTYPE_ITEMS;
	m_DType_Col		=	DTYPE_ITEMS;
	m_DType_Page	=	DTYPE_ITEMS;

	// Initialisation des élmts dragdrop
	m_bDragging		= false;
	m_pDragImage	= 0;
	m_pDragWnd		= 0;
	m_pDropWnd		= 0;
	m_nDragIndex	= -1;
	m_nDropIndex	= -1;

	//////////////////////////////// Activation des modes sélections ////////////////////////////
	//
	// -- Si seulement dimension 1, le mode sélection dimension 1 est automatiquement activé
	//
	// -- Si dimension 1 et dimension 2, le mode sélection dimension 1 est désactivé, mais le mode
	//    sélection dimension 2 est activé  (mode de sélection le plus courant dans le cas de 2 dimensions)
	//
	// Par défaut mode ajout sélection actif en dimension 1
	m_AjoutSelLigActif_Dim1	 = true;
	m_AjoutSelColActif_Dim1	 = true;
	m_AjoutSelPageActif_Dim1 = true;
	//
	// Et non actif sur la dimension 2
	m_AjoutSelLigActif_Dim2	 = true;
	m_AjoutSelColActif_Dim2	 = false;
	m_AjoutSelPageActif_Dim2 = false;

	// Et actif sur la dimension 2
	m_AjoutSelLigActif_Dim3	 = false;
	m_AjoutSelColActif_Dim3	 = false;
	m_AjoutSelPageActif_Dim3 = false;

	// On réinitialise le map des sélections dim1, dimé, dim3 par questions
	m_MapModeSelQuestionLigDim1.Reset(); 
	m_MapModeSelQuestionLigDim2.Reset(); 
	m_MapModeSelQuestionLigDim3.Reset(); 
	m_MapModeSelQuestionColDim1.Reset(); 
	m_MapModeSelQuestionColDim2.Reset(); 
	m_MapModeSelQuestionColDim3.Reset(); 
	m_MapModeSelQuestionPageDim1.Reset(); 
	m_MapModeSelQuestionPageDim2.Reset(); 
	m_MapModeSelQuestionPageDim3.Reset(); 

	// Mode ligne au départ
	m_Id_CurModePrec = CPaletteMode::Line;
}

////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgTableau::~CDlgTableau()
{
	for(m_Items_Del.MoveFirst(); m_Items_Del.IsValid(); m_Items_Del.MoveNext())
		delete m_Items_Del.GetItem();
	m_Items_Del.Reset();

	if(m_pTerrain)
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);


}

void CDlgTableau::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if (this->m_hWnd != NULL)
		// this->ShowWindow(SW_SHOWMINNOACTIVE); // this->SetShowMode()
		this->SetShowMode(SW_SHOWMINNOACTIVE); // this->SetShowMode()

	// TODO : ajoutez ici le code de votre gestionnaire de messages
}

void CDlgTableau::SetTerrainTemoin(CTerrain *pTerrain)
{
	// Chargement Source et Terrain en cours d'utilisation dans Tris Croisés	
	if(m_pTerrain)
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);

	// Positionnement du terrain témoin courant
	m_pTerrain		=	pTerrain;
	if(m_pTerrain)
	{
		m_Id_Source		= m_pTerrain->GetSourceID();
		m_Id_Terrain	= m_pTerrain->GetTerrainID();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// 
void CDlgTableau::SetParams(UINT TypePoidsId, const JList<UINT> & SegmentsId)
{
	m_ContingentMask = 0;
	m_Id_PopSegments.Reset();
	m_PopSegmentFilter.SetSize(0);
	JVector<CSegment*> Segments;
	if(m_pTerrain->GetSegments(Segments))
	{
		JList<JInt32> TmpSegments;
		JInt32 NbSegments = Segments.GetCount();
		for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1)
		{
			Segments.MoveTo(IdxSegment);
			UINT SegID = Segments.GetItem()->GetID();
			for (SegmentsId.MoveFirst(); SegmentsId.IsValid(); SegmentsId.MoveNext())
				if (SegmentsId.GetItem() == SegID) { TmpSegments.AddTail() = IdxSegment; SegmentsId.MoveLast(); }
		}
		if (TmpSegments.GetCount() > 0)
		{
			JInt32 JdxSegment = 0;
			m_Id_PopSegments.SetCount(TmpSegments.GetCount());
			for (TmpSegments.MoveFirst(); TmpSegments.IsValid(); TmpSegments.MoveNext())
			{
				Segments.MoveTo(TmpSegments.GetItem()); m_Id_PopSegments.MoveTo(JdxSegment);
				m_ContingentMask |= Segments.GetItem()->GetContingentMask();
				if(JdxSegment > 0)
					m_PopSegmentFilter |= Segments.GetItem ()->GetPopSegment();
				else
					m_PopSegmentFilter = Segments.GetItem ()->GetPopSegment();
				m_Id_PopSegments.GetItem() = Segments.GetItem()->GetID();
				JdxSegment += 1;
			}
		}
		else
		{
			m_Id_PopSegments.SetCount(NbSegments);
			for (JInt32 JdxSegment = 0; JdxSegment < NbSegments; JdxSegment += 1)
			{
				Segments.MoveTo(JdxSegment); m_Id_PopSegments.MoveTo(JdxSegment);
				m_ContingentMask |= Segments.GetItem()->GetContingentMask();
				if(JdxSegment > 0)
					m_PopSegmentFilter |= Segments.GetItem ()->GetPopSegment();
				else
					m_PopSegmentFilter = Segments.GetItem ()->GetPopSegment();
				m_Id_PopSegments.GetItem() = Segments.GetItem()->GetID();
			}
		}
	}
	m_Id_TypePoids = TypePoidsId;
	if(!m_pTerrain->HasTypePoids(m_Id_TypePoids))
		m_Id_TypePoids = m_pTerrain->GetDefaultTypePoidsID(); 
}


BOOL CDlgTableau::OnInitDialog() 
{
	// Init Dialogue
	CDialog::OnInitDialog();

	// Si terrain non initiliasé, on arrête tout
	if(!m_pTerrain)
	{
		EndDialog(-1);
		return true;
	}

	if(AfxGetApp())
		// m_hIcon = AfxGetApp()->LoadIcon(IDI_RI_ICON);
		m_hIcon = AfxGetApp()->LoadIcon(IDI_ATP_ANALYSEUR);

	// Affichage titre fenêtre avec nom du dossier courant
	m_Title.LoadString(IDS_RI_TITLE_PUPITRE);
	CString TxtNom;
	TxtNom.LoadString(IDS_AN_NONAME); 
	if (m_pDoc)
	{
		if(m_pDoc->GetTitle().IsEmpty())
			m_Title.AppendFormat(" - %s",TxtNom);
		else
			m_Title.AppendFormat(" - %s",m_pDoc->GetTitle());
	}
	this->SetWindowText(m_Title);
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// If the data is not initialized then quit
	if(!GetItems())
	{
		this->EndDialog(false);
		return false;
	}

	m_FIcones = AfxGetApp()->GetProfileInt("Options\\RepItem", "OpIcons", 1);
	AfxGetApp()->WriteProfileInt("Options\\RepItem", "OpIcons", m_FIcones);

	// Initialisation des modes création palette dans pupitre
	m_pCurMode			=	0;

	// On réinitialise le map des sélections dim1, dimé, dim3 par questions
	m_MapModeSelQuestionLigDim1.Reset(); 
	m_MapModeSelQuestionLigDim2.Reset(); 
	m_MapModeSelQuestionLigDim3.Reset(); 
	m_MapModeSelQuestionColDim1.Reset(); 
	m_MapModeSelQuestionColDim2.Reset(); 
	m_MapModeSelQuestionColDim3.Reset(); 
	m_MapModeSelQuestionPageDim1.Reset(); 
	m_MapModeSelQuestionPageDim2.Reset(); 
	m_MapModeSelQuestionPageDim3.Reset(); 

	// Par la suite il faudra à ce niveau récupérer les info en cours
	if (m_Modes.GetCount() == 0)
	{
		m_Modes.Reset();
		m_Modes.Add(CPaletteMode::Line)	= PTR_ABSTRACTMODE(new CLineMode(this));  
		m_Modes.Add(CPaletteMode::Column)	= PTR_ABSTRACTMODE(new CColumnMode(this));  
		m_Modes.Add(CPaletteMode::Page)	= PTR_ABSTRACTMODE(new CPageMode(this));  
	}

	// Définition des contrôles pour Pupitre
	SetupControls();

	// Mise à jour interface via ident mode courant avec ligne comme onglet par défaut
	m_Id_CurModePrec = CPaletteMode::Line;
	UpdateMode(m_Id_CurMode, false);

	// Positionne Pseudo Item si nécessaire
	m_pCurMode->SetPseudoItem();

	// Set the theme tree data
   	SetThemeData();

	// Set tree rubriques items
	SetRubriqueItems();

	// Set tree rubriques palettes
	SetRubriquePalettes();

	// Sets the current
	CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);
	UpdateTheme(IDC_RI_RAD_QUESTIONS);

	// Init mode pupitre
	CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,IDC_RI_RAD_MODE_LIGNES);

	// Affichage editeur pupitre
	UpdatePupitre(IDC_RI_RAD_MODE_LIGNES);

	// Default radio button setting
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_OU);
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
	CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_OU);
	CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_OU);

	//  Par défaut pas de liste rubriques visible
	m_Chk_Rubrique_Defaut.ShowWindow(SW_HIDE);
	m_ComboRubriques.ShowWindow(SW_HIDE);
	
	// Attention initialiser les opérateurs si aucun élmt sélectionné présent
	if (m_Items_Row.GetCount() == 0)
	{
		// Opérateurs par défaut
		AllOperateurParDefaut();
	}

	// Mise à jour des opérateurs actifs
	SetUpdateOpeLogiques();

	const JSRCTables & Sources = JSRCTables::GetInstance();
	m_Label_TypePoids.SetText(Sources.m_TBLPoids.GetLabelByID(m_Id_TypePoids));

	// Default results
	m_Edit_Name      .SetWindowText(""); 
	m_Static_Effectif.SetText("");
	m_Static_NbCas   .SetText("");
	m_Static_Percent .SetText("");
	m_bSegment = 0;
	m_Button_CalcSegment.SetCheck(m_bSegment);

	UpdateData(false);

	//Taille nominale de la boite 730x560
	SetWindowPos(NULL, 0,0, 760, 560, SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOREDRAW );

	// Boite Tree Theme visible, boite résultat non visible
	SetVisibleBoiteTreeTheme(true);
	SetVisibleBoiteValidItem(true);
	SetVisibleBoiteResultat(false);
	
	// Pas de boite result palette non plus
	SetVisibleResultPalette(false);
		
	// Rend le mode pupitre actif et la fenetre selection invisible
	SetVisibleModePupitre(true);
	SetVisibleBoitePupitre(true);

	// Retaille fenêtre en mode répertoire item - sans barre onglet pupitre
	ResizeInterfacePupitre(m_SelectionEnBas);

	// Creer les mémoires tempo stockage modes ligne, colonne, page avant modif
	pMementoLigne		= JMemento::Create();
	pMementoColonne		= JMemento::Create();
	pMementoPage		= JMemento::Create();

	/////////////////////////////////////////////////////////////////////////////////////
	// Anciennement dans CDlgVectoriele :: InitDialog
	RecupItemSelect();
	
	// Mise à jour infos sélections	
	UpdateCount();

	// Mise en place arbre des questions avec validation des sélectionnés
	OnTree_Questions();

	// Mise en place listes résultats, au départ seule la liste lignes apparait
	SetListeResultat();

	// Par défaut seule dimension 1 active
	m_Button_Dimension1.SetCheck(1);
	m_Button_Dimension2.SetCheck(0);

	// Et actif sur la dimension 1, pas sur la dimension 2
	m_AjoutSelLigActif_Dim1		= true;
	m_AjoutSelLigActif_Dim2		= false;
	m_AjoutSelLigActif_Dim3		= false;

	m_AjoutSelColActif_Dim1		= true;
	m_AjoutSelColActif_Dim2		= false;
	m_AjoutSelColActif_Dim3		= false;

	m_AjoutSelPageActif_Dim1	= true;
	m_AjoutSelPageActif_Dim2	= false;
	m_AjoutSelPageActif_Dim3	= false;

	/////////////////////////////////////////////////////////////////////////////////////
	// Remettre les elmts précédemment sélectionnés dans la partie présentoir
	// A REVOIR JUIN 2006
	if (m_pCurModeLigne)
		m_pCurModeLigne->OnItem_Raz();
	if (m_pCurModeColonne)
		m_pCurModeColonne->OnItem_Raz();
	if (m_pCurModePage)
		m_pCurModePage->OnItem_Raz();

	// On vire les elmts du sas
	// UpdateAllListSelect();

	// Mise à jour sélection pages
	m_Id_CurModePrec = CPaletteMode::Page;
	SetListeResultat();
	ResizeColListSelect(m_Id_CurModePrec);

	// Mise à jour sélection colonnes
	m_Id_CurModePrec = CPaletteMode::Column;
	SetListeResultat();
	ResizeColListSelect(m_Id_CurModePrec);

	// Mise à jour sélection lignes
	m_Id_CurModePrec = CPaletteMode::Line;
	SetListeResultat();
	ResizeColListSelect(m_Id_CurModePrec);

	// Mise à jour de la liste sélection courante via les sélections mode palette
	UpdateListSelect();

	// Valide les élmts du SAS, afin d'afficher les composants en totalité dans les listes sélections
	if (m_List_SAS.GetCount() > 0)
	{
		for (int IndexSAS = m_List_SAS.GetCount(); IndexSAS >= 0; IndexSAS--)
		{
			m_List_SAS.SetCurSel(IndexSAS);
			m_pCurMode->OnSelchange_ListSas();
		}
	}

	// Puis mise en place de l'opérateur sélectionné pour le SAS
	if (m_List_SAS.GetCount() == 0 || !m_pCurMode->ModeCroisementPalette())
		// Opération "Ou" = mode suite entre élément du Sas
		OnSAS_Ou();
	else
		// Opération "Et" = mode croisement entre élmts du sas
		OnSAS_Et();

	// on calcule la configuration du split vertical
	ConfigSplit();
	
	// Flag 1er affichage fenetre pupitre
	m_bFirstTime	= TRUE;

	// Repositionne le split via dernière sauvegarde (après visu fen dialog)
	ShowWindow(SW_SHOW);
	PosLastSplit();

	// Les elmts non visibles
	m_Label_Effectif.ShowWindow(FALSE); 
	m_Static_Effectif.ShowWindow(FALSE); 
	m_Label_Percent.ShowWindow(FALSE); 
	m_Cadre_Result.ShowWindow(FALSE);
	m_Static_Percent.ShowWindow(FALSE); 
	m_Chk_Rubrique_Defaut.ShowWindow(FALSE);

	// Rend invalide le bouton filtre
	m_Button_Filtre.EnableWindow(FALSE);


	// Bouton sélection items moyennables (actif ssi on est en mode "MOY_LIGNE" dans les Tris Croisés)
	if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE && this->GetShowSite() != SHOW_SITES)
	{
		m_Button_Item_Moy_Lig.EnableWindow(TRUE);
		m_Button_Item_Moy_Col.EnableWindow(FALSE);
	}
	else if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE && this->GetShowSite() != SHOW_SITES)
	{
		m_Button_Item_Moy_Lig.EnableWindow(FALSE);
		m_Button_Item_Moy_Col.EnableWindow(TRUE);
	}
	else
	{
		m_Button_Item_Moy_Lig.EnableWindow(FALSE);
		m_Button_Item_Moy_Col.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE

}

//////////////////////////////////////////////////////////////////////////////////
// Repositionne le split via dernière sauvegarde
void CDlgTableau::PosLastSplit()
{
	// Recup position barre split
	int NewLeft;
	NewLeft = AfxGetApp()->GetProfileInt("Options\\Analyseur", "LeftSplitVert", -1);   

	// Position origine split
    RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); 
	this->ScreenToClient(&RectSP);
	if (NewLeft >= 0)
	{
		// Déplacement split
		LONG Depl = NewLeft - RectSP.left;
		RectSP.left		= NewLeft;
		RectSP.right	= RectSP.right + Depl;
		DeplaceSplitVert(Depl);
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Récupération des items sélectionnés
void CDlgTableau::RecupItemSelect()
{
	m_Items_New.Reset();
	int i=0;

	//////////////////////////////////// Récupération des items lignes précédemment sélectionnées //////////////////////// 
	// Setup the list control Rows
	m_List_Row.DeleteAllItems();
	for(i=0, m_Items_Row.MoveFirst(); m_Items_Row.IsValid(); m_Items_Row.MoveNext(), ++i)
	{
		CString StrHeader	= m_Items_Row.GetItem()->GetHeader();
		CString StrLabel	= m_Items_Row.GetItem()->GetLabel();
		CString Str;
		Str.Format("%s - %s",StrHeader,StrLabel); 
		m_List_Row.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)m_Items_Row.GetItem());
	}

	//////////////////////////////////// Récupération des items colonnes précédemment sélectionnées //////////////////////// 
	// Setup the list control Columns
	m_List_Col.DeleteAllItems();
	for(i=0, m_Items_Col.MoveFirst(); m_Items_Col.IsValid(); m_Items_Col.MoveNext(), ++i)
	{
		CString StrHeader	= m_Items_Col.GetItem()->GetHeader();
		CString StrLabel	= m_Items_Col.GetItem()->GetLabel();
		CString Str;
		Str.Format("%s - %s",StrHeader,StrLabel); 
		m_List_Col.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)m_Items_Col.GetItem());
	}

	//////////////////////////////////// Récupération des pages lignes précédemment sélectionnées //////////////////////// 
	// Setup the list control Pages
	m_List_Page.DeleteAllItems();
	for(i=0, m_Items_Page.MoveFirst(); m_Items_Page.IsValid(); m_Items_Page.MoveNext(), ++i)
	{
		CString StrHeader	= m_Items_Page.GetItem()->GetHeader();
		CString StrLabel	= m_Items_Page.GetItem()->GetLabel();
		CString Str;
		Str.Format("%s - %s",StrHeader,StrLabel); 
		m_List_Page.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)m_Items_Page.GetItem());
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Réinitialisation des élmts sélection
void CDlgTableau::InitSelGeneral()
{
	// Init Tableau des modes pupitres
	m_Modes.Reset();

	// Init palettes ligne
	if (m_pCurModeLigne)
	{
		m_pCurModeLigne		= 0;
	}

	// Init palettes colonne
	if (m_pCurModeColonne)	
	{
		m_pCurModeColonne	= 0;
	}

	// Init palettes page
	if (m_pCurModePage)	
	{
		m_pCurModePage		= 0;
	}

	// Init mode courant palette
	if (m_pCurMode)			
		m_pCurMode			= 0;

}

//////////////////////////////////////////////////////////////////////////////////
// Mise à jour de toutes les listes sélections (lignes, colonnes, pages)
// (affichera toutes les sélections en cours)
//
void CDlgTableau::UpdateAllListSelect(bool ModeAjout)
{
	// Mise à jour des dernières sélections pages
	if (m_List_Page.GetItemCount() > 0)
		OnBnClickedRiRadModePages();

	// Mise à jour des dernières sélections colonne
	if (m_List_Col.GetItemCount() > 0)
		OnBnClickedRiRadModeColonnes();

	// Mise à jour des dernières sélections ligne, et on reste positionné sur cette sélection
	if (m_List_Row.GetItemCount() > 0)
		OnBnClickedRiRadModeLignes();

}

/*
void CDlgTableau::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//Taille minimale de la boite 850x425  : Attention ici ça limite la largeur et hauteur de la fenetre
	lpMMI->ptMinTrackSize.x = 800; 
	lpMMI->ptMinTrackSize.y = 450; 
	CDialog::OnGetMinMaxInfo(lpMMI);
}
*/

void CDlgTableau::SetupControls()
{
	CDlgItems::SetupControls(); 

	// Remove the menu
	SetMenu(NULL);

	// Hide those objects not needed
	m_Tab_Segment.ShowWindow(SW_HIDE);  
	m_Button_Segment_OU.ShowWindow(SW_HIDE);  
	m_Button_Segment_ET.ShowWindow(SW_HIDE);
	m_Button_CalcSegment.ShowWindow(SW_HIDE);

	// Show the objects specific to us
	m_Button_OK.ShowWindow(SW_SHOW);  
	m_Button_Cancel.ShowWindow(SW_SHOW);
	m_Button_Questions_Tous.ShowWindow(SW_SHOW);

	// Liste questions en mode marquage
	m_List_Questions.SetModeMarquage(true); 
}

void CDlgTableau::OnOk()
{
	// Attention si list SAS en cours non vide (élmts non positionnés en ligne ou colonne ou page)
	if (m_List_SAS.GetCount() > 0)
	{
		if (AfxMessageBox("Attention, certains éléments sélectionnés n'ont pas été disposés dans le Tri Croisé. Voulez-vous continuer ?",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return;
	}

	// Traitement des sélections pour revenir à l'analyseur
	if (ValideSelectionTriCroise())
	{
		// efface les fenêtres flottantes
		m_pCurMode->EffaceFenDimDlg(true);

		// Supprime les mémoires de sauvegarde des modes
		delete pMementoLigne;
		delete pMementoColonne;
		delete pMementoPage;

		// Sauve positionnement split vertical rép items
		// Aout 2013
		RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);
		AfxGetApp()->WriteProfileInt("Options\\Analyseur", "LeftSplitVert", RectSP.left);

		CDialog::OnOK();
	}
}

void CDlgTableau::OnGenese() 
{
	if(m_List_Items.GetSelCount() > 0)
	{
		if(m_pCurMode)
		{
			m_pCurMode->OnGenese();
			m_pCurMode->GetSelectedItems(m_Items);
		}
	}
}


void CDlgTableau::ValideElmtSas(bool ChangeSas)
{
	if (m_pCurMode)
	{
		// Valider 1er élmt actif du sas
		if (m_List_SAS.GetCount() > 0)
		{

			int InxCurSel = m_List_SAS.GetCurSel();

			if (m_List_SAS.GetCurSel() == LB_ERR)
			{
				// Active 1er item sélection du sas 
				m_List_SAS.SetCurSel(0);

				if (ChangeSas)
					m_pCurMode->OnSelchange_ListSas();
	
				// NEW !!!!
				// Met à jour la liste des questions via tree item
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					// Sélection des questions dans tree-items
					m_Tree_Theme.SetQuestionList(ListQuestionSAS);
				}
				// Update list des questions
				m_pCurMode->UpdateQuestionList();
			}
		}
		else
		{
			// Aucun élmt de sélection, on vire l'item précédent
			// OnItem_Raz();
			OnSAS_Raz();

			// Update de toutes les dimensions
			m_pCurMode->UpdateDimensionsList();

			// Update list des questions
			m_pCurMode->UpdateQuestionList();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode lignes pour les tris croisés
void CDlgTableau::OnBnClickedRiRadModeLignes()
{
	// Effacement des fenêtres dimension mobiles
	if (m_pCurMode != m_pCurModeLigne)
		m_pCurMode->EffaceFenDimDlg(false);


	// Si Bouton Controle Actif au même moment, on est en mode non effacement sélection courante
	// (cela permet de copier dans le même temps en ligne, colonn, page sans recommencer la sélection)
	JBool EffaceSelCur = true;
	if(GetAsyncKeyState(VK_CONTROL)<0)
		EffaceSelCur = false;

	// Valider 1er élmt actif du sas
	if (m_List_SAS.GetCount() > 0)
	{
		// En blanc liste sélections ligne
		m_List_Row.				SetBkColor(CATPColors::GetColorWhite());
		m_List_Row.				SetBackColor(CATPColors::GetColorWhite());
		m_List_Col.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Col.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));

		// Active le bouton ligne, désactive les autres
		m_Button_Mode_Ligne.SetTypeOnglet(FALSE,TRUE,TRUE); 
		m_Button_Mode_Colonne.SetTypeOnglet(FALSE,FALSE,TRUE); 
		m_Button_Mode_Page.SetTypeOnglet(FALSE,FALSE,TRUE); 

		/* Modif Filtre/Filtre Janv 2007 (permet de valider choix items avec filtre dim 1 et filtre dim 2 A voir !!!!)
		// Boutons sélection non actifs
		if (!m_AjoutSelLigActif_Dim1 && !m_AjoutSelLigActif_Dim2)
			return;
		*/

		// Mise à jour du pupitre avec nouveau mode
		// OPTIM 2.7 UpdatePupitre(IDC_RI_RAD_MODE_LIGNES);
		m_Id_CurMode		=	CPaletteMode::Line;
		// FIN OPTIM
		
		// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non sélectionné
		ValideElmtSas(true);
		
		// Mise à jour affichage via nouveau mode
		AffichePaletteModeCur();	

		// Mise à jour nb lignes
		UpdateCount();
	}


	// Valider 1er élmt actif du sas
	if (m_List_SAS.GetCount() > 0 )
	{
		// Active 1er item sélection du sas
		m_List_SAS.SetCurSel(0);
		m_pCurMode->OnSelchange_ListSas();

		// Positionne nouveau mode
		m_Id_CurModePrec = CPaletteMode::Line;

		// Mise à jour liste sélection
		UpdateListSelect();

		// Disposition liste résultat
		SetListeResultat(); 

		// Si on est en mode effacement sel
		if (m_List_SAS.GetCount() > 0 && EffaceSelCur)
			m_pCurMode->OnItem_Raz();
	}
	else
	{
		// Checké le dernier mode en cours
		m_Button_Mode_Ligne.SetCheck(m_Id_CurModePrec == CPaletteMode::Line);
		m_Button_Mode_Colonne.SetCheck(m_Id_CurModePrec == CPaletteMode::Column);
		m_Button_Mode_Page.SetCheck(m_Id_CurModePrec == CPaletteMode::Page);

		// Rien dans la sélection en cours
		CString TxtMess;
		TxtMess.LoadString(IDS_AN_MESS_SELQULIGNE); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
		// AfxMessageBox("Veuillez sélectionner des questions à mettre en ligne", MB_ICONEXCLAMATION);
	}

}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode colonnes pour les tris croisés
void CDlgTableau::OnBnClickedRiRadModeColonnes()
{
	// Effacement des fenêtres dimension mobiles
	if (m_pCurMode != m_pCurModeColonne)
		m_pCurMode->EffaceFenDimDlg(false);

	// Si Bouton Controle Actif au même moment, on est en mode non effacement item courant
	JBool EffaceSelCur = true;
	if(GetAsyncKeyState(VK_CONTROL)<0)
		EffaceSelCur = false;

	if (m_List_SAS.GetCount() > 0)
	{
		// En blanc liste sélections colonne
		m_List_Row.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Row.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Col.				SetBkColor(CATPColors::GetColorWhite());
		m_List_Col.				SetBackColor(CATPColors::GetColorWhite());
		m_List_Page.			SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));

		// Active le bouton colonne, désactive les autres
		m_Button_Mode_Ligne.SetTypeOnglet(FALSE,FALSE,TRUE); 
		m_Button_Mode_Colonne.SetTypeOnglet(FALSE,TRUE,TRUE); 
		m_Button_Mode_Page.SetTypeOnglet(FALSE,FALSE,TRUE); 

		// Boutons sélection non actifs
		/* Modif Filtre/Filtre Janv 2007 (permet de valider choix items avec filtre dim 1 et filtre dim 2 A voir !!!!)
		if (!m_AjoutSelColActif_Dim1 && !m_AjoutSelColActif_Dim2)
			return;
		*/

		// Mise à jour du pupitre avec nouveau mode
		// OPTIM 2.7 UpdatePupitre(IDC_RI_RAD_MODE_COLONNES);
		m_Id_CurMode		=	CPaletteMode::Column;
		// FIN OPTIM
	}

	// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non sélectionné
	if (m_pCurMode)
	{
		// Valider 1er élmt actif du sas
		if (m_List_SAS.GetCount() > 0)
		{
			////////////////////////////////////////// TEST /////////////////////////////////
			int InxCurSel = m_List_SAS.GetCurSel();
			if (m_List_SAS.GetCurSel() == LB_ERR)
			{
				// Active 1er item sélection du sas 
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();

				// Met à jour la liste des questions via tree item
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					// Sélection des questions dans tree-items
					m_Tree_Theme.SetQuestionList(ListQuestionSAS);
				}

				// Update list des questions
				m_pCurMode->UpdateQuestionList();
			}
			else
			{
				// Ajout en attente du problème transposition !!!!!
				for (JUnt32 IndexSas = 0; IndexSas < m_List_SAS.GetCount(); IndexSas++)
				{
					// Active 1er item sélection du sas 
					m_List_SAS.SetCurSel(IndexSas);
					m_pCurMode->OnSelchange_ListSas();

					// Met à jour la liste des questions via tree item
					// Build List all current questions in all segment 
					JList <JInt32> ListQuestionSAS;
					m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

					// Sélectionner les items tree-questions à afficher dans la liste des questions
					if (ListQuestionSAS.GetCount() > 0)
					{
						// Sélection des questions dans tree-items
						m_Tree_Theme.SetQuestionList(ListQuestionSAS);
					}

					// Update list des questions
					m_pCurMode->UpdateQuestionList();
				}

				// Revient activation item sélectionné du sas 
				m_List_SAS.SetCurSel(InxCurSel);
				m_pCurMode->OnSelchange_ListSas();

				// Met à jour la liste des questions via tree item
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					// Sélection des questions dans tree-items
					m_Tree_Theme.SetQuestionList(ListQuestionSAS);
				}

				// Update list des questions
				m_pCurMode->UpdateQuestionList();
			}

		}
	}

	// Valider 1er élmt actif du sas
	if (m_List_SAS.GetCount() > 0)
	{
		// Mise à jour affichage via nouveau mode
		AffichePaletteModeCur();

		// Mise à jour nb colonnes
		UpdateCount();

		// Active 1er item sélection du sas 
		m_List_SAS.SetCurSel(0);
		m_pCurMode->OnSelchange_ListSas();

		// Positionne nouveau mode
		m_Id_CurModePrec = CPaletteMode::Column;

		// Mise à jour liste sélection
		UpdateListSelect();

		// Disposition liste résultat
		SetListeResultat();

		// Si on est en mode effacement sel
		if (m_List_SAS.GetCount() > 0 && EffaceSelCur)
			m_pCurMode->OnItem_Raz();
	}
	else
	{
		// Checké le dernier mode en cours
		m_Button_Mode_Ligne.SetCheck(m_Id_CurModePrec == CPaletteMode::Line);
		m_Button_Mode_Colonne.SetCheck(m_Id_CurModePrec == CPaletteMode::Column);
		m_Button_Mode_Page.SetCheck(m_Id_CurModePrec == CPaletteMode::Page);

		// Rien dans la sélection en cours
		CString TxtMess;
		TxtMess.LoadString(IDS_AN_MESS_SELQUCOL); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Sélection mode pages pour les tris croisés
void CDlgTableau::OnBnClickedRiRadModePages()
{
	// Effacement des fenêtres dimension mobiles
	if (m_pCurMode != m_pCurModePage)
		m_pCurMode->EffaceFenDimDlg(false);

	// Si Bouton Controle Actif au même moment, on est en mode non effacement item courant
	JBool EffaceSelCur = true;
	if(GetAsyncKeyState(VK_CONTROL)<0)
		EffaceSelCur = false;

	if (m_List_SAS.GetCount() > 0)
	{
		// En blanc liste sélections page
		m_List_Row.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Row.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Col.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Col.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBkColor(CATPColors::GetColorWhite());
		m_List_Page.			SetBackColor(CATPColors::GetColorWhite());

		// Active le bouton page, désactive les autres
		m_Button_Mode_Ligne.SetTypeOnglet(FALSE,FALSE,TRUE); 
		m_Button_Mode_Colonne.SetTypeOnglet(FALSE,FALSE,TRUE); 
		m_Button_Mode_Page.SetTypeOnglet(FALSE,TRUE,TRUE); 

		// Boutons sélection non actifs
		/* Modif Filtre/Filtre Janv 2007 (permet de valider choix items avec filtre dim 1 et filtre dim 2 A voir !!!!)
		if (!m_AjoutSelPageActif_Dim1 && !m_AjoutSelPageActif_Dim2)
			return;
		*/

		// Mise à jour du pupitre avec nouveau mode
		// OPTIM 2.7 UpdatePupitre(IDC_RI_RAD_MODE_PAGES);
		m_Id_CurMode		=	CPaletteMode::Page;
		// FIN OPTIM
	}

	// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non sélectionné
	if (m_pCurMode)
	{
		// Valider 1er élmt actif du sas
		if (m_List_SAS.GetCount() > 0)
		{
			int InxCurSel = m_List_SAS.GetCurSel();
			if (m_List_SAS.GetCurSel() == LB_ERR)
			{
				// Active 1er item sélection du sas
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();

				// NEW !!!!
				// Met à jour la liste des questions via tree item
				// Build List all current questions in all segment 
				JList <JInt32> ListQuestionSAS;
				m_pCurMode->Find_IBListQuestionSAS(ListQuestionSAS);

				// Sélectionner les items tree-questions à afficher dans la liste des questions
				if (ListQuestionSAS.GetCount() > 0)
				{
					// Sélection des questions dans tree-items
					m_Tree_Theme.SetQuestionList(ListQuestionSAS);
				}

				// Update list des questions
				m_pCurMode->UpdateQuestionList();
			}
		}
	}

	// Valider 1er élmt actif du sas
	if (m_List_SAS.GetCount() > 0)
	{
		// Mise à jour affichage via nouveau mode
		AffichePaletteModeCur();

		// Mise à jour nb colonnes
		UpdateCount();

		// Active 1er item sélection du sas 
		m_List_SAS.SetCurSel(0);
		m_pCurMode->OnSelchange_ListSas();

		// Positionne nouveau mode
		m_Id_CurModePrec = CPaletteMode::Page;

		// Mise à jour liste sélection
		UpdateListSelect();

		// Disposition liste résultat
		SetListeResultat();

		// Si on est en mode effacement sel
		if (m_List_SAS.GetCount() > 0 && EffaceSelCur)
			m_pCurMode->OnItem_Raz();
	}
	else
	{
		// Checké le dernier mode en cours
		m_Button_Mode_Ligne.SetCheck(m_Id_CurModePrec == CPaletteMode::Line);
		m_Button_Mode_Colonne.SetCheck(m_Id_CurModePrec == CPaletteMode::Column);
		m_Button_Mode_Page.SetCheck(m_Id_CurModePrec == CPaletteMode::Page);

		// Rien dans la sélection en cours
		CString TxtMess;
		TxtMess.LoadString(IDS_AN_MESS_SELQUPAGE); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
		// AfxMessageBox("Veuillez sélectionner des questions à mettre en page", MB_ICONEXCLAMATION);
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Disposition listes résultas (lignes, colonnes, pages)
void CDlgTableau::SetListeResultat()
{
	switch (m_Id_CurMode)
	{

		// Mode ligne en cours
		case CPaletteMode::Line: 
		{
			if (m_List_Col.GetItemCount() == 0 && m_List_Page.GetItemCount() == 0)
			{
				// 1 seule fenêtre résultat > liste lignes
				// SetFenResultat(1);

				// Maintenant décision de faire apparaitre les 2 fenetres principales (sélection lignes et colonnes)
				SetFenResultat(2);
			}
			else if (m_List_Page.GetItemCount() == 0)
			{
				// 2 seule fenêtre résultat > liste lignes et liste colonnes
				SetFenResultat(2);
			}
			else
			{
				// Les 3 fenêtres lignes, colonnes et pages
				SetFenResultat(3);
			}
		}
		break;

		// Mode colonne en cours
		case CPaletteMode::Column: 
		{
			
			if (m_List_Page.GetItemCount() == 0)
			{
				if (m_List_Col.GetItemCount() != 0)
				{
					// 2 seule fenêtre résultat > liste lignes et liste colonnes
					SetFenResultat(2);
				}
				else
				{
					// 1 seule fenêtre résultat > liste lignes
					// SetFenResultat(1);

					// Les 2 fenêtres lignes et colonnes tout le temps
					SetFenResultat(2);
				}
			}
			else
			{
				// Les 3 fenêtres lignes, colonnes et pages
				SetFenResultat(3);
			}
		}
		break;

		// Mode page en cours
		case CPaletteMode::Page: 
		{
			if (m_List_Page.GetItemCount() != 0)
			{
				// Les 3 fenêtres lignes, colonnes et pages
				SetFenResultat(3);
			}
			else
			{
				if (m_List_Col.GetItemCount() != 0)
				{
					// 2 seule fenêtre résultat > liste lignes et liste colonnes
					SetFenResultat(2);
				}
				else
				{
					// 1 seule fenêtre résultat > liste lignes
					// SetFenResultat(1);

					// Toujours les fenetres lignes et colonnes
					SetFenResultat(2);
				}
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage de la palette du mode en cours
void CDlgTableau::AffichePaletteModeCur()
{
	// On vide d'abord à l'écran la palette du mode précédent
	// OnItem_Raz();

	// Puis on met à jour avec nouvelle palette du mode en cours
	switch (m_Id_CurMode)
	{
		case CPaletteMode::Line : 
		{
			
		}
		break;


		case CPaletteMode::Column :
			break;
		case CPaletteMode::Page:
			break;
	}
}
	


///////////////////////////////////////////////////////////////////////////////////
// Gestion boutons pupitre
void CDlgTableau::UpdatePupitre(int ID)
{
	// A VOIR
	// GetItems()->Save();
	
	// Affichage en entete du mode "LIGNES" ou "COLONNES" ou "PAGES"
	switch (ID)
	{
		// Mode lignes
		case IDC_RI_RAD_MODE_LIGNES:
		{
			m_Id_CurMode		=	CPaletteMode::Line;
			UpdateMode(m_Id_CurMode);
			break;
		}

		// Mode colonnes
		case IDC_RI_RAD_MODE_COLONNES:
		{
			m_Id_CurMode		=	CPaletteMode::Column;

			// UpdateMode(m_Id_CurMode);
			break;
		}

		// Mode pages
		case IDC_RI_RAD_MODE_PAGES:
		{
			m_Id_CurMode		=	CPaletteMode::Page;

			// UpdateMode(m_Id_CurMode);
			break;
		}
	}
	
	// Valide le bouton
	CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,ID);

	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Enables or disables windows according to the selected mode + mise à jour sélection mode courant
void CDlgTableau::UpdateMode(int ID, bool MajSAS)
{
	// Sauve la config item en cours, sur le mode en cours (ligne ou colonne ou page)
	// CItemsConstruits * pItemsConstruit = GetItems();
	CItemsConstruitsBase * pItemsConstruit = GetItems();
	GetItems()->Save();

	/////////////////////////////////////////////////////////////////////////////////
	// Sauve le mode ligne ou colonne ou page précédent
	if (m_Id_CurModePrec == CPaletteMode::Line)
	{
		// Sauve mode ligne en cours
		if (m_pCurMode)
			m_pCurModeLigne = m_pCurMode;
	}
	else if (m_Id_CurModePrec == CPaletteMode::Column)
	{
		// Sauve mode colonne en cours
		if (m_pCurMode)
			m_pCurModeColonne = m_pCurMode;
	}
	else if (m_Id_CurModePrec == CPaletteMode::Page)
	{
		// Sauve mode page en cours
		if (m_pCurMode)
			m_pCurModePage = m_pCurMode;
	}
	///////////////////////////////////////////////////////////////////////////////////////

	m_Modes.MoveTo(ID);
	if(!m_Modes.IsValid())
		return;

	// Mode en cours
	m_Id_CurMode = ID;

	// Change les textes opérateurs s'il y a lieu
	// SetUpdateOpeLogiques();

	CAbstractMode * pMode;
	bool NewMode = false;
	if ((m_Id_CurMode == CPaletteMode::Line		&& !m_pCurModeLigne)	||
		(m_Id_CurMode == CPaletteMode::Column		&& !m_pCurModeColonne)	||
		(m_Id_CurMode == CPaletteMode::Page		&& !m_pCurModePage))
	{
		pMode = m_Modes.GetItem().get();
		NewMode = true;
	}
	else
	{
		// Récup pointeur mode pour cette sélection
		if (m_Id_CurMode == CPaletteMode::Line)
			pMode = m_pCurModeLigne;
		if (m_Id_CurMode == CPaletteMode::Column)	
			pMode = m_pCurModeColonne;
		if (m_Id_CurMode == CPaletteMode::Page)
			pMode = m_pCurModePage;
	}

	// Mise à jour Curmode
	m_pCurMode = pMode;
	
	// Change les textes opérateurs s'il y a lieu
	// SetUpdateOpeLogiques();

	// On se remet par défaut avec une seule dimension
	SetDimension2Visible(false);

	// Mise à jour via source et terrain chargée à chaque fois
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JSource*  Src = Sources.m_TBLSources .GetItem(m_Id_Source);
	const JTerrain* Ter = Sources.m_TBLTerrains.GetItem(m_Id_Source, m_Id_Terrain);
	if(Src && Ter)
	{
		if(m_pTerrain && m_pTerrain->IsLoaded())  
		{
			// mIse à jour titre terrain et segments utilisés
			UpdateTitreTerrainSeg();
		}
	}

	// Ici on détecte changement de mode, afin de réinitialiser
	if (NewMode)
	{
		// Init mode page en cours -- pas pour pupitre
		m_Label_Questions.			ShowWindow(SW_SHOW);
		m_List_Questions.			EnableWindow(TRUE);

		// Activation des objets panel sas questions sélectionnées
		m_Button_Menu_Ope_QuSel.	EnableWindow(TRUE); 
		m_Button_SAS_ET.			EnableWindow(TRUE);
		m_Button_SAS_OU.			EnableWindow(TRUE);
		m_Button_SAS_RAZ.			EnableWindow(TRUE);

		// Activation des objets panel dimension 1
		m_List_Dimension1.			EnableWindow(TRUE);
		m_Button_Dimension1.		EnableWindow(TRUE);
		m_Button_Dim1_ET.			EnableWindow(TRUE);
		m_Button_Dim1_OU.			EnableWindow(TRUE);
		m_Button_Dim1_NI.			EnableWindow(TRUE);
		m_Button_Dim1_XOR.			EnableWindow(TRUE);
		m_Button_Menu_Ope_Dim1.		EnableWindow(TRUE);
		// m_Button_Filtre.			EnableWindow(TRUE);
		m_Button_Dim1_UP.			EnableWindow(TRUE);
		m_Button_Dim1_DOWN.			EnableWindow(TRUE);
		m_Button_Dim1_RAZ.			EnableWindow(TRUE);

		// Activation des objets panel dimension 2
		m_List_Dimension2.			EnableWindow(TRUE);
		m_Button_Dimension2.		EnableWindow(TRUE);
		m_Button_Dim2_ET.			EnableWindow(TRUE);
		m_Button_Dim2_OU.			EnableWindow(TRUE);
		m_Button_Dim2_NI.			EnableWindow(TRUE);
		m_Button_Dim2_XOR.			EnableWindow(TRUE);
		m_Button_Menu_Ope_Dim2.		EnableWindow(TRUE); 
		m_Button_Dim2_RAZ.			EnableWindow(TRUE);
		m_Tab_Segment.				EnableWindow(TRUE);

		// Activation des objets panel segments
		m_Button_Menu_Ope_Segment.	EnableWindow(TRUE); 
		m_Button_Segment_OU.		EnableWindow(TRUE);
		m_Button_Segment_ET.		EnableWindow(TRUE);
		// m_Button_Segment_RAZ.		EnableWindow(TRUE);

		m_Tree_Theme.				EnableWindow(TRUE);
		m_List_Items.				EnableWindow(TRUE);

		m_Button_Title.				EnableWindow(TRUE);
		m_Button_TreeQuestions.		EnableWindow(TRUE);
        m_Button_List1.				EnableWindow(TRUE);
		m_Button_List2.				EnableWindow(TRUE);
		m_Edit_Search.				EnableWindow(TRUE);
		m_Button_RAZ_search.		EnableWindow(TRUE);
		m_Button_Genese.			EnableWindow(TRUE);
		m_Edit_QFilter.				EnableWindow(TRUE);
		m_Button_QFilter.			EnableWindow(TRUE);
		m_Button_QFilterRaz.		EnableWindow(TRUE);
		// m_Button_Suppression.		EnableWindow(TRUE);
		m_Button_Accept.			EnableWindow(TRUE);
		m_Button_Genese.			EnableWindow(TRUE);
		m_Static_Effectif.			EnableWindow(TRUE);
		m_Static_Percent.			EnableWindow(TRUE);
		m_Static_NbCas.				EnableWindow(TRUE);
		m_Edit_Name.				EnableWindow(TRUE);
		
		// Default results
		m_Edit_Name      .SetWindowText(""); 
		m_Static_Effectif.SetText("");
		m_Static_NbCas   .SetText("");
		m_Static_Percent .SetText("");

		/*
		// Mise à jour via source et terrain chargée
		const JSRCTables & Sources = JSRCTables::GetInstance ();
		const JSource*  Src = Sources.m_TBLSources .GetItem(m_Id_Source);
		const JTerrain* Ter = Sources.m_TBLTerrains.GetItem(m_Id_Source, m_Id_Terrain);
		if(Src && Ter)
		{
			if(m_pTerrain && m_pTerrain->IsLoaded())  
			{
				// mIse à jour titre terrain et segments utilisés
				UpdateTitreTerrainSeg();
			}
		}
		*/

		// Mode Pupitre via Tris Croisés
		// =============================
		// boutons items + palettes
		m_Button_List1.SetBitmaps(IDB_RI_LOGO_ITEM, RGB(192,192,192), IDB_RI_LOGO_ITEM, RGB(192,192,192));
		m_Button_List2.SetBitmaps(IDB_RI_LOGO_PALETTE, RGB(192,192,192), IDB_RI_LOGO_PALETTE, RGB(192,192,192));

		// Pas de logo dans le titre
		m_Button_Title.ShowWindow(SW_HIDE); 

		// Mode rep items
		SetVisibleBoiteValidItem(true);
		
		UpdateData(false);

		m_pCurMode = pMode ;
		m_bSegment = 0;
		m_Button_CalcSegment.SetCheck(m_bSegment, FALSE);

		// Vide la liste des questions, et passe en mode mono dimension
		InitListQuestion();

		// +++ vire sélection liste item
		m_Tree_Theme.RemoveSelections(); 
		
		// Init nouveau mode
		if (m_pCurMode)
			m_pCurMode->StartMode();

		// Positionne le mode courant pour la liste des questions
		m_Tree_Theme.SetModeVisu(m_Id_CurMode); 

		// On modifie aspect bouton, onglet tree-theme actif
		m_Button_TreeQuestions.SetTypeOnglet(TRUE,TRUE,TRUE);
		m_Button_List1.SetTypeOnglet		(TRUE,FALSE,TRUE);
		m_Button_List2.SetTypeOnglet		(TRUE,FALSE,TRUE);
		
		OnQFilter_RAZ();

		// Opérateurs par défaut
		AllOperateurParDefaut();

		/////////////////////////////////////////////////////////////////////////////////
		// Sauve le mode ligne ou colonne ou page courante
		if (m_Id_CurMode == CPaletteMode::Line)
		{
			// Sauve mode ligne en cours
			m_pCurModeLigne = m_pCurMode;
		}
		else if (m_Id_CurMode == CPaletteMode::Column)
		{
			// Sauve mode colonne en cours
			m_pCurModeColonne = m_pCurMode;
		}
		else if (m_Id_CurMode == CPaletteMode::Page)
		{
			// Sauve mode page en cours
			m_pCurModePage = m_pCurMode;
		}

	}
	else
	{
		// Mise en place du mode récupéré (ligne ou colonne ou page)
		if (m_pCurMode)
		{
			// Positionnement opérateur du SAS (Suite d'éléments ou croisement d'éléments)
			if (m_pCurMode->ModeCroisementPalette())
			{
				// Affichage choix menu opérateur SAS "ET"
				CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);
			}
			else
			{
				// Affichage choix menu opérateur SAS "OU"
				CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_OU);
			}
	
			// Mise en place des opérateurs logiques
			SetUpdateOpeLogiques();
			
			// Récup les composants
			m_pCurMode->RecupMode();

			// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non sélectionné
			if (MajSAS)
				ValideElmtSas(false);
		}
	}
}


// Displays the selected theme, support window
void CDlgTableau::UpdateTheme(int ID)
{
	// Boite Tree Theme visible, boite résultat non visible et boite pupitre non visible
	SetVisibleBoiteTreeTheme(true);
	SetVisibleBoiteValidItem(true);
	SetVisibleBoiteResultat(false);

	// Boite et composants pupitre visible
	SetVisibleBoitePupitre(true);
	SetVisibleModePupitre(true);

	// Efface les fenêtres mobiles restantes
	if (m_pCurMode)
		m_pCurMode->EffaceFenDimDlg(false);
	
	// activation ou pas des boutons tris si au moins 2 élmts
	UpdateBtnTriSel();

	GetItems()->Save();

	CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,ID);
	if(m_pCurMode)
		m_pCurMode->UpdateTheme(ID);

	// CheckRadioButton(IDC_RI_RAD_MODE_LIGNES,IDC_RI_RAD_MODE_PAGES,m_Id_CurMode);

	UpdateWindow();

	

	// Dessine l'onglet actif
	UpdateOngletConstruction(ID);
}

// Sets up the theme tree
void CDlgTableau::SetThemeData()
{
	m_Tree_Theme.SetData(m_pTerrain, GetItems(), m_QFilter, m_iModality);

	// Repositionne état du tree thème
	m_Tree_Theme.SetEtatData(m_pDoc->GetEtatTreeTheme()); 

	// Repositionne l'index thématique via dernière sélection
	m_Tree_Theme.TreeViaSauvegarde();
	m_Tree_Theme.UpdateQuestionList(); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour validation des boutons tri sélection lignes/colonnes/pages
void CDlgTableau::UpdateBtnTriSel()
{
	SetValidUpDownListe(m_List_Row);
	SetValidUpDownListe(m_List_Col);
	SetValidUpDownListe(m_List_Page);

	// Les boutons tri sélection
	m_Btn_All_Lignes.		EnableWindow(m_List_Row.GetItemCount()  > 1);
	m_Btn_All_Colonnes.		EnableWindow(m_List_Col.GetItemCount()  > 1);
	m_Btn_All_Pages.		EnableWindow(m_List_Page.GetItemCount()  > 1);
}	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//						TRAITEMENT ANCIEN DLG VECTORIELLE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDlgTableau::ValideSelectionTriCroise()
{
	// Aout 2013 : Calcul nombre total de cellules et message si au delà du maximum autorisé
	long NbItemsRow = m_List_Row.GetItemCount();
	if (NbItemsRow == 0) NbItemsRow = 1;
	long NbItemsCol = m_List_Col.GetItemCount();
	if (NbItemsCol == 0) NbItemsCol = 1;
	long NbItemsPage = m_List_Page.GetItemCount();
	if (NbItemsPage == 0) NbItemsPage = 1;
	long TotalCellules = NbItemsRow * NbItemsCol * NbItemsPage;

	// Pour le moment on limite à 20000 cellules ce qui est déjà pas mal
	if (TotalCellules > 5000)
	{
		JChar Buffer[500];
		sprintf_s(Buffer, 500, "%s\n%s%d%s","Attention, votre sélection dépasse le nombre de cellules maximum conseillé (5 000).", "Voulez-vous continuer quand même avec cette sélection de ", TotalCellules, " cellules ?");

		// if (AfxMessageBox(Buffer,MB_YESNO|MB_ICONQUESTION) == IDNO)
		if (MessageBox(Buffer, "Atelier Courrier", MB_YESNO|MB_ICONQUESTION) == IDNO)
		{		
			return false;
		}
	}
	
	// Les composants items lignes, colonnes, pages sélectionnés dans le pupitre
	CONST_LIST_ITEM_PUPITRE		Temp_Row;
	CONST_LIST_ITEM_PUPITRE		Temp_Col;
	CONST_LIST_ITEM_PUPITRE		Temp_Page;

	// Totalité des sélections
	LIST_ITEM_PUPITRE			Temp_Original;

	// Remove all new items
	m_Items_New.Reset();

	
	// Retrieve all rows
	int NbItems = m_List_Row.GetItemCount();
	for(int n=0; n < NbItems; ++n)
	{
		DWORD_PTR  data = m_List_Row.GetItemData(n) ;
		if(data)
		{
			Temp_Original.AddTail() = (CItemWrapper *)data; 
			Temp_Row.AddTail() = (const CItemWrapper *)data; 
		}
	}

	// Check if rows have changed
	if (m_bRowsChanged || m_Items_Row.GetCount() != Temp_Row.GetCount())
	{
		// Definately changed
		m_Items_Row.Swap(Temp_Row);
		m_bRowsChanged = true;
	}
	else
	{
		// A more thoughrough test
		for(m_Items_Row.MoveFirst(), Temp_Row.MoveFirst(); m_Items_Row.IsValid(); m_Items_Row.MoveNext(), Temp_Row.MoveNext())
		{
			if(m_Items_Row.GetItem() != Temp_Row.GetItem())
			{
				// Has changed
				m_Items_Row.Swap(Temp_Row);
				m_bRowsChanged =true;
				break;
			}
		}
	}

	// Add to the column list
	NbItems = m_List_Col.GetItemCount();
	for(int n=0; n < NbItems; ++n)
	{
		DWORD_PTR data = m_List_Col.GetItemData(n) ;
		if(data)
		{
			Temp_Original.AddTail() = (CItemWrapper *)data; 
			Temp_Col.AddTail() = (const CItemWrapper *)data; 
		}
	}

	// Check if rows have changed
	if(m_bColsChanged || m_Items_Col.GetCount() != Temp_Col.GetCount())
	{
		// Definately changed
		m_Items_Col.Swap(Temp_Col);
		m_bColsChanged = true;
	}
	else
	{
		// A more thoughrough test
		for(m_Items_Col.MoveFirst(), Temp_Col.MoveFirst(); m_Items_Col.IsValid(); m_Items_Col.MoveNext(), Temp_Col.MoveNext())
		{
			if(m_Items_Col.GetItem() != Temp_Col.GetItem())
			{
				// Has changed
				m_Items_Col.Swap(Temp_Col);
				m_bColsChanged =true;
				break;
			}
		}
	}

	// Add to the page list
	NbItems = m_List_Page.GetItemCount();
	for(int n=0; n < NbItems; ++n)
	{
		DWORD_PTR data = m_List_Page.GetItemData(n) ;
		if(data)
		{
			Temp_Original.AddTail() = (CItemWrapper *)data; 
			Temp_Page.AddTail() = (const CItemWrapper *)data; 
		}
	}

	// Check if rows have changed
	if(m_bPagesChanged || m_Items_Page.GetCount() != Temp_Page.GetCount())
	{
		// Definately changed
		m_Items_Page.Swap(Temp_Page);
		m_bPagesChanged = true;
	}
	else
	{
		// A more thoughrough test
		for(m_Items_Page.MoveFirst(), Temp_Page.MoveFirst(); m_Items_Page.IsValid(); m_Items_Page.MoveNext(), Temp_Page.MoveNext())
		{
			if(m_Items_Page.GetItem() != Temp_Page.GetItem())
			{
				// Has changed
				m_Items_Page.Swap(Temp_Page);
				m_bPagesChanged =true;
				break;
			}
		}
	}

	if(m_bRowsChanged || m_bColsChanged || m_bPagesChanged)
	{
		// Remove original list
		m_Items_Original.Swap(Temp_Original);

		m_bItemsChanged = true;
	}
	return true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Positionne les choix déjà effectués  ------  TRIS CROISES >>>> PUPITRE
// Affecter les modes en cours ligne, colonne, page aux élmts correspondant dans pupitre
bool CDlgTableau::SetData(CAnalyseurDoc *pDoc)
{
	// Positionne les modifications évrntuelles tris croisés
	m_bItemsChanged	=	false;
	m_bRowsChanged	=	false;
	m_bColsChanged	=	false;
	m_bPagesChanged =	false;

	if(pDoc)
	{
		// Récup document
		m_pDoc = pDoc;

		m_pTerrain = pDoc->Get_TerrainTemoin();
		pDoc->GetAllItems(m_Items_Original);
		pDoc->GetRowItems(m_Items_Row);
		pDoc->GetColItems(m_Items_Col);
		pDoc->GetPageItems(m_Items_Page);
		m_DType_Row		=	pDoc->GetRowDataType();
		m_DType_Col		=	pDoc->GetColDataType();
		m_DType_Page	=	pDoc->GetPageDataType();

		m_TypePoidsId = pDoc->Get_TypePoidsId();
		const JList<UINT> & SegmentsId = pDoc->Get_SegmentsId();
		m_SegmentsId.Reset();
		for (SegmentsId.MoveFirst(); SegmentsId.IsValid(); SegmentsId.MoveNext())
			m_SegmentsId.AddTail() = SegmentsId.GetItem();

		if(m_pTerrain)
		{
			m_Id_Source = m_pTerrain->GetSourceID();
			m_Id_Terrain = m_pTerrain->GetTerrainID();
		}

		// Récupère les dernières sélections mode pupitre de tris croisés
		pDoc->GetModeLigne(m_pCurModeLigne); 
		pDoc->GetModeColonne(m_pCurModeColonne); 
		pDoc->GetModePage(m_pCurModePage); 
		
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére les choix effectués dans pupitre ----  PUPITRE >>> TRIS CROISES
bool CDlgTableau::GetData(CAnalyseurDoc *pDoc)
{
	// remove deleted items to complete list
	for(m_Items_Del.MoveFirst(); m_Items_Del.IsValid(); m_Items_Del.MoveNext())
		delete m_Items_Del.GetItem();
	m_Items_Del.Reset();

	if(pDoc)
	{
		CWaitCursor Cw;

		if(m_bItemsChanged)
			pDoc->SetAllItems(m_Items_Original);

		if(m_bRowsChanged)
			pDoc->SetRowItems(m_Items_Row, m_DType_Row);

		if(m_bColsChanged)
			pDoc->SetColItems(m_Items_Col, m_DType_Col);

		if(m_bPagesChanged)
			pDoc->SetPageItems(m_Items_Page, m_DType_Page);

		// A VOIR
		// Récupère aussi les pointeurs via les différents mode ligne, colonne, page
		if(m_bRowsChanged)
			pDoc->SetModeLigne(m_pCurModeLigne); 
		if(m_bColsChanged)
			pDoc->SetModeColonne(m_pCurModeColonne); 
		if(m_bPagesChanged)
			pDoc->SetModePage(m_pCurModePage); 

		return true;

	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour compteur nb sélection lignes, colonnes ou pages
void CDlgTableau::UpdateCount()
{
	CString Label;
	CString Txt;

	// Affichage nb items en ligne
	Txt.LoadString(IDS_AN_LINE);
	Label.Format("%d %s",m_List_Row.GetItemCount(),Txt); 
	// m_Label_Lignes.SetWindowText(Label);
	m_Label_Lignes.SetText(Label);
	
	// Affichage nb items en colonne
	Txt.LoadString(IDS_AN_COLUMN);
	Label.Format("%d %s",m_List_Col.GetItemCount(),Txt); 
	// m_Label_Colonnes.SetWindowText(Label);
	m_Label_Colonnes.SetText(Label);

	// Affichage nb items en page
	Txt.LoadString(IDS_AN_PAGE);
	Label.Format("%d %s",m_List_Page.GetItemCount(),Txt); 
	m_Label_Pages.SetText(Label);

	// Activation ou pas des boutons sélection tri (lignes/colonnes/pages)
	UpdateBtnTriSel();
}

void CDlgTableau::Cancel()
{
	OnCancel();
}

void CDlgTableau::Hide()
{
	this->ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////////////////
// Enleve tous les items
void CDlgTableau::OnCancel()
{
	// remove all new items
	for(m_Items_New.MoveFirst(); m_Items_New.IsValid(); m_Items_New.MoveNext())
		delete m_Items_New.GetItem();

	// efface les fenêtres flottantes
	if (m_pCurMode)
		m_pCurMode->EffaceFenDimDlg(true);

	m_Items_New.Reset();
	m_Items_Del.Reset();

	// Pas de changement à prendre en compte
	m_bItemsChanged	=	false;
	m_bRowsChanged	=	false;
	m_bColsChanged	=	false;
	m_bPagesChanged =	false;

	

	// Supprime les mémoires de sauvegarde des modes
	delete pMementoLigne;
	delete pMementoColonne;
	delete pMementoPage;

	CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////////
// Validation sélection en mode (ligne ou colonne ou page) courant
// >> mise à jour de la liste correspondante, via ma sélection mode palette en cours
//
////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste sélection courante via les sélections mode palette
void CDlgTableau::UpdateListSelect(bool ModeSelItemQuanti)
{
	if(m_pCurMode)
	{
		// En mode normal, les items viennent de la sélection du questionnaire
		if (!ModeSelItemQuanti)
		{
			// Construire les nouveaux items
			if(m_pCurMode->BuildSelectedItems(true))
			{
				// Récupère els items sélectionnées
				m_pCurMode->GetSelectedItems(m_Items);

				// Récupére liste des items sélections à ajouter
				JList<CItemWrapper *> List;
				GetSelectedItems(List);

				// Mise à jour des listes sélections
				MiseAJourListeSelection(List);
			}

			// Retaille largeur colonnes des liste de  sélection
			ResizeColListSelect(m_Id_CurMode);
		}
		// Ici on vient de la sélection via le sélecteur d'items quantitatifs
		else
		{
			// Nombre des items quantis à ajouter
			JUnt32 Nb = m_AllItemsAutos.GetCount() + m_AllItemsUtils.GetCount();

			// Mise à jour des listes sélections
			MiseAJourListeSelection(m_AllItemsAutos);
			MiseAJourListeSelection(m_AllItemsUtils);

			// Retaille largeur colonnes des liste de  sélection
			ResizeColListSelect(m_Id_CurMode);
		}
	}

	// Mise à jour des compteurs nb sélection
	UpdateCount();
}

///////////////////////////////////////////////////////////////////////////////////////
// Validation sélection en mode (ligne ou colonne ou page) courant
// >> mise à jour de la liste correspondante, via ma sélection mode palette en cours
//
////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste sélection courante via les sélections mode palette
void CDlgTableau::UpdateListSelect_Synchro(bool Raz)
{
	/////////////////// ??????????????? A VIRER ///////////////////////////
	return;

	/// Tout ce qui suit marchait mais avec la version d'avant
	if(m_pCurMode)
	{

		// Recup Liste sélection lignes si on n'efface pas tout
		JList <CString> LstTmp;
		if (!Raz)
		{
			// Selon le mode courant, mise à jour des listes
			switch (m_Id_CurMode)
			{
				// Mode lignes
				case CPaletteMode::Line:
				{
					// Récup les composants de la sélection en ligne, et les stocke en attente
					for (int i= 0; i < m_List_Row.GetItemCount(); i++)
					{
						LstTmp.AddTail() = m_List_Row.GetItemText(i,0);
					}
					break;
				}

				// Mode colonnes
				case CPaletteMode::Column:
				{
					// Récup les composants de la sélection en colonne, et les stocke en attente
					for (int i= 0; i < m_List_Col.GetItemCount(); i++)
					{
						LstTmp.AddTail() = m_List_Col.GetItemText(i,0);
					}
					break;
				}

				// Mode pages
				case CPaletteMode::Page:
				{
					// Récup les composants de la sélection en page, et les stocke en attente
					for (int i= 0; i < m_List_Page.GetItemCount(); i++)
					{
						LstTmp.AddTail() = m_List_Page.GetItemText(i,0);
					}
					break;
				}
			}
		}

		// Construire
		if(m_pCurMode->BuildSelectedItems(true))
		{
			// Récupère els items sélectionnées
			m_pCurMode->GetSelectedItems(m_Items);

			// Récupére liste des items sélections
			JList<CItemWrapper *> List;
			GetSelectedItems(List);

			// Mise à jour des listes sélections
			MiseAJourListeSelection(List);
		}

		else
		{
			// Aucune sélection sur la liste en cours
			// Selon le mode courant, mise à jour des listes
			switch (m_Id_CurMode)
			{
				// Mode lignes
				case CPaletteMode::Line:
				{
					// Init Liste sélection lignes
					m_List_Row.DeleteAllItems();

					// On remet les précédents composants, car construction non active mais composants présents !!!
					if (!Raz)
					{
						// A Voir si on récupère la liste précédente sélection en ligne
						if (LstTmp.GetCount() > 0)
						{
							int i = m_List_Row.GetItemCount(); 
							for (LstTmp.MoveFirst(); LstTmp.IsValid(); LstTmp.MoveNext())
							{
								// Add to the list control
								CString Str = LstTmp.GetItem();
								m_List_Row.InsertItem(i,Str);
							}
						}
					}
				}
				break;

				// Mode colonnes
				case CPaletteMode::Column:
				{
					// Init Liste sélection colonnes
					m_List_Col.DeleteAllItems();

					// On remet les précédents composants, car construction non active mais composants présents !!!
					if (!Raz)
					{
						// A Voir si on récupère la liste précédente sélection en colonne
						if (LstTmp.GetCount() > 0)
						{
							int i = m_List_Col.GetItemCount(); 
							for (LstTmp.MoveFirst(); LstTmp.IsValid(); LstTmp.MoveNext())
							{
								// Add to the list control
								CString Str = LstTmp.GetItem();
								m_List_Col.InsertItem(i,Str);
							}
						}
					}
				}
				break;

				// Mode pages
				case CPaletteMode::Page:
				{
					// Init Liste sélection pages
					m_List_Page.DeleteAllItems();

					// On remet les précédents composants, car construction non active mais composants présents !!!
					if (!Raz)
					{
						// A Voir si on récupère la liste précédente sélection en page
						if (LstTmp.GetCount() > 0)
						{
							int i = m_List_Page.GetItemCount(); 
							for (LstTmp.MoveFirst(); LstTmp.IsValid(); LstTmp.MoveNext())
							{
								// Add to the list control
								CString Str = LstTmp.GetItem();
								m_List_Page.InsertItem(i,Str);
							}
						}
					}
				}
				break;
			}
		}

		// Retaille largeur colonnes des liste de  sélection
		ResizeColListSelect(m_Id_CurMode);
	}

	// Mise à jour des compteurs nb sélection
	UpdateCount();
}	

// Mise à jour sur Liste Row ou Col ou Page avec les nouveaux elmts sélectionnées de List (avec le tri)
void CDlgTableau::MajViaNewElementAvecTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List, int IndiceDepIdemHeader)
{
	int		PosTrouve = -1;
	bool	AjoutModaExistante = false;
	
	// Récupère le nom header correspondant à la question en cours d'ajout
	List.MoveFirst();
	CString NewHeader = List.GetItem()->GetHeader();

	// Init Liste tri
	m_List_Tri.DeleteAllItems();

	CString Str = ""; 
	// Puis on y ajoute les items déjà sélectionnés
	for (int i = IndiceDepIdemHeader; i < ListCtrlCourante.GetItemCount(); i++)
	{
		Str = ListCtrlCourante.GetItemText(i,0);

		PosTrouve = Str.Find(NewHeader);
		if (PosTrouve >= 0)
		{
			m_List_Tri.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)ListCtrlCourante.GetItemData(i));
		}
	}

	// Puis on ajoute les nouveaux
	int i = m_List_Tri.GetItemCount(); 
	for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
	{
		// Entete ligne
		CString StrEntete = List.GetItem()->GetHeader();

		// Suite ligne
		CString StrLigne = List.GetItem()->GetLabel();

		if (StrLigne != "Error")
		{
			// Format the string simple
			if (!m_AjoutSelLigActif_Dim1 && !m_AjoutSelLigActif_Dim2)
				// Inversion libellé dans le cas du mode Filtre/Filtre sur les 2 dimensions
				Str.Format("%s - %s",StrLigne, StrEntete); 
			else
				Str.Format("%s - %s",StrEntete,StrLigne); 

			// Add to the list control
			m_List_Tri.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());

		}
	}

	// Suppression des doublons
	SuppressionDoublonRepAbsolu(m_List_Tri, false);

	// A ce niveau on a une mini liste avec ts les elmts concernant la question courante triés
	//
	// Lock list
	ListCtrlCourante.LockWindowUpdate();

	
	// Puis on y ajoute les items déjà sélectionnés
	for (int i= 0; i<m_List_Tri.GetItemCount(); i++)
	{
		Str = m_List_Tri.GetItemText(i,0);
		
		// Add to the list control
		ListCtrlCourante.InsertItem(LVIF_PARAM|LVIF_TEXT,IndiceDepIdemHeader + i,Str,0,0,0,(LPARAM)m_List_Tri.GetItemData(i));

		// Positionne Flag changement sur un des modes
		if (Mode == CPaletteMode::Line)
			m_bRowsChanged = true;
		else if (Mode == CPaletteMode::Column)
			m_bColsChanged = true;
		else if (Mode == CPaletteMode::Page)
			m_bPagesChanged = true;

		// Rend visible le dernier élmt ligne ajouté
		if (ListCtrlCourante.GetItemCount() > 0)
			ListCtrlCourante.EnsureVisible(ListCtrlCourante.GetItemCount()-1,true); 
	}

	// Suppression des doublons
	SuppressionDoublonRepAbsolu(ListCtrlCourante, false);

	ListCtrlCourante.UnlockWindowUpdate();
}

// Mise à jour sur Liste Row ou Col ou Page avec les nouveaux elmts sélectionnées de List
// Ici on insére comme avant directement à la fin, on enlève juste les doublons
void CDlgTableau::MajViaNewElementSansTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List)
{
	// Add them to the List control and item count
	int i = ListCtrlCourante.GetItemCount(); 

	CString Str = ""; 
	
	// Lock list
	ListCtrlCourante.LockWindowUpdate();

	// Loop on all nex item to add in list row
	for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
	{
		// Entete ligne
		CString StrEntete = List.GetItem()->GetHeader();

		// Suite ligne
		CString StrLigne = List.GetItem()->GetLabel();

		if (StrLigne != "Error")
		{
			// Format the string simple
			if (!m_AjoutSelLigActif_Dim1 && !m_AjoutSelLigActif_Dim2)
				// Inversion libellé dans le cas du mode Filtre/Filtre sur les 2 dimensions
				Str.Format("%s - %s",StrLigne, StrEntete); 
			else
				Str.Format("%s - %s",StrEntete,StrLigne); 

			// Add to the list control
			ListCtrlCourante.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());
			
			// Positionne Flag changement sur un des modes
			if (Mode == CPaletteMode::Line)
				m_bRowsChanged = true;
			else if (Mode == CPaletteMode::Column)
				m_bColsChanged = true;
			else if (Mode == CPaletteMode::Page)
				m_bPagesChanged = true;

			// Add to the main item list
			m_Items_New.AddTail() = List.GetItem();
		}
	
		// Rend visible le dernier élmt ligne ajouté
		if (ListCtrlCourante.GetItemCount() > 0)
			ListCtrlCourante.EnsureVisible(m_List_Row.GetItemCount()-1,true);  
	}

	// Suppression des doublons
	SuppressionDoublonRepAbsolu(ListCtrlCourante, false);

	ListCtrlCourante.UnlockWindowUpdate();
}

///////////////////////////////////////////////////////////////////////////
// Mise à jour des listes sélections
void CDlgTableau::MiseAJourListeSelection(JList<CItemWrapper *> &List)
{
	int		IndiceDepIdemHeader = 0;
	bool	AjoutModaExistante = false;
	int		PosTrouve = 0;

	// Header nouvelle question ajout
	List.MoveFirst();
	CString NewHeader = List.GetItem()->GetHeader();

	// Selon le mode courant, mise à jour des listes
	switch (m_Id_CurMode)
	{
		// Mode lignes
		case CPaletteMode::Line:
		{
			// Par defaut on ajoute à la fin
			IndiceDepIdemHeader = m_List_Row.GetItemCount();

			// Repérer l'indice dans la liste existante de la même question
			for (int i= 0; i < m_List_Row.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				CString Txt = m_List_Row.GetItemText(i,0);

				PosTrouve = Txt.Find(NewHeader);
				if (PosTrouve >= 0)
				{
					// Détecter si des modalités existent déjà sur cette question en mode ligne
					IndiceDepIdemHeader = i;
					AjoutModaExistante = true;
					break;
				}
			}

			// On demande à l'utilisateur si on ajoute en tenant compte du tri pour cette question
			if (AjoutModaExistante)
			{
				if (AfxMessageBox("Attention, des éléments de cette question sont déjà sélectionnés en ligne, voulez-vous les insérer en les triant ?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
				{
					AjoutModaExistante = false;
				}
			}

			if (AjoutModaExistante)
			{
				// On ajoute les nouveaux elmts à la liste lignes en tenant compte du tri
				MajViaNewElementAvecTri(m_Id_CurMode, m_List_Row, List, IndiceDepIdemHeader);
			}
			else
			{
				// Comme avant on insére à la fin
				MajViaNewElementSansTri(m_Id_CurMode, m_List_Row, List);
			}
			break;
		}

		// Mode colonnes
		case CPaletteMode::Column:
		{
			// Par defaut on ajoute à la fin
			IndiceDepIdemHeader = m_List_Col.GetItemCount(); 

			// Repérer l'indice dans la liste existante de la même question
			for (int i= 0; i < m_List_Col.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				CString Txt = m_List_Col.GetItemText(i,0);

				PosTrouve = Txt.Find(NewHeader);
				if (PosTrouve >= 0)
				{
					// Détecter si des modalités existent déjà sur cette question en mode colonne
					IndiceDepIdemHeader = i;
					AjoutModaExistante = true;
					break;
				}
			}

			if (AjoutModaExistante)
			{
				// On demande à l'utilisateur si on ajoute en tenant compte du tri pour cette question
				if (AfxMessageBox("Attention, des éléments de cette question sont déjà sélectionnés en colonne, voulez-vous les insérer en les triant ?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
				{
					AjoutModaExistante = false;
				}
			}

			if (AjoutModaExistante)
			{
				// On ajoute les nouveaux elmts à la liste colonnes en tenant compte du tri
				MajViaNewElementAvecTri(m_Id_CurMode, m_List_Col, List, IndiceDepIdemHeader);
			}
			else
			{
				// Comme avant on insére à la fin
				MajViaNewElementSansTri(m_Id_CurMode, m_List_Col, List);
			}
			break;
		}

		// Mode pages
		case CPaletteMode::Page:
		{
			// Par defaut on ajoute à la fin
			IndiceDepIdemHeader = m_List_Page.GetItemCount(); 

			// Repérer l'indice dans la liste existante de la même question
			for (int i= 0; i < m_List_Page.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				CString Txt = m_List_Page.GetItemText(i,0);

				PosTrouve = Txt.Find(NewHeader);
				if (PosTrouve >= 0)
				{
					// Détecter si des modalités existent déjà sur cette question en mode page
					IndiceDepIdemHeader = i;
					AjoutModaExistante = true;
					break;
				}
			}

			if (AjoutModaExistante)
			{
				// On demande à l'utilisateur si on ajoute en tenant compte du tri pour cette question
				if (AfxMessageBox("Attention, des éléments de cette question sont déjà sélectionnés en page, voulez-vous les insérer en les triant ?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
				{
					AjoutModaExistante = false;
				}
			}

			if (AjoutModaExistante)
			{
				// On ajoute les nouveaux elmts à la liste pages en tenant compte du tri
				MajViaNewElementAvecTri(m_Id_CurMode, m_List_Page, List, IndiceDepIdemHeader);
			}
			else
			{
				// Comme avant on insére à la fin
				MajViaNewElementSansTri(m_Id_CurMode, m_List_Page, List);
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Mise à jour des listes sélections
/*
void CDlgTableau::OLD_MiseAJourListeSelection(JList<CItemWrapper *> &List)
{
	int IndiceDepIdemHeader = 0;
	int PosTrouve = 0;
	
	List.MoveFirst();
	CString NewHeader = List.GetItem()->GetHeader();


	// Selon le mode courant, mise à jour des listes
	switch (m_Id_CurMode)
	{
		// Mode lignes
		case CPaletteMode::Line:
		{
			// Add them to the List control and item count
			int i = m_List_Row.GetItemCount(); 

			CString Str = ""; 
			
			// Lock list
			m_List_Row.LockWindowUpdate();

			// Loop on all nex item to add in list row
			for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
			{
				// Entete ligne
				CString StrEntete = List.GetItem()->GetHeader();

				// Suite ligne
				CString StrLigne = List.GetItem()->GetLabel();

				if (StrLigne != "Error")
				{
					// Format the string simple
					if (!m_AjoutSelLigActif_Dim1 && !m_AjoutSelLigActif_Dim2)
						// Inversion libellé dans le cas du mode Filtre/Filtre sur les 2 dimensions
						Str.Format("%s - %s",StrLigne, StrEntete); 
					else
						Str.Format("%s - %s",StrEntete,StrLigne); 

					// Add to the list control
					m_List_Row.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());
											
					m_bRowsChanged = true; 
					// Add to the main item list
					m_Items_New.AddTail() = List.GetItem();
				}
			
				// Rend visible le dernier élmt ligne ajouté
				if (m_List_Row.GetItemCount() > 0)
					m_List_Row.EnsureVisible(m_List_Row.GetItemCount()-1,true);  
			}

			// Trier la liste des lignes en cours
			TriAllItemsOrdreAlpha(m_List_Row);

			// Suppression des doublons
			SuppressionDoublonRepAbsolu(m_List_Row, false);

			m_List_Row.UnlockWindowUpdate();
			break;
		}

		// Mode colonnes
		case CPaletteMode::Column:
		{
			// Add them to the List control and item count
			int i = m_List_Col.GetItemCount(); 
			for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
			{
				// Entete ligne
				CString StrEntete = List.GetItem()->GetHeader();

				// Suite ligne
				CString StrLigne = List.GetItem()->GetLabel();

				if (StrLigne != "Error")
				{
					CString Str;

					// Format the string simple
					if (!m_AjoutSelColActif_Dim1 && !m_AjoutSelColActif_Dim2)
						// Inversion libellé dans le cas du mode Filtre/Filtre sur les 2 dimensions
						Str.Format("%s - %s",StrLigne, StrEntete); 
					else
						Str.Format("%s - %s",StrEntete,StrLigne); 

					// Add to the list control
					m_List_Col.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());

					m_bColsChanged = true; 
					// Add to the main item list
					m_Items_New.AddTail() = List.GetItem();
				}
			}

			// Rend visible le dernier élmt colonne ajouté
			if (m_List_Col.GetItemCount() > 0)
				m_List_Col.EnsureVisible(m_List_Col.GetItemCount()-1,false);  

			// Trier la liste des colonnes en cours
			TriAllItemsOrdreAlpha(m_List_Col);

			// Suppression des doublons
			SuppressionDoublonRepAbsolu(m_List_Col, false);

			break;
		}

		// Mode pages
		case CPaletteMode::Page:
		{
			// Add them to the List control and item count
			int i = m_List_Page.GetItemCount(); 
			for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
			{
				// Entete ligne
				CString StrEntete = List.GetItem()->GetHeader();

				// Suite ligne
				CString StrLigne = List.GetItem()->GetLabel();

				if (StrLigne != "Error")
				{
					CString Str;

					// Format the string simple
					if (!m_AjoutSelPageActif_Dim1 && !m_AjoutSelPageActif_Dim2)
						// Inversion libellé dans le cas du mode Filtre/Filtre sur les 2 dimensions
						Str.Format("%s - %s",StrLigne, StrEntete); 
					else
						Str.Format("%s - %s",StrEntete,StrLigne); 

					// Add to the list control
					m_List_Page.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)List.GetItem());

					m_bPagesChanged = true; 
					// Add to the main item list
					m_Items_New.AddTail() = List.GetItem();
				}
			}

			// Rend visible le dernier élmt page ajouté
			if (m_List_Page.GetItemCount() > 0)
				m_List_Page.EnsureVisible(m_List_Page.GetItemCount()-1,false);  

			// Trier la liste des pages en cours
			TriAllItemsOrdreAlpha(m_List_Page);

			// Suppression des doublons
			SuppressionDoublonRepAbsolu(m_List_Page, false);

			break;
		}
	}
}
*/

///////////////////////////////////////////////////////////////////////
// Tri des elmts sélectionnés en lignes ou colonnes ou pages
void CDlgTableau::TriAllItemsOrdreAlpha(JFCListCtrl &Liste)
{
	if (Liste.GetItemCount() >= 2)
	{
		CString Str;

		m_List_Tri.DeleteAllItems();
		for (int i= 0; i<Liste.GetItemCount(); i++)
		{
			Str = Liste.GetItemText(i,0);
			m_List_Tri.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)Liste.GetItemData(i));
		}

		// On récupére l'ordre
		Liste.DeleteAllItems();
		for (int i= 0; i<m_List_Tri.GetItemCount(); i++)
		{
			Str = m_List_Tri.GetItemText(i,0);
			Liste.InsertItem(LVIF_PARAM|LVIF_TEXT,i,Str,0,0,0,(LPARAM)m_List_Tri.GetItemData(i));
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Retaille largeur colonnes des liste de  sélection
void CDlgTableau::ResizeColListSelect(JUnt32 IdMode)
{
	// Mise à jour via taille des élmts
	JInt32 MaxTailleStr = 0;

	switch (IdMode)
	// switch (m_Id_CurMode)
	{
		// Mode lignes
		case CPaletteMode::Line:
		{
			// Récup les composants de la sélection en ligne, et les stocke en attente
			for (int i= 0; i < m_List_Row.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				CString Txt = m_List_Row.GetItemText(i,0);
				JInt32 Taille = m_List_Row.GetStringWidth(m_List_Row.GetItemText(i,0));

				// Si plus grand, on garde cette dernière taile
				if (Taille > MaxTailleStr)
					MaxTailleStr = Taille;
			}

			// Puis on redimensionne les colonnes
			m_List_Row.SetColumnWidth(0,MaxTailleStr+20); 
			break;
		}

		// Mode colonnes
		case CPaletteMode::Column:
		{
			// Récup les composants de la sélection en colonne, et les stocke en attente
			for (int i= 0; i < m_List_Col.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				JInt32 Taille = m_List_Col.GetStringWidth(m_List_Col.GetItemText(i,0));

				// Si plus grand, on garde cette dernière taile
				if (Taille > MaxTailleStr)
					MaxTailleStr = Taille;
			}

			// Puis on redimensionne les colonnes
			m_List_Col.SetColumnWidth(0,MaxTailleStr+50); 
			break;
		}

		// Mode pages
		case CPaletteMode::Page:
		{
			// Récup les composants de la sélection en page, et les stocke en attente
			for (int i= 0; i < m_List_Page.GetItemCount(); i++)
			{
				// Taille chaine de caractère de l'élmt
				JInt32 Taille = m_List_Page.GetStringWidth(m_List_Page.GetItemText(i,0));

				// Si plus grand, on garde cette dernière taile
				if (Taille > MaxTailleStr)
					MaxTailleStr = Taille;
			}

			// Puis on redimensionne les colonnes
			m_List_Page.SetColumnWidth(0,MaxTailleStr+50); 
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Changement de la question en cours
void CDlgTableau::OnSelchange_ListQuestions() 
{
	// AfxMessageBox("????", MB_ICONERROR);
	// Nouvelle question, on initialise le filtrage
	m_MapFiltrage.Reset();

	JUnt32 InxSel2 = m_List_Questions.GetCurSel();
	
	// Si dans liste dimension 1 aucun item de sélectionné, on déselectionne la question dans la liste des questions
	if(m_List_Questions.GetCurSel()		== LB_ERR && 
	   m_List_Dimension1.GetCount()		!= 0 && 
	   m_List_Dimension1.GetSelCount()	== 0) 
	{
		// Dévalide la question courante
		m_List_Questions.SetCurSel(LB_ERR);

		// Vire les sélections dans toutes les dimensions
		m_List_Dimension1.ResetContent(); 
		m_List_Dimension2.ResetContent(); 

		// Update de toutes les dimensions
		//m_pCurMode->UpdateDimensionsList();
		m_Button_Dimension1.SetCheck(0);
		m_Button_Dimension1.UpdateWindow(); 

		// efface les fenêtres flottantes
		m_pCurMode->EffaceFenDimDlg(true);

		if (m_List_SAS.GetCount() <= 0 && m_List_Questions.GetSelCount() <= 0)
			return;
	}

	// Mise à jour des questions container
	if(m_pCurMode)
	{
		// Modif question via le mode courant
		// m_pCurMode->OnSelchange_ListQuestions(); 

		// Récupére les infos questions, en autre pour savoir si plusieurs dimensions
		IQuestion * pCurQuestion = 0;
		JInt32 IDs = 0;
		if(m_List_Questions.GetCurSel() != LB_ERR)
		{
			// Passe id item et nb dimensions pour cette question
			pCurQuestion	= (IQuestion *)m_List_Questions.GetItemData(m_List_Questions.GetCurSel());
			IDs				= m_List_Questions.GetSelectedID();
		}
		
		// Récupére le paramétrage sélection
		if (IDs)
			LoadModeSelectionDim1Dim2(IDs,pCurQuestion->HasDimensions());

		// Modif question via le mode courant
		m_pCurMode->OnSelchange_ListQuestions(); 


		// Selon les modes on positionne ou pas les boutons sélections
		SetBoutonSelection();

		// !!!!!!
		// Attention laisser marquer les questions déjà sélectionnées
		JUnt32 IdSel = m_List_Questions.GetSelectedID();
		JUnt32 InxSel = m_List_Questions.GetCurSel();

		// Mise à jour liste sélection compo palettes
		// Attention, ne pas mettre à jour la liste des composants palettes si question sélectionnée, et
		// pas encore de dimensions sélectionnées ........... en cours de construction
		if(m_List_Questions.GetCurSel() != LB_ERR &&  m_pCurMode->BuildSelectedItemsPalette() == false)
		{
			// ne fait rien
		}
		else
			// On reconstruit les composants palettes
			UpdateListSelCompoPalette();

		// Replace les opérateurs logiques
		SetUpdateOpeLogiques();

		// En mode palette, on remet le bouton opé dimension 2 visible mais non actif
		if (pCurQuestion && pCurQuestion->HasDimensions() > 1)
			SetOpeDimension2(true,false);
		else
			SetOpeDimension2(false,false);

		// Mise à jour des compteurs nb sélection
		UpdateCount();


		//////////////////////////////////////////////////////////////////////////////////////////
		// A VOIR
		// Mise à jour des marquages liste question
		for (JInt32 Index = 0; Index < m_List_Questions.GetCount(); Index ++)
		{
			// Récup question
			pCurQuestion	= (IQuestion *)m_List_Questions.GetItemData(Index);

			if (pCurQuestion)
			{
				// Test si dans le SAS
				JInt32 IdQu = pCurQuestion->GetID();

				// Flag appartenance au SAS
				JBool QuInSAS = false;
				IQuestion * pCurQuSAS = 0;
				for (JInt32 IndexSAS = 0; IndexSAS < m_List_SAS.GetCount(); IndexSAS++)
				{
					// Test si la question est sélectionnée dans la SAS
					pCurQuSAS = (IQuestion *)m_List_SAS.GetItemData(IndexSAS);
					if (pCurQuSAS->GetID() == IdQu)
					{
						QuInSAS = true;
						break;
					}
				}

				// Si sélectionnée dans le SAS, on marque la question, sinon on la démarque
				if (QuInSAS)
					m_List_Questions.MarqueSel(IdQu); 
				else
					m_List_Questions.DemarqueSel(IdQu); 
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Changement de la sélection via le SAS
void CDlgTableau::OnSelchange_ListSas()
{
	// Affichage d'un autre élmt via le SAS
	if(m_pCurMode)
	{
		m_pCurMode->OnSelchange_ListSas(); 
	}
	
	// Repositionne les sélecteurs dim1 et dim2 pour cet élmt, via la précédente sélection
	IQuestion * pCurQuestion = 0;
	if(m_List_SAS.GetCurSel() != LB_ERR && m_List_Questions.GetCurSel() != LB_ERR)
	{
		// Passe id item et nb dimensions pour cette question
		pCurQuestion = (IQuestion *)m_List_Questions.GetItemData(m_List_Questions.GetCurSel());
		if (pCurQuestion)
		{
			JInt32 IDs = m_List_SAS.GetSelectedID();
			
			// Récupére le paramétrage sélection
			LoadModeSelectionDim1Dim2(IDs,pCurQuestion->HasDimensions());
		}
	}

	// Test bouton Dim1 Checké
	UpdateSelectionOpe();

	// Selon les modes on positionne ou pas les boutons sélections
	SetBoutonSelection();
	
	// Mise à jour sélection
	UpdateListSelect_Synchro(true);

	// Mise à jour nb cellules
	// UpdateNbCells();
}

// Selon les modes on positionne ou pas les boutons sélections
void CDlgTableau::SetBoutonSelection()
{
	// Selon que l'on est en mode ligne ou colonne ou page
	switch (m_Id_CurMode)
	{
		// Mode ajout lignes actif
		case CPaletteMode::Line:
		{
			// Pour dimension 1
			m_Button_Dimension1.SetCheck(m_AjoutSelLigActif_Dim1);
			
			// Pour dimension 2
			m_Button_Dimension2.SetCheck(m_AjoutSelLigActif_Dim2);
		}
		break;

		// Mode ajout colonnes actif
		case CPaletteMode::Column:
		{
			// Pour dimension 1
			m_Button_Dimension1.SetCheck(m_AjoutSelColActif_Dim1);
			
			// Pour dimension 2
			m_Button_Dimension2.SetCheck(m_AjoutSelColActif_Dim2);
		}
		break;

		// Mode ajout pages actif
		case CPaletteMode::Page:
		{
			// Pour dimension 1
			m_Button_Dimension1.SetCheck(m_AjoutSelPageActif_Dim1);
			
			// Pour dimension 2
			m_Button_Dimension2.SetCheck(m_AjoutSelPageActif_Dim2);
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde des modes des sélections pour dimension 1 et dimension 2
void CDlgTableau::SaveModeSelectionDim1Dim2(JInt32 IdItem)
{
	// Selon que l'on est en mode ligne ou colonne ou page
	switch (m_Id_CurMode)
	{
		// Mode ajout lignes actif
		case CPaletteMode::Line:
		{
			// Pour le mode sélection dimension 1
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;
			m_MapModeSelQuestionLigDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim1.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
				JBool &Actif = m_MapModeSelQuestionLigDim1.GetItem();
				Actif = m_AjoutSelLigActif_Dim1;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 1
				m_MapModeSelQuestionLigDim1.Add(KeyItemTerrain) = m_AjoutSelLigActif_Dim1;  
			}

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionLigDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				JBool &Actif = m_MapModeSelQuestionLigDim2.GetItem();
				Actif = m_AjoutSelLigActif_Dim2;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 1
				m_MapModeSelQuestionLigDim2.Add(KeyItemTerrain) = m_AjoutSelLigActif_Dim2;  
			}

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionLigDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				JBool &Actif = m_MapModeSelQuestionLigDim3.GetItem();
				Actif = m_AjoutSelLigActif_Dim3;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 3
				m_MapModeSelQuestionLigDim3.Add(KeyItemTerrain) = m_AjoutSelLigActif_Dim3;  
			}
		}
		break;

		// Mode ajout colonnes actif
		case CPaletteMode::Column:
		{
			// en remplacement
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;

			// Pour le mode sélection dimension 1
			m_MapModeSelQuestionColDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim1.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
				JBool &Actif = m_MapModeSelQuestionColDim1.GetItem();
				Actif = m_AjoutSelColActif_Dim1;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 1
				m_MapModeSelQuestionColDim1.Add(KeyItemTerrain) = m_AjoutSelColActif_Dim1;  
			}

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionColDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				JBool &Actif = m_MapModeSelQuestionColDim2.GetItem();
				Actif = m_AjoutSelColActif_Dim2;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 2
				m_MapModeSelQuestionColDim2.Add(KeyItemTerrain) = m_AjoutSelColActif_Dim2;  
			}

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionColDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				JBool &Actif = m_MapModeSelQuestionColDim3.GetItem();
				Actif = m_AjoutSelColActif_Dim3;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 3
				m_MapModeSelQuestionColDim3.Add(KeyItemTerrain) = m_AjoutSelColActif_Dim3;  
			}
		}
		break;

		// Mode ajout pages actif
		case CPaletteMode::Page:
		{
			// Pour le mode sélection dimension 1
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;
			m_MapModeSelQuestionPageDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim1.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
				JBool &Actif = m_MapModeSelQuestionPageDim1.GetItem();
				Actif = m_AjoutSelPageActif_Dim1;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 1
				m_MapModeSelQuestionPageDim1.Add(KeyItemTerrain) = m_AjoutSelPageActif_Dim1;  
			}

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionPageDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				JBool &Actif = m_MapModeSelQuestionPageDim2.GetItem();
				Actif = m_AjoutSelPageActif_Dim2;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 2
				m_MapModeSelQuestionPageDim2.Add(KeyItemTerrain) = m_AjoutSelPageActif_Dim2;  
			}

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionPageDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				JBool &Actif = m_MapModeSelQuestionPageDim3.GetItem();
				Actif = m_AjoutSelPageActif_Dim3;
			}
			else
			{
				// Nouvel item, sauve son mode de sélection en dimension 3
				m_MapModeSelQuestionPageDim3.Add(KeyItemTerrain) = m_AjoutSelPageActif_Dim3;  
			}
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére modes des sélections pour dimension 1 et dimension 2
void CDlgTableau::LoadModeSelectionDim1Dim2(JInt32 IdItem, JUnt32 NbDimension)
{
	// Selon que l'on est en mode ligne ou colonne ou page
	switch (m_Id_CurMode)
	{
		// Mode ajout lignes actif
		case CPaletteMode::Line:
		{
			// Pour le mode sélection dimension 1
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;
			m_MapModeSelQuestionLigDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim1.IsValid())
			{
				// Récup à jour item déjà sélectionné pour le mode sélection dimension 1
				m_AjoutSelLigActif_Dim1 = m_MapModeSelQuestionLigDim1.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelLigActif_Dim1 = true;

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionLigDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				m_AjoutSelLigActif_Dim2 = m_MapModeSelQuestionLigDim2.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelLigActif_Dim2 = false;

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionLigDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionLigDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				m_AjoutSelLigActif_Dim3 = m_MapModeSelQuestionLigDim2.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelLigActif_Dim3 = false;
		}
		break;

		// Mode ajout colonnes actif
	case CPaletteMode::Column:
		{
			// en remplacement
			// Pour le mode sélection dimension 1
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;

			m_MapModeSelQuestionColDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim1.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
				m_AjoutSelColActif_Dim1 = m_MapModeSelQuestionColDim1.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelColActif_Dim1 = true;

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionColDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				m_AjoutSelColActif_Dim2 = m_MapModeSelQuestionColDim2.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelColActif_Dim2 = false;

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionColDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionColDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				m_AjoutSelColActif_Dim3 = m_MapModeSelQuestionColDim3.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelColActif_Dim3 = true;
		}
		break;

		// Mode ajout pages actif
		case CPaletteMode::Page:
		{
			// Pour le mode sélection dimension 1
			CKeyItemTerrain KeyItemTerrain;
			KeyItemTerrain.m_IdItem		= IdItem;
			KeyItemTerrain.m_IdTerrain	= m_Id_Terrain;

			// Pour le mode sélection dimension 1
			m_MapModeSelQuestionPageDim1.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim1.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 1
				m_AjoutSelPageActif_Dim1 = m_MapModeSelQuestionPageDim1.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelPageActif_Dim1 = true;

			// Pour le mode sélection dimension 2
			m_MapModeSelQuestionPageDim2.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim2.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 2
				m_AjoutSelPageActif_Dim2 = m_MapModeSelQuestionPageDim2.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelPageActif_Dim2 = false;

			// Pour le mode sélection dimension 3
			m_MapModeSelQuestionPageDim3.MoveTo(KeyItemTerrain);
			if (m_MapModeSelQuestionPageDim3.IsValid())
			{
				// Mise à jour item déjà sélectionné pour le mode sélection dimension 3
				m_AjoutSelPageActif_Dim3 = m_MapModeSelQuestionPageDim3.GetItem();
			}
			else
				// Par défaut 
				m_AjoutSelPageActif_Dim3 = false;
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
// On active ou désactive les éléments en dimension 1
void CDlgTableau::OnChange_Dim_1_PaletteSel()
{
	// Change le mode ajout dimension 1, et mise à jour avec raz des composants palette
	ActiveModeAjoutDim1(true);
}

///////////////////////////////////////////////////////////////////////////
// Change mode activation de la dimension 1 pour construction palette
void CDlgTableau::ActiveModeAjoutDim1(bool RazSelection)
{
	// Changement sélection palette
	if(m_pCurMode)
	{
		// Mise à jour du mode activation sélection
		if (m_Button_Dimension1.GetCheck())
		{
			// Active le mode ajout sélection courant
			ActiveModeSel(m_Id_CurMode,1,true);
		}
		else
		{
			// Attention uniquement si les 2 dimensions apparentes
			if (m_Button_Dimension2.IsWindowVisible())
				// Désactive le mode ajout sélection courant
				ActiveModeSel(m_Id_CurMode,1,false);
			else
			{
				// Active le mode ajout sélection courant
				ActiveModeSel(m_Id_CurMode,1,true);
				m_Button_Dimension1.SetCheck(1,true);
			}
		}

		// Attention aussi à la dimension 2 (si non visible)
		// Mise à jour du mode activation sélection
		if (!m_Button_Dimension2.IsWindowVisible())
		{
			// Désactive le mode ajout sélection courant en dimension 2
			ActiveModeSel(m_Id_CurMode,2,false);
		}

		// Modifie le mode sélection dimension 1 pour le pupitre
		m_pCurMode->OnChange_Dim_1_PaletteSel();

		// Sauvegarde nouveau mode sélection dim1
		if(m_List_Questions.GetCurSel() != LB_ERR)
		{
			JInt32 IDs = m_List_Questions.GetSelectedID();
			SaveModeSelectionDim1Dim2(IDs);
		}

		// Mise à jour de la sélection tris croisés
		UpdateListSelect_Synchro(RazSelection);
	}
}

////////////////////////////////////////////////////////////////////////////
// On active ou désactive les éléments en dimension 2
void CDlgTableau::OnChange_Dim_2_PaletteSel()
{
	if(m_pCurMode)
	{	
		// Mise à jour du mode activation sélection
		if (m_Button_Dimension2.GetCheck())
		{
			// Active le mode ajout sélection courant
			ActiveModeSel(m_Id_CurMode,2,true);
		}
		else
		{
			// Désactive le mode ajout sélection courant
			ActiveModeSel(m_Id_CurMode,2,false);
		}

		// Changement mode palette en dimension 2
		m_pCurMode->OnChange_Dim_2_PaletteSel();

		// Sauvegarde nouveau mode sélection dim2
		if(m_List_Questions.GetCurSel() != LB_ERR)
		{
			JInt32 IDs = m_List_Questions.GetSelectedID();
			SaveModeSelectionDim1Dim2(IDs);
		}

		// Mise à jour de la sélection tris croisés
		UpdateListSelect_Synchro(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Modifie la sélection dans boite dimension 1
void CDlgTableau::OnSelchange_ListDimension_1() 
{
	if(m_pCurMode)
	{
		// Précédente sélection active sur Sas
		int InxPrecSelSas = m_List_SAS.GetCurSel();

		// Change la sélection dimension1
		m_pCurMode->OnSelchange_ListDimension_1(); 

		// Attention si quelque chose dans le sas et non sélectionné
		// SAS Non vide
		// Recup index question courante
		JUnt32 IndexQuCur = LB_ERR;
		
		// Attention si aucun élmt sélectionné, on revalide un elmt du sas, puis on reviendra sur la question
		if (m_List_SAS.GetCount() > 0)
		{
			// Voir si aucune sélection active sur Sas désormais
			if (m_List_SAS.GetCurSel() == LB_ERR		&& 
				m_List_Dimension1.GetSelCount() <= 0	&& 
				InxPrecSelSas !=  LB_ERR				&&
				m_List_Dimension2.GetSelCount() <= 0)
			{
				// Revient sur la question en cours
				IndexQuCur = m_List_Questions.GetCurSel();

				// Repositionne élmt du sas
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();
			}
		}
		
		// Mise à jour de la liste sélection 
		if ((m_AjoutSelLigActif_Dim1  && m_Id_CurMode == CPaletteMode::Line)   ||
			(m_AjoutSelColActif_Dim1  && m_Id_CurMode == CPaletteMode::Column) ||
			(m_AjoutSelPageActif_Dim1 && m_Id_CurMode == CPaletteMode::Page)) 
		{	
			 m_pCurMode->OnSetDimSelection1();
		}

		// Mise à jour liste sélection
		UpdateListSelect_Synchro(true);
		
		// mise à jour opérateur courant
		SetUpdateOpeLogiques();

		// On se remet sur les modalités de la question en cours si nécessaire
		if (IndexQuCur != LB_ERR)
		{
			m_List_Questions.SetCurSel(IndexQuCur);
			OnSelchange_ListQuestions();
		}

		// Mise à jour nb cellules
		// UpdateNbCells();
	}
}

// Mise à jour sélection des opérateurs sur dimension 1, 2, 3 etc...
void CDlgTableau::UpdateNbCells()
{
	JChar Buffer[500];
	long TotalCellules = 0;

	if (MajNbCells == false) return;

	if (!m_List_Row || !m_List_Col || !m_List_Page) return; 

	// Aout 2013 : Calcul nombre total de cellules et message si au delà du maximum autorisé
	long NbItemsRow = m_List_Row.GetItemCount();
	if (NbItemsRow == 0) NbItemsRow = 1;
	long NbItemsCol = m_List_Col.GetItemCount();
	if (NbItemsCol == 0) NbItemsCol = 1;
	long NbItemsPage = m_List_Page.GetItemCount();
	if (NbItemsPage == 0) NbItemsPage = 1;

	if (m_List_Row.GetItemCount() != 0 || m_List_Col.GetItemCount() != 0 || m_List_Page.GetItemCount() != 0)
		TotalCellules = NbItemsRow * NbItemsCol * NbItemsPage;

	sprintf_s(Buffer, 500, "%d ",TotalCellules);

	if (TotalCellules > 20000)
	{
		m_Label_NbCells.SetTextColor(CATPColors::GetColorRed());
		m_Label_NbCells.SetFontBold(TRUE);
	}
	else
	{
		m_Label_NbCells.SetTextColor(CATPColors::GetColorBlack());
		m_Label_NbCells.SetFontBold(FALSE);
	}

	m_Label_NbCells.SetText(Buffer);
	m_Label_NbCells.SetActiveWindow();
	m_Label_NbCells.BringWindowToTop();
	m_Label_NbCells.Invalidate(TRUE);

	/*
	m_Label_NbCells.ShowWindow(SW_SHOW);
	m_Label_NbCells.UpdateWindow();

	m_Label_NbCells.Invalidate(TRUE);
	m_Label_NbCells.RedrawWindow();
	*/
	
}

////////////////////////////////////////////////////////////////////////////
// On sélectionne ou désélectionne un élmt de la liste dimension 2
void CDlgTableau::OnSelchange_ListDimension_2() 
{
	if(m_pCurMode)
	{
		// Précédente sélection active sur Sas
		int InxPrecSelSas = m_List_SAS.GetCurSel();

		// Change la sélection dimension1
		m_pCurMode->OnSelchange_ListDimension_2(); 

		// Attention si quelque chose dans le sas et non sélectionné
		// SAS Non vide
		if (m_List_SAS.GetCount() > 0)
		{
			// Attention activer le 1er élmt Sas ssi aucun sélectionné et que il y en avait un de sélectionner avant
			if (m_List_SAS.GetCurSel() == LB_ERR		&& 
				m_List_Dimension1.GetSelCount() <= 0	&& 
				InxPrecSelSas !=  LB_ERR)
			{
				// Active le 1er élmt SAS
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();
			}
		}

		// Mise à jour de la liste sélection 
		if ((m_AjoutSelLigActif_Dim2  && m_Id_CurMode == CPaletteMode::Line)   ||
			(m_AjoutSelColActif_Dim2  && m_Id_CurMode == CPaletteMode::Column) ||
			(m_AjoutSelPageActif_Dim2 && m_Id_CurMode == CPaletteMode::Page)) 
		{	
			 m_pCurMode->OnSetDimSelection2();
		}

		UpdateListSelect_Synchro(true);

		// Mise à jour nb cellules
		// UpdateNbCells();
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Update de la liste des questions
LRESULT CDlgTableau::OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
	{
		LRESULT Result = m_pCurMode->OnUpdated_QuestionList(wParam,lParam); 

		//Mise à jour sélection
		UpdateListSelect_Synchro(true);

		 return Result;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// Changement sélection items (cadre sélection à gauche)
void CDlgTableau::OnSelchange_ListItems() 
{
	// Sauve index 1er item de la liste
	int TopIndex = m_List_Items.GetTopIndex(); 

	// Si mode en cours (items, palettes ....)
	int NbSelItem = 0;
	if(m_pCurMode)
	{
		// Change la sélection des items courants
		m_pCurMode->OnSelchange_ListItems(); 

		// Nb élmt restant sélectionnés (aussi bien en question, qu'en item ou palette)
		NbSelItem = m_List_Questions.GetCount(); 
	}
	
	// Mise à jour de la liste des items
	m_pCurMode->UpdateListItems(); 

	// Remet sur la dernière position sélectionnée
	if (TopIndex >= 0)
		m_List_Items.SetTopIndex(TopIndex); 

	// A VOIR / Mise à jour des segments
	// m_pCurMode->UpdateSegments();

	// Si aucun item, on vire tout
	if (NbSelItem <= 0)
	{
		// OnItem_Raz();
		m_pCurMode->OnItem_Raz(); 
	}

	// Si mode palette, ajustement des modes compo palette si 1er élmt
	if (m_List_Items.GetSelCount() == 1)
	{
		// Si 1er élmt item, on active le mode compo palette
		m_AjoutSelCompoPalette_Dim1 = true;
		m_AjoutSelCompoPalette_Dim2 = false;
		SetBoutonSelection();

		// Active le mode ajout sélection courant pour dimension 1
		ActiveModeAjoutDim1(false);
		// OnChange_Dim_1_PaletteSel();

		// Active le mode ajout sélection courant pour dimension 2
		ActiveModeSel(m_Id_CurMode,2,true);
		m_pCurMode->OnChange_Dim_2_PaletteSel();
	}

	// Attention, mettre à jour sélection du SAS à tout moment si non vide et rien dans dimension 1
	if (m_List_SAS.GetCount() > 0 && m_List_Dimension1.GetCount() == 0) 
	{	
		// Si aucun élmt du SAS sélectionné
		int InxCur = m_List_SAS.GetCurSel();
		if (m_List_SAS.GetCurSel() == LB_ERR)
		{
			// On sélectionne au moins le 1er
			m_List_SAS.SetCurSel(0); 
			m_pCurMode->OnSelchange_ListSas();
		}
		else
			m_pCurMode->OnSelchange_ListSas();
	}

	if (m_AjoutSelCompoPalette_Dim1)
	{
		// Mise à jour de la liste sélection ligne ou colonne ou page
		if (NbSelItem <= 0)
			UpdateListSelect_Synchro(true);
		else
			// UpdateListSelect(false);
			UpdateListSelect_Synchro(false);
	}

	// mise à jour oérateur courant
	SetUpdateOpeLogiques();
}

///////////////////////////////////////////////////////////////////////////////////
// Mise à jour des sélecteurs dimesnion 1, dimension 2, éventuellement dimension 3
void CDlgTableau::UpdateSelecteurDimension(bool Dim1Active, bool Dim2Active, bool Dim3Active)
{
	// Selon le mode courant, mise à jour des listes
	switch (m_Id_CurMode)
	{
		// Mode lignes
		case CPaletteMode::Line:
		{
			m_AjoutSelLigActif_Dim1 = Dim1Active;
			m_AjoutSelLigActif_Dim2 = Dim2Active;
			m_AjoutSelLigActif_Dim3 = Dim3Active;
			break;
		}

		// Mode colonnes
		case CPaletteMode::Column:
		{
			m_AjoutSelColActif_Dim1 = Dim1Active;
			m_AjoutSelColActif_Dim2 = Dim2Active;
			m_AjoutSelColActif_Dim3 = Dim3Active;
			break;
		}

		// Mode pages
		case CPaletteMode::Page:
		{
			m_AjoutSelPageActif_Dim1 = Dim1Active;
			m_AjoutSelPageActif_Dim2 = Dim2Active;
			m_AjoutSelPageActif_Dim3 = Dim3Active;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Activation ou Désactivation du mode sélection courante (lignes ou colonnes ou pages)
void CDlgTableau::ActiveModeSel(JUnt32 CurModeSel, JUnt32 Dimension, bool Actif)
{
	switch (CurModeSel)
	{
		// Mode ajout lignes actif
		case CPaletteMode::Line:
		{
			if (Dimension == 1)
			{
				// Update du mode ajout ligne via dimension 1
				m_AjoutSelLigActif_Dim1	= Actif;
			}
			else if (Dimension == 2)
			{
				// Update du mode ajout ligne via dimension 2
				m_AjoutSelLigActif_Dim2	= Actif;
			}
		}
		break;

		// Mode ajout colonnes actif
		case CPaletteMode::Column:
		{
			if (Dimension == 1)
			{
				// Update du mode ajout ligne via dimension 1
				m_AjoutSelColActif_Dim1	= Actif;
			}
			else if (Dimension == 2)
			{
				// Update du mode ajout ligne via dimension 2
				m_AjoutSelColActif_Dim2	= Actif;
			}
		}
		break;

		// Mode ajout pages actif
		case CPaletteMode::Page:
		{
			if (Dimension == 1)
			{
				// Update du mode ajout ligne via dimension 1
				m_AjoutSelPageActif_Dim1	= Actif;
			}
			else if (Dimension == 2)
			{
				// Update du mode ajout ligne via dimension 2
				m_AjoutSelPageActif_Dim2	= Actif;
			}
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Opération ET sur les éléments du SAS
void CDlgTableau::OnSAS_Et() 
{
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);
	if(m_pCurMode)
	{
		m_pCurMode->OnSAS_Et(); 
		
		// Affichage choix menu opérateur SAS "ET"
		AfficheChoixMenuSasEt();
	}

	// Mise à jour sélection courante si mode ajout actif
	UpdateListSelect_Synchro(true);

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSelQu(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 

	UpdateWindow();
}

// Affichage choix menu opérateur SAS pour ET
void CDlgTableau::AfficheChoixMenuSasEt()
{
	CString Txt = "";
	if (m_FIcones)
	{
		// Affichage opérateur ET en mode icone
		m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
		m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
	}
	else
	{
		// En mode texte
		m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CROISE_PALETTE,RGB(192,192,192),IDB_RI_CROISE_PALETTE,RGB(192,192,192)); 
		m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		Txt = "";
		m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
	}
}

// Affichage choix menu opérateur SAS pour OU
void CDlgTableau::AfficheChoixMenuSasOu()
{
	CString Txt = "";
	if (m_FIcones)
	{
		// Affichage opérateur OU en mode icone
		m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
		m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
	}
	else
	{
		// En mode texte
		m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
		m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		Txt = "";
		m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Opération OU sur les éléments du SAS
void CDlgTableau::OnSAS_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_OU);
	if(m_pCurMode)
	{
		m_pCurMode->OnSAS_Ou(); 

		// Affichage choix menu opérateur SAS "OU"
		AfficheChoixMenuSasOu();

		/*
		CString Txt = "";
		if (m_FIcones)
		{
			// Affichage opérateur OU en mode icone
			if (m_Id_CurMode != 4 && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
			}
			m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
		}
		else
		{
			// Affichage opérateur OU en mode texte
			if (m_Id_CurMode != 4 && GetTypeRepItem())
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(0,0);
				m_Button_SAS_OU.GetWindowText(Txt);
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
			else
			{
				m_Button_Menu_Ope_QuSel.SetBitmaps(IDB_RI_SUITE_PALETTE,RGB(192,192,192),IDB_RI_SUITE_PALETTE,RGB(192,192,192)); 
				m_Button_Menu_Ope_QuSel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
				Txt = "";
				m_Button_Menu_Ope_QuSel.SetWindowText(Txt); 
			}
		}
		*/
	}

	// Mise à jour sélection courante si mode ajout actif
	UpdateListSelect_Synchro(true);

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeSelQu(FALSE);
	
	// Rend actif l'arrière plan
	m_List_SAS.EnableWindow(TRUE); 
}

///////////////////////////////////////////////////////////////////////////////////
// Validation du libellé pour croisement items
void CDlgTableau::OnAccept() 
{
	UpdateData(true);
	if(!m_Edit_Name.GetWindowTextLength())
	{
		CString str;
		str.LoadString(IDS_RI_ITEM_LABEL);
		MessageBox(str,_T(""),MB_OK | MB_ICONEXCLAMATION);
		m_Edit_Name.SetFocus();		
		return;
	}

	if(m_pCurMode)
	{
		CString Text;
		m_Edit_Name.GetWindowText(Text);
		if(m_pCurMode->OnAccept(Text))
		{
			UpdateData(false);
			GetItems()->Save();
			OnItem_Raz();
			m_Edit_Name.SetWindowText(""); 
		}
	}

	// Mise à jour sélection courante si mode ajout actif
	UpdateListSelect_Synchro(true);
}

static int CALLBACK SortItemWrappersUp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CItemWrapper * pItem1 = (CItemWrapper *)lParam1;//pListCtrl->GetItemData(lParam1);
	CItemWrapper * pItem2 = (CItemWrapper *)lParam2;//pListCtrl->GetItemData(lParam2);

	/* 
	if(pItem1 && pItem2)
		return strcmp(pItem1->GetLabel(),pItem2->GetLabel());
	else
		return 0;
	*/

	if(pItem1 && pItem2)
	{
		CString Txt1, Txt2;
		Txt1.Format("%s %s",pItem1->GetHeader(), pItem1->GetLabel());
		Txt2.Format("%s %s",pItem2->GetHeader(), pItem2->GetLabel());
		return strcmp(Txt1,Txt2);
	}
	else
		return 0;
}

static int CALLBACK SortItemWrappersDown(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CItemWrapper * pItem1 = (CItemWrapper *)lParam1;//pListCtrl->GetItemData(lParam1);
	CItemWrapper * pItem2 = (CItemWrapper *)lParam2;//pListCtrl->GetItemData(lParam2);

	/*
	if(pItem1 && pItem2)
		return strcmp(pItem2->GetLabel(),pItem1->GetLabel());
	else
		return 0;
	*/
	if(pItem1 && pItem2)
	{
		CString Txt1, Txt2;
		Txt1.Format("%s %s",pItem1->GetHeader(), pItem1->GetLabel());
		Txt2.Format("%s %s",pItem2->GetHeader(), pItem2->GetLabel());
		return strcmp(Txt2,Txt1);
	}
	else
		return 0;
}

/*
// Sort the item in reverse alphabetical order.
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   return strcmp(strItem2, strItem1);
}
*/

//////////////////////////////////////////////////////////////
// Tri de la sélection lignes
/*
void CDlgTableau::OnBnClickedRiButTriLignes()
{
	if(m_bRowsSort) 
		m_List_Row.SortItems(SortItemWrappersDown,(LPARAM) &m_List_Row);
	else
		m_List_Row.SortItems(SortItemWrappersUp,(LPARAM) &m_List_Row);

	m_bRowsSort = !m_bRowsSort;  
}

//////////////////////////////////////////////////////////////
// Tri de la sélection colonnes
void CDlgTableau::OnBnClickedRiButTriColonnes()
{
	if(m_bColsSort) 
		m_List_Col.SortItems(SortItemWrappersDown,(LPARAM) &m_List_Col);
	else
		m_List_Col.SortItems(SortItemWrappersUp,(LPARAM) &m_List_Col);

	m_bColsSort = !m_bColsSort;  
}

//////////////////////////////////////////////////////////////
// Tri de la sélection pages
void CDlgTableau::OnBnClickedRiButTriPages()
{
	if(m_bPagesSort) 
		m_List_Page.SortItems(SortItemWrappersDown,(LPARAM) &m_List_Page);
	else
		m_List_Page.SortItems(SortItemWrappersUp,(LPARAM) &m_List_Page);

	m_bPagesSort = !m_bPagesSort;  
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////
// Tri ascendant ou descendant sur les sélections lignes, ou colonnes, ou pages
void CDlgTableau::TriSelElmt(JFCSelListCtrl &ListElmt)
{
	if (ListElmt == m_List_Row)
	{
		// Tri sur la sélection lignes
		if(m_bRowsSort) 
			ListElmt.SortItems(SortItemWrappersDown,(LPARAM) &ListElmt);
		else
			ListElmt.SortItems(SortItemWrappersUp,(LPARAM) &ListElmt);

		m_bRowsSort = !m_bRowsSort;  
	}
	else if (ListElmt == m_List_Col)
	{
		// Tri sur la sélection colonnes
		if(m_bColsSort) 
			ListElmt.SortItems(SortItemWrappersDown,(LPARAM) &ListElmt);
		else
			ListElmt.SortItems(SortItemWrappersUp,(LPARAM) &ListElmt);

		m_bColsSort = !m_bColsSort;  
	}
	else if (ListElmt == m_List_Page)
	{
		// Tri sur la sélection pages
		if(m_bPagesSort) 
			ListElmt.SortItems(SortItemWrappersDown,(LPARAM) &ListElmt);
		else
			ListElmt.SortItems(SortItemWrappersUp,(LPARAM) &ListElmt);

		m_bPagesSort = !m_bPagesSort;  
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Tout ou rien sur la dimension 1
void CDlgTableau::OnDim_1_Raz() 
{
	if(m_List_Dimension1.ToggleSelection())
	if(m_pCurMode)
			m_pCurMode->OnDim_1_Raz ();

	// Recup index question courante
	JUnt32 IndexQuCur = LB_ERR;
		
	// Mise à jour de la liste sélection 
	if ((m_AjoutSelLigActif_Dim1  && m_Id_CurMode == CPaletteMode::Line)   ||
		(m_AjoutSelColActif_Dim1  && m_Id_CurMode == CPaletteMode::Column) ||
		(m_AjoutSelPageActif_Dim1 && m_Id_CurMode == CPaletteMode::Page)) 
	{

		// Attention si quelque chose dans le sas et non sélectionné, SAS Non vide
		// Attention si aucun élmt sélectionné, on revalide un elmt du sas, puis on reviendra sur la question
		if (m_List_SAS.GetCount() > 0)
		{
			// Voir si aucune sélection active sur Sas désormais
			if (m_List_SAS.GetCurSel() == LB_ERR		&& 
				m_List_Dimension1.GetSelCount() <= 0	&& 
				m_List_Dimension2.GetSelCount() <= 0)
			{
				// Revient sur la question en cours
				IndexQuCur = m_List_Questions.GetCurSel();

				// Repositionne élmt du sas
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();
			}
		}

		m_pCurMode->OnSetDimSelection1();
		UpdateListSelect_Synchro(true);
	}

	// On se remet sur les modalités de la question en cours si nécessaire
	if (IndexQuCur != LB_ERR)
	{
		m_List_Questions.SetCurSel(IndexQuCur);
		OnSelchange_ListQuestions();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Tout ou rien sur la dimension 2
void CDlgTableau::OnDim_2_Raz() 
{
	if(m_List_Dimension2.ToggleSelection())
	if(m_pCurMode)
			m_pCurMode->OnDim_2_Raz();

	// Mise à jour de la liste sélection 
	if ((m_AjoutSelLigActif_Dim2  && m_Id_CurMode == CPaletteMode::Line)   ||
		(m_AjoutSelColActif_Dim2  && m_Id_CurMode == CPaletteMode::Column) ||
		(m_AjoutSelPageActif_Dim2 && m_Id_CurMode == CPaletteMode::Page)) 
	{
		m_pCurMode->OnSetDimSelection2();
		UpdateListSelect_Synchro(true);
	}
}

//////////////////////////////////////////////////////////////
// Mise en place des fenêtres résultats
void CDlgTableau::SetFenResultat(JInt32 NbFenVisible)
{
	// Pour les bordures
	JInt32 LargSepar = 4;

	// Hauteur des labels
	CRect RectLabel;
	m_Label_Lignes.GetWindowRect(&RectLabel); 
	JInt32 HtLabel = RectLabel.Height(); 

	if (this->GetShowSite() != SHOW_SITES)
	{
		if (NbFenVisible <= 1)
		{
			// Seul la fenêtre résultat ligne est visible
			m_List_Row.ShowWindow(SW_SHOW); 
			m_Label_Lignes.ShowWindow(SW_SHOW); 
			m_Btn_All_Lignes.ShowWindow(SW_SHOW); 

			// Si mode SQN_Profiling, bouton sites ligne visible uniquement
			int Value = this->GetShowSite();
			if (this->GetShowSite() == SHOW_SITES) 
			{
				// On visualise les boutons de sélection sites
				m_Btn_SitesEnLigne.		ShowWindow(SW_SHOW);
				m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
				m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);

				// On cache le bouton moyenne
				m_Button_Item_Moy_Lig.	EnableWindow(FALSE);
				m_Button_Item_Moy_Col.	EnableWindow(FALSE);
					
			}
			else
			{
				// On cache les boutons de sélection sites
				m_Btn_SitesEnLigne.		ShowWindow(SW_HIDE);
				m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
				m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);
			}
			
			// Fenêtres résultat colonnes et pages non visibles
			m_List_Col.					ShowWindow(SW_HIDE);  
			m_Label_Colonnes.			ShowWindow(SW_HIDE); 
			m_Button_Item_Moy_Col.		ShowWindow(SW_HIDE); 
			m_Btn_All_Colonnes.			ShowWindow(SW_HIDE); 
			m_Button_Colonne_UP.		ShowWindow(SW_HIDE);
			m_Button_Colonne_DOWN.		ShowWindow(SW_HIDE);

			m_List_Page.				ShowWindow(SW_HIDE); 
			m_Label_Pages.				ShowWindow(SW_HIDE); 
			m_Btn_All_Pages.			ShowWindow(SW_HIDE); 
			m_Button_Page_UP.			ShowWindow(SW_HIDE);
			m_Button_Page_DOWN.			ShowWindow(SW_HIDE);

			// Agrandit la fenêtre liste lignes
			CRect RectBoxGlobal;
			m_Box_Result.GetWindowRect(&RectBoxGlobal);

			CRect RectListRow;
			m_List_Row.GetWindowRect(&RectListRow);

			CRect RectListPage;
			m_List_Page.GetWindowRect(&RectListPage);

			RectListRow.bottom = RectListPage.bottom;
			this->ScreenToClient(&RectListRow);
			m_List_Row.SetWindowPos(NULL,RectListRow.left, RectListRow.top, RectListRow.Width(), RectListRow.Height() ,SWP_NOZORDER | SWP_NOMOVE);
		}
		else if (NbFenVisible == 2)
		{
			// Seul les fenêtres résultat ligne et colonne sont visibles
			m_List_Row.ShowWindow(SW_SHOW);  
			m_Label_Lignes.ShowWindow(SW_SHOW); 
			m_Btn_All_Lignes.ShowWindow(SW_SHOW); 

			// Si mode SQN_Profiling, bouton sites ligne et colonne visibles uniquement
			if (this->GetShowSite() == SHOW_SITES) 
			{
				m_Btn_SitesEnLigne.		ShowWindow(SW_SHOW);
				m_Btn_SitesEnColonne.	ShowWindow(SW_SHOW);
				m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);

				// On cache le bouton moyenne
				m_Button_Item_Moy_Lig.		EnableWindow(FALSE);
				m_Button_Item_Moy_Col.		EnableWindow(FALSE);
			}
			else
			{
				m_Btn_SitesEnLigne.		ShowWindow(SW_HIDE);
				m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
				m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);
			}

			m_List_Col.ShowWindow(SW_SHOW); 
			m_Label_Colonnes.ShowWindow(SW_SHOW); 
			m_Button_Item_Moy_Col.ShowWindow(SW_SHOW); 
			m_Btn_All_Colonnes.ShowWindow(SW_SHOW); 
			m_Button_Colonne_UP.	ShowWindow(SW_SHOW);
			m_Button_Colonne_DOWN.	ShowWindow(SW_SHOW);

			// Fenêtre résultat pages non visible
			m_List_Page.ShowWindow(SW_HIDE); 
			m_Label_Pages.ShowWindow(SW_HIDE); 
			m_Btn_All_Pages.ShowWindow(SW_HIDE); 
			m_Button_Page_UP.		ShowWindow(SW_HIDE);
			m_Button_Page_DOWN.		ShowWindow(SW_HIDE);

			// Agrandit les fenêtres liste lignes et liste colonne
			CRect RectBoxGlobal;
			m_Box_Result.GetWindowRect(&RectBoxGlobal);

			CRect RectLabelRow;
			m_Label_Lignes.GetWindowRect(&RectLabelRow); 

			CRect RectListRow;
			m_List_Row.GetWindowRect(&RectListRow);
			RectListRow.bottom = RectListRow.top + (RectBoxGlobal.Height() / 2 - HtLabel - 2 * LargSepar);

			this->ScreenToClient(&RectListRow);
			m_List_Row.SetWindowPos(NULL,RectListRow.left, RectListRow.top, RectListRow.Width(), RectListRow.Height() ,SWP_NOZORDER | SWP_NOMOVE);

			// Reprend nouvelle taille fenêtre ligne
			m_List_Row.GetWindowRect(&RectListRow);

			CRect RectLabelCol;
			m_Label_Colonnes.GetWindowRect(&RectLabelCol); 

			CRect RectListCol;
			m_List_Col.GetWindowRect(&RectListCol);

			CRect RectListPage;
			m_List_Page.GetWindowRect(&RectListPage);

			// Placement liste colonne
			RectListCol.top		= RectListRow.bottom + LargSepar + HtLabel + LargSepar;
			RectListCol.bottom	= RectListPage.bottom;
			
			this->ScreenToClient(&RectListCol);
			m_List_Col.SetWindowPos(NULL,RectListCol.left, RectListCol.top, RectListCol.Width(), RectListCol.Height() ,SWP_NOZORDER);

			// Placement label
			RectLabelCol.top	= RectListRow.bottom + LargSepar;
			RectLabelCol.bottom = RectLabelCol.top + HtLabel;

			// Placement bouton moyenne colonne
			CRect RectMoyCol;
			m_Button_Item_Moy_Col.GetWindowRect(&RectMoyCol); 
			RectMoyCol.top		= RectLabelCol.top;
			RectMoyCol.bottom	= RectLabelCol.bottom;

			// Placement bouton tri colonne
			CRect RectTriCol;
			m_Btn_All_Colonnes.GetWindowRect(&RectTriCol); 
			RectTriCol.top		= RectLabelCol.top;
			RectTriCol.bottom	= RectLabelCol.bottom;

			// Placement bouton liste sites colonne
			CRect RectBtnSiteCol;
			m_Btn_SitesEnColonne.GetWindowRect(&RectBtnSiteCol);
			RectBtnSiteCol.top		= RectLabelCol.top;
			RectBtnSiteCol.bottom	= RectLabelCol.bottom;

			// Placement boutons replacement item
			CRect RectBtnUpCol;
			CRect RectBtnDownCol;
			m_Button_Colonne_UP.GetWindowRect(&RectBtnUpCol);
			m_Button_Colonne_DOWN.GetWindowRect(&RectBtnDownCol);
			RectBtnUpCol.top		= RectLabelCol.top;
			RectBtnUpCol.bottom		= RectLabelCol.bottom;
			RectBtnDownCol.top		= RectLabelCol.top;
			RectBtnDownCol.bottom	= RectLabelCol.bottom;

			this->ScreenToClient(&RectLabelCol);
			m_Label_Colonnes.SetWindowPos(NULL,RectLabelCol.left, RectLabelCol.top, RectLabelCol.Width(), RectLabelCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnSiteCol);
			m_Btn_SitesEnColonne.SetWindowPos(NULL,RectBtnSiteCol.left, RectBtnSiteCol.top, RectBtnSiteCol.Width(), RectBtnSiteCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectMoyCol);
			m_Button_Item_Moy_Col.SetWindowPos(NULL,RectMoyCol.left, RectMoyCol.top, RectMoyCol.Width(), RectMoyCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectTriCol);
			m_Btn_All_Colonnes.SetWindowPos(NULL,RectTriCol.left, RectTriCol.top, RectTriCol.Width(), RectTriCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnUpCol);
			m_Button_Colonne_UP.SetWindowPos(NULL,RectBtnUpCol.left, RectBtnUpCol.top, RectBtnUpCol.Width(), RectBtnUpCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnDownCol);
			m_Button_Colonne_DOWN.SetWindowPos(NULL,RectBtnDownCol.left, RectBtnDownCol.top, RectBtnDownCol.Width(), RectBtnDownCol.Height() ,SWP_NOZORDER);
		}
		else
		{
			// Toutes les fenêtres résultat visibles
			m_List_Row.ShowWindow(SW_SHOW);  
			m_Label_Lignes.ShowWindow(SW_SHOW); 
			m_Btn_All_Lignes.ShowWindow(SW_SHOW); 

			// Si mode SQN_Profiling, bouton sites ligne,colonne et pages visibles uniquement
			if (this->GetShowSite() == SHOW_SITES) 
			{
				m_Btn_SitesEnLigne.		ShowWindow(SW_SHOW);
				m_Btn_SitesEnColonne.	ShowWindow(SW_SHOW);
				m_Btn_SitesEnPage.		ShowWindow(SW_SHOW);

				// On cache le bouton moyenne
				m_Button_Item_Moy_Lig.		EnableWindow(FALSE);
				m_Button_Item_Moy_Col.		EnableWindow(FALSE);
			}
			else
			{
				m_Btn_SitesEnLigne.		ShowWindow(SW_HIDE);
				m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
				m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);
			}

			m_List_Col.ShowWindow(SW_SHOW); 
			m_Label_Colonnes.ShowWindow(SW_SHOW); 
			m_Btn_All_Colonnes.ShowWindow(SW_SHOW); 
			m_Button_Colonne_UP.	ShowWindow(SW_SHOW);
			m_Button_Colonne_DOWN.	ShowWindow(SW_SHOW);

			m_List_Page.ShowWindow(SW_SHOW); 
			m_Label_Pages.ShowWindow(SW_SHOW); 
			m_Btn_All_Pages.ShowWindow(SW_SHOW); 
			m_Button_Page_UP.		ShowWindow(SW_SHOW);
			m_Button_Page_DOWN.		ShowWindow(SW_SHOW);

			// Retaille les fenêtres liste lignes et liste colonne
			CRect RectBoxGlobal;
			m_Box_Result.GetWindowRect(&RectBoxGlobal);

			CRect RectLabelRow;
			m_Label_Lignes.GetWindowRect(&RectLabelRow); 

			CRect RectListRow;
			m_List_Row.GetWindowRect(&RectListRow);
			RectListRow.bottom = RectListRow.top + (RectBoxGlobal.Height() / 3 - HtLabel - 2 * LargSepar);

			this->ScreenToClient(&RectListRow);
			m_List_Row.SetWindowPos(NULL,RectListRow.left, RectListRow.top, RectListRow.Width(), RectListRow.Height() ,SWP_NOZORDER | SWP_NOMOVE);

			// Reprend nouvelle taille fenêtre ligne
			m_List_Row.GetWindowRect(&RectListRow);

			CRect RectLabelCol;
			m_Label_Colonnes.GetWindowRect(&RectLabelCol); 

			CRect RectListCol;
			m_List_Col.GetWindowRect(&RectListCol);

			CRect RectListPage;
			m_List_Page.GetWindowRect(&RectListPage);

			// Placement liste colonne
			RectListCol.top		= RectListRow.bottom + LargSepar + HtLabel + LargSepar;
			RectListCol.bottom	= RectListCol.top + (RectBoxGlobal.Height() / 3 - HtLabel - 2 * LargSepar);
			
			this->ScreenToClient(&RectListCol);
			m_List_Col.SetWindowPos(NULL,RectListCol.left, RectListCol.top, RectListCol.Width(), RectListCol.Height() ,SWP_NOZORDER);

			// Placement label et bouton tri colonne
			RectLabelCol.top	= RectListRow.bottom + LargSepar;
			RectLabelCol.bottom = RectLabelCol.top + HtLabel;

			// Placement bouton tri colonne
			CRect RectTriCol;
			m_Btn_All_Colonnes.GetWindowRect(&RectTriCol); 
			RectTriCol.top		= RectLabelCol.top;
			RectTriCol.bottom	= RectLabelCol.bottom;

			// Placement bouton liste sites colonne
			CRect RectBtnSiteCol;
			m_Btn_SitesEnColonne.GetWindowRect(&RectBtnSiteCol);
			RectBtnSiteCol.top		= RectLabelCol.top;
			RectBtnSiteCol.bottom	= RectLabelCol.bottom;

			// Placement boutons replacement item
			CRect RectBtnUpCol;
			CRect RectBtnDownCol;
			m_Button_Colonne_UP.GetWindowRect(&RectBtnUpCol);
			m_Button_Colonne_DOWN.GetWindowRect(&RectBtnDownCol);
			RectBtnUpCol.top		= RectLabelCol.top;
			RectBtnUpCol.bottom		= RectLabelCol.bottom;
			RectBtnDownCol.top		= RectLabelCol.top;
			RectBtnDownCol.bottom	= RectLabelCol.bottom;

			this->ScreenToClient(&RectLabelCol);
			m_Label_Colonnes.SetWindowPos(NULL,RectLabelCol.left, RectLabelCol.top, RectLabelCol.Width(), RectLabelCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnSiteCol);
			m_Btn_SitesEnColonne.SetWindowPos(NULL,RectBtnSiteCol.left, RectBtnSiteCol.top, RectBtnSiteCol.Width(), RectBtnSiteCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectTriCol);
			m_Btn_All_Colonnes.SetWindowPos(NULL,RectTriCol.left, RectTriCol.top, RectTriCol.Width(), RectTriCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnUpCol);
			m_Button_Colonne_UP.SetWindowPos(NULL,RectBtnUpCol.left, RectBtnUpCol.top, RectBtnUpCol.Width(), RectBtnUpCol.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnDownCol);
			m_Button_Colonne_DOWN.SetWindowPos(NULL,RectBtnDownCol.left, RectBtnDownCol.top, RectBtnDownCol.Width(), RectBtnDownCol.Height() ,SWP_NOZORDER);

			// Placement liste pages
			m_List_Col.GetWindowRect(&RectListCol);

			CRect RectLabelPage;
			m_Label_Pages.GetWindowRect(&RectLabelPage); 
			m_List_Page.GetWindowRect(&RectListPage);

			RectListPage.top	= RectListCol.bottom + LargSepar + HtLabel + LargSepar;
			this->ScreenToClient(&RectListPage);
			m_List_Page.SetWindowPos(NULL,RectListPage.left, RectListPage.top, RectListPage.Width(), RectListPage.Height() ,SWP_NOZORDER);

			// Placement label et bouton tri pages
			RectLabelPage.top		= RectListCol.bottom + LargSepar;
			RectLabelPage.bottom	= RectLabelPage.top + HtLabel;

			CRect RectTriPage;
			m_Btn_All_Pages.GetWindowRect(&RectTriPage); 
			RectTriPage.top		= RectLabelPage.top;
			RectTriPage.bottom	= RectLabelPage.bottom;

			// Placement bouton liste sites colonne
			CRect RectBtnSitePage;
			m_Btn_SitesEnPage.GetWindowRect(&RectBtnSitePage);
			RectBtnSitePage.top		= RectLabelPage.top;
			RectBtnSitePage.bottom	= RectLabelPage.bottom;

			// Placement boutons replacement item
			CRect RectBtnUpPage;
			CRect RectBtnDownPage;
			m_Button_Page_UP.GetWindowRect(&RectBtnUpPage);
			m_Button_Page_DOWN.GetWindowRect(&RectBtnDownPage);
			RectBtnUpPage.top		= RectLabelPage.top;
			RectBtnUpPage.bottom	= RectLabelPage.bottom;
			RectBtnDownPage.top		= RectLabelPage.top;
			RectBtnDownPage.bottom	= RectLabelPage.bottom;

			this->ScreenToClient(&RectLabelPage);
			m_Label_Pages.SetWindowPos(NULL,RectLabelPage.left, RectLabelPage.top, RectLabelPage.Width(), RectLabelPage.Height() ,SWP_NOZORDER);		

			this->ScreenToClient(&RectBtnSitePage);
			m_Btn_SitesEnPage.SetWindowPos(NULL,RectBtnSitePage.left, RectBtnSitePage.top, RectBtnSitePage.Width(), RectBtnSitePage.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnUpPage);
			m_Button_Page_UP.SetWindowPos(NULL,RectBtnUpPage.left, RectBtnUpPage.top, RectBtnUpPage.Width(), RectBtnUpPage.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectBtnDownPage);
			m_Button_Page_DOWN.SetWindowPos(NULL,RectBtnDownPage.left, RectBtnDownPage.top, RectBtnDownPage.Width(), RectBtnDownPage.Height() ,SWP_NOZORDER);

			this->ScreenToClient(&RectTriPage);
			m_Btn_All_Pages.SetWindowPos(NULL,RectTriPage.left, RectTriPage.top, RectTriPage.Width(), RectTriPage.Height() ,SWP_NOZORDER);
		}
	}
	else
	{
		// Affichage particulier pour mode SQN_Profiling
		// Toutes les fenêtres résultat visibles
		m_List_Row.ShowWindow(SW_SHOW);  
		m_Label_Lignes.ShowWindow(SW_SHOW); 
		m_Btn_All_Lignes.ShowWindow(SW_SHOW); 

		// Si mode SQN_Profiling, bouton sites ligne,colonne et pages visibles uniquement
		if (this->GetShowSite() == SHOW_SITES) 
		{
			m_Btn_SitesEnLigne.		ShowWindow(SW_SHOW);
			m_Btn_SitesEnColonne.	ShowWindow(SW_SHOW);
			m_Btn_SitesEnPage.		ShowWindow(SW_SHOW);

			// On cache le bouton moyenne
			m_Button_Item_Moy_Lig.		EnableWindow(FALSE);
			m_Button_Item_Moy_Col.		EnableWindow(FALSE);
		}
		else
		{
			m_Btn_SitesEnLigne.		ShowWindow(SW_HIDE);
			m_Btn_SitesEnColonne.	ShowWindow(SW_HIDE);
			m_Btn_SitesEnPage.		ShowWindow(SW_HIDE);
		}

		m_List_Col.ShowWindow(SW_SHOW); 
		m_Label_Colonnes.ShowWindow(SW_SHOW); 
		m_Button_Item_Moy_Col.ShowWindow(SW_SHOW); 
		m_Btn_All_Colonnes.ShowWindow(SW_SHOW); 
		m_Button_Colonne_UP.	ShowWindow(SW_SHOW);
		m_Button_Colonne_DOWN.	ShowWindow(SW_SHOW);

		m_List_Page.ShowWindow(SW_SHOW); 
		m_Label_Pages.ShowWindow(SW_SHOW); 
		m_Btn_All_Pages.ShowWindow(SW_SHOW); 
		m_Button_Page_UP.		ShowWindow(SW_SHOW);
		m_Button_Page_DOWN.		ShowWindow(SW_SHOW);

		// Retaille les fenêtres liste lignes et liste colonne
		CRect RectBoxGlobal;
		m_Box_Result.GetWindowRect(&RectBoxGlobal);

		CRect RectLabelRow;
		m_Label_Lignes.GetWindowRect(&RectLabelRow); 

		CRect RectListRow;
		m_List_Row.GetWindowRect(&RectListRow);
		RectListRow.bottom = RectListRow.top + (RectBoxGlobal.Height() * 2 / 5 - HtLabel - 2 * LargSepar);

		this->ScreenToClient(&RectListRow);
		m_List_Row.SetWindowPos(NULL,RectListRow.left, RectListRow.top, RectListRow.Width(), RectListRow.Height() ,SWP_NOZORDER | SWP_NOMOVE);

		// Reprend nouvelle taille fenêtre ligne
		m_List_Row.GetWindowRect(&RectListRow);

		CRect RectLabelCol;
		m_Label_Colonnes.GetWindowRect(&RectLabelCol); 

		CRect RectListCol;
		m_List_Col.GetWindowRect(&RectListCol);

		CRect RectListPage;
		m_List_Page.GetWindowRect(&RectListPage);

		// Placement liste colonne
		RectListCol.top		= RectListRow.bottom + LargSepar + HtLabel + LargSepar;
		RectListCol.bottom	= RectListCol.top + (RectBoxGlobal.Height() * 2 / 5 - HtLabel - 2 * LargSepar + 3);
		
		this->ScreenToClient(&RectListCol);
		m_List_Col.SetWindowPos(NULL,RectListCol.left, RectListCol.top, RectListCol.Width(), RectListCol.Height() ,SWP_NOZORDER);

		// Placement label et bouton tri colonne
		RectLabelCol.top	= RectListRow.bottom + LargSepar;
		RectLabelCol.bottom = RectLabelCol.top + HtLabel;

		// Placement bouton tri colonne
		CRect RectTriCol;
		m_Btn_All_Colonnes.GetWindowRect(&RectTriCol); 
		RectTriCol.top		= RectLabelCol.top;
		RectTriCol.bottom	= RectLabelCol.bottom;

		// Placement bouton liste sites colonne
		CRect RectBtnSiteCol;
		m_Btn_SitesEnColonne.GetWindowRect(&RectBtnSiteCol);
		RectBtnSiteCol.top		= RectLabelCol.top;
		RectBtnSiteCol.bottom	= RectLabelCol.bottom;

		// Placement bouton moyenne colonne
		CRect RectMoyCol;
		m_Button_Item_Moy_Col.GetWindowRect(&RectMoyCol); 
		RectMoyCol.top		= RectLabelCol.top;
		RectMoyCol.bottom	= RectLabelCol.bottom;

		// Placement boutons replacement item
		CRect RectBtnUpCol;
		CRect RectBtnDownCol;
		m_Button_Colonne_UP.GetWindowRect(&RectBtnUpCol);
		m_Button_Colonne_DOWN.GetWindowRect(&RectBtnDownCol);
		RectBtnUpCol.top		= RectLabelCol.top;
		RectBtnUpCol.bottom		= RectLabelCol.bottom;
		RectBtnDownCol.top		= RectLabelCol.top;
		RectBtnDownCol.bottom	= RectLabelCol.bottom;

		this->ScreenToClient(&RectLabelCol);
		m_Label_Colonnes.SetWindowPos(NULL,RectLabelCol.left, RectLabelCol.top, RectLabelCol.Width(), RectLabelCol.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectBtnSiteCol);
		m_Btn_SitesEnColonne.SetWindowPos(NULL,RectBtnSiteCol.left, RectBtnSiteCol.top, RectBtnSiteCol.Width(), RectBtnSiteCol.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectMoyCol);
		m_Button_Item_Moy_Col.SetWindowPos(NULL,RectMoyCol.left, RectMoyCol.top, RectMoyCol.Width(), RectMoyCol.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectTriCol);
		m_Btn_All_Colonnes.SetWindowPos(NULL,RectTriCol.left, RectTriCol.top, RectTriCol.Width(), RectTriCol.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectBtnUpCol);
		m_Button_Colonne_UP.SetWindowPos(NULL,RectBtnUpCol.left, RectBtnUpCol.top, RectBtnUpCol.Width(), RectBtnUpCol.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectBtnDownCol);
		m_Button_Colonne_DOWN.SetWindowPos(NULL,RectBtnDownCol.left, RectBtnDownCol.top, RectBtnDownCol.Width(), RectBtnDownCol.Height() ,SWP_NOZORDER);

		// Placement liste pages
		m_List_Col.GetWindowRect(&RectListCol);

		CRect RectLabelPage;
		m_Label_Pages.GetWindowRect(&RectLabelPage); 
		m_List_Page.GetWindowRect(&RectListPage);

		RectListPage.top	= RectListCol.bottom + LargSepar + HtLabel + LargSepar;
		this->ScreenToClient(&RectListPage);
		m_List_Page.SetWindowPos(NULL,RectListPage.left, RectListPage.top, RectListPage.Width(), RectListPage.Height() ,SWP_NOZORDER);

		// Placement label et bouton tri pages
		RectLabelPage.top		= RectListCol.bottom + LargSepar;
		RectLabelPage.bottom	= RectLabelPage.top + HtLabel;

		CRect RectTriPage;
		m_Btn_All_Pages.GetWindowRect(&RectTriPage); 
		RectTriPage.top		= RectLabelPage.top;
		RectTriPage.bottom	= RectLabelPage.bottom;

		// Placement bouton liste sites colonne
		CRect RectBtnSitePage;
		m_Btn_SitesEnPage.GetWindowRect(&RectBtnSitePage);
		RectBtnSitePage.top		= RectLabelPage.top;
		RectBtnSitePage.bottom	= RectLabelPage.bottom;

		// Placement boutons replacement item
		CRect RectBtnUpPage;
		CRect RectBtnDownPage;
		m_Button_Page_UP.GetWindowRect(&RectBtnUpPage);
		m_Button_Page_DOWN.GetWindowRect(&RectBtnDownPage);
		RectBtnUpPage.top		= RectLabelPage.top;
		RectBtnUpPage.bottom	= RectLabelPage.bottom;
		RectBtnDownPage.top		= RectLabelPage.top;
		RectBtnDownPage.bottom	= RectLabelPage.bottom;

		this->ScreenToClient(&RectLabelPage);
		m_Label_Pages.SetWindowPos(NULL,RectLabelPage.left, RectLabelPage.top, RectLabelPage.Width(), RectLabelPage.Height() ,SWP_NOZORDER);		

		this->ScreenToClient(&RectBtnSitePage);
		m_Btn_SitesEnPage.SetWindowPos(NULL,RectBtnSitePage.left, RectBtnSitePage.top, RectBtnSitePage.Width(), RectBtnSitePage.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectBtnUpPage);
		m_Button_Page_UP.SetWindowPos(NULL,RectBtnUpPage.left, RectBtnUpPage.top, RectBtnUpPage.Width(), RectBtnUpPage.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectBtnDownPage);
		m_Button_Page_DOWN.SetWindowPos(NULL,RectBtnDownPage.left, RectBtnDownPage.top, RectBtnDownPage.Width(), RectBtnDownPage.Height() ,SWP_NOZORDER);

		this->ScreenToClient(&RectTriPage);
		m_Btn_All_Pages.SetWindowPos(NULL,RectTriPage.left, RectTriPage.top, RectTriPage.Width(), RectTriPage.Height() ,SWP_NOZORDER);
	}
}

LRESULT CDlgTableau::OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
	{
		LRESULT Result = m_pCurMode->OnSelChange_ListDimension_x(wParam,lParam); 

		if (m_List_SAS.GetCount() > 0)
		{
			int InxCurSel = m_List_SAS.GetCurSel();

			if (m_List_SAS.GetCurSel() == LB_ERR && m_List_Dimension1.GetCount() <= 0)
			{
				m_List_SAS.SetCurSel(0);
				m_pCurMode->OnSelchange_ListSas();
			}
		}

		// Mise à jour de la liste sélection
		UpdateListSelect_Synchro(true);
		return Result;
	}
	return 0;
}

LRESULT CDlgTableau::OnChange_Dim_ValueSel(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		m_pCurMode->OnChange_Value_PaletteSel();

	// Mise à jour sélection
	UpdateListSelect_Synchro(true);

	return 0;
}

LRESULT CDlgTableau::OnSelChange_ListValue(WPARAM wParam, LPARAM lParam)
{
	if(m_pCurMode)
		m_pCurMode->OnSelchange_ListValue();

	// Mise à jour sélection
	UpdateListSelect_Synchro(true);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//Renvoi état sélection dimension 1 ou 2
bool CDlgTableau::GetSelDimPupitreActive(JUnt32 Dim)
{
	// Dimension 1
	if (Dim == 1)
	{
		// en mode ligne
		if (m_Id_CurMode		== CPaletteMode::Line)
			return  m_AjoutSelLigActif_Dim1;
		// En mode colonne
		else if (m_Id_CurMode	== CPaletteMode::Column)
			return  m_AjoutSelColActif_Dim1;
		// En mode page
		else if (m_Id_CurMode	== CPaletteMode::Page)
			return  m_AjoutSelPageActif_Dim1;
	}
	// Dimension 2
	else if (Dim == 2)
	{
		// en mode ligne
		if (m_Id_CurMode		== CPaletteMode::Line)
			return  m_AjoutSelLigActif_Dim2;
		// En mode colonne
		else if (m_Id_CurMode	== CPaletteMode::Column)
			return  m_AjoutSelColActif_Dim2;
		// En mode page
		else if (m_Id_CurMode	== CPaletteMode::Page)
			return  m_AjoutSelPageActif_Dim2;
	}
		
	return false;
}

// Visibilité Boite Items Validation
void CDlgTableau::SetVisibleBoiteValidItem(bool Visible)
{
	if (Visible)
	{
		// Puis on décache la boite Box_Items
		m_Box_Items.			ShowWindow(SW_SHOW);

		// Et ses composants (idem que pour rep item, sauf impression toujours cachée)
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);
		m_But_Export_Item.		ShowWindow(SW_HIDE);

		m_Button_Accept.		ShowWindow(SW_SHOW);

		// m_Button_Suppression.	ShowWindow(SW_SHOW);

		m_Edit_Name.			ShowWindow(SW_SHOW);

		m_Label_TypePoids.		ShowWindow(SW_SHOW);

		m_Button_Segment_RAZ.ShowWindow(SW_HIDE);
	}
	else
	{
		// Puis on cache la boite Box_Items
		m_Box_Items.			ShowWindow(SW_HIDE);

		// Et ses composants
		m_But_Down_Item.		ShowWindow(SW_HIDE);
		m_But_Up_Item.			ShowWindow(SW_HIDE);
		m_But_Imprimer_Result.	ShowWindow(SW_HIDE);
		m_But_Export_Item.		ShowWindow(SW_HIDE);

		m_Button_Accept.		ShowWindow(SW_HIDE);
		// m_Button_Suppression.	ShowWindow(SW_HIDE);
		m_Edit_Name.			ShowWindow(SW_HIDE);

		m_Label_TypePoids.		ShowWindow(SW_HIDE);

		m_Button_Segment_RAZ.ShowWindow(SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Sauvegarde des palettes ligne, colonne, page
bool CDlgTableau::SavePalettes(CStdioFile & file)
{
	// Sauvegarde dossier palettes
	JMFCStream* pstream = NULL;
	try
	{
		// Creer en mode ouverture le fichier de sauvegarde des palettes
		// CStdioFile file(PathName, CFile::modeWrite);
		CArchive ar(&file,CArchive::store);
		pstream = JMFCStream::Create(ar);
		
		// Sérialisation palette ligne si élmts sélectionnés
		if (m_pCurModeLigne)
			// Sauve les élmts palette ligne
			m_pCurModeLigne->SendPalette(*pstream, CPaletteMode::Line);
		else
			// Aucun élmt palette ligne
			pstream->Send((JInt32)0); 

		// Sérialisation palette colonne si élmts sélectionnés
		if (m_pCurModeColonne)
			// Sauve les élmts palette colonne
			m_pCurModeColonne->SendPalette(*pstream, CPaletteMode::Column);
		else
			// Aucun élmt palette colonne
			pstream->Send((JInt32)0); 

		// Sérialisation palette page si élmts sélectionnés
		if (m_pCurModePage)
			// Sauve les élmts palette page
			m_pCurModePage->SendPalette(*pstream, CPaletteMode::Page);
		else
			// Aucun élmt palette pge
			pstream->Send((JInt32)0); 

		// Injecte dans fichier
		pstream->Flush();

		return true;
	}
	catch(...)
	{
		return false;
	}
	if (pstream)
		delete pstream;
}

////////////////////////////////////////////////////////////////////////////////
// Restitution des palettes
bool CDlgTableau::LoadPalettes(CStdioFile & file)
{
	// Sauvegarde dossier palettes
	JMFCStream* pstream = NULL;
	try
	{
		// Recup fichier en mode lecture
		// CStdioFile file(PathName, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pstream = JMFCStream::Create(ar);
		
		// Reinit tableau stockage des modes pupitre
		if (m_Modes.GetCount() == 0)
		{
			m_Modes.Reset();
			m_Modes.Add(CPaletteMode::Line)			= PTR_ABSTRACTMODE(new CLineMode(this));  
			m_Modes.Add(CPaletteMode::Column)		= PTR_ABSTRACTMODE(new CColumnMode(this));  
			m_Modes.Add(CPaletteMode::Page)			= PTR_ABSTRACTMODE(new CPageMode(this));  
		}

		// Récupération palette ligne
		m_Modes.MoveTo(CPaletteMode::Line);
		m_pCurModeLigne		= m_Modes.GetItem().get(); 
		m_pCurModeLigne->RecvPalette(*pstream, CPaletteMode::Line);

		// Récupère segment et Buider de la palette ligne, ainsi que le Psuedo Item
		m_pCurModeLigne->SetSegmentIBuilder(); 
		// m_pCurModeLigne->SetPseudoItem();
		
		// Récupération palette colonne
		m_Modes.MoveTo(CPaletteMode::Column);
		m_pCurModeColonne		= m_Modes.GetItem().get(); 
		m_pCurModeColonne->RecvPalette(*pstream, CPaletteMode::Column);

		// Récupère segment et Buider de la palette colonne, ainsi que le Psuedo Item
		m_pCurModeColonne->SetSegmentIBuilder(); 
		// m_pCurModeColonne->SetPseudoItem();

		// Récupération palette page
		m_Modes.MoveTo(CPaletteMode::Page);
		m_pCurModePage		= m_Modes.GetItem().get(); 
		m_pCurModePage->RecvPalette(*pstream, CPaletteMode::Page);
		
		// Récupère segment et Buider de la palette page, ainsi que le PseudoItem
		m_pCurModePage->SetSegmentIBuilder(); 
		// m_pCurModePage->SetPseudoItem();

		// Init Mode par défaut (mode ligne)
		m_pCurMode = m_pCurModeLigne;
		
		return true;
	}
	catch(...)
	{
		return false;
	}
	if (pstream)
		delete pstream;
}

///////////////////////////////////////////////////////////////////////////////////////
// Transposition des palettes
void CDlgTableau::Transpose(int Mode1, int Mode2)
{
	// Normalement ce n'est plus nécessaire !!!! A VOIR !!!!
	return;
		
	// Test validité de la transposition
	if (Mode1 != Mode2 && Mode1 >= CPaletteMode::Line && Mode1 <= CPaletteMode::Page && Mode2 >= CPaletteMode::Line && Mode2 <= CPaletteMode::Page)
	{
		// A FAIRE avec pMemento
		JMemento *pMemento1	= JMemento::Create();
		JMemento *pMemento2	= JMemento::Create();


		if (Mode1 == CPaletteMode::Line  && m_pCurModeLigne &&
			Mode2 == CPaletteMode::Column && m_pCurModeColonne)
		{
			// Sauve les modes en tempo
			m_pCurModeLigne->SaveMemento(pMemento1,CPaletteMode::Line); 
			m_pCurModeColonne->SaveMemento(pMemento2,CPaletteMode::Column); 

			// Récupère info ligne et reconstruit segment builder colonne
			m_pCurModeLigne->LoadMemento(pMemento2,CPaletteMode::Line);
			m_pCurModeLigne->SetSegmentIBuilder(); 

			// Récupère info colonne et reconstruit segment builder colonne
			m_pCurModeColonne->LoadMemento(pMemento1,CPaletteMode::Column);
			m_pCurModeColonne->SetSegmentIBuilder(); 
		}

		else if (Mode1 == CPaletteMode::Line  && m_pCurModeLigne && Mode2 == CPaletteMode::Column && !m_pCurModeColonne)
		{
			// Ici on a seulement des lignes, on les transposent en colonnes
			// Sauve les sélections lignes en tempo
			m_pCurModeLigne->SaveMemento(pMemento1,CPaletteMode::Line); 

			// Récupère le pointeur mode colonne
			CAbstractMode * pMode;
			m_Modes.MoveTo(CPaletteMode::Column);  
			if (m_Modes.IsValid())
			{
				pMode = m_Modes.GetItem().get();
				m_pCurModeColonne = pMode;

				// Sauve rien pour pointeur colonne
				m_pCurModeColonne->SaveMemento(pMemento2,CPaletteMode::Column); 

				// Récupère info colonne et reconstruit segment builder colonne
				m_pCurModeColonne->LoadMemento(pMemento1,CPaletteMode::Column);
				m_pCurModeColonne->SetSegmentIBuilder(); 
			}
					
			// Récupère info ligne et reconstruit segment builder colonne
			m_pCurModeLigne->LoadMemento(pMemento2,CPaletteMode::Line);
			m_pCurModeLigne->SetSegmentIBuilder(); 

		}
		else if (Mode1 == CPaletteMode::Line  && !m_pCurModeLigne && Mode2 == CPaletteMode::Column && m_pCurModeColonne)
		{
			// Ici on a seulement des colonnes, on les transpose en lignes
			// Sauve les sélections colonnes en tempo
			m_pCurModeColonne->SaveMemento(pMemento2,CPaletteMode::Column); 

			// Récupère le pointeur mode ligne
			CAbstractMode * pMode;
			m_Modes.MoveTo(CPaletteMode::Line);  
			if (m_Modes.IsValid())
			{
				pMode = m_Modes.GetItem().get();
				m_pCurModeLigne = pMode;

				// Sauve rien pour pointeur ligne
				m_pCurModeLigne->SaveMemento(pMemento1,CPaletteMode::Line); 

				// Récupère info ligne et reconstruit segment builder colonne
				m_pCurModeLigne->LoadMemento(pMemento2,CPaletteMode::Line);
				m_pCurModeLigne->SetSegmentIBuilder(); 
			}

			// Récupère info colonne et reconstruit segment builder colonne
			m_pCurModeColonne->LoadMemento(pMemento1,CPaletteMode::Column);
			m_pCurModeColonne->SetSegmentIBuilder(); 
		}

		// Détruit les mémoires mode temporaires
		delete pMemento1;
		delete pMemento2;	

	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Sélection de la totalité des questions
void CDlgTableau::OnQuestions_Tous()
{
	if(m_pCurMode)
	{
		m_pCurMode->OnQuestions_Tous(); 
		UpdateListSelect_Synchro(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection lignes
void CDlgTableau::OnBnClickedRiButLignesDown()
{
	// Décalage bloc items vers le bas
    DecalItemDown(m_List_Row);	
}

// Tri ascendant sélection lignes
void CDlgTableau::OnBnClickedRiButLignesUp()
{
	// Décalage bloc items vers le haut
    DecalItemUp(m_List_Row);	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri ascendant sélection colonnes
void CDlgTableau::OnBnClickedRiButColonnesDown()
{
	// Décalage bloc items vers le bas
    DecalItemDown(m_List_Col);	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection colonnes
void CDlgTableau::OnBnClickedRiButColonnesUp()
{
	// Décalage bloc items vers le haut
    DecalItemUp(m_List_Col);	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri ascendant sélection lignes
void CDlgTableau::OnBnClickedRiButPagesDown()
{
	// Décalage bloc items vers le bas
    DecalItemDown(m_List_Page);	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tri descendant sélection pages
void CDlgTableau::OnBnClickedRiButPagesUp()
{
	// Décalage bloc items vers le haut
    DecalItemUp(m_List_Page);	
}

//////////////////////////////////////////////////////////////////////////////
// Décalage bloc items vers le bas sur les listes sélections
void CDlgTableau::DecalItemDown(JFCListCtrl &Liste)
{
	if (Liste.GetSelectedCount() == 1)
	{
		// Index de l'élément sélectionné
		POSITION  Pos = Liste.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Voir si ce n'est pas le dernier élmt
		if (SelIndex != Liste.GetItemCount()-1)
		{
			// Permutation contenu item en page
			for (int i= 0; i < Liste.GetItemCount(); i++)
			{
				if (i == SelIndex)
				{
					// Prend le texte courant
					CString TxtCur = Liste.GetItemText(i,0);

					// Prend le texte suivant
					CString TxtSuiv = Liste.GetItemText(i+1,0);
		
					// Item Data Courant
					DWORD_PTR  DataCur  = Liste.GetItemData(i); 
					
					// Item Data Suivant
					DWORD_PTR  DataSuiv = Liste.GetItemData(i+1); 
					
					// Permute les datas
					Liste.SetItemData(i+1,DataCur);
					Liste.SetItemData(i,DataSuiv);

					// Permut les textes
					Liste.SetItemText(i,0,TxtSuiv);
					Liste.SetItemText(i+1,0,TxtCur);

					// Mise à jour Sélection sur item suivant
					Liste.SetItemState(i+1,LVIS_SELECTED, LVIS_SELECTED);
					Liste.SetItemState(i,0,LVIS_SELECTED);

					Liste.SetSelectionMark(i+1);

					// Redessine la partie liste modifiée
					Liste.RedrawItems(i,i+1);

					// Stop
					break;
				}
			}
		}
	}
	else if (Liste.GetSelectedCount() > 1)
	{
		// Plusieurs items sélectionnés / Index du 1er et dernier elmt sélectionné
		POSITION  Pos = Liste.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Nombre de sélections
		JUnt32 NbSel = Liste.GetSelectedCount(); 

		// Index elmt à remonter (= juste apres le bloc sélection)
		int IndexSuivBloc = SelIndex + NbSel;

		if (IndexSuivBloc < Liste.GetItemCount())
		{
			// Récup Texte de cet item
			CString TxtSuiv = Liste.GetItemText(IndexSuivBloc,0);

			// Recup Info Item 
			DWORD_PTR	DataSuiv = Liste.GetItemData(IndexSuivBloc); 

			// Puis on décale tout le bloc sélection d'un cran vers le bas de la liste
			for (int i = SelIndex + NbSel - 1; i >= SelIndex; i--)
			{
				// Prend le texte courant
				CString TxtCur = Liste.GetItemText(i,0);

				// Item Data Courant
				DWORD_PTR  DataCur  = Liste.GetItemData(i); 

				// Puis on le décale vers la ligne suivante
				Liste.SetItemData(i + 1,DataCur);
				Liste.SetItemText(i+1,0,TxtCur);

				// Mise à jour Sélection sur item suivant
				Liste.SetItemState(i + 1,LVIS_SELECTED, LVIS_SELECTED);
				Liste.SetItemState(i,0,LVIS_SELECTED);
				Liste.SetSelectionMark(i + 1);
			}

			// Remet en tête de bloc, l'item qui était derriere le bloc
			Liste.SetItemData(SelIndex,DataSuiv);
			Liste.SetItemText(SelIndex,0,TxtSuiv);
			Liste.SetItemState(SelIndex,0,LVIS_SELECTED);

			// Redessine la liste
			Liste.RedrawItems(0,Liste.GetItemCount()-1);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Décalage bloc items vers le haut sur les listes sélections
void CDlgTableau::DecalItemUp(JFCListCtrl &Liste)
{
	if (Liste.GetSelectedCount() == 1)
	{
		// Index de l'élément sélectionné
		POSITION  Pos = Liste.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Voir si ce n'est pas le 1er élmt
		if (SelIndex != 0)
		{
			// Permutation contenu item en page
			for (int i= 0; i < Liste.GetItemCount(); i++)
			{
				if (i == SelIndex)
				{
					// Prend le texte courant
					CString TxtCur = Liste.GetItemText(i,0);

					// Prend le texte précédent
					CString TxtPrec = Liste.GetItemText(i-1,0);
					
					// Item Data Courant
					DWORD_PTR  DataCur  = Liste.GetItemData(i); 
					
					// Item Data Precedent
					DWORD_PTR  DataPrec = Liste.GetItemData(i-1); 
					
					// Permute les datas
					Liste.SetItemData(i-1,DataCur);
					Liste.SetItemData(i,DataPrec);

					// Permut les textes
					Liste.SetItemText(i-1,0,TxtCur);
					Liste.SetItemText(i,0,TxtPrec);

					// Mise à jour Sélection sur item précédent
					Liste.SetItemState(i-1,LVIS_SELECTED, LVIS_SELECTED);
					Liste.SetItemState(i,0,LVIS_SELECTED);

					Liste.SetSelectionMark(i-1);

					// Redessine la partie liste modifiée
					Liste.RedrawItems(i-1,i);

					// Stop
					break;
				}
			}
		}
	}
	else if (Liste.GetSelectedCount() > 1)
	{
		// Plusieurs items sélectionnés / Index du 1er et dernier elmt sélectionné
		POSITION  Pos = Liste.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Nombre de sélections
		JUnt32 NbSel = Liste.GetSelectedCount(); 

		// Index elmt à redescendre (= juste apres le bloc sélection)
		int IndexPrecBloc = SelIndex - 1;

		if (IndexPrecBloc >= 0)
		{
			// Récup Texte de cet item
			CString TxtPrec = Liste.GetItemText(IndexPrecBloc,0);

			// Recup Info Item 
			DWORD_PTR	DataPrec = Liste.GetItemData(IndexPrecBloc); 

			// Puis on décale tout le bloc sélection d'un cran vers le bas de la liste
			for (int i = SelIndex; i < SelIndex + Liste.GetSelectedCount() ; i++)
			{
				// Prend le texte courant
				CString TxtCur = Liste.GetItemText(i,0);

				// Item Data Courant
				DWORD_PTR  DataCur  = Liste.GetItemData(i); 

				// Puis on le décale vers la ligne précédente
				Liste.SetItemData(i - 1,DataCur);
				Liste.SetItemText(i - 1,0,TxtCur);

				// Mise à jour Sélection sur item précédent
				Liste.SetItemState(i - 1,LVIS_SELECTED, LVIS_SELECTED);
				Liste.SetItemState(i,0,LVIS_SELECTED);
				Liste.SetSelectionMark(i - 1);
			}

			// Remet en fin de bloc, l'item qui était devant le bloc
			Liste.SetItemData(SelIndex + Liste.GetSelectedCount() - 1,DataPrec);
			Liste.SetItemText(SelIndex + Liste.GetSelectedCount() - 1,0,TxtPrec);
			Liste.SetItemState(SelIndex + Liste.GetSelectedCount() - 1 ,0,LVIS_SELECTED);

			// Redessine la liste
			Liste.RedrawItems(0,Liste.GetItemCount()-1);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// Gestion KeyDown des lignes sélectionnées
void CDlgTableau::OnLvnKeydownRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		// Suppression des élmts sélection lignes
		SuppressionSelection(m_List_Row);
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Gestion KeyDown des colonnes sélectionnées
void CDlgTableau::OnLvnKeydownRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		// Suppression des élmts sélections colonnes
		SuppressionSelection(m_List_Col);
	}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Gestion KeyDown des pages sélectionnées
void CDlgTableau::OnLvnKeydownRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		// Suppression des élmts sélection pages
		SuppressionSelection(m_List_Page);
	}

	*pResult = 0;
}

// Mise à jour validite bouton recalage en bas, en haut
void CDlgTableau::SetValidUpDownListe(JFCListCtrl &Liste)
{
	if (MajNbCells == false) return;

	// Flag actif par défaut
	JBool BtnActif = true;

	if (Liste.GetSelectedCount() > 0 && Liste)
	{
		// La liste a au moins 1 sélection, et 2 elmts (btn decalage actif, si consécutifs)
		POSITION pos		= Liste.GetFirstSelectedItemPosition();
		POSITION LastPos = pos;
		while(pos)
		{
			// Récup pos sélectionnée suivante
			Liste.GetNextSelectedItem(pos);

			if (pos)
			{
				// Si non consécutif, on arrête
				if (pos != LastPos + 1)
				{
					BtnActif = false;
					break;
				}
				else
					LastPos = pos;
			}
		}
	}
	else
	{
		// La liste a aucune sélection ou 1 seul elmt (btn decalage non actif)
		BtnActif = false;
	}

	// Puis on valide ou dévalide les 2 boutons up & down de la liste concernée
	if (Liste == m_List_Row)
	{
		// Concernant le sélecteur d'items à moyenner
		if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
		{
			m_Button_Item_Moy_Lig.EnableWindow(true);
			m_Button_Item_Moy_Col.EnableWindow(false);
		}
		else
		{
			m_Button_Item_Moy_Lig.EnableWindow(false);
			m_Button_Item_Moy_Col.EnableWindow(true);
		}

		// Concernant les boutons up & down de la liste ligne
		m_Button_Ligne_UP.EnableWindow(BtnActif); 
		m_Button_Ligne_DOWN.EnableWindow(BtnActif); 
	}
	else if (Liste == m_List_Col)
	{
		// Concernant le sélecteur d'items à moyenner
		// Concernant le sélecteur d'items à moyenner
		if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
		{
			m_Button_Item_Moy_Lig.EnableWindow(false);
			m_Button_Item_Moy_Col.EnableWindow(true);
		}
		else
		{
			m_Button_Item_Moy_Lig.EnableWindow(true);
			m_Button_Item_Moy_Col.EnableWindow(false);
		}

		// Concernant les boutons up & down de la liste colonne
		m_Button_Colonne_UP.EnableWindow(BtnActif); 
		m_Button_Colonne_DOWN.EnableWindow(BtnActif);
	}
	else 
	{
		// Concernant les boutons up & down de la liste page
		m_Button_Page_UP.EnableWindow(BtnActif); 
		m_Button_Page_DOWN.EnableWindow(BtnActif);
	}

	// Mise à jour nb cellules
	UpdateNbCells();
}

/////////////////////////////////////////////////////////////////////////////////
// Suppression des élmts sélections (en ligne, colonne ou page)
void CDlgTableau::SuppressionSelection(JFCListCtrl &Liste)
{
	// Suppression des élmts sélections colonnes
	if (Liste.GetSelectedCount() > 0)
	{
		// Positionner sur 1er elmt sélectionné
		POSITION pos		= Liste.GetFirstSelectedItemPosition();
		JUnt32 NbSuppElmt	= 0;
		while(pos)
		{
			// Récup index correspondant
			int nItem  = Liste.GetNextSelectedItem(pos);

			// Supprime l'émt sélection courant
			Liste.DeleteItem(nItem); 

			// Pour recalage nouveau 1er sélectionné
			pos		= Liste.GetFirstSelectedItemPosition();
		}

		// Mise à jour validite bouton recalage en bas, en haut
		SetValidUpDownListe(Liste);

		// Update mode Lignes/Colonnes/Pages
		UpdateModeSites();
	}
}

/////////////////////////////////////////////////////////////////////////////////
void CDlgTableau::UpdateModeSites()
{
	if (m_DType_Row == DTYPE_SITES)
	{
		// Sites en lignes ????
		JBool SitesEnLigne = false;

		// Nombre d'items ligne sélectionnés
		int NbItems = m_List_Row.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Row.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			if (pItemConstruit->IsItemSite())
			{
				SitesEnLigne = true;	
			}
		}
		if (!SitesEnLigne)
			m_DType_Row = DTYPE_ITEMS;
	}
	else if(m_DType_Col == DTYPE_SITES)
	{
		// Sites en colonnes ????
		JBool SitesEnCol = false;

		// Nombre d'items colonnes sélectionnés
		int NbItems = m_List_Col.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Col.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			if (pItemConstruit->IsItemSite())
			{
				SitesEnCol = true;	
			}
		}
		if (!SitesEnCol)
			m_DType_Col = DTYPE_ITEMS;
	}
	else if(m_DType_Col == DTYPE_SITES)
	{
		// Sites en pages ????
		JBool SitesEnPage = false;

		// Nombre d'items page sélectionnés
		int NbItems = m_List_Page.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Page.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			if (pItemConstruit->IsItemSite())
			{
				SitesEnPage = true;	
			}
		}
		if (!SitesEnPage)
			m_DType_Page = DTYPE_ITEMS;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Suppression des élmts en doublon (en ligne, colonne ou page)
void CDlgTableau::SuppressionDoublon(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon)
{
	// Flag indiquant au moins 1 elmt à supprimer
	JBool DoublonExistant = false;

	JMap <CString, JUnt32> MapDoublon;
	for (JUnt32 nItem = 0; nItem < Liste.GetItemCount(); nItem++)
	{
		CString TxtItem = Liste.GetItemText(nItem, 0);

		MapDoublon.MoveTo(TxtItem);
		if (!MapDoublon.IsValid())
		{
			// 1ere apparition élmt liste
			MapDoublon.Add(TxtItem) = 1;
		}	
		else
		{
			// Elmt répétitif, on incrémente le compteur nb fois
			JUnt32 &Cpt = MapDoublon.GetItem();
			Cpt++;

			// Update Info doublon
			DoublonExistant = true;
		}
	}

	if (!DoublonExistant && MessageSiPasDeDoublon == true)
	{
		// Aucun doublon dans cette liste
		CString TxtMess;
		TxtMess.LoadString(IDS_AN_MESS_AUCUN_ITEM_IDENT); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
	}
	else if (DoublonExistant)
	{
		// Demande si on veut bien supprimer les doublons
		if (AfxMessageBox("Attention, il y aura des doublons suite à cette sélection. Voulez-vous supprimer ces doublons ?",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			// Balayage pour éliminer les doublons
			for (JInt32 nItem = Liste.GetItemCount()-1; nItem >=0; nItem--)
			{
				// Texte trouvé
				CString TxtItem = Liste.GetItemText(nItem, 0);

				// On se place sur l'info compteur
				MapDoublon.MoveTo(TxtItem);

				// Si info texte trouvé
				if (MapDoublon.IsValid())
				{
					// Récupère nb fois texte dans liste
					JUnt32 &Cpt = MapDoublon.GetItem();

					// Décrémente d'office le 1er trouvé
					Cpt--;

					if (Cpt > 0)
					{
						// Vire l'élmt doublon
						Liste.DeleteItem(nItem);
					}
				}
			}
		}
	}

	// Mise à jour validite bouton recalage en bas, en haut
	SetValidUpDownListe(Liste);

	// Remise à jour des compteurs nb sélections
	UpdateCount();
}

// Suppression des élmts en doublon (en ligne, colonne ou page) via les repères absolus
void CDlgTableau::SuppressionDoublonRepAbsolu(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon)
{
	// Flag indiquant au moins 1 elmt à supprimer
	JBool DoublonExistant = false;

	JMap <CString, JUnt32> MapDoublon;
	for (JUnt32 nItem = 0; nItem < Liste.GetItemCount(); nItem++)
	{
		CItemWrapper *pItemWrapper = (CItemWrapper *)Liste.GetItemData(nItem);

		JStringEx StrRepAbsolu = pItemWrapper->PtrItem()->GetABString();

		CString TxtItem = StrRepAbsolu.AsJCharPtr();

		MapDoublon.MoveTo(TxtItem);
		if (!MapDoublon.IsValid())
		{
			// 1ere apparition élmt liste
			MapDoublon.Add(TxtItem) = 1;
		}	
		else
		{
			// Elmt répétitif, on incrémente le compteur nb fois
			JUnt32 &Cpt = MapDoublon.GetItem();
			Cpt++;

			// Update Info doublon
			DoublonExistant = true;
		}
	}

	if (!DoublonExistant && MessageSiPasDeDoublon == true)
	{
		// Aucun doublon dans cette liste
		CString TxtMess;
		TxtMess.LoadString(IDS_AN_MESS_AUCUN_ITEM_IDENT); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION);
	}
	else if (DoublonExistant)
	{
		// Demande si on veut bien supprimer les doublons
		/* On n'affiche plus le message
		if (AfxMessageBox("Attention, il y aura des doublons suite à cette sélection. Voulez-vous supprimer ces doublons ?",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
		*/
		// Balayage pour éliminer les doublons
		for (JInt32 nItem = Liste.GetItemCount()-1; nItem >=0; nItem--)
		{
			CItemWrapper *pItemWrapper = (CItemWrapper *)Liste.GetItemData(nItem);

			JStringEx StrRepAbsolu = pItemWrapper->PtrItem()->GetABString();

			CString TxtItem = StrRepAbsolu.AsJCharPtr();

			// On se place sur l'info compteur
			MapDoublon.MoveTo(TxtItem);

			// Si info texte trouvé
			if (MapDoublon.IsValid())
			{
				// Récupère nb fois texte dans liste
				JUnt32 &Cpt = MapDoublon.GetItem();

				// Décrémente d'office le 1er trouvé
				Cpt--;

				if (Cpt > 0)
				{
					// Vire l'élmt doublon
					Liste.DeleteItem(nItem);
				}
			}
		}
		// }
	}

	// Mise à jour validite bouton recalage en bas, en haut
	SetValidUpDownListe(Liste);

	// Remise à jour des compteurs nb sélections
	UpdateCount();



	/*
	JMap <CString, JUnt32> MapDoublon;
	for (JUnt32 nItem = 0; nItem < Liste.GetItemCount(); nItem++)
	{
		CItemWrapper *pItemWrapper = (CItemWrapper *)m_List_Row.GetItemData(nItem);

		JStringEx StrRepAbsolu = pItemWrapper->PtrItem()->GetABString();

		CString Txt = StrRepAbsolu.AsJCharPtr();

		DoublonExistant = true;
	}
	*/

	
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste lignes
void CDlgTableau::OnLvnBegindragRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	return;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_nDragIndex = pNMLV->iItem;

	POINT pt;
	int noffset = 10;
	pt.x = noffset;
	pt.y = noffset;


	// m_pDragImage = m_List_Row.CreateDragImage(m_nDragIndex,&pt);
	if (m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = 0;
	}

	if(m_List_Row.GetSelectedCount() > 1)
	{
		// Plusieurs items sélectionnés
		m_pDragImage = new CImageList();
		m_pDragImage->Create(IDB_AN_MULTISEL,163,0,RGB(255,255,255));
	}
	else
		// 1 seul item sélectionné
		m_pDragImage = m_List_Row.CreateDragImage(m_nDragIndex,&pt);

	ASSERT(m_pDragImage);

	m_pDragImage->BeginDrag(0,CPoint(noffset,noffset));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	// Set dragging mode
	m_bDragging = true;
	m_nDropIndex = -1;
	m_pDragWnd = &m_List_Row; 
	m_pDropWnd = &m_List_Row; 

	SetCapture(); 

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste colonnes
void CDlgTableau::OnLvnBegindragRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nDragIndex = pNMLV->iItem;

	POINT pt;
	int noffset = 10;
	pt.x = noffset;
	pt.y = noffset;

	if(m_List_Col.GetSelectedCount() > 1)
	{
		// Plusieurs items sélectionnés
		m_pDragImage = new CImageList();
		m_pDragImage->Create(IDB_AN_MULTISEL,163,0,RGB(255,255,255));
	}
	else
		// 1 seul item sélectionné
		m_pDragImage = m_List_Col.CreateDragImage(m_nDragIndex,&pt);


	ASSERT(m_pDragImage);

	m_pDragImage->BeginDrag(0,CPoint(noffset,noffset));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	// Set dragging mode
	m_bDragging = true;
	m_nDropIndex = -1;
	m_pDragWnd = &m_List_Col; 
	m_pDropWnd = &m_List_Col; 

	SetCapture(); 
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Debut DragDrop sur liste page
void CDlgTableau::OnLvnBegindragRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nDragIndex = pNMLV->iItem;

	POINT pt;
	int noffset = 10;
	pt.x = noffset;
	pt.y = noffset;
	
	if(m_List_Page.GetSelectedCount() > 1)
	{
		// Plusieurs items sélectionnés
		m_pDragImage = new CImageList();
		m_pDragImage->Create(IDB_AN_MULTISEL,163,0,RGB(255,255,255));
	}
	else
		// 1 seul item sélectionné
		m_pDragImage = m_List_Page.CreateDragImage(m_nDragIndex,&pt);

	ASSERT(m_pDragImage);

	m_pDragImage->BeginDrag(0,CPoint(noffset,noffset));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	// Set dragging mode
	m_bDragging = true;
	m_nDropIndex = -1;
	m_pDragWnd = &m_List_Page; 
	m_pDropWnd = &m_List_Page; 

	SetCapture(); 
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Fin execution DragDrop
void CDlgTableau::OnLButtonUp(UINT nFlags, CPoint point)
{
	return;

	if (m_bDragging)
	{
		// Release mouse capture, so that other controls 
		// can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();

		delete m_pDragImage; 
		m_pDragImage = 0; 

		//must delete it because it was created at the beginning of the drag
		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
		{
			CListCtrl * pDropList = (CListCtrl*)pDropWnd;
			CListCtrl * pDragList = (CListCtrl*)m_pDragWnd;
			//Set pointer to the list we are dropping on
			DropItemOnList(pDragList, pDropList); 
			//Call routine to perform the actual drop
		}
		else
		{
			CListCtrl * pDropList = 0;
			CListCtrl * pDragList = (CListCtrl*)m_pDragWnd;
			//Set pointer to the list we are dropping on
			DropItemOnList(pDragList, pDropList); 
			//Call routine to perform the actual drop
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////////
// Copie Items sélection en DragDrop sur la liste cible
void CDlgTableau::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	//This routine performs the actual drop of the item dragged.
	//It simply grabs the info from the Drag list (pDragList)
	// and puts that info into the list dropped on (pDropList).
	//Send:    pDragList = pointer to CListCtrl we dragged from,
	//        pDropList = pointer to CListCtrl we are dropping on.
	//Return: nothing.

	return;

	////Variables
	LVITEM lviT;
	LVITEM* plvitem;

	// Unhilight the drop target
	if(pDropList)
		pDropList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);

	//Set up the LV_ITEM for retrieving item from pDragList and adding the
	//new item to the pDropList
	ZeroMemory(&lviT, sizeof (LVITEM)); //allocate and clear memory space  

	// for LV_ITEM
	lviT.iItem			= m_nDragIndex;
	lviT.mask			= LVIF_PARAM;
	lviT.pszText        = 0;
	lviT.cchTextMax		= 0;
	lviT.lParam			= 0;

	if(pDragList->GetSelectedCount() == 1)
	{
		// Get item that was dragged
		pDragList->GetItem (&lviT);
		// Delete the original item (for Move operation)
		pDragList->DeleteItem (m_nDragIndex);
		if(pDragList == pDropList)
		{
			//decrement drop index to account for item
			//being deleted above it
			if(m_nDragIndex < m_nDropIndex) --m_nDropIndex; 
		}

		// Insert item into pDropList
		// if m_nDropIndex == -1, iItem = GetItemCount() 
		//   (inserts at end of list), else iItem = m_nDropIndex
		if(pDropList)
		{
			lviT.iItem = (m_nDropIndex == -1) ? pDropList->GetItemCount () : m_nDropIndex;
			CItemWrapper * pItem = (CItemWrapper *)lviT.lParam;
			CString Str;
			Str.Format("%s - %s",pItem->GetHeader(),pItem->GetLabel()); 
			pDropList->InsertItem(LVIF_PARAM|LVIF_TEXT,lviT.iItem,Str,0,0,0,(LPARAM)pItem);
			// Select the new item we just inserted
			pDropList->SetItemState (lviT.iItem, LVIS_SELECTED,LVIS_SELECTED);
		}
		else
		{
			m_Items_Del.AddTail() = (CItemWrapper *)lviT.lParam;
		}
	}
	else //more than 1 item is being dropped
	{
		//We have to parse through all of the selected items from the
		// DragList
		//1) Retrieve the info for the items and store them in memory
		//2) If we are reordering, delete the items from the list
		//3) Insert the items into the list (either same list or
		//   different list)

		CList<LVITEM*, LVITEM*> listItems;
		POSITION listPos;

		//Retrieve the selected items
		POSITION pos = pDragList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
		while(pos) //so long as we have a valid POSITION, we keep
		// iterating
		{
			plvitem = new LVITEM;
			ZeroMemory(plvitem, sizeof(LVITEM));

			m_nDragIndex = pDragList->GetNextSelectedItem(pos);
			//Get the item
			plvitem->iItem		= m_nDragIndex; //set the index in
			plvitem->mask		= LVIF_PARAM;
			//the drag list to the selected item
			pDragList->GetItem(plvitem); //retrieve the
			//Save the pointer to the new item in our CList
			listItems.AddTail(plvitem);
		} //EO while(pos) -- at this point we have deleted the moving
		// items and stored them in memory

		//Delete the selected items
		pos = pDragList->GetFirstSelectedItemPosition();
		while(pos)
		{
			pos = pDragList->GetFirstSelectedItemPosition();
			m_nDragIndex = pDragList->GetNextSelectedItem(pos);

			pDragList->DeleteItem(m_nDragIndex); 
			if(pDragList == pDropList) //we are reordering the list (moving)
			{
				//since we are MOVING, delete the item
				if(m_nDragIndex < m_nDropIndex) --m_nDropIndex; 
			}
			//must decrement the drop index to account
			//for the deleted items
		} //EO while(pos)


		//Iterate through the items stored in memory and add them 
		//back into the CListCtrl at the drop index
		listPos = listItems.GetHeadPosition();
		while(listPos)
		{
			plvitem = listItems.GetNext(listPos);

			if(pDropList)
			{
				m_nDropIndex = (m_nDropIndex == -1) ? pDropList->GetItemCount() : m_nDropIndex;
				plvitem->iItem = m_nDropIndex;
				CItemWrapper * pItem = (CItemWrapper *)plvitem->lParam;
				CString Str;
				Str.Format("%s - %s",pItem->GetHeader(),pItem->GetLabel()); 
				pDropList->InsertItem(LVIF_PARAM|LVIF_TEXT,plvitem->iItem,Str,0,0,0,(LPARAM)pItem);
				//highlight/select the item we just added
				pDropList->SetItemState(plvitem->iItem,LVIS_SELECTED, LVIS_SELECTED); 
			}
			else
			{
				m_Items_Del.AddTail() = (CItemWrapper *)plvitem->lParam;
			}
			//increment the index we are dropping at to keep the 
			//dropped items in the same order they were in in the 
			//Drag List. If we dont' increment this, the items are
			//added in reverse order
			++m_nDropIndex; 
			//Lastly, we need to clean up by deleting our "new"
			//variables
			delete plvitem;

		} //EO while(listPos)

	}

	// Remise à jour des compteurs nb sélections
	UpdateCount();

	// Retaille les listes sélections
	ResizeColListSelect(CPaletteMode::Line);
	ResizeColListSelect(CPaletteMode::Column);
	ResizeColListSelect(CPaletteMode::Page);
}

////////////////////////////////////////////////////////////////////////////////
// Copie Items sélection sur la liste cible
void CDlgTableau::CopierItemOnList(JFCListCtrl *pListSrc, JFCListCtrl *pListDst, JBool Remove)
{
	// Variables pointeur élmt item
	LVITEM* plvitem;

	// Liste des élmts sélectionnés
	CList<LVITEM*, LVITEM*> listItems;
	POSITION listPos;

	// Tout d'abord on vérifie aucun site dans la sélection, sinon on n'autorise pas le couper/coller ou copier
	if (SelectAvecSite(pListSrc))
	{
		// Action impossible avec des sites
		this->MessageBox("Déplacement ou duplication impossible avec items sites",AfxGetAppName(), MB_ICONEXCLAMATION); 
		return;
	}

	//Retrieve the selected items
	POSITION pos = pListSrc->GetFirstSelectedItemPosition(); 

	// Index sur elmt liste source
	int IndexSrc;

	// Boucle récupération des élmts sélectionnés en Source
	while(pos) 
	{
		plvitem = new LVITEM;
		ZeroMemory(plvitem, sizeof(LVITEM));

		// Passe à la position sélection suivante
		IndexSrc = pListSrc->GetNextSelectedItem(pos);

		// Récup info item
		plvitem->iItem		= IndexSrc; 
		plvitem->mask		= LVIF_PARAM;
		pListSrc->GetItem(plvitem); 

		//Save the pointer to the new item in our CList
		listItems.AddTail(plvitem);
	} 
		
	// Si couper/coller Suppression des elmts sélectionnés dans la liste source
	if (Remove)
	{
		// Position 1er elmt
		pos = pListSrc->GetFirstSelectedItemPosition();

		// Boucle sur tous les elmts
		while(pos)
		{
			pos = pListSrc->GetFirstSelectedItemPosition();
			IndexSrc = pListSrc->GetNextSelectedItem(pos);
			pListSrc->DeleteItem(IndexSrc); 
		} 
	}

	//Iterate through the items stored in memory and add them 
	//back into the CListCtrl at the drop index
	listPos = listItems.GetHeadPosition();

	int IndexDst;

	while(listPos)
	{
		plvitem = listItems.GetNext(listPos);

		if(pListDst)
		{
			// Toujours à la fin
			IndexDst = pListDst->GetItemCount();
			plvitem->iItem = IndexDst;

			// Récupération texte item à afficher
			CItemWrapper * pItem = (CItemWrapper *)plvitem->lParam;
			CString Str;
			Str.Format("%s - %s",pItem->GetHeader(),pItem->GetLabel()); 

			// Attention en mode dupli, il faut reconstruire un elmt, et recopier les infos
			if (!Remove)
			{
				// Créer un nouvel élmt CItemWrapper
				CItemWrapper * pItemDupli = new CItemWrapper();

				// Récup le texte header élmt
				pItemDupli->SetHeader(pItem->GetHeader());

				// Récup Item Construit Source
				const CItemConstruit *pItemConstruit = pItem->PtrItem(); 

				// Créer un nouvei Item Construit et duplique l'item construit origine
				CItemConstruit * pItemConstruitDupli = GetItems()->GetNewItemConstruit(pItemConstruit);
				pItemDupli->SetItemConstruit(pItemConstruitDupli); 
				
				// Insére nouvel item créé dans la liste propriéraire
				pListDst->InsertItem(LVIF_PARAM|LVIF_TEXT,plvitem->iItem,Str,0,0,0,(LPARAM)pItemDupli);

				// Laisse état sélection active
				pListDst->SetItemState(plvitem->iItem,LVIS_SELECTED, LVIS_SELECTED); 
			}
			else
			{
				// Mode copie, uniquement changement de liste propriètaire
				pListDst->InsertItem(LVIF_PARAM|LVIF_TEXT,plvitem->iItem,Str,0,0,0,(LPARAM)pItem);

				// Laisse état sélection active
				pListDst->SetItemState(plvitem->iItem,LVIS_SELECTED, LVIS_SELECTED); 
			}
		}
		else
		{
			m_Items_Del.AddTail() = (CItemWrapper *)plvitem->lParam;
		}
		delete plvitem;
	}

	// Remise à jour des compteurs nb sélections
	UpdateCount();

	// Retaille les listes sélections
	ResizeColListSelect(CPaletteMode::Line);
	ResizeColListSelect(CPaletteMode::Column);
	ResizeColListSelect(CPaletteMode::Page);

	// Mise à jour validite bouton recalage en bas, en haut
	SetValidUpDownListe(*pListSrc);
	SetValidUpDownListe(*pListDst);
}

// Test si dans sélection active, sites présents
bool CDlgTableau::SelectAvecSite(JFCListCtrl *pListSrc)
{
	// Nombre d'items ligne sélectionnés
	int NbItems = pListSrc->GetItemCount();

	// Boucle sur tous les items
	for(int x = 0; x < NbItems; x++)
	{
		// Récup élmt item liste
		DWORD_PTR  data = pListSrc->GetItemData(x) ;
		CItemWrapper *pItemWrapper =(CItemWrapper *)data;

		// Vérifie que c'est un site
		const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

		// Et en + s'il est sélectionné
		if (pItemConstruit->IsItemSite() && pListSrc->GetItemState(x,TVIS_SELECTED))
		{
			return true;
		}
	}

	// Sinon pas des sites
	return false;
}

/////////////////////////////////////////////////////////////
// Gestion déplacement souris dur la fenêtre de dialogue
void CDlgTableau::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	// Test tsi au moins 1 opérateur actif
	if (OpeActif())
	{
		// Rend invisible tous les menus opérateurs
		SetVisibleOpeDim(1,false);
		SetVisibleOpeDim(2,false);
		SetVisibleOpeSelQu(false);
		SetVisibleOpeSegment(false);

		// La liste Sas redevient accessible
		m_List_SAS.EnableWindow(TRUE); 

		// La lise Dimension 1 et dimension 2 aussi
		m_List_Dimension1.EnableWindow(TRUE); 
		m_List_Dimension2.EnableWindow(TRUE);

		// Les élmts segments aussi
		m_Tab_Segment.EnableWindow(TRUE); 
	}

	if(m_bDragging)
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		m_pDragImage->DragMove(pt);
		m_pDragImage->DragShowNolock(false);

		CWnd * pDropWnd = WindowFromPoint(pt);
		ASSERT(pDropWnd);

		if (pDropWnd != m_pDropWnd)
		{
			//If we drag over the CListCtrl header, turn off the
			// hover highlight
			if (m_nDropIndex != -1) 
			{
				//TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0,LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex,m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				//TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; ++i)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;
		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{            
			UINT uFlags;
			CListCtrl* pList = (CListCtrl*)pDropWnd;

			// Turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);

			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			// Highlight it
			pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED,LVIS_DROPHILITED);
			// Redraw item
			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}

	CDialog::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////
// 
void CDlgTableau::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// Selon la liste sur laquelle on clicke
	switch(pWnd->GetDlgCtrlID())
	{
		// Menu contextuel sur liste des items à distribuer en lignes ou colonnes ou pages
		case IDC_RI_LIST_DIMENSION1:
		case IDC_RI_LIST_DIMENSION2:
		{
			CMenu Menu;
			CString Label;

			//Création Menu
			Menu.CreatePopupMenu();

			// Option ajout des items en ligne en cours de sélection
			Label.LoadString(IDS_PU_AJOUT_LIGNES);
			Menu.AppendMenu(MF_STRING,IDM_PU_AJOUT_LIGNES,Label);

			// Option ajout des items en colonne en cours de sélection
			Label.LoadString(IDS_PU_AJOUT_COLONNES);
			Menu.AppendMenu(MF_STRING,IDM_PU_AJOUT_COLONNES,Label);

			// Option ajout des items en pages en cours de sélection
			Label.LoadString(IDS_PU_AJOUT_PAGES);
			Menu.AppendMenu(MF_STRING,IDM_PU_AJOUT_PAGES,Label);

			// Traite le click sur menu popup
			switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,this))
			{
				// Ajout en lignes
				case IDM_PU_AJOUT_LIGNES:
				{
					OnBnClickedRiRadModeLignes();
				}
				break;

				// Ajout en colonnes
				case IDM_PU_AJOUT_COLONNES:
				{
					OnBnClickedRiRadModeColonnes();
				}
				break;

				// Ajout en pages
				case IDM_PU_AJOUT_PAGES:
				{
					OnBnClickedRiRadModePages();
				}
				break;
			}
						
			// Mise à jour fenêtre après traitement option menu popup
			pWnd->InvalidateRect(NULL, TRUE);
			pWnd->UpdateWindow();
		}
		break;

		// Menu contextuel sur la liste sélection lignes
		case IDC_RI_LIST_LIGNES:
		{
			if (m_List_Row.GetSelectedCount() > 0)
			{
				CMenu Menu;
				CString Label;

				//Création Menu
				Menu.CreatePopupMenu();

				// Options Suppression Lignes Sélectionnées
				Label.LoadString(IDS_PU_SUPP_SEL_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPPRESSION,Label);

				// Options Supprimer tous les items lignes
				Label.LoadString(IDS_PU_SUPP_ALL_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_ALL_LIGNES,Label);

				// Option Virer les lignes répétitives
				Label.LoadString(IDS_PU_SUPP_DOUBLON);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_DOUBLON,Label);
				
				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Option Couper/Coller vers sélection colonnes
				Label.LoadString(IDS_PU_CC_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_COLONNES,Label);

				// Option Couper/Coller vers sélection pages
				Label.LoadString(IDS_PU_CC_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_PAGES,Label);

				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Options de transfert colonnes
				Label.LoadString(IDS_PU_TR_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_COLONNES,Label);
				
				// Options de transfert pages
				Label.LoadString(IDS_PU_TR_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_PAGES,Label);
				
				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Tri sélection lignes
				Label.LoadString(IDS_PU_TRI_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TRI_LIGNES,Label);
				
				switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,this))
				{
					// Suppression des élmts sélection lignes
					case IDM_PU_SUPPRESSION:
					{
						SuppressionSelection(m_List_Row);
					}
					break;

					case IDM_PU_SUPP_ALL_LIGNES:
					{
						// Suppression de toutes les lignes
						OnSupprimertoutesleslignes();
					}
					break;
					
					// Suppression des élmts en doublon
					case IDM_PU_SUPP_DOUBLON:
					{
						SuppressionDoublon(m_List_Row, true);
					}
					break;

					// Copie sélection ligne vers colonnes
					case IDM_PU_TR_COLONNES:
					{
						CopierItemOnList(&m_List_Row, &m_List_Col);
					}
					break;
					
					// Copie sélection ligne vers pages
					case IDM_PU_TR_PAGES:
					{
						CopierItemOnList(&m_List_Row, &m_List_Page);
					}
					break;

					// Couper/Coller sélection ligne vers colonnes
					case IDM_PU_CC_COLONNES:
					{
						CopierItemOnList(&m_List_Row, &m_List_Col, true);
					}
					break;

					// Couper/Coller sélection ligne vers pages
					case IDM_PU_CC_PAGES:
					{
						CopierItemOnList(&m_List_Row, &m_List_Page, true);
					}
					break;

					// Tri sélection lignes
					case IDM_PU_TRI_LIGNES:
					{
						TriSelElmt(m_List_Row);
					}
					break;
				}
			
				// Mise à jour fenêtre après traitement option menu popup
				pWnd->InvalidateRect(NULL, TRUE);
				pWnd->UpdateWindow();
			}
		}
		break;

		// Menu contextuel sur la liste sélection colonnes
		case IDC_RI_LIST_COLONNES:
		{
			if (m_List_Col.GetSelectedCount() > 0)
			{
				CMenu Menu;
				CString Label;

				//Création Menu
				Menu.CreatePopupMenu();

				// Options Suppression Colonnes sélectionnées
				Label.LoadString(IDS_PU_SUPP_SEL_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPPRESSION,Label);

				// Options Supprimer tous les items colonnes
				Label.LoadString(IDS_PU_SUPP_ALL_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_ALL_COLONNES,Label);
				
				// Option Virer les lignes répétitives
				Label.LoadString(IDS_PU_SUPP_DOUBLON);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_DOUBLON,Label);
				
				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Option Couper/Coller vers sélection lignes
				Label.LoadString(IDS_PU_CC_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_LIGNES,Label);

				// Option Couper/Coller vers sélection pages
				Label.LoadString(IDS_PU_CC_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_PAGES,Label);

				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);
				
				// Options de transfert lignes
				Label.LoadString(IDS_PU_TR_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_LIGNES,Label);
				
				// Options de transfert pages
				Label.LoadString(IDS_PU_TR_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_PAGES,Label);

				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Tri sélection colonnes
				Label.LoadString(IDS_PU_TRI_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TRI_COLONNES,Label);

				switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,this))
				{
					// Suppression des élmts sélection colonnes
					case IDM_PU_SUPPRESSION:
					{
						SuppressionSelection(m_List_Col);
					}
					break;

					case IDM_PU_SUPP_ALL_COLONNES:
					{
						// Suppression de toutes les lignes
						OnSupprimertouteslescolonnes();
					}
					break;

					// Suppression des élmts en doublon
					case IDM_PU_SUPP_DOUBLON:
					{
						SuppressionDoublon(m_List_Col, true);
					}
					break;
					
					// Copie sélection colonne vers lignes
					case IDM_PU_TR_LIGNES:
					{
						CopierItemOnList(&m_List_Col, &m_List_Row);
					}
					break;
				
					// Copie sélection colonne vers pages
					case IDM_PU_TR_PAGES:
					{
						CopierItemOnList(&m_List_Col, &m_List_Page);
					}
					break;

					// Couper/Coller sélection colonne vers lignes
					case IDM_PU_CC_LIGNES:
					{
						CopierItemOnList(&m_List_Col, &m_List_Row, true);
					}
					break;

					// Couper/Coller sélection colonne vers pages
					case IDM_PU_CC_PAGES:
					{
						CopierItemOnList(&m_List_Col, &m_List_Page, true);
					}
					break;

					// Tri sélection colonnes
					case IDM_PU_TRI_COLONNES:
					{
						TriSelElmt(m_List_Col);
					}
					break;
				}
			
				// Mise à jour fenêtre après traitement option menu popup
				pWnd->InvalidateRect(NULL, TRUE);
				pWnd->UpdateWindow();
			}
		}
		break;

		// Menu contextuel sur la liste sélection pages
		case IDC_RI_LIST_PAGES:
		{
			if (m_List_Page.GetSelectedCount() > 0)
			{
				CMenu Menu;
				CString Label;

				//Création Menu
				Menu.CreatePopupMenu();

				// Options Suppression Sélection Pages
				Label.LoadString(IDS_PU_SUPP_SEL_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPPRESSION,Label);

				// Options Supprimer tous les items pages
				Label.LoadString(IDS_PU_SUPP_ALL_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_ALL_PAGES,Label);

				// Option Virer les lignes répétitives
				Label.LoadString(IDS_PU_SUPP_DOUBLON);
				Menu.AppendMenu(MF_STRING,IDM_PU_SUPP_DOUBLON,Label);
				
				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Option Couper/Coller vers sélection lignes
				Label.LoadString(IDS_PU_CC_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_LIGNES,Label);

				// Option Couper/Coller vers sélection colonnes
				Label.LoadString(IDS_PU_CC_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_CC_COLONNES,Label);

				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);
			
				// Options de transfert lignes
				Label.LoadString(IDS_PU_TR_LIGNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_LIGNES,Label);
				
				// Options de transfert pages
				Label.LoadString(IDS_PU_TR_COLONNES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TR_COLONNES,Label);
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Séparateur
				Menu.AppendMenu(MF_SEPARATOR, 0);

				// Tri sélection pages
				Label.LoadString(IDS_PU_TRI_PAGES);
				Menu.AppendMenu(MF_STRING,IDM_PU_TRI_PAGES,Label);

				switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,this))
				{
					// Suppression des élmts sélection pages
					case IDM_PU_SUPPRESSION:
					{
						SuppressionSelection(m_List_Page);
					}
					break;

					case IDM_PU_SUPP_ALL_PAGES:
					{
						// Suppression de toutes les pages
						OnSupprimertouteslespages();
					}
					break;

					// Suppression des élmts en doublon
					case IDM_PU_SUPP_DOUBLON:
					{
						SuppressionDoublon(m_List_Page, true);
					}
					break;
					
					// Copie sélection page vers lignes
					case IDM_PU_TR_LIGNES:
					{
						CopierItemOnList(&m_List_Page, &m_List_Row);
					}
					break;
				
					// Copie sélection page vers colonnes
					case IDM_PU_TR_COLONNES:
					{
						CopierItemOnList(&m_List_Page, &m_List_Col);
					}
					break;

					// Couper/Coller sélection page vers lignes
					case IDM_PU_CC_LIGNES:
					{
						CopierItemOnList(&m_List_Page, &m_List_Row, true);
					}
					break;

					// Couper/Coller séletion page vers colonnes
					case IDM_PU_CC_COLONNES:
					{
						CopierItemOnList(&m_List_Page, &m_List_Col, true);
					}
					break;

					// Tri sélection pages
					case IDM_PU_TRI_PAGES:
					{
						TriSelElmt(m_List_Page);
					}
					break;
				}

				// Mise à jour fenêtre après traitement option menu popup
				pWnd->InvalidateRect(NULL, TRUE);
				pWnd->UpdateWindow();
			}
		}
		break;
		
		// Par défaut / rien
		default: 
			break;

	}

	// Mise à jour infos sélections	
	UpdateCount();

	// Réaffecte les fenêtres visibles
	SetListeResultat();
}

////////////////////////////////////////////////////////////////////////////////////////
// Retaille fenetre dialog pupitre
void CDlgTableau::OnSize(UINT nType, int cx, int cy) 
{
	// Resize des composants
	CDialog::OnSize(nType, cx, cy);

	m_Resizer.Move();

	// Réaffecte les fenêtres visibles
	if (m_List_Row && m_List_Col && m_List_Page)
		SetListeResultat();
	
	// Positionne la fenêtre des valeurs
	PosFenValeursQuantis();

	// Redessine le tableau
	UpdateNbCells();
	Invalidate();

	UpdateWindow();
	UpdateNbCells();
	
}

// Gestion du déplacement split horizontal fenêtes cumulativité
LRESULT CDlgTableau::OnSplitVert(WPARAM WParam, LPARAM LPAram)
{
	// on récupère le déplacement
	LONG Disp = (LONG)WParam;

	// Déplace le split vertical
	DeplaceSplitVert(Disp);

	// Si boite dimension 2 visible, on retaille aussi la boite dimension 1
	if (m_Box_Dimension2.IsWindowVisible())
	{
		this->SetDimension2Visible(true); 
	}
	
	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Déplacement du split vertical
void CDlgTableau::DeplaceSplitVert(LONG Disp)
{
	// on récupère la position élmts boite questions
	RECT RectBQu_1; m_Box_Question.GetWindowRect(&RectBQu_1);				this->ScreenToClient(&RectBQu_1);
	RECT RectBQu_2; m_List_Questions.GetWindowRect(&RectBQu_2);				this->ScreenToClient(&RectBQu_2);
	RECT RectBQu_3; m_Label_Questions.GetWindowRect(&RectBQu_3);			this->ScreenToClient(&RectBQu_3);
	RECT RectBQu_4; m_Button_Questions_Tous.GetWindowRect(&RectBQu_4);		this->ScreenToClient(&RectBQu_4);

	// on récupère la position élmts boite dimension 1
	RECT RectBDim1_1; m_Box_Dimension1.GetWindowRect(&RectBDim1_1);			this->ScreenToClient(&RectBDim1_1);
	RECT RectBDim1_2; m_Header_Dimension1.GetWindowRect(&RectBDim1_2);		this->ScreenToClient(&RectBDim1_2);
	RECT RectBDim1_3; m_Button_Menu_Ope_Dim1.GetWindowRect(&RectBDim1_3);	this->ScreenToClient(&RectBDim1_3);	
	RECT RectBDim1_4; m_Button_Dim1_RAZ.GetWindowRect(&RectBDim1_4);		this->ScreenToClient(&RectBDim1_4);
	RECT RectBDim1_5bis; m_Button_Filtre.GetWindowRect(&RectBDim1_5bis);			this->ScreenToClient(&RectBDim1_5bis);
	RECT RectBDim1_5; m_Button_Dim1_UP.GetWindowRect(&RectBDim1_5);			this->ScreenToClient(&RectBDim1_5);
	RECT RectBDim1_6; m_Button_Dim1_DOWN.GetWindowRect(&RectBDim1_6);		this->ScreenToClient(&RectBDim1_6);
	RECT RectBDim1_7; m_Button_Dimension1.GetWindowRect(&RectBDim1_7);		this->ScreenToClient(&RectBDim1_7);
	RECT RectBDim1_8; m_Button_Dim1_OK.GetWindowRect(&RectBDim1_8);			this->ScreenToClient(&RectBDim1_8);
	RECT RectBDim1_9; m_Button_Dim1_ET.GetWindowRect(&RectBDim1_9);			this->ScreenToClient(&RectBDim1_9);
	RECT RectBDim1_10;m_Button_Dim1_OU.GetWindowRect(&RectBDim1_10);		this->ScreenToClient(&RectBDim1_10);
	RECT RectBDim1_11;m_Button_Dim1_NI.GetWindowRect(&RectBDim1_11);		this->ScreenToClient(&RectBDim1_11);
	RECT RectBDim1_12;m_Button_Dim1_XOR.GetWindowRect(&RectBDim1_12);		this->ScreenToClient(&RectBDim1_12);
	RECT RectBDim1_13;m_List_Dimension1.GetWindowRect(&RectBDim1_13);		this->ScreenToClient(&RectBDim1_13);

	// on récupère la position élmts boite dimension 2
	RECT RectBDim2_1; m_Box_Dimension2.GetWindowRect(&RectBDim2_1);			this->ScreenToClient(&RectBDim2_1);
	RECT RectBDim2_2; m_Header_Dimension2.GetWindowRect(&RectBDim2_2);		this->ScreenToClient(&RectBDim2_2);
	RECT RectBDim2_3; m_List_Dimension2.GetWindowRect(&RectBDim2_3);		this->ScreenToClient(&RectBDim2_3);
	RECT RectBDim2_4; m_Button_Dimension2.GetWindowRect(&RectBDim2_4);		this->ScreenToClient(&RectBDim2_4);
	RECT RectBDim2_5; m_Button_Dim2_ET.GetWindowRect(&RectBDim2_5);			this->ScreenToClient(&RectBDim2_5);
	RECT RectBDim2_6; m_Button_Dim2_OU.GetWindowRect(&RectBDim2_6);			this->ScreenToClient(&RectBDim2_6);
	RECT RectBDim2_7; m_Button_Dim2_NI.GetWindowRect(&RectBDim2_7);			this->ScreenToClient(&RectBDim2_7);
	RECT RectBDim2_8; m_Button_Dim2_XOR.GetWindowRect(&RectBDim2_8);		this->ScreenToClient(&RectBDim2_8);
	RECT RectBDim2_9; m_Button_Menu_Ope_Dim2.GetWindowRect(&RectBDim2_9);	this->ScreenToClient(&RectBDim2_9);
	RECT RectBDim2_10;m_Button_Dim2_RAZ.GetWindowRect(&RectBDim2_10);		this->ScreenToClient(&RectBDim2_10);

	// on récupère la position élmts boite SAS
	RECT RectBSAS_1; m_Box_SAS.GetWindowRect(&RectBSAS_1);					this->ScreenToClient(&RectBSAS_1);
	RECT RectBSAS_2; m_Label_SAS.GetWindowRect(&RectBSAS_2);				this->ScreenToClient(&RectBSAS_2);
	RECT RectBSAS_3; m_List_SAS.GetWindowRect(&RectBSAS_3);					this->ScreenToClient(&RectBSAS_3);
	RECT RectBSAS_4; m_Button_Menu_Ope_QuSel.GetWindowRect(&RectBSAS_4);	this->ScreenToClient(&RectBSAS_4);
	RECT RectBSAS_5; m_Button_SAS_ET.GetWindowRect(&RectBSAS_5);			this->ScreenToClient(&RectBSAS_5);
	RECT RectBSAS_6; m_Button_SAS_OU.GetWindowRect(&RectBSAS_6);			this->ScreenToClient(&RectBSAS_6);
	RECT RectBSAS_7; m_Button_Mode_Ligne.GetWindowRect(&RectBSAS_7);		this->ScreenToClient(&RectBSAS_7);
	RECT RectBSAS_8; m_Button_Mode_Colonne.GetWindowRect(&RectBSAS_8);		this->ScreenToClient(&RectBSAS_8);
	RECT RectBSAS_9; m_Button_Mode_Page.GetWindowRect(&RectBSAS_9);			this->ScreenToClient(&RectBSAS_9);
	RECT RectBSAS_10; m_Button_SAS_RAZ.GetWindowRect(&RectBSAS_10);			this->ScreenToClient(&RectBSAS_10);

	// on récupère la position élmts boite Segment
	/*
	RECT RectBSeg_1; m_Box_Segment.GetWindowRect(&RectBSeg_1);				this->ScreenToClient(&RectBSeg_1);
	RECT RectBSeg_2; m_Tab_Segment.GetWindowRect(&RectBSeg_2);				this->ScreenToClient(&RectBSeg_2);
	RECT RectBSeg_3; m_Button_CalcSegment.GetWindowRect(&RectBSeg_3);		this->ScreenToClient(&RectBSeg_3);
	RECT RectBSeg_4; m_Button_Menu_Ope_Segment.GetWindowRect(&RectBSeg_4);	this->ScreenToClient(&RectBSeg_4);
	RECT RectBSeg_5; m_Button_Segment_ET.GetWindowRect(&RectBSeg_5);		this->ScreenToClient(&RectBSeg_5);
	RECT RectBSeg_6; m_Button_Segment_OU.GetWindowRect(&RectBSeg_6);		this->ScreenToClient(&RectBSeg_6);
	*/
	
	// on récupère la position élmts boite Pupitre
	RECT RectBPup_1; m_Box_Pupitre.GetWindowRect(&RectBPup_1);				this->ScreenToClient(&RectBPup_1);
	RECT RectBPup_2; m_List_Row.GetWindowRect(&RectBPup_2);					this->ScreenToClient(&RectBPup_2);
	RECT RectBPup_3; m_List_Col.GetWindowRect(&RectBPup_3);					this->ScreenToClient(&RectBPup_3);
	RECT RectBPup_4; m_List_Page.GetWindowRect(&RectBPup_4);				this->ScreenToClient(&RectBPup_4);
	RECT RectBPup_5; m_Label_Lignes.GetWindowRect(&RectBPup_5);				this->ScreenToClient(&RectBPup_5);
	RECT RectBPup_6; m_Label_Colonnes.GetWindowRect(&RectBPup_6);			this->ScreenToClient(&RectBPup_6);
	RECT RectBPup_7; m_Label_Pages.GetWindowRect(&RectBPup_7);				this->ScreenToClient(&RectBPup_7);
	RECT RectBPup_7b; m_Button_Item_Moy_Lig.GetWindowRect(&RectBPup_7b);	this->ScreenToClient(&RectBPup_7b);
	RECT RectBPup_7c; m_Button_Item_Moy_Col.GetWindowRect(&RectBPup_7c);	this->ScreenToClient(&RectBPup_7c);
	RECT RectBPup_8;  m_Button_Ligne_UP.GetWindowRect(&RectBPup_8);			this->ScreenToClient(&RectBPup_8);
	RECT RectBPup_9;  m_Button_Colonne_UP.GetWindowRect(&RectBPup_9);		this->ScreenToClient(&RectBPup_9);
	RECT RectBPup_10; m_Button_Page_UP.GetWindowRect(&RectBPup_10);			this->ScreenToClient(&RectBPup_10);
	RECT RectBPup_11; m_Button_Ligne_DOWN.GetWindowRect(&RectBPup_11);		this->ScreenToClient(&RectBPup_11);
	RECT RectBPup_12; m_Button_Colonne_DOWN.GetWindowRect(&RectBPup_12);	this->ScreenToClient(&RectBPup_12);
	RECT RectBPup_13; m_Button_Page_DOWN.GetWindowRect(&RectBPup_13);		this->ScreenToClient(&RectBPup_13);
	RECT RectBPup_14; m_Btn_All_Lignes.GetWindowRect(&RectBPup_14);			this->ScreenToClient(&RectBPup_14);
	RECT RectBPup_15; m_Btn_All_Colonnes.GetWindowRect(&RectBPup_15);		this->ScreenToClient(&RectBPup_15);
	RECT RectBPup_16; m_Btn_All_Pages.GetWindowRect(&RectBPup_16);			this->ScreenToClient(&RectBPup_16);
	RECT RectBPup_17; m_Btn_SitesEnLigne.GetWindowRect(&RectBPup_17);		this->ScreenToClient(&RectBPup_17);
	RECT RectBPup_18; m_Btn_SitesEnColonne.GetWindowRect(&RectBPup_18);		this->ScreenToClient(&RectBPup_18);
	RECT RectBPup_19; m_Btn_SitesEnPage.GetWindowRect(&RectBPup_19);		this->ScreenToClient(&RectBPup_19);

	// on récupère la position élmts boite Edit Item
	RECT RectBIt_1;  m_Box_Items.GetWindowRect(&RectBIt_1);					this->ScreenToClient(&RectBIt_1);
	RECT RectBIt_2;  m_Button_Suppression.GetWindowRect(&RectBIt_2);		this->ScreenToClient(&RectBIt_2);
	RECT RectBIt_3;  m_Edit_Name.GetWindowRect(&RectBIt_3);					this->ScreenToClient(&RectBIt_3);
	RECT RectBIt_4;  m_Button_Accept.GetWindowRect(&RectBIt_4);				this->ScreenToClient(&RectBIt_4);
	RECT RectBIt_5;  m_Label_Item.GetWindowRect(&RectBIt_5);				this->ScreenToClient(&RectBIt_5);

	// Position split
	RECT RectSP; m_SplitVert.GetWindowRect(&RectSP); this->ScreenToClient(&RectSP);

	// Limite boite questions et SAS
	// JUnt32 MinLargBoiteSas = 500; // 230;
	JUnt32 MinLargBoiteSas = 100;
	LONG Larg = RectBQu_1.right - RectBQu_1.left; 
	if ((Larg + Disp) < MinLargBoiteSas || (Larg + Disp) < 0) 
		Disp = MinLargBoiteSas - Larg;

	// Limitation réduction de la boite centrale
	if ((Larg + Disp) < 230)
		Disp = 0;

	// Limite boite résultat
	Larg = RectBPup_1.right - RectBPup_1.left; 
	if ((Larg - Disp) < MinLargBoiteSas || (Larg - Disp) < 0)  
		Disp = Larg - MinLargBoiteSas;
	
	// on corrige la position elmts fenetre boite questions
	RectBQu_1.right += Disp;
	RectBQu_2.right += Disp;
	RectBQu_3.right += Disp;
	RectBQu_4.left  += Disp;
	RectBQu_4.right += Disp;

	// on corrige la position elmts fenetre dimension 1
	RectBDim1_1.right += Disp;
	RectBDim1_2.right += Disp;
	RectBDim1_4.left  += Disp;
	RectBDim1_4.right += Disp;
	RectBDim1_5.left  += Disp;
	RectBDim1_5.right += Disp;
	RectBDim1_5bis.left  += Disp;
	RectBDim1_5bis.right += Disp;
	RectBDim1_6.left  += Disp;
	RectBDim1_6.right += Disp;
	RectBDim1_7.right += Disp;
	RectBDim1_13.right += Disp;

	// on corrige la position elmts fenetre dimension 2
	RectBDim2_1.right  += Disp;
	RectBDim2_2.right  += Disp;
	RectBDim2_3.right  += Disp;
	RectBDim2_4.right  += Disp;
	RectBDim2_10.left  += Disp;
	RectBDim2_10.right += Disp;

	// on corrige la position elmts fenetre SAS
	RectBSAS_1.right  += Disp;
	RectBSAS_2.right  += Disp;
	RectBSAS_3.right  += Disp;
	RectBSAS_10.right += Disp;
	RectBSAS_10.left  += Disp;

	// Augmente taille des flèches, on les limites à 90 comme sur les RN, et à 47 taille origine
	const int LimXMinFleche = 47;
	const int LimXMaxFleche = 90;
	JUnt32 Ecart = RectBSAS_8.left - RectBSAS_7.right;

	RectBSAS_7.right  += Disp/3; 
	RectBSAS_8.left   += Disp/3; 
	RectBSAS_8.right  += 2* Disp/3; 
	RectBSAS_9.left   += 2* Disp/3; 
	RectBSAS_9.right  += Disp; 

	// Test limite taille flèche
	if (RectBSAS_1.right - RectBSAS_1.left < 3* LimXMaxFleche + 15)
	{
		JUnt32 NewLarg = RectBSAS_1.right - RectBSAS_1.left - 15;
		RectBSAS_7.right  = RectBSAS_7.left  + NewLarg/3; 
		RectBSAS_8.left   = RectBSAS_7.right + Ecart; 
		RectBSAS_8.right  = RectBSAS_8.left  + NewLarg/3; 
		RectBSAS_9.left   = RectBSAS_8.right + Ecart; 
		RectBSAS_9.right  = RectBSAS_9.left  + NewLarg/3; 
	}

	// Limite maxi largeur flèche
	if (RectBSAS_7.right - RectBSAS_7.left > LimXMaxFleche)
	{
		RectBSAS_7.right = RectBSAS_7.left + LimXMaxFleche;
		RectBSAS_8.left  = RectBSAS_7.right + Ecart; 
		RectBSAS_8.right = RectBSAS_8.left + LimXMaxFleche;
		RectBSAS_9.left  = RectBSAS_8.right + Ecart; 
		RectBSAS_9.right = RectBSAS_9.left + LimXMaxFleche;
	}
	// Limite mini largeur flèche
	else if (RectBSAS_7.right - RectBSAS_7.left < LimXMinFleche)
	{
		RectBSAS_7.right = RectBSAS_7.left + LimXMinFleche;
		RectBSAS_8.left  = RectBSAS_7.right + Ecart; 
		RectBSAS_8.right = RectBSAS_8.left + LimXMinFleche;
		RectBSAS_9.left  = RectBSAS_8.right + Ecart; 
		RectBSAS_9.right = RectBSAS_9.left + LimXMinFleche;
	}
			
	// on corrige la position elmts fenetre segment
	/*
	CLabel										m_Box_Segment;
	JFCTabCtrl									m_Tab_Segment;
	CButtonST									m_Button_CalcSegment;
	CButtonST									m_Button_Menu_Ope_Segment;
	CButtonST									m_Button_Segment_ET;
	ButtonST									m_Button_Segment_OU;
	*/

	// on corrige la position elmts fenêtre sélection pupitre
	RectBPup_1.left    += Disp;
	RectBPup_2.left	   += Disp;
	RectBPup_3.left	   += Disp;
	RectBPup_4.left	   += Disp;
	RectBPup_5.left	   += Disp;
	RectBPup_6.left	   += Disp;;
	RectBPup_7.left	   += Disp;
	RectBPup_17.left   += Disp;
	RectBPup_17.right  += Disp;
	RectBPup_18.left   += Disp;
	RectBPup_18.right  += Disp;
	RectBPup_19.left   += Disp;
	RectBPup_19.right  += Disp;

	// on corrige la position elmts fenêtre Edit Items
	RectBIt_1.left    += Disp;
	RectBIt_2.left    += Disp;
	RectBIt_2.right   += Disp;
	RectBIt_3.left    += Disp;
	
	// La barre split
	RectSP.left += Disp; RectSP.right += Disp;

	// on déplace objets boite questions
	m_Box_Question.SetWindowPos(NULL, RectBQu_1.left, RectBQu_1.top, RectBQu_1.right - RectBQu_1.left, RectBQu_1.bottom - RectBQu_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Questions.SetWindowPos(NULL, RectBQu_2.left, RectBQu_2.top, RectBQu_2.right - RectBQu_2.left, RectBQu_2.bottom - RectBQu_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Questions.SetWindowPos(NULL, RectBQu_3.left, RectBQu_3.top, RectBQu_3.right - RectBQu_3.left, RectBQu_3.bottom - RectBQu_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Questions_Tous.SetWindowPos(NULL, RectBQu_4.left, RectBQu_4.top, RectBQu_4.right - RectBQu_4.left, RectBQu_4.bottom - RectBQu_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite dimension 1
	m_Box_Dimension1.SetWindowPos(NULL, RectBDim1_1.left, RectBDim1_1.top, RectBDim1_1.right - RectBDim1_1.left, RectBDim1_1.bottom - RectBDim1_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Header_Dimension1.SetWindowPos(NULL, RectBDim1_2.left, RectBDim1_2.top, RectBDim1_2.right - RectBDim1_2.left, RectBDim1_2.bottom - RectBDim1_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_RAZ.SetWindowPos(NULL, RectBDim1_4.left, RectBDim1_4.top, RectBDim1_4.right - RectBDim1_4.left, RectBDim1_4.bottom - RectBDim1_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Filtre.SetWindowPos(NULL, RectBDim1_5bis.left, RectBDim1_5bis.top, RectBDim1_5bis.right - RectBDim1_5bis.left, RectBDim1_5bis.bottom - RectBDim1_5bis.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_UP.SetWindowPos(NULL, RectBDim1_5.left, RectBDim1_5.top, RectBDim1_5.right - RectBDim1_5.left, RectBDim1_5.bottom - RectBDim1_5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim1_DOWN.SetWindowPos(NULL, RectBDim1_6.left, RectBDim1_6.top, RectBDim1_6.right - RectBDim1_6.left, RectBDim1_6.bottom - RectBDim1_6.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dimension1.SetWindowPos(NULL, RectBDim1_7.left, RectBDim1_7.top, RectBDim1_7.right - RectBDim1_7.left, RectBDim1_7.bottom - RectBDim1_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Dimension1.SetWindowPos(NULL, RectBDim1_13.left, RectBDim1_13.top, RectBDim1_13.right - RectBDim1_13.left, RectBDim1_13.bottom - RectBDim1_13.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite dimension 2
	m_Box_Dimension2.SetWindowPos(NULL, RectBDim1_1.left, RectBDim1_1.top, RectBDim1_1.right - RectBDim1_1.left, RectBDim1_1.bottom - RectBDim1_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Header_Dimension2.SetWindowPos(NULL, RectBDim2_2.left, RectBDim2_2.top, RectBDim2_2.right - RectBDim2_2.left, RectBDim2_2.bottom - RectBDim2_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Dimension2.SetWindowPos(NULL, RectBDim2_3.left, RectBDim2_3.top, RectBDim2_3.right - RectBDim2_3.left, RectBDim2_3.bottom - RectBDim2_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dimension2.SetWindowPos(NULL, RectBDim2_4.left, RectBDim2_4.top, RectBDim2_4.right - RectBDim2_4.left, RectBDim2_4.bottom - RectBDim2_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Dim2_RAZ.SetWindowPos(NULL, RectBDim2_10.left, RectBDim2_10.top, RectBDim2_10.right - RectBDim2_10.left, RectBDim2_10.bottom - RectBDim2_10.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite SAS
	m_Box_SAS.SetWindowPos(NULL, RectBSAS_1.left, RectBSAS_1.top, RectBSAS_1.right - RectBSAS_1.left, RectBSAS_1.bottom - RectBSAS_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_SAS.SetWindowPos(NULL, RectBSAS_2.left, RectBSAS_2.top, RectBSAS_2.right - RectBSAS_2.left, RectBSAS_2.bottom - RectBSAS_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_SAS.SetWindowPos(NULL, RectBSAS_3.left, RectBSAS_3.top, RectBSAS_3.right - RectBSAS_3.left, RectBSAS_3.bottom - RectBSAS_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	/* Plus de resize fléche sélection
	m_Button_Mode_Ligne.SetWindowPos(NULL, RectBSAS_7.left, RectBSAS_7.top, RectBSAS_7.right - RectBSAS_7.left, RectBSAS_7.bottom - RectBSAS_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Mode_Colonne.SetWindowPos(NULL, RectBSAS_8.left, RectBSAS_8.top, RectBSAS_8.right - RectBSAS_8.left, RectBSAS_8.bottom - RectBSAS_8.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Mode_Page.SetWindowPos(NULL, RectBSAS_9.left, RectBSAS_9.top, RectBSAS_9.right - RectBSAS_9.left, RectBSAS_9.bottom - RectBSAS_9.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	*/
	m_Button_SAS_RAZ.SetWindowPos(NULL, RectBSAS_10.left, RectBSAS_10.top, RectBSAS_10.right - RectBSAS_10.left, RectBSAS_10.bottom - RectBSAS_10.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite pupitre
	m_Box_Pupitre.SetWindowPos(NULL, RectBPup_1.left, RectBPup_1.top, RectBPup_1.right - RectBPup_1.left, RectBPup_1.bottom - RectBPup_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Row.SetWindowPos(NULL, RectBPup_2.left, RectBPup_2.top, RectBPup_2.right - RectBPup_2.left, RectBPup_2.bottom - RectBPup_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Col.SetWindowPos(NULL, RectBPup_3.left, RectBPup_3.top, RectBPup_3.right - RectBPup_3.left, RectBPup_3.bottom - RectBPup_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_List_Page.SetWindowPos(NULL, RectBPup_4.left, RectBPup_4.top, RectBPup_4.right - RectBPup_4.left, RectBPup_4.bottom - RectBPup_4.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Lignes.SetWindowPos(NULL, RectBPup_5.left, RectBPup_5.top, RectBPup_5.right - RectBPup_5.left, RectBPup_5.bottom - RectBPup_5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Colonnes.SetWindowPos(NULL, RectBPup_6.left, RectBPup_6.top, RectBPup_6.right - RectBPup_6.left, RectBPup_6.bottom - RectBPup_6.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label_Pages.SetWindowPos(NULL, RectBPup_7.left, RectBPup_7.top, RectBPup_7.right - RectBPup_7.left, RectBPup_7.bottom - RectBPup_7.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Btn_SitesEnLigne.SetWindowPos(NULL, RectBPup_17.left, RectBPup_17.top, RectBPup_17.right - RectBPup_17.left, RectBPup_17.bottom - RectBPup_17.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Btn_SitesEnColonne.SetWindowPos(NULL, RectBPup_18.left, RectBPup_18.top, RectBPup_18.right - RectBPup_18.left, RectBPup_18.bottom - RectBPup_18.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Btn_SitesEnPage.SetWindowPos(NULL, RectBPup_19.left, RectBPup_19.top, RectBPup_19.right - RectBPup_19.left, RectBPup_19.bottom - RectBPup_19.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// On déplace objets boite edit items
	m_Box_Items.SetWindowPos(NULL, RectBIt_1.left, RectBIt_1.top, RectBIt_1.right - RectBIt_1.left, RectBIt_1.bottom - RectBIt_1.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Button_Suppression.SetWindowPos(NULL, RectBIt_2.left, RectBIt_2.top, RectBIt_2.right - RectBIt_2.left, RectBIt_2.bottom - RectBIt_2.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Edit_Name.SetWindowPos(NULL, RectBIt_3.left, RectBIt_3.top, RectBIt_3.right - RectBIt_3.left, RectBIt_3.bottom - RectBIt_3.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	RectBIt_5.right = RectBIt_3.left - 5;
	RectBIt_5.left = RectBIt_1.left + 5;
	m_Label_Item.SetWindowPos(NULL, RectBIt_5.left, RectBIt_5.top, RectBIt_5.right - RectBIt_5.left, RectBIt_5.bottom - RectBIt_5.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	
	// On déplace la barre split vertical 
	m_SplitVert.SetWindowPos(NULL, RectSP.left, RectSP.top, RectSP.right - RectSP.left, RectSP.bottom - RectSP.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// on calcule la configuration du split
	LONG SPLeft		= RectBQu_1.right - RectBQu_1.left; if (SPLeft < 0) SPLeft = 0;
	LONG SPRight	= RectBPup_1.right - RectBPup_1.left; if (SPRight < 0) SPRight = 0;

	
	// on reconfigure le split
	m_SplitVert.SetRange(SPLeft, SPRight);

	// Positionne la fenêtre des valeurs
	PosFenValeursQuantis();

	// Mise à jour de la fenêtre pupitre
	UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
// Détection sélection ou désélection sur la liste lignes
void CDlgTableau::OnNMClickRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	if (MajNbCells)
		SetValidUpDownListe(m_List_Row);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// Détection sélection ou désélection sur la liste colonnes
void CDlgTableau::OnNMClickRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	if (MajNbCells)
		SetValidUpDownListe(m_List_Col);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// Détection sélection ou désélection sur la liste pages
void CDlgTableau::OnNMClickRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	if (MajNbCells)
		SetValidUpDownListe(m_List_Page);

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////
// Suppression de toutes les lignes
void CDlgTableau::OnSupprimertoutesleslignes()
{
	// Nombre d'items ligne sélectionnés
	int NbItems = m_List_Row.GetItemCount();

	// Suppression des élmts en mémoire
	for(int x = NbItems-1; x >= 0 ; --x)
	{
		m_Items_Del.AddTail()=(CItemWrapper *)m_List_Row.GetItemData(x);
	}

	// Vide liste sélections ligne
	m_List_Row.DeleteAllItems();

	// Mise à jour
	m_DType_Row = DTYPE_ITEMS;  
	m_bRowsChanged = true;
	UpdateCount();

	// Mise à jour affichage listes sélections
	SetListeResultat();
}

/////////////////////////////////////////////////////////////////////////
// Suppression de toutes les colonnes
void CDlgTableau::OnSupprimertouteslescolonnes()
{
	// Nombre d'items colonne sélectionnés
	int NbItems = m_List_Col.GetItemCount();

	// Suppression des élmts en mémoire
	for(int x = NbItems-1; x >= 0 ; --x)
	{
		m_Items_Del.AddTail()=(CItemWrapper *)m_List_Col.GetItemData(x);
	}

	// Vide liste sélections colonne
	m_List_Col.DeleteAllItems(); 

	// Mise à jour
	m_DType_Col = DTYPE_ITEMS;  
	m_bColsChanged = true;
	UpdateCount();

	// Mise à jour affichage listes sélections
	SetListeResultat();
}

/////////////////////////////////////////////////////////////////////////
// Suppression de toutes les pages
void CDlgTableau::OnSupprimertouteslespages()
{
	// Nombre d'items page sélectionnés
	int NbItems = m_List_Page.GetItemCount();

	// Suppression des élmts en mémoire
	for(int x = NbItems-1; x >= 0 ; --x)
	{
		m_Items_Del.AddTail()=(CItemWrapper *)m_List_Page.GetItemData(x);
	}

	// Vide liste sélections page
	m_List_Page.DeleteAllItems(); 
	
	// Mise à jour affichage listes sélections
	SetListeResultat();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression lignes sélectionnées
void CDlgTableau::OnRiMenuSuppLignes()
{
	// Suppression des élmts sélection lignes
	SuppressionSelection(m_List_Row);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression colonnes sélectionnées
void CDlgTableau::OnRiMenuSuppColonnes()
{
	// Suppression des élmts sélection colonnes
	SuppressionSelection(m_List_Col);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression pages sélectionnées
void CDlgTableau::OnRiMenuSuppPages()
{
	// Suppression des élmts sélection pages
	SuppressionSelection(m_List_Page);	
}

//////////////////////////////////////////////////////////////////////////////////////////
// Suppression de toutes les sélections
void CDlgTableau::OnRiMenuSuppTout()
{
	// Effacer tous les élmts pages
	OnSupprimertouteslespages();
	
	// Effacer tous les élmts colonnes
	OnSupprimertouteslescolonnes();

	// Effacer tous les élmts lignes
	OnSupprimertoutesleslignes();

	// En + virer l'ajout sélection
	// OnItem_Raz();
	OnTout_Raz();
}

///////////////////////////////////////////////////////////////////////////////
// Effacement de toutes les sélections
void CDlgTableau::OnRiMenuSuppAllLcp()
{
	// Effacer tous les élmts pages
	OnSupprimertouteslespages();
	
	// Effacer tous les élmts colonnes
	OnSupprimertouteslescolonnes();

	// Effacer tous les élmts lignes
	OnSupprimertoutesleslignes();
	
	// Mettre les opérateurs par défaut
	// AllOperateurParDefaut();
}

////////////////////////////////////////////////////////////////////////////////////////
// Suppression de toute la sélection (lignes + colonnes + pages)
void CDlgTableau::EffacerAllSel()
{
	OnRiMenuSuppLignes();
	OnRiMenuSuppColonnes();
	OnRiMenuSuppPages();
}

///////////////////////////////////////////////////////////////////////////////////////
// Etat sélection items lignes changent
void CDlgTableau::OnLvnItemchangedRiListLignes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	SetValidUpDownListe(m_List_Row);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	// Mise à jour total cellules
	// UpdateNbCells();
}

///////////////////////////////////////////////////////////////////////////////////////
// Etat sélection items colonnes changent
void CDlgTableau::OnLvnItemchangedRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	SetValidUpDownListe(m_List_Col);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	// Mise à jour total cellules
	// UpdateNbCells();
}

///////////////////////////////////////////////////////////////////////////////////////
// Etat sélection items pages changent
void CDlgTableau::OnLvnItemchangedRiListPages(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Mise à jour validté des boutons positionnement up & down
	SetValidUpDownListe(m_List_Page);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	// Mise à jour total cellules
	// UpdateNbCells();
}

///////////////////////////////////////////////////////////////////////////////////////
// Ajout de sites en ligne pour la version SQN-Profiling
void CDlgTableau::OnBnClickedRiBtnSitesLig()
{
	CSiteSelectionDlg Dlg(this);

	// Positionne le terrain témoin pour sélection des sites
	Dlg.SetTerrainTemoin(m_pTerrain);

	// Affiche la boite de sélection des sites
	if(Dlg.DoModal() == IDOK)
	{
		// AJOUTER JUIN 2007
		m_DType_Row = DTYPE_SITES;
		if(m_DType_Col == DTYPE_SITES)
		{
			m_DType_Col = DTYPE_ITEMS;
		}
		if(m_DType_Page == DTYPE_SITES)
		{
			m_DType_Page = DTYPE_ITEMS;
		}

		// Get a list of item construits 
		JList<CItemWrapper *> List;
		Dlg.GetSelectedItems(List);

		// Sélection site sur une seule dimension
		if (List.GetCount() > 0)
		{
			// On vire les sites si sélection en colonne ou page
			VireSites(m_List_Col);
			VireSites(m_List_Page);
		}

		// Add them to the List control and item count
		int i = m_List_Row.GetItemCount(); 
		for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
		{
			/*
			// Add to the list control
			CItemWrapper * pItemWrapper = List.GetItem();
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 
			if (pItemConstruit->IsItemConstruit())
			{
				// JStringEx Str = pItemConstruit->GetIBString().AsJCharPtr(); 
				CString Str = pItemConstruit->GetIBString().AsJCharPtr(); 
				bool Ok = true;
			}
			*/
			
			m_List_Row.InsertItem(LVIF_PARAM|LVIF_TEXT,i,List.GetItem()->GetLabel(),0,0,0,(LPARAM)List.GetItem());
			m_bRowsChanged = true; 
			// Add to the main item list
			m_Items_New.AddTail() = List.GetItem();
		}
	}

	// Mise à jour compteur nb sélections lignes
	UpdateCount();

	// Retaille colonne sélections
	ResizeColListSelect(CPaletteMode::Line);
}

///////////////////////////////////////////////////////////////////////////////////////
// Ajout de sites en ligne pour la version SQN-Profiling
void CDlgTableau::OnBnClickedRiBtnSitesCol()
{
	CSiteSelectionDlg Dlg(this);

	// Positionne le terrain témoin pour sélection des sites
	Dlg.SetTerrainTemoin(m_pTerrain);

	// Affiche la boite de sélection des sites
	if(Dlg.DoModal() == IDOK)
	{
		// AJOUTER JUIN 2007
		m_DType_Col = DTYPE_SITES;
		if(m_DType_Row == DTYPE_SITES)
		{
			m_DType_Row = DTYPE_ITEMS;
		}
		if(m_DType_Page == DTYPE_SITES)
		{
			m_DType_Page = DTYPE_ITEMS;
		}

		// Get a list of item construits 
		JList<CItemWrapper *> List;
		Dlg.GetSelectedItems(List);

		// Sélection site sur une seule dimension
		if (List.GetCount() > 0)
		{
			// On vire les sites si sélection en ligne ou page
			VireSites(m_List_Row);
			VireSites(m_List_Page);
		}

		// Add them to the List control and item count
		int i = m_List_Col.GetItemCount(); 
		for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
		{
			// Add to the list control
			m_List_Col.InsertItem(LVIF_PARAM|LVIF_TEXT,i,List.GetItem()->GetLabel(),0,0,0,(LPARAM)List.GetItem());
			m_bColsChanged = true; 

			// Add to the main item list
			m_Items_New.AddTail() = List.GetItem();
		}
	}

	// Mise à jour compteur nb sélections colonnes
	UpdateCount();

	// Retaille colonne sélections
	ResizeColListSelect(CPaletteMode::Column);
}

///////////////////////////////////////////////////////////////////////////////////////
// Ajout de sites en ligne pour la version SQN-Profiling
void CDlgTableau::OnBnClickedRiBtnSitesPage()
{
	CSiteSelectionDlg Dlg(this);

	// Positionne le terrain témoin pour sélection des sites
	Dlg.SetTerrainTemoin(m_pTerrain);

	// Affiche la boite de sélection des sites
	if(Dlg.DoModal() == IDOK)
	{
		// AJOUTER JUIN 2007
		m_DType_Page = DTYPE_SITES;

		if(m_DType_Row == DTYPE_SITES)
		{
			m_DType_Row = DTYPE_ITEMS;
		}
		if(m_DType_Col == DTYPE_SITES)
		{
			m_DType_Col = DTYPE_ITEMS;
		}

	
		// Get a list of item construits 
		JList<CItemWrapper *> List;
		Dlg.GetSelectedItems(List);

		// Sélection site sur une seule dimension
		if (List.GetCount() > 0)
		{
			// On vire les sites si sélection en ligne ou page
			VireSites(m_List_Row);
			VireSites(m_List_Col);
		}

		// Add them to the List control and item count
		int i = m_List_Page.GetItemCount(); 
		for(List.MoveFirst(); List.IsValid(); List.MoveNext(),++i)
		{
			// Add to the list control
			m_List_Page.InsertItem(LVIF_PARAM|LVIF_TEXT,i,List.GetItem()->GetLabel(),0,0,0,(LPARAM)List.GetItem());
			m_bPagesChanged = true; 

			// Add to the main item list
			m_Items_New.AddTail() = List.GetItem();
		}
	}

	// Mise à jour compteur nb sélections pages
	UpdateCount();

	// Retaille colonne sélections
	ResizeColListSelect(CPaletteMode::Page);
}

///////////////////////////////////////////////////////////////////////////////////////
// Vire les sites sélectionnés en ligne
void CDlgTableau::VireSites(JFCSelListCtrl &Liste)
{
	if (Liste == m_List_Row)
	{
		// Nombre d'items ligne sélectionnés
		int NbItems = m_List_Row.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Row.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			if (pItemConstruit->IsItemSite())
			{
				m_Items_Del.AddTail()=(CItemWrapper *)m_List_Row.GetItemData(x);
				m_List_Row.DeleteItem(x);
			}
		}

		// Mise à jour
		m_DType_Row = DTYPE_ITEMS;  
		m_bRowsChanged = true;
		UpdateCount();

		// Mise à jour affichage listes sélections
		SetListeResultat();
	}
	else if (Liste == m_List_Col)
	{
		// Nombre d'items colonne sélectionnés
		int NbItems = m_List_Col.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Col.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 
			if (pItemConstruit->IsItemSite())
			{
				m_Items_Del.AddTail()=(CItemWrapper *)m_List_Col.GetItemData(x);
				m_List_Col.DeleteItem(x);
			}
		}

		// Mise à jour
		m_DType_Col = DTYPE_ITEMS;  
		m_bColsChanged = true;
		UpdateCount();

		// Mise à jour affichage listes sélections
		SetListeResultat();
	}
	else if (Liste == m_List_Page)
	{
		// Nombre d'items page sélectionnés
		int NbItems = m_List_Page.GetItemCount();

		// Suppression des élmts en mémoire
		for(int x = NbItems-1; x >= 0 ; --x)
		{
			// Récup élmt item liste
			DWORD_PTR  data = m_List_Page.GetItemData(x) ;
			CItemWrapper *pItemWrapper =(CItemWrapper *)data;

			// Vérifie que c'est un site
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			if (pItemConstruit->IsItemSite())
			{
				m_Items_Del.AddTail()=(CItemWrapper *)m_List_Page.GetItemData(x);
				m_List_Page.DeleteItem(x);
			}
		}

		// Mise à jour
		m_DType_Page = DTYPE_ITEMS;  
		m_bPagesChanged = true;
		UpdateCount();

		// Mise à jour affichage listes sélections
		SetListeResultat();
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection de toutes les lignes
void CDlgTableau::OnBnClickedRiButAllLignes()
{
	m_List_Row.SetRedraw(FALSE);
	MajNbCells = FALSE;

	// On inverse, sélection de tous si aucune sélection
	if (m_List_Row.GetSelectedCount() == 0)
	{
		// Si aucun sélectionné, sélection globale
		for (int i=0; i<m_List_Row.GetItemCount(); i++)
			m_List_Row.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		// Si au moins 1 sélectionné, désélection globale
		for (int i=0; i<m_List_Row.GetItemCount(); i++)
			m_List_Row.SetItemState(i,0, LVIS_SELECTED);
	}

	MajNbCells = TRUE;
	SetValidUpDownListe(m_List_Row);
	m_List_Row.SetRedraw(TRUE);
	m_List_Row.Invalidate();
	m_List_Row.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection de toutes les colonnes
void CDlgTableau::OnBnClickedRiButAllColonnes()
{
	m_List_Col.SetRedraw(FALSE);
	MajNbCells = FALSE;

	// On inverse, sélection de tous si aucune sélection
	if (m_List_Col.GetSelectedCount() == 0)
	{
		// Si aucun sélectionné, sélection globale
		for (int i=0; i<m_List_Col.GetItemCount(); i++)
			m_List_Col.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		// Si au moins 1 sélectionné, désélection globale
		for (int i=0; i<m_List_Col.GetItemCount(); i++)
			m_List_Col.SetItemState(i,0, LVIS_SELECTED);
	}

	MajNbCells = TRUE;
	SetValidUpDownListe(m_List_Row);
	m_List_Col.SetRedraw(TRUE);
	m_List_Col.Invalidate();
	m_List_Col.UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////////////
// Sélection ou déselection de toutes les pages
void CDlgTableau::OnBnClickedRiButAllPages()
{
	m_List_Page.SetRedraw(FALSE);
	MajNbCells = FALSE;

	// On inverse, sélection de tous si aucune sélection
	if (m_List_Page.GetSelectedCount() == 0)
	{
		// Si aucun sélectionné, sélection globale
		for (int i=0; i<m_List_Page.GetItemCount(); i++)
			m_List_Page.SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		// Si au moins 1 sélectionné, désélection globale
		for (int i=0; i<m_List_Page.GetItemCount(); i++)
			m_List_Page.SetItemState(i,0, LVIS_SELECTED);
	}

	MajNbCells = TRUE;
	SetValidUpDownListe(m_List_Row);
	m_List_Page.SetRedraw(TRUE);
	m_List_Page.Invalidate();
	m_List_Page.UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lance le sélecteur d'items quantitatifs à moyenner en ligne
void CDlgTableau::OnBnClickedRiItemMoyenLig()
{
	// Prépare les listes sélection de retour
	JList <JUnt32> LstItemMoyAuto;
	JList <JUnt32> LstItemMoyUtil;

	// Mise en place fenêtre dialogue sélection items à moyenner
	CDlgItemsMoyenne DlgItemsMoyenne(this, &LstItemMoyAuto, &LstItemMoyUtil);
	DlgItemsMoyenne.m_CATPColorsApp = this->m_CATPColorsApp;

	// Affiche la fenêtre de sélection des items à moyenner
	if(IDOK == DlgItemsMoyenne.DoModal())
	{	
		// Ajoute les items quantitatifs sélectionnés en ligne
		AddItemsQuantis(&LstItemMoyAuto, &LstItemMoyUtil);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lance le sélecteur d'items quantitatifs à moyenner en colonne
void CDlgTableau::OnBnClickedRiItemMoyenCol()
{
	// Prépare les listes sélection de retour
	JList <JUnt32> LstItemMoyAuto;
	JList <JUnt32> LstItemMoyUtil;

	// Mise en place fenêtre dialogue sélection items à moyenner
	CDlgItemsMoyenne DlgItemsMoyenne(this, &LstItemMoyAuto, &LstItemMoyUtil);
	DlgItemsMoyenne.m_CATPColorsApp = this->m_CATPColorsApp;

	// Affiche la fenêtre de sélection des items à moyenner
	if(IDOK == DlgItemsMoyenne.DoModal())
	{	
		// Ajoute les items quantitatifs sélectionnés en ligne
		AddItemsQuantis(&LstItemMoyAuto, &LstItemMoyUtil);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Ajout des items quantitatifs sélectionnés et positionnement en ligne
void CDlgTableau::AddItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil)
{
	// Voir si les listes sont bien définies
	if (pLstItemMoyAuto == NULL || pLstItemMoyUtil == NULL) return;

	// Effacement des fenêtres dimension mobiles
	if (m_pCurMode != m_pCurModeLigne)
		m_pCurMode->EffaceFenDimDlg(false);

	// Si Bouton Controle Actif au même moment, on est en mode non effacement sélection courante
	// (cela permet de copier dans le même temps en ligne, colonn, page sans recommencer la sélection)
	JBool EffaceSelCur = true;
	if(GetAsyncKeyState(VK_CONTROL)<0)
		EffaceSelCur = false;

	// Valider 1er élmt actif du sas
	if (pLstItemMoyAuto->GetCount() > 0 || pLstItemMoyUtil->GetCount() > 0)
	{
		// En blanc liste sélections ligne
		m_List_Row.				SetBkColor(CATPColors::GetColorWhite());
		m_List_Row.				SetBackColor(CATPColors::GetColorWhite());
		m_List_Col.				SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Col.				SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBkColor(CATPColors::GetColorLight(m_CATPColorsApp));
		m_List_Page.			SetBackColor(CATPColors::GetColorLight(m_CATPColorsApp));

		// Active le bouton ligne, désactive les autres
		m_Button_Mode_Ligne.	SetTypeOnglet(FALSE,TRUE,TRUE); 
		m_Button_Mode_Colonne.	SetTypeOnglet(FALSE,FALSE,TRUE); 
		m_Button_Mode_Page.		SetTypeOnglet(FALSE,FALSE,TRUE); 


		if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
		{
			// Mise à jour du pupitre avec nouveau mode
			UpdatePupitre(IDC_RI_RAD_MODE_LIGNES);

			// Valide les items à moyenner
			ValideItemsQuantis(pLstItemMoyAuto, pLstItemMoyUtil);

			// Mise à jour nb lignes
			UpdateCount();
		}
		else if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
		{
			// Mise à jour du pupitre avec nouveau mode
			UpdatePupitre(IDC_RI_RAD_MODE_COLONNES);

			// Valide les items à moyenner
			ValideItemsQuantis(pLstItemMoyAuto, pLstItemMoyUtil);

			// Mise à jour nb lignes
			UpdateCount();	
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Validation des items à moyenner
void CDlgTableau::ValideItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil)
{
	// Ici à ce niveau les 2 listes sont bien définies
	if (m_pCurMode)
	{
		// Récup instance des items construits automatiques
		CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);
		CItemsConstruitsBase	* pItemsConstruitsAuto		= CItemsConstruitsManager::GetInstance()->GetItemsInstance();
		
		// Positionne nouveau mode
		if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
			m_Id_CurModePrec = CPaletteMode::Line;
		else
			m_Id_CurModePrec = CPaletteMode::Column;

		// Fabrique liste des ItemsWrappers Items Quantis Auto à prendre en compte
		m_AllItemsAutos.Reset();

		// Balaye la liste des items sélectionnés quantis automatiques
		for (pLstItemMoyAuto->MoveFirst(); pLstItemMoyAuto->IsValid(); pLstItemMoyAuto->MoveNext())
		{
			// Ident item quanti
			JInt32 IdItem = pLstItemMoyAuto->GetItem();

			// Construction de l'item construit correspondant
			IItem * pIItem = pItemsConstruitsAuto->GetIItemByID(IdItem);
			CItemConstruit * pItemConstruit = (CItemConstruit *)(pIItem->AsItemConstruit());

			// On indique que cet item sera moyennable
			pItemConstruit->SetMoyennable(true);

			// Récup header question et libellé de l'item quantitatif crée
			CString LibItem		= pItemConstruit->GetLabel().AsJCharPtr();

			// Position du séparateur
			JInt32 PosSepHeaderLib = LibItem.Find(_T('@'));
			if(PosSepHeaderLib)
			{
				CString HeaderStr	= LibItem.Left(LibItem.Find(_T('@')));
				CString LabelStr	= LibItem.Mid(LibItem.Find(_T('@'))+1);

				// Construit l'ItemWrapper associé
				CItemWrapper * item = new CItemWrapper();
				item->SetItemConstruit(pItemConstruit);

				// Entete de l'item  (correspond au nom de la question)
				item->SetHeader(HeaderStr);

				// LI bellé de l'item
				item->SetLabel(LabelStr);

				// Ajoute à la liste
				m_AllItemsAutos.AddTail() = item;
			}
			else
			{
				AfxMessageBox("Problème séparateur libellé item quantitatif");
			}
		}

		// Revient sur l'instance items utilisateur
		CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);

		// Récup instance des items construits utilisateur
		CItemsConstruitsBase		* pItemsConstruits			= CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Fabrique liste des ItemsWrappers Items Utilisateurs Quantis à prendre en compte
		m_AllItemsUtils.Reset();

		// Balaye la liste des items sélectionnés quantis automatiques
		for (pLstItemMoyUtil->MoveFirst(); pLstItemMoyUtil->IsValid(); pLstItemMoyUtil->MoveNext())
		{
			// Ident item quanti
			JUnt32 IdItem = pLstItemMoyUtil->GetItem();

			// Construction de l'item construit correspondant
			IItem * pIItem = pItemsConstruits->GetIItemByID(IdItem);
			CItemConstruit *pItemConstruit = (CItemConstruit *)(pIItem->AsItemConstruit()->AsItemConstruit());

			// On indique que cet item sera moyennable
			pItemConstruit->SetMoyennable(true);

			// Construit l'ItemWrapper associé
			CItemWrapper * item = new CItemWrapper();
			item->SetItemConstruit(pItemConstruit);

			/*
			CString LabelHeader = "Items Moy.";
			item->SetHeader(LabelHeader);
			*/
			CString TxtHeader = pItemConstruit->GetLabel().AsJCharPtr();
			item->SetHeader(TxtHeader);

			/*
			CString TxtLabel = "0..45"; 
			item->SetLabel(TxtLabel);
			*/

			// Intervalle de modalités
			CString IBString = pItemConstruit->GetIBString().AsJCharPtr();

			// Libellé afficher après le header (par défaut Tous)
			CString Label2 = "Tous";

			// Recherche de la chaine correspondant à l'intervalle de classe
			JUnt32 PosSup		 = IBString.Find(">=%");
			JUnt32 PosFinClasse1 = 0;
			if (PosSup)
				PosFinClasse1 = IBString.Find(",",PosSup);

			JUnt32 PosInf = IBString.Find("<=%");
			JUnt32 PosFinClasse2 = 0;
			if (PosInf)
				PosFinClasse2 = IBString.Find(",",PosInf);

			if (PosSup && PosFinClasse1 && PosInf && PosFinClasse2)
			{
				// Valeur intervalle borne inf
				CString StrInf = IBString.Mid(PosSup+3,PosFinClasse1-PosSup - 3);
				JInt32 Valeur1 = atoi(StrInf);
				if (Valeur1 < 0) Valeur1 = 0;

				// Valeur intervalle borne sup
				CString StrSup = IBString.Mid(PosInf+3,PosFinClasse2-PosInf - 3);
				JInt32 Valeur2 = atoi(StrSup );

				// Compose l'intervalle correspondant (exemple si age 15..99)
				Label2.Format("%d..%d",Valeur1, Valeur2);
			}
			item->SetLabel(Label2);

			// Ajoute à la liste
			m_AllItemsUtils.AddTail() = item;
		}

		// Mise à jour liste sélection
		UpdateListSelect(true);

		// Disposition liste résultat
		SetListeResultat(); 
	}
}