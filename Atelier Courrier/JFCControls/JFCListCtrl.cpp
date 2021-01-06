// JFCListCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Resource.h"
#include "JFCListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// JFCListCtrl

IMPLEMENT_DYNAMIC(JFCListCtrl, CListCtrl)
JFCListCtrl::JFCListCtrl()
{
	m_ColorBk	= ::GetSysColor(COLOR_WINDOW);
	m_ColorSel	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_ColorText = ::GetSysColor(COLOR_WINDOWTEXT); 
	m_ColorSelText = ::GetSysColor(COLOR_HIGHLIGHTTEXT); 
	m_ColorAttribue = ::GetSysColor(COLOR_HIGHLIGHTTEXT); 
}

JFCListCtrl::~JFCListCtrl()
{
}

BEGIN_MESSAGE_MAP(JFCListCtrl, CListCtrl)
	ON_WM_DRAWITEM_REFLECT()
END_MESSAGE_MAP()

// Gestionnaires de messages IItalicListCtrl

void JFCListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	// If there are no list box items, skip this message. 

	if (lpDrawItemStruct->itemID == -1) 
		return; 
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	CString sLabel;
	//GetText( lpDrawItemStruct->itemID, sLabel);
	sLabel = GetItemText(lpDrawItemStruct->itemID,0);


	// Save these values to restore them when done drawing.
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor = pDC->GetBkColor();

	// item selected
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		pDC->SetTextColor(m_ColorSelText);
		pDC->SetBkColor(m_ColorSel);
	}
	else// item deselected
	{
		pDC->SetTextColor(m_ColorText);
		pDC->SetBkColor(m_ColorBk);
		if ( IsAttribue((int)GetItemData(lpDrawItemStruct->itemID)) )
			pDC->SetTextColor(m_ColorAttribue);
	}

	// Remplissage zone
	// lpDrawItemStruct->rcItem.top	= lpDrawItemStruct->rcItem.top - (lpDrawItemStruct->itemID * 6);
	// lpDrawItemStruct->rcItem.bottom = lpDrawItemStruct->rcItem.bottom - ((lpDrawItemStruct->itemID + 1) * 6);
	pDC->FillSolidRect(&lpDrawItemStruct->rcItem, pDC->GetBkColor());

	// Ecriture texte
	RECT Rect = lpDrawItemStruct->rcItem;
	Rect.left	+=5; 
	Rect.right	-=5;
	pDC->DrawText( sLabel, -1, &Rect, DT_WORDBREAK | DT_NOPREFIX );

	if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		// Draw the focus rect 
		pDC->DrawFocusRect(&(lpDrawItemStruct->rcItem)); 
	}

	// Reset the background color and the text color back to their
	// original values.
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
}
