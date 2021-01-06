// DlgCible.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "DlgCible.h"
#include ".\dlgcible.h"
#include "SearchAlgo.h"
#include "ItemsConstruitsManager.h"

// Bo�te de dialogue CDlgCible

IMPLEMENT_DYNAMIC(CDlgCible, ETSLayoutDialog)
CDlgCible::CDlgCible(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDlgCible::IDD, pParent)
{
	// m_pItems				= CItemsConstruits::GetInstance();
	m_pItems				= CItemsConstruitsManager::GetInstance()->GetItemsInstance();
	m_pTerrainC				= NULL;
	m_pData					= NULL;
	m_lstHaut.m_pDlgParent	= this;
	m_lstBas.m_pDlgParent	= this;
	m_bGripper				= false;

	// R�cup�re la fen�tre parent au niveau du tree rubrique items
	m_Tree_Rubrique_Items.m_pDialog		= this;

	// R�cup�re la fen�tre parent au niveau du tree rubrique palettes
	m_Tree_Rubrique_Palettes.m_pDialog	= this;

	// R�cup�re la fen�tre parent au niveau du tree rubrique r�f affinit�
	m_Tree_Rubrique_RefAffs.m_pDialog	= this;

	// Pas d'�lmts en cours
	m_NbElmtLstBas = 0;

	// m_Tree_Rubrique_Palettes.m_pDialog = this;

	// Infos source et terrains
	m_IdSrcAudience = 0;
	m_LstIdTerrains.Reset();

	// Init Map des informations terrains interdits
	m_MapTerrainsInterdits.Reset(); 
}

CDlgCible::~CDlgCible()
{
	ResetMapParams();
	// m_pItems->Release();

}

///////////////////////////////////////////////////////////////////////////////////
// Positionne l'offre avec les �l�ments cible
void CDlgCible::SetData (CDialogChoixOffre* data)
{
	// R�cup�ration de l'offre
	m_pData = data;

	// Init map des terrains interdits
	m_MapTerrainsInterdits.Reset();
}

///////////////////////////////////////////////////////////////////////////////////
// R�cup�re l'offre en cours
CDialogChoixOffre * CDlgCible::GetData ()
{
	return m_pData;
}

bool CDlgCible::IsComplete()
{
	if(m_pData->m_offre.m_IdItemAffinite != 0 && m_pData->m_offre.m_TBLIdItems.GetCount() != 0)
		return true;
	else
		return false;
}

void CDlgCible::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FD_EDITSEARCH, m_searchStr);
	DDX_Control(pDX, IDC_FD_PROGRESS2, m_progress2);
}

void CDlgCible::ResetSize()
{
	CRect rect;

	m_ttlCibles.GetWindowRect(rect);

	m_ttlCibles.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_lstHaut.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	// m_Tree_Rubrique_Items.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER); 
	// m_Tree_Rubrique_Palettes.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER); 
	// m_Tree_Rubrique_RefAff.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER); 
	m_lstBas.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);

	m_btnItem.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnPalette.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnAffinite.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnRAZSearch.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnAfficheAll.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);

	GetDlgItem(IDC_FD_EDITSEARCH)->SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
}

BOOL CDlgCible::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	SetupControls();

	Init();
	m_lstHaut.EnableToolTips(true);
	m_lstBas.EnableToolTips(true);

	ResetSize();

	UpdateLayout(
		pane(VERTICAL)
					<<item(IDC_FD_STATIC_TTLCIBLES, ABSOLUTE_VERT)
					<<item(IDC_FD_LIST_HAUT)
					// <<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
					<<item(IDC_FD_SEP_TITLE1,ABSOLUTE_VERT) 
					<<item(IDC_FD_TREE_RUBRIC_ITEMS,ABSOLUTE_VERT) 
					<<item(IDC_FD_SEP_TITLE2,ABSOLUTE_VERT) 
					<<item(IDC_FD_LIST_BAS, RELATIVE_VERT,0, 60)
					<<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
					<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
										<<item(IDC_FD_BTN_RAZSEARCH , ABSOLUTE_VERT)
										<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
										)
					<<(pane(HORIZONTAL)	<<item(IDC_FD_BTNR1_ITEMS, ABSOLUTE_VERT)
										<<item(IDC_FD_BTNR2_PALETTES , ABSOLUTE_VERT)
										<<item(IDC_FD_BTNR3_AFFINITE, ABSOLUTE_VERT)
										)
		);

	return 	TRUE;
}

void CDlgCible::Init()
{
	for(m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem());
		m_Terrains.AddTail() = m_pData->m_OffreTerrainManager->GetTerrain(k);
	}
	CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_IdTerrainCiblage);
	m_pTerrainC = m_pData->m_OffreTerrainManager->GetTerrain(k);
	OnBnClickedBtnItems();
}


// Gestionnaires de messages CDlgCible
BEGIN_MESSAGE_MAP(CDlgCible, ETSLayoutDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_FD_BTNR1_ITEMS,			OnBnClickedBtnItems)
	ON_BN_CLICKED(IDC_FD_BTNR2_PALETTES,		OnBnClickedBtnPalettes)
	ON_BN_CLICKED(IDC_FD_BTNR3_AFFINITE,		OnBnClickedBtnAffinite)
	ON_LBN_SELCHANGE(IDC_FD_LIST_HAUT,			OnLbnSelchangeListHaut)
	ON_LBN_SELCHANGE(IDC_FD_LIST_BAS,			OnLbnSelchangeListBas)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_FD_EDITSEARCH,				OnEnChangeSearch)
	ON_BN_CLICKED(IDC_FD_BTN_RAZSEARCH,			OnBnClickedRazSearch)
	ON_BN_CLICKED(IDC_FD_BTN_LISTALL,			OnBnClickedBtnAll)
	ON_MESSAGE(WM_USER_ITEMLISTUPDATED,			OnUpdated_ItemList)
END_MESSAGE_MAP()

void CDlgCible::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas ETSLayoutFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(m_pData->m_appColor));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}

void CDlgCible::OnShowWindow(BOOL bShow, UINT nStatus)
{
	SetLayoutElmt(bShow, nStatus);

	if(bShow)
	{
		m_Terrains.Reset();
		for(m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem());
			m_Terrains.AddTail() = m_pData->m_OffreTerrainManager->GetTerrain(k);
		}
		CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_IdTerrainCiblage);
		m_pTerrainC = m_pData->m_OffreTerrainManager->GetTerrain(k);

		// Recharge les items valides quand la source ou les terrains changent
		SetRubriqueItems();

		// Mise � jour liste des items visibles
		m_Tree_Rubrique_Items.UpdateItemList();

		switch(m_internalMode)
		{
			case ITEM:
				/*
				FillResultListItem();
				FillListItem();
				*/
				FillListItem();
				FillResultListItem();
				break;

			case PALETTE:
				FillListPalette();
				FillResultListPalette();
				break;

			case AFFINITE:
				FillListRefAff();
				FillResultListAffinite();
				break;

			default:
				break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des contr�les selon l'�l�ment en cours (soit ITEMS, ou PALETTES, ou REF AFF)
void CDlgCible::SetLayoutElmt(BOOL bShow, UINT nStatus)
{
	ETSLayoutDialog::OnShowWindow(bShow, nStatus);
	ResetSize();

	switch(m_internalMode)
	{
		case ITEM:
		{
			UpdateLayout(
				pane(VERTICAL)
							<<item(IDC_FD_STATIC_TTLCIBLES, ABSOLUTE_VERT)
							<<item(IDC_FD_LIST_HAUT)
							// <<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE1,ABSOLUTE_VERT) 
							<<item(IDC_FD_TREE_RUBRIC_ITEMS,ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE2,ABSOLUTE_VERT) 
							<<item(IDC_FD_LIST_BAS, RELATIVE_VERT,0, 60)
							<<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_RAZSEARCH , ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
												)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_BTNR1_ITEMS, ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR2_PALETTES , ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR3_AFFINITE, ABSOLUTE_VERT)
												)
			);
		}
		break;

		case PALETTE:
		{
			UpdateLayout(
				pane(VERTICAL)
							<<item(IDC_FD_STATIC_TTLCIBLES, ABSOLUTE_VERT)
							<<item(IDC_FD_LIST_HAUT)
							// <<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE1,ABSOLUTE_VERT) 
							<<item(IDC_FD_TREE_RUBRIC_PALETTES, ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE2,ABSOLUTE_VERT) 
							<<item(IDC_FD_LIST_BAS, RELATIVE_VERT,0, 60)
							<<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_RAZSEARCH , ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
												)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_BTNR1_ITEMS, ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR2_PALETTES , ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR3_AFFINITE, ABSOLUTE_VERT)
												)
			);
		}
		break;

		case AFFINITE:
		{
			UpdateLayout(
				pane(VERTICAL)
							<<item(IDC_FD_STATIC_TTLCIBLES, ABSOLUTE_VERT)
							<<item(IDC_FD_LIST_HAUT)
							// <<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE1,ABSOLUTE_VERT) 
							<<item(IDC_FD_TREE_RUBRIC_REFAFFS,ABSOLUTE_VERT)
							<<item(IDC_FD_SEP_TITLE2,ABSOLUTE_VERT) 
							<<item(IDC_FD_LIST_BAS, RELATIVE_VERT,0, 60)
							<<item(IDC_FD_PROGRESS2, ABSOLUTE_VERT)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_RAZSEARCH , ABSOLUTE_VERT)
												<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
												)
							<<(pane(HORIZONTAL)	<<item(IDC_FD_BTNR1_ITEMS, ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR2_PALETTES , ABSOLUTE_VERT)
												<<item(IDC_FD_BTNR3_AFFINITE, ABSOLUTE_VERT)
												)
			);
		}
		break;

		default:
			break;
	}
	UpdateLayout();
}

////////////////////////////////////////////////////////////////////////////////////////////
// D�finition des controles 
void CDlgCible::SetupControls()
{
//subclass 

	m_ttlCibles				.SubclassDlgItem(IDC_FD_STATIC_TTLCIBLES,		this);
	m_lstHaut				.SubclassDlgItem(IDC_FD_LIST_HAUT,				this);
	m_Tree_Rubrique_Items	.SubclassDlgItem(IDC_FD_TREE_RUBRIC_ITEMS,		this);
	m_Tree_Rubrique_Palettes.SubclassDlgItem(IDC_FD_TREE_RUBRIC_PALETTES,	this);
	m_Tree_Rubrique_RefAffs	.SubclassDlgItem(IDC_FD_TREE_RUBRIC_REFAFFS,	this);
	m_lstBas				.SubclassDlgItem(IDC_FD_LIST_BAS,				this);
	m_btnItem				.SubclassDlgItem(IDC_FD_BTNR1_ITEMS,			this);
	m_btnPalette			.SubclassDlgItem(IDC_FD_BTNR2_PALETTES,			this);
	m_btnAffinite			.SubclassDlgItem(IDC_FD_BTNR3_AFFINITE,			this);
	m_btnRAZSearch			.SubclassDlgItem(IDC_FD_BTN_RAZSEARCH,			this);
	m_btnAfficheAll			.SubclassDlgItem(IDC_FD_BTN_LISTALL,			this);

	//	m_btnSeuils				.SubclassDlgItem(IDC_FD_BTN_SEUILS, this);
	//  m_btnInfoSeuils			.SubclassDlgItem(IDC_FD_BTN_INFO_SEUILS,		this);

	// Font titre de la fen�tre
	m_ttlCibles.SetFontName(CATPColors::GetFontLabelName());
	m_ttlCibles.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlCibles.SetBorder(false);

	// D�finition des couleurs contr�les
	SetColors();
}

void CDlgCible::SetColors()
{
	m_ttlCibles.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlCibles.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));

	m_lstHaut.SetSelectionColor( NORMAL, CATPColors::GetColorWhite() );
	m_lstHaut.SetAttribueColor( CATPColors::GetColorRed() );
	m_lstHaut.SetSelectedTextColor( ::GetSysColor(COLOR_WINDOWTEXT));

	// m_lstBas.SetSelectionColor( NORMAL, CATPColors::GetColorWhite() );
	m_lstBas.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(m_pData->m_appColor));
	m_lstBas.SetAttribueColor( CATPColors::GetColorRed() );
	// m_lstBas.SetSelectedTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );
	
	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_IN,			CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_OUT,			CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,		CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,		CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnAffinite.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAffinite.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAffinite.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	//m_btnSeuils.SetColor( CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pData->m_appColor));
	//m_btnSeuils.SetColor( CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pData->m_appColor));
	//m_btnSeuils.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pData->m_appColor));


	m_progress2.SetBarColour(CATPColors::GetColorMedium(m_pData->m_appColor));

}

void CDlgCible::OnBnClickedBtnItems()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_ITEM);
	m_ttlCibles.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_ITEMS, IDC_FD_BTNR3_AFFINITE, IDC_FD_BTNR1_ITEMS);
	m_internalMode = ITEM;
	m_searchStr = "";
	UpdateData(FALSE);

	// Affiche liste des rubriques items
	if (m_Tree_Rubrique_Items.GetCount() == 0)
		SetRubriqueItems();

	FillListItem();
	FillResultListItem();

	// Visibilit� des arbres
	m_Tree_Rubrique_Items.ShowWindow(SW_SHOW); 
	m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
	m_Tree_Rubrique_RefAffs.ShowWindow(SW_HIDE);

	// Mise en place Tree Items
	// SetLayoutElmt(false, 0);

	// Valide la liste du bas
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 

	// Valide la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection des palettes
void CDlgCible::OnBnClickedBtnPalettes()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_PALETTES);
	m_ttlCibles.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_ITEMS, IDC_FD_BTNR3_AFFINITE, IDC_FD_BTNR2_PALETTES);
	m_internalMode = PALETTE;
	m_searchStr = "";
	UpdateData(FALSE);

	// Affiche liste des rubriques palettes
	if (m_Tree_Rubrique_Palettes.GetCount() == 0)
		SetRubriquePalettes();

	FillListPalette();
	FillResultListPalette();

	// Visibilit� des arbres
	m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
	m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW);
	m_Tree_Rubrique_RefAffs.ShowWindow(SW_HIDE);

	// Mise en place Tree Palettes
	SetLayoutElmt(false, 0);

	// Valide la liste du bas
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 

	// Valide la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection de la cible affinit�
void CDlgCible::OnBnClickedBtnAffinite()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_ITEMAFF);
	m_ttlCibles.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_ITEMS, IDC_FD_BTNR3_AFFINITE, IDC_FD_BTNR3_AFFINITE);
	m_internalMode = AFFINITE;
	m_searchStr = "";
	UpdateData(FALSE);

	// Affiche liste des rubriques refs affinit�
	if (m_Tree_Rubrique_RefAffs.GetCount() == 0)
		SetRubriqueRefAffs();

	FillListRefAff();
	FillResultListAffinite();

	// Visibilit� des arbres
	m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
	m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
	m_Tree_Rubrique_RefAffs.ShowWindow(SW_SHOW);

	// Mise en place Tree Ref Affs
	SetLayoutElmt(false, 0);

	// Valide la liste du bas
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 

	// Valide la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modification �lmts s�lectionn�s
void CDlgCible::OnLbnSelchangeListHaut()
{
	//on suppr
	JInt32 id = m_lstHaut.GetSelectedID();
	if (id != LB_ERR)
	{
		switch(m_internalMode)
		{
		case ITEM:
			RemoveID(id, m_pData->m_offre.m_TBLIdItems);
			FillResultListItem();
			m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLES;
			break;

		case PALETTE:
			RemoveID(id, m_pData->m_offre.m_TBLIdItems);
			FillResultListPalette();
			m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLES;
			break;

		case AFFINITE:
			m_pData->m_offre.m_IdItemAffinite = 0;
			FillResultListAffinite();
			m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLE_AFFINITE;
			break;
		default:
			break;
		}
		m_pData->m_bModif = true;
		m_pData->m_preselcible = false;
	}
	m_lstHaut.SetCurSel(LB_ERR);
	m_pData->EnableOK();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Liste des �lmts � s�lectionner
void CDlgCible::OnLbnSelchangeListBas()
{
	// Attention ??? on peut passer � cet endroit avec une liste vide et non d�fini
	if (m_NbElmtLstBas == 0) return;
	
	// Voir si �lmt s�lectionn�
	JList <JInt32> selIdlst;
	JInt32 SelID = m_lstBas.GetSelectedID();

	// Index s�lection liste
	int IndexLst  =0;
	for (JUnt32 InxLst = 0; InxLst < m_lstBas.GetCount(); InxLst++)
	{
		// R�cup �lmt item
		IItem* item = (IItem*)m_lstBas.GetItemData(InxLst);

		// Ident �lmt courant
		JUnt32 IdItem = item->GetID();
		if (IdItem  == SelID)
		{
			IndexLst = InxLst;
			break;
		}	
	}
	 
	// Si �lmt s�lectionn�, on traite la s�lection selon le mode items, ou palettes, ou r�f affinit�
	if (SelID != LB_ERR)
	{
		switch(m_internalMode)
		{
			case ITEM:
				//////////////////////////////////////////// Cas d'un item cible ///////////////////////////////////////
				// 
				// Si au moins 1 item
				if(m_mapItemParams.GetCount())
				{
					// On se place sur l'info item (sur tous les terrains)
					m_mapItemParams.MoveTo(SelID);

					// Voir si item valide
					if (m_mapItemParams.IsValid())
					{
						// Si item simple interdit, on interdit sa s�lection
						JBool MessageAffich = false;
						if (ItemSimpleInterdit(SelID, IndexLst, MessageAffich) == true) 	
						{
							// Cible non s�lectionnable
							break;
						}	

						// Si aucun message affich� auparavant
						if (!MessageAffich)
						{
							// Affiche les cibles avec seuils d'avertissement non atteints pour certains terrains
							AvertissementItemSimple(SelID, IndexLst);
						}
					}
					else
					{
						AfxMessageBox("Item non valide");
					}
				}

				// S�lection liste bas courante
				m_lstBas.GetSelectedID(selIdlst);

				// Parcours tous les �lmts de la liste du bas, pour voir �tat s�lection de chaque item		
				for (JUnt32 InxLst = 0; InxLst < m_lstBas.GetCount(); InxLst++)
				{
					// R�cup �lmt item
					IItem* item = (IItem*)m_lstBas.GetItemData(InxLst);

					// Ident �lmt courant
					JUnt32 IdItem = item->GetID();

					// Voir �tat s�lection
					if (m_lstBas.GetSel(InxLst))
					{
						// D�j� s�lectionn� ou pas
						JBool DejaSelect = false;
						for (m_pData->m_offre.m_TBLIdItems.MoveFirst();
							m_pData->m_offre.m_TBLIdItems.IsValid();
							m_pData->m_offre.m_TBLIdItems.MoveNext())
						{
							// Ident 
							JInt32 Id = m_pData->m_offre.m_TBLIdItems.GetItem();
							
							if (Id ==  IdItem)
							{
								DejaSelect = true;
								break;
							}
						}
						// Si non d�j� s�lectionn� on le rajoute
						if (!DejaSelect) 
							m_pData->m_offre.m_TBLIdItems.AddTail() = IdItem;
					}
					else
					{
						// on supprime la s�lection si existe dans s�lection en cours
						for (m_pData->m_offre.m_TBLIdItems.MoveFirst();
							m_pData->m_offre.m_TBLIdItems.IsValid();
							m_pData->m_offre.m_TBLIdItems.MoveNext())
						{
							// Ident 
							JInt32 Id = m_pData->m_offre.m_TBLIdItems.GetItem();
							
							if (Id ==  IdItem)
							{
								m_pData->m_offre.m_TBLIdItems.Remove(1);
								break;
							}
						}
					}
				}

				FillResultListItem();
				m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLES;
				break;

			case PALETTE:
				//////////////////////////////////////////// Cas d'un item palette ///////////////////////////////////////
				// 
				// Si au moins 1 �lmt
				if(m_mapItemParams.GetCount())
				{
					// Est-ce une palette ???
					CPalettes * palette = m_pItems->GetIItemByID(SelID)->AsPalette();
					if(palette)
					{
						// R�cup�re liste d'�lmt de la palette
						JList<JInt32> idList;
						palette->GetItems(idList);

						// Peut-on utiliser cette palette
						if (DegreInterdit(SelID) == Item_Interdit_Total)
						{
							// Texte info �lmt non autoris�
							CString LblItemInterdit = "Items non d�finis pour les terrains ci-dessous\n\n";
							
							// Boucle sur les infos seuils terrains
							JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

							// R�cup le source
							const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pData->m_offre.m_IdSourceAudience);

							// Balaye tous les titres pour indiqu� ceux interdit
							for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
							{
								m_mapItemParams.MoveTo(idList.GetItem());
								if (m_mapItemParams.IsValid())
								{
									// R�cup�re le composant item
									IItem * item = m_pItems->GetIItemByID( idList.GetItem() );

									// Ident du composant
									JUnt32 IdItem = item->GetID(); 

									// R�cup�re infos terrains interdit pour cet item
									m_MapTerrainsInterdits.MoveTo(IdItem);

									// Ajoute info map item
									if (m_MapTerrainsInterdits.IsValid())
									{
										// Recup infos map terrains invalides 
										JMap <JUnt32, JBool> & MapTerrains = m_MapTerrainsInterdits.GetItem();

										// Balaye tous les terrains, et indique les non autorisations sur certains terrains
										for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); 
											m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); 
											m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
										{
											// Ident terrain en cours
											JUnt32 IdTerrain		  = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
											const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience, IdTerrain);

											// Voir si pour ce terrain, l'item est invalide
											MapTerrains.MoveTo(IdTerrain);

											if (MapTerrains.IsValid()) 
											{
												// On se positionne sur les infos items pour ce terrain
												m_mapItemParams.GetItem().MoveTo(IdTerrain); 

												// Seuil r�dhibitoire terrain
												CString TxtSeuilTerrain;
												TxtSeuilTerrain.Format("%s %d %s","(seuil = ",pTerrain->m_SeuilRedhibitoire,")"); 

												// Nombre de cas sur ce terrain
												CString TxtNbCas;
												TxtNbCas.Format("%s %d %s %s - %s %s","Nb cas =", m_mapItemParams.GetItem().GetItem()->GetNbCas(),"sur terrain ", ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr(), TxtSeuilTerrain);
												LblItemInterdit.Append(TxtNbCas);
												LblItemInterdit += "\n";
											}
										}
									}
								}
							}

							// Affichage message des items/palette interdit
							MessageBox(LblItemInterdit, "INFORMATION SEUILS", MB_ICONINFORMATION | MB_OK); 

							// D�selectionne l'item
							m_lstBas.SetSel(IndexLst, FALSE);

							break;
						}
					}
				}

				// S�lection liste bas courante
				m_lstBas.GetSelectedID(selIdlst);

				// Parcours tous les �lmts de la liste du bas, pour voir �tat s�lection de chaque item		
				for (JUnt32 InxLst = 0; InxLst < m_lstBas.GetCount(); InxLst++)
				{
					// R�cup �lmt item
					IItem* item = (IItem*)m_lstBas.GetItemData(InxLst);

					// Ident �lmt courant
					JUnt32 IdItem = item->GetID();

					// Voir �tat s�lection
					if (m_lstBas.GetSel(InxLst))
					{
						// D�j� s�lectionn� ou pas
						JBool DejaSelect = false;
						for (m_pData->m_offre.m_TBLIdItems.MoveFirst();
							m_pData->m_offre.m_TBLIdItems.IsValid();
							m_pData->m_offre.m_TBLIdItems.MoveNext())
						{
							// Ident 
							JInt32 Id = m_pData->m_offre.m_TBLIdItems.GetItem();
							
							if (Id ==  IdItem)
							{
								DejaSelect = true;
								break;
							}
						}
						// Si non d�j� s�lectionn� on le rajoute
						if (!DejaSelect) 
							m_pData->m_offre.m_TBLIdItems.AddTail() = IdItem;
					}
					else
					{
						// on supprime la s�lection si existe dans s�lection en cours
						for (m_pData->m_offre.m_TBLIdItems.MoveFirst();
							m_pData->m_offre.m_TBLIdItems.IsValid();
							m_pData->m_offre.m_TBLIdItems.MoveNext())
						{
							// R�cup�re palette
							CPalettes * palette = m_pItems->GetPalette( m_pData->m_offre.m_TBLIdItems.GetItem() );
							if (palette)
							{
								// Ident 
								JInt32 Id = m_pData->m_offre.m_TBLIdItems.GetItem();
								
								if (Id ==  IdItem)
								{
									m_pData->m_offre.m_TBLIdItems.Remove(1);
									break;
								}
							}
						}
					}
				}

				FillResultListPalette();
				m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLES;
				break;

			case AFFINITE:
				//////////////////////////////////////////// Cas d'un item affinit� ////////////////////////////////////////
				// 
				// Existe t-il au moins 1 item
				if(m_mapItemParams.GetCount())
				{
					// On se positionne sur 'item
					m_mapItemParams.MoveTo(SelID);

					// est-il valide
					if (m_mapItemParams.IsValid())
					{
						// Si item simple interdit, on interdit sa s�lection
						JBool MessageAffich = false;
						if (ItemSimpleInterdit(SelID, IndexLst, MessageAffich) == true) 	
						{
							// Cible non s�lectionnable
							break;
						}	

						// Si aucun message affich� auparavant
						if (!MessageAffich)
						{
							// Affiche les cibles avec seuils d'avertissement non atteints pour certains terrains
							AvertissementItemSimple(SelID, IndexLst);
						}
					}
					else
					{
						AfxMessageBox("Item non valide");
					}
				}

				// Sauve item affinit� s�lectionn�
				m_pData->m_offre.m_IdItemAffinite = SelID;


				// Attention, liste affinit� avec 1 seul s�lection
				for (int i=0; i<m_lstBas.GetCount(); i++)
				{
					// R�cup �lmt item
					IItem* item = (IItem*)m_lstBas.GetItemData(i);

					// Ident �lmt courant
					JUnt32 IdItem = item->GetID();

					if (IdItem != SelID)
					{
						// Ce n'est pas l'�lmt derni�rement s�lectionn�
						m_lstBas.SetSel(i, FALSE);
					}
					else
					{
						// Dernier �lmt s�lectionn�, on le marque
						m_lstBas.SetSel(i, TRUE);
					}
				}
							
				// Mise � jour liste r�sultat s�lection affinit�
				FillResultListAffinite();
				m_pData->m_offre.m_Changes |= m_pData->m_offre.CIBLE_AFFINITE;
				break;

			default:
				break;
		}

		m_pData->m_bModif = true;
		m_pData->m_preselcible = false;
	}

	// m_lstBas.SetCurSel(LB_ERR);
	m_pData->EnableOK();

	// Valide la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi si item simple autoris� ou interdit (le flag MessageAffich est l� pour indiqu� si au moins 1 message a �t� affich�
JBool CDlgCible::ItemSimpleInterdit(JInt32 ID_Item, JUnt32 IndexLst, JBool & MessageAffich)
{
	// Pas d'affichage par d�faut
	MessageAffich = false;

	if (DegreInterdit(ID_Item) == Item_Interdit_Total || (DegreInterdit(ID_Item) == Item_Interdit_Partiel))
	{
		// On affiche les terrains sur lequel il est interdit
		IItem * item = m_pItems->GetIItemByID(ID_Item);

		// Init message des interdits (avec nom de l'item)
		CString LblItemInterdit = "";
		LblItemInterdit.Format("%s\n\n","ATTENTION, le nombre de cas de cet item �tant faible, il ne sera pas utilisable sur le(s) terrain(s) ci-dessous :");

		// R�cup�re infos terrains interdit pour cet item
		m_MapTerrainsInterdits.MoveTo(ID_Item);

		// Ajoute info map item
		if (m_MapTerrainsInterdits.IsValid())
		{
			// Recup infos map terrains invalides 
			JMap <JUnt32, JBool> & MapTerrains = m_MapTerrainsInterdits.GetItem();

			// Boucle sur les infos seuils terrains
			JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

			// R�cup le source
			const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pData->m_offre.m_IdSourceAudience);

			// Balaye tous les terrains, et indique les non autorisations sur certains terrains
			for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); 
				m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); 
				m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				// Ident terrain en cours
				JUnt32 IdTerrain		  = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience, IdTerrain);

				// Voir si pour ce terrain, l'item est invalide
				MapTerrains.MoveTo(IdTerrain);

				if (MapTerrains.IsValid()) 
				{
					// On se positionne sur les infos items pour ce terrain
					m_mapItemParams.GetItem().MoveTo(IdTerrain); 
					
					// Seuil terrain
					CString TxtSeuilTerrain;
					TxtSeuilTerrain.Format("%d%s",pTerrain->m_SeuilRedhibitoire,")"); 

					// Nombre de cas sur ce terrain
					CString TxtNbCas;
					TxtNbCas.Format("%s%s %s (%s%d %s %s","  - ",ptSrc->m_Libelle.AsJCharPtr(),pTerrain->m_Libelle.AsJCharPtr(),"Nb cas = ",m_mapItemParams.GetItem().GetItem()->GetNbCas()," alors que seuil r�dhibitoire =",TxtSeuilTerrain);
					LblItemInterdit.Append(TxtNbCas);
					LblItemInterdit += "\n";
				}
			}
		}
		
		// Affichage du message terrains interdits
		MessageBox(LblItemInterdit, "INFORMATION SEUILS", MB_ICONINFORMATION | MB_OK);
		MessageAffich = true;

		// Item interdit
		if (DegreInterdit(ID_Item) == Item_Interdit_Total)
		{
			// D�selectionne l'item
			m_lstBas.SetSel(IndexLst, FALSE);
			return true;
		}
		else
			// Signal uniquement que certains terrains sont interdits
			return false;
	}
	
	// Item autoris�
	return false;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi si avertissement nb cas cible en dessous seuil avertissement
void CDlgCible::AvertissementItemSimple(JInt32 ID_Item, JUnt32 IndexLst)
{
	// On affiche les terrains sur lequel le seuil d'avertissement n'est pas atteint
	IItem * item = m_pItems->GetIItemByID(ID_Item);

	// Init message des avertissements (avec nom de l'item)
	CString LblItemAvertissement = "";
	LblItemAvertissement.Format("%s\n\n","ATTENTION, le nombre de cas de cet item est inf�rieur au seuil d'avertissement sur le(s) terrain(s) ci-dessous :");

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// R�cup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pData->m_offre.m_IdSourceAudience);

	// Flag idiquant si au moins 1 terrain sur lequel la cible n'a pas atteint le seuil d'avertissement
	JBool OkAvertissement = false; 

	// Balaye tous les terrains, et indique les non autorisations sur certains terrains
	for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); 
		m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); 
		m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		// Ident terrain en cours
		JUnt32 IdTerrain		  = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
		const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience, IdTerrain);
				
		// On se positionne sur les infos items pour ce terrain
		m_mapItemParams.GetItem().MoveTo(IdTerrain); 
		if (m_mapItemParams.GetItem().IsValid())
		{
			// Test si nb cas < seuil d'avertissement
			if (m_mapItemParams.GetItem().GetItem()->GetNbCas() < pTerrain->m_SeuilAvertissement)
			{
				// Seuil avertissement terrain
				CString TxtSeuilTerrain;
				TxtSeuilTerrain.Format("%d%s",pTerrain->m_SeuilAvertissement,")"); 

				// Nombre de cas sur ce terrain
				CString TxtNbCas;
				TxtNbCas.Format("%s%s %s (%s%d %s %s","  - ",ptSrc->m_Libelle.AsJCharPtr(),pTerrain->m_Libelle.AsJCharPtr(),"Nb cas = ",m_mapItemParams.GetItem().GetItem()->GetNbCas()," alors que seuil d'avertissement =",TxtSeuilTerrain);
				LblItemAvertissement.Append(TxtNbCas);
				LblItemAvertissement += "\n";

				// Mise � jour du flag d'avertissement
				OkAvertissement = true;
			}
		}
	}

	if (OkAvertissement)
		// Affichage du message avertissements seuils terrains
		MessageBox(LblItemAvertissement, "INFORMATION SEUILS AVERTISSEMENT", MB_ICONINFORMATION | MB_OK);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des items
JVoid CDlgCible::FillListItem()
{
	m_lstBas.ResetContent();
	JList<const IItem *> lstItem;
	if (m_Terrains.GetCount())
	{
		m_pItems->GetItemsIndependant(lstItem, m_Terrains);
		//pour chaque item
		for (lstItem.MoveFirst(); lstItem.IsValid(); lstItem.MoveNext())
		{
			IItem* item = (IItem*)lstItem.GetItem();
			if ( item->IsRegistered() && CSearchAlgo::Contains(item->GetLabel().AsJCharPtr(), m_searchStr) )
			{
				if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
					&& !item->AsItemConstruit()->IsSouche(m_pData->m_offre.m_IdSourceAudience) )
					continue;
				else
				{
					// Ident de l'item
					JInt32 IdItem = item->GetID();
				
					// Il faut de plus que l'item soit visualisable (s�lectionn� dans une rubrique item)
					if (ShowItem(IdItem))
					{
						// Index et ajout nouvel �lmt 
						int IndexAdd = m_lstBas.AddItem(item);

						// S�lection par d�faut
						if (item->GetID() == m_pItems->GetDefaultItemId())
						{
							m_lstBas.SetCurSel(IndexAdd);
							// m_pData->m_offre.m_TBLIdItems.AddTail() = item->GetID();
						}
					}
				}
			}
		}
	}

	// Nombre d'�l�ments s�lectionnables
	m_NbElmtLstBas = m_lstBas.GetCount(); 

	// Marquage des cibles via les seuils
	CheckSeuils();

	// Mise � jour liste des elmts
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des palettes
void CDlgCible::FillListPalette()
{
	m_lstBas.ResetContent();
	JList<const IItem*> lstPalette;

	if (m_Terrains.GetCount())
	{
		m_pItems->GetPalettes(lstPalette, m_Terrains);
		//pour chaque palette
		for (lstPalette.MoveFirst(); lstPalette.IsValid(); lstPalette.MoveNext())
		{
			IItem* item = (IItem*)lstPalette.GetItem();

			if( CSearchAlgo::Contains(item->GetLabel().AsJCharPtr(), m_searchStr))
			{
				// Ident de la palette
				JInt32 IdPalette = item->GetID();

				// Il faut de plus que la palette soit visualisable (s�lectionn� dans une rubrique palette)
				if (ShowPalette(IdPalette))
				{
					// Ajoute � la liste des palettes s�lectionnables
					m_lstBas.AddItem(item);
				}

				/*
				// Ajoute � la liste des palettes s�lectionnables
				m_lstBas.AddItem(item);
				*/
			}
		}
	}

	// Nombre d'�l�ments s�lectionnables
	m_NbElmtLstBas = m_lstBas.GetCount(); 

	// Marquage des palettes via les seuils
	CheckSeuils();

	// Mise � jour liste des elmts
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des cibles affinit�
void CDlgCible::FillListRefAff()
{
	m_lstBas.ResetContent();
	JList<const IItem *> lstItem;

	if (m_Terrains.GetCount())
	{
		m_pItems->GetItemsIndependant(lstItem, m_Terrains);
		//pour chaque item
		for (lstItem.MoveFirst(); lstItem.IsValid(); lstItem.MoveNext())
		{
			IItem* item = (IItem*)lstItem.GetItem();
			if ( item->IsRegistered() && CSearchAlgo::Contains(item->GetLabel().AsJCharPtr(), m_searchStr) )
			{
				if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
					&& !item->AsItemConstruit()->IsSouche(m_pData->m_offre.m_IdSourceAudience) )
					continue;
				else
				{
					// Ident de l'item
					JInt32 IdItem = item->GetID();
				
					// Il faut de plus que l'item soit visualisable (s�lectionn� dans une rubrique item)
					if (ShowRefAff(IdItem))
					{
						// Index et ajout nouvel �lmt 
						int IndexAdd = m_lstBas.AddItem(item);

						// S�lection par d�faut
						if (item->GetID() == m_pItems->GetDefaultItemId())
						{
							m_lstBas.SetCurSel(IndexAdd);
							// m_pData->m_offre.m_TBLIdItems.AddTail() = item->GetID();
						}
					}
				}
			}
		}
	}

	// Nombre d'�l�ments s�lectionnables
	m_NbElmtLstBas = m_lstBas.GetCount(); 

	// Marquage des r�f affinit�s via les seuils
	CheckSeuils();

	// Mise � jour liste des elmts
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des cibles s�lectionn�es 
void CDlgCible::FillResultListItem()
{
	m_lstHaut.ResetContent();
	for( m_pData->m_offre.m_TBLIdItems.MoveFirst(); m_pData->m_offre.m_TBLIdItems.IsValid(); m_pData->m_offre.m_TBLIdItems.MoveNext() )
	{
		IItem * item = m_pItems->GetItemConstruit( m_pData->m_offre.m_TBLIdItems.GetItem() );
		if (item)
		{
			bool bValid = true;
			for(m_Terrains.MoveFirst(); m_Terrains.IsValid(); m_Terrains.MoveNext())
				if (!item->IsValid(m_Terrains.GetItem()))
					bValid = false;

			if ( bValid && item->IsRegistered()
				|| item->GetID() == m_pItems->GetDefaultItemId() )
			{
				m_lstHaut.AddItem(item);

			}
			else //l'item n'est plus valide pour le terrain de ciblage selectionn�
				m_pData->m_offre.m_TBLIdItems.Remove(-1);
		}
	}

	// Coordination s�lection avec liste de tous les items
	JList<JInt32> selIdlst;
	for(m_pData->m_offre.m_TBLIdItems.MoveFirst(); m_pData->m_offre.m_TBLIdItems.IsValid(); m_pData->m_offre.m_TBLIdItems.MoveNext() )
	{
		// R�cup�re ident des items s�lection en cours
		JInt32 IdItem = m_pData->m_offre.m_TBLIdItems.GetItem();
		selIdlst.AddTail() =  IdItem;
	}

	// On capture le topindex, pour remettre apr�s s�lection la liste dans son �tat initial
	m_lstBas.LockWindowUpdate(); 
	JUnt32 InxTopIndex = m_lstBas.GetTopIndex(); 

	// Mis � jour s�lection liste du bas
	m_lstBas.SetSelectedID(selIdlst);

	// Remet liste � sa position original
	m_lstBas.SetTopIndex(InxTopIndex); 
	m_lstBas.UnlockWindowUpdate(); 
	
	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 

	// Redessine la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des palettes s�lectionn�es
void CDlgCible::FillResultListPalette()
{
	m_lstHaut.ResetContent();

	int NbCount = m_pData->m_offre.m_TBLIdItems.GetCount();

	for( m_pData->m_offre.m_TBLIdItems.MoveFirst(); m_pData->m_offre.m_TBLIdItems.IsValid(); m_pData->m_offre.m_TBLIdItems.MoveNext() )
	{
		CPalettes * palette = m_pItems->GetPalette( m_pData->m_offre.m_TBLIdItems.GetItem() );
		if (palette)
		{
			bool bValid = true;
			for(m_Terrains.MoveFirst(); m_Terrains.IsValid(); m_Terrains.MoveNext())
				if (!palette->IsValid(m_Terrains.GetItem()))
					bValid = false;
			if (bValid)
				m_lstHaut.AddItem(palette);
			else //la palette n'est plus valide pour le terrain de ciblage selectionn�
				m_pData->m_offre.m_TBLIdItems.Remove(-1);
		}
	}

	// Coordination s�lection avec liste de tous les items
	JList<JInt32> selIdlst;
	for(m_pData->m_offre.m_TBLIdItems.MoveFirst(); m_pData->m_offre.m_TBLIdItems.IsValid(); m_pData->m_offre.m_TBLIdItems.MoveNext() )
	{
		CPalettes * palette = m_pItems->GetPalette( m_pData->m_offre.m_TBLIdItems.GetItem() );
		if (palette)
		{
			// R�cup�re ident des items s�lection en cours
			JInt32 IdItem = m_pData->m_offre.m_TBLIdItems.GetItem();
			selIdlst.AddTail() =  IdItem;
		}
	}

	// On capture le topindex, pour remettre apr�s s�lection la liste dans son �tat initial
	m_lstBas.LockWindowUpdate(); 
	JUnt32 InxTopIndex = m_lstBas.GetTopIndex(); 

	// Mis � jour s�lection liste du bas
	m_lstBas.SetSelectedID(selIdlst);

	// Remet liste � sa position original
	m_lstBas.SetTopIndex(InxTopIndex); 
	m_lstBas.UnlockWindowUpdate(); 


	m_lstBas.Invalidate(); 
	m_lstBas.RedrawWindow(); 

	// Redessine la liste du haut
	m_lstHaut.Invalidate(); 
	m_lstHaut.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des cibles affinit� s�lectionn�es
void CDlgCible::FillResultListAffinite()
{
	m_lstHaut.ResetContent();
	if (m_pData->m_offre.m_IdItemAffinite)
	{
		IItem * item = m_pItems->GetItemConstruit( m_pData->m_offre.m_IdItemAffinite );
		if (item)
		{
			bool bValid = true;
			for(m_Terrains.MoveFirst(); m_Terrains.IsValid(); m_Terrains.MoveNext())
				if (!item->IsValid(m_Terrains.GetItem()))
					bValid = false;

			if ( bValid && item->IsRegistered()
				|| item->GetID() == m_pItems->GetDefaultItemId() )
			{
					m_lstHaut.AddItem(item);
			}
			else //l'item n'est plus valide pour le terrain de ciblage selectionn�
				m_pData->m_offre.m_IdItemAffinite = 0;
		}
	}

	// Coordination s�lection avec liste de tous les items
	IItem * item = m_pItems->GetItemConstruit( m_pData->m_offre.m_IdItemAffinite);
	JList<JInt32> selIdlst;

	// Ajoute ident item si non existant
	if (item != NULL)
		selIdlst.AddTail() =  item->GetID();
	
	// Mis � jour s�lection liste du bas
	m_lstBas.SetSelectedID(selIdlst);
}

bool CDlgCible::IDExist( JUnt32 id, JList<JUnt32>& lst )
{
	for (lst.MoveFirst(); lst.IsValid(); lst.MoveNext() )
		if (lst.GetItem() == id )
			return true; 
	return false;
}
void CDlgCible::RemoveID( JUnt32 id , JList<JUnt32>& lst )
{
	for (lst.MoveFirst(); lst.IsValid(); lst.MoveNext() )
		if (lst.GetItem() == id )
			lst.Remove(); 
}

void CDlgCible::OnBnClickedRazSearch()
{
	m_searchStr = "";
	UpdateData(FALSE);
	switch(m_internalMode)
	{
		case ITEM:
			FillListItem();
			break;
		case PALETTE:
			FillListPalette();
			break;
		case AFFINITE:
			FillListRefAff();
			break;
	}
	GetDlgItem(IDC_FD_EDITSEARCH)->SetFocus();
}

void CDlgCible::OnEnChangeSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

void CDlgCible::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == IdTimer_Search)
	{
		KillTimer(IdTimer_Search);
		UpdateData();
		switch(m_internalMode)
		{
		case ITEM:
			FillListItem();
			break;
		case PALETTE:
			FillListPalette();
			break;
		case AFFINITE:
			FillListRefAff();
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgCible::OnBnClickedBtnAll()
{
	switch(m_internalMode)
	{
		case ITEM:
			FillListItem();
			break;
		case PALETTE:
			FillListPalette();
			break;
		case AFFINITE:
			FillListRefAff();
			break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Marquage des items selon seuil r�dhibitoire et seuil avertissement li�s aux terrains
///
/// Principe : ne pas autoriser certains cibles ou palettes
/// 
/// Pour les items
///					>> Code couleur (Orange : interdit sur certains terrains s�lectionn�s (la cible sera s�lectionnable, mais les r�sulats pour
///                                  les terrains non valides seront des *, Rouge : interdit sur tous les terrains s�lectionn�s (non s�lectionnable) 
///
/// Pour les palettes 
///					>> idem
///
void CDlgCible::CheckSeuils()
{
	//CListBox

	ResetMapParams();

	// Liste des items ind�pendants et des palettes
	JList<const IItem *> lstItem;
	if (m_Terrains.GetCount())
	{
		// R�cup�ration des items ind�pendants
		m_pItems->GetItemsIndependant(lstItem, m_Terrains);

		// R�cup�ration des palettes
		m_pItems->GetPalettes(lstItem, m_Terrains);
	}

	// Initilisation progress bar
	m_progress2.ShowWindow(SW_SHOW);
	m_progress2.SetRange(0, 100*lstItem.GetCount());
	int szStep = 100;
	m_progress2.SetStep(szStep);
	m_progress2.SetPos(0);

	// Nombre d'items en cours
	JInt32 NbItems = lstItem.GetCount(); 

	// Boucle sur tous les items existants
	for(lstItem.MoveFirst(); lstItem.IsValid(); lstItem.MoveNext())
	{

		if(lstItem.GetItem()->IsItemConstruit())
		{
			const CItemConstruit *item = lstItem.GetItem()->AsItemConstruit();
			AddItemParam(item);
			m_progress2.SetStep(szStep);
			m_progress2.StepIt();
		}

		if(lstItem.GetItem()->IsPalette())
		{
			CPalettes *palette = (CPalettes *)lstItem.GetItem()->AsPalette();
			JList<JInt32> idList;
			palette->GetItems(idList);

			// Attention si la palette est obsol�te, plus d'�l�ment valide (en fait il faudrait d�truire cette palette inutile)
			if (idList.GetCount() > 0)
			{
				m_progress2.SetStep(szStep/idList.GetCount());
			}

			for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
			{
				CItemConstruit * item = m_pItems->GetIItemByID( idList.GetItem() )->AsItemConstruit();
				AddItemParam(item);
				m_progress2.StepIt();
			}
		}
	}

//	m_btnSeuils.EnableWindow(false);
	m_progress2.ShowWindow(SW_HIDE);
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajout des informations seuils pour chaque items et palettes (en tenant compte de tous les terrains)
void CDlgCible::AddItemParam(const CItemConstruit *item)
{
	// Ident item en cours
	JUnt32 IdItem = item->GetID();

	// V�rifie que non d�j� existant
	m_mapItemParams.MoveTo(IdItem);
	if (!m_mapItemParams.IsValid()) 
	{	
		// Map des param�tres item pour chaque terrain
		JMap <JInt32, CCalcItemParams*> & MapParamsParTerrain = m_mapItemParams.Add(IdItem) ;

		// Boucle sur les infos seuils terrains
		JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

		// Boucle sur les terrains
		for (m_LstIdTerrains.MoveFirst(); m_LstIdTerrains.IsValid(); m_LstIdTerrains.MoveNext())
		{
			// R�cup l'info terrain
			JUnt32 IdTerrain = m_LstIdTerrains.GetItem();
			// const JTerrain * pTerrain	= pSrcTables->m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience, IdTerrain);

			CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, IdTerrain);
			CTerrain * pTerrain = m_pData->m_OffreTerrainManager->GetTerrain(k);

			if (pTerrain)
			{
				// Cr�er nouvelle instance param�trages pour calcul effectif cible 
				CCalcItemParams *Params = new CCalcItemParams();

				// Init des param�trages
				Params->SetTerrain(pTerrain);
				Params->SetEchantillonID(1);
				Params->SetUniversID(pTerrain->GetDefaultUnivers());
				Params->SetTypePoidsID( m_pData->m_offre.m_IdTypePoids);

				// Tenir compte de la population segment et positionner le param�tre
				CBitArray bArrPopSeg;
				JUnt32 contingent =  0xFFFF;
				pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent, true);    // en tenant compte des multi-segments
				Params->SetPopulationSegment(bArrPopSeg);

				// Set param segments
				Params->SetSegmentsIDs(m_pData->m_offre.m_TBLIdSegmentsAudience);

				// Calcul de l'effectif pour cet item
				item->CalculateEffectif(Params);

				// On se positionne sur les infos params du terrain
				MapParamsParTerrain.MoveTo(IdTerrain);
				if (!MapParamsParTerrain.IsValid())
				{
					// Ajoute les infos param�tres pour ce terrain
					MapParamsParTerrain.Add(IdTerrain) = Params; 	
				}
			}
		}
	}
}

// Initialise table infos param�tres de chaque item (et pour tous les terrains)
void CDlgCible::ResetMapParams()
{
	// Boucle sur les items
	for(m_mapItemParams.MoveFirst(); m_mapItemParams.IsValid(); m_mapItemParams.MoveNext())
	{
		// Boucle sur tous les terrains associ�s � l'item
		for (m_mapItemParams.GetItem().MoveFirst(); m_mapItemParams.GetItem().IsValid(); m_mapItemParams.GetItem().MoveNext())
		{
			delete m_mapItemParams.GetItem().GetItem();
		}
	}

	// Rest du map g�n�ral
	m_mapItemParams.Reset();
}

/*
// Si au moins 1 source et 1 terrain, cr�er texte source + terrains
	if (m_offre.m_IdSourceAudience != 0 && m_offre.m_TBLIdTerrainsAudience.GetCount() > 0)
	{
		// R�cup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// R�cup le source
		const JSource * ptSrc		= srcTables.m_TBLSources.GetItem(m_offre.m_IdSourceAudience);

		if (m_offre.m_TBLIdTerrainsAudience.GetCount() == 1)
		{
			// 1 seul terrain on recherche le nom du terrain
			m_offre.m_TBLIdTerrainsAudience.MoveFirst();
			if (m_offre.m_TBLIdTerrainsAudience.IsValid())
			{
				// Ident etrrain en cours
				JUnt32 TerID = m_offre.m_TBLIdTerrainsAudience.GetItem();
				const JTerrain * ptTer	= srcTables.m_TBLTerrains.GetItem(m_offre.m_IdSourceAudience,TerID);

				// Et on cr�e la chaine "source + terrain"
				TxtSourceTerrain.Format("- %s - %s", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr());m_sTerrainName
			}
			else
			{
				// Et on cr�e la chaine "source"
				TxtSourceTerrain.Format("- %s", ptSrc->m_Libelle.AsJCharPtr());
			}
		}
		else
		{
			// Plusieurs terrains, on affiche la source en indiquant +sieurs terrains
			TxtSourceTerrain.Format("- %s (%s)", ptSrc->m_Libelle.AsJCharPtr(), "Multi-terrains");
		}
	}
*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Affichage des seuils par enqu�tes s�lectionn�es
void CDlgCible::AffichSeuils()
{
	// Init du message
	CString TxtSeuil = "";

	// Cr�ation du message
	// TxtSeuil = "Informations seuils enqu�tes \n";

	// R�cup Table
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	// R�cup le source
	const JSource * ptSrc		= srcTables.m_TBLSources.GetItem(m_pData->m_offre.m_IdSourceAudience);

	// Boucle sur les terrains
	for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		// Ident etrrain en cours
		JUnt32 TerID			= m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
		const JTerrain * ptTer	= srcTables.m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience,TerID);

		// Et on cr�e la chaine "source + terrain" et les seuils
		CString TxtSourceTerrain;
		// TxtSourceTerrain.Format("%s %s\t - %s %d % s %s %d %s \n", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr(), "Seuil r�dhibitoire (", ptTer->m_SeuilRedhibitoire, ") / ", "Seuil d'avertissement (",  ptTer->m_SeuilAvertissement, ")");
		TxtSourceTerrain.Format("%s %s\t - %s = %d / %s = %d \n", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr(), "Seuil r�dhibitoire", ptTer->m_SeuilRedhibitoire,"Seuil d'avertissement", ptTer->m_SeuilAvertissement);

		// Mise � jour texte g�n�ral
		TxtSeuil += TxtSourceTerrain;
	}	

	// Affichage du message
	// ::AfxMessageBox(TxtSeuil, MB_ICONINFORMATION | MB_OK);
	MessageBox(TxtSeuil, "INFORMATION SEUILS ENQUETES", MB_ICONINFORMATION | MB_OK); 
}

//////////////////////////////////////////////
// Sets up rubriques items dispos
void CDlgCible::SetRubriqueItems()
{
	// Pour les rubriques items
	m_Tree_Rubrique_Items.SetTypeRubrique(m_Tree_Rubrique_Items.TYPE_RUBRIQUE_ITEM);

	// Set tree rubriques items
	// Chargement des rubriques elmts items 
	m_Tree_Rubrique_Items.LoadRubriquesItems(m_pItems, m_Terrains);

	// Init �tat s�lection rubrique
	m_Tree_Rubrique_Items.SetEtatData(&m_MapSelRubriqueItem);

	// Repositionne l'index rubriques items via derni�re s�lection
	m_Tree_Rubrique_Items.TreeViaSauvegarde();

	// Rend rubriques �ditables
    m_Tree_Rubrique_Items.EnableEdit(true);
	m_Tree_Rubrique_Items.EnableExpandAll(false);
}

///////////////////////////////////////////////////////
// Sets up rubriques des palettes dispos
void CDlgCible::SetRubriquePalettes()
{
	// Pour les rubriques palettes
	m_Tree_Rubrique_Palettes.SetTypeRubrique(m_Tree_Rubrique_Palettes.TYPE_RUBRIQUE_PALETTE);

	// Set tree rubriques palettes
	m_Tree_Rubrique_Palettes.LoadRubriquesItems(m_pItems, m_Terrains);
	m_Tree_Rubrique_Palettes.m_pItemsConstruits	= m_pItems; 

	// Init �tat s�lection rubrique
	m_Tree_Rubrique_Palettes.SetEtatData(&m_MapSelRubriquePalette);

	// Repositionne l'index rubriques items via derni�re s�lection
	m_Tree_Rubrique_Palettes.TreeViaSauvegarde();

	// Rend rubriques �ditables
    m_Tree_Rubrique_Palettes.EnableEdit(true);
	m_Tree_Rubrique_Palettes.EnableExpandAll(false);

	// Visibilit� des arbres
	m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
	m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW);
	m_Tree_Rubrique_RefAffs.ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////
// Sets up rubriques r�f�rences affinit� dispos
void CDlgCible::SetRubriqueRefAffs()
{
	// Pour les rubriques r�fs affinit�s
	m_Tree_Rubrique_RefAffs.SetTypeRubrique(m_Tree_Rubrique_RefAffs.TYPE_RUBRIQUE_ITEM);

	// Set tree rubriques RefAffs 
	m_Tree_Rubrique_RefAffs.LoadRubriquesItems(m_pItems, m_Terrains);

	// Init �tat s�lection rubrique
	m_Tree_Rubrique_RefAffs.SetEtatData(&m_MapSelRubriqueRefAff);

	// Repositionne l'index rubriques r�f aff via derni�re s�lection
	m_Tree_Rubrique_RefAffs.TreeViaSauvegarde();

	// Rend rubriques �ditables
    m_Tree_Rubrique_RefAffs.EnableEdit(true);
	m_Tree_Rubrique_RefAffs.EnableExpandAll(false);

	// Visibilit� des arbres
	m_Tree_Rubrique_Items.ShowWindow(SW_HIDE); 
	m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
	m_Tree_Rubrique_RefAffs.ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////////////
// Mise � jour de la liste des items (pour listes items, ou palette ou ref affinit�)
LRESULT CDlgCible::OnUpdated_ItemList(WPARAM wParam, LPARAM lParam)
{
	switch(m_internalMode)
	{
		case ITEM:
			FillListItem();
			FillResultListItem();
			break;

		case PALETTE:
			FillListPalette();
			FillResultListPalette();
			break;

		case AFFINITE:
			FillListRefAff();
			FillResultListAffinite();
			break;

		default:
			break;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Visualisation ou pas d'un item (via s�lection dans rubriques items)
bool CDlgCible::ShowItem(JInt32 IdItem)
{
	return m_Tree_Rubrique_Items.ShowItem(IdItem);
}

//////////////////////////////////////////////////////////////////////////////////////
// Visualisation ou pas d'une palette (via s�lection dans rubriques items)
bool CDlgCible::ShowPalette(JInt32 IdPalette)
{
	return m_Tree_Rubrique_Palettes.ShowItem(IdPalette);
}

//////////////////////////////////////////////////////////////////////////////////////
// Visualisation ou pas d'une r�f affinit� (via s�lection dans rubriques items)
bool CDlgCible::ShowRefAff(JInt32 IdItem)
{
	return m_Tree_Rubrique_RefAffs.ShowItem(IdItem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Avec en + liste des terrains interdits
JUnt32 CDlgCible::DegreInterdit(int ID)
{
	// Par d�faut pas d'interdiction
	JUnt32 DegreInterdit = Item_Interdit_Aucun;
	
	// Cas d'une palette
	if (m_pItems->GetIItemByID(ID)->IsPalette())
		DegreInterdit = DegrePaletteInterdit(ID);

	// Case d'un item
	else
		DegreInterdit = DegreItemInterdit(ID);

	return DegreInterdit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Idem mais en retournant en +, la liste des terrains interdits (par composants items)
JUnt32 CDlgCible::DegreItemInterdit(int ID)
{
	// Par d�faut pas d'interdiction
	JUnt32 DegreInterdit = Item_Interdit_Aucun;
	
	// On se positionne sur ma map item parametres
	m_mapItemParams.MoveTo(ID);
	if(m_mapItemParams.IsValid())
	{
		/*
		if (m_pDlgParent->m_mapItemParams.GetItem()->GetNbCas() < AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100))
		{
			DegreInterdit = Item_Interdit_Total;
		}
		*/
		// Init nombre d'interdiction par terrain
		JUnt32 NbInterdit = 0;

		// Boucle sur les infos seuils terrains
		JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

		for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst(); 
			 m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); 
			 m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			// Ident terrain en cours
			JUnt32 IdTerrain		  = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
			const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience, IdTerrain);

			// On se positionne sur les infos items pour ce terrain
			m_mapItemParams.GetItem().MoveTo(IdTerrain); 

			if (m_mapItemParams.GetItem().IsValid()) 
			{
				// Test nombre de cas item via les seuils du terrain
				JInt32 NbCasItem = m_mapItemParams.GetItem().GetItem()->GetNbCas();
				if (NbCasItem < pTerrain->m_SeuilRedhibitoire)
				{
					// Si 1er terrain
					m_MapTerrainsInterdits.MoveTo(ID);

					// Ajoute info map item
					if (!m_MapTerrainsInterdits.IsValid())
					{
						// Ajoute infos map terrains invalides 
						JMap <JUnt32, JBool> & MapTerrains = m_MapTerrainsInterdits.Add((JUnt32)ID);

						// Update liste des terrains interdits
						MapTerrains.MoveTo(IdTerrain);
						if (!MapTerrains.IsValid()) 
						{
							// Ajoute le terrain invalide
							MapTerrains.Add(IdTerrain) = true;
						}
					}
					else
					{
						// Recup infos map terrains invalides 
						JMap <JUnt32, JBool> & MapTerrains = m_MapTerrainsInterdits.GetItem();

						// Update liste des terrains interdits
						MapTerrains.MoveTo(IdTerrain);
						if (!MapTerrains.IsValid()) 
						{
							// Ajoute le terrain invalide
							MapTerrains.Add(IdTerrain) = true; 
						}
					}

					// Incr�mente le nombre de terrains interdits
					NbInterdit++; 
				}
			}
		}

		// Si nombre d'interdit = nombre de terrains, alors cet item est inutilisable
		if (NbInterdit == m_LstIdTerrains.GetCount())
			DegreInterdit = Item_Interdit_Total; 

		// Si au moins 1 interdit , alors cet item est partiellement utilisable
		else if (NbInterdit > 0)
			DegreInterdit = Item_Interdit_Partiel; 
	}

	// Renvoi le degr� d'interdiction de l'item
	return DegreInterdit;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Idem mais en retournant en +, la liste des terrains interdits (par composants items)
JUnt32 CDlgCible::DegrePaletteInterdit(int ID)
{
	// Par d�faut palette OK
	JUnt32 DegreInterdit = Item_Interdit_Aucun;

	// R�cup�re la palette
	CPalettes * palette = m_pItems->GetIItemByID(ID)->AsPalette();
	if(palette)
	{
		// Init le nombre d'interdictions partielles trouv�es et le nombre d'interdictions totales
		JUnt32 NbInterditPartiel	= 0;
		JUnt32 NbInterditTotal		= 0;

		// R�cup�re les composants items de la palette
		JList<JInt32> idList;
		palette->GetItems(idList);
		for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
		{
			// Test si item composant avec interdiction partielle
			if (DegreItemInterdit(idList.GetItem()) == Item_Interdit_Partiel)
			{
				NbInterditPartiel++;
			}
			// Test si item composant avec interdiction totale
			else if (DegreItemInterdit(idList.GetItem()) == Item_Interdit_Total)
			{
				NbInterditTotal++; 
			}
		}

		// Test si interdiction totale
		if (NbInterditTotal == idList.GetCount())
		{
			// Cas o� tous les items composants ont une interdiction totale
			DegreInterdit = Item_Interdit_Total;
		}

		// Test si interdiction partielle
		if (NbInterditPartiel > 0)
		{
			// Cas o� au moins 1 composant item a une interdiction totale
			DegreInterdit = Item_Interdit_Partiel;
		}
	}

	// Revoi degr� interdiction palette trouv�
	return DegreInterdit;
}