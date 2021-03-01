#pragma once
#include "afxwin.h"
#include "SGridWnd.h"
#include "JATPDocChoixSupport.h"

class CDlgCumulativite;

class CCumGridWnd :
	public CSGridWnd
{
public:
	// Constructeur
	CCumGridWnd(void);

	// Destructeur
	~CCumGridWnd(void);

	// Init document choix support
	JBool OnInitDocument(JATPDocChoixSupport *pDoc, CDlgCumulativite *pDlg, JUnt32 &NoColIdSupport);
	
	// Gestion sélection cellule du grid click gauche (pour sélection ou tri)
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Gestion sélection cellule du grid click droit (pour sélection ou tri)
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

	// Tri sur une colonne du grille cumulativité
	JVoid SortColonne(ROWCOL nCol, JBool PermutTri = true);

	// Tri sur colonne courante du grille cumulativité
	JVoid SortColonneCourante(JBool PermutTri = true);

private :

	// la fenêtre
	CDlgCumulativite	*m_pDlgGrille;

	// Tri d'une colonne
	void TrierColonne (JUnt32 NoColonne, JBool PermutTri);

	// Sélection ou déselection couleur
	void MajColorGraphe(ROWCOL nRow, ROWCOL nCol);

	// Colonne courante de tri
	ROWCOL m_NoColTri;

};
