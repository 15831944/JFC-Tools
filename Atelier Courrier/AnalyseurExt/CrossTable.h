#pragma once

#include "Terrain.h"
#include "IItemTable.h"
#include "ItemWrapper.h"
#include "AnCrossTable.h"
#include "CoordRowCol.h"
#include "ICellResult.h"
#include "AnalyseurDoc.h"
#include "ItemConstruit.h"
#include "BtnST.h"

class CDlgAnalyseur;
class CDlgItems;


#define COL_HEADERS		2
#define ROW_HEADERS		2

#define MargeErreurMax		75

typedef JMap<CCoordRowCol,PTR_ICELLRESULT> MAPCELLRESULTS;
typedef std::auto_ptr< MAPCELLRESULTS > PTR_MAPCELLRESULTS;

#define IDM_AN_GLOBAL_STRUCTURE         32782

class CCrossTable : public IItemTable
{
public:
	// Setup Cross Table Grid display
	// virtual bool SetupGrid(CAnCrossTable * Grid, CAnalyseurDoc *pDoc, CButtonST* pBut_Base100, CButtonST* pBut_Surlignage);
	virtual bool SetupGrid(CAnCrossTable * Grid, CAnalyseurDoc *pDoc, BOOL Base100, CButtonST* pBut_Surlignage);

	// Gets the row col Grid reference style
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle & style, GXModifyType mt, int nType);

	// Idem fonction GetStyleRowCol, mais en globalité OPTIM 2.7
	virtual BOOL GetTotStyleRowCol(ROWCOL nRowBegin, ROWCOL nRowEnd, ROWCOL nColBegin, ROWCOL nColEnd, CGXStyle& style,	GXModifyType mt, int nType);

	virtual BOOL TypeRowMoyennable(ROWCOL nRow, ROWCOL nCol);

	// Called when page combo has changed its selection
	virtual void PageChanged(int Page);

	// Called when right mouse buton clicked
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Called when left mouse buton clicked
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Called when left button is down
	// virtual BOOL LButtonDown(UINT nFlags, CPoint point, UINT nHitState);

	// Column tracking
	virtual BOOL OnTrackColWidth(ROWCOL nCol);
	// Row tracking
	virtual BOOL OnTrackRowHeight(ROWCOL nRow);

	// Mouse move over
	virtual BOOL OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	
	// Gets the cell results
	virtual bool	GetCellValue(double & Result, RESULTTYPE Type, int Ri, int Ci, int Universe, RESULTSTYLE Style, int PageItem, int BaseColItem, int BaseRowItem, bool & DivZero, double & NbCas, bool & MargeErreurDepasse, double & MargeErreur, double & IntervalleConfiance);

	// Gets the Formatted Cell Result String
	virtual void	GetFormattedResult(CString & ResultOut, double ResultIn, RESULTSTYLE Style, RESULTTYPE Type, int RowItem, int ColItem, int BaseRowItem, int BaseColItem, bool DivZero, double NbCas, bool MargeErreurDepasse, bool ItemQuanti, bool RowMoyennable, bool ColMoyennable);

	// Gets the cell header string
	virtual void	GetColCellString(CString & St, CELLTYPE t,int Universe, int i, int c);
	virtual void	GetRowCellString(CString & St, CELLTYPE t,int Universe, int i, int c);
	virtual void	GetPageCellString(CString & St, CString & St2, int Page);

	// Gets the Cell coordinates and type
	virtual CELLTYPE	GetRowCoordinates(int RawRow,  int & Item, int & Universe, int & Calc, int & Value);
	virtual CELLTYPE	GetColCoordinates(int RawCol,  int & Item, int & Universe, int & Calc, int & Value);

	// Recupere nombre de grandeurs utilises
	virtual int         GetItemColCount() {return m_ItemColCount; };
	virtual int         GetItemRowCount() {return m_ItemRowCount; };

	// Récupere les infos nb terrains et nb grandeurs en header col ou header ligne
	virtual JUnt32 Get_NbHeaderColTerrain();
	virtual JUnt32 Get_NbHeaderColGrandeur();
	virtual JUnt32 Get_NbHeaderRowTerrain();
	virtual JUnt32 Get_NbHeaderRowGrandeur();

	// Renvoi le nombre de ligne et colonne par cellule résultat (selon type affichage et grandeurs sélectionnées)
	virtual void GetResultRowColCount(JUnt32 &ResultRowCount, JUnt32 &ResultColCount);
	
	// Récupère catégorie d'une entete ligne (+ infos nombre de modalités constituantes)
	virtual void	GetRowCellCategorie(CELLTYPE t, int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemMoyennable);	

	// Récupère catégorie d'une entete colonne (+ infos nombre de modalités constituantes)
	virtual void	GetColCellCategorie(CELLTYPE t, int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemMoyennable);	

	// Récupère info row moyennable 
	virtual void    GetRowCellMoyennable(CELLTYPE t, int i, int c, bool & ItemMoyennable, ITEMTYPE RowType);

	// Récupère info col moyennable 
	virtual void    GetColCellMoyennable(CELLTYPE t, int i, int c, bool & ItemMoyennable, ITEMTYPE ColType);

	// Calculate the items
	virtual bool	CalculateItems(bool Redraw = true);

	virtual void RedrawGrid();

	// Exports the table to the given file
	virtual bool Export(CFile & exportFile);

	// Copies to the clipboard
	virtual bool CopyToClipboard();

	// Copies the table to the clipboard
	virtual bool CopyAllToClipboard();

	// Index premiere ligne tableau affilié avec élmt item
	virtual JUnt32 FirstIndexItemRow();

	// Index premiere colonne tableau affilié avec élmt item
	virtual JUnt32 FirstIndexItemCol();

	// Returns the Result Style from inner cell coordinates
	virtual RESULTSTYLE	GetResultStyle(int Rv,int Cv);

	// Returns the Result Type from inner cell coordinates
	virtual RESULTTYPE	GetResultType(int Rv,int Cv);

	// Recup dimension hauteur row
	virtual int GetRowHdrHeight(int Row)
	{
		if (m_pDoc) 
			return m_pDoc->GetRowHdrHeight(Row);
		else
			return 0;
	};

public:
	CCrossTable(void);
	virtual ~CCrossTable(void);

	// Gets the Row Types
	JList<ITEMTYPE> &			GetItemRowTypes() { return m_Item_Row_Types; }
	// Gets the Col Types
	JList<ITEMTYPE> &			GetItemColTypes() { return m_Item_Col_Types; }
	// Gets the Page Types
	JList<ITEMTYPE> &			GetItemPageTypes() { return m_Item_Page_Types; }

	int GetRowCurHeader() {return m_RowCurHeader;};
	int GetColCurHeader() {return m_ColCurHeader;};

	// Returns the Page Item
	int							GetPageItem(int Item);

	/*
	// Returns the Result Style from inner cell coordinates
	RESULTSTYLE							GetResultStyle(int Rv,int Cv);

	// Returns the Result Type from inner cell coordinates
	RESULTTYPE							GetResultType(int Rv,int Cv);
	*/

protected:
	// Sets the base Row and Col
	bool								SetBase(int Row, int Col);
	// Transposes Grid rows and Cols
	virtual bool						Transpose();
	// Format the data result string
	void								Format(CString &Data);
	// Gets the caculation row params
	JList<CCalcItemParams> &			GetRowCalcParams(int Universe);
	// Gets the row item params
	JList<CCalcItemParams> &			GetRowItParams(int Universe);
	// Get the Exclusive row item params
	JList<CCalcItemParams> &			GetRowExParams(int Universe);
	// Get the Duplique row item params
	JList<CCalcItemParams> &			GetRowDuParams(int Universe);
	// Gets the calculation Col params
	JList<CCalcItemParams> &			GetColCalcParams(int Universe);
	// Gets the Col item params
	JList<CCalcItemParams> &			GetColItParams(int Universe);
	// Gets the exclusive Col item params
	JList<CCalcItemParams> &			GetColExParams(int Universe);
	// Gets the duplique Col item params
	JList<CCalcItemParams> &			GetColDuParams(int Universe);
	// Gets the caculation page params
	JList<CCalcItemParams> &			GetPageCalcParams(int Universe);
	// Gets the page item params
	JList<CCalcItemParams> &			GetPageItParams(int Universe);
	// Gets the exclusive page item params
	JList<CCalcItemParams> &			GetPageExParams(int Universe);
	// Gets the duplique page item params
	JList<CCalcItemParams> &			GetPageDuParams(int Universe);
	// Returns the Target Params
	CCalcItemParams *					GetTargetCalcParams(int Univers);
	// Get the cell results for a given universe
	MAPCELLRESULTS *					GetCellResults(int Universe);
	// Calculates the given dimension
	bool								CalculateDimensionIt(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt);
	// Calculates the given dimension
	bool								CalculateDimensionEx(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt, JList<CCalcItemParams> & ParamsEx);
	bool								CalculateDimensionDu(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt, JList<CCalcItemParams> & ParamsDu);
	// Calculates the Col items
	bool								CalculateCols();
	bool								CalculateCols(int Univers);
	int									CalcColSize();
	// Calculates the row items
	bool								CalculateRows();
	bool								CalculateRows(int Univers);
	int									CalcRowSize();
	// Calculates the cell items
	bool								CalculateCells();
	// Calculates the page items
	bool								CalculatePages();
	bool								CalculatePages(int Univers);
	int									CalcPageSize();
	// Calculates the univers items
	// bool								CalculateUniverse();
	void CalculateUniverse();

	// Calculate all the universe targets	
	bool								CalculateTargets();
	// Calculate the highlight values
	bool								CalculateSurlignage();
	// Calculates the number of cells for each row and column
	void								CalcRowColCells();

	// Data dans Grid
	void								SetGridData();

	// Sorts the columns
	bool								SortColumn();

	// Resets the sort parameters
	void								ResetSort();

protected:
	// Pointer to the document
	CAnalyseurDoc *						m_pDoc;

	// Pointer to the grid object
	CAnCrossTable *					m_pGrid;
	// Univers ItemParams
	JList<CCalcItemParams *>		m_UniversParams;

	// Univers variable moyennable

	// Univers Targets
	JList<PTR_CALCITEMPARAMS>		m_UniversTarget;
	// List of Dimension Item params
	JList< JList<CCalcItemParams> > m_Dim_ItemParams[3];
	// List of Dimension Exclusive Item params
	JList< JList<CCalcItemParams> > m_Dim_ExItemParams[3];
	// List of Dimension Duplique  Item params
	JList< JList<CCalcItemParams> > m_Dim_DuItemParams[3];

	// Col Variables
	int								m_Result_Col_Count;
	int								m_Col_StartItem;
	JList<ITEMTYPE>					m_Item_Col_Types;
	unsigned int					m_ItemColCount;
	unsigned int					m_FixedCols;
	unsigned int					m_NbCols;
	ROWCOL							m_CurCol;
	JList<int>						m_ItemColIndex;
	JList<CELLTYPE>					m_Col_Hdr_Type;
	int								m_ColWidth[2];

	// Row variables
	int								m_Result_Row_Count;
	int								m_Row_StartItem;
	JList<ITEMTYPE>					m_Item_Row_Types;
	unsigned int					m_ItemRowCount;
	unsigned int					m_FixedRows;
	unsigned int					m_NbRows;
	ROWCOL							m_CurRow;
	JList<int>						m_ItemRowIndex;
	JList<CELLTYPE>					m_Row_Hdr_Type;

	// Page Variable
	int								m_BasePageItem;
	int								m_Page_StartItem;
	JList<ITEMTYPE>					m_Item_Page_Types;
	JList<int>						m_ItemPageIndex;
	long							m_NbPages;
//	int								m_CurPage;
	CString							m_PageChoice;

	JList<RESULTSTYLE>				m_CellResultStyles;
	// Cell results MAP
	JList< PTR_MAPCELLRESULTS >		m_CellResults;
	// The sorted Row Item index
	JArray<int>						m_SortedRowIndex;

	// CButtonST*						m_pBut_Base100;   (voir si non retour en arrière)
	BOOL							m_Base100;

	CButtonST*						m_pBut_Surlignage;

	// Ligne et colonne header en cours
	unsigned int					m_RowCurHeader;
	unsigned int					m_ColCurHeader;

	// Infos terrains et grandeurs
	JUnt32							m_NbHeaderColTerrain;
	JUnt32							m_NbHeaderColGrandeur;
	JUnt32							m_NbHeaderRowTerrain;
    JUnt32							m_NbHeaderRowGrandeur;

};
