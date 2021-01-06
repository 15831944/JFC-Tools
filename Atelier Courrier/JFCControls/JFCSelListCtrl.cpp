#include "StdAfx.h"
#include "JFCSelListCtrl.h"

JFCSelListCtrl::JFCSelListCtrl(void)
{
	// Init index list pour gestion mouse move
	m_idxFirst = m_idxPrec = -1;
}

JFCSelListCtrl::~JFCSelListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(JFCSelListCtrl, JFCListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void JFCSelListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Récupére 1er item sélectionné
	UINT uFlags;
	int InxItem = HitTest(point, &uFlags);

	// Test si index sélectionné sur un élmt de la liste 
	if (InxItem < 0 || InxItem >= this->GetItemCount())
		return;

	// Positionne les indices début sélection et précédent
	m_idxFirst = InxItem;
	m_idxPrec  = InxItem;

	// Capture tous les événements 
	this->SetCapture();
	
	// Positionne l'état des élmts
	UINT EtatSel = this->GetItemState(m_idxFirst,LVIS_SELECTED); 
	if (EtatSel)
		this->SetItemState(m_idxFirst, 0, LVIS_SELECTED);
	else
		this->SetItemState(m_idxFirst, LVIS_SELECTED, LVIS_SELECTED);

	// Save état du premier élément à changer
	m_bFirstSel = (EtatSel != FALSE);
}	

// Selectionne ou deselectionne un item sauf s'il est sur-selectionné
void JFCSelListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);

	// Position de la liste
	CRect RectLst;
	this->GetWindowRect(&RectLst);

	// Test si un 1er élmt sélectionné
	if (m_idxFirst >= 0)
	{
		// Récupère l'item positionné par le pointeur souris
		UINT uFlags;
		int InxItem = HitTest(point, &uFlags);

		ClientToScreen(&point);

		// Si on est sur un élmt de la liste
		if (InxItem >= 0 && InxItem < this->GetItemCount())
		{
			// Change état ssi nouvel élmt
			if (InxItem != m_idxPrec)
			{
				// On positionne état des élmts suivants comme le 1er élmt
				if (m_bFirstSel)
					this->SetItemState(InxItem, 0, LVIS_SELECTED);
				else
					this->SetItemState(InxItem, LVIS_SELECTED, LVIS_SELECTED);
			}

			// Repositionne élmt précédent
			m_idxPrec = InxItem;
		}
		else
		{
			if (point.y > RectLst.bottom)
			{
				// On remonte dans la liste en sélectionnant les elmts (scroll vers le haut)
				if (m_idxPrec < this->GetItemCount())
				{
					this->EnsureVisible(++m_idxPrec, 0);

					// On positionne état des élmts suivants comme le 1er élmt
					if (m_bFirstSel)
						this->SetItemState(m_idxPrec, 0, LVIS_SELECTED);
					else
						this->SetItemState(m_idxPrec, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			else if (point.y < RectLst.top)
			{
				// On descend dans la liste en sélectionnant les elmts (scroll vers le bas)
				if (m_idxPrec > 0)
				{
					this->EnsureVisible(--m_idxPrec, 0);

					// On positionne état des élmts suivants comme le 1er élmt
					if (m_bFirstSel)
						this->SetItemState(m_idxPrec, 0, LVIS_SELECTED);
					else
						this->SetItemState(m_idxPrec, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}	

		/*
		else
			// On est en dehors de la liste, réinitialise le 1er élmt sélectionné
			m_idxFirst = -1;
		*/
	}
}	


void JFCSelListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Pas de 1er élmt sélectionné
	if (m_idxFirst < 0) return;

	// Fin capture événement
	ReleaseCapture();

	// Init 1er sélectionné
	m_idxFirst = -1;

	CListCtrl::OnLButtonUp(nFlags, point);
}
