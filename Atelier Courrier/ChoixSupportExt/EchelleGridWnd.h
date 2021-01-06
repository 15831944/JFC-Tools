#pragma once
#include "afxwin.h"
#include "SGridWnd.h"
#include "JATPDocChoixSupport.h"

class CDlgEchelle;

// Classe Grille résultat Echelle
class CEchelleGridWnd :	public CSGridWnd
	// public CGXGridWnd
{
public:
	// Constructeur
	CEchelleGridWnd();

	// Destructeur
	virtual ~CEchelleGridWnd();

	// Init document choix support
	JBool OnInitDocument(JATPDocChoixSupport *pDoc, CDlgEchelle *pDlg, JUnt32 &NoColIdSupport);

	// Gestion sélection cellule du grid
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Bidouille blocage 1ere colonne
	virtual BOOL OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	// Modification sélection format support
	virtual void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

	// Tri sur une colonne du grille échelle
	JVoid SortColonne(ROWCOL nCol, JBool PermutTri = true);

	// A REMETTRE
	// virtual void SortKeys( const CGXSortInfoArray& sortInfo, CPtrArray& sortKeyArray);

	// Fonctions Clipboard
	virtual BOOL Copy(JBool ParCible);
	virtual BOOL Paste( ) { return false; } ;

private :

	// la fenêtre
	CDlgEchelle			*m_pDlgGrille;

	// Tri d'une colonne (par défaut on change l'ordre de tri courant)
	void TrierColonne(JUnt32 NoColonne, JBool PermutTri);

};


