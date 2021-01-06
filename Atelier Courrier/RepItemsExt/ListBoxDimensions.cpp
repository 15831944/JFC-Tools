#include "StdAfx.h"
#include ".\ListBoxDimensions.h"

BEGIN_MESSAGE_MAP(CListBoxDimensions,JFC_MultiListBox<IModalite const *>)
	//{{AFX_MSG_MAP()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListBoxDimensions::CListBoxDimensions(void)
{
	m_fRightFocus = FALSE;
}

VOID CListBoxDimensions::SetRightFocus(BOOL fRightFocus)
{
	m_fRightFocus = fRightFocus;
}

void CListBoxDimensions::OnLButtonDown(UINT nFlags, CPoint point)
{
	JFC_MultiListBox<IModalite const *>::OnLButtonDown(nFlags, point);
}

void CListBoxDimensions::OnRButtonDown(UINT nFlags, CPoint point)
{
	BOOL Ok = FALSE;
	BOOL Outside = TRUE;
	INT IdxItem  = this->ItemFromPoint(point, Outside);

	if (m_fRightFocus != FALSE)
	{
		if (IdxItem >= 0 && IdxItem < this->GetCount() && Outside == FALSE)
		{
			CRect Rect;
			if (LB_ERR != this->GetItemRect(IdxItem, &Rect))
			{
				if (Rect.PtInRect(point))
				{
					if (LB_ERR != this->SetCaretIndex(IdxItem, TRUE))
					{
						Ok = TRUE;
						CDC* pDC = this->GetWindowDC();
						if (pDC != 0)
						{
							CFont* pOldFont = pDC->SelectObject(this->GetFont());
							IModalite const* lpModalite = (IModalite const *)this->GetItemData(IdxItem);
							CSize Size = pDC->GetTextExtent(lpModalite->GetLabel().AsJCharPtr());
							Rect.left  = Rect.left + m_LeftMargin - 2;
							Rect.right = Rect.left + Size.cx + 4;
							pDC->DrawFocusRect(&Rect);
							pDC->SelectObject(pOldFont);
							this->ReleaseDC(pDC);
						}
					}
				}
			}
		}
		if (Ok == FALSE)
			this->SetCaretIndex(LB_ERR, FALSE);
	}

	// A VOIR (modif convention)
	// CListBox::OnRButtonDown(nFlags, point);

	// Voir si élmt clické déjà sélectionné
	int Sel;
	Sel = this->GetSel(IdxItem);

	if (Sel == 0) 
	{
		// Si non sélectionné, on lance le process comme si on avait clické sur click gauche 
		// + affichage menu popup
		OnLButtonDown(nFlags, point);
		OnLButtonUp(nFlags,point);
	}

}

CListBoxDimensions::~CListBoxDimensions(void)
{
}
