#pragma once

#include "ItemConstruit.h"
#include "CalcItemParams.h"
#include "Terrain.h"
#include "ItemWrapper.h"
#include "terraindata.h"
#include "ICellResult.h"
#include "AnalyseurEnums.h"
#include "ColSortKey.h"
#include "IItemTable.h"
#include "ItemsConstruitsManager.h"

class CCrossTable;
class CAbstractMode;
class CDlgSurlignage;
class CDlgAnalyseur;

//Message pour update tri
#define ATPMSG_UPDATE_TRI_ANALYSEUR			WM_APP + 1 

///////////////////////// Info opérations surlignages ////////////////////
class COpeSurlignage
{
	public:

		// sérialisation clefs des opérateurs paramètres surlignage
		void Send(JStream & Stream) const
		{
			// on sérialise l'élément
			Stream.Send((JUnt32)Param_Grandeur);
			Stream.Send((JUnt32)Param_Result);
			Stream.Send((JUnt32)TypeParam);
			Stream.Send((JFlt64)Valeur1);
			Stream.Send((JFlt64)Valeur2);
			Stream.Send((JInt32)Param_Color);
		};

		// sérialisation clefs des opérateurs paramètres surlignage
		void Recv(JStream & Stream)
		{
			// Récup élmts
			JUnt32 ParamJUnt32;
			JFlt64 ParamFlt64;
			JInt32 ParamJInt32;

			Stream.Recv(ParamJUnt32);
			Param_Grandeur = (RESULTTYPE)  ParamJUnt32;

			Stream.Recv(ParamJUnt32);
			Param_Result   = (RESULTSTYLE) ParamJUnt32;

			Stream.Recv(ParamJUnt32);
            TypeParam      = (BYTE)		   ParamJUnt32;

			Stream.Recv(ParamFlt64);
			Valeur1		   = (double)	   ParamFlt64;		

			Stream.Recv(ParamFlt64);
			Valeur2		   = (double)	   ParamFlt64;		

			Stream.Recv(ParamJInt32);
			Param_Color    = (COLORREF)	   ParamJInt32;
		};

		RESULTTYPE		Param_Grandeur;			// Grandeur pour test paramétrage
		RESULTSTYLE		Param_Result;			// Résultat pour test résultat
		BYTE			TypeParam;				// Type de test valeur
		double			Valeur1;				// 1ere valeur de test
		double			Valeur2;				// 2ème valeur de test
		COLORREF		Param_Color;			// Couleur de surlignage
};

///////////////////////// Clef position cellule //////////////////////////
class CKeyRowCol
{
	public:

		// test d'infériorité clef 
		bool operator<(const CKeyRowCol & Rhs) const
		{
			if(m_NoRow < Rhs.m_NoRow)
				return true;
				
			if(m_NoRow == Rhs.m_NoRow && m_NoCol < Rhs.m_NoCol)
				return true;
				
			return false;
		};

		// test de supériorité clef
		bool operator>(const CKeyRowCol & Rhs) const
		{
			if(m_NoRow > Rhs.m_NoRow)
				return true;
				
			if(m_NoRow == Rhs.m_NoRow && m_NoCol > Rhs.m_NoCol)
				return true;
				
			return false;
		};

		// test d'égalité clef 
		bool operator==(const CKeyRowCol & Rhs) const
		{
			return (m_NoRow == Rhs.m_NoRow && m_NoCol == Rhs.m_NoCol);
		};

		// affectation via clef  
		CKeyRowCol & operator=(const CKeyRowCol & Rhs)
		{
			m_NoRow		= Rhs.m_NoRow;
			m_NoCol		= Rhs.m_NoCol;
			return *this;
		};

		// sérialisation clefs row & col
		void Send(JStream & Stream) const
		{
			// on sérialise l'élément
			Stream.Send(m_NoRow);
			Stream.Send(m_NoCol);
		};

		// sérialisation clefs row & col
		void Recv(JStream & Stream)
		{
			// Récup élmts
			Stream.Recv(m_NoRow);
			Stream.Recv(m_NoCol);
		};

		// test d'infériorité clef item + terrain 
		CKeyRowCol() 
		{ 
			m_NoRow	= 0; 
			m_NoCol = 0; 
		};
		
		// attributs clef item + terrain
		JInt32	m_NoRow;
		JInt32	m_NoCol;
};

class CAnalyseurDoc
{
public:
	// CAnalyseurDoc(CProgressCtrl & ProgressCtrl, CLabel & Titre, CDlgSurlignage & DlgSurlignage);
	CAnalyseurDoc(CProgressCtrl & ProgressCtrl, CDlgAnalyseur & DlgAnalyseur, CDlgSurlignage & DlgSurlignage);
	virtual ~CAnalyseurDoc(void);

	// Load the default profile settings
	//virtual void SetDefaultSettings();
	void Reset();

	// **************************************************
	// ****	Document Name							 ****
	// **************************************************

public:
	const CString	&	GetFilePath() const
							{ return m_DocPath; } ;
	const CString	&	GetTitle() const
							{ return m_DocTitle; } ;
	virtual bool		IsDirty();

	virtual bool		SaveDoc(CStdioFile & FileOut);
	virtual bool		LoadDoc(CStdioFile & FileIn);
	
	// No de version de la sauvegarde
	const int		&	GetNoVersion() const
							{return m_NoVersion; } ;

	virtual bool		ExportHeader(CFile * pFile);

	// Entete Header Tris Croisés
	CString				HeaderTrisCroises();

	// **************************************************
	// ****	Table Display flags						 ****
	// **************************************************

public:
	enum AffichSelEffectif {MODE_EFFECTIF_000 = 0, MODE_EFFECTIF};
	enum AffichSelMoyenne  {MODE_AVERAGEQUANTUM_P00 = 0, MODE_AVERAGEQUANTUM};

	const JList<RESULTTYPE> &	Get_Grandeurs() const 
									{ return m_Grandeurs; } ;
	void						Set_Grandeurs(const JList<RESULTTYPE> & Grandeurs) ;
	void						GetGrandeurString(CString & Str, RESULTTYPE Res);
	UINT						Get_TableStyle() const 
									{ return m_TableStyle; } ;
	void						Set_TableStyle(UINT TableStyle) ;
	void						Add_TableStyle(UINT TableStyle) ;
	void						Remove_TableStyle(UINT TableStyle) ;
	UINT						Get_ResultStyle() const 
									{ return m_ResultStyle; } ;
	void						Set_ResultStyle(UINT ResultStyle) ;

	// Gestion affichage marge erreur
	bool						CalculMargeErreur() const
									{ return m_CalculMargeErreur;} ;
	void						SetModeMargeErreur(bool AffichMargeErreur);

	// Aout 2013 : Force au calcul Quantitatif (m^me si item non quanti)
	bool						GetForceQuantiCalcul() const
									{ return m_ForceQuantiCalcul;} ;

	// Aout 2013 : Renvoie si Niche quantifiable (au moins 1 elmt ligne ou colonne quantifiable)
	bool						GetNicheQuantifiable() const
									{ return m_NicheQuantifiable;} ;

	void						SetNicheQuantifiable(bool ValNicheQuanti);

	void						SetForceQuantiCalcul(bool ForceQuantiCalcul);
	
	void						Add_ResultStyle(UINT ResultStyle) ;
	void						Remove_ResultStyle(UINT ResultStyle) ;
	UINT						Get_ClickMode() const
									{ return m_ClickMode; } ;
	void						Set_ClickMode(UINT ClickMode);
	UINT						Get_Calculation() const 
									{ return m_Calculation; } ;
	void						Set_Calculation(UINT Calculation) ;
	void						Add_Calculation(UINT Calculation) ;
	void						Remove_Calculation(UINT Calculation) ;

	bool						IsTableStyleSet(TABLESTYLE Style) const
									{ return m_TableStyle&Style; } ;

	// **************************************************
	// ****	Table Sort details						 ****
	// **************************************************

public:
	int			Get_SortUniv() const
					{ return m_SortUniv; } ;
	void		Set_SortUniv(int SortUniv) 
					{ m_SortUniv = SortUniv; m_Calculation |= CALC_SORT; m_bDirty = true; } ;
	int			Get_SortCol() const 
					{ return m_SortCol; } ;
	void		Set_SortCol(int SortCol) 
					{ m_SortCol = SortCol; m_Calculation |= CALC_SORT;  m_bDirty = true;} ;
	int 		Get_SortPage() const 
					{ return m_SortPage; } ;
	void		Set_SortPage(int SortPage) 
					{ m_SortPage = SortPage; m_Calculation |= CALC_SORT;  m_bDirty = true;} ;
	RESULTTYPE	Get_SortCalc() const 
					{ return m_SortCalc; } ;
	void		Set_SortCalc(RESULTTYPE SortCalc) 
					{ m_SortCalc = SortCalc; m_Calculation |= CALC_SORT;  m_bDirty = true;} ;
	BYTE		Get_SortDirection() const 
					{ return m_bSortDirection; } ;
	void		Set_SortDirection(BYTE SortDirection) 
					{ m_bSortDirection = SortDirection; m_Calculation |= CALC_SORT;  m_bDirty = true;} ;
	RESULTSTYLE	Get_SortedStyle() const 
					{ return m_SortedStyle; } ;
	void		Set_SortedStyle(RESULTSTYLE SortedStyle) 
					{ m_SortedStyle = SortedStyle; m_Calculation |= CALC_SORT;  m_bDirty = true;} ;
	bool		Get_SortType() const 
					{ return m_SortType; } ;
	void		Set_SortType(bool SortType) 
					{ m_SortType = SortType; m_Calculation |= CALC_SORT; m_bDirty = true;} ;
	void		ResetSort();

	// Mise à jour titre tableau croisé
	void		UpdateTitre();

	// Mise à jour du curseur tableau croisé
	void		UpdateCursor();

	// Positionne flag curseur tri
	void		SetCurseurTri(JBool Tri);

	// **************************************************
	// ***  Paramètres sélection tri courant
	// **************************************************
	//
	//Enumeration des différents modes de tri
	enum ModeTri {MODE_LIGNE = 0, MODE_BLOC};

	// Enumération des différents type de tri
	enum TypeTri {TYPE_BASE = 0, TYPE_DECROISSANT, TYPE_CROISSANT};

	// Positionne Mode de tri courant (mode ligne, mode bloc critères)
	void		Set_ModeTri(BYTE ModeTri)
					{m_ModeTri = ModeTri; } ;	

	// Récupère Mode de tri courant 
	BYTE	    Get_ModeTri() const 
					{return	m_ModeTri; } ;

	// Récupère libellé mode de tri
	CString     Get_LibModeTri() const;
	
	// Positionne Type de tri courant (sans tri, tri croissant, tri décroissant)
	void		Set_TypeTri(BYTE TypeTri)
					{m_TypeTri = TypeTri; } ;

	// Récupère Type de tri courant
	BYTE		Get_TypeTri() const
					{return m_TypeTri; } ;	

	// Récupère libellé type de tri
	CString     Get_LibTypeTri() const;

	// Positionne Ident courant grandeur pour tri (de type ResultType)
	void		Set_GrandeurTri(RESULTTYPE Grandeur)
					{m_IdTriGrandeurs = Grandeur; } ;

	// Récupère Ident courant grandeur pour tri
	RESULTTYPE	Get_GrandeurTri() const
					{return m_IdTriGrandeurs; } ;

	// Récupère libellé grandeur tri
	CString     Get_LibGrandeurTri() const;

	// Positionne Ident courant résultat pour tri (Grandeur, H%, V%, Autres...)
	void		Set_ResultTri(RESULTSTYLE Resultat)
					{m_IdTriResultats = Resultat; } ;

	// Récupère Ident courant résultat pour tri
	RESULTSTYLE	Get_ResultTri()
					{return m_IdTriResultats; } ;

	// Récupère libellé résultat tri
	CString     Get_LibResultTri() const;

	// **************************************************
	// ***  Paramètres sélection params de surlignage
	// **************************************************
	//
	// Enumération des différents type de tri
	// TYPE_SUP			: Valeur Supérieure strictement
	// TYPE_SUP_EGAL	: Valeur Supérieure ou égale
	// TYPE_INF			: Valeur Inférieure strictement
	// TYPE_INF_EGAL	: Valeur Inférieure ou égale
	// TYPE_IN_BORNES	: Valeur entre les 2 bornes (bornes comprises)	
	// TYPE_OUT_BORNES	: Valeur en dehors des 2 bornes
	enum TypeParamVal {TYPE_SUP = 0, TYPE_SUP_EGAL = 1, TYPE_INF = 2, TYPE_INF_EGAL = 3, TYPE_IN_BORNES = 4, TYPE_OUT_BORNES = 5};

	// Mise à jour Map des cellules à surligner via la liste des opérateurs surlignage
	void UpdateSurlignage(IItemTable * pIItemTable, CAnCrossTable *pCrossTable);

	// **************************************************
	// ****	Table Surlignage details				 ****
	// **************************************************

	RESULTTYPE	Get_SurlignageCalc() const 
					{ return m_SurlignageCalc; } ;
	void		Set_SurlignageCalc(RESULTTYPE SurlignageCalc) 
					{ m_SurlignageCalc = SurlignageCalc; m_Calculation |= CALC_SURLIGNAGE;  m_bDirty = true;} ;
	RESULTSTYLE	Get_SurlignageStyle() const 
					{ return m_SurlignageStyle; } ;

	void		Set_SurlignageStyle(RESULTSTYLE SurlignageStyle); 

	// void		Set_SurlignageList(JVector<CTableKey> &List, IItemTable * pIItemTable);
	void		Set_SurlignageList(JVector<CTableKey> &List, CCrossTable * pIItemTable);
	void		ResetSurlignage();

	// Enumération des différents type d'affichage résultat dans le grid (en ligne, en colonne ou en 2 x 2)
	enum TypeAffichResult {AFFRESULT_LIGNE = 1, AFFRESULT_COLONNE = 2, AFFRESULT_2X2 = 3};

	// Enumération des différents modes de calcul moyenne
	enum ModeMoyenne {MOY_TABLEAU = 0, MOY_LIGNE = 1, MOY_COLONNE = 2};

	// Enumération du mode calcul moyenne (avec ou sans tenir compte des 0)
	enum ModeCalcMoyenne {CALC_MOY_0_INCLUS = 0, CALC_MOY_0_EXCLUS = 1};

	// Flag indiquant quel mode est pris en compte pour calcul des moyennes
	// Mode Univers	: ça sera l'univers numérisé qui sera pris en compte pour les valeurs   >>> Mode par défaut
	// Mode Ligne	: ça sera les lignes items numérisés qui seront prise en compte pour les valeurs
	JUnt32	m_ModeMoyenne;

	// Flag indiquant la manière de calculer la moyenne (en tenant compte ou pas des valeurs 0)
	JUnt32  m_ModeCalculMoyenne;

	// Option Cumul Quanti
	JBool	m_bCumulQuanti; 

	// Ident de la variable moyennabe à prendre en compte
	JInt32	m_IdValMoyennable;

	// Indice de calcul courant
	UINT  m_ModeIndiceGlobal;

	// Item Wrapper correspondant à la variable moyennable;
	CItemWrapper	* m_pItemWrapperMoy;

	// Liste des items variables moyennables sélectionnées
	LIST_ITEM_PUPITRE		m_AllVarMoyennables;

	// Test si grandeur moyenne (et/ou Moyenne(000)) sélectionnée en tant que grandeur de calcul
	bool    GrandeurMoyenneActive();

	// Flag indiquant si au moins 1 ligne moyenne
	bool m_AtLeastOneRowAverage;

	// Au moins 1 elmt grandeur moyennable sélectionné
	bool m_ElmtGrandeurMoyennableSel;

	// Mode découpage pour l'export
	enum AffichModeDecoupage  {MODE_SANS_DECOUPAGE = 0, MODE_AVEC_DECOUPAGE};
	int	m_ModeDecoupage;

public:
	// Load the newly selected terrains
	const JList<JInt32> &					Get_SelectedTerrains() { return m_SelectedTerrains; } ;	

	// Renvoi le nombre de terrains sélectionnées
	const JUnt32							Get_NbSelectedTerrains() {return m_SelectedTerrains.GetCount(); } ;

	// Renvoi le nombre de grandeurs sélectionnées
	const JUnt32							Get_NbSelectedGrandeurs() {return m_MapGrandeursDispos.GetCount(); } ;

	void									Set_SelectedTerrains(const JList<JInt32> & SelList, bool bChanged);
	virtual bool							LoadTerrains(const JUnt32 SourceId, const JList<UINT> & TerrainIds, const JList<UINT> & Segments, const JUnt32 TypePoidsId);
	UINT									Get_SourceId() const
												{ return m_SourceId; };
	void									Set_SourceId(UINT SourceId)
												{ m_SourceId = SourceId; } ;
	const JList<UINT> &						Get_TerrainId() const
												{ return m_TerrainId; };
	void									Set_TerrainId(const JList<UINT> & TerrainId) ;
	UINT									Get_TypePoidsId() const
												{ return m_TypePoidsId; };
	void									Set_TypePoidsId(UINT TypePoidsId)
												{ m_TypePoidsId = TypePoidsId; } ;
	const JList<UINT> &						Get_SegmentsId() const 
												{ return m_SegmentsId; } ;
	void									Set_SegmentsId(const JList<UINT> & SegmentsId) ;
	const CItemConstruit *					Get_SelectedCible() const
												{ return m_pSelectedCible; } ;
	void									Set_SelectedCible(const CItemConstruit * pSelectedCible) ;
	const JList<const CItemConstruit *> &	Get_ListCibles() const
												{ return m_ListCibles; } ;
	void									Set_ListCibles(const JList<const CItemConstruit *> & ListCibles) ;
	JMap<UINT, PTR_TERRAINDATA> &			Get_ListTerrainData()
												{ return m_pTerrainData; } ;
	void									Set_ListTerrainData(JMap<UINT, PTR_TERRAINDATA> & TerrainData) ;
	CTerrain *								Get_TerrainTemoin() const 
												{ return m_pTerrainTemoin; } ;
    void									Set_TerrainTemoin(CTerrain * TerrainTemoin)  
												{ m_pTerrainTemoin = TerrainTemoin; } ;

	void									Set_TerrainTemoin_ViaRef();	

	// Gestion Source et Terrain De Référence
	UINT									Get_SourceIdRef() const
												{ return m_SourceIdRef; };
	void									Set_SourceIdRef(UINT SourceId)
												{ m_SourceIdRef = SourceId; } ;

	UINT									Get_TerrainIdRef() const
												{ return m_TerrainIdRef; };
	void									Set_TerrainIdRef(UINT TerrainId)
												{ m_TerrainIdRef = TerrainId; } ;

	// Récupération de l'item ensemble
	CItemConstruit *						Get_ItemEnsemble() const  
												{ return CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemEnsemble() ; };

	// Set seuil levels
	virtual void							Set_Seuils();

	// Récupération via doc des seuils limite calcul moteur
	unsigned int							Get_SeuileLimit() const { return m_SeuilLimit; } ;
	unsigned int							Get_SeuileAvert() const { return m_SeuilAvert; } ;

	// Récupération via doc des seuils marge d'erreur utilisateur et application
	unsigned int							Get_Seuil_MargeErreur_Util()	const { return m_SeuilMargeErreurUtil; } ;
	unsigned int							Get_Seuil_MargeErreur_Appli()	const { return m_SeuilMargeErreurAppli; } ;

	// **************************************************
	// **** List of Dimension Items Row, Col, Page   ****
	// **************************************************

	// Uniquement pour test
	int m_NoRowCur;
	int m_NoColCur;
	void AffichRowCol();


	// N° d e la cellule survolée
	long							m_NoCellSurvol;



public:
	// Transpose the rows with the columns
	void    				TransposeRowCol();
	int						Get_RowIndex() const
								{ return m_RowIndex; } ;
	int						Get_ColIndex() const 
								{ return m_ColIndex; } ;
	int						Get_PageIndex() const 
								{ return m_PageIndex; } ;
	bool					GetAllItems(LIST_ITEM_PUPITRE & All);
	bool					SetAllItems(LIST_ITEM_PUPITRE & All);

	// Returns the row items
	const CONST_LIST_ITEM_PUPITRE &	GetRowItems() const	{ return m_Dim[m_RowIndex]; };
	bool					GetRowItems(CONST_LIST_ITEM_PUPITRE & Rows) const;
	bool					SetRowItems(CONST_LIST_ITEM_PUPITRE & Rows, int DType);
	int						GetRowHdrHeight(int Row) { if(Row < 2) return m_RowHdrHeight[Row];
													else return 16; } ;
	void					SetRowHdrHeight(int Row, int Height);
	int						GetRowDataType() { return m_DType[m_RowIndex]; } ;
	void					SetRowDataType(int DType) { m_DType[m_RowIndex] = DType; m_bDirty = true; } ;

	// Returns the column items
	const CONST_LIST_ITEM_PUPITRE &	GetColItems() const	{ return m_Dim[m_ColIndex]; };
	bool					GetColItems(CONST_LIST_ITEM_PUPITRE & Cols) const ;
	bool					SetColItems(CONST_LIST_ITEM_PUPITRE & Cols, int DType);
	int						GetColHdrWidth(int Col) { if(Col < 2) return m_ColHdrWidth[Col];
													else return 45; } ;
	void					SetColHdrWidth(int Col, int Width);
	int						GetColDataType() { return m_DType[m_ColIndex]; } ;
	void					SetColDataType(int DType) { m_DType[m_ColIndex] = DType; m_bDirty = true;} ;

	// Returns the page items
	const CONST_LIST_ITEM_PUPITRE &	GetPageItems() const { return m_Dim[m_PageIndex]; };
	bool					GetPageItems(CONST_LIST_ITEM_PUPITRE & Pages) const ;
	bool					SetPageItems(CONST_LIST_ITEM_PUPITRE & Pages, int DType);

	// returns the current page
	int						GetCurPage() const { return m_CurPage; } ;
	void					SetCurPage(int CurPage) { m_CurPage = CurPage; m_bDirty = true; } ;
	int						GetPageDataType() { return m_DType[m_PageIndex]; } ;
	void					SetPageDataType(int DType) { m_DType[m_PageIndex] = DType; m_bDirty = true;} ;

	// return the progres control
	CProgressCtrl &			GetProgressCtrl() { return m_ProgressCtrl; } ;

	// Returns all the items
// protected:
	LIST_ITEM_PUPITRE &		GetAllItems() { return m_AllItems; };

	LIST_ITEM_PUPITRE &		GetAllVariablesMoyennables() { return m_AllVarMoyennables; };
	bool					SetAllVariablesMoyennables(LIST_ITEM_PUPITRE & All);

public:
	int 					Get_BaseColItem() const { return m_BaseColItem; } ;
	void					Set_BaseColItem(int BaseColItem) ;
	int 					Get_BaseRowItem() const { return m_BaseRowItem; } ;
	void					Set_BaseRowItem(int BaseRowItem) ;
	const JList<CCalcItemParams *> &	Get_UniverseParams() const 	{ return m_UniverseParams; } ;
	void								Set_UniverseParams(const JList<CCalcItemParams *> & UniverseParams) ;

	// Afin de gérer les changements de seuil, pour recalcul avec nouveaux seuils
	void SetLastCalculation() {m_LastCalculation = m_Calculation;};
	void SetCalculation() {m_Calculation = m_LastCalculation;};

	// **************************************************
	// ****	All Parameters   						 ****
	// **************************************************

	CDlgAnalyseur &					m_DlgAnalyseur;

	// Liste sélection en cours sur tree quanti des elmts à moyenner
	JMap<DWORD,JUnt32>				m_MapSelTreeQuanti;

protected:
	// Grid progress control
	CProgressCtrl &					m_ProgressCtrl;

	// Analyseur dialogue
	// CDlgAnalyseur &				m_DlgAnalyseur;	

	// Surlignage Dialog
	CDlgSurlignage &				m_DlgSurlignage;

	// The document name
	CString							m_DocTitle;
	CString							m_DocPath;
	bool							m_bDirty;
	int								m_NoVersion;
	// Results to display
	JList<RESULTTYPE>				m_Grandeurs;
	// Table display styles
	UINT							m_TableStyle;
	// Result styles
	UINT							m_ResultStyle;
	// Calcul ou pas marge d'erreur
	bool							m_CalculMargeErreur;

	// Force calcul Quanti
	bool							m_ForceQuantiCalcul;

	// Niche quantifiable ??? (Aout 2013)
	bool							m_NicheQuantifiable;

	// Left click mode
	UINT							m_ClickMode;
	// Calculation Flag
	UINT							m_Calculation;
	// Last Calculation Flag
	UINT							m_LastCalculation;
	// The sort universe
	int								m_SortUniv;
	// The sort column
	int								m_SortCol;
	// The sort page
	int								m_SortPage;
	// The sort calculation base
	RESULTTYPE						m_SortCalc;
	// The sort direction
	BYTE							m_bSortDirection;
	// The sorted style
	RESULTSTYLE						m_SortedStyle;
	// The sort type
	bool							m_SortType;
	// Source Id
	UINT							m_SourceId;
	// Terrain Ids
	JList<UINT>						m_TerrainId;

	// Source et terrain de référence
	UINT							m_SourceIdRef;
	UINT							m_TerrainIdRef;

	// Type poids Id
	UINT							m_TypePoidsId;
	// Segment Ids
	JList<UINT>						m_SegmentsId;
	// Pointer to selected target
	const CItemConstruit *			m_pSelectedCible;
	// List of targets
	JList<const CItemConstruit *>	m_ListCibles;
	// Pointers to loaded terrains
	JList<JInt32>					m_SelectedTerrains;
	JMap<UINT, PTR_TERRAINDATA>		m_pTerrainData;
	// default terrain
	CTerrain *						m_pTerrainTemoin;
	// Dimension Row index
	int								m_RowIndex;
	// Dimension Col index
	int								m_ColIndex;
	// Dimension Page index
	int								m_PageIndex;
	// List of all Dimension Items
	LIST_ITEM_PUPITRE				m_AllItems;

	// List of Dimension Items by Row, Col and Page  (1/ Rows  2/ Columns  3/Pages 
	CONST_LIST_ITEM_PUPITRE			m_Dim[3];
	int								m_DType[3];
	int								m_BaseColItem;
	int								m_BaseRowItem;
	JList<CCalcItemParams *>		m_UniverseParams;
	// Current page
	int								m_CurPage;

	// Dimensions Headers Lignes
	int								m_RowHdrHeight[2];

	// Dimensions Headers Colonnes
	int								m_ColHdrWidth[2];

	// The surlignage calculation base
	RESULTTYPE						m_SurlignageCalc;
	// The surlignage style
	RESULTSTYLE						m_SurlignageStyle;

	// Les seuils pour moteur
	unsigned int					m_SeuilLimit;
	unsigned int					m_SeuilAvert;

	// Les seuils de marge d'erreur
	unsigned int					m_SeuilMargeErreurUtil;
	unsigned int					m_SeuilMargeErreurAppli;


	/// Current Mode Pointer mode ligne, et mode colonne, et mode page
	CAbstractMode	*				m_pModeLigne;
	CAbstractMode	*				m_pModeColonne;
	CAbstractMode	*				m_pModePage;

	// Liste sélection en cours sur tree theme (index thématique)
    JMap<DWORD,JUnt32>				m_MapSelTreeTheme;

	// Liste sélection en cours sur tree quanti des elmts à moyenner
	// JMap<DWORD,JUnt32>				m_MapSelTreeQuanti;

	// Les sélections courantes
	BYTE							m_ModeTri;					// Mode de tri courant (par défaut mode ligne)
	BYTE							m_TypeTri;					// Type de tri courant (par défaut retour sans tri)
	RESULTTYPE						m_IdTriGrandeurs;			// Ident courant grandeur pour tri
	RESULTSTYLE						m_IdTriResultats;			// Ident courant résultat pour tri

	// Mode affichage résultat en ligne ou en 2x2
	int							    m_ModeResult;				// 1 en ligne, 2 en colonne, 3 en 2 x 2 (par défaut en ligne)

public:
	// Récupère aussi les pointeurs via les différents mode ligne, colonne, page via pupitre
	void SetModeLigne	(CAbstractMode	*pCurMode); 
	void SetModeColonne	(CAbstractMode	*pCurMode); 
	void SetModePage	(CAbstractMode	*pCurMode); 

	// Positionne dans pupitre les modes courants ligne, colonne, page
	void GetModeLigne	(CAbstractMode	*pCurMode); 
	void GetModeColonne	(CAbstractMode	*pCurMode); 
	void GetModePage	(CAbstractMode	*pCurMode); 

	// Récupère le mode d'affichage
	void SetModeAffichResult(int ModeAffichResult)
					{m_ModeResult = ModeAffichResult; };

	// Positionne le mode d'affichage résultat
	int GetModeAffichResult() const
					{return m_ModeResult; } ;

	// Recup Map Etat tree theme
	JMap<DWORD,JUnt32> * GetEtatTreeTheme();

	// Recup Map Etat tree quantis à moyenner
	JMap<DWORD,JUnt32> * GetEtatTreeQuanti();

	// Initialise le Map déploiement et sélection index thématique
	void ResetEtatTreeTheme(bool Init);

	// Initialise le Map déploiement et sélection quantis à moyenner
	void ResetEtatTreeQuanti(bool Init);

	// Les elmts pour paramétrages tri
	JMap  <JUnt32, CString>			m_MapGrandeursDispos;		// les grandeurs sélectionnées dans analyse de l'offre
	JMap  <JUnt32, CString>			m_MapResultCellDispo;		// les résultats cellules valides

	// Les elmts cellules à surligner
	JMap  <CKeyRowCol, COLORREF>	m_MapCellTotSurlign;		// Map des cellules à surlignées	

	// Les actions surlignage en cours
	JList <COpeSurlignage>			m_Lst_Ope_Surlignages;		// Liste des opérations surlignages
	
	// Récup les paramètres par défaut
	void SetParamDefSurlignage(COpeSurlignage *pDefault_OpeSurlignage);

	// L'opération surlignage par défaut
	COpeSurlignage *				m_pDefault_OpeSurlignage;	

	// Set & Get Type Affichage pour sélection effectif et moyenne dans la liste des grandeurs
	JInt32							GetTypeAffichSelEffectif(){return m_TypeAffichSelEffectif;};
	void							SetTypeAffichSelEffectif(JInt32 Type){m_TypeAffichSelEffectif = Type;};

	JInt32							GetTypeAffichSelMoyenne(){return m_TypeAffichSelMoyenne;};
	void							SetTypeAffichSelMoyenne(JInt32 Type){m_TypeAffichSelMoyenne = Type;};	

protected:
	// Les modes pour les grandeurs spécifiques (Effectif et Moyenne)
	JInt32							m_TypeAffichSelEffectif;
	JInt32							m_TypeAffichSelMoyenne;


};
