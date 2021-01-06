#include "StdAfx.h"
#include "GridColorCum.h"
#include "DlgCumulativite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CGridColorCum::CGridColorCum(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CGridColorCum::~CGridColorCum(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Init document choix support associ�
JBool CGridColorCum::OnInitDocument(JATPDocChoixSupport *pDoc, CDlgCumulativite *pDlg)
{
	// R�cup�re document choix support
	pDocChoixSupport = pDoc;

	// R�cup�re la fen�tre m�re
	pDlgParent		 = pDlg;

	// Test si valide
	if (pDocChoixSupport->IsValid() && pDlgParent != NULL) 
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Gestion s�lection cellule du grid
BOOL CGridColorCum::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	// R�cup�re indice couleur courbe a utiliser
	if (pDlgParent != NULL)
	{
		pDocChoixSupport->m_IdColorCum = pDlgParent->GetIdColorCourbe(nRow,nCol) + 1;
		pDocChoixSupport->UpdateAllViews(UPDATE_SEL_COLOR);
	}
	return TRUE;
}
