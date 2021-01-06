#pragma once
#include "afxwin.h"
#include <grid\gxwnd.h>
#include "JATPDocChoixSupport.h"

class CDlgCumulativite;

class CGridColorCum : public CGXGridWnd
{
public:

	// Constructeur
	CGridColorCum(void);

	// Destructeur
	~CGridColorCum(void);

	// Init document choix support
	JBool OnInitDocument(JATPDocChoixSupport *pDoc, CDlgCumulativite *pDlg);

	// Gestion sélection cellule du grid
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

private :
	// le document
	JATPDocChoixSupport *pDocChoixSupport;	

	// la fenêtre
	CDlgCumulativite	*pDlgParent;
};
