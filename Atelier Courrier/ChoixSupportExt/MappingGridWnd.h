#pragma once
#include "afxwin.h"
#include "SGridWnd.h"
#include "JATPDocChoixSupport.h"

class CDlgMapping;

class CMappingGridWnd :
	public CSGridWnd
{
public:
	// Constructeur
	CMappingGridWnd(void);

	// Destructeur
	virtual ~CMappingGridWnd(void);

	// Init document choix support
	JBool OnInitDocument(JATPDocChoixSupport *pDoc, CDlgMapping *pDlg, JUnt32 &NoColIdSupport);

	// Gestion sélection cellule du grid
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Bidouille blocage 1ere colonne
	virtual BOOL OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Modification sélection format support
	virtual void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

	// A REMETTRE
	// virtual void SortKeys( const CGXSortInfoArray& sortInfo, CPtrArray& sortKeyArray);

	// Fonctions Clipboard
	virtual BOOL Copy();
	virtual BOOL Paste( ) { return false; } ;

private :

	// la fenêtre
	CDlgMapping			*m_pDlgGrille;
	
	// Tri d'une colonne
	void TrierColonne(JUnt32 NoColonne);

};
