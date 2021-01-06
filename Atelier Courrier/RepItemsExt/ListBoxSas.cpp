#include "StdAfx.h"
#include ".\ListBoxSas.h"

BEGIN_MESSAGE_MAP(CListBoxSas,JFC_MultiListBox<IQuestion const *>)
	//{{AFX_MSG_MAP()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListBoxSas::CListBoxSas(void)
{
}

void CListBoxSas::OnLButtonDown(UINT nFlags, CPoint point)
{
	JFC_MultiListBox<IQuestion const *>::OnLButtonDown(nFlags, point);
}

void CListBoxSas::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnRButtonDown(nFlags, point);
}

CListBoxSas::~CListBoxSas(void)
{
}

void CListBoxSas::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// If there are no list box items, skip this message. 
	if (lpDrawItemStruct->itemID == -1) 
		return; 

	CDC* pDC		= CDC::FromHandle(lpDrawItemStruct->hDC);
	// T lpItem		= (T) lpDrawItemStruct->itemData;
	IQuestion * lpItem = (IQuestion *) lpDrawItemStruct->itemData;
	CString sLabel	= lpItem->GetLabel().AsJCharPtr();
	JInt32 ItemID	= lpItem->GetID();

	// Save these values to restore them when done drawing.
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor	= pDC->GetBkColor();

	// item selected
	COLORREF colorref;
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		// Recup couleur sélection
		colorref = m_ColorNORMALSel;

		// Passe brosse en mode couleur sélection
		CBrush Brush(colorref);

		// Ecriture texte
		CRect Rect = lpDrawItemStruct->rcItem;
		Rect.left	+=	m_LeftMargin; 
		Rect.right	-=	m_RightMargin;
		pDC->DrawText( sLabel, &Rect, DT_WORDBREAK | DT_NOPREFIX );
	
		// Cadre focus
		Rect.top	+= 1;
		Rect.left	-= 5;
		Rect.right	+= 5; 
		pDC->FrameRect(&Rect, &Brush);
		colorref = crOldBkColor;
	}
	else// item deselected
	{
		// Récup ancienne couleur
		colorref = crOldBkColor;

		// Attribue la couleur du texte
		if ( IsAttribue(ItemID) )
			pDC->SetTextColor( m_ColorAttribue );
		
		// Ecriture du texte
		RECT Rect	=	lpDrawItemStruct->rcItem;
		Rect.left	+=	m_LeftMargin; 
		Rect.right	-=	m_RightMargin;
		pDC->DrawText( sLabel, &Rect, DT_WORDBREAK | DT_NOPREFIX );
	}

	// Reset the background color and the text color back to their original values.
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
};

