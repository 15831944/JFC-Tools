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
	// R�cup�re 1er item s�lectionn�
	UINT uFlags;
	int InxItem = HitTest(point, &uFlags);

	// Test si index s�lectionn� sur un �lmt de la liste 
	if (InxItem < 0 || InxItem >= this->GetItemCount())
		return;

	// Positionne les indices d�but s�lection et pr�c�dent
	m_idxFirst = InxItem;
	m_idxPrec  = InxItem;

	// Capture tous les �v�nements 
	this->SetCapture();
	
	// Positionne l'�tat des �lmts
	UINT EtatSel = this->GetItemState(m_idxFirst,LVIS_SELECTED); 
	if (EtatSel)
		this->SetItemState(m_idxFirst, 0, LVIS_SELECTED);
	else
		this->SetItemState(m_idxFirst, LVIS_SELECTED, LVIS_SELECTED);

	// Save �tat du premier �l�ment � changer
	m_bFirstSel = (EtatSel != FALSE);
}	

// Selectionne ou deselectionne un item sauf s'il est sur-selectionn�
void JFCSelListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);

	// Position de la liste
	CRect RectLst;
	this->GetWindowRect(&RectLst);

	// Test si un 1er �lmt s�lectionn�
	if (m_idxFirst >= 0)
	{
		// R�cup�re l'item positionn� par le pointeur souris
		UINT uFlags;
		int InxItem = HitTest(point, &uFlags);

		ClientToScreen(&point);

		// Si on est sur un �lmt de la liste
		if (InxItem >= 0 && InxItem < this->GetItemCount())
		{
			// Change �tat ssi nouvel �lmt
			if (InxItem != m_idxPrec)
			{
				// On positionne �tat des �lmts suivants comme le 1er �lmt
				if (m_bFirstSel)
					this->SetItemState(InxItem, 0, LVIS_SELECTED);
				else
					this->SetItemState(InxItem, LVIS_SELECTED, LVIS_SELECTED);
			}

			// Repositionne �lmt pr�c�dent
			m_idxPrec = InxItem;
		}
		else
		{
			if (point.y > RectLst.bottom)
			{
				// On remonte dans la liste en s�lectionnant les elmts (scroll vers le haut)
				if (m_idxPrec < this->GetItemCount())
				{
					this->EnsureVisible(++m_idxPrec, 0);

					// On positionne �tat des �lmts suivants comme le 1er �lmt
					if (m_bFirstSel)
						this->SetItemState(m_idxPrec, 0, LVIS_SELECTED);
					else
						this->SetItemState(m_idxPrec, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			else if (point.y < RectLst.top)
			{
				// On descend dans la liste en s�lectionnant les elmts (scroll vers le bas)
				if (m_idxPrec > 0)
				{
					this->EnsureVisible(--m_idxPrec, 0);

					// On positionne �tat des �lmts suivants comme le 1er �lmt
					if (m_bFirstSel)
						this->SetItemState(m_idxPrec, 0, LVIS_SELECTED);
					else
						this->SetItemState(m_idxPrec, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}	

		/*
		else
			// On est en dehors de la liste, r�initialise le 1er �lmt s�lectionn�
			m_idxFirst = -1;
		*/
	}
}	


void JFCSelListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Pas de 1er �lmt s�lectionn�
	if (m_idxFirst < 0) return;

	// Fin capture �v�nement
	ReleaseCapture();

	// Init 1er s�lectionn�
	m_idxFirst = -1;

	CListCtrl::OnLButtonUp(nFlags, point);
}
