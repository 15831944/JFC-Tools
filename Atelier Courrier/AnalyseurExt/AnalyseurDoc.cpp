#include "StdAfx.h"
#include "analyseurdoc.h"
#include "DlgSurlignage.h"
#include "DlgAnalyseur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAnalyseurDoc::CAnalyseurDoc(CProgressCtrl & ProgressCtrl, CDlgAnalyseur & DlgAnalyseur, CDlgSurlignage & DlgSurlignage)
 : m_ProgressCtrl(ProgressCtrl), m_DlgAnalyseur(DlgAnalyseur), m_DlgSurlignage(DlgSurlignage)
{
	Set_TableStyle(TSTYLE_TERRAINCOL | TSTYLE_UNIVERSEX | TSTYLE_ORDONNERFORTE | TSTYLE_ORDONNERPAGE | TSTYLE_ITEMSX);

	m_ResultStyle = RSTYLE_RESULT;
	
	// Par défaut on calcule la marge d'erreur
	m_CalculMargeErreur = true;

	// Pas de forcage au calcul quanti
	m_ForceQuantiCalcul = true; //false;

	// Aout 2013 , Niche quantifiable ???
	m_NicheQuantifiable = false;

	// Cellule survolée
	m_NoCellSurvol = 0;

	Set_ClickMode(CMODE_TRI);
	m_Grandeurs.Reset();
	m_Grandeurs.AddTail() = RTYPE_EFFECTIVE_000;

	m_Calculation = 0;
	m_LastCalculation = 0;

	m_BaseColItem		=	0;
	m_BaseRowItem		=	0;

	m_RowIndex			=	0;
	m_ColIndex			=	1;
	m_PageIndex			=	2;

	m_CurPage			=	0;

	ResetSort();
	ResetSurlignage();

	m_SourceId			=	0;
	m_TypePoidsId		=	0;
	m_pSelectedCible	=	0;
	m_pTerrainTemoin	=	0;

	// Initialisation source et terrain de référence pour construction tris croisés
	m_SourceIdRef		=	0;
	m_TerrainIdRef		=   0;

	m_DocPath.Empty();
	m_DocTitle.Empty();
	m_NoVersion			=   0;

	Set_SelectedCible(0);

	m_RowHdrHeight[0]	=	16;
	m_RowHdrHeight[1]	=	16;
	m_ColHdrWidth[0]	=	160;
	m_ColHdrWidth[1]	=	160;

	// Récupération des seuils limite calcul
	m_SeuilAvert				= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Avertissement", 200);
	m_SeuilLimit				= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100);

	// Récupération des seuils marge d'erreur
	m_SeuilMargeErreurUtil		= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Util", 30);
	m_SeuilMargeErreurAppli		= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Appli", 75);

	m_bDirty = false;

	for(int x = 0; x < 3; ++x)
	{
		m_Dim[x].Reset();
		m_DType[x] = DTYPE_ITEMS;
	}

	// Init pointeurs sur les modes pupitres ligne, colonne, page
	m_pModeLigne	= 0;
	m_pModeColonne	= 0;
	m_pModePage		= 0;

	// Initialisation des sélections courantes pour le tri
	m_ModeTri						= MODE_LIGNE;				// Mode de tri courant (par défaut mode ligne)
	m_TypeTri						= TYPE_BASE;				// Type de tri courant (par défaut retour sans tri)
	m_IdTriGrandeurs				= RTYPE_EFFECTIVE_000;		// Ident courant grandeur pour tri (par défaut effectif en milliers)
	m_IdTriResultats				= RSTYLE_RESULT;			// Ident courant résultat pour tri (RSTYLE_RESULT, RSTYLE_HORIZONTAL, RSTYLE_VERTICAL, RSTYLE_GLOBAL)

	// Mode affichage résultat en ligne ou en 2x2  (par defaut en ligne)
	int ModeAffichResult = AfxGetApp()->GetProfileInt("Options\\Analyseur", "ModeAffichResult", AFFRESULT_LIGNE );
	SetModeAffichResult(ModeAffichResult);

	// Init Maps des cellules à surligner
	m_MapCellTotSurlign.Reset();

	// Init liste des opérations surlignage 
	m_Lst_Ope_Surlignages.Reset();

	// Init opération surlignage par défaut
	/*
	m_pDefault_OpeSurlignage->Param_Color		= RGB(255, 255, 0);
	m_pDefault_OpeSurlignage->Param_Grandeur	= RTYPE_EFFECTIVE_000;
	m_pDefault_OpeSurlignage->Param_Result		= RSTYLE_RESULT;
	m_pDefault_OpeSurlignage->TypeParam			= TYPE_SUP;
	m_pDefault_OpeSurlignage->Valeur1			= 0.0;
	m_pDefault_OpeSurlignage->Valeur2			= 0.0;
	*/

	// Mode moyenne / par défaut ce sera le calcul moyenne en mode ligne)
	m_ModeMoyenne = MOY_LIGNE;

	// Mode calcul moyenne (par défaut on tiendra compte des 0)
	m_ModeCalculMoyenne = CALC_MOY_0_INCLUS;

	// Les modes pour les grandeurs spécifiques (Effectif et Moyenne)
	m_TypeAffichSelEffectif = MODE_EFFECTIF_000; 
	m_TypeAffichSelMoyenne  = MODE_AVERAGEQUANTUM_P00;

	// La variable moyennable en mode tableau
	m_IdValMoyennable	= 0;
	m_pItemWrapperMoy	= 0;

	// Par défaut l'indice sera l'indice d'affinité
	m_ModeIndiceGlobal = RSTYLE_GBL_CORRELATION;

	// Init Flag indiquant si au moins 1 ligne moyennable
	m_AtLeastOneRowAverage = false;

	// A FAIRE
	m_ElmtGrandeurMoyennableSel = false;

	// Mode de decoupage par defaut  (0 sans découpage, 1 avec decoupage);
	m_ModeDecoupage = MODE_AVEC_DECOUPAGE;

	// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)
	// m_bCumulQuanti				= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
	m_bCumulQuanti				= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
}

////////////////////////////////////////////////////////////////////////////////
// Récupération des seuils
void CAnalyseurDoc::Set_Seuils()
{
	// Récupération des seuils limite calcul moteur
	m_SeuilAvert			= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Avertissement", 200);
    m_SeuilLimit			= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100);

	// Récupération des seuils marge d'erreur
	m_SeuilMargeErreurUtil	= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Util", 30);
	m_SeuilMargeErreurAppli	= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Appli", 75);
}

void CAnalyseurDoc::Reset()
{
	m_BaseColItem		=	0;
	m_BaseRowItem		=	0;
	m_RowIndex			=	0;
	m_ColIndex			=	1;
	m_PageIndex			=	2;
	m_CurPage			=	0;
	m_RowHdrHeight[0]	=	16;
	m_RowHdrHeight[1]	=	16;
	m_ColHdrWidth[0]	=	160;
	m_ColHdrWidth[1]	=	160;

	// Remove all dimensions
	for(int x = 0; x < 3; ++x)
	{
		m_Dim[x].Reset();
		m_DType[x] = DTYPE_ITEMS;
	}

	// Initialisation des items
	for(m_AllItems.MoveFirst(); m_AllItems.IsValid(); m_AllItems.MoveNext())
	{
		CItemWrapper *pItemWraper = m_AllItems.GetItem();
		delete m_AllItems.GetItem();
	}
	
	m_AllItems.Reset();

	// Initialisation des variables moyennables
	for(m_AllVarMoyennables.MoveFirst(); m_AllVarMoyennables.IsValid(); m_AllVarMoyennables.MoveNext())
	{
		CItemWrapper * pItemWraper = m_AllVarMoyennables.GetItem();
		delete m_AllVarMoyennables.GetItem();
	}
	m_AllVarMoyennables.Reset();

	// Initialise pointeur sur la variable moyennable sélectionnée
	m_pItemWrapperMoy = 0;

	ResetSort();
	ResetSurlignage();

	m_DocPath.Empty();
	m_DocTitle.Empty();
	m_NoVersion = 0;

	m_Calculation |= CALC_UNIVERSE;
	m_LastCalculation = m_Calculation;

	// Récupération des seuils limite calcul moteur
	m_SeuilAvert			= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Avertissement", 200);
	m_SeuilLimit			= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Limite", 100);

	// Récupération des seuils marge d'erreur
	m_SeuilMargeErreurUtil	= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Util", 30);
	m_SeuilMargeErreurAppli	= AfxGetApp()->GetProfileInt("Moteur", "Seuil_Marge_Erreur_Appli", 75);

	// Init pas de source et terrain référence pour nouveau dossier
	m_SourceIdRef		= 0;
	m_TerrainIdRef	= 0;

	m_bDirty = false;

	// Init Maps des cellules à surligner
	m_MapCellTotSurlign.Reset();

	// Init liste des opérations surlignage 
	m_Lst_Ope_Surlignages.Reset();

	// Init mode moyenne
	m_ModeMoyenne = MOY_LIGNE;

		
}

bool CAnalyseurDoc::IsDirty()
{
	if(m_bDirty)
	{
		if(GetColItems().GetCount() != 0 || GetRowItems().GetCount() != 0 || GetPageItems().GetCount() != 0 || GetAllVariablesMoyennables().GetCount() != 0)
			return true;
	}
	return false;
}

// Load the default profile settings
//void CAnalyseurDoc::SetDefaultSettings()
//{
//	CString Str =	"";
//	m_ListCibles.Reset();
//    m_TableStyle		=	(TSTYLE_TERRAINCOL | TSTYLE_UNIVERSEX | TSTYLE_ORDONNERFORTE | TSTYLE_ORDONNERPAGE | TSTYLE_ITEMSX);
//    m_ResultStyle		=	RSTYLE_RESULT;
//    m_ClickMode			=	CMODE_TRI;
//	m_Grandeurs.Reset();
//	m_Grandeurs.AddTail() = RTYPE_EFFECTIVE_000;
//	UINT SourceId = 1;
//	m_SourceId = 0;
//	UINT TypePoidsId = 1;
//	m_TypePoidsId = 0;
//	JList<UINT> SegmentsId;
//	m_SegmentsId.Reset();
//	SegmentsId.AddTail() = 1;
//	m_TerrainId.Reset();
//	m_pTerrainData.Reset() ;
//	LoadTerrains(SourceId,m_TerrainId,SegmentsId,TypePoidsId);
//	JList<JInt32> SelList;
//	if(m_pTerrainData.GetCount() != 0)
//	{
//		m_pTerrainData.MoveFirst();
//	    SelList.AddTail() = m_pTerrainData.GetItem()->m_TerrainId;
//	}
//	Set_SelectedTerrains(SelList); 
//}

bool CAnalyseurDoc::LoadTerrains(const JUnt32 SourceId, const JList<UINT> & TerrainIds, const JList<UINT> & Segments, const JUnt32 TypePoidsId)
{
	CWaitCursor Wait;
	bool bChanged = false;

	JMap<UINT, PTR_TERRAINDATA>		tmpTerrainData;
	JList<UINT>						tmpTerrainIds;
	JList<UINT>						tmpSegments;

	if(m_TypePoidsId != TypePoidsId)
	{
		m_TypePoidsId = TypePoidsId;
		bChanged = true;
	}

	for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
			tmpSegments.AddTail() = Segments.GetItem();

	if(m_SegmentsId.GetCount() == Segments.GetCount())
	{
		for(m_SegmentsId.MoveFirst(), Segments.MoveFirst(); m_SegmentsId.IsValid(); m_SegmentsId.MoveNext(), Segments.MoveNext())
		{
			if(m_SegmentsId.GetItem() != Segments.GetItem())
			{
				bChanged = true;
				break;
			}
		}
	}
	else
		bChanged = true;
	
	m_SegmentsId.Swap(tmpSegments);

	// Had the source changed ?
	if(bChanged || SourceId != m_SourceId)
	{
		m_SourceId = SourceId;
		// Yes so change all the terrains
		bChanged = true;
		if(TerrainIds.GetCount() == 0)
		{
			PTR_TERRAINDATA pTerrain = PTR_TERRAINDATA(new CTerrainData());
			if(pTerrain->LoadTerrain(m_SourceId,0,m_SegmentsId,m_TypePoidsId))
			{
				tmpTerrainIds.AddTail() = pTerrain->m_TerrainId; 
				tmpTerrainData.Add(pTerrain->m_TerrainId) = pTerrain;
			}
		}
		else
		{
			for(TerrainIds.MoveFirst(); TerrainIds.IsValid(); TerrainIds.MoveNext())
			{
				PTR_TERRAINDATA pTerrain = PTR_TERRAINDATA(new CTerrainData());
				if(!pTerrain->LoadTerrain(m_SourceId,TerrainIds.GetItem(),m_SegmentsId,m_TypePoidsId))
					continue;
				tmpTerrainIds.AddTail() = pTerrain->m_TerrainId; 
				tmpTerrainData.Add(pTerrain->m_TerrainId) = pTerrain;
			}
		}
	}
	else
	{
		// No so just verify the terrains loaded
		for(TerrainIds.MoveFirst(); TerrainIds.IsValid(); TerrainIds.MoveNext())
		{
			JInt32 TerrainId = TerrainIds.GetItem();
			m_pTerrainData.MoveTo(TerrainId);
			if(m_pTerrainData.IsValid())
			{
				tmpTerrainData.Add(TerrainId) = m_pTerrainData.GetItem(); 
				tmpTerrainIds.AddTail() = TerrainId;
			}
			else
			{
				PTR_TERRAINDATA pTerrain = PTR_TERRAINDATA(new CTerrainData());
				if(!pTerrain->LoadTerrain(m_SourceId,TerrainId,m_SegmentsId,m_TypePoidsId))
					continue;
				bChanged = true;
				tmpTerrainData.Add(TerrainId) = pTerrain;
				tmpTerrainIds.AddTail() = TerrainId;
			}
		}
		if(tmpTerrainData.GetCount() != m_pTerrainData.GetCount())
			bChanged = true;
	}

	m_TerrainId.Swap(tmpTerrainIds); 
	m_pTerrainData.Swap(tmpTerrainData); 

	m_pTerrainData.MoveFirst();
	if(m_pTerrainData.IsValid())
	{
		if (!m_pTerrainTemoin)
			m_pTerrainTemoin  = m_pTerrainData.GetItem()->m_pTerrain;
	}
	else
		m_pTerrainTemoin = 0;

	m_bDirty = bChanged;
	return bChanged;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Positionne source et terrain temoin par rapport à la source et au terrain de référence
void CAnalyseurDoc::Set_TerrainTemoin_ViaRef()
{
	// On vérifie déjà que la source et le terrain de référence sont actifs
	if (m_SourceIdRef != 0 && m_TerrainIdRef != 0)
	{
		// Construction d'un terrain
		PTR_TERRAINDATA pTerrain = PTR_TERRAINDATA(new CTerrainData());

		// Test si possible avec terrain et source de référence
		if(pTerrain->LoadTerrain(m_SourceIdRef,m_TerrainIdRef,m_SegmentsId,m_TypePoidsId))
			m_pTerrainTemoin  = pTerrain->m_pTerrain;
		else
			m_pTerrainTemoin = 0;
	}
	else
		// Pas de terrain témoin valide
		m_pTerrainTemoin = 0;
}

void CAnalyseurDoc::ResetSort()
{
	Set_SortType(false);
	Set_SortUniv(0);
	Set_SortCol(-1);
	Set_SortPage(-1);
	Set_SortCalc(RTYPE_EFFECTIVE_000);
	Set_SortedStyle(RSTYLE_NOSTYLE);
	Set_SortDirection(0);
	m_Calculation |= CALC_SORT; 
	m_bDirty = true;
}

void CAnalyseurDoc::ResetSurlignage()
{
	m_Grandeurs.MoveFirst();
	if(m_Grandeurs.IsValid())
	{
        Set_SurlignageCalc(m_Grandeurs.GetItem());
	}
	else
		Set_SurlignageStyle(RSTYLE_NOSTYLE);
	JVector<CTableKey> Dummy;
	Set_SurlignageList(Dummy,0); 
	m_Calculation |= CALC_SURLIGNAGE; 
	m_bDirty = true;
}

void CAnalyseurDoc::Set_ClickMode(UINT ClickMode)
{
	if(m_ClickMode == ClickMode)
		return;

	ResetSurlignage(); 

	m_ClickMode = ClickMode;

	if(ClickMode == CMODE_ORDONNER)
	{
		m_DlgSurlignage.ShowWindow(SW_SHOW);
	}
	else
	{
		m_DlgSurlignage.ShowWindow(SW_HIDE);
	}
}

// void CAnalyseurDoc::Set_SurlignageList(JVector<CTableKey> &List, IItemTable * pIItemTable)
void CAnalyseurDoc::Set_SurlignageList(JVector<CTableKey> &List, CCrossTable * pIItemTable)
{
	m_DlgSurlignage.SetData(List,pIItemTable, m_SurlignageCalc, m_SurlignageStyle, this);
}


CAnalyseurDoc::~CAnalyseurDoc(void)
{
	// Remove all items
	for(m_AllItems.MoveFirst(); m_AllItems.IsValid(); m_AllItems.MoveNext())
		delete m_AllItems.GetItem();

	// Remove all variables moyennables
	for(m_AllVarMoyennables.MoveFirst(); m_AllVarMoyennables.IsValid(); m_AllVarMoyennables.MoveNext())
		delete m_AllVarMoyennables.GetItem();

	// On n'oublie pas de virer le pointeur sur item variable moyennable sélectionnée
	m_pItemWrapperMoy = 0;
}

bool	CAnalyseurDoc::GetAllItems(LIST_ITEM_PUPITRE & All)
{
	LIST_ITEM_PUPITRE & Items = GetAllItems();
	All.Reset();
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		All.AddTail() = Items.GetItem();
	return true;
}

void	CAnalyseurDoc::Set_SurlignageStyle(RESULTSTYLE SurlignageStyle) 
{ 
	m_SurlignageStyle = SurlignageStyle; 
	m_Calculation |= CALC_SURLIGNAGE;
	 m_DlgSurlignage.UpdateTitre(); 
	 m_bDirty = true;
}

bool	CAnalyseurDoc::SetAllItems(LIST_ITEM_PUPITRE & All)
{
	LIST_ITEM_PUPITRE & Items = GetAllItems();
	Items.Reset();
	for(All.MoveFirst(); All.IsValid(); All.MoveNext())
		Items.AddTail() = All.GetItem();
	m_bDirty = true;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Positionnement des variables moyennables
bool	CAnalyseurDoc::SetAllVariablesMoyennables(LIST_ITEM_PUPITRE & All)
{
	LIST_ITEM_PUPITRE & Items = GetAllVariablesMoyennables();
	Items.Reset();
	for(All.MoveFirst(); All.IsValid(); All.MoveNext())
		Items.AddTail() = All.GetItem();
	m_bDirty = true;
	return true;
}

bool	CAnalyseurDoc::GetRowItems(CONST_LIST_ITEM_PUPITRE & Rows) const
{
	const CONST_LIST_ITEM_PUPITRE & Items = GetRowItems();
	Rows.Reset();
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		Rows.AddTail() = Items.GetItem();
	return true;
}

bool	CAnalyseurDoc::SetRowItems(CONST_LIST_ITEM_PUPITRE & Rows, int DType)
{
	m_DType[m_RowIndex]		= DType;

	// Mise à jour des items rows
	CONST_LIST_ITEM_PUPITRE & Items = m_Dim[m_RowIndex];
	Items.Reset();
	for(Rows.MoveFirst(); Rows.IsValid(); Rows.MoveNext())
	{
		const CItemWrapper *pItemWrapper = Rows.GetItem();
		

		Items.AddTail() = Rows.GetItem();
	}

	m_BaseColItem = 0;
	m_BaseRowItem = 0;

	m_Calculation |= CALC_ROWS;
	m_bDirty = true;
	return true;
}

bool	CAnalyseurDoc::GetColItems(CONST_LIST_ITEM_PUPITRE & Cols) const
{
	const CONST_LIST_ITEM_PUPITRE & Items = GetColItems();
	Cols.Reset();
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		Cols.AddTail() = Items.GetItem();
	return true;
}

bool	CAnalyseurDoc::SetColItems(CONST_LIST_ITEM_PUPITRE & Cols, int DType)
{
	m_DType[m_ColIndex]		= DType;
	CONST_LIST_ITEM_PUPITRE & Items = m_Dim[m_ColIndex];
	Items.Reset();
	ResetSort();
	ResetSurlignage();
	m_BaseColItem = 0;
	m_BaseRowItem = 0;

	for(Cols.MoveFirst(); Cols.IsValid(); Cols.MoveNext())
		Items.AddTail() = Cols.GetItem();

	m_Calculation |= CALC_COLS;
	m_bDirty = true;
	return true;
}

bool	CAnalyseurDoc::GetPageItems(CONST_LIST_ITEM_PUPITRE & Pages) const
{
	const CONST_LIST_ITEM_PUPITRE & Items = GetPageItems();
	Pages.Reset();
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		Pages.AddTail() = Items.GetItem();
	return true;
}

bool	CAnalyseurDoc::SetPageItems(CONST_LIST_ITEM_PUPITRE & Pages, int DType)
{
	m_DType[m_PageIndex]	= DType;
	CONST_LIST_ITEM_PUPITRE & Items = m_Dim[m_PageIndex];
	Items.Reset();
	for(Pages.MoveFirst(); Pages.IsValid(); Pages.MoveNext())
		Items.AddTail() = Pages.GetItem();

	m_Calculation |= CALC_PAGES;
	m_CurPage = 0;
	m_bDirty = true;
	return true;
}

// Adds the Grid results styles
void CAnalyseurDoc::Add_ResultStyle(UINT ResultStyle)
{
	Set_ResultStyle(m_ResultStyle | ResultStyle);
	m_bDirty = true;
}

void CAnalyseurDoc::Remove_ResultStyle(UINT ResultStyle)
{
	Set_ResultStyle(m_ResultStyle ^ ResultStyle);
	m_bDirty = true;
}

// Sets the Grid results styles
void CAnalyseurDoc::Set_ResultStyle(UINT ResultStyle)
{
	m_ResultStyle	= ResultStyle;

	m_Calculation |= CALC_COLSIZE;
	m_Calculation |= CALC_ROWSIZE;
	m_Calculation |= CALC_SORT; 
	m_Calculation |= CALC_GRIDUPDATE; 
	m_bDirty = true;
}

// Sets the Grid TableStyle styles
void CAnalyseurDoc::Add_TableStyle(UINT TableStyle)
{
	Set_TableStyle(m_TableStyle | TableStyle);
	m_bDirty = true;
}

void CAnalyseurDoc::Remove_TableStyle(UINT TableStyle)
{
	Set_TableStyle(m_TableStyle ^ TableStyle);
	m_bDirty = true;
}

void CAnalyseurDoc::Set_TableStyle(UINT TableStyle)
{
	unsigned int OldTableStyle		=	m_TableStyle;
	m_TableStyle					=	TableStyle;

	// If universe or niche changes to does the base 100
	if((OldTableStyle & (TSTYLE_UNIVERSEX|TSTYLE_NICHEX|TSTYLE_CONTRENICHEX|TSTYLE_ITEMMOYENX|TSTYLE_ITEMSX))!= (m_TableStyle & (TSTYLE_UNIVERSEX|TSTYLE_NICHEX|TSTYLE_CONTRENICHEX|TSTYLE_ITEMMOYENX|TSTYLE_ITEMSX)))
	{
		ResetSort();
		ResetSurlignage();
		m_BaseColItem = 0;
		m_BaseRowItem = 0;
		m_CurPage = 0;
	}

	if((OldTableStyle & (TSTYLE_EXCLUSIVE|TSTYLE_DUPLIQUE)) != (m_TableStyle & (TSTYLE_EXCLUSIVE|TSTYLE_DUPLIQUE)))
		m_Calculation |= CALC_CELLS;

	m_Calculation |= CALC_PAGESIZE;
	m_Calculation |= CALC_COLSIZE;
	m_Calculation |= CALC_ROWSIZE;
	m_Calculation |= CALC_SORT; 
	m_Calculation |= CALC_SURLIGNAGE; 
	m_Calculation |= CALC_GRIDUPDATE; 
	m_bDirty = true;
}

void	CAnalyseurDoc::Set_Grandeurs(const JList<RESULTTYPE> & Grandeurs)
{
	bool bResetSort = true;
	bool bResetSurlignage = true;

	m_Grandeurs.Reset();
	for(Grandeurs.MoveFirst (); Grandeurs.IsValid(); Grandeurs.MoveNext())
	{
		m_Grandeurs.AddTail() = Grandeurs.GetItem();
		if(Grandeurs.GetItem() == m_SortCalc)
			bResetSort =false;
		if(Grandeurs.GetItem() == m_SurlignageCalc)
			bResetSurlignage =false;
	}

	if(bResetSort)
		ResetSort();

	if(bResetSurlignage)
		ResetSurlignage();

	m_Calculation |= CALC_COLSIZE;
	m_Calculation |= CALC_ROWSIZE;
	m_Calculation |= CALC_SORT; 
	m_Calculation |= CALC_GRIDUPDATE; 
	m_bDirty = true;
}

// Renvoi true si le calcul de la moyenne est sélectionné
bool    CAnalyseurDoc::GrandeurMoyenneActive()
{
	// Balaye toutes les grandeurs sélectionnées
	for (m_Grandeurs.MoveFirst (); m_Grandeurs.IsValid(); m_Grandeurs.MoveNext())
	{
		const RESULTTYPE ResultType = m_Grandeurs.GetItem();
		if (ResultType == RTYPE_AVERAGEQUANTUM || ResultType == RTYPE_AVERAGEQUANTUM_P00)
			return true;
	}

	return false;
}

void	CAnalyseurDoc::Set_SelectedCible(const CItemConstruit * pSelectedCible)
{
	if(m_pSelectedCible != pSelectedCible)
	{
		m_pSelectedCible = pSelectedCible;
		m_Calculation |= CALC_CIBLES;
		m_Calculation |= CALC_CELLS;
		m_bDirty = true;
	}
}

// Adds the Grid Calculation styles
void CAnalyseurDoc::Add_Calculation(UINT Calculation)
{
	Set_Calculation(m_Calculation | Calculation);
	m_bDirty = true;
}

void	CAnalyseurDoc::Remove_Calculation(UINT Calculation)
{
	Set_Calculation(m_Calculation ^ Calculation);
	m_bDirty = true;
}

// Sets the Grid results styles
void CAnalyseurDoc::Set_Calculation(UINT Calculation)
{
	m_Calculation = Calculation;
	m_bDirty = true;
}

void CAnalyseurDoc::Set_UniverseParams(const JList<CCalcItemParams *> & UniverseParams)
{
	m_UniverseParams.Reset();
	for(UniverseParams.MoveFirst (); UniverseParams.IsValid(); UniverseParams.MoveNext())
		m_UniverseParams.AddTail() = UniverseParams.GetItem();

	ResetSort();
	ResetSurlignage();
	m_BaseColItem = 0;
	m_BaseRowItem = 0;
	m_Calculation |= CALC_UNIVERSE; 
	m_bDirty = true;
}

void	CAnalyseurDoc::Set_ListCibles(const JList<const CItemConstruit *> & ListCibles)
{
	m_ListCibles.Reset();
	for(ListCibles.MoveFirst (); ListCibles.IsValid(); ListCibles.MoveNext())
		m_ListCibles.AddTail() = ListCibles.GetItem();
	m_bDirty = true;
}

void	CAnalyseurDoc::Set_BaseColItem(int BaseColItem)
{
	if(m_BaseColItem != BaseColItem)
	{
		m_BaseColItem = BaseColItem;
		m_Calculation |= CALC_GRIDUPDATE; 
		m_bDirty = true;
	}
}

void	CAnalyseurDoc::Set_BaseRowItem(int BaseRowItem)
{
	if(m_BaseRowItem != BaseRowItem)
	{
		m_BaseRowItem = BaseRowItem;
		m_Calculation |= CALC_GRIDUPDATE; 
		m_bDirty = true;
	}
}

void	CAnalyseurDoc::Set_ListTerrainData(JMap<UINT, PTR_TERRAINDATA> & TerrainData)
{
	m_pTerrainData.Swap(TerrainData);

	m_bDirty = true;
}

void	CAnalyseurDoc::Set_SegmentsId(const JList<UINT> & SegmentsId)
{
	m_SegmentsId.Reset();
	for(SegmentsId.MoveFirst (); SegmentsId.IsValid(); SegmentsId.MoveNext())
		m_SegmentsId.AddTail() = SegmentsId.GetItem();

	m_bDirty = true;
}

void	CAnalyseurDoc::Set_TerrainId(const JList<UINT> & TerrainId)
{
	m_TerrainId.Reset();
	for(TerrainId.MoveFirst (); TerrainId.IsValid(); TerrainId.MoveNext())
		m_TerrainId.AddTail() = TerrainId.GetItem();

	m_bDirty = true;
}

// Transpose the rows with the columns
void    CAnalyseurDoc::TransposeRowCol()
{
	int Temp = m_RowIndex;
	m_RowIndex = m_ColIndex;
	m_ColIndex = Temp;

	Temp = m_BaseColItem;
	m_BaseColItem = m_BaseRowItem;
	m_BaseRowItem = Temp;

	ResetSort();

	m_Calculation |= CALC_TRANSPOSE;
	m_Calculation |= CALC_COLSIZE;
	m_Calculation |= CALC_ROWSIZE;
	m_Calculation |= CALC_SURLIGNAGE; 
	m_Calculation |= CALC_SORT; 
	m_Calculation |= CALC_GRIDUPDATE; 

	m_bDirty = true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde du document Tris Croises
//
// Attention, au delà de la version 6, on prend en compte les constructions du mode pupitre
// afin de sauvegarder les palettes lignes, colonnes et pages qui ont servies à construire
// les composants des tris croisés (Alain Avril 2006)
//
// A partir de la version 7, sauvegarde des opérations de surlignage
//
// A partir de la version 8, sauvegarde du mode moyenne (mode ligne ou mode tableau)
//							 + sauvegarde si mode tableau de la liste des variables moyennables et de la variable moyennable
// 
bool CAnalyseurDoc::SaveDoc(CStdioFile & FileOut)
{
	// Version n° 7 (à partir de la version 2.5)
	// int Version = 7;

	// Version n° 8 (à partir de la version 2.6)
	int Version = 8;

	// Write the file version number
	FileOut.Write(&Version,sizeof(Version));

	// Results to display
	int Count = m_Grandeurs.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	for(m_Grandeurs.MoveFirst(); m_Grandeurs.IsValid(); m_Grandeurs.MoveNext())
			FileOut.Write(&(m_Grandeurs.GetItem()),sizeof(RESULTTYPE));

	// Table display styles
	FileOut.Write(&m_TableStyle,sizeof(m_TableStyle));
	// Result styles
	FileOut.Write(&m_ResultStyle,sizeof(m_ResultStyle));
	// Left click mode
	FileOut.Write(&m_ClickMode,sizeof(m_ClickMode));
	// Calculation Flag
	FileOut.Write(&m_Calculation,sizeof(m_Calculation));
	// Source Id
	FileOut.Write(&m_SourceId,sizeof(m_SourceId));
	// Type poids Id
	FileOut.Write(&m_TypePoidsId,sizeof(m_TypePoidsId));

	// Segment Ids
	Count = m_SegmentsId.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	for(m_SegmentsId.MoveFirst(); m_SegmentsId.IsValid(); m_SegmentsId.MoveNext())
			FileOut.Write(&(m_SegmentsId.GetItem()),sizeof(UINT));

	// default terrain
	JInt32 TerrainId = 0;
	if(m_pTerrainTemoin)
		TerrainId = m_pTerrainTemoin->GetTerrainID();
	FileOut.Write(&TerrainId,sizeof(TerrainId));

	// Terrain Ids
	Count = m_TerrainId.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	for(m_TerrainId.MoveFirst(); m_TerrainId.IsValid(); m_TerrainId.MoveNext())
			FileOut.Write(&(m_TerrainId.GetItem()),sizeof(UINT));


	Count = m_SelectedTerrains.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	for(m_SelectedTerrains.MoveFirst(); m_SelectedTerrains.IsValid() ; m_SelectedTerrains.MoveNext())
	{
		JInt32 Id = m_SelectedTerrains.GetItem();
		FileOut.Write(&Id,sizeof(Id));
	}

	// Items export map
	JMap<JInt32,JBool> ExpMap;
	// List of targets
	Count = m_ListCibles.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	if(Count)
	{
		JList<JInt32> ListIds;
		for(m_ListCibles.MoveFirst(); m_ListCibles.IsValid(); m_ListCibles.MoveNext())
			ListIds.AddTail() = m_ListCibles.GetItem()->GetID();

		// Export the items
		// if(!Get_ItemsConstruits()->Export(FileOut, ListIds, ExpMap))
		if(!CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Export(FileOut, ListIds, ExpMap))
			return false;

		// List of targets
		JInt32 Id = 0;
		for(ListIds.MoveFirst(); ListIds.IsValid(); ListIds.MoveNext())
		{
			Id = ListIds.GetItem();
			FileOut.Write(&Id,sizeof(Id));
		}

		// Selected target Id
		if(m_pSelectedCible)
			Id = m_pSelectedCible->GetID();
		FileOut.Write(&Id,sizeof(Id));
	}

	// Dimension Row index
	FileOut.Write(&m_RowIndex,sizeof(m_RowIndex));
	// Dimension Col index
	FileOut.Write(&m_ColIndex,sizeof(m_ColIndex));
	// Dimension Page index
	FileOut.Write(&m_PageIndex,sizeof(m_PageIndex));

	// List of all Dimension Items  (les items "normaux" et les items "moyenne" / sélectionnés via la boite de sélection items moyennes)
	Count = m_AllItems.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	if(Count)
	{
		for(m_AllItems.MoveFirst(); m_AllItems.IsValid(); m_AllItems.MoveNext())
			m_AllItems.GetItem()->Write(FileOut,ExpMap);

		// List of Dimension Items by Row, Col and Page
		Count = 3;
		FileOut.Write(&Count,sizeof(Count));
		for(int x=0; x<3 ; ++x)
		{
			Count = m_Dim[x].GetCount();
			FileOut.Write(&Count,sizeof(Count));
			for(m_Dim[x].MoveFirst();m_Dim[x].IsValid();m_Dim[x].MoveNext())
			{
				JInt32 Id = m_Dim[x].GetItem()->PtrItem()->GetID();
				FileOut.Write(&Id,sizeof(Id));
			}
		}
	}

	// Base 100 Col Item
	FileOut.Write(&m_BaseColItem,sizeof(m_BaseColItem));
	// Base 100 Row Item
	FileOut.Write(&m_BaseRowItem,sizeof(m_BaseRowItem));
	// Current page
	FileOut.Write(&m_CurPage,sizeof(m_CurPage));
	// The sort universe
	FileOut.Write(&m_SortUniv,sizeof(m_SortUniv));
	// The sort column
	FileOut.Write(&m_SortCol,sizeof(m_SortCol));
	// The sort page
	FileOut.Write(&m_SortPage,sizeof(m_SortPage));
	// The sort calculation base
	FileOut.Write(&m_SortCalc,sizeof(m_SortCalc));
	// The sort direction
	FileOut.Write(&m_bSortDirection,sizeof(m_bSortDirection));
	// The sorted style
	FileOut.Write(&m_SortedStyle,sizeof(m_SortedStyle));
	// The Row Header Hight
	FileOut.Write(m_RowHdrHeight,sizeof(int)*2);
	// The Col Header Width
	FileOut.Write(m_ColHdrWidth,sizeof(int)*2);
	// The Sort Type
	FileOut.Write(&m_SortType,sizeof(m_SortType));

	// The surlignageg calculation base
	FileOut.Write(&m_SurlignageCalc,sizeof(m_SurlignageCalc));
	// The surlignage style
	FileOut.Write(&m_SurlignageStyle,sizeof(m_SurlignageStyle));

	// The Row Col page data types
	for(int x=0; x<3 ; ++x)
	{
		int DType = m_DType[x];
		FileOut.Write(&DType,sizeof(DType));
	}

	// Source Id Reference
	FileOut.Write(&m_SourceIdRef,sizeof(m_SourceIdRef));

	// Terrain Id Reference
	FileOut.Write(&m_TerrainIdRef,sizeof(m_TerrainIdRef));

	////////////////////////////////////////////////// Version N° 7 /////////////////////////////////////////
	// A partir de la version n° 7, sauvegarde des opérations surlignages
	//
	// Nombre d'opérations surlignage
	Count = m_Lst_Ope_Surlignages.GetCount();
	FileOut.Write(&Count,sizeof(Count));

	// On stocke toutes les opérations surlignage
	for (m_Lst_Ope_Surlignages.MoveFirst(); m_Lst_Ope_Surlignages.IsValid(); m_Lst_Ope_Surlignages.MoveNext())
	{
		// Récup opération
		const COpeSurlignage & OpeSurlignage = m_Lst_Ope_Surlignages.GetItem();	

		// On stocke paramétre type de grandeur
		FileOut.Write(&OpeSurlignage.Param_Grandeur,	sizeof(OpeSurlignage.Param_Grandeur));

		// On stocke paramétre style de résultat
		FileOut.Write(&OpeSurlignage.Param_Result,		sizeof(OpeSurlignage.Param_Result));

		// On stocke type de paramétrage valeurs
		FileOut.Write(&OpeSurlignage.TypeParam,			sizeof(OpeSurlignage.TypeParam));

		// On stocke les valeurs 
		FileOut.Write(&OpeSurlignage.Valeur1,			sizeof(OpeSurlignage.Valeur1));
		FileOut.Write(&OpeSurlignage.Valeur2,			sizeof(OpeSurlignage.Valeur2));

		// On stocke la couleur de surlignage
		FileOut.Write(&OpeSurlignage.Param_Color,		sizeof(OpeSurlignage.Param_Color));
	}

	// A partir de la version 8, sauvegarde des informations concernant le calcul moyenne 
	//
	// On stocke paramétre type mode moyenne (lignes ou tableau)
	FileOut.Write(&m_ModeMoyenne,	sizeof(m_ModeMoyenne));

	// On stocke mode calcul moyenne (0 exclus ou inclus)
	FileOut.Write(&m_ModeCalculMoyenne,	sizeof(m_ModeCalculMoyenne));
	
	// sauvegarde des variables moyennables du mode tableau ainsi que la variable sélectionnée
	//
	Count = m_AllVarMoyennables.GetCount();
	FileOut.Write(&Count,sizeof(Count));
	if(Count)
	{
		for(m_AllVarMoyennables.MoveFirst(); m_AllVarMoyennables.IsValid(); m_AllVarMoyennables.MoveNext())
			m_AllVarMoyennables.GetItem()->Write(FileOut,ExpMap);

		// sauvegarde de la variable moyennable sélectionnée
		// m_pItemWrapperMoy->Write(FileOut, ExpMap);

		JInt32 IdVarMoySel = 0;
		if (m_pItemWrapperMoy)
		{
			if (m_pItemWrapperMoy->PtrItem())
			{
				IdVarMoySel = m_pItemWrapperMoy->PtrItem()->GetID();
			}
		}					
		FileOut.Write(&IdVarMoySel, sizeof(IdVarMoySel));

	}
	// 
	// fin supplément version 8

	m_DocPath = FileOut.GetFilePath();
	m_DocTitle = FileOut.GetFileTitle();
	m_DocTitle.TrimRight(_T(".anl"));
	m_bDirty = false;

	// Maj no de version courante
	m_NoVersion = Version;

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Récupération du document Tris Croises
//
// Attention, au delà de la version 6, on prend en compte les constructions du mode pupitre
// afin de sauvegarder les palettes lignes, colonnes et pages qui ont servies à construire
// les composants des tris croisés (Alain Avril 2006)
// donc en renvoie le n° de version pour savoir s'il faut à la suit du document, récupérer
// les éléments de construction pupitre
//
// Version 7 : A VOIR ajout lecture des seuils utilisateur et application
//             + récupérations des opérations surlignage
//
// Vesrion 8 : Récupération du mode moyenne (lignes ou tableau)		
//
bool CAnalyseurDoc::LoadDoc(CStdioFile & FileIn)
{
	int Version = 0;

	// Read the file version number
	FileIn.Read(&Version,sizeof(Version));

	//*********************************************************************************
	//*****************		File Version 1							*******************
	//*********************************************************************************
	// Import map for importing items
	JMap<JInt32, const IItem *> ImpMap;

	if(Version >= 1)
	{
		// Results to display
		m_Grandeurs.Reset();
		int Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		for(; Count > 0; --Count)
				FileIn.Read(&(m_Grandeurs.AddTail()),sizeof(RESULTTYPE));

		//*********************************************************************************
		//*****************		File Version < 4						*******************
		//*********************************************************************************
		if(Version < 4)
		{
			// Highlight value no longer used
			int Dummy;
			FileIn.Read(&Dummy,sizeof(Dummy));
			// Table display styles
			FileIn.Read(&m_TableStyle,sizeof(m_TableStyle));
			// Remove old style surlignage forte
			m_TableStyle &= ~TSTYLE_ORDONNERFORTE;
			m_bDirty = true;
		}
		else
			// Table display styles
			FileIn.Read(&m_TableStyle,sizeof(m_TableStyle));

		// Result styles
		FileIn.Read(&m_ResultStyle,sizeof(m_ResultStyle));

		// Avant la version 5, pas de coeff corrélation en horizontal
		if (Version <= 5)
		{
			m_ResultStyle = (((DWORD)m_ResultStyle << 1) & (~0x07)) | ((DWORD)m_ResultStyle & 0x03);

			// A cause de l'info cible décalé par zscore
			if (m_ResultStyle >= RSTYLE_GBL_ZSCORE)
				m_ResultStyle += RSTYLE_GBL_ZSCORE;
		}

		// Left click mode
		FileIn.Read(&m_ClickMode,sizeof(m_ClickMode));
		// Calculation Flag
		FileIn.Read(&m_Calculation,sizeof(m_Calculation));

		// Source Id
		UINT SourceId = 0;
		m_SourceId = 0;
		FileIn.Read(&SourceId,sizeof(SourceId));

		// Type poids Id
		UINT TypePoidsId = 0;
		m_TypePoidsId = 0;
		FileIn.Read(&TypePoidsId,sizeof(TypePoidsId));

		// Segment Ids
		JList<UINT> SegmentsId;
		m_SegmentsId.Reset();

		Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		for(; Count > 0; --Count)
		{
			UINT Seg = 0;
			FileIn.Read(&Seg,sizeof(Seg));
			SegmentsId.AddTail() = Seg;
		}

		// default terrain Id
		m_pTerrainTemoin = 0;
		JInt32 TerrainTemoinId = 0;
		FileIn.Read(&TerrainTemoinId,sizeof(TerrainTemoinId));

		// Terrain Ids
		m_TerrainId.Reset();
		Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		for(; Count > 0; --Count)
		{
			UINT Id = 0;
			FileIn.Read(&Id,sizeof(Id));
			m_TerrainId.AddTail() = Id;
		}

		LoadTerrains(SourceId,m_TerrainId,SegmentsId,TypePoidsId);

		m_pTerrainData.MoveTo(TerrainTemoinId);
		if(m_pTerrainData.IsValid())
			m_pTerrainTemoin = m_pTerrainData.GetItem().get()->m_pTerrain;  

		m_SelectedTerrains.Reset();
		Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		JList<JInt32> SelectedTerrains;
		for(; Count > 0; --Count)
		{
			JInt32 Id = 0;
			FileIn.Read(&Id,sizeof(Id));
			m_pTerrainData.MoveTo(Id);
			if(m_pTerrainData.IsValid())
				SelectedTerrains.AddTail() = Id;
		}
		Set_SelectedTerrains(SelectedTerrains, true); 

		// Import map for importing items
		// JMap<JInt32, const IItem *> ImpMap;

		// List of targets
		m_pSelectedCible = 0;
		m_ListCibles.Reset();
		Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		if(Count)
		{
			// Import the selected targets
			// if(!Get_ItemsConstruits()->Import(FileIn, ImpMap))
			if(!CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Import(FileIn, ImpMap))
				
				return false;

			// List of targets
			for(; Count > 0; --Count)
			{
				JInt32 Id = 0;
				FileIn.Read(&Id,sizeof(Id));
				ImpMap.MoveTo(Id);
				if(ImpMap.IsValid())
					m_ListCibles.AddTail() = ImpMap.GetItem()->AsItemConstruit();
			}

			// Selected target Id
			JInt32 SelectedCibleId = 0;
			FileIn.Read(&SelectedCibleId,sizeof(SelectedCibleId));
			ImpMap.MoveTo(SelectedCibleId);
			if(ImpMap.IsValid())
				m_pSelectedCible = ImpMap.GetItem()->AsItemConstruit();
		}

		// Dimension Row index
		FileIn.Read(&m_RowIndex,sizeof(m_RowIndex));
		// Dimension Col index
		FileIn.Read(&m_ColIndex,sizeof(m_ColIndex));
		// Dimension Page index
		FileIn.Read(&m_PageIndex,sizeof(m_PageIndex));

		// List of all Dimension Items  (les items "normaux" et les items "moyenne" / sélectionnés via la boite de sélection items moyennes)
		m_AllItems.Reset();
		Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		if(Count)
		{
			JMap<JInt32,CItemWrapper *> WrapperMap;

			for(; Count > 0; --Count)
			{
				CItemWrapper * item = new CItemWrapper();

				if(item->Read(FileIn,WrapperMap,ImpMap))
				{
					m_AllItems.AddTail() = item;
				}
				else
				{
					delete item;
					return false;
				}
			}

			// List of Dimension Items by Row, Col and Page
			Count = 0;
			FileIn.Read(&Count,sizeof(Count));
			for(int x=0; x <Count ; ++x)
			{
				m_Dim[x].Reset();
				int Cnt = 0;
				FileIn.Read(&Cnt,sizeof(Cnt));
				for(; Cnt > 0; --Cnt)
				{
					JInt32 Id = 0;
					FileIn.Read(&Id,sizeof(Id));
					WrapperMap.MoveTo(Id);
					if(WrapperMap.IsValid())
						m_Dim[x].AddTail() = WrapperMap.GetItem();
				}
			}
		}

		// Base 100 Col Item
		FileIn.Read(&m_BaseColItem,sizeof(m_BaseColItem));
		// Base 100 Row Item
		FileIn.Read(&m_BaseRowItem,sizeof(m_BaseRowItem));
		// Cur Page
		FileIn.Read(&m_CurPage,sizeof(m_CurPage));
		// The sort universe
		FileIn.Read(&m_SortUniv,sizeof(m_SortUniv));
		// The sort column
		FileIn.Read(&m_SortCol,sizeof(m_SortCol));

		// The sort page
		FileIn.Read(&m_SortPage,sizeof(m_SortPage));

		// The sort calculation base
		FileIn.Read(&m_SortCalc,sizeof(m_SortCalc));
		Set_GrandeurTri(m_SortCalc); 

		// The sort direction
		FileIn.Read(&m_bSortDirection,sizeof(m_bSortDirection));
		Set_TypeTri(m_bSortDirection);

		// The sorted style
		FileIn.Read(&m_SortedStyle,sizeof(m_SortedStyle));
		Set_ResultTri(m_SortedStyle);
	}

	//*********************************************************************************
	//*****************		File Version 2							*******************
	//*********************************************************************************
	if(Version >= 2)
	{
		// The Row Header Hight
		FileIn.Read(m_RowHdrHeight,sizeof(int)*2);
		// The Col Header Width
		FileIn.Read(m_ColHdrWidth,sizeof(int)*2);
	}

	//*********************************************************************************
	//*****************		File Version 3							*******************
	//*********************************************************************************
	if(Version >= 3)
	{
		FileIn.Read(&m_SortType,sizeof(m_SortType));
		if (m_SortType)
		{
			// Tri par blocs de critères
			this->Set_ModeTri(MODE_BLOC); 
		}
		else
		{
			// Tri sur toutes les lignes
			this->Set_ModeTri(MODE_LIGNE); 
		}
	}

	//*********************************************************************************
	//*****************		File Version 4							*******************
	//*********************************************************************************
	if(Version >= 4)
	{
		// The sort calculation base
		FileIn.Read(&m_SurlignageCalc ,sizeof(m_SurlignageCalc));
		// The sorted style
		FileIn.Read(&m_SurlignageStyle,sizeof(m_SurlignageStyle));

		// The Row Col page data types
		if(Version > 4)
		{
			for(int x=0; x<3 ; ++x)
			{
				int DType = 0; 
				FileIn.Read(&DType,sizeof(DType));
				m_DType[x] = DType;
			}
		}
		else
		{
			for(int x=0; x<3 ; ++x)
				m_DType[x] = DTYPE_ITEMS;
			m_bDirty = true;
		}
	}

	// Source Id Reference
	FileIn.Read(&m_SourceIdRef,sizeof(m_SourceIdRef));

	// Terrain Id Reference
	FileIn.Read(&m_TerrainIdRef,sizeof(m_TerrainIdRef));

	/* A VOIR
	//*********************************************************************************
	//*****************		File Version 7							*******************
	//*********************************************************************************
	// A partir de la version n°7
	if (Version >= 7)
	{
		// Sauvegarde seuil marge erreur utilisateur
		FileIn.Read(&m_SeuilMargeErreurUtil,sizeof(m_SeuilMargeErreurUtil));

		// Sauvegarde seuil marge erreur application
		FileIn.Read(&m_SeuilMargeErreurAppli, sizeof(m_SeuilMargeErreurAppli));	
	}
	*/

	////////////////////////////////////////////////// Version N° 7 /////////////////////////////////////////
	// A partir de la version n° 7, sauvegarde des opérations surlignages
	//

	if (Version >= 7)
	{
		// Nombre d'opérations surlignage
		int Count;
		FileIn.Read(&Count,sizeof(Count));
		if (Count)
		{
			// On récupère toutes les opérations surlignage
			for (JUnt32 NbOpe = 0; NbOpe < Count; NbOpe++)
			{
				// Récup opération
				COpeSurlignage & OpeSurlignage = m_Lst_Ope_Surlignages.AddTail();

				// Récup paramètre grandeur
				FileIn.Read(&OpeSurlignage.Param_Grandeur,	sizeof(OpeSurlignage.Param_Grandeur));
				
				// Récup paramétre résultat
				FileIn.Read(&OpeSurlignage.Param_Result,	sizeof(OpeSurlignage.Param_Result));

				// Récup type paramètrage valeurs
				FileIn.Read(&OpeSurlignage.TypeParam,		sizeof(OpeSurlignage.TypeParam));

				// Recup les valeurs
				FileIn.Read(&OpeSurlignage.Valeur1,			sizeof(OpeSurlignage.Valeur1));
				FileIn.Read(&OpeSurlignage.Valeur2,			sizeof(OpeSurlignage.Valeur2));

				// Récup couleur surlignage
				FileIn.Read(&OpeSurlignage.Param_Color,		sizeof(OpeSurlignage.Param_Color));
			}
		}
	}

	// Par défaut les élmts moyennes non existants avant la version 8
	m_ModeMoyenne		= MOY_LIGNE;
	m_ModeCalculMoyenne	= CALC_MOY_0_INCLUS;
	m_AllVarMoyennables.Reset();

	if (Version >= 8)
	{
		// Récupération mode moyenne (lignes ou tableau)
		FileIn.Read(&m_ModeMoyenne,	sizeof(m_ModeMoyenne));

		// Récupération mode calcul moyenne (0 exclus ou inclus)
		FileIn.Read(&m_ModeCalculMoyenne,	sizeof(m_ModeCalculMoyenne));

		// Récupération des variables moyennables
		m_AllVarMoyennables.Reset();
		m_pItemWrapperMoy = 0;
		int Count = 0;
		FileIn.Read(&Count,sizeof(Count));
		if(Count)
		{
			JMap<JInt32,CItemWrapper *> WrapperMap;

			for(; Count > 0; --Count)
			{
				CItemWrapper * item = new CItemWrapper();

				if(item->Read(FileIn,WrapperMap,ImpMap))
				{
					m_AllVarMoyennables.AddTail() = item;
				}
				else
				{
					delete item;
					return false;
				}
			}


			for (WrapperMap.MoveFirst(); WrapperMap.IsValid(); WrapperMap.MoveNext())
			{
				JInt32 IdWrapper			= WrapperMap.GetKey();
				CItemWrapper * pItemWrapper = WrapperMap.GetItem();
				JBool Ok = true;
			}

			// Récupération variable moyennable sélectionnée
			JInt32 IdVarMoySelect;
			FileIn.Read(&IdVarMoySelect,	sizeof(IdVarMoySelect));
			WrapperMap.MoveTo(IdVarMoySelect);
			if (WrapperMap.IsValid())
			{
				m_pItemWrapperMoy = WrapperMap.GetItem();
			}
			else
				m_pItemWrapperMoy = 0;


			/*
			CItemWrapper * item = new CItemWrapper();
			if(!item->Read(FileIn,WrapperMap,ImpMap))
			{
				m_pItemWrapperMoy = 0;
				delete item;
			}
			else
			{
				m_pItemWrapperMoy = item;
			}
			*/

			

		}
	}

	m_Calculation |= CALC_UNIVERSE;
	m_DocPath = FileIn.GetFilePath();
	m_DocTitle = FileIn.GetFileTitle();
	m_DocTitle.TrimRight(_T(".anl"));
	m_bDirty = false;

	// Récup no de version
	m_NoVersion = Version;

	return true;
}

////////////////////////////////////////////////////////////////
// Largeur des colonnes headers
void	CAnalyseurDoc::SetColHdrWidth(int Col, int Width)
{ 
	if(Col < 2)
	{
		m_ColHdrWidth[Col] = Width;
		m_bDirty = true;
	}
}

////////////////////////////////////////////////////////////////
// Hauteur des lignes headers
void	CAnalyseurDoc::SetRowHdrHeight(int Row, int Height)
{ 
	if(Row < 2) 
	{
		m_RowHdrHeight[Row] = Height;
		m_bDirty = true;
	}
}

void	CAnalyseurDoc::Set_SelectedTerrains(const JList<JInt32> & SelList, bool bChanged)
{
	if(m_SelectedTerrains.GetCount() != SelList.GetCount())
		bChanged = true;
	if(!bChanged)
	{
		for(m_SelectedTerrains.MoveFirst(); m_SelectedTerrains.IsValid(); m_SelectedTerrains.MoveNext())
		{
			bool Found = false;
			for(SelList.MoveFirst(); SelList.IsValid(); SelList.MoveNext())
				if(m_SelectedTerrains.GetItem() == SelList.GetItem())
					Found = true;
			if(!Found)
				bChanged = true;
		}
	}

	if(bChanged)
	{
		JList<CCalcItemParams *> Universes;
		m_SelectedTerrains.Reset();

		for(SelList.MoveFirst(); SelList.IsValid(); SelList.MoveNext())
		{
			m_pTerrainData.MoveTo(SelList.GetItem());
			if(m_pTerrainData.IsValid())
			{
				m_SelectedTerrains.AddTail() = SelList.GetItem();
				CCalcItemParams * pParams = m_pTerrainData.GetItem()->m_pParams.get();

				// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)  // Par défaut on reste en mode calcul sans cumul  (modif Juillet 2013)
				// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
				JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
				pParams->SetCumulQuanti(CumulQuanti);

				Universes.AddTail() = pParams;   
				Get_ItemEnsemble()->CalculateEffectif(pParams);
			}
		}
		Set_UniverseParams(Universes);
		m_bDirty = true;
	}
	m_SelectedTerrains.MoveFirst();
	if(m_SelectedTerrains.IsValid())
	{
		m_pTerrainData.MoveTo(m_SelectedTerrains.GetItem());
		if(m_pTerrainData.IsValid())
			m_pTerrainTemoin =   m_pTerrainData.GetItem()->m_pTerrain;  
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Entete Header Impression Tris Croisés
bool	CAnalyseurDoc::ExportHeader(CFile * pFile)
{
	CString txt, txt2;

	// Nom de la version	
	txt.Format(_T("@VERSION@\t1\r\n")); 
	pFile->Write(txt.GetBuffer(),txt.GetLength());

	// Mode exclusif ou dupliqué s'il y a lieu
	if(IsTableStyleSet(TSTYLE_EXCLUSIVE))
	{
		txt.LoadString(IDS_AN_ITEMEXCLUSIVE);
		txt2.Format(_T("@REPONDANTS@\t%s\r\n"),txt);
		pFile->Write(txt2.GetBuffer(),txt2.GetLength());
	}
	else if(IsTableStyleSet(TSTYLE_DUPLIQUE))
	{
		txt.LoadString(IDS_AN_ITEMDUPLIQUANT);
		txt2.Format(_T("@REPONDANTS@\t%s\r\n"),txt);
		pFile->Write(txt2.GetBuffer(),txt2.GetLength());
	}

	// Mode calcul global standard
	if(Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
	{
		txt.LoadString(IDS_AN_CORRELATION);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		pFile->Write(txt2.GetBuffer(),txt2.GetLength());
	}
	else if(Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
	{
		txt.LoadString(IDS_AN_PROXIMITY);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		pFile->Write(txt2.GetBuffer(),txt2.GetLength());
	}
	else if(Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
	{
		txt.LoadString(IDS_AN_ZSCORE);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		pFile->Write(txt2.GetBuffer(),txt2.GetLength());
	}

	// Plusieurs grandeurs utilisées
	CString Str = "";
	for (m_Grandeurs.MoveFirst(); m_Grandeurs.IsValid(); m_Grandeurs.MoveNext())
	{
		// Nom de la grandeur
		CString StrGrandeur = "";
		GetGrandeurString(StrGrandeur,m_Grandeurs.GetItem());
		Str += StrGrandeur;

		// Séparateur grandeur
		if (m_Grandeurs.GetIndex() != m_Grandeurs.GetCount() - 1)
			Str += ",";
	}
	txt.Format(_T("@GRANDEUR@\t%s\r\n"),Str) ;
	pFile->Write(txt.GetBuffer(),txt.GetLength());
	
	// Info Source / Terrain
	m_SelectedTerrains.MoveFirst();
	if(m_SelectedTerrains.IsValid())
	{
		m_pTerrainData.MoveTo(m_SelectedTerrains.GetItem());
		if(m_pTerrainData.IsValid())
		{
			txt.Format(_T("@SOURCE@\t%s\r\n"),m_pTerrainData.GetItem()->GetSourceLabel());
			pFile->Write(txt.GetBuffer(),txt.GetLength());
			if(m_SelectedTerrains.GetCount() == 1)
			{
				txt.Format(_T("@TERRAIN@\t%s\r\n"),m_pTerrainData.GetItem()->GetTerrainLabel());
				pFile->Write(txt.GetBuffer(),txt.GetLength());
			}
		}
	}

	// Info Segments
	if(m_SegmentsId.GetCount() > 0)
	{
		txt.Format(_T("@SEGMENT@"));
		pFile->Write(txt.GetBuffer(),txt.GetLength());
		for (m_SegmentsId.MoveFirst(); m_SegmentsId.IsValid();m_SegmentsId.MoveNext())
		{
			const JSegment* Seg = JSRCTables::GetInstance().m_TBLSegments.GetItem(m_SegmentsId.GetItem());
			if (Seg != 0)
				txt.Format(_T("\t%s"), Seg->m_Libelle.AsJCharPtr());
			else
				txt.Empty();
			pFile->Write(txt.GetBuffer(),txt.GetLength());
		}
		txt.Format(_T("\r\n"));
		pFile->Write(txt.GetBuffer(),txt.GetLength());
	}

	// Info Cible ou Univers
	if(Get_SelectedCible())
	{
		if(Get_ResultStyle() & RSTYLE_CIBLES)
			txt.Format(_T("@CIBLE@\t%s\r\n"),Get_SelectedCible()->GetLabel().AsJCharPtr());
		else
			txt.Format(_T("@UNIVERS@\t%s\r\n"),Get_SelectedCible()->GetLabel().AsJCharPtr());

		pFile->Write(txt.GetBuffer(),txt.GetLength());
	}

	////////////////////////////////// Export Informations sur la moyenne /////////////////////////////////////////////////////
	// 
	if (m_ModeMoyenne == MOY_LIGNE)
	{
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Lignes ");
	}
	else if (m_ModeMoyenne == MOY_COLONNE)
	{
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Colonnes ");
	}
	else
	{
		// En mode tableau, ne pas oublier d'ajouter la variable moyennable
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Tableau: ");

		// Recup texte de la variable moyennable du mode tableau
		JInt32 InxSelVarMoy = m_DlgAnalyseur.m_List_VarMoy.GetCurSel();
		if (InxSelVarMoy >= 0)
		{
			CString StrVarMoy = "";
			m_DlgAnalyseur.m_List_VarMoy.GetText(InxSelVarMoy, StrVarMoy);
			txt += StrVarMoy;
		}
	}

	// Informations 0 exclu / 0 inclu
	if (m_ModeCalculMoyenne == CALC_MOY_0_INCLUS)
	{
		txt += " (0 inclus)\r\n";
	}
	else
	{
		txt += " (0 exclus)\r\n";
	}
	pFile->Write(txt.GetBuffer(),txt.GetLength());
	
	// Info dimensions hauteur et largeur header (pour mise en page)
	// Largeur header Colonne 1 et 2
	txt.Format(_T("@LARGHEADERLIG@\t%d\t%d\n"),GetColHdrWidth(0),GetColHdrWidth(1));
	pFile->Write(txt.GetBuffer(),txt.GetLength());

	// Hauteur header ligne 1 et 2
	txt.Format(_T("@HAUTHEADERCOL@\t%d\t%d\n"),GetRowHdrHeight(0),GetRowHdrHeight(1));
	pFile->Write(txt.GetBuffer(),txt.GetLength());

	// Mode découpage
	txt.Format(_T("@DECOUPAGE@\t%d\n"), m_ModeDecoupage);
	pFile->Write(txt.GetBuffer(),txt.GetLength());

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// Entete Header Tris Croisés
CString	CAnalyseurDoc::HeaderTrisCroises()
{
	CString txt, txt2;
	CString TxtEntete;

	// Init txt
	TxtEntete = "";

	// Entete Surlignage
	txt.Format(_T("@SURLIGNAGE@\r\n")); 
	TxtEntete += txt;

	// Nom de la version	
	txt.Format(_T("@VERSION@\t1\r\n")); 
	TxtEntete += txt;

	// Mode exclusif ou dupliqué s'il y a lieu
	if(IsTableStyleSet(TSTYLE_EXCLUSIVE))
	{
		txt.LoadString(IDS_AN_ITEMEXCLUSIVE);
		txt2.Format(_T("@REPONDANTS@\t%s\r\n"),txt);
		TxtEntete += txt2;
	}
	else if(IsTableStyleSet(TSTYLE_DUPLIQUE))
	{
		txt.LoadString(IDS_AN_ITEMDUPLIQUANT);
		txt2.Format(_T("@REPONDANTS@\t%s\r\n"),txt);
		TxtEntete += txt2;
	}

	// Mode calcul global standard
	if(Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
	{
		txt.LoadString(IDS_AN_CORRELATION);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		TxtEntete += txt2;
	}
	else if(Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
	{
		txt.LoadString(IDS_AN_PROXIMITY);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		TxtEntete += txt2;
	}
	else if(Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
	{
		txt.LoadString(IDS_AN_ZSCORE);
		txt2.Format(_T("@RSTYLE_GLOBAL@\t%s\r\n"),txt);
		TxtEntete += txt2;
	}

	// Plusieurs grandeurs utilisées
	CString Str = "";
	for (m_Grandeurs.MoveFirst(); m_Grandeurs.IsValid(); m_Grandeurs.MoveNext())
	{
		// Nom de la grandeur
		CString StrGrandeur = "";
		GetGrandeurString(StrGrandeur,m_Grandeurs.GetItem());
		Str += StrGrandeur;

		// Séparateur grandeur
		if (m_Grandeurs.GetIndex() != m_Grandeurs.GetCount() - 1)
			Str += ",";
	}
	txt.Format(_T("@GRANDEUR@\t%s\r\n"),Str) ;
	TxtEntete += txt;

	// Indicateur de calcul initialisé à non défini
	CString StrStyleCalc;
	StrStyleCalc.LoadString(IDS_AN_NON_DEFINI);

	switch (m_SurlignageStyle) 
	{
		case RSTYLE_NOSTYLE:
			break;
		case RSTYLE_RESULT:
			// "Grandeur";
			StrStyleCalc.LoadString(IDS_AN_LABEL_GRANDEUR);
			break;
		case RSTYLE_HORZ_STRUCTURE:
			// "H% Structure"
			StrStyleCalc.LoadString(IDS_AN_LABEL_H_STRUCTURE);
			break;
		case RSTYLE_HORZ_CORRELATION:
			// "H% Corrélation"
			StrStyleCalc.LoadString(IDS_AN_LABEL_H_CORRELATION);
			break;
		case RSTYLE_HORIZONTAL:
			// "H%"
			StrStyleCalc.LoadString(IDS_AN_ABR_HORIZ);
			break;
		case RSTYLE_VERTICAL:
			// "V%"
			StrStyleCalc.LoadString(IDS_AN_ABR_VERT);
			break;
		case RSTYLE_GBL_STRUCTURE:
			// "Indice structure"
			StrStyleCalc.LoadString(IDS_AN_LABEL_INDICE_STRUCTURE);
			break;
		case RSTYLE_GBL_CORRELATION:
			// "Indice corrélation"
			StrStyleCalc.LoadString(IDS_AN_LABEL_INDICE_CORRELATION);
			break;
		case RSTYLE_GBL_PROXIMITY:
			// "Indice proximité"
			StrStyleCalc.LoadString(IDS_AN_LABEL_INDICE_PROXIMITE);
			break;
		case RSTYLE_GBL_ZSCORE:
			// "Zscore"
			StrStyleCalc.LoadString(IDS_AN_ZSCORE);
			break;
		case RSTYLE_GLOBAL:
			// "Global"
			StrStyleCalc.LoadString(IDS_AN_LABEL_GLOBAL);
			break;
		case RSTYLE_CIBLES:
			// "Cibles";
			StrStyleCalc.LoadString(IDS_AN_LABEL_CIBLES);
			break;
	}
	txt.Format(_T("@INDICATEUR@\t%s\r\n"),StrStyleCalc);
	TxtEntete += txt;
		
	// Info Source / Terrain
	m_SelectedTerrains.MoveFirst();
	if(m_SelectedTerrains.IsValid())
	{
		m_pTerrainData.MoveTo(m_SelectedTerrains.GetItem());
		if(m_pTerrainData.IsValid())
		{
			txt.Format(_T("@SOURCE@\t%s\r\n"),m_pTerrainData.GetItem()->GetSourceLabel());
			TxtEntete += txt;
			if(m_SelectedTerrains.GetCount() == 1)
			{
				txt.Format(_T("@TERRAIN@\t%s\r\n"),m_pTerrainData.GetItem()->GetTerrainLabel());
				TxtEntete += txt;
			}
		}
	}

	// Info Segments
	if(m_SegmentsId.GetCount() > 0)
	{
		txt.Format(_T("@SEGMENT@"));
		TxtEntete += txt;
		for (m_SegmentsId.MoveFirst(); m_SegmentsId.IsValid();m_SegmentsId.MoveNext())
		{
			const JSegment* Seg = JSRCTables::GetInstance().m_TBLSegments.GetItem(m_SegmentsId.GetItem());
			if (Seg != 0)
				txt.Format(_T("\t%s"), Seg->m_Libelle.AsJCharPtr());
			else
				txt.Empty();
			TxtEntete += txt;
		}
		txt.Format(_T("\r\n"));
		TxtEntete += txt;
	}

	// Info Cible ou Univers
	if(Get_SelectedCible())
	{
		if(Get_ResultStyle() & RSTYLE_CIBLES)
			txt.Format(_T("@CIBLE@\t%s\r\n"),Get_SelectedCible()->GetLabel().AsJCharPtr());
		else
			txt.Format(_T("@UNIVERS@\t%s\r\n"),Get_SelectedCible()->GetLabel().AsJCharPtr());

		TxtEntete += txt;
	}

	////////////////////////////////// Export Informations sur la moyenne /////////////////////////////////////////////////////
	// 
	if (m_ModeMoyenne == MOY_LIGNE)
	{
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Lignes ");
	}
	else if (m_ModeMoyenne == MOY_COLONNE)
	{
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Colonnes ");
	}
	else
	{
		// En mode tableau, ne pas oublier d'ajouter la variable moyennable
		txt.Format(_T("@MODE MOYENNE@\t%s"),"Tableau :");

		// Recup texte de la variable moyennable du mode tableau
		JInt32 InxSelVarMoy = m_DlgAnalyseur.m_List_VarMoy.GetCurSel();
		if (InxSelVarMoy >= 0)
		{
			CString StrVarMoy = "";
			m_DlgAnalyseur.m_List_VarMoy.GetText(InxSelVarMoy, StrVarMoy);
			txt += StrVarMoy;
		}
	}

	// Informations 0 exclu / 0 inclu
	if (m_ModeCalculMoyenne == CALC_MOY_0_INCLUS)
	{
		txt += " (0 inclus)\r\n";
	}
	else
	{
		txt += " (0 exclus)\r\n";
	}
	txt.Format(_T("\r\n"));
	TxtEntete += txt;

	// Info dimensions hauteur et largeur header (pour mise en page)
	// Largeur header Colonne 1 et 2
	txt.Format(_T("@LARGHEADERLIG@\t%d\t%d\n"),GetColHdrWidth(0),GetColHdrWidth(1));
	TxtEntete += txt;

	// Hauteur header ligne 1 et 2
	txt.Format(_T("@HAUTHEADERCOL@\t%d\t%d\n"),GetRowHdrHeight(0),GetRowHdrHeight(1));
	TxtEntete += txt;

	return TxtEntete;
}

void CAnalyseurDoc::GetGrandeurString(CString & Str, RESULTTYPE Res)
{
	switch(Res)
	{
		case RTYPE_EFFECTIVE_000: 	Str.LoadString(IDS_AN_EFFECTIVE);
		break;
		case RTYPE_EFFECTIVE_CIBLE: Str.LoadString(IDS_AN_EFFECTIVEBRUT);
		break;
		case RTYPE_NBCAS: Str.LoadString(IDS_AN_NBCAS);
		break;
		case RTYPE_QUANTITY_000: 	Str.LoadString(IDS_AN_QUANTITY);
		break;
		case RTYPE_AVERAGEQUANTUM: Str.LoadString(IDS_AN_AVERAGEQUANTUM);
		break;
		case RTYPE_AVERAGEQUANTUM_P00: Str.LoadString(IDS_AN_AVERAGEQUANTUM_P00);
		break;
		case RTYPE_CENTRAGE: 	Str.LoadString(IDS_AN_CENTRAGE);
		break;
		case RTYPE_PENETRATION: Str.LoadString(IDS_AN_PENETRATION);
		break;
		case RTYPE_CONCOMITENCE: Str.LoadString(IDS_AN_CONCOMITENCE);
		break;
		case RTYPE_COEFFYULE: Str.LoadString(IDS_AN_COEFFYULE);
		break;
		case RTYPE_ZSCORE: Str.LoadString(IDS_AN_ZSCORE);
		break;
	}
}

void CAnalyseurDoc::SetModeMargeErreur(bool AffichMargeErreur)
{
	// Récupère mode affichae marge d'erreur (avec ou sans)
	m_CalculMargeErreur = AffichMargeErreur;
}

// Aout 2013 : Force au calcul Quantitatif (m^me si item non quanti)
void CAnalyseurDoc::SetForceQuantiCalcul(bool ForceQuantiCalcul)
{
	m_ForceQuantiCalcul = ForceQuantiCalcul;
}

void CAnalyseurDoc::SetNicheQuantifiable(bool ValNicheQuanti)
{
	m_NicheQuantifiable = ValNicheQuanti;
}

void CAnalyseurDoc::AffichRowCol()
{
	CString Txt;
	Txt.Format("%d-%d",m_NoRowCur,m_NoColCur);

}

// Récupère pointeur courant en mode ligne de pupitre
void CAnalyseurDoc::SetModeLigne	(CAbstractMode	*pCurMode)
{
	m_pModeLigne	= pCurMode;
}

// Récupère pointeur courant en mode colonne de pupitre
void CAnalyseurDoc::SetModeColonne	(CAbstractMode	*pCurMode) 
{
	m_pModeColonne	= pCurMode;
}

// Récupère pointeur courant en mode page de pupitre
void CAnalyseurDoc::SetModePage	(CAbstractMode	*pCurMode) 
{
	m_pModePage		= pCurMode;
}

// Positionne dans pupitre les modes courants ligne
void CAnalyseurDoc::GetModeLigne	(CAbstractMode	*pCurMode)
{
	pCurMode		= m_pModeLigne;
}

// Positionne dans pupitre les modes courants colonne
void CAnalyseurDoc::GetModeColonne	(CAbstractMode	*pCurMode) 
{
	pCurMode		= m_pModeColonne;
}

// Positionne dans pupitre les modes courants page
void CAnalyseurDoc::GetModePage	(CAbstractMode	*pCurMode) 
{
	pCurMode		= m_pModePage;
}

// Recup Map Etat tree theme
JMap<DWORD,JUnt32> * CAnalyseurDoc::GetEtatTreeTheme()
{
	return (&m_MapSelTreeTheme);
}

// Recup Map Etat tree theme
JMap<DWORD,JUnt32> * CAnalyseurDoc::GetEtatTreeQuanti()
{
	return (&m_MapSelTreeQuanti);
}

// Initialise le Map déploiement et sélection index thématique
void CAnalyseurDoc::ResetEtatTreeTheme(bool Init)
{
	if (Init)
		m_MapSelTreeTheme.Reset(); 
}

// Initialise le Map déploiement et sélection index thématique des elmts sélectionnés à moyenner
void CAnalyseurDoc::ResetEtatTreeQuanti(bool Init)
{
	if (Init)
		m_MapSelTreeQuanti.Reset(); 
}


/////////////////////////////////////////////////// Récupération des libellés paramétres de tri /////////////////////////////////////////////
//
// Récupère libellé mode de tri
CString CAnalyseurDoc::Get_LibModeTri() const
{	
	CString Libelle = "Mode";

	switch (m_ModeTri)
	{
		case MODE_LIGNE : Libelle = "Global";	break;
		case MODE_BLOC  : Libelle = "Par question";	break;
	}

	return Libelle;
}
	
// Récupère libellé type de tri
CString CAnalyseurDoc::Get_LibTypeTri() const
{
	CString Libelle = "Type";

	switch (m_TypeTri)
	{
		// Enumération des différents type de tri
		case TYPE_BASE			: Libelle = "Aucun tri";		break;
		case TYPE_DECROISSANT	: Libelle = "Décroissant";	break;
		case TYPE_CROISSANT		: Libelle = "Croissant";		break;
	}

	return Libelle;
}

// Récupère libellé grandeur tri
CString CAnalyseurDoc::Get_LibGrandeurTri() const
{
	CString Libelle = "";

	switch (m_IdTriGrandeurs)
	{
		case RTYPE_EFFECTIVE_000		: Libelle = "Effectif(000)";	break; 
		case RTYPE_NBCAS				: Libelle = "Nb Cas";			break; 
		case RTYPE_QUANTITY_000			: Libelle = "Quantity(000)";	break; 
		case RTYPE_AVERAGEQUANTUM		: Libelle = "Moyenne";			break;
		case RTYPE_PENETRATION			: Libelle = "Penetration";		break;
		case RTYPE_CENTRAGE				: Libelle = "Centrage";			break;
		case RTYPE_EFFECTIVE_CIBLE		: Libelle = "Eff. ciblé";		break;
		case RTYPE_CONCOMITENCE			: Libelle = "Concomitence";		break;
		case RTYPE_COEFFYULE			: Libelle = "Coeff. Yule";		break;
		case RTYPE_EFFECTIVE_NONCIBLE	: Libelle = "Eff. non ciblé";	break;
		case RTYPE_AVERAGEQUANTUM_P00	: Libelle = "Moyenne (000)";	break;
		case RTYPE_ZSCORE				: Libelle = "ZScore";			break;
	}
			
	return Libelle;
}

// Récupère libellé résultat tri
CString CAnalyseurDoc::Get_LibResultTri() const
{
	CString Libelle = "";

	switch (m_IdTriResultats)
	{
		// Result
		case RSTYLE_RESULT:		Libelle = "Grandeur";	break;
		
		// Horizontal
		case RSTYLE_HORIZONTAL: Libelle = "H%";			break;

		// Vertical
		case RSTYLE_VERTICAL:	Libelle = "V%";			break;

		// Global
		case RSTYLE_GLOBAL:     
		{
			// Mode calcul global standard
			if(Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
			{
				// LibResult = "IC";
				Libelle.LoadString(IDS_AN_ABR_CONCOMITENCE);
			}
			else if(Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
			{
				// LibResult = "IP";
				Libelle.LoadString(IDS_AN_ABR_COEFFYULE); 
			}
			else if(Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
			{
				// LibResult = "ZSCORE";
				Libelle.LoadString(IDS_AN_ABR_ZSCORE);
			}
			else
			{
				// Libelle G%
				// Libelle = "G%";
				Libelle.LoadString(IDS_AN_ABR_GLOBAL);
			}
		}	
		break;
	}

	return Libelle;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour titre tableau croisé
void CAnalyseurDoc::UpdateTitre()
{
	m_DlgAnalyseur.SetTitre(); 
}

/////////////////////////////////////////////////////////////////////////////////////////
// Positionne flag curseur tri
void CAnalyseurDoc::SetCurseurTri(JBool Tri)
{
	m_DlgAnalyseur.SetCursorTri(Tri);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour du curseur tableau croisé
void CAnalyseurDoc::UpdateCursor()
{
	m_DlgAnalyseur.SetGridCursor(); 
}

///////////////////////////////////////////////////////////////////////////////
// Récup les paramètres par défaut
void CAnalyseurDoc::SetParamDefSurlignage(COpeSurlignage * pDefault_OpeSurlignage)
{
	m_pDefault_OpeSurlignage = pDefault_OpeSurlignage;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour Map des cellules à surligner via la liste des opérateurs surlignage
void CAnalyseurDoc::UpdateSurlignage(IItemTable * pIItemTable, CAnCrossTable *pCrossTable)
{
	// Lock update tableur
	pCrossTable->LockUpdate(true);

	// if (m_Lst_Ope_Surlignages.GetCount() == 0 && m_ElmtGrandeurMoyennableSel == false) return;

	// Init color all rows & cols
	if (m_Lst_Ope_Surlignages.GetCount() == 0)
	{
		// Init map des cellules à surligner
		m_MapCellTotSurlign.Reset();
	}

	if (m_Lst_Ope_Surlignages.GetCount() != 0 || m_ElmtGrandeurMoyennableSel == true)
	{
		///////////////////////////// Reporter dans une routine spécifique ///////////////////////////////////
		// Index Ligne et Colonne de base
		int BaseColItem		=	Get_BaseColItem();
		int BaseRowItem		=	Get_BaseRowItem();
		
		// Index dimension page courante
		int PageItem		=	pIItemTable->GetPageItem(GetCurPage());

		// Style paramétrage tableur	
		UINT TableStyle		=	Get_TableStyle();

		// Init map des cellules à surligner
		m_MapCellTotSurlign.Reset();

		// Boucle sur les opérations surlignage
		for (m_Lst_Ope_Surlignages.MoveFirst(); m_Lst_Ope_Surlignages.IsValid(); m_Lst_Ope_Surlignages.MoveNext())
		{
			//Récupère opération surlignage
			const COpeSurlignage OpeSurlignage = m_Lst_Ope_Surlignages.GetItem();

			// Récupération couleur de surlignage
			COLORREF ColorSurlign		= OpeSurlignage.Param_Color; 

			// Récupération du paramétrage surlignage supplémentaire
			double		ParamVal1		= OpeSurlignage.Valeur1; 
			double		ParamVal2		= OpeSurlignage.Valeur2; 
			BYTE		ParamType     	= OpeSurlignage.TypeParam; 
			RESULTTYPE	ParamGrandeur	= OpeSurlignage.Param_Grandeur; 
			RESULTSTYLE ParamResult		= OpeSurlignage.Param_Result; 

			// Boucle sur toutes les lignes et colonnes
			for (int nRow = 1; nRow <= pCrossTable->GetRowCount(); ++nRow)
			{
				for (int nCol = 1; nCol <= pCrossTable->GetColCount() ; ++nCol)
				{
					// Récupère coordonnées lignes et colonnes
					int Ri,Ru,Rc,Rv;
					CELLTYPE Rt			=	pIItemTable->GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);

					int Ci,Cu,Cc,Cv;
					CELLTYPE Ct			=	pIItemTable->GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

					// Récupère le type résultat (résultat) de la cellule
					RESULTSTYLE ResultCell		= pIItemTable->GetResultStyle(Rv, Cv);

					// Récupère le style résultat (grandeur) de la cellule
					const JList<RESULTTYPE> & CalcTypes = Get_Grandeurs();
					if(TableStyle & TSTYLE_GRANDEURCOL)
						CalcTypes.MoveTo(Cc);
					else
						CalcTypes.MoveTo(Rc);

					// Récupère le type résultat (résultat) de la cellule
					RESULTTYPE	GrandeurCell =  CalcTypes.GetItem();

					// Récupère résult pour test surlignage seulement pour cellules item
					if (Rt == CTYPE_ITEMCELL && Ct == CTYPE_ITEMCELL)
					{
						// Univers en ligne ou en colonne ?
						int Universe = 0;
						if(TableStyle & TSTYLE_TERRAINCOL)
							Universe = Cu;
						else
							Universe = Ru;

						// Update Map des cellules à ajouter au surlignage
						double Result	= 0.0;

						// Flag si division par zéro
						bool DivZero			= false;

						// Flag si marge d'erreur atteinte
						bool MargeErreurDepasse = false;

						// Indication nombre de cas
						double NbCas = 0.0;

						// Marge d'erreur
						double MargeErreur = 0.0;

						// Intervalle confiance
						double IntervalleConfiance = 0.0;

						/*
						bool Glob;
						switch(ResultCell & RSTYLE_GLOBAL)
						{
							case RSTYLE_GBL_STRUCTURE :
								Glob = true;
								break;

							case RSTYLE_GBL_CORRELATION:
								Glob = true;
								break;

							case RSTYLE_GBL_PROXIMITY:
								Glob = true;
								break;

							case RSTYLE_GBL_ZSCORE:
								Glob = true;
								break;
						}
						*/

						bool FlagResultGlobal = false;
						if((ResultCell & RSTYLE_GLOBAL) && (ParamResult & (RSTYLE_GBL_STRUCTURE|RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE)))
						{
							FlagResultGlobal = true;
						}

						if (ParamGrandeur == GrandeurCell && (ParamResult == ResultCell || FlagResultGlobal))
						{
							// Récupération résultat cellule via type et style
							// if (pIItemTable->GetCellValue(Result, GrandeurCell, nRow, nCol, Universe, ResultCell , PageItem, BaseColItem, BaseRowItem, 
							//							     DivZero, NbCas, MargeErreurDepasse, MargeErreur,IntervalleConfiance))
							if (pIItemTable->GetCellValue(Result, GrandeurCell, Ri, Ci, Universe, ResultCell , PageItem, BaseColItem, BaseRowItem, 
														 DivZero, NbCas, MargeErreurDepasse, MargeErreur,IntervalleConfiance))
							{
								// Attention aux pourcentages
								if (ResultCell == RSTYLE_HORIZONTAL || ResultCell == RSTYLE_VERTICAL || FlagResultGlobal)
									Result *= 100;

								// Par défaut cellule non surlignée
								JBool CelluleSurligne = false;
								switch (ParamType)
								{
									// Surlignage cellule si résultat strictement supérieur
									case TYPE_SUP :
									{
										if (Result > ParamVal1)
											CelluleSurligne = true;
									}
									break;

									// Surlignage cellule si résultat supérieur et égal
									case TYPE_SUP_EGAL :
									{
										if (Result >= ParamVal1)
											CelluleSurligne = true;
									}
									break;

									// Surlignage cellule si résultat strictement inférieur
									case TYPE_INF :
									{
										if (Result < ParamVal1)
											CelluleSurligne = true;
									}
									break;

									// Surlignage cellule si résultat inférieur et égal
									case TYPE_INF_EGAL :
									{
										if (Result < ParamVal1)
											CelluleSurligne = true;
									}
									break;

									// Surlignage si résultat compris entre les 2 bornes (incluses)
									case TYPE_IN_BORNES :
									{
										
										if (Result >= ParamVal1 && Result <= ParamVal2)
											CelluleSurligne = true;
									}
									break;

									// Surlignage si résultat en dehors des 2 bornes (excluses)
									case TYPE_OUT_BORNES :
									{
										
										if (Result < ParamVal1 || Result > ParamVal2)
											CelluleSurligne = true;
									}
									break;
								}

								// Update map si cellule à surligner
								if (CelluleSurligne)
								{
									// Définition de la clef
									CKeyRowCol KeyRowCol;
									KeyRowCol.m_NoRow = nRow;
									KeyRowCol.m_NoCol = nCol; 
									
									// On se positionne sur le map des cellules à surligner
									m_MapCellTotSurlign.MoveTo(KeyRowCol);
									if (!m_MapCellTotSurlign.IsValid())
									{
										// Ajoute la cellule à surligner avec sa couleur
										m_MapCellTotSurlign.Add(KeyRowCol) = ColorSurlign;
									}
									else
									{
										// On modifie couleur de surlignage cellule déjà présente dans map total cellules à surligner
										COLORREF &ExColorCell = m_MapCellTotSurlign.GetItem();
										ExColorCell = ColorSurlign;
									}
								}
							}
						}
					}
				} // fin boucle cols
			} // fin boucle rows
		}
	}

	m_DlgAnalyseur.UpdateElmtMoyennable();

	/* Derniere modif 2.7 track 73
	if (m_Lst_Ope_Surlignages.GetCount() == 0 && m_AtLeastOneRowAverage == false && m_ElmtGrandeurMoyennableSel == false)
	{
		pCrossTable->Redraw();
	}
	*/
	if (m_Lst_Ope_Surlignages.GetCount() == 0 && m_ElmtGrandeurMoyennableSel == false)
	{
		pCrossTable->Redraw();
	}
	else if (m_Lst_Ope_Surlignages.GetCount() == 0 && m_AtLeastOneRowAverage == false && m_ElmtGrandeurMoyennableSel == true)
	{
		// Cas d'un surlignage à redessiner
		// pCrossTable->UpdateAllRowsCols();
		pCrossTable->UpdateAllRowsColsSize();

		pCrossTable->Redraw();
	}
	else if (m_Lst_Ope_Surlignages.GetCount() > 0 || m_AtLeastOneRowAverage == true)
	{
		// Cas d'un surlignage à redessiner
		pCrossTable->UpdateAllRowsCols();
		pCrossTable->Redraw();
	}
	else if (m_AtLeastOneRowAverage && m_ElmtGrandeurMoyennableSel == true && pCrossTable->SomeRowToHide())
	{
		// Ici je répéte volontairement les lignes du dessus car 2eme condition plus honéreuse en temps
		pCrossTable->UpdateAllRowsCols();
		pCrossTable->Redraw();
	}
	else
		pCrossTable->Redraw();

	pCrossTable->LockUpdate(false);
	pCrossTable->Invalidate(); 

	/* AVANT
	// Redessine toutes les cellules
	pCrossTable->UpdateAllRowsCols();

	// Lock update tableur
	pCrossTable->LockUpdate(false);

	pCrossTable->Redraw();
	pCrossTable->Invalidate(); 
	*/

	// le document a changé
	m_bDirty = true;

}

