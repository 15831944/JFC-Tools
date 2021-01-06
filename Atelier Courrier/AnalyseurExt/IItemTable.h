#pragma once

#include "AnalyseurEnums.h"

class CAnalyseurDoc;
class CAnCrossTable;
class CButtonST;

class IItemTable
{
public:
	IItemTable(void);
	virtual ~IItemTable(void);

	// Setup Cross Table Grid display
	virtual bool SetupGrid(CAnCrossTable * Grid, CAnalyseurDoc *pDoc, BOOL Base100, CButtonST* pBut_Surlignage) = 0;

	// Gets the row col Grid reference style
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle & style, GXModifyType mt, int nType) = 0;

	// Idem fonction GetStyleRowCol, mais en globalité OPTIM 2.7
	virtual BOOL GetTotStyleRowCol(ROWCOL nRowBegin, ROWCOL nRowEnd, ROWCOL nColBegin, ROWCOL nColEnd, CGXStyle& style,	GXModifyType mt, int nType) = 0;

	// Est ce une ligne moyenable (pour reduire hauteur ligne à 0 quand nécessaire = mode hide row)
	virtual BOOL TypeRowMoyennable(ROWCOL nRow, ROWCOL nCol) = 0;

	// Transposes Grid rows and columns
	virtual bool Transpose() = 0;

	// Called when page combo has changed its selection
	virtual void PageChanged(int Page) = 0;

	// Called when left mouse buton clicked
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt) = 0;
	// Called when right mouse buton clicked
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt) = 0;

	// Called when left button is down
	// virtual BOOL LButtonDown(UINT nFlags, CPoint point, UINT nHitState) = 0;
	
	// Column tracking
	virtual BOOL OnTrackColWidth(ROWCOL nCol) = 0;
	// Row tracking
	virtual BOOL OnTrackRowHeight(ROWCOL nRow) = 0;
	// Mouse move over
	virtual BOOL OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt) = 0;
	// Exports the table to the given file
	virtual bool Export(CFile & exportFile) = 0;
	// Copy selected cells to the clipboard
	virtual bool CopyToClipboard() = 0;
	// Copy the table to the clipboard
	virtual bool CopyAllToClipboard() = 0;
	// Gets the cell results
	virtual bool	GetCellValue(double & Result, RESULTTYPE Type, int Ri, int Ci, int Universe, RESULTSTYLE Style, int PageItem, int BaseColItem, int BaseRowItem, bool & DivZero, double & NbCas, bool &MargeErreurDepasse, double &MargeErreur, double &IntervalleConfiance) = 0;
	// Gets the Formatted Cell Result String
	virtual void	GetFormattedResult(CString & ResultOut, double ResultIn, RESULTSTYLE Style, RESULTTYPE Type, int RowItem, int ColItem, int BaseRowItem, int BaseColItem, bool DivZero, double NbCas, bool MargeErreurDepasse, bool ItemQuanti = false, bool RowMoyennable = false, bool ColMoyennable = false) = 0;
	// Gets the cell header string
	virtual void	GetColCellString(CString & St, CELLTYPE t,int Universe, int i, int c) = 0;
	virtual void	GetRowCellString(CString & St, CELLTYPE t,int Universe, int i, int c) = 0;
	virtual void	GetPageCellString(CString & St, CString & St2, int Page) = 0;

	// Gets the Cell coordinates and type
	virtual CELLTYPE	GetRowCoordinates(int RawRow,  int & Item, int & Universe, int & Calc, int & Value) = 0;
	virtual CELLTYPE	GetColCoordinates(int RawCol,  int & Item, int & Universe, int & Calc, int & Value) = 0;

	// Recupere nombre de grandeurs utilises
	virtual int         GetItemColCount() = 0;
	virtual int         GetItemRowCount() = 0;

	virtual int			GetRowCurHeader() = 0;
	virtual int			GetColCurHeader() = 0;

	virtual int			GetPageItem(int Item) = 0;

	// Récupere les infos nb terrains et nb grandeurs en header col ou header ligne
	virtual JUnt32		Get_NbHeaderColTerrain()	= 0;
	virtual JUnt32		Get_NbHeaderColGrandeur()	= 0;
	virtual JUnt32		Get_NbHeaderRowTerrain()	= 0;
	virtual JUnt32		Get_NbHeaderRowGrandeur()	= 0;

	// Renvoi le nombre de ligne et colonne par cellule résultat (selon type affichage et grandeurs sélectionnées)
	virtual void GetResultRowColCount(JUnt32 &ResultRowCount, JUnt32 &ResultColCount) = 0;

	// Récupère catégorie d'une entete ligne
	virtual void	GetRowCellCategorie(CELLTYPE t, int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemQuanti) = 0;	

	// Récupère catégorie d'une entete colonne
	virtual void	GetColCellCategorie(CELLTYPE t, int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemQuanti) = 0;	

	// Récupère info row moyennable 
	virtual void    GetRowCellMoyennable(CELLTYPE t, int i, int c, bool & ItemMoyennable, ITEMTYPE RowType) = 0;

	// Récupère info col moyennable 
	virtual void    GetColCellMoyennable(CELLTYPE t, int i, int c, bool & ItemMoyennable, ITEMTYPE ColType) = 0;

	virtual bool	CalculateItems(bool Redraw = true) = 0;

	virtual void RedrawGrid() = 0;

	// Index premiere ligne tableau affilié avec élmt item
	virtual JUnt32 FirstIndexItemRow() = 0;

	// Index premiere colonne tableau affilié avec élmt item
	virtual JUnt32 FirstIndexItemCol() = 0;

	// Returns the Result Style from inner cell coordinates
	virtual RESULTSTYLE	GetResultStyle(int Rv,int Cv) = 0;

	// Returns the Result Type from inner cell coordinates
	virtual RESULTTYPE	GetResultType(int Rv,int Cv) = 0;

	// Recup dimension hauteur row
	virtual int GetRowHdrHeight(int Row) = 0;
};
