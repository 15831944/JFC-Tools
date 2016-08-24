// GridVisuFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "GridVisuFilm.h"
#include "DlgProduitFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridVisuFilm

CGridVisuFilm::CGridVisuFilm()
{
	// Ligne courante au d�part = 1ere ligne
	m_RowCur = 1;

	// Init campagne films associ�e
	m_pCampagneFilm = NULL;
}

CGridVisuFilm::~CGridVisuFilm()
{
}

void CGridVisuFilm::Init(CCampagneFilm * CampagneFilm)
{
	// Associe la campagne films
	m_pCampagneFilm = CampagneFilm;
}

void CGridVisuFilm::OnModifyCell (ROWCOL nRow, ROWCOL nCol)
{
	//AfxMessageBox ("Modification cellule");
}	

BEGIN_MESSAGE_MAP(CGridVisuFilm, CGXGridWnd)
	//{{AFX_MSG_MAP(CGridVisuFilm)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONUP()
	ON_WM_ASKCBFORMATNAME()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridVisuFilm message handlers

void CGridVisuFilm::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	/* Marche pas avec la gestion du simple click !!!!quelle merde !!!!!
	// R�cup�re la ligne valider
	ROWCOL nRow = CalcClientRowFromPt(point);

	// Va au information de la ligne concern�e
	int NbLigne = GetRowCount();
	if (nRow < NbLigne)
	{
		// Appel Boite de dialogue Ajout/Modif Produits 
		CDlgProduitFilm DlgProduitFilm;
		DlgProduitFilm.m_ModePlusieurFilm = false;
		DlgProduitFilm.m_pCampagneFilm = m_pCampagneFilm;

		
		// Sinon on se positionne sur le film courant
		ROWCOL nRow, nCol;
		if (GetCurrentCell(nRow, nCol))
		{
			// Il faut que ce soit un film, pas la derni�re ligne totale
			if (nRow < GetRowCount()) 
			{
				DlgProduitFilm.m_NumFilm = nRow;

				// Appel dialogue pour modifier les films
				if (DlgProduitFilm.DoModal() != IDOK) 
					AfxMessageBox ("Probl�me modification films !!!");
				else
				{
					// Mise � jour de la vue grille films
					DlgProduitFilm.m_pCampagneFilm->m_UpdateFilm = true;
					m_pCampagneFilm->m_pDoc->UpdateAllViews(NULL,1);
				}
			}
		}
	}
	*/
}

void CGridVisuFilm::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CGXGridWnd::OnActivate(nState, pWndOther, bMinimized);
}


void CGridVisuFilm::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CGXGridWnd::OnActivateApp(bActive, hTask);
}

void CGridVisuFilm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// R�cup�re la ligne valider
	ROWCOL nRow = CalcClientRowFromPt(point);
	ROWCOL nCol = CalcClientColFromPt(point);

	// Mise � jour �tat de la s�lection produit
	int NbLigne = GetRowCount();
	if (nRow > 0 && nRow < NbLigne && nCol == 0)
	{
		// Change la s�lection produit
		CProduitFilm ProduitFilm;
		ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(nRow-1); 
		ProduitFilm.m_Select = !(ProduitFilm.m_Select);
		m_pCampagneFilm->m_TabProduitFilm.SetAt(nRow-1,ProduitFilm);

		// Mise � jour de la vue campagne films 
		m_pCampagneFilm->m_UpdateFilm = true;
		m_pCampagneFilm->m_UpdateSelectFilm = true;
		m_pCampagneFilm->m_pDoc->UpdateAllViews(NULL,1);

		// Campagne film modifi�e
		m_pCampagneFilm->m_pDoc->SetModifiedFlag();

	}

	else if (nRow > 0 && nRow < NbLigne && nCol == 1)
	{
		// Appel Boite de dialogue Ajout/Modif Produits 
		CDlgProduitFilm DlgProduitFilm;
		DlgProduitFilm.m_ModePlusieurFilm = false;
		DlgProduitFilm.m_pCampagneFilm = m_pCampagneFilm;

		
		// Sinon on se positionne sur le film courant
		// Il faut que ce soit un film, pas la derni�re ligne totale
		if (nRow < GetRowCount()) 
		{
			DlgProduitFilm.m_NumFilm = nRow;

			// Appel dialogue pour modifier les films
			if (DlgProduitFilm.DoModal() == IDOK) 
			{
				// Mise � jour de la vue grille films
				DlgProduitFilm.m_pCampagneFilm->m_UpdateFilm = true;
				m_pCampagneFilm->m_pDoc->UpdateAllViews(NULL,1);

				// Rafraichit la grille r�sultat
				this->RedrawWindow(); 

				// Campagne film modifi�e
				m_pCampagneFilm->m_pDoc->SetModifiedFlag();
			}
		}
	}
}

void CGridVisuFilm::OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGXGridWnd::OnAskCbFormatName(nMaxCount, lpszString);
}
