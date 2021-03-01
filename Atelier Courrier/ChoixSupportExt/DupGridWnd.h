#pragma once

#include "afxwin.h"
#include "SGridWnd.h"
#include "JATPDocChoixSupport.h"

class CDlgDuplication;

class CDupGridWnd : public CSGridWnd
{
public:
	// Constructeur
	CDupGridWnd(void);

	// Destructeur
	~CDupGridWnd(void);

	// Init document choix support
	JBool OnInitDocument(JATPDocChoixSupport *pDoc, CDlgDuplication *pDlg, JUnt32 &NoColIdSupport);
	
	// Gestion sélection cellule du grid
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Gestion sélection cellule du grid click droit (pour sélection ou tri)
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Modification sélection format support
	virtual void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

	// A REMETTRE
	// virtual void SortKeys( const CGXSortInfoArray& sortInfo, CPtrArray& sortKeyArray);

	// Tri sur une colonne du grille duplication
	JVoid SortColonne(ROWCOL nCol, JBool PermutTri = true);

	// Tri sur colonne courante du grille cumulativité
	JVoid SortColonneCourante(JBool PermutTri = true);

	// Fonctions Clipboard
	virtual BOOL Copy();
	virtual BOOL Paste( ) { return false; } ;

private :

	// la fenêtre
	CDlgDuplication		*m_pDlgGrille;

	// Tri d'une colonne (par défaut on change l'ordre de tri courant)
	void TrierColonne(JUnt32 NoColonne, JBool PermutTri);

	// Colonne courante de tri
	ROWCOL m_NoColTri;

};
